#define PORT_ABS_PUCK
#include "Pub_PUCK.h"
#include "Includes.h"


/********************************************************************************
void  Get_Meter_Hard_Mode(void)
函数功能：获取表计跳线模式
入口：  
返回：
    无
********************************************************************************/
INT8U Get_Meter_Hard_Mode(void)
{
  if(B_TEST_FAC_STATUS && B_TEST_HARD_STATUS)  //大多数是这种模式，
    return MODE_RUN;
  
  if(B_TEST_FAC_STATUS && B_TEST_HARD_STATUS==0)
    return MODE_DEBUG;
  
  if(B_TEST_FAC_STATUS==0 && B_TEST_HARD_STATUS)
    return MODE_FAC;
  
  if(B_TEST_FAC_STATUS==0 && B_TEST_HARD_STATUS==0)  //这种模式最少
    return MODE_TEST;  
  
  //异常！
  return MODE_RUN;
}


/********************************************************************************
********************************************************************************/

void PORT_ChangeP142Output_PUCK(INT8U Flag)
{
  PORT_ChangeP142Output(0,Flag);
  
}

#pragma pack(1)
typedef struct
{
  INT8U NeedInit:1;                 //需要初始化标志
  INT8U InitLevel:1;                //正常工作下，闲置时的电平值；
  INT8U SleepLevel:1;               //低功耗情况下，需要初始的电平值；
  INT8U ValidLevel:1;               //电平模式下，输出有效的电平值；
  INT8U MultiFlag:2;                //是否是复合端子标志：如3合1，0-------不是复合端子；1------是复合端子；2------特殊端子
  INT8U LevelProprty:1;             //该端子是电平端子
  INT8U LoopFlag:1;                 //仅对脉冲方式有效 
  void  (* const FuncPtr)(INT8U);
  INT8U (* const MultiPortEn)(void); //多合一端子(MultiFlag为1时)，根据此条件判定输出
}CONST_PORT_STATUS;

//以下对所有输出口的抽象定义
CONST_PORT_STATUS  Const_Port_Status[MAX_PORT_NUM]={
  {0},
  {1,0,0,0,0,LEVEL_MODE,0,&PORT_ChangeP81Output,&Get_GridAlarm_Out_En},    //电网信息报警输出(如超功率)------------------------------1
  {1,0,0,0,0,LEVEL_MODE,0,&PORT_ChangeP81Output,&Get_FeeAlarm_Out_En},    //预付费欠费报警------------------------------------------2
  {1,0,0,0,0,LEVEL_MODE,0,&PORT_ChangeP80Output,0},    //超需量跳闸输出----------------------------------------------------------3
  {0,1,0,0,0,PULSE_MODE,0,&PORT_ChangeP66Output,0},    //有功脉冲输出------------------------------------------------------------4
  {0,1,0,0,0,PULSE_MODE,0,&PORT_ChangeP67Output,0},    //无功脉冲输出------------------------------------------------------------5
  {0,1,0,0,0,LEVEL_MODE,0,&PORT_ChangeP64Output,0},    //有功方向输出------------------------------------------------------------6
  {0,1,0,0,0,LEVEL_MODE,0,&PORT_ChangeP65Output,0},    //无功方向输出------------------------------------------------------------7
  {1,1,0,0,0,LEVEL_MODE,0,&PORT_ChangeP82Output,&Get_Sec_Out_En},    //秒脉冲使能------------------------------------------------8
  
  {1,1,0,0,2,PULSE_MODE,0,&PORT_ChangeP83Output,&Get_Demand_Out_En},    //需量周期到输出-----------------------------------------9
  {1,1,0,0,2,PULSE_MODE,0,&PORT_ChangeP83Output,&Get_Slipe_Out_En},     //滑差周期到输出-----------------------------------------10 
  
  {1,1,0,0,0,PULSE_MODE,0,&PORT_ChangeP84Output,&Get_Parse_Out_En},    //时段输出-----------------------------------------------11
  {0,1,0,0,0,PULSE_MODE,0,&PORT_ChangeP150Output,0},   //总有功脉冲灯----------------------------------------------12
  {0,1,0,0,0,PULSE_MODE,0,&PORT_ChangeP151Output,0},   //总无功脉冲灯----------------------------------------------13
#if METER_HARD_TYPE==HARD_TYPE_20081005
  {1,0,0,1,0,PULSE_MODE,0,&PORT_ChangeP54Output,0},    //报警指示灯------------------------------------------------14
#endif
#if METER_HARD_TYPE>=HARD_TYPE_20090224
  {1,0,0,1,0,PULSE_MODE,0,&PORT_ChangeP157Output,0},    //报警指示灯-----------------------------------------------14
#endif
  {1,0,0,1,0,PULSE_MODE,0,&PORT_ChangeP142Output_PUCK,0}  //喇叭报警 ----------------------------------------------15
};

#pragma pack()

/********************************************************************************
INT8U  Get_Sec_Out_En(void)
函数功能：根据模式字，获取秒脉冲输出使能
入口：  
返回：
    无
********************************************************************************/
INT8U Get_Sec_Out_En(void)
{
  INT8U Err,Len;
  INT8U Temp;
  
  Len=Get_DLT645_Data(0xCA12,&Temp,&Temp,1,&Err);
  if(NO_ERR==Err && 1==Len)
  {
    if(Temp&0x03==0x00)
     return 1;  
  }
  return 0;  
}
/********************************************************************************
INT8U  Get_Parse_Out_En(void)
函数功能：根据模式字，获取时段输出使能
入口：  
返回：
    无
********************************************************************************/
INT8U Get_Parse_Out_En(void)
{
  INT8U Err,Len;
  INT8U Temp;
  
  Len=Get_DLT645_Data(0xCA12,&Temp,&Temp,1,&Err);
  if(NO_ERR==Err && 1==Len)
  {
    if(Temp&0x03==0x01)
     return 1;  
  }  
  return 0;
}
/********************************************************************************
INT8U  Get_Demand_Out_En(void)
函数功能：根据模式字，获取需量输出使能
入口：  
返回：
    无
********************************************************************************/
INT8U Get_Demand_Out_En(void)
{
  INT8U Err,Len;
  INT8U Temp;
  
  if(Const_Port_Status[EXT_ID_DEMAND].MultiFlag!=Const_Port_Status[EXT_ID_DEMAND].MultiFlag)  //不管怎样，就是一个端子，所以属性一直
     ASSERT(A_WARNING,0);
     
  if(Mode_Word.Mode[2].Bit.Bit4==0)  //模式字3 bit4:需量信号输出方式,1:按需量周期,0:按滑差时间
    return 0;
  
  if(Const_Port_Status[EXT_ID_DEMAND].MultiFlag==2)  //不是复合端子，特殊端子
      return 1;
  
  Len=Get_DLT645_Data(0xCA12,&Temp,&Temp,1,&Err);
  if(NO_ERR==Err && 1==Len)
  {
    if(Temp&0x03==0x02)
     return 1;  
  }  
  return 0;
}
/********************************************************************************
INT8U  Get_Slipe_Out_En(void)
函数功能：根据模式字，获取滑差时间到输出使能
入口：  
返回：
    无
********************************************************************************/
INT8U Get_Slipe_Out_En(void)
{
  INT8U Err,Len;
  INT8U Temp;
  
  if(Const_Port_Status[EXT_ID_DEMAND].MultiFlag!=Const_Port_Status[EXT_ID_DEMAND].MultiFlag)  //不管怎样，就是一个端子，所以属性一直
     ASSERT(A_WARNING,0);
  
  if(Mode_Word.Mode[2].Bit.Bit4==1)  //模式字3 bit4:需量信号输出方式,1:按需量周期,0:按滑差时间
    return 0;
   
  if(Const_Port_Status[EXT_ID_SLIPE].MultiFlag==2) //不是复合端子，特殊端子
      return 1;
      
  Len=Get_DLT645_Data(0xCA12,&Temp,&Temp,1,&Err);  //复合端子
  if(NO_ERR==Err && 1==Len)
  {
    if(Temp&0x03==0x02)
     return 1;  
  }  
  return 0;
}
/********************************************************************************
INT8U  Get_FeeAlarm_Out_En(void)
函数功能：根据模式字，获取欠费报警输出使能
入口：  
返回：
    无
********************************************************************************/
INT8U Get_FeeAlarm_Out_En(void)
{
  INT8U Err,Len;
  INT8U Temp;
  
  Len=Get_DLT645_Data(0xCA12,&Temp,&Temp,1,&Err);
  if(NO_ERR==Err && 1==Len)
  {
    if(Temp&0x03==0x03 && Mode_Word.Mode[1].Bit.Bit5==1)   //模式字2 bit5＝1，预付费功能
     return 1;  
  }  
  return 0;
}
/********************************************************************************
INT8U  Get_GridAlarm_Out_En(void)
函数功能：根据模式字，获取滑差时间到输出使能
入口：  
返回：
    无
********************************************************************************/
INT8U Get_GridAlarm_Out_En(void)
{
  INT8U Err,Len;
  INT8U Temp;
  
  Len=Get_DLT645_Data(0xCA12,&Temp,&Temp,1,&Err);
  if(NO_ERR==Err && 1==Len)
  {
    if(Temp&0x03==0x03 && Mode_Word.Mode[1].Bit.Bit4==1)  //模式字2 bit4＝1，负控功能功能
     return 1;  
  }  
  return 0;
}

/********************************************************************************
void void Init_All_Device_Port(INT8U Mode)
函数功能：初始化所有端口,
入口：   根据不同状态，设定端口状态
返回：
    无
********************************************************************************/
void Init_All_Port(INT32U Mode)
{
   INT8U i;
   
  for(i=MIN_PORT_DI;i<=MAX_PORT_DI;i++)
  {
    if(0==Const_Port_Status[i].NeedInit)
    {
      Ext_Port_Hander[i].InitFlag=1;
      continue;
    }
    
    if(SYS_NORMAL==Mode)
    {
      Const_Port_Status[i].FuncPtr(Const_Port_Status[i].InitLevel);    //正常模式：置输出为无效位
    }
    //其他模式，已经在Init_All_IO_XX()函数处理。
    
    Ext_Port_Hander[i].InitFlag=1;
  }
}
/********************************************************************************
void Port_Level_Out(INT8U Id,INT16U StartOrEnd)
函数功能：对外部端子排的电平输出操作函数
入口：
    Id -------外部端子功能Id号码
    StartOrEnd---------输出高电平还是低电
返回：
    无
注意：函数入口参数和Port_Pulse_Out保持一致！
********************************************************************************/
void Port_Level_Out(INT8U Id,INT8U Level)
{
  if(Id<MIN_PORT_DI || Id>MAX_PORT_DI)
    return ;
 
  if(Const_Port_Status[Id].LevelProprty==PULSE_MODE)
    return ;
  
  //电平模式输出,直接对端口输出电平
  if(Const_Port_Status[Id].ValidLevel==Level)  //启动
  {
    Const_Port_Status[Id].FuncPtr(Const_Port_Status[Id].ValidLevel);
  }
  else
  {
      Const_Port_Status[Id].FuncPtr(!Const_Port_Status[Id].ValidLevel);
  }
}

/********************************************************************************
void Ext_Port_Out(INT8U Id,INT16U MsCounts)
函数功能：对外部端子排的冲输出操作函数脉
入口：
    Id -------外部端子功能Id号码
    PulseWidth ---外部端子为脉冲输出时，脉冲宽度，单位ms，电平输出时，此参数无效
返回：
    无
********************************************************************************/
void Port_Pulse_Out(INT8U Id,INT16U PulseWidth)
{
  if(Id<MIN_PORT_DI || Id>MAX_PORT_DI)
    return ;
  
 
  //脉冲模式输出，端口输出在ExtPort_xMs_Hook中调用。  
  if(PulseWidth==0)  //清除端子的临时输出
  {
    Ext_Port_Hander[Id].Status=0;
    Ext_Port_Hander[Id].CurCount=0;
    Ext_Port_Hander[Id].CurCountBak=0;
    Ext_Port_Hander[Id].LoopCount=0;
    return ;
  }
  
  if(Const_Port_Status[Id].LoopFlag)
    Ext_Port_Hander[Id].LoopCount++;
  else
    Ext_Port_Hander[Id].LoopCount=1;
  
  Ext_Port_Hander[Id].CurCountBak=PulseWidth/MS_HOOK_NUM;   //每次都更新一下
  if(Ext_Port_Hander[Id].LoopCount==1)   //首次启动
  {    
    Ext_Port_Hander[Id].Status=1;
  }
}
/********************************************************************************
void Port_Out_Pub(INT8U Id,INT16U Para)
入口：
    Id -------端子功能Id号码
    Para -------对于电平脉冲，PORT_START 表示激活有效电平；PORT_END 为无效电平；对于脉冲输出，表示脉冲宽度
返回：
    无
********************************************************************************/
void Port_Out_Pub(INT8U Id,INT16U Para)
{
  if(Id<MIN_PORT_DI || Id>MAX_PORT_DI)
    return ;
  
  if(Const_Port_Status[Id].MultiFlag) //不是复合端子
  {
    if(Const_Port_Status[Id].MultiPortEn==0 || Const_Port_Status[Id].MultiPortEn()==0) //是复合端子，但不允许该端子输出
      return ;
  }
    
  if(Const_Port_Status[Id].LevelProprty==LEVEL_MODE)      //电平方式
  {
    if(PORT_START==Para)  //启动：输出有效电平
      Port_Level_Out(Id,Const_Port_Status[Id].ValidLevel);
    
     if(PORT_END==Para)   //启动：输出无效电平
      Port_Level_Out(Id,!(Const_Port_Status[Id].ValidLevel));
     
     return ;
  }
  
  //脉冲方式
  Port_Pulse_Out(Id,Para); 
}
/********************************************************************************
void Beep_For_Measu_Alarm_PUCK(void)
函数功能：蜂鸣器报警函数
入口：
返回：
    无
********************************************************************************/
void Beep_For_Measu_Alarm_PUCK(void)
{
  //不叫，事件在事件记录中读出供分析
 //Port_Out_Pub(INTER_ID_ALARM_BEEP,BEEP_MODE_100);
 //Port_Out_Pub(INTER_ID_ALARM_DIS,150); 
}
/********************************************************************************
void Beep_For_OSC_Alarm_PUCK(void)
函数功能：蜂鸣器报警函数
入口：
返回：
    无
********************************************************************************/
void Beep_For_OSC_Alarm_PUCK(void)
{
 Port_Out_Pub(INTER_ID_ALARM_BEEP,BEEP_MODE_150); 
 Port_Out_Pub(INTER_ID_ALARM_DIS,150); 
}
/********************************************************************************
void Beep_For_Test_Alarm_PUCK(void)
函数功能：蜂鸣器报警函数
入口：
返回：
    无
********************************************************************************/
void Beep_For_Test_Alarm_PUCK(void)
{
 Port_Out_Pub(INTER_ID_ALARM_BEEP,BEEP_MODE_200);
}
/********************************************************************************
void ExtPort_xMs_Hook(void)
函数功能：1ms对外部IO口的钩子函数
入口：
返回：
    无
********************************************************************************/
//ms端口钩子函数,毫秒级单位：MS_HOOK_NUM
void ExtPort_xMs_Hook(void)
{
  INT8U i;
  
  for(i=MIN_PORT_DI;i<=MAX_PORT_DI;i++)
  {
    if(Const_Port_Status[i].NeedInit && 0==Ext_Port_Hander[i].InitFlag)
      continue ;
    
    if((Ext_Port_Hander[i].LoopCount)&&(Const_Port_Status[i].LevelProprty==PULSE_MODE))
    {
      switch(Ext_Port_Hander[i].Status)
      {
      case 0:   //无动作
        break;
      case 1:                                 //轮回启动
        //调用端口操作函数，电平是PULSE_OUT
        Const_Port_Status[i].FuncPtr(Const_Port_Status[i].ValidLevel);
        Ext_Port_Hander[i].Status=2;
        break;
      case 2:  //进行中
        if(Ext_Port_Hander[i].CurCount>=Ext_Port_Hander[i].CurCountBak)  //进入下一轮回
        {
            Ext_Port_Hander[i].CurCount=0;
            Ext_Port_Hander[i].Status=1;
            Ext_Port_Hander[i].LoopCount--;   
            Const_Port_Status[i].FuncPtr(!Const_Port_Status[i].ValidLevel);
            if(Ext_Port_Hander[i].LoopCount==0) //结束
            { 
              Ext_Port_Hander[i].Status=0;     //结束
              Ext_Port_Hander[i].CurCount=0;
              Ext_Port_Hander[i].CurCountBak=0;   //
            }
         }
         else                               //当前轮回
         {  
           Ext_Port_Hander[i].CurCount++;         
         }
        break;
      }
    }
  }
}
