#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "pre_modules.h"
#include "server_epoll.h"
#include "server_pool.h"
#include "server_tcp.h"
#include "server_udp.h"

static void set_pre_modules(void)
{
    // file_
    init_log_file_clear(1, 0);
    write_pre_modules_addr()->file_.status = ALWAYS_OPEN;
    getcwd(write_pre_modules_addr()->file_.path, LOG_FILE_PATH_LEN);
    strcat(write_pre_modules_addr()->file_.path, "/logsrv");
    // endid
    write_pre_modules_addr()->endid = g_server_endid;
}

static void unset_pre_modules(void)
{
    destroy_pre_modules();
}

static void init_all_modules(void)
{
    init_server_epoll();
    init_server_tcp();
    init_server_udp();
    

    create_server_thread_pool();
}

static void release_all_modules_resources(void)
{
    destroy_server_thread_pool();

    destroy_server_udp();
    destroy_server_tcp();
    destroy_server_epoll();
}

static void* test_thread_pool(void* arg)
{
    printf("test thread pool\n");
    fflush(stdout);

    return NULL + 1;
}

int main(int argc, const char* argv[])
{
    set_pre_modules();
    lc_logout("start server");
    init_all_modules();

    while (1) {
        // add_task_to_server_pool_release_arg_mem(test_thread_pool, NULL, 0);
        // usleep(200000);
        sleep(5);
    }

    release_all_modules_resources();
    unset_pre_modules();

    return 0;
}