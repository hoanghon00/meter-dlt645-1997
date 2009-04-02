#define DEVICE_ABS_PUCK
#include "Pub_PUCK.h"


/********************************************************************************
函数功能：各种模式下：CPU外围时钟初始化
入口：
返回：
    无
********************************************************************************/
void Init_CPUDevice_Clock(INT32U Mode)
{ 
   switch(Mode)
   {
     case SYS_NORMAL:
       break;       
     case SYS_RESUME:
       TAU0EN=1;
       SAU0EN=1;
       SAU1EN=0;
       IIC0EN=1;
       ADCEN=0;
       DACEN=0;
       RTCEN=1;
       
       EXBEN=0;  //PER1
       break;
     case SYS_SLEEP:
       TAU0EN=1;
       SAU0EN=0;
       SAU1EN=0;
       IIC0EN=0;
       ADCEN=0;
       DACEN=0;
       RTCEN=1;
       
       EXBEN=0;  //PER1
       break;
   }   
}    
/********************************************************************************
函数功能：复位后打印复位源
入口：
返回：
    无
********************************************************************************/
void Print_Reset_Source(void)
{ 
    INT8U resetflag = RESF;
    
    if(!resetflag)
      DEBUG_PRINT(PUCK,1,"Reset Source:External Reset Signal Or POC!");
    
    if(resetflag&0x01)
      DEBUG_PRINT(PUCK,1,"Reset Source:Low Voltage Reset Detect!");
    
    if(resetflag&0x10)
      DEBUG_PRINT(PUCK,1,"Reset Source:CPU Internal WatchDog!");
    
    if(resetflag&0x80)
      DEBUG_PRINT(PUCK,1,"Reset Source:Illegal Instrucion!");
    
}

/********************************************************************************
函数功能：resume时，对CPU IO口的定义
入口：
返回：
1：resum下不用的IO口，IO复位后默认为输入
********************************************************************************/
void Init_All_IO_Resume(void)
{
    P0=0;
    PU0=0;
    
    PM0_bit.no2=0;     //预留----------------------
#if METER_HARD_TYPE==HARD_TYPE_20081005
    PM0_bit.no0=1;     //红外唤醒(P00)-------------IRDA_WKP,TO0实际未用，做悬空处理
    PM0_bit.no1=0;     //预留----------------------
    PM0_bit.no3=1;     //RTC(VBB供电)--------------RTC1_SDA
    PM0_bit.no4=1;     //RTC(VBB供电)--------------RTC1_SCL
    PM0_bit.no6=0;     //外部总线扩展 WAIT---------预留不用
#endif
#if METER_HARD_TYPE>=HARD_TYPE_20090224
    PM0_bit.no0=1;     //KEY6---------------------开端盖(后端盖)铅封
    PM0_bit.no1=1;     //KEY5---------------------开大盖(上端盖)
    PM0_bit.no3=0;     //------新板子(2009-02-18):RXD_GPRS(VCC供电,CPU收)
    PM0_bit.no4=0;     //------新板子(2009-02-18):事件指示脚：EVENT_GPRS(VCC供电)
    PM0_bit.no6=0;     //------新板子(2009-02-18):门节点输入检测
#endif   
    PM0_bit.no5=0;     //内卡电源控制--------------MEN_PWR
    
    PM0|=PM0_DEFAULT;
    
    P1=0;
    PU1=0;
    PM1_bit.no0=0;   //远红外发送使能-------IR_FAR_EN
    PM1_bit.no1=0;   //串口1----------------RXD1
    PM1_bit.no2=0;   //串口1----------------TXD1
    PM1_bit.no3=0;   //串口2----------------TXD2
    PM1_bit.no4=0;   //串口2----------------RXD2
    PM1_bit.no5=0;   //485控制口------------RE/DE2
    PM1_bit.no6=0;   //遥控器解码-----------RXD1
    PM1_bit.no7=1;   //PWM------------------PWM
    
    P2=0;
    PM2_bit.no0=0;    //计量RST---------7022_RST
    PM2_bit.no1=0;    //计量SIG---------7022_SIG
    PM2_bit.no2=0;    //计量SDO---------7022_SDO
    PM2_bit.no3=0;    //计量SDI ---------7022_SDI    
    PM2_bit.no4=0;    //计量SCK---------7022_SCK
    PM2_bit.no5=0;    //计量POWER-------PULSE_PWR
#if METER_HARD_TYPE==HARD_TYPE_20081005
    PM2_bit.no6=0;    //----------------NC
    PM2_bit.no7=0;    //----------------NC
#endif    
#if METER_HARD_TYPE>=HARD_TYPE_20090224
    PM2_bit.no6=1;    //-----新板子(2009-02-18):RTC(VBB供电)--------------RTC1_SDA，先设为输入，在I2CSoft中会重新初始化
    PM2_bit.no7=1;    //-----新板子(2009-02-18):RTC(VBB供电)--------------RTC1_SCL，先设为输入，在I2CSoft中会重新初始化
#endif 
    
    P3=0;
    PU3=0; 
    PM3_bit.no0=1; //红外唤醒,外部中断计数-------Laser_WKP------新板子(2009-02-18):IC_FAIL
    PM3_bit.no1=1; //掉电检测-----------------------V_D1
    PM3|=PM3_DEFAULT;
    
    P4=0;
    PU4=0;
    PM4_bit.no0=1; //仿真调试口(VBB上拉)----------TOOL0
    PM4_bit.no1=0; //仿真调试口-------------------TOOL1
    PM4_bit.no2=0; //DataFlash片选----------------45DB161_/CS
    PM4_bit.no3=0; //DataFlash时钟----------------45DB161_SCK
    PM4_bit.no4=0; //DataFlash输出----------------45DB161_SO
    PM4_bit.no5=0; //DataFlash输入----------------45DB161_SI
    PM4_bit.no6=0; //全失压----------ALL_V_LOSS
    PM4_bit.no7=0; //DataFlash--------------------45DB161_RDY  
        
    P5=0;
    PU5=0;
    PM5_bit.no0=0;   //总线扩展(EX8)------------NC------新板子(2009-02-18):IC_IN
    PM5_bit.no1=0;   //总线扩展(EX9)------------NC------新板子(2009-02-18):IC_OUT
    PM5_bit.no2=0;   //采样计量REFOUT-----------ALL_V_LOSS_EN,NC------新板子(2009-02-18):IC_RST
    PM5_bit.no3=0;   //喂狗信号-----------------WDI------新板子(2009-02-18):IC_SW
    PM5_bit.no4=1;   //P54----------------------LED报警指示------新板子(2009-02-18):喂狗信号，设为输入，喂狗无效
    PM5_bit.no5=1;   //自检(VBB上拉)------------K2
    PM5_bit.no6=1;   //干皇管-------------------K1,输入：用于红外模式判定-------09-02-01
    PM5_bit.no7=1;   //工厂模式(VBB上拉)--------
    
    P6=0;
    PU6=0;
    PM6_bit.no0=1;   //内卡硬件I2C SCLK-------MEM_SCL，先设为输入，在I2C中会重新初始化
    PM6_bit.no1=1;   //内卡硬件I2C SDA--------MEM_SDA，先设为输入，在I2C中会重新初始化
    PM6_bit.no2=1;   //内卡硬件I2C写保护------MEM_WP，先设为输入，在I2C中会重新初始化
    PM6_bit.no3=1;   //------新板子(2009-02-18)(VBB上拉，设计错误):IC_PWR,要改啊，FUCK大哥！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
    PM6_bit.no4=0;   //无功方向---------------A_DIR
    PM6_bit.no5=0;   //无功方向---------------R_DIR
    PM6_bit.no6=0;   //有功输出---------------A_EOUT
    PM6_bit.no7=0;   //无功输出---------------R_EOUT
    
    P7=0;
    PU7=0;
    
#if METER_HARD_TYPE==HARD_TYPE_20081005
    PM7_bit.no0=0;   //UPKEY------------------KEY7
    PM7_bit.no1=0;   //UPKEY------------------KEY6
    PM7_bit.no2=0;   //UPKEY------------------KEY5
    PM7_bit.no5=1;   //UPKEY(VBB上拉)---------KEY3
#endif 
#if METER_HARD_TYPE>=HARD_TYPE_20090224
    PM7_bit.no0=0;   //NC-------------------必须为输出
    PM7_bit.no1=1;   //UPKEY------------------KEY6
    PM7_bit.no2=1;   //UPKEY------------------KEY5
    PM7_bit.no5=0;   //NC--------------------必须为输出
#endif
    PM7_bit.no3=1;   //UPKEY(VBB上拉)---------KEY1
    PM7_bit.no4=1;   //UPKEY(VBB上拉)---------KEY2
    PM7_bit.no6=1;   //UPKEY(VBB上拉)---------KEY4
    PM7_bit.no7=1;   //红外唤醒(VBB上拉)------LASER_WAKEUP
    
   
    P8=0;
    PU8=0;
    PM8_bit.no0=0;   //跳闸输出-------------------SWITCH_TOGGLE
    PM8_bit.no1=0;   //报警端子排-----------------ALARM
    PM8_bit.no2=1;   //秒脉冲使能(和逻辑门连接，但和3231连接作为逻辑门)-----SECOND_/EN
    PM8_bit.no3=0;   //需量周期-------------------DEMAND
    PM8_bit.no4=0;   //时段输出-------------------TIME_SW
    PM8_bit.no5=0;   //低功耗电池检测-------------VB3_OK1
    PM8_bit.no6=0;   //远红外电源控制-------------IR_FAR_PWR
    PM8_bit.no7=0;   //总线扩展(EX7)--------------NC------新板子(2009-02-18):IC_SW
    
    P11=0;
    PM11_bit.no0=0;   //远红外/吸附红外切换----------IR_FAR_/SELECT
    PM11_bit.no1=0;   //时钟电池检测-----------VB1_OK
    PM11|=PM11_DEFAULT;
    
    P12=0;
    PU12=0;
    PM12_bit.no0=0;   //CF2输入-----------7022_CF2
    PM12_bit.no1=1;   //X1-----------------
    PM12_bit.no2=1;   //X2-----------------
    PM12_bit.no3=1;   //XT1----------------
    PM12_bit.no4=1;   //XT2----------------
    PM12|=PM12_DEFAULT;
    
    P13=0;
    PU13=0;    
    PM13_bit.no0=0;   //7022_CS
    PM13_bit.no1=0;   //预留-------------7022_341_331
    PM13|=PM13_DEFAULT;
    
    P14=0;
    PU14=0;
    PM14_bit.no0=0;   //CF1输入---------7022_CF1
    PM14_bit.no1=1;   //时钟秒脉冲(VCC上拉,与3231相连ECOND_PULSE
    PM14_bit.no2=0;   //蜂鸣器----------ALARM_SND
    PM14_bit.no3=0;   //串口0----------RXD0
    PM14_bit.no4=0;   //串口0----------TXD0
    PM14_bit.no5=0;   //485控制口--------RE/DE0
    PM14|=PM14_DEFAULT;
    
    P15=0;            //-------09-02-02:P15=0x10改为 P15=0；因为 Init_All_IO_Resume的调用，HUCK会处理
    PM15_bit.no0=0;   //有功脉冲指示灯-------------A-EOU1    
    PM15_bit.no1=0;   //无功脉冲指示灯-------------R-EOU1
    PM15_bit.no2=1;   //LCD背光改为输入，背光无效------Back_Light_On   
    PM15_bit.no3=0;   //LCD CS--------------------1623-/CS
    PM15_bit.no4=0;   //LCD POWER-----------------LCD_POWER
    PM15_bit.no5=0;   //LCD DATA------------------1623-DATA
    PM15_bit.no6=0;   //LCD WRITE-----------------1623-/WR
    PM15_bit.no7=0;   //预留----------------------NC
 }
/********************************************************************************
函数功能：进入睡眠前，对CPU IO口输出为0
入口：
返回：
1：原有输入脚还是输入脚，不变；
2：未用的输入脚，已用的输出脚，强制输出为0；
3：双向IO，
4：注意的几个脚：IC_PWR，SECOND_PULSE，PWM，K1(干皇管)
********************************************************************************/
void Init_All_IO_Sleep(void)
{    

    P0=0;
    PU0=0;
    
    PM0_bit.no2=0;     //预留----------------------------新板子(2009-02-18):TXD_GPRS(VCC供电,CPU收)
#if METER_HARD_TYPE==HARD_TYPE_20081005
    PM0_bit.no0=1;     //红外唤醒(P00)-------------IRDA_WKP,TO0实际未用，做悬空处理
    PM0_bit.no1=0;     //预留----------------------    
    PM0_bit.no3=1;     //RTC(VBB供电)--------------RTC1_SDA
    PM0_bit.no4=1;     //RTC(VBB供电)--------------RTC1_SCL
    PM0_bit.no6=0;     //外部总线扩展 WAIT---------预留不用
#endif
#if METER_HARD_TYPE>=HARD_TYPE_20090224
    PM0_bit.no0=0;     //开盖检测-------------
    PM0_bit.no1=0;     //KEY5---------------------开大盖(上端盖)
    PM0_bit.no3=0;     //RTC(VBB供电)--------------RTC1_SDA------新板子(2009-02-18):RXD_GPRS(VCC供电,CPU收)
    PM0_bit.no4=0;     //RTC(VBB供电)--------------RTC1_SCL------新板子(2009-02-18):事件指示脚：EVENT_GPRS(VCC供电)
    PM0_bit.no6=0;     //------新板子(2009-02-18):门节点输入检测
#endif    
    PM0_bit.no5=0;     //内卡电源控制--------------MEN_PWR
    PM0|=PM0_DEFAULT;
    
    P1=0;
    PU1=0;
    PM1_bit.no0=0;   //远红外发送使能-------IR_FAR_EN
    PM1_bit.no1=0;   //串口1----------------RXD1
    PM1_bit.no2=0;   //串口1----------------TXD1
    PM1_bit.no3=0;   //串口2----------------TXD2
    PM1_bit.no4=0;   //串口2----------------RXD2
    PM1_bit.no5=0;   //485控制口------------RE/DE2
    PM1_bit.no6=0;   //遥控器解码-----------RXD1
    PM1_bit.no7=1;   //PWM------------------PWM
    
    P2=0;
    PM2_bit.no0=0;    //计量RST---------7022_RST
    PM2_bit.no1=0;    //计量SIG---------7022_SIG
    PM2_bit.no2=0;    //计量SDO---------7022_SDO
    PM2_bit.no3=0;    //计量SDI ---------7022_SDI    
    PM2_bit.no4=0;    //计量SCK---------7022_SCK
    PM2_bit.no5=0;    //计量POWER-------PULSE_PWR
#if METER_HARD_TYPE==HARD_TYPE_20081005
    PM2_bit.no6=0;    //----------------NC
    PM2_bit.no7=0;    //----------------NC
#endif    
#if METER_HARD_TYPE>=HARD_TYPE_20090224
    PM2_bit.no6=1;    //----------------NC------新板子(2009-02-18):RTC(VBB供电)--------------RTC1_SDA
    PM2_bit.no7=1;    //----------------NC------新板子(2009-02-18):RTC(VBB供电)--------------RTC1_SCL
#endif 
    
    
    P3=0;
    PU3=0; 
    PM3_bit.no0=1; //红外唤醒,外部中断计数-------Laser_WKP------新板子(2009-02-18):IC_FAIL
    PM3_bit.no1=1; //掉电检测-----------------------V_D1
    PM3|=PM3_DEFAULT;
    
    P4=0;
    PU4=0;
    PM4_bit.no0=1; //仿真调试口(VBB上拉)----------TOOL0
    PM4_bit.no1=0; //仿真调试口-------------------TOOL1
    PM4_bit.no2=0; //DataFlash片选----------------45DB161_/CS
    PM4_bit.no3=0; //DataFlash时钟----------------45DB161_SCK
    PM4_bit.no4=0; //DataFlash输出----------------45DB161_SO
    PM4_bit.no5=0; //DataFlash输入----------------45DB161_SI
    PM4_bit.no6=1; //------新板子(2009-02-18):全失压----------ALL_V_LOSS
    PM4_bit.no7=0; //DataFlash--------------------45DB161_RDY  
        
    P5=0;
    PU5=0;
    PM5_bit.no0=0;   //总线扩展(EX8)------------NC------新板子(2009-02-18):IC_IN
    PM5_bit.no1=0;   //总线扩展(EX9)------------NC------新板子(2009-02-18):IC_OUT
    PM5_bit.no2=0;   //采样计量REFOUT-----------ALL_V_LOSS_EN,NC------新板子(2009-02-18):IC_RST
    PM5_bit.no3=0;   //喂狗信号-----------------WDI------新板子(2009-02-18):IC_SW
    PM5_bit.no4=0;   //P54----------------------LED报警指示------新板子(2009-02-18):喂狗信号
    PM5_bit.no5=0;   //自检(VBB上拉)------------K2------新板子(2009-02-18):干皇管---K1
    PM5_bit.no6=1;   //干皇管-------------------K1------新板子(2009-02-18):自检---K2
    PM5_bit.no7=1;   //工厂模式(VBB上拉)----------
    
    P6=0;
    PU6=0;
    PM6_bit.no0=0;   //内卡硬件I2C SCLK-------MEM_SCL
    PM6_bit.no1=0;   //内卡硬件I2C SDA--------MEM_SDA
    PM6_bit.no2=0;   //内卡硬件I2C写保护------MEM_WP
    PM6_bit.no3=1;   //------新板子(2009-02-18)(VBB上拉，设计错误):IC_PWR,要改啊，FUCK大哥！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
    PM6_bit.no4=0;   //无功方向---------------A_DIR
    PM6_bit.no5=0;   //无功方向---------------R_DIR
    PM6_bit.no6=0;   //有功输出---------------A_EOUT
    PM6_bit.no7=0;   //无功输出---------------R_EOUT
    
    P7=0;
    PU7=0;

#if METER_HARD_TYPE==HARD_TYPE_20081005
    PM7_bit.no0=0;   //编程按钮检测------------------KEY7
    PM7_bit.no1=0;   //UPKEY------------------KEY6
    PM7_bit.no2=0;   //UPKEY------------------KEY5
    PM7_bit.no5=1;   //UPKEY(VBB上拉)---------KEY3
#endif 
#if METER_HARD_TYPE>=HARD_TYPE_20090224
    PM7_bit.no0=0;   //NC----------------------
    PM7_bit.no1=1;   //右键------------------KEY3
    PM7_bit.no2=1;   //左键------------------KEY1
    PM7_bit.no5=0;   //NC------------------------    
#endif
    PM7_bit.no3=1;   //上翻(VBB上拉)---------KEY4
    PM7_bit.no4=1;   //下翻(VBB上拉)---------KEY2
    PM7_bit.no6=0;   //------新板子(2009-02-18):编程---------KEY7
    PM7_bit.no7=1;   //红外唤醒(VBB上拉)------IRD_WKP
    
    
    P8=0;
    PU8=0;
    PM8_bit.no0=0;   //跳闸输出-------------------SWITCH_TOGGLE
    PM8_bit.no1=0;   //报警端子排-----------------ALARM
    PM8_bit.no2=0;   //------新板子(2009-02-18):秒脉冲使能(和3231连接)-----SECOND_/EN
    PM8_bit.no3=0;   //需量周期-------------------DEMAND
    PM8_bit.no4=0;   //时段输出-------------------TIME_SW
    PM8_bit.no5=0;   //低功耗电池检测-------------VB3_OK1
    PM8_bit.no6=0;   //远红外电源控制-------------IR_FAR_PWR
    PM8_bit.no7=0;   //总线扩展(EX7)--------------NC------新板子(2009-02-18):IC_SW
    
    P11=0;
    PM11_bit.no0=0;   //远红外/吸附红外切换----------IR_FAR_/SELECT
    PM11_bit.no1=0;   //时钟电池检测-----------VB1_OK
    PM11|=PM11_DEFAULT;
    
    P12=0;
    PU12=0;
    PM12_bit.no0=0;   //CF2输入-----------7022_CF2
    PM12_bit.no1=1;   //X1-----------------
    PM12_bit.no2=1;   //X2-----------------
    PM12_bit.no3=1;   //XT1----------------
    PM12_bit.no4=1;   //XT2----------------
    PM12|=PM12_DEFAULT;
    
    P13=0;
    PU13=0;    
    PM13_bit.no0=0;   //7022_CS
    PM13_bit.no1=0;   //预留-------------7022_341_331
    PM13|=PM13_DEFAULT;
    
    P14=0;
    PU14=0;
    PM14_bit.no0=0;   //CF1输入口----------7022_CF1
    PM14_bit.no1=0;   //------新板子(2009-02-18):时钟秒脉冲-------------------SECOND_PULSE
    PM14_bit.no2=0;   //蜂鸣器----------ALARM_SND
    PM14_bit.no3=0;   //串口0----------RXD0
    PM14_bit.no4=0;   //串口0----------TXD0
    PM14_bit.no5=0;   //485控制口--------RE/DE0
    PM14|=PM14_DEFAULT;
    
    P15=0;
    PM15_bit.no0=0;   //有功脉冲指示灯-------------A-EOU1    
    PM15_bit.no1=0;   //无功脉冲指示灯-------------R-EOU1
    PM15_bit.no2=0;   //LCD背光--------------------Back_Light_On   
    PM15_bit.no3=0;   //LCD CS--------------------1623-/CS
    PM15_bit.no4=0;   //LCD POWER-----------------LCD_POWER
    PM15_bit.no5=0;   //LCD DATA------------------1623-DATA
    PM15_bit.no6=0;   //LCD WRITE-----------------1623-/WR
    PM15_bit.no7=0;   //预留----------------------NC------新板子(2009-02-18):LED报警指示
}
/********************************************************************************
函数功能：复位后对CPU的初始化
入口：
返回：
    无
********************************************************************************/
void Init_Hard_PUCK(void)
{
  hdwinit(SYS_NORMAL);
  Clr_Ext_Inter_Dog();
  START_1MS;    //打开毫秒中断，为OS提供tick
  START_RTC_COUNTER; 
  
  START_POWR_DOWN;   //STOP_POWR_DOWN;------应HUCK要求
  STOP_CF1;
  STOP_CF2;  
  
  STOP_UP_KEY;
  STOP_DOWN_KEY;
  STOP_LEFT_KEY;
  STOP_RIGHT_KEY;  
  STOP_PRG_KEY; 
  STOP_LASER_UP;
  STOP_IR_DECODE;
  
  STOP_RTC_ALARM;   
  STOP_IRDA_WAKE;
  STOP_1HZ;        
  STOP_ALL_LOSS;         //关闭全失压
}
/********************************************************************************
函数功能：睡眠前切换只sub时钟，关闭主时钟振荡,进入halt模式
入口：
返回：
    无
********************************************************************************/
void Goto_Sleep_PUCK(void)
{
   if(Switch_Main_Osc(HALT_MODE)) 
   {
     Clear_CPU_Dog();  //睡前喂狗
     while(1)   //对RTC闹铃和全失压的处理------PUCK
     {
       //CG_SelectPowerSaveMode(PSHALT);
       //STOP();
       HALT();
       //..........................................
       Clear_CPU_Dog();
       if((Resume_Src.Src_Flag&(IRAD_RESUME|KEY_RESUME)))  //不是全失压或者RTC闹铃，退出睡眠
         break;       
     } 
   }
     //醒来了，根据唤醒源马上切换高速晶振-----------PUCK 
    Switch_Main_Osc(RUN_MODE);
    Clear_CPU_Dog(); 
}  
/********************************************************************************
函数功能：对睡眠做了次数限制
入口：
返回：
    无
********************************************************************************/
void Cpu_Sleep_Proc(void)
{
  static S_Int8U Counts={CHK_BYTE,0,CHK_BYTE};
  static S_Int8U Last_Date={CHK_BYTE,0xFF,CHK_BYTE};
  
  while(1)
  {
    Goto_Sleep_PUCK();
    
    Get_Time_From_INTER_RTC((S_BCD_Time *)&(Cur_Time1));
    if(Cur_Time1.Date!=Last_Date.Var)
    {
      Counts.Var=0;
      Last_Date.Var=Cur_Time1.Date;
    }
    else
      Counts.Var++;
    
    if(Counts.Var<3)
      break;
  }
}
/********************************************************************************
函数功能：根据不同的工作模式，设置相应的中断及中断等级
入口：
返回：
    无
********************************************************************************/
void Init_Inter_Abs(INT32U Mode)
{  
  switch(Mode)//以下需要区分唤醒源---------PUCK
  {
    case SYS_NORMAL:   // SYS_NORMAL模式下，不必判定唤醒源      
        START_1MS;    //打开毫秒中断，为OS提供tick
        STOP_CF1;
        STOP_CF2;
        START_POWR_DOWN;
        START_UP_KEY;
        START_DOWN_KEY;
        START_LEFT_KEY;
        START_RIGHT_KEY;
        START_PRG_KEY;
        START_RTC_COUNTER; 
        START_LASER_UP;
        START_IR_DECODE;
        
        STOP_RTC_ALARM;   
        STOP_IRDA_WAKE;
        STOP_1HZ;        
        STOP_ALL_LOSS;         //关闭全失压
    break;
    
    case SYS_RESUME:  //SYS_RESUME模式下，根据唤醒源开启中断
        STOP_CF1;
        STOP_CF2;
        STOP_1HZ;
        STOP_POWR_DOWN;
        STOP_LEFT_KEY;
        STOP_RIGHT_KEY;
        STOP_LASER_UP;
        STOP_PRG_KEY;
        
        if(Resume_Src.Src_Flag&IRAD_RESUME)  //是在sleep模式下的红外唤醒，停止唤醒
        {  
          STOP_IRDA_WAKE;        //与 START_IR_DECODE 互斥
#if RSUME_REMOTER_EN==1         
          START_IR_DECODE;       //唤醒下，可以使用红外遥控器
#endif
        }
        else
        { 
          START_IRDA_WAKE; 
          STOP_IR_DECODE;       //与 START_IR_DECODE 互斥
        }
        
        START_1MS;          //启动1ms中断-----PUCK
        START_UP_KEY;
        START_DOWN_KEY;
        
        
        START_RTC_COUNTER; 
        START_MIN_ALARM;
        START_ALL_LOSS;         //全失压
       
    break;
    
    case SYS_SLEEP:  //SYS_SLEEP模式下，不必判定唤醒源
        STOP_CF1;
        STOP_CF2;
        STOP_1HZ;
        STOP_POWR_DOWN;
        STOP_DOWN_KEY;
        STOP_LEFT_KEY;
        STOP_RIGHT_KEY;
        STOP_PRG_KEY;
        STOP_LASER_UP;
        STOP_1MS;
        STOP_PMW;
        STOP_IR_DECODE;
        STOP_FAST_TIMER;  //关闭快速时钟，避免进入halt时，还在继续执行！
        
        START_RTC_COUNTER; 
        START_MIN_ALARM;        
        START_UP_KEY;        
        START_IRDA_WAKE; 
        START_ALL_LOSS;       //打开全失压
        break;
  }  
}               
/********************************************************************************
void void Init_ExtDevice_PUCK(void)
函数功能：不同状态下对外设的初始化工作
入口： Mode--------工作模式，表示对此模式下设备的初始化
返回：
    无
模式宏定义：SYS_NORMAL、SYS_SLEEP、SYS_RESUME
********************************************************************************/
void Init_ExtDevice_PUCK(INT32U Mode)
{
  Clear_CPU_Dog();
    
  switch(Mode)
  {
    case SYS_NORMAL:      //SYS_NORMAL模式下，不判定唤醒源----------PUCK
        DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Initial SYS_NORMAL Ext Device.......");     
        MAIN_POWER_FOR_MEM;             //内卡电源由主电源供给;
        MAIN_POWER_FOR_IRDA;            //远红外电源由主电源供给;              
        IRDA_SEND_EN;                   //红外发送使能
        Init_HTLCD_Mode(SYS_NORMAL);    //工作模式下初始化LCD  
        MainHTLCD_Puck_Pub(1);
        OPEN_MEASU_PWR;                 //计量芯片电源开启
        Init_ExtRTC_Pulse(1);            //使能秒脉冲初始化----PUCK    
    break;
    
    case SYS_RESUME:     //SYS_RESUME模式下，根据唤醒源设定相应的外设----------PUCK
        DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Initial SYS_RESUME Ext Device,Wakup Source=%d",Resume_Src.Src_Flag);  
        IRDA_SEND_DIS;                  //红外发送禁止
        
        if(Resume_Src.Src_Flag&IRAD_RESUME)//----------是红外唤醒，打开红外后备电源
          BAK_POWER_FOR_IRDA;             //远红外电源由低功耗电池供给;
        else
          MAIN_POWER_FOR_IRDA;              //远红外电源由主电源供给      
        
        Init_HTLCD_Mode(SYS_RESUME);   //唤醒模式下初始化LCD 
        MainHTLCD_Puck_Pub(0);
        CLOSE_MEASU_PWR;                //计量芯片电源关闭
        Init_ExtRTC_Pulse(0);            //使能秒脉冲初始化----PUCK 
    break;
    
    case SYS_SLEEP:            //SYS_SLEEP模式下，不判定唤醒源----------PUCK
        DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Initial SYS_SLEEP Ext Device......"); 
        IRDA_SEND_DIS;                  //红外发送禁止
        MAIN_POWER_FOR_IRDA;            //远红外电源由主电源供给，关闭远红外电路电源;
        Init_HTLCD_Mode(SYS_SLEEP);   //唤醒模式下初始化LCD 
        MAIN_POWER_FOR_LCD;       //低功耗模式下初始化LCD，关LCD后备电源
        MAIN_POWER_FOR_MEM;             //内卡电源由主电源供给;
        CLOSE_MEASU_PWR;                //计量芯片电源关闭
        Init_ExtRTC_Pulse(0);            //使能秒脉冲初始化----PUCK 
    break;    
  }  
}
/********************************************************************************
void void Init_CPU_PUCK(INT32U Mode)
函数功能：不同状态下对CPU初始化工作
入口： Mode--------工作模式，表示对此模式下设备的初始化
返回：
    无
模式宏定义：SYS_NORMAL、SYS_SLEEP、SYS_RESUME
********************************************************************************/
void Init_CPU_PUCK(INT32U Mode)
{
  Clear_CPU_Dog();
 
 switch(Mode) //以下需要区分唤醒源---------PUCK
 {
    case SYS_NORMAL:
        hdwinit(SYS_NORMAL);                     //硬件底层初始化
        Init_Inter_Abs(SYS_NORMAL);    //中断初始化------PUCK
        Init_All_UART(SYS_NORMAL);      //正常模式下的串口初始化  
        LVI_Start();
        Init_All_Port(SYS_NORMAL);      //正常情况下对所有端口的电平设置
    break;
    
    case SYS_RESUME:
        //红外、按钮,需要初始化较多的设备
        hdwinit(SYS_RESUME);                    //硬件底层初始化
        BAK_POWER_FOR_MEM;             //内卡电源最先开启，因后面就要读参数
        Init_Inter_Abs(SYS_RESUME);        
        Init_All_UART(SYS_RESUME);      //正常模式下的串口初始化  
        LVI_Start();
        Init_All_Port(SYS_RESUME);    //唤醒情况下对所有端口的电平设置
    break;
    
    case SYS_SLEEP:
        Init_Resume_Source();//清除所有唤醒源
        Init_Interr_Sram();
        Init_All_UART(SYS_SLEEP);       //睡眠模式下的串口初始化
        Init_Inter_Abs(SYS_SLEEP);
        Init_All_IO_Sleep();  //设IO
        Init_CPUDevice_Clock(SYS_SLEEP);        
        Goto_Sleep_PUCK();           //将主时钟切换为 sub，关闭振荡器，进入halt模式
    break;    
 }  
}
/********************************************************************************
void PwrCtrl_ExtDevice_HigSpeed(void)
函数功能：在主电源掉电情况下，快速控制外围设备,注意，此函数是为中断里面调用！
入口：
返回：
    无
********************************************************************************/
void PwrCtrl_ExtDevice_HigSpeed(void)
{
    CLOSE_MEASU_PWR;           //计量芯片电源关闭
    Init_Pulse_Port(0);       
    DIS_ALL_HARD_I2C_WP;           //关闭所有硬件II2硬件写
    DIS_ALL_HARD_I2C_WP;           //关闭所有软件II2硬件写
    BAK_POWER_FOR_MEM;         //内卡电源由低功耗电池供给;
    BAK_POWER_FOR_LCD;         //LCD电源由低功耗电池供给;
    IRDA_SEND_DIS;             //红外发送禁止
    //关闭系列中断
    STOP_IR_DECODE;
    STOP_FAST_TIMER;
    STOP_IRDA_WAKE;
    STOP_ALL_LOSS;
    
    IRDA_Rec_Disable();         //红接收关闭
    MAIN_POWER_FOR_IRDA;       //远红外电源由主电源供给;实际是关闭红外电源！
}