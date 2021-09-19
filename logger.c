#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "data.h"
#include "logger.h"

extern FILE* pFile;
extern pthread_mutex_t logger_mutex[QUEUE_SIZE];
extern TDATA priority_queue[QUEUE_SIZE];
extern int record_cnt;

void* logger_func(void* arg)
{
  struct timespec time_stamp;
  char str[256];
    
  while (record_cnt >= 0)
  {
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
      if (pthread_mutex_trylock(&logger_mutex[i]) == 0)
      {
        if (priority_queue[i].valid == 1)
        {
          clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp);
          sprintf(str, "log time: %lu [us], cli id: %lu, pri: %3d, org time: %lu [us]\n", time_stamp.tv_nsec/1000, priority_queue[i].dwClientId, priority_queue[i].cPriority, priority_queue[i].dwTicks);
          printf("%s", str);
          fprintf(pFile, "%s", str);
          memset(&priority_queue[i], 0, sizeof(TDATA));
        }
        
        pthread_mutex_unlock(&logger_mutex[i]);
      }
    }
    
    fflush(pFile);
    //sleep(5);
  }
  
  fflush(pFile);
  pthread_exit((void*)0);
}
