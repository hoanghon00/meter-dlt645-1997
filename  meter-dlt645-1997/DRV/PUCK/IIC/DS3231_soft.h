#ifndef SOFT_DS3231_H
#define SOFT_DS3231_H

#ifdef DRV_SIMU_DS3231     //宏开启，打开DS3231的软件模拟功能

#if METER_HARD_TYPE==HARD_TYPE_20081005
  #define DIR_SCLK              PM0_bit.no4
  #define DIR_SDA               PM0_bit.no3
  #define DATA_SCLK             P0_bit.no4
  #define DATA_SDA              P0_bit.no3
#endif

#if METER_HARD_TYPE==HARD_TYPE_20090224
  #define DIR_SCLK              PM2_bit.no7
  #define DIR_SDA               PM2_bit.no6
  #define DATA_SCLK             P2_bit.no7
  #define DATA_SDA              P2_bit.no6
#endif

#define SET_3231_SCLK_OUTPUT  DIR_SCLK=0
#define SET_3231_SCLK_INPUT   DIR_SCLK=1

#define SET_3231_SDA_OUTPUT   DIR_SDA=0
#define SET_3231_SDA_INPUT    DIR_SDA=1

#define CLR_3231_SCLK_0       DATA_SCLK=0    //将SCL总线置0---------PUCK   
#define SET_3231_SCLK_1       DATA_SCLK=1    //将SCL总线置1---------PUCK

#define CLR_3231_SDA_0        DATA_SDA=0     //将SDA总线置0---------PUCK
#define SET_3231_SDA_1        DATA_SDA=1     //将SDA总线置1---------PUCK

#define GET_SDA_STATUS       (DATA_SDA)


void delay_cycles1(unsigned int num);
void Set_3231_Sclk_Dir(INT8U Dir);
void Set_3231_Sda_Dir(INT8U Dir);
void Set_3231_Sclk_Pin(INT8U Level);
void Set_3231_Sda_Pin(INT8U Level);
INT8U Get_3231_Sda_Pin(void);
#endif

#endif