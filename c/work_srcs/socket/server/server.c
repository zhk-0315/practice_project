#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "pre_modules.h"
#include "server_pool.h"

static void set_pre_modules(void)
{
    // file_
    init_log_file_clear(1, 0);
    write_pre_modules_addr()->file_.status = ALWAYS_OPEN;
    getcwd(write_pre_modules_addr()->file_.path, LOG_FILE_PATH_LEN);
    strcat(write_pre_modules_addr()->file_.path, "/logsrv");
    // endid
    write_pre_modules_addr()->endid = getpid();
}

static void unset_pre_modules(void)
{
    destroy_pre_modules();
}

static void init_all_modules(void)
{
    create_server_thread_pool();
}

static void release_all_modules_resources(void)
{
    destroy_server_thread_pool();
}

int main(int argc, const char* argv[])
{
    set_pre_modules();
    lc_logout("start server");
    init_all_modules();

    while (1) {
    }

    release_all_modules_resources();
    unset_pre_modules();

    return 0;
}