#include <stdio.h>
#include <stdlib.h>

#include "task_queue.h"

struct list_head* init_task_queue(void)
{
    struct list_head* task_queue
        = (struct list_head*)malloc(sizeof(struct list_head));

    INIT_LIST_HEAD(task_queue);

    return task_queue;
}

queue_ret_t en_task_to_queue(struct list_head* task_queue, const task_t* _task)
{
    task_queue_node_t* node = NULL;

    if (!task_queue || !_task) {
        return QUEUE_ERROR;
    }

    node = (task_queue_node_t*)malloc(sizeof(task_queue_node_t));
    node->task = *_task;
    list_add_tail(&node->list, task_queue);

    return QUEUE_SUCCESS;
}

queue_ret_t de_task_from_queue(struct list_head* task_queue, task_t* _task)
{
    task_queue_node_t* node = NULL;

    if (!task_queue) {
        return QUEUE_ERROR;
    }

    node = list_entry(task_queue->next, task_queue_node_t, list);
    if (_task) {
        *_task = node->task;
    }
    list_del(&node->list);
    free(node);

    return QUEUE_SUCCESS;
}

queue_ret_t destroy_task_queue(struct list_head** _task_queue)
{
    struct list_head* task_queue = *_task_queue;

    if (!task_queue) {
        return QUEUE_SUCCESS;
    }

    while (QUEUE_SUCCESS == de_task_from_queue(task_queue, NULL)) { }
    free(task_queue);
    *_task_queue = NULL;

    return QUEUE_SUCCESS;
}