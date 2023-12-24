#include <netinet/in.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include "pre_modules.h"
#include "server_epoll.h"

static int g_tcpfd = 0;

int init_server_tcp(void)
{
    int iret = 0;
    int optval_ = 1;
    struct sockaddr_in addr = { 0 };

    if (CHECK_FD(g_tcpfd)) {
        return 0;
    }

    g_tcpfd = socket(AF_INET, SOCK_STREAM, 0);
    if (!CHECK_FD(g_tcpfd)) {
        lc_err_logout("socket g_tcpfd error");
        return -1;
    }

    iret = setsockopt(g_tcpfd, SOL_SOCKET, SO_REUSEADDR, &optval_, sizeof(optval_));
    if (iret != 0) {
        lc_err_logout("setsockopt g_tcpfd error");
    }

    iret = bind(g_tcpfd, (struct sockaddr*)&addr, g_addrlen);
    if (iret != 0) {
        lc_err_logout("bind g_tcpfd error");
        return -1;
    }

    iret = listen(g_tcpfd, 3);
    if (iret != 0) {
        lc_err_logout("listen g_tcpfd error");
        return -1;
    }

    add_fd_to_srv_epoll(g_tcpfd, EPOLLIN | EPOLLET);

    return 0;
}

int destroy_server_tcp(void)
{
    if (!CHECK_FD(g_tcpfd)) {
        return 0;
    }

    del_fd_from_srv_epoll(g_tcpfd);

    return close(g_tcpfd);
}