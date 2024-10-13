#ifndef __INCLUDE_PREDEF_H_
#define __INCLUDE_PREDEF_H_

#include "dbgout.h"

#define RET_ERROR (-1)
typedef enum {
    QUEUE_SUCCESS = 0,
    QUEUE_FULL,
    QUEUE_EMPTY
} QUEUE_RET_VALUE;

LogOp* GetLogOp(void);
#define QueueDbgout(fmt, args...) Dbgout(GetLogOp(), fmt, ##args)

#endif