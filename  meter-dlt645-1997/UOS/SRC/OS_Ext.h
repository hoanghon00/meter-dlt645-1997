#ifndef OS_EXT_H
#define OS_EXT_H
#include "OS_Core.h"

#undef OS_EXT
#ifndef OS_EXT_C
#define OS_EXT extern
#else
#define OS_EXT
#endif

#define OS_IDLE_TASK_STK_SIZE 50 //分配给时基任务的堆栈大小
#define MIN_STK_SIZE 10

//#define OS_CHECK_STRUCT_VAR(Var) ((CHK_BYTE==Var.Head  && CHK_BYTE==Var.Tail)?1:0)
//#define OS_INIT_STRUCT_VAR(Var)  do{Var.Head=CHK_BYTE;Var.Tail=CHK_BYTE;}while(0)

#if OS_USE_IDLE_TASK
OS_EXT OS_STK Idle_Task_STK[OS_IDLE_TASK_STK_SIZE];//定义时基任务的堆栈
OS_EXT void OS_Idle_Task();
#endif

OS_EXT OS_INT8U Check_Task_Stk(OS_INT8U Task_ID);
OS_EXT OS_INT8U Check_All_Task_Stk();
OS_EXT OS_INT16U Calc_Stk_Left(OS_INT8U Task_ID);
OS_EXT OS_INT8U Get_Cur_Task_ID();
OS_EXT OS_INT8U Get_Cur_Task_Num();
OS_EXT void Print_Task_Stk_Left_Bef_Rst();
OS_EXT void Print_Task_Stk_Left();
OS_EXT void OS_Task_Context_Chk();

//Tick钩子函数，注意：该函数中不能调用操作系统提供的调用
extern void OS_Tick_Hook();
#if OS_USE_IDLE_TASK==1
extern void OS_Idle_Task();
#endif

#endif

