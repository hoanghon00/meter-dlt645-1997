#ifndef ALARM_VAR
#define ALARM_VAR


#ifndef ALARM_C
#define ALARM_C_EXT  extern
#else
#define ALARM_C_EXT 
#endif



typedef struct
{
  INT8U  PortID;
  INT8U  AlarmBit;
  const  INT8U *AlarmEn;      //声音报警,LCD_LED报警，端子排报警
  INT16U  AlarmWidth;          //输出脉冲宽度：ms
}ALARM_OUT_EN;


CONST ALARM_C_EXT ALARM_OUT_EN Alarm_Mode_Property[]=
{
  INTER_ID_ALARM_BEEP,2, (INT8U *)(&(Mode_Word.Mode[3])),300,     //声音报警允许模式字
  INTER_ID_ALARM_DIS,1, (INT8U *)(&(Mode_Word.Mode[3])),300,      //LED报警模式字
  EXT_ID_POWER_ALARM,0, (INT8U *)(&(Mode_Word.Mode[3])),80              //电表及电网状态报警模式字
};

#define ALARM_MODE_NUM (sizeof(Alarm_Mode_Property)/sizeof(ALARM_OUT_EN))


typedef struct
{
  INT8U  EventID;
  const  INT8U *EventEn;      //报警输出使能
  INT8U  EventBit;
}ALARM_OUT_EVENT;

CONST ALARM_C_EXT ALARM_OUT_EVENT Alarm_Event_Property[]={
#ifdef ID_EVENT_ALL_LOSS_CURR
  {ID_EVENT_ALL_LOSS_CURR,(INT8U *)(&(Mode_Word.Mode[4])),7},
#endif

#ifdef ID_EVENT_A_NEG_CURR
{ID_EVENT_A_NEG_CURR,(INT8U *)(&(Mode_Word.Mode[4])),6},
#endif

#ifdef ID_EVENT_B_NEG_CURR
{ID_EVENT_B_NEG_CURR,(INT8U *)(&(Mode_Word.Mode[4])),6},
#endif

#ifdef ID_EVENT_C_NEG_CURR
{ID_EVENT_C_NEG_CURR,(INT8U *)(&(Mode_Word.Mode[4])),6},
#endif
    
#ifdef ID_EVENT_VOLT_NEG_SEQ
{ID_EVENT_VOLT_NEG_SEQ,(INT8U *)(&(Mode_Word.Mode[4])),5},
#endif   
 
#ifdef ID_EVENT_A_LOSS_CURR
{ID_EVENT_A_LOSS_CURR,(INT8U *)(&(Mode_Word.Mode[4])),4},
#endif   

#ifdef ID_EVENT_B_LOSS_CURR
{ID_EVENT_B_LOSS_CURR,(INT8U *)(&(Mode_Word.Mode[4])),4},
#endif

#ifdef ID_EVENT_C_LOSS_CURR
{ID_EVENT_C_LOSS_CURR,(INT8U *)(&(Mode_Word.Mode[4])),4},
#endif

#ifdef ID_EVENT_A_LOSS_VOLT
{ID_EVENT_A_LOSS_VOLT,(INT8U *)(&(Mode_Word.Mode[4])),3},
#endif

#ifdef ID_EVENT_B_LOSS_VOLT
{ID_EVENT_B_LOSS_VOLT,(INT8U *)(&(Mode_Word.Mode[4])),3},
#endif

#ifdef ID_EVENT_C_LOSS_VOLT
{ID_EVENT_C_LOSS_VOLT,(INT8U *)(&(Mode_Word.Mode[4])),3},
#endif

#ifdef ID_EVENT_AB_LOSS_VOLT
{ID_EVENT_AB_LOSS_VOLT,(INT8U *)(&(Mode_Word.Mode[4])),3},
#endif

#ifdef ID_EVENT_BC_LOSS_VOLT
   {ID_EVENT_BC_LOSS_VOLT,(INT8U *)(&(Mode_Word.Mode[4])),3},
#endif

#ifdef ID_EVENT_AC_LOSS_VOLT
   {ID_EVENT_AC_LOSS_VOLT,(INT8U *)(&(Mode_Word.Mode[4])),3},
#endif


#ifdef ID_EVENT_ABC_LOSS_VOLT
   {ID_EVENT_ABC_LOSS_VOLT,(INT8U *)(&(Mode_Word.Mode[4])),3},
#endif


#ifdef ID_EVENT_A_LOSS_PARSE
   {ID_EVENT_A_LOSS_PARSE,(INT8U *)(&(Mode_Word.Mode[4])),3},
#endif


#ifdef ID_EVENT_B_LOSS_PARSE
   {ID_EVENT_B_LOSS_PARSE,(INT8U *)(&(Mode_Word.Mode[4])),3},
#endif


#ifdef ID_EVENT_C_LOSS_PARSE
   {ID_EVENT_C_LOSS_PARSE,(INT8U *)(&(Mode_Word.Mode[4])),3},
#endif

#ifdef ID_EVENT_A_VOLT_OVERLOAD
   {ID_EVENT_A_VOLT_OVERLOAD,(INT8U *)(&(Mode_Word.Mode[4])),3},
#endif

#ifdef ID_EVENT_B_VOLT_OVERLOAD
   {ID_EVENT_B_VOLT_OVERLOAD,(INT8U *)(&(Mode_Word.Mode[4])),3},
#endif

#ifdef ID_EVENT_C_VOLT_OVERLOAD
   {ID_EVENT_C_VOLT_OVERLOAD,(INT8U *)(&(Mode_Word.Mode[4])),3},
#endif


#ifdef ID_EVENT_RTCERR_RUN
{ID_EVENT_RTCERR_RUN,(INT8U *)(&(Mode_Word.Mode[4])),2},
#endif


#ifdef ID_EVENT_EPPROM_FAULT
{ID_EVENT_EPPROM_FAULT,(INT8U *)(&(Mode_Word.Mode[4])),1},
#endif

#ifdef ID_EVENT_RTC_BAT_LOW
{ID_EVENT_RTC_BAT_LOW,(INT8U *)(&(Mode_Word.Mode[4])),0},
#endif


#ifdef ID_EVENT_LOWPWR_BAT_LOW
{ID_EVENT_LOWPWR_BAT_LOW,(INT8U *)(&(Mode_Word.Mode[4])),0}
#endif
};

#define ALARM_EVENT_NUM (sizeof(Alarm_Event_Property)/sizeof(ALARM_OUT_EVENT))

#endif