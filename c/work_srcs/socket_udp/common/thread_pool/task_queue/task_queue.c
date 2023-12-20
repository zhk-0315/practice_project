#include <stdio.h>
#include <stdlib.h>

#include "task_queue.h"

struct list_head* InitTaskQueue(void)
{
    struct list_head* taskQueue = NULL;

    taskQueue = (struct list_head*)malloc(sizeof(struct list_head));
    if (taskQueue == NULL)
        return NULL;

    INIT_LIST_HEAD(taskQueue);

    return taskQueue;
}

int DestoryTaskQueue(struct list_head** _taskQueue)
{
    struct list_head* taskQueue = *_taskQueue;

    if (taskQueue == NULL)
        return QUEUE_SUCCESS;

    while (DeTaskQueue(taskQueue, NULL) != QUEUE_EMPTY) { }

    free(taskQueue);
    *_taskQueue = NULL;

    return QUEUE_SUCCESS;
}

int EnTaskQueue(struct list_head* taskQueue, const Task* _task)
{
    TaskNode* taskNode = NULL;

    if (taskQueue == NULL)
        return QUEUE_ERROR;

    taskNode = (TaskNode*)malloc(sizeof(TaskNode));
    if (_task != NULL)
        taskNode->task = *_task;

    list_add_tail(&taskNode->list, taskQueue);

    return QUEUE_SUCCESS;
}

int DeTaskQueue(struct list_head* taskQueue, Task* _task)
{
    TaskNode* taskNode = NULL;

    if (taskQueue == NULL)
        return QUEUE_ERROR;

    if (list_empty(taskQueue))
        return QUEUE_EMPTY;

    taskNode = list_entry(taskQueue->next, TaskNode, list);
    if (_task != NULL)
        *_task = taskNode->task;

    list_del(&taskNode->list);
    free(taskNode);

    return QUEUE_SUCCESS;
}