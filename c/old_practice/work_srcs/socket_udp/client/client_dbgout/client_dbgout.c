#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dbgout.h"
#include "pre_process.h"
#include "safe_api.h"

LogOp* GetClientLogFile(void)
{
    static LogOp logFile = {
        .logType = ALWAYS_OPEN,
        .logFp = NULL,
        .filePath = { 0 }
    };

    if (logFile.logFp == NULL) {
        char* s = LCget_current_dir_name();
        sprintf(logFile.filePath, "%s/logudpsrv.log", s);
        free(s);
    }

    return &logFile;
}