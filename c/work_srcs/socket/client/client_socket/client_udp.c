#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include "pre_modules.h"

static int g_cli_udpfd = 0;

int init_client_udp(void)
{
    int iret = 0;
    int optval_ = 1;
    struct sockaddr_in lcaddr = { 0 };

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

int destroy_client_udp(void)
{
    if (!CHECK_FD(g_cli_udpfd)) {
        return 0;
    }

    return close(g_cli_udpfd);
}
