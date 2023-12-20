#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clientinfo_list.h"
#include "list.h"
#include "server_udp.h"

static CLisInfoList* g_clisInfoList = NULL;

int InitCliInfoList(void)
{
    if (g_clisInfoList)
        return 0;

    g_clisInfoList = (CLisInfoList*)malloc(sizeof(CLisInfoList));
    g_clisInfoList->infoList = (struct list_head*)malloc(sizeof(struct list_head));

    INIT_LIST_HEAD(g_clisInfoList->infoList);
    pthread_mutex_init(&g_clisInfoList->mutex, NULL);

    return 0;
}

int DestoryCliInfoList(void)
{
    ClisInfoListNode* listNode = NULL;

    pthread_mutex_lock(&g_clisInfoList->mutex);
    while (!list_empty(g_clisInfoList->infoList)) {
        listNode = list_entry(g_clisInfoList->infoList->next, ClisInfoListNode, list);
        list_del(&listNode->list);
        free(listNode);
    }
    free(g_clisInfoList->infoList);
    pthread_mutex_unlock(&g_clisInfoList->mutex);

    free(g_clisInfoList);

    return 0;
}

void AddNodeToCliInfoList(EndID cliID, struct sockaddr_in* _addr)
{
    ClisInfoListNode* listNode = NULL;

    listNode = (ClisInfoListNode*)malloc(sizeof(ClisInfoListNode));
    listNode->cliID = cliID;
    memcpy(&listNode->addr, _addr, sizeof(struct sockaddr_in));

    pthread_mutex_lock(&g_clisInfoList->mutex);
    list_add(&listNode->list, g_clisInfoList->infoList);
    pthread_mutex_unlock(&g_clisInfoList->mutex);
}

ClisInfoListNode* GetCliInfoNodeByCliID(EndID cliID)
{
    ClisInfoListNode* listNode = NULL;
    struct list_head* pos = NULL;

    pthread_mutex_lock(&g_clisInfoList->mutex);
    list_for_each(pos, g_clisInfoList->infoList)
    {
        listNode = list_entry(pos, ClisInfoListNode, list);
        if (listNode->cliID == cliID) {
            pthread_mutex_unlock(&g_clisInfoList->mutex);
            return listNode;
        }
    }
    pthread_mutex_unlock(&g_clisInfoList->mutex);

    return NULL;
}

void DelCliInfoNodeFormListByCliID(EndID cliID)
{
    ClisInfoListNode* listNode = NULL;

    listNode = GetCliInfoNodeByCliID(cliID);

    pthread_mutex_lock(&g_clisInfoList->mutex);
    list_del(&listNode->list);
    pthread_mutex_unlock(&g_clisInfoList->mutex);

    free(listNode);
}