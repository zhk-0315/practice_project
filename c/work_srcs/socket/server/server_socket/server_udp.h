#ifndef __SERVER__SERVER_SOCKET__SERVER_UDP_H
#define __SERVER__SERVER_SOCKET__SERVER_UDP_H

#include <netinet/in.h>

#include "sock_msg.h"

int init_server_udp(void);
int destroy_server_udp(void);
_Bool epoll_trigge_udpfd(int fd);
int send_msg_by_udp(endid_t endid, lc_msg_package_t* msg_pack, struct sockaddr_in* _lcaddr);

#endif