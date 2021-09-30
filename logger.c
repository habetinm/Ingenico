#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "data.h"
#include "logger.h"
#include "client.h"


extern FILE* pFile;
extern TDATA shared_data[CLIENT_CNT];
extern int record_cnt;
int queue_idx = 0;
struct timespec time_stamp;
unsigned char do_logging = 0;

void logger_exec(void)
{
    static char str[256];
    static unsigned long last_cli_id;
    
    if (do_logging)
    {
        if (record_cnt > 0)
        {
            if (shared_data[queue_idx].valid == 1)
            {
                clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp);
                sprintf(str, 
                        "log time: %lu [us], cli id: %lu, pri: %3d, org time: %lu [us]\n", 
                        time_stamp.tv_nsec/1000, 
                        shared_data[queue_idx].dwClientId, 
                        shared_data[queue_idx].cPriority, 
                        shared_data[queue_idx].dwTicks);
                    
                fprintf(pFile, "%s", str);
                --record_cnt;
            }
                
            if (queue_idx >= 1)
            { 
                last_cli_id = (queue_idx - 1);
            }
            else // queue_idx == 0
            {
                last_cli_id = CLIENT_CNT - 1;
            }        
            
            client_invalidate(&shared_data[last_cli_id]);            
                
            queue_idx = ++queue_idx % QUEUE_SIZE;
        }
    }
    
    do_logging = 1;
}
