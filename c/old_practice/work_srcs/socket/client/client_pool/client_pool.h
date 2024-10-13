#ifndef __CLIENT__CLIENT_POOL__CLIENT_POOL_H
#define __CLIENT__CLIENT_POOL__CLIENT_POOL_H

#include <stddef.h>

void create_client_thread_pool(void);
void destroy_client_thread_pool(void);
void add_task_to_client_pool_use_arg_mem(void* (*handler)(void*), void* arg);
void add_task_to_client_pool_release_arg_mem(void* (*handler)(void*),
    void* arg, size_t arg_size);

#endif