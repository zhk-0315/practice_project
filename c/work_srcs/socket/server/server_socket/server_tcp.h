#ifndef __SERVER__SERVER_SOCKET__SERVER_TCP_H
#define __SERVER__SERVER_SOCKET__SERVER_TCP_H

#include "pre_modules.h"
#include "sock_msg.h"

int init_server_tcp(void);
int destroy_server_tcp(void);
_Bool epoll_trigge_tcpfd(int fd);
int send_msg_by_tcp(endid_t endid, lc_msg_package_t* msg_pack, int cli_fd);

#endif