#ifndef __SERVER__SERVER_POOL__SERVER_POOL_H
#define __SERVER__SERVER_POOL__SERVER_POOL_H

#include <stddef.h>

void create_server_thread_pool(void);
void destroy_server_thread_pool(void);
void add_task_to_server_pool_use_arg_mem(void* (*handler)(void*), void* arg);
void add_task_to_server_pool_release_arg_mem(void* (*handler)(void*),
    void* arg, size_t arg_size);

#endif