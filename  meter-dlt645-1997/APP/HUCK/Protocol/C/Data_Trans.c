#define DATA_TRANS_C
#include "Includes.h"

CONST INT8U Software_Version[5]=SOFT_VERSION;//软件版本号
CONST INT8U Comm_Proto_Version[]={COMM_PROTO_VERSION};
CONST INT8U No_Var[10]={0};

//设置参数开放时间
#ifdef EXTEND_OPEN_SET_AUTH_TIME
#define OPEN_SET_AUTH_TIME EXTEND_OPEN_SET_AUTH_TIME
#else
#define OPEN_SET_AUTH_TIME 0
#endif

const S_Protocol_Data_Info Protocol_Data[]=
{
  DATA_TRANS_EXTEND 

  
//标识编码	数据格式	数据长度	单位	  读	写	数据项名称
  {0x6000,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,1,4,0x3010|((HIS_ENERGY_DATA_MONTHS+1)<<8)|(MAX_RATES+1),SPEC_RATE},//正向有功
  {0x6010,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTS,1,4,0x3010|((HIS_ENERGY_DATA_MONTHS+1)<<8)|(MAX_RATES+1),SPEC_RATE},//正向无功
  {0x6020,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,1,4,0x3010|((HIS_ENERGY_DATA_MONTHS+1)<<8)|(MAX_RATES+1),SPEC_RATE},//反向有功
  {0x6030,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTS,1,4,0x3010|((HIS_ENERGY_DATA_MONTHS+1)<<8)|(MAX_RATES+1),SPEC_RATE},//反向无功
  {0x6040,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,1,4,0x3040|((HIS_ENERGY_DATA_MONTHS+1)<<8)|(MAX_RATES+1),SPEC_RATE},//四象限无功 
  
  //正反向有功电能
  {0x9010,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,1,4,0x1120|(MAX_RATES+1),SPEC_RATE},
  //正反向无功、1-4象限无功电能
  {0x9110,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTS,1,4,0x1120|(MAX_RATES+1),SPEC_RATE},
  {0x9130,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,1,4,0x1140|(MAX_RATES+1),SPEC_RATE},
  //上月正反向有功电能
  {0x9410,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,1,4,0x1120|(MAX_RATES+1),SPEC_RATE},
  //上月正反向无功、1-4象限无功电能
  {0x9510,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTS,1,4,0x1120|(MAX_RATES+1),SPEC_RATE},
  {0x9530,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,1,4,0x1140|(MAX_RATES+1),SPEC_RATE},
  //上上月正反向有功电能
  {0x9810,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,1,4,0x1120|(MAX_RATES+1),SPEC_RATE},
  //上上月正反向无功、1-4象限无功电能
  {0x9910,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTS,1,4,0x1120|(MAX_RATES+1),SPEC_RATE},
  {0x9930,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,1,4,0x1140|(MAX_RATES+1),SPEC_RATE},
  
  {0xE701,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,1,4,0x11E3,SPEC_PHASE_ENERGY},//分相电能,正有
  {0xE704,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTS,1,4,0x11E3,SPEC_PHASE_ENERGY},//分相电能,正无
  {0xE707,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,1,4,0x11E3,SPEC_PHASE_ENERGY},//分相电能,反有
  {0xE70A,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTS,1,4,0x11E3,SPEC_PHASE_ENERGY},//分相电能,反无
  
  //铜损
  {0x9A00,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Cur_Energy.CopperLoss[0],0,4,S_INTU,0,4,1,SPEC_NO},//总铜损
  {0x9A01,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Cur_Energy.CopperLoss[1],0,4,S_INTU,0,4,1,SPEC_NO},//A相铜损
  {0x9A02,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Cur_Energy.CopperLoss[2],0,4,S_INTU,0,4,1,SPEC_NO},//B相铜损
  {0x9A03,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Cur_Energy.CopperLoss[3],0,4,S_INTU,0,4,1,SPEC_NO},//C相铜损
  //铁损
  {0x9B00,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Cur_Energy.IronLoss[0],0,4,S_INTU,0,4,1,SPEC_NO},//总铁损
  {0x9B01,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Cur_Energy.IronLoss[1],0,4,S_INTU,0,4,1,SPEC_NO},//A相铁损
  {0x9B02,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Cur_Energy.IronLoss[2],0,4,S_INTU,0,4,1,SPEC_NO},//B相铁损
  {0x9B03,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Cur_Energy.IronLoss[3],0,4,S_INTU,0,4,1,SPEC_NO},//C相铁损
  //安培时
  {0x9C00,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Cur_Energy.TransErr[0],0,4,S_INTU,0,4,1,SPEC_NO},//总铁损
  {0x9C01,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Cur_Energy.TransErr[1],0,4,S_INTU,0,4,1,SPEC_NO},//A相铁损
  {0x9C02,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Cur_Energy.TransErr[2],0,4,S_INTU,0,4,1,SPEC_NO},//B相铁损
  {0x9C03,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Cur_Energy.TransErr[3],0,4,S_INTU,0,4,1,SPEC_NO},//C相铁损
  
  //正反向有功最大需量
  {0xA010,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,0,4,0x1120|(MAX_RATES+1),SPEC_RATE},
  //正反向无功、1-4象限无功最大需量
  {0xA110,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,0,4,0x1160|(MAX_RATES+1),SPEC_RATE},
  //上月正反向有功最大需量
  {0xA410,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,0,4,0x1120|(MAX_RATES+1),SPEC_RATE},
  //上月正反向无功、1-4象限无功最大需量
  {0xA510,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,0,4,0x1160|(MAX_RATES+1),SPEC_RATE},
  //上上月正反向有功最大需量
  {0xA810,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,0,4,0x1120|(MAX_RATES+1),SPEC_RATE},
  //上上月正反向无功、1-4象限无功最大需量
  {0xA910,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,0,4,0x1160|(MAX_RATES+1),SPEC_RATE},
 
  //正反向有功最大需量发生时间 
  {0xB010,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,0,4,0x1120|(MAX_RATES+1),SPEC_RATE},
  //正反向无功、1-4象限无功最大需量发生时间
  {0xB110,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,0,4,0x1160|(MAX_RATES+1),SPEC_RATE},
  //上月正反向有功最大需量发生时间  
  {0xB410,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,0,4,0x1120|(MAX_RATES+1),SPEC_RATE},
  //上月正反向无功、1-4象限无功最大需量发生时间
  {0xB510,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,0,4,0x1160|(MAX_RATES+1),SPEC_RATE},
  //上上月正反向有功最大需量发生时间
  {0xB810,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,0,4,0x1120|(MAX_RATES+1),SPEC_RATE},
  //上上月正反向无功、1-4象限无功最大需量发生时间
  {0xB910,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,0,4,0x1160|(MAX_RATES+1),SPEC_RATE},
 
//	
  {0xB210,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Prog_Record.Time[0],0,4,S_BCD,0,4,1,SPEC_NO},//最近一次编程时间
  {0xB211,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Clr_Demand_Record.Time[0],0,4,S_BCD,0,4,1,SPEC_NO},//最近一次需量清0时间
  {0xB212,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Prog_Record.Counts,0,4,S_INTU,0,2,1,SPEC_NO},//编程次数
  {0xB213,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Clr_Demand_Record.Counts,0,4,S_INTU,0,2,1,SPEC_NO},//需量清0次数
  
  {0xB214,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1111,SPEC_BAT_TIME},//低功耗电池工作时间
//标识编码	数据格式	功能	单位	数据长度	数据项名称
  {0xB220,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,36,S_BCD,0,36,0x1115,SPEC_VOLSTAT},//B220		R	分	36	电压监测总累计数据 （注1）
  {0xE501,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,3,S_BCD,0,3,0x1113,SPEC_VOLSTAT},//B221		R	分	36	  本月电压监测数据（注1）
  {0xE511,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,3,S_BCD,0,3,0x1113,SPEC_VOLSTAT},//B222		R	分	36	上1月电压监测数据（注1）
  {0xE521,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,3,S_BCD,0,3,0x1113,SPEC_VOLSTAT},//B223		R	分	36	上2月电压监测数据（注1）
  {0xE531,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,3,S_BCD,0,3,0x1113,SPEC_VOLSTAT},//B224		R	分	36	上3月电压监测数据（注1）
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
/*
  //失压累计次数事件数据
  {0xB310,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Event_Cumu.Phase_Loss_Vol[0].Counts,0,4,S_INTU,0,2,1,SPEC_CUMU_EVENT},      //总以及A\B\C三相失压次数  
  {0xB311,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Event_Cumu.Phase_Loss_Vol[1].Counts,0,4,S_INTU,0,2,1,SPEC_CUMU_EVENT},      //总以及A\B\C三相失压次数 
  {0xB312,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Event_Cumu.Phase_Loss_Vol[2].Counts,0,4,S_INTU,0,2,1,SPEC_CUMU_EVENT},      //总以及A\B\C三相失压次数 
  {0xB313,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Event_Cumu.Phase_Loss_Vol[3].Counts,0,4,S_INTU,0,2,1,SPEC_CUMU_EVENT},      //总以及A\B\C三相失压次数
  //失压累计时间事件数据
  {0xB320,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Event_Cumu.Phase_Loss_Vol[0].Time,0,4,S_INTU,0,3,1,SPEC_CUMU_EVENT},      //总以及A\B\C三相失压失流累计时间 
  {0xB321,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Event_Cumu.Phase_Loss_Vol[1].Time,0,4,S_INTU,0,3,1,SPEC_CUMU_EVENT},      //总以及A\B\C三相失压失流累计时间 
  {0xB322,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Event_Cumu.Phase_Loss_Vol[2].Time,0,4,S_INTU,0,3,1,SPEC_CUMU_EVENT},      //总以及A\B\C三相失压失流累计时间 
  {0xB323,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Event_Cumu.Phase_Loss_Vol[3].Time,0,4,S_INTU,0,3,1,SPEC_CUMU_EVENT},      //总以及A\B\C三相失压失流累计时间 
  {0xB324,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Event_Cumu.Phase_Loss_Vol[8].Time,0,4,S_INTU,0,3,1,SPEC_CUMU_EVENT},      //总以及A\B\C三相失压失流累计时间
  //失流累计时间
  {0xB328,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Event_Cumu.Phase_Loss_Cur[0].Time,0,4,S_INTU,0,3,1,SPEC_CUMU_EVENT},      //总以及A\B\C三相失压失流累计时间 
  {0xB329,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Event_Cumu.Phase_Loss_Cur[1].Time,0,4,S_INTU,0,3,1,SPEC_CUMU_EVENT},      //总以及A\B\C三相失压失流累计时间 
  {0xB32A,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Event_Cumu.Phase_Loss_Cur[2].Time,0,4,S_INTU,0,3,1,SPEC_CUMU_EVENT},      //总以及A\B\C三相失压失流累计时间 
  {0xB32B,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Event_Cumu.Phase_Loss_Cur[3].Time,0,4,S_INTU,0,3,1,SPEC_CUMU_EVENT},      //总以及A\B\C三相失压失流累计时间
 */ 
  {0xB330,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_BCD,0,4,4,SPEC_NO},      //最近一次失压起始时刻,B330-B333进入块抄数据集
  {0xB334,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_BCD,0,4,5,SPEC_NO},      //最近一次失压起始时刻,B334-B338不进入块抄数据集  
  {0xB340,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_BCD,0,4,4,SPEC_NO},      //最近一次失压结束时刻,B340-B343进入块抄数据集
  {0xB344,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_BCD,0,4,5,SPEC_NO},      //最近一次失压结束时刻,B344-B348不进入块抄数据集              
   
//B347		4	月日时分	*		ABC相最近一次失压结束时刻
  {0xB611,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Volt.A,0,4,S_INTU,4,2,1,SPEC_NO},  //B611		2		*		A相电压
  {0xB612,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Volt.B,0,4,S_INTU,4,2,1,SPEC_NO},  //B612		2		*		B相电压
  {0xB613,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Volt.C,0,4,S_INTU,4,2,1,SPEC_NO},  //B613		2		*		C相电压
//B61F		6		*		电压数据打包
  {0xB621,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Curr.A,0,4,S_INTU,2,2,1,SPEC_NO},//B621	XX.XX	2	A	*		A相电流
  {0xB622,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Curr.B,0,4,S_INTU,2,2,1,SPEC_NO},//B622		2		*		B相电流
  {0xB623,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Curr.C,0,4,S_INTU,2,2,1,SPEC_NO},//B623		2		*		C相电流
//B62F		6		*		电流数据打包
  {0xB630,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.AcPower.Sum,0,4,S_INTU,2,3,1,SPEC_NO},//B630	XX.XXXX	3	kW	*		瞬时有功功率
  {0xB631,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.AcPower.A,0,4,S_INTU,2,3,1,SPEC_NO},//B631		3		*		A相有功功率
  {0xB632,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.AcPower.B,0,4,S_INTU,2,3,1,SPEC_NO},//B632		3		*		B相有功功率
  {0xB633,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.AcPower.C,0,4,S_INTU,2,3,1,SPEC_NO},//B633		3		*		C相有功功率
//B63F		12		*		有功功率数据打包
  {0xB640,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.ReacPower.Sum,0,4,S_INTU,4,2,1,SPEC_NO},//B640	XX.XX	2	kvar	*		瞬时无功功率
  {0xB641,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.ReacPower.A,0,4,S_INTU,4,2,1,SPEC_NO},//B641		2		*		A相无功功率
  {0xB642,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.ReacPower.B,0,4,S_INTU,4,2,1,SPEC_NO},//B642		2		*		B相无功功率
  {0xB643,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.ReacPower.C,0,4,S_INTU,4,2,1,SPEC_NO},//B643		2		*		C相无功功率
//B64F		8		*		无功功率数据打包
  {0xB650,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.PFactor.Sum,0,4,S_INTU,3,2,1,SPEC_NO},//B650		2		*		总功率因数
  {0xB651,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.PFactor.A,0,4,S_INTU,3,2,1,SPEC_NO},  //B651		2		*		A相功率因数
  {0xB652,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.PFactor.B,0,4,S_INTU,3,2,1,SPEC_NO},  //B652	X.XXX	2		*		B相功率因数
  {0xB653,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.PFactor.C,0,4,S_INTU,3,2,1,SPEC_NO},  //B653		2		*		C相功率因数
//B65F		8		*		功率因数数据打包
  {0xB660,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Freq.Sum,0,4,S_INTU,4,2,1,SPEC_NO},//B660	XX.XX	2	Hz	*		频率
  
  {0xB670,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Angle.Sum,0,4,S_INTU,1,2,1,SPEC_NO},//B670	XXX.X	2	度	*		相角
  {0xB671,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Angle.A,0,4,S_INTU,1,2,1,SPEC_NO},  //B671		2		*		A相相角
  {0xB672,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Angle.B,0,4,S_INTU,1,2,1,SPEC_NO},  //B672		2		*		B相相角
  {0xB673,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Angle.C,0,4,S_INTU,1,2,1,SPEC_NO},  //B673		2		*		C相相角
//B67F		8		*		相角数据打包
  {0xB680,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.AppPower.Sum,0,4,S_INTU,2,3,1,SPEC_NO},//B680		3		*		总视在功率
  {0xB681,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.AppPower.A,0,4,S_INTU,2,3,1,SPEC_NO},  //B681		3		*		A相视在功率
  {0xB682,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.AppPower.B,0,4,S_INTU,2,3,1,SPEC_NO},  //B682	XX.XXXX	3	kVA	*		B相视在功率
  {0xB683,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.AppPower.C,0,4,S_INTU,2,3,1,SPEC_NO},  //B683		3		*		C相视在功率
//B68F		12		*		视在功率数据打包
//B6FF		62		*		瞬时量数据打大包   
//						上述（2~9）所有量（除小包外）的集合
/*
  {0xB710,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,4,S_INTU,0,4,0x1117,SPEC_CUMU_EVENT},	
  {0xB720,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,4,S_INTU,0,4,0x1116,SPEC_CUMU_EVENT},
  {0xB730,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,4,S_INTU,0,4,0x1114,SPEC_CUMU_EVENT},
  {0xB740,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,4,S_INTU,0,4,0x1115,SPEC_CUMU_EVENT},
 */ 
  {0xB7A0,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,4,S_INTU,0,4,0x1111,SPEC_PSW_LOCK},
//
  {0xBE70,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Curr.Sum,0,4,S_INTU,2,2,1,SPEC_NO},//B621	XX.XX	2	A	*		A相电流

  {0xC010,DI_EQU_NULL,1,1,PSW_SET_DATE,S_RAM,&Cur_Time1.Week,0,4,S_BCD,0,4,0x1111,SPEC_DATE},//C010	YYMMDDWW	4	年月日周	*	*	日期及周次                                                                                                                                                                                            
  {0xC011,DI_EQU_NULL,1,1,PSW_SET_TIME,S_RAM,&Cur_Time1.Sec,0,3,S_BCD,0,3,0x1111,SPEC_TIME}, //C011	hhmmss	3	时分秒	*	*	时间                                                                                       
  //{0xC020,0xC040,1,1,PSW_RD_ONLY,S_RAM,&Meter_Run_Status.Stat1.Byte,0,1,S_BCD,0,1,1,SPEC_NO},//0xc020                                                      //C020	状态字	1		*		电表运行状态字                                                                                   
  //0xc021                                                      //C021		1		*		电网状态字                                                                                             
  //{0xC022,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},      //C030	NNNNNN	3	imp/kWh	*	*	有功脉冲常数          
  //0xc022                                                      //C022		1		*	*	周休日状态字                                                                                           
  //0xc025                                                      //C025		1		*	*	负荷曲线记录模式字              
  //0xc026                                                      //C026		1		*	*	电表运行状态字2                 
  //0xc027                                                      //C027		1		*	*	电表运行状态字3                 
  //0xc028                                                      //C028		1		*	*	电表运行状态字4                 
                                                                       
  {0xC030,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,1,SPEC_NO},      //C030	NNNNNN	3	imp/kWh	*	*	有功脉冲常数          
  {0xC031,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,1,SPEC_NO},      //C031	NNNNNN	3	imp/kvarh	*	*	无功脉冲常数        HK_PARA_NUM];
  {0xC032,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,6,S_BIN,0,6,1,SPEC_NO},      //C032	NNNNNNNNNNNN	6		*	*	表号                  
  {0xC033,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,6,S_BIN,0,6,1,SPEC_NO},      //C033	NNNNNNNNNNNN 	6		*	*	用户号                
  {0xC034,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,6,S_BIN,0,6,1,SPEC_NO},      //C034	NNNNNNNNNNNN	6		*	*	设备码                

  {0xC040,0xC020,1,1,PSW_RD_ONLY,S_RAM,&Meter_Run_Status.Meter_Stat1.Byte,0,1,S_BCD,0,1,1,SPEC_NO},//C040		1		*		电表运行状态字1        (见附录B1)                                                              
  {0xC041,0xC026,1,0,PSW_RD_ONLY,S_RAM,&Meter_Run_Status.Meter_Stat2.Byte,0,1,S_BCD,0,1,1,SPEC_NO},//C041		1		*		电表运行状态字2        (见附录B2)                                                              E
  {0xC042,0xC027,1,0,PSW_RD_ONLY,S_RAM,&Meter_Run_Status.Meter_Stat3.Byte,0,1,S_BCD,0,1,1,SPEC_NO},//C042		1		*		电表运行状态字3        (见附录B3)                                                              E
  {0xC043,0xC028,1,0,PSW_RD_ONLY,S_RAM,&Meter_Run_Status.Meter_Stat4.Byte,0,1,S_BCD,0,1,1,SPEC_NO},//C043		1		*		电表运行状态字4        (见附录B4)                                                              E
  {0xC044,0xC021,1,1,PSW_RD_ONLY,S_RAM,&Meter_Run_Status.Grid_Stat.Byte,0,1,S_BCD,0,1,1,SPEC_NO},//C044		1		*		电网状态字             （见附录B5）                                                            E
  {0xC045,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,&Meter_Run_Status.Meter_Stat6,0,1,S_BCD,0,1,1,SPEC_NO},//C045		1		*		错误状态字             （见附录B6）                                                            
  {0xC046,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,1,S_BCD,0,1,0x1111,SPEC_BAUD},//C045		1		*		错误状态字             （见附录B6）                                                            

//C046		1		*		波特率状态字           （见附录B7）                                                            #endif
  {0xC047,DI_EQU_NULL,1,1,PSW_FACTORY_STATUS,S_ROM,(void *)0,0,1,S_BIN,0,1,1,SPEC_NO},      //C047		1		*	*	模式字1(安全)          （见附录B11）                                                           
  {0xC048,DI_EQU_NULL,1,1,PSW_FACTORY_STATUS,S_ROM,(void *)0,0,1,S_BIN,0,1,1,SPEC_NO},      //C048		1		*	*	模式字2(功能)          （见附录B12）                                                           
  {0xC049,DI_EQU_NULL,1,1,PSW_FACTORY_STATUS,S_ROM,(void *)0,0,1,S_BIN,0,1,1,SPEC_NO},      //C049		1		*	*	模式字3(功能)          （见附录B13）                                                           
  {0xC04A,DI_EQU_NULL,1,1,PSW_SET_MODE45,S_ROM,(void *)0,0,1,S_BIN,0,1,1,SPEC_NO},       //C04A		1		*	*	模式字4(用户)          （见附录B14）                                                           
  {0xC04B,DI_EQU_NULL,1,1,PSW_SET_MODE45,S_ROM,(void *)0,0,1,S_BIN,0,1,1,SPEC_NO},       //C04B		1		*	*	模式字5(用户)          （见附录B15）                                                           
  {0xC04C,DI_EQU_NULL,1,1,PSW_SET_MODE678,S_ROM,(void *)0,0,1,S_BIN,0,1,1,SPEC_NO},      //C04C		1		*	*	模式字6(无功组合1)    （见附录B16）                                                            
  {0xC04D,DI_EQU_NULL,1,1,PSW_SET_MODE678,S_ROM,(void *)0,0,1,S_BIN,0,1,1,SPEC_NO},      //C04D		1		*	*	模式字7(无功组合2)    （见附录B16）                                                            
  {0xC04E,0xC025,1,0,PSW_SET_MODE678,S_ROM,(void *)0,0,1,S_BIN,0,1,1,SPEC_NO},      //C04E		1		*	*	模式字8(负荷曲线)      （见附录B17）                                                           

  {0xC111,0xC121,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},      //C111	NN	1	分	*	*	最大需量周期                  
  {0xC112,0xC122,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},      //C112	NN	1	分	*	*	滑差时间                      
  {0xC113,0xC123,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},      //C113	NN	1	秒	*	*	循显时间                      
  {0xC114,0xC124,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},      //C114	NN	1		*	*	循显项目数                      
  {0xC115,0xC125,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},      //C115	NN	1		*	*	显示电能小数位数                
  {0xC116,0xC126,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},      //C116	NN	1		*	*	显示功率（最大需量）小数位数    
  {0xC117,0xC127,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,2,S_BCD,0,2,1,SPEC_NO},      //C117	DDhh	2	月日	*	*	自动抄表日期              
  {0xC118,DI_EQU_NULL,1,0,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},      //C115	NN	1		*	*	显示电能小数位数                
  {0xC119,DI_EQU_NULL,1,0,PSW_SET_PARA,S_ROM,(void *)0,0,4,S_BCD,0,4,1,SPEC_NO},      //C116	NN	1		*	*	显示功率（最大需量）小数位数    
  {0xC11A,DI_EQU_NULL,1,0,PSW_SET_PARA,S_ROM,(void *)0,0,4,S_BCD,0,4,1,SPEC_NO},      //C117	DDhh	2	月日	*	*	自动抄表日期              

  //{0xC121,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C121	XX	1	分	*	*	最大需量周期                                                                               
  //{0xC122,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C122	…	1	分	*	*	滑差时间                                                                                   
  //{0xC123,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C123	…	1	秒	*	*	循显时间                                                                                   
  //{0xC124,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C124	XX	1		*	*	A屏显示项目数                                                                                
  //{0xC125,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C125	NN	1		*	*	电能[协议和显示]小数位[范围 ：2或3]                                                          
  //{0xC126,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C126	NN	1		*	*	功率&需量[显示]小数位                                                                        
//						[显示小数位为2或3或4；协议固定为4]                                                                 
  //{0xC127,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,2,S_BCD,0,2,1,SPEC_NO},         //C127	DDhh	2	日时	*	*	自动抄表日期                                                                           
  {0xC128,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,2,S_BCD,0,2,1,SPEC_NO},         //C128	NNNN	2		*	*	B屏显示项目数                                                                              
  {0xC129,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)&Meter_Self_Test,0,2,S_BCD,0,2,1,SPEC_NO},         //C129	NNNN	2		*		电表自检状态信息字（附录19）                                                               
  
  {0xC211,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,2,S_BCD,0,2,1,SPEC_NO},         //C211	NNNN	2	毫秒	*	 	输出脉冲宽度                                                                           
  {0xC213,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,12,S_BCD,0,12,1,SPEC_NO},       //C213	4组 XXXX.XX	12	V	*	*	电压合格率参数（注1）                                                              
  {0xC214,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C214	NN	1		*	*	用户级别，1或2或3（注2）                                                                     
  {0xC215,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C215	NN	1	分	*	*	跳闸延时                                                                                   
  {0xC216,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C216	NN	1	分	*	*	合闸延时                                                                                   
  {0xC217,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C217	XX	1				密码闭锁错误次数                                                                             
  
  {0xC310,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C310	NN	1		*	*	第1套年时区数P≤12                                                                           
  {0xC311,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C311		1		*	*	第1套日时段表数q≤8                                                                            
  {0xC312,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C312	…	1		*	*	第1套日时段数m≤14                                                                           
  {0xC313,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C313		1		*	*	第1套费率数k≤8                                                                                
  {0xC314,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C314		1		*	*	第1套公共假日数≤90                                                                            
  {0xC315,0xC41E,1,0,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C315		1		*	*	第1套周休日采用的时段表号                                                                      
  {0xC316,0xC022,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BIN,0,1,1,SPEC_NO},         //C316		1		*	*	第1套周休日状态字（见附录B8）                                                                  
  
  {0xC321,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_YEAR_PERIODS,SPEC_YEAR_PERIOD},         //C321	MMDDNN	3	月日时段表	*	*	第1时区               （第1套）                                                  
  {0xC32F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_YEAR_PERIODS,S_BCD,0,3*MAX_YEAR_PERIODS,1,SPEC_NO},        //C331	hhmmNN	3	时分费率	*	*	第1时段表第1时段      （第1套）                                                  
  
  {0xC331,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //C331	hhmmNN	3	时分费率	*	*	第1时段表第1时段      （第1套）                                                  
  {0xC341,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //C341	hhmmNN	3	时分费率	*	*	第2时段表第1时段      （第1套）                                                  
  {0xC351,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //C351	hhmmNN	3	时分费率	*	*	第3时段表第1时段      （第1套）                                                  
  {0xC361,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //C361	hhmmNN	3	时分费率	*	*	第4时段表第1时段      （第1套）                                                  
  {0xC371,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //C37F	hhmmNN	3*14	时分费率	*	*	第5时段表块           （第1套）                                              
  {0xC381,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //C38F						第6时段表块                                                                                    
  {0xC391,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //C39F	…					第7时段表块                                                                                  
  {0xC3A1,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //C3AF						第8时段表块                                                                                    
  {0xC3B1,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //C3BF	
  {0xC3C1,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //C3CF	
  {0xC3D1,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //C3DF	
  {0xC3E1,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //C3EF	

  {0xC33F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C331	hhmmNN	3	时分费率	*	*	第1时段表第1时段      （第1套）                                                  
  {0xC34F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C341	hhmmNN	3	时分费率	*	*	第2时段表第1时段      （第1套）                                                  
  {0xC35F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C351	hhmmNN	3	时分费率	*	*	第3时段表第1时段      （第1套）                                                  
  {0xC36F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C361	hhmmNN	3	时分费率	*	*	第4时段表第1时段      （第1套）                                                  
  {0xC37F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C37F	hhmmNN	3*14	时分费率	*	*	第5时段表块           （第1套）                                              
  {0xC38F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C38F						第6时段表块                                                                                    
  {0xC39F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C39F	…					第7时段表块                                                                                  
  {0xC3AF,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C3AF						第8时段表块                                                                                    
  {0xC3BF,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C3BF	
  {0xC3CF,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C3CF	
  {0xC3DF,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C3DF	
  {0xC3EF,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C3EF	

  {0xC411,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x111D,SPEC_HOLIDAY},//节假日特殊处理

  {0xC41F,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,45,S_BCD,0,45,1,SPEC_NO},         //C411	MMDDNN	3		*	*	第1套第1个公共假日                                                                       
  //{0xC41E,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C411	MMDDNN	3		*	*	第1套第1个公共假日                                                                       
  {0xC42F,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,45,S_BCD,0,45,1,SPEC_NO},                                                                         
  {0xC43F,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,45,S_BCD,0,45,1,SPEC_NO},                                                                            
  {0xC44F,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,45,S_BCD,0,45,1,SPEC_NO},                                                                           
  {0xC45F,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,45,S_BCD,0,45,1,SPEC_NO},                                                                           
  {0xC46F,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,45,S_BCD,0,45,1,SPEC_NO},                                                                             
  
  {0xC510,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,4,S_BCD,0,4,1,SPEC_NO},         //C510	MMDDhhmm	4	月日时分	*	*	负荷记录起始时间                                                                              
  {0xC511,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,2,S_BCD,0,2,1,SPEC_NO},         //C511	Mmmm	2	分	*	*	负荷曲线1类数据记录间隔                                                                  
  {0xC512,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,2,S_BCD,0,2,1,SPEC_NO},         //C512	…	…	分	*	*	负荷曲线2类数据记录间隔                                                                  
  {0xC513,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,2,S_BCD,0,2,1,SPEC_NO},         //C513	…	…	分	*	*	负荷曲线3类数据记录间隔                                                                                                                                        
  {0xC514,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,2,S_BCD,0,2,1,SPEC_NO},         //C514	…	…	分	*	*	负荷曲线4类数据记录间隔                                                                                                                                             
  {0xC515,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,2,S_BCD,0,2,1,SPEC_NO},         //C515	…	…	分	*	*	负荷曲线5类数据记录间隔                                                                                                                                    
  {0xC516,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,2,S_BCD,0,2,1,SPEC_NO},         //C516	…	…	分	*	*	负荷曲线6类数据记录间隔                                                                                                                                          
  
  {0xC51A,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,2,S_BCD,0,2,1,SPEC_NO},         //C51A	NNNN	2	分	*	*	编程有效时间                                                                                                                                                        
  {0xC51B,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C51B	NN	1	秒	*	*	失压判断时间                                                                                                                                                 
  {0xC51C,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C51C	NN	1	秒	*	*	失流判断时间                                                                                 
  {0xC51D,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //C51D	NN	1	秒	*	*	断相判断时间                                                                                                                                            
                                                                             
  {0xC610,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},        //C610	 NNNNNNNN,NNNNNNNN,NN	9		*	*	第01项显示项目      （A屏）                                                
  {0xC620,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},
  {0xC630,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},
  {0xC640,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},
  
  {0xC650,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},        //C650	 NNNNNNNN,NNNNNNNN,NN	9		*	*	第01项显示项目      （B屏）                                                                            
  {0xC660,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},                                               
  {0xC670,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},                                               
  {0xC680,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},                                               
  {0xC690,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},                                               
  {0xC6A0,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},                                               
  {0xC6B0,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},                                               
  {0xC6C0,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},
  {0xC6D0,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},
  {0xC6E0,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,9,S_BIN,0,9,15,SPEC_NO},

  {0xC712,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)Comm_Proto_Version,0,45,S_BCD,0,45,1,SPEC_NO},       //通讯规约版本号(参见企标QG/OKRW013－2005,  QG/OKRW004.7-2004) 
  {0xC713,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //读电表是否处于工厂状态，写退出工厂状态

  {0xC91F,DI_EQU_NULL,0,0,PSW_SET_ENSTART,S_ROM,(void *)0,0,36,S_BCD,0,36,0x1111,SPEC_SET_ENSTART},   //C91F	块中每个数据格式为：	4*9	kWh	*	*	正有功初始电量块  （总和8个分费率）                                                                             
  {0xC92F,DI_EQU_NULL,0,0,PSW_SET_ENSTART,S_ROM,(void *)0,0,36,S_BCD,0,36,0x1111,SPEC_SET_ENSTART},   //C92F	XXXXXX.XX或XXXXX.XXX			*	*	反有功初始电量块                                                           
  {0xC93F,DI_EQU_NULL,0,0,PSW_SET_ENSTART,S_ROM,(void *)0,0,36,S_BCD,0,36,0x1111,SPEC_SET_ENSTART},   //C93F		…		*	*	1象限无功初始电量块                                                                          
  {0xC94F,DI_EQU_NULL,0,0,PSW_SET_ENSTART,S_ROM,(void *)0,0,36,S_BCD,0,36,0x1111,SPEC_SET_ENSTART},   //C94F				*	*	2象限无功初始电量块                                                                            
  {0xC95F,DI_EQU_NULL,0,0,PSW_SET_ENSTART,S_ROM,(void *)0,0,36,S_BCD,0,36,0x1111,SPEC_SET_ENSTART},   //C95F		…		*	*	3象限无功初始电量块                                                                          
  {0xC96F,DI_EQU_NULL,0,0,PSW_SET_ENSTART,S_ROM,(void *)0,0,36,S_BCD,0,36,0x1111,SPEC_SET_ENSTART},   //C96F				*	*	"4象限无功初始电量块(沈阳、辽阳MB3无初始电量设置；                                             

  {0xCA10,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,4,S_BCD,0,4,1,SPEC_NO},         //CA10	NN, XX.XXXX	4	%,A	*	*	失压判定阈值（注4）                                             
  {0xCA11,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,4,S_BCD,0,4,1,SPEC_NO},         //CA11	NN, XX.XXXX	4	%,A	*	*	失流判定阈值（注5）                                                                
  {0xCA12,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BIN,0,1,1,SPEC_NO},         //CA12	NN	1		*	*	三合一信号输出控制字(见附录18)                                                               
  {0xCA13,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,5,S_BIN,0,5,1,SPEC_NO},         //CA13	YYMMDDhhmm	5	年月日时分	*	*	主副费率切换时刻                                                           
  {0xCA14,DI_EQU_NULL,1,1,PSW_SET_PARA,S_RAM,(void *)&Rate_Status,0,2,S_BCD,0,2,1,SPEC_NO},         //CA14	NN,NN	2		*		当前套费率,执行费率(见附录B9)                                                              
  {0xCA15,DI_EQU_NULL,1,1,PSW_FACTORY_STATUS,S_ROM,(void *)0,0,6,S_BIN,0,6,1,SPEC_NO},         //CA15	NN, … ,NN	6		*	*	用户权限控制字（见附录B10）                                                          
  {0xCA16,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,1,SPEC_NO},         //CA16	XX.XXXX	3	kW	*	*	有功需量限额                                                                           
  {0xCA17,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,1,SPEC_NO},         //CA17	XX.XXXX	3	kVA	*	*	视在功率限额（保留，暂时没用）                                                         
  {0xCA18,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //CA18	NN	1		*	*	打包费率数（保留，暂时没用）                                                                 
  {0xCA19,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,1,SPEC_NO},         //过流判定阀值
  {0xCA1A,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BIN,0,1,1,SPEC_NO},         //CA1A	NN	1		*	*	模式字9（b0－1：秒脉冲、需量周期合时段切换3合1输出；b0－0秒脉冲、需量周期合时段切换单独输出）
  //{0xCA1B,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //CA1B	NN	1		*		内卡状态1－有故障，0－正常（b0－内卡1，b1－内卡2，b2－－内卡3，b3－－内卡4）                 

  {0xCB10,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //CB10	NN	1		*	*	第2套年时区数P≤12                                                                           
  {0xCB11,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //CB11		1		*	*	第2套日时段表数q≤8                                                                            
  {0xCB12,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //CB12	…	1		*	*	第2套日时段数m≤14                                                                           
  {0xCB13,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //CB13		1		*	*	第2套费率数k≤8                                                                                
  {0xCB14,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //CB14		1		*	*	第2套公共假日数≤90                                                                            
  {0xCB15,0xCC1E,1,0,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //CB15		1		*	*	第2套周休日采用的时段表号                                                                      
  {0xCB16,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BIN,0,1,1,SPEC_NO},         //CB16		1		*	*	第2套周休日状态字（见附录B8）                                                                  
  
  {0xCB21,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_YEAR_PERIODS,SPEC_YEAR_PERIOD},         //CB21	MMDDNN	3	月日时段表	*	*	第1时区               （第2套）                                                                                                               
  {0xCB2F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_YEAR_PERIODS,S_BCD,0,3*MAX_YEAR_PERIODS,1,SPEC_NO},        //C331	hhmmNN	3	时分费率	*	*	第1时段表第1时段      （第1套）                                                  
  
  {0xCB31,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //CB31	hhmmNN	3	时分费率	*	*	第1时段表第1时段      （第2套）                                                  
  {0xCB41,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //CB41	hhmmNN	3	时分费率	*	*	第2时段表第1时段      （第2套）                                                  
  {0xCB51,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //CB51	hhmmNN	3	时分费率	*	*	第3时段表第1时段      （第2套）                                                  
  {0xCB61,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //CB61	hhmmNN	3	时分费率	*	*	第4时段表第1时段      （第2套）                                                  
  {0xCB71,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //CB7F	hhmmNN	3*14	时分费率	*	*	第5时段表块           （第2套）                                              
  {0xCB81,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //CB8F						第6时段表块                                                                                    
  {0xCB91,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //CB9F	…					第7时段表块                                                                                  
  {0xCBA1,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //CBAF						第8时段表块                                                                                    
  {0xCBB1,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //CBBF	
  {0xCBC1,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //CBCF	
  {0xCBD1,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //CBDF	
  {0xCBE1,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x1110|MAX_DATE_PERIODS,SPEC_DATE_PERIOD},        //CBEF                                                                 

  {0xCB3F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C331	hhmmNN	3	时分费率	*	*	第1时段表第1时段      （第1套）                                                  
  {0xCB4F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C341	hhmmNN	3	时分费率	*	*	第2时段表第1时段      （第1套）                                                  
  {0xCB5F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C351	hhmmNN	3	时分费率	*	*	第3时段表第1时段      （第1套）                                                  
  {0xCB6F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C361	hhmmNN	3	时分费率	*	*	第4时段表第1时段      （第1套）                                                  
  {0xCB7F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C37F	hhmmNN	3*14	时分费率	*	*	第5时段表块           （第1套）                                              
  {0xCB8F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C38F						第6时段表块                                                                                    
  {0xCB9F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C39F	…					第7时段表块                                                                                  
  {0xCBAF,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C3AF						第8时段表块                                                                                    
  {0xCBBF,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C3BF	
  {0xCBCF,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C3CF	
  {0xCBDF,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C3DF	
  {0xCBEF,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,3*MAX_DATE_PERIODS,S_BCD,0,3*MAX_DATE_PERIODS,1,SPEC_NO},        //C3EF	
  
  {0xCC11,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,3,S_BCD,0,3,0x111D,SPEC_HOLIDAY},
  
  {0xCC1F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,45,S_BCD,0,45,1,SPEC_NO},         //CC11	MMDDNN	3		*	*	第2套第1个公共假日                                                                       
  //{0xCC1E,DI_EQU_NULL,1,1,PSW_SET_PARA,S_ROM,(void *)0,0,1,S_BCD,0,1,1,SPEC_NO},         //CC11	MMDDNN	3		*	*	第2套第1个公共假日                                                                       
  {0xCC2F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,45,S_BCD,0,45,1,SPEC_NO},                                                                         
  {0xCC3F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,45,S_BCD,0,45,1,SPEC_NO},                                                                            
  {0xCC4F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,45,S_BCD,0,45,1,SPEC_NO},                                                                           
  {0xCC5F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,45,S_BCD,0,45,1,SPEC_NO},                                                                           
  {0xCC6F,DI_EQU_NULL,0,0,PSW_SET_PARA,S_ROM,(void *)0,0,45,S_BCD,0,45,1,SPEC_NO}, 

  {0xCDE0,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_ROM,(void *)0,0,1,S_BIN,0,1,0x1111,SPEC_RD_ROM},
  
//14个校表参数
  {0xCEE0,DI_EQU_NULL,1,1,PSW_FACTORY_STATUS,S_ROM,(void *)0,0,3,S_BIN,0,3,1,SPEC_NO},
  {0xCEE1,DI_EQU_NULL,1,1,PSW_FACTORY_STATUS,S_ROM,(void *)0,0,1,S_BIN,0,1,1,SPEC_NO},
  {0xCEE2,DI_EQU_NULL,1,1,PSW_FACTORY_STATUS,S_ROM,(void *)0,0,3,S_BIN,0,3,12,SPEC_NO},
  {0xCEEE,DI_EQU_NULL,0,0,PSW_FACTORY_STATUS,S_ROM,(void *)0,0,1,S_BIN,0,1,0x1111,SPEC_ADJ_CLR},

//校表参数结束

  {0xD110,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_ROM,(void *)0,0,4,S_INTU,0,4,0x1116,SPEC_LOAD_DATA},//负荷曲线
  
//{0xEF00,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,(void *)0,0,122,S_BCD,0,122,0x1111,SPEC_FREEZE_DATA},//注意:EF00和EF02是在Get_Freeze_Data函数中单独处理的
  {0xEF01,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_ROM,(void *)0,0,127,S_BCD,0,127,1,SPEC_NO},
  //{0xEF02,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,(void *)0,0,144,S_BCD,0,144,0x1111,SPEC_FREEZE_DATA},
  {0xEF03,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_ROM,(void *)0,0,149,S_BCD,0,149,1,SPEC_NO}, 
  
//////////
  ////////////////////扩展的标示字//////////////
  //显示任务需要的变量
  {DI_REG_CKC,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&CKC,0,1,S_BCD,0,1,1,SPEC_NO},//扩充失压/断相，DI:DI_LOW_VOL                                                            
  {DI_REG_CSC,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&CSC,0,1,S_BCD,0,1,1,SPEC_NO},//扩充失压/断相，DI:DI_LOW_VOL                                                            
  
  {DI_LOW_VOL,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Meter_Run_Status.Volt_LossMiss.Byte,0,1,S_BCD,0,1,1,SPEC_NO},//扩充失压/断相，DI:DI_LOW_VOL                                                            
  {DI_CURRENT_QUADRANT,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Meter_Run_Status.NegCurr_Qudrant.Byte,0,1,S_BCD,0,1,1,SPEC_NO},//扩充电流反极性/电表工作象限，DI_CURRENT_QUADRANT                                                            
  {DI_METER_PERIPHERY,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Meter_Run_Status.Key_Event.Byte,0,1,S_BCD,0,1,1,SPEC_NO},//扩充编程键打开/跳线短接/需量复位/电表上锁，DI:DI_METER_ PERIPHERY                                                      
  {DI_MODULE_STATUS,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Meter_Run_Status.Comm_Status.Byte,0,1,S_BCD,0,1,1,SPEC_NO},//扩充通信状态字，DI:DI_MODULE_STATUS
  {DI_POWER_DOWN_TIME,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,(volatile void *)&No_Var[0],0,5,S_BCD,0,5,1,SPEC_NO},

  {DI_METER_ADDR0,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_ROM,(void *)0,0,6,S_BCD,0,6,SPEC_NO},//第一路表地址
  {DI_METER_ADDR1,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_ROM,(void *)0,0,6,S_BCD,0,6,SPEC_NO},//第二路表地址
  {DI_METER_ADDR2,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_ROM,(void *)0,0,6,S_BCD,0,6,SPEC_NO},//第三路表地址

  {DI_COMM_BAUD0,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_ROM,(void *)0,0,1,S_BCD,0,1,SPEC_NO},//第一路波特率
  {DI_COMM_BAUD1,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_ROM,(void *)0,0,1,S_BCD,0,1,SPEC_NO},//第二路波特率
  {DI_COMM_BAUD2,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_ROM,(void *)0,0,1,S_BCD,0,1,SPEC_NO},//第三路波特率

  {DI_METER_SOFTWARE_VERSION,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,(volatile void *)&Software_Version,0,5,S_BCD,0,5,1,SPEC_NO},//软件版本号
  {DI_METER_SPECIFI,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,(volatile void *)&MeasuSysMode.ParseMode,0,5,S_BCD,0,5,1,SPEC_NO},                                                                                                                           //                                                               E
  //有无功方向，负荷曲线使用该数据
  {DI_ACTIVE_DIRECT,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,(volatile void *)&Meter_Run_Status.Ext_Ac_Power_Dir,0,1,S_BCD,0,1,1,SPEC_NO},
  {DI_REACTIVE_DIRECT,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,(volatile void *)&Meter_Run_Status.Ext_Reac_Power_Dir,0,1,S_BCD,0,1,1,SPEC_NO},
  //总以及三相无功功率,负荷曲线使用该数据
  {DI_REACTIVE_ALL,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.ReacPower.Sum,0,4,S_INTU,2,3,1,SPEC_NO},//B630	XX.XXXX	3	kW	*		瞬时有功功率
  {DI_REACTIVE_A,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.ReacPower.A,0,4,S_INTU,2,3,1,SPEC_NO},//B631		3		*		A相有功功率
  {DI_REACTIVE_B,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.ReacPower.B,0,4,S_INTU,2,3,1,SPEC_NO},//B632		3		*		B相有功功率
  {DI_REACTIVE_C,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.ReacPower.C,0,4,S_INTU,2,3,1,SPEC_NO},//B633		3		*		C相有功功率
  //有无功需量，负荷曲线使用该数据
  {DI_ACTIVE_DEMAND,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Demand_Accu.Cur_Pos_Active,0,4,S_INTU,0,3,1,SPEC_NO},//B631		3		*		A相有功功率
  {DI_REACTIVE_DEMAND,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Demand_Accu.Cur_Pos_Reactive,0,4,S_INTU,0,3,1,SPEC_NO},//B632		3		*		B相有功功率

  {DI_CUR_TIME,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,(volatile void *)&No_Var[0],0,5,S_BCD,0,5,0x1111,SPEC_CUR_TIME},//当前时间 分
  {DI_EVENT_TIME,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,(volatile void *)&No_Var[0],0,5,S_BCD,0,5,0x1111,SPEC_EVENT_TIME},//当前时间 分

  //A、B、C三相AH
  {_DI_TOTAL_AH,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Cur_Energy.Phase_AH[0],0,4,S_INTU,0,4,1,SPEC_NO},//A相总AH  
  {_DI_A_AH,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Cur_Energy.Phase_AH[1],0,4,S_INTU,0,4,1,SPEC_NO},//A相总AH
  {_DI_B_AH,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Cur_Energy.Phase_AH[2],0,4,S_INTU,0,4,1,SPEC_NO},//B相总AH
  {_DI_C_AH,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Cur_Energy.Phase_AH[3],0,4,S_INTU,0,4,1,SPEC_NO},//C相总AH
  
  //A、B、C三相AH
  {DI_TOTAL_AH,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Cur_Energy.Phase_AH[0],0,4,S_INTU,1,4,1,SPEC_NO},//A相总AH  
  {DI_A_AH,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Cur_Energy.Phase_AH[1],0,4,S_INTU,1,4,1,SPEC_NO},//A相总AH
  {DI_B_AH,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Cur_Energy.Phase_AH[2],0,4,S_INTU,1,4,1,SPEC_NO},//B相总AH
  {DI_C_AH,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Cur_Energy.Phase_AH[3],0,4,S_INTU,1,4,1,SPEC_NO},//C相总AH
  
  //A、B、C三相电压
  {DI_A_VOL,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Volt.A,0,4,S_INTU,2,3,1,SPEC_NO},//A相电压,2小数
  {DI_B_VOL,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Volt.B,0,4,S_INTU,2,3,1,SPEC_NO},//B相总AH,2小数
  {DI_C_VOL,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Volt.C,0,4,S_INTU,2,3,1,SPEC_NO},//C相总AH,2小数
  {DI_EVENT_VOL,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Event_Vol_Cur.Pub_Vol,0,4,S_INTU,2,3,1,SPEC_NO},//发生过压或电压超限时的电压值

  {DI_A_VOL_1,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Volt.A,0,4,S_INTU,3,2,1,SPEC_NO},//A相电压,1小数
  {DI_B_VOL_1,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Volt.B,0,4,S_INTU,3,2,1,SPEC_NO},//B相总AH,1小数
  {DI_C_VOL_1,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Volt.C,0,4,S_INTU,3,2,1,SPEC_NO},//C相总AH,1小数

  {DI_A_CUR,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Curr.A,0,4,S_INTU,0,3,1,SPEC_NO},//B621	XX.XX	2	A	*		A相电流
  {DI_B_CUR,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Curr.B,0,4,S_INTU,0,3,1,SPEC_NO},//B622		2		*		B相电流
  {DI_C_CUR,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Curr.C,0,4,S_INTU,0,3,1,SPEC_NO},//B623		2		*		C相电流

  {DI_A_CUR_3,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Curr.A,0,4,S_INTU,1,3,1,SPEC_NO},//B621	XX.XX	2	A	*		A相电流
  {DI_B_CUR_3,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Curr.B,0,4,S_INTU,1,3,1,SPEC_NO},//B622		2		*		B相电流
  {DI_C_CUR_3,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Curr.C,0,4,S_INTU,1,3,1,SPEC_NO},//B623		2		*		C相电流


  {DI_A_ANGLE,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Angle.A,0,4,S_INTU,0,3,1,SPEC_NO},  //B671  	2		*		A相相角
  {DI_B_ANGLE,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Angle.B,0,4,S_INTU,0,3,1,SPEC_NO},  //B672		2		*		B相相角
  {DI_C_ANGLE,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Angle.C,0,4,S_INTU,0,3,1,SPEC_NO},  //B673		2		*		C相相角
  {DI_SUM_ANGLE,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Measu_InstantData_ToPub_PUCK.Angle.Sum,0,4,S_INTU,0,3,1,SPEC_NO},  //总相角

//当前需量
  {DI_CUR_DEMAND,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Demand_Accu.Cur_Pos_Active,0,4,S_INTU,0,3,1,SPEC_NO},//当前需量
  //最近一次清需量方式
  {DI_CLR_DEMAND_WAY,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Clear_Demand_Way,0,1,S_BIN,0,1,1,SPEC_NO},//清需量方式
  {DI_ADJ_FLAG,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,(volatile void *)&No_Var[0],0,1,S_BIN,0,1,1,SPEC_NO},//校表标识字
  //两个编程标识字
  {DI_PROG_FLAG0,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Event_Cumu_Data.Prog_Flag.Flag[0],0,1,S_BIN,0,1,1,SPEC_NO},//编程标识字1
  {DI_PROG_FLAG1,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Event_Cumu_Data.Prog_Flag.Flag[1],0,1,S_BIN,0,1,1,SPEC_NO},//编程标识字2
  {DI_PROG_TIME,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Prog_Record.Time[0],0,5,S_BCD,0,5,1,SPEC_NO},//编程起始时间
  //最近一次的错误时间
  {DI_ERR_TIME,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,Err_Time,0,5,S_BCD,0,5,1,SPEC_NO},//时钟纠错前的时钟数据
  {DI_ERR_ROM,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&Rom_Err_Flag,0,1,S_BCD,0,1,1,SPEC_NO},//读写ROM错误
  {DI_NULL_1BYTE,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,(volatile void *)&No_Var[0],0,1,S_BCD,0,1,1,SPEC_NO},//标识字，无意义
  
  {DI_ALL_LOSS_VOL_START_TIME,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&All_Loss_Vol_Time.Start_Time,0,5,S_BCD,0,5,1,SPEC_NO},//读写ROM错误
  {DI_ALL_LOSS_VOL_END_TIME,DI_EQU_NULL,0,0,PSW_RD_ONLY,S_RAM,&All_Loss_Vol_Time.End_Time,0,5,S_BCD,0,5,1,SPEC_NO},//标识字，无意义  
  ////////////////////扩展的标示字--end//////////////////
};
CONST INT8U BROAD_ADDR_99[]={0x99,0x99,0x99,0x99,0x99,0x99};
CONST INT8U BROAD_ADDR_AA[]={0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};

//初始化数据传输相关的内存变量
void Init_Data_Trans_Ram_Para()
{
  //参数修改缓冲区
  mem_set((void *)&Para_Modify,0,sizeof(Para_Modify),(void *)&Para_Modify,sizeof(Para_Modify));
  INIT_STRUCT_VAR(Para_Modify);
  SET_STRUCT_SUM(Para_Modify);
  
  INIT_STRUCT_VAR(User_Authority_Flag);
  User_Authority_Flag.Flag=0;
  SET_STRUCT_SUM(User_Authority_Flag);
  
  INIT_STRUCT_VAR(Clear_Flag);
  Clear_Flag.Flag=CLEAR_END_FLAG;
  SET_STRUCT_SUM(Clear_Flag);
  
  INIT_STRUCT_VAR(PSW_Err_Info);
  PSW_Err_Info.Err_Times=0;
  PSW_Err_Info.Lock_Mins=0;
  SET_STRUCT_SUM(PSW_Err_Info);

  //INIT_STRUCT_VAR(Event_Cumu_Data.Prog_Flag);
  //Event_Cumu_Data.Prog_Flag.Flag[0]=0;
  //Event_Cumu_Data.Prog_Flag.Flag[1]=0;
  //SET_STRUCT_SUM(Prog_Flag);
  INIT_STRUCT_VAR(Time_Modify_Flag);
  Time_Modify_Flag.Var=0;
  
  INIT_STRUCT_VAR(Cur_PSW);
}

//检查协议传输相关参数和数据的正确性
void Check_Data_Trans_Data_Avail()
{
  INT8U Re;
  
  //清除标志正确？
  Re=1;
  Re&=CHECK_STRUCT_SUM(Clear_Flag);
  Re&=CHECK_STRUCT_VAR(Clear_Flag);
  if(0==Re)
  {
    ASSERT(A_WARNING,0);
    INIT_STRUCT_VAR(Clear_Flag);
    Clear_Flag.Flag=CLEAR_END_FLAG;
    SET_STRUCT_SUM(Clear_Flag);
  }
  
  //编程标志正确
  /*
  Re=1;
  Re&=CHECK_STRUCT_SUM(Prog_Flag);
  Re&=CHECK_STRUCT_VAR(Prog_Flag);
  if(0==Re)
  {
    ASSERT(A_WARNING,0);
    INIT_STRUCT_VAR(Prog_Flag);
    Prog_Flag.Flag[0]=0;
    Prog_Flag.Flag[1]=0;
    SET_STRUCT_SUM(Prog_Flag);    
  }
  */
  //用户权限标志正确?
  Re=1;
  Re&=CHECK_STRUCT_SUM(User_Authority_Flag);
  Re&=CHECK_STRUCT_VAR(User_Authority_Flag);
  if(0==Re)
  {
    ASSERT(A_WARNING,0);
    INIT_STRUCT_VAR(User_Authority_Flag);
    User_Authority_Flag.Flag=0;
    SET_STRUCT_SUM(User_Authority_Flag);    
  }
  
  //参数修改缓冲区发生了变化?
  Re=1;
  Re&=CHECK_STRUCT_SUM(Para_Modify);
  Re&=CHECK_STRUCT_VAR(Para_Modify);
  Re&=CHECK_STRUCT_SUM(Disp_Digs);
  Re&=CHECK_STRUCT_SUM(Meter_Addr);
  Re&=CHECK_STRUCT_VAR(Cur_PSW);
  Re&=CHECK_STRUCT_SUM(Cur_PSW);
  if(0==Re)
  {
    ASSERT(A_WARNING,0);
    Read_Data_Trans_Para_From_Rom();
  }
}

//读取数据转换相关参数
void Read_Data_Trans_Para_From_Rom()
{
  INT8U Err;
  //电能显示小数位数
  if(Read_Storage_Data(0xC115,(void *)(&Disp_Digs.Energy_Digs),(void *)(&Disp_Digs.Energy_Digs),1,&Err)!=1 || Err!=NO_ERR)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Read 0xC125 Para Err");
    Disp_Digs.Energy_Digs=3;
  }    
  Disp_Digs.Energy_Digs=Bcd2Hex_Byte(Disp_Digs.Energy_Digs);
  SET_STRUCT_SUM(Disp_Digs);
  
  //功率与需量显示小数位数
  if(Read_Storage_Data(0xC116,(void *)(&Disp_Digs.Power_Digs),(void *)(&Disp_Digs.Power_Digs),1,&Err)!=1 || Err!=NO_ERR)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Read 0xC126 Para Err");
    Disp_Digs.Power_Digs=3;
  }
  Disp_Digs.Power_Digs=Bcd2Hex_Byte(Disp_Digs.Power_Digs);
  SET_STRUCT_SUM(Disp_Digs);
  
  //用户权限控制字
  if(Read_Storage_Data(0xCA15,(void *)User_Authority_Ctrl.PSW,(void *)User_Authority_Ctrl.PSW,sizeof(User_Authority_Ctrl.PSW),&Err)!=6 || Err!=NO_ERR)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Read 0xCA15 Para Err");
    Read_Def_Para(0xCA15,(void *)User_Authority_Ctrl.PSW,6,(void *)User_Authority_Ctrl.PSW,sizeof(User_Authority_Ctrl.PSW));
  }
  SET_STRUCT_SUM(User_Authority_Ctrl);
  
  //读取清编程时间和次数
  if(Read_Storage_Data(PROG_RECORD,(void *)(&Prog_Record),(void *)(&Prog_Record),sizeof(Prog_Record),&Err)!=sizeof(Prog_Record) || Err!=NO_ERR)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Read 0x%lx Para Err",PROG_RECORD);
    Read_Def_Para(PROG_RECORD,(void *)(&Prog_Record),sizeof(Prog_Record),(void *)(&Prog_Record),sizeof(Prog_Record));
  }
  SET_STRUCT_SUM(Prog_Record); 

  //读取清需量时间和次数
  if(Read_Storage_Data(CLR_DEMAND_RECORD,(void *)(&Clr_Demand_Record),(void *)(&Clr_Demand_Record),sizeof(Clr_Demand_Record),&Err)!=sizeof(Clr_Demand_Record) || Err!=NO_ERR)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Read 0x%lx Para Err",CLR_DEMAND_RECORD);
    mem_set((void *)(&Clr_Demand_Record),0,sizeof(Clr_Demand_Record),(void *)(&Clr_Demand_Record),sizeof(Clr_Demand_Record));
  }
  SET_STRUCT_SUM(Clr_Demand_Record); 
  
  //读表地址0-2,读表地址不可用Get_DLT645_Data函数
  if(Read_Storage_Data(METER_ADDR0,(void *)(Meter_Addr.Addr[0]),(void *)(Meter_Addr.Addr[0]),sizeof(Meter_Addr.Addr[0]),&Err)!=sizeof(Meter_Addr.Addr[0]) || Err!=NO_ERR)
    DEBUG_PRINT(HUCK,DEBUG_0,"Read 0x%lx Para Err",METER_ADDR0); 
  if(Read_Storage_Data(METER_ADDR1,(void *)(Meter_Addr.Addr[1]),(void *)(Meter_Addr.Addr[1]),sizeof(Meter_Addr.Addr[1]),&Err)!=sizeof(Meter_Addr.Addr[1]) || Err!=NO_ERR)
    DEBUG_PRINT(HUCK,DEBUG_0,"Read 0x%lx Para Err",METER_ADDR1);
    SET_STRUCT_SUM(Meter_Addr);
  //读取密码参数  
  if(Read_Storage_Data(PASSWORD_0,(void *)Cur_PSW.PSW[0],(void *)Cur_PSW.PSW[0],sizeof(Cur_PSW.PSW[0]),&Err)!=sizeof(Cur_PSW.PSW[0]))
      DEBUG_PRINT(HUCK,DEBUG_0,"Read 0x%lx Para Err",PASSWORD_0);
  if(Read_Storage_Data(PASSWORD_1,(void *)Cur_PSW.PSW[1],(void *)Cur_PSW.PSW[1],sizeof(Cur_PSW.PSW[1]),&Err)!=sizeof(Cur_PSW.PSW[1]))
      DEBUG_PRINT(HUCK,DEBUG_0,"Read 0x%lx Para Err",PASSWORD_1);
  if(Read_Storage_Data(PASSWORD_2,(void *)Cur_PSW.PSW[2],(void *)Cur_PSW.PSW[2],sizeof(Cur_PSW.PSW[2]),&Err)!=sizeof(Cur_PSW.PSW[2]))
      DEBUG_PRINT(HUCK,DEBUG_0,"Read 0x%lx Para Err",PASSWORD_2);
  SET_STRUCT_SUM(Cur_PSW);

}

//获取电量数据的显示小数位数
INT8U Get_Energy_Disp_Digs()
{
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(Disp_Digs);
  ASSERT(A_WARNING,0!=Re);//此函数在Get_DLT_645函数中调用，不可进行Check_Data_Avail();
  
  if(Disp_Digs.Energy_Digs==3)
    return 3; 
  else
    return 2;
}

//获取功率数据的显示小数位数
INT8U Get_Power_Disp_Digs()
{
  //INT8U Re;
  
  //功率显示小数位数只能是2、3、4
  if(Disp_Digs.Power_Digs>=2 && Disp_Digs.Power_Digs<=4)
    return Disp_Digs.Power_Digs; 
  else
    return 3;//默认取3
}

/*
        标识字1：0位：设置电表常数
              1位：设置周休日状态
                  2位：设置表号、设备号、用户号
                  3位：设置最大需量周期及滑差步进时间
                  4位：设置循环显示参数：循环显示间隔，循环显示项目及总数
                  5位：设置电表的电池使用时间
                  6位：设置结算日
                  7位：其他参数
        标识字2： 0位：设置密码
                  1位：设置时段参数  //改为设置费率和时段参数!!!!!!!!!!
                  2位：设置负荷曲线记录参数 
                  3位：设置费率，剩余电费报警限额 //改为剩余电量和报警限额!!!!!
                  4位：设置功率限额
                  5位：设置跳合闸延时，用户级别
                  6位：设置电压合格率参数
*/
//根据DI值更新编程标识字
void Set_Pog_Flag(PROTO_DI DI)
{
  INT8U Re;
  
  Extend_Set_Prog_Flag(DI);
  
  Re=1;
  Re&=CHECK_STRUCT_SUM(Event_Cumu_Data.Prog_Flag);
  ASSERT(A_WARNING,1==Re);

  if(Read_Event_Status(ID_EVENT_PROG)==0)//当前没有编程事件发生，也就是不在一个编程的10分钟周期内
  {
    Event_Cumu_Data.Prog_Flag.Flag[0]=0;
    Event_Cumu_Data.Prog_Flag.Flag[1]=0;
    SET_STRUCT_SUM(Event_Cumu_Data.Prog_Flag);
    SET_STRUCT_SUM(Event_Cumu_Data);
    
    Set_Event_Instant(ID_EVENT_PROG);//编程事件
    
    //保存编程次数和最近一次编程时间
    Prog_Record.Time[0]=Cur_Time1.Min;
    Prog_Record.Time[1]=Cur_Time1.Hour;
    Prog_Record.Time[2]=Cur_Time1.Date;
    Prog_Record.Time[3]=Cur_Time1.Month;
    Prog_Record.Time[4]=Cur_Time1.Year;
    Prog_Record.Counts++;
    SET_STRUCT_SUM(Prog_Record);
    Write_Storage_Data(PROG_RECORD,(void *)&Prog_Record,sizeof(Prog_Record));
  }
  
  if(0xC030==DI || 0xC031==DI)
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[0],0);
  else if(0xC316==DI || 0xC022==DI || 0xCB16==DI)//周休日状态字
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[0],1);
  else if((0xC032<=DI && DI<=0xC034) ||\
          METER_ADDR0==DI || METER_ADDR1==DI || METER_ADDR2==DI)//表地址、表号、设备号、用户号
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[0],2);
  else if(0xC111==DI || 0xC112==DI || 0xC121==DI || 0xC122==DI)//需量周期和滑差时间
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[0],3);
  else if((0xC113<=DI && DI<=0xC116) || (0xC123<=DI && DI<=0xC126) || 0xC128==DI)//循环显示参数
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[0],4);
  else if(0xB214==DI)//电池使用时间
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[0],5);
  else if(0xC117==DI || 0xC127==DI)//结算日
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[0],6);
  
  else if(PASSWORD_0==DI || PASSWORD_1==DI || PASSWORD_2==DI)//修改密码
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[1],0);
  else if((DI & 0xFF00)==0xC300 || (DI & 0xFF00)==0xCB00)//费率和时段参数
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[1],1);
  else if(0xC510<=DI && DI<=0xC516)
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[1],2);//负荷曲线参数
  //else if()//剩余电量和报警限额
    //;
  else if(0xCA16==DI || 0xCA17==DI)//功率限额
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[1],4);
  else if(0xC214<=DI && DI<=0xC216)//跳合闸延时，用户级别
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[1],5);
  else if(0xC213==DI)//电压合格率
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[1],6);
  else if((DI>=0xC047 && DI<=0xC04E) || 0xC025==DI)
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[1],7);
  else       //其他参数
    SET_BIT(Event_Cumu_Data.Prog_Flag.Flag[0],7);
  
  SET_STRUCT_SUM(Event_Cumu_Data.Prog_Flag);
  SET_STRUCT_SUM(Event_Cumu_Data);  
}

/*****************************************
  数原型：
void Set_Para_Modify(PROTO_DI DI)
  数功能：
  某参数被设置时调用该函数
入口参数：DI,参数的标示码
 隹诓问何?
******************************************/
void Set_Para_Modify(PROTO_DI DI)
{
  
  INT8U i,Posi,Re;
  INT8U Task_Num;
  
  Task_Num=Get_Cur_Task_Num();
  
  Re=CHECK_STRUCT_SUM(Para_Modify);
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();  
    
  for(i=0;i<Task_Num && i<OS_TASK_NUM;i++)
  {
    if(ASSERT(A_WARNING,Para_Modify.Posi[i]<CHK_PARA_NUM))
      Para_Modify.Posi[i]=0;
    
    Para_Modify.Posi[i]++;//第0个字节记录当前修改参数标示码的存放位置
    if(Para_Modify.Posi[i]>=CHK_PARA_NUM)
      Para_Modify.Posi[i]=0;
    
    Posi=Para_Modify.Posi[i];
    if(0!=Para_Modify.Buf[i][Posi])//判断已经修改了的参数是否被读出去了?
    {
     // DEBUG_PRINT(HUCK,DEBUG_0,"Waitfor Set_Para_Modify %s Task ara_Modify.Buf[%d][%d]==%d",\
                               // Tcb[i].Name,i,Posi,Para_Modify.Buf[i][Posi]);
      SET_STRUCT_SUM(Para_Modify);
      OS_Waitfor_Sec(0==Para_Modify.Buf[i][Posi],5);//等待5S该位置变为0
      
      if(0!=Para_Modify.Buf[i][Posi])
        DEBUG_PRINT(HUCK,DEBUG_0,"Para_Modify.Buf[%d][%d]==%d error",i,Posi,Para_Modify.Buf[i][Posi]);
    }
    Para_Modify.Buf[i][Posi]=DI;
  }
  SET_STRUCT_SUM(Para_Modify);
  Set_Para_Modi_DI(DI);//设置跳屏标志
  Set_Pog_Flag(DI);//设置编程标志
}

/*****************************************
  数原型：
INT8U Check_Para_Modify(PROTO_DI DI)
  数功能：
 檠掣霾问欠穹⑸薷?注意不同的程序位置查询同一参数，需要使用不同的ID
入口参数：
DI,参数的标示码
 隹诓问?表示发生了修改,0表示没有发生修改
******************************************/
INT8U Check_Para_Modify(PROTO_DI DI)
{
  INT8U i,Cur_Task_ID;

  Cur_Task_ID=Get_Cur_Task_ID();
    
  if(ASSERT(A_WARNING,Cur_Task_ID<OS_TASK_NUM))
    return 0;
  
  for(i=1;i<CHK_PARA_NUM;i++)
  {
    if(DI==Para_Modify.Buf[Cur_Task_ID][i])
    {
      Para_Modify.Buf[Cur_Task_ID][i]=0;
      return 1; 
    }
  }
  return 0;
}

//获取某个DI参数的存储属性,返回S_ROM或者S_RAM
//注意：每个人物都应该调用该函数，且调用该函数的频率应该较高，因为
INT8U Get_DLT645_Data_Storage_Type(PROTO_DI DI)
{
  INT16U i;
  //INT16U Index;
  INT8U Spec_Flag;
  
  if((DI & 0xF000) == 0x6000)//电量数据
  {
    if((DI & 0x0F00)==0)
      return S_RAM;
    else
      return S_ROM;
  }
  else if((DI & 0xF000) == 0x7000 || (DI & 0xF000) == 0x8000)//需量和需量时间数据
  {
    if((DI & 0x0F00)==0)//当月数据
    {
      if((DI & 0x000F)==0 || (DI & 0x000F)==Cur_Demand.Rate)//当月数据也有部分需要从ROM中读取
        return S_RAM;
      else
        return S_ROM; //非总和当前费率的数据需要从ROM中读取
    }
    else
      return S_ROM;    
  }
  
  for(i=0;i<S_NUM(Protocol_Data);i++)
  {
    if(Check_Protocol_Data_Single(i,DI,FOR_EVENT,&Spec_Flag))
    {
      return Protocol_Data[i].Storage;
    }
  }  
  return S_ROM;
}
/*****************************************
  数原型：
INT8U Get_Para_Modify(PROTO_DI *pDI)
  数功能：
 榧觳槭欠裼胁问⑸薷模绻行薷模蚍祷?，并且*pDI为修改的DI值
入口参数：
*pDI,修改的DI的返回
 隹诓问?表示发生了修改,0表示没有发生修改
******************************************/
INT8U Get_Para_Modify(PROTO_DI *pDI)
{
  INT8U i,Posi,Re,Cur_Task_ID;
  INT8U Count=0;

  Cur_Task_ID=Get_Cur_Task_ID();
  
  if(ASSERT(A_WARNING,Cur_Task_ID<OS_TASK_NUM))
    return 0;
  
  Re=CHECK_STRUCT_SUM(Para_Modify);
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();
  
  Posi=Para_Modify.Posi[Cur_Task_ID];//该任务的修改参数存储位置
  if(ASSERT(A_WARNING,Posi<CHK_PARA_NUM))
    Posi=0;
  
  Count=0;//查询次数计数，每次最多查询CHK_PARA_NUM个参数 
  i=(Posi+1)%CHK_PARA_NUM;//从Posi+1开始查询到Posi
  while(Count<=CHK_PARA_NUM)
  {
    if(Para_Modify.Buf[Cur_Task_ID][i]!=0)//!=0表示该参数还未读出
    {
      *pDI=Para_Modify.Buf[Cur_Task_ID][i];
      if(Cur_Task_ID<OS_TASK_NUM)
        Para_Modify.Buf[Cur_Task_ID][i]=0;
      
      SET_STRUCT_SUM(Para_Modify);
      return 1; 
    }
    Count++;
    i=(i+1)%CHK_PARA_NUM;
  }
  SET_STRUCT_SUM(Para_Modify);
  return 0;//没有找到则返回0 
}

//获取协议数据的密码等级
INT8U Get_Protocol_Data_PSW_Flag(PROTO_DI DI)
{
  INT16U i;
//暂时不允许设置初始电量功能  
/*  
  //电量或需量数据
  if(((DI & 0xF000)==0x9000 || (DI & 0xF000)==0xA000 || (DI & 0xF000)==0xB000) &&\
     ((DI & 0x0C00)==0x0000 || (DI & 0x0C00)==0x0400 || (DI & 0x0C00)==0x0800) &&\
     ((DI & 0x0300)==0x0000 || (DI & 0x0300)==0x0100))
    return PSW_SET_ENSTART;
  
  if((DI & 0xF000)==0x6000 || (DI & 0xF000)==0x7000 || (DI & 0xF000)==0x8000)
    return PSW_SET_ENSTART;

  if((DI & 0xFF00)==0xE700 && ((DI & 0x00F0)>>4)<=0x0D)//历史分相电量
    return PSW_SET_ENSTART;
*/
  
  for(i=0;i<S_NUM(Protocol_Data);i++)
  {
    if(Protocol_Data[i].Spec_Flag==SPEC_NO)
    {
      if((Protocol_Data[i].DI==DI ||\
         (Protocol_Data[i].Num!=0 && DI>Protocol_Data[i].DI && DI<Protocol_Data[i].DI+Protocol_Data[i].Num)) ||\
         (Protocol_Data[i].DI_Equ!=DI_EQU_NULL &&\
         (Protocol_Data[i].DI_Equ==DI ||\
         (Protocol_Data[i].Num!=0 && DI>Protocol_Data[i].DI_Equ && DI<Protocol_Data[i].DI_Equ+Protocol_Data[i].Num))))
        return Protocol_Data[i].PSW_Flag;
    }
    else
    {
      if(Protocol_Data[i].DI==DI ||\
        (Protocol_Data[i].Num!=0 && DI>Protocol_Data[i].DI && DI<=Protocol_Data[i].DI+Protocol_Data[i].Num-0x1111))
        return Protocol_Data[i].PSW_Flag;
    }
  }
  ASSERT(A_WARNING,0);
  return PSW_NULL_FLAG;
}


//判断是否可以从Protocol_Data[Index]获得DI数据
//DI是类似于0xXXXF的数据项
//返回1表示是Protocol_Data中的数据项.DI数据项，2表示是.DI_EQU类型的数据项,0表示在Protocol_Data中没有这个数据项
INT8U Check_Protocol_Data_Block(INT16U Index,PROTO_DI DI,INT8U Data_Flag,INT8U *pSpec_Flag)
{
  INT16U TempDI;
  INT8U Re;
  
  if(SPEC_NO==Protocol_Data[Index].Spec_Flag)
  {
    *pSpec_Flag=SPEC_NO;
    if(((Protocol_Data[Index].DI & 0xFFF0)==(DI & 0xFFF0) &&\
       Protocol_Data[Index].DI_Set_Flag==1))
      return 1;
    else if(Protocol_Data[Index].DI_Equ!=DI_EQU_NULL &&\
            (Protocol_Data[Index].DI_Equ & 0xFFF0)==(DI & 0xFFF0) &&\
            Protocol_Data[Index].DI_Equ_Set_Flag==1)//找到符合条件的数据项Set_Flag置1表示该项允许进入数据集中
    {
      return 2;
    }
    return 0;
  }
  else
  {
    TempDI=(DI & 0xFFF0)+(Protocol_Data[Index].DI & 0x000F);//将最后一个FF换成Protocol_Data[Index].DI的最后两个XX
    Re=Check_Protocol_Data_Single(Index,TempDI,Data_Flag,pSpec_Flag);
    if(1==Re)
    {
      *pSpec_Flag=Protocol_Data[Index].Spec_Flag;
      if(Protocol_Data[Index].DI_Set_Flag==1)
        return 1;
    }
    else if(2==Re)
    {
      *pSpec_Flag=Protocol_Data[Index].Spec_Flag;
      if(Protocol_Data[Index].DI_Equ_Set_Flag==1)
        return 2;      
    }
    return 0;
  }
}

//检查一个DI是否一个合法的块DI,是的话返回1，否的话返回0
INT8U Check_Block_DI(PROTO_DI DI,INT8U Data_Flag)
{
  INT16U i;
  INT8U Spec_Flag;
  
  for(i=0;i<S_NUM(Protocol_Data);i++)
  {
    if(Check_Protocol_Data_Block(i,DI,Data_Flag,&Spec_Flag)>0)
      return 1;
  }
  return 0;
}

//判断是否可以从Protocol_Data[Index]获得DI数据
//DI为非数据集和数据块的数据，DI是类似于0xXXFF的数据项
INT8U Check_Protocol_Data_Set(INT16U Index,PROTO_DI DI,INT8U Data_Flag,INT8U *pSpec_Flag)
{
  INT16U TempDI;
  INT8U Re;
  
  if(SPEC_NO==Protocol_Data[Index].Spec_Flag)
  {
    *pSpec_Flag=SPEC_NO;
    if(((Protocol_Data[Index].DI & 0xFF00)==(DI & 0xFF00) &&\
       Protocol_Data[Index].DI_Set_Flag==1))
      return 1;
    else if(Protocol_Data[Index].DI_Equ!=DI_EQU_NULL &&\
            (Protocol_Data[Index].DI_Equ & 0xFF00)==(DI & 0xFF00) &&\
            Protocol_Data[Index].DI_Equ_Set_Flag==1)//找到符合条件的数据项Set_Flag置1表示该项允许进入数据集中
    {
      return 2;
    }
    return 0;
  }
  else
  {
    *pSpec_Flag=Protocol_Data[Index].Spec_Flag;
    TempDI=(DI & 0xFF00)+(Protocol_Data[Index].DI & 0x00FF);//将最后两个FF换成Protocol_Data[Index].DI的最后两个XX
    Re=Check_Protocol_Data_Single(Index,TempDI,Data_Flag,pSpec_Flag);
    if(1==Re)
    {
      if(Protocol_Data[Index].DI_Set_Flag==1)
        return 1;
    }
    else if(2==Re)
    {
      if(Protocol_Data[Index].DI_Equ_Set_Flag==1)
        return 2;      
    }
    return 0;  
  }
}

//读取某数据项内又含有其他数据项的数据，就是对于Spec_Flag==SPEC_MULTI_DATA的数据 
INT16U Get_Multi_Item_Data(INT8U *pSrc,S_Multi_Data *p,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen)
{
  INT8U i;
  INT16U Src_Off,Src_Len,Dst_Len;
  INT32U Data;
  
  Dst_Len=0;
  for(i=0;i<p->Num;i++)
  {
    Src_Off=p->Data_Item[i].Src_Off;//源数据的偏移
    Src_Len=p->Data_Item[i].Src_Len;
    if(p->Data_Item[i].Src_Format==S_INTU)
    {
      Data=0;
      mem_cpy(&Data,pSrc+Src_Off,Src_Len,&Data,sizeof(Data));
      if(p->Data_Item[i].Dst_Start!=0)
        Data=Data/Pow_10x(p->Data_Item[i].Dst_Start);
      Hex2Bcd(Data,pDst+Dst_Len,p->Data_Item[i].Dst_Len,pDst_Start,DstLen);
      Dst_Len+=p->Data_Item[i].Dst_Len;
    }  
  }
  return Dst_Len;
}

//读取某条独立的数据项
//Index表示DI标示符在Protocol_Data中的索引
//pPara表示输入参数
//pDst表示目标缓冲区起始地址
//pDst_Start和DstLen一起限定pDst和返回数据长度 
//Data_Flag,FOR_COMM表示数据提供给通信,FOR_DISP表示数据提供给显示
//pErr,错误状态返回字
//读取成功的条件:返回长度>0，且NO_ERR==*pErr
INT16U Get_DLT645_Single_Data(INT16U Index,PROTO_DI DI,void *pPara,INT8U ParaLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pErr)
{
  INT16U Len;
  
  Clr_Ext_Inter_Dog();
  
  OS_Sem_Pend(PUB_BUF_SEM_ID);//下面函数内部会用到Pub_Buf,因此使用信号量互斥
  Len=_Get_DLT645_Single_Data(Index,DI,pPara,ParaLen,pDst,pDst_Start,DstLen,Data_Flag,pErr); 
  OS_Sem_Post(PUB_BUF_SEM_ID);
  return Len;
}

//10的a次方
INT32U Pow_10x(INT8U a)
{
  INT8U i;
  INT32U Val=1;
  for(i=0;i<a;i++)
    Val=Val*10;
  return Val;
}

//判断是否可以从Protocol_Data[Index]获得DI数据
//DI为非数据集和数据块的数据
//*pSec_Flag用于返回该项数据的特殊标志
//返回1表示是Protocol_Data中的数据项.DI数据项，2表示是.DI_EQU类型的数据项,0表示在Protocol_Data中没有这个数据项
INT8U Check_Protocol_Data_Single(INT16U Index,PROTO_DI DI,INT8U Data_Flag,INT8U *pSpec_Flag)
{
  INT16U Num,i;
  INT16U TempDI;
  
  if(SPEC_NO==Protocol_Data[Index].Spec_Flag)
  {
    *pSpec_Flag=SPEC_NO;
    if(Protocol_Data[Index].DI==DI ||\
       (Protocol_Data[Index].Num>1 &&\
       DI>Protocol_Data[Index].DI &&\
       DI<Protocol_Data[Index].DI+Protocol_Data[Index].Num))    
    {
      return 1;
    }
    else if(Protocol_Data[Index].DI_Equ!=DI_EQU_NULL &&\
         (Protocol_Data[Index].DI_Equ==DI ||\
         (Protocol_Data[Index].Num>1 &&\
          DI>Protocol_Data[Index].DI_Equ &&\
          DI<Protocol_Data[Index].DI_Equ+Protocol_Data[Index].Num)))
    {
      return 2;   
    }
    else
      return 0;
  }
  else
  {
    *pSpec_Flag=Protocol_Data[Index].Spec_Flag;
    
    //电量或需量数据因为和费率相关，需要特殊处理
    if(SPEC_RATE==Protocol_Data[Index].Spec_Flag)//费率相关的特殊项，例如分费率电能等
    {
      if(Multi_Rate_Para.Rates<=MAX_RATES && (Data_Flag & FOR_EVENT)!=FOR_EVENT)
        Num=(Protocol_Data[Index].Num & 0xFFF0)+(Multi_Rate_Para.Rates+1);
      else
        Num=(Protocol_Data[Index].Num & 0xFFF0)+(MAX_RATES+1);
    }
    else
      Num=Protocol_Data[Index].Num;
    
    //分别对每4位进行比较，都在该范围内
    for(i=0;i<16;i+=4)
    {
      TempDI=(DI>>i) & 0x000F;
      if(TempDI==((Protocol_Data[Index].DI>>i) & 0x000F) ||\
         (TempDI>((Protocol_Data[Index].DI>>i) & 0x000F) &&\
         TempDI<(((Protocol_Data[Index].DI+Num)>>i) & 0x000F)))
      {
         continue;
      }
      else
         break;
    }
    if(i==16)
    {
      return 1;  
    }
    else
    {
      //////////////判断Equ/////////////////////
      for(i=0;i<16;i+=4)
      {
        TempDI=(DI>>i) & 0x000F;
        if(Protocol_Data[Index].DI_Equ!=DI_EQU_NULL &&\
          TempDI>=((Protocol_Data[Index].DI_Equ>>i) & 0x000F) &&\
          TempDI<(((Protocol_Data[Index].DI_Equ+Num)>>i) & 0x000F))
        {
          continue;     
        }
        else
          break;
      }
      if(i==16)
      {
        return 2;
      }
      else
        return 0;
    }
 }
}

//获取某个DI的在Protocol_Data中的Index
INT16U Get_Protocol_Data_Index(PROTO_DI DI)
{
  INT8U Spec_Flag;
  INT16U i;
  
  Clr_Ext_Inter_Dog();
  
  for(i=0;i<S_NUM(Protocol_Data);i++)
  {
    if(Check_Protocol_Data_Single(i,DI,FOR_EVENT,&Spec_Flag)>0)
    {
      return i;
    }
  }  
  return NULL_2BYTES;
}

//Index表示DI标示符在Protocol_Data中的索引
//pPara表示输入参数
//pDst表示目标缓冲区起始地址
//pDst_Start和DstLen一起限定pDst和返回数据长度 
//Data_Flag,FOR_COMM表示数据提供给通信,FOR_DISP表示数据提供给显示
//pErr,错误状态返回字
//读取成功的条件:返回长度>0，且NO_ERR==*pErr
INT16U _Get_DLT645_Single_Data(INT16U Index,PROTO_DI DI,void *pPara,INT8U ParaLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pErr)
{
  INT8U *pSrc;
  INT8U Re,Spec_Flag;
  INT16U Len;//Bef_Mon;
  INT32U Data;

  *pErr=NO_ERR;

  //取出源数据
  if(ASSERT(A_WARNING,Index<S_NUM(Protocol_Data)))
  {
    *pErr=PROTOCOL_DI_ERR; 
    return 0;
  }
  
  //找到该数据项所在的Protocol_Data中的索引
  //必须是Protocol_Data中的某项DI或者其对应的DI_Equ
  Re=Check_Protocol_Data_Single(Index,DI,Data_Flag,&Spec_Flag);
  if(SPEC_NO==Spec_Flag)//不是特殊项
  {
    if(1==Re)
    {
      DI=DI;
    }
    else if(2==Re)
    {
      DI=Protocol_Data[Index].DI+DI-Protocol_Data[Index].DI_Equ;//将DI转化为Protocol_Data[i].DI,方便下面的读取存储
    }
    else
    {
      *pErr=PROTOCOL_DI_ERR;
      return 0;
    }
  }
  else//特殊数据项
  {
    if(Re>0)//是一条单独数据项
    {
      Len=Get_DLT645_Spec_Single_Data(DI,Spec_Flag,pPara,ParaLen,pDst,pDst_Start,DstLen,Data_Flag,pErr);//先当作特殊数据项读取
      return Len;
    }
    else
    {
      *pErr=PROTOCOL_DI_ERR;//不是单独数据项
      return 0;      
    }
  }
  
  if(S_RAM==Protocol_Data[Index].Storage)//数据存储在RAM中
  {
    pSrc=(INT8U *)(Protocol_Data[Index].pSrc)+Protocol_Data[Index].Src_Off;
    pSrc+=(DI-Protocol_Data[Index].DI)*Protocol_Data[Index].Src_Len;
  }
  else if(S_ROM==Protocol_Data[Index].Storage)//数据存储在EEROM或者Flash中
  {
    Len=0;
    //从EEROM或者Flash中读取数据，偏移已经在存储中形成，因此不需要psrc赋值时取偏移
    Len=Read_Storage_Data(DI,(void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),pErr);
    if(ASSERT(A_WARNING,Len>0))
    {
      if(NO_ERR==*pErr)
        *pErr=PROTOCOL_DATA_ERR;
      return 0;
    }
    pSrc=(INT8U *)Pub_Buf+Protocol_Data[Index].Src_Off;
  }
  else
  {
    ASSERT(A_WARNING,0);
    return 0;
  }
  
  //进行数据转换
  if(S_INTU==Protocol_Data[Index].Src_Format)//源数据的类型是无符号整形
  {
    Data=0;
    if(S_LEN4==Protocol_Data[Index].Src_Len)
      mem_cpy(&Data,pSrc,4,&Data,sizeof(Data));//Data=*(INT32U *)(pSrc);
    else if(S_LEN2==Protocol_Data[Index].Src_Len)
      mem_cpy(&Data,pSrc,2,&Data,sizeof(Data));//Data=*(INT16U *)(pSrc);
    else if(S_LEN1==Protocol_Data[Index].Src_Len)
      mem_cpy(&Data,pSrc,1,&Data,sizeof(Data));//Data=*(INT8U *)(pSrc);
    else 
    {
      ASSERT(A_WARNING,0);
      return 0;
    }
    
    if((FOR_SRC & Data_Flag)==FOR_SRC)//获取源数据
    {
      if(Protocol_Data[Index].Src_Len!=Protocol_Data[Index].Dst_Len || Protocol_Data[Index].Dst_Len!=4)
        ASSERT(A_WARNING,0);
      mem_cpy(pDst,&Data,Protocol_Data[Index].Dst_Len,pDst_Start,DstLen);      
    }
    else
    {
      Len=Protocol_Data[Index].Dst_Len;//转换出来的BCD数据的长度
      Data=Data/Pow_10x(Protocol_Data[Index].Dst_Start);//源数据/Dst_Start表示需要的目标数据
      Hex2Bcd(Data,(INT8U *)Pub_Buf,Len,(void *)Pub_Buf,sizeof(Pub_Buf));//转换出BCD数
      mem_cpy(pDst,(INT8U *)Pub_Buf,Protocol_Data[Index].Dst_Len,pDst_Start,DstLen);//提取数据复制到目标缓冲区
    }
    return Protocol_Data[Index].Dst_Len;
  }
  else if(S_INTS==Protocol_Data[Index].Src_Format)//源数据类型是有符号整型
  {
    //目前无该类型 
  }
  else if(S_BCD==Protocol_Data[Index].Src_Format || S_BIN==Protocol_Data[Index].Src_Format)//源数据类型是BCD类型，表示无需进行格式转换
  {
    mem_cpy(pDst,pSrc+Protocol_Data[Index].Dst_Start,Protocol_Data[Index].Dst_Len,pDst_Start,DstLen);//复制到目标缓冲区  
    return Protocol_Data[Index].Dst_Len;    
  }
  ASSERT(A_WARNING,0);
  return 0;
}

//读一个数据块的数据,例如0x901F之类,但不能是0x90FF之类的，必须且只能DI[0]L=0F
//DI表示数据标识符,必须满足DI[0]L=0F;
//pDst表示目标数据缓冲区
//pDst_Start和DstLen一起界定目标数据长度
INT16U Get_DLT645_Block_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pErr)
{
  INT16U i,j;
  INT16U Len,TempLen;
  PROTO_DI TempDI;
  INT8U Re,Spec_Flag,Num;
  
  if(ASSERT(A_WARNING,pDst>=pDst_Start &&  pDst<pDst_Start+DstLen))
    return 0;
  
  Len=0;
  Clr_Ext_Inter_Dog();
  
  for(i=0;i<S_NUM(Protocol_Data);i++)
  {
    Re=Check_Protocol_Data_Block(i,DI,Data_Flag,&Spec_Flag);
    if(0<Re)
    {
      if(1==Re)
        TempDI=(DI & 0xFFF0)+(Protocol_Data[i].DI & 0x000F);
      else
        TempDI=(DI & 0xFFF0)+(Protocol_Data[i].DI_Equ & 0x000F);
      
      if(SPEC_RATE==Spec_Flag)//是费率相关的特殊项?
      {
        if((FOR_EVENT & Data_Flag)==FOR_EVENT)//事件获取的数据都按照满费率算
          Num=MAX_RATES+1;
        else
        {
          if(Multi_Rate_Para.Rates<=MAX_RATES)
            Num=Multi_Rate_Para.Rates+1;
          else
            Num=MAX_RATES+1;
        }
      }
      else 
        Num=(Protocol_Data[i].Num & 0x000F);
      
      for(j=0;j<Num;j++)
      {
        //读取每个子项
        if(pDst+Len+Protocol_Data[i].Src_Len>pDst_Start+DstLen)
        {
          ASSERT(A_WARNING,0);
          *pErr=PROTOCOL_DATA_ERR;
          return 0;
        }
        //OS_TimeDly_Ms(20);//让出给其他任务,Refresh_Para_From_Rom可能进入该流程，要让出给其他任务
        TempLen=Get_DLT645_Single_Data(i,TempDI+j,(void *)0,0,pDst+Len,pDst_Start,DstLen,Data_Flag,pErr);
        
        if(!(TempLen>0 && *pErr==NO_ERR))
          ASSERT(A_WARNING,0);
        
        if(!(pDst+Len+TempLen<=pDst_Start+DstLen))
        {
          ASSERT(A_WARNING,0);
          *pErr=PROTOCOL_DATA_ERR;
          return 0;
        }
        Len+=TempLen;          
      }
    }
  }
  if(ASSERT(A_WARNING,Len!=0))
    *pErr=PROTOCOL_DI_ERR;
  return Len;  
}

//读一个数据块的数据,例如0x90FF之类,但不能是0x900F之类的，必须DI[0]H=F0
//DI表示数据标识符
//pDst表示目标数据缓冲区
//pDst_Start和DstLen一起界定目标数据长度
//*pFollowFlag表示是否有后续帧，返回1表示有，0表示没有
INT16U Get_DLT645_Set_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pFollowFlag,INT8U *pErr)
{
  INT16U i,Len,TempLen;
  PROTO_DI TempDI;
  
  DECLARE_VAR_TYPE(PROTO_DI,S_PROTO_DI);

  static S_PROTO_DI DI_Bak={CHK_BYTE,0xFFFF,CHK_BYTE};
  static S_Int16U i_Bak={CHK_BYTE,0xFFFF,CHK_BYTE};
  
  *pErr=NO_ERR;
  
  if(DI_Bak.Var!=DI)//表示这次是读取一个新的数据项
  {
    i_Bak.Var=0;
    *pFollowFlag=0;
  }
  
  if(ASSERT(A_WARNING,DI!=0xFFFF))//确保不会传入一个DI=0xFFFF的参数正好和DI_Bak的无效标志一致
  {
    i_Bak.Var=0;
    *pFollowFlag=0;
    *pErr=PROTOCOL_DI_ERR;
    return 0;
  }
  
  Len=0;
  TempLen=0;
  //继续从上一帧还没有读完处继续读
  for(i=i_Bak.Var;i<0x0F;i++)//举例:从901F->90FF逐个查找读取
  {
    TempDI=(DI & 0xFF0F)+(i<<4);//生成0x901F这个数据
    if(Check_Block_DI(TempDI,Data_Flag)==1)//这个DI是否是一个块DI？
    {
      TempLen=Get_DLT645_Block_Data(TempDI|0x000F,pDst+Len,pDst_Start,DstLen,Data_Flag,pErr);

      if(TempLen+Len>=MAX_DLT645_DATA_LEN)//长度超过199
      {
        DI_Bak.Var=DI;
        i_Bak.Var=i;//保存该次的i
        *pFollowFlag=1;//表示还有后续帧
        
        pDst[Len]=0xAA;//块分隔符
        Len++;//长度增加
        return Len;
      }
      else
      {
        Len+=TempLen;//长度没有超过200，则生成一个数据块
        pDst[Len]=0xAA;
        Len++;
      }
     }
  }
  DI_Bak.Var=0xFFFF;//没有后续帧了，则DI_Bak置成一个无效值，下次进入该函数就会认为是个新数据项
  i_Bak.Var=0;
  *pFollowFlag=0;//没有后续帧
  return Len;
}

//PROTO_DI
INT16U _Get_DLT645_Spec_Data(PROTO_DI DI, void *pPara, INT8U ParaLen, void *pDst, void *pDst_Start, INT16U DstLen, INT8U Data_Flag, INT8U *pErr)
{
  //INT8U Err;
  INT16U Len;
  //在Protocol_Data中没有找到这个数据项，同时也不是数据集和数据块,因该是报警数据
  //if(Single_Flag==0 && (DI & 0x00F0)!=0x00F0 && (DI & 0x000F)!=0x000F)
 // if(Single_Flag==0 && (DI & 0x000F)!=0x000F)  
  //{
    Len = Extend_Get_Proto_Data(DI,pDst,pDst_Start,DstLen,pErr);//扩展的数据项
    if(Len > 0 || *pErr EQ NO_ERR)
      return Len;
    
    Len=Get_Freeze_Data(DI,pDst,pDst_Start,DstLen,pErr);//因为EF00和EF02这两个数据项又要调用_Get_DLT645_Data所以需要单独处理
    if(Len>0 || *pErr EQ NO_ERR)
      return Len;
    
    Len=Get_MultiTimes_Event_Data(DI,pDst,pDst_Start,DstLen,pErr);
    if(Len>0 || *pErr EQ NO_ERR)
      return Len;

    Len = Get_Cumu0_Event_Data(DI,pDst,pDst_Start,DstLen,pErr);
    if(Len > 0 || *pErr EQ NO_ERR)
      return Len;

    *pErr=PROTOCOL_DI_ERR;//表示不是特殊项
    return 0;

  //}  
  
}

//INT16U Get_Protocol_Single_Data_Index
//读取645数据
//DI,645协议标识
//pPara,入口参数,接收帧的数据域
//pDst,目标数据缓冲区
//pDst_Start和DstLen一起限定pDst和读回数据长度的范围
//DataFlag,FOR_DISP表示读取的数据是给显示用，FOR_DISP表示做显示用
//返回时读取到的数据长度
INT16U _Get_DLT645_Normal_Data(PROTO_DI DI,void *pPara,INT8U ParaLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pFollowFlag,INT8U *pErr)
{
  INT8U Single_Flag,Spec_Flag;
  INT16U Len,i;

  *pErr=NO_ERR;
  *pFollowFlag=0;
  
  if(ASSERT(A_WARNING,(DI & 0xF000)!=0xF000))
  {
    *pErr=PROTOCOL_DI_ERR;
    return 0;
  }
  //程序到这里，表示不是一条特殊数据项
  
  Single_Flag=0;//是否是个独立的数据项，而不是数据集之类
  Len=0;
  for(i=0;i<S_NUM(Protocol_Data);i++)
  {
    if(Check_Protocol_Data_Single(i,DI,Data_Flag,&Spec_Flag)>0)
    {
      Single_Flag=1;
      break;
    }
  }
  
  if(Single_Flag==1)//是条独立数据项
  {
    Len=Get_DLT645_Single_Data(i,DI,pPara,ParaLen,pDst,pDst_Start,DstLen,Data_Flag,pErr);
  }
  else
  {
    if((DI & 0x00F0)==0x00F0)//读取数据集
      Len=Get_DLT645_Set_Data(DI,pDst,pDst_Start,DstLen,Data_Flag,pFollowFlag,pErr);
    else if((DI & 0x000F)==0x000F)//读取数据块
      Len=Get_DLT645_Block_Data(DI,pDst,pDst_Start,DstLen,Data_Flag,pErr);
    else//数据标识错
    { 
      *pErr=PROTOCOL_DI_ERR;
      Len=0;
    }
  }
  
  if(ASSERT(A_WARNING,Len>0))
  {
    if(NO_ERR==*pErr)
      *pErr=PROTOCOL_DATA_ERR;
    return 0;
  }
  else
  {
    *pErr=NO_ERR;
    return Len;
  }
}

INT16U _Get_DLT645_Data(PROTO_DI DI,void *pPara,INT8U ParaLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pFollowFlag,INT8U *pErr)
{
  INT8U FollowFlag;
  INT16U Len;
  
  *pErr=NO_ERR;
  
  Len = _Get_DLT645_Spec_Data(DI,pPara,ParaLen,pDst,pDst_Start,DstLen,Data_Flag,pErr);
  if(Len > 0 || *pErr EQ NO_ERR)
    return Len;
  
  Len = _Get_DLT645_Normal_Data(DI,pPara,ParaLen,pDst,pDst_Start,DstLen,Data_Flag,&FollowFlag,pErr);
  if(ASSERT(A_WARNING,Len>0))
  {
    if(NO_ERR==*pErr)
      *pErr=PROTOCOL_DATA_ERR;
    return 0;
  }
  else
  {
    *pErr=NO_ERR;
    return Len;
  }  
  
}

//获取645数据的简单函数
INT16U Get_DLT645_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U *pErr)
{
  INT8U FollowFlag;
  INT16U Len;
  
  Len = _Get_DLT645_Spec_Data(DI,(void *)0,0,pDst,pDst_Start,DstLen,FOR_COMM,pErr);
  if(Len > 0 || *pErr EQ NO_ERR)
    return Len;
  
  Len = _Get_DLT645_Data(DI,(void *)0,0,pDst,pDst_Start,DstLen,FOR_COMM,&FollowFlag,pErr);
  if(ASSERT(A_WARNING,Len>0))
  {
    if(NO_ERR==*pErr)
      *pErr=PROTOCOL_DATA_ERR;
    return 0;
  }
  else
  {
    *pErr=NO_ERR;
    return Len;
  }
}

//获取645数据的简单函数,专提供给现实任务调用
INT16U Get_DLT645_Data_For_Disp(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen)
{
  INT8U FollowFlag;
  INT16U Len;
  INT8U Err;
  
  Len=_Get_DLT645_Data(DI,(void *)0,0,pDst,pDst_Start,DstLen,FOR_DISP,&FollowFlag,&Err);
  return Len;
  /*
  if(ASSERT(A_WARNING,Len>0))
    return 0;
  else
    return Len;
  */
}

//#define PROTOCOL_DLT645 0
//Protocol_Type填PROTOCOL_DLT645宏
//DI数据标识符
//pDst目标缓冲区
//pDst_Start和DstLen一起限定pDst和返回长度(Len):
//ASSERT(A_WARNING,pDst>=pDst_Start && pDst+Len<=pDst_Start+DstLen)
//Data_Flag表示该函数提供给通信还是显示,FOR_DISP表示给显示，FOR_COMM表示给通信
//*pFollowFlag表示是否有后续帧,*pFollowFlag==1表示有后续帧
//有后续帧的情况下，如果要读取后续帧，则再一次调用该函数即可，DI需填与前帧相同的DI
INT16U Get_Meter_Protocol_Data(INT8U Protocol_Type,PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pFollowFlag)
{
  INT16U Len;//返回长度
  INT8U Err;
  
  if(PROTOCOL_DLT645==Protocol_Type)
  {
    Len=_Get_DLT645_Data(DI,(S_HEX_Time *)&Cur_Time0,0,pDst,pDst_Start,DstLen,Data_Flag,pFollowFlag,&Err);
    if(ASSERT(A_WARNING,pDst>=pDst_Start && pDst+Len<=pDst_Start+DstLen))
      return 0;
    
    if(ASSERT(A_WARNING,Len>0 && NO_ERR==Err))
      return 0;
    
    return Len;
  }
  else
  {
    ASSERT(A_WARNING,0);
    return 0;
  }
}

//检查收到的DLT645数据的长度是否合法
INT8U Check_DLT645_Data_Len(INT8U Ctrl_Code,INT8U *pSrc,INT8U SrcLen)
{
  if(Ctrl_Code==C_READ_DATA && SrcLen>=2)//一般数据是２个字节的标识，但是读取给定时间块的负荷曲线是７字节
    return NO_ERR;
  else if(Ctrl_Code==C_READ_FOLLOW_DATA)
    return NO_ERR;
  if(Ctrl_Code==C_SET_DATA && SrcLen>=2)
    return NO_ERR;
  else if(Ctrl_Code==C_ADJUST_METER && SrcLen>2)
    return NO_ERR;
  else if(Ctrl_Code==C_ADJUST_TIME)
  {
    if(Mode_Word.Mode[0].Bit.Bit4==1 && SrcLen==0x0A)//广播校时
      return NO_ERR;
    else if(SrcLen==0x06)
      return NO_ERR;
  }
  else if(Ctrl_Code==C_SET_ADDR && SrcLen==0x06)//写设备地址
    return NO_ERR;
  else if(Ctrl_Code==C_SET_COM_BAUD && SrcLen==0x01)//写波特率
    return NO_ERR;
  else if(Ctrl_Code==C_SET_PSW && SrcLen==0x08)//修改密码
    return NO_ERR;
  else if(Ctrl_Code==C_CLR_ALL && (SrcLen==0x04 || SrcLen==0x03))//数据总清或者冻结
    return NO_ERR;
  else if(Ctrl_Code==C_CLR_DEMAND && SrcLen==0x04)//清需量
    return NO_ERR;
  else if(Ctrl_Code==C_CLR_EVENT && SrcLen==0x04)//清过程(事件)
    return NO_ERR;
  else if(Ctrl_Code==C_CLR_STAT && SrcLen==0x04)//请电压合格率(统计)
    return NO_ERR;
  
  return PROTOCOL_DATA_ERR;
}




//设置某DI项数据
//DI,数据标识
//pSrc,源数据起始指针
//SrcLen,源数据长度
INT8U Set_DI_Data_Proc(INT16U Index,PROTO_DI DI,INT8U *pSrc,INT8U SrcLen)
{
  INT8U Re,Spec_Flag;
  
  //在Protocol_Data中查找到该项
  //for(i=0;i<S_NUM(Protocol_Data);i++)
  Re=Check_Protocol_Data_Single(Index,DI,FOR_COMM,&Spec_Flag);
  if(1==Re)
  {
    if(Protocol_Data[Index].Src_Format==S_BCD)//找到该项，且该项要求是BCD格式
    {
      if(!Check_BCD_Data(pSrc,SrcLen))//发送来的数据是BCD?
        return PROTOCOL_DATA_ERR;
    }
  }
  else if(2==Re)
  {
    DI=Protocol_Data[Index].DI+DI-Protocol_Data[Index].DI_Equ;//将DI转化为DI_Equ,以使读存储有效
    if(Protocol_Data[Index].Src_Format==S_BCD)//找到该项，且该项要求是BCD格式
    {
      if(!Check_BCD_Data(pSrc,SrcLen))
        return PROTOCOL_DATA_ERR;
    }
  }
  else
    return PROTOCOL_DI_ERR;
  
  Re=Write_Storage_Data(DI,pSrc,SrcLen);
  return Re;
}

//检查设置数据的格式，正确返回1，错误返回0
INT8U Set_Data_Format_Check(INT16U Index,PROTO_DI DI,INT8U *pSrc,INT8U SrcLen)
{
  if(DI==0xC310 || DI==0xCB10)//年时区数
  {
    if(Bcd2Hex_Byte(*pSrc)>MAX_YEAR_PERIODS)
    {
      SET_BIT(Sys_Run_Flag.Err_Flag,4);
      return 0;
    }
  }
  else if(DI==0xC312 || DI==0xCB12)//日时段数超
  {
    if(Bcd2Hex_Byte(*pSrc)>MAX_DATE_PERIODS)
    {
      SET_BIT(Sys_Run_Flag.Err_Flag,5);  
      return 0;
    }
  }
  else if(DI==0xC313 || DI==0xCB13)//费率数超
  {
    if(Bcd2Hex_Byte(*pSrc)>MAX_RATES)
    {
      SET_BIT(Sys_Run_Flag.Err_Flag,6);
      return 0;
    }
  }
  return 1;
  
}

//设置数据处理
INT8U Set_Data_Proc(INT8U *pSrc,INT8U SrcLen)
{
  INT8U Re;
  PROTO_DI DI;
  INT16U i;
  INT8U Single_Flag,Spec_Flag;
 
  DI=0;
  mem_cpy(&DI,pSrc,2,&DI,sizeof(DI));//DI=*(INT16U *)pSrc;
  if(SrcLen>6)//包括标识和密码起码有6字节
  {
    Single_Flag=0;//是否是个独立的数据项，而不是数据集之类
    for(i=0;i<S_NUM(Protocol_Data);i++)
    {
      if(Check_Protocol_Data_Single(i,DI,FOR_COMM,&Spec_Flag)>0)
      {
        if(Protocol_Data[i].PSW_Flag==PSW_RD_ONLY)//该数据不可写
          return PROTOCOL_DI_ERR;
        
        Single_Flag=1;
        break;
      }
    }
  
    if(Single_Flag==0)//不是条独立数据项
    {
      SET_BIT(Sys_Run_Flag.Err_Flag,1);//数据标识错
      return PROTOCOL_DI_ERR;
    }
    
    Re=Set_Data_Format_Check(i,DI,pSrc+6,SrcLen-6);//检查数据格式等
    if(0==Re)
      return PROTOCOL_DATA_ERR;
    
    //DI到数据之间有6字节的密码哦
    if(SPEC_NO==Spec_Flag)
      Re=Set_DI_Data_Proc(i,DI,pSrc+6,SrcLen-6);//正常项的设置
    else
      Re=Set_Spec_Data_Proc(DI,Spec_Flag,pSrc+6,SrcLen-6);

    if(NO_ERR==Re)
      Set_Para_Modify(DI);
    else
    {
      SET_BIT(Sys_Run_Flag.Err_Flag,0);//非法数据
    }
    return Re;
  }
  else
  {
    SET_BIT(Sys_Run_Flag.Err_Flag,0);//非法数据
    return PROTOCOL_DATA_ERR;
  }
}

//校表处理
INT8U Adjust_Meter_Proc(INT8U *pSrc,INT8U SrcLen)
{
  return NO_ERR;
}

INT8U Adj_Time(S_BCD_Time *pTime)
{
    //设置事件时间
    mem_cpy((void *)&Event_Cumu_Data.Time, (void *)&Cur_Time1, sizeof(Cur_Time1),\
            (void *)&Event_Cumu_Data.Time, sizeof(Event_Cumu_Data.Time));
    SET_STRUCT_SUM(Event_Cumu_Data);
    
    if(Set_Time_EXT_INTER_Ram_RTC(pTime))
    {
      //Set_Event_Instant(ID_EVENT_ADJUST_TIME);//校时开始
      //OS_Waitfor_Sec(Read_Event_Status(ID_EVENT_ADJUST_TIME)==1,5);//等待5s还没有处理完则报断言错误
      //if(Read_Event_Status(ID_EVENT_ADJUST_TIME)!=1)
        //ASSERT(A_WARNING,0);
      Event_MultiTimes_Proc(ID_EVENT_ADJUST_TIME,EVENT_OCCUR,EVENT_REAL);
      
      //设置事件时间
      mem_cpy((void *)&Event_Cumu_Data.Time, (void *)&Cur_Time1, sizeof(Cur_Time1),\
              (void *)&Event_Cumu_Data.Time, sizeof(Event_Cumu_Data.Time));
      SET_STRUCT_SUM(Event_Cumu_Data);
      
      //Clr_Event_Instant(ID_EVENT_ADJUST_TIME);//校时结束
      Event_MultiTimes_Proc(ID_EVENT_ADJUST_TIME,EVENT_END,EVENT_REAL);
      
      Time_Modify_Flag.Var = 1;
      return NO_ERR;
    }
    
    return ADJ_TIME_ERR;
  
}

//广播校时处理
INT8U Adjust_Time_Proc(INT8U *pSrc,INT8U SrcLen)
{
  S_BCD_Time TempTime;
  S_HEX_Time TempTime1,Temp_Cur_Time;
  static S_BCD_Time Last_Adj_Time={0}; //上次校时时间  
  INT8U y,m,d;//用于计算星期
  INT8U Re;
  
  if(Last_Adj_Time.Date EQ Cur_Time1.Date &&\
     Last_Adj_Time.Month EQ Cur_Time1.Month &&\
     Last_Adj_Time.Year EQ Cur_Time1.Year) //当日内已经校时一次了
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Adj Time >1 times today!");
    return ADJ_TIME_ERR;
  }
  if(Mode_Word.Mode[0].Bit.Bit4==1)//需要密码,前3个字节是密码等级和密码
    pSrc=pSrc+4;;
  //else
  {
    TempTime.Sec=*pSrc;
    TempTime.Min=*(pSrc+1);
    TempTime.Hour=*(pSrc+2);
    TempTime.Date=*(pSrc+3);
    TempTime.Month=*(pSrc+4);
    TempTime.Year=*(pSrc+5);
    y=Bcd2Hex_Byte(TempTime.Year); 
    m=Bcd2Hex_Byte(TempTime.Month);
    d=Bcd2Hex_Byte(TempTime.Date);
    TempTime.Week=Week(y,m,d);
    SET_STRUCT_SUM(TempTime);
    if(Check_BCD_Time(&TempTime))
    {
      Convert_BCD_2_HEX_Time(&TempTime,&TempTime1);
      //当前时间超过当前时间的正负10分钟，则返回失败
      Time_Inc((S_HEX_Time *)&Cur_Time0,10,UNIT_MIN,&Temp_Cur_Time);//当前时间向后延10分钟
      if(Cmp_Time(&TempTime1,&Temp_Cur_Time)==TIME_AFT)
      {
        ASSERT(A_WARNING,0);
        return ADJ_TIME_ERR;
      }
      
      Time_Dec((S_HEX_Time *)&Cur_Time0,10,UNIT_MIN,&Temp_Cur_Time);
      if(Cmp_Time(&TempTime1,&Temp_Cur_Time)==TIME_BEF)
      {
        ASSERT(A_WARNING,0);
        return ADJ_TIME_ERR;
      }
      
      //必须是同一日内才可校时
      if(!(TempTime.Date==Cur_Time1.Date && TempTime.Month == Cur_Time1.Month && TempTime.Year == Cur_Time1.Year))
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Adj Time must be the in same day!");
        return ADJ_TIME_ERR;
      }
      //Extend_Set_Prog_Flag(DI_BROAD_ADJ_TIME);//广播校时编程记录
      
      Re = Adj_Time(&TempTime);
      if(Re EQ NO_ERR)
       mem_cpy((void *)&Last_Adj_Time,(void *)&Cur_Time1,sizeof(Cur_Time1),(void *)&Last_Adj_Time,sizeof(Last_Adj_Time));
      ASSERT(A_WARNING,Re==NO_ERR);
      return Re;
    }
  }
  ASSERT(A_WARNING,0);
  return ADJ_TIME_ERR;  
}

//写设备地址处理
//Ch表示数据的通道
//pSrc,数据起始地址
//SrcLen,数据长度
INT8U Set_MeterAddr_Proc(INT8U Ch,INT8U *pSrc,INT8U SrcLen)
{
  INT8U Re;
  
  if(Check_Set_ADDR_Key()==0)
    return SET_ADDR_ERR;
  
  if(CH_RS485_1==Ch || CH_IRAD==Ch)//第一路485设置表地址
  {
    Re=Write_Storage_Data(METER_ADDR0,pSrc,6);
    if(NO_ERR==Re)
      Set_Para_Modify(DI_METER_ADDR0);
  }
  else if(CH_RS485_2==Ch)//第二路485设置表地址
  {
    Re=Write_Storage_Data(METER_ADDR1,pSrc,6);
    if(NO_ERR==Re)
      Set_Para_Modify(DI_METER_ADDR1);
  }

  return Re;
}

//设通信波特率处理
INT8U Set_CommBaud_Proc(INT8U Ch,INT8U *pSrc,INT8U SrcLen)
{
  if(Ch==CH_RS485_1)//第一路485
  {
    Write_Storage_Data(DI_COMM_BAUD0,pSrc,1);
    Set_Para_Modify(DI_COMM_BAUD0);
    return NO_ERR;
  }
  else if(Ch==CH_RS485_2)//第2路485
  {
    Write_Storage_Data(DI_COMM_BAUD1,pSrc,1);
    Set_Para_Modify(DI_COMM_BAUD1); 
    return NO_ERR;
  }
  return WR_STORAGE_DATA_AUTH_ERR;  
}

//修改密码处理
INT8U Set_PSW_Proc(INT8U *pSrc,INT8U SrcLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen)
{
  INT8U Level;
  INT8U Re;
  STORA_DI DI;
  
  Level=pSrc[4];//新设密码的级别
  if(Level<3)
  {
    if(Level==0)
      DI=PASSWORD_0;
    else if(Level==1)
      DI=PASSWORD_1;
    else if(Level==2)
      DI=PASSWORD_2;
    else
      return SET_PSW_ERR;
    
    mem_cpy((void *)Cur_PSW.PSW[Level],pSrc+5,3,(void *)Cur_PSW.PSW[Level],sizeof(Cur_PSW.PSW[Level]));
    SET_STRUCT_SUM(Cur_PSW);
    Re=Write_Storage_Data(DI,(void *)Cur_PSW.PSW[Level],sizeof(Cur_PSW.PSW[Level]));

    if(NO_ERR==Re)
    {
      Set_Para_Modify(DI);
      mem_cpy(pDst,pSrc+4,4,pDst_Start,DstLen);
      return NO_ERR;
    }
    else
      ASSERT(A_WARNING,0);
  }
  return SET_PSW_ERR;
  
}

//数据总清处理
INT8U Clear_AllData_Proc(INT8U *pSrc,INT8U SrcLen)
{
  Clear_Energy_Data();
  Clear_All_Demand_Data();
  Clear_VolStat_Data();
  Clear_Event_Data();
  return NO_ERR;  
}

//清需量处理
INT8U Clear_Demand_Proc(INT8U *pSrc,INT8U SrcLen)
{
  Clear_Cur_Demand_Data();
  return NO_ERR;  
}

//清过程处理(请时间)
INT8U Clear_Event_Proc(INT8U *pSrc,INT8U SrcLen)
{
  Clear_Event_Data();
  return NO_ERR;  
}

//请电压合格率数据处理(请统计)
INT8U Clear_Stat_Proc(INT8U *pSrc,INT8U SrcLen)
{
  Clear_VolStat_Data();
  return NO_ERR;  
}

//数据打包
//Ch表示通道号
//Ctrl_Code控制码
//pSrc需要打包的数据域
//SrcLen数据域长度
//pDst目标缓冲区
//pDst_Start和DstLen一起限定pDst和帧长
INT16U DLT645_Data_Pack(INT8U Ch,INT8U Ctrl_Code,INT8U *pSrc,INT16U SrcLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen)
{
  INT8U i;
  
  if(ASSERT(A_WARNING,pDst>=pDst_Start && pDst+SrcLen+15<=pDst_Start+DstLen))
    return 0;
  
  Clear_Ext_Dog();
  
  *(pDst)=0x68;
  if(CH_RS485_1==Ch || CH_IRAD==Ch)//根据不同通道填充不同的表地址
    mem_cpy(pDst+1,(void *)Meter_Addr.Addr[0],6,pDst_Start,DstLen);
  else
    mem_cpy(pDst+1,(void *)Meter_Addr.Addr[1],6,pDst_Start,DstLen);
  
  *(pDst+7)=0x68;
  *(pDst+8)=Ctrl_Code;//控制码
  *(pDst+9)=(INT8U)SrcLen;
  mem_cpy(pDst+10,pSrc,SrcLen,pDst_Start,DstLen);
  
  for(i=0;i<SrcLen;i++)//数据域+0x33
    *(pDst+10+i)+=0x33;
  Get_Buf_MulitByte_Sum(pDst,SrcLen+10,1,pDst+10+SrcLen,1);
  *(pDst+SrcLen+11)=0x16;
  return SrcLen+12;
}

//检查645帧格式,pFrame表示帧起始地址,FrameLen表示帧的长度
INT8U Check_Frame_Format(INT8U *pFrame,INT8U FrameLen)
{
  INT8U Sum;
  if(!(0x68==*pFrame && 0x68==*(pFrame+7) && 0x16==*(pFrame+FrameLen-1)))//判断帧头和帧尾
    return 0;

  Get_Buf_MulitByte_Sum(pFrame,FrameLen-2,1,&Sum,1);//判断校验和
  if(!(Sum==*(pFrame+FrameLen-2)))
    return 0; 
     
  return 1;
}
//检查645帧中的表地址是否正确?
//Ch是帧的来源通道
//Addr是帧中的表地址
//Ctrl_Code是帧中的控制码
INT8U Check_Frame_Meter_Addr(INT8U Ch,INT8U Addr[],INT8U Ctrl_Code)
{
  INT8U i;

  if(CH_RS485_1==Ch || CH_IRAD==Ch)
  {
    if(memcmp(Addr,(void *)Meter_Addr.Addr[0],6)==0)
      return 1;
  }
  else
  {
    if(memcmp(Addr,(void *)Meter_Addr.Addr[1],6)==0)
      return 1;
  }
  
//帧中的表地址和Meter_Addr参数中的表地址不一致，则需判断是否是广播地址
  if(memcmp(Addr,BROAD_ADDR_99,6)==0)
  {
    //广播地址99具备的权限：读数据、校表、较时、设表地址、冻结数据
    //C_CLR_ALL主要是用来冻结
    if(C_READ_DATA==Ctrl_Code ||\
       C_READ_FOLLOW_DATA==Ctrl_Code ||\
       C_CLR_ALL==Ctrl_Code ||  
       C_ADJUST_METER==Ctrl_Code ||\
       C_ADJUST_TIME==Ctrl_Code ||\
       C_SET_ADDR==Ctrl_Code)
      return 1;
    else if(C_SET_DATA==Ctrl_Code)//写数据因为在较表时也需要，可能需要用广播地址，因此在工厂状态也可用广播地址写数据
    {
      //在工厂状态下可以用广播地址写数据，方便校表
      if(Check_Meter_Factory_Status()==1)
        return 1;
      else
        return 0;
    } 

    return 0;
  }
  else if(memcmp(Addr,(void *)BROAD_ADDR_AA,6)==0)//全AA地址,只能具备读权限
  {
    //地址AA具备的权限：读数据
    if(C_READ_DATA==Ctrl_Code ||\
       C_READ_FOLLOW_DATA==Ctrl_Code)
      return 1;
    else
      return 0;
  }
  else
  {
    //高位为0xAA,低位为表地址，同样可以设置表参数等，低地址至少要有3字节匹配
    for(i=0;i<6;i++)
    {
     if(Addr[5-i]!=0xAA)
     {
        if(i<=3)//最多3个
        {
          if(CH_RS485_1==Ch || CH_IRAD==Ch)
          {
            if(memcmp(Addr,(void *)Meter_Addr.Addr[0],6-i)==0)
              return 1;
          }
          else
          {
            if(memcmp(Addr,(void *)Meter_Addr.Addr[1],6-i)==0)
              return 1;             
          }
        }
     }
    }
  }
  return 0;
}

//等待某事件
void Wait_Event_Data_Proc_End(INT8U Event_ID)
{
  OS_Waitfor_Sec(Check_Event_End(Event_ID)==0,5);//等待5s还没有处理完则报断言错误
  if(Check_Event_End(Event_ID)==1)
    ASSERT(A_WARNING,0);
  OS_Waitfor(Check_Event_End(Event_ID));
}
//645数据帧的处理
//Ctrl_Code控制码
//pSrc,645帧起始
//SrcLen,帧长
//pDst,目标缓冲区
//pDst_Start,目标缓冲区起始地址
//DstLen,目标缓冲区长度
INT16U Rcv_DLT645_Data_Proc(INT8U Ch,INT8U *pFrame,INT8U FrameLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen)
{
  INT8U FollowFlag,Meter_Addr_Modify_Flag;
  INT8U Data_PSW;//修改数据需要的权限
  INT8U User_PSW;//当前用户拥有的权限
  INT8U Re,ErrFlag;
  static PROTO_DI DI;
  INT16U Len;
  INT8U Ctrl_Code,SrcLen;
  INT8U *pSrc,i;
  
  ErrFlag=NO_ERR;
  Sys_Run_Flag.Err_Flag=0;//错误信息字清零
  
  if(Check_Frame_Format(pFrame,FrameLen)==0)
    return 0;
  
  Ctrl_Code=*(pFrame+8);
  if(Check_Frame_Meter_Addr(Ch,pFrame+1,Ctrl_Code)==0)
    return 0;


  //判断表地址
  //ErrFlag=Err_Info.ErrFlag;
  pSrc=pFrame+10;//数据域起始地址
  SrcLen=FrameLen-12;//数据域长度
  
  for(i=0;i<SrcLen;i++)
    *(pSrc+i)=*(pSrc+i)-0x33;
  
  Re=Check_DLT645_Data_Len(Ctrl_Code,pSrc,SrcLen);//判断数据长度
  if(ASSERT(A_WARNING,Re==NO_ERR))//数据长度不对
  {
    SET_BIT(Sys_Run_Flag.Err_Flag,ILLEGAL_DATA_ERR);//非法数据
    *(pDst+10)=Sys_Run_Flag.Err_Flag;
    return DLT645_Data_Pack(Ch,Ctrl_Code|0xC0,pDst+10,1,pDst,pDst_Start,DstLen);
  }
  
  //获取执行该操作需要的权限级别
  Data_PSW=Get_Protocol_Data_Authority(Ch,Ctrl_Code,pSrc,SrcLen);
  if(ASSERT(A_WARNING,Data_PSW!=PSW_NULL))//没有找到该数据的密码标记
  {
    SET_BIT(Sys_Run_Flag.Err_Flag,ILLEGAL_DATA_ERR);//非法数据
    *(pDst+10)=Sys_Run_Flag.Err_Flag;
    return DLT645_Data_Pack(Ch,Ctrl_Code|0xC0,pDst+10,1,pDst,pDst_Start,DstLen);
  }
  
  //获取当前用户拥有的权限级别
  User_PSW=Get_User_Authority(Ctrl_Code,pSrc,SrcLen,&ErrFlag);//检查当前用户拥有的权限
  if(User_PSW==PSW_NULL)//返回用户权限不够
  {
    if(ErrFlag!=PROTOCOL_PSW_ERR)//如果是密码错，则需进行闭锁控制
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"User Authority Error");
      SET_BIT(Sys_Run_Flag.Err_Flag,PSW_ERR);//权限不够,也认为是密码错
      *(pDst+10)=Sys_Run_Flag.Err_Flag;
      return DLT645_Data_Pack(Ch,Ctrl_Code|0xC0,pDst+10,1,pDst,pDst_Start,DstLen);
    }
  }
  
  if(Check_Meter_Factory_Status()==0)//不在工厂状态的情况才检查用户权限
  { 
    if(PSW_Err_Info.Set_En_Mins EQ 0)//没有开放设置权限,密码正确一次，开发30分钟设置权限
    {
      if(PSW_Err_Info.Lock_Mins>0 && PSW_Err_Info.Lock_Mins <= 1440)//在锁定的情况下，认为用户只具备最低权限
        User_PSW=PSW_3; 
      
      if(User_PSW>Data_PSW)//当前用户权限高于或等于当前数据设置需要的权限,权限不够!
      {
        if(PSW_Err_Info.Err_Times<99)
        {
          PSW_Err_Info.Clr_Err_Times_Mins=1440;//1天后自动清除密码错误次数
          PSW_Err_Info.Err_Times++;
          if(PSW_Err_Info.Err_Times==5)
          {
            PSW_Err_Info.Lock_Mins=1440;//锁定1440分钟
            Set_Event_Instant(ID_EVENT_METER_LOCK);  //密码锁事件发生
          }
          SET_STRUCT_SUM(PSW_Err_Info);
        }
       
        DEBUG_PRINT(HUCK,DEBUG_0,"User do not have the authority!");
        SET_BIT(Sys_Run_Flag.Err_Flag,PSW_ERR);
        *(pDst+10)=Sys_Run_Flag.Err_Flag;
        return DLT645_Data_Pack(Ch,Ctrl_Code|0xC0,pDst+10,1,pDst,pDst_Start,DstLen);
      }
      
      if(Data_PSW <= PSW_2)//需要有写权限，而且密码等都正确，则开发写功能30分钟
      {
        PSW_Err_Info.Set_En_Mins = OPEN_SET_AUTH_TIME;
        SET_STRUCT_SUM(PSW_Err_Info);
      }
    }
    else
      DEBUG_PRINT(HUCK,DEBUG_0,"PSW Set_En_Mins:%u",PSW_Err_Info.Set_En_Mins);
  }
  
  Meter_Addr_Modify_Flag=0;//表地址设置标志  
  Cur_Comm_Ch.Ch=Ch;//保存当前通信通道
  //设置用户的密码级别
  Re=CHECK_STRUCT_SUM(User_Authority_Flag);
  ASSERT(A_WARNING,1==Re);
  User_Authority_Flag.Flag=AUTHORITY_FLAG;//权限标志，增加安全性
  SET_STRUCT_SUM(User_Authority_Flag);
  
  Clr_Ext_Inter_Dog();//清除外部以及内部看门狗
  
  //Extend_Rcv_Data_Proc();
  //如果需要返回数据，则放在pDst+10开始的位置
  switch(Ctrl_Code)
  {
  case C_READ_DATA:    //读数据
    DI=(PROTO_DI)(*pSrc)+(PROTO_DI)(*(pSrc+1))*256;  
    Len=_Get_DLT645_Data(DI,pSrc+2,SrcLen-2,pDst+12,pDst_Start,DstLen,FOR_COMM,&FollowFlag,&Re);
    mem_cpy(pDst+10,&DI,2,pDst_Start,DstLen);
    Len+=2;
    break;
  case C_READ_FOLLOW_DATA://读后续数据
    //pSrc+2是数据域在DI两个字节后的内容
    Len=_Get_DLT645_Data(DI,pSrc+2,SrcLen-2,pDst+12,pDst_Start,DstLen,FOR_COMM,&FollowFlag,&Re);
    mem_cpy(pDst+10,&DI,2,pDst_Start,DstLen);
    Len+=2;
    break;
  case C_SET_DATA:     //写数据
    Re=Set_Data_Proc(pSrc,SrcLen);
    if(Re==NO_ERR) //成功设置参数的情况下清除密码错误信息
      Clear_PSW_Err_Info();
    Len=0;
    break;
  case C_ADJUST_METER://校表
    Re=CalibMeterProc_PUCK(*(INT16U *)pSrc,pSrc+2,SrcLen-2);
    Len=0;
    break;
  case C_ADJUST_TIME:  //广播校时
    Re=Adjust_Time_Proc(pSrc,SrcLen);
    Len=0;
    //break;
    return 0;//校时不要求应答
  case C_SET_ADDR:    //写表地址
    Meter_Addr_Modify_Flag=0;
    Re=Set_MeterAddr_Proc(Ch,pSrc,SrcLen);
    if(Re==NO_ERR)
    {
      if(CH_RS485_1==Ch || CH_IRAD==Ch)
        Meter_Addr_Modify_Flag=1;//第一路表地址发生修改
      else
        Meter_Addr_Modify_Flag=2;//第二路表地址发生修改
    }
    Len=0;
    break;
  case C_SET_COM_BAUD: //设通信速度
    Re=Set_CommBaud_Proc(Ch,pSrc,SrcLen);
    Len=0;
    break;
  case C_SET_PSW:   //修改密码
    Re=Set_PSW_Proc(pSrc,SrcLen,pDst+10,pDst_Start,DstLen);
    if(Re==NO_ERR)
      Len=4;
    else
      Len=0;
    break;
  case C_CLR_ALL:      //总清0或者常规冻结
    if(SrcLen==0x04)//总清
    {
      Set_Event_Instant(ID_EVENT_CLR_ALL_DATA);//总清零
      Wait_Event_Data_Proc_End(ID_EVENT_CLR_ALL_DATA);//等待事件处理完成
      Set_Clear_Data_Flag(CLEAR_ALL_FLAG);
      OS_Waitfor(Check_Clear_Data_Flag()==CLEAR_END_FLAG);
      Re=NO_ERR;
      Len=0;
    }
    else if(SrcLen==0x03)//冻结
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Freez Data Time:%x-%x",*(pSrc+1),*(pSrc+2));      
      if(*pSrc==0x13)//总电量立即冻结
      {//总电量立即冻结命令本身并不存储数据，只依靠事件记录来存储数据
        DEBUG_PRINT(HUCK,DEBUG_0,"Total Energy Data Freeze RightNow!");
        Set_Event_Instant(ID_EVENT_ENERG_FREEZ);//总电量立即冻结
        Wait_Event_Data_Proc_End(ID_EVENT_ENERG_FREEZ);//等待事件处理完成
        Re=NO_ERR;
      }
      else if(*pSrc==0x03)//顺时量和电量冻结
      {
        if(*(pSrc+1)==0x99 && *(pSrc+2)==0x99)//连续两个99表示立即冻结
        {
          DEBUG_PRINT(HUCK,DEBUG_0,"Instant and Energy Data Freeze RightNow!");
          Freeze_Right_Now(0);//冻结瞬时量
          Freeze_Right_Now(1);//冻结电量
          Re=NO_ERR;
        }
        else//设置冻结时间,周期冻结
        {
          if(*(pSrc+1)<0x60 && *(pSrc+2)<0x24)
          {
            Re=Write_Storage_Data(ENERGY_FREEZE_TIME,pSrc+1,2);
            Read_Freeze_Data_Para_From_Rom();//重新读取冻结时刻
            Re=NO_ERR;
          }
          else
            Re=PROTOCOL_DATA_ERR;
        }
      }
      else
        Re=PROTOCOL_DATA_ERR;

      Len=0;
    }
    break;
  case C_CLR_DEMAND:   //清需量
    Set_Event_Instant(ID_EVENT_CLR_DEMAND_COM);//通讯清需量事件
    Wait_Event_Data_Proc_End(ID_EVENT_CLR_DEMAND_COM);//等待事件处理完成
    Set_Clear_Data_Flag(CLEAR_DEMAND_FLAG);
    OS_Waitfor(Check_Clear_Data_Flag()==CLEAR_END_FLAG);
    Re=NO_ERR;
    Len=0;
    break;
  case C_CLR_EVENT:    //清过程(事件)
    Set_Event_Instant(ID_EVENT_CLR_PROCESS);//清过程
    Wait_Event_Data_Proc_End(ID_EVENT_CLR_PROCESS);//等待事件处理完成
    Set_Clear_Data_Flag(CLEAR_EVENT_FLAG);
    OS_Waitfor(Check_Clear_Data_Flag()==CLEAR_END_FLAG);
    Re=NO_ERR;
    Len=0;
    break;
  case C_CLR_STAT:     //请电压合格率(统计)
    Set_Event_Instant(ID_EVENT_CLR_VOLT_PASS);//清电压合格率
    Wait_Event_Data_Proc_End(ID_EVENT_CLR_VOLT_PASS);//等待事件处理完成
    Set_Clear_Data_Flag(CLEAR_VOLSTAT_FLAG);
    OS_Waitfor(Check_Clear_Data_Flag()==CLEAR_END_FLAG);
    Re=NO_ERR;
    Len=0;
    break;
  default:
    Re=PROTOCOL_DATA_ERR;
    Len=0;
    break;  
  }
  User_Authority_Flag.Flag=0;
  SET_STRUCT_SUM(User_Authority_Flag);
  
  if(Re==NO_ERR)
  {
    Len=DLT645_Data_Pack(Ch,Ctrl_Code|0x80,pDst+10,Len,pDst,pDst_Start,DstLen);
    
    //判断表地址是否发生了修改，如果已经修改，则要重新读取到Meter_Addr变量中，读取只能在打包函数后
    //因为应答帧仍然采用老的表地址
    if(Meter_Addr_Modify_Flag==1)
      Read_Storage_Data(METER_ADDR0,(void *)Meter_Addr.Addr[0],(void *)Meter_Addr.Addr[0],6,&ErrFlag);
    else if(Meter_Addr_Modify_Flag==2)
      Read_Storage_Data(METER_ADDR1,(void *)Meter_Addr.Addr[1],(void *)Meter_Addr.Addr[1],6,&ErrFlag);

    ASSERT(A_WARNING,NO_ERR==ErrFlag);
    return Len;
  }
  else
  {
    //否定应答设错误标示字
    if(0==Sys_Run_Flag.Err_Flag)
    {
      SET_BIT(Sys_Run_Flag.Err_Flag,DI_ERR);
    }
    *(pDst+10)=Sys_Run_Flag.Err_Flag;
    Len=1;

    return DLT645_Data_Pack(Ch,Ctrl_Code|0xC0,pDst+10,Len,pDst,pDst_Start,DstLen);
  }
}

//设置表计协议数据
//返回值
//pSrc是帧起始位置，也就是第一个68的位置
//SrcLen是帧长，从68到16的长度
INT8U Rcv_Meter_Protocol_Data_Proc(INT8U Ch,INT8U Protocol_Type,INT8U *pFrame,INT8U FrameLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen)
{
  if(PROTOCOL_DLT645==Protocol_Type)
  {
    return Rcv_DLT645_Data_Proc(Ch,pFrame,FrameLen,pDst,pDst_Start,DstLen);
  }
  return 0;
}

//在缓冲区中找645帧,并处理之，生成应答帧
//Ch,表示数据来源通道
//pSrc,源缓冲区
//SrcLen,源缓冲区长度
//pDst,目标缓冲区长度
//pDst_Start,和DstLen一起限定应答帧长度
//返回长度,0标识没有应答帧，>0有应答帧，且为帧长
INT16U Rcv_PROTOCOL_Frame_Proc(INT8U Ch,INT8U *pSrc,INT16U SrcLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen)
{
  INT16U Len;//Offset;
  
  //Len=Search_Protocol_Frame(pSrc,SrcLen,&Offset);
  //if(Len>0)
  //{
    Len=Rcv_DLT645_Data_Proc(Ch,pSrc,SrcLen,pDst,pDst_Start,DstLen);
    return Len;
  //}

  //return 0;
}

//寻找协议帧，在某个pSrc开始的buf中
//寻找一条645帧
//pSrc，搜索的缓冲区起始位置
//SrcLen,缓冲区长度
//pOffset,找到的帧的起始偏移，相对于SrcLen
//返回值:找到的帧长
INT16U Search_Protocol_Frame(INT8U *pSrc,INT16U SrcLen,INT16U *pOffset)
{
  INT16U i;
  INT16U Len;
  
  if(SrcLen<10)
    return 0;
  
  for(i=0;i<SrcLen-10;i++)
  {
    if(*(pSrc+i)==0x68 &&*(pSrc+i+7)==0x68)
    {
      Len=*(pSrc+i+9);//数据域长度
      Len+=12;//加上帧头尾
      if((i+Len)<=SrcLen && Check_Frame_Format(pSrc+i,Len)==1)//帧格式正确
      {
        *pOffset=i;
        return Len;       
      }
    }
  }
  return 0;
}

//寻找协议帧，在某个pSrc开始的buf中
//寻找一条645帧
//pSrc，搜索的缓冲区起始位置
//SrcLen,缓冲区长度
//pOffset,找到的帧的起始偏移，相对于SrcLen
//返回值:找到的帧长
INT16U Simple_Search_Frame(INT8U *pSrc,INT16U SrcLen,INT16U *pOffset)
{
  INT16U i;
  INT16U Len;
  
  if(SrcLen<10)
    return 0;
  
  for(i=0;i<SrcLen-10;i++)
  {
    if(*(pSrc+i)==0x68 &&*(pSrc+i+7)==0x68)
    {
      Len=*(pSrc+i+9);//数据域长度
      Len+=12;//加上帧头尾
      
      if(0x16==*(pSrc+i+Len-1))//判断帧头和帧尾
      return Len;
    }
  }
  return 0;
}

//检查协议数据的存储，Protocol_Data中S_ROM属性的数据应该在存储中都有数据
INT8U Check_Protocol_Data_Storage()
{ 
  INT16U i;
  INT8U Re;
  
  Re = NO_ERR;
  Clear_Ext_Dog();
  for(i=0;i<S_NUM(Protocol_Data);i++)
  {
    if(S_ROM==Protocol_Data[i].Storage && Protocol_Data[i].Spec_Flag==SPEC_NO)
    {    
      if(Get_Storage_Data_Len(Protocol_Data[i].DI)==0)
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Protocol_Data %x Storage Err",Protocol_Data[i].DI);
        Re |= PROTOCOL_DATA_STORAGE_ERR;
      }
    }
  }
  return Re;
}

//设置需要清除数据标志
//入口参数:
//CLEAR_END_FLAG，没有数据需要清除
//CLEAR_ENERGY_FLAG,需要清除电量
//CLEAR_DEMAND_FLAG,需要清除需量
//CLEAR_VOSTAT_FLAG,需要清楚电压统计
void Set_Clear_Data_Flag(INT32U Flag)
{
  INT8U Re;
  Re=CHECK_STRUCT_SUM(Clear_Flag);
  ASSERT(A_WARNING,1==Re);
  Clear_Flag.Flag=Flag;
  SET_STRUCT_SUM(Clear_Flag); 
}

//检查是否需要清除数据
//返回值:
//CLEAR_END_FLAG，没有数据需要清除
//CLEAR_ENERGY_FLAG,需要清除电量
//CLEAR_DEMAND_FLAG,需要清除需量
//CLEAR_VOSTAT_FLAG,需要清楚电压统计
//CLEAR_EVENT_FLAG,需要清楚电压统计
INT32U Check_Clear_Data_Flag()
{
  INT8U Re;
  
  Re=1;
  Re&=CHECK_STRUCT_VAR(Clear_Flag);
  Re&=CHECK_STRUCT_SUM(Clear_Flag);
  if(ASSERT(A_WARNING,1==Re))
    return CLEAR_END_FLAG;
  else
    return Clear_Flag.Flag;
}

//通知通信流程，清除操作完成
//内部将Clear_Flag.Flag置成CLEAR_END_FLAG
//从而使下次调用Check_Clear_Data_Flag返回CLEAR_END_FLAG
void Clear_Data_End()
{
  INT8U Re;
  Re=CHECK_STRUCT_SUM(Clear_Flag);
  ASSERT(A_WARNING,1==Re);
  Clear_Flag.Flag=CLEAR_END_FLAG;
  SET_STRUCT_SUM(Clear_Flag);
}
#undef DATA_TRANS_C

