#ifndef FREEZE_DATA_H
#define FREEZE_DATA_H

#undef EXT
#ifdef FREEZE_DATA_C
#define EXT
#else
#define EXT extern
#endif

typedef struct{
  PROTO_DI DI;
  INT8U Data_Num;
  PROTO_DI Freeze_DI[12];
}S_Freeze_Data;

//定义定时冻结时间
typedef struct{
  INT8U Min;
  INT8U Hour;  
  INT8U CS[CS_BYTES];
}S_Period_Freeze_Time;

#ifdef FREEZE_DATA_C
CONST S_Freeze_Data Freeze_Data[2]=
{
  {0xEF01,12,{0x901F,0x902F,DI_A_VOL_1,DI_B_VOL_1,DI_C_VOL_1,0xB62F,0xB63F,0xB64F,0xB65F,0xB66F,0xB67F,DI_CUR_TIME}}, 
  {0xEF03,5,{0x913F,0x915F,0x916F,0x914F,DI_CUR_TIME}}
};
#endif
extern CONST S_Freeze_Data Freeze_Data[];
EXT volatile S_Period_Freeze_Time Energy_Freeze_Time;//定义周期冻结时间
EXT volatile S_Period_Freeze_Time Reactive_Freeze_Time;//武功周期冻结时间

EXT void Read_Freeze_Data_Para_From_Rom();
EXT void Freeze_Right_Now(INT8U Index);
EXT INT8U Protocol_Freeze_Data(INT8U Flag);
EXT void Period_Freeze_Data_Proc();
EXT INT16U Get_Period_Freeze_Data(S_Freeze_Data *pData,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen);
EXT void Set_Def_Freeze_Data();//置默认的冻结数据
#endif
