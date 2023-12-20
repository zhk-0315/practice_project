#include <stdio.h>
#include <string.h>

#include "dbgout.h"
#include "pre_process.h"

LogOp* GetServerLogFile(void)
{
    static LogOp logFile = {
        .logType = ALWAYS_OPEN,
        .logFp = NULL,
        .filePath = { 0 }
    };

    if (logFile.logFp == NULL) {
        char* s = "/data/zkdata/local/zklog/udpsrv.log";
        strcpy(logFile.filePath, s);
    }

    return &logFile;
}