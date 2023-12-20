#ifndef CLIENT__CLIENT_THREADPOOL__CLIENT_THREADPOOL_H
#define CLIENT__CLIENT_THREADPOOL__CLIENT_THREADPOOL_H

int CreateClientThreadPool(void);
int DestoryClientThreadPool(void);
int AddTaskToClientPool(void* (*Handler)(void*), void* arg);

#endif