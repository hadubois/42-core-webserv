/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-duc <lle-duc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:51:51 by lle-duc           #+#    #+#             */
/*   Updated: 2025/08/02 12:49:22 by lle-duc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Server.hpp"
#include "Monitor.hpp"
#include "log_macros.hpp"

#define MY_SOCK_PATH "./sockets/mysocket"
#define LISTEN_BACKLOG 100

bool sig_stop = false;

void handle_error(std::string str)
{
	std::cerr << str << std::endl;
}

void signal_handler(int signal)
{
    if (signal == SIGINT)
    {
        sig_stop = true;
        INFO("\nSIGINT RECEIVED STOPPING ALL SERVERS ...\n" );
    }
}

int main(int ac, char **av)
{

    std::signal(SIGINT, signal_handler);
    if (ac > 2)
    {
        INFO("[ERROR] : Usage ./webserv [OPTIONAL CONFIGURATION FILE]" );
        return (1);
    }
    if (ac == 1)
    {
        INFO("Starting all servers from default configuration..." );
        Start_all_servers("./config/one_server");
    }
    else
    {
        INFO("Starting the server with configuration file " << av[1] );
        Start_all_servers(av[1]);
    }
}
