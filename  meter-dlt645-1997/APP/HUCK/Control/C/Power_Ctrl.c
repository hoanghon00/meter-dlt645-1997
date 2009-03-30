#define POWER_CTRL_C
#include "Includes.h"

//读取负荷控制参数，从ROM中读取
void Read_Power_Ctrl_Para_From_Rom()
{
  INT8U Err;
  INT8U Temp[4];
  INT16U Len;

  Len=Read_Storage_Data(0xCA16,Temp,Temp,sizeof(Temp),&Err);//功率上限
  ASSERT(A_WARNING,3==Len && NO_ERR==Err);
  
  Power_Ctrl_Para.AcDemand_Up_Limit=Bcd2Hex(Temp,3);
  SET_STRUCT_SUM(Power_Ctrl_Para);
  
  Len=Read_Storage_Data(0xC214,Temp,Temp,sizeof(Temp),&Err);//用户级别 
  ASSERT(A_WARNING,1==Len && NO_ERR==Err); 
  
  Power_Ctrl_Para.User_Level=Temp[0];
  SET_STRUCT_SUM(Power_Ctrl_Para);
}

//检查信息输出控制字
//信息输出端子排布局
//秒脉冲、报警、欠费报警、欠费跳闸都有独立的端子输出
//时段切换和需量的脉冲输出共用一个端子
/*
INT8U Check_Output_Mode(INT8U Output_Flag)
{
  INT8U Mode;
  INT8U Re;
  
  Re=1;
  Re&=CHECK_STRUCT_SUM(Output_Mode);//检查信号输出控制字
  Re&=CHECK_STRUCT_SUM(Mode_Word);//检查状态字
  if(ASSERT(A_WARNING,1==Re))
    Check_Data_Avail();
  
  Mode=Output_Mode.Mode;
  switch(Output_Flag)
  {
  case EXT_ID_SEC_EN://有独立的端子，秒脉冲独立输出
    return 1;
  case EXT_ID_PARSE://时段切换脉冲
    if(OUTPUT_TIME_CHANGE_FLAG==Mode)
      return 1;
    else 
      return 0;
  case EXT_ID_DEMAND://需量周期脉冲
    if(OUTPUT_DEMAND_FLAG==Mode)
    {
      if(0==Mode_Word.Mode[2].Bit.Bit4)
        return 1;
      else
        return 0;
    }
    else 
      return 0;
  case OUTPUT_DEMAND_SLIDE_PULSE://滑差时间脉冲
    if(OUTPUT_DEMAND_FLAG==Mode)
    {
      if(1==Mode_Word.Mode[2].Bit.Bit4)
        return 1;
      else
        return 0;
    }
    else 
      return 0;    
  case OUTPUT_POWER_WARN://负荷控制报警脉冲,负控报警和电表以及电网状态报警，共用一个端子
    if(1==Mode_Word.Mode[1].Bit.Bit4)//使用负控功能
    {
      return 1; 
    }
    else
      return 0;
  case OUTPUT_METER_GRID_WARN://电表以及电网状态报警
    if(0==Mode_Word.Mode[1].Bit.Bit4)//负控功能关闭
    {
      //if(1==Mode_Word.Mode[3].Bit.Bit0)//允许报警输出,因为有独立的报警端子，所以该处不判
      {
        return 1; 
      }
      //else
        //return 0;
    }
    else
      return 0;
  //case OUTPUT_OWN_MONEY_WARN://有独立的端子，所以允许输出
    return 1;
  //case OUPPUT_OWN_MONEY_TRIP://有独立的端子，所以允许输出
    return 1;
  }
  return 0;
}
*/
/*
//脉冲输出级的毫秒钩子函数
void Meter_Info_Output_Timer_Hook()
{
  static INT8U Counts;
  
  
  
}
//表计信息输出
void Meter_Info_Output(INT8U Flag)
{
  switch(Flag)
  {
    case OUTPUT_SEC_PULSE:
      break;
    case OUTPUT_TIME_CHANGE_PULSE://时段切换
      break;   
  } 
}
*/

void Check_Power_Ctrl_Para_Avail()
{
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(Mode_Word);
  if(ASSERT(A_WARNING,0!=Re))
  {
    Read_Power_Ctrl_Para_From_Rom();
  }
}

//负荷控制处理
void Power_Ctrl_Proc()
{
  INT8U Re;
  Re=CHECK_STRUCT_SUM(Mode_Word);
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();
  
  if(1==Mode_Word.Mode[1].Bit.Bit4)//模式字2的bit4表示是否有负控功能
  {
    //当前正向有功需量超!
    if(Demand_Accu.Cur_Pos_Active>Power_Ctrl_Para.AcDemand_Up_Limit)
    {
      if(2==Power_Ctrl_Para.User_Level)//1级用户不报警不跳闸，2级用户只报警不跳闸，3级用户既报警又跳闸
      {
        Set_Event_Instant(ID_EVENT_DEMAND_ALARM);//报警
        Port_Out_Pub(EXT_ID_POWER_ALARM,PORT_START);
      }
      else if(3==Power_Ctrl_Para.User_Level)//用户级别2或者3才有
      {
        Set_Event_Instant(ID_EVENT_DEMAND_ALARM);//报警
        Set_Event_Instant(ID_EVENT_DEMAND_SWITCH);//跳闸
        Port_Out_Pub(EXT_ID_POWER_ALARM,PORT_START);
        Port_Out_Pub(EXT_ID_TOGGLE,PORT_START);
      }
    }
    else//超需量恢复
    {
      if(2==Power_Ctrl_Para.User_Level)//1级用户不报警不跳闸，2级用户只报警不跳闸，3级用户既报警又跳闸
      {
        Clr_Event_Instant(ID_EVENT_DEMAND_ALARM);//报警
        Port_Out_Pub(EXT_ID_POWER_ALARM,PORT_END);        
      }
      else if(3==Power_Ctrl_Para.User_Level)//用户级别2或者3才有
      {
        Clr_Event_Instant(ID_EVENT_DEMAND_ALARM);//报警
        Clr_Event_Instant(ID_EVENT_DEMAND_SWITCH);//跳闸
        Port_Out_Pub(EXT_ID_POWER_ALARM,PORT_END);        
        Port_Out_Pub(EXT_ID_TOGGLE,PORT_END);        
      }
    }
  }
}