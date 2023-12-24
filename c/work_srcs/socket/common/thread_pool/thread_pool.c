#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pre_modules.h"
#include "thread_pool.h"
#include "unsafe_api.h"

void add_task_to_pool_use_arg_mem(thread_pool_t* pool, void* (*handler)(void*),
    void* arg)
{
    task_t task = {
        .handler = handler,
        .arg = arg,
        .arg_size = 0
    };

    pthread_mutex_lock(&pool->mutex_);
    if (pool->en_queue(pool->queue, &task) == QUEUE_SUCCESS) {
        pool->cur_tasks_cnt++;
    }
    pthread_cond_broadcast(&pool->cond_);
    pthread_mutex_unlock(&pool->mutex_);
}

void add_task_to_pool_release_arg_mem(thread_pool_t* pool,
    void* (*handler)(void*), void* arg, size_t arg_size)
{
    task_t task = {
        .handler = handler,
        .arg = arg ? malloc(arg_size) : NULL,
        .arg_size = arg_size
    };

    if (task.arg_size > 0 && task.arg) {
        memcpy(task.arg, arg, task.arg_size);
    }

    pthread_mutex_lock(&pool->mutex_);
    if (pool->en_queue(pool->queue, &task) == QUEUE_SUCCESS) {
        pool->cur_tasks_cnt++;
    }
    pthread_cond_broadcast(&pool->cond_);
    pthread_mutex_unlock(&pool->mutex_);
}

static void* pool_task_process_thread(void* arg)
{
    thread_pool_t* pool = (thread_pool_t*)arg;
    uint32_t num = pool->cur_threads_cnt;
    char thread_name[17] = { 0 };
    task_t task = { 0 };
    pthread_t tid = pthread_self();

    if (!pool) {
        return NULL;
    }

    pthread_detach(tid);
    sprintf(thread_name, "pool_thread_%u", num);
    lc_pthread_setname_np(tid, thread_name);

    while (1) {
        pthread_mutex_lock(&pool->mutex_);
        while (pool->de_queue(pool->queue, &task) != QUEUE_SUCCESS) {
            pthread_cond_wait(&pool->cond_, &pool->mutex_);
        }
        pool->cur_tasks_cnt--;
        pthread_mutex_unlock(&pool->mutex_);
        task.handler(task.arg);
        if (task.arg_size > 0 && task.arg) {
            free(task.arg);
        }
    }

    return NULL + 1;
}

thread_pool_t* create_thread_pool(uint32_t init_threads_cnt, uint32_t max_threads_cnt)
{
    int i = 0;
    int iret = 0;
    thread_pool_t* pool = (thread_pool_t*)malloc(sizeof(thread_pool_t));

    pool->init_threads_cnt = init_threads_cnt;
    pool->max_threads_cnt = max_threads_cnt;
    pool->cur_threads_cnt = 0;
    pool->cur_tasks_cnt = 0;
    pool->queue = init_task_queue();
    pool->en_queue = en_task_to_queue;
    pool->de_queue = de_task_from_queue;
    pthread_mutex_init(&pool->mutex_, NULL);
    pthread_cond_init(&pool->cond_, NULL);

    pool->tids = (pthread_t*)malloc(pool->init_threads_cnt * sizeof(pthread_t));
    pthread_mutex_lock(&pool->mutex_);
    for (i = 0; i < pool->init_threads_cnt; i++) {
        iret = pthread_create(pool->tids + i, NULL, pool_task_process_thread,
            (void*)pool);
        if (iret) {
            lc_logout("create pool thread(%d) error", i);
        } else {
            usleep(10000);
            pool->cur_threads_cnt++;
        }
    }
    pthread_mutex_unlock(&pool->mutex_);

    return pool;
}

void destroy_thread_pool(thread_pool_t** _pool)
{
    thread_pool_t* pool = *_pool;
    int i = 0;

    if (!pool) {
        return;
    }

    pthread_mutex_lock(&pool->mutex_);
    for (i = 0; i < pool->cur_threads_cnt; i++) {
        pthread_cancel(pool->tids[i]);
    }
    destroy_task_queue(&pool->queue);
    pthread_mutex_unlock(&pool->mutex_);
    pthread_cond_destroy(&pool->cond_);
    pthread_mutex_destroy(&pool->mutex_);

    free(pool->tids);
    free(pool);
    *_pool = NULL;
}