#define EVENT_DATA_C
#include <includes.h>

//检查当前事件是否在发生
INT8U Check_Event_Occuring(INT8U Event_ID)
{
  if(Read_Event_Status(Event_ID)==1)
    return 1;
  else
    return 0;
}

void Event_Data_Proc(INT8U Event_ID,INT8U Occur_Or_End)
{
  INT8U Total_Event_ID;
  
  DEBUG_PRINT(HUCK,DEBUG_0,"\r\n++++++++++Event Data Proc,ID:%d,Occur_Or_End:%d++++++++++",Event_ID,Occur_Or_End);
  OS_Sem_Pend(PUB_BUF0_SEM_ID);//申请PUB_BUF0，下面的函数需要用到这个信号量
  
  Event_Cumu_Data_Proc(Event_ID,Occur_Or_End,EVENT_REAL);
  
  Total_Event_ID = Get_Total_Event_ID(Event_ID);//本事件对应的总计数也要处理
  if(Total_Event_ID != NULL_EVENT_ID)
  {
    DEBUG_PRINT(HUCK, DEBUG_0, "Total Event Proc");
    Event_Cumu_Data_Proc(Total_Event_ID,Occur_Or_End,EVENT_REAL);
  }
  Event_MultiTimes_Proc(Event_ID,Occur_Or_End,EVENT_REAL);
  
  OS_Sem_Post(PUB_BUF0_SEM_ID);//释放PUB_BUF0 
  DEBUG_PRINT(HUCK,DEBUG_0,"++++++++++Event Data Proc:End++++++++++\r\n");
}


//清除事件
void Clear_Event_Data()
{
  if(Check_Clear_Data_Authority()==0)
    return;
  
  if(CLEAR_EVENT_FLAG!=Check_Clear_Data_Flag() &&
     CLEAR_ALL_FLAG!=Check_Clear_Data_Flag())
    return;
  DEBUG_PRINT(HUCK,DEBUG_0,"Clear_Event_Data!!!");
  //Set_Def_Event_Data();
  Set_Def_Event_Cumu0_Data();
  Clear_Event_MultiTimes_Data();
  Clr_All_Event_Sram(); //清除内存中的事件数据，重新开始判定事件
}

//设置默认的事件参数与数据
void Set_Def_Event_Data()
{
  Set_Def_Event_Cumu0_Data();
  Set_Def_Event_MultiTimes_Data();
}


#undef EVENT_DATA_C


