#include <pthread.h>
#include <stdio.h>

#include "client_interface.h"
#include "client_socket.h"
#include "pre_modules.h"
#include "unsafe_api.h"

typedef struct interface_info_t {
    char* info;
    int (*op_handler)(void);
} interface_info_t;

static interface_info_t interface_info[] = {
    { "选择网络模式(TCP/UDP)", switch_socket_type },
    { "发送消息到server", send_msg_to_server },
    { "发送消息到其他client", send_msg_to_other_cli }
};

static void* cli_interface_thread(void* arg)
{
    pthread_t tid = pthread_self();
    int lc_switch = 0;
    int i = 0;
    int info_cnt = sizeof(interface_info) / sizeof(interface_info_t);

    pthread_detach(tid);
    lc_pthread_setname_np(tid, "CliUI");

    while (1) {
        lc_display_str("欢迎使用client! %s, endid=%d", display_cur_socket_type(), read_pre_modules_addr()->endid);
        for (i = 0; i < info_cnt; i++) {
            lc_display_str("%d: %s", i, interface_info[i].info);
        }
        lc_printf("请选择你的选项: ");
        lc_scanf("%d", &lc_switch);

        if (interface_info[lc_switch].op_handler) {
            interface_info[lc_switch].op_handler();
        }
    }

    return NULL + 1;
}

int create_cli_interface_thread(void)
{
    int iret = 0;
    pthread_t tid = 0;

    iret = pthread_create(&tid, NULL, cli_interface_thread, NULL);
    if (iret != 0) {
        lc_err_logout("create cli_interface_thread error");
    }

    return 0;
}
