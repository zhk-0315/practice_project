#ifndef __SERVER__SERVER_EPOLL__SERVER_EPOLL_H
#define __SERVER__SERVER_EPOLL__SERVER_EPOLL_H

#include <stdint.h>

int init_server_epoll(void);
int destroy_server_epoll(void);
int add_fd_to_srv_epoll(int fd, uint32_t events_mask);
int del_fd_from_srv_epoll(int fd);
void create_server_epoll_thread(void);

#endif