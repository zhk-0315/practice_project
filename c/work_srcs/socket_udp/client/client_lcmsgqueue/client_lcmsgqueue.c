#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "client_lcmsgqueue.h"
#include "lcmsg_queue.h"
#include "list.h"

static CliMsgQueueManager* manager = NULL;

static CliMsgQueueManager* InitCliMsgQueueManager(void)
{
    manager = malloc(sizeof(CliMsgQueueManager));
    manager->msgQueue = InitMsgQueue();
    pthread_mutex_init(&manager->mutex, NULL);

    return manager;
}

int DestoryCliMsgQueue(void)
{
    if (manager == NULL)
        return 0;

    DestoryMsgQueue(&manager->msgQueue);
    pthread_mutex_destroy(&manager->mutex);
    free(manager);
    manager = NULL;

    return 0;
}

CliMsgQueueManager* GetCliMsgQueueManager(void)
{

    if (manager == NULL) {
        InitCliMsgQueueManager();
    }

    return manager;
}

int CliEnMsgQueue(const LcMsg* _msg)
{
    return EnMsgQueue(GetCliMsgQueueManager()->msgQueue, _msg);
}

int CliDeMsgQueue(LcMsg* _msg)
{
    return DeMsgQueue(GetCliMsgQueueManager()->msgQueue, _msg);
}