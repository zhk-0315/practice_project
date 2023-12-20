#ifndef SERVER__CLIENTINFO_LIST__CLIENTINFO_LIST_H
#define SERVER__CLIENTINFO_LIST__CLIENTINFO_LIST_H

#include <netinet/in.h>

#include "lcmsg.h"
#include "list.h"

typedef struct CliInfoListNode {
    EndID cliID;
    struct sockaddr_in addr;
    struct list_head list;
} CliInfoListNode;

int DestoryCliInfoList(void);
void AddNodeToCliInfoList(EndID cliID, struct sockaddr_in* _addr);
CliInfoListNode* GetCliInfoNodeByCliID(EndID cliID);
void DelCliInfoNodeFormListByCliID(EndID cliID);

#endif