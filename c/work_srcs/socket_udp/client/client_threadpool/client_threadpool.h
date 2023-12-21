#ifndef CLIENT__CLIENT_THREADPOOL__CLIENT_THREADPOOL_H
#define CLIENT__CLIENT_THREADPOOL__CLIENT_THREADPOOL_H

#include <stddef.h>

int CreateClientThreadPool(void);
int DestoryClientThreadPool(void);
int AddTaskToClientPool(void* (*Handler)(void*), void* arg);
int AddTaskToClientPoolReleaseArgMem(void* (*Handler)(void*), void* arg, size_t argSize);

#endif