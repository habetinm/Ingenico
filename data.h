#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#ifndef THREAD_CNT
  #define THREAD_CNT 10
#endif

#if THREAD_CNT > 10
  #undef THREAD_CNT
  #define THREAD_CNT 10
#endif

#define MAX_MSG_SIZE 255
#define MAX_MSG_PRIORITY 0
#define MIN_MSG_PRIORITY 255
#define QUEUE_SIZE (MIN_MSG_PRIORITY + 1)

typedef struct tagTDATA {
         unsigned char cPriority;  //request priority 0 – 255 (0 – the highest priority)
         unsigned long dwTicks;    //time when request forming in system ticks
         unsigned long dwClientId; //unique client identifier
         char          Data[MAX_MSG_SIZE];  //abstract data
         char          valid;
         
} TDATA, *PTDATA;

#endif // DATA_H_INCLUDED
