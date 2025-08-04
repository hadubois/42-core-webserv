/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-duc <lle-duc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 11:46:23 by lle-duc           #+#    #+#             */
/*   Updated: 2025/08/02 12:05:23 by lle-duc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../http/HttpRequest.hpp"
#include "./Client.hpp"
#include "Client.hpp"
#include "ServerConfig.hpp"
#include "config/ServerConfig.hpp"
#include "log_macros.hpp"
#include "node.hpp"
#include "webserv.hpp"
#include <ctime>
#include <map>
#include <sstream>
#include <sys/wait.h>
#include <vector>

class Server
{
  private:
	int client_timeout;
	int Server_socket;
	int Port;
	size_t Max_client_body_size;
	int Max_request;
	int Request_timeout;
	size_t client_header_buffer_size;
	char **env;
	std::string default_page;
	std::string error_page;
	std::string redirection_page;
	std::string redirection_url;
	std::string server_name;
	std::string current_request;
	ServerConfig Config;
	bool POST_Enabled;
	bool GET_Enabled;
	bool DELETE_Enabled;
	std::map<std::string, void(Server::*)(const std::string& line, std::size_t pos)> func_ptr;
  public:
	Server(void);
	Server(std::string config_file);
	~Server(void);

	void Manage_Location(std::ifstream &file, std::string line);
	void Manage_Methods(std::ifstream &file, std::string line);
	void LoadConfig(std::ifstream &file, std::string line);
	void Fill_data(std::string *data, std::string raw_data, std::size_t pos);
	void Fill_data(size_t *data, std::string raw_data, std::size_t pos);
	void Fill_data(int *data, std::string raw_data, std::size_t pos);
	size_t Skip_white_spaces(std::string raw_data, std::size_t pos);
	int Get_port(void);
	int Get_Server_Socket(void);
	int Get_MCBS(void);
	int Get_client_timeout_delay(void);
	std::string Get_default_page(void);
	std::string Get_error_page(void);
	std::string Get_redirection_page(void);
	bool Get_POST_Enabled(void);
	bool Get_GET_Enabled(void);
	bool Get_DELETE_Enabled(void);
	void send_page_to_client(Client &client, std::map<int, t_node *> &node_map,
		t_node *node, t_sockets_datas &datas);
	void respond_to_client(void);
	void new_client_setup(t_sockets_datas &datas, Monitor &ServerMonitor);
	int read_client_request(Client &client, std::map<int, t_node *> &node_map,
		t_node *node, t_sockets_datas &datas);
	void setup_epoll(int serverSocket);
	void close_client(std::map<int, t_node *> &node_map, t_node *node,
		t_sockets_datas &datas);
	void Start_server(void);
	Server *clone(void) const;
	void Print_Config(void);
	void fill_config_datas(void);


	/*config parser*/
	void parseDefaultPage(const std::string& line, std::size_t pos) {
		Fill_data(&this->default_page, line, pos + 13);
	}

	void parse404Page(const std::string& line, std::size_t pos) {
		Fill_data(&this->error_page, line, pos + 9);
	}

	void parseServerName(const std::string& line, std::size_t pos) {
		Fill_data(&this->server_name, line, pos + 12);
	}

	void parseRedirectionPage(const std::string& line, std::size_t pos) {
		Fill_data(&this->redirection_page, line, pos + 17);
	}

	void parseRedirectionURL(const std::string& line, std::size_t pos) {
		Fill_data(&this->redirection_url, line, pos + 16);
	}

	void parsePort(const std::string& line, std::size_t pos) {
		Fill_data(&this->Port, line, pos + 5);
	}

	void parseClientTimeout(const std::string& line, std::size_t pos) {
		Fill_data(&this->client_timeout, line, pos + 26);
	}

	void parseMaxRequest(const std::string& line, std::size_t pos) {
		Fill_data(&this->Max_request, line, pos + 12);
	}

	void parseRequestTimeout(const std::string& line, std::size_t pos) {
		Fill_data(&this->Request_timeout, line, pos + 16);
	}

	void parseMaxClientBodySize(const std::string& line, std::size_t pos) {
		Fill_data(&this->Max_client_body_size, line, pos + 21);
	}

	void parseClientHeaderBufferSize(const std::string& line, std::size_t pos) {
		Fill_data(&this->client_header_buffer_size, line, pos + 26);
	}
};