
#ifndef TASKPRG_CFG_H
#define TASKPRG_CFG_H

#ifdef  TASKPRG_CFG_C
#define TASK_EXT
#else
#define TASK_EXT extern
#endif

#include "includes.h"

//-------------------------------主任务-------------------------------//
#define TASK_STK_SIZE_main   190//160
TASK_EXT NO_INIT OS_STK     AppStartTaskStk_main[TASK_STK_SIZE_main];	


//------------------------------计量任务(PUCK)-------------------------------//
#define  TASK_MEASU_PUCK_EN
#ifdef TASK_MEASU_PUCK_EN
#define TASK_MEASU_SIZE_PUCK   130//160
TASK_EXT NO_INIT OS_STK     AppMeasuTaskStk_PUCK[TASK_MEASU_SIZE_PUCK];	
#endif

//------------------------------通道维护任务,包含了事件任务(PUCK)-------------------------------//
//#define  TASK_CHANNEL_PUCK_EN
#ifdef TASK_CHANNEL_PUCK_EN
#define TASK_CHANNEL_SIZE_PUCK   160
TASK_EXT NO_INIT OS_STK     AppChannelTaskStk_PUCK[TASK_CHANNEL_SIZE_PUCK];	
#endif


//------------------------------事件任务(PUCK)-------------------------------//
#define  TASK_EVENT_PUCK_EN
#ifdef TASK_EVENT_PUCK_EN
#define TASK_EVENT_STK_SIZE_PUCK   185//160
TASK_EXT NO_INIT OS_STK     AppEventTaskStk_PUCK[TASK_EVENT_STK_SIZE_PUCK];	
#endif


//-------------------------------冻结任务(HUCK)-------------------------------//
//#define  TASK_FREEZ_HUCK_EN
#ifdef TASK_FREEZ_HUCK_EN
#define TASK_FREEZ_SIZE_HUCK   256
TASK_EXT NO_INIT OS_STK     AppFreezTaskStk_HUCK[TASK_FREEZ_SIZE_HUCK];	
#endif


//-------------------------------电能任务(HUCK)-------------------------------//
#define  TASK_ENERGE_HUCK_EN
#ifdef TASK_ENERGE_HUCK_EN
#define TASK_ENERGE_SIZE_HUCK   160
TASK_EXT NO_INIT OS_STK     AppEnergTaskStk_huck[TASK_ENERGE_SIZE_HUCK];	
#endif




//------------------------------LCD/KEY任务(LUCK)-------------------------------//
#define  TASK_LCD_LUCK_EN
#ifdef TASK_LCD_LUCK_EN
#define TASK_LCD_SIZE_LUCK   275 //300
TASK_EXT NO_INIT OS_STK     AppLCDTaskStk_LUCK[TASK_LCD_SIZE_LUCK];	
#endif

#ifdef TEST_MODE
#define TASK_TEST_SIZE_HUCK   160
TASK_EXT NO_INIT OS_STK     AppTestTaskStk_HUCK[TASK_TEST_SIZE_HUCK];	
#endif

#ifdef TASKPRG_CFG_C
TASK_EXT S_Int16U Main_Task_Init_Clr_Dog_Counts={CHK_BYTE,0x00,CHK_BYTE};
#endif
extern S_Int16U Main_Task_Init_Clr_Dog_Counts;

void Clear_All_Dog(void);
void Creat_Energy_Demand_Task(void);
void Creat_AllSub_Tasks(void);
void Measu_Main_Puck(INT8U Flag);
void LCD_main_LUCK (INT8U Flag);
void Channel_Main_PUCK(INT8U Flag);
void Event_main_PUCK(INT8U Flag);
void  Main_Task_PUB(void *p_arg);
void  AppMeasuTask_PUCK (void *p_arg);
void  AppEnergeTask_HUCK (void *p_arg);
void  AppLCDTask_LUCK (void *p_arg);
void  AppTestTask_HUCK (void *p_arg);
void  AppEventTask_PUCK(void *p_arg);
void Clr_Ext_Inter_Dog(void);
#endif
