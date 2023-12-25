#ifndef __SERVER__SERVER_SOCKET__SERVER_UDP_H
#define __SERVER__SERVER_SOCKET__SERVER_UDP_H

int init_server_udp(void);
int destroy_server_udp(void);
_Bool epoll_trigge_udpfd(int fd);

#endif