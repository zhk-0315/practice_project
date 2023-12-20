#include <stdio.h>
#include <stdlib.h>

#include "lcmsg_queue.h"

int DeMsgQueue(struct list_head* msgQueue, LcMsg* _lcMsg);

static struct list_head* g_srvMsgQueue = NULL;
static int InitServerQueue(void)
{
    if (!g_srvMsgQueue) {
        g_srvMsgQueue = (struct list_head*)malloc(sizeof(struct list_head));
    }

    return 0;
}

int DestorySrvMsgQueue(void)
{
    if (!g_srvMsgQueue)
        return 0;

    return DestoryMsgQueue(&g_srvMsgQueue);
}

int EnSrvMsgQueue(const LcMsg* _lcMsg)
{
    if (!g_srvMsgQueue)
        InitServerQueue();

    return EnMsgQueue(g_srvMsgQueue, _lcMsg);
}

int DeSrvMsgQueue(LcMsg* _lcMsg)
{
    if (!g_srvMsgQueue)
        InitServerQueue();

    return DeMsgQueue(g_srvMsgQueue, _lcMsg);
}