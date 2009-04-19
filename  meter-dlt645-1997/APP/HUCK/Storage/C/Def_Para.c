#define DEF_PARA_C
#include "Includes.h"

#if METER_PLACE == 0
CONST INT8U Def_PSW_0[3]={0x00,0x00,0x00};
CONST INT8U Def_PSW_1[3]={0x00,0x00,0x00};
CONST INT8U Def_PSW_2[3]={0x00,0x00,0x00};


CONST INT8U Zero[100]={0};
//默认校表参数
CONST INT8U Def_ADJ_METER_PUL_VOL_CUR_SPEC[]={0x00,0x01,0x01};
CONST INT8U Def_ADJ_METER_PARA_STATUS[]={0x00};
/*
CONST INT8U Def_ADJ_METER_A_POW[36]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
CONST INT8U Def_MeterAddr0[]={0x00,0x00,0x00,0x00,0x00,0x00};
CONST INT8U Def_MeterAddr1[]={0x00,0x00,0x00,0x00,0x00,0x00};
*/
CONST INT8U Def_ADJ_METER_A_POW[]= 
{
BYTE_0(0x000C491E),BYTE_1(0x000C491E),BYTE_2(0x000C491E),
BYTE_0(0x000D08FA),BYTE_1(0x000D08FA),BYTE_2(0x000D08FA),
BYTE_0(0x000C3D32),BYTE_1(0x000C3D32),BYTE_2(0x000C3D32),
          
BYTE_0(0x00000000),BYTE_1(0x00000000),BYTE_2(0x00000000),
BYTE_0(0x00000000),BYTE_1(0x00000000),BYTE_2(0x00000000),
BYTE_0(0x00000000),BYTE_1(0x00000000),BYTE_2(0x00000000),
          
BYTE_0(0x000020CD),BYTE_1(0x000020CD),BYTE_2(0x000020CD),
BYTE_0(0x0001665E),BYTE_1(0x0001665E),BYTE_2(0x0001665E),
BYTE_0(0x00011A6E),BYTE_1(0x00011A6E),BYTE_2(0x00011A6E),
          
BYTE_0(0x00450100),BYTE_1(0x00450100),BYTE_2(0x00450100),
BYTE_0(0x00442897),BYTE_1(0x00442897),BYTE_2(0x00442897),
BYTE_0(0x00439E74),BYTE_1(0x00439E74),BYTE_2(0x00439E74),
  
};
#define Def_MeterAddr0      Zero
#define Def_MeterAddr1      Zero
#define Def_LoadData_Index  Zero
//CONST S_LoadData_Index Def_LoadData_Index=
//{0x00,0x00,0x00,{0x00,0x00,0x01,0x01,0x00,0x00,{0x00}},{0x00}};

//CONST S_Demand Def_Demand={0};
//CONST S_VolStat Def_VolStat={0};
#define Def_VolStat Zero

CONST INT8U Def_C047[]={0x00};//模式字1
CONST INT8U Def_C048[]={0x02};//模式字2
CONST INT8U Def_C049[]={0x00};//模式字3
CONST INT8U Def_C04A[]={0x02};//模式字4
CONST INT8U Def_C04B[]={0x06};//模式字5
CONST INT8U Def_C04C[]={0x03};//模式字6
CONST INT8U Def_C04D[]={0x0C};//模式字7
CONST INT8U Def_C04E[]={0x3F};//模式字8
CONST INT8U Def_CA1A[]={0x03};//模式字9-4合1模式字

/*
//复费率参数--test
CONST INT8U Def_C31F[]=
{
  (MAX_YEAR_PERIODS%10)+((MAX_YEAR_PERIODS/10)<<4),//年时区数(Byte%10)+((Byte/10)<<4)
  (MAX_DATE_TABLES%10)+((MAX_DATE_TABLES/10)<<4),//日时段表数
  (MAX_DATE_PERIODS%10)+((MAX_DATE_PERIODS/10)<<4),//日时段数
  (MAX_RATES%10)+((MAX_RATES/10)<<4),//费率数
  0x00,//公共假日数
  0x01,//周休日采用的时段表号
  0x3E//周休日状态字
};
*/

//复费率参数
CONST INT8U Def_C31F[]=
{
  0x01,//年时区数
  0x02,//日时段表数
  0x08,//日时段数
  0x04,//费率数
  0x00,//公共假日数
  0x01,//周休日采用的时段表号
  0x7F//周休日状态字
};

//年时区表,待改
//时段表号,日，月
CONST INT8U Def_C32F[]=
{
  1,1,1,
  1,1,2,
  1,1,3,
  1,1,4,
  1,1,5,
  1,1,6,  
  1,1,7,
  1,1,8,
  1,1,9,
  1,1,0x10,
  1,1,0x11,
  1,1,0x12    
};

//日时段表,待改
//费率，分，时
CONST INT8U Def_C33F[]=
{
  0x03,0x00,0x06,
  0x01,0x00,0x08,
  0x02,0x00,0x11,
  0x03,0x00,0x13,
  0x02,0x00,0x15,
  0x01,0x00,0x18,
  0x02,0x00,0x21,
  0x04,0x00,0x22,
  0x03,0x00,0x06,//从此开始同Def_C331
  0x01,0x00,0x08,
  0x02,0x00,0x11,
  0x03,0x00,0x13,
  0x02,0x00,0x15,
  0x01,0x00,0x18
};

//默认的三路设备地址
/*
CONST INT8U Def_Device_Addr[]=
{
  0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00  
};
*/
#define Def_Device_Addr Zero

//默认通信波特率
CONST INT8U Def_Comm_Baud[]=
{
  0x01,
  0x01,
  0x01
};

#define Def_Energy_Freeze_Time Zero
#define Def_Reactive_Freeze_Time Zero

//有功脉冲常数
CONST INT8U Def_C030[]=
{
  0x00,0x50,0x00
};

//无功脉冲常数
CONST INT8U Def_C031[]=
{
  0x00,0x50,0x00
};

//表号、用户号、设备码
/*
CONST INT8U Def_C032_4[]=
{
  0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00, 
  0x00,0x00,0x00,0x00,0x00,0x00
};
*/
#define Def_C032_4 Zero
//
CONST INT8U Def_C111_6[]=
{
  0x15,//需量周期
  0x01,//滑差时间
  0x06,//循显时间
  0x13,//A屏循显项目数
  0x03,//显示电能小数位数
  0x03//显示功率小数位数
};

//自动抄表日期
CONST INT8U Def_C117[]=
{
  0x00,0x01 
};

//负荷代表日
CONST INT8U Def_C118[]=
{
 0x01 
};

/*
//有功起始读数
CONST INT8U Def_C119[]=
{
 0x00,0x00,0x00,0x00 
};

//无功起始读数
CONST INT8U Def_C11A[]=
{
 0x00,0x00,0x00,0x00
};
*/
#define Def_C119 Zero
#define Def_C11A Zero

//自动抄表日期
CONST INT8U Def_C127[]=
{
  0x00,
  0x01
};

//B屏显示项目数
CONST INT8U Def_C128[]=
{
  0x20,0x00 
};

//输出脉冲宽度80ms
CONST INT8U Def_C211[]=
{
  0x80,0x00
};

//电压考核参数
CONST INT8U Def_C213[]=
{
  0x00,0x40,0x02,//合格上限 240V
  0x00,0x00,0x02,//合格下限 200V
  0x00,0x62,0x02,//电压考核上限 262V
  0x00,0x78,0x01//电压考核下限 178V
};

//用户级别
CONST INT8U Def_C214[]=
{
  0x00 
};

//跳闸、合闸延时
CONST INT8U Def_C215_6[]=
{
 0x00,
 0x00 
};

//节假日
/*
CONST INT8U Def_C41F[45]=
{
0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0
};
*/
#define Def_C41F Zero

//负荷曲线记录起始时间 1月1日0时0分
CONST INT8U Def_C510[]=
{
  0x00,0x00,0x01,0x01
};

//负荷曲线记录间隔,1小时
CONST INT8U Def_C511_6[]=
{
  0x60,0x00,
  0x60,0x00,
  0x60,0x00,
  0x60,0x00,
  0x60,0x00,
  0x60,0x00,
};

//A屏第1-10项显示项目
CONST INT8U Def_C610_9[]=
{
0x01,0x00,0x00,0x00,   0x00,0x00,0x00,0x00,   0x02,
0x02,0x00,0x00,0x00,   0x00,0x01,0x00,0x00,   0x02,
0x03,0x00,0x00,0x00,   0x00,0x02,0x00,0x00,   0x02,
0x04,0x00,0x00,0x00,   0x00,0x03,0x00,0x00,   0x02,
0x05,0x00,0x00,0x00,   0x20,0x00,0x00,0x00,   0x02,
0x06,0x00,0x00,0x00,   0x40,0x00,0x00,0x00,   0x02,
0x07,0x00,0x00,0x00,   0x50,0x00,0x00,0x00,   0x02,
0x08,0x00,0x00,0x00,   0x60,0x00,0x00,0x00,   0x02,
0x09,0x00,0x00,0x00,   0x70,0x00,0x00,0x00,   0x02,
0x10,0x00,0x00,0x00,   0x00,0x00,0x01,0x00,   0x02,
};

//A屏第11-20项显示项目
CONST INT8U Def_C620_9[]=
{
0x11,0x00,0x00,0x00,   0x00,0x00,0x02,0x00,   0x02,
0x12,0x00,0x00,0x00,   0x00,0x00,0x08,0x00,   0x02,
0x13,0x00,0x00,0x00,   0x01,0x00,0x08,0x00,   0x02,

0x01,0x00,0x00,0x00,   0x00,0x00,0x00,0x00,   0x02,
0x02,0x00,0x00,0x00,   0x00,0x01,0x00,0x00,   0x02,
0x03,0x00,0x00,0x00,   0x00,0x02,0x00,0x00,   0x02,
0x04,0x00,0x00,0x00,   0x00,0x03,0x00,0x00,   0x02,
0x05,0x00,0x00,0x00,   0x20,0x00,0x00,0x00,   0x02,
0x06,0x00,0x00,0x00,   0x40,0x00,0x00,0x00,   0x02,
0x07,0x00,0x00,0x00,   0x50,0x00,0x00,0x00,   0x02,
};


//B屏1-10项显示项目
CONST INT8U Def_C650_9[]=
{
0x20,0x00,0x00,0x00,   0x00,0x00,0x03,0x00,   0x02,
0x21,0x00,0x00,0x00,   0x00,0x01,0x03,0x00,   0x02,
0x22,0x00,0x00,0x00,   0x00,0x02,0x03,0x00,   0x02,
0x23,0x00,0x00,0x00,   0x00,0x03,0x03,0x00,   0x02,
0x24,0x00,0x00,0x00,   0x10,0x00,0x03,0x00,   0x02,
0x25,0x00,0x00,0x00,   0x10,0x01,0x03,0x00,   0x02,
0x26,0x00,0x00,0x00,   0x10,0x02,0x03,0x00,   0x02,
0x27,0x00,0x00,0x00,   0x10,0x03,0x03,0x00,   0x02,
0x28,0x00,0x00,0x00,   0x00,0x01,0x04,0x00,   0x02,
0x29,0x00,0x00,0x00,   0x00,0x02,0x04,0x00,   0x02
};

//B屏11-20项显示项目
CONST INT8U Def_C660_9[]=
{
0x30,0x00,0x00,0x00,   0x00,0x03,0x04,0x00,   0x02,
0x31,0x00,0x00,0x00,   0x00,0x01,0x05,0x00,   0x02,
0x32,0x00,0x00,0x00,   0x00,0x02,0x05,0x00,   0x02,
0x33,0x00,0x00,0x00,   0x00,0x03,0x05,0x00,   0x02,  

0x20,0x00,0x00,0x00,   0x00,0x00,0x03,0x00,   0x02,
0x21,0x00,0x00,0x00,   0x00,0x01,0x03,0x00,   0x02,
0x22,0x00,0x00,0x00,   0x00,0x02,0x03,0x00,   0x02,
0x23,0x00,0x00,0x00,   0x00,0x03,0x03,0x00,   0x02,
0x24,0x00,0x00,0x00,   0x10,0x00,0x03,0x00,   0x02,
0x25,0x00,0x00,0x00,   0x10,0x01,0x03,0x00,   0x02 
};

//通信规约版本号,45字节
CONST INT8U Def_C712[]=
{
 0 
};
//失压判断阀值 78V 10%
CONST INT8U Def_CA10[]=
{
  //0x00,0x10,0x00,0x78
  0x78,0x00,0x10,0x00
};

//失流判断阀值
CONST INT8U Def_CA11[]=
{
  //0x00,0x10,0x00,0x30
  0x30,0x00,0x10,0x00
};

//信号输出控制字
/*
CONST INT8U Def_CA12[]=
{0};
*/
#define Def_CA12 Zero

//主副费率切换时刻
CONST INT8U Def_CA13[]=
{
  0xFF,0xFF,0xFF,0xFF,0xFF 
};

//用户权限控制字
CONST INT8U Def_CA15[]=
{ 
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF 
};

//有功功率限额4.7520KW
CONST INT8U Def_CA16[]=
{
  0x20,0x75,0x04
};

//视在功率限额4.7520KW
CONST INT8U Def_CA17[]=
{
  0x20,0x75,0x04
};

//过流判定阀值
CONST INT8U Def_CA19[]=
{
  0x00,0x00,0x00
};

//注意，下表中出现的数据项，必须肯定在Data_Para_Storage中出现
CONST S_Def_Para_Info Def_Para[]=
{
  {PASSWORD_0,1,Def_PSW_0},
  {PASSWORD_1,1,Def_PSW_1},
  {PASSWORD_2,1,Def_PSW_2},
  
  {METER_ADDR0,1,Def_MeterAddr0},
  {METER_ADDR1,1,Def_MeterAddr1},

//默认电量数据，CUR_ENERGY_BAK0是读取数据是最后一个读取项，因此选择为该项提供默认项,一下几项皆同
  //{LOAD_DATA_INDEX,1,(INT8U CONST *)&Def_LoadData_Index},
  {LOAD_DATA_INDEX_BAK,1,(INT8U CONST *)&Def_LoadData_Index},
  
  {CUR_VOLSTAT,1,(INT8U CONST *)&Def_VolStat},
  {TOTAL_VOLSTAT,1,(INT8U CONST *)&Def_VolStat},
  //上1-13的历史电量、需量、需量时间，默认为0
  //{HIS_ENERGY,HIS_ENERGY_DATA_MONTHS,(INT8U CONST *)&Def_Energy},
  //{HIS_DEMAND,HIS_DEMAND_DATA_MONTHS,(INT8U CONST *)&Def_Demand},
  //{HIS_DEMAND_TIME,HIS_DEMAND_DATA_MONTHS,(INT8U CONST *)&Def_Demand_Time},
  //{HIS_VOLSTAT,HIS_VOLSTAT_DATA_MONTHS,(INT8U CONST *)&Def_VolStat},
  {PROG_RECORD,1,Zero},//编程时间和次数
  {CLR_DEMAND_RECORD,1,Zero},//清需量时间和次数
  
  {COMM_BAUD0,3,Def_Comm_Baud},//通信波特率
  {ENERGY_FREEZE_TIME,1,Def_Energy_Freeze_Time},//总电量冻结时间
  {REACTIVE_FREEZE_TIME,1,Def_Reactive_Freeze_Time},//四象限无功冻结时间
  
/*
  { OCCUR_TOTAL_LOSS_VOL,1,Zero},
  { OCCUR_A_LOSS_VOL,1,Zero},             
  { OCCUR_B_LOSS_VOL,1,Zero},
  { OCCUR_C_LOSS_VOL,1,Zero},
  { OCCUR_AB_LOSS_VOL,1,Zero},
  { OCCUR_AC_LOSS_VOL,1,Zero},
  { OCCUR_BC_LOSS_VOL,1,Zero},
  { OCCUR_ABC_LOSS_VOL,1,Zero},
  //全失压
  { OCCUR_ALL_LOSS_VOL,1,Zero},
  
  { TOTAL_TOTAL_LOSS_VOL,1,Zero},
  { TOTAL_A_LOSS_VOL,1,Zero},
  { TOTAL_B_LOSS_VOL,1,Zero},
  { TOTAL_C_LOSS_VOL,1,Zero},
  { TOTAL_AB_LOSS_VOL,1,Zero},
  { TOTAL_AC_LOSS_VOL,1,Zero},
  { TOTAL_BC_LOSS_VOL,1,Zero},
  { TOTAL_ABC_LOSS_VOL,1,Zero},
  //全失压
  { TOTAL_ALL_LOSS_VOL,1,Zero},
  
  //失流
  { OCCUR_TOTAL_LOSS_CUR,1,Zero},
  { OCCUR_A_LOSS_CUR,1,Zero},
  { OCCUR_B_LOSS_CUR,1,Zero},
  { OCCUR_C_LOSS_CUR,1,Zero},
  { OCCUR_AB_LOSS_CUR ,1,Zero},
  { OCCUR_AC_LOSS_CUR,1,Zero},
  { OCCUR_BC_LOSS_CUR,1,Zero},
  //全失流
  { OCCUR_ABC_LOSS_CUR,1,Zero},
  
  { TOTAL_TOTAL_LOSS_CUR,1,Zero},
  { TOTAL_A_LOSS_CUR,1,Zero},
  { TOTAL_B_LOSS_CUR,1,Zero},
  { TOTAL_C_LOSS_CUR,1,Zero},
  { TOTAL_AB_LOSS_CUR,1,Zero},
  { TOTAL_AC_LOSS_CUR,1,Zero},
  { TOTAL_BC_LOSS_CUR,1,Zero},
  { TOTAL_ABC_LOSS_CUR,1,Zero},
  
  //反向
  { OCCUR_TOTAL_NEG_CUR,1,Zero},
  { OCCUR_A_NEG_CUR,1,Zero},
  { OCCUR_B_NEG_CUR,1,Zero},
  { OCCUR_C_NEG_CUR,1,Zero},
  
  { TOTAL_TOTAL_NEG_CUR,1,Zero},
  { TOTAL_A_NEG_CUR ,1,Zero},
  { TOTAL_B_NEG_CUR,1,Zero},
  { TOTAL_C_NEG_CUR,1,Zero},
  
  //断相
  { OCCUR_TOTAL_BREAK_VOL,1,Zero},
  { OCCUR_A_BREAK_VOL,1,Zero},
  { OCCUR_B_BREAK_VOL,1,Zero},
  { OCCUR_C_BREAK_VOL,1,Zero},
  
  { TOTAL_TOTAL_BREAK_VOL,1,Zero},
  { TOTAL_A_BREAK_VOL,1,Zero},
  { TOTAL_B_BREAK_VOL,1,Zero},
  { TOTAL_C_BREAK_VOL,1,Zero},
  */
//--------------------------------------
  {0xB214,1,Zero},//电池工作时间
  //{0xB710,7,Zero},
  //{0xB720,6,Zero},
  //{0xB730,4,Zero},
  //{0xB740,3,Zero},
  
  {0xC047,1,Def_C047},//模式字1
  {0xC048,1,Def_C048},//模式字2
  {0xC049,1,Def_C049},//模式字3
  {0xC04A,1,Def_C04A},//模式字4
  {0XC04B,1,Def_C04B},//模式字5
  {0xC04C,1,Def_C04C},//模式字6
  {0xC04D,1,Def_C04D},//模式字7
  {0xC04E,1,Def_C04E},//模式字8  
  {0xCA1A,1,Def_CA1A},//模式字9
  
  {0xC111,6,Def_C111_6},//需量周期等参数
  {0xC117,1,Def_C117},//自动抄表日
  {0xC118,1,Def_C118},//负荷代表日
  {0xC119,1,Def_C119},//有功起始读数
  {0xC11A,1,Def_C11A},//无功起始读数
  
  {0xC128,1,Def_C128},//B屏显示项目数
  {0xC211,1,Def_C211},//输出脉冲宽度80ms
  {0xC213,1,Def_C213},//电压考核参数
  
  {0xC214,1,Def_C214},//用户级别
  {0xC215,2,Def_C215_6},//跳闸与合闸延时
  
  {0xC030,1,Def_C030},//有功脉冲常数
  {0xC031,1,Def_C031},//无功脉冲常数
  {0xC032,3,Def_C032_4},//表号，用户号，设备码
  
  {0xC310,7,Def_C31F},//第一套年时区数等
  {0xC32F,1,Def_C32F},//第一套年时区表
  
  {0xC33F,1,Def_C33F},//第一套日时段表
  {0xC34F,1,Def_C33F},
  {0xC35F,1,Def_C33F},
  {0xC36F,1,Def_C33F},
  {0xC37F,1,Def_C33F},
  {0xC38F,1,Def_C33F},
  {0xC39F,1,Def_C33F},
  {0xC3AF,1,Def_C33F},
  {0xC3BF,1,Def_C33F},
  {0xC3CF,1,Def_C33F},
  {0xC3DF,1,Def_C33F},
  {0xC3EF,1,Def_C33F},
  
  {0xC41F,1,Def_C41F},//第一套节假日 
  {0xC42F,1,Def_C41F},
  {0xC43F,1,Def_C41F},
  {0xC44F,1,Def_C41F},
  {0xC45F,1,Def_C41F},
  {0xC46F,1,Def_C41F},

  {0xCB10,7,Def_C31F},//第二套年时区数等
  {0xCB2F,1,Def_C32F},//第二套年时区表
  
  {0xCB3F,1,Def_C33F},//第二套日时段表
  {0xCB4F,1,Def_C33F},
  {0xCB5F,1,Def_C33F},
  {0xCB6F,1,Def_C33F},
  {0xCB7F,1,Def_C33F},
  {0xCB8F,1,Def_C33F},
  {0xCB9F,1,Def_C33F},
  {0xCBAF,1,Def_C33F},
  {0xCBBF,1,Def_C33F},
  {0xCBCF,1,Def_C33F},
  {0xCBDF,1,Def_C33F},
  {0xCBEF,1,Def_C33F},
  
  {0xC510,1,Def_C510},
  {0xC511,6,Def_C511_6},
  
  {0xC610,10,Def_C610_9},//A屏第1-10项
  {0xC620,10,Def_C620_9},//A屏第11-20项
  {0xC630,10,Def_C610_9},//A屏第21-30项
  {0xC640,10,Def_C610_9},//A屏第31-40项
  
  {0xC650,10,Def_C650_9},//B屏第01-10项
  {0xC660,10,Def_C660_9},//B屏第11-20项
  {0xC670,10,Def_C650_9},//B屏第21-30项
  {0xC680,10,Def_C650_9},//B屏第31-40项
  {0xC690,10,Def_C650_9},//B屏第41-50项
  {0xC6A0,10,Def_C650_9},//B屏第51-60项
  {0xC6B0,10,Def_C650_9},//B屏第61-70项
  {0xC6C0,10,Def_C650_9},//B屏第71-80项
  {0xC6D0,10,Def_C650_9},//B屏第81-90项
  {0xC6E0,10,Def_C650_9},//B屏第91-100项 
  
  {0xC91F,1,Zero},
  {0xC92F,1,Zero},
  {0xC93F,1,Zero},
  {0xC94F,1,Zero},
  {0xC95F,1,Zero},
  {0xC96F,1,Zero},
  
  {0xCA10,1,Def_CA10},//失压判断阀值 78V 10%
  {0xCA11,1,Def_CA11},//失流判断阀值
  {0xCA12,1,Def_CA12},//信号输出控制字
  {0xCA13,1,Def_CA13},//主副费率切换时刻
  {0xCA15,1,Def_CA15},//用户权限控制字
  {0xCA16,1,Def_CA16},//有功功率限额4.7520KW
  {0xCA17,1,Def_CA17},//视在功率限额4.7520KW
  {0xCA19,1,Def_CA19},//过流阀值
  
  {0xCC1F,1,Def_C41F}, //第二套节假日
  {0xCC2F,1,Def_C41F},
  {0xCC3F,1,Def_C41F},
  {0xCC4F,1,Def_C41F},
  {0xCC5F,1,Def_C41F},
  {0xCC6F,1,Def_C41F},
  
  {ADJ_METER_PUL_VOL_CUR_SPEC,1,Def_ADJ_METER_PUL_VOL_CUR_SPEC},//脉冲、电压、电流规格
  {ADJ_METER_PARA_STATUS,1,Def_ADJ_METER_PARA_STATUS},//校表参数状态设置
  {ADJ_METER_A_POW,12,Def_ADJ_METER_A_POW},//功率、相位、电压、电流设置参数一共有12项目
  
  DEF_PARA_EXTEND
};

#endif
//写一条默认参数，参数标识为DI
void Write_One_Def_Para(PROTO_DI DI)
{
  INT16U i;
  INT16U Len;
 
  if(Check_Meter_Factory_Status()==0)//当前不是工厂状态则推出
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Write Def Para failed! not in factory status!");
    return;
  }
  
  for(i=0;i<S_NUM(Def_Para);i++)
  {
    if(DI>=Def_Para[i].DI && DI<Def_Para[i].DI+Def_Para[i].Num)
    {
      if(Check_Meter_Factory_Status()==0)//当前不是工厂状态则推出
        return;
      Len=Get_Storage_Data_Len(Def_Para[i].DI);
      Write_Storage_Data(DI,(void *)(Def_Para[i].pPara+(DI-Def_Para[i].DI)*Len),Len);
      return;
    }
  }  
}
//设置默认的校表参数,设为全0
void Set_Def_Adj_Para()
{
  PROTO_DI DI;
  INT8U Temp[4];
  
  Main_Dis_Info("CLr PArA");
  mem_set(Temp,0,sizeof(Temp),Temp,sizeof(Temp));
  Write_Storage_Data(0xCEE1,Temp,1);
  for(DI=0xCEE2;DI<=0xCEED;DI++)
  {
    Write_Storage_Data(DI,Temp,3);
    //Write_One_Def_Para(DI);
  }
}

//读数据项DI的默认参数
INT16U Read_Def_Para(STORA_DI DI,void *pDst,INT16U Len,void *pDst_Start,INT16U DstLen)
{
  INT16U i;
  for(i=0;i<S_NUM(Def_Para);i++)
  {
    if(DI==Def_Para[i].DI ||\
      (DI>Def_Para[i].DI && DI<Def_Para[i].DI+Def_Para[i].Num))
    {
      mem_cpy(pDst,(INT8U *)Def_Para[i].pPara+(DI-Def_Para[i].DI)*Len,Len,pDst_Start,DstLen);
      return Len;
    }
  }
  return 0;
}

//设置所有数据为默认数据
void Set_All_Def_Data()
{
  if(Check_Meter_Factory_Status()==0)//当前不是工厂状态则推出
    return; 
  
  Init_Clock(SYS_NORMAL);//设置历史电量时需要时间
  Data_Freeze_Time.DDHH[0]=0;
  Data_Freeze_Time.DDHH[1]=1;
  SET_STRUCT_SUM(Data_Freeze_Time);//结算时间
  
  Set_Def_Energy_Data();//设置默认的电量数据
  if(Check_Meter_Factory_Status()==0)//当前不是工厂状态则推出
    return;
  Set_Def_Demand_Data();//设置默认的需量数据
  if(Check_Meter_Factory_Status()==0)//当前不是工厂状态则推出
    return;
  Set_Def_VolStat_Data();//设置默认的电压统计数据
  if(Check_Meter_Factory_Status()==0)//当前不是工厂状态则推出
    return;
  Set_Def_Load_Data_Index();
  if(Check_Meter_Factory_Status()==0)//当前不是工厂状态则推出
    return;  
  Set_Def_Freeze_Data();//设置默认的冻结数据
  if(Check_Meter_Factory_Status()==0)//当前不是工厂状态则推出
    return;
  Set_Def_Event_Data();//设置默认的事件数据   
  
}

//设置除校表参数意外的所有参数
void Set_Def_Para_Except_Adj()
{
  INT16U i,j,Len;
  
  Main_Dis_Info("CLr PArA");
  for(i=0;i<S_NUM(Def_Para);i++)
  {
    if(Check_Meter_Factory_Status()==0)//当前不是工厂状态则推出
      return;
    
    Len=Get_Storage_Data_Len(Def_Para[i].DI);
    
    for(j=0;j<Def_Para[i].Num;j++)
    {
      if(Check_Meter_Factory_Status()==0)
        return;   
      if(!(Def_Para[i].DI+j>=0xCEE0 && Def_Para[i].DI+j<=0xCEEF))
        Write_Storage_Data(Def_Para[i].DI+j,(void *)(Def_Para[i].pPara+j*Len),Len);
    }
  }  
}

//写默认参数到存储器中,该函数必须在工厂状态下才能执行
void Write_Def_Para()
{
  INT16U i,j;
  INT16U Len;
 
  DISP_CLR_DATA_INFO;
  for(i=0;i<S_NUM(Def_Para);i++)
  {
    if(Check_Meter_Factory_Status()==0)//当前不是工厂状态则推出
      return;
    
    Len=Get_Storage_Data_Len(Def_Para[i].DI);
    
    for(j=0;j<Def_Para[i].Num;j++)
    {
      if(Check_Meter_Factory_Status()==0)
        return;   
      //if(!(Def_Para[i].DI+j>=0xCEE0 && Def_Para[i].DI+j<=0xCEED))//test
      Write_Storage_Data(Def_Para[i].DI+j,(void *)(Def_Para[i].pPara+j*Len),Len);
    }
  }
  
  Set_All_Def_Data(); 
  
  DEBUG_PRINT(HUCK,DEBUG_0,"--------------Set Def Para End!------------");
}

//检查某个参数项是否有默认参数,有则返回1，否则返回0
INT8U Check_Def_Para(STORA_DI DI)
{
  INT16U i;
 
  for(i=0;i<S_NUM(Def_Para);i++)
  {
    if(DI>=Def_Para[i].DI && DI<Def_Para[i].DI+Def_Para[i].Num)
    {
      return 1;
    }
  }  
  return 0;
}


//检查是否需要设置默认参数
void Check_Boot_On_Flag()
{
  INT32U Boot_Flag;
  INT8U Err;
 
/*
  Boot_Flag=0;
  Write_Storage_Data(BOOT_FLAG,&Boot_Flag,sizeof(Boot_Flag));
  
  Main_Dis_Info("CLr PArA");  
  while(1)
  {
    Clr_Ext_Inter_Dog();
  }
*/       
  if(Check_Meter_Factory_Status()==1)//只有在工厂状态才能设置默认参数
  {
    if(Check_Sys_Status()==SYS_NORMAL)//系统电源正常的情况下才进行数据冻结和需量处理
    {
      Check_Sys_Storage();//存储系统自检  
      
      Boot_Flag=0;
      if(Read_Storage_Data(BOOT_FLAG,&Boot_Flag,&Boot_Flag,sizeof(Boot_Flag),&Err)==sizeof(Boot_Flag) &&\
         FIRST_BOOT_ON_FLAG==Boot_Flag)
        return;
      else
      {
        Read_Ext_RTC_Status();//第一次上电读取一下外部时钟！震荡器停止状态位清除,避免报警

        Write_Def_Para();
        Boot_Flag=FIRST_BOOT_ON_FLAG;
        Write_Storage_Data(BOOT_FLAG,&Boot_Flag,sizeof(Boot_Flag));
      }
    }
  }
}

#undef DEF_PARA_C
