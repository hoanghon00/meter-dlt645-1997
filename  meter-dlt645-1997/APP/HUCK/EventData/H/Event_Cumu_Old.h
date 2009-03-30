#ifndef EVENT_CUMU_H
#define EVENT_CUMU_H

#undef EXT
#ifdef EVENT_CUMU_C
#define EXT
#else
#define EXT extern
#endif

#define PHASE_A   0x01
#define PHASE_B   0x02
#define PHASE_C   0x04
#define PHASE_AB  0x03
#define PHASE_AC  0x05
#define PHASE_BC  0x06
#define PHASE_ABC 0x07
#define PHASE_ALL 0x08

#define NULL_EVENT 0xFF//事件发生标志，次宏不能取0，不能和任何一个Event_ID重合

//定义分相事件累加信息
typedef struct
{
  INT32U Counts;//失压次数 
  INT32U Time;//失压累计时间
  
  INT8U CS[CS_BYTES];
}S_Phase_Cumu;

//失压累计数据
typedef struct
{
  INT32U Counts;//失压次数 
  INT32U Time;//失压累计时间
  
  INT32U Phase_Pos_Active[4];//总、A、B、C相累计有功电量
  INT32S Phase_Pos_Reactive[4];//总、A、B、C三相正向无功，组合无功1
  INT32U Phase_Neg_Active[4];//总、A、B、C三相反向有功
  INT32S Phase_Neg_Reactive[4];//总、A、B、C三相反向无功，组合无功2
  INT32U Phase_AH[3];//A、B、C三相安培时间
  
  INT8U CS[CS_BYTES]; 
}S_Loss_Vol_Cumu;


//失流累计数据
typedef struct
{
  INT32U Counts;//失压次数 
  INT32U Time;//失压累计时间
  
  INT32U Phase_Pos_Active[4];//总、A、B、C相累计有功电量
  INT32S Phase_Pos_Reactive[4];//总、A、B、C三相正向无功，组合无功1
  INT32U Phase_Neg_Active[4];//总、A、B、C三相反向有功
  INT32S Phase_Neg_Reactive[4];//总、A、B、C三相反向无功，组合无功2
  //INT32U Phase_AH[3];//A、B、C三相安培时间
  
  INT8U CS[CS_BYTES]; 
}S_Loss_Cur_Cumu;

//反向期间累计数据
typedef struct
{
  INT32U Counts;
  INT32U Time;
  
  INT32U Phase_Pos_Active[4];//总、A、B、C相累计有功电量
  INT32U Phase_Neg_Active[4];//总、A、B、C三相反向有功
  INT8U CS[CS_BYTES];
}S_Reverse_Cumu;//


//全失压，全失流、断相累计数据
typedef struct
{
  INT32U Counts;
  INT32U Time;
  INT8U CS[CS_BYTES];
}S_Phase_Break_Cumu;

typedef struct
{
  INT8U Loss_Vol_Flag;      //失压标志
  INT8U Loss_Cur_Flag;      //失流标志
  INT8U All_Loss_Cur_Flag;  //全失流标志，全失流和某相失流是可能同时发生的，因此独立一个表标志
  
  INT8U Phase_Reverse_Flag; //反向标志
  INT8U Phase_Break_Flag;   //断相标志

  INT8U Loss_Vol_Flag_Bak;      //失压标志
  INT8U Loss_Cur_Flag_Bak;      //失流标志
  INT8U All_Loss_Cur_Flag_Bak;  //全失流标志，全失流和某相失流是可能同时发生的，因此独立一个表标志
  
  INT8U Phase_Reverse_Flag_Bak; //反向标志
  INT8U Phase_Break_Flag_Bak;   //断相标志 
  
  INT8U CS[CS_BYTES];
}S_Event_Flag;

//总的事件累加量
typedef struct
{
  INT8U Head;
  
  S_BCD_Time Event_Time;
  INT8U Temp0;//不用，保证上面的结构体对齐
  
  EVENT_INSTANT_STATUS Meter_Instant_Status;//事件状态信息
  INT8U Temp1;//不用保证上面的结构体对齐
 
  S_Prog_Flag Prog_Flag;//编程记录
  
  S_Event_Flag Event_Flag; //失压失流等的发生标志

  S_Phase_Cumu Phase_Loss_Vol[9];//总、A、B、C和AB、AC、BC、ABC、全失压、
  S_Phase_Cumu Phase_Loss_Cur[8];//总、A、B、C和AB、AC、BC、全失流
  S_Phase_Cumu Phase_Break[4];//三相断相(不考虑AB,BC,AC,ABC的情况)总、A、B、C
  S_Reverse_Cumu Phase_Reverse[4];//三相反向(不考虑AB,BC,AC,ABC的情况)总、A、B、C

  
  S_Loss_Vol_Cumu Loss_Vol;//失压期间累计电能等,必须考虑合相的情况
  S_Loss_Cur_Cumu Loss_Cur;//失流期间累计电能等

  INT8U CS[CS_BYTES];
  INT8U Tail;
}S_Event_Cumu;

typedef struct
{
  INT32S Pos_Reactive[4];//A、B、C、总正向无功  
  INT32U Neg_Reactive[4];//A、B、C、总反向无功
}S_Phase_Reactive;


EXT NO_INIT volatile S_Event_Cumu Event_Cumu;     //事件累加数据
EXT volatile S_Phase_Reactive Cumu_Phase_Reactive; //分相组合无功累加数据

EXT void Calc_Total_Loss_Vol();
EXT void Calc_Total_Loss_Cur();
EXT void Calc_Total_Phase_Break();
EXT void Calc_Total_Phase_Reverse();

EXT INT8U Check_Phase_Event(INT8U Flag,INT8U Phase);
EXT void Event_Cumu_Proc(INT8U Event_ID,INT8U Occur_Or_End);
EXT void Read_Event_Cumu_Data_From_Rom();
//EXT void Save_Event_Cumu_PD_Data();
//EXT void Event_Energy_Cumu(MEASU_ENERG *pEnergy);
EXT INT8U Check_Event_Occuring(INT8U Event_ID);
//EXT void Save_Event_Cumu_Bak_Data();
EXT void Set_Def_Event_Cumu_Data();

EXT void Init_Event_Cumu_Data();

#endif
