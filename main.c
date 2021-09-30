#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "data.h"
#include "client.h"
#include "logger.h"

FILE* pFile;
TDATA shared_data[CLIENT_CNT];
int record_cnt = RECORD_CNT;

void main_exec(TDATA* arg);

void main(void)
{
    unsigned char do_logging = 0;
    int client_counter = 0;
    struct timespec time_stamp_start, time_stamp;
    int highest_priority_idx = 0;
    int head_idx = 0, tail_idx = 0, free_idx = 0;

    pFile = fopen("./log.txt", "w+");
    if (pFile == NULL)
    {
        printf("log file create failed\n");
        return;
    }
  
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp_start);
    memset(&shared_data, 0, sizeof(TDATA) * CLIENT_CNT);
  
    for (int client_id = 0; client_id < CLIENT_CNT; client_id++)
    {
        client_init(&shared_data[client_id], client_id);
    }
  
    while (record_cnt > 0)
    {
        client_exec(&shared_data[client_counter]);
        main_exec(&shared_data[client_counter]);
        
        if (do_logging)
        {
            logger_exec();
        }
        do_logging = 1;

        client_counter = ++client_counter % CLIENT_CNT;
    }  
 
    printf("%li [us] - Finished\n", time_stamp.tv_nsec/1000);
    
    fflush(pFile);
    fclose(pFile);
    return;
}

TDATA tmp;
void main_exec(TDATA* arg)
{
    //static TDATA tmp;
    static unsigned long last_cli_id;
    
    if (arg->dwClientId >= 1)
    { 
        last_cli_id = (arg->dwClientId - 1);
    }
    else // arg->dwClientId == 0
    {
        last_cli_id = CLIENT_CNT - 1;
    }
    
    if (shared_data[last_cli_id].valid)
    {
        if (arg->dwTicks == shared_data[last_cli_id].dwTicks)
        {
            if (arg->cPriority < shared_data[last_cli_id].cPriority)
            {
                // swap
                printf("%li %d %li, %li %d %li\n", 
                       shared_data[last_cli_id+1].dwClientId, 
                       shared_data[last_cli_id+1].cPriority, 
                       shared_data[last_cli_id+1].dwTicks, 
                       shared_data[last_cli_id].dwClientId, 
                       shared_data[last_cli_id].cPriority, 
                       shared_data[last_cli_id].dwTicks); 
                
                //memcpy(&tmp, arg, sizeof(TDATA));
                //memcpy(arg, &shared_data[last_cli_id], sizeof(TDATA));
                //memcpy(&shared_data[last_cli_id], &tmp, sizeof(TDATA));
                
                memcpy(&tmp, &shared_data[last_cli_id], sizeof(TDATA));
                memcpy(&shared_data[last_cli_id], arg, sizeof(TDATA));
                memcpy(arg, &tmp, sizeof(TDATA));

                //tmp = *arg;
                //shared_data[last_cli_id] = *arg;
                //*arg = tmp;
                
                printf("%li %d %li, %li %d %li\n", 
                       shared_data[last_cli_id+1].dwClientId, 
                       shared_data[last_cli_id+1].cPriority, 
                       shared_data[last_cli_id+1].dwTicks, 
                       shared_data[last_cli_id].dwClientId, 
                       shared_data[last_cli_id].cPriority, 
                       shared_data[last_cli_id].dwTicks); 
                
                printf("swp: %li\n", arg->dwTicks);
            }
        }
    }
}


