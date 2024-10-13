#ifndef SERVER__SERVER_UDP__SERVER_UDP_H
#define SERVER__SERVER_UDP__SERVER_UDP_H

#include <netinet/in.h>

typedef struct SrvUdp {
    int udpFd;
    const socklen_t addrlen;
    struct sockaddr_in LcAddr;
} SrvUdp;

SrvUdp* GetSrvUdp(void);
int InitSrvUdp(void);
int DestorySrvUdp(void);
void* ProcessStdin(void* arg);
void* PorcessUdpMsg(void* arg);
int CreateSrvMsgProcessThread(void);

#endif