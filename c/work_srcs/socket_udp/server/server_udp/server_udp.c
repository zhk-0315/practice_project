#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "pre_process.h"
#include "server_lcmsgqueue.h"
#include "server_udp.h"

SrvUdp* GetSrvUdp(void)
{
    static SrvUdp udp = {
        .addrlen = sizeof(struct sockaddr),
    };

    return &udp;
}

int InitSrvUdp(void)
{
    int udpfd = -1;
    int sRet = 0;
    const int ov = 1;
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
    sRet = bind(udpfd, (const struct sockaddr*)&LcAddr, GetSrvUdp()->addrlen);
    CHECK_FUNCRET_ERR(sRet, -1, "setsockopt udpfd SO_REUSEADDR error");

    GetSrvUdp()->udpFd = udpfd;
    GetSrvUdp()->LcAddr = LcAddr;

    return 0;
}

int DestorySrvUdp(void)
{
    if (!FD_CHECK(GetSrvUdp()->udpFd))
        return -1;

    return close(GetSrvUdp()->udpFd);
}

void* ProcessStdin(void* arg)
{
    size_t RxSize = 0;
    int fd = *(int*)arg;
    LcMsg MsgBuf = { 0 };

    InitTxBuf(&MsgBuf, SETVER_ENDID);
    RxSize = read(fd, MsgBuf.msg.data, LCMSG_DATA_MAXLEN);
    if (RxSize == -1) {
        LocalDbgout("read fd(%d) error!", fd);
        return NULL;
    } else if (RxSize == 0) {
        return arg;
    }

    EnSrvMsgQueue(&MsgBuf);

    return arg;
}

void* PorcessUdpMsg(void* arg)
{
    size_t RxSize = 0;
    int fd = *(int*)arg;
    LcMsg MsgBuf = { 0 };
    struct sockaddr_in sockaddr = { 0 };

    RxSize = recvfrom(fd, &MsgBuf, sizeof(LcMsg), 0, (struct sockaddr*)&sockaddr,
        (socklen_t*)&GetSrvUdp()->addrlen);
    if (RxSize == -1) {
        LocalDbgout("recvfrom fd(%d) error!", fd);
        return NULL;
    } else if (RxSize == 0) {
        return arg;
    }

    return arg;
}