#ifndef EVENTPUB_VAR
#define EVENTPUB_VAR


#ifndef EVENTPUB_C
#define EVENTPUB_C_EXT  extern
#else
#define EVENTPUB_C_EXT 
#endif



//以下是事件定义ID,单独定义，与协议无关,注意：保证所有事件一定是连续的，宏修改时需注意！
#define ID_EVENT_NO_VALID    0  	                      //无效事件--------0
#define ID_EVENT_A_LOSS_VOLT 1  	                      //A相失压--------1
#define ID_EVENT_B_LOSS_VOLT (ID_EVENT_A_LOSS_VOLT+1)  	      //B相失压--------2
#define ID_EVENT_C_LOSS_VOLT (ID_EVENT_B_LOSS_VOLT+1)	      //C相失压--------3

#define ID_EVENT_A_LOSS_CURR (ID_EVENT_C_LOSS_VOLT+1)         //A相失流--------4
#define ID_EVENT_B_LOSS_CURR (ID_EVENT_A_LOSS_CURR+1)  	      //B相失流--------5
#define ID_EVENT_C_LOSS_CURR (ID_EVENT_B_LOSS_CURR+1)	      //C相失流--------6

#define ID_EVENT_CUR_NEG_SEQ  (ID_EVENT_C_LOSS_CURR+1)        //电流逆相序--------7
#define ID_EVENT_VOLT_NEG_SEQ (ID_EVENT_CUR_NEG_SEQ+1)        //电压逆相序--------8

#define ID_EVENT_A_VOLT_MAX    (ID_EVENT_VOLT_NEG_SEQ+1)      //A相电压超上限--------9
#define ID_EVENT_B_VOLT_MAX    (ID_EVENT_A_VOLT_MAX+1)        //B相电压超上限--------10
#define ID_EVENT_C_VOLT_MAX    (ID_EVENT_B_VOLT_MAX+1)	      //C相电压超上限--------11
#define ID_EVENT_A_VOLT_MIN    (ID_EVENT_C_VOLT_MAX+1)        //A相电压超下限--------12
#define ID_EVENT_B_VOLT_MIN    (ID_EVENT_A_VOLT_MIN+1)        //B相电压超下限--------13
#define ID_EVENT_C_VOLT_MIN    (ID_EVENT_B_VOLT_MIN+1)	      //C相电压超下限--------14

#define ID_EVENT_DEMAND_ALARM  (ID_EVENT_C_VOLT_MIN+1)         //超需量报警--------15
#define ID_EVENT_DEMAND_SWITCH (ID_EVENT_DEMAND_ALARM+1)        //超需量跳闸--------16
#define ID_EVENT_NC            (ID_EVENT_DEMAND_SWITCH+1)      //空空空--------17
#define ID_EVENT_RESET           (ID_EVENT_NC+1)          //复位--------18
#define ID_EVENT_CLR_DEMAND_COM  (ID_EVENT_RESET+1)             //通信清需量--------19
#define ID_EVENT_CLR_DEMAND_MAN  (ID_EVENT_CLR_DEMAND_COM+1)    //手动清需量--------20
#define ID_EVENT_CLR_PROCESS     (ID_EVENT_CLR_DEMAND_MAN+1)      //清过程--------21
#define ID_EVENT_CLR_VOLT_PASS   (ID_EVENT_CLR_PROCESS+1)      //清电压合格率--------22
#define ID_EVENT_SET_ENERG       (ID_EVENT_CLR_VOLT_PASS+1)    //设置初始电量--------23
#define ID_EVENT_UP_COVER_OPEN   (ID_EVENT_SET_ENERG+1)        //开大盖--------24
#define ID_EVENT_LEAD_COVER_OPEN (ID_EVENT_UP_COVER_OPEN+1)    //开铅封门--------25
#define ID_EVENT_RTC_BAT_LOW     (ID_EVENT_LEAD_COVER_OPEN+1) //时钟电池欠压--------26
#define ID_EVENT_LOWPWR_BAT_LOW  (ID_EVENT_RTC_BAT_LOW+1)     //低功耗电池欠压--------27
#define ID_EVENT_ADJUST_TIME     (ID_EVENT_LOWPWR_BAT_LOW+1)   //校时--------28
#define ID_EVENT_RTCERR_RUN      (ID_EVENT_ADJUST_TIME+1)      //运行时钟乱--------29
#define ID_EVENT_RTCERR_PWRON    (ID_EVENT_RTCERR_RUN+1)      //上电时钟乱--------30
#define ID_EVENT_EPPROM_FAULT    (ID_EVENT_RTCERR_PWRON+1)    //EEPROM坏--------31
#define ID_EVENT_ADJUST_MEASU    (ID_EVENT_EPPROM_FAULT+1)    //校表--------32
#define ID_EVENT_MODI_ENERG      (ID_EVENT_ADJUST_MEASU+1)    //电量纠错--------33

#define ID_EVENT_A_VOLT_OVERLOAD (ID_EVENT_MODI_ENERG+1)      //A相电压过压--------34
#define ID_EVENT_B_VOLT_OVERLOAD (ID_EVENT_A_VOLT_OVERLOAD+1)   //B相电压过压--------35
#define ID_EVENT_C_VOLT_OVERLOAD (ID_EVENT_B_VOLT_OVERLOAD+1)   //C相电压过压--------36
#define ID_EVENT_ENERG_FREEZ     (ID_EVENT_C_VOLT_OVERLOAD+1)   //电量冻结--------37
#define ID_EVENT_POWER_ALARM     (ID_EVENT_ENERG_FREEZ+1)    //超功率报警--------38
#define ID_EVENT_POWER_SWITCH  (ID_EVENT_POWER_ALARM+1)       //超功率跳闸--------39


#define ID_EVENT_A_LOSS_PARSE  (ID_EVENT_POWER_SWITCH+1)    //A相断相--------40
#define ID_EVENT_B_LOSS_PARSE  (ID_EVENT_A_LOSS_PARSE+1)         //B相断相--------41
#define ID_EVENT_C_LOSS_PARSE  (ID_EVENT_B_LOSS_PARSE+1)         //C相断相--------42

#define ID_EVENT_A_NEG_CURR  (ID_EVENT_C_LOSS_PARSE+1)        //A相电流反向--------43
#define ID_EVENT_B_NEG_CURR  (ID_EVENT_A_NEG_CURR+1)          //B相电流反向--------44
#define ID_EVENT_C_NEG_CURR  (ID_EVENT_B_NEG_CURR+1)          //C相电流反向--------45

#define ID_EVENT_KEY_SWITCH     (ID_EVENT_C_NEG_CURR+1)     //跳线短接--------46
#define ID_EVENT_ERR_DI       (ID_EVENT_KEY_SWITCH+1)       //数据标识错--------47
#define ID_EVENT_ERR_PASSWD   (ID_EVENT_ERR_DI+1)         //密码错--------48
#define ID_EVENT_YEAR_FLOW    (ID_EVENT_ERR_PASSWD+1)     //年时区数超--------49
#define ID_EVENT_DATE_FLOW    (ID_EVENT_YEAR_FLOW+1)      //日时段数超--------50
#define ID_EVENT_TRRI_FLOW    (ID_EVENT_DATE_FLOW+1)      //费率数超--------51
#define ID_EVENT_PRGKEY_ON    (ID_EVENT_TRRI_FLOW+1)      //编程键打开--------52
#define ID_EVENT_FAC_ON       (ID_EVENT_PRGKEY_ON+1)       //工厂模式--------53
#define ID_EVENT_METER_LOCK   (ID_EVENT_FAC_ON+1)       //对电表编程(设置重要参数)时，若密码连续出错次数大于等于5次后--------54

#define ID_EVENT_AB_FUNC_KEY     (ID_EVENT_METER_LOCK+1)    //AB切换功能键识别--------55
#define ID_EVENT_POWER_OFF  (ID_EVENT_AB_FUNC_KEY+1)        //掉电事件--------56----PUCK 
#define ID_DEMAND_ARRIVE      (ID_EVENT_POWER_OFF+1)          //需量周期到--------57
#define ID_OVER_LOAD        (ID_DEMAND_ARRIVE+1)            //负荷控制报警--------58
#define ID_NOT_CHARGE        (ID_OVER_LOAD+1)              //欠费报警--------59
#define ID_PARSE_SWITCH      (ID_NOT_CHARGE+1)             //时段切换--------60
#define ID_MEASURE_ERROR     (ID_PARSE_SWITCH+1)           //计量错误--------61
#define ID_METER_ADDR_EN     (ID_MEASURE_ERROR+1)          //设置表地址按钮-------62
#define ID_EVENT_ALL_LOSS_VOLT (ID_METER_ADDR_EN+1)        //设置表地址按钮-------63

#define ID_EVENT_AB_LOSS_VOLT (ID_EVENT_ALL_LOSS_VOLT+1)   //AB失压------------------64
#define ID_EVENT_AC_LOSS_VOLT (ID_EVENT_AB_LOSS_VOLT+1)    //AC失压------------------65
#define ID_EVENT_BC_LOSS_VOLT (ID_EVENT_AC_LOSS_VOLT+1)   //BC失压------------------66
#define ID_EVENT_ABC_LOSS_VOLT (ID_EVENT_BC_LOSS_VOLT+1)   //ABC失压------------------67

#define ID_EVENT_PROG          (ID_EVENT_ABC_LOSS_VOLT+1)  //编程事件：设置参数-----------68
#define ID_EVENT_CLR_ALL_DATA  (ID_EVENT_PROG+1)           //数据总清-----------69
#define ID_EVENT_UP_COVER_CLOSE  (ID_EVENT_CLR_ALL_DATA+1)     //关大盖--------70(HUCK需求) 
#define ID_EVENT_LEAD_COVER_CLOSE (ID_EVENT_UP_COVER_CLOSE+1)   //关铅封门------------71(HUCK需求)  


#define ID_EVENT_AB_LOSS_CURR (ID_EVENT_LEAD_COVER_CLOSE+1)   //AB失流------------------72
#define ID_EVENT_AC_LOSS_CURR (ID_EVENT_AB_LOSS_CURR+1)    //AC失流------------------73
#define ID_EVENT_BC_LOSS_CURR (ID_EVENT_AC_LOSS_CURR+1)   //BC失流------------------74

#define ID_EVENT_ALL_LOSS_CURR (ID_EVENT_BC_LOSS_CURR+1)   //全失流-------------75

#define ID_EVENT_DOWN_COVER_OPEN   (ID_EVENT_ALL_LOSS_CURR+1)        //开端盖--------76
#define ID_EVENT_DOWN_COVER_CLOSE   (ID_EVENT_DOWN_COVER_OPEN+1)    //关端盖--------77

#define ID_EVENT_DOOR_OPEN          (ID_EVENT_DOWN_COVER_CLOSE+1)        //开计量装置门--------78
#define ID_EVENT_DOOR_CLOSE           (ID_EVENT_DOOR_OPEN+1)                  //关计量装置门--------79
#define ID_SLIP_ARRIVE            (ID_EVENT_DOOR_CLOSE+1)          //滑差到--------80
#define ID_ALARM_OUT              (ID_SLIP_ARRIVE+1)             //是否有报警事件输出：如声音报警，LCD_LED报警，端子排报警--------81
#define ID_CLR_DEMAND_KEY         (ID_ALARM_OUT+1)             //手动清需量按钮--------81
#define ID_EVENT_EXT_PROG         (ID_CLR_DEMAND_KEY+1)             //编程事件：设置参数--------82(同 ID_EVENT_PROG,应HUCKGD需求增加)
/*
#define ID_EVENT_LOSS_VOL         (ID_EVENT_EXT_PROG+1)        //失压变化：--------83(应HUCKGD需求增加，瞬态字由HUCK维护)
#define ID_EVENT_LOSS_CUR         (ID_EVENT_LOSS_VOL+1)        //失流变化：--------84(应HUCKGD需求增加，瞬态字由HUCK维护)
#define ID_EVENT_NEG_CUR         (ID_EVENT_LOSS_CUR+1)        //功率反向变化：--------85(应HUCKGD需求增加，瞬态字由HUCK维护)
*/
#define ID_EVENT_OSC_ERR         (ID_EVENT_EXT_PROG+1)        //单片机时钟系统故障--------86


#define MIN_ID_EVENT          ID_EVENT_A_LOSS_VOLT
#define MAX_ID_EVENT          ID_EVENT_OSC_ERR

#define MAX_EVENT_NUM          (MAX_ID_EVENT+1)



typedef struct
{
  INT8U Instant_Event[MAX_EVENT_NUM/8+1];
  INT8U CS[CS_BYTES];
}EVENT_INSTANT_STATUS;
NO_INIT EVENTPUB_C_EXT  EVENT_INSTANT_STATUS Meter_Instant_Status;

typedef struct
{
  U_Byte_Bit  Meter_Stat1;        //C020/C040----B1
  U_Byte_Bit  Meter_Stat2;        //C026/C041----B2     
  U_Byte_Bit  Meter_Stat3;        //C027/C042----B3   
  U_Byte_Bit  Meter_Stat4;        //C028/C043----B4 
  U_Byte_Bit  Grid_Stat;          //C021/C044----B5
  U_Byte_Bit  Meter_Stat6;        //C045---------B6
  U_Byte_Bit  Volt_LossMiss;      //扩充失压/断相，DI:DI_LOW_VOL
  U_Byte_Bit  NegCurr_Qudrant;    //扩充电流反极性/电表工作象限，DI_CURRENT_QUADRANT
  U_Byte_Bit  Key_Event;          //扩充编程键打开/跳线短接/需量复位/电表上锁，DI:DI_METER_PERIPHERY
  U_Byte_Bit  Comm_Status;        //扩充通信状态字，DI:DI_MODULE_STATUS
  
  //扩展不兼容的功率方向状态字
  U_Byte_Bit  Ext_Ac_Power_Dir;        //有功方向标志字节（1字节，0-3位为总、a、b、c元件，高4字节为0）
  U_Byte_Bit  Ext_Reac_Power_Dir;        //无功方向标志字节（1字节，0-3位为总、a、b、c元件，高4字节为0）
  
  U_Byte_Bit  GD_Meter_Stat;        //广东：电表状态字
  U_Byte_Bit  GD_PwrDir_Stat;       //广东：有功功率方向状态字
  U_Byte_Bit  GD_LossVolt_Stat;        //广东：失压状态字
  U_Byte_Bit  GD_LossCurr_Stat;        //广东：失流状态字
  U_Byte_Bit  GD_Hard_Stat;        //广东：硬件状态字
  //U_Byte_Bit  GD_Prog_Stat;        //广东：编程状态字(HUCK)
  
  INT8U CS[CS_BYTES];
}POWER_STATS;


EVENTPUB_C_EXT POWER_STATS Meter_Run_Status;


typedef struct
{
  U_Byte_Bit  Low_Byte;        //C129低8位
  U_Byte_Bit  High_Byte;        //C129高8位
  INT8U CS[CS_BYTES];
}SELF_TSET;
EVENTPUB_C_EXT SELF_TSET Meter_Self_Test;


typedef struct{
  INT8U  Occur:1;         //事件发生标志，不对外访问
  
  INT8U  Status:1;        //事件发生的状态位，外部访问，访问后，该位不清除，还是反应该事件的发生：0-------无事件发生；1-------事件发生；  
  INT8U  Status1:1;       //事件发生标志备份，外部访问1，访问后，该位清除
  INT8U  Status2:1;       //事件发生标志备份，外部访问2，访问后，该位清除
  INT8U  Status3:1;       //事件发生标志备份，外部访问3，访问后，该位清除
  INT8U  Status4:1;       //事件发生标志备份，外部访问4，访问后，该位清除  
  INT8U  Reserved:2;      //预留
  INT8U  Mins;           //分计数器
  INT8U  StartCount;      //事件发生计数器
  INT8U  EndCount;        //事件结束计数器  
  INT8U  CS[CS_BYTES];
}EventProcStatus;         //事件处理状态


EVENTPUB_C_EXT EventProcStatus EventProcStatusVar[MAX_EVENT_NUM]; 



#define POWER_OFF        0x37   //掉电
#define POWER_ON         0xF6   //上电
#define ABNORMAL_RESET   0xE8   //异常复位



typedef struct{
  INT8U  Power;
  INT8U  CS[CS_BYTES];
}POWER_STATUS;         //上电处理状态

NO_INIT EVENTPUB_C_EXT POWER_STATUS Power_Status;

void Get_Meter_Status(void);
void Event_Main_PUCK(INT8U Flag);


INT8U Read_Event_Ststus(INT8U PersonID,INT8U EventId);
void Set_Event_Instant(INT8U Id);
void Clr_Event_Instant(INT8U Id);

INT8U Check_Prog_Status(void);
void Save_Event_Data(void);
INT8U Check_AB_Switch(void);
INT8U Check_C_Switch(void);
INT8U Check_MeterAddr_Status(void);
INT8U Read_Event_Status(INT8U Id);
void Init_Event_Sram(INT8U);

//void Emerg_Event_Proc(void);
void Event_Proc_Sec(void);
INT8U Check_Event_End(INT8U Id);
void Save_All_Loss_Data(void);
void Init_All_Loss_Sram(void);
void Count_All_Loss_Proc(void);
INT8U RTC_Read_Fxt(volatile INT8U Time[]);
void Clr_All_Loss_Data(void);
void Init_Event_DIS_PUCK(void);
void PwrOff_Proc_PUCK(void);
void Check_Loss_All_Curr(void);
void Sound_LED_LCD_Port_Alarm(void);
INT8U Check_Man_ClrDemd_En(void);
void Get_Meter_Run_Status(void);
void Set_Event_Occur_Status(INT8U i);
void Set_Event_End_Status(INT8U i);
void Clr_Event_Real_Status(INT8U i);
void Clr_All_Event_Sram(void);
void Init_All_Event_Sram(void);
#endif
