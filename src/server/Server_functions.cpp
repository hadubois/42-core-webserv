/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_functions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadubois <hadubois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 11:55:56 by lle-duc           #+#    #+#             */
/*   Updated: 2025/08/04 10:43:45 by hadubois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "log_macros.hpp"

void Server::Fill_data(std::string *data, std::string raw_data, std::size_t pos)
{
	int	equal_count;

	equal_count = 0;
	while (raw_data[pos] == ' ' || raw_data[pos] == '=')
	{
		if (raw_data[pos] == '=')
			equal_count++;
		pos++;
	}
	if (equal_count != 1)
	{
		INFO("str-> " << raw_data << " []= count] " << equal_count);
		throw std::runtime_error("Invalid Configuration !\n");
	}
	*data = raw_data.substr(pos);
}

void Server::Fill_data(size_t *data, std::string raw_data, std::size_t pos)
{
	int	equal_count;

	equal_count = 0;
	while (raw_data[pos] == ' ' || raw_data[pos] == '=')
	{
		if (raw_data[pos] == '=')
			equal_count++;
		pos++;
	}
	if (equal_count != 1)
	{
		INFO("str-> " << raw_data << " []= count] " << equal_count);
		throw std::runtime_error("Invalid Configuration !\n");
	}
	*data = std::atoi(raw_data.substr(pos).c_str());
}

void Server::Fill_data(int *data, std::string raw_data, std::size_t pos)
{
	int	equal_count;

	equal_count = 0;
	while (raw_data[pos] == ' ' || raw_data[pos] == '=')
	{
		if (raw_data[pos] == '=')
			equal_count++;
		pos++;
	}
	if (equal_count != 1)
	{
		INFO("str-> " << raw_data << " []= count] " << equal_count);
		throw std::runtime_error("Invalid Configuration !\n");
	}
	*data = std::atoi(raw_data.substr(pos).c_str());
}

size_t Server::Skip_white_spaces(std::string raw_data, std::size_t pos)
{
	int	equal_count;

	equal_count = 0;
	while (raw_data[pos] == ' ' || raw_data[pos] == '=')
	{
		if (raw_data[pos] == '=')
			equal_count++;
		pos++;
	}
	return (pos);
}

void Server::Manage_Location(std::ifstream &file, std::string line)
{
	size_t			index;
	LocationConfig	NewConfig;
	std::string tmp;
    std::istringstream iss_tmp(line.substr(line.find("location") + 8));
    std::string token;

    iss_tmp >> token;
    if (token.at(token.length() - 1) != '/')
		token += '/';
    NewConfig.setPath(token);
    token.clear();
	
	while (file.good())
	{
		std::getline(file, line);
		if ((line.find("}", 0)) != std::string::npos)
			break ;
		else if ((index = line.find("cgi_enabled")) != std::string::npos)
			NewConfig.setCgiEnabled(line.substr(Skip_white_spaces(line, index + 12), 4) == "true");
		else if ((index = line.find("cgi_folder")) != std::string::npos)
		{
			tmp = "";
			tmp += line.substr(Skip_white_spaces(line, index + 10),
					line.length());
			if (tmp.substr(tmp.length() - 1) != "/")
				tmp += "/";
			NewConfig.setRoot(tmp);
		}
		else if ((index = line.find("cgi_allowed_ext")) != std::string::npos)
		{
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string after_equal = line.substr(pos + 1);
                std::istringstream iss(after_equal);
                while (iss >> token) {
					if (token == ".py")
                    	NewConfig.insertCgiPath(token, "/bin/python3");
					if (token == ".rb")
                    	NewConfig.insertCgiPath(token, "/bin/ruby");
                }
            }
		}
		else if ((index = line.find("auto_index")) != std::string::npos)
		{
			index = Skip_white_spaces(line, index + 11);
			NewConfig.setAutoIndex(line.substr(index, 4) == "true");
		}
		else if ((index = line.find("upload_enabled")) != std::string::npos)
		{
			index = Skip_white_spaces(line, index + 15);
			NewConfig.setUploadEnabled(line.substr(index, 4) == "true");
		}
		else if ((index = line.find("upload_folder")) != std::string::npos)
		{
			index = Skip_white_spaces(line, index + 14);
			NewConfig.setUploadPath(line.substr(index));
		}
	}
	this->Config.insertLocationConfig(NewConfig);
}

void Server::Manage_Methods(std::ifstream &file, std::string line)
{
	while (file.good())
	{
		std::getline(file, line);
		if ((line.find("}", 0)) != std::string::npos)
			break ;
		else
		{   
			std::istringstream iss(line);
			while (iss >> line) {
				if (line == "POST")
					Config.insertMethod("POST");
				if (line == "GET")
					Config.insertMethod("GET");
				if (line == "DELETE")
					Config.insertMethod("DELETE");
			}
		}
	}
}

void Server::LoadConfig(std::ifstream &file, std::string line)
{
	std::size_t pos;
	std::map<std::string, void(Server::*)(const std::string& line, std::size_t pos)>::iterator it;

	if (check_empty_line(line))
		return ;
	for (it = func_ptr.begin(); it != func_ptr.end(); it++)
	{
		if ((pos = line.find(it->first)) != std::string::npos)
			return (this->*(it->second))(line, pos);
	}
	if ((pos = line.find("location")) != std::string::npos)
		Manage_Location(file, line);
	else if ((pos = line.find("allowed_methods")) != std::string::npos)
		Manage_Methods(file, line);
	else
	{
		INFO("[" << line << "]");
		throw std::runtime_error("Invalid Stuff inside configuration file!\n");
	}
}

void Server::close_client(std::map<int, t_node *> &node_map, t_node *node, t_sockets_datas &datas)
{
	INFO("Closing client\n");
    int fd = node->fd;
	std::string response = "HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n";
    send(fd, response.c_str(), response.size(), MSG_NOSIGNAL);
    epoll_ctl(datas.epoll_fd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
    node_map.erase(fd);
    delete node;
}

Server *Server::clone(void) const
{
	Server *Ret = new Server;

	Ret->server_name = server_name;
	Ret->Port = Port;
	Ret->Server_socket = Server_socket;
	Ret->client_header_buffer_size = client_header_buffer_size;
	Ret->Max_request = Max_request;
	Ret->Request_timeout = Request_timeout;
	Ret->env = env;
	Ret->default_page = default_page;
	Ret->error_page = error_page;
	Ret->redirection_page = redirection_page;
	Ret->current_request = current_request;
	Ret->Max_client_body_size = Max_client_body_size;

	return (Ret);
}

void Server::Print_Config(void)
{
    INFO("Default page -> " << this->default_page );
    INFO("404 page -> " << this->error_page );
    INFO("redirection page -> " << this->redirection_page );
    INFO("Port -> " << this->Port );
    INFO("Max client body size -> " << this->Max_client_body_size );
    INFO("Max Request -> " << this->Max_request );
    INFO("Request timeout -> " << this->Request_timeout );
    INFO("server_name -> " << this->server_name );
    INFO("client_header_buffer_size -> " << this->client_header_buffer_size );
    INFO("client_header_buffer_size -> " << this->client_header_buffer_size );
    INFO("GET ENABLED -> " << GET_Enabled );
    INFO("POST ENABLED -> " << POST_Enabled );
    INFO("DELETE ENABLED -> " << DELETE_Enabled );
}

bool Server::Get_POST_Enabled(void)
{
	return POST_Enabled;
}

bool Server::Get_GET_Enabled(void)
{
	return GET_Enabled;
}

bool Server::Get_DELETE_Enabled(void)
{
	return DELETE_Enabled;
}

int Server::Get_port(void)
{
	return (this->Port);
}

int Server::Get_Server_Socket(void)
{
	return (this->Server_socket);
}

int Server::Get_MCBS(void)
{
	return (this->Max_client_body_size);
}
std::string Server::Get_default_page(void)
{
	return (this->default_page);
}

std::string Server::Get_error_page(void)
{
	return (this->error_page);
}
std::string Server::Get_redirection_page(void)
{
	return (this->redirection_page);
}
int Server::Get_client_timeout_delay(void)
{
	return (this->client_timeout);
}
