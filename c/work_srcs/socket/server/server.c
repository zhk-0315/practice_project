#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "pre_modules.h"
#include "server_epoll.h"
#include "server_pool.h"
#include "server_socket.h"
#include "server_tcp.h"
#include "server_udp.h"

static void set_srv_pre_modules(void)
{
    // file_
    init_log_file_clear(1, 0);
    write_pre_modules_addr()->file_.status = ALWAYS_OPEN;
    getcwd(write_pre_modules_addr()->file_.path, LOG_FILE_PATH_LEN);
    strcat(write_pre_modules_addr()->file_.path, "/logsrv");
    // endid
    write_pre_modules_addr()->endid = g_server_endid;
    usleep(10000);
}

static void unset_srv_pre_modules(void)
{
    destroy_pre_modules();
}

static void init_all_srv_modules(void)
{
    init_server_epoll();
    init_server_tcp();
    init_server_udp();

    create_server_thread_pool();
    create_server_epoll_thread();
    create_socket_msg_process_thread();
}

static void release_all_srv_modules_resources(void)
{
    destroy_server_thread_pool();

    destroy_server_udp();
    destroy_server_tcp();
    destroy_server_epoll();
}

// #define LC_TEST_POOL
#ifdef LC_TEST_POOL
static void* test_thread_pool(void* arg)
{
    printf("test thread pool\n");
    fflush(stdout);

    return NULL + 1;
}
#endif

int main(int argc, const char* argv[])
{
    set_srv_pre_modules();
    lc_logout("start server");
    init_all_srv_modules();

    while (1) {
#ifdef LC_TEST_POOL
        add_task_to_server_pool_release_arg_mem(test_thread_pool, NULL, 0);
        usleep(200000);
#endif
        sleep(5);
    }

    release_all_srv_modules_resources();
    unset_srv_pre_modules();

    return 0;
}