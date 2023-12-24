#include <stdio.h>
#include <sys/epoll.h>

#include "pre_modules.h"
#include "server_epoll.h"

static int g_epfd = 0;

int init_server_epoll(void)
{
    if (CHECK_FD(g_epfd)) {
        return 0;
    }

    g_epfd = epoll_create1(EPOLL_CLOEXEC);
    if (!CHECK_FD(g_epfd)) {
        lc_err_logout("create g_epfd error");
        return -1;
    }

    add_fd_to_srv_epoll(STDIN_FILENO, EPOLLIN | EPOLLET);

    return 0;
}

int destroy_server_epoll(void)
{
    if (!CHECK_FD(g_epfd)) {
        return 0;
    }

    return close(g_epfd);
}

int add_fd_to_srv_epoll(int fd, uint32_t events_mask)
{
    struct epoll_event event_ = {
        .data.fd = fd,
        .events = events_mask
    };

    if (!CHECK_FD(g_epfd)) {
        init_server_epoll();
    }

    return epoll_ctl(g_epfd, EPOLL_CTL_ADD, fd, &event_);
}

int del_fd_from_srv_epoll(int fd)
{
    if (!CHECK_FD(g_epfd)) {
        init_server_epoll();
    }

    return epoll_ctl(g_epfd, EPOLL_CTL_DEL, fd, NULL);
}