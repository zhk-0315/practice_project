#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include "pre_modules.h"
#include "server_epoll.h"
#include "sock_msg.h"

static int g_udpfd = 0;

int init_server_udp(void)
{
    int iret = 0;
    int optval_ = 1;
    struct sockaddr_in lcaddr = { 0 };

    if (CHECK_FD(g_udpfd)) {
        return 0;
    }

    g_udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (!CHECK_FD(g_udpfd)) {
        lc_err_logout("socket g_udpfd error");
        return -1;
    }

    iret = setsockopt(g_udpfd, SOL_SOCKET, SO_REUSEADDR, &optval_, sizeof(optval_));
    if (iret != 0) {
        lc_err_logout("setsockopt g_udpfd error");
    }

    lcaddr.sin_family = AF_INET;
    lcaddr.sin_addr.s_addr = INADDR_ANY;
    lcaddr.sin_port = htons(g_server_udp_port);
    iret = bind(g_udpfd, (struct sockaddr*)&lcaddr, g_addrlen);
    if (iret != 0) {
        lc_err_logout("bind g_udpfd error");
        return -1;
    }

    add_fd_to_srv_epoll(g_udpfd, EPOLLIN | EPOLLET);

    return 0;
}

int destroy_server_udp(void)
{
    if (!CHECK_FD(g_udpfd)) {
        return 0;
    }

    del_fd_from_srv_epoll(g_udpfd);

    return close(g_udpfd);
}

_Bool epoll_trigge_udpfd(int fd)
{
    return (fd == g_udpfd);
}

int send_msg_by_udp(endid_t endid, lc_msg_package_t* msg_pack, struct sockaddr_in* _lcaddr)
{
    ssize_t TXsize = 0;

    TXsize = sendto(g_udpfd, msg_pack, sizeof(lc_msg_package_t), 0,
        (struct sockaddr*)_lcaddr, g_addrlen);
    if (TXsize != sizeof(lc_msg_package_t)) {
        lc_err_logout("sendto to endid(%d) error", endid);
        return -1;
    }

    return 0;
}
