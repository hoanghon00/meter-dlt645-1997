#define OS_PORT_C
#include "OS_Core.h"

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
