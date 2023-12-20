#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "pre_process.h"
#include "server_lcmsgqueue.h"
#include "server_udp.h"

SrvUdp* GetSrvUdp(void)
{
    static SrvUdp udp = { 0 };

    return &udp;
}

int InitSrvUdp(void)
{
    int udpfd = -1;
    int sRet = 0;
    const int ov = 1;
    socklen_t addrlen = sizeof(struct sockaddr);
    struct sockaddr_in LcAddr = { 0 };

    if (FD_CHECK(GetSrvUdp()->udpFd))
        return -1;

    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    CHECK_FUNCRET_ERR(udpfd, -1, "socket udpfd error");

    sRet = setsockopt(udpfd, SOL_SOCKET, SO_REUSEADDR, &ov, sizeof(ov));
    CHECK_FUNCRET_ERR(sRet, -1, "setsockopt udpfd SO_REUSEADDR error");

    LcAddr.sin_family = AF_INET;
    LcAddr.sin_addr.s_addr = INADDR_ANY;
    LcAddr.sin_port = htons(SERVER_PORT);
    sRet = bind(udpfd, (const struct sockaddr*)&LcAddr, addrlen);
    CHECK_FUNCRET_ERR(sRet, -1, "setsockopt udpfd SO_REUSEADDR error");

    GetSrvUdp()->udpFd = udpfd;
    GetSrvUdp()->LcAddr = LcAddr;
    GetSrvUdp()->addrlen = addrlen;

    return 0;
}

int DestorySrvUdp(void)
{
    if (!FD_CHECK(GetSrvUdp()->udpFd))
        return -1;

    return close(GetSrvUdp()->udpFd);
}

static void* ProcessStdin(void* arg)
{
    size_t RxSize = 0;
    int fd = *(int*)arg;
    unsigned char RxData[LCMSG_DATA_MAXLEN] = { 0 };

    RxSize = read(fd, RxData, LCMSG_DATA_MAXLEN);
    if (RxSize == -1) {
        LocalDbgout("read fd(%d) error!", fd);
        return NULL;
    } else if (RxSize == 0) {
        return arg;
    }
    
    return arg;
}

static void* PorcessUdpMsg(void* arg)
{
    return arg;
}