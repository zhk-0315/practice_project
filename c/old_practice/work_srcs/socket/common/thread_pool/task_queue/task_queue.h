#ifndef __COMMON__THREAD_POOL__TASK_QUEUE__TASK_QUEUE_H
#define __COMMON__THREAD_POOL__TASK_QUEUE__TASK_QUEUE_H

#include <stddef.h>

#include "list.h"

typedef struct task_t {
    void* (*handler)(void* arg);
    void* arg;
    size_t arg_size;
} task_t;

typedef struct task_queue_node_t {
    task_t task;
    struct list_head list;
} task_queue_node_t;

#ifndef LC_QUEUE_RET
#define LC_QUEUE_RET
typedef enum queue_ret_t {
    QUEUE_SUCCESS,
    QUEUE_ERROR,
    QUEUE_EMPTY,
    QUEUE_FULL
} queue_ret_t;
#endif

struct list_head* init_task_queue(void);
queue_ret_t en_task_to_queue(struct list_head* task_queue, const task_t* _task);
queue_ret_t de_task_from_queue(struct list_head* task_queue, task_t* _task);
queue_ret_t destroy_task_queue(struct list_head** _task_queue);

#endif