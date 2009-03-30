#ifndef MONITOR_H
#define MONITOR_H

#undef EXT
#ifdef MONITOR_C
#define EXT
#else
#define EXT extern
#endif

#define TASK_DOG_SEC 15

typedef struct
{
  INT8U Counts[OS_TASK_NUM];
  INT8U CS[CS_BYTES];
}S_Task_Monitor;

EXT volatile S_Task_Monitor Task_Monitor;

EXT void Monitor_Init();
EXT void Task_Monitor_Proc();
EXT void Clear_Task_Dog();
#endif
