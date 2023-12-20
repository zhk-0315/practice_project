#ifndef SERVER__CLIENTINFO_LIST__CLIENTINFO_LIST_H
#define SERVER__CLIENTINFO_LIST__CLIENTINFO_LIST_H

#include <netinet/in.h>

#include "lcmsg.h"
#include "list.h"

typedef struct ClisInfoListNode {
    EndID cliID;
    struct sockaddr_in addr;
    struct list_head list;
} ClisInfoListNode;

typedef struct CLisInfoList {
    struct list_head* infoList;
    pthread_mutex_t mutex;
} CLisInfoList;

int DestoryCliInfoList(void);
void AddNodeToCliInfoList(EndID cliID, struct sockaddr_in* _addr);
ClisInfoListNode* GetCliInfoNodeByCliID(EndID cliID);
void DelCliInfoNodeFormListByCliID(EndID cliID);

#endif