/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Monitor.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-duc <lle-duc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 11:46:23 by lle-duc           #+#    #+#             */
/*   Updated: 2025/07/23 11:46:46 by lle-duc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "node.hpp"
#include <map>
#include <ctime>

class	Server;
class Monitor
{
  private:
	std::map<int, t_node *> node_map;

  public:
	Monitor(void);
	~Monitor(void);
	void add_to_node(t_node *node);
	std::map<int, t_node *> &Get_Monitor_map(void);
	void close_inactive_clients(int epoll_fd);
};