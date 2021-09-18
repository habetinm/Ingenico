#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "data.h"
#include "client.h"
#include "logger.h"

FILE* pFile;
pthread_mutex_t mutexes[THREAD_CNT];
pthread_mutex_t logger_mutex[QUEUE_SIZE];
pthread_t thread_id[THREAD_CNT];
pthread_t logger_id;
//TDATA queue[QUEUE_SIZE];
TDATA shared_data[THREAD_CNT];
TDATA priority_queue[QUEUE_SIZE]; // each priority has its own slot
int data;
int record_cnt = RECORD_CNT;

int search_for_highest_priority(void);
void invalidate_data(int idx);

void main(void)
{
  TDATA higest_priority_data;
  struct timespec time_stamp_start, time_stamp;
  void* status;
  int highest_priority_idx = 0;
  int head_idx = 0, tail_idx = 0, free_idx = 0;

  pFile = fopen("./log.txt", "w+");
  if (pFile == NULL)
  {
    printf("log create failed\n");
  }
  
  memset(priority_queue, 0, sizeof(TDATA) * QUEUE_SIZE);
  memset(&higest_priority_data, 0, sizeof(TDATA));
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp_start);
  data = 0;
  memset(&shared_data, 0, sizeof(TDATA) * THREAD_CNT);
  
  
  
  for (int i = 0; i < THREAD_CNT; i++)
  {
    invalidate_data(i);
    pthread_mutex_init(&mutexes[i], NULL);
    pthread_create(&thread_id[i], NULL, client_func, (void*)data++);
    shared_data[i].dwClientId = (unsigned long)thread_id[i];
  }
  
  for (int i = 0; i < QUEUE_SIZE; i++)
  {
    pthread_mutex_init(&logger_mutex[i], NULL);
  }
  
  pthread_create(&logger_id, NULL, logger_func, NULL);
  
  
  
  while (record_cnt >= 0)
  {
    for (int i = 0; i < THREAD_CNT; i++)
    {
      if (pthread_mutex_trylock(&mutexes[i]) == 0)
      {
        if (priority_queue[shared_data[i].cPriority].valid == 0)
        {
          if (shared_data[i].valid)
          {
            //printf("> %d, %d\n", i, shared_data[i].cPriority);
            pthread_mutex_lock(&logger_mutex[shared_data[i].cPriority]);
            //printf("<<<\n");
            memcpy(&priority_queue[shared_data[i].cPriority], &shared_data[i], sizeof(TDATA));
            pthread_mutex_unlock(&logger_mutex[shared_data[i].cPriority]);
            
            printf("main: [%3d] - client id: %lu enqued %d\n", shared_data[i].cPriority, shared_data[i].dwClientId, record_cnt);
            
            invalidate_data(i);
            record_cnt--;
          }
            
          //pthread_mutex_unlock(&mutexes[i]);
        }
        
        pthread_mutex_unlock(&mutexes[i]);
      }
      
      //printf("main: %d, %d\n", i, j[i]);
      sleep(1);
    }
  }
/*
  for (int i = 0; i < THREAD_CNT; i++)
  {
    pthread_join(thread_id[i], &status);
  }
  
  pthread_join(logger_id, &status);
*/  
  printf("%li [us] - Konec\n", time_stamp.tv_nsec/1000);
  
  for (int i = 0; i < THREAD_CNT; i++)
  {
    pthread_mutex_destroy(&mutexes[i]);
  }
  
  for (int i = 0; i < QUEUE_SIZE; i++)
  {
    pthread_mutex_destroy(&logger_mutex[i]);
  }
  
  fclose(pFile);
  pthread_exit((void*)0);
}

int search_for_highest_priority(void)
{
  int i;
  int min = 255;
  for (i = 0; i < THREAD_CNT; i++)
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

void invalidate_data(int idx)
{
  shared_data[idx].cPriority = 255;
  shared_data[idx].dwTicks = 0;
  memset(&shared_data[idx].Data[0], 0, MAX_MSG_SIZE);
  shared_data[idx].valid = 0;
}

/*
Hi Milan,

Let imagine case with 2 clients and 1 server.
1st client has ID 12345. 2nd client has ID 99999.

Suppose that priority 3  is bigger 10 means 3 go first, than 10 if ticks is same.
Ticks - time of message generation.

Example of execution 2 clients. Priority for each message is generated randomly.
1. 1st client generate request. tagTDATA {cPriority = 10, dwTicks = 1, dwClientId = 12345, Data = "ABCDEFG....ANY DATA TO SIMULATE"}; = Msg1
QUEUE = {Msg1}
2. 1st client generate request. tagTDATA {cPriority = 5, dwTicks = 10, dwClientId = 12345, Data = "ABCDEFG....ANY DATA TO SIMULATE"}; = Msg2
QUEUE = { Msg2, Msg1}
3. 2st client generate request. tagTDATA {cPriority = 3, dwTicks = 10, dwClientId = 99999, Data = "ABCDEFG....ANY DATA TO SIMULATE"}; = Msg3
QUEUE = { Msg3, Msg2, Msg1}
4. 1st client generate request. tagTDATA {cPriority = 5, dwTicks = 63, dwClientId = 12345, Data = "ABCDEFG....ANY DATA TO SIMULATE"}; = Msg4
QUEUE = { Msg3, Msg2, Msg4, Msg1}
5. Server takes first msg: Msg3 and write to log dwTicks_writeLog = 64
QUEUE = { Msg2, Msg4, Msg1}
6. Server takes first msg: Msg2 and write to log dwTicks_writeLog = 65
QUEUE = { Msg4, Msg1}
7. 2st client generate request. tagTDATA {cPriority = 3, dwTicks = 10, dwClientId = 99999, Data = "ABCDEFG....ANY DATA TO SIMULATE"}; = Msg5
QUEUE = { Msg5, Msg4, Msg1}
...... <-- Simulations
N. Stop Simulations
After that think how to better implement it adding, getting from Queue, write log and stopping simulations.

Let me know if you have any questions.

-----------------
Best regards,
Maksym MAKARYCHEV
*/
