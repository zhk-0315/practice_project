#ifndef CLIENT__CLIENT_UDP__CLIENT_UDP_H
#define CLIENT__CLIENT_UDP__CLIENT_UDP_H

#include <netinet/in.h>

typedef struct CliUdp {
    int udpFd;
    const socklen_t addrlen;
    struct sockaddr_in lcAddr;
    struct sockaddr_in srvAddr;
} CliUdp;

CliUdp* GetCliUdp(void);
int InitClientUdp(void);
int DestoryCliUdp(void);
int CreateRecvServerMsgThread(void);
int SendMsgToServer(void);
int SendMsgToOtherClient(void);
int InitClientInfoToServer(void);

#endif