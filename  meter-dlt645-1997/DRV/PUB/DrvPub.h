#ifndef DRV_CFG_H
#define DRV_CFG_H



#ifndef DRV_PUB
#define DRV_PUB_EXT  extern
#else
#define DRV_PUB_EXT 
#endif



#include "includes.h"

#include "DrvCfg.h"
#include "TAU.h"
#include "Int.h"
#include "LVI.h"

#include "CPUPub.h"
#include "Epp_Soft.h"
#include "IIC0_Hard.h"
#include "I2C_Soft.h"
#include "Epp_Hard.h"
#include "DS3231_soft.h"
#include "App_Epp_All.h"
#include "ICcardDrv.h"


#define DIR_OUTPUT  0
#define DIR_INPUT   1

DRV_PUB_EXT volatile INT32U Timeout_Ms_Timer;
DRV_PUB_EXT volatile INT8U  Timeout_Ms_Num;

//定义驱动为带条件的超时，condition为1或者mstimeout超时，将退出,mstimeout内部限定在 OS_MS_PER_TICK内
#define  IF_WAITFOR_DRV_MS_CONDITION(Condition,MsTimeout) \
do{\
    if(Condition)  break;   \
    if(MsTimeout>OS_MS_PER_TICK) Timeout_Ms_Num=OS_MS_PER_TICK; else Timeout_Ms_Num=MsTimeout+1;\
      Timeout_Ms_Timer=Ms_Timer_Pub;\
    while((Timeout_Ms_Num)&&(!(Condition)))\
     if(Timeout_Ms_Timer!=Ms_Timer_Pub) {Timeout_Ms_Num--; Timeout_Ms_Timer=Ms_Timer_Pub;} \
}while(0);\
if(!(Condition))    //检查判定是否超时，1表示已经超时,0表示条件成立，没有超时


//定义驱动的延时宏,入口：ms，内部范围限定在[1,OS_MS_PER_TICK]
#define  WAITFOR_DRV_MS_TIMEOUT(MsTimeout) {IF_WAITFOR_DRV_MS_CONDITION(0,MsTimeout);}
    


#define  CYCLE_NUM_IN_1MS     12000 //定义一个ms内 nop的个数
DRV_PUB_EXT volatile INT32U  Timeout_Cycle_Num;
//定义驱动为带条件的超时，condition为1或者nop个数超过，将退出
#define  IF_WAITFOR_DRV_CYCLE_CONDITION(Condition,cycle) \
do{\
    if(Condition)  break;   \
    if(cycle>CYCLE_NUM_IN_1MS) Timeout_Cycle_Num=CYCLE_NUM_IN_1MS/4; else if(cycle<=4) Timeout_Cycle_Num=1; else Timeout_Cycle_Num=cycle/4;\
      while((Timeout_Cycle_Num)&&(!(Condition)))    Timeout_Cycle_Num--;\
}while(0);\
if(!(Condition))    //检查判定是否超时，1表示已经超时,0表示条件成立，没有超时

//定义驱动的nop延时宏,入口：nop个数，内部范围限定在[4,CYCLE_NUM_IN_1MS]
#define  WAITFOR_DRV_CYCLE_TIMEOUT(cycle) {IF_WAITFOR_DRV_CYCLE_CONDITION(0,cycle);}


void Read_DS3231(INT8U *src);
INT8U Write_DS3231(INT8U *src);
void Drv_WaitFor_TimeOut(INT32U MsTimeout);
void Init_IIC_Soft(void);
void Init_DS3231_IIC_Soft(void);
void Init_U27_IIC_Soft(void);
void Close_DS3231_IIC_Soft(void);
void Close_U27_IIC_Soft(void);
#endif
