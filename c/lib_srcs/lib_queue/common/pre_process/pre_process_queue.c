#include <stdio.h>

#include "dbgout.h"

LogOp* GetLogOp(void)
{
    static LogOp logOp_ = {
        .logType = ALWAYS_OPEN,
        .filePath = "/data/zkdata/local/zklog/queue.log",
        .logFp = NULL
    };

    return &logOp_;
}