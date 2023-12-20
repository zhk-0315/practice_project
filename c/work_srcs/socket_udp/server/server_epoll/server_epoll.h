#ifndef SERVER__SERVER_EPOLL__SERVER_EPOLL_H
#define SERVER__SERVER_EPOLL__SERVER_EPOLL_H

int InitServerEpoll(void);
int DestoryServerEpoll(void);
int CreateServerEpollThread(void);

#define MAXEVENTS (50)

#endif