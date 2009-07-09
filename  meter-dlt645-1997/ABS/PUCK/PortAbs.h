
#ifndef PORT_ABS_H
#define PORT_ABS_H



#ifndef PORT_ABS_PUCK
#define PORT_ABS_PUCK_EXT  extern
#else
#define PORT_ABS_PUCK_EXT 
#endif


//多合一端子排配置：秒脉冲、时段、需量、滑差
//1:配置以下宏，与Const_Port_Status[]中的MultiFlag为0还是1含义不一样；
//2：配置 Const_Port_Status[]中的 MultiFlag为0还是1
//3:对于秒脉冲与其他3合一端子共用，秒脉冲使能配置：MultiFlag＝0(为1，Port_Out_Pub函数中，则不能 END！)；SEC_MULTI_PORT＝1；
#if METER_HARD_TYPE == HARD_TYPE_20090620_SD
  #define SEC_MULTI_PORT 1        //0：秒脉冲不是复合端子；1：秒脉冲是复合端子(共用）
  #define MULTI_3_PORT   2        //0：不是复合端子；1：是复合端子，但与秒脉冲不共用；2是复合端子，且与秒脉冲共用；
#else
  #define SEC_MULTI_PORT 0        //秒脉冲不是复合端子
  #define MULTI_3_PORT   1        //0：不是复合端子；1：是复合端子，但与秒脉冲不共用；2是复合端子，且与秒脉冲共用；
#endif


//LED报警指示灯
#define LED_ALARM_CLR        ALARM_0   
#define LED_ALARM_SET        ALARM_1

//喇叭报警
#define BEEP_ALARM_CLR       ALARM_BEEP_0   
#define BEEP_ALARM_SET       ALARM_BEEP_1

//端子排报警
#define EXT_ALARM_CLR        ALARM_PORT_0   
#define EXT_ALARM_SET        ALARM_PORT_1


//需量周期
#define EXT_DEMAND_CLR        B_dma_0   
#define EXT_DEMAND_SET        B_dma_1


//时段切换
#define EXT_PARSE_CLR        B_time_priod_0   
#define EXT_PARSE_SET        B_time_priod_1

//跳闸
#define EXT_SWITCH_CLR        RELAY_PORT_0   
#define EXT_SWITCH_SET        RELAY_PORT_1

//干篁管状态：0表示吸合
#define GUMB_STATUS          B_REED_TEST_STATUS

//外部RTC电池状态：0表示欠压
#define RTC_BAT_LOW      B_BATLOWRTC_TEST_STATUS

//低功耗电池状态：0表示欠压
#define LOWCOST_BAT_LOW      B_BATLOW_TEST_STATUS

#define IRDA_SEND_EN              IRA_1             //红外发送使能
#define IRDA_SEND_DIS             IRA_0             //红外发送禁止

#define IRDA_FAR_REC_EN            B_ird_chose_0     //远红接收开启
#define IRDA_NEAR_REC_EN           B_ird_chose_1     //吸附红外接收开启

#define BAK_POWER_FOR_IRDA         B_ird_pow_1       //远红外电源由低功耗电池供给;
#define MAIN_POWER_FOR_IRDA        B_ird_pow_0       //远红外电源由主电源供给;


#define BAK_POWER_FOR_LCD          LCD_POW_1         //LCD电源由低功耗电池供给;
#define MAIN_POWER_FOR_LCD         LCD_POW_0         //LCD电源由主电源供给;


#define BAK_POWER_FOR_MEM        EEPOWER_1         //内卡电源由低功耗电池供给;
#define MAIN_POWER_FOR_MEM       EEPOWER_0         //内卡电源由主电源供给;


#define OPEN_MEASU_PWR           B_7022_POWER_1    //计量芯片电源开启
#define CLOSE_MEASU_PWR          B_7022_POWER_0    //计量芯片电源关闭

#define B_PRG_KEY_STATUS        OPEN_PROG_KEY    //编程开关
#define DOWN_COVER_STATUS       OPEN1_STATUS  //开端盖(后端盖)铅封
#define UP_COVER_STATUS         OPEN2_STATUS  //开大盖(上端盖)


//端子排有无功脉冲输出指示

//有功方向输出
#define A_DIR_1   B_4IN1_1_1
#define A_DIR_0   B_4IN1_1_0
//无功方向输出
#define R_DIR_1   B_4IN1_2_1
#define R_DIR_0   B_4IN1_2_0

//有功输出
#define A_OUT_1   B_4IN1_3_1
#define A_OUT_0   B_4IN1_3_0

//无功输出
#define R_OUT_1   B_4IN1_4_1
#define R_OUT_0   B_4IN1_4_0


//有功脉冲 LED 输出口
#define MEASU_A_DIR_0        B_POWERDIR1_0   
#define MEASU_A_DIR_1        B_POWERDIR1_1

//无功脉冲 LED 输输出口
#define MEASU_R_DIR_0        B_POWERDIR2_0   //无功方向指示
#define MEASU_R_DIR_1        B_POWERDIR2_1


/**********************************************************************************
函数功能：清除外部看门狗
入口：    
出口：    
**********************************************************************************/

 


//喂狗信号
#define Clear_Ext_Dog()       {B_WTD_1;  B_WTD_0; B_WTD_0; B_WTD_1;}    //一个B_WTD_0大约 240ns


#define BEEP_MODE_50        50
#define BEEP_MODE_100       100
#define BEEP_MODE_150       150
#define BEEP_MODE_200       200
#define BEEP_MODE_300       300
#define BEEP_MODE_400       400
#define BEEP_MODE_500       500

//所有PORT设备ID
#define EXT_ID_NONE         0  //无效端子
#define EXT_ID_POWER_ALARM        (EXT_ID_NONE+1)          //电网信息报警，如超功率报警输出-------------1
#define EXT_ID_FEE_ALARM          (EXT_ID_POWER_ALARM+1)    //欠费报警输出------------------------------2
#define EXT_ID_TOGGLE             (EXT_ID_FEE_ALARM+1)      //跳闸输出----------------------------------3
#define EXT_ID_ACPOWER            (EXT_ID_TOGGLE+1)        //有功脉冲-----------------------------------4
#define EXT_ID_REACPOWER          (EXT_ID_ACPOWER+1)       //无功脉冲-----------------------------------5
#define EXT_ID_AP_DIR             (EXT_ID_REACPOWER+1)     //有功脉冲方向-------------------------------6
#define EXT_ID_RP_DIR             (EXT_ID_AP_DIR+1)        //无功脉冲方向-------------------------------7
#define EXT_ID_SEC_EN             (EXT_ID_RP_DIR+1)        //秒脉冲使能---------------------------------8
#define EXT_ID_DEMAND             (EXT_ID_SEC_EN+1)        //需量周期到输出-----------------------------9
#define EXT_ID_SLIPE              (EXT_ID_DEMAND+1)        //滑差周期输出-------------------------------10
#define EXT_ID_PARSE              (EXT_ID_SLIPE+1)        //时段切换输出--------------------------------11
#define GERNAL_ID_ACPOWER         (EXT_ID_PARSE+1)         //总有功脉冲---------------------------------12
#define GERNAL_ID_REACPOWER       (GERNAL_ID_ACPOWER+1)    //总无功脉冲---------------------------------13
#define INTER_ID_ALARM_DIS        (GERNAL_ID_REACPOWER+1)  //报警指示灯---------------------------------14
#define INTER_ID_ALARM_BEEP       (INTER_ID_ALARM_DIS+1)  //报警喇叭------------------------------------15
  

//最大的PORT设备ID数目
#define MIN_PORT_DI      (EXT_ID_NONE+1)
#define MAX_PORT_DI      (INTER_ID_ALARM_BEEP)

#define MAX_PORT_NUM      (MAX_PORT_DI+1)

//输出模式
#define LEVEL_MODE        0x00   //电平输出模式
#define PULSE_MODE        0x01   //脉冲输出模式


#define PORT_END   0
#define PORT_START 1

typedef struct
{ 
  INT8U InitFlag;           //端子初始化完成
  INT8U Status;             //状态：0表示无动作；1表示启动，2表示进行；3表示结束；
  INT16U CurCount;          //当前1ms计数，0表示执行动作结束，最大65536ms，某一轮回中的脉冲宽度
  INT16U CurCountBak;       //某一轮回中的脉冲宽度备份
  INT16U LoopCount;         //轮回数，每一个轮回出一个脉冲
  INT8U CS[CS_BYTES];
}PORT_HANDER;
PORT_ABS_PUCK_EXT volatile PORT_HANDER Ext_Port_Hander[MAX_PORT_NUM];


#define MODE_ERR     0     //未知模式
#define MODE_RUN     1     //现场运行模式，不输出调试信息，无权限
#define MODE_DEBUG   2     //调试模式，可输出调试信息，无权限
#define MODE_FAC     4     //工厂模式，可输出调试信息，具备最高权限
#define MODE_TEST    8     //自检模式，可输出调试信息，无权限


void Beep_For_Measu_Alarm_PUCK(void);
void Beep_For_OSC_Alarm_PUCK(void);
void Beep_For_Test_Alarm_PUCK(void);
void ExtPort_xMs_Hook(void);
void Init_All_Port(INT32U Mode);
void Port_Out_Pub(INT8U Id,INT16U Para);
INT8U Get_Meter_Hard_Mode(void);

INT8U Get_Sec_Out_En(void);
INT8U Get_Parse_Out_En(void);
INT8U Get_Demand_Out_En(void);
INT8U Get_Slipe_Out_En(void);
INT8U Get_FeeAlarm_Out_En(void);
INT8U Get_GridAlarm_Out_En(void);

#endif