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
TDATA priority_queue[QUEUE_SIZE]; // each priority has its own slot
int record_cnt = RECORD_CNT;

int search_for_highest_priority(void);
void main_exec(TDATA* arg);

void main(void)
{
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
  
    memset(priority_queue, 0, sizeof(TDATA) * QUEUE_SIZE);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp_start);
    memset(&shared_data, 0, sizeof(TDATA) * CLIENT_CNT);
  
    for (int client_id = 0; client_id < CLIENT_CNT; client_id++)
    {
        client_init(&shared_data[client_id], client_id);
    }
  
    while (record_cnt > 0)
    {
        client_exec(&shared_data[client_counter]);
        logger_exec();
        client_counter = ++client_counter % CLIENT_CNT;
    }  
 
    printf("%li [us] - Finished\n", time_stamp.tv_nsec/1000);
    
    fflush(pFile);
    fclose(pFile);
    return;
}

int search_for_highest_priority(void)
{
    int i;
    int min = 255;
    for (i = 0; i < CLIENT_CNT; i++)
    {
        if (shared_data[i].valid == 1)
        {
            if (shared_data[i].cPriority < min)
            {
                min = shared_data[i].cPriority;
            }
        }
    }
  
    return i; 
}

void main_exec(TDATA* arg)
{
    if (record_cnt >= 0)
    {
        if (arg->valid)
        {
            if (priority_queue[arg->cPriority].valid == 0)
            {
                printf("m: %d %li %d\n", record_cnt, arg->dwClientId, arg->cPriority);
                memcpy(&priority_queue[arg->cPriority], arg, sizeof(TDATA)); // TODO: Data is empty
                client_invalidate(arg);
                record_cnt--;
            }
        }            
    }
}



