#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "pre_modules.h"

static sqlite3* lc_sql = NULL;

int create_cli_sql_database(void)
{
    int iret = 0;
    char fpath[256] = { 0 };
    char sql_err[256] = { 0 };
    char sql_cmd[256] = { 0 };

    if (!lc_sql) {
        return 0;
    }

    getcwd(fpath, sizeof(fpath));
    strcat(fpath, "cli_info_sql.db");

    iret = sqlite3_open(fpath, &lc_sql);
    if (iret != SQLITE_OK) {
        lc_logout("sqlite3_open cli_info_sql.db error");
    }

    sprintf(sql_cmd, "CREATE TABLE IF NOT EXISTS INFO(endid int )");
    iret = sqlite3_exec(lc_sql, sql_cmd, NULL, NULL, (char**)&sql_err);
    if (iret != SQLITE_OK) {
        lc_logout("sqlite3_open cli_info_sql.db error");
    }

    return 0;
}