/*
*******************************************************************************
**
**  This device driver was created by Applilet2 for 78K0R/Kx3
**  16-Bit Single-Chip Microcontrollers
**
**  Copyright(C) NEC Electronics Corporation 2002 - 2008
**  All rights reserved by NEC Electronics Corporation.
**
**  This program should be used on your own responsibility.
**  NEC Electronics Corporation assumes no responsibility for any losses
**  incurred by customers or third parties arising from the use of this file.
**
**  Filename :	user_define.h
**  Abstract :	This file includes user define.
**  APIlib :	Applilet2 for 78K0R/Kx3 V2.30 [18 Oct. 2007]
**
**  Device :	uPD78F1166_A0
**
**  Compiler :	IAR_C
**
**  Creation date:	2008-6-10
**  
*******************************************************************************
*/
#ifndef _MDUSER_DEF_
#define _MDUSER_DEF_

#include "macrodriver.h"
#include "port.h"
#include "data_flash.h"
#include "macrodriver.h"
#include "Port.h"
#include "Serial.h"
#include "TAU.h"
#include "Int.h"
#include "RTC.h"
#include "WatchdogTimer.h"
#include "System.h"
#include "Int.h"

/*

*******************************************************************************
**  Macro define
*******************************************************************************
*/
/* Start user code for definition. Do not edit comment generated here */
#define SUCESS 0;



#define  CONSTANT_RTC_INT_ENABLE   //宏开启，开启中断
//#define  CONSTANT_RTC_INT_ONESEC   //宏开启，按每秒中断闹铃；否则按分闹铃


#define ALARM_BEEP_0  PORT_ChangeP142Output(0, 0 )//喇叭报警         
#define ALARM_BEEP_1  PORT_ChangeP142Output(0, 1 ) 

#define ALARM_PORT_0  PORT_ChangeP81Output( 0 )//报警端子---接继电器       
#define ALARM_PORT_1  PORT_ChangeP81Output( 1 ) 


//-----------------------------------------------------------
         
#define LCD_BACK_LIGHT_0  PORT_ChangeP152Output( 0 )//液晶背光        
#define LCD_BACK_LIGHT_1  PORT_ChangeP152Output( 1 ) 
         
#define LCD_CS_0  PORT_ChangeP153Output( 0 )//液晶的CS         
#define LCD_CS_1  PORT_ChangeP153Output( 1 ) 
         
#define LCD_DATA_0         PORT_ChangeP155Output( 0 )//液晶的DATA         
#define LCD_DATA_1         PORT_ChangeP155Output( 1 ) 
         
#define LCD_WR_0         PORT_ChangeP156Output( 0 )//液晶的WR         
#define LCD_WR_1         PORT_ChangeP156Output( 1 ) 

#define LCD_POW_0         PORT_ChangeP154Output( 0 )//液晶电源      
#define LCD_POW_1         PORT_ChangeP154Output( 1 ) 


#define POWER_OFF_STATUS   (P3_bit.no1)  //掉电检测状态--------------PUCK增加，08-10-10,测试OK
#define ALL_V_LOSS_STATUS   (P14_bit.no1)  //全失压下的IO口状态查询,测试OK




#define RELAY_PORT_0         PORT_ChangeP80Output( 0 )//跳闸开关    
#define RELAY_PORT_1         PORT_ChangeP80Output( 1 ) 

//----------------------------------------------------
//以下7022B端口模拟
//----------------------------------------------------
#define B_7022_CS_0  PORT_ChangeP130Output( 0 )//CS         
#define B_7022_CS_1  PORT_ChangeP130Output( 1 ) 

#define B_7022_RST_0  PORT_ChangeP20Output( 0 )//RST         
#define B_7022_RST_1  PORT_ChangeP20Output( 1 ) 

#define B_7022_SDO_0  PORT_ChangeP22Output( 0 )//SDO        
#define B_7022_SDO_1  PORT_ChangeP22Output( 1 ) 

#define B_7022_SCK_0  PORT_ChangeP24Output( 0 )//SCK       
#define B_7022_SCK_1  PORT_ChangeP24Output( 1 ) 

#define B_7022_POWER_0  PORT_ChangeP25Output( 0 )//POWER      
#define B_7022_POWER_1  PORT_ChangeP25Output( 1 ) 

#define B_7022_SIG_STATUS  (P2_bit.no1)//SIG
#define B_7022_SDI_STATUS  (P2_bit.no3)//SDI
//---------------------------------------------P2
#define B_ird_chose_0  PORT_ChangeP110Output( 0 )//红外选择  
#define B_ird_chose_1  PORT_ChangeP110Output( 1 ) 

#define B_ird_pow_0  PORT_ChangeP86Output( 0 )//红外电源控制-PUCK  
#define B_ird_pow_1  PORT_ChangeP86Output( 1 ) 

//-------------------------------------------------485
#define B_2_485_0  PORT_ChangeP15Output( 0 )//选择
#define B_2_485_1  PORT_ChangeP15Output( 1 ) 

#define B_1_485_0  PORT_ChangeP145Output( 0 )//选择  
#define B_1_485_1  PORT_ChangeP145Output( 1 ) 


//----------------------------------------------------wtd
#if METER_HARD_TYPE==HARD_TYPE_20081005
  #define B_7022VOL_TEST_STATUS  (P5_bit.no2)//7022基准指示
  #define B_WTD_0  P5_bit.no3=0          //OUT WTD  
  #define B_WTD_1  P5_bit.no3=1  
  #define B_TEST_HARD_STATUS  (P5_bit.no5)   //自检脚，生产自检程序运行------PUCK
  #define B_REED_TEST_STATUS  (P5_bit.no6)//干簧管
  #define OPEN_PROG_KEY  (P7_bit.no0)         //编程开关
  #define OPEN1_STATUS   (P7_bit.no1)        //开端盖(后端盖)铅封
  #define OPEN2_STATUS   (P7_bit.no2)        //开大盖(上端盖)
  //--------------用于按键的状态指示
  #define Inter_Left_Key_STATUS  (P7_bit.no3)
  //Down Key
  #define Inter_Down_Key_STATUS (P7_bit.no4)
  //Right key
  #define Inter_Right_Key_STATUS (P7_bit.no5)
  //UP Key
  #define Inter_Up_Key_STATUS (P7_bit.no6)

  #define ALARM_0  PORT_ChangeP54Output( 0 )//报警灯 
  #define ALARM_1  PORT_ChangeP54Output( 1 ) 

#endif

#if METER_HARD_TYPE>=HARD_TYPE_20090224

  #define SET_IC_CARD_IN_DIR       PM5_bit.no0=1
  #define SET_IC_CARD_FAIL_DIR     PM3_bit.no0=1
  #define SET_IC_CARD_SW_DIR       PM5_bit.no3=1 

  #define SET_IC_CARD_OUT_DIR      PM5_bit.no1=0
  #define SET_IC_CARD_RST_DIR      PM5_bit.no2=0
  #define SET_IC_CARD_CLK_DIR      PM8_bit.no7=0
  #define SET_IC_CARD_PWR_DIR      PM6_bit.no3=0 

  #define IC_CARD_IN   (P5_bit.no0)   //输入
  #define IC_CARD_FAIL (P3_bit.no0)   //输入
  #define IC_CARD_SW   (P5_bit.no3)   //输入

  #define IC_CARD_OUT_0  P5_bit.no1=0   //输出
  #define IC_CARD_OUT_1  P5_bit.no1=1   //输出
  #define IC_CARD_RST_0  P5_bit.no2=0   //输出
  #define IC_CARD_RST_1  P5_bit.no2=1   //输出
  #define IC_CARD_CLK_0  P8_bit.no7=0  //输出
  #define IC_CARD_CLK_1  P8_bit.no7=1  //输出
  #define IC_CARD_POWR_ON  P6_bit.no3=1   //输出
  #define IC_CARD_POWR_OFF P6_bit.no3=0   //输出


  #define B_WTD_0  P5_bit.no4=0   //OUT WTD  
  #define B_WTD_1  P5_bit.no4=1   //OUT WTD  

  #define B_REED_TEST_STATUS  (P5_bit.no5)   //新板子(2009-02-18):干皇管---K1
  #define B_TEST_HARD_STATUS  (P5_bit.no6)   //新板子(2009-02-18):自检---K2
  #define OPEN_PROG_KEY  (P7_bit.no6)         //编程开关
  #define OPEN1_STATUS   (P0_bit.no0)        //开端盖(后端盖)铅封
  #define OPEN2_STATUS   (P0_bit.no1)        //开大盖(上端盖)
  //--------------用于按键的状态指示
  #define Inter_Left_Key_STATUS  (P7_bit.no2)
  //Down Key
  #define Inter_Down_Key_STATUS (P7_bit.no4)
  //Right key
  #define Inter_Right_Key_STATUS (P7_bit.no1)
  //UP Key
  #define Inter_Up_Key_STATUS (P7_bit.no3)

  #define ALARM_0  PORT_ChangeP157Output( 0 )//报警灯 
  #define ALARM_1  PORT_ChangeP157Output( 1 ) 
#endif


#define B_TEST_FAC_STATUS  (P5_bit.no7)      //工厂状态-----------PUCK

//----------------------------------------------------杂项
#define B_second_enable_0  PORT_ChangeP82Output( 0 )//秒脉冲  
#define B_second_enable_1  PORT_ChangeP82Output( 1 ) 

#define B_dma_0  PORT_ChangeP83Output( 0 )//需量周期  
#define B_dma_1  PORT_ChangeP83Output( 1 ) 

#define B_time_priod_0  PORT_ChangeP84Output( 0 )//时段输出  
#define B_time_priod_1  PORT_ChangeP84Output( 1 ) 

#define EEPOWER_0  PORT_ChangeP05Output( 0 )//内卡电源控制引脚         
#define EEPOWER_1  PORT_ChangeP05Output( 1 ) 

#define IRA_0  PORT_ChangeP10Output( 0 )//远红外发送使能-----PUCK     
#define IRA_1  PORT_ChangeP10Output( 1 ) 

#define B_4IN1_1_0  PORT_ChangeP64Output( 0 )//4IN1-1  
#define B_4IN1_1_1  PORT_ChangeP64Output( 1 ) 

#define B_4IN1_2_0  PORT_ChangeP65Output( 0 )//4IN1-2   
#define B_4IN1_2_1  PORT_ChangeP65Output( 1 ) 
#define B_4IN1_3_0  PORT_ChangeP66Output( 0 )//4IN1-3    
#define B_4IN1_3_1  PORT_ChangeP66Output( 1 ) 
#define B_4IN1_4_0  PORT_ChangeP67Output( 0 )//4IN1-4   
#define B_4IN1_4_1  PORT_ChangeP67Output( 1 ) 

#define B_BATLOWRTC_TEST_STATUS  (P11_bit.no1)//时钟电池低

#define B_POWERDIR1_0  PORT_ChangeP150Output( 0 )//功率指示1  
#define B_POWERDIR1_1  PORT_ChangeP150Output( 1 ) 

#define B_POWERDIR2_0  PORT_ChangeP151Output( 0 )//功率指示2   
#define B_POWERDIR2_1  PORT_ChangeP151Output( 1 ) 

#define B_BATLOW_TEST_STATUS  (P8_bit.no5)//低功耗电池




#ifndef USER_DEFINE_C
#define USER_EXT extern
#else
#define USER_EXT 
#endif



#define UART0_RX_BUFF_MAXSIZE 60
USER_EXT INT16U Uart0_Rx_Count; //当前串口收到的数据长度----PUCK
USER_EXT INT8U Uart0_RecBuf[UART0_RX_BUFF_MAXSIZE];

#ifdef UART1
  #define UART1_RX_BUFF_MAXSIZE 70
  USER_EXT INT16U Uart1_Rx_Count; //当前串口收到的数据长度----PUCK
  USER_EXT INT8U Uart1_RecBuf[UART1_RX_BUFF_MAXSIZE];
#endif
  
#define UART2_RX_BUFF_MAXSIZE 70
USER_EXT INT16U Uart2_Rx_Count;   //当前串口收到的数据长度----PUCK
USER_EXT INT8U Uart2_RecBuf[UART2_RX_BUFF_MAXSIZE];

#define UART3_RX_BUFF_MAXSIZE 70
USER_EXT INT16U Uart3_Rx_Count;  //当前串口收到的数据长度----PUCK
USER_EXT INT8U Uart3_RecBuf[UART3_RX_BUFF_MAXSIZE];


void UART0_Init_PUCK(ULONG baud_rate,INT16U CheckBit);
INT8U UART0_RecData_pre_PUCK(void);
INT8U UART0_SendData_PUCK(UCHAR* txbuf, USHORT txnum);

void UART1_Init_PUCK(ULONG baud_rate,INT16U CheckBit);
INT8U UART1_SendData_PUCK(UCHAR* txbuf, USHORT txnum);
INT8U UART1_RecData_pre_PUCK(void);


void UART2_Init_PUCK(ULONG baud_rate,INT16U CheckBit);
INT8U UART2_SendData_PUCK(UCHAR* txbuf, USHORT txnum);
INT8U UART2_RecData_pre_PUCK(void);


void UART3_Init_PUCK(ULONG baud_rate,INT16U CheckBit);
INT8U UART3_SendData_PUCK(UCHAR* txbuf, USHORT txnum);
INT8U UART3_RecData_pre_PUCK(void);



//#define START_IR_DECODE  TAU_Channel1_Start()
//#define STOP_IR_DECODE   TAU_Channel1_Stop()

#define START_PMW       TAU_Channel2_Start()
#define STOP_PMW        TAU_Channel2_Stop()

#define START_ALL_LOSS  TAU_Channel5_Start()
#define STOP_ALL_LOSS   TAU_Channel5_Stop()

#define START_TIMER_10uS    {TAU_Channel6_Stop();Set_Fast_Timer(RUN_MODE);TAU_Channel6_Start();}
#define START_TIMER_1mS  {TAU_Channel6_Stop();Set_Fast_Timer(FXT_RUN_MODE);TAU_Channel6_Start();}

#define STOP_FAST_TIMER        TAU_Channel6_Stop()

#define START_1MS TAU_Channel7_Start()
#define STOP_1MS  TAU_Channel7_Stop()

#define START_RTC_COUNTER RTC_CounterEnable()
#define STOPT_RTC_COUNTER RTC_CounterDisable()

#define START_SEC_ALARM RTC_ConstPeriodInterruptEnable(ONESEC)
#define START_MIN_ALARM RTC_ConstPeriodInterruptEnable(ONEMIN)
#define STOP_RTC_ALARM  RTC_ConstPeriodInterruptDisable()




void hdwinit(INT32U Mode);
void count_1ms(void);
MD_STATUS CG_ChangeClockMode( enum ClockMode mode );
/* End user code for definition. Do not edit comment generated here */
#endif
