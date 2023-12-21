#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pre_process.h"
#include "thread_pool.h"

static ThreadPool* g_srvThreadPool = NULL;

int CreateServerThreadPool(void)
{
    if (g_srvThreadPool != NULL)
        return 0;

    g_srvThreadPool = CreateThreadPool(5, 10);

    LocalDbgout("CreateServerThreadPool complete");

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

int AddTaskToServerPoolReleaseArgMem(void* (*Handler)(void*), void* arg, size_t argSize)
{
    void* tmp = malloc(argSize);
    memcpy(tmp, arg, argSize);

    return AddTaskToPool(g_srvThreadPool, Handler, tmp);
}