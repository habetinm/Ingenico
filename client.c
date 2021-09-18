#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "data.h"
#include "client.h"

extern pthread_mutex_t mutexes[THREAD_CNT];
extern TDATA shared_data[THREAD_CNT];
extern int record_cnt;

void* client_func(void* arg)
{
  char ClientData[MAX_MSG_SIZE];
  struct timespec time_stamp;
  int cli_id = (int)pthread_self();
  int my_idx;
  
  for (int i = 0; i < THREAD_CNT; i++)
  {
    if (shared_data[i].dwClientId == cli_id)
    {
      my_idx = i;
      break;
    }
  }
  
  while (record_cnt >= 0)
  {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp);
  
    pthread_mutex_lock(&mutexes[my_idx]);
  
    if (shared_data[my_idx].valid == 0)
    {
      shared_data[my_idx].dwTicks = time_stamp.tv_nsec/1000;
      shared_data[my_idx].dwClientId = my_idx; //cli_id;
      shared_data[my_idx].cPriority = random() % (MIN_MSG_PRIORITY + 1);
      shared_data[my_idx].valid = 1;
      strncpy(shared_data[my_idx].Data, ClientData, MAX_MSG_SIZE);
    }
    
    pthread_mutex_unlock(&mutexes[my_idx]);
    
    printf("client [%d]: %u created\n", my_idx, my_idx); 
    sleep(1);
  }
  
  shared_data[my_idx].dwClientId = INVALID_CLIENT_ID;
  
  pthread_exit((void*)0);
}
