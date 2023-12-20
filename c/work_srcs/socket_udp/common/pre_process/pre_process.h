#ifndef COMMON__PRE_PROCESS__PRE_PROCESS_H
#define COMMON__PRE_PROCESS__PRE_PROCESS_H

#include <errno.h>
#include <stdbool.h>
#include <string.h>

#include "dbgout.h"
#include "lcmsg.h"

typedef struct ModulesManager {
    EndID LcEndID;
    //
    LogOp* (*GetLogFile)(void);
} ModulesManager;

ModulesManager* ToSetModulesManager(void);
const ModulesManager* GetModulesManager(void);

#if 1
#define LocalDbgout(fmt, args...)                               \
    do {                                                        \
        Dbgout(GetModulesManager()->GetLogFile(), fmt, ##args); \
    } while (0)
#else
#define LocalDbgout(fmt, args...)
#endif

#ifdef LOCAL_DEBUG
#define DebugDgbout LocalDbgout
#else
#define DebugDgbout(fmt, args...)
#endif

#define LCprintf(fmt, args...) \
    do {                       \
        printf(fmt, ##args);   \
        fflush(stdout);        \
    } while (0)

#define LCscanf(fmt, args...)         \
    do {                              \
        scanf(fmt, ##args);           \
        while (getchar() != '\n') { } \
    } while (0)

#define DisplayStr(fmt, args...)    \
    do {                            \
        LCprintf(fmt "\n", ##args); \
    } while (0)

#define LCclear()                  \
    do {                           \
        LCprintf("\033[H\033[2J"); \
    } while (0)

#define CHECK_FUNCRET_ERR(ret, errorRet, errorInfo)         \
    ({                                                      \
        bool isError = false;                               \
        if (ret == errorRet) {                              \
            LocalDbgout(errorInfo ": %s", strerror(errno)); \
            isError = true;                                 \
        } else {                                            \
            isError = false;                                \
        }                                                   \
        isError;                                            \
    })

#define CHECK_FUNCRET_SUC(ret, successRet, errorInfo)       \
    ({                                                      \
        bool isError = false;                               \
        if (ret != successRet) {                            \
            LocalDbgout(errorInfo ": %s", strerror(errno)); \
            isError = true;                                 \
        } else {                                            \
            isError = false;                                \
        }                                                   \
        isError;                                            \
    })

#define SERVER_IP ("127.0.0.1")
#define SERVER_PORT (35666)
#define SETVER_ENDID (0)

#define FD_CHECK(fd) (fd > 2)

#endif