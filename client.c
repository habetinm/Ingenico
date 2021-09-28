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

void client_init(int client_id)
{
    if (client_id >= 0 && client_id < CLIENT_CNT)
    {
        client_invalidate(&shared_data[client_id]);
        shared_data[client_id].dwClientId = (unsigned long)client_id;
    }
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
    if (message_cnt >= 0)
    {
        if (arg->valid == 0)
        {
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp);
    
            arg->dwTicks = time_stamp.tv_nsec/1000;
            arg->cPriority = random() % (MIN_MSG_PRIORITY + 1);
            arg->valid = 1;
            strncpy(arg->Data, ClientData, MAX_MSG_SIZE);
            printf("++ %li\n", arg->dwClientId);
            
            --message_cnt;
        }
    }
}
