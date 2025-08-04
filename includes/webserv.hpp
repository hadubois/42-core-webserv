/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-duc <lle-duc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 11:07:16 by lle-duc           #+#    #+#             */
/*   Updated: 2025/07/28 16:22:31 by lle-duc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <csignal>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/epoll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#define MAX_EVENTS 1024

class					Monitor;

typedef struct s_sockets_datas
{
	int					epoll_fd;
	int					max_request;
	struct epoll_event	epoll_datas;
	struct epoll_event	*request;
}						t_sockets_datas;

#define MAX_CLIENTS 1024

void					respond_to_client(t_sockets_datas &datas,
							Monitor &node_datas);
int						get_file_size(std::string filename);
bool					check_empty_line(std::string line);
void					print_errors(std::string str);
void					print_logs(std::string str);
std::string build_directory_indexing_page(std::string directory_path);
void					Start_all_servers(std::string config_file);

typedef struct s_server_config
{
	int					Port;
	int					Max_client_body_size;
	std::string default_page;
	std::string error_page;
	std::string redirection_page;
}						t_server_config;

extern bool				sig_stop;
