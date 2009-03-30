#ifndef MEM_CFG_H
#define MEM_CFG_H

//物理存储器定义
#define PHY_MEM0 0 //512 EEROM
#define PHY_MEM1 1 //32K EEROM
#define PHY_MEM2 2 //32K EEROM
#define PHY_MEM3 3 //32K EEROM
#define PHY_MEM4 4 //8K FRAM
#define PHY_MEM5 5 //2M FLASH

//每个存储器预留10个字节用于测试
#define MEM_TEST_BYTES 10L

#define PHY_MEM0_SIZE (512L-MEM_TEST_BYTES)
#define PHY_MEM1_SIZE (32*1024L-MEM_TEST_BYTES)
#define PHY_MEM2_SIZE (32*1024L-MEM_TEST_BYTES)
#define PHY_MEM3_SIZE (32*1024L-MEM_TEST_BYTES)
#define PHY_MEM4_SIZE (8*1024L-MEM_TEST_BYTES)
#define PHY_MEM5_SIZE (2000*1024L-MEM_TEST_BYTES)

//逻辑存储器定义，每个逻辑存储器都在一个物理存储器的一片连续空间上
#define NULL_LOG_MEM 0xFF//定义一个不存在的存储器 

//在PHY_MEM0中
#define LOG_MEM0 0//用于存储超级密码下设置的参数，包括校表参数和部分模式字,IMP0类数据

//在PHY_MEM1中
#define LOG_MEM1 1//用于存储超级密码下设置的参数，包括校表参数和部分模式字的一个备份,IMP0类数据
#define LOG_MEM2 2//用于存储当前重要电量数据和参数等,IMP1类数据
#define LOG_MEM3 3//用于存储历史电量需量数据等,IMP2类数据

//在PHY_MEM2中
#define LOG_MEM4 4//用于存储超级密码下设置的参数，包括校表参数和部分模式字的一个备份,IMP0类数据
#define LOG_MEM5 5//用于存储当前重要电量数据和参数等的一个备份,IMP1类数据
#define LOG_MEM6 6//用于存储当前重要电量数据和参数等的一个备份,IMP1类数据

//在PHY_MEM3中
#define LOG_MEM7 7//用于存储事件记录


//在PHY_MEM4中
#define LOG_MEM8 8//用于存储事件记录的一个备份
#define LOG_MEM9 9//用于存储掉电存储的数据,IMP4类数据

//在PHY_MEM5中
#define LOG_MEM10 10//用于存储超级密码下设置的参数，包括校表参数和部分模式字的一个备份
#define LOG_MEM11 11//用于存储历史电量需量数据等的一个备份,IMP2类数据
#define LOG_MEM12 12//用于存储历史电量需量数据等的一个备份,IMP2类数据
#define LOG_MEM13 13//用于存储超级密码下设置的参数，包括校表参数和部分模式字的一个备份

//用于存储重要参数
#define LOG_MEM14 14//在PHY_MEM1中
#define LOG_MEM15 15//在PHY_MEM2中
#define LOG_MEM16 16//在PHY_MEM5中
#define LOG_MEM17 17//在PHY_MEM5中
#define LOG_MEM18 18//在PHY_MEM5中

#define LOG_MEM19 19//用于存储符合曲线

#define LOG_MEM20 20//用于存储重要事件记录
#define LOG_MEM21 21//用于存储重要事件记录的备份

/************************以下是逻辑内存的配置信息**********************/
//在PHY_MEM0中
#define LOG_MEM0_START 0
#define LOG_MEM0_SIZE (PHY_MEM0_SIZE)               //超级密码下设置的参数,存储IMP0类数据


//在PHY_MEM1中
#define LOG_MEM1_START 0
#define LOG_MEM1_SIZE (LOG_MEM0_SIZE)              //超级密码下设置的参数的一个备份,存储IMP0类数据 

#define LOG_MEM2_START (LOG_MEM1_START+LOG_MEM1_SIZE)    
#define LOG_MEM2_SIZE 3000 //电量、需量参数等重要数据,存储IMP1类数据

#define LOG_MEM3_START (LOG_MEM2_START+LOG_MEM2_SIZE)
#define LOG_MEM3_SIZE 20000L //用于存储历史电量需量数据等

#define LOG_MEM14_START (LOG_MEM3_START+LOG_MEM3_SIZE)
#define LOG_MEM14_SIZE 8000//用于存储重要参数IMP4类数据

//在PHY_MEM2中
#define LOG_MEM4_START 0
#define LOG_MEM4_SIZE (LOG_MEM0_SIZE)              //超级密码下设置的参数的一个备份,存储IMP0类数据 

#define LOG_MEM5_START (LOG_MEM4_START+LOG_MEM4_SIZE)
#define LOG_MEM5_SIZE LOG_MEM2_SIZE //电量、需量参数等重要数据的备份,存储IMP1类数据

#define LOG_MEM6_START (LOG_MEM5_START+LOG_MEM5_SIZE)
#define LOG_MEM6_SIZE LOG_MEM2_SIZE //电量、需量参数等重要数据的备份,存储IMP1类数据

#define LOG_MEM15_START (LOG_MEM6_START+LOG_MEM6_SIZE)
#define LOG_MEM15_SIZE (PHY_MEM2_SIZE-LOG_MEM15_START)//用于存储重要参数,IMP4类数据

//在PHY_MEM3中
#define LOG_MEM7_START 0
#define LOG_MEM7_SIZE (PHY_MEM3_SIZE-8200)      //用于存储事件记录

#define LOG_MEM20_START (LOG_MEM7_START+LOG_MEM7_SIZE)//用于存储重要事件记录,IMP7类数据
#define LOG_MEM20_SIZE 4100

#define LOG_MEM21_START (LOG_MEM20_START+LOG_MEM20_SIZE)//用于存储重要事件记录的备份,IMP7类数据
#define LOG_MEM21_SIZE 4100

//在PHY_MEM4中
//#define LOG_MEM8_START 0
//#define LOG_MEM8_SIZE PHY_MEM4_SIZE/2

#define LOG_MEM9_START 0//(LOG_MEM8_START+LOG_MEM8_SIZE)
#define LOG_MEM9_SIZE PHY_MEM4_SIZE //存储IMP4类数据,即掉电存储类数据

//在PHY_MEM5中
#define LOG_MEM10_START 0
#define LOG_MEM10_SIZE LOG_MEM0_SIZE //用于校表参数等IMP0类数据

#define LOG_MEM11_START (LOG_MEM10_START+LOG_MEM10_SIZE)
#define LOG_MEM11_SIZE LOG_MEM3_SIZE  //历史电量需量数据的备份

#define LOG_MEM12_START (LOG_MEM11_START+LOG_MEM11_SIZE)
#define LOG_MEM12_SIZE LOG_MEM3_SIZE  //历史电量需量数据的备份

#define LOG_MEM13_START (LOG_MEM12_START+LOG_MEM12_SIZE)
#define LOG_MEM13_SIZE LOG_MEM0_SIZE  //用于校表参数等IMP0类数据

#define LOG_MEM16_START (LOG_MEM13_START+LOG_MEM13_SIZE)
#define LOG_MEM16_SIZE LOG_MEM14_SIZE  //重要参数,IMP4类数据

#define LOG_MEM17_START (LOG_MEM16_START+LOG_MEM16_SIZE)
#define LOG_MEM17_SIZE LOG_MEM14_SIZE  //重要参数,IMP4类数据

#define LOG_MEM18_START (LOG_MEM17_START+LOG_MEM17_SIZE)
#define LOG_MEM18_SIZE LOG_MEM14_SIZE  //重要参数,IMP4类数据

#define LOG_MEM19_START (LOG_MEM18_START+LOG_MEM18_SIZE)
#define LOG_MEM19_SIZE (PHY_MEM5_SIZE-20000-LOG_MEM18_START)  //负荷曲线

/************************以上是逻辑内存的配置信息**********************/

//物理存储器信息
typedef struct
{
  INT8U MemNo;//物理存储器编号
  INT32U Mem_Addr;//存储器起始地址
  INT32U Mem_Size;//存储器长度
}S_PHY_Mem_Info;

//系统的逻辑存储信息
typedef struct
{
  INT8U LOG_MemNo;//逻辑存储器编号
  S_PHY_Mem_Info PHY_Mem_Info;//对应的物理存储器信息
}S_LOG_Mem_Info;

#ifdef MEMORY_C
//定义物理存储器信息
const S_PHY_Mem_Info Sys_PHY_Mem[]=
{
  {PHY_MEM0,0,PHY_MEM0_SIZE},
  {PHY_MEM1,0,PHY_MEM1_SIZE},
  {PHY_MEM2,0,PHY_MEM2_SIZE},
  {PHY_MEM3,0,PHY_MEM3_SIZE},
  {PHY_MEM4,0,PHY_MEM4_SIZE},
  {PHY_MEM5,0,PHY_MEM5_SIZE}
};

//定义逻辑存储器信息,位于哪个物理存储器，及其在物理存储器中的起始位置和大小
const S_LOG_Mem_Info Sys_LOG_Mem[]=
{
  //PHY_MEM0中
  {LOG_MEM0,{PHY_MEM0,LOG_MEM0_START,LOG_MEM0_SIZE}},//
  //PHY_MEM1中
  {LOG_MEM1,{PHY_MEM1,LOG_MEM1_START,LOG_MEM1_SIZE}},
  {LOG_MEM2,{PHY_MEM1,LOG_MEM2_START,LOG_MEM2_SIZE}},
  {LOG_MEM3,{PHY_MEM1,LOG_MEM3_START,LOG_MEM3_SIZE}},
  {LOG_MEM14,{PHY_MEM1,LOG_MEM14_START,LOG_MEM14_SIZE}},
  //PHY_MEM2中
  {LOG_MEM4,{PHY_MEM2,LOG_MEM4_START,LOG_MEM4_SIZE}},
  {LOG_MEM5,{PHY_MEM2,LOG_MEM5_START,LOG_MEM5_SIZE}},
  {LOG_MEM6,{PHY_MEM2,LOG_MEM6_START,LOG_MEM6_SIZE}},
  {LOG_MEM15,{PHY_MEM2,LOG_MEM15_START,LOG_MEM15_SIZE}},
  //PHY_MEM3中
  {LOG_MEM7,{PHY_MEM3,LOG_MEM7_START,LOG_MEM7_SIZE}},
  {LOG_MEM20,{PHY_MEM3,LOG_MEM20_START,LOG_MEM20_SIZE}},
  {LOG_MEM21,{PHY_MEM3,LOG_MEM21_START,LOG_MEM21_SIZE}},
  //PHY_MEM4中
  //{LOG_MEM8,{PHY_MEM4,LOG_MEM8_START,LOG_MEM8_SIZE}},//该存储器暂时未用
  {LOG_MEM9,{PHY_MEM4,LOG_MEM9_START,LOG_MEM9_SIZE}},
  //PHY_MEM5中
  {LOG_MEM10,{PHY_MEM5,LOG_MEM10_START,LOG_MEM10_SIZE}},
  {LOG_MEM11,{PHY_MEM5,LOG_MEM11_START,LOG_MEM11_SIZE}},
  {LOG_MEM12,{PHY_MEM5,LOG_MEM12_START,LOG_MEM12_SIZE}},
  {LOG_MEM13,{PHY_MEM5,LOG_MEM13_START,LOG_MEM13_SIZE}},
  
  {LOG_MEM16,{PHY_MEM5,LOG_MEM16_START,LOG_MEM16_SIZE}},
  {LOG_MEM17,{PHY_MEM5,LOG_MEM17_START,LOG_MEM17_SIZE}},
  {LOG_MEM18,{PHY_MEM5,LOG_MEM18_START,LOG_MEM18_SIZE}},
  {LOG_MEM19,{PHY_MEM5,LOG_MEM19_START,LOG_MEM19_SIZE}},
};

//注意：Sys_PHY_Mem和Sys_LOG_Mem两个变量相当于是系统存储器的配置
//一旦系统的物理存储结构发生变化，则需要修改Sys_PHY_Mem和Sys_LOG_Mem两个变量
//如果修改后的存储分布大小不能满足存储的要求，则可能需要修改Data_Mem_Map变量
#endif
#endif

