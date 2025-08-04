/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Monitor.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-duc <lle-duc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:23:34 by lle-duc           #+#    #+#             */
/*   Updated: 2025/08/02 12:46:54 by lle-duc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Monitor.hpp"
#include "log_macros.hpp"

void Monitor::add_to_node(t_node *node)
{
	if (node_map.find(node->fd) != node_map.end())
	{
		delete node_map[node->fd];
		node_map.erase(node->fd);
	}
	node_map[node->fd] = node;
}

std::map<int, t_node *> &Monitor::Get_Monitor_map(void)
{
    return this->node_map;
}

void Monitor::close_inactive_clients(int epoll_fd)
{
    time_t curtime = time(NULL);

    std::map<int, t_node*>::iterator it = node_map.begin();
    while (it != node_map.end())
    {
        t_node *node = it->second; 
        std::map<int, t_node*>::iterator cur = it++;

        if (!node->IsServer &&
            (curtime - node->client->Get_Last_Activity()) >
            node->server->Get_client_timeout_delay())
        {
            int fd = node->fd;
            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
            close(fd);
            delete node;
            node_map.erase(cur);
            DEBUG("Client has been closed for inactivity!");
        }
    }
}


Monitor::Monitor(void)
{
    DEBUG("Monitor created!\n");
}

Monitor::~Monitor(void)
{
    DEBUG("Monitor destroyed!\n");
}
