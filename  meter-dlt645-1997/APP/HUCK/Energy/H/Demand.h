#ifndef DEMAND_H
#define DEMAND_H

#undef EXT
#ifdef DEMAND_C
#define EXT
#else
#define EXT extern
#endif
/*
需量数据存储的特点:
CUR_DEMAND总以及所有费率的数据
CUR_DEMAND_BAK存储格式和CUR_DEMAND一样
CUR_DEMAND_PD主要是掉电存储,只存储S_Demand结构体Cur_Demand
HIS_DEMAND主要存储前N个月的数据，每个数据项和CUR_DEMAND格式一样
*/
#define MAX_PERIOD_SLIDE_RATIO 15//需量周期/滑差时间的最大值

#define POWER_DIRECT_N 0//定义功率方向正向
#define POWER_DIRECT_P 1//反向

#define POWER_DATA_RATIO 100

#define DEMAND_RATE_OFF(x) ((ONE_DEMAND_SAVE_SIZE+ROM_CS_BYTES)*(INT32U)x)//计算某个费率的需量数据在整个需量存储区中的偏移
//需量相关参数
typedef struct
{
  INT8U Period;//最大需量时间
  INT8U Slide_Time;//滑差时间
  
  INT8U CS[CS_BYTES];	
}S_Demand_Para;

typedef struct
{
  //INT8U Period;//需量周期
  //INT8U Slide_Time;//滑差时间
  
  INT32U Pos_Active[MAX_PERIOD_SLIDE_RATIO];//正向有功15个功率累加单元,单位0.0001kw
  INT32U Neg_Active[MAX_PERIOD_SLIDE_RATIO];//反向有功15个功率累加单元,单位0.0001kw
  INT32U Pos_Reactive[MAX_PERIOD_SLIDE_RATIO];//正向无功15个功率累加单元,单位0.0001kvar
  INT32U Neg_Reactive[MAX_PERIOD_SLIDE_RATIO];//反向无功15个功率累加单元,单位0.0001kvar
  INT16U Counts[MAX_PERIOD_SLIDE_RATIO];//每个累加单元的累加次数计数
  INT8U Mins[MAX_PERIOD_SLIDE_RATIO];//每个单元累加了多少分钟?
  //INT8U Init_Flag[MAX_PERIOD_SLIDE_RATIO];//初始化标志，经过初始化后，置为1，第一次算需量对counts的修整
  
  INT32U Cur_Pos_Active;//当前正有需量,单位0.0001kw
  INT32U Cur_Neg_Active;//当前反有需量,单位0.0001kw
  INT32U Cur_Pos_Reactive;//当前正无需量,单位0.0001kvar
  INT32U Cur_Neg_Reactive;//当前反无需量,单位0.0001kvar
  
  INT8U CS[CS_BYTES];
}S_Demand_Accu;//功率累加

#ifdef DEMAND_C
CONST S_Demand_Para Def_Demand_Para={0x15,1,{0}};
#endif

//定义一条需量数据项，该数据结构通常用来定义某个费率或总的所有需量数据
typedef struct
{
  //INT8U Time[5];//冻结时间点，表示该数据的冻结时间点,冻结完成后，马上更新为下次冻结的时间点
  S_HEX_Time Time;
  
  INT8U Temp;//无用，用于上面结构体对齐
  //正向有功及其发生时间
  INT32U Pos_Active;
  INT8U Pos_Active_Time[5];
  //反向有功及其发生时间
  INT32U Neg_Active;
  INT8U Neg_Active_Time[5];
  //四象限无功及其发生时间
  INT32U Quad_Reactive[4];
  INT8U Quad_Reactive_Time[4][5];
  //正向无功及其发生时间
  INT32U Pos_Reactive;
  INT8U Pos_Reactive_Time[5];
  //反向无功及其发生时间
  INT32U Neg_Reactive;
  INT8U Neg_Reactive_Time[5];
  //定义校验和
  INT8U CS[CS_BYTES];
}S_One_Demand;

//需量数据结构体，表示每个费率的需量信息
typedef struct 
{
  INT8U Head;//头
  
  INT8U Rate;//当前费率
  S_One_Demand Demand[2];//定义总和当前费率的需量数据 
  
  INT32U Data_Ready_Flag;   
  
  INT8U CS[CS_BYTES];
  
  INT8U Tail;//尾
}S_Demand;//

EXT volatile S_Demand_Para Demand_Para;
EXT volatile S_Demand_Accu Demand_Accu;
EXT NO_INIT volatile S_Demand Cur_Demand;     //当前最大需量数据结构体

EXT volatile INT8U Clear_Demand_Way;//请需量方式

EXT INT8U Check_Demand_Data(S_Demand *pDemand);
EXT void Read_Demand_Para_From_Rom();
EXT void Read_Demand_Data_From_Rom();
EXT INT16U Read_Demand_Rate_Data(INT8U Rate,void *pDst,void *pDst_Start,INT16U DstLen);
EXT void Save_Cur_Demand_Data();
EXT void Save_Cur_Demand_PD_Data();
EXT void Save_Cur_Demand_Bak_Data();
EXT void Set_Def_Cur_Demand_Data();
EXT void Set_Def_Demand_Data();
EXT void Check_Man_Clr_Demand();
EXT void Freeze_Demand_Data(S_HEX_Time *pTime);
EXT void Demand_Check_Rate_Change();
EXT void Check_Demand_Para_Avail();
EXT void Demand_Calc();
EXT void Demand_Accu_Clear();
EXT void Demand_Clear(S_HEX_Time *pTime);
EXT void Demand_Proc();
EXT void Clear_Demand_Record();
EXT void Debug_Print_Time(INT8U Time[]);
EXT void Debug_Print_HEX_Time(INT8U Time[]);
EXT void PowerOn_Demand_Proc();
EXT void Check_Demand_Data_Avail();
EXT void Clear_Cur_Demand_Data();
EXT void Clear_All_Demand_Data();
EXT void Get_Last_Freeze_Time(S_HEX_Time *pSrc,S_HEX_Time *pDst);
EXT void Get_Next_Freeze_Time(S_HEX_Time *pSrc,S_HEX_Time *pDst);
EXT void Get_NowMonth_Freeze_Time(S_HEX_Time *pSrc,S_HEX_Time *pDst);
EXT void Set_One_Demand_Time(S_One_Demand *pSrc,S_HEX_Time *pTime);
EXT void PowerOn_Demand_Data_Freeze();
EXT void PowerDown_Save_Demand_Data();
EXT void Debug_Print_Cur_Demand_Info();
#endif
