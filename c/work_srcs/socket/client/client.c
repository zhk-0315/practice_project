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

int main(int argc, const char* argv[])
{
    set_cli_pre_modules();
    lc_logout("start client");

    while (1) {
    }

    unset_cli_pre_modules();

    return 0;
}