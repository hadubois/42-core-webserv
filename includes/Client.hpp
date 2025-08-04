/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadubois <hadubois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 11:46:23 by lle-duc           #+#    #+#             */
/*   Updated: 2025/07/26 14:08:56 by hadubois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "../http/HttpExchange.hpp"
#include "log_macros.hpp"
#include <ctime>


class Client
{
  private:
	int Client_socket;
	// size_t Client_Timeout;
	std::string buffer;
	HttpExchange Exchange;
	time_t last_activity;
  public:
	Client(int socket, ServerConfig Config);
	Client(void);
	~Client(void);

	std::string &Get_Buffer();
	HttpExchange &Get_Exchange();
	const int	&Get_Socket() const;
	void Update_Last_Activity(void);
	const time_t &Get_Last_Activity(void);
	void Clear_exchange(ServerConfig &Config);
};
