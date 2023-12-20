#ifndef __SERVER_EPOLL_SERVER_EPOLL_H_
#define __SERVER_EPOLL_SERVER_EPOLL_H_

#include <stdint.h>

int GetEpFd(void);
int AddEpollByFd(const char*, int, uint32_t);
int InitEpoll(void);
int ProcessEventsByFd(int);
int ProcessEpoll(void);
int CreateEpollProcessThread(void);

#endif