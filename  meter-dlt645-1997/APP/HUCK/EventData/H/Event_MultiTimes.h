#ifndef EVENT_MULTITIMES_H
#define EVENT_MULTITIMES_H

//#include "Data_Para.h"

#undef EXT
#ifndef EVENT_MULTITIMES_C
#define EXT extern
#else
#define EXT
#endif

#define MAX_MULTI_TIMES_DI_NUM 18
#define MAX_EVENT_MULTITIMES_NUM 30

#define EVENT_REAL 0    //真实事件发生
#define EVENT_VIRTRUAL 1//虚拟事件发生

#define ONE_RECORD_EVENT 0
#define MULTI_RECORD_EVENT 1

#define EVENT_OCCUR 0x5A//表示事件发生,该数值和EVENT_END都不能为0
#define EVENT_END 0xA5//表示事件的结束，这两个宏主要是针对CONTIN_EVENT类型的事件

#define NULL_EVENT_STORAGE_DI 0xFF00FF00
/*
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
*/
#define MAX_EVENT_DATA_NUM 30

typedef struct
{
  INT32U Mins;
  INT8U Start_Time[5];
  INT8U End_Time[5];
  INT8U CS[CS_BYTES];  
}S_All_Loss_Vol_Time;

typedef struct
{
  INT32U Occur_Mask; //事件发生数据项屏蔽字,某位为1表示采用某数据项,下同
  INT32U End_Mask;  //事件结束数据项屏蔽字
  INT32U Diff_Mask;  //发生-结束期间差量屏蔽字
  INT8U Data_Num;  //需要记录的数据项个数
  INT16U Data_Len; //报警数据长度
  PROTO_DI DI[MAX_EVENT_DATA_NUM]; //需要记录的数据项
}S_Event_DI;


typedef struct
{
  INT8U Event_ID;    //事件记录的ID
  //定义数据生成相关信息
  INT8U Property;    //事件记录属性

  S_Event_DI const *pData;

  PROTO_DI Proto_DI;//最近一次该事件发生的协议DI
                    // 当事件为OCCUR_AND_END_EVENT类事件时，因为发生和结束都在一个协议DI里面，所以
                    // 只需用该DI表示该事件的DI
  //PROTO_DI End_DI;  //最近一次该事件结束的协议DI,当事件为OCCUR_AND_END_EVENT类事件时该DI没有作用
                    // 当时间为OCCUR_OR_END_EVENT类事件时该DI表示事件结束的协议DI
                    
  // 定义存储相关的信息
  STORA_DI  Last_Storage_DI;
  //存储最近一次事件的存储DI,其存储的内容为 Start_DI-Start_DI+Num
  STORA_DI Storage_DI;
  INT8U Storage_Num;//该事件共存储多少条记录
  
  INT8U Clr_Flag; //是否允许清除标志
}S_Event_Data_Info;

extern CONST S_Event_Data_Info Event_Data_Info[];

EXT volatile INT8U Last_PD_Time[5];//上次掉电时间
EXT volatile S_All_Loss_Vol_Time All_Loss_Vol_Time;//最近一次全失压的发生和结婚时间,用于生成协议数据

EXT void All_Loss_Vol_Data_Proc(INT8U Start_Time[],INT8U End_Time[]);
EXT void Event_Vol_Cur_Var_Pre_Set(INT8U Event_ID,INT8U Occur_Or_End);
EXT INT16U Get_Event_Data_Index(INT8U Event_ID);
EXT INT16U Get_Event_Data_Index_From_DI(PROTO_DI DI);
EXT STORA_DI Read_Last_Event_Storage_DI(INT8U Event_ID);
EXT INT16U Read_Last_Event_Data(INT8U Event_ID,STORA_DI Storage_DI,void *pDst,void *pDst_Start,INT16U DstLen);
EXT INT16U Event_MultiTimes_Normal_Proc(INT8U Event_ID,INT8U Occur_Or_End,INT8U Flag);
EXT INT16U Event_MultiTimes_Proc(INT8U Event_ID,INT8U Occur_Or_End,INT8U Flag);
EXT INT16U Get_Event_Data_Info_Num();
EXT void Set_Def_Event_MultiTimes_Data();
EXT void Clear_Event_MultiTimes_Data();
EXT void Event_Loss_Vol_Spec_Proc(INT8U Event_ID,INT8U Occur_Or_End,INT8U Flag);

#endif

