#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "data.h"
#include "client.h"

extern TDATA shared_data[CLIENT_CNT];
int message_cnt = RECORD_CNT;
char ClientData[MAX_MSG_SIZE];
struct timespec time_stamp;

void client_init(TDATA* arg, int client_id)
{
    client_invalidate(arg);
    arg->dwClientId = (unsigned long)client_id;
}

void client_invalidate(TDATA* arg)
{
    arg->cPriority = 255;
    arg->dwTicks = 0;
    memset(&arg->Data[0], 0, MAX_MSG_SIZE);
    arg->valid = 0;
}

void client_exec(TDATA* arg)
{
    //printf("c: %d, %li, %d\n", message_cnt, arg->dwClientId, arg->cPriority);
            
    if (message_cnt >= 0)
    {
        if (arg->valid == 0)
        {
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp);
            arg->dwTicks   = time_stamp.tv_nsec/1000;
            arg->cPriority = random() % (MIN_MSG_PRIORITY + 1);
            arg->valid     = 1;
            strncpy(arg->Data, ClientData, MAX_MSG_SIZE);
            
            printf("c: %d, %li, %d\n", message_cnt, arg->dwClientId, arg->cPriority);
            
            --message_cnt;
        }
    }
}
