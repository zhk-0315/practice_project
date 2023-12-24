#ifndef __COMMON__THREAD_POOL__THREAD_POOL_H
#define __COMMON__THREAD_POOL__THREAD_POOL_H

#include <pthread.h>
#include <stdint.h>

#include "task_queue.h"

typedef struct thread_pool_t {
    //
    uint32_t init_threads_cnt;
    uint32_t max_threads_cnt;
    uint32_t cur_threads_cnt;
    uint32_t cur_tasks_cnt;
    //
    pthread_t* tids;
    struct list_head* queue;
    queue_ret_t (*en_queue)(struct list_head*, const task_t*);
    queue_ret_t (*de_queue)(struct list_head*, task_t*);
    //
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
} thread_pool_t;

void add_task_to_pool_use_arg_mem(thread_pool_t* pool, void* (*handler)(void*),
    void* arg);
void add_task_to_pool_release_arg_mem(thread_pool_t* pool,
    void* (*handler)(void*), void* arg, size_t arg_size);
thread_pool_t* create_thread_pool(uint32_t init_threads_cnt, uint32_t max_threads_cnt);
void destroy_thread_pool(thread_pool_t** _pool);

#endif