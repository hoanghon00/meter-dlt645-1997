#ifndef DATA_SPEC_H
#define DATA_SPEC_H

#include "Authority.h"
#include "Demand.h"

#undef EXT
#ifdef DATA_SPEC_C
#define EXT
#else
#define EXT extern
#endif

#define POS_REACTIVE          0//正向无功,组合无功1
#define NEG_REACTIVE          (POS_REACTIVE+1)//反向无功,组合无功2
#define PHASE_A_POS_REACTIVE  (NEG_REACTIVE+1)//A正向无功
#define PHASE_B_POS_REACTIVE  (PHASE_A_POS_REACTIVE+1)//B正向无功
#define PHASE_C_POS_REACTIVE  (PHASE_B_POS_REACTIVE+1)//C正向无功
#define PHASE_A_NEG_REACTIVE  (PHASE_C_POS_REACTIVE+1)//A反向无功
#define PHASE_B_NEG_REACTIVE  (PHASE_A_NEG_REACTIVE+1)//B反向无功
#define PHASE_C_NEG_REACTIVE  (PHASE_B_NEG_REACTIVE+1)//C反向无功


EXT void Calc_HIS_Data_Time(S_HEX_Time *pSrc,INT8U Bef_Mon,S_HEX_Time *pDst);

EXT INT16U Get_HIS_ENERGY_Data(PROTO_DI DI,S_HEX_Time *pCurTime,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen);
EXT INT16U Get_HIS_DEMAND_Data(PROTO_DI DI,S_HEX_Time *pCurTime,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen);
EXT INT16U Get_HIS_DEMAND_TIME_Data(PROTO_DI DI,S_HEX_Time *pCurTime,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen);
EXT INT16U Get_HIS_VolStat_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U *pErr);
EXT INT16U Get_Freeze_Data(PROTO_DI DI,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr);
EXT INT16U Get_MultiTimes_Event_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U *pErr);
EXT INT16U Get_DLT645_Spec_Single_Data(PROTO_DI DI,INT8U Spec_Flag,void *pPara,INT8U ParaLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pErr);
EXT INT16U Get_DLT645_Spec_Data(PROTO_DI DI,void *pPara,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pErr);
EXT INT8U Set_Spec_Holiday_Data(PROTO_DI DI,INT8U *pSrc,INT8U SrcLen,INT8U *pErr);
EXT INT8U Set_Spec_Data_Proc(PROTO_DI DI,INT8U Spec_Flag,INT8U *pSrc,INT8U SrcLen);
#endif

