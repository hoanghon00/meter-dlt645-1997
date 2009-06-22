#define MEASULIB_PUCK
#include "Pub_PUCK.h"
#include   "MeasuLib.h"


/******************************文件功能*********************************************
文件功能：计量库函数，包括：

***********************************************************************************/
/**********************************************************************************/
void Delayn10MS(INT32U n)
{
  INT32U i;
  for(i=0;i<n;i++)
  {
    WAITFOR_DRV_MS_TIMEOUT(10);
  }
}

/**********************************************************************************
函数功能：清除计量所有电能寄存器内容,电能初值为0
入口： 无   
出口： 1---------成功;
       0---------失败;
**********************************************************************************/
INT8U Clr_Energ_Reg_PwrOn(void)
{
  INT8U i,Flag,temp[4];
  
  //读A、B、C及合相有功电能，无功电能，清电能寄存器数据-------------------------------PUCK
  for(i=REG_R_AENERG_CLR;i<=REG_R_SUMENERG_CLR;i++)
  {
    Flag=Measu_RdAndCompData_3Times(i,temp);
    if(Flag==0)
      break;
  }
  return  Flag;
}  

/**********************************************************************************
函数功能：硬件复位
入口：    
LastData-----参数寄存器地址
LastPara-------参数
出口： 1---------SIG软硬件信号都正确,复位成功;
       0---------SIG软硬件信号任何一个不对,复位失败;
**********************************************************************************/
INT8U MeasuIC_HardRst(void)
{
  INT8U i,flag;
  INT32U temp;
  
  //CLOSE_MEASU_PWR;
  //SYS_TimeDly_Ms(200);                  //延时200ms
  OPEN_MEASU_PWR;
  SYS_TimeDly_Ms(20);            //延时200ms 
  for(i=0;i<5;i++)
  {
    MEASU_RST_0;
    SYS_TimeDly_Ms(10);           //原则上延时25us即可！
    MEASU_RST_1;
    SYS_TimeDly_Ms(10);            //原则上复位后500us才能操作SPI
    if(!MEASU_SIG_STAT)          //判断SIG脚是否为0，若是，则表示正常------------------PUCK  
    {
      //SYS_TimeDly_Ms(10);           //延时10mS
      flag=Measu_RdAndCompData_3Times(REG_R_SIG,(INT8U *)&temp);
           
      if((flag==0)||(temp==0xffffff))
      {
        continue;
      }
      if(GET_BIT(temp,7))        //判断Sflag.7状态是否为1，若是，则表示正常------------------PUCK
      {
        DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"MeasuIC Reset Ok,SIG=0X%lx!",temp);  
        return 1;
      }
      SYS_TimeDly_Ms(50);            //延时50mS
    }
    else
    {
      SYS_TimeDly_Ms(50);               //延时50mS
    }    
  }
  MeasuStatusMode.HardRstErr=1;
  SET_STRUCT_SUM(MeasuStatusMode);
  return  0;
}
/**********************************************************************************
函数功能： 清除上电校表请求字
入口：   void
出口： 1-------表示需要校表；0--------------表示不需要校表
**********************************************************************************/
void Clr_Cal_Requst(void)
{
  INT8U Flag,i;
  INT32U temp;
  
  for(i=0;i<3;i++)
  {
    Flag=Measu_RdAndCompData_3Times(REG_R_ENFLAG,(INT8U *)&temp);
    if((Flag==0)||(temp==0xffffff))  //避免状态寄存器读错后，对电能的影响，此值不可能都为0xffffff
    continue;
    
    if(GET_BIT(temp,16)==0)           //判断Sflag.16状态是否为1，若是，则表示校表请求
      return ;
  }
}
/**********************************************************************************
函数功能：对计量芯片的para和内卡para比对，重新装载参数
入口：    
出口：1----------装载成功 
**********************************************************************************/
INT8U Load_MeasureIC_Para(void)
{
  INT8U Flag;
  
  Flag=Load_All_Para_To_IC();      
  if(Flag)                     //写入成功
  {
    //Clr_Energ_Reg_PwrOn();    //清除电能寄存器
    Pri_MeasuStatVar.MeasuIntOk=1;   //计量芯片初始化完成，进入正常计量流程
    Init_Pulse_Port(1);
    SET_STRUCT_SUM(Pri_MeasuStatVar);
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Write To IC Para Succeed!");
    Clr_Cal_Requst();
  }
  else
  {
    MeasuStatusMode.Retry=1;   //计量芯片参数写错误重试
    SET_STRUCT_SUM(MeasuStatusMode);
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Write To IC Para Failed,Waiting For Retry!");
  }
  return Flag;  
}
/**********************************************************************************
函数功能：上电初始化MeasuIC
入口：    
LastData-----参数寄存器地址
LastPara-------参数
出口： 表示复位成功;返回0：表示复位失败
**********************************************************************************/
INT8U InitMeasuAfterPwrOn(void)
{
  INT8U Flag;
  
  Flag=MeasuIC_HardRst();
  if(!Flag)     //硬件复位失败
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Hard Reset Error After Power On!");
    return 0;
  }
  
  //Flag=Clr_Energ_Reg_PwrOn();    //如果硬件可靠复位，芯片内部自行清除，此句可以屏蔽
  Flag=Load_MeasureIC_Para();
  Clr_Energ_Accu();   //清除用于计算电能增量的寄存器
  return Flag;
}
/**********************************************************************************
函数功能：根据上次复位后的系统状态，来决定初始化MeasuIC的方式
入口：    
LastData-----参数寄存器地址
LastPara-------参数
出口： 表示复位成功;返回0：表示复位失败
**********************************************************************************/
void Init_Measure_IC(void)
{
  InitMeasuAfterPwrOn();
  SET_STRUCT_SUM(Pri_MeasuStatVar);
}
/**********************************************************************************
函数功能：检查SIG引脚状态以及校表参数校验和是否正确
入口：无
出口：1-------状态正常;0--------状态异常
**********************************************************************************/
INT8U ChkMeasuICWorkStatus(void)
{
  INT8U i,num;

  if(MEASU_SIG_STAT) //正常
    return MEASU_NO_ERR;
  
  for(i=0,num=0;i<5;i++)  //连续读5次
  {
    SYS_TimeDly_Ms(10);
    if(!MEASU_SIG_STAT)      //异常
      num++;
  }
  
  if(num<=2)   //5次中有3次以上是正常的
     return MEASU_NO_ERR;

  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->MeasuIC SIG_Signal=0!"); 
  MeasuStatusMode.Retry=1;
  SET_STRUCT_SUM(MeasuStatusMode);
  return MEASU_SIGN_ERR; 
}
/**********************************************************************************
函数功能：获取当前 331/341模式字
入口：无
出口：331/341模式字
**********************************************************************************/
INT8U Get_SysParse_Mode(void)
{ 
  if(MeasuSysMode.ParseMode<=PARSE_341)
    return MeasuSysMode.ParseMode;
  
  ASSERT(A_WARNING,0);
  return DEFAULT_PARSE_SPEC;
}

/**********************************************************************************
函数功能：获取电压通道ADC增益倍数
入口：无
出口：电压通道ADC增益倍数
**********************************************************************************/
INT32U Get_Volt_Adj_Rate(void)
{  
  INT8U ParseMode;
  INT32U temp;
  
  ParseMode=Get_SysParse_Mode();
   if(ParseMode==PARSE_331)    //331模式下电压通道ADC增益选择为2倍
    temp=V_GAIN_RATE2;
  else                         //341模式下电压通道ADC增益选择为1倍
    temp=V_GAIN_RATE1;  
  return temp;
}
/**********************************************************************************
函数功能：获取当前 电压规格模式字
入口：无
出口：电压模式字
**********************************************************************************/
INT8U Get_SysVolt_Mode(void)
{  
  if(MeasuSysMode.V_Spec<=VOLT_3380)
    return MeasuSysMode.V_Spec;
  
  ASSERT(A_WARNING,0); 
  return DEFAULT_VOLT_SPEC;
}

/**********************************************************************************
函数功能：获取当前 电流规格模式字
入口：无
出口：电流模式字
**********************************************************************************/
INT8U Get_SysCurr_Mode(void)
{
  if(MeasuSysMode.I_Spec<=CURR_30_100)
    return MeasuSysMode.I_Spec;
  
  ASSERT(A_WARNING,0);
  return DEFAULT_CURR_SPEC;
}
/**********************************************************************************
函数功能：获取当前脉冲常数模式字
入口：无
出口：331/341模式字
**********************************************************************************/
INT8U Get_SysPulse_Mode(void)
{  
  if(MeasuSysMode.P_Spec<=PULSE_TYPE2)
    return MeasuSysMode.P_Spec;
  
  ASSERT(A_WARNING,0);
  return DEFAULT_PULSE_SPEC;
}

/**********************************************************************************
函数功能：获取TH和TF模式字
入口：无
出口：TH_MODE/TF_MODE模式字
**********************************************************************************/
INT8U Get_SysTHF_Mode(void)
{
  if(MeasuSysMode.T_FH<=TF_MODE)
    return MeasuSysMode.T_FH;
  
  ASSERT(A_WARNING,0);
  return DEFAULT_TFH_SPEC;
}

/**********************************************************************************
函数功能：根据331/341和TF/TH模式字来决定MeasuIC合相能量累加寄存器；
入口：无
出口：
        ENERG_SUM_MODE0-------341绝对和加；331代数和
        ENERG_SUM_MODE1--------341代数和加；331绝对
**********************************************************************************/
INT32U GetEnergSumMode(void)
{
  INT8U THF_Spec,ParseMode;
  INT32U WrPara;
  
  THF_Spec=Get_SysTHF_Mode();
  ParseMode=Get_SysParse_Mode();
   
     
  if(THF_Spec==TF_MODE)    //单方向
  {
    if(ParseMode==PARSE_341)    //341绝对和加；331代数和
      WrPara=ENERG_SUM_MODE0;
    else                        //341代数和加；331绝对和
      WrPara=ENERG_SUM_MODE1;
  }
  else                   //双方向    
  {
    if(ParseMode==PARSE_341)    //341代数和加；331绝对和
      WrPara=ENERG_SUM_MODE1;
    else                        //341绝对和加；331代数和
      WrPara=ENERG_SUM_MODE0;
  }
  return WrPara;
}

/**********************************************************************************
函数功能：更新A/B/C/合相象限值
入口：无
出口：1/2/3/4：象限
注： P-----横坐标；Q-------纵坐标，逆时针方向：1/2/3/4。
**********************************************************************************/
INT8U	GetQuadrant(INT8U AcpowerDir,INT8U ReacPowerDir)
{	
  if(AcpowerDir==ReacPowerDir)      //1或者3象限
  {
    if(AcpowerDir==0)     //P+ Q+
      return QUADRANT1;
    else                  //P- Q-
      return QUADRANT3;    
  }
  else
  {
    if(AcpowerDir==0)     //P+ Q-
      return QUADRANT4;
    else                  //P- Q+
      return QUADRANT2;     
  }  
}
/**********************************************************************************
函数功能：根据象限，无功功率，有功功率，算出电压电流实际夹角
入口：无
出口：真实角度
**********************************************************************************/
INT32U	GetAngle(INT8U Quadrant,INT32U Reacower,INT32U ActiPower,INT8U Reg)
{	
  FP32S Dgree;
  
#ifdef MEASU_ANGLE_RD 
  INT8U Flag;
  INT32U Angle;
#endif

#ifdef MEASU_ANGLE_RD
    Flag=Measu_RdAndCompData_3Times(Reg,(INT8U *)&Angle);
    if(Flag==0)
    return 0;
    Dgree=(FP32S)Angle*0.001366;  //注意单位：UNIT_DEGREE
#else
    if(ActiPower<=5)  //防止除0溢出，0.005w,启动功率：0.33w
      Dgree=0L;
    else
    {
      Dgree=atan((FP32S)Reacower/ActiPower);
      Dgree=(Dgree*180*UNIT_DEGREE)/PAI;	//单位0.01度，角度不是从寄存器取出，是自己算出的
    }
#endif    
  switch(Quadrant)
  {
  case QUADRANT1:				//第1象限
    break;
  case QUADRANT2:				//第2象限
    Dgree=18000-Dgree;
    break;	
  case QUADRANT3:				//第3象限
    Dgree=18000+Dgree;
    break;	
  default:			        //第4象限
    Dgree=36000-Dgree;
    break;
  }
  return (INT32U)Dgree;
}
/**********************************************************************************
函数功能：根据校表字，校表内容，校表
入口：
       Code-----------校表指令
       Para3----------校表内容
       ParaLen-------校表数据字节长度
出口：1------------成功；0-----------------失败
**********************************************************************************/
INT8U CalibMeterProc_PUCK(INT16U Code,INT8U Para[],INT8U ParaLen)
{
   return 1;
}
/**********************************************************************************
函数功能：获取IC的参数校验和

出口：1---------读成功，读比较正确;0--------表示失败
**********************************************************************************/
INT8U Get_IC_ParaSum3(INT32U *Cs)
{
  INT32U Sum;
  INT8U Flag;
  
  Flag=Measu_RdAndCompData_3Times(REG_R_SUM,(INT8U *)&Sum);     //读IC校表参数的校验和
  if(Flag==0)
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get IC Cs Failed!"); 
    return 0;
  }
  
  *Cs=Sum;  
  return 1;
}
/**********************************************************************************
函数功能：获取实际用的部分默认参数的校验和
**********************************************************************************/
INT8U Get_User_All_ParaSum3(INT32U *Cs)
{
  INT8U i,Flag,temp[3];
  INT32U Sum;
  
  Flag=1;
  for(Sum=0,i=1;i<IC_PARA_NUM;i++)
  {
    if(InitMeasuPara[i].ParaFlag==DEFA_TYPE)
      Sum+=InitMeasuPara[i].Para;
    if(InitMeasuPara[i].ParaFlag==SPEC_TYPE)
      Sum+=InitMeasuPara[i].FuncPtr();
    if(InitMeasuPara[i].ParaFlag==ADJU_TYPE)
    {
      Flag=Read_Storage_Data_PUCK(InitMeasuPara[i].Di,temp,3);
      ASSERT(A_WARNING,1==Flag); 
      Sum+=(INT32U)(temp[2]*65536L+temp[1]*256L+temp[0]);
    }
  }
  if(!Flag)
    return 0;
  
  *Cs=(Sum&0xffffff);
  return 1;  
}
/**********************************************************************************
函数功能：获取高频脉冲输出
入口：无
出口：高频脉冲常熟设置
**********************************************************************************/
INT32U Get_HighPulse_Const(void)
{  
  INT32U temp;
  INT8U U_spec,I_spec;
  
  U_spec=Get_SysVolt_Mode();
  I_spec=Get_SysCurr_Mode();
  
  temp=HighPulseConst[U_spec][I_spec];

  if((INT32U)temp<0x04 || (INT32U)temp>0x0D00)
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Hight Pulse Const < 0x04");
    temp=0x04;
  }
  
  if((INT32U)temp>0x0D00)
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Hight Pulse Const > 0x0d00");
    temp=0x0D00;
  }
  
  return  (((INT32U)temp)&0xFFFFFF);
}

/**********************************************************************************
函数功能：获取启动电流常数
入口：无
出口：
**********************************************************************************/
INT32U Get_Istart_Const(void)
{
  INT8U I_Spec;
  I_Spec=Get_SysCurr_Mode();
  return (Startup_Current[I_Spec]);  
}

/**********************************************************************************
函数功能：获取Iregion4
入口：无
出口：

INT32U Get_Iregion4(void)
{
  FP32S temp;
  
  temp=0.648*I_REGION_AREA*Get_In()*pow(2,23);
  
  return ((INT32U)temp&0XFFFFFF);  
}
**********************************************************************************/

/***********************************************************************
函数功能：根据发生的事件Id和状态，设置相应的状态
入口：无
出口：无
***********************************************************************
void Clr_All_Cal_Status(void)
{ 
  mem_set((void *)(&Cal_Status_Var),
          0x00,
          sizeof(Cal_Status_Var),
          (void *)(&Cal_Status_Var),
          sizeof(Cal_Status_Var));
  SET_STRUCT_SUM(Cal_Status_Var); 

}
***********************************************************************
函数功能：根据发生的事件Id和状态，设置相应的状态
入口：无
出口：无
***********************************************************************
void Set_Cal_Status_Byte(INT16U Id,INT8U Status)
{
  
  INT8U Byte,Bit;
  if((Id< ADJ_METER_A_POW)||(Id>ADJ_METER_C_CUR))
    return ;
  
  Byte=(INT8U)(Id-ADJ_METER_A_POW)/8;
  Bit=(INT8U)(INT8U)(Id-ADJ_METER_A_POW)%8;
  if(Status)
    SET_BIT(Cal_Status_Var.Cal_Status[Byte],Bit);
  else
    CLR_BIT(Cal_Status_Var.Cal_Status[Byte],Bit);    
  SET_STRUCT_SUM(Cal_Status_Var);
}

***********************************************************************
函数功能：根据发生的事件，设置相应的瞬态状态字
入口：无
出口：无
***********************************************************************
void Set_Cal_Status(INT16U Id)
{
  Set_Cal_Status_Byte(Id,1);
}
***********************************************************************
函数功能：根据位或者字节单位，清除对应的位信息或字节信息
入口：无
出口：无
***********************************************************************
void Clr_Cal_Status(INT16U Id)
{
  Set_Cal_Status_Byte(Id,0); 
}
***********************************************************************
函数功能：根据位或者字节单位，获取对应的位信息或字节信息
入口：无
出口：无
***********************************************************************
INT8U Get_Cal_Status(INT16U Id)
{
  INT8U Byte,Bit,temp;
  if((Id< ADJ_METER_A_POW)||(Id>ADJ_METER_C_CUR))
    return 0;
  
  Byte=(INT8U)(Id-ADJ_METER_A_POW)/8;
  Bit=(INT8U)(Id-ADJ_METER_A_POW)%8;
  
  temp=CHECK_STRUCT_SUM(Cal_Status_Var); 
  ASSERT(A_WARNING,1==temp);  

  temp=GET_BIT(Cal_Status_Var.Cal_Status[Byte],Bit);  
  return temp;    
}
**********************************************************************************
函数功能：根据不同的类型，获取误差值=(测量值-真实值)/真实值
出口：误差

**********************************************************************************
FP32S Get_Error(INT8U Parse,INT8U Flag)
{
  FP32S err,Measu;
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(Measu_InstantData_ToPub_PUCK);
  ASSERT(A_WARNING,1==Re);
  ASSERT(A_WARNING,Parse<3);
  
  switch(Flag)
  {
    case TYPE_POWER:
      Measu=(FP32S)(*(&(Measu_InstantData_ToPub_PUCK.AcPower.A)+Parse));  
      if(Measu==0)
        return 1.0;
      err=(Measu*1000)/((FP32S)330*UNIT_KW_KAR)-1;
      return err;
    case TYPE_ANGLE:
      Measu=(FP32S)(*(&(Measu_InstantData_ToPub_PUCK.Angle.A)+Parse)); 
      if(Measu==0)
        return 1.0;
      err=Measu/((FP32S)60*UNIT_DEGREE)-1;
      return err;
    case TYPE_VOLT:
      Measu=(FP32S)(*(&(Measu_InstantData_ToPub_PUCK.Volt.A)+Parse));
      if(Measu==0)
        return 1.0;
      err=Measu/(Get_Un()*UNIT_V)-1;
      return err;
    case TYPE_CURR:
      Measu=(FP32S)(*(&(Measu_InstantData_ToPub_PUCK.Curr.A)+Parse));
      if(Measu==0)
      return 1.0;
      err=Measu/(Get_In()*UNIT_A)-1;
      return err;
  }  
  return 1.0;
}
**********************************************************************************
函数功能：根据不同的类型，获取误差值=(测量值-真实值)/真实值
出口：误差

**********************************************************************************
void Set_Modi_Offset(INT8U Parse, FD32S err,INT8U Reg,INT8U RegLast,INT8U RegNum)
{
  INT32U para;
  INT8U Wrflag,Flag=0;
  
  switch(Parse)
  {
    case TYPE_POWER:
    case TYPE_VOLT:
    case TYPE_CURR:
      err=-err/(1+err);
      if(err>=0)
        para=(INT32U)(err*pow(2,23));
      else
        para=(INT32U)(pow(2,24)+err*pow(2,23));
      Flag=1;
      break;
      
    case TYPE_ANGLE:
      err=acos((1+err)*0.5)-PAI/3;
      if(err>=0)
        para=(INT32U)(err*pow(2,23));
      else
        para=(INT32U)(pow(2,24)+err*pow(2,23));
      Flag=1;
      break;
  }  
  if(!Flag)
    return ;
  Wrflag=Measu_WrAndCompData_3Times(Reg,para);
  ASSERT(A_WARNING,1==Wrflag); 
  for(Flag=0;Flag<RegNum;Flag++)
    Wrflag&=Measu_WrAndCompData_3Times(RegLast+Flag,para);
  ASSERT(A_WARNING,1==Wrflag); 
}
**********************************************************************************
函数功能：
出口：
注意：
**********************************************************************************
FP32S Get_Calc_Err(INT8U Type,FP32S Err)
{
  FD32S data;
  
  data=(FP32S)Err/MIN_ER_COFI;
  data=MIN_CAL_STEP*data;  
  return data;
}
**********************************************************************************
函数功能：
出口：
注意：
**********************************************************************************/
void Cal_Reg_Write(INT8U Reg,INT8U Reg1ast,INT8U Num,INT32U CalData)
{
  INT8U i;
  
  if(Measu_WrAndCompData_3Times(Reg,CalData)==0)
    ASSERT(A_WARNING,0);
  
  for(i=0;i<Num;i++)  //对后续的补偿寄存器写
  {
    if(Measu_WrAndCompData_3Times(Reg1ast+i,CalData)==0)
      ASSERT(A_WARNING,0);
  }
}     
/**********************************************************************************
函数功能：将校表用的某一个参数写入计量芯片
出口：1---------写成功，;0--------写失败
注意：所有的校表参数都是3个字节！
**********************************************************************************/
void Load_Adj_OnePara_To_IC(INT16U TempDI)
{
  INT8U Reg;
  //FD32S err;
  
  INT32U CalData;
  INT8U Rdflag;
  INT8U temp[3];
  
  if((TempDI<DI_POWER_A_0)||(TempDI>DI_CLR_PARA))
    return ;
  
  Rdflag=Read_Storage_Data_PUCK(TempDI,temp,3);
  if(!Rdflag)
    ASSERT(A_WARNING,0); 
  CalData=(INT32U)(temp[2]*65536L+temp[1]*256L+temp[0]);
  DEBUG_PRINT(PUCK,PRINT_PUCK_CHANNEL_EN,"Measure_Info----->Cal_Meter DI=0x%x,CalData=0x%lx",TempDI,CalData);
  
  EnMeasuCal();                             //校表数据写使能
  if((TempDI>=ADJ_METER_A_POW)&&(TempDI<=ADJ_METER_C_POW))     //功率增益参数
  {
    Reg=REG_W_PGAIN_A0+(INT8U)(TempDI-ADJ_METER_A_POW);
    Cal_Reg_Write(Reg,Reg+3,1,CalData);
  }
  
  if((TempDI>=ADJ_METER_A_PHASE)&&(TempDI<=ADJ_METER_C_PHASE)) //相位增益参数
  {
    Reg=REG_W_PHSREG_A0+(INT8U)(TempDI-ADJ_METER_A_PHASE)*5;
    Cal_Reg_Write(Reg,Reg+1,4,CalData);
  }
  
  if((TempDI>=ADJ_METER_A_VOL)&&(TempDI<=ADJ_METER_C_VOL))     //电压参数
  {
     Reg=REG_W_UGAIN_A+(INT8U)(TempDI-ADJ_METER_A_VOL);
     Cal_Reg_Write(Reg,0,0,CalData);
   }


  if((TempDI>=ADJ_METER_A_CUR)&&(TempDI<=ADJ_METER_C_CUR))    //电流参数
  {
     Reg=REG_W_IGAIN_A+(INT8U)(TempDI-ADJ_METER_A_CUR); 
     Cal_Reg_Write(Reg,0,0,CalData);
 }
  //更新校验和，用于比对
  DisMeasuCal(); 
  
  /*
  OS_TimeDly_Ms(400);  //for Cs Refresh
  Rdflag=Get_User_All_ParaSum3(&CalData);
  if(Rdflag)
  {
    MeasuPara_CS.EppRom_Cs=CalData;
    SET_STRUCT_SUM(MeasuPara_CS);
    Rdflag=Get_IC_ParaSum3(&CalData);
    if(Rdflag)
    {
      MeasuPara_CS.IC_Cs=CalData;
      SET_STRUCT_SUM(MeasuPara_CS);
      return ;
    }
    ASSERT(A_WARNING,0); 
  }
  ASSERT(A_WARNING,0); 
  */
 
}
/**********************************************************************************
函数功能：将使用的校表参数写入计量芯片
出口：1---------写成功，;0--------写失败
**********************************************************************************/
INT8U Load_Spec_Adj_Para_To_IC(void)
{
  INT8U i,Rdflag,Wrflag,temp[3],Flag;
  INT32U para,CS;
  
  CS=0;
  EnMeasuCal();                             //校表数据写使能
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Info----->Load Epprom Para To Measure IC........");
  for(Flag=1,i=1;i<IC_PARA_NUM;i++)
  {
    if(InitMeasuPara[i].ParaFlag==DEFA_TYPE)
    {
      CS+=InitMeasuPara[i].Para;
      continue;
    }
    if(InitMeasuPara[i].ParaFlag==SPEC_TYPE)
      para=InitMeasuPara[i].FuncPtr();
    if(InitMeasuPara[i].ParaFlag==ADJU_TYPE)
    {
      Rdflag=Read_Storage_Data_PUCK(InitMeasuPara[i].Di,temp,3);
      if(0==Rdflag)
      ASSERT(A_WARNING,0); 
      para=(INT32U)(temp[2]*65536L+temp[1]*256L+temp[0]);
      Flag&=Rdflag;
    }
     Wrflag=Measu_WrAndCompData_3Times(i|0x80,para);
     if(0==Wrflag)
      ASSERT(A_WARNING,0); 
     Flag&=Wrflag;
     CS+=para;
  }
  
  DisMeasuCal();
  MeasuPara_CS.EppRom_Cs=CS&0xffffff;
  SET_STRUCT_SUM(MeasuPara_CS);
  
  OS_TimeDly_Ms(400);  //for Cs Refresh
  
  if(Flag && Get_IC_ParaSum3(&CS))
  {
    if(CS==MeasuPara_CS.EppRom_Cs)
    {
      MeasuPara_CS.IC_Cs=CS;
      SET_STRUCT_SUM(MeasuPara_CS);  
      return 1;
    }
  }
  return 0;
}
/**********************************************************************************
函数功能：将MeasuIC校表数据寄存器的内容恢复到上电初始值；初始化与校表参数相关的参数
          EPROM中的校表参数同步改变
入口：无
出口：1---------恢复成功；0-----------恢复失败
**********************************************************************************/
INT8U Load_All_Para_To_IC(void)
{
  INT8U Flag;
  INT32U Cs;
  
  Flag=0;
  
  EnMeasuCal();
  ClrMeasuCal();     //将校表数据寄存器的内容恢复到上电初始值
  DisMeasuCal();
  
  
  Flag=Measu_RdAndCompData_3Times(REG_R_SUM,(INT8U *)&Cs);     //读7022中校表参数的校验和
  if(Flag==0)
  {
    MeasuStatusMode.Retry=1;
    SET_STRUCT_SUM(MeasuStatusMode);
    return 0;
  }
  
  if((Get_SysParse_Mode()==PARSE_341) && (Cs!=CHKSUM_INIT_341))  //不是上电时候的CS
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Info----->Measure_IC Para Data(341) !=Default Value!");
  
  if((Get_SysParse_Mode()==PARSE_331) && (Cs!=CHKSUM_INIT_331)) //不是上电时候的CS
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Info----->Measure_IC Para Data(331) !=Default Value!");
     
  Flag=Load_Spec_Adj_Para_To_IC();
  
  if(Flag==1)
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Info----->All Cal Para Write Succeed!");
    return 1;  
  }
  if(Flag==0)
  {
    MeasuStatusMode.Retry=1;
    SET_STRUCT_SUM(MeasuStatusMode);
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Load Spec&&Adj Para Failed!");
    return 0;
  }
  return 0;
}

/**********************************************************************************
函数功能：计量参数重新写
入口：3数
出口：最大
**********************************************************************************/
void Retry_WritePara_To_IC(void)   //运行过程中，IC的CS和EPPROM的CS不一致，或者某次写计量参数错误，需要重新写狡辩参数
{
  INT8U Flag;
  
  Flag=CHECK_STRUCT_SUM(MeasuStatusMode); 
  if(ASSERT(A_WARNING,1==Flag)) //错误状态字可能被破坏！
   return ;
  
  if(MeasuStatusMode.Retry==1)
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Retry Write IC Para.........");
    Flag=Load_Spec_Adj_Para_To_IC();
    SYS_TimeDly_Sec(5);
    if(Flag)
    {      
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Info----->Retry Write IC Para Succeed!");
      MeasuStatusMode.Retry=0;   //已经写正确,不需再写
      SET_STRUCT_SUM(MeasuStatusMode);
    }  
    else
      SYS_TimeDly_Sec(3);   //避免下次写的过快
  }
}
/**********************************************************************************
函数功能：
入口：无
出口：
**********************************************************************************/
void PrintParaComp(void)
{
  INT8U Flag,temp[3],i;
  INT32U paraic,para;
  
  for(i=1;i<IC_PARA_NUM;i++)
  {
    Flag=Measu_RdAndCompPara_3Times(i,temp);
    ASSERT(A_WARNING,(1==Flag)); 
    paraic=(INT32U)(temp[2]*65536L+temp[1]*256L+temp[0]);
    
    if(InitMeasuPara[i].ParaFlag==DEFA_TYPE)
    {
      if(paraic!=InitMeasuPara[i].Para)
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error-----Reg=%d,Rd=0x%lx,Real=0x%lx",i,paraic,(InitMeasuPara[i].Para&0xffffff)); 
    }
    
    if(InitMeasuPara[i].ParaFlag==SPEC_TYPE)
    {
      if(paraic!=InitMeasuPara[i].FuncPtr())
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error-----Reg=%d,Rd=0x%lx,Real=0x%lx",i,paraic,InitMeasuPara[i].FuncPtr()); 
    }
    
    if(InitMeasuPara[i].ParaFlag==ADJU_TYPE)
    {
      Flag=Read_Storage_Data_PUCK(InitMeasuPara[i].Di,temp,3);
      ASSERT(A_WARNING,1==Flag); 
      para=(INT32U)(temp[2]*65536L+temp[1]*256L+temp[0]);
      if(paraic!=para)
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error-----Reg=%d,Rd=0x%lx,Real=%0xlx",i,paraic,para); 
    }
  }
}

