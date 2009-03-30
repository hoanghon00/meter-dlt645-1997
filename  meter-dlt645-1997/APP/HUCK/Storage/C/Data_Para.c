#define DATA_PARA_C
#include "includes.h"

//定义各种数据的存储分布
CONST S_Data_Mem_Map Data_Mem_Map[]=
{
  //掉电存储器放前面，以利于快速检索
  {IMP6,{LOG_MEM9,NULL_LOG_MEM,NULL_LOG_MEM,NULL_LOG_MEM,NULL_LOG_MEM}},//掉电存储数据或电量备份
  
  {IMP0,{LOG_MEM0,LOG_MEM1,LOG_MEM4,LOG_MEM10,LOG_MEM13}},//重要参数
  {IMP1,{LOG_MEM2,LOG_MEM5,LOG_MEM6,NULL_LOG_MEM,NULL_LOG_MEM}},//当前电量需量等数据
  {IMP2,{LOG_MEM3,LOG_MEM11,LOG_MEM12,NULL_LOG_MEM,NULL_LOG_MEM}},//历史电量需量
  {IMP3,{LOG_MEM7,NULL_LOG_MEM,NULL_LOG_MEM,NULL_LOG_MEM,NULL_LOG_MEM}},//事件记录
  {IMP4,{LOG_MEM14,LOG_MEM15,LOG_MEM16,LOG_MEM17,LOG_MEM18}},//参数等
  {IMP5,{LOG_MEM19,NULL_LOG_MEM,NULL_LOG_MEM,NULL_LOG_MEM,NULL_LOG_MEM}},//负荷曲线
  {IMP7,{LOG_MEM20,LOG_MEM21,NULL_LOG_MEM,NULL_LOG_MEM,NULL_LOG_MEM}},//重要事件记录
};

//数据存储映射表
CONST S_Data_Para_Storage_Info Data_Para_Storage[]=
{
 {BOOT_FLAG,IMP1,4,1},//启动标志，表征是否是第一次启动
 
 {CUR_ENERGY_PD,IMP6,ENERGY_SAVE_SIZE,1},//当前电量,掉电存储,平时也定时存储
 {CUR_DEMAND_PD,IMP6,sizeof(S_Demand),1},//掉电只保存总和当前费率需量，即S_Demand_

 {CUR_VOLSTAT_PD,IMP6,sizeof(S_VolStat),1},//当前电压统计数据
 {TOTAL_VOLSTAT_PD,IMP6,sizeof(S_VolStat),1},//总电压检测数据
 //{CUR_EVENT_PD,IMP6,sizeof(S_Event_Cumu),1},//事件累计数据
 
 {LOAD_DATA_INDEX,IMP1,sizeof(S_LoadData_Index),1},
 //负荷曲线存储信息,每存储一条负荷曲线存储一次该数据,存储频繁，因此选择FRAM作为存储介质

 {CUR_ENERGY,IMP6,ENERGY_SAVE_SIZE,1},//当前电量
 {CUR_DEMAND,IMP6,DEMAND_SAVE_SIZE,1},//当月最大需量
 
 {TIME_PD,IMP6,sizeof(Cur_Time1),1},
 {TIME_BAK_PD,IMP6,sizeof(Cur_Time1),1},
 {CUMU_EVENT_PD,IMP6,sizeof(S_Event_Cumu_Data),1},//事件掉电数据
 
 {SAVE_PD_DATA_TIME,IMP6,4,1},//掉电存数所花掉的时间，单位ms
 {LOAD_DATA_INDEX_PD,IMP6,sizeof(S_LoadData_Index),1},
 
 {CUR_VOLSTAT,IMP1,sizeof(S_VolStat),1},//当前电压统计数据
 {TOTAL_VOLSTAT,IMP1,sizeof(S_VolStat),1},//总电压检测数据
 //{EVENT_CUMU,IMP1,sizeof(S_Event_Cumu),1},//事件累计数据
 
 {METER_ADDR0,IMP1,6,1},//设备地址0
 {METER_ADDR1,IMP1,6,1},//设备地址1
 {METER_ADDR2,IMP1,6,1},//设备地址2
 
 {COMM_BAUD0,IMP1,1,1},//第一路波特率
 {COMM_BAUD1,IMP1,1,1},//第二路波特率
 {COMM_BAUD2,IMP1,1,1},//第三路波特率
 
 {PASSWORD_0,IMP1,3,1},//0级密码
 {PASSWORD_1,IMP1,3,1},//1级密码
 {PASSWORD_2,IMP1,3,1},//2级密码

 {CUR_ENERGY_BAK0,IMP1,ENERGY_SAVE_SIZE,1},//当前电量备份,平时定时存储
 {CUR_DEMAND_BAK,IMP1,DEMAND_SAVE_SIZE,1},//当月最大需量及其发生时间
 {CUR_ENERGY_BAK1,IMP1,ENERGY_SAVE_SIZE,1},//当前电量备份,平时定时存储
 {CUR_ENERGY_BAK2,IMP2,ENERGY_SAVE_SIZE,1},//当前电量备份,平时定时存储
 {CUR_VOLSTAT_BAK,IMP1,sizeof(S_VolStat),1},//当前电压统计数据备份
 {TOTAL_VOLSTAT_BAK,IMP1,sizeof(S_VolStat),1},//总电压检测数据备份
 //{EVENT_CUMU_BAK,IMP1,sizeof(S_Event_Cumu),1},//事件累计数据备份

 {ENERGY_FREEZE_TIME,IMP1,2,1},//周期冻结时间
 {REACTIVE_FREEZE_TIME,IMP1,2,1},//周期冻结时间
 
 {PROG_RECORD,IMP1,sizeof(Prog_Record),1},
 {CLR_DEMAND_RECORD,IMP1,sizeof(Clr_Demand_Record),1},
 
 {LOAD_DATA_INDEX_BAK,IMP1,sizeof(S_LoadData_Index),1},//负荷曲线存储索引备份等
 {LOAD_DATA_HOUR_INDEX,IMP5,2,LOAD_DATA_HOUR_INDEX_NUM},//定义负荷曲线小时索引,LOAD_DATA_HOUR_INDEX_NUM条
 {LOAD_DATA,IMP5,LOAD_DATA_SIZE,LOAD_DATA_NUM},//定义负荷曲线,LOAD_DATA_NUM条

 
 {HIS_ENERGY,IMP2,ENERGY_SAVE_SIZE,HIS_ENERGY_DATA_MONTHS},//上13月电量
 {HIS_DEMAND,IMP2,DEMAND_SAVE_SIZE,HIS_DEMAND_DATA_MONTHS},//上13月最大需量
 {HIS_VOLSTAT,IMP2,sizeof(S_VolStat),HIS_VOLSTAT_DATA_MONTHS},//上3个月电压统计数据
//////////////////事件记录DI///////////
 {E_E400,IMP7,sizeof(STORA_DI),1},
 
 {E_E900,IMP7,sizeof(STORA_DI),1},
 {E_E910,IMP7,sizeof(STORA_DI),1},
 {E_E920,IMP7,sizeof(STORA_DI),1},
 {E_E930,IMP7,sizeof(STORA_DI),1},
 {E_E940,IMP7,sizeof(STORA_DI),1},
 {E_E950,IMP7,sizeof(STORA_DI),1},
 {E_E960,IMP7,sizeof(STORA_DI),1},
 {E_E970,IMP7,sizeof(STORA_DI),1},
 {E_E980,IMP7,sizeof(STORA_DI),1},
 {E_E990,IMP7,sizeof(STORA_DI),1},
 {E_E9A0,IMP7,sizeof(STORA_DI),1},
 {E_E9B0,IMP7,sizeof(STORA_DI),1},
 {E_E9C0,IMP7,sizeof(STORA_DI),1},
 {E_E9D0,IMP7,sizeof(STORA_DI),1},
 //{E_E9E0,IMP1,sizeof(STORA_DI),1},
 //{E_E9F0,IMP1,sizeof(STORA_DI),1},

 {E_EA00,IMP7,sizeof(STORA_DI),1},
 {E_EA10,IMP7,sizeof(STORA_DI),1},
 {E_EA20,IMP7,sizeof(STORA_DI),1},
 {E_EA30,IMP7,sizeof(STORA_DI),1},
 {E_EA40,IMP7,sizeof(STORA_DI),1},
 {E_EA50,IMP7,sizeof(STORA_DI),1},
 {E_EA60,IMP7,sizeof(STORA_DI),1},
 {E_EA70,IMP7,sizeof(STORA_DI),1},
 {E_EA80,IMP7,sizeof(STORA_DI),1},
 {E_EA90,IMP7,sizeof(STORA_DI),1},
 {E_EAA0,IMP7,sizeof(STORA_DI),1},
 {E_EAB0,IMP7,sizeof(STORA_DI),1},
 {E_EAC0,IMP7,sizeof(STORA_DI),1},
 {E_EAD0,IMP7,sizeof(STORA_DI),1},
 {E_EAE0,IMP7,sizeof(STORA_DI),1},
 {E_EAF0,IMP7,sizeof(STORA_DI),1},

 {E_EB00,IMP7,sizeof(STORA_DI),1},
 {E_EB10,IMP7,sizeof(STORA_DI),1},
 {E_EB20,IMP7,sizeof(STORA_DI),1},
 {E_EB30,IMP7,sizeof(STORA_DI),1},
 {E_EB40,IMP7,sizeof(STORA_DI),1},
 {E_EB50,IMP7,sizeof(STORA_DI),1},
 {E_EB60,IMP7,sizeof(STORA_DI),1},
 {E_EB70,IMP7,sizeof(STORA_DI),1},
 {E_EB80,IMP7,sizeof(STORA_DI),1},
 {E_EB90,IMP7,sizeof(STORA_DI),1},
 {E_EBA0,IMP7,sizeof(STORA_DI),1},
 {E_EBB0,IMP7,sizeof(STORA_DI),1},
 {E_EBC0,IMP7,sizeof(STORA_DI),1},
 {E_EBD0,IMP7,sizeof(STORA_DI),1},
 {E_EBE0,IMP7,sizeof(STORA_DI),1},
 {E_EBF0,IMP7,sizeof(STORA_DI),1},

 {E_EC00,IMP7,sizeof(STORA_DI),1},
 {E_EC10,IMP7,sizeof(STORA_DI),1},
 {E_EC20,IMP7,sizeof(STORA_DI),1},
 {E_EC30,IMP7,sizeof(STORA_DI),1},
 {E_EC40,IMP7,sizeof(STORA_DI),1},
 {E_EC50,IMP7,sizeof(STORA_DI),1},
 {E_EC60,IMP7,sizeof(STORA_DI),1},
 {E_EC70,IMP7,sizeof(STORA_DI),1},
 {E_EC80,IMP7,sizeof(STORA_DI),1},
 {E_EC90,IMP7,sizeof(STORA_DI),1},
 {E_ECA0,IMP7,sizeof(STORA_DI),1},
 {E_ECB0,IMP7,sizeof(STORA_DI),1},
 {E_ECC0,IMP7,sizeof(STORA_DI),1},
 {E_ECD0,IMP7,sizeof(STORA_DI),1},
 {E_ECE0,IMP7,sizeof(STORA_DI),1},
 {E_ECF0,IMP7,sizeof(STORA_DI),1},

 {E_ED00,IMP7,sizeof(STORA_DI),1},
 {E_ED10,IMP7,sizeof(STORA_DI),1},
 {E_ED20,IMP7,sizeof(STORA_DI),1},
 {E_ED30,IMP7,sizeof(STORA_DI),1},
 {E_ED40,IMP7,sizeof(STORA_DI),1},
 {E_ED50,IMP7,sizeof(STORA_DI),1},
 {E_ED60,IMP7,sizeof(STORA_DI),1},
 {E_ED70,IMP7,sizeof(STORA_DI),1},
 {E_ED80,IMP7,sizeof(STORA_DI),1},
 {E_ED90,IMP7,sizeof(STORA_DI),1},
 {E_EDA0,IMP7,sizeof(STORA_DI),1},
 {E_EDB0,IMP7,sizeof(STORA_DI),1},
 {E_EDC0,IMP7,sizeof(STORA_DI),1},
 {E_EDD0,IMP7,sizeof(STORA_DI),1},
 {E_EDE0,IMP7,sizeof(STORA_DI),1},
 
 //分相失压数据
 //{TOTAL_LOSS_VOL,IMP7,sizeof(S_Phase_Cumu),9},
 //分相失流数据
 //{TOTAL_LOSS_CUR,IMP7,sizeof(S_Phase_Cumu),8},
 //分相断相数据
 //{TOTAL_BREAK_VOL,IMP7,sizeof(S_Phase_Cumu),4},
 //反向数据
 //{TOTAL_REVERSE,IMP7,sizeof(S_Reverse_Cumu),4},
 //合相失压
 //{_TOTAL_LOSS_VOL,IMP7,sizeof(S_Loss_Vol_Cumu),9},
  //合相失流
 //{_TOTAL_LOSS_CUR,IMP7,sizeof(S_Loss_Cur_Cumu),8},

//////////////////以下的数据项都是在Protocol_Data中定义了的////////////////
 
 {0xB214,IMP1,4,1},//时钟电池工作时间
 
// {0xB220,IMP1,36		R	分	36	电压监测总累计数据 （注1）
//B221		R	分	36	  本月电压监测数据（注1）
//B222		R	分	36	上1月电压监测数据（注1）
//B223		R	分	36	上2月电压监测数据（注1）
//B224		R	分	36	上3月电压监测数据（注1）
//E501		R	%	3	本月A相电压合格率（注7）
//E502		R	%	3	本月B相电压合格率（注7）
//E503		R	%	3	本月C相电压合格率（注7）
//E511		R	%	3	上1月A相电压合格率（注7）
//E512		R	%	3	上1月B相电压合格率（注7）
//E513		R	%	3	上1月C相电压合格率（注7）
//E521		R	%	3	上2月A相电压合格率（注7）
//E522		R	%	3	上2月B相电压合格率（注7）
//E523		R	%	3	上2月C相电压合格率（注7）
//E531		R	%	3	上3月A相电压合格率（注7）
//E532		R	%	3	上3月B相电压合格率（注7）
//E533		R	%	3	上3月C相电压合格率（注7）

 //{0xB310,IMP7,4,4},//总失压次数
 //{0xB311,IMP1,2,1},//A相失压次数
 //{0xB312,IMP1,2,1},//B相失压次数
 //{0xB313,IMP1,2,1},//C相失压次数

 //{0xB320,IMP7,4,8},//总失压累计时间    
 //{0xB321,IMP1,3,1},//A相失压累计时间
 //{0xB322,IMP1,3,1},//B相失压累计时间
 //{0xB323,IMP1,3,1},//C相失压累计时间
 //{0xB324,IMP1,3,1},//总全失压累计时间间
 //{0xB328,IMP1,3,1},//总失流累计时间                            
 //{0xB329,IMP1,3,1},//A相失流累计时间 
 //{0xB32A,IMP1,3,1},//B相失流累计时间 
 //{0xB32B,IMP1,3,1},//C相失流累计时间 

 {0xB330,IMP1,4,1},//最近一次失压起始时刻     
 {0xB331,IMP1,4,1},//A相最近一次失压起始时刻  
 {0xB332,IMP1,4,1},//B相最近一次失压起始时刻  
 {0xB333,IMP1,4,1},//C相最近一次失压起始时刻  
 {0xB334,IMP1,4,1},//AB相最近一次失压起始时刻 
 {0xB335,IMP1,4,1},//AC相最近一次失压起始时刻 
 {0xB336,IMP1,4,1},//BC相最近一次失压起始时刻 
 {0xB337,IMP1,4,1},//ABC相最近一次失压起始时刻
 {0xB338,IMP1,4,1},//全失压最近一次起始时刻

 {0xB340,IMP1,4,1},//最近一次失压结束时刻     
 {0xB341,IMP1,4,1},//A相最近一次失压结束时刻  
 {0xB342,IMP1,4,1},//B相最近一次失压结束时刻  
 {0xB343,IMP1,4,1},//C相最近一次失压结束时刻  
 {0xB344,IMP1,4,1},//AB相最近一次失压结束时刻 
 {0xB345,IMP1,4,1},//AC相最近一次失压结束时刻 
 {0xB346,IMP1,4,1},//BC相最近一次失压结束时刻 
 {0xB347,IMP1,4,1},//ABC相最近一次失压结束时刻
 {0xB348,IMP1,4,1},//全失压最近一次结束时刻

 //{0xB710,IMP7,sizeof(S_Loss_Vol_Cumu),7},//失压累计数据   
 //{0xB720,IMP3,sizeof(S_Loss_Cur_Cumu),6},//失流累计数据
 //{0xB730,IMP3,sizeof(S_Reverse_Cumu),4},//反向累计数据
 //{0xB740,IMP3,sizeof(S_Phase_Cumu),3},//断相累计数据   

 
 {0xC022,IMP4,1,1},//周休日状态字
 {0xC023,IMP4,7,1},//计量方式字1
 {0xC024,IMP4,7,1},//计量方式字2

       
 {0xC030,IMP4,3,1},//有功常数
 {0xC031,IMP4,3,1},//无功常数
 {0xC032,IMP4,6,1},//表号
 {0xC033,IMP4,6,1},//用户号
 {0XC034,IMP4,6,1},//设备码

 {0xC047,IMP0,1,1},//模式字1
 {0xC048,IMP0,1,1},//模式字2
 {0xC049,IMP0,1,1},//模式字3
 {0xC04A,IMP4,1,1},//模式字4//属于用于模式字
 {0XC04B,IMP4,1,1},//模式字5
 {0xC04C,IMP4,1,1},//模式字6
 {0xC04D,IMP4,1,1},//模式字7
 {0xC04E,IMP4,1,1},//模式字8

 {0xC111,IMP4,1,1},//最大需量周期                                               
 {0xC112,IMP4,1,1},//滑差时间                                                     
 {0xC113,IMP4,1,1},//循显时间                                                     
 {0xC114,IMP4,1,1},//A屏显示项目数                                                
 {0XC115,IMP4,1,1},//电能[协议和显示]小数位[范围 ：2或3]                          
 {0xC116,IMP4,1,1},//功率&需量[显示]小数位 [显示小数位为2或3或4；协议固定为4] 
 {0xC117,IMP4,2,1},//自动抄表日期 
 {0XC118,IMP4,1,1},//负荷代表日                        
 {0xC119,IMP4,4,1},//有功起始读数
 {0xC11A,IMP4,4,1},//无功起始读数
 
 //{0xC121,IMP1,1,1},//最大需量周期--同C111-C117，因此不用                                               
 //{0xC122,IMP1,1,1},//滑差时间                                                     
 //{0xC123,IMP1,1,1},//循显时间                                                     
 //{0xC124,IMP1,1,1},//A屏显示项目数                                                
 //{0xC125,IMP1,1,1},//电能[协议和显示]小数位[范围 ：2或3]                          
 //{0xC126,IMP1,1,1},//功率&需量[显示]小数位 [显示小数位为2或3或4；协议固定为4] 
 //{0xC127,IMP1,2,1},//自动抄表日期                                                 
 {0xC128,IMP4,2,1},//B屏显示项目数                                                
 {0xC129,IMP4,2,1},//电表自检状态信息字        
                          
 {0xC211,IMP4,2,1}, //输出脉冲宽度                                         
 {0xC213,IMP4,12,1},//电压合格率参数                                         
 {0xC214,IMP4,1,1}, //用户级别，1或2或3                                      
 {0xC215,IMP4,1,1}, //跳闸延时                                               
 {0XC216,IMP4,1,1}, //合闸延时                                               
 {0XC217,IMP4,1,1}, //密码闭锁次数
 
 {0xC310,IMP4,1,1}, //第1套年时区数P≤12                                
 {0xC311,IMP4,1,1}, //第1套日时段表数q≤8                               
 {0xC312,IMP4,1,1}, //第1套日时段数m≤14                                
 {0xC313,IMP4,1,1}, //第1套费率数k≤8                                   
 {0XC314,IMP4,1,1}, //第1套公共假日数≤90          
 {0xC315,IMP4,1,1}, //第1套周休日采用的时段表号    
 {0xC316,IMP4,1,1}, //第1套周休日状态字（见附录B8）                    
 
 {0xC32F,IMP4,3*MAX_YEAR_PERIODS,1}, //第1套年时区                                                  

 {0xC33F,IMP4,3*MAX_DATE_PERIODS,1}, //第1套第1时段表                                     
 {0xC34F,IMP4,3*MAX_DATE_PERIODS,1}, //第1套第2时段表                                    
 {0xC35F,IMP4,3*MAX_DATE_PERIODS,1}, //第1套第3时段表                                     
 {0xC36F,IMP4,3*MAX_DATE_PERIODS,1}, //第1套第4时段表
 {0xC37F,IMP4,3*MAX_DATE_PERIODS,1}, //第1套第5时段表                                                                           
 {0xC38F,IMP4,3*MAX_DATE_PERIODS,1}, //第1套第6时段表                                     
 {0xC39F,IMP4,3*MAX_DATE_PERIODS,1}, //第1套第7时段表                                     
 {0xC3AF,IMP4,3*MAX_DATE_PERIODS,1}, //第1套第8时段表
 {0xC3BF,IMP4,3*MAX_DATE_PERIODS,1}, //第1套第9时段表
 {0xC3CF,IMP4,3*MAX_DATE_PERIODS,1}, //第1套第10时段表
 {0xC3DF,IMP4,3*MAX_DATE_PERIODS,1}, //第1套第11时段表
 {0xC3EF,IMP4,3*MAX_DATE_PERIODS,1}, //第1套第12时段表

 
 {0xC41F,IMP4,45,1}, //第1套第1-13个公共假日 
 //{0xC41E,IMP1,3,1}, //周休日采用日时段表号  
 {0xC42F,IMP4,45,1}, //第1套第16-30个公共假日 
 {0xC43F,IMP4,45,1}, //第1套第31-45个公共假日 
 {0xC44F,IMP4,45,1}, //第1套第46-60个公共假日 
 {0xC45F,IMP4,45,1}, //第1套第61-75个公共假日 
 {0xC46F,IMP4,45,1}, //第1套第76-90个公共假日
 
 {0xC510,IMP4,4,1},//负荷记录起始时间       
 {0xC511,IMP4,2,1},//负荷曲线1类数据记录间隔
 {0xC512,IMP4,2,1},//负荷曲线2类数据记录间隔
 {0xC513,IMP4,2,1},//负荷曲线3类数据记录间隔
 {0xC514,IMP4,2,1},//负荷曲线4类数据记录间隔
 {0xC515,IMP4,2,1},//负荷曲线5类数据记录间隔
 {0xC516,IMP4,2,1},//负荷曲线6类数据记录间隔

 {0xC51A,IMP4,2,1},//编程有效时间       
 {0xC51B,IMP4,1,1},//失压判断时间
 {0xC51C,IMP4,1,1},//失流判断时间
 {0xC51D,IMP4,1,1},//断相判断时间


 {0xC610,IMP4,9,15},//A屏第0-15项显示
 {0xC620,IMP4,9,15},//A屏第16-30项显示
 {0xC630,IMP4,9,15},//A屏第31-45项显示
 {0xC640,IMP4,9,15},//A屏第46-60项显示

 {0xC650,IMP4,9,15},//B屏第0-15项显示
 {0xC660,IMP4,9,15},//B屏第16-30项显示
 {0xC670,IMP4,9,15},//B屏第31-45项显示
 {0xC680,IMP4,9,15},//B屏第46-60项显示
 {0xC690,IMP4,9,15},//B屏第61-75项显示
 {0xC6A0,IMP4,9,15},//B屏第76-90项显示
 {0xC6B0,IMP4,9,15},//B屏第91-105项显示
 {0xC6C0,IMP4,9,15},//B屏第106-120项显示
 {0xC6D0,IMP4,9,15},//B屏第121-135项显示
 {0xC6E0,IMP4,9,15},//B屏第136-150项显示

 {0xC712,IMP4,45,1},//通讯规约版本号(参见企标QG/OKRW013－2005,  QG/OKRW004.7-2004) 
 //{0xC713,IMP4,1,1}, //读电表是否处于工厂状态，写退出工厂状态
 
 {0xC91F,IMP4,36,1},//正有功初始电量块  （总和8个分费率）
 {0xC92F,IMP4,36,1},//反有功初始电量块                   
 {0xC93F,IMP4,36,1},//1象限无功初始电量块                
 {0xC94F,IMP4,36,1},//2象限无功初始电量块                
 {0xC95F,IMP4,36,1},//3象限无功初始电量块                
 {0xC96F,IMP4,36,1},//4象限无功初始电量块                
                                                                                                          
 {0xCA10,IMP4,4,1},//失压判定阀值                
 {0xCA11,IMP4,4,1},//失流判定阀值                
 {0xCA12,IMP4,1,1},//脉冲输出切换  (见附录9)                                                          
 {0xCA13,IMP4,5,1},//主副费率切换时刻                                                                 
 {0xCA14,IMP4,2,1},//当前套费率,执行费率(见附录9)                                                     
 {0xCA15,IMP4,6,1},//用户权限控制字（见附录10）  
 {0xCA16,IMP4,3,1},//有功功率限额                                                                      
 {0xCA17,IMP4,3,1},//视在功率限额                 
 {0xCA18,IMP4,1,1},//打包费率数 
 {0xCA19,IMP4,3,1},//过流阀值 
 {0xCA1A,IMP4,1,1},//模式字9                
 //{0xCA1B,IMP1,1,1},//内卡状态
 
 {0xCB10,IMP4,1,1},//第2套年时区数P≤12           
 {0xCB11,IMP4,1,1},//第2套日时段表数q≤8          
 {0xCB12,IMP4,1,1},//第2套日时段数m≤14                                                              
 {0xCB13,IMP4,1,1},//第2套费率数k≤8                                                                 
 {0xCB14,IMP4,1,1},//第2套公共假日数≤90                                                             
 {0xCB15,IMP4,1,1},//第2套周休日采用的时段表号    
 {0xCB16,IMP4,1,1},//第2套周休日状态字（见附录B8）
 
 {0xCB2F,IMP4,3*MAX_YEAR_PERIODS,1}, //第2套年时区                                                    

 {0xCB3F,IMP4,3*MAX_DATE_PERIODS,1}, //第2套第1时段表                                     
 {0xCB4F,IMP4,3*MAX_DATE_PERIODS,1}, //第2套第2时段表                                    
 {0xCB5F,IMP4,3*MAX_DATE_PERIODS,1}, //第2套第3时段表                                     
 {0xCB6F,IMP4,3*MAX_DATE_PERIODS,1}, //第2套第4时段表
 {0xCB7F,IMP4,3*MAX_DATE_PERIODS,1}, //第2套第5时段表                                                                           
 {0xCB8F,IMP4,3*MAX_DATE_PERIODS,1}, //第2套第6时段表                                     
 {0xCB9F,IMP4,3*MAX_DATE_PERIODS,1}, //第2套第7时段表                                     
 {0xCBAF,IMP4,3*MAX_DATE_PERIODS,1}, //第2套第8时段表
 {0xCBBF,IMP4,3*MAX_DATE_PERIODS,1}, //第2套第9时段表
 {0xCBCF,IMP4,3*MAX_DATE_PERIODS,1}, //第2套第10时段表
 {0xCBDF,IMP4,3*MAX_DATE_PERIODS,1}, //第2套第11时段表
 {0xCBEF,IMP4,3*MAX_DATE_PERIODS,1}, //第2套第12时段表 
 
 {0xCC1F,IMP4,45,1}, //第2套第1-15个公共假日 
 //{0xCC1E,IMP1,3,1}, //周休日采用日时段表号 
 {0xCC2F,IMP4,45,1}, //第2套第16-30个公共假日 
 {0xCC3F,IMP4,45,1}, //第2套第31-45个公共假日 
 {0xCC4F,IMP4,45,1}, //第2套第46-60个公共假日 
 {0xCC5F,IMP4,45,1}, //第2套第61-75个公共假日 
 {0xCC6F,IMP4,45,1}, //第2套第76-90个公共假日 

 //校表相关参数,扩充的-----
 {ADJ_METER_PUL_VOL_CUR_SPEC,IMP0,3,1},//脉冲、电压、电流规格,3字节
 {ADJ_METER_PARA_STATUS,IMP0,1,1},//校表参数状态设置
 {ADJ_METER_A_POW,IMP0,3,12},//ABC三相功率、相位、电压、电流等

 {0xE400,IMP3,10+1,ADJ_TIME_EVENT_NUM},//前100次校时事件记录（注4）
 {0xE900,IMP3,149+1,3},//前3次总清零记录（注17）
 {0xE910,IMP3,86+1,10},//前10次A相失压记录（注1）
 {0xE920,IMP3,86+1,10},//前10次B相失压记录（注1）
 {0xE930,IMP3,86+1,10},//前10次C相失压记录（注1）
 {0xE940,IMP3,86+1,10},//前10次AB相失压记录（注1）
 {0xE950,IMP3,86+1,10},//前10次AC相失压记录（注1）
 {0xE960,IMP3,86+1,10},//前10次BC相失压记录（注1）
 {0xE970,IMP3,86+1,10},//前10次ABC相失压记录（注1）
 {0xE980,IMP3,74+1,10},//前10次A失流记录（注2）
 {0xE990,IMP3,74+1,10},//前10次B失流记录（注2）
 {0xE9A0,IMP3,74+1,10},//前10次C失流记录（注2）
 {0xE9B0,IMP3,74+1,10},//前10次AB失流记录（注2）
 {0xE9C0,IMP3,74+1,10},//前10次AC失流记录（注2）
 {0xE9D0,IMP3,74+1,10},//前10次BC失流记录（注2）

 {0xEA00,IMP3,42+1,10},//前10次A相反向记录（注3）//规约 描述有错
 {0xEA10,IMP3,42+1,10},//前10次B相反向记录（注3）//规约 描述有错
 {0xEA20,IMP3,42+1,10},//前10次C相反向记录（注3）//规约 描述有错
 
 {0xEA30,IMP3,10+1,10},//前10次全失压记录（注4）//规约 描述有错
 {0xEA40,IMP3,10+1,10},//前10次A相断相记录（注4）//规约 描述有错
 {0xEA50,IMP3,10+1,10},//前10次B相断相记录（注4）//规约 描述有错
 {0xEA60,IMP3,10+1,10},//前10次C相断相记录（注4）//规约 描述有错
 {0xEA70,IMP3,10+1,10}, //前10次电压逆相序记录（注4）//规约 描述有错
 
 {0xEA80,IMP3,13+1,10}, //前10次A相电压超合格上限记录（注5）
 {0xEA90,IMP3,13+1,10}, //前10次B相电压超合格上限记录（注5）
 {0xEAA0,IMP3,13+1,10}, //前10次C相电压超合格上限记录（注5）
 {0xEAB0,IMP3,13+1,10}, //前10次A相电压超合格下限记录（注5）
 {0xEAC0,IMP3,13+1,10}, //前10次B相电压超合格下限记录（注5）
 {0xEAD0,IMP3,13+1,10}, //前10次C相电压超合格下限记录（注5）
 {0xEAE0,IMP3,13+1,10}, //前10次超需量报警记录（注6）
 {0xEAF0,IMP3,8+1,10}, //前10次超需量跳闸记录（注7）

 {0xEB20,IMP3,10+1,10}, //前10次上下电记录（注4）
 {0xEB30,IMP3,5+1,10}, //前10次复位记录(注8)
 {0xEB40,IMP3,38+1,10}, //前10次通讯清需量记录（注9） 
 {0xEB50,IMP3,38+1,10}, //前10次手动清需量记录（注9）
 {0xEB60,IMP3,5+1,10}, //前10次前10次清过程记录（注8） 
 {0xEB70,IMP3,5+1,10}, //前10次清电压合格率记录（注8） 
 {0xEB80,IMP3,78+1,10}, //设置初始电量记录（注10）
 {0xEB90,IMP3,7+1,10}, //设置初始电量记录（注10）
 {0xEBA0,IMP3,5+1,10}, //前10次开端盖记录（注8）--->修改为开大盖，也就是开上盖！！！！！,规约描述有错，修改之
 {0xEBB0,IMP3,5+1,10}, //前10次开铅封门记录（注8）
 {0xEBC0,IMP3,10+1,10}, //前10次时钟电池欠压记录（注4）
 {0xEBD0,IMP3,10+1,10}, //前10次低功耗电池欠压记录（注4）
 {0xEBE0,IMP3,10+1,10}, //前10次校时记录（注4）
 {0xEBF0,IMP3,11+1,10}, //前10次运行时钟乱记录（注12） 
 
 {0xEC00,IMP3,11+1,10}, //前10次上电时钟乱记录（注12） 
 {0xEC20,IMP3,6+1,10}, //前10次EEPROM坏记录（注14）
 {0xEC30,IMP3,6+1,10}, //前10次校表记录(注14)
 {0xEC40,IMP3,10+1,ALL_LOSS_CUR_EVENT_NUM}, //前100次全失流记录（注4）
 /*
 {0xEC50,IMP3,10,10}, 
 {0xEC60,IMP3,10,10}, 
 {0xEC70,IMP3,10,10}, 
 {0xEC80,IMP3,10,10}, 
 {0xEC90,IMP3,10,10}, 
 {0xECA0,IMP3,10,10}, 
 {0xECB0,IMP3,10,10}, 
 {0xECC0,IMP3,10,10}, 
 {0xECD0,IMP3,10,10}, 
 */                     
 
 {0xED00,IMP3,6+1,1}, //前1次电量纠错时间及类型（注16）
 {0xED01,IMP3,36+1,12}, //前1次电量纠错前后的电能（注16）  
 {0xED10,IMP3,6+1,1}, //前2次电量纠错时间及类型（注16）
 {0xED11,IMP3,36+1,12}, //前2次电量纠错前后的电能（注16）
 {0xED20,IMP3,6+1,1}, //前3次电量纠错时间及类型（注16）
 {0xED21,IMP3,36+1,12}, //前3次电量纠错前后的电能（注16）
 
 {0xED30,IMP3,13+1,10}, //前10次A相过压记录（注5）
 {0xED40,IMP3,13+1,10}, //前10次B相过压记录（注5）
 {0xED50,IMP3,13+1,10}, //前10次C相过压记录（注5）
 {0xED60,IMP3,21+1,20}, //前20次总电量立即冻结记录（注18）
 //{0xED70,IMP3,21,10}, //前10-20次总电量立即冻结记录（注18）
 {0xED90,IMP3,5+1,10}, //前10次开端盖结束记录（注8）--->修改为开大盖，也就是开上盖！！！！！,规约描述有错，修改之
 {0xEDA0,IMP3,5+1,10}, //前10次开铅封门结束记录（注8）
 {0xEDB0,IMP3,5+1,10}, //前1次开大盖起始记录（注8）---->修改为开端盖,规约描述有错，修改之
 {0xEDC0,IMP3,5+1,10}, //前10次开大盖结束记录（注8）---->修改为开端盖,规约描述有错，修改之
 {0xEDD0,IMP3,5+1,10}, //前10次开计量柜门起始记录（注8）
 {0xEDE0,IMP3,5+1,10}, //前10次开计量柜门结束记录（注8）
 
 //{0xEF00,IMP1,122,1},//当前电量数据及瞬时量
 {0xEF01,IMP1,127,1},//冻结电量数据及冻结瞬时量
 //{0xEF02,IMP1,144,1},//当前四象限无功电量
 {0xEF03,IMP1,149,1},//冻结四象限无功电量
 /*
 {0xED80,IMP3,175,12}, //前12次指定冻结电量数据及冻结瞬时量（注19）(河南) 
 {0xED90,IMP3,175,12}, //前12次瞬时冻结电量数据及冻结瞬时量（注19）(河南)
 {0xEDA0,IMP3,175,12}, //前12次约定冻结电量数据及冻结瞬时量（注19）(河南)
 {0xEDB0,IMP3,175,13}, //前13次结算周期内按天冻结电量（注19）(河南)
 {0xEDC0,IMP3,13,13}, //前14-26次结算周期内按天冻结电量（注19）(河南)
 */

//失压累计
{ OCCUR_TOTAL_LOSS_VOL,IMP7,LOSS_VOL_DATA_LEN,8},
{ TOTAL_TOTAL_LOSS_VOL,IMP7,LOSS_VOL_DATA_LEN,8},
//全失压
{ OCCUR_ALL_LOSS_VOL,IMP7,ALL_LOSS_VOL_DATA_LEN,1},
{ TOTAL_ALL_LOSS_VOL,IMP7,ALL_LOSS_VOL_DATA_LEN,1},
//失流累计                               
{ OCCUR_TOTAL_LOSS_CUR,IMP7,LOSS_CUR_DATA_LEN,7}, 
{ TOTAL_TOTAL_LOSS_CUR,IMP7,LOSS_CUR_DATA_LEN,7},
//全失流
{ OCCUR_ABC_LOSS_CUR,IMP7,ALL_LOSS_CUR_DATA_LEN,1},
{ TOTAL_ABC_LOSS_CUR,IMP7,ALL_LOSS_CUR_DATA_LEN,1}, 

//反向
{ OCCUR_TOTAL_NEG_CUR,IMP7,NEG_CUR_DATA_LEN,4},  
{ TOTAL_TOTAL_NEG_CUR,IMP7 ,NEG_CUR_DATA_LEN,4},  
//断相                              
{ OCCUR_TOTAL_BREAK_VOL,IMP7, BREAK_VOL_DATA_LEN,4},
{ TOTAL_TOTAL_BREAK_VOL,IMP7, BREAK_VOL_DATA_LEN,4},

 STORA_EXTEND

 {CHK_IMP0,IMP0,1,1}, //IMP每类数据的测试字 
 {CHK_IMP1,IMP1,1,1},
 {CHK_IMP2,IMP2,1,1},
 {CHK_IMP3,IMP3,1,1},
 {CHK_IMP4,IMP4,1,1},
 {CHK_IMP5,IMP5,1,1},
 {CHK_IMP6,IMP6,1,1},
 {CHK_IMP7,IMP7,1,1}
};

//检查超级密码的跳线是否跳上了，跳上返回1，否则返回0
INT8U Check_SuperPSW_Jump()
{
  return 1;  
}

//检查Imp_Flag标志的合法性，正确返回1，否则返回0
INT8U Check_Imp_Flag(INT8U Imp_Flag)
{
  if(Imp_Flag<=IMP7)
    return 1;
  else
    return 0;
}
//获取某个存储的数据项数据重要标志
INT8U Get_Storage_Data_ImpFlag(STORA_DI DI)
{
  INT16U i;
  
  for(i=0;i<S_NUM(Data_Para_Storage);i++)
  {
    if(Data_Para_Storage[i].DI==DI ||\
      (Data_Para_Storage[i].Num>1 && Data_Para_Storage[i].DI<DI && DI<Data_Para_Storage[i].DI+Data_Para_Storage[i].Num))
    {
      return Data_Para_Storage[i].Imp_Flag;
    }
  }
  ASSERT(A_WARNING,0);
  return NULL_IMP;
}

//获取某个存储的数据项的存储偏移，该偏移表示该数据项在同Imp_Flag数据中的存储品偏移
//也表示在逻辑存储器中的整体偏移
INT32U Get_Storage_Data_Off(STORA_DI DI)
{
  INT16U i;
  INT32U Offset=0;
  INT8U Imp_Flag,Re;
  
  Imp_Flag=Get_Storage_Data_ImpFlag(DI);
  Re=Check_Imp_Flag(Imp_Flag);
  ASSERT(A_WARNING,0!=Re);
  
  for(i=0;i<S_NUM(Data_Para_Storage);i++)
  {
    if(Data_Para_Storage[i].DI==DI)
    {
      return Offset;
    }
    else if(Data_Para_Storage[i].Num>1 && Data_Para_Storage[i].DI<DI && DI<Data_Para_Storage[i].DI+Data_Para_Storage[i].Num)
    {
      Offset+=(Data_Para_Storage[i].Len+ROM_CS_BYTES)*(DI-Data_Para_Storage[i].DI);
      return Offset;     
    }
    else
    {
      if(Data_Para_Storage[i].Imp_Flag==Imp_Flag)//增加偏移
      {
        //if(Data_Para_Storage[i].Num==1)
         // Offset+=Data_Para_Storage[i].Len+ROM_CS_BYTES; 
        //else
          Offset+=(Data_Para_Storage[i].Len+ROM_CS_BYTES)*Data_Para_Storage[i].Num;
    
      }
    }
  }

  DEBUG_PRINT(HUCK,DEBUG_0,"Get_Storage_Data_Off Error,Data 0x%x Not Found\r\n",DI);
  return NULL_4BYTES; 
}

//获取某个存储的数据项的长度,DI只能是Data_Para_Storage中定义的数据
INT16U Get_Storage_Data_Len(STORA_DI DI)
{
  INT16U i;
  
  for(i=0;i<S_NUM(Data_Para_Storage);i++)
  {
    if(Data_Para_Storage[i].DI==DI ||\
       (Data_Para_Storage[i].Num>1 &&\
        Data_Para_Storage[i].DI<DI &&\
        Data_Para_Storage[i].DI+Data_Para_Storage[i].Num>DI))
    {
      return Data_Para_Storage[i].Len;
    }
  }
  ASSERT(A_WARNING,0);
  return 0;  
}

//获取某个Imp_Flag重要等级数据的整个数据长度Get_Imp_Data_Size的返回值应该<=Get_Imp_Mem_Size
INT32U Get_Imp_Data_Size(INT8U Imp_Flag)
{
  INT8U Re;
  INT16U i;
  INT32U Size=0;
  
  Re=Check_Imp_Flag(Imp_Flag);
  ASSERT(A_WARNING,Re!=0);
  for(i=0;i<S_NUM(Data_Para_Storage);i++)
  {
    if(Data_Para_Storage[i].Imp_Flag==Imp_Flag)
    {
      Size+=((INT32U)Data_Para_Storage[i].Len+ROM_CS_BYTES)*Data_Para_Storage[i].Num;
    }
  }
  ASSERT(A_WARNING,Size!=0);
  return Size;
}

//获得分配给某个Imp_Flag等级的数据的存储空间大小
INT32U Get_Imp_Mem_Size(INT8U Imp_Flag)
{
  INT16U i,j;
  INT32U Size=0;//定义一个不存在的size
  INT8U Re;
  
  Re=Check_Imp_Flag(Imp_Flag);
  ASSERT(A_WARNING,Re!=0);
  
  for(i=0;i<S_NUM(Data_Mem_Map);i++)
  {
    if(Data_Mem_Map[i].Imp_Flag==Imp_Flag)//先找到数据内存映射表中的对应项
    {
      Size=0xFFFFFFFF;//先置一个极大值
      for(j=0;j<IMP_LOG_MEM;j++)
      { 
        if(Data_Mem_Map[i].LOG_Mem[j]!=NULL_LOG_MEM)
        {
         //找出最小的一个Log_Mem就是分配个数据的存储空间大小
         if(Get_LOG_Mem_Size(Data_Mem_Map[i].LOG_Mem[j])<Size)
           Size=Get_LOG_Mem_Size(Data_Mem_Map[i].LOG_Mem[j]);
        }
      }
      break;
    }
  }
  //ASSERT(A_WARNING,Size!=0 && Size!=0xFFFFFFFF);
  if(0==Size || 0xFFFFFFFF==Size)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Imp%d Data Mem_map error",Imp_Flag);
  } 
  return Size; 
}

//从存储器中读取某个重要等级的数据
//Imp_Flag，表示数据的重要等级标志
//Offset,表示数据在存储器中的偏移
//pDst，表示数据读取后的目标指针
//RD_Len，表示需要读取的数据的长度
//pDst_Start和DstLeny一起对pDst和RD_Len的范围进行约束
//返回NO_ERR表示成功
INT8U Read_Imp_Data(INT8U Imp_Flag,INT32U Offset,void *pDst,INT16U RD_Len,void *pDst_Start,INT16U DstLen)
{
  INT16U i,j;
  INT32U Sum;
  INT8U Re;

  Re=Check_Imp_Flag(Imp_Flag);
  ASSERT(A_WARNING,Re!=0);
  ASSERT(A_WARNING,(INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst+RD_Len<=(INT8U *)pDst_Start+DstLen);
  
  for(i=0;i<S_NUM(Data_Mem_Map);i++)
  {
    if(Data_Mem_Map[i].Imp_Flag==Imp_Flag)//找到数据内存映射表中的对应项
    {
      for(j=0;j<IMP_LOG_MEM;j++)//读取存储的逻辑存储器
      {
        if(j!=0)//前一次读取没有成功 ，则延时一下，防止因为总线干扰出问题
          OS_TimeDly_Ms(10);
        
        if(Data_Mem_Map[i].LOG_Mem[j]!=NULL_LOG_MEM)//找到一个存储该数据的逻辑存储器
        {
          //读该逻辑存储器中的数据
          Re=Read_LOG_Mem(Data_Mem_Map[i].LOG_Mem[j],Offset,pDst,RD_Len,pDst_Start,DstLen);
          if(ASSERT(A_WARNING,NO_ERR==Re)) //确保返回正确
            continue;
          //读校验和
          Re=Read_LOG_Mem(Data_Mem_Map[i].LOG_Mem[j],Offset+RD_Len,&Sum,ROM_CS_BYTES,&Sum,sizeof(Sum));
          if(ASSERT(A_WARNING,NO_ERR==Re))
            continue;
          //检查校验和是否正确
          if(Check_Sum(pDst,RD_Len,&Sum,ROM_CS_BYTES))//校验和正确则直接返回NO_ERR，否则继续读取下个LOG_MEM
            return NO_ERR;
          else
            continue; 
        }
      }
      ASSERT(A_WARNING,0);
      return RD_IMP_DATA_ERR;
    }
  }
  ASSERT(A_WARNING,0);
  return RD_IMP_DATA_ERR;
}

//从存储器中读取某个重要等级的数据
//Imp_Flag，表示数据的重要等级标志
//Offset,表示数据在存储器中的偏移
//pDst，表示数据读取后的目标指针
//RD_Len，表示需要读取的数据的长度
//pDst_Start和DstLeny一起对pDst和RD_Len的范围进行约束
//返回NO_ERR表示成功
INT8U PD_Read_Imp_Data(INT8U Imp_Flag,INT32U Offset,void *pDst,INT16U RD_Len,void *pDst_Start,INT16U DstLen)
{
  INT16U i,j;
  INT32U Sum;
  INT8U Re;

  Power_Down_Check();
  
  Re=Check_Imp_Flag(Imp_Flag);
  ASSERT(A_WARNING,Re!=0);
  ASSERT(A_WARNING,(INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst+RD_Len<=(INT8U *)pDst_Start+DstLen);
  
  for(i=0;i<S_NUM(Data_Mem_Map);i++)
  {
    if(Data_Mem_Map[i].Imp_Flag==Imp_Flag)//找到数据内存映射表中的对应项
    {
      for(j=0;j<IMP_LOG_MEM;j++)//读取存储的逻辑存储器
      {
        if(Data_Mem_Map[i].LOG_Mem[j]!=NULL_LOG_MEM)//找到一个存储该数据的逻辑存储器
        {
          Re=Read_LOG_Mem(Data_Mem_Map[i].LOG_Mem[j],Offset,(void *)Pub_Buf,RD_Len+ROM_CS_BYTES,(void *)Pub_Buf,sizeof(Pub_Buf));
          if(ASSERT(A_WARNING,NO_ERR==Re)) //确保返回正确
            continue;
          Sum=0;
          mem_cpy(&Sum,(INT8U *)Pub_Buf+RD_Len,ROM_CS_BYTES,&Sum,sizeof(Sum));
          if(Check_Sum(pDst,RD_Len,&Sum,ROM_CS_BYTES))//校验和正确则直接返回NO_ERR，否则继续读取下个LOG_MEM
          {
            mem_cpy(pDst,(void *)Pub_Buf,RD_Len,pDst_Start,DstLen);
            return NO_ERR;
          }
          else
            continue; 
        }
      }
      ASSERT(A_WARNING,0);
      return RD_IMP_DATA_ERR;
    }
  }
  ASSERT(A_WARNING,0);
  return RD_IMP_DATA_ERR;
}
//将某个重要标志的数据写入到存储器中
//Imp_Flag，表示数据的重要等级标志
//Offset,表示数据在存储器中的偏移
//pSrc，表示数据的起始指针
//SrcLen，表示数据的长度
//返回NO_ERR表示写成功
//返回WR_IMP_DATA_ERR表示写数据失败
INT8U Write_Imp_Data(INT8U Imp_Flag,INT32U Offset,void *pSrc,INT16U SrcLen)
{
  INT8U i,j;
  INT8U Re;
  INT32U Sum;
  INT8U Err_Flag=0;
  
  Power_Down_Check();
  
  Re=Check_Imp_Flag(Imp_Flag);
  ASSERT(A_WARNING,Re!=0);
  Set_Sum(pSrc,SrcLen,&Sum,ROM_CS_BYTES,&Sum,sizeof(Sum));//生成校验和
  
  for(i=0;i<S_NUM(Data_Mem_Map);i++)
  {
    if(Data_Mem_Map[i].Imp_Flag==Imp_Flag)
    {
      Err_Flag=0;//只要成功了一次就 Err_Flag|=1;
      for(j=0;j<IMP_LOG_MEM;j++)
      { 
        if(j!=0 && Err_Flag==0)//前一次没有写成功，则延时一下再写
          OS_TimeDly_Ms(10);
        
        if(Data_Mem_Map[i].LOG_Mem[j]!=NULL_LOG_MEM)
        {
          Re=Write_LOG_Mem(Data_Mem_Map[i].LOG_Mem[j],Offset,pSrc,SrcLen);//写数据
          ASSERT(A_WARNING,NO_ERR==Re);
          if(NO_ERR==Re)
          {
            Re=Write_LOG_Mem(Data_Mem_Map[i].LOG_Mem[j],Offset+SrcLen,&Sum,ROM_CS_BYTES);//写校验和
            ASSERT(A_WARNING,NO_ERR==Re);
            if(NO_ERR==Re)
              Err_Flag|=1;
          }
        }
      }
      if(Err_Flag!=0)//至少写成功了一次
        return NO_ERR;
      else 
        return WR_IMP_DATA_ERR;
    }
  }
  ASSERT(A_WARNING,0);
  return WR_IMP_DATA_ERR;
}

//将某个重要标志的数据写入到存储器中
//Imp_Flag，表示数据的重要等级标志
//Offset,表示数据在存储器中的偏移
//pSrc，表示数据的起始指针
//SrcLen，表示数据的长度
//返回NO_ERR表示写成功
//返回WR_IMP_DATA_ERR表示写数据失败
INT8U PD_Write_Imp_Data(INT8U Imp_Flag,INT32U Offset,void *pSrc,INT32U SrcLen)
{
  INT8U i,j;
  INT8U Re;
  INT32U Sum;
  INT8U Err_Flag=0;
  
  Re=Check_Imp_Flag(Imp_Flag);
  ASSERT(A_WARNING,Re!=0);
  Set_Sum(pSrc,SrcLen,&Sum,ROM_CS_BYTES,&Sum,sizeof(Sum));//生成校验和
  mem_cpy((INT8U *)Pub_Buf,pSrc,SrcLen,(void *)Pub_Buf,sizeof(Pub_Buf));
  mem_cpy((INT8U *)Pub_Buf+SrcLen,&Sum,ROM_CS_BYTES,(void *)Pub_Buf,sizeof(Pub_Buf));
  
  for(i=0;i<S_NUM(Data_Mem_Map);i++)
  {
    if(Data_Mem_Map[i].Imp_Flag==Imp_Flag)
    {
      Err_Flag=0;//只要成功了一次就 Err_Flag|=1;
      for(j=0;j<IMP_LOG_MEM;j++)
      { 
        if(Data_Mem_Map[i].LOG_Mem[j]!=NULL_LOG_MEM)
        {
          Re=Write_LOG_Mem(Data_Mem_Map[i].LOG_Mem[j],Offset,(void *)Pub_Buf,SrcLen+ROM_CS_BYTES);//写数据
          ASSERT(A_WARNING,NO_ERR==Re);
          if(NO_ERR==Re)
              Err_Flag|=1;
        }
      }
      if(Err_Flag!=0)//至少写成功了一次
        return NO_ERR;
      else 
        return WR_IMP_DATA_ERR;
    }
  }
  ASSERT(A_WARNING,0);
  return WR_IMP_DATA_ERR;
}

//读取存储器中的某项数据的固定长度，DI的值可参考Data_Para_Storage的定义
//DI,需要读取的数据项标识
//Offset,读取DI数据的起始偏移
//Len,需要读取的数据长度
//pDst表示读取参数后放到该缓冲区
//pDst_Start和DstLen一起界定目标缓冲区的范围
//*pErr表示读取失败时的原因,NO_ERR表示读取成功,其他错误原因参见Pub_Err.h
//返回值：读取的数据长度,读取失败时返回0
INT16U Read_Storage_Data_Fix_Len(STORA_DI DI,INT16U Offset,INT16U Len,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr)
{
  INT32U Off;
  INT8U Imp_Flag;

  DEBUG_PRINT(HUCK,DEBUG_0,"Read Storage Data:0x%lx,Off=%d,Len=%d",DI,Offset,Len);
  
  if(ASSERT(A_WARNING,(INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst+Len<=(INT8U *)pDst_Start+DstLen))
    return 0;
  
  Imp_Flag=Get_Storage_Data_ImpFlag(DI);//获取该数据的重要标志 
 
  Off=Get_Storage_Data_Off(DI);//获取数据偏移
  if(ASSERT(A_WARNING,NULL_4BYTES!=Off))
  {
    *pErr=RD_STORAGE_DATA_DI_ERR;
    return 0;
  }
  
  Off+=Offset;//+内部偏移

  OS_Sem_Pend(PUB_RW_ROM_ID);//申请写ROM的信号量ID
  //if(Check_Power_Status()==POWER_ON_STATUS)
    *pErr=Read_Imp_Data(Imp_Flag,Off,pDst,Len,pDst_Start,DstLen);
  //else
    //*pErr=PD_Read_Imp_Data(Imp_Flag,Off,pDst,Len,pDst_Start,DstLen);
  OS_Sem_Post(PUB_RW_ROM_ID);//释放写ROM的信号量ID
  if(NO_ERR==*pErr)
    return Len;
  else//读存储失败则读取默认参数
  { 
    *pErr=RD_FIX_DATA_ERR;
    return 0;
  }
  //return 0;  
}

//写存储器中的某项数据的固定长度，DI的值可参考Data_Para_Storage的定义
//DI,需要写的数据项标识
//Offset,写DI数据的起始偏移
//SrcLen,需要写的数据长度
//pSrc表示写的源数据缓冲区
//SrcLen表示源数据的长度
//返回NO_ERR表示成功，其他表示失败
INT8U Write_Storage_Data_Fix_Len(STORA_DI DI,INT16U Offset,void *pSrc,INT16U SrcLen)
{
  INT32U Off;
  INT8U Imp_Flag,Re;

  DEBUG_PRINT(HUCK,DEBUG_0,"Write Storage Data:0x%lx,Off=%d,Len=%d",DI,Offset,SrcLen);
  
  Imp_Flag=Get_Storage_Data_ImpFlag(DI);//获取该数据的重要标志 
  
  Off=Get_Storage_Data_Off(DI);//获取数据偏移
  if(ASSERT(A_WARNING,NULL_4BYTES!=Off))
  {
    return WR_STORAGE_DATA_DI_ERR;
  }
  
  Off+=Offset;//+内部偏移
  OS_Sem_Pend(PUB_RW_ROM_ID); 
  //if(Check_Power_Status()==POWER_ON_STATUS)
    Re=Write_Imp_Data(Imp_Flag,Off,pSrc,SrcLen);
  //else
    //Re=PD_Write_Imp_Data(Imp_Flag,Off,pSrc,SrcLen);    
  OS_Sem_Post(PUB_RW_ROM_ID);

  //OS_TimeDly_Ms(20);//延时20ms
  
  if(NO_ERR==Re)
    return NO_ERR;
  else//读存储失败则读取默认参数
  { 
    return WR_FIX_DATA_ERR;
  }
 
}


//读取存储器中的某项数据，DI的值可参考Data_Para_Storage的定义
//DI,需要读取的数据项标识
//pPara，在读取某些参数或数据时需要的参数，例如HIS_ENERGY,HIS_DEMAND,HIS_DEMAND_TIME时需要加入时间参数
//时间参数的格式定义如下pPara[0]-pPara[4]分别是分、时、日、月、年,其他情况下该参数无意义
//pDst表示读取参数后放到该缓冲区
//pDst_Start和DstLen一起界定目标缓冲区的范围
//*pErr表示读取失败时的原因,NO_ERR表示读取成功,其他错误原因参见Pub_Err.h
//返回值：读取的数据长度,读取失败时返回0
INT16U Read_Storage_Data(STORA_DI DI,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr)
{
  INT16U Len,Len1;
  
  //Clear_CPU_Dog();    //清CPU内部看门狗
  //Clear_Ext_Dog();    //清CPU外部看门狗
  
  //Cur_Task_ID=Get_Cur_Task_ID();
  //DEBUG_PRINT(HUCK,DEBUG_0,"Read Storage Data:0x%lx",DI,Tcb[Cur_Task_ID].Name);
  Len=Get_Storage_Data_Len(DI);
  if(Len>0)
  {
    //OS_TimeDly_Ms(20);//延时给其他任务，防止某任务连续读多个参数占用CPU
    Len1=Read_Storage_Data_Fix_Len(DI,0,Len,pDst,pDst_Start,DstLen,pErr);
    if(ASSERT(A_WARNING,Len==Len1 && NO_ERR==*pErr))
    {
      Len1=Read_Def_Para(DI,pDst,Len,pDst_Start,DstLen);
      DEBUG_PRINT(HUCK,DEBUG_0,"Read_Def_Para,Len = %u",Len1);
      if(ASSERT(A_WARNING,Len1==Len))
      {
        *pErr=RD_STORAGE_DATA_DI_ERR;
        return 0; 
      }
      else
      {
        *pErr=NO_ERR;
        return Len;        
      }
    }
    else
    {
      *pErr=NO_ERR;
      return Len;
    }
  }
  else
  {
    ASSERT(A_WARNING,0);
    *pErr=RD_STORAGE_DATA_DI_ERR;
    return 0; 
  }
}

//向存储器中写某项数据，DI的值可参考Data_Para_Storage的定义
//DI,需要写的数据项标识
//pPara，在写某些参数或数据时需要的参数，例如HIS_ENERGY,HIS_DEMAND,HIS_DEMAND_TIME时需要加入时间参数
//时间参数的格式定义如下pPara[0]-pPara[4]分别是分、时、日、月、年,其他情况下该参数无意义
//pSrc表示写数据的源缓冲区
//返回值：NO_ERR表示成功，其他表示失败
INT8U Write_Storage_Data(STORA_DI DI,void *pSrc,INT16U SrcLen)
{
  volatile INT16U Len;
  INT8U Re;

  //Cur_Task_ID=Get_Cur_Task_ID();
  //DEBUG_PRINT(HUCK,DEBUG_0,"Write Storage Data:0x%lx",DI,Tcb[Cur_Task_ID].Name);
  //获取并比较数据长度
  Len=Get_Storage_Data_Len(DI);
  if(ASSERT(A_WARNING,Len==SrcLen))
  {
    DEBUG_PRINT(HUCK, DEBUG_0, "WR_Data_Err DI=0x%lx, Len=%d, WR_Len=%d", DI, Len, SrcLen);
    return WR_STORAGE_DATA_LEN_ERR;
  }
  //写Imp数据
  //OS_Sem_Pend(PUB_RW_ROM_ID);//申请写ROM的信号量ID
  Re=Write_Storage_Data_Fix_Len(DI,0,pSrc,SrcLen);
  //OS_Sem_Post(PUB_RW_ROM_ID);//释放该信号量
  if(ASSERT(A_WARNING,NO_ERR==Re))
    return Re;
  return NO_ERR;  
}

//检查存储映射是否合理,主要是检查
void Check_Storage_Map()
{
  INT8U i,j,Count;
  INT32U Size;
  
  //Re=Check_Mem_Map();
  //ASSERT(A_WARNING,NO_ERR==Re);//检查内存区间分配的映射是否正确
  //Get_Imp_Data_Size(IMP0);
  for(i=0;i<S_NUM(Data_Mem_Map);i++)
  {
    Count=0;
    Size=Get_Imp_Data_Size(Data_Mem_Map[i].Imp_Flag);
    for(j=0;j<IMP_LOG_MEM;j++)
    {
      if(Data_Mem_Map[i].LOG_Mem[j]!=NULL_LOG_MEM)
      {
         if(Get_LOG_Mem_Size(Data_Mem_Map[i].LOG_Mem[j])>=Size)
           Count++;
         else
           ASSERT(A_WARNING,0);

      }
    }
    ASSERT(A_WARNING,(Data_Mem_Map[i].Imp_Flag==IMP5 && Count>=1)||\
                     (Data_Mem_Map[i].Imp_Flag!=IMP5 && Count>=2));//每种数据起码有2个存储空间,即至少一个备份,除负荷曲线相关数据
  }
  
}

//检查参数的存储等
INT8U Check_Imp_Data_Storage()
{
  INT8U i,j,Count,Test,Re,Re1,Err;
  INT32U DataSize,MemSize;
  
  Re = NO_ERR;
  Clr_Ext_Inter_Dog();
  //先检查分配给每个Imp_Flag类数据的逻辑存储器的大小和个数是否合法
  DEBUG_PRINT(HUCK,DEBUG_0,"----PHY_IMP_LOG_MEM Map Check Start!----");
  for(i=0;i<S_NUM(Data_Mem_Map);i++)
  {
    Count=0;
    DataSize=Get_Imp_Data_Size(Data_Mem_Map[i].Imp_Flag);
    DEBUG_PRINT(HUCK,DEBUG_0,"Imp%d Data size=%ld",Data_Mem_Map[i].Imp_Flag,DataSize); 

    for(j=0;j<IMP_LOG_MEM;j++)
    {
      if(Data_Mem_Map[i].LOG_Mem[j]!=NULL_LOG_MEM)
      {         
        MemSize = Get_LOG_Mem_Size(Data_Mem_Map[i].LOG_Mem[j]);
        DEBUG_PRINT(HUCK,DEBUG_0,"----Log_Mem%d size=%ld",Data_Mem_Map[i].LOG_Mem[j],MemSize);        
        if(MemSize>=DataSize)
         Count++;
        else
        {
         DEBUG_PRINT(HUCK,DEBUG_0,"!!!!Log_Mem%d have not enough space!",Data_Mem_Map[i].Imp_Flag,Data_Mem_Map[i].LOG_Mem[j]); 
         Re |= IMP_DATA_STORAGE_ERR;
        }
      }
    }

    if((Count==0))
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Imp%d Data do not have enough LOG Mems",Data_Mem_Map[i].Imp_Flag);
      Re |= IMP_DATA_STORAGE_ERR;
    }
  }
 
  DataSize=0;
  MemSize=0;
  for(j=0;j<S_NUM(Data_Mem_Map);j++)
  {
    i=Data_Mem_Map[j].Imp_Flag;
    DataSize=Get_Imp_Data_Size(i);
    MemSize=Get_Imp_Mem_Size(i);
    DEBUG_PRINT(HUCK,DEBUG_0,"IMP%d Data_size=%ld,Mem_Size=%ld",i,DataSize,MemSize);
    if(DataSize>=MemSize)
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"IMP%d Data Mem not enough space!",i);
      Re |= IMP_DATA_STORAGE_ERR;
    }
  }
  
  //对每个IMP类数据进行一次读和写操作，测试
  for(i=0;i<S_NUM(Data_Mem_Map);i++)
  {
    Test=0x5A;
    Re1=Write_Storage_Data(CHK_IMP0+i,&Test,1);
    if(NO_ERR==Re1)
    {
      Test=0;
      Re1=Read_Storage_Data(CHK_IMP0+i,&Test,&Test,sizeof(Test),&Err);
      if(0!=Re1 && NO_ERR==Err && 0x5A==Test)
        continue;
    }
    DEBUG_PRINT(HUCK,DEBUG_0,"IMP%d Data write && read test error",i);
    Re |= IMP_DATA_STORAGE_ERR;
  }
  
  DEBUG_PRINT(HUCK,DEBUG_0,"----PHY_IMP_LOG_MEM Map Check End!----");
  return Re;
}

//打印调试信息，将数据的存储地址和长度打印出来
void Debug_Print_Data_Storage()
{
  INT8U Imp_Flag;
  INT16U i,j;
  INT8U Num;
  INT16U Addr,Len;
  
  DEBUG_PRINT(HUCK,DEBUG_0,"PRINT Data_Para_Storage Info-------------");
  for(Imp_Flag=IMP0;Imp_Flag<IMP5;Imp_Flag++)
  {
    for(i=0;i<S_NUM(Data_Para_Storage);i++)
    {
      if(Data_Para_Storage[i].Imp_Flag==Imp_Flag)
      {
        if(Data_Para_Storage[i].Num==0)
          Num=1;//Data_Para_Storage[i].Num+1;
        else
          Num=Data_Para_Storage[i].Num;
        
        for(j=0;j<Num;j++)
        {
          Addr=Get_Storage_Data_Off(Data_Para_Storage[i].DI+j);
          Len=Data_Para_Storage[i].Len;
          DEBUG_PRINT(HUCK,DEBUG_0,"DI:0x%x,Addr:%d,Len:%d",\
                                   Data_Para_Storage[i].DI+j,\
                                   Addr,\
                                   Len);
        }
      } 
    }
  }
}

//检查整个存储系统是否正确
void Check_Sys_Storage()
{
  INT8U Re;
  
  if(Check_Meter_Factory_Status())
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"------System Storage_SelfCheck Start!-----");
    //物理存储器读写检查
    //检查逻辑与物理存储器之间的映射
    //检查IMP类数据的存储映射
    //检查Protocol_Data的存储映射
    Re = NO_ERR;
    
    Re |= Check_PHY_Mem();
    Re |= Check_LOG_Mem_Map();
    Re |= Check_Imp_Data_Storage();
    Re |= Check_Protocol_Data_Storage();
    if(Re == NO_ERR)
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"------System Storage_SelfCheck Succeed!-----");
    }
    else
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"------System Storage_SelfCheck failed!-----");
    }      
  }
}

#undef DATA_PARA_C

