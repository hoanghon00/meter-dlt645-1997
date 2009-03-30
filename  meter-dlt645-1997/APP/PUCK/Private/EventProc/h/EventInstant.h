#ifndef EVENTINSTANT_VAR
#define EVENTINSTANT_VAR


#ifndef EVENTINSTANT_C
#define EVENTINSTANT_C_EXT  extern
#else
#define EVENTINSTANT_C_EXT 
#endif



#define      DI_LOSS_VOLT 0xCA10          //失压判定阀值
#define      DI_LOSS_CURR 0xCA11          //失流判定阀值

#define      ALL_LOSS_CURR_VALUE          0.02   //全失流阀值：0.02*In

//全失流阀值必须比失流电流阀值小
typedef struct{
  FP32S  LossVolt_Per;        //失压电压阀值,%-----CA10,取值后除100:0.0~1.0，默认：0.78
  FP32S LossVolt_I;           //失压电流判定值:A，默认：0.1
  
  FP32S  LossCurr_Per;       //失流电压阀值,%-----CA11,取值后除100:0.0~1.0，默认：0.3
  FP32S LossCurr_I;          //失流电流判定值:A，默认：0.1
  INT8U CS[CS_BYTES];
}EventInsPara;         //事件处理状态



EVENTINSTANT_C_EXT EventInsPara EventInsParaVar;


#define KEY_INIT_STATUS  0x35
typedef struct{
  INT8U Prg;            //编程
  INT8U CS[CS_BYTES];
}KeyStatus;         //事件处理状态

EVENTINSTANT_C_EXT KeyStatus  KeyInitStatus;


#define MAX_CHANGE_SEC    8     //变位判断时间
typedef struct
{ 
  INT8U Head;
  INT8U OldStatus;           //旧状态
  INT8U NewStatus;           //新状态
  INT8U Counts;             //计数器
  INT8U Tail;
}EVENT_CHANGE;
EVENTINSTANT_C_EXT volatile EVENT_CHANGE Loss_Volt_Change;
EVENTINSTANT_C_EXT volatile EVENT_CHANGE Loss_Curr_Change;
EVENTINSTANT_C_EXT volatile EVENT_CHANGE Curr_Neg_Change;

INT8U Get_Event_Instant(INT8U Id);
void Check_Volt_Curr_Seq(void);
void  Check_Overload_Volt(void);
void  Check_Loss_Curr(void);
void  Check_Loss_Parse(void);
void Check_Loss_Volt(void);
void Check_RTCBat_Low(void);
void Check_LPWBat_Low(void);
void Check_Prg_Key(void);
void Check_Fac_Status(void);
void Check_Key_Switch(void);
void Init_Event_Instant_Para(void);
void Check_Neg_Curr(void);
void Check_UpCover(void);
void Check_DownCover(void);
void Check_AB_Func_Key(void);
void Check_MeterAddr_Key(void);
void Set_Cal_Event(void);
void Event_Para_Modi(void);
void Check_ClrDemand_Key(void);
void Get_Loss_Volt_Change(void);
void Get_Loss_Curr_Change(void);
void Get_Curr_Neg_Change(void);
void Monitor_Meter_System_Osc(void);
void Get_Meter_PowerOn(void);
#endif