#ifndef COMMON__THREAD_POOL__TASK_QUEUE__TASK_QUEUE_H
#define COMMON__THREAD_POOL__TASK_QUEUE__TASK_QUEUE_H

#include "list.h"

#ifndef QUEUE_ERROR
#define QUEUE_ERROR (-1)
enum {
    QUEUE_SUCCESS,
    QUEUE_EMPTY,
    QUEUE_FULL
};
#endif

typedef struct Task {
    void* (*Handler)(void* arg);
    void* arg;
} Task;

typedef struct TaskNode {
    Task task;
    struct list_head list;
} TaskNode;

struct list_head* InitTaskQueue(void);
int EnTaskQueue(struct list_head* taskQueue, const Task* _task);
int DeTaskQueue(struct list_head* taskQueue, Task* _task);
int DestoryTaskQueue(struct list_head** _taskQueue);

#endif