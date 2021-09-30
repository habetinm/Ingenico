#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#ifndef CLIENT_CNT
  #define CLIENT_CNT 10
#endif

#if CLIENT_CNT > 10
  #undef CLIENT_CNT
  #define CLIENT_CNT 10
#endif

#ifndef RECORD_CNT
  #define RECORD_CNT 10000
#endif

#define MAX_MSG_SIZE 255
#define MAX_MSG_PRIORITY 0
#define MIN_MSG_PRIORITY 255
#define QUEUE_SIZE CLIENT_CNT

typedef struct tagTDATA {
         unsigned char cPriority;  //request priority 0 – 255 (0 – the highest priority)
         unsigned long dwTicks;    //time when request forming in system ticks
         unsigned long dwClientId; //unique client identifier
         char          Data[MAX_MSG_SIZE];  //abstract data
         char          valid;
         
} TDATA, *PTDATA;

#endif // DATA_H_INCLUDED
