#ifndef DATA_PARA_H
#define DATA_PARA_H

#include "Data_Trans.h"

#undef EXT
#ifdef DATA_PARA_C
#define EXT
#else
#define EXT extern
#endif

#define MAX_INIT_CLR_DOG_COUTS 20000

//定义存储在到eerom中的电量数据的大小
#define ENERGY_SAVE_SIZE sizeof(S_Energy)//(S_OFF(S_Energy,CS))
#define ONE_DEMAND_SAVE_SIZE sizeof(S_One_Demand)//(S_OFF(S_One_Demand,CS))
#define DEMAND_SAVE_SIZE ((ONE_DEMAND_SAVE_SIZE+ROM_CS_BYTES)*(MAX_RATES+1)+ROM_CS_BYTES)//将该数据区分为MAX_RATES+1区，每个区域存储一个费率的数据
#define LOAD_DATA_HOUR_INDEX_NUM ((INT16U)24*12*31*4)//每小时一个索引数据 090311修改为没15分钟一条索引数据

#define HIS_ENERGY_DATA         0//历史电量数据
#define HIS_DEMAND_DATA         1//历史需量数据
#define HIS_DEMAND_TIME_DATA    2//历史需量时间数据
#define HIS_VOLSTAT_DATA        3//历史电压统计数据

//---------定义各种扩充数据的存储标示,都以0x10000开始,这些数据项不会在Protocol_Data中定义-begin-----------------------//
#define BOOT_FLAG          0x010000//启动标志,表征是否是第一次上电

#define CUR_ENERGY_PD      0x010001//当前电量掉电存储
#define CUR_DEMAND_PD      0x010002//当前需量掉电存储

#define CUR_VOLSTAT_PD     0x010003//当月电压检测数据
#define TOTAL_VOLSTAT_PD   0x010004//总电压检测数据
#define CUR_EVENT_PD       0x010005//事件累计数据
#define CUMU_EVENT_PD      0x010006//事件累计掉电存储
#define TIME_PD            0x010008//掉电时间
#define TIME_BAK_PD        0x010009//掉电时间备份

#define CUR_ENERGY         0x01000A//当前电量
#define CUR_DEMAND         0x01000B//当前需量
#define CUR_DEMAND_TIME    0x01000C//当前需量时间
#define CUR_VOLSTAT        0x01000D//当前电压检测数据
#define TOTAL_VOLSTAT      0x01000E//总电压检测数据
#define EVENT_CUMU         0x01000F//事件累计数据

#define CUR_ENERGY_BAK0    0x010010//当前电量备份
#define CUR_ENERGY_BAK1    0x010011
#define CUR_ENERGY_BAK2    0x010012
#define CUR_DEMAND_BAK     0x010013//当前需量备份
#define CUR_VOLSTAT_BAK    0x010014//当前电压检测数据备份
#define TOTAL_VOLSTAT_BAK  0x010015//总电压检测数据备份
//#define EVENT_CUMU_BAK     0x010016//事件累计数据的备份
#define SAVE_PD_DATA_TIME  0x010016//存储掉电存数使用的时间

#define ADJ_METER_PARA     0x010020//校表参数,256字节
#define METER_PARA0        0x010021//以下是5个1字节的较表参数
#define METER_PARA1        0x010022
#define METER_PARA2        0x010023
#define METER_PARA3        0x010024
#define METER_PARA4        0x010025

#define HIS_ENERGY         0x010100//历史电量,HIS_ENERGY_DATA_MONTHS个数据项
                                   //保留HIS_ENERGY_DATA_MONTHS个
#define HIS_DEMAND         0x010200//历史需量,HIS_ENERGY_DATA_MONTHS个数据项
                                   //保留HIS_ENERGY_DATA_MONTHS个
#define HIS_VOLSTAT        0x010300//历史电压统计数据,HIS_VOLSTAT_DATA_MONTHS个数据项
                                   //保留HIS_VOLSTAT_DATA_MONTHS个
#define ENERGY_FREEZE_TIME   0x010400//定义总电量周期冻结时间
#define REACTIVE_FREEZE_TIME 0x010401//无功电量周期冻结时间

#define PROG_RECORD          0x010402 //编程记录
#define CLR_DEMAND_RECORD    0x010403 //清需量记录

#define LOAD_DATA_INDEX      0x010500//负荷曲线的存储信息
#define LOAD_DATA_INDEX_BAK  0x010501//负荷曲线的存储信息备份
#define LOAD_DATA_INDEX_PD   0x010502//负荷曲线索引掉电存储

//事件记录最近一次存储DI的存储
#define E_E400                0x01E400//
#define E_E900                0x01E900//事件记录
#define E_E910                0x01E910
#define E_E920                0x01E920
#define E_E930                0x01E930
#define E_E940                0x01E940
#define E_E950                0x01E950//事件记录
#define E_E960                0x01E960
#define E_E970                0x01E970
#define E_E980                0x01E980
#define E_E990                0x01E990
#define E_E9A0                0x01E9A0
#define E_E9B0                0x01E9B0
#define E_E9C0                0x01E9C0
#define E_E9D0                0x01E9D0
#define E_E9E0                0x01E9E0
#define E_E9F0                0x01E9F0

#define E_EA00                0x01EA00
#define E_EA10                0x01EA10
#define E_EA20                0x01EA20
#define E_EA30                0x01EA30
#define E_EA40                0x01EA40
#define E_EA50                0x01EA50
#define E_EA60                0x01EA60
#define E_EA70                0x01EA70
#define E_EA80                0x01EA80
#define E_EA90                0x01EA90
#define E_EAA0                0x01EAA0
#define E_EAB0                0x01EAB0
#define E_EAC0                0x01EAC0
#define E_EAD0                0x01EAD0
#define E_EAE0                0x01EAE0
#define E_EAF0                0x01EAF0

#define E_EB00                0x01EB00
#define E_EB10                0x01EB10
#define E_EB20                0x01EB20
#define E_EB30                0x01EB30
#define E_EB40                0x01EB40
#define E_EB50                0x01EB50
#define E_EB60                0x01EB60
#define E_EB70                0x01EB70
#define E_EB80                0x01EB80
#define E_EB90                0x01EB90
#define E_EBA0                0x01EBA0
#define E_EBB0                0x01EBB0
#define E_EBC0                0x01EBC0
#define E_EBD0                0x01EBD0
#define E_EBE0                0x01EBE0
#define E_EBF0                0x01EBF0

#define E_EC00                0x01EC00
#define E_EC10                0x01EC10
#define E_EC20                0x01EC20
#define E_EC30                0x01EC30
#define E_EC40                0x01EC40
#define E_EC50                0x01EC50
#define E_EC60                0x01EC60
#define E_EC70                0x01EC70
#define E_EC80                0x01EC80
#define E_EC90                0x01EC90
#define E_ECA0                0x01ECA0
#define E_ECB0                0x01ECB0
#define E_ECC0                0x01ECC0
#define E_ECD0                0x01ECD0
#define E_ECE0                0x01ECE0
#define E_ECF0                0x01ECF0

#define E_ED00                0x01ED00
#define E_ED10                0x01ED10
#define E_ED20                0x01ED20
#define E_ED30                0x01ED30
#define E_ED40                0x01ED40
#define E_ED50                0x01ED50
#define E_ED60                0x01ED60
#define E_ED70                0x01ED70
#define E_ED80                0x01ED80
#define E_ED90                0x01ED90
#define E_EDA0                0x01EDA0
#define E_EDB0                0x01EDB0
#define E_EDC0                0x01EDC0
#define E_EDD0                0x01EDD0
#define E_EDE0                0x01EDE0

/*
//失压数据-时间和次数
#define TOTAL_LOSS_VOL        0x01F000
#define A_LOSS_VOL            0x01F001
#define B_LOSS_VOL            0x01F002
#define C_LOSS_VOL            0x01F003
#define AB_LOSS_VOL           0x01F004
#define AC_LOSS_VOL           0x01F005
#define BC_LOSS_VOL           0x01F006
#define ABC_LOSS_VOL          0x01F007
#define ALL_LOSS_VOL          0x01F008

//失流数据-时间和次数
#define TOTAL_LOSS_CUR        0x01F010
#define A_LOSS_CUR            0x01F011
#define B_LOSS_CUR            0x01F012
#define C_LOSS_CUR            0x01F013
#define AB_LOSS_CUR           0x01F014
#define AC_LOSS_CUR           0x01F015
#define BC_LOSS_CUR           0x01F016
#define ALL_LOSS_CUR          0x01F017

//断相数据
#define TOTAL_BREAK_VOL       0x01F020
#define A_BREAK_VOL           0x01F021
#define B_BREAK_VOL           0x01F022
#define C_BREAK_VOL           0x01F023

#define TOTAL_REVERSE         0x01F030
#define A_REVERSE             0x01F031
#define B_REVERSE             0x01F032 
#define C_REVERSE             0x01F033
 
//失压累计电量数据
#define _TOTAL_LOSS_VOL       0x01F040
#define _A_LOSS_VOL           0x01F041
#define _B_LOSS_VOL           0x01F042
#define _C_LOSS_VOL           0x01F043
#define _AB_LOSS_VOL          0x01F044 
#define _AC_LOSS_VOL          0x01F045
#define _BC_LOSS_VOL          0x01F046
#define _ABC_LOSS_VOL         0x01F047
#define _ALL_LOSS_VOL         0x01F048

//失流累计电量数据
#define _TOTAL_LOSS_CUR       0x01F050
#define _A_LOSS_CUR           0x01F051
#define _B_LOSS_CUR           0x01F052
#define _C_LOSS_CUR           0x01F053
#define _AB_LOSS_CUR          0x01F054
#define _AC_LOSS_CUR          0x01F055
#define _BC_LOSS_CUR          0x01F056
#define _ALL_LOSS_CUR         0x01F057
*/
//失压
#define OCCUR_TOTAL_LOSS_VOL 0x020000
#define OCCUR_A_LOSS_VOL     0x020001
#define OCCUR_B_LOSS_VOL     0x020002
#define OCCUR_C_LOSS_VOL     0x020003
#define OCCUR_AB_LOSS_VOL    0x020004
#define OCCUR_AC_LOSS_VOL    0x020005
#define OCCUR_BC_LOSS_VOL    0x020006
#define OCCUR_ABC_LOSS_VOL   0x020007
//全失压
#define OCCUR_ALL_LOSS_VOL   0x020008

#define TOTAL_TOTAL_LOSS_VOL 0x020010
#define TOTAL_A_LOSS_VOL     0x020011
#define TOTAL_B_LOSS_VOL     0x020012
#define TOTAL_C_LOSS_VOL     0x020013
#define TOTAL_AB_LOSS_VOL    0x020014
#define TOTAL_AC_LOSS_VOL    0x020015
#define TOTAL_BC_LOSS_VOL    0x020016
#define TOTAL_ABC_LOSS_VOL   0x020017
//全失压
#define TOTAL_ALL_LOSS_VOL   0x020018

//失流
#define OCCUR_TOTAL_LOSS_CUR 0x020020
#define OCCUR_A_LOSS_CUR     0x020021
#define OCCUR_B_LOSS_CUR     0x020022
#define OCCUR_C_LOSS_CUR     0x020023
#define OCCUR_AB_LOSS_CUR    0x020024
#define OCCUR_AC_LOSS_CUR    0x020025
#define OCCUR_BC_LOSS_CUR    0x020026
//全失流
#define OCCUR_ABC_LOSS_CUR   0x020027

#define TOTAL_TOTAL_LOSS_CUR 0x020030
#define TOTAL_A_LOSS_CUR     0x020031
#define TOTAL_B_LOSS_CUR     0x020032
#define TOTAL_C_LOSS_CUR     0x020033
#define TOTAL_AB_LOSS_CUR    0x020034
#define TOTAL_AC_LOSS_CUR    0x020035
#define TOTAL_BC_LOSS_CUR    0x020036
#define TOTAL_ABC_LOSS_CUR   0x020037

//反向
#define OCCUR_TOTAL_NEG_CUR 0x020040
#define OCCUR_A_NEG_CUR     0x020041
#define OCCUR_B_NEG_CUR     0x020042
#define OCCUR_C_NEG_CUR     0x020043

#define TOTAL_TOTAL_NEG_CUR 0x020050
#define TOTAL_A_NEG_CUR     0x020051
#define TOTAL_B_NEG_CUR     0x020052
#define TOTAL_C_NEG_CUR     0x020053

//断相
#define OCCUR_TOTAL_BREAK_VOL 0x020060
#define OCCUR_A_BREAK_VOL     0x020061
#define OCCUR_B_BREAK_VOL     0x020062
#define OCCUR_C_BREAK_VOL     0x020063

#define TOTAL_TOTAL_BREAK_VOL 0x020070
#define TOTAL_A_BREAK_VOL     0x020071
#define TOTAL_B_BREAK_VOL     0x020072
#define TOTAL_C_BREAK_VOL     0x020073

#define LOAD_DATA_HOUR_INDEX 0x030000//符合曲线小时索引存储,共有24*31*12*4个项目
#define LOAD_DATA            0x040000//负荷曲线,0x1000-->0x1000+LOAD_DATA_NUM
                             //保留LOAD_DATA_NUM个

//////////以下DI必须和Data_trans.h中的保持一致,以为可能需要外部来通过Get_DLT645_Data来抄读数据
#define METER_ADDR0        0x0105//表地址0
#define METER_ADDR1        0x0106//表地址1
#define METER_ADDR2        0x0107//表地址2

#define COMM_BAUD0         0x0108//第一路波特率
#define COMM_BAUD1         0x0109//第二路波特率
#define COMM_BAUD2         0x010A//第三路波特率

#define PASSWORD_0         0x010B//0级密码,协议中无此项，因为此项不能读，也不能直接通过DI写，但是该项必须符合PROTO_DI的类型大小,因为Set_Prog_函数会使用该DI
#define PASSWORD_1         0x010C//1级密码
#define PASSWORD_2         0x010D//2级密码

#define ADJ_METER_PUL_VOL_CUR_SPEC 0xCEE0//脉冲、电压、电流规格,3字节
#define ADJ_METER_PARA_STATUS      0xCEE1//校表参数状态设置,1字节
#define ADJ_METER_A_POW            0xCEE2//A相功率增益参数设置,3字节
#define ADJ_METER_B_POW            0xCEE3//B相功率增益参数设置,3字节
#define ADJ_METER_C_POW            0xCEE4//C相功率增益参数设置,3字节
#define ADJ_METER_A_PHASE          0xCEE5//A相相位增益参数设置,3字节
#define ADJ_METER_B_PHASE          0xCEE6//B相相位增益参数设置,3字节
#define ADJ_METER_C_PHASE          0xCEE7//B相相位增益参数设置,3字节
#define ADJ_METER_A_VOL            0xCEE8//A相电压参数设置,3字节
#define ADJ_METER_B_VOL            0xCEE9//B相电压参数设置,3字节
#define ADJ_METER_C_VOL            0xCEEA//C相电压参数设置,3字节
#define ADJ_METER_A_CUR            0xCEEB//A相电流参数设置,3字节
#define ADJ_METER_B_CUR            0xCEEC//B相电流参数设置,3字节
#define ADJ_METER_C_CUR            0xCEED//C相电流参数设置,3字节

#define TOTAL_LOSS_VOL_OCCUR_TIME 0xB330
#define A_LOSS_VOL_OCCUR_TIME     0xB331
#define B_LOSS_VOL_OCCUR_TIME     0xB332
#define C_LOSS_VOL_OCCUR_TIME     0xB333
#define AB_LOSS_VOL_OCCUR_TIME    0xB334
#define AC_LOSS_VOL_OCCUR_TIME    0xB335
#define BC_LOSS_VOL_OCCUR_TIME    0xB336
#define ABC_LOSS_VOL_OCCUR_TIME   0xB337
//#define ALL_LOSS_VOL_OCCUR_TIME   0xB338

#define TOTAL_LOSS_VOL_END_TIME   0xB340
#define A_LOSS_VOL_END_TIME       0xB341
#define B_LOSS_VOL_END_TIME       0xB342
#define C_LOSS_VOL_END_TIME       0xB343
#define AB_LOSS_VOL_END_TIME      0xB344
#define AC_LOSS_VOL_END_TIME      0xB345
#define BC_LOSS_VOL_END_TIME      0xB346
#define ABC_LOSS_VOL_END_TIME     0xB347
//#define ALL_LOSS_VOL_END_TIME     0xB348


#define CHK_IMP0           0xFFFFFFF0  
#define CHK_IMP1           0xFFFFFFF1
#define CHK_IMP2           0xFFFFFFF2
#define CHK_IMP3           0xFFFFFFF3
#define CHK_IMP4           0xFFFFFFF4
#define CHK_IMP5           0xFFFFFFF5
#define CHK_IMP6           0xFFFFFFF6
#define CHK_IMP7           0xFFFFFFF7
//---------------定义各种数据的存储标示-end-----------------------//

//----------------定时数据长度------------
#define LOSS_VOL_DATA_LEN ((19*4) + 13)//失压累计数据长度 
#define LOSS_CUR_DATA_LEN ((16*4) + 13)//失流累计数据长度
#define NEG_CUR_DATA_LEN  ((8*4) + 13) //反向累计数据长度
#define ALL_LOSS_VOL_DATA_LEN 13     //全失压累计数据长度
#define ALL_LOSS_CUR_DATA_LEN 13
#define BREAK_VOL_DATA_LEN 13        //断相累计数据长度

//------------------------------------------------

#define ADJ_METER_PARA_SIZE  256
#define LOAD_DATA_SIZE       118//每条负荷曲线数据的大小
#define LOAD_DATA_INDEX_SIZE 5//每个小时索引数据的大小
#define LOAD_DATA_INDEX_NUM  8544//负荷曲线的小时索引条数,每小时一条，可以存8544条

#define PSW0_DI            0x0001//0级密码标识符
#define PSW1_DI            0x0002//1级密码标识符
#define PSW2 DI            0x0003//2级密码标识符

#define NULL_IMP 0xFF //一个不存在的IMP标志
#define IMP0 0x00  //重要数据,校表参数和模式字
#define IMP1 0x01  //当前电量需量等数据
#define IMP2 0x02  //历史数据和备份数据
#define IMP3 0x03  //普通事件记录
#define IMP4 0x04  //参数
#define IMP5 0x05  //负荷曲线以及相关参数
#define IMP6 0x06  //掉电存储数据
#define IMP7 0x07  //重要事件记录

//#define HIS_HEAD sizeof(S_HEX_Time) //存储历史电量需量数据时的头长度

#define IMP_LOG_MEM 5//某类重要数据存储的逻辑存储器个数,最大值
/*
#define IMP_ADJ     0x00 //重要数据,校表参数和模式字
#define IMP_ENERGY   0x01
#define IMP_HIS      0x02
#define IMP_EVENT    0x03
#define IMP_LOAD     0x04
#define IMP_POWD     0x05
*/
//下面需要引用该类型
/*
typedef struct
{
  INT8U Loss_Vol_Flag[4];
  INT32U Loss_Vol_Counts[4];//总、A、B、C失压次数 
  INT32U Loss_Vol_Time[4];//总失压累计时间
  
  INT8U Loss_Cur_Flag[4];
  INT32U Loss_Cur_Counts[4];//总失流次数
  INT32U Loss_Cur_Time[4];//总失流时间
  
  INT8U CS[CS_BYTES];
}S_Event_Cumu;
*/

typedef struct
{
  INT8U Imp_Flag;//数据标志
  INT8U LOG_Mem[IMP_LOG_MEM];//其存储的逻辑存储器，第一个是原始数据，后三个是备份，NULL_MEM表示没有
}S_Data_Mem_Map;

//#define RD_WR 0//可读可写
#define WR 1   //只可写
#define RD 2   //可读
typedef struct
{
  INT8U Imp_Flag;
  INT32U Offset;
}S_Data_Para_Info;

typedef struct{
  STORA_DI DI; 
//参数标识,与DLT645兼容，同时可扩充。
  //INT8U RW_Flag;
//参数读写标志，宏READ表示可读，宏WRITE 表示可写,READ|WRITE表示可读可写
  //INT8U PSW_Flag;
//参数密码等级标志,PSW_N表示在N级密码以上才可修改该参数(N=0,1,2)
  INT8U Imp_Flag;
//重要等级标志 宏IMP0表示最重要 IMP1->IMP5重要性依次递减
  INT16U Len; //数据或参数长度
//可否考虑加读写错误次数？这样可加强IMP不同的参数的容错能力。
  INT16U Num;
  //类似于此项的个数
}S_Data_Para_Storage_Info;


EXT INT8U Check_SuperPSW_Jump();
EXT INT8U Check_Imp_Flag(INT8U Imp_Flag);
EXT INT8U Get_Storage_Data_ImpFlag(STORA_DI DI);
EXT INT32U Get_Storage_Data_Off(STORA_DI DI);
EXT INT16U Get_Storage_Data_Len(STORA_DI DI);
EXT INT32U Get_Imp_Data_Size(INT8U Imp_Flag);
EXT INT32U Get_Imp_Mem_Size(INT8U Imp_Flag);
EXT INT8U Read_Imp_Data(INT8U Imp_Flag,INT32U Offset,void *pDst,INT16U RD_Len,void *pDst_Start,INT16U DstLen);
EXT INT8U Write_Imp_Data(INT8U Imp_Flag,INT32U Offset,void *pSrc,INT16U SrcLen);

EXT INT16U Read_Storage_Data_Fix_Len(STORA_DI DI,INT16U Offset,INT16U Len,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr);
EXT INT8U Write_Storage_Data_Fix_Len(STORA_DI DI,INT16U Offset,void *pSrc,INT16U SrcLen);

EXT INT16U Read_Storage_Data(STORA_DI DI,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr);
EXT INT8U Write_Storage_Data(STORA_DI DI,void *pSrc,INT16U SrcLen);

EXT INT8U Check_Imp_Data_Storage();
EXT void Check_Sys_Storage();
//EXT INT16U Read_HIS_Data(INT8U Data_Flag,S_HEX_Time *pTime,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr);
//EXT INT8U Write_HIS_Data(INT8U Data_Flag,S_HEX_Time *pTime,void *pSrc,INT16U SrcLen);

EXT void Check_Data_Storage();
EXT void Debug_Print_Data_Storage();
EXT void Check_Sys_Storage();
#endif
