#include "sock_msg.h"
#include "pre_modules.h"

void* dispaly_msg_package(void* arg)
{
    int i = 0;
    char out[512] = { 0 };
    char str[8] = { 0 };
    lc_msg_package_t* msgbuf_ = (lc_msg_package_t*)arg;

    sprintf(out, "info from id(%d) to id(%d) is: ",
        msgbuf_->msg.srcid, msgbuf_->msg.destid);
    for (i = 0; i < 50; i++) {
        sprintf(str, "0x%x ", msgbuf_->msg.data[i]);
        strcat(out, str);
    }
    strcat(out, "\n");

    lc_printf("%s", out);
    lc_logout("%s", out);

    return 0;
}