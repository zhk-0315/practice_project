#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "client_lcmsgqueue.h"
#include "lcmsg_queue.h"
#include "list.h"

static CliMsgQueue* g_cliMsgQueue = NULL;

static int InitCliMsgQueue(void)
{
    if (g_cliMsgQueue != NULL)
        return 0;

    g_cliMsgQueue = malloc(sizeof(CliMsgQueue));
    g_cliMsgQueue->queue = InitMsgQueue();
    pthread_mutex_init(&g_cliMsgQueue->mutex, NULL);

    return 0;
}

int DestoryCliMsgQueue(void)
{
    if (g_cliMsgQueue == NULL)
        return 0;

    pthread_mutex_lock(&g_cliMsgQueue->mutex);
    DestoryMsgQueue(&g_cliMsgQueue->queue);
    free(g_cliMsgQueue->queue);
    pthread_mutex_unlock(&g_cliMsgQueue->mutex);
    pthread_mutex_destroy(&g_cliMsgQueue->mutex);

    free(g_cliMsgQueue);
    g_cliMsgQueue = NULL;

    return 0;
}

int EnCliMsgQueue(const LcMsg* _msg)
{
    int ret = 0;

    if (g_cliMsgQueue == NULL)
        InitCliMsgQueue();

    pthread_mutex_lock(&g_cliMsgQueue->mutex);
    ret = EnMsgQueue(g_cliMsgQueue->queue, _msg);
    pthread_mutex_unlock(&g_cliMsgQueue->mutex);

    return ret;
}

int DeCliMsgQueue(LcMsg* _msg)
{
    int ret = 0;

    if (g_cliMsgQueue == NULL)
        InitCliMsgQueue();

    pthread_mutex_lock(&g_cliMsgQueue->mutex);
    ret = DeMsgQueue(g_cliMsgQueue->queue, _msg);
    pthread_mutex_unlock(&g_cliMsgQueue->mutex);

    return ret;
}