#define OS_EXT_C
#include "OS_Core.h"
#include "Pub_Debug.h"

//检查某任务堆栈使用情况，堆栈使用有误则返回0，否则返回1
//主要是查询堆栈的头尾字节,头尾字节正确,基本认为堆栈内容没有被破坏或者越界
OS_INT8U Check_Task_Stk(OS_INT8U Task_ID)
{
  OS_ASSERT(Task_ID<Task_Info.Cur_Task_Num);
  
#if OS_STK_GROWTH==1//堆栈从高地址向低地址递减 
  if(CHK_BYTE==*(OS_INT8U *)(Tcb[Task_ID].pStk) &&\
     CHK_BYTE==*(OS_INT8U *)(Tcb[Task_ID].pStk-Tcb[Task_ID].Stk_Size/sizeof(OS_STK)+1))
    return 1;
#else  //堆栈从低地址向高地址增长
  if(CHK_BYTE==*(OS_INT8U *)(Tcb[Task_ID].pStk) &&
     CHK_BYTE==*(OS_INT8U *)(Tcb[Task_ID].pStk+Stk_Size-1))
    return 1;
#endif
  OS_ASSERT(0);
  return 0;
}

//检查所有任务堆栈的使用情况
OS_INT8U Check_All_Task_Stk()
{
  OS_INT8U i;
#if OS_USE_CS==1
  OS_INT8U Re;
  
  Re=OS_CHECK_STRUCT_VAR(Task_Info);
  OS_ASSERT(1==Re);
#endif
  
  for(i=0;i<Task_Info.Cur_Task_Num;i++)
  {
    if(Check_Task_Stk(i)==0 || Calc_Stk_Left(i)<MIN_STK_SIZE)
    {
      OS_ASSERT(0);//任务堆栈有错,显示断言
      return 0;
    }
  }
  return 1;
}

//检查某个任务的堆栈还剩余多少空间
OS_INT16U Calc_Stk_Left(OS_INT8U Task_ID)
{
  OS_INT16U i;
  OS_INT8U *p;
  
  OS_ASSERT(Task_ID<Task_Info.Cur_Task_Num);
  
#if OS_STK_GROWTH==1//堆栈从高地址向低地址递减 
  p=(OS_INT8U *)(Tcb[Task_ID].pStk-Tcb[Task_ID].Stk_Size/sizeof(OS_STK)+1);
#else  //堆栈从低地址向高地址增长
  p=(OS_INT8U *)(Tcb[Task_ID].pStk);
#endif  
  
  i=0;
  while(1)
  {
    if(CHK_BYTE!=*(p+i))
      return i;
    i++;
  } 
}

//获取当前任务的ID号
OS_INT8U Get_Cur_Task_ID()
{
  OS_INT8U Re;
  
  Re=OS_CHECK_STRUCT_VAR(Task_Info);
  if(1!=Re)
    OS_ASSERT(0);
  
  return Task_Info.Cur_Task_ID;
}

//获取当前任务总数
OS_INT8U Get_Cur_Task_Num()
{
  OS_INT8U Re;
  
  Re=OS_CHECK_STRUCT_VAR(Task_Info);
  if(1!=Re)
    OS_ASSERT(0);

  return Task_Info.Cur_Task_Num;
}


//打印复位前的任务堆栈使用情况,在所有任务都已经创建好后才能调用该函数
void Print_Task_Stk_Left_Bef_Rst()
{
#if OS_RST_CHK_STK==1
  OS_INT8U i;
  
#if OS_USE_CS==1
  OS_INT8U Re;
  
  Re=OS_CHECK_STRUCT_VAR(Task_Info);
  OS_ASSERT(1==Re);
#endif
  
  for(i=0;i<Task_Info.Cur_Task_Num;i++)
    DEBUG_PRINT(HUCK,DEBUG_0,"Task %d Stack Left bef Rst:%d\r\n",i,Task_Stk_Left[i]);  
#endif
}

//打印所有任务堆栈信息
void Print_Task_Stk_Left()
{
  OS_INT8U i;
  DEBUG_PRINT(HUCK,DEBUG_0,"------Task Stack Left Stat----");
  for(i=0;i<Task_Info.Cur_Task_Num;i++)
    DEBUG_PRINT(HUCK,DEBUG_0,"Task %s Stack Left:%d",Tcb[i].Name,Calc_Stk_Left(i));
}

//检查任务的上下文是否正确，注意：该函数中不能调用操作系统提供的调用
void OS_Task_Context_Chk()
{
  OS_INT8U Re,Next_Task_ID;

  Next_Task_ID=(Task_Info.Cur_Task_ID+1)%Task_Info.Cur_Task_Num;  
  Re=OS_CHECK_STRUCT_VAR(Tcb[Next_Task_ID]);
  if(0==Re)
    OS_ASSERT(0);
  
  Re=Check_Task_Stk(Next_Task_ID);
  if(0==Re)
    OS_ASSERT(0);
  /*
#if OS_USE_CS==1
  OS_INT8U Re,Next_Task_ID;

  OS_SET_STRUCT_SUM(Tcb[Task_Info.Cur_Task_ID]);
  Next_Task_ID=(Task_Info.Cur_Task_ID+1)%Task_Info.Cur_Task_Num;  
  Re=OS_CHECK_STRUCT_SUM(Tcb[Next_Task_ID]);
  OS_ASSERT(1==Re);

  Re=Check_Task_Stk(Next_Task_ID);
  OS_ASSERT(1==Re);
#endif
  */
}

//时钟Tick钩子函数
void OS_Tick_Hook()
{
  static OS_INT16U i=0;
  
  i+=OS_MS_PER_TICK;
  if(i>=1000)
  {
    i=0;
    Sec_Timer_Pub++;
  }
}
#undef OS_EXT_C

