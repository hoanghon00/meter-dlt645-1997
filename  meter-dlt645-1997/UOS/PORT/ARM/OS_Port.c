#define OS_PORT_C
#include "OS_Core.h"


void OS_Tick_Task_Init()
{
  /*
  INT32U Tmr0_Rst_Value;
  Tmr0_Rst_Value = (BSP_Peripheral_Clk2_Freq/ (BSP_TMR0_PRESCALER + 1))/TMR0_TICKS_PER_SEC ;//TMR0_TICKS_PER_SEC;
  TIM_Init ( TIM0 );
  TIM_PrescalerConfig ( TIM0,BSP_TMR0_PRESCALER );
  TIM_ITConfig ( TIM0, TIM_OCA_IT, ENABLE );
  TIM_OCMPModeConfig ( TIM0, TIM_CHANNEL_A, Tmr0_Rst_Value, TIM_TIMING, TIM_HIGH );  
*/
}

//检查操作系统的一个Tick时间是否到,该函数仅在不实用中断产生Tick时查询Tick时间是否到用
//返回1表示一个Tick时间到，否则返回0
OS_INT8U OS_Check_Tick()
{
  return 1;
  /*
  if(TIM_FlagStatus(TIM0,TIM_OCFA)!=SET)
    return 0;
  
  TIM_FlagClear(TIM0,TIM_OCFA);//清中断标志与计数器
  TIM_CounterConfig(TIM0,TIM_CLEAR);//清计数器
  return 1;
*/  
}
#undef OS_PORT_C
