#pragma once

#include "./Client.hpp"

class Server;
typedef struct s_node
{
	bool IsServer;
	int fd;
	Server *server;
	Client *client;
}			t_node;
