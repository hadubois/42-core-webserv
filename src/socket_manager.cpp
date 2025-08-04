#include "webserv.hpp"
#include "Server.hpp"
#include "Monitor.hpp"
#include "node.hpp"

void respond_to_client(t_sockets_datas &datas, Monitor &node_datas)
{
    int nbr_of_connections;
    while (1)
    {
        if (sig_stop)
            return ;
        nbr_of_connections = epoll_wait(datas.epoll_fd, datas.request, MAX_EVENTS, -1);
        if (nbr_of_connections < 0)
            return ;
        for (int n = 0; n < nbr_of_connections; ++n)
        {
            t_node *node = static_cast<t_node*>(datas.request[n].data.ptr);
            int ret = 0;
            if (datas.request[n].events & EPOLLIN)
            {
                if (node->IsServer)
                    node->server->new_client_setup(datas, node_datas);
                else
                {
                    struct epoll_event ev;
                    ev.events = EPOLLIN | EPOLLOUT;
                    ev.data.ptr = node;
                    if (epoll_ctl(datas.epoll_fd, EPOLL_CTL_MOD, node->fd, &ev) == -1)
                    {
                        DEBUG("Client was already closed!");
                    }
                    node->client->Update_Last_Activity();
                    ret = node->server->read_client_request(*node->client, node_datas.Get_Monitor_map(), node, datas);
                }
            }
            if (datas.request[n].events & EPOLLOUT) 
            {
                if (ret != -1)
                {
                    node->client->Update_Last_Activity();
                    node->server->send_page_to_client(*node->client, node_datas.Get_Monitor_map(), node, datas);
                    struct epoll_event ev;
                    ev.events = EPOLLIN;
                    ev.data.ptr = node;
                    if (epoll_ctl(datas.epoll_fd, EPOLL_CTL_MOD, node->fd, &ev) == -1)
                    {
                        // perror("epoll_ctl: remove EPOLLOUT"); connection interrompu entre temps, rien de derangeant
                    }   
                }
            }
        }
        node_datas.close_inactive_clients(datas.epoll_fd);
    }
}
