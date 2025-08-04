/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadubois <hadubois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 11:49:29 by lle-duc           #+#    #+#             */
/*   Updated: 2025/08/04 14:11:56 by hadubois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "node.hpp"
#include "Monitor.hpp"
#include "log_macros.hpp"

Server::Server(void)
{
    this->Port = 8080;
	this->Max_client_body_size = 10;
	this->default_page = "./tmp/www/index.html";
	this->error_page = "./tmp/www/404.html";
	this->redirection_page = "./tmp/www/redirectme";
	this->redirection_url = "https://profile.intra.42.fr/";
    this->Max_request = 5;
    this->Request_timeout = 10;
    this->client_header_buffer_size = 500;
    this->POST_Enabled = false;
    this->GET_Enabled = false;
    this->DELETE_Enabled = false;
    
    func_ptr["default_page"] = &Server::parseDefaultPage;
    func_ptr["404_page"] = &Server::parse404Page;
    func_ptr["server_name"] = &Server::parseServerName;
    func_ptr["redirection_page"] = &Server::parseRedirectionPage;
    func_ptr["redirection_url"] = &Server::parseRedirectionURL;
    func_ptr["Port"] = &Server::parsePort;
    func_ptr["client_timeout_inactivity"] = &Server::parseClientTimeout;
    func_ptr["Max_request"] = &Server::parseMaxRequest;
    func_ptr["request_timeout"] = &Server::parseRequestTimeout;
    func_ptr["Max_client_body_size"] = &Server::parseMaxClientBodySize;
    func_ptr["client_header_buffer_size"] = &Server::parseClientHeaderBufferSize;

    DEBUG("Server has been created !");
}

void    Server::fill_config_datas(void)
{
    this->Config.setRedirectionPage(redirection_page);
    this->Config.setRedirectionUrl(redirection_url);
    this->Config.setDefaultErrorPage(error_page);
    this->Config.setMaxBodySize(Max_client_body_size);
    this->Config.setMaxHeaderSize(client_header_buffer_size);
}

void Server::Start_server(void)
{
    fill_config_datas();
    std::cout << "Starting server on port " << Get_port() << std::endl;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->Get_port());
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int yes = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    // binding socket.

    if (bind(serverSocket, (struct sockaddr*)&serverAddress,
            sizeof(serverAddress)) == -1)
    {
        DEBUG("PORT ISSUE : " << Get_port());
        throw std::runtime_error("Port already used ! Bind Failed ! Exiting..\n");
    }
    if(listen(serverSocket, 50) != 0)
        print_errors("Listen failed, can't handle connections");
    this->Server_socket = serverSocket;
}

Server::Server(std::string config_file)
{
	std::cerr << "Initializing a new server with " << config_file << std::endl;
}

void Server::send_page_to_client(Client &client, std::map<int, t_node *> &node_map, t_node *node, t_sockets_datas &datas)
{
    char buffer[1024];
    std::string response_str = client.Get_Exchange().getOutBuf();
    
    if (send(client.Get_Socket(), response_str.c_str(), response_str.size(), MSG_NOSIGNAL) == -1)
    {
        close_client(node_map, node, datas);
        return;
    }
    std::ifstream fs;
    fs.open(client.Get_Exchange().getReq().getUrl().c_str(), std::ifstream::binary);
    if (!fs.is_open())
    {
        // if (client.Get_Exchange().getRes().getStatusCode() == 302)
        //     close_client(node_map, node, datas);
        node->client->Clear_exchange(Config);
        return ;
    }
    while (true)
    {
        int gcount = 0;
        fs.read(buffer, sizeof(buffer));
        gcount = fs.gcount();
        if (gcount == 0)
            break;

        if (send(client.Get_Socket(), buffer, gcount, MSG_NOSIGNAL) == -1)
        {
            close_client(node_map, node, datas);
            return;
        }
		std::memset(buffer, '\0', 1024);
    }
    node->client->Clear_exchange(Config);
    if (client.Get_Exchange().getReq().getState() == RS_CLOSE || client.Get_Exchange().getRes().getStatusCode() != 200)
        close_client(node_map, node, datas);
    else
    {
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.ptr = node;
        if (epoll_ctl(datas.epoll_fd, EPOLL_CTL_MOD, node->fd, &ev) == -1)
        {
            // perror("epoll_ctl: remove EPOLLOUT"); connection interrompu entre temps, rien de derangeant
        }   
    }
}

void Server::new_client_setup(t_sockets_datas &datas, Monitor &ServerMonitor)
{
    int clientSocket = accept(this->Server_socket, NULL, NULL);
    if (clientSocket == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
    {
        print_errors("set non bloicking flags failed !");
        exit(EXIT_FAILURE);
    }

    t_node *ClientNode = new t_node;
    ClientNode->fd = clientSocket;
    ClientNode->server = this;
    ClientNode->client = new Client(clientSocket, Config);
    ClientNode->IsServer = false;

    ServerMonitor.add_to_node(ClientNode);
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = ClientNode;
    if (epoll_ctl(datas.epoll_fd, EPOLL_CTL_ADD, clientSocket,
                &ev) == -1) {
        perror("epoll_ctl: clientSocket");
        exit(EXIT_FAILURE);
    }
}
int Server::read_client_request(Client &client, std::map<int, t_node *> &node_map, t_node *node, t_sockets_datas &datas)
{
    char buffer[1024];
    ssize_t bytes;

    while (true)
    {
        bytes = recv(client.Get_Socket(), buffer, sizeof(buffer), 0);
        if (bytes > 0)
        {
            std::string data(buffer, bytes);
            client.Get_Exchange().process(data.c_str());

            if (client.Get_Exchange().getReq().getState() == RS_CLOSE)
            {
                send(client.Get_Socket(), client.Get_Exchange().getOutBuf().c_str(), client.Get_Exchange().getOutBuf().size(), MSG_NOSIGNAL);
                close_client(node_map, node, datas);
                return -1;
            }
            if (client.Get_Exchange().getReq().getState() == RS_READY)
                break;
        }
        else
        {
            send(client.Get_Socket(), client.Get_Exchange().getOutBuf().c_str(), client.Get_Exchange().getOutBuf().size(), MSG_NOSIGNAL);
            close_client(node_map, node, datas);
            return -1;
        }
    }
    return (0);
}

Server::~Server(void)
{
    close(this->Server_socket);
    DEBUG("FERMETURE DU SERVEUR...");
}
