#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clientinfo_list.h"
#include "list.h"
#include "server_udp.h"

static LIST_HEAD(g_cliInfoList);

int DestoryCliInfoList(void)
{
    CliInfoListNode* listNode = NULL;

    while (!list_empty(&g_cliInfoList)) {
        listNode = list_entry(g_cliInfoList.next, CliInfoListNode, list);
        list_del(&listNode->list);
        free(listNode);
    }

    return 0;
}

void AddNodeToCliInfoList(EndID cliID, struct sockaddr_in* _addr)
{
    CliInfoListNode* listNode = NULL;

    listNode = (CliInfoListNode*)malloc(sizeof(CliInfoListNode));
    listNode->cliID = cliID;
    memcpy(&listNode->addr, _addr, sizeof(struct sockaddr_in));

    list_add(&listNode->list, &g_cliInfoList);
}

CliInfoListNode* GetCliInfoNodeByCliID(EndID cliID)
{
    CliInfoListNode* listNode = NULL;
    struct list_head* pos = NULL;

    list_for_each(pos, &g_cliInfoList)
    {
        listNode = list_entry(pos, CliInfoListNode, list);
        if (listNode->cliID == cliID)
            return listNode;
    }

    return NULL;
}

void DelCliInfoNodeFormListByCliID(EndID cliID)
{
    CliInfoListNode* listNode = NULL;

    listNode = GetCliInfoNodeByCliID(cliID);

    list_del(&listNode->list);
    free(listNode);
}