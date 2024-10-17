#include <stdio.h>

static char *path = NULL;

void _dbgout()
{
    static FILE *dbg_fp = NULL;
    va_list ap;

    if (dbg_fp && path) {
        dbg_fp =fopen(path, "w+");
    }

    if (!dbg_fp) {
        return;
    }

    va_start(ap, param)



}


