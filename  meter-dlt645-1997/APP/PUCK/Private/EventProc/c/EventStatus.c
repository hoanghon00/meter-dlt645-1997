#define EVENTSTATUS_C
#include "Pub_PUCK.h"
#include "EventStatus.h"

/***********************************************************************
函数功能：初始化事件状态等参数
入口：1------------强行清除事件瞬态字，和事件发生结果状态字sram；0-------根据CS值来清除
出口：无
***********************************************************************/
void Init_Event_Sram(INT8U Flag)
{
  INT8U i;
  
  if((CHECK_STRUCT_SUM(Meter_Run_Status)==0) || Flag)
     mem_set((void *)(&Meter_Run_Status),0,sizeof(Meter_Run_Status),\
                 (void *)(&Meter_Run_Status),sizeof(Meter_Run_Status));    
  SET_STRUCT_SUM(Meter_Run_Status);

  if((CHECK_STRUCT_SUM(Meter_Self_Test)==0) || Flag)
     mem_set((void *)(&Meter_Self_Test),0,sizeof(Meter_Self_Test),\
                 (void *)(&Meter_Self_Test),sizeof(Meter_Self_Test));    
  SET_STRUCT_SUM(Meter_Self_Test);
  
  for(i=0;i<MAX_EVENT_NUM;i++)
  {
    if((CHECK_STRUCT_SUM(EventProcStatusVar[i])==0) || Flag)
      Clr_Event_Real_Status(i);      
  }
  
  if((CHECK_STRUCT_SUM(Meter_Run_Status)==0) || Flag)
   
    mem_set((void *)(&Meter_Instant_Status),0,sizeof(Meter_Instant_Status),\
                 (void *)(&Meter_Instant_Status),sizeof(Meter_Instant_Status));
  SET_STRUCT_SUM(Meter_Instant_Status); 

  //初始化事件变位sram
  mem_set((void *)(&Loss_Volt_Change),0,sizeof(Loss_Volt_Change),\
               (void *)(&Loss_Volt_Change),sizeof(Loss_Volt_Change));   
  INIT_STRUCT_VAR(Loss_Volt_Change);
  mem_set((void *)(&Loss_Curr_Change),0,sizeof(Loss_Curr_Change),\
               (void *)(&Loss_Curr_Change),sizeof(Loss_Curr_Change));   
  INIT_STRUCT_VAR(Loss_Curr_Change);
  
  mem_set((void *)(&Curr_Neg_Change),0,sizeof(Curr_Neg_Change),\
               (void *)(&Curr_Neg_Change),sizeof(Curr_Neg_Change));   
  INIT_STRUCT_VAR(Curr_Neg_Change);
  
  //回复掉电事件状态，为掉电结束做准备（因为前几次清除了）
  if(!Flag)
  {
    if(POWER_OFF==Power_Status.Power)   //掉电事件发生了,将其置为发生,为掉电结束做准备
    {
      EventProcStatusVar[ID_EVENT_POWER_OFF].Occur=1;
      EventProcStatusVar[ID_EVENT_POWER_OFF].Status=1;
      EventProcStatusVar[ID_EVENT_POWER_OFF].StartCount=0;
      EventProcStatusVar[ID_EVENT_POWER_OFF].EndCount=0;
     SET_STRUCT_SUM(EventProcStatusVar[ID_EVENT_POWER_OFF]); 
    } 
  }
}

/***********************************************************************
函数功能：清除所有事件状态
入口：
出口：无
***********************************************************************/
void Clr_All_Event_Sram(void)
{  
  Init_Event_Sram(1);  
}
/***********************************************************************
函数功能：初始化所有事件状态
入口：
出口：无
***********************************************************************/
void Init_All_Event_Sram(void)
{  
  Init_Event_Sram(0);  
}
/***********************************************************************
函数功能：将全失压数据清零，调用条件：fx1晶振，在掉电存数中调用，为开始计全失压做准备
入口：无
出口：无
***********************************************************************/
void Clr_All_Loss_Data(void)
{
  INT8U i;
  mem_set((void *)(&All_Loss_Var),0,sizeof(All_Loss_Var),\
                   (void *)(&All_Loss_Var),sizeof(All_Loss_Var));
  All_Loss_Var.Status.First=1;
  SET_VAR_CS_PUCK(All_Loss_Var.Status); 
  for(i=0;i<ALL_LOSS_NUM;i++)
    SET_VAR_CS_PUCK(All_Loss_Var.RecordTime[i]);
}

/***********************************************************************
函数功能：根据CS成立条件，清零全失压sram，调用条件：fx1晶振，程序复位后
入口：无
出口：无
***********************************************************************/
void Init_All_Loss_Sram(void)
{
  INT8U Flag,i;
  
  Flag=CHK_VAR_CS_PUCK(All_Loss_Var.Status);
  for(i=0;i<ALL_LOSS_NUM;i++)
    Flag&=CHK_VAR_CS_PUCK(All_Loss_Var.RecordTime[i]);
  
  if(!Flag)
  {
    ASSERT(A_WARNING,0);  
    mem_set((void *)(&All_Loss_Var),0,sizeof(All_Loss_Var),(void *)(&All_Loss_Var),sizeof(All_Loss_Var));
    All_Loss_Var.Status.First=1;
    SET_VAR_CS_PUCK(All_Loss_Var.Status); 
    for(i=0;i<ALL_LOSS_NUM;i++)
    SET_VAR_CS_PUCK(All_Loss_Var.RecordTime[i]); 
  }
}

/***********************************************************************
函数功能：存储全失压数据，调用条件：fx1晶振，事件任务已经开启，并且是主电源来电
入口：无
出口：无
***********************************************************************/
void Save_All_Loss_Data(void)
{
  INT8U i,temp,num;
  INT32U temp32;
  
  temp=CHK_VAR_CS_PUCK(All_Loss_Var.Status);
  if(0==temp)
  ASSERT(A_WARNING,0);  
  
  if(All_Loss_Var.Status.BatRunTime)  //时间累计
  {
    temp=Read_Storage_Data_PUCK(0xb214,(INT8U *)(&temp32),4);
    ASSERT(A_WARNING,1==temp);  
    temp32+=All_Loss_Var.Status.BatRunTime;
    temp=Write_Storage_Data(0xb214,(INT8U *)(&temp32),4);
    ASSERT(A_WARNING,NO_ERR==temp); 
    All_Loss_Var.Status.BatRunTime=0; //清除
    SET_VAR_CS_PUCK(All_Loss_Var.Status); 
  }  
  
  if((All_Loss_Var.Status.Nums==0)||(All_Loss_Var.Status.Mins==0))   //没有事件发生
  {
    Clr_All_Loss_Data();   //存完后清0
    return ;
  }
   
  if(All_Loss_Var.Status.start && All_Loss_Var.Status.Index<ALL_LOSS_NUM)  //有发生没有结束,且不越界
  {
    if((CHeck_Same_Byte((INT8U*)All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime,0x00,sizeof(All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime))))
    {
      All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime[0]=Cur_Time1.Min;
      All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime[1]=Cur_Time1.Hour;
      All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime[2]=Cur_Time1.Date;
      All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime[3]=Cur_Time1.Month;
      All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime[4]=Cur_Time1.Year;  //取系统时钟
      SET_VAR_CS_PUCK(All_Loss_Var.RecordTime[All_Loss_Var.Status.Index]);
    }
  }
  num=(All_Loss_Var.Status.Nums<=ALL_LOSS_NUM)?All_Loss_Var.Status.Nums:ALL_LOSS_NUM;
  if(All_Loss_Var.Status.Index<ALL_LOSS_NUM)   //index保证不越界
  {
     for(i=0;i<num;i++)  
     {
       if(num<=ALL_LOSS_NUM)  //没有翻转：按顺序依次写
         temp=i;   
       else                   //翻转：按老-->新依次写  
        temp=((All_Loss_Var.Status.Index+1)%ALL_LOSS_NUM+i)%ALL_LOSS_NUM;       
      All_Loss_Vol_Data_Proc((INT8U *)All_Loss_Var.RecordTime[temp].StartTime,(INT8U *)All_Loss_Var.RecordTime[temp].EndTime);
     }
  }
  
  All_Loss_Vol_Counts_Time_Proc(All_Loss_Var.Status.Nums,All_Loss_Var.Status.Mins); 
  Clr_All_Loss_Data();   //存完后清0  
}
/***********************************************************************
函数功能：计算全失压数据，调用条件：fxt晶振，RTC闹铃。在RTC中断中调用！
入口：无
出口：无
***********************************************************************/
void Count_All_Loss_Proc(void)
{ 
  All_Loss_Var.Status.BatRunTime++;  //中断一次，记电池工作1分钟----PUCK

  if(All_Loss_Var.Status.Occur)
  {
    All_Loss_Var.Status.Occur=0;
    All_Loss_Var.Status.Exist=1;
  }
  else
    All_Loss_Var.Status.Exist=0;
  
  if(All_Loss_Var.Status.Exist==0 && All_Loss_Var.Status.start==0) //无失压发生，以最快的速度退出
  {
    SET_VAR_CS_PUCK(All_Loss_Var.Status); 
    return  ;
  }
  
  if(All_Loss_Var.Status.Exist && All_Loss_Var.Status.start==0)  //全失压发生
  {
    All_Loss_Var.Status.start=1;
    if(All_Loss_Var.Status.First)
    {
      All_Loss_Var.Status.First=0;
      All_Loss_Var.Status.Index=0;
    }
    else
      All_Loss_Var.Status.Index=(All_Loss_Var.Status.Index+1)%ALL_LOSS_NUM;
    
    /*
    if(All_Loss_Var.Status.Index>ALL_LOSS_NUM)  //防止越界
      All_Loss_Var.Status.Index=0;
    */
    All_Loss_Var.Status.Nums++;
    All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].StartTime[0]=MIN;      //CPU_RTC_Time.RTC.Min;
    All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].StartTime[1]=HOUR;      //CPU_RTC_Time.RTC.Hour;
    All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].StartTime[2]=DAY;      //CPU_RTC_Time.RTC.Date;
    All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].StartTime[3]=MONTH;      //CPU_RTC_Time.RTC.Month;
    All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].StartTime[4]=YEAR;      //CPU_RTC_Time.RTC.Year;
    
    memset((INT8U*)All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime,0x00,\
          sizeof(All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime));     //死写，不用mem_set
    
    SET_VAR_CS_PUCK(All_Loss_Var.Status); 
    SET_VAR_CS_PUCK(All_Loss_Var.RecordTime[All_Loss_Var.Status.Index]); 
    return ;
  }
  
  if(All_Loss_Var.Status.Exist && All_Loss_Var.Status.start)    //全失压延续
  {
    All_Loss_Var.Status.Mins++;
    SET_VAR_CS_PUCK(All_Loss_Var.Status);
    return ;
  }
  
  if(All_Loss_Var.Status.Exist==0 && All_Loss_Var.Status.start) //全失压结束
  {
    All_Loss_Var.Status.Mins++;
    All_Loss_Var.Status.start=0;
    All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime[0]=MIN;      //CPU_RTC_Time.RTC.Min;
    All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime[1]=HOUR;     // CPU_RTC_Time.RTC.Hour;
    All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime[2]=DAY;      //CPU_RTC_Time.RTC.Date;
    All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime[3]=MONTH;    //  CPU_RTC_Time.RTC.Month;
    All_Loss_Var.RecordTime[All_Loss_Var.Status.Index].EndTime[4]=YEAR;     // CPU_RTC_Time.RTC.Year;
    SET_VAR_CS_PUCK(All_Loss_Var.Status); 
    SET_VAR_CS_PUCK(All_Loss_Var.RecordTime[All_Loss_Var.Status.Index]); 
    return ;    
  }
  
  SET_VAR_CS_PUCK(All_Loss_Var.Status); 
  SET_VAR_CS_PUCK(All_Loss_Var.RecordTime[All_Loss_Var.Status.Index]);
}
/***********************************************************************
函数功能：根据发生的事件Id和状态，设置相应的状态
入口：无
出口：无
***********************************************************************/
void Set_Event_Instant_Status(INT8U Id,INT8U Status)
{
  INT8U Byte,Bit;
  if(Id>=MAX_EVENT_NUM)
    return ;
  
  Byte=Id/8;
  Bit=Id%8;
  DIS_PD_INT;  //关掉电中断
  if(Status)
    SET_BIT(Meter_Instant_Status.Instant_Event[Byte],Bit);
  else
    CLR_BIT(Meter_Instant_Status.Instant_Event[Byte],Bit);    
  SET_STRUCT_SUM(Meter_Instant_Status);
  EN_PD_INT; //开掉电中断
}

/***********************************************************************
函数功能：根据发生的事件，设置相应的瞬态状态字
入口：无
出口：无
***********************************************************************/
void Set_Event_Instant(INT8U Id)
{
  if(EventPropertyVar[Id].DoubleFlag==0)  //不成对事件，马上清除原有真实状态
    Clr_Event_Real_Status(Id);
  Set_Event_Instant_Status(Id,1);
}
/***********************************************************************
函数功能：根据位或者字节单位，清除对应的位信息或字节信息
入口：无
出口：无
***********************************************************************/
void Clr_Event_Instant(INT8U Id)
{
  Set_Event_Instant_Status(Id,0); 
}

/***********************************************************************
函数功能：将事件模拟为发生后的状态,为结束做准备
入口：无
出口：无
***********************************************************************/
void Set_Event_Occur_Status(INT8U i)
{
  if(i>MAX_ID_EVENT)
    return ;
  
  mem_set((void *)(EventProcStatusVar+i),1,sizeof(EventProcStatusVar+i),\
                 (void *)(EventProcStatusVar+i),sizeof(EventProcStatusVar+i));
  
  EventProcStatusVar[i].EndCount=EventPropertyVar[i].EndCount+1;
  SET_STRUCT_SUM(EventProcStatusVar[i]);    
}
/***********************************************************************
函数功能：将事件模拟为结束后的状态,为发生做准备
入口：无
出口：无
***********************************************************************/
void Set_Event_End_Status(INT8U i)
{
  if(i>MAX_ID_EVENT)
    return ;
  
  mem_set((void *)(EventProcStatusVar+i),0,sizeof(EventProcStatusVar+i),\
                 (void *)(EventProcStatusVar+i),sizeof(EventProcStatusVar+i));
  
  EventProcStatusVar[i].StartCount=EventPropertyVar[i].StartCount+1;
  SET_STRUCT_SUM(EventProcStatusVar[i]);    
}
/***********************************************************************
函数功能：清除事件发生的真正状态
入口：无
出口：无
***********************************************************************/
void Clr_Event_Real_Status(INT8U Id)
{
  if(Id>MAX_ID_EVENT)
    return ;
  
  mem_set((void *)(EventProcStatusVar+Id),0,sizeof(EventProcStatusVar+Id),\
                 (void *)(EventProcStatusVar+Id),sizeof(EventProcStatusVar+Id));
  SET_STRUCT_SUM(EventProcStatusVar[Id]);

}
/***********************************************************************
函数功能：根据位或者字节单位，获取对应的位信息或字节信息
入口：无
出口：无
***********************************************************************/
INT8U Get_Event_Instant(INT8U Id)
{  
  INT8U Byte,Bit,temp;
  
  if(Id>=MAX_EVENT_NUM)
    return 0;
  
  Byte=Id/8;
  Bit=Id%8;
  
  temp=CHECK_STRUCT_SUM(Meter_Instant_Status); 
  if(0==temp)
  ASSERT(A_WARNING,0);  

  temp=GET_BIT(Meter_Instant_Status.Instant_Event[Byte],Bit);  
  return temp;    
}


/********************************************************************************
函数原型：void Event_Judge_StartAndEnd(INT8U Id,INT8U CountMode,INT8U SaveMode)
函数功能：检查事件(成对)发生
CountMode:0----------表示设定次数判定为0，非零-------------按照事件表格时间判定
SaveMode:0----------表示不存储，非零-------------表示存储
*********************************************************************************/
void Event_Judge_StartAndEnd(INT8U Id,INT8U CountMode,INT8U SaveMode)
{
  INT16U ConStartcounts,ConEndcounts;
  INT16U RamStartcounts,RamEndcounts;
  
  if(Id<MIN_ID_EVENT || Id>MAX_ID_EVENT) //保护一下
    return ;
  
  if(CHECK_STRUCT_SUM(EventProcStatusVar[Id])==0)  //校验和不对，防止事件乱存乱搞，清除之
  {
    ASSERT(A_WARNING,0);
    Clr_Event_Real_Status(Id);
    return ;
  }
  if(Check_Sys_Status()==SYS_NORMAL) //正常模式下，连续判定时间，由表格决定
  {
    ConStartcounts=(EventPropertyVar[Id].StartUnit==0)?\
                EventPropertyVar[Id].StartCount:((INT16U)EventPropertyVar[Id].StartCount*60);
    ConEndcounts=(EventPropertyVar[Id].EndUnit==0)?\
                EventPropertyVar[Id].EndCount:((INT16U)EventPropertyVar[Id].EndCount*60); 
  }
  else                               //非正常模式下，发生和结束只判定一次
  {
    ConStartcounts=1;
    ConEndcounts=1; 
  }
  
  if(0==CountMode)
  {
    ConStartcounts=1;
    ConEndcounts=1; 
  }
  
  if(EventProcStatusVar[Id].Occur==0)   //未发生该事件
  {
    if(Get_Event_Instant(Id))  //当前瞬时状态指示了事件的发生
      EventProcStatusVar[Id].StartCount++;
    else                                 //不是连续发生，清计数器
      EventProcStatusVar[Id].StartCount=0;
    
    if(EventProcStatusVar[Id].StartCount>=60)  //分钟计数
    {
      EventProcStatusVar[Id].Mins++;
      EventProcStatusVar[Id].StartCount=0;
    }
      
    RamStartcounts=EventProcStatusVar[Id].Mins*60+(INT16U)EventProcStatusVar[Id].StartCount;
    if(RamStartcounts>=ConStartcounts)  //事件连续发生
    {       
      if(EventPropertyVar[Id].DisEvent)
        DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Event_Occur----->%s(%d)(%x-%x-%x %x:%x:%x)",EventPropertyVar[Id].EventName,Id,\
                        Cur_Time1.Year,Cur_Time1.Month,Cur_Time1.Date,Cur_Time1.Hour,Cur_Time1.Min,Cur_Time1.Sec);  
     
      if((EventPropertyVar[Id].StartSave)&&(0!=SaveMode))
      {
        //现将系统状态字置位，为Event_Data_Proc()服务
        EventProcStatusVar[Id].Status=1;
        Get_Meter_Run_Status();
        //将系统状态字清0，作为Event_Data_Proc()某些判定的条件
        EventProcStatusVar[Id].Status=0;
        
        Event_Data_Proc(Id,EVENT_OCCUR);  //调用事件发生存储函数  
        DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Write Event_Occur----->%s(%d)",EventPropertyVar[Id].EventName,Id);
      }
      EventProcStatusVar[Id].Occur=1; EventProcStatusVar[Id].Status=1;  
      EventProcStatusVar[Id].Status1=1; EventProcStatusVar[Id].Status2=1; 
      EventProcStatusVar[Id].Status3=1; EventProcStatusVar[Id].Status4=1; 
      EventProcStatusVar[Id].StartCount=0;EventProcStatusVar[Id].EndCount=0;  
      EventProcStatusVar[Id].Mins=0;

      if(EventPropertyVar[Id].DoubleFlag==0)  //对于不成对的事件，马上清除发生瞬态字
      {
          Clr_Event_Instant(Id); //清除对应瞬时状态字，用于对紧急时候的事件回应
          //DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Emergece Event End,Clear Instant---------->%s",EventPropertyVar[Id].EventName);
      }
      if(EventPropertyVar[Id].DoubleFlag &&ConEndcounts==0)  //成对的事件，需要马上清除发生瞬态字
         Clr_Event_Instant(Id);
    }
  }
  else                         //已经发生了该事件,清除计数器开始计数
  {
    if(EventPropertyVar[Id].DoubleFlag==1)  //成对事件
    {
      if(EventProcStatusVar[Id].Status==1)  //成对事件发生后,判结束事件
      {
        if(Get_Event_Instant(Id)==0)  //在结束进行时
          EventProcStatusVar[Id].EndCount++;
        else                          
        {
          if(EventPropertyVar[Id].ForbitEnd==0)       //不需要强行结束
            EventProcStatusVar[Id].EndCount=0;         //不是连续结束，清计数器
          else                                    //需要强行结束,但需延时,计数器更新
          {
            Clr_Event_Instant(Id); //强行清除对应瞬时状态字
            EventProcStatusVar[Id].EndCount++;
            if(EventPropertyVar[Id].EndIndex<MAX_END_TIME_NUM)
              ConEndcounts=Const_End_Time[EventPropertyVar[Id].EndIndex]; 
            else
              ASSERT(A_WARNING,0);
          }
        }          
      }
    }
    else                                //不是成对事件：发生后立即启动清除计数器计数
    {
      Clr_Event_Instant(Id); //强行清除对应瞬时状态字-------PUCK!!!!!!!!!
      EventProcStatusVar[Id].EndCount++;
    }
    
    if(EventProcStatusVar[Id].EndCount>=60)  //分钟计数
    {
      EventProcStatusVar[Id].Mins++;
      EventProcStatusVar[Id].EndCount=0;
    }
    RamEndcounts=EventProcStatusVar[Id].Mins*60+(INT16U)EventProcStatusVar[Id].EndCount; 
    if(RamEndcounts>=ConEndcounts)  //事件结束/恢复
    {
      if(EventPropertyVar[Id].DisEvent)
        DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Event_End----->%s(%d)(%x-%x-%x %x:%x:%x)",EventPropertyVar[Id].EventName,Id,\
                        Cur_Time1.Year,Cur_Time1.Month,Cur_Time1.Date,Cur_Time1.Hour,Cur_Time1.Min,Cur_Time1.Sec); 
       if((EventPropertyVar[Id].EndSave)&&(0!=SaveMode))
       {
         //现将系统状态字清0，为Event_Data_Proc()服务
        EventProcStatusVar[Id].Status=0;
        Get_Meter_Run_Status();
        //将系统状态字还原，作为Event_Data_Proc()某些判定的条件
        EventProcStatusVar[Id].Status=1;
        
         Event_Data_Proc(Id,EVENT_END);  //调用事件发生存储函数
         DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Write Event_End----->%s(%d)",EventPropertyVar[Id].EventName,Id);
       }
       EventProcStatusVar[Id].Occur=0;    EventProcStatusVar[Id].Status=0;
       EventProcStatusVar[Id].Status1=0;  EventProcStatusVar[Id].Status2=0;
       EventProcStatusVar[Id].Status3=0;   EventProcStatusVar[Id].Status4=0;
       EventProcStatusVar[Id].StartCount=0;EventProcStatusVar[Id].EndCount=0; 
       EventProcStatusVar[Id].Mins=0;       
      
       if(Id==ID_EVENT_PRGKEY_ON)
        Key_Value_Pub.Key.Bit.PrgKey=0;
    }
  }
    SET_STRUCT_SUM(EventProcStatusVar[Id]);  
}
/********************************************************************************
函数原型：void Get_Meter_Instant_Status(void)
函数功能：获取表计瞬态字,通过函数指针调用相应函数实现
*********************************************************************************/
void Get_Meter_Instant_Status(void)
{
  INT8U i;
  
  for(i=MIN_ID_EVENT;i<=MAX_ID_EVENT;i++)
  {
    if(EventPropertyVar[i].GetInstStatus)
      EventPropertyVar[i].GetInstStatus();
  }
}
/********************************************************************************
函数原型：void Event_Judge_Save_Alarm(INT8U Flag)
函数功能：所有事件发生/结束判定，,同时对外报警
*********************************************************************************/
void Event_Judge_Save_Alarm(INT8U CountMode,INT8U SaveMode)
{
  INT8U i;
  
  for(i=MIN_ID_EVENT;i<=MAX_ID_EVENT;i++)
  {
    if(REC_FRAME_CONDITION)
    {
        OS_TimeDly_Ms(10);  
    }
    Event_Judge_StartAndEnd(i,CountMode,SaveMode);
  }
}
/********************************************************************************
函数原型：void Read_Event_Status_Perone(INT8U PersonID,INT8U EventId)
函数功能：用于内部个人检查是否发生,读后清除
入口：    PersonID--------个人ID
          EventId---------事件ID
出口：0---------无此事件产生；1---------此事件发生；
*********************************************************************************/
INT8U Read_Event_Status_Perone(INT8U PersonID,INT8U EventId)
{
  INT8U temp;  
 
  if(EventId>MAX_EVENT_NUM)
    return 0;
  
  temp=0;   
  switch(PersonID)
  {
  case HUCK:
    temp=EventProcStatusVar[EventId].Status1;
    EventProcStatusVar[EventId].Status1=0;
    break;
    
   case SUCK:
    temp=EventProcStatusVar[EventId].Status2;
    EventProcStatusVar[EventId].Status2=0;
    break;
      
   case PUCK:
    temp=EventProcStatusVar[EventId].Status3;
    EventProcStatusVar[EventId].Status3=0;
    break;
      
    
    case LUCK:
    temp=EventProcStatusVar[EventId].Status4;
    EventProcStatusVar[EventId].Status4=0;
    break;
  }
  SET_STRUCT_SUM(EventProcStatusVar[EventId]);  
  return  temp;
}
/********************************************************************************
函数原型：void Read_Event_Status(INT8U EventId)
函数功能：外部(如485接口)检查事件是否发生
          注意：若事件是成对发生，读后不清除，状态一直存在；若不是成对发生，此事件会在一定的时间自动清除
入口：    EventId---------事件ID
出口：0---------无此事件产生；1---------此事件发生；
*********************************************************************************/
INT8U Read_Event_Status(INT8U Id)
{
  if(Id>MAX_EVENT_NUM)
    return 0;

  return  (EventProcStatusVar[Id].Status);
}
/********************************************************************************
函数原型：void Check_Event_End(INT8U EventId)
函数功能：判断紧急事件是否结束，对于事件是否真正发生和结束，需要调用Read_Event_Status()函数
入口：    EventId---------事件ID
出口：1---------此事件已经结束；0---------此事件发生；
*********************************************************************************/
INT8U Check_Event_End(INT8U Id)
{
  if(Id>MAX_EVENT_NUM)
    return 0;
  
  if(EventPropertyVar[Id].DoubleFlag==1)    //判断是否是紧急事件    
    ASSERT(A_WARNING,0);  
  if(Get_Event_Instant(Id)==0)
    return 1;
  
  return  0;
}
/***********************************************************************
函数功能：获取电表运行状态字,按照协议格式组帧
入口：无
出口：无
***********************************************************************/
void Get_Meter_Run_Status(void)
{

  if(CHECK_STRUCT_SUM(Measu_Status_Mode_ToPub_PUCK)==0)
  ASSERT(A_WARNING,0);  
  
  //mem_set(&Meter_Run_Status,0xff,sizeof(Meter_Run_Status),&Meter_Run_Status,sizeof(Meter_Run_Status));  
  
  //电表运行状态字1
  Meter_Run_Status.Meter_Stat1.Bit.Bit0=0;                                        //抄表方式：0--自动；1----手动；定死
  Meter_Run_Status.Meter_Stat1.Bit.Bit1=0;                                        //最大需量结算方式：0--滑差；1----区间；??????????
  Meter_Run_Status.Meter_Stat1.Bit.Bit2=Read_Event_Status(ID_EVENT_LOWPWR_BAT_LOW);    //低功耗电池电压：0--正常；1----欠压；
  Meter_Run_Status.Meter_Stat1.Bit.Bit3=0;                                        //保留：0
  Meter_Run_Status.Meter_Stat1.Bit.Bit4=Measu_Status_Mode_ToPub_PUCK.SUM_APwrDir; //总有功方向：0--正向；1----反向；
  Meter_Run_Status.Meter_Stat1.Bit.Bit5=Measu_Status_Mode_ToPub_PUCK.SUM_RPwrDir; //总无功方向：0--正向；1----反向；
  Meter_Run_Status.Meter_Stat1.Bit.Bit6=0;                                        //保留：0
  Meter_Run_Status.Meter_Stat1.Bit.Bit7=0;                                        //保留：0
  
  
  //电表运行状态字2
  Meter_Run_Status.Meter_Stat2.Bit.Bit0=Measu_Status_Mode_ToPub_PUCK.A_APwrDir;   //有功方向：0--正向；1----反向；
  Meter_Run_Status.Meter_Stat2.Bit.Bit1=Measu_Status_Mode_ToPub_PUCK.B_APwrDir;   //有功方向：0--正向；1----反向；
  Meter_Run_Status.Meter_Stat2.Bit.Bit2=Measu_Status_Mode_ToPub_PUCK.C_APwrDir;   //有功方向：0--正向；1----反向；
  Meter_Run_Status.Meter_Stat2.Bit.Bit3=0;                                        //保留：0
  Meter_Run_Status.Meter_Stat2.Bit.Bit4=Measu_Status_Mode_ToPub_PUCK.A_RPwrDir;   //无功方向：0--正向；1----反向；
  Meter_Run_Status.Meter_Stat2.Bit.Bit5=Measu_Status_Mode_ToPub_PUCK.B_RPwrDir;   //无功方向：0--正向；1----反向；
  Meter_Run_Status.Meter_Stat2.Bit.Bit6=Measu_Status_Mode_ToPub_PUCK.C_RPwrDir;   //无功方向：0--正向；1----反向；
  Meter_Run_Status.Meter_Stat2.Bit.Bit7=0;                                        //保留：0
  
  
  //电表运行状态字3
  Meter_Run_Status.Meter_Stat3.Bit.Bit0=Read_Event_Status(ID_EVENT_CUR_NEG_SEQ);    //电流逆相序：0--正相序；1----逆相序；
  Meter_Run_Status.Meter_Stat3.Bit.Bit1=Read_Event_Status(ID_EVENT_VOLT_NEG_SEQ);    //电压逆相序：0--正相序；1----逆相序；
  Meter_Run_Status.Meter_Stat3.Bit.Bit2=0;                                        //保留：0
  Meter_Run_Status.Meter_Stat3.Bit.Bit3=0;                                        //保留：0
  Meter_Run_Status.Meter_Stat3.Bit.Bit4=Read_Event_Status(ID_EVENT_A_LOSS_CURR)|\
                                        Read_Event_Status(ID_EVENT_AB_LOSS_CURR)|\
                                        Read_Event_Status(ID_EVENT_AC_LOSS_CURR)|\
                                        Read_Event_Status(ID_EVENT_ALL_LOSS_CURR);  //A相失流：0--不失流；1----失流;
  
  Meter_Run_Status.Meter_Stat3.Bit.Bit5=Read_Event_Status(ID_EVENT_B_LOSS_CURR)|\
                                        Read_Event_Status(ID_EVENT_AB_LOSS_CURR)|\
                                        Read_Event_Status(ID_EVENT_BC_LOSS_CURR)|\
                                        Read_Event_Status(ID_EVENT_ALL_LOSS_CURR);  //B相失流：0--不失流；1----失流;
  
  Meter_Run_Status.Meter_Stat3.Bit.Bit6=Read_Event_Status(ID_EVENT_C_LOSS_CURR)|\
                                        Read_Event_Status(ID_EVENT_AC_LOSS_CURR)|\
                                        Read_Event_Status(ID_EVENT_BC_LOSS_CURR)|\
                                        Read_Event_Status(ID_EVENT_ALL_LOSS_CURR);  //C相失流：0--不失流；1----失流;
  Meter_Run_Status.Meter_Stat3.Bit.Bit7=0;                                        //保留：0

  //电表运行状态字4
  Meter_Run_Status.Meter_Stat4.Bit.Bit0=Read_Event_Status(ID_EVENT_POWER_ALARM);   //超功率报警：0--没发生；1----发生；
  Meter_Run_Status.Meter_Stat4.Bit.Bit1=Read_Event_Status(ID_EVENT_POWER_SWITCH);   //超功率跳闸：0--没发生；1----发生；
  Meter_Run_Status.Meter_Stat4.Bit.Bit2=Read_Event_Status(ID_EVENT_RTC_BAT_LOW);    //内部电池欠压：0--正常；1----欠压；
  Meter_Run_Status.Meter_Stat4.Bit.Bit3=0;                                        //保留：0
  Meter_Run_Status.Meter_Stat4.Bit.Bit4=0;                                        //欠费报警：0--没发生；1----发生；??????????
  Meter_Run_Status.Meter_Stat4.Bit.Bit5=0;                                        //欠费跳闸：0--没发生；1----发生；??????????
  Meter_Run_Status.Meter_Stat4.Bit.Bit6=Read_Event_Status(ID_EVENT_LOWPWR_BAT_LOW); //外部电池欠压：0--正常；1----欠压；
  Meter_Run_Status.Meter_Stat4.Bit.Bit7=0;                                        //保留：0
  
  
  //电网运行状态字
  Meter_Run_Status.Grid_Stat.Bit.Bit0=Read_Event_Status(ID_EVENT_A_LOSS_PARSE); //A相断电：0--没发生；1----发生；用断相替代
  Meter_Run_Status.Grid_Stat.Bit.Bit1=Read_Event_Status(ID_EVENT_B_LOSS_PARSE); //B相断电：0--没发生；1----发生；用断相替代
  Meter_Run_Status.Grid_Stat.Bit.Bit2=Read_Event_Status(ID_EVENT_C_LOSS_PARSE); //C相断电：0--没发生；1----发生；用断相替代
  Meter_Run_Status.Grid_Stat.Bit.Bit3=0;                                        //保留：0
  Meter_Run_Status.Grid_Stat.Bit.Bit4=Read_Event_Status(ID_EVENT_A_VOLT_OVERLOAD);//A相过压：0--没发生；1----发生；
  Meter_Run_Status.Grid_Stat.Bit.Bit5=Read_Event_Status(ID_EVENT_B_VOLT_OVERLOAD); //B相过压：0--没发生；1----发生；
  Meter_Run_Status.Grid_Stat.Bit.Bit6=Read_Event_Status(ID_EVENT_C_VOLT_OVERLOAD); //C相过压：0--没发生；1----发生；
  Meter_Run_Status.Grid_Stat.Bit.Bit7=0;                                        //保留：0
  
  
  //错误信息字ERR
  Meter_Run_Status.Meter_Stat6.Bit.Bit0=0;//Read_Event_Status(ID_EVENT_ERR_DATA);  //非法数据
  Meter_Run_Status.Meter_Stat6.Bit.Bit1=Read_Event_Status(ID_EVENT_ERR_DI); //数据标识错
  Meter_Run_Status.Meter_Stat6.Bit.Bit2=Read_Event_Status(ID_EVENT_ERR_PASSWD);  //密码错
  Meter_Run_Status.Meter_Stat6.Bit.Bit3=0;                                        //保留：0
  Meter_Run_Status.Meter_Stat6.Bit.Bit4=Read_Event_Status(ID_EVENT_YEAR_FLOW); //年时区数超
  Meter_Run_Status.Meter_Stat6.Bit.Bit5=Read_Event_Status(ID_EVENT_DATE_FLOW); //日时段数超
  Meter_Run_Status.Meter_Stat6.Bit.Bit6=Read_Event_Status(ID_EVENT_TRRI_FLOW); //费率数超
  Meter_Run_Status.Meter_Stat6.Bit.Bit7=0;                                        //保留：0
  
  //失压/断相状态字
  Meter_Run_Status.Volt_LossMiss.Bit.Bit0=Read_Event_Status(ID_EVENT_A_LOSS_PARSE); //A相断相：0--没发生；1----发生；
  Meter_Run_Status.Volt_LossMiss.Bit.Bit1=Read_Event_Status(ID_EVENT_B_LOSS_PARSE); //B相断相：0--没发生；1----发生；
  Meter_Run_Status.Volt_LossMiss.Bit.Bit2=Read_Event_Status(ID_EVENT_C_LOSS_PARSE); //C相断相：0--没发生；1----发生；
  Meter_Run_Status.Volt_LossMiss.Bit.Bit3=0;                                    //保留：0
  Meter_Run_Status.Volt_LossMiss.Bit.Bit4=Read_Event_Status(ID_EVENT_A_LOSS_VOLT)|\
                                          Read_Event_Status(ID_EVENT_AB_LOSS_VOLT)|\
                                          Read_Event_Status(ID_EVENT_AC_LOSS_VOLT)|\
                                          Read_Event_Status(ID_EVENT_ABC_LOSS_VOLT);//A相失压：0--没发生；1----发生；
  Meter_Run_Status.Volt_LossMiss.Bit.Bit5=Read_Event_Status(ID_EVENT_B_LOSS_VOLT)|\
                                          Read_Event_Status(ID_EVENT_AB_LOSS_VOLT)|\
                                          Read_Event_Status(ID_EVENT_BC_LOSS_VOLT)|\
                                          Read_Event_Status(ID_EVENT_ABC_LOSS_VOLT);//B相失压：0--没发生；1----发生；
  Meter_Run_Status.Volt_LossMiss.Bit.Bit6=Read_Event_Status(ID_EVENT_C_LOSS_VOLT)|\
                                          Read_Event_Status(ID_EVENT_AC_LOSS_VOLT)|\
                                          Read_Event_Status(ID_EVENT_BC_LOSS_VOLT)|\
                                          Read_Event_Status(ID_EVENT_ABC_LOSS_VOLT);//C相失压：0--没发生；1----发生；
  Meter_Run_Status.Volt_LossMiss.Bit.Bit7=0;                                    //保留：0
  
  //电流反极性/象限
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit0=0;  //电表工作象限1：
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit1=0;  //电表工作象限2:
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit2=0;  //电表工作象限3：
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit3=0;  //电表工作象限4：
  
  if(Judge_I_Start())  //电流都大于启动电流
  {
    switch(Measu_InstantData_ToPub_PUCK.Quadrant.Sum)
    {
    case QUADRANT1:
        Meter_Run_Status.NegCurr_Qudrant.Bit.Bit0=1;
      break;
    case QUADRANT2:
        Meter_Run_Status.NegCurr_Qudrant.Bit.Bit1=1;
      break;
    case QUADRANT3:
        Meter_Run_Status.NegCurr_Qudrant.Bit.Bit2=1;
      break;
    case QUADRANT4:
        Meter_Run_Status.NegCurr_Qudrant.Bit.Bit3=1;
      break;
    }
  }
  else
    Meter_Run_Status.NegCurr_Qudrant.Byte&=0xf0;
    
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit4=Read_Event_Status(ID_EVENT_A_NEG_CURR);    //A相电流反极性：0--正常；1----反;
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit5=Read_Event_Status(ID_EVENT_B_NEG_CURR);    //B相电流反极性：0--正常；1----反；
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit6=Read_Event_Status(ID_EVENT_C_NEG_CURR);    //C相电流反极性：0--正常；1----反；
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit7=0;                                          //保留：0
  
  //程键打开/跳线短接/需量复位/电表上锁
  Meter_Run_Status.Key_Event.Bit.Bit0=Read_Event_Status(ID_EVENT_PRGKEY_ON);   //编程键打开
  Meter_Run_Status.Key_Event.Bit.Bit1=Read_Event_Status(ID_EVENT_FAC_ON);      //工厂模式
  Meter_Run_Status.Key_Event.Bit.Bit2=Read_Event_Status(ID_EVENT_CLR_DEMAND_MAN)|Read_Event_Status(ID_EVENT_CLR_DEMAND_COM) |Read_Event_Status(ID_EVENT_CLR_ALL_DATA); //需量复位
  Meter_Run_Status.Key_Event.Bit.Bit3=Read_Event_Status(ID_EVENT_METER_LOCK);   //电表上锁
  Meter_Run_Status.Key_Event.Bit.Bit4=Read_Event_Status(ID_ALARM_OUT);   //表是否存在报警指示
  Meter_Run_Status.Key_Event.Bit.Bit5=Read_Event_Status(ID_EVENT_KEY_SWITCH);    //跳线短接模式
  Meter_Run_Status.Key_Event.Bit.Bit6=Read_Event_Status(ID_EVENT_ADJUST_MEASU);          //校表状态或者系统参数未设置
  Meter_Run_Status.Key_Event.Bit.Bit7=0;                                       //保留：0
  
  //通信状态字
  Meter_Run_Status.Comm_Status.Bit.Bit0=0;                                       //在线，??????????
  Meter_Run_Status.Comm_Status.Bit.Bit1=0;                                       //场强0，??????????
  Meter_Run_Status.Comm_Status.Bit.Bit2=0;                                       //场强1，??????????
  Meter_Run_Status.Comm_Status.Bit.Bit3=0;                                       //场强2，??????????
  Meter_Run_Status.Comm_Status.Bit.Bit4=0;                                       //保留：0
  Meter_Run_Status.Comm_Status.Bit.Bit5=0;                                       //保留：0
  Meter_Run_Status.Comm_Status.Bit.Bit6=0;                                       //保留：0
  Meter_Run_Status.Comm_Status.Bit.Bit7=0;                                       //保留：0 
  
  
      
  //扩展不兼容的状态字
  Meter_Run_Status.Ext_Ac_Power_Dir.Byte=0;
  Meter_Run_Status.Ext_Ac_Power_Dir.Bit.Bit0=Measu_Status_Mode_ToPub_PUCK.SUM_APwrDir;   //有功方向标志字节（1字节，0-3位为总、a、b、c元件，高4字节为0）
  Meter_Run_Status.Ext_Ac_Power_Dir.Bit.Bit1=Measu_Status_Mode_ToPub_PUCK.A_APwrDir;   //有功方向：0--正向；1----反向；
  Meter_Run_Status.Ext_Ac_Power_Dir.Bit.Bit2=Measu_Status_Mode_ToPub_PUCK.B_APwrDir;   //有功方向：0--正向；1----反向；
  Meter_Run_Status.Ext_Ac_Power_Dir.Bit.Bit3=Measu_Status_Mode_ToPub_PUCK.C_APwrDir;   //有功方向：0--正向；1----反向；
  
  Meter_Run_Status.Ext_Reac_Power_Dir.Byte=0;
  Meter_Run_Status.Ext_Reac_Power_Dir.Bit.Bit0=Measu_Status_Mode_ToPub_PUCK.SUM_RPwrDir;   //有功方向标志字节（1字节，0-3位为总、a、b、c元件，高4字节为0）
  Meter_Run_Status.Ext_Reac_Power_Dir.Bit.Bit1=Measu_Status_Mode_ToPub_PUCK.A_RPwrDir;   //无功方向：0--正向；1----反向；
  Meter_Run_Status.Ext_Reac_Power_Dir.Bit.Bit2=Measu_Status_Mode_ToPub_PUCK.B_RPwrDir;   //无功方向：0--正向；1----反向；
  Meter_Run_Status.Ext_Reac_Power_Dir.Bit.Bit3=Measu_Status_Mode_ToPub_PUCK.C_RPwrDir;   //无功方向：0--正向；1----反向；
  
 
  //广东：电表状态字
  Meter_Run_Status.GD_Meter_Stat.Byte=0;                              
  Meter_Run_Status.GD_Meter_Stat.Bit.Bit0=0;        //命令拉闸                          
  Meter_Run_Status.GD_Meter_Stat.Bit.Bit1=0;        //实际拉闸
  Meter_Run_Status.GD_Meter_Stat.Bit.Bit3=0;        //时钟超差
  Meter_Run_Status.GD_Meter_Stat.Bit.Bit4=1;        //电表时钟已校
  Meter_Run_Status.GD_Meter_Stat.Bit.Bit5=Read_Event_Status(ID_EVENT_DEMAND_ALARM);        //电表大负荷状态(同超需量)
  Meter_Run_Status.GD_Meter_Stat.Bit.Bit6=Read_Event_Status(ID_EVENT_LOWPWR_BAT_LOW)|Read_Event_Status(ID_EVENT_RTC_BAT_LOW); //电池欠压

  
  //广东：有功功率方向状态字
  Meter_Run_Status.GD_PwrDir_Stat.Byte=0;                              
  Meter_Run_Status.GD_PwrDir_Stat.Bit.Bit0=Measu_Status_Mode_ToPub_PUCK.A_APwrDir;        //A相功率方向                          
  Meter_Run_Status.GD_PwrDir_Stat.Bit.Bit1=Measu_Status_Mode_ToPub_PUCK.B_APwrDir;        //B相功率方向  
  Meter_Run_Status.GD_PwrDir_Stat.Bit.Bit2=Measu_Status_Mode_ToPub_PUCK.C_APwrDir;        //C相功率方向  
  Meter_Run_Status.GD_PwrDir_Stat.Bit.Bit4=Measu_Status_Mode_ToPub_PUCK.SUM_APwrDir;        //合相功率方向  

  //广东：失压状态字
  Meter_Run_Status.GD_LossVolt_Stat.Byte=0;                              
  Meter_Run_Status.GD_LossVolt_Stat.Bit.Bit0=Read_Event_Status(ID_EVENT_A_LOSS_VOLT);        //A相失压                         
  Meter_Run_Status.GD_LossVolt_Stat.Bit.Bit1=Read_Event_Status(ID_EVENT_B_LOSS_VOLT);        //B相失压    
  Meter_Run_Status.GD_LossVolt_Stat.Bit.Bit2=Read_Event_Status(ID_EVENT_C_LOSS_VOLT);        //C相失压    
  
  //广东：失流状态字
  Meter_Run_Status.GD_LossCurr_Stat.Byte=0;                              
  Meter_Run_Status.GD_LossCurr_Stat.Bit.Bit0=Read_Event_Status(ID_EVENT_A_LOSS_CURR);        //A相失流                          
  Meter_Run_Status.GD_LossCurr_Stat.Bit.Bit1=Read_Event_Status(ID_EVENT_B_LOSS_CURR);        //B相失流          
  Meter_Run_Status.GD_LossCurr_Stat.Bit.Bit2=Read_Event_Status(ID_EVENT_C_LOSS_CURR);        //C相失流          

  //广东：硬件状态字
  Meter_Run_Status.GD_Hard_Stat.Byte=0;                              
  Meter_Run_Status.GD_Hard_Stat.Bit.Bit0=1;           //有红外功能                       
  Meter_Run_Status.GD_Hard_Stat.Bit.Bit1=0;           //有拉闸功能        
  Meter_Run_Status.GD_Hard_Stat.Bit.Bit2=1;           //有485功能 
  
  //广东：编程状态字
  /*
  Meter_Run_Status.GD_Prog_Stat.Byte=0;                              
  Meter_Run_Status.GD_Prog_Stat.Bit.Bit0=0;           //时段费率更改                     
  Meter_Run_Status.GD_Prog_Stat.Bit.Bit1=0;           //抄表日更改       
  Meter_Run_Status.GD_Prog_Stat.Bit.Bit2=0;           //表号更改
  Meter_Run_Status.GD_Prog_Stat.Bit.Bit3=0;           //密码更改
  Meter_Run_Status.GD_Prog_Stat.Bit.Bit2=0;           //广播校时
  Meter_Run_Status.GD_Prog_Stat.Bit.Bit3=0;           //普通校时
  Meter_Run_Status.GD_Prog_Stat.Bit.Bit3=0;           //底度清零
  Meter_Run_Status.GD_Prog_Stat.Bit.Bit3=0;           //显示设置更改
  */
  SET_STRUCT_SUM(Meter_Run_Status);
}


/***********************************************************************
函数功能：获取电表自检状态字,按照协议格式组帧
入口：无
出口：无
***********************************************************************/
void Get_Meter_Self_Test(void)
{
  
  Meter_Self_Test.Low_Byte.Byte=0;
  Meter_Self_Test.Low_Byte.Bit.Bit0=Read_Event_Status(ID_EVENT_RTCERR_RUN)|Read_Event_Status(ID_EVENT_RTCERR_PWRON);  //时钟乱
  Meter_Self_Test.Low_Byte.Bit.Bit4=Read_Event_Status(ID_EVENT_DEMAND_ALARM);    //超需量/功率
  Meter_Self_Test.Low_Byte.Bit.Bit6=Read_Event_Status(ID_EVENT_A_LOSS_PARSE)| \
                                    Read_Event_Status(ID_EVENT_B_LOSS_PARSE)| \
                                    Read_Event_Status(ID_EVENT_C_LOSS_PARSE);      //断相


  
  Meter_Self_Test.High_Byte.Bit.Bit0=Meter_Run_Status.Meter_Stat3.Bit.Bit4|\
                                     Meter_Run_Status.Meter_Stat3.Bit.Bit5|\
                                     Meter_Run_Status.Meter_Stat3.Bit.Bit6;          //失流(含全失流)
  Meter_Self_Test.High_Byte.Bit.Bit1=Meter_Run_Status.Grid_Stat.Bit.Bit4|\
                                     Meter_Run_Status.Grid_Stat.Bit.Bit5|\
                                     Meter_Run_Status.Grid_Stat.Bit.Bit6;           //过压
  Meter_Self_Test.High_Byte.Bit.Bit2=Meter_Run_Status.Volt_LossMiss.Bit.Bit4|\
                                     Meter_Run_Status.Volt_LossMiss.Bit.Bit5|\
                                     Meter_Run_Status.Volt_LossMiss.Bit.Bit6;       //失压
  
  Meter_Self_Test.High_Byte.Bit.Bit3=Meter_Run_Status.Meter_Stat3.Bit.Bit1;        //逆相序
  Meter_Self_Test.High_Byte.Bit.Bit4=Read_Event_Status(ID_EVENT_LOWPWR_BAT_LOW);        //低功耗电池欠压
  Meter_Self_Test.High_Byte.Bit.Bit5=Read_Event_Status(ID_EVENT_RTC_BAT_LOW);        //时钟电池欠压
  Meter_Self_Test.High_Byte.Bit.Bit6=0;        //内卡数据错
  Meter_Self_Test.High_Byte.Bit.Bit7=0;        //内卡故障
  
  SET_STRUCT_SUM(Meter_Self_Test);  
  
}
/********************************************************************************
函数原型：void Check_Prog_Status(void)
函数功能：检查是否处于编程状态
入口： 
出口：0---------不是编程状态；1---------是编程状态；
*********************************************************************************/
INT8U Check_Prog_Status(void)
{
  return (Read_Event_Status(ID_EVENT_PRGKEY_ON));  
  
}
/********************************************************************************
函数原型：void Check_MeterAddr_Status(void)
函数功能：检查设置表地址按钮是否开启
入口： 
出口：0---------表地址按钮没有按下；1---------表地址按钮按下了；
*********************************************************************************/
INT8U Check_MeterAddr_Status(void)
{
  return (Read_Event_Status(ID_METER_ADDR_EN));  
  
}
/********************************************************************************
函数原型：INT8U Check_Man_ClrDemd_En(void)
函数功能：检查设置表地址按钮是否开启
入口： 
出口：0---------不允许手动清需量；1---------允许手动清需量
*********************************************************************************/
INT8U Check_Man_ClrDemd_En(void)
{
  return (Read_Event_Status(ID_CLR_DEMAND_KEY));  
}
/********************************************************************************
函数原型：void ClrDemand_Request(void)
函数功能：手动清需量请求，访问后自行清除
入口： 
出口：1------允许清需量；0-------不允许清需量
*********************************************************************************/
INT8U ClrDemand_Request(void)
{
  return (Read_Event_Status_Perone(PUCK,ID_EVENT_PROG));  
  
}
/********************************************************************************
函数原型：void Check_AB_Switch(void)
函数功能：
入口： 
出口：0---------不是AB显示模式切换状态；1---------是AB显示模式切换状态；
*********************************************************************************/
INT8U Check_AB_Switch(void)
{
  static INT8U KeyStatus=0;
  
  if(Get_Sys_Status()==SYS_NORMAL)
    return (Read_Event_Status_Perone(PUCK,ID_EVENT_AB_FUNC_KEY)); 
  if(Get_Sys_Status()==SYS_RESUME)  //10ms查询一次
  {
    if((Inter_Up_Key_STATUS==1)&&(Inter_Down_Key_STATUS==1))  //没有按钮按下
      KeyStatus=0;
    else    //有一个按钮按下了
      KeyStatus++;
  }
  if(KeyStatus>=80)
  {
    KeyStatus=0;
    return 1;
  }
  return 0;
}
/********************************************************************************
函数原型：void Check_Event_StruCS(void)
函数功能：检查与事件相关的所有结构体的校验和
入口： 
出口：
*********************************************************************************/
void Check_Event_StruCS(void)
{
  INT8U i,Flag;
  for(i=0;i<MAX_EVENT_NUM;i++)
  {
    Flag=CHECK_STRUCT_SUM(EventProcStatusVar[i]);
    ASSERT(A_WARNING,1==Flag);  
  }
  
  Flag=CHECK_STRUCT_SUM(Meter_Instant_Status);
  if(Flag==0)  //Sram错误，清除事件瞬态字
  {
    ASSERT(A_WARNING,0);
    mem_set((void *)(&Meter_Instant_Status),0,sizeof(Meter_Instant_Status),\
                   (void *)(&Meter_Instant_Status),sizeof(Meter_Instant_Status));
    SET_STRUCT_SUM(Meter_Instant_Status);  
  }
  
  Flag=CHECK_STRUCT_SUM(EventInsParaVar);
  if(Flag==0)
  {
    ASSERT(A_WARNING,0);
    Init_Event_Instant_Para(); 
  }
  
  Flag=CHECK_STRUCT_SUM(Meter_Run_Status);
  if(Flag==0)  //Sram错误，清除事件瞬态字
  {
    ASSERT(A_WARNING,0);
    mem_set((void *)(&Meter_Run_Status),0,sizeof(Meter_Run_Status),\
                   (void *)(&Meter_Run_Status),sizeof(Meter_Run_Status));
    SET_STRUCT_SUM(Meter_Run_Status);  
  }
  
  Flag=CHECK_STRUCT_SUM(Meter_Self_Test);
  if(Flag==0)  //Sram错误，清除事件瞬态字
  {
    ASSERT(A_WARNING,0);
    mem_set((void *)(&Meter_Self_Test),0,sizeof(Meter_Self_Test),\
                   (void *)(&Meter_Self_Test),sizeof(Meter_Self_Test));
    SET_STRUCT_SUM(Meter_Self_Test);  
  }

  if(CHECK_STRUCT_SUM(Power_Status)==0)
    ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_VAR(Loss_Volt_Change)==0)
    ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_VAR(Loss_Curr_Change)==0)
    ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_VAR(Curr_Neg_Change)==0)
    ASSERT(A_WARNING,0);

}
/********************************************************************************
函数原型：void Test_Interr_IO_Status(void)
函数功能：查询中断引脚的IO口状态
入口： 
出口：
*********************************************************************************/
void Test_Interr_IO_Status(void)
{    
    if(Inter_Up_Key_STATUS==0)
      DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Check Up_Key_Pin IO==0");  
    if(Inter_Down_Key_STATUS==0)
      DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Check Down_Key_Pin IO==0"); 
    if(Inter_Left_Key_STATUS==0)
      DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Check Left_Key_Pin IO==0"); 
    if(Inter_Right_Key_STATUS==0)
      DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Check Right_Key_Pin IO==0"); 
    if(POWER_OFF_STATUS==0)
      DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Check PowerOff_Pin IO==0"); 
    //if(ALL_V_LOSS_STATUS==0)
    //  DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Check LossVolt_Pin IO==0"); 
    
}
/********************************************************************************
函数原型：void ReNew_PwrOff_Status(void)
函数功能：设置掉电后的相关瞬时状态字
*********************************************************************************/
void ReNew_PwrOff_Status(void)
{   
  INT8U i;
  if(CHECK_STRUCT_SUM(Meter_Instant_Status)==0)  //掉电前事件的状态，校验和错，不允许存数
  {
    ASSERT(A_WARNING,0);
     mem_set((void *)(&Meter_Instant_Status),0,sizeof(Meter_Instant_Status),\
                   (void *)(&Meter_Instant_Status),sizeof(Meter_Instant_Status));
    SET_STRUCT_SUM(Meter_Instant_Status); 
    return ;
  }
  else
  {
    for(i=MIN_ID_EVENT;i<=MAX_ID_EVENT;i++)   //根据掉电时，事件发生的真实状态，来初始化瞬态字，供掉电存储
    {
      if(Read_Event_Status(i))
        Set_Event_Instant(i);
      else
        Clr_Event_Instant(i);
    }    
  }
  
 /*
  for(i=0;i<MAX_EVENT_NUM;i++)
  {
    if(Get_Event_Instant(i)&&EventPropertyVar[i].EndSave)    //先前有事件发生,且恢复时需要存储
    {
       DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Event_End(Power Off)----->%s(%d)(%x-%x-%x %x:%x:%x)",EventPropertyVar[i].EventName,i,\
                        Cur_Time1.Year,Cur_Time1.Month,Cur_Time1.Date,Cur_Time1.Hour,Cur_Time1.Min,Cur_Time1.Sec); 
       if(EventPropertyVar[i].EndSave)
       {
         Event_Data_Proc(i,EVENT_END);  //调用事件发生存储函数
         DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Write Event_End----->%s(%d)",EventPropertyVar[i].EventName,i);
       }
    }
  } 
  */
}
/********************************************************************************
函数原型：void Set_Power_Or_Reset_Ststus(void)
函数功能：复位前，设置正常复位标志
*********************************************************************************/
void Set_PwrOrRst_Status(void)
{  
  Set_Event_Instant(ID_EVENT_POWER_OFF);  //掉电事件发生
  Power_Status.Power=POWER_OFF;          //掉电事件标志
  SET_STRUCT_SUM(Power_Status);    
}
/********************************************************************************
函数原型：void Save_Event_Data(void)
函数功能：上电后，对掉电的事件数据存储
*********************************************************************************/
void Save_Event_Data(void)
{ 
  INT8U Flag,i;
  
  Flag=CHECK_STRUCT_SUM(Power_Status);  
  if(Flag)    //上次复位前的状态,sram状态正确
  {
    if(Get_Sys_Status()==SYS_NORMAL) 
    {
      if(POWER_OFF!=Power_Status.Power)     //前次没有捕获掉电信息
      {
        Power_Status.Power=ABNORMAL_RESET;      //置为复位状态，为上电事件判定做准备，避免作为上电事件而存储
      }
    } 
  }
  else
    Power_Status.Power=POWER_ON;       //上电事件置位，为判定做准备
  SET_STRUCT_SUM(Power_Status);    
      
  
  for(i=MIN_ID_EVENT;i<=MAX_ID_EVENT;i++)  //根据上次掉电的瞬态字，强行设置事件的发生
  {
    if(Get_Event_Instant(i))   //此状态实际是从 内卡 取出的状态，不是 sram状态
    {
      if(ID_EVENT_POWER_OFF!=i)        //上电后，对前次事件,模拟发生了的状态,等待结束
        Set_Event_Occur_Status(i);
      else                             //上电后，对前次掉电事件，模拟结束发生了的状态，等待发生
      {
         if(ABNORMAL_RESET!=Power_Status.Power)     //Sram 判定不为复位，内卡判定为掉电，那确实掉电了！
         {
           Set_Event_End_Status(i);     
           Power_Status.Power=POWER_OFF;    //以内卡的掉电为准
         }
      }
    }
    else
        Clr_Event_Real_Status(i);
  }  

  Event_Judge_Save_Alarm(0,1);    //根据强行设置的瞬态字，模拟一次结束    
  Clr_All_Event_Sram();     //清除报警相关字
  
}
/********************************************************************************
函数原型：void Afer_PwrOff_Proc(void)
函数功能：掉电存事件数据
*********************************************************************************/
void PwrOff_Proc_PUCK(void)
{ 
  Clr_All_Loss_Data();         //清零全失压Sram数据
  Set_Soft_RTC_PUCK(ID_CPU_RTC);        //初始化软时钟
  ReNew_PwrOff_Status();       //掉电存瞬时状态字
  Set_PwrOrRst_Status();
}
/********************************************************************************
函数原型：void Event_Main_PUCK(INT8U Flag)
函数功能：状态字及事件主函数
*********************************************************************************/
void Event_Main_PUCK(INT8U Flag)
{
  Check_Event_StruCS();  
  Get_Meter_Instant_Status();  //获取表计瞬态字
  Event_Judge_Save_Alarm(1,1);    //通过瞬态字，获取表计所有事件真实状态,存储事件信息,同时对外报警
   //Test_Interr_IO_Status();
}
/********************************************************************************
函数原型：void Event_Proc_Sec(void)
函数功能：每秒对事件的处理及其判定机制
*********************************************************************************/
void Event_Proc_Sec(void)
{
  static volatile S_Int32U Sec_Timer_Bak={CHK_BYTE,0xFFFFFFFF,CHK_BYTE};
    
  if(Sec_Timer_Bak.Var!=Sec_Timer_Pub)
  {
    Event_Para_Modi();
    Event_Main_PUCK(0);
    Get_Meter_Run_Status(); 
    Get_Meter_Self_Test();
    Sec_Timer_Bak.Var=Sec_Timer_Pub;
  }
}
/********************************************************************************
函数原型：void Emerg_Event_Proc(void)
函数功能：对突发事件的份额处理
出口：0---------无此事件产生；1---------此事件发生；
*********************************************************************************/
void Emerg_Event_Proc(void)
{
  INT8U i;
  for(i=MIN_ID_EVENT;i<=MAX_ID_EVENT;i++)
  {
    if((EventPropertyVar[i].DoubleFlag==0)&&(Get_Event_Instant(i)))  //不成对事件发生了瞬态字设置
    {
      if(REC_FRAME_CONDITION)
      {
        OS_TimeDly_Ms(10);  
      }
      
      Event_Judge_StartAndEnd(i,1,1);
      
      /*
      Get_Meter_Instant_Status();     
      Event_Judge_Save_Alarm(1,1);    //通过瞬态字，获取表计所有事件真实状态,存储事件信息,同时对外报警
      */
      return ;
    }    
  }
}

/********************************************************************************
函数原型：void Init_Event_DIS_PUCK(void)
函数功能：初始化事件
出口：
*********************************************************************************/
void Init_Event_DIS_PUCK(void)
{  
  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.A/UNIT_V<60.0)
      Meter_Run_Status.Volt_LossMiss.Bit.Bit0=1;  //A相断相：0--没发生；1----发生；
  else
      Meter_Run_Status.Volt_LossMiss.Bit.Bit0=0; 
  
  if(Get_SysParse_Mode()==PARSE_341)
  {  
    if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.B/UNIT_V<60.0)
      Meter_Run_Status.Volt_LossMiss.Bit.Bit1=1;  //B相断相：0--没发生；1----发生；
    else
      Meter_Run_Status.Volt_LossMiss.Bit.Bit1=0;
  }
  
  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.C/UNIT_V<60.0)
      Meter_Run_Status.Volt_LossMiss.Bit.Bit2=1;  //C相断相：0--没发生；1----发生；
  else
     Meter_Run_Status.Volt_LossMiss.Bit.Bit2=0;
  
  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.A/(FP32S)UNIT_V>Get_Un()*(FP32S)1.25)
  {
    Meter_Run_Status.Grid_Stat.Bit.Bit4=1;        //A电压过压
  }
  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.A/(FP32S)UNIT_V<Get_Un()*(FP32S)1.20)  //或者掉电,需加上掉电条件
    Meter_Run_Status.Grid_Stat.Bit.Bit4=0;        //A电压过压清除

  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.B/(FP32S)UNIT_V>Get_Un()*(FP32S)1.25)
  {
    Meter_Run_Status.Grid_Stat.Bit.Bit5=1;  //B电压过压
  }
  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.B/(FP32S)UNIT_V<Get_Un()*(FP32S)1.20)  //或者掉电,需加上掉电条件
    Meter_Run_Status.Grid_Stat.Bit.Bit5=0;  //B电压过压清除

  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.C/(FP32S)UNIT_V>Get_Un()*(FP32S)1.25)
  {
    Meter_Run_Status.Grid_Stat.Bit.Bit6=1;  //B电压过压
    Event_Vol_Cur.Over_Vol[2]=Measu_InstantData_ToPub_PUCK.Volt.C;
  }
  if(Measu_InstantData_ToPub_PUCK.Volt.C/(FP32S)UNIT_V<Get_Un()*(FP32S)1.20)  //或者掉电,需加上掉电条件
    Meter_Run_Status.Grid_Stat.Bit.Bit6=0;  //B电压过压清除
  
  
  
  if((FP32S)Measu_InstantData_ToPub_PUCK.Curr.A/UNIT_A<(Get_In()/50))  //电流小于2%In
    Meter_Run_Status.Meter_Stat3.Bit.Bit4=1;     //A相失流：0--没发生；1----发生；
  else
    Meter_Run_Status.Meter_Stat3.Bit.Bit4=0;
  
  if(Get_SysParse_Mode()==PARSE_341)
  {
    if((FP32S)Measu_InstantData_ToPub_PUCK.Curr.B/UNIT_A<(Get_In()/50))  //电流小于2%In
      Meter_Run_Status.Meter_Stat3.Bit.Bit5=1;     //B相失流：0--没发生；1----发生；
    else
      Meter_Run_Status.Meter_Stat3.Bit.Bit5=0;
  }
  
  if((FP32S)Measu_InstantData_ToPub_PUCK.Curr.C/UNIT_A<(Get_In()/50))  //电流小于2%In
    Meter_Run_Status.Meter_Stat3.Bit.Bit6=1;     //C相失流：0--没发生；1----发生；
  else
    Meter_Run_Status.Meter_Stat3.Bit.Bit6=0;  
  
    //电流反极性/象限
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit0=0;  //电表工作象限1：
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit1=0;  //电表工作象限2:
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit2=0;  //电表工作象限3：
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit3=0;  //电表工作象限4：
  
  if(Judge_I_Start())  //电流都大于启动电流
  {
    switch(Measu_InstantData_ToPub_PUCK.Quadrant.Sum)
    {
    case QUADRANT1:
        Meter_Run_Status.NegCurr_Qudrant.Bit.Bit0=1;
      break;
    case QUADRANT2:
        Meter_Run_Status.NegCurr_Qudrant.Bit.Bit1=1;
      break;
    case QUADRANT3:
        Meter_Run_Status.NegCurr_Qudrant.Bit.Bit2=1;
      break;
    case QUADRANT4:
        Meter_Run_Status.NegCurr_Qudrant.Bit.Bit3=1;
      break;
    }
  }
  else
    Meter_Run_Status.NegCurr_Qudrant.Byte&=0xf0;
  
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit4=Measu_Status_Mode_ToPub_PUCK.A_APwrDir; 
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit5=Measu_Status_Mode_ToPub_PUCK.B_APwrDir; 
  Meter_Run_Status.NegCurr_Qudrant.Bit.Bit6=Measu_Status_Mode_ToPub_PUCK.C_APwrDir; 
    
        
  SET_STRUCT_SUM(Meter_Run_Status);  
}
/********************************************************************************

*********************************************************************************/
void Event_main_PUCK(INT8U Flag)
{
  if(Get_Sys_Status()!=SYS_NORMAL)  //只有在主电源供电情况下，此任务才执行
  {
    while(1)
     Task_Sleep();    
  }
  
  if(Get_Sys_Status()!=SYS_NORMAL) //只有在正常模式下，才有事件的处理
    return ;
  
  if(Flag)
  { 
    Init_All_Event_Sram();
    Set_Cal_Event();  //上电后，根据flash状态，初始化校表状态或者系统参数未设定事件状态，以后进入事件流程    
    Init_Event_Instant_Para();
    Save_All_Loss_Data();
    OS_TimeDly_Sec(1);
    return ;
  }
  Emerg_Event_Proc();     //对突发事件的每10ms处理，必须与Channel_Data_Proc_PUCKStatus）同步
  Event_Proc_Sec();       //对所有事件的每秒处理
 }  