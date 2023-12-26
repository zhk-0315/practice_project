#ifndef __SERVER__SERVER_SOCKET__SERVER_TCP_H
#define __SERVER__SERVER_SOCKET__SERVER_TCP_H

#include "sock_msg.h"

int init_server_tcp(void);
int destroy_server_tcp(void);
_Bool epoll_trigge_tcpfd(int fd);
int send_msg_by_tcp(lc_msg_package_t* msg_pack, int cli_fd);
int recv_tcp_cli_msg(int fd);

#endif