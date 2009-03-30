#ifndef LOW_POWER_H
#define LOW_POWER_H

#undef EXT
#ifdef LOW_POWER_C
#define EXT
#else
#define EXT extern
#endif

#define POWER_ON_STATUS   0x55
#define POWER_DOWN_STATUS 0xAA

#define MAIN_INITING      0x5AAAA55A
#define MAIN_INITED       0

#define MAIN_INIT_END     do{Main_Init_Flag.Var=MAIN_INITED;}while(0)
//一、基本宏与函数
//定义中断唤醒源,每个独占一个位
#define IRAD_RESUME         0x01//红外唤醒
#define KEY_RESUME          0x02//按键唤醒
#define ALL_LOSS_VOL_RESUME 0x04//全失压唤醒
//#define POWERON_RESUME      0x08//外部电源上电
//#define BATON_RESUME        0x10//电池上电
#define RTC_RESUME          0x20//闹铃唤醒
#define LASER_RESUME        0x40//激光唤醒
#define ALL_RESUME          0x7F//所有唤醒源集合

//定义系统状态
#define SYS_NORMAL          0x55aa55aa//正常状态,即外部电源正常
#define SYS_SLEEP           0xaa55aa55//睡眠状态,电池供电
#define SYS_RESUME	    0xa5a5a5a5//唤醒状态,电池供电,可判断唤醒源
//#define SYS_NORMAL_TO_SLEEP 0x5a5a5a5a//正常到睡眠的过渡状态，是因为发生了掉电
//#define SYS_RESUME_TO_SLEEP 0x5555aaaa//唤醒到睡眠的过渡状态，因为唤醒后相关工作已经完成，因此进入睡眠
#define SYS_NULL            0x00FF00FF//定义不存储在的系统状态,可以认为是外部电源没电，电池也没电的状态
                                      // 
#define RESUME_SEC_COUNTS 120

#define PD_DATA_PROC() if(setjmp(PD_Proc_Env.Buf))\
                       { Save_PD_Data();\
                         Soft_Restart();}\
                       else\
                       { INIT_STRUCT_VAR(PD_Proc_Env);\
                         SET_STRUCT_SUM(PD_Proc_Env);}

#define JMP_PD_PROC()   if(CHECK_STRUCT_VAR(PD_Proc_Env) && CHECK_STRUCT_SUM(PD_Proc_Env))\
                          longjmp(PD_Proc_Env.Buf,1);\
                        else\
                        { DEBUG_PRINT(HUCK,DEBUG_0,"PD_Proc_Env sum error!");\
                          Save_PD_Data();\
                          Soft_Restart();}
                           
//定义系统状态
typedef struct
{
  INT8U Head;
  INT32U Status;//系统状态SYS_NORMAL、SYS_SLEEP、SYS_RESUME等
  INT32U Last_Status;//上次的系统状态
  //INT16U Resume_Sec_Counts;//唤醒的时间计数
  OS_INT32U CS;
  INT8U Tail;
}S_Sys_Status;

//定义任务睡眠标志
typedef struct
{
  INT8U Head;
  INT32U Sleep_Flag;//任务睡眠标志，每位表示一个任务，第0位表示ID为0的任务,其值为1表示已经睡眠，0表示唤醒状态
  INT8U CS[CS_BYTES];
  INT8U Tail;
}S_Task_Status;

//定义唤醒中断源表标志
typedef struct
{
  INT8U Head;
  INT8U Src_Flag;//中断源标志，每位代表一个中断源
  INT8U CS[CS_BYTES];
  INT8U Tail;
}S_Resume_Src;

typedef struct
{
  INT32U Head;
  jmp_buf Buf;
  INT8U CS[CS_BYTES];
  INT32U Tail;
}S_PD_Proc_Env;

EXT S_PD_Proc_Env PD_Proc_Env;
EXT volatile S_Task_Status Task_Status;
EXT NO_INIT volatile S_Sys_Status Sys_Status;
EXT volatile S_Resume_Src Resume_Src;
EXT volatile INT8U Resume_Time_Flag;
#ifdef LOW_POWER_C
EXT volatile S_Int32U Main_Init_Flag={CHK_BYTE,MAIN_INITING,CHK_BYTE};
#endif
extern volatile S_Int32U Main_Init_Flag;

#ifdef LOW_POWER_C
EXT volatile S_Int32U PD_Chk_Ms={CHK_BYTE,0,CHK_BYTE};//两次连续调用掉电监测函数间的计数
#else
extern volatile S_Int32U PD_Chk_Ms;
#endif

EXT INT8U PD_INT_EN_Flag;
//#define Power_Down_Check()//暂时将此函数设置为空，采用中断存数的方式

#define CHECK_SYS_STATUS_SUM() ((Sys_Status.Status+Sys_Status.Last_Status==Sys_Status.CS)?1:0)
#define SET_SYS_STATUS_SUM() (Sys_Status.CS=Sys_Status.Status+Sys_Status.Last_Status)

EXT void Soft_Restart();
EXT void Power_Down_Int_Proc();
EXT void Power_Down_Check();
EXT void Save_PD_Data();
EXT INT8U Check_Task_Sleep(INT8U Task_ID);
EXT INT8U Check_Power_Status();
EXT void Init_Task_Status();
EXT INT32U Check_Sys_Status();
EXT INT32U Get_Sys_Status();
EXT void Check_Sys_Status_Avail();
EXT void Wait_Tasks_Sleep(INT32U Ms);
EXT void Set_Sys_Status(INT32U Status);
EXT void Set_Resume_Source(INT8U Src);
EXT INT8U Check_Resume_Source(INT8U Src);
EXT void Init_Low_Power_Proc();
EXT void Task_Sleep();
EXT void Resume_Tasks();
EXT INT8U Check_Tasks_Sleep_Done();
EXT void Init_Low_Power_Ram();
EXT void Init_Resume_Source();
EXT void Low_Power_Proc();
EXT void Check_Low_Power_Data_Avail();
EXT void Init_Sys_Status();
#endif

