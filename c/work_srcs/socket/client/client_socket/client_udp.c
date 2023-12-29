#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include "client_pool.h"
#include "client_socket.h"
#include "pre_modules.h"
#include "sock_msg.h"
#include "client.h"

static int g_cli_udpfd = 0;

int init_client_udp(void)
{
    int iret = 0;
    int optval_ = 1;

    if (CHECK_FD(g_cli_udpfd)) {
        return 0;
    }

    g_cli_udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (!CHECK_FD(g_cli_udpfd)) {
        lc_err_logout("socket g_cli_udpfd error");
        return -1;
    }

    iret = setsockopt(g_cli_udpfd, SOL_SOCKET, SO_REUSEADDR, &optval_, sizeof(optval_));
    if (iret != 0) {
        lc_err_logout("setsockopt g_cli_udpfd error");
    }

    return 0;
}

int try_destroy_client_udp(void)
{
    if (!CHECK_FD(g_cli_udpfd)) {
        return 0;
    }

    pthread_mutex_lock(&get_cli_flag()->mutex_);
    get_cli_flag()->flag.init_cli_info = 0;
    pthread_mutex_unlock(&get_cli_flag()->mutex_);

    close(g_cli_udpfd);
    g_cli_udpfd = 0;

    return 0;
}

void* send_msg_by_udp(void* arg)
{
    ssize_t TXsize = 0;
    lc_msg_package_t* _msgbuf = (lc_msg_package_t*)arg;
    static struct sockaddr_in srv_addr = { 0 };

    if (!srv_addr.sin_family) {
        srv_addr.sin_family = AF_INET;
        srv_addr.sin_addr.s_addr = inet_addr(g_server_addr);
        srv_addr.sin_port = htons(g_server_udp_port);
    }
    TXsize = sendto(g_cli_udpfd, _msgbuf, sizeof(lc_msg_package_t), 0,
        (struct sockaddr*)&srv_addr, g_addrlen);
    if (TXsize != sizeof(lc_msg_package_t)) {
        lc_err_logout("send msg by udp error");
    }

    return NULL + 1;
}

void* recv_msg_by_udp(void* arg)
{
    lc_msg_package_t msgbuf = { 0 };
    ssize_t RXsize = 0;

    RXsize = recvfrom(g_cli_udpfd, &msgbuf, sizeof(lc_msg_package_t), 0,
        NULL, &g_addrlen);
    if (RXsize < 0) {
        lc_err_logout("recv udp msg error");
        return NULL;
    } else if (RXsize == 0) {
        return NULL;
    }

    if (msgbuf.msg.msg_type == SAVE_CLI_INFO) {
        add_task_to_client_pool_use_arg_mem(complete_init_client_info, NULL);
    } else {
        add_task_to_client_pool_release_arg_mem(dispaly_msg_package,
            &msgbuf, sizeof(lc_msg_package_t));
    }

    return NULL + 1;
}
