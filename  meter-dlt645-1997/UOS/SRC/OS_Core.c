#define OS_CORE_C
#include "OS_Core.h"

#pragma optimize=none
//创建任务函数
OS_INT8U OS_Create_Task(void(*Entry)(),OS_STK *pStk,OS_INT16U Stk_Size,OS_INT8U Name[])
{
  static OS_INT8U i=0;//使用静态局部变量，防止上下文变化时局部变量的变化
  static OS_INT8U Task_ID;

  if(Task_Info.Cur_Task_Num>=OS_TASK_NUM)
  {
    OS_ASSERT(0);
    return 0;  
  }
  
  i=0;
  Task_ID=Task_Info.Cur_Task_Num;
  
  Task_Info.Cur_Task_Num++; 
#if OS_USE_CS==1
    OS_SET_STRUCT_SUM(Task_Info);
#endif
  
  OS_INIT_STRUCT_VAR(Tcb[Task_ID]);
  
  Tcb[Task_ID].pEntry=Entry;  
  Tcb[Task_ID].pStk=pStk;
  Tcb[Task_ID].Stk_Size=Stk_Size;
  Tcb[Task_ID].Time_Dly=0;
  
#if OS_RST_CHK_STK==1
  Task_Stk_Left[Task_ID]=Calc_Stk_Left(Task_ID);
#endif
  
  //将堆栈内容初始化成特殊字符，以统计堆栈用量
  //同时保留堆栈的头尾两个字节，用于判断堆栈是否溢出或者被覆盖
#if OS_STK_GROWTH==1//堆栈从高地址向低地址递减
  OS_memset(pStk-Stk_Size/sizeof(OS_STK)+1,CHK_BYTE,Stk_Size,pStk-Stk_Size/sizeof(OS_STK)+1,Stk_Size);
  pStk--;
#else  //堆栈从低地址向高地址增长
  OS_memset((INT8U *)pStk,CHK_BYTE,Stk_Size,(INT8U *)pStk,Stk_Size);
  pStk++;
#endif
  
  SAVE_TASK_SP();//将SP保存到Old_SP
  New_SP=(OS_SP)pStk;
  RESTORE_TASK_SP();//将New_SP恢复到SP中

  if(OS_Save_Env(Tcb[Task_ID].Env)==0)//保存当前环境变量
  { 
    New_SP=Old_SP;
    RESTORE_TASK_SP();//恢复老的SP

#if OS_USE_TASK_NAME //如果使用名字 
    while(i<OS_TASK_NAME_SIZE-1 && Name[i]!=0)
    {
      Tcb[Task_ID].Name[i]=Name[i];  
      i++;
    }
    Tcb[Task_ID].Name[i]=0;
#else
    Name[i]=Name[i];//防止编译警告
#endif
    
#if OS_USE_CS==1
    OS_SET_STRUCT_SUM(Tcb[Task_ID]);
#endif 

  }
  else//else内程序在新任务中运行
  {
    Tcb[Task_Info.Cur_Task_ID].pEntry();//Entry在切换到任务堆栈后可能丢失，因此采用pEntry  
  }
  return 1;
}

//修改当前任务ID，以便调度到下一个任务
void Change_Cur_Task_ID()
{
  /*
#if OS_USE_CS==1
  OS_INT8U Re;
  
  Re=OS_CHECK_STRUCT_SUM(Task_Info);
  OS_ASSERT(1==Re);
#endif 
*/
  OS_INT8U Re;
  
  Re=OS_CHECK_STRUCT_VAR(Task_Info);
  if(0==Re)
    OS_ASSERT(0);
  
  Task_Info.Cur_Task_ID++;
  if(Task_Info.Cur_Task_ID>=Task_Info.Cur_Task_Num) 
    Task_Info.Cur_Task_ID=0;
/*  
#if OS_USE_CS==1
  OS_SET_STRUCT_SUM(Task_Info);
#endif
*/  
}

//检查任务的延时时间
void OS_Check_TaskDly()
{
  OS_INT16U i;
  OS_INT8U Flag=0;
  
#if OS_USE_TICK_ISR //使用OS_Tick_ISR中Tick_ISR_Flag置1
  if(1==Tick_ISR_Flag)//在中断中该标志置1表示一个时基周期到
  {
    Tick_ISR_Flag=0;
    Flag=1;
  }
#else
  if(OS_Check_Tick())//查询定时器等确定时基，该函数根据具体CPU来实现
  {
    OS_Tick_Hook();//时基Hook函数
    Flag=1;
  }
#endif
  
  if(1==Flag)//一个时基到了
  {
    for(i=0;i<Task_Info.Cur_Task_Num;i++)//修改每个任务的延时时间
    {
      if(0!=Tcb[i].Time_Dly)//对延时非0的函数，减少其延时
      {
        /*
    #if OS_USE_CS==1
        Flag=OS_CHECK_STRUCT_SUM(Tcb[i]);//先检查校验和
        OS_ASSERT(1==Flag);
    #endif
        */
        
        //对每个任务的延时进行减数
        if(Tcb[i].Time_Dly>OS_MS_PER_TICK)
          Tcb[i].Time_Dly-=OS_MS_PER_TICK;
        else
          Tcb[i].Time_Dly=0;
     /*   
    #if OS_USE_CS==1
        OS_SET_STRUCT_SUM(Tcb[i]);//设置校验和
    #endif
     */
      }
    }
  }
}

void OS_Task_Switch()
{
  OS_Task_Context_Chk();
  OS_Check_TaskDly();
  Task_Switch_Hook();
  Change_Cur_Task_ID();
  OS_Restore_Env(Tcb[Task_Info.Cur_Task_ID].Env);   
}

#if OS_USE_TICK_ISR //使用中断作为Tick基准
void OS_Tick_ISR() //定衣ms定时器,每毫秒中断调用
{
  Tick_ISR_Flag=1;
  OS_Tick_Hook();
}
#endif

//延时调用
void OS_TimeDly(OS_INT32U Hour,OS_INT32U Min,OS_INT32U Sec,OS_INT32U m_Sec)
{
  Tcb[Task_Info.Cur_Task_ID].Time_Dly=Hour*3600L*1000L+Min*60L*1000L+Sec*1000L+m_Sec;
  if(Tcb[Task_Info.Cur_Task_ID].Time_Dly<=OS_MS_PER_TICK)//不能让延时小于一个Tick的时间
    Tcb[Task_Info.Cur_Task_ID].Time_Dly+=OS_MS_PER_TICK;
  OS_Waitfor((Tcb[Task_Info.Cur_Task_ID].Time_Dly==0));
}

//操作系统启动函数
void OS_Start()
{
#if OS_USE_IDLE_TASK==1
  OS_Create_Task(&OS_Idle_Task,&Idle_Task_STK[OS_IDLE_TASK_STK_SIZE-1],100,"OS_Idle_Task");
#endif
  OS_Restore_Env(Tcb[0].Env);//切换到第一个任务的执行环境   
}

//操作系统初始化函数
void OS_Init()
{
#if OS_USE_TICK_ISR==1
  Tick_ISR_Flag=0;
#endif
  
  Task_Info.Cur_Task_ID=0;
  Task_Info.Cur_Task_Num=0;
  OS_INIT_STRUCT_VAR(Task_Info);

#if OS_USE_CS==1
  OS_SET_STRUCT_SUM(Task_Info);
#endif
}
#undef OS_CORE_C
