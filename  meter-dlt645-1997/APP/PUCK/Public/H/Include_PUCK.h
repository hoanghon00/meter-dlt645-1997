
#ifndef PUBLIC_C_F_S
#define PUBLIC_C_F_S

#ifndef PUB_PUCK_C
#define PUB_PUCK_EXT  extern
#else
#define PUB_PUCK_EXT 
#endif



/**********************************以下是计量公共数据**************************************/

//定义计量错误状态字
#define MEASU_NO_ERR      	0
#define MEASU_SIGN_ERR    	1  //SIG错误
#define MEASU_RD3TIMES_ERR    	2  //读数据错
#define MEASU_CAL_ERR    	3  //计量过程中，出现校表请求
#define MEASU_PUL_SPEC_ERR     	4  //计量参数规格错误
#define MEASU_FREQU_ERR   	5  //频率错误
#define MEASU_VOLT_ERR  	6  //电压超限
#define MEASU_CURR_ERR  	7  //电电流超限
#define MEASU_POWER_ERR  	8  //功率超限
#define MEASU_ACCU_FLOW_ERR   	9  //电能增量太大
#define MEASU_ENR_REDUCE_ERR 	10  //电能倒走
#define MEASU_ENR_LIMIT_ERR 	11  //计量范围超限
#define MEASU_DATA_LOGIC_ERR 	12  //逻辑错误
#define MEASU_RESET_ERR 	13  //复位过程错误
#define MEASU_CS_ERR            14  //校验和错
#define MEASU_ENERG_FLOW_ERR   	15  //电能溢出时，电能增量太大
#define MEASU_CREEP_ENERG_ADD  	16  //潜动时，电能增量了

#define MAX_MEASU_ERR 	        MEASU_CREEP_ENERG_ADD

#define NO_QUADRANT   0             //无效象限
#define QUADRANT1     (NO_QUADRANT+1)
#define QUADRANT2     (QUADRANT1+1)
#define QUADRANT3     (QUADRANT2+1)
#define QUADRANT4     (QUADRANT3+1)



#ifdef MEASURE_ERROR_STATICS_EN
  #define MEASURE_ERR_NUM  (MAX_MEASU_ERR+1)
  typedef struct
  { 
    INT16U        ResetNum;
    INT8U         AcFlag;
    INT32U        LastValue;
    INT32U        CurrValue;
    INT32U        ChangeValue;
    INT8U         Num[MEASURE_ERR_NUM];
    INT8U CS[CS_BYTES]; 
  }MEASURE_ERROR;
NO_INIT PUB_PUCK_EXT MEASURE_ERROR Measure_Err_Info;  
#endif


typedef struct
{
  INT8U A_LossV:1;            //A相失压
  INT8U B_LossV:1;            //B相失压
  INT8U C_LossV:1;            //C相失压
  INT8U V_SeqErr:1;           //电压相序错
  INT8U I_SeqErr:1;           //电流相序错
  INT8U A_Creep:1;            //A潜动
  INT8U B_Creep:1;            //B潜动
  INT8U C_Creep:1;            //C潜动
  
  INT8U A_APwrDir:1;        //A有功功率方向：0－正，1－负
  INT8U B_APwrDir:1;        //B有功功率方向：0－正，1－负
  INT8U C_APwrDir:1;        //C有功功率方向：0－正，1－负
  INT8U SUM_APwrDir:1;      //合相有功功率方向：0－正，1－负  
  INT8U A_RPwrDir:1;        //A无功功率方向：0－正，1－负
  INT8U B_RPwrDir:1;        //B无功功率方向：0－正，1－负
  INT8U C_RPwrDir:1;        //C无功功率方向：0－正，1－负  
  INT8U SUM_RPwrDir:1;      //合相无功功率方向：0－正，1－负 
  
  INT8U MeasuIntOk:1;       //计量芯片初始化完成
  INT8U PulseEn:1;          //脉冲输出使能
  INT8U DataRecFlg:1;       //1：接收到计量数据标志，应用层可取数据；0-----还没有准备好计量数据，不要取
  INT8U CalStatus:2;        //校表状态：0------待校表；1------校表中；2------校表完成：
  INT8U Reserved:3;
  //以上8bits对齐 
  INT8U CS[CS_BYTES];
}MEASU_STATUS;              //计量状态字
PUB_PUCK_EXT MEASU_STATUS Pri_MeasuStatVar;    //MeasuIC状态字

typedef struct
{ 
  INT8U Sync_Flag;
  INT8U ParseMode;                  //331或者341方式,由 V_Spec推算出: PARSE_331-- 0;341-----1
  INT8U P_Spec;                    //脉冲常数规格:
  INT8U V_Spec;                    //电压规格:
  INT8U I_Spec;                    //电流规格:  
  INT8U T_FH;                      //由内卡读出
  INT8U CS[CS_BYTES];              //重要参数，SRAM加校验和
}MEASU_SYSMODE;                   //MeasuIC系统模式字
PUB_PUCK_EXT MEASU_SYSMODE MeasuSysMode;

typedef struct
{  
  INT32U A;
  INT32U B;
  INT32U C;
  INT32U Sum;
}MEASU_DATA;       //MeasuIC瞬时量计量数据


typedef struct
{  
  INT8U A;
  INT8U B;
  INT8U C;
  INT8U Sum;
}MEASUFLAG;       //MeasuIC瞬时量状态数据


/*
typedef struct
{
  INT8U Cal_Status[2];    //较表状态字：顺序：A/B/C相功率,相A/B/C相位,A/B/C相电压，A/B/C相电流
  INT8U CS[CS_BYTES];
}CAL_STSTUS;
PUB_PUCK_EXT  CAL_STSTUS Cal_Status_Var;
*/

//定义各计量数据的精度：保留的小数点数
#define UNIT_KW_KAR   1000000    //0.000001kw，有无功小数点保持一致

#define UNIT_V        10000      //电压单位：0.0001V
#define UNIT_A        10000      //电流单位：0.0001A;或者安时单位
#define UNIT_FACTOR   1000000    //功率因数单位
#define UNIT_HZ       1000000    //频率单位
#define UNIT_DEGREE   100        //角度单位

#define UNIT_KWH_KVAR 100000     //0.00001kwh=0.01wh，有无功小数点保持一致

#define  UNIT_AS      10000                //安时:单位：  0.0001An.s
#define  UNIT_KA2S    10000                //铜损:单位： 0.0000001kA2s
#define  UNIT_MV2S    100                  //铁损:单位：0.00000001MV2s
#define  UNIT_TGFAS   100                  //合成误差：单位：0.01kvars



#define TEMP_BUF_LEN    220
DECLARE_BUF_TYPE (INT8U,TEMP_BUF_LEN,S_Buf_Temp);

#ifdef PUB_PUCK_C
PUB_PUCK_EXT S_Buf_Temp S_Buf_PUCK={CHK_BYTE,{0},CHK_BYTE};
#endif
PUB_PUCK_EXT S_Buf_Temp S_Buf_PUCK;

#define  Temp_Buf_PUCK S_Buf_PUCK.Buf

typedef struct
{
 MEASU_DATA AcPower;       //有功功率:UNIT:0.000001kW
 MEASU_DATA ReacPower;     //无功功率:UNIT:0.000001kvar
 MEASU_DATA AppPower;      //视在功率:UNIT:0.000001kva
 MEASU_DATA Volt;          //电压有效值：无合相 UNIT:0.0001V
 MEASU_DATA Curr;          //电流有效值: UNIT:0.0001A	
 MEASU_DATA PFactor;       //功率因数: UNIT:0.000001
 MEASU_DATA Freq;          //频率:UNIT:0.000001Hz
 MEASUFLAG  Quadrant;      //各相象限：QUADRANT1～QUADRANT4代表1～4象限。
 MEASU_DATA Angle;         //电压电流夹角：UINT:0.01度
 INT32U     Temprate;      //温度
 INT8U CS[CS_BYTES];
}MEASU_INSTANT;   //MeasuIC瞬时量计量数据


typedef struct
{
  INT32U Chang_Plus_AcEnerg[4];            //A/B/C/SUM各元件正向有功增量:单位:0.01wh
  INT32U Chang_Nega_AcEnerg[4];            //A/B/C/SUM各元件反向有功增量:单位:0.01wh
  
  //INT32U Chang_Plus_ReacEnerg[4];          //A/B/C/SUM各元件正向无功增量(无功组合1):单位:0.01warh  
  //INT32U Chang_Nega_ReacEnerg[4];          //A/B/C/SUM各元件反向无功增量(无功组合2):单位:0.01warh
  
  INT32U Chang_Quad_ReacEnerg[4][4];       //A/B/C/SUM各元件四象限无功增量,Chang_Quad_ReacEnerg[1][2]表示B向3象限无功
  
  INT32U Add_AmpaHour[4];                 //A,B,C,sum安时累计:0.0001A.s,累计量 
  INT32U Add_CopperLoss[4];		  //A,B,C,sum铜损,单位为：0.0000001kA2s，累计量
  INT32U Add_IronLoss[4];		  //A,B,C,sum铁损,单位为 0.00000001MV2s，累计量
  INT32U Add_GenaralErr[4];		  //A,B,C,sum合成误差,单位:0.01kvars，累计量
  INT8U CS[CS_BYTES];
}MEASU_ENERG;                             //计量累计数据和增量数据


/************************以下是提供给应用层的数据接口*************************************/
PUB_PUCK_EXT volatile MEASU_STATUS      Measu_Status_Mode_ToPub_PUCK;   //计量基本状态字(已按照331/341处理)
PUB_PUCK_EXT  volatile MEASU_INSTANT    Measu_InstantData_ToPub_PUCK;    //计量瞬时数据
PUB_PUCK_EXT volatile MEASU_ENERG       Measu_Energ_ToPub_PUCK;       //计量累计量数据

#ifdef MEASURE_VAR_BAK_EN
  PUB_PUCK_EXT volatile MEASU_ENERG       Measu_Energ_ToPub_Bak;       //计量累计量数据
#endif

#ifdef Measu_VOLTANGLE_EN
PUB_PUCK_EXT volatile Measu_ANGLE   MeaVoltAngle_ToPub_PUCK;          //提供给应用的数据结构体
#endif




/************************以下函数申明*************************************/
void Measu_Main_Puck(INT8U Flag);


INT16U Get_Measu_AcculateData_PUCK(INT8U ID,void *Dst,void *Dst_Start,INT16U DstLen,INT16U DstMaxLen);


INT8U Get_RemoteCtrlKey_PUCK(INT8U Id);
INT8U Get_MeterPowerStatus_PUCK(void);
INT8U Get_MeterKey_PUCK(INT8U Id);
INT8U Get_Curr_ComType_PUCK(INT8U Id);


void Delayn10MS(INT32U n);
void SYS_TimeDly_Ms(INT32U n);
void SYS_TimeDly_Sec(INT32U n);

INT8U Get_Buf_MulitByte_Sum(INT8U *Src,INT16U SrcLen,INT8U ByteGroup,INT8U *Dst,INT8U DstLen);
INT8U CalibMeterProc_PUCK(INT16U Code,INT8U Para[],INT8U ParaLen);
void Retry_WritePara_To_IC(void);
INT8U Init_Pulse_Ram(void);
INT8U Check_Power_Grid_Event(void);
FP32S Get_Un(void);
FP32S Get_In(void);
INT32U Get_Sys_Pulse(void);
INT8U GetSysModeProc(void);
void Load_Adj_OnePara_To_IC(INT16U);
void Measure_Error_Statics(INT8U Code,INT8U RstFlag);
void Init_Measure_Ram(void);
INT8U Load_MeasureIC_Para(void);
INT8U Load_Spec_Para_To_IC(void);
//void Clr_All_Cal_Status(void);
INT32U Get_Istart_Const(void);
INT32U Get_HighPulse_Const(void);
INT32U Get_Volt_Adj_Rate(void);
INT32U GetEnergSumMode(void);
INT32U Get_Iregion4(void);
void Test_HardWare_PUCK(void);
void InitLcdAllRam(void);
INT32U Get_Min_Num(INT32U a,INT32U b,INT32U c);
void Chk_Table_Conflict(void);
void Init_Measu_PUCK(void);
INT8U Init_Para(void);
INT8U Judge_I_Start(void);
INT8U CHeck_Same_Byte(INT8U *Src,INT8U Byte,INT16U Len);
FP32S Get_Start_Current(void);
void Print_Measure_Err_Info(void);
void Clr_Energ_Accu(void);
#endif
