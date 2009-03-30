#ifndef Lib_h
#define Lib_h



#ifndef MEASULIB_PUCK
#define MEASULIB_PUCK_EXT  extern
#else
#define MEASULIB_PUCK_EXT 
#endif



#define	PAI		3.14159265




#ifdef  MEASULIB_PUCK

#define DEFA_TYPE    0       //校表参数以 InitMeasuPara[i]为准，即计量芯片上电后的默认参数；
#define SPEC_TYPE    1       //校表参数以 其他特殊参数为准,校表时不需校正；
#define ADJU_TYPE    2       //校表参数以校表为准；




#define  TYPE_POWER 0
#define  TYPE_ANGLE 1
#define  TYPE_VOLT  2
#define  TYPE_CURR  4

#define MIN_CAL_STEP  109.227

#define MIN_ER_COFI   0.0001
//#define MODI_POWER_CONST   10000
#define MAX_POWER_ERR 0.001
#define MIN_POWER_ERR 0.0001

//#define MODI_ANGLE_CONST   10000
#define MAX_ANGLE_ERR 0.0001
#define MIN_ANGLE_ERR 0.0001

//#define MODI_VOLT_CONST   10000
#define MAX_VOLT_ERR 0.001
#define MIN_VOLT_ERR 0.0001

//#define MODI_CURR_CONST   10000
#define MAX_CURR_ERR 0.001
#define MIN_CURR_ERR 0.0001


#define I_REGION_AREA 0.35      //相位补偿分段点Ib的倍数

typedef struct
{ 
  INT8U  ParaFlag;          //见上面的3个宏
  INT32U Para;
  INT16U Di;
  INT32U (* const FuncPtr)(void);
}MEASU_ADJ_PARA;                   //MeasuIC系统模式字


//IC上电后者复位后的参数,由IC给出。
#define IC_PARA_NUM 64

MEASULIB_PUCK_EXT const MEASU_ADJ_PARA InitMeasuPara[IC_PARA_NUM] =
{
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,复位后值不确定,此参数不计内！------------------0
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,000000H，-----------------------1
      {DEFA_TYPE,0x00000000,0,0},                                      //w_Iregion1,000000H-----------------------2
      {DEFA_TYPE,0x00000000,0,0},                                      //w_Iregion2,000000H-----------------------3
      {DEFA_TYPE,0x00000000,0,0},                                      //w_Iregion3,000000H-----------------------4
      {DEFA_TYPE,0x00000000,0,0},                                      //w_Iregion4,000000H-----------------------5
      
      {ADJU_TYPE,0x00000000,DI_POWER_A_0,0},                                      //w_PainA0,000000H-------------------------6
      {ADJU_TYPE,0x00000000,DI_POWER_B_0,0},                                      //w_PainB0,000000H-------------------------7
      {ADJU_TYPE,0x00000000,DI_POWER_C_0,0},                                      //w_PainC0,000000H-------------------------8
      
      {ADJU_TYPE,0x00000000,DI_POWER_A_0,0},                                      //w_PainA1,000000H-------------------------9
      {ADJU_TYPE,0x00000000,DI_POWER_B_0,0},                                      //w_PainB1,000000H-------------------------10
      {ADJU_TYPE,0x00000000,DI_POWER_C_0,0},                                      //w_PainC1,000000H-------------------------11
      
      {ADJU_TYPE,0x00000000,DI_ANGLE_A_0,0},                                      //w_PhsregA0,000000H-----------------------12
      {ADJU_TYPE,0x00000000,DI_ANGLE_A_0,0},                                      //w_PhsregA1,000000H-----------------------13
      {ADJU_TYPE,0x00000000,DI_ANGLE_A_0,0},                                      //w_PhsregA2,000000H-----------------------14
      {ADJU_TYPE,0x00000000,DI_ANGLE_A_0,0},                                      //w_PhsregA3,000000H-----------------------15
      {ADJU_TYPE,0x00000000,DI_ANGLE_A_0,0},                                      //w_PhsregA4,000000H-----------------------16
      
      {ADJU_TYPE,0x00000000,DI_ANGLE_B_0,0},                                      //w_PhsregB0,000000H-----------------------17
      {ADJU_TYPE,0x00000000,DI_ANGLE_B_0,0},                                      //w_PhsregB1,000000H-----------------------18
      {ADJU_TYPE,0x00000000,DI_ANGLE_B_0,0},                                      //w_PhsregB2,000000H-----------------------19
      {ADJU_TYPE,0x00000000,DI_ANGLE_B_0,0},                                      //w_PhsregB3,000000H-----------------------20
      {ADJU_TYPE,0x00000000,DI_ANGLE_B_0,0},                                      //w_PhsregB4,000000H-----------------------21
      
      {ADJU_TYPE,0x00000000,DI_ANGLE_C_0,0},                                      //w_PhsregC0,000000H-----------------------22
      {ADJU_TYPE,0x00000000,DI_ANGLE_C_0,0},                                      //w_PhsregC1,000000H-----------------------23
      {ADJU_TYPE,0x00000000,DI_ANGLE_C_0,0},                                      //w_PhsregC2,000000H-----------------------24
      {ADJU_TYPE,0x00000000,DI_ANGLE_C_0,0},                                      //w_PhsregC3,000000H-----------------------25
      {ADJU_TYPE,0x00000000,DI_ANGLE_C_0,0},                                      //w_PhsregC4,000000H-----------------------26
      
      {ADJU_TYPE,0x00000000,DI_U_GAIN_A,0},                                      //w_UgainA,000000H-------------------------27
      {ADJU_TYPE,0x00000000,DI_U_GAIN_B,0},                                      //w_UgainB,000000H-------------------------28
      {ADJU_TYPE,0x00000000,DI_U_GAIN_C,0},                                      //w_UgainC,000000H-------------------------29
      
      {DEFA_TYPE,0x00000000,0,0},                                                 //w_Irechg,000000H-------------------------30
      
      {SPEC_TYPE,0x00000280,0,&Get_Istart_Const},                                 //w_Istartup,000280H-----------------------31
      {SPEC_TYPE,0x00000010,0,&Get_HighPulse_Const},                                //w_HFConst,000010H------------------------32
      {DEFA_TYPE,0x00000010,0,0},                                      //RESERVED,000010H-------------------------33
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,000000H-------------------------34
      {DEFA_TYPE,0x00000003,0,0},                                      //RESERVED,000003H-------------------------35
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,000000H-------------------------36
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,000000H-------------------------37
      {ADJU_TYPE,0x00000000,DI_I_GAIN_A,0},                               //w_IgainA,000000H-------------------------38
      {ADJU_TYPE,0x00000000,DI_I_GAIN_B,0},                                 //w_IgainB,000000H-------------------------39
      {ADJU_TYPE,0x00000000,DI_I_GAIN_C,0},                                 //w_IgainC,000000H-------------------------40
      {DEFA_TYPE,0x00068000,0,0},                                      //w_FailVoltage,068000H(341)----------------41
      {SPEC_TYPE,0x00000000,0,&GetEnergSumMode},                                      //w_EAddMode,000000H-----------------------42
      {DEFA_TYPE,0x00000000,0,0},                                      //w_GainAdc7,000000H-----------------------43
      {DEFA_TYPE,0x00000003,0,0},                                      //w_GCtrlT7Adc,000003H---------------------44
      {DEFA_TYPE,0x00000000,0,0},                                      //w_EnLineFreq,000000H---------------------45
      {DEFA_TYPE,0x00000000,0,0},                                      //w_EnUAngle,000000H-----------------------46
      {DEFA_TYPE,0x00000000,0,0},                                      //w_SelectPQSU,000000H---------------------47
      {DEFA_TYPE,0x00000000,0,0},                                      //w_EnDtIorder,000000H---------------------48
      {DEFA_TYPE,0x000020C4,0,0},                                      //w_LineFreqPg,0020C4H---------------------49
      {DEFA_TYPE,0x00040000,0,0},                                      //RESERVED,040000H-------------------------50
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,000000H-------------------------51
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,000000H-------------------------52
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,000000H-------------------------53
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,000000H-------------------------54
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,000000H-------------------------55
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,000000H-------------------------56
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,000000H-------------------------57
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,000000H-------------------------58
      {DEFA_TYPE,0x00F99999,0,0},                                      //RESERVED,F99999H-------------------------59
      {DEFA_TYPE,0x00000000,0,0},                                      //w_EnHarmonic,000000H---------------------60
      {DEFA_TYPE,0x00000000,0,0},                                      //RESERVED,000000H-------------------------61
      {DEFA_TYPE,0x00000000,0,0},                                      //w_HFDouble,000000H-----------------------62
      {SPEC_TYPE,0x00000000,0,&Get_Volt_Adj_Rate}                                       //w_UADCPga,000000H------------------------63
};
//0x0016bd03,                                      //check sum:331,地址0x3E-----------PUCK
//0x00043d03,                                      //check sum:341,地址0x3E-----------PUCK

/*
MEASULIB_PUCK_EXT const INT32U HighPulseConst[4][10]=
{
      //11,  11,  14,  13,  14,  11,  13,  13,  08,  13,
        0x0b,0x0b,0x0e,0x0d,0x0e,0x0b,0x0d,0x0d,0x08,0x0d,
      //13,  13,  13,  11,  13,  13,  11,  11,  09,  11,
        0x0d,0x0d,0x0d,0x0b,0x0d,0x0d,0x0b,0x0b,0x09,0x0b,
      //10,  10,  10,  18,  20,  10,  18,  18,  15,  18
        0x0a,0x0a,0x0a,0x12,0x14,0x0a,0x12,0x12,0x0f,0x12,
      //15,  15,  15,  13,  15,  15,  13,  13,  11,  13
        0x0f,0x0f,0x0f,0x0d,0x0f,0x0f,0x0d,0x0d,0x0b,0x0d,
};
*/

MEASULIB_PUCK_EXT const INT32U Startup_Current[10]={0x0146,0x00d9,0x022a,0x00c3,0x0146,0x043f,0x0124,0x0187,0x0146,0x024a};

//电压端采样电压(V): 331,3×100V	;   341，3×220V;  341，3×57.7V;     331,3×380V
MEASULIB_PUCK_EXT const FP32S AD_V_CONST[4]=
{
      0.18,0.350,0.350,0.350
};


//电流端采样电压(V): 1.5(6)A	1(2)A	5(10)A	10(40)A	0.3(1.2)A 5(6)	15(60)	20(80)	5(20)A	30(100) A
MEASULIB_PUCK_EXT const FP32S AD_I_CONST[10]=
{
      0.1,0.07,5,10,0.3,5,15,20,5,30    //只有前2项是确定的，其他不确定
};


#endif


#endif
