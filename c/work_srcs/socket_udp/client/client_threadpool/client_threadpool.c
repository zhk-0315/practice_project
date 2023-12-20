#include <stddef.h>
#include <stdio.h>

#include "pre_process.h"
#include "thread_pool.h"

static ThreadPool* g_cliThreadPool = NULL;

int CreateClientThreadPool(void)
{
    if (g_cliThreadPool == NULL)
        g_cliThreadPool = CreateThreadPool(2, 5);

    DebugDgbout("leave");

    if (g_cliThreadPool == NULL)
        return -1;
    else
        return 0;
}

int DestoryClientThreadPool(void)
{
    return DestoryThreadPool(&g_cliThreadPool);
}

int AddTaskToClientPool(void* (*Handler)(void*), void* arg)
{
    return AddTaskToPool(g_cliThreadPool, Handler, arg);
}