#ifndef __SRC_DBGOUT_H_
#define __SRC_DBGOUT_H_

#include <stdio.h>

typedef enum LogType {
    ALWAYS_OPEN,
    OPEN_EVERY_TIME_WRITE
} LogType;

#define FILEPATH_LEN (128)
typedef struct LogOp {
    const LogType logType;
    FILE* logFp;
    char filePath[FILEPATH_LEN];
} LogOp;

void InitLogFile(void);
void _dbgout(LogOp*, const char*, ...);
const char* NodirStr(const char* str);

#define NOTDIR_STR(str) (strrchr(str, '/') != NULL ? (strrchr(str, '/') + 1) : str)

#define Dbgout(logOp, fmt, args...)                                                                              \
    do {                                                                                                         \
        _dbgout(logOp, "[file:%s, line:%u, func:%s]->" fmt, NodirStr(__FILE__), __LINE__, __FUNCTION__, ##args); \
    } while (0)

#endif