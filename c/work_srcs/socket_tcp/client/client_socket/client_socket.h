#ifndef __CLIENT_SOCKET_CLIENT_SOCKET_H_
#define __CLIENT_SOCKET_CLIENT_SOCKET_H_

#include <arpa/inet.h>

int GetClientFd(void);
socklen_t GetAddrLen(void);
int InitClientSock(void);
int RxFromServer(void);
int InitMsgBuf(void);
int CreateConnectServerThread(void);
int CreateMsgQueueProcessThread(void);
int TxToServer(void);
int TxToOtherCli(void);
int InitCliInfoToServer(void);

#endif