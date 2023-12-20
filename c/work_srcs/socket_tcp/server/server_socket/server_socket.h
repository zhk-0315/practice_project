#ifndef __SERVER_SOCKET_SERVER_SOCKET_H_
#define __SERVER_SOCKET_SERVER_SOCKET_H_

#include <arpa/inet.h>

int GetServerFd(void);
socklen_t GetAddrlen(void);
int InitServerSock(void);
int AcceptNewClient(void);
int BroadcastAllClient(int);
int InitMsgBuf(void);
int ClientMsgEnQueue(int fd);
int InitServerMsgBufInfo(void);
int CreateProcessCliMsgThread(void);

#endif