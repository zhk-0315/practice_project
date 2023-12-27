#include <stdio.h>
#include <stdlib.h>

#include "thread_pool.h"

static thread_pool_t* g_cli_thread_pool = NULL;

void create_client_thread_pool(void)
{
    if (g_cli_thread_pool) {
        return;
    }

    g_cli_thread_pool = create_thread_pool(3, 3);
}

void destroy_client_thread_pool(void)
{
    if (g_cli_thread_pool) {
        free(g_cli_thread_pool);
    }
}

void add_task_to_client_pool_use_arg_mem(void* (*handler)(void*), void* arg)
{
    if (!g_cli_thread_pool) {
        create_client_thread_pool();
    }

    return add_task_to_pool_use_arg_mem(g_cli_thread_pool, handler, arg);
}

void add_task_to_client_pool_release_arg_mem(void* (*handler)(void*),
    void* arg, size_t arg_size)
{
    if (!g_cli_thread_pool) {
        create_client_thread_pool();
    }

    return add_task_to_pool_release_arg_mem(g_cli_thread_pool, handler, arg, arg_size);
}