#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "data.h"
#include "client.h"

extern TDATA shared_data[CLIENT_CNT];
extern int record_cnt;
char ClientData[MAX_MSG_SIZE];
struct timespec time_stamp;

void client_init(int client_id)
{
    if (client_id >= 0 && client_id < CLIENT_CNT)
    {
        invalidate_data(client_id);
        shared_data[client_id].dwClientId = (unsigned long)client_id;
    }
}

void invalidate_data(int idx)
{
  //shared_data[idx].dwClientId = (unsigned long)idx;
  shared_data[idx].cPriority = 255;
  shared_data[idx].dwTicks = 0;
  memset(&shared_data[idx].Data[0], 0, MAX_MSG_SIZE);
  shared_data[idx].valid = 0;
}

void* client_func(void* arg)
{
/*    
  char ClientData[MAX_MSG_SIZE];
  struct timespec time_stamp;
  int my_idx;
  
  for (int i = 0; i < CLIENT_CNT; i++)
  {
    if (shared_data[i].dwClientId == 0xFFFF)
    {
      my_idx = i;
      break;
    }
  }
  
  while (record_cnt >= 0)
  {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp);
  
    if (shared_data[my_idx].valid == 0)
    {
      shared_data[my_idx].dwTicks = time_stamp.tv_nsec/1000;
      shared_data[my_idx].dwClientId = my_idx; //cli_id;
      shared_data[my_idx].cPriority = random() % (MIN_MSG_PRIORITY + 1);
      shared_data[my_idx].valid = 1;
      strncpy(shared_data[my_idx].Data, ClientData, MAX_MSG_SIZE);
    }
  }
  
  shared_data[my_idx].dwClientId = INVALID_CLIENT_ID;
*/  
}

void client_exec(TDATA* arg)
{
    if (arg->valid == 0)
    {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp);
  
        arg->dwTicks = time_stamp.tv_nsec/1000;
        arg->cPriority = random() % (MIN_MSG_PRIORITY + 1);
        arg->valid = 1;
        strncpy(arg->Data, ClientData, MAX_MSG_SIZE);
    }    
    
    return;
}
