#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "lcmsg_queue.h"
#include "list.h"
#include "server_lcmsgqueue.h"

static SrvMsgQueue* g_srvMsgQueue = NULL;

static int InitSrvMsgQueue(void)
{
    if (g_srvMsgQueue != NULL)
        return 0;

    g_srvMsgQueue = (SrvMsgQueue*)malloc(sizeof(SrvMsgQueue));
    g_srvMsgQueue->queue = (struct list_head*)malloc(sizeof(struct list_head));

    INIT_LIST_HEAD(g_srvMsgQueue->queue);
    pthread_mutex_init(&g_srvMsgQueue->mutex, NULL);

    return 0;
}

int DestorySrvMsgQueue(void)
{
    if (!g_srvMsgQueue)
        return 0;

    pthread_mutex_lock(&g_srvMsgQueue->mutex);
    DestoryMsgQueue(&g_srvMsgQueue->queue);
    free(g_srvMsgQueue->queue);
    pthread_mutex_unlock(&g_srvMsgQueue->mutex);
    pthread_mutex_destroy(&g_srvMsgQueue->mutex);

    free(g_srvMsgQueue);

    return 0;
}

int EnSrvMsgQueue(const LcMsg* _lcMsg)
{
    int ret = 0;

    if (!g_srvMsgQueue)
        InitSrvMsgQueue();

    pthread_mutex_lock(&g_srvMsgQueue->mutex);
    ret = EnMsgQueue(g_srvMsgQueue->queue, _lcMsg);
    pthread_mutex_unlock(&g_srvMsgQueue->mutex);

    return ret;
}

int DeSrvMsgQueue(LcMsg* _lcMsg)
{
    int ret = 0;

    if (!g_srvMsgQueue)
        InitSrvMsgQueue();

    pthread_mutex_lock(&g_srvMsgQueue->mutex);
    ret = DeMsgQueue(g_srvMsgQueue->queue, _lcMsg);
    pthread_mutex_unlock(&g_srvMsgQueue->mutex);

    return ret;
}