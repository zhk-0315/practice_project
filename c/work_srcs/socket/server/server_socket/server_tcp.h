#ifndef __SERVER__SERVER_SOCKET__SERVER_TCP_H
#define __SERVER__SERVER_SOCKET__SERVER_TCP_H

int init_server_tcp(void);
int destroy_server_tcp(void);
_Bool epoll_trigge_tcpfd(int fd);

#endif