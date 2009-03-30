#ifndef RegMeasu_h
#define RegMeasu_h



//#undef MeasuBase_PUCK_EXT
#ifndef MeasuBase_PUCK
#define MeasuBase_PUCK_EXT  extern
#else
#define MeasuBase_PUCK_EXT 
#endif



#define  Measu_IC_Delay  {asm("nop"); asm("nop"); asm("nop");} //SYS_TimeDly_Ms(20)

//以下定义MeasuIC参数读寄存器地址
#define REG_R_A_AP         0x01
#define REG_R_B_AP         0x02
#define REG_R_C_AP         0x03
#define REG_R_SUM_AP       0x04
#define REG_R_A_RP         0x05
#define REG_R_B_RP         0x06
#define REG_R_C_RP         0x07
#define REG_R_SUM_RP       0x08
#define REG_R_A_QP         0x09
#define REG_R_B_QP         0x0A
#define REG_R_C_QP         0x0B
#define REG_R_SUM_QP       0x0C

#define REG_R_A_U         0x0D
#define REG_R_B_U         0x0E
#define REG_R_C_U         0x0F

#define REG_R_A_I         0x10
#define REG_R_B_I         0x11
#define REG_R_C_I         0x12
#define REG_R_SUM_I       0x13

#define REG_R_A_PF        0x14
#define REG_R_B_PF        0x15
#define REG_R_C_PF        0x16
#define REG_R_SUM_PF      0x17

#define REG_R_ANGLE_A     0x18
#define REG_R_ANGLE_B     0x19
#define REG_R_ANGLE_C     0x1A
#define REG_R_ANGLE_SUM   0x1B
#define REG_R_SUM_FREQ    0x1C

#define REG_R_A_PENERG    0x1E
#define REG_R_B_PENERG    0x1F
#define REG_R_C_PENERG    0x20
#define REG_R_SUM_PENERG  0x21
#define REG_R_A_QENERG    0x22
#define REG_R_B_QENERG    0x23
#define REG_R_C_QENERG    0x24
#define REG_R_SUM_QENERG  0x25

#define REG_R_LASTRD      0x28
#define REG_R_SUM_U       0x2B
#define REG_R_SIG         0x2c
#define REG_R_LASTWR      0x2d
#define REG_R_ENFLAG      0x30
#define REG_R_AENERG_CLR  0x31
#define REG_R_SUMENERG_CLR    0x38
#define REG_R_PQ_DIR     0x3D
#define REG_R_AB_ANGLE   0x5C
#define REG_R_AC_ANGLE   0x5D
#define REG_R_BC_ANGLE   0x5E
#define REG_R_SUM         0x5f//0x3e

//以下定义MeasuIC参数写寄存器地址
#define REG_W_START     0x81     //0x00无效
#define REG_W_IREGION1  0x82
#define REG_W_PGAIN_A0   0x86
#define REG_W_PGAIN_B0   0x87
#define REG_W_PGAIN_C0   0x88

#define REG_W_PGAIN_A1   0x89
#define REG_W_PGAIN_B1   0x8A
#define REG_W_PGAIN_C1   0x8B

#define REG_W_PHSREG_A0  0x8C
#define REG_W_PHSREG_A1  0x8D
#define REG_W_PHSREG_A2  0x8E
#define REG_W_PHSREG_A3  0x8F
#define REG_W_PHSREG_A4  0x90


#define REG_W_PHSREG_B0  0x91
#define REG_W_PHSREG_B1  0x92
#define REG_W_PHSREG_B2  0x93
#define REG_W_PHSREG_B3  0x94
#define REG_W_PHSREG_B4  0x95


#define REG_W_PHSREG_C0  0x96
#define REG_W_PHSREG_C1  0x97
#define REG_W_PHSREG_C2  0x98
#define REG_W_PHSREG_C3  0x99
#define REG_W_PHSREG_C4  0x9A


#define REG_W_UGAIN_A    0x9B
#define REG_W_UGAIN_B    0x9C
#define REG_W_UGAIN_C    0x9D


#define REG_W_ISTART    0x9f
#define REG_W_PULSE     0xa0

#define REG_W_IGAIN_A   0xa6
#define REG_W_IGAIN_B   0xa7
#define REG_W_IGAIN_C   0xa8

#define REG_W_CAL_POWER 0xb1
#define REG_W_SEL_HARM  0xbc
#define REG_W_PULSE_MUL 0xbe
#define REG_W_U_ADC     0xbf
#define REG_W_ENMODE    0xaa
#define REG_W_VGAIN     0xbf




//电压通道ADC增益
#define V_GAIN_RATE1     0x000001
#define V_GAIN_RATE2     0x465500
#define V_GAIN_RATE4     0x465501
#define V_GAIN_RATE8     0x465502
#define V_GAIN_RATE16    0x465503


#define ENERG_SUM_MODE0     0x00    //341绝对和加；331代数和,此参数和TH_F模式字有关
#define ENERG_SUM_MODE1     0x01    //341代数和加；331绝对和,此参数和TH_F模式字有关


#define EnMeasuCal()  {Measu_WrData(0xc9,0x000000);Measu_WrData(0xc9,0x000000);Measu_WrData(0xc9,0x000000);}         //校表数据写使能
#define DisMeasuCal() {Measu_WrData(0xc9,0x000001);Measu_WrData(0xc9,0x000001);Measu_WrData(0xc9,0x000001);}         //校表数据禁止
#define ClrMeasuCal() {Measu_WrData(0xc3,0x000000);Measu_WrData(0xc3,0x000000);Measu_WrData(0xc3,0x000000);}         //清校表数据

#define EnMeasuPara() {Measu_WrData(0xc6,0x00005a);Measu_WrData(0xc6,0x00005a);Measu_WrData(0xc6,0x00005a);} 
#define EnMeasuData() {Measu_WrData(0xc6,0x000000);Measu_WrData(0xc6,0x000000);Measu_WrData(0xc6,0x000000);} 

#define MEASU_CS_1  B_7022_CS_1
#define MEASU_CS_0  B_7022_CS_0

#define MEASU_CLK_1     B_7022_SCK_1
#define MEASU_CLK_0     B_7022_SCK_0

#define MEASU_DOUT_1  B_7022_SDO_1
#define MEASU_DOUT_0  B_7022_SDO_0

#define MEASU_RST_1     B_7022_RST_1
#define MEASU_RST_0     B_7022_RST_0



#define MEASU_SIG_STAT       B_7022_SIG_STATUS
#define MEASU_DIN_STAT       B_7022_SDI_STATUS


typedef struct
{
  INT8U Sync_Flag;
  
  INT8U CSErr:1;          //MeasuIC校验和错误,:未用
  INT8U SigErr:1;         //MeasuIC SIG信号异常错误
  INT8U WrCsErr:1;        //MeasuIC写校验错误
  INT8U RdCsErr:1;        //读校验错误
  INT8U MeasuAbort:1;     //MeasuIC计量终止标志:未用
  INT8U Retry:1;          //将SRAM参数写到计量时，错误，需要重试
  INT8U HardRstErr:1;     //MeasuIC复位错误
  INT8U CalReqstErr:1;    //校验请求标志
  /*
  INT8U A_PAFlow:1;       //A正向有功寄存器溢出
  INT8U B_PAFlow:1;       //B正向有功寄存器溢出
  INT8U C_PAFlow:1;       //C正向有功寄存器溢出
  INT8U SUM_PAFlow:1;     //合相正向有功寄存器溢出  
  INT8U A_PRFlow:1;       //A正向无功寄存器溢出
  INT8U B_PRFlow:1;       //B正向无功寄存器溢出
  INT8U C_PRFlow:1;       //C正向无功寄存器溢出
  INT8U SUM_PRFlow:1;     //合相正向无功寄存器溢出 
  
  INT8U A_NAFlow:1;       //A反向有功寄存器溢出
  INT8U B_NAFlow:1;       //B反向有功寄存器溢出
  INT8U C_NAFlow:1;       //C反向有功寄存器溢出  
  INT8U SUM_NAFlow:1;     //合相反向有功寄存器溢出  
  INT8U A_NRFlow:1;       //A反向无功寄存器溢出
  INT8U B_NRFlow:1;       //B反向无功寄存器溢出
  INT8U C_NRFlow:1;       //C反向无功寄存器溢出  
  INT8U SUM_NRFlow:1;     //合相反向无功寄存器溢出    */
  INT8U CS[CS_BYTES];   
}MEASU_ERR;            //底层错误状态字

MeasuBase_PUCK_EXT volatile MEASU_ERR MeasuStatusMode;    //MeasuIC状态字模式字

//#define Set_MeasuErrMode_Struc_Sum() SET_STRUCT_SUM(MeasuStatusMode)


INT32U Measu_RdData(INT8U Reg);
void Measu_WrData(INT8U ParaReg,INT32U paradata);

INT8U Measu_RdCompData(INT32U LastData);
INT8U Measu_WrAndCompData(INT8U ParaReg,INT32U LastPara);

INT8U Measu_WrAndCompData(INT8U ParaReg,INT32U WrData);
INT8U Measu_RdAndCompData(INT8U ParaReg,INT8U DstBuf[]);

INT8U Measu_WrAndCompData_3Times(INT8U ParaReg,INT32U WrData);
INT8U Measu_RdAndCompData_3Times(INT8U ParaReg,INT8U DstBuf[]);
INT8U Measu_RdAndCompPara_3Times(INT8U ParaReg,INT8U DstBuf[]);
#endif
