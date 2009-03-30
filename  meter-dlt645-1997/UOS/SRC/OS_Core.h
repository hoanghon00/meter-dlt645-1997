#ifndef OS_CORE_H
#define OS_CORE_H

#include "OS_Cfg.h"
#include "OS_Port.h"
#include "OS_Sem.h"
#include "OS_Ext.h"
#include "OS_Pub.h"

#if OS_USE_LIB
#include "setjmp.h"
#endif

#undef OS_EXT
#ifndef OS_CORE_C
#define OS_EXT extern
#else
#define OS_EXT
#endif

//#define OS_VERSION 106 //2008.03.29
//#define OS_VERSION 107 //2008.04.08
//#define OS_VERSION 108 //2008.04.19
//#define OS_VERSION 109 //2008.05.06
//#define OS_VERSION 110 //2008.06.12
//#define OS_VERSION 111 //2008.06.29
//#define OS_VERSION 112 //2008.07.25
//#define OS_VERSION 113 //2008.09.28
#define OS_VERSION 114 //2008.10.08

//定义堆栈检查字
#define CHK_BYTE (OS_INT8U)0x5A

#if OS_USE_LIB
#define OS_Task_Env jmp_buf
#define OS_Save_Env(x) setjmp(x)
#define OS_Restore_Env(x) longjmp(x,1) 
#endif

//定义三个位操作宏
#define OS_SET_BIT(x,y) (x)|=0x01<<(y)
#define OS_CLR_BIT(x,y) (x)&=~(0x01<<(y))
#define OS_GET_BIT(x,y) (((x)>>(y))&0x01)

//4个独立的延时宏,方便调用
#define OS_TimeDly_Ms(x) OS_TimeDly(0,0,0,x)
#define OS_TimeDly_Sec(x) OS_TimeDly(0,0,x,0)
#define OS_TimeDly_Min(x) OS_TimeDly(0,x,0,0)
#define OS_TimeDly_Hou(x) OS_TimeDly(x,0,0,0)

extern void Power_Down_Check();
extern void Task_Switch_Hook();
//条件等待,注意Condition中的全局变量必须是Volatile类型
#define OS_Waitfor(Condition)\
do{ OS_Save_Env(Tcb[Task_Info.Cur_Task_ID].Env);\
    Power_Down_Check();\
    if(!(Condition))\
    { OS_Task_Switch();\
      OS_Restore_Env(Tcb[Task_Info.Cur_Task_ID].Env);\
    }\
  }while(0)

//带超时的条件等待,注意Condition中的全局变量必须是Volatile类型
#define OS_Waitfor_HMSM(Condition,Hour,Min,Sec,m_Sec)\
  do{Tcb[Task_Info.Cur_Task_ID].Time_Dly=Hour*3600L*1000L+Min*60L*1000L+Sec*1000L+m_Sec;\
     OS_Waitfor((Condition||Tcb[Task_Info.Cur_Task_ID].Time_Dly==0));\
   }while(0)

//四个独立的带超时的条件等待,方便调用,注意Condition中的全局变量必须是Volatile类型
#define OS_Waitfor_Hou(Condition,Hou) OS_Waitfor_HMSM(Condition,Hou,0,0,0) 
#define OS_Waitfor_Min(Condition,Min) OS_Waitfor_HMSM(Condition,0,Min,0,0)
#define OS_Waitfor_Sec(Condition,Sec) OS_Waitfor_HMSM(Condition,0,0,Sec,0)
#define OS_Waitfor_Ms(Condition,Ms) OS_Waitfor_HMSM(Condition,0,0,0,Ms)


//定义任务控制块信息
typedef struct{
  OS_INT16U Head;
  
  OS_Task_Env Env;          //当前任务环境
  void (*pEntry)();         //函数入口
  OS_STK *pStk;             //堆栈起始指针
  OS_INT16U Stk_Size;       //堆栈大小

#if OS_USE_TASK_NAME       //使用任务名称
  OS_INT8S Name[OS_TASK_NAME_SIZE];//任务名字,多保留一个字节填0
#endif

#if OS_USE_CS==1//使用校验和,用于保护TCB信息
  OS_INT8U CS[OS_CS_BYTES];
#endif
  
  OS_INT32U Time_Dly;       //延时时间，单位ms
  
  OS_INT16U Tail;
}OS_TCB;

typedef struct{
  OS_INT8U Head;
  
  OS_INT16U Cur_Task_ID;  //当前任务ID号
  OS_INT16U Cur_Task_Num; //当前任务总数 
  
#if OS_USE_CS==1//使用校验和,用于保护Task_Info信息
  OS_INT8U CS[OS_CS_BYTES];
#endif
  
  OS_INT8U Tail;
  
}OS_Task_Info;

#ifdef OS_CORE_C //只提供给OS_Core.c文件
OS_EXT OS_SP volatile Old_SP;//用于创建任务时保存老的任务堆栈指针,SAVE_TASK_SP()将当前SP保存到Old_SP中
OS_EXT OS_SP volatile New_SP;//用于创建任务时恢复老的任务堆栈指针,RESTORE_TASK_SP()将当前New_SP保存到SP中
#if OS_USE_TICK_ISR
OS_EXT OS_INT8U volatile Tick_ISR_Flag;//中断标志，该标志置1表示一个时基周期过去
#endif
#endif

OS_EXT OS_TCB Tcb[OS_TASK_NUM];//任务控制信息块
OS_EXT OS_Task_Info volatile Task_Info;//当前任务信息

#if OS_RST_CHK_STK==1
OS_EXT OS_INT16U Task_Stk_Left[OS_TASK_NUM];//用于存储异常复位前的堆栈剩余量
#endif

OS_EXT void OS_Init();
OS_EXT void Change_Cur_Task_ID();
OS_EXT void OS_Check_TaskDly();
OS_EXT void OS_Task_Switch();
OS_EXT OS_INT8U OS_Create_Task(void(*Entry)(),OS_STK *pStk,OS_INT16U Stk_Size,OS_INT8U Name[]);
OS_EXT void OS_TimeDly(OS_INT32U Hour,OS_INT32U Min,OS_INT32U Sec,OS_INT32U m_Sec);
OS_EXT void OS_Start();

#if OS_USE_TICK_ISR
OS_EXT void OS_Tick_ISR();//定义时基中断服务
#endif

#endif
