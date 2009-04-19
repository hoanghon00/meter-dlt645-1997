#ifndef RTC_ABS_H
#define RTC_ABS_H



#ifndef RTC_ABS_PUCK
#define RTC_ABS_PUCK_EXT  extern
#else
#define RTC_ABS_PUCK_EXT 
#endif


#define EXT_RTC_OK            0
#define EXT_RTC_FIRST_PWRON   1
#define EXT_RTC_COUNT_STOP    2
#define EXT_RTC_BUSY          4  //RTC 忙
#define EXT_RTC_I2C_ERR       8  //RTC I2c错


#define ID_EXT_RTC    0     //外部硬件时钟
#define ID_CPU_RTC    1     //CPU内部时钟
#define ID_RAM_RTC    2     //备份在SRAM的时钟
#define ID_SLEEP_RTC  3     //低功耗下的sram时钟


typedef struct
{
  INT8U Sec;
  INT8U Min;
  INT8U Hour;
  INT8U Week;//0表示周日,1-6分别表示周1->周6
  INT8U Date;
  INT8U Month;
  INT8U Year; 
  INT8U cS;  
}CPU_RTC;

//时间定义结构体
typedef struct
{
  INT8U Head;
  CPU_RTC RTC;
  INT8U Tail;
}RTC_BCD_Time;
NO_INIT RTC_ABS_PUCK_EXT volatile RTC_BCD_Time  CPU_RTC_Time;


INT8U DS3231_Read_Buf(INT8U addr,INT8U Len,INT8U *Dst);
INT8U DS3231_Write_Buf(INT8U addr,INT8U Len,INT8U *Src);

INT8U Write_ExtRTC_PUCK(INT8U *Src);
INT8U Read_ExtRTC_PUCK(INT8U *Dst,INT8U DstLen);
void Init_ExtRTC_PUCK(void);
INT8U Read_InterRTC_PUCK(INT8U *Dst,INT8U DstLen);
INT8U Write_InterRTC_PUCK(INT8U *Src);
void Init_ExtRTC_Pulse(INT8U Flag);
INT8U Read_Ext_RTC_Status(void);
void Refresh_CPU_RTC(void);
INT8U Get_Soft_RTC_PUCK(INT8U *Dst,INT8U DstLen);
void Set_Soft_RTC_PUCK(INT8U Flag);
#endif
