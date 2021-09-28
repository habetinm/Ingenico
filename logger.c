#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "data.h"
#include "logger.h"

extern FILE* pFile;
extern TDATA priority_queue[QUEUE_SIZE];
extern int record_cnt;
int queue_idx = 0;


void logger_exec(void)
{
    struct timespec time_stamp;
    char str[256];
  
    if (record_cnt >= 0)
    {
        //for (int i = 0; i < QUEUE_SIZE; i++)
        {
            if (priority_queue[queue_idx].valid == 1)
            {
                clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp);
                sprintf(str, 
                        "log time: %lu [us], cli id: %lu, pri: %3d, org time: %lu [us]\n", 
                        time_stamp.tv_nsec/1000, 
                        priority_queue[queue_idx].dwClientId, 
                        priority_queue[queue_idx].cPriority, 
                        priority_queue[queue_idx].dwTicks);
                
                printf("%s", str);
                
                fprintf(pFile, "%s", str);
                memset(&priority_queue[queue_idx], 0, sizeof(TDATA));
                queue_idx = ++queue_idx % QUEUE_SIZE;
                --record_cnt;
                
                printf("-- %d %d\n", queue_idx, record_cnt);
            }
        }
        
        //fflush(pFile);        
    }
}
