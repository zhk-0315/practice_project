#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include "client.h"
#include "client_pool.h"
#include "client_socket.h"
#include "pre_modules.h"
#include "sock_msg.h"

static int g_cli_tcpfd = 0;

int init_client_tcp(void)
{
    int iret = 0;
    int optval_ = 1;
    struct sockaddr_in srv_addr = { 0 };

    if (CHECK_FD(g_cli_tcpfd)) {
        return 0;
    }

    g_cli_tcpfd = socket(AF_INET, SOCK_STREAM, 0);
    if (!CHECK_FD(g_cli_tcpfd)) {
        lc_err_logout("socket g_cli_tcpfd error");
        return -1;
    }

    iret = setsockopt(g_cli_tcpfd, SOL_SOCKET, SO_REUSEADDR, &optval_,
        sizeof(optval_));
    if (iret != 0) {
        lc_err_logout("setsockopt g_cli_tcpfd error");
    }

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr(g_server_addr);
    srv_addr.sin_port = htons(g_server_tcp_port);
    iret = connect(g_cli_tcpfd, (struct sockaddr*)&srv_addr, g_addrlen);
    if (iret != 0) {
        lc_err_logout("connect server error");
    }

    return 0;
}

int try_destroy_client_tcp(void)
{
    if (!CHECK_FD(g_cli_tcpfd)) {
        return 0;
    }

    pthread_mutex_lock(&get_cli_flag()->mutex_);
    get_cli_flag()->flag.init_cli_info = 0;
    pthread_mutex_unlock(&get_cli_flag()->mutex_);

    close(g_cli_tcpfd);
    g_cli_tcpfd = 0;

    return 0;
}

void* send_msg_by_tcp(void* arg)
{
    ssize_t TXsize = 0;
    lc_msg_package_t* _msgbuf = (lc_msg_package_t*)arg;

    TXsize = send(g_cli_tcpfd, _msgbuf, sizeof(lc_msg_package_t), 0);
    if (TXsize != sizeof(lc_msg_package_t)) {
        lc_err_logout("send msg by tcp error");
    }

    return NULL + 1;
}

void* recv_msg_by_tcp(void* arg)
{
    lc_msg_package_t msgbuf = { 0 };
    ssize_t RXsize = 0;

    RXsize = recv(g_cli_tcpfd, &msgbuf, sizeof(lc_msg_package_t), 0);
    if (RXsize < 0) {
        lc_err_logout("recv tcp msg error");
        return NULL;
    } else if (RXsize == 0) {
        lc_logout("tcp link shutdown");
        try_destroy_client_tcp();
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