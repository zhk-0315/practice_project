#ifndef SERVER__SERVER_THREADPOOL__SERVER_THREADPOOL_H
#define SERVER__SERVER_THREADPOOL__SERVER_THREADPOOL_H

int CreateServerThreadPool(void);
int DestoryServerThreadPool(void);
int AddTaskToServerPool(void* (*Handler)(void*), void* arg);

#endif