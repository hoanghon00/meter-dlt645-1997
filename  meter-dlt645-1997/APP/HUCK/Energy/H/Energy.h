#ifndef ENERGY_H
#define ENERGY_H

#undef EXT
#ifdef ENERGY_C
#define EXT
#else
#define EXT extern
#endif

#define POWER_ON_RST 0x5A
#define SOFT_RST 0xA5

//定义Cmp_Time函数返回值
#define TIME_EQU 0
#define TIME_AFT 1
#define TIME_BEF 2

//主副费率方案,Check_Rate_Scheme()函数的返回
#define RATE_SCHEME0 1
#define RATE_SCHEME1  2

#define CALC_RAM 0//计算费率或者费率方案时采用RAM中的参数
#define CALC_ROM 1//计算费率或者费率方案时采用ROM中的参数

//日标志定义，包括公共假日、周休日和一般日
#define HOLIDAY 0
#define WEEK_REST_DAY 1
#define NORMAL_DAY 2

//累加量的比率值,规约中的数据都定位2位小数，Cur_Energy中的数据都是3位小数
#define ENERGY_RATIO ((INT32U)100)//电能
#define COPPERLOSS_RATIO ((INT32U)3600*100000)//铜损
#define IRONLOSS_RATIO   ((INT32U)3600*1000000)//铁损
#define TRANSERR_RATIO   ((INT32U)3600)//互感器合成误差
#define AH_RATIO         ((INT32U)3600*100)//安时:1000----0.1AH;100-----0.01AH

#define MAX_ACTIVE_INC 50
#define MAX_REACTIVE_INC 50

//上电标志
typedef struct
{
  INT8U Flag[10];
  INT8U CS[CS_BYTES];
}S_Power_On_Flag;

//错误信息状态字
typedef struct
{
  INT8U ErrFlag;
  INT8U CS[CS_BYTES];
}S_Err_Info;

//电量结构体
typedef struct 
{
  INT8U Head;
  //INT8U Time[5];//冻结时间点,该时间在冻结时复制为冻结时间
  S_HEX_Time Time;//冻结时间点,该时间在冻结时复制为冻结时间
  
  INT8U Temp;//无用，用于上面结构体对齐
  
  INT32U  Pos_Active[MAX_RATES+1];//总以及分费率正向有功,三位小数,单位KWh，下同
  INT32U  Neg_Active[MAX_RATES+1];//总以及分费率反向有功
  
  INT32U  Quad_Reactive[4][MAX_RATES+1];//四象限分费率无功
  
  INT32U  Phase_Pos_Active[3];//三相正向有功
  INT32U  Phase_Neg_Active[3];//三相反向有功
 
  INT32U Phase_Quad_Reactive[3][4];//A、B、C、总的四象限无功,如Phase_Quad_Reactive[1][2]表示B相3象限无功
  
  INT32U  Phase_AH[4];//三相总AH

  INT32U  CopperLoss[4];//总ABC铜损
  INT32U  IronLoss[4];  //总ABC铁损
  INT32U  TransErr[4];  //总ABC互感器误差
  
  //INT32U  Phase_Reactive[3][4];//三相各四象限无功
  INT32U Data_Ready_Flag;
  
  INT8U   CS[CS_BYTES];//后面的不需要保存到Flash中
  
  INT8U Tail;
  //以下部分是根据以上量计算得，不需要保存到Flash或EERom中
  //INT32U Pos_Reactive[MAX_RATES+1];//总以及分费率正向无功
  //INT32U Neg_Reactive[MAX_RATES+1];

}S_Energy;




/*
//最大需量发生时间
//0-4字节分别是分、时、日、月、年
typedef struct
{
  INT8U Time[5];
  
  INT8U   Pos_Active[MAX_RATES+1][5];//正向有功最大需量发生时间
  INT8U   Neg_Active[MAX_RATES+1][5];//反向有功最大需量发生时间
  
  INT8U   Quad_Reactive[4][MAX_RATES+1][5];//四象限无功最大需量发生时间
  
  INT32U  Pos_Reactive[MAX_RATES+1][5];//反向无功(组合无功1)最大需量发生时间
  INT32U  Neg_Reactive[MAX_RATES+1][5];//反向无功(组合无功2)最大需量发生时间
  
  INT8U   CS[CS_BYTES];//校验和
}S_Demand_Time;
*/

DATA_PACKED_BEGIN
//周休日状态字
typedef struct
{
  INT8U Status;
  INT8U CS[CS_BYTES];
}S_WeekRest_Status;
//复费率相关参数
typedef struct
{
  INT8U Year_Period; //年时区数
  INT8U Date_Table;  //日时段表数
  INT8U Date_Period; //日时段数
  INT8U Rates;    //费率数
  INT8U Holidays; //节假日数
  INT8U WeekRest_DataTable;//周休日使用的时段表号
  INT8U WeekRest_Status;  //周休日状态字
  
  INT8U CS[CS_BYTES];
}S_Multi_Rate_Para;//复费率参数
//年时区表
typedef struct
{       /*"年时区结构体"*/
  INT8U MDN[MAX_YEAR_PERIODS][3];
  INT8U CS[CS_BYTES];
}S_Year_Table;
//日时段表
typedef struct
{       /*"日时段表结构体"*/
  INT8U HMN[MAX_DATE_PERIODS][3];
  
  //INT8U Rate_Scheme;//当前套费率方案
  //INT8U Index;//当前时段表索引号
  INT8U CS[CS_BYTES];
}S_Date_Table;

typedef struct
{
  INT8U Index;
  INT8U CS[CS_BYTES]; 
}S_Cur_Date_Table;
//-------------300 byte-----------------------
//节假日结构体
typedef struct
{       /*"节假日结构体"*/
  //INT8U WeekRest_DataTable;//节假日采用的日时段表号 
  INT8U MDN[MAX_YEAR_HOLIDAYS][3];
  INT8U CS[CS_BYTES];
}S_Holiday_Table;

//负荷曲线相关参数
typedef struct
{
  INT8U Mode;//负荷曲线模式字
  
  INT8U Start_Time[4];//记录的起始时间
  INT8U Rec_Period[6][2];//6类数据的记录周期
  INT8U CS[CS_BYTES];
  
}S_Load_Data_Para;
//无功模式字
typedef struct
{
  INT8U Mode[2];
  INT8U CS[CS_BYTES];
  
}S_Reactive_Com_Mode;

//当前模式字
typedef struct
{
  U_Byte_Bit Mode[9];
  INT8U CS[CS_BYTES];  
}S_Mode_Word;

//当前费率状态字
typedef struct
{
  U_Byte_Bit Status[2];
  INT8U CS[CS_BYTES];
}S_Rate_Status;

//自动抄表日参数,DDHH[0]表示小时,DDHH[1]表示日
typedef struct
{
  INT8U DDHH[2];
  INT8U CS[CS_BYTES];
}S_Data_Freeze_Time;

typedef struct
{
  INT8U Mode;//信号输出控制字
  INT8U CS[CS_BYTES];  
}S_Output_Mode;

//当前费率相关信息，主要用于打印输出
typedef struct
{
  INT8U Scheme;//当前使用的套费率方案
  INT8U Date_Flag;//HOLIDAY,WeekRest,NORMAL_DAY
  
  INT8U Holiday_Index; //第多少个假日
  INT8U Year_Period;  //年时区号
  
  INT8U Date_Table;//日时段表号
  INT8U Date_Period; //日时段号
  INT8U Rate; //当前费率
  
  INT8U CS[CS_BYTES];
}S_Cur_Rate_Info;


DATA_PACKED_END

typedef struct
{
  INT8U Flag;
  INT8U CS[CS_BYTES];
}S_Flag;

typedef struct
{
  INT8U Head;
 
  INT8U Err_Flag;//错误标志
  INT8U Tail;
}S_Sys_Run_Flag;

typedef struct
{
  INT32U Chk_Flag;
  INT8U CS[CS_BYTES];
}S_Chk_Data_Flag;

#define PUB_BUF_LEN ((sizeof(S_Energy)+50))//MAX_SIZE(sizeof(S_Energy),sizeof(S_Demand),sizeof(S_Demand_Time))
/*
typedef struct
{
  INT8U Head;
  INT8U Buf[PUB_BUF_LEN];
  INT8U Tail;
}S_Pub_Buf;
*/
DECLARE_BUF_TYPE (INT8U,PUB_BUF_LEN,S_Pub_Buf);
//EXT volatile INT16U Pub_Buf[PUB_BUF_LEN];//从ROM中读取数据时使用该buf
EXT S_Pub_Buf _Pub_Buf;
#define Pub_Buf _Pub_Buf.Buf

EXT volatile INT8U Rst_Flag;

EXT volatile S_Err_Info Err_Info;    //错误信息状态字

EXT volatile S_Chk_Data_Flag Chk_Data_Flag;
EXT NO_INIT volatile S_Power_On_Flag Power_On_Flag;   //上电或复位标志

EXT volatile S_Mode_Word Mode_Word;           //当前无功模式字
EXT volatile S_Rate_Status Rate_Status;       //当前费率状态字
EXT volatile S_HEX_Time Rate_Scheme_Switch_Time;   //主副费率切换时刻

EXT NO_INIT volatile S_Multi_Rate_Para Multi_Rate_Para;//当前费率方案参数
EXT volatile S_Year_Table Year_Table;          //当前年时区表
EXT volatile S_Date_Table Date_Table;          //当前日时段表

EXT volatile S_Data_Freeze_Time Data_Freeze_Time;//自动抄表日参数

EXT volatile S_Cur_Date_Table Cur_Date_Table;//当前日时段表索引号

EXT NO_INIT volatile S_Energy Cur_Energy;     //当前电量数据结构体

#if USE_ENERGY_RAM_BAK
EXT NO_INIT volatile S_Energy Cur_Energy_Bak;     //当前电量数据结构体
#endif


EXT volatile S_Output_Mode Output_Mode;
EXT volatile S_Cur_Rate_Info Cur_Rate_Info;//当前费率相关信息,主要是供调试用
EXT volatile S_Sys_Run_Flag Sys_Run_Flag;
EXT volatile S_Int32U Energy_Cumu_Counts;

EXT INT8U Check_Rst_Flag();
EXT void Main_Chk_Data_Avail();
EXT void Check_Pub_Var_Avail();
EXT void Check_Cur_Energy_Data();
EXT INT8U Get_Using_Rate();
EXT void Read_Mode_Word();

EXT void Read_Energy_Para_From_Rom();
EXT void Read_Energy_Data_From_Rom();

EXT void Calc_Pos_Neg_Reactive(S_Energy *pEnergy);
EXT void Get_Cur_Date_Table(INT8U Rate_Scheme,INT8U Date_Table_Index);

EXT void Rate_Scheme_Changed_Proc();
EXT void Rate_Changed_Proc();

EXT INT8U Check_Energy_Data(S_Energy *pEnergy);
EXT void Check_Energy_Data_Avail();
EXT void Check_Data_Avail();
EXT void Check_Energy_Demand_Task_Data_Avail();
EXT void Freeze_Energy_Data(S_HEX_Time *pTime);

EXT void Save_Cur_Energy_Data();
EXT void Save_Cur_Energy_Bak0_Data();
EXT void Save_Cur_Energy_PD_Data();
EXT void Clear_Energy_Data();
EXT void Set_Def_Energy_Data();

EXT void Read_Energy_Data_Para_For_Show();
EXT void Energy_Task_Ram_Init();
EXT void Read_Para_From_Rom();
EXT void Refresh_Para_From_Rom();
EXT void Energy_Demand_Task_Init();
EXT void Energy_Demand_Task(INT8U Flag);
EXT void Last_Power_Down_Proc();
#endif
