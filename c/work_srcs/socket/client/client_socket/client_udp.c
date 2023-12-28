#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include "pre_modules.h"
#include "sock_msg.h"

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
        lc_err_logout("send msg by tcp error");
    }

    return NULL + 1;
}
