#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <unistd.h>

#include "list.h"
#include "pre_process.h"
#include "safe_api.h"
#include "task_queue.h"
#include "thread_pool.h"

int AddTaskToPool(ThreadPool* pool, void* (*Handler)(void*), void* arg)
{
    Task task_ = {
        .Handler = Handler,
        .arg = arg,
    };

    if (pool == NULL)
        return -1;

    pthread_mutex_lock(&pool->mutex);
    if (pool->EnTaskQueue(pool->taskQueue, &task_) == QUEUE_SUCCESS) {
        pool->curTaskCnt++;
    }
    pthread_mutex_unlock(&pool->mutex);
    pthread_cond_signal(&pool->cond);
    return 0;
}

static void* ProcessTask(void* arg)
{
    char threadName[17] = { 0 };
    int sRet = 0;
    unsigned int num = 0;
    ThreadPool* pool = (ThreadPool*)arg;
    pthread_t selfTid = pthread_self();
    Task task_ = { 0 };

    if (!pool)
        return pool;

    num = pool->curThreadsCnt;
    pthread_detach(selfTid);

    sprintf(threadName, "poolThNum%u", num);
    LCpthread_setname_np(selfTid, threadName);

    while (1) {
        pthread_mutex_lock(&pool->mutex);
        while ((sRet = pool->DeTaskQueue(pool->taskQueue, &task_)) == QUEUE_EMPTY) {
            pthread_cond_wait(&pool->cond, &pool->mutex);
        }
        pthread_mutex_unlock(&pool->mutex);

        if (sRet == QUEUE_SUCCESS) {
            pool->curTaskCnt--;
            task_.Handler(task_.arg);
        }
    }

    return pool;
}

ThreadPool* CreateThreadPool(unsigned int initThreadsCnt, unsigned int maxThreadsCnt)
{
    int sRet = 0;
    register int i = 0;
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));

    //
    pool->curThreadsCnt = 0;
    pool->initThreadsCnt = initThreadsCnt;
    pool->maxThreadsCnt = maxThreadsCnt;
    pool->curTaskCnt = 0;
    //
    pool->taskQueue = InitTaskQueue();
    pool->EnTaskQueue = EnTaskQueue;
    pool->DeTaskQueue = DeTaskQueue;
    //
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);
    //
    pool->poolTids = (pthread_t*)calloc(pool->initThreadsCnt,
        sizeof(pthread_t));

    pthread_mutex_lock(&pool->mutex);
    for (i = 0; i < pool->maxThreadsCnt; i++) {
        if (pool->curThreadsCnt >= pool->maxThreadsCnt) {
            break;
        }
        if (pool->curThreadsCnt >= pool->initThreadsCnt) {
            break;
        }

        sRet = pthread_create(pool->poolTids + i, NULL, ProcessTask,
            (void*)pool);
        if (sRet != 0) {
            LocalDbgout("create ThreadPoolTaskHandler error: %s", strerror(errno));
            continue;
        } else {
            pool->curThreadsCnt++;
        }
        usleep(5000);
    }
    pthread_mutex_unlock(&pool->mutex);

    return pool;
}

int DestoryThreadPool(ThreadPool** _threadPool)
{
    int i = 0;
    ThreadPool* pool = *_threadPool;

    if (pool == NULL)
        return 0;

    pthread_mutex_lock(&pool->mutex);
    for (i = 0; i < pool->curThreadsCnt; i++) {
        pthread_cancel(pool->poolTids[i]);
    }
    free(pool->poolTids);
    DestoryTaskQueue(&pool->taskQueue);
    pthread_mutex_unlock(&pool->mutex);
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
    free(pool);
    *_threadPool = NULL;

    return 0;
}