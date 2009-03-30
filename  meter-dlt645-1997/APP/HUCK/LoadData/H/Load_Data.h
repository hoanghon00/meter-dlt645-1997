#ifndef LOAD_DATA_H
#define LOAD_DATA_H

#undef EXT
#ifdef LOAD_DATA_C
#define EXT
#else
#define EXT extern
#endif

#define NULL_LOAD_INDEX 0xFFFF

#define LOAD_DATA_HEAD_SIZE 7//负荷曲线头数据的大小包括2字节的0xA0和5个字节的时间
#define LOAD_DATA_TYPE_NUM 6//负荷曲线的6类数据

//定义符合曲线索引
typedef struct{
  INT16U Start; //第一条的索引
  INT16U Last; //最后一条的索引
  
  INT16U Last_Read;
  INT16U Hour_Index;
  
  INT8U CS[CS_BYTES];//校验和
}S_LoadData_Index;

//负荷曲线存储相关信息，定义需要存储的数据项
typedef struct{
  INT16U DI;
  INT16U Len;
  INT8U Type;
}S_LoadData_Info;

//负荷曲线参数
typedef struct{
 INT8U Time[4];
 INT16U Period[6];
 INT8U CS[CS_BYTES]; 
}S_LoadData_Para;

#ifdef LOAD_DATA_C
CONST S_LoadData_Info LoadData_Info[]=
{
  //模式字bit0
  {DI_A_VOL,3,0},//电压
  {DI_B_VOL,3,0},//电压
  {DI_C_VOL,3,0},//电压
  {DI_A_CUR_3,3,0},//电流
  {DI_B_CUR_3,3,0},//电流
  {DI_C_CUR_3,3,0},//电流
  {0xB660,2,0},//频率
  //模式字bit1
  {DI_ACTIVE_DIRECT,1,1},//有功方向标识字
  {DI_REACTIVE_DIRECT,1,1},//无功方向标识字
  //{0xB63F,12,1},//有功功率
  {0xB631,3,1},
  {0xB632,3,1},
  {0xB633,3,1},
  {0xB630,3,1},

  {DI_REACTIVE_A,3,1},//无功功率
  {DI_REACTIVE_B,3,1},//无功功率
  {DI_REACTIVE_C,3,1},//无功功率
  {DI_REACTIVE_ALL,3,1},//无功功率  
  
  //模式字bit2
  {0xB65F,8,2},//功率因素
  //模式字bit3
  {0x9010,4,3},//正向有功总
  {0x9110,4,3},//正向无功总，(组合无功1总)
  {0x9020,4,3},//反向有功总
  {0x9120,4,3},//反向无功总，(组合无功2总)
  //模式字bit4
  {0x9130,4,4},//一象限无功总
  {0x9150,4,4},//二象限无功总
  {0x9160,4,4},//三象限无功总
  {0x9140,4,4},//四象限无功总
  //模式字bit5 
  {DI_ACTIVE_DEMAND,3,5},//有功需量
  {DI_REACTIVE_DEMAND,3,5}//无功需量
};
#endif

//EXT volatile INT8U LoadData_Buf[150];
EXT NO_INIT volatile S_LoadData_Para LoadData_Para;
EXT NO_INIT volatile S_LoadData_Index LoadData_Index;
EXT NO_INIT volatile S_LoadData_Index LoadData_Index_Bak;

EXT void Read_LoadData_Para_From_Rom();
EXT void Read_LoadData_Index_Data_From_Rom();
EXT INT16U Read_Load_Data_With_Time(S_HEX_Time *pTime,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr);
EXT INT16U Read_Load_Data(PROTO_DI DI,INT8U Para[],INT8U ParaLen,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr);
EXT void Set_Def_Load_Data_Index();
EXT void Save_LoadData_Index();
EXT void Save_LoadData_Index_Bak();
EXT INT8U Write_Load_Data(INT8U *pSrc,INT16U SrcLen);
EXT void Check_LoadData_Index_Avail();
EXT void Check_LoadData_Para_Avail();
EXT INT16U Get_LoadData_Hour_Index(INT8U Time[]);
EXT INT8U Check_LoadData_Index(S_LoadData_Index *pInfo);
EXT void Save_LoadData_PD_Data();
EXT INT16U Make_LoadData();
EXT INT16U Get_LoadData_All_Len();
EXT INT16U Get_LoadData_Type_Len(INT8U Type);
EXT void LoadData_Proc();

#endif
