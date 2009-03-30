#ifndef DATA_TRANS_H
#define DATA_TRANS_H

#include "Authority.h"
#include "Demand.h"

#undef EXT
#ifdef DATA_TRANS_C
#define EXT
#else
#define EXT extern
#endif

#define CH_RS485_1  CHANEL_485_1//0//第一路485
#define CH_RS485_2  CHANEL_485_2//1//第二路485
#define CH_MODULE   2//网络模块
#define CH_IRAD     CHANEL_IRDA//3//红外


#define MAX_SINGLE_DATA_LEN 10     //定义DLT645最大单项数据长度
#define MAX_DLT645_DATA_LEN 230    //DLT645数据域最大长度
#define AUTHORITY_FLAG 0x55aa55aa  //权限标志

#define SPEC_NO 0           //非特殊项
#define SPEC_RATE 1         //费率相关项,0xXXXX中的最后一个X与费率数相关
#define SPEC_HOLIDAY 2      //节假日相关
#define SPEC_PHASE_ENERGY 3 //分相电能
#define SPEC_LOAD_DATA 4    //负荷曲线
#define SPEC_POW_DIRECT 5   //负荷曲线数据使用的功率方向
#define SPEC_CUR_TIME 6     //当前时间点(带年)
#define SPEC_DATE     7     //当前日期(不带年)
#define SPEC_TIME     8     //当前时间
#define SPEC_ADJ_CLR  9     //校表清0
#define SPEC_FREEZE_DATA 10 //冻结数据(当前数据)
#define SPEC_MULTI_DATA 11 
#define SPEC_CUMU_EVENT 12   //累计电量相关数据项

#define SPEC_PSW_LOCK   13   //自锁状态字
#define SPEC_VOLSTAT    14   //电压统计数据
#define SPEC_BAUD       15   //波特率状态字
#define SPEC_EVENT_TIME 16   //事件发生时间
#define SPEC_BAT_TIME   17   //电池工作时间
#define SPEC_RD_ROM     18   //读ROM数据
#define SPEC_YEAR_PERIOD 19 //年时区表
#define SPEC_DATE_PERIOD 20 //日时段表
#define SPEC_MULTI_EVENT 21 //分次事件
#define SPEC_SET_ENSTART 22 //设置初始电量
#define SPEC_EXTEND      99 //扩展数据

//定义错误信息状态字
#define ILLEGAL_DATA_ERR 0//非法数据
#define DI_ERR 1          //数据标识错
#define PSW_ERR 2         //密码错误
#define YEAR_PERIOD_ERR 4 //年时区数超
#define DATE_PERIOD_ERR 5 //日时段数超
#define RATES_NUM_ERR   6 //费率数超

//调用获取协议数据的函数时入口Data_Flag值
#define FOR_DISP 1//表示为显示
#define FOR_COMM 2//表示为通信,电能等小数位位数按规约来
#define FOR_EVENT 4//表示为事件,费率相关数据都是最大费率数
#define FOR_SRC 8

#define CLEAR_ALL_FLAG     0x05AA5ABC//总清标志
#define CLEAR_ENERGY_FLAG  0x0055AABB//清电量标志 
#define CLEAR_DEMAND_FLAG  0xAABBCCDD//清需量标志
#define CLEAR_VOLSTAT_FLAG 0x11223344//清电压统计标志
#define CLEAR_EVENT_FLAG   0x33445566//清事件标志
#define CLEAR_END_FLAG     0x00000000//无数据清除标志

//定义设置数据时的返回值
//#define SET_ERR 0           //设置时发生错误      
//#define SET_OK  1           //设置成功
//#define SET_NOT_SPEC_DATA 2 //特殊Set_DLT645_Spec_Data函数返回时表示该数据项不是特殊项
//#define SET_NO_DATA 3
//#define SET_AUTH_ERR 4

//定义规约种类
#define PROTOCOL_DLT645 0 

typedef struct{
  INT8U Energy_Digs;
  INT8U Power_Digs;
  INT8U CS[CS_BYTES];
}S_Disp_Digs;//定义功率和电能的显示位数

typedef struct{
  INT8U Addr[3][6];
  INT8U CS[CS_BYTES];
}S_Meter_Addr;

typedef struct
{
  INT8U Head;
  INT8U Ch;
  INT8U Tail;
}S_Cur_Comm_Ch;

//定义源数据存储类型
#define S_RAM 0x00
#define S_ROM 0x01

///定义源数据类型,通过645设数据时，可根据一下类型判断,S_BCD类型的数据要求一定是BCD类数据
//必须进行BCD类型检查
#define S_INTS 0x01//是否有符号INT类型
#define S_INTU 0x02//是否无符号INT类型
#define S_BCD  0x03//是否为BCD类型,这种情况下源数据->目标数据无需进行格式转换
#define S_BIN  0x04//二进制状态字之类的

//定义源数据长度 
#define S_LEN8 0x08
#define S_LEN7 0x07
#define S_LEN6 0x06
#define S_LEN5 0x05
#define S_LEN4 0x04
#define S_LEN3 0x03
#define S_LEN2 0x02
#define S_LEN1 0x01

//定义目标数据在取数时的起始字节
#define D_START4 0x04
#define D_START3 0x03
#define D_START2 0x02
#define D_START1 0x01
#define D_START0 0x00

//定义目标数据的长度
#define D_LEN19 0x04
#define D_LEN18 0x03
#define D_LEN17 0x02
#define D_LEN16 0x01
#define D_LEN15 0x00
#define D_LEN14 0x04
#define D_LEN13 0x03
#define D_LEN12 0x02
#define D_LEN11 0x01
#define D_LEN10 0x00
#define D_LEN9 0x04
#define D_LEN8 0x03
#define D_LEN7 0x02
#define D_LEN6 0x01
#define D_LEN5 0x00
#define D_LEN4 0x04
#define D_LEN3 0x03
#define D_LEN2 0x02
#define D_LEN1 0x01
#define D_LEN0 0x00


//#define D_S_SAME 0x80

#define DI_EQU_NULL 0

//用户权限标志字
typedef struct
{
  INT8U Head;
  INT32U Flag; 
  INT8U CS[CS_BYTES];
  INT8U Tail;
}S_User_Authority_Flag;

typedef struct{
 INT8U Src_Off;
 INT8U Src_Len;
 INT8U Src_Format;
 
 INT8U Dst_Start;
 INT8U Dst_Len; 
}S_Data_Item;

typedef struct{
  INT8U Num;
  S_Data_Item Data_Item[25]; 
}S_Multi_Data;

typedef struct{
  INT8U Time[5];//最近一次编程时间
  INT32U Counts;//编程次数
  INT8U CS[CS_BYTES];
}S_Prog_Record;

typedef struct
{
  INT8U Time[5];//最近一次最大需量清0时间
  INT32U Counts;//需量清0次数
  INT8U CS[CS_BYTES];
}S_Clr_Demand_Record;

EXT S_Prog_Record Prog_Record;//编程次数和时间
EXT S_Clr_Demand_Record Clr_Demand_Record;//清需量次数和时间

//定义当前通信通道
#ifdef DATA_TRANS_C
EXT S_Cur_Comm_Ch Cur_Comm_Ch={CHK_BYTE,0,CHK_BYTE};
#endif
extern S_Cur_Comm_Ch Cur_Comm_Ch;

typedef struct{
  PROTO_DI DI;   //645协议的ID,该ID也是存储ID
  PROTO_DI DI_Equ;//与其DI完全等效的标示符，用于扩充协议于标准645协议对应

  INT8U DI_Set_Flag:1;//DI数据项是否进入数据集中
  INT8U DI_Equ_Set_Flag:1;//DI_Equ是否进入数据集中?
        
  INT8U PSW_Flag:4;       //修改的密码等级
  
  INT8U Storage:2;        //源存储属性,S_ROM或者S_RAM
  //INT16U Storage_DI;    //如果存储在Rom中，Storage_DI表示存储的编号
  
  volatile void * pSrc; //最终源数据的指针
  INT16U Src_Off;       //从pSrc中取源数据的偏移
  
  INT8U Src_Len;        //源数据长度,当源数据为S_INTU或S_INTS时可根据该长度得知数据具体类型	
  INT8U Src_Format;     //源数据的格式
  
  INT8U Dst_Start;      //目标数据在源数据的BCD缓冲中取数据的起始位置,如果源
                              //数据是S_INTU数据类型,则目标数据的应该是源数据/Dst_Start后转化为BCD码
  INT8U Dst_Len;        //目标数据的提取长度,如果是S_BCD类型,则直接从源数据的Dst_Start字节处开始取数

  INT16U Num;            //与此数据类似的数据项个数
        
  INT8U Spec_Flag;      //是否特殊数据项？如果是 Num域则是作为特殊项的一个参数
}S_Protocol_Data_Info;


//EXT volatile S_Prog_Flag Prog_Flag;//编程标识字
////定义扩充的DI,扩充的数据标识都以0x01开始,如果该数据存储在ROM中，则Data_Para_Storage中也定义该数据项//
//以下数据全部从当前ram里面取，以免和读取存储数据冲突
#define DI_REG_CKC                 0x0001
#define DI_REG_CSC                 0x0002

#define DI_LOW_VOL                 0x0100
#define DI_CURRENT_QUADRANT        0x0101
#define DI_METER_PERIPHERY         0x0102
#define DI_MODULE_STATUS           0x0103
#define DI_POWER_DOWN_TIME         0x0104

#define DI_METER_ADDR0             0x0105//第一路表地址,和METER_ADDR0一样
#define DI_METER_ADDR1             0x0106//第二路表地址
#define DI_METER_ADDR2             0x0107//第三路表地址

#define DI_COMM_BAUD0              0x0108//第一路波特率,和COMM_BAUD0一样
#define DI_COMM_BAUD1              0x0109//第二路波特率
#define DI_COMM_BAUD2              0x010A//第三路波特率

#define DI_METER_SOFTWARE_VERSION  0x0110//软件版本号
#define DI_METER_SPECIFI           0x0111//电表规格

///一下数据是为负荷曲线增加,不要再此范围增加其他数据
#define DI_ACTIVE_DIRECT           0x0120//有功方向
#define DI_REACTIVE_DIRECT         0x0121//无功方向

//#define _DI_ACTIVE_DIRECT          0x0122//有功方向，包括总
//#define _DI_REACTIVE_DIRECT        0x0123//无功方向，包括总

#define DI_REACTIVE_ALL            0x0130//总无功功率
#define DI_REACTIVE_A              0x0131//A、B、C三项无功功率
#define DI_REACTIVE_B              0x0132
#define DI_REACTIVE_C              0x0133

#define DI_CUR_TIME                0x0134//当前BCD时间 年月日时分

#define DI_ACTIVE_DEMAND           0x0140//有功需量
#define DI_REACTIVE_DEMAND         0x0141//无功需量

#define _DI_TOTAL_AH                0x0142//总AH,两位小数
#define _DI_A_AH                    0x0143//A相总AH
#define _DI_B_AH                    0x0144//B相总AH
#define _DI_C_AH                    0x0145//C相总AH

#define DI_TOTAL_AH                0x0151//总AH
#define DI_A_AH                    0x0152//A相总AH
#define DI_B_AH                    0x0153//B相总AH
#define DI_C_AH                    0x0154//C相总AH

#define DI_A_VOL                   0x0155//A相电压,2位小数,3字节
#define DI_B_VOL                   0x0156//B相电压,2位小数
#define DI_C_VOL                   0x0157//C相电压,2位小数

#define DI_A_CUR                   0x0158//A相电流,4位小数,3字节
#define DI_B_CUR                   0x0159//B相电流,4位小数
#define DI_C_CUR                   0x015A//C相电流,4位小数

#define DI_A_ANGLE                 0x015B//A相相角,2位小数,3字节
#define DI_B_ANGLE                 0x015C//B相相角,2位小数,3字节
#define DI_C_ANGLE                 0x015D//C相相角,2位小数,3字节
#define DI_SUM_ANGLE               0x015E//总相角,2位小数,3字节

#define DI_PROG_FLAG0              0x0160//编程标识字1
#define DI_PROG_FLAG1              0x0161//编程标识字2

#define DI_ERR_TIME                0x0162//错误的时钟数据
#define DI_ERR_ROM                 0x0163//EEROM错误标识字

#define DI_CUR_DEMAND              0x0164//当前需量
#define DI_CLR_DEMAND_WAY          0x0165//清需量方式
#define DI_ADJ_FLAG                0x0166//校表标识字
#define DI_NULL_1BYTE              0x0167//无意义的标识字 

#define DI_EVENT_VOL               0x0168//电压超限或者过压时的电压值

#define DI_ALL_LOSS_VOL_START_TIME 0x0170//全失压起始时间
#define DI_ALL_LOSS_VOL_END_TIME   0x0171//全失压结束时间

#define DI_EVENT_TIME              0x0172//事件发生时间

#define DI_A_VOL_1                 0x0173//A相电压,1位小数,3字节
#define DI_B_VOL_1                 0x0174//B相电压,1位小数
#define DI_C_VOL_1                 0x0175//C相电压,1位小数

#define DI_A_CUR_3                 0x0176//A相电流,3位小数,3字节
#define DI_B_CUR_3                 0x0177//B相电流,3位小数
#define DI_C_CUR_3                 0x0178//C相电流,3位小数

#define DI_PROG_TIME               0x0179//最近一次编程时间

#define DI_BROAD_ADJ_TIME          0x017A //定义广播校时DI,协议传输中暂时未用


#define DI_EXTNED_START            0x0300//扩充的DI的起始
/////负荷曲线补充结束

////定义扩充的DI结束///////

//注意，读取CUR_ENERGY或者HIS_ENERGY为ID的数据后要根据当前的模式字计算当前的组合无功
//HIS_ENERGY或HIS_DEMAND或HIS_DEMAND_TIME数据都需要特殊处理
//


extern CONST S_Multi_Data Loss_Vol_Data;
extern CONST S_Multi_Data Loss_Cur_Data;
extern CONST S_Multi_Data Phase_Reverse_Data;
extern CONST S_Multi_Data Phase_Break;

#define CHK_PARA_NUM 15
typedef struct{
  INT8U Head;
  INT8U Posi[OS_TASK_NUM];
  PROTO_DI Buf[OS_TASK_NUM][CHK_PARA_NUM];
  INT8U CS[CS_BYTES];
  INT8U Tail;
}S_Para_Modify;

EXT volatile S_Para_Modify Para_Modify;

EXT volatile S_Disp_Digs Disp_Digs;//定义功率和电能的显示位数
EXT volatile S_Meter_Addr Meter_Addr;
EXT volatile S_User_Authority_Flag User_Authority_Flag;

typedef struct{
  INT8U Head;
  INT32U Flag;
  INT8U CS[CS_BYTES];
  INT8U Tail;
}S_Clear_Flag;

typedef struct{
  INT8U Head;
  INT8U Set_En_Mins;
  INT8U Err_Times;
  INT16U Clr_Err_Times_Mins;//清错误次数剩余时间
  INT16U Lock_Mins;//锁定时间，就是解锁剩余时间
  INT8U CS[CS_BYTES];
  INT8U Tail;
}S_PSW_Err_Info;

extern const S_Protocol_Data_Info Protocol_Data[];
EXT volatile S_Clear_Flag Clear_Flag;
EXT volatile S_PSW_Err_Info PSW_Err_Info;
EXT volatile S_Int8U Time_Modify_Flag;

EXT void Init_Data_Trans_Ram_Para();
EXT void Check_Data_Trans_Data_Avail();
EXT void Read_Data_Trans_Para_From_Rom();
EXT void Set_Para_Modify(PROTO_DI DI);
EXT INT8U Check_Para_Modify(PROTO_DI DI);
EXT INT8U Get_Para_Modify(PROTO_DI *pDI);
EXT INT32U Pow_10x(INT8U a);

EXT INT16U Get_Protocol_Data_Index(PROTO_DI DI);
EXT INT8U Get_Protocol_Data_PSW_Flag(PROTO_DI DI);
EXT INT8U Get_Energy_Disp_Digs();
EXT INT16U Get_Multi_Item_Data(INT8U *pSrc,S_Multi_Data *p,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen);
EXT INT16U _Get_DLT645_Single_Data(INT16U Index,PROTO_DI DI,void *pPara,INT8U ParaLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pErr);
EXT INT16U Get_DLT645_Single_Data(INT16U Index,PROTO_DI DI,void *pPara,INT8U ParaLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pErr);
EXT INT16U _Get_DLT645_Data(PROTO_DI DI,void *pPara,INT8U ParaLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pNextFlag,INT8U *pErr);
EXT INT16U Get_DLT645_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U *pErr);
EXT INT16U Get_DLT645_Data_For_Disp(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen);
EXT INT16U _Get_DLT645_Normal_Data(PROTO_DI DI,void *pPara,INT8U ParaLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pFollowFlag,INT8U *pErr);

EXT INT16U Simple_Search_Frame(INT8U *pSrc,INT16U SrcLen,INT16U *pOffset);
EXT INT16U Rcv_DLT645_Data_Proc(INT8U Ch,INT8U *pFrame,INT8U FrameLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen);
EXT INT8U Check_Protocol_Data_Single(INT16U Index,PROTO_DI DI,INT8U Data_Flag,INT8U *pSpec_Flag);
EXT INT8U Check_Protocol_Data_Storage();
EXT INT8U Get_DLT645_Data_Storage_Type(PROTO_DI DI);
EXT INT16U Search_Protocol_Frame(INT8U *pSrc,INT16U SrcLen,INT16U *pOffset);
EXT INT16U Rcv_PROTOCOL_Frame_Proc(INT8U Ch,INT8U *pSrc,INT16U SrcLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen);

EXT INT8U Adj_Time(S_BCD_Time *pTime);
EXT void Wait_Event_Data_Proc_End(INT8U Event_ID);
EXT void Set_Clear_Data_Flag(INT32U Flag);
EXT INT32U Check_Clear_Data_Flag();
EXT void Clear_Data_End();
#endif
