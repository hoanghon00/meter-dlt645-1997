#ifndef VOLSTAT_H
#define VOLSTAT_H

#undef EXT
#ifdef VOLSTAT_C
#define EXT
#else
#define EXT extern
#endif

#define VOL_RATIO 100

typedef struct{
 INT32U Qual_Upper_Limit;//合格上限
 INT32U Qual_Lower_Limit;//合格下限
 INT32U Exam_Upper_Limit;//考核上限
 INT32U Exam_Lower_Limit;//考核下限
 INT8U CS[CS_BYTES];
}S_VolStat_Para;


typedef struct{
  INT8U Head;
  
  S_HEX_Time Time;
  
  INT8U Temp;//无用，用于上面结构体对齐
  
  INT32U Total_Time[3];//三相监测总时间
  INT32U Upper_Time[3];//超上时间
  INT32U Lower_Time[3];//超下时间
  
  INT32U Qual[3];//三相合格率
  
  INT32U Data_Ready_Flag;
  
  INT8U CS[CS_BYTES];
  
  INT8U Tail;
}S_VolStat;

typedef struct{
  INT8U Qual_Upper[3];
  INT8U Qual_Lower[3];//超合格计数
  INT8U Exam_Upper[3];
  INT8U Exam_Lower[3];//超考核计数  
  
  INT8U CS[CS_BYTES];
}S_VolStat_Count;

typedef struct
{
  INT32U Qual_Up_Vol[3];//越合格上限电压值,0-2分别表示A-B-C相
  INT32U Qual_Low_Vol[3];//越合格下限电压值
  
  INT32U Over_Vol[3];//电压超上限电压值
  
  INT8U CS[CS_BYTES];
  INT32U Pub_Vol;
  INT32U Pub_Cur;
}S_Event_Vol_Cur;

EXT NO_INIT S_VolStat Cur_VolStat;//当月电压检测数据
EXT NO_INIT S_VolStat Total_VolStat;//电压检测总数据
EXT NO_INIT S_VolStat_Para VolStat_Para;//电压统计参数
EXT S_VolStat_Count VolStat_Count;//电压统计计数
EXT S_Event_Vol_Cur Event_Vol_Cur;

EXT INT8U Check_VolStat_Data(S_VolStat *pVolStat);
EXT void Freeze_VolStat_Data(S_HEX_Time *pTime);
EXT void Save_Cur_VolStat_PD_Data();
EXT void Save_Cur_VolStat_Data();
EXT void Vol_Stat_Proc();
EXT void Set_Def_Cur_VolStat_Data();
EXT void Set_Def_VolStat_Data();
EXT void Read_VolStat_Data_From_Rom();
EXT void PowerDown_Save_VolStat_Data();
EXT void PowerOn_VolStat_Data_Freeze();
EXT void Read_VolStat_Para_From_Rom();
EXT void Clear_VolStat_Data();
EXT void Check_VolStat_Data_Avail();
EXT void Check_VolStat_Para_Avail();
EXT void VolStat_Proc();
EXT void Init_VolStat_Ram_Para();
#endif
