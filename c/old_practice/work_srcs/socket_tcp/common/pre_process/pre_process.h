#ifndef __PREPROCESS_PREDEF_H_
#define __PREPROCESS_PREDEF_H_

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#include "dbgout.h"

#define SERVER_IP_STR ("127.0.0.1")
#define SERVER_PORT (36666)

#define MAXEVENTS (200)
#define MAX_MSG_LEN (1024)

#define MAX_LINK_CNT (500)

typedef enum MsgType {
    PROCESS_MSG,
    SAVE_CLI_INFO,
    OTHER_MSGTYPE
} MsgType;

typedef pid_t EndID;
typedef struct MsgBuf {
    MsgType msgType;
    EndID srcEndID;
    EndID destEndID;
    unsigned char msg[MAX_MSG_LEN];
} MsgBuf;
#define SERVER_ENDID (0)

typedef enum MsgBufType {
    RX_BUF,
    TX_BUF
} MsgBufType;

typedef struct InitModules {
    LogOp* (*GetLogFileInfo)(void);
    EndID (*GetEndId)(void);
} InitModules;

InitModules* GetInitModules(void);
MsgBuf* GetMsgBuf(MsgBufType msgBufType);
int CloseFd(const char* info, int fd);

#define LocalDbgout(fmt, args...)                                \
    do {                                                         \
        Dbgout(GetInitModules()->GetLogFileInfo(), fmt, ##args); \
    } while (0)

#ifdef LOCAL_DEBUG
#define DebugDbgout(fmt, args...) LocalDbgout(fmt, ##args)
#else
#define DebugDbgout(fmt, args...)
#endif

#define DisplayStr(fmt, args...)                                        \
    do {                                                                \
        printf("%d-> " fmt "\n", GetInitModules()->GetEndId(), ##args); \
        fflush(stdout);                                                 \
    } while (0)

#define LCPRINTF(fmt, args...) \
    do {                       \
        printf(fmt, ##args);   \
        fflush(stdout);        \
    } while (0)

#define LCSCANF(fmt, args...) \
    do {                      \
        scanf(fmt, ##args);   \
        fflush(stdout);       \
    } while (0)

#endif