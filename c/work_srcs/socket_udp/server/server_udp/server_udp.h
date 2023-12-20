#ifndef SERVER__SERVER_UDP__SERVER_UDP_H
#define SERVER__SERVER_UDP__SERVER_UDP_H

#include <netinet/in.h>

typedef struct SrvUdp {
    int udpFd;
    socklen_t addrlen;
    struct sockaddr_in LcAddr;
} SrvUdp;

SrvUdp* GetSrvUdp(void);
int InitSrvUdp(void);
int DestorySrvUdp(void);

#endif