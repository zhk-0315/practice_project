#include "server_dbgout.h"

LogOp* GetTcpServerLogFileInfo(void)
{
    static LogOp logOp = {
        .logType = ALWAYS_OPEN,
        .filePath = "/data/zkdata/local/zklog/TcpServer.log",
        .logFp = NULL
    };

    return &logOp;
}
