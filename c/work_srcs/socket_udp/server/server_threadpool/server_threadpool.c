#include <stddef.h>
#include <stdio.h>

#include "pre_process.h"
#include "thread_pool.h"

static ThreadPool* g_srvThreadPool = NULL;

int CreateServerThreadPool(void)
{
    if (g_srvThreadPool != NULL)
        return 0;

    g_srvThreadPool = CreateThreadPool(5, 10);

    return 0;
}

int DestoryServerThreadPool(void)
{
    return DestoryThreadPool(&g_srvThreadPool);
}

int AddTaskToServerPool(void* (*Handler)(void*), void* arg)
{
    return AddTaskToPool(g_srvThreadPool, Handler, arg);
}