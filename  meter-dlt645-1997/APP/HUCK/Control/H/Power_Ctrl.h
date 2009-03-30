#ifndef POWER_CTRL_H
#define POWER_CTRL_H

#undef EXT
#ifdef POWER_CTRL_C
#define EXT
#else
#define EXT extern
#endif
/*
//所有PORT设备ID
#define EXT_ID_ALARM        0  //报警输出
#define EXT_ID_TOGGLE       1  //跳闸输出
#define EXT_ID_ACPOWER      2  //有功脉冲
#define EXT_ID_REACPOWER    3  //无功脉冲
#define EXT_ID_AP_DIR       4  //有功脉冲方向
#define EXT_ID_RP_DIR       5  //无功脉冲方向
#define EXT_ID_SEC_EN       6  //秒脉冲使能
#define EXT_ID_DEMAND       7  //需量周期输出
#define EXT_ID_PARSE        8  //时段切换输出
#define GERNAL_ID_ACPOWER    9 //总有功脉冲
#define GERNAL_ID_REACPOWER  10  //总无功脉冲
#define INTER_ID_ALARM_DIS   11  //报警指示灯
#define INTER_ID_ALARM_BEEP  12  //报警喇叭
*/
#define OUTPUT_TIME_CHANGE_FLAG 0x01//输出时段切换信号标志
#define OUTPUT_DEMAND_FLAG 0x02//输出需量周期或者滑差信号

#define OUTPUT_SEC_PULSE 0//输出秒脉冲 
#define OUTPUT_TIME_CHANGE_PULSE 1//时段切换脉冲
#define OUTPUT_DEMAND_PERIOD_PULSE 2//需量周期脉冲
#define OUTPUT_DEMAND_SLIDE_PULSE 3//需量滑差脉冲
#define OUTPUT_POWER_WARN 4//负荷控制报警
#define OUTPUT_METER_GRID_WARN 5//电网以及电表状态报警
#define OUTPUT_OWN_MONEY_WARN 6//欠费报警
#define OUPPUT_OWN_MONEY_TRIP 7//欠费条闸

//定义有功功率上限
typedef struct
{
  INT32U AcDemand_Up_Limit;//有功需量上限
  INT8U User_Level;//用户级别
  INT8U CS[CS_BYTES];
}S_Power_Ctrl_Para;

//定义输出脉冲标志
typedef struct
{
  INT32U Pulse_Flag;
  INT8U CS[CS_BYTES];  
}S_Output_Pulse;

EXT volatile S_Power_Ctrl_Para Power_Ctrl_Para;
EXT volatile S_Output_Pulse Output_Pulse;//脉冲输出标志

EXT void Read_Power_Ctrl_Para_From_Rom();
EXT void Check_Power_Ctrl_Para_Avail();
EXT void Power_Ctrl_Proc();
#endif
