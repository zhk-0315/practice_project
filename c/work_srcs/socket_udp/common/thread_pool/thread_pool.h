#ifndef COMMON__THREAD_POOL__THREAD_POOL_H
#define COMMON__THREAD_POOL__THREAD_POOL_H

#include <pthread.h>

#include "list.h"
#include "task_queue.h"

typedef struct ThreadPool {
    unsigned int curThreadsCnt;
    unsigned int maxThreadsCnt;
    unsigned int initThreadsCnt;
    unsigned int curTaskCnt;
    //
    pthread_t* poolTids;
    struct list_head* taskQueue;
    int (*EnTaskQueue)(struct list_head*, const Task*);
    int (*DeTaskQueue)(struct list_head*, Task*);
    //
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} ThreadPool;

int AddTaskToPool(ThreadPool* pool, void* (*Handler)(void*), void* arg);
ThreadPool* CreateThreadPool(unsigned int initThreadsCnt, unsigned int maxThreadsCnt);
int DestoryThreadPool(ThreadPool** _threadPool);

#define MAX_THREADS_CNT (50)

#endif