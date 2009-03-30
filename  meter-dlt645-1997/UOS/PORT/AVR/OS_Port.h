#ifndef OS_PORT_H
#define OS_PORT_H

#include "iom88.h"

#undef OS_EXT
#ifndef OS_PORT_C
#define OS_EXT extern
#else
#define OS_EXT
#endif

//iar 编译器支持
#define CONST const       //定义Flash变量
#define NO_INIT __no_init //定义非初始化变量

typedef unsigned long int OS_INT32U;
typedef unsigned int OS_INT16U;
typedef unsigned char OS_INT8U;
typedef char OS_INT8S;
typedef unsigned int OS_STK;
typedef unsigned int OS_SP;

//将SP保存到Old_SP
#define SAVE_TASK_SP()  Old_SP=SP

//将New_SP保存到SP
#define RESTORE_TASK_SP()  SP=New_SP

OS_EXT void OS_Tick_Task_Init();
OS_EXT OS_INT8U OS_Check_Tick();

#endif
