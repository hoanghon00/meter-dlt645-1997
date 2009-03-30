#define EXTEND_GD_C
#include "Includes.h"

#if METER_PLACE == METER_GD
//分次事件数据项目的扩展处理
INT16U Extend_Event_MultiTimes_Proc(INT8U Event_ID,INT8U Occur_Or_End,INT8U Flag,INT8U *pReturn_Flag)
{
  Event_ID = Event_ID;
  Occur_Or_End = Occur_Or_End;
  Flag = Flag;
  *pReturn_Flag=0;
  return 0; 
}

INT16U Extend_Event_Cumu_Proc(INT8U Event_ID,INT8U Occur_Or_End,INT8U Flag,INT8U *pReturn_Flag)
{
  Event_ID = Event_ID;
  Occur_Or_End = Occur_Or_End;
  Flag = Flag;
  *pReturn_Flag=0;
  return 0;   
  
}

void Extend_Init()
{
  
}

INT8U Extend_Set_Data_Proc(PROTO_DI DI,INT8U *pSrc, INT16U SrcLen)
{
    return PROTOCOL_DI_ERR;  
}

//读取扩展的数据项目
INT16U Extend_Get_Proto_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start, INT16U DstLen, INT8U *pErr)
{
  *pErr=PROTOCOL_DI_ERR;
  return 0;
}

INT16U Extend_Get_Single_Proto_Data_Proc(PROTO_DI DI,INT8U *pDst, INT8U *pDst_Start, INT16U DstLen, INT8U *pErr)
{
  *pErr = PROTOCOL_DI_ERR;
  return 0;  
}

void Extend_Save_PD_Data()
{
  
}

//扩展的编程记录
void Extend_Set_Prog_Flag(PROTO_DI DI)
{
  DI = DI;
  
}

void Extend_Main_Proc()
{
  
  
}

void Extend_Def_Event_MultiTimes_Data()
{
  
}

void Extend_Set_Def_Event_Cumu_Data()
{
  
}

void  Extend_Set_Def_Energy_Data()
{
  
}
#endif
#undef EXTEND_GD_C
