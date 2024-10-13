#include <stdio.h>

#include "link_queue.h"
#include "local_queue.h"
#include "pre_process.h"

static LinkQueue* linkQueue = NULL;
LinkQueue* GetMsgLinkQueue(void)
{
    if (linkQueue == NULL) {
        linkQueue = InitLinkQueue(sizeof(MsgBuf));
    }

    return linkQueue;
}

int DestoryMsgLinkQueue(void)
{
    if (linkQueue == NULL) {
        return -1;
    }

    return DestoryLinkQueue(&linkQueue);
}

int EnMsgLinkQueue(const MsgBuf* msgBuf)
{
    DebugDbgout("EnMsgLinkQueue");
    return EnLinkQueue(GetMsgLinkQueue(), (const void*)msgBuf, sizeof(MsgBuf));
}

int DeMsgLinkQueue(MsgBuf* msgBuf)
{
    return DeLinkQueue(GetMsgLinkQueue(), (void*)msgBuf, sizeof(MsgBuf));
}