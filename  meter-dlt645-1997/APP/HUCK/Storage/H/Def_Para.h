#ifndef DEF_PARA_H
#define DEF_PARA_H

#undef EXT
#ifdef DEF_PARA_C
#define EXT
#else
#define EXT extern
#endif

#define FIRST_BOOT_ON_FLAG 0x55AAA55A
#define BYTE_0(x) (INT8U)((INT32U)x&0xFF)
#define BYTE_1(x) (INT8U)(((INT32U)x&0x0000FF00)>>8)
#define BYTE_2(x) (INT8U)(((INT32U)x&0x00FF0000)>>16)

#define DISP_CLR_DATA_INFO Main_Dis_Info("CLr dATA")

typedef struct
{
  STORA_DI DI;
  INT8U Num;
  CONST INT8U *pPara;
}S_Def_Para_Info;


extern CONST INT8U Zero[];
//extern CONST S_Def_Para_Info;
//extern CONST S_Energy Def_Energy;//默认的电量、需量和需量时间
//extern CONST S_Demand Def_Demand;
#define Def_VolStat Zero
#define Def_LoadData_Index Zero

EXT void Write_Def_Para();
EXT void Write_One_Def_Para(PROTO_DI DI);
EXT INT8U Check_Def_Para(STORA_DI DI);
EXT void Set_Def_Adj_Para();
EXT void Set_All_Def_Data();
EXT void Set_Def_Para_Except_Adj();
EXT void Check_Boot_On_Flag();
EXT INT16U Read_Def_Para(STORA_DI DI,void *pDst,INT16U Len,void *pDst_Start,INT16U DstLen);
#endif
