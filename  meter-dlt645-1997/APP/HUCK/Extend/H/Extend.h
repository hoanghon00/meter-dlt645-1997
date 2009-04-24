#ifndef EXTEND_H
#define EXTEND_H

#undef EXT
#ifdef EXTEND_C
#define EXT
#else
#define EXT extern
#endif

#define METER_GD 1 //定义表计算省份

#define METER_PLACE 0

#if METER_PLACE == METER_GD
#include "Extend_GD.h"
#include "Extend_GD_Def_Para.h"
#endif

#if METER_PLACE == 0

#define EVENT_MULTITIMES_DI_EXTEND //分次事件DI的扩展
#define EVENT_MULTITIMES_EXTEND //扩展的分次事件
#define DATA_TRANS_EXTEND     //扩展的传输数据
#define STORA_EXTEND  //存储的扩展
#define DEF_PARA_EXTEND

//版本号定义-->日期-硬件号-软件号-规约版本号-地点-厂家
#define COMM_PROTO_VERSION "090424-1.00-1.00-1.00-General-DongRui"

EXT void Extend_Init();
EXT INT8U Extend_Set_Data_Proc(PROTO_DI DI,INT8U *pSrc, INT16U SrcLen);
EXT INT16U Extend_Event_MultiTimes_Proc(INT8U Event_ID,INT8U Occur_Or_End,INT8U Flag,INT8U *pReturn_Flag);
EXT INT16U Extend_Event_Cumu_Proc(INT8U Event_ID,INT8U Occur_Or_End,INT8U Flag,INT8U *pReturn_Flag);
EXT INT16U Extend_Get_Proto_Data(PROTO_DI DI, INT8U *pDst, INT8U *pDst_Start, INT16U DstLen,INT8U *pErr);
EXT INT16U Extend_Get_Single_Proto_Data_Proc(PROTO_DI DI,INT8U *pDst, INT8U *pDst_Start, INT16U DstLen, INT8U *pErr);
EXT void Extend_Set_Prog_Flag(PROTO_DI DI);
EXT void Extend_Save_PD_Data();
EXT void Extend_Main_Proc();
EXT void Extend_Def_Event_MultiTimes_Data();
EXT void Extend_Set_Def_Event_Cumu_Data();
EXT void Extend_Set_Def_Energy_Data();
#endif

#endif
