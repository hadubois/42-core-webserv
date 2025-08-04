/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-duc <lle-duc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:24:13 by lle-duc           #+#    #+#             */
/*   Updated: 2025/08/02 12:47:12 by lle-duc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Monitor.hpp"
#include "Server.hpp"
#include "node.hpp"
#include "webserv.hpp"

bool	check_empty_line(std::string line)
{
	int	size;

	size = line.size();
	while (size)
	{
		if (!std::isspace(line[size]) && line[size] != '\n')
			return (false);
	}
	return (true);
}

static void	setup_epoll(t_sockets_datas &datas)
{
	datas.epoll_fd = epoll_create1(0);
	if (datas.epoll_fd < 0)
	{
		print_errors("EPOLL failed on creation in setup epoll!");
		return ;
	}
}

static void	add_server_to_epoll(Server *New_Server, t_sockets_datas &datas,
		Monitor &node_datas)
{
	t_node				*ServerNode;
	struct epoll_event	ev;

	ServerNode = new t_node;
	ServerNode->fd = New_Server->Get_Server_Socket();
	ServerNode->server = New_Server;
	ServerNode->client = NULL;
	ServerNode->IsServer = true;
	ev.events = EPOLLIN;
	ev.data.ptr = ServerNode;
	node_datas.add_to_node(ServerNode);
	if (epoll_ctl(datas.epoll_fd, EPOLL_CTL_ADD,
			New_Server->Get_Server_Socket(), &ev) == -1)
	{
		print_errors("EPOLL failed on creation in add server to epoll!");
		return ;
	}
}

void	Start_all_servers(std::string config_file)
{
	std::ifstream file;
	std::string line;
	t_sockets_datas datas;
	Monitor node_datas;
	std::vector<Server *> Server_List;

	try
	{
		datas.request = new epoll_event[MAX_EVENTS];
		setup_epoll(datas);
		file.open(config_file.c_str());
		if (!file.good())
			throw std::runtime_error("Invalid Configuration File!\n");
		while (file.good())
		{
			std::getline(file, line);
			if (check_empty_line(line))
				continue ;

			size_t index;
			if ((index = line.find("{", 0)) != std::string::npos)
			{
				if (index != 0)
					throw std::runtime_error("Invalid Configuration File!\n");
				Server_List.push_back(new Server());
				while (file.good())
				{
					std::getline(file, line);
					if ((index = line.find("}", 0)) == 0)
						break ;
					Server_List.back()->LoadConfig(file, line);
				}
				Server_List.back()->Start_server();
				add_server_to_epoll(Server_List.back(), datas, node_datas);
				// Server_List.back()->Print_Config();
			}
		}
		file.close();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
	respond_to_client(datas, node_datas);
	for (int i = Server_List.size(); i > 0; i--)
	{
		delete Server_List.back();
		Server_List.pop_back();
	}
	for (std::map<int,
		t_node *>::iterator i = node_datas.Get_Monitor_map().begin(); i != node_datas.Get_Monitor_map().end(); i++)
	{
		delete i->second;
	}
	delete[] datas.request;
}