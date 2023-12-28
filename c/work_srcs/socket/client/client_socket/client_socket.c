#include "client_socket.h"
#include "client_pool.h"
#include "client_tcp.h"
#include "client_udp.h"
#include "pre_modules.h"
#include "sock_msg.h"
#include "unsafe_api.h"

static socket_type_t socket_type = UNSET;

int switch_socket_type(void)
{
    if (socket_type == UNSET) {
        socket_type = UDP_LINK; // default
    } else {
        socket_type = (socket_type == UDP_LINK) ? TCP_LINK : UDP_LINK;
    }

    switch (socket_type) {
    case TCP_LINK:
        try_destroy_client_udp();
        init_client_tcp();
        break;
    case UDP_LINK:
        try_destroy_client_tcp();
        init_client_udp();
        break;
    default:
        break;
    }

    return 0;
}

char* display_cur_socket_type(void)
{
    switch (socket_type) {
    case TCP_LINK:
        return "socket=TCP!";
    case UDP_LINK:
        return "socket=UDP!";
    default:
        return "socket=UNSET!";
    }
}

static inline void send_msg_by_socket(lc_msg_package_t* _msgbuf)
{
    if (socket_type == TCP_LINK) {
        add_task_to_client_pool_release_arg_mem(send_msg_by_tcp,
            (void*)_msgbuf, sizeof(lc_msg_package_t));
    } else if (socket_type == UDP_LINK) {
        add_task_to_client_pool_release_arg_mem(send_msg_by_udp,
            (void*)_msgbuf, sizeof(lc_msg_package_t));
    }
}

int send_msg_to_server(void)
{
    lc_msg_package_t msgbuf = { 0 };
    ssize_t RXsize = 0;

    msgbuf.msg.msg_type = (lc_msg_type_t)socket_type;
    msgbuf.msg.srcid = read_pre_modules_addr()->endid;
    msgbuf.msg.destid = g_server_endid;

    while (1) {
        lc_printf("请输入你要发送的消息: ");
        RXsize = read(STDIN_FILENO, msgbuf.msg.data, sizeof(msgbuf.msg.data));
        if (RXsize < 0) {
            lc_err_logout("read STDIN_FILENO error");
            continue;
        } else if (RXsize == 0) {
            continue;
        }

        if (lc_strcasestr((char*)msgbuf.msg.data, "zkquit")) {
            return 0;
        }

        send_msg_by_socket(&msgbuf);
    }
}

int send_msg_to_other_cli(void)
{
    lc_msg_package_t msgbuf = { 0 };
    ssize_t RXsize = 0;

    msgbuf.msg.msg_type = (lc_msg_type_t)socket_type;
    msgbuf.msg.srcid = read_pre_modules_addr()->endid;
    lc_printf("请输入cli endid: ");
    lc_scanf("%d", &msgbuf.msg.destid);

    while (1) {
        lc_printf("请输入你要发送的消息: ");
        RXsize = read(STDIN_FILENO, msgbuf.msg.data, sizeof(msgbuf.msg.data));
        if (RXsize < 0) {
            lc_err_logout("read STDIN_FILENO error");
            continue;
        } else if (RXsize == 0) {
            continue;
        }

        if (lc_strcasestr((char*)msgbuf.msg.data, "zkquit")) {
            return 0;
        }

        send_msg_by_socket(&msgbuf);
    }
}

int init_cient_info_to_server(void)
{
    lc_msg_package_t msgbuf = { 0 };

    msgbuf.msg.msg_type = SAVE_CLI_INFO;
    msgbuf.msg.srcid = read_pre_modules_addr()->endid;
    msgbuf.msg.destid = g_server_endid;
    send_msg_by_socket(&msgbuf);

    return 0;
}
