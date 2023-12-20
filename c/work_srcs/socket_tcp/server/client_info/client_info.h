#ifndef __CLIENT_INFO_CLIENT_INFO_H_
#define __CLIENT_INFO_CLIENT_INFO_H_

#include <netinet/in.h>
#include <stdbool.h>

#include "list.h"
#include "pre_process.h"

typedef struct CliInfoNode {
    int fd;
    EndID cliEndid;
    struct sockaddr_in addr;
    struct list_head list;
} CliInfoNode;

struct list_head* GetCliInfoManager(void);
int AddToCliInfo(CliInfoNode*);
int DelFromCliInfoByFd(int fd);
CliInfoNode* GetOneCliInfoByFd(int fd);
int SaveOneCliInfoByFd(int fd, MsgBuf* msgBuf);
int SendInitInfoToCliByFd(int fd);
int InitCliInfoManager(void);
int SendMsgAllClient(const MsgBuf* msg);
const char* GetCliInfoErrStr(CliInfoNode* cliInfo);
CliInfoNode* GetOneCliInfoByEndID(EndID cliEndid);

#endif