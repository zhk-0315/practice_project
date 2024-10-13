#include "client_dbgout.h"
#include <stdio.h>
#include <unistd.h>

LogOp* GetTcpClientLogFileInfo(void)
{
    static LogOp logOp = {
        .logType = ALWAYS_OPEN,
        .filePath = { 0 },
        .logFp = NULL
    };

    if (logOp.logFp == NULL) {
        sprintf(logOp.filePath, "/data/zkdata/local/zklog/TcpClient.log");
    }

    return &logOp;
}
