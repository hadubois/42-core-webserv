#include "../includes/Client.hpp"
#include "log_macros.hpp"

std::string &Client::Get_Buffer()
{
    return (this->buffer);
}

HttpExchange &Client::Get_Exchange()
{
    return (this->Exchange);
}

const int	&Client::Get_Socket() const
{
    return (this->Client_socket);
}

Client::Client(int socket, ServerConfig Config) : Client_socket(socket), Exchange(Config)
{
    Update_Last_Activity();
    DEBUG("A new client with socket " << this->Client_socket << " has been created!");
}

Client::Client(void) : Client_socket(-1)
{
    DEBUG("A new client with socket " << this->Client_socket << " has been created!");
}

Client::~Client(void)
{
    DEBUG("Client with socket " << this->Client_socket << " has been closed!");
}

void Client::Update_Last_Activity(void)
{
    last_activity = time(NULL);
}

const time_t &Client::Get_Last_Activity(void)
{
    return last_activity;
}

void Client::Clear_exchange(ServerConfig &Config)
{
    this->Exchange = HttpExchange(Config);
}