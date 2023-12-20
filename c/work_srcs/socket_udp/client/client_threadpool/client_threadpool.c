#include <stddef.h>
#include <stdio.h>

#include "pre_process.h"
#include "thread_pool.h"

static ThreadPool* g_cliThreadPool = NULL;

int CreateClientThreadPool(void)
{
    if (g_cliThreadPool != NULL)
        return 0;

    g_cliThreadPool = CreateThreadPool(2, 5);

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