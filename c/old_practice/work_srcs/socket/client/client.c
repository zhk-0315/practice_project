#include "client.h"
#include "client_interface.h"
#include "client_pool.h"
#include "client_socket.h"
#include "pre_modules.h"

static cli_flag_manager_t g_cli_flag = {
    .flag = { 0 },
    .mutex_ = PTHREAD_MUTEX_INITIALIZER
};

cli_flag_manager_t* get_cli_flag(void)
{
    return &g_cli_flag;
}

static void set_cli_pre_modules(void)
{
    // file_
    init_log_file_clear(1, 0);
    write_pre_modules_addr()->file_.status = ALWAYS_OPEN;
    getcwd(write_pre_modules_addr()->file_.path,
        sizeof(write_pre_modules_addr()->file_.path));
    strcat(write_pre_modules_addr()->file_.path, "/logcli");
    // endid
    write_pre_modules_addr()->endid = getpid();
    usleep(10000);
}

static void unset_cli_pre_modules(void)
{
    destroy_pre_modules();
}

static void init_all_cli_modules(void)
{
    create_client_thread_pool();
    create_sock_msg_recv_thread();
    switch_socket_type();
    create_cli_interface_thread();
}

static void release_all_cli_modules_resources(void)
{
    destroy_client_thread_pool();

    destroy_socket();
}

// #define LC_TEST_POOL
#ifdef LC_TEST_POOL
static int aaaa = 0;
static void* test_thread_pool(void* arg)
{
    int num = *(int*)arg;

    printf("test thread pool %d\n", num);
    fflush(stdout);

    return NULL + 1;
}
#endif

int main(int argc, const char* argv[])
{
    set_cli_pre_modules();
    lc_logout("start client");
    init_all_cli_modules();

    while (1) {
#ifdef LC_TEST_POOL
        add_task_to_client_pool_release_arg_mem(test_thread_pool, &aaaa, sizeof(aaaa));
        usleep(200000);
#else
        sleep(5);
#endif
    }

    release_all_cli_modules_resources();
    unset_cli_pre_modules();

    return 0;
}