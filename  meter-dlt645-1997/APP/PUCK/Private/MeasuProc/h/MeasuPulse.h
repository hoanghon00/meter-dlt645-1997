#ifndef Pulse_h
#define Pulse_h


//#undef MeasuProc_PUCK_EXT
#ifndef MeasuPulse_PUCK
#define MeasuPulse_PUCK_EXT  extern
#else
#define MeasuPulse_PUCK_EXT 
#endif



#define DI_AC_PULSE     0xc030    //有功脉冲常数
#define DI_REAC_PULSE   0xc031    //无功脉冲常数
#define DI_PULSE_WIDTH   0xc211    //输出脉冲宽度：毫秒


typedef struct
{ 
  INT32U Ac_Pulse;               //有功脉冲
  INT32U Rea_Pulse;               //无功脉冲
  INT8U Ac_Pulse_Freq;           //有功脉冲分频系数
  INT8U Reac_Pulse_Freq;         //无功脉冲分频系数
  INT16U Pulse_Width;             //输出脉冲宽度：毫秒,有无功一致。
  INT8U CS[CS_BYTES];            //重要参数，SRAM加校验和
}PULSE_STRUC;                    //MeasuIC系统模式字


MeasuPulse_PUCK_EXT PULSE_STRUC Sys_Pulse_Var;

INT8U Get_Pulse_Para(void);
void Send_Acpower_Pulse(void);
void Send_Reacpower_Pulse(void);
#endif

