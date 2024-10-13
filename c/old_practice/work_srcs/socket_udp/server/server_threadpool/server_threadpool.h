#ifndef SERVER__SERVER_THREADPOOL__SERVER_THREADPOOL_H
#define SERVER__SERVER_THREADPOOL__SERVER_THREADPOOL_H

#include <stddef.h>

int CreateServerThreadPool(void);
int DestoryServerThreadPool(void);
int AddTaskToServerPool(void* (*Handler)(void*), void* arg);
int AddTaskToServerPoolReleaseArgMem(void* (*Handler)(void*), void* arg, size_t argSize);

#endif