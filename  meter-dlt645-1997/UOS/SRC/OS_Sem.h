#ifndef OS_SEM_H
#define OS_SEM_H

#undef OS_EXT
#ifdef OS_SEM_C
#define OS_EXT
#else
#define OS_EXT extern
#endif

typedef struct
{
  OS_INT8U Sem_Num;
  OS_INT32U Sem;

#if OS_SEM_LOCK_CHK==1
  OS_INT32U Task_Wait_Flag[OS_TASK_NUM];//等待事件标志
  OS_INT32U Task_Hold_Flag[OS_TASK_NUM];//拥有时间标志
#endif
  
#if OS_USE_CS==1
  OS_INT8U CS[OS_CS_BYTES];  
#endif
}S_OS_Sem;

OS_EXT volatile S_OS_Sem OS_Sem;
OS_EXT void OS_Sem_Clr();
OS_EXT OS_INT8U OS_Sem_Init(OS_INT8U ID,OS_INT8U Val);
OS_EXT OS_INT8U OS_Sem_Post(OS_INT8U ID);
OS_EXT OS_INT8U OS_Sem_Pend(OS_INT8U ID);
#endif
