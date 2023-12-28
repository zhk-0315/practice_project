#include "client_interface.h"
#include "client_pool.h"
#include "client_socket.h"
#include "pre_modules.h"

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
    switch_socket_type();

    create_client_thread_pool();
    create_cli_interface_thread();
}

static void release_all_cli_modules_resources(void)
{
    destroy_client_thread_pool();

    destroy_socket();
}

int main(int argc, const char* argv[])
{
    set_cli_pre_modules();
    lc_logout("start client");
    init_all_cli_modules();

    while (1) {
    }

    release_all_cli_modules_resources();
    unset_cli_pre_modules();

    return 0;
}