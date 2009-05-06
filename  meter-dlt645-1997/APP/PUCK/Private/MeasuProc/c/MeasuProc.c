

#include "Pub_PUCK.h"

#define    MeasuProc_PUCK
#include   "MeasuProc.h"


/**********************************************************************************
函数功能：判定是否发生了校表事件
入口：无
出口：0/1
**********************************************************************************/
INT8U Get_Cal_Exsit(void)
{
  if(MODE_RUN!=Get_Meter_Hard_Mode() && Read_Event_Status(ID_EVENT_ADJUST_MEASU))   //确实需要校表或者未校完，且处于非出厂模式
    return 1;
  
  return 0;
}
/**********************************************************************************
函数功能：获取额定电压
入口：无
出口：TH_MODE/TF_MODE模式字
**********************************************************************************/
FP32S Get_Un(void)
{  
  INT8U U_Spec;

  U_Spec=Get_SysVolt_Mode();
  return (U_CONST[U_Spec]);
}
/**********************************************************************************
函数功能：获取额定电流
入口：无
出口：
**********************************************************************************/
FP32S Get_In(void)
{  
  INT8U I_Spec;

  I_Spec=Get_SysCurr_Mode();
  return (I_CONST[I_Spec]);
}

/**********************************************************************************
函数功能：获取启动电流
入口：无
出口：启动电流
**********************************************************************************/
FP32S Get_Start_Current(void)
{  
  INT8U I_Spec;

  I_Spec=Get_SysCurr_Mode();
  return ((FP32S)I_CONST[I_Spec]/I_START_RATIO);
}

/**********************************************************************************
函数功能：获取启动功率
入口：无
出口：启动电流
**********************************************************************************/
FP32S Get_Start_Power(void)
{  
  INT8U U_Spec,I_Spec;
  FP32S P;
  
  U_Spec=Get_SysVolt_Mode();
  I_Spec=Get_SysCurr_Mode();  
  
  P=(U_CONST[U_Spec]*I_CONST[I_Spec]*I_START_RATIO)/1000; //转换成 0.000001kW=0.001w
  
  return P;
}

/**********************************************************************************
函数功能：清除用于计算电量的累计数据的寄存器
入口：无
出口：
**********************************************************************************/
void Clr_Energ_Accu(void)
{
   mem_set((void *)(&Pri_TempMeasuVar),
            0x00,
            sizeof(Pri_TempMeasuVar),
            (void *)(&Pri_TempMeasuVar),
            sizeof(Pri_TempMeasuVar));
    SET_STRUCT_SUM(Pri_TempMeasuVar);
    
#ifdef MEASURE_VAR_BAK_EN
    mem_set((void *)(&Pri_TempMeasuVarBak1),
            0x00,
            sizeof(Pri_TempMeasuVarBak1),
            (void *)(&Pri_TempMeasuVarBak1),
            sizeof(Pri_TempMeasuVarBak1));
    SET_STRUCT_SUM(Pri_TempMeasuVarBak1);
  
      mem_set((void *)(&Pri_TempMeasuVarBak2),
            0x00,
            sizeof(Pri_TempMeasuVarBak2),
            (void *)(&Pri_TempMeasuVarBak2),
            sizeof(Pri_TempMeasuVarBak2));
    SET_STRUCT_SUM(Pri_TempMeasuVarBak2); 
#endif

}
/**********************************************************************************
函数功能：初始化内存
入口：无
出口：无
**********************************************************************************/
void Init_Private_Ram(void)
{
    mem_set(&RstCtrlStatus,0x00,sizeof(RstCtrlStatus),&RstCtrlStatus,sizeof(RstCtrlStatus));  
    SET_STRUCT_SUM(RstCtrlStatus);


    mem_set((void *)(&MeasuPara_CS),0x00,sizeof(MeasuPara_CS),(void *)(&MeasuPara_CS),sizeof(MeasuPara_CS));  
    SET_STRUCT_SUM(MeasuPara_CS);
    
    mem_set((void *)(&MeasuStatusMode),\
            0x00,\
            sizeof(MeasuStatusMode),\
           (void *)(&MeasuStatusMode),\
             sizeof(MeasuStatusMode));
    SET_STRUCT_SUM(MeasuStatusMode);

  mem_set((void *)(&Pri_TempIntantVar),\
         0x00,\
         sizeof(Pri_TempIntantVar),\
         (void *)(&Pri_TempIntantVar),\
          sizeof(Pri_TempIntantVar));
  SET_STRUCT_SUM(Pri_TempIntantVar);
  
  
  mem_set((void *)(&Curr_Under_Istart),
          0x00,\
          sizeof(Curr_Under_Istart),\
         (void *)(&Curr_Under_Istart),\
          sizeof(Curr_Under_Istart));
  SET_STRUCT_SUM(Curr_Under_Istart);  
    
    
  mem_set((void *)(&Curr_1_Per_100),
          0x00,\
          sizeof(Curr_1_Per_100),\
         (void *)(&Curr_1_Per_100),\
          sizeof(Curr_1_Per_100));
  SET_STRUCT_SUM(Curr_1_Per_100);

  mem_set((void *)(&Curr_In_6_13x5),
          0x00,
          sizeof(Curr_In_6_13x5),\
         (void *)(&Curr_In_6_13x5),
         sizeof(Curr_In_6_13x5));
   SET_STRUCT_SUM(Curr_In_6_13x5);

  mem_set((void *)(&Curr_6_13x5_Slipe),
          0x00,
          sizeof(Curr_6_13x5_Slipe),
          (void *)(&Curr_6_13x5_Slipe),
          sizeof(Curr_6_13x5_Slipe));
  SET_STRUCT_SUM(Curr_6_13x5_Slipe);

  mem_set((void *)(&SpecChange_CurrFlag),
          0x00,
          sizeof(SpecChange_CurrFlag),
          (void *)(&SpecChange_CurrFlag),
          sizeof(SpecChange_CurrFlag));
  SET_STRUCT_SUM(SpecChange_CurrFlag);

  Clr_Energ_Accu();
 

  mem_set(&Pri_MeasuStatVar,\
          0x00,\
           sizeof(Pri_MeasuStatVar),\
        (void *)(&Pri_MeasuStatVar),\
          sizeof(Pri_MeasuStatVar));
  SET_STRUCT_SUM(Pri_MeasuStatVar);

}
/**********************************************************************************
函数功能：初始化内存
入口：无
出口：无
**********************************************************************************/
void Init_Public_Ram(void)
{
    mem_set((void *)(&Measu_Status_Mode_ToPub_PUCK),0x00,sizeof(Measu_Status_Mode_ToPub_PUCK),(void *)(&Measu_Status_Mode_ToPub_PUCK),sizeof(Measu_Status_Mode_ToPub_PUCK));
    SET_STRUCT_SUM(Measu_Status_Mode_ToPub_PUCK);

    mem_set((void *)(&Measu_InstantData_ToPub_PUCK),0x00,sizeof(Measu_InstantData_ToPub_PUCK),(void *)(&Measu_InstantData_ToPub_PUCK),sizeof(Measu_InstantData_ToPub_PUCK));
    SET_STRUCT_SUM(Measu_InstantData_ToPub_PUCK);

    mem_set((void *)(&Measu_Energ_ToPub_PUCK),0x00,sizeof(Measu_Energ_ToPub_PUCK),(void *)(&Measu_Energ_ToPub_PUCK),sizeof(Measu_Energ_ToPub_PUCK));
    SET_STRUCT_SUM(Measu_Energ_ToPub_PUCK);
  
#ifdef VOLTANGLE_EN
  if(Pri_MeasuStatVar.ParaInit)
    mem_set(&App_VoltAngle_Struct,0x00,sizeof(App_VoltAngle_Struct),(void *)(&App_VoltAngle_Struct),sizeof(App_VoltAngle_Struct));
    SET_STRUCT_SUM(App_VoltAngle_Struct);
#endif
}
/**********************************************************************************
函数功能：初始化计量SRAM
入口：无
出口：无
**********************************************************************************/	
void Init_Measure_Ram(void)  //需要在Init_Low_Power_Proc()后调用
{  
  Init_Interr_Sram();
  Init_Public_Ram();
  Init_Private_Ram();
  Init_All_Loss_Sram();  //初始化全失压
  Clr_All_Event_Sram();
}
/**********************************************************************************
函数功能：获取331或者341方式，单或者双方向，脉冲常数规格，电压规格，电流规格
入口：Flag-------1:初始化；0---------------主程序
出口：无
**********************************************************************************/	
INT8U GetSysModeProc(void)
{
#ifdef SYS_MEASU_MARCO
  MeasuSysMode.ParseMode=SYS_PARSE_SPEC;
  MeasuSysMode.T_FH=SYS_THF_SPEC;
  MeasuSysMode.P_Spec=SYS_PULSE_TYPE;
  MeasuSysMode.V_Spec=SYS_VOLT_SPEC;
  MeasuSysMode.I_Spec=SYS_CURR_SPEC;
#else
  INT8U Flag;
  INT8U temp[3];
      
  Flag=Read_Storage_Data_PUCK(DI_MODE1,temp,1);  //T_FH模式字
  Flag&=Read_Storage_Data_PUCK(DI_P_U_I_SPEC,temp,3);
  
  
  MeasuSysMode.T_FH=GET_BIT(temp[0],6);        //T_FH模式字
  MeasuSysMode.P_Spec=temp[2];
  MeasuSysMode.V_Spec=temp[1];
  MeasuSysMode.I_Spec=temp[0];
  
  if(MeasuSysMode.P_Spec>PULSE_TYPE2) //数据超限，采用默认参数
  {
    MeasuSysMode.P_Spec=DEFAULT_PULSE_SPEC;
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->MeasuSysMode.P_Spec Get Failed!");  
  }
  
  if(MeasuSysMode.V_Spec>VOLT_3380)   //数据超限，采用默认参数
  {
    MeasuSysMode.P_Spec=DEFAULT_VOLT_SPEC;
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->MeasuSysMode.V_Spec Get Failed!");  
  }
 
  if(MeasuSysMode.I_Spec>CURR_30_100)  //数据超限，采用默认参数
  {
    MeasuSysMode.I_Spec=DEFAULT_CURR_SPEC;
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->MeasuSysMode.I_Spec Get Failed!");  
  }


  if((MeasuSysMode.V_Spec==VOLT_3100)||(MeasuSysMode.V_Spec==VOLT_3380))
   MeasuSysMode.ParseMode=PARSE_331;
  else
   MeasuSysMode.ParseMode=PARSE_341;
 #endif 
  

  SET_STRUCT_SUM(MeasuSysMode);
  return Flag;  
}
/**********************************************************************************
函数功能：获取表计最大脉冲常数
入口：无
出口：
**********************************************************************************/
INT32U Get_Sys_Pulse(void)
{  
  INT8U PulseSpec,U_Spec,I_Spec;
  PulseSpec=Get_SysPulse_Mode();
  U_Spec=Get_SysVolt_Mode();
  I_Spec=Get_SysCurr_Mode();
  return(PULSE_CONST[PulseSpec][U_Spec][I_Spec]);
}


/**********************************************************************************
函数功能：根据内卡来初始化内存，初始化与校表参数无关的参数
入口：无
出口：无
**********************************************************************************/
INT8U Init_Para(void)
{
  INT8U Flag;
 
  Flag=GetSysModeProc();        //获取系统331/341模式字，电压电流规格，脉冲常数规格
  if(!Flag)
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->System Mode Para Get Failed!"); 
 
  Flag=Get_Pulse_Para();        //有无功脉冲常数，脉冲宽度输出参数
  if(!Flag)
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Pulse Para Get Failed!");  
  return Flag;
}
/**********************************************************************************
函数功能：更新A/B/C/合相象限值:1~4
入口：无
出口：无
**********************************************************************************/
void	RefreshQuadrant(void)
{
  
  if((CHECK_STRUCT_SUM(Pri_MeasuStatVar)==0) || (CHECK_STRUCT_SUM(Pri_TempIntantVar)==0))
    ASSERT(A_WARNING,0);  
  
  Pri_TempIntantVar.Quadrant.A=GetQuadrant(Pri_MeasuStatVar.A_APwrDir,Pri_MeasuStatVar.A_RPwrDir);
  Pri_TempIntantVar.Quadrant.B=GetQuadrant(Pri_MeasuStatVar.B_APwrDir,Pri_MeasuStatVar.B_RPwrDir);
  Pri_TempIntantVar.Quadrant.C=GetQuadrant(Pri_MeasuStatVar.C_APwrDir,Pri_MeasuStatVar.C_RPwrDir);
  Pri_TempIntantVar.Quadrant.Sum=GetQuadrant(Pri_MeasuStatVar.SUM_APwrDir,Pri_MeasuStatVar.SUM_RPwrDir); 
  
  SET_STRUCT_SUM(Pri_MeasuStatVar);
  SET_STRUCT_SUM(Pri_TempIntantVar);
}	

/**********************************************************************************
函数功能：获取相邻2相的电压夹角
入口：无
出口：1------------获取成功；0-----------获取失败
**********************************************************************************/	
INT8U  GetVoltAngle_PUCK(void)
{
#ifdef VOLTANGLE_EN
  INT8U Flag,Reg;
  INT32U *Ptr,CurData;
  FP32S   temp;  
  
  Ptr=&(App_VoltAngle_Struct.AB);
  for(Reg=REG_R_AB_ANGLE;Reg<=REG_R_BC_ANGLE;Reg++,Ptr++)
  {
    Flag=Measu_RdAndCompData_3Times(Reg,(INT8U *)&CurData);  //读取夹角
    if(!Flag)
      return 0;
    temp=(FP32S)CurData/8192;
    *Ptr=(INT32U)temp;
  }
#endif 
  return 1;
}

/**********************************************************************************
函数功能：判断是否大于启动电流
入口：无
出口：无
**********************************************************************************/
INT8U Judge_I_Start(void)
{
  INT32U MaxI;
  
  if(CHECK_STRUCT_SUM(Pri_TempIntantVar)==0)
    ASSERT(A_WARNING,0);
  
  
  MaxI=Get_Max_Num(Pri_TempIntantVar.Curr.A,Pri_TempIntantVar.Curr.B,Pri_TempIntantVar.Curr.C);
  if((FP32S)MaxI>=(Get_Start_Current()*UNIT_A))  //电流都大于启动电流
  return 1;
  
  return 0;    
}
/**********************************************************************************
函数功能：求总,A,B,C的相角
入口：无
出口：无
**********************************************************************************/
void	GetAllAngel_PUCK(void)
{
  INT8U Flag;
  
  Flag=Get_SysCurr_Mode();
  if(Judge_I_Start())  //电流都大于启动电流
  { 
    //A相
    Pri_TempIntantVar.Angle.A=GetAngle(Pri_TempIntantVar.Quadrant.A,Pri_TempIntantVar.ReacPower.A,Pri_TempIntantVar.AcPower.A,REG_R_ANGLE_A);
    if((FP32S)Pri_TempIntantVar.Curr.A<=(Get_Start_Current()*UNIT_A))   //A分相电流小于启动电流时，其相角强制置为0	
      Pri_TempIntantVar.Angle.A=0;
    
    //B相
    Pri_TempIntantVar.Angle.B=GetAngle(Pri_TempIntantVar.Quadrant.B,Pri_TempIntantVar.ReacPower.B,Pri_TempIntantVar.AcPower.B,REG_R_ANGLE_B);
    if((FP32S)Pri_TempIntantVar.Curr.B<=(Get_Start_Current()*UNIT_A))   //B分相电流小于启动电流时，其相角强制置为0	
      Pri_TempIntantVar.Angle.B=0;    

    //C相
    Pri_TempIntantVar.Angle.C=GetAngle(Pri_TempIntantVar.Quadrant.C,Pri_TempIntantVar.ReacPower.C,Pri_TempIntantVar.AcPower.C,REG_R_ANGLE_C);
    if((FP32S)Pri_TempIntantVar.Curr.C<=(Get_Start_Current()*UNIT_A))   //C分相电流小于启动电流时，其相角强制置为0	
      Pri_TempIntantVar.Angle.C=0;
    
    //合相
    Pri_TempIntantVar.Angle.Sum=GetAngle(Pri_TempIntantVar.Quadrant.Sum,Pri_TempIntantVar.ReacPower.Sum,Pri_TempIntantVar.AcPower.Sum,REG_R_ANGLE_SUM);
  }                           
  else  //电流都小于启动电流
  {
    //所有分相电流都小于启动电流时，所有相角强制置为0
    Pri_TempIntantVar.Angle.A=0;
    Pri_TempIntantVar.Angle.B=0;
    Pri_TempIntantVar.Angle.C=0;
    Pri_TempIntantVar.Angle.Sum=0;
  }
  
  Flag=Get_SysParse_Mode();
  if(Flag==PARSE_331)    //331模式下 B相电压电流夹角为0
    Pri_TempIntantVar.Angle.B=0;
  
   SET_STRUCT_SUM(Pri_TempIntantVar);
}

/**********************************************************************************
函数功能：有功和无功功率方向处理
入口：无
出口：无
**********************************************************************************/	
INT8U Get_PQ_PowerDir_PUCK(void)
{
  INT8U Flag;
  INT32U RdData;
  INT8U *Ptr;
  
  Flag=Measu_RdAndCompData_3Times(REG_R_PQ_DIR,(INT8U *)&RdData);
  if(!Flag)
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get PQ_Direction Error!");  
    return MEASU_RD3TIMES_ERR;
  }
  
  if(CHECK_STRUCT_SUM(Pri_MeasuStatVar)==0)
    ASSERT(A_WARNING,0);
  
  Ptr=(INT8U *)(&Pri_MeasuStatVar);
  Ptr++;
  mem_cpy(Ptr, (INT8U *)(&RdData),1,Ptr,1);
  
  SET_STRUCT_SUM(Pri_MeasuStatVar);
  return MEASU_NO_ERR;
}
/**********************************************************************************
函数功能：处理总电能增量，或者总功率因数：根据功率方向和其他值，处理得到代数和。
          注意调用前，需得到正确得到功率方向！
入口：    PowerFlag-------有无功功率方向 
          ParseFlag-------1表示无功，0表示有功
          Data------------有无功数据
         PowerOrEnerg-----功率还是电能标志---0:功率;1:电能
出口：1------------获取增量成功；0-----------获取失败
**********************************************************************************/	
INT32U GetSumEngeOrPowerValue(INT8U PowerFlag,INT8U ParseFlag,MEASU_DATA Data,INT8U PowerOrEnerg)
{
  INT8U THF_Spec;
  INT32U PlusEnerg,NegaEnerg;
  
  PlusEnerg=0;
  NegaEnerg=0;

  //A相
  if(GET_BIT(PowerFlag,4*ParseFlag))
    NegaEnerg+=Data.A;
  else
    PlusEnerg+=Data.A;
  
  //B相
  if(GET_BIT(PowerFlag,1+4*ParseFlag))
    NegaEnerg+=Data.B;
  else
    PlusEnerg+=Data.B;
  
  //C相
   if(GET_BIT(PowerFlag,2+4*ParseFlag))
    NegaEnerg+=Data.C;
  else
    PlusEnerg+=Data.C;
  
  THF_Spec=Get_SysTHF_Mode();   
  if((PowerOrEnerg)&&(THF_Spec==TF_MODE))    //处理的是电能，并且是单方向
  {
    return (PlusEnerg+NegaEnerg);    
  }

  //功率按照代数和；双方向并且算合相电能
  if(PlusEnerg>=NegaEnerg)
     return (PlusEnerg-NegaEnerg);
  else
     return (NegaEnerg-PlusEnerg); 
 
}
/**********************************************************************************
函数功能：处理某相电能增量
入口：无
出口：0------------获取增量成功；1--------------获取失败
**********************************************************************************/	
INT8U  GetEnergChangeValue_PerOne(INT8U EngReg,INT8U FlowFlag,volatile INT32U *CurrEnerg,volatile INT32U *LastEnerg,volatile INT32U *ChangEnerg,INT32U Power,INT32U Curr)
{
  INT8U Flag;
  INT32U LastData,ChangData,RdData;
  FP32S CurData;
   
   
  Flag=Measu_RdAndCompData_3Times(EngReg,(INT8U *)&RdData);  //读取电能
  if(!Flag)
    return MEASU_RD3TIMES_ERR;
    
  CurData=Get_Sys_Pulse();
  
  if(CurData==0)
    return MEASU_PUL_SPEC_ERR;
  
  CurData=UNIT_KWH_KVAR/CurData;  
  CurData=(FP32S)RdData*CurData;
  LastData=*LastEnerg;   //取出上次值
  
  if(FlowFlag && CurData<=LastData)  //溢出
  {
    *CurrEnerg=(INT32U)CurData;
    *LastEnerg=(INT32U)CurData; 
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Info----->Energy OverFlow:LastData=%ld,CurData=%ld,Chang=%ld",LastData,(INT32U)CurData,*ChangEnerg); 
    ChangData=(INT32U)((FP32S)0x1000000*UNIT_KWH_KVAR/Get_Sys_Pulse()+CurData-LastData); //更新增量
#ifdef MEASU_LIMITED_EN 
    if(ChangData>=MAX_ENG_RATE*Get_Un()*Get_In())   //电量猛增，此次电能增量丢弃
    {
      *ChangEnerg=0;
       DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->CurData=0x%lx,RdData=0x%lx,Energ Change Over Limited!",CurData,RdData);       
       return  MEASU_ENERG_FLOW_ERR;
    }
    else
      *ChangEnerg=ChangData;
#endif    
    return MEASU_NO_ERR;
  }
  
  
  if((INT32U)CurData>LastData) //本次读出的电能>上次读出的电能,说明有电能递增------------PUCK
  {
    ChangData=(INT32U)(CurData-LastData);  //得出递增的部分--------------PUCK
    
#ifdef MEASU_LIMITED_EN 
    if(ChangData>=MAX_ENG_RATE*Get_Un()*Get_In())   //电量猛增，此次电能增量丢弃，所有数据不做更新
    {
      /**CurrEnerg=(INT32U)CurData;
      *LastEnerg=(INT32U)CurData;
      *ChangEnerg=0;
       DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->CurData=0x%lx,RdData=0x%lx,Energ Change Over Limited!",CurData,RdData);  
       */
      return  MEASU_ACCU_FLOW_ERR;
    }
#endif

     LastData=(INT32U)CurData;      //更新上次电能值------------------PUCK
  }
  else if(CurData<LastData) //电能倒走，或者翻转时出现读计量错，导致进入，此次丢掉，------------PUCK
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Energ Reversed:Cur=%ld,Last=%ld",(INT32U)CurData,LastData);
    //*CurrEnerg=(INT32U)CurData;
    //*LastEnerg=(INT32U)CurData;   //更新上次电能值,避免本次总比上次小------------------PUCK
    
    *CurrEnerg=0;
    *LastEnerg=0;   //更新上次电能值,避免本次总比上次小------------------PUCK
    *ChangEnerg=0;
    Measu_RdData(EngReg+0x13);
    Measu_RdData(EngReg+0x13);
    Measu_RdData(EngReg+0x13);
    return MEASU_ENR_REDUCE_ERR;
  }
  else                                   //本次读出的电能＝上次读出的电能，说明电能递增为0------------PUCK
  {
    ChangData=0;
  }
  
  if(EngReg>=REG_R_A_PENERG && EngReg<=REG_R_SUM_PENERG)  //有功
  {
    if(ChangData>0)
    {
      if(Power<(INT32U)Get_Start_Power()/2)
      {        
        if((EngReg!=REG_R_SUM_PENERG && Curr <(INT32U)Get_Start_Current()/2) ||(EngReg==REG_R_SUM_PENERG)) //功率和电流同时小于启动的一半,合相无电流
        *CurrEnerg=0;
        *LastEnerg=0;
        *ChangEnerg=0;
        Measu_RdData(EngReg+0x13);
        Measu_RdData(EngReg+0x13);
        Measu_RdData(EngReg+0x13);
        return MEASU_CREEP_ENERG_ADD;
      }
    }
    
  }
  
  *CurrEnerg=(INT32U)CurData;
  *LastEnerg=LastData;
  *ChangEnerg=ChangData;
  
  return MEASU_NO_ERR;
}
/**********************************************************************************
函数功能：处理分相有功，无功增量：根据当前功率方向，当前电能和上次电能差值算出
入口：无
出口：1------------获取增量成功；0-----------获取失败
**********************************************************************************/	
INT8U GetParseEnergChangeValue_PUCK(void)
{
  INT8U Flag,i;
  INT32U EnergFlow;  
  
#ifdef MEASURE_ERROR_STATICS_EN
    INT32U LastValue;
#endif
    
  if(CHECK_STRUCT_SUM(Pri_TempMeasuVar)==0)
    ASSERT(A_WARNING,0); 
  
  Flag=Measu_RdAndCompData_3Times(REG_R_ENFLAG,(INT8U *)&EnergFlow);  //读取电能寄存器工作状态:溢出标志
  if((!Flag)||(EnergFlow==0xffffff)) //避免状态寄存器读错后，对电能的影响，此值不可能都为0xffffff
  {
    return MEASU_RD3TIMES_ERR;
  }
  
  if(GET_BIT(EnergFlow,16))           //判断Sflag.16状态是否为1，若是，则表示校表请求
  {
    MeasuStatusMode.CalReqstErr=1;   //校表请求
    SET_STRUCT_SUM(MeasuStatusMode);
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Measu Cal Request During In Measure Circle");  
    return MEASU_CAL_ERR;
  }
  
  MeasuStatusMode.CalReqstErr=0;   //校表请求
  SET_STRUCT_SUM(MeasuStatusMode);
  //以下3项获取分相有功增量：处理溢出
  for(i=0;i<4;i++)
  {
    LastValue=*((&Pri_TempMeasuVar.AcEnerg.A)+i);
    Flag=GetEnergChangeValue_PerOne(REG_R_A_PENERG+i,GET_BIT(EnergFlow,i),(&Pri_TempMeasuVar.AcEnerg.A)+i,(&Pri_TempMeasuVar.Last_AcEnerg.A)+i,\
                                   (&Pri_TempMeasuVar.Change_AcEnerg.A)+i,*((&Pri_TempIntantVar.AcPower.A)+i),*((&Pri_TempIntantVar.Curr.A)+i));
#ifdef MEASURE_ERROR_STATICS_EN
    if(*((&Pri_TempMeasuVar.Change_AcEnerg.A)+i)!=0)
    {
      if(*((&Pri_TempIntantVar.AcPower.A)+i)<(INT32U)Get_Start_Power()/2)
      {
        if((i!=3 && *((&Pri_TempIntantVar.Curr.A)+i) <(INT32U)Get_Start_Current()/2) ||(i==3)) //功率和电流同时小于启动的一半,合相无电流
        {
          Measure_Err_Info.AcFlag=Flag;
          Measure_Err_Info.LastValue=LastValue;
          Measure_Err_Info.CurrValue=*((&Pri_TempMeasuVar.AcEnerg.A)+i);
          Measure_Err_Info.ChangeValue=*((&Pri_TempMeasuVar.Change_AcEnerg.A)+i);
          SET_STRUCT_SUM(Measure_Err_Info);
        }
      }
    }
#endif
    
    if(Flag!=MEASU_NO_ERR)
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get Ac_Energe Change Error,Index=%d",i);  
      return Flag;
    }
  }
 
  //以下3项获取分相无功增量：处理溢出
  for(i=0;i<4;i++)
  {
    Flag=GetEnergChangeValue_PerOne(REG_R_A_QENERG+i,GET_BIT(EnergFlow,4+i),(&Pri_TempMeasuVar.ReacEnerg.A)+i,(&Pri_TempMeasuVar.Last_ReacEnerg.A)+i,\
                                   (&Pri_TempMeasuVar.Change_ReacEnerg.A)+i,*((&Pri_TempIntantVar.ReacPower.A)+i),*((&Pri_TempIntantVar.Curr.A)+i));
    if(Flag!=MEASU_NO_ERR)
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get Reac_Energe Change Error,Index=%d",i);  
      return Flag;
    }
  }
  SET_STRUCT_SUM(Pri_TempMeasuVar);  
  return MEASU_NO_ERR;
}
/**********************************************************************************
函数功能：处理合相有功，无功增量：根据分相功率方向，分相电能增量，TH/TF模式算出。
入口：无
出口：1------------获取增量成功；0-----------获取失败
**********************************************************************************
INT8U GetSumEnergChangeValue_PUCK(void)
{
  INT8U PowerFlag;
  //以下获取总有功增量/无功增量  
  PowerFlag=*(INT8U *)(&MeasuStatVar+2);    //功率方向
  Pri_TempMeasuVar.Change_AcEnerg.Sum=GetSumEngeOrPowerValue(PowerFlag,0,Pri_TempMeasuVar.Change_AcEnerg,1);
  Pri_TempMeasuVar.Change_ReacEnerg.Sum=GetSumEngeOrPowerValue(PowerFlag,1,Pri_TempMeasuVar.Change_ReacEnerg,1);
  return 1;
}
**********************************************************************************/	

/**********************************************************************************
函数功能：读取某相电压有效值。
入口：
           Reg----------寄存器地址
           VoltData------------目标缓冲区  
出口：1----------获取成功；0----------获取失败
**********************************************************************************/	
INT8U GetVoltValue_PerOne(INT8U Reg,volatile INT32U *VoltData)
{
  INT8U volatile Flag;
  INT32U volatile RdData;
  FP32S  ResultData;

  
  Flag=Measu_RdAndCompData_3Times(Reg,(INT8U *)&RdData);
  if(!Flag)
      return MEASU_RD3TIMES_ERR;
      
  if(RdData>=0x00800000)
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get Volt Error,RdData=0x%lx",RdData);  
    return MEASU_VOLT_ERR;
  }

  ResultData=(FP32S)RdData*(FP32S)UNIT_V/pow(2,13);
  RdData=(INT32U)ResultData;
  *VoltData=RdData;    
  return MEASU_NO_ERR;
}
/**********************************************************************************
函数功能：读取某相电流有效值。
入口：
           Reg----------寄存器地址
           UorIflag-----------0：电压；1----电流
           DstBuf------------目标缓冲区  
出口：1----------获取成功；0----------获取失败
**********************************************************************************/	
INT8U GetCurrValue_PerOne(INT8U Reg,volatile INT32U *LastCur,volatile INT32U *CurrCur)
{
  INT8U volatile Flag,ParseSpec,ISpec;
  INT32U volatile RdData;
  INT32U LastData;
  FP32S  ResultData;

   
   LastData=*CurrCur;   //将当前的值作为上次的值保存！
   
   Flag=Measu_RdAndCompData_3Times(Reg,(INT8U *)&RdData);
   if(!Flag)
     return MEASU_RD3TIMES_ERR;
      
   if(RdData>=0x00800000)
   {
     DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get Curr Error,RdData=0x%lx",RdData);  
     return MEASU_CURR_ERR;
   }
      
   ResultData=(FP32S)RdData*(FP32S)UNIT_A/pow(2,13);
   ISpec=Get_SysCurr_Mode();
   ResultData/=I_RATE_CONST[ISpec];        

   *LastCur=LastData;
   *CurrCur=(INT32U)ResultData;
   return MEASU_NO_ERR;      
}
/**********************************************************************************
函数功能：读取A/B/C的电压/电流有效值。
入口：无
出口：无
**********************************************************************************/	
INT8U GetVoltCurrValue_PUCK(void)
{
  INT8U Flag,i;
  
  if((CHECK_STRUCT_SUM(Pri_TempIntantVar)==0) ||(CHECK_STRUCT_SUM(Pri_TempMeasuVar)==0))
    ASSERT(A_WARNING,0);
  
  //获取电压+零序电压  
  for(i=0;i<4;i++)
  {
    if(3!=i)
      Flag=GetVoltValue_PerOne(REG_R_A_U+i,&(Pri_TempIntantVar.Volt.A)+i);
    else
      Flag=GetVoltValue_PerOne(REG_R_SUM_U,&(Pri_TempIntantVar.Volt.Sum));
    if(Flag!=MEASU_NO_ERR)
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get Volt Error,Index=%d",i);  
      return Flag;
    }    
  }
  
  //获取三相电流+零序电流 
  for(i=0;i<4;i++)
  {
    Flag=GetCurrValue_PerOne(REG_R_A_I+i,(&Pri_TempMeasuVar.Last_Curr.A)+i,(&Pri_TempIntantVar.Curr.A)+i);
    if(Flag!=MEASU_NO_ERR)
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get Curr Error,Index=%d",i);  
      return Flag;
    }   
  }
  
  SET_STRUCT_SUM(Pri_TempIntantVar);
  SET_STRUCT_SUM(Pri_TempMeasuVar);  
  return MEASU_NO_ERR;      
}
/**********************************************************************************
函数功能：计算出合相功率：合相有功功率、合相无功功率、合相视在功率
入口：无 
出口：无
**********************************************************************************/	
INT8U GetSumPowerValue_PerOne(void)
{
  INT8U PowerFlag;
  
  PowerFlag=*(INT8U *)(&Pri_MeasuStatVar+1);
  
  //合相有功
  Pri_TempIntantVar.AcPower.Sum=GetSumEngeOrPowerValue(PowerFlag,0,Pri_TempIntantVar.AcPower,0);
  
 //合相无功
  Pri_TempIntantVar.ReacPower.Sum=GetSumEngeOrPowerValue(PowerFlag,1,Pri_TempIntantVar.ReacPower,0);
  
  //Measu_InstantData_ToPub_PUCK.AppPower.Sum=Measu_InstantData_ToPub_PUCK.AppPower.A+Measu_InstantData_ToPub_PUCK.AppPower.B+Measu_InstantData_ToPub_PUCK.AppPower.C;
  return MEASU_NO_ERR;
}
/**********************************************************************************
函数功能：读取功率
入口：
      PowerReg--------功率寄存器
      SumFlag-------功率标志,1------合相;0--------分相功率；
      SFlag-------视在功率标志,1------视在功率标志;0--------不是视在功率标志；
出口：
**********************************************************************************/	
INT8U GetPowerValue_PerOne(INT8U PowerReg,INT8U SumFlag,volatile INT32U *CurrPower,INT8U SFlag)
{
  INT8U Flag;
  INT32U RdData,PulseConst;
  FP32S Result,temp;
  
  Flag=Measu_RdAndCompData_3Times(PowerReg,(INT8U *)&RdData);
  if(!Flag)
     return MEASU_RD3TIMES_ERR;
  
  
  PulseConst=Get_Sys_Pulse();
  
  //SOURCE CODE  125 5001 
  //有功，单位为k,0.000001kw=0.001w/var，计量芯片出的单位是w/var。3200/256*1000=12500;3200/64*1000=50000;
  if(SumFlag==1)  //合相
  {
    temp=3.2/64*UNIT_KW_KAR;
  }
  else
  {
    temp=3.2/256*UNIT_KW_KAR;
  }
  if(RdData>0x00800000)
  {
    if(SFlag)  //视在功率不可能为负
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->App Power Is Negative,ReadValue=0x%lx",RdData);  
      return MEASU_POWER_ERR;
    }
    Result=(FP32S)(0x1000000-RdData)*temp;
    Result/=(FP32S)PulseConst;
  }
  else if(RdData==0x00800000)
  {
    Result=0;
  }
  else
  {
    Result=(FP32S)RdData*temp;
    Result/=(FP32S)PulseConst;
  }

  *CurrPower=(INT32U)Result;
    return MEASU_NO_ERR;   
}
/**********************************************************************************
函数功能：读取A/B/C/合相有功功率,无功功率,视在功率,修正小电流下的功率方向，修正小电流值。
入口：无
出口：1---------成功；0-----------失败
**********************************************************************************/	
INT8U GetAllPowerValue_PUCK(void)
{
  INT8U Flag,i;

  if(CHECK_STRUCT_SUM(Pri_TempIntantVar)==0)
    ASSERT(A_WARNING,0); 
  
 //A相有功功率,无功功率,视在功率
   for(i=0;i<3;i++)
   {
    Flag=GetPowerValue_PerOne(REG_R_A_AP+4*i,0,(&(Pri_TempIntantVar.AcPower.A)+4*i),i/2);
    if(Flag!=MEASU_NO_ERR)
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get A_Power Error,Inde=%d",i);  
      return Flag;
    }      
   } 

 
 //B相有功功率,无功功率,视在功率
   for(i=0;i<3;i++)
   {
    Flag=GetPowerValue_PerOne(REG_R_B_AP+4*i,0,(&(Pri_TempIntantVar.AcPower.B)+4*i),i/2);
    if(Flag!=MEASU_NO_ERR)
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get B_Power Error,Inde=%d",i);  
      return Flag;
    }      
   }   


 //C相有功功率,无功功率,视在功率
   for(i=0;i<3;i++)
   {
      Flag=GetPowerValue_PerOne(REG_R_C_AP+4*i,0,(&(Pri_TempIntantVar.AcPower.C)+4*i),i/2);
      if(Flag!=MEASU_NO_ERR)
      {
        DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get C_Power Error,Inde=%d",i);  
        return Flag;
      }      
   } 

    
 //合相有功/无功/视在功率
 for(i=0;i<3;i++)
 {
    Flag=GetPowerValue_PerOne(REG_R_SUM_AP+4*i,1,(&(Pri_TempIntantVar.AcPower.Sum)+4*i),i/2);
    if(Flag!=MEASU_NO_ERR)
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get Sum_Power Error,Inde=%d",i);  
      return Flag;
    }      
 }  
  
  SET_STRUCT_SUM(Pri_TempIntantVar);
  return MEASU_NO_ERR;
}
/**********************************************************************************
函数功能：相功率因数
入口：
      PowerReg----------功率因数寄存器
出口：MEASU_NO_ERR---------成功；-----------失败
**********************************************************************************/	
INT8U GetPowerFactorValue_PerOne(INT8U Reg,volatile INT32U *DstPower)
{
  INT8U Flag;
  INT32U volatile RdData;
  FP32S Result;
  
  Flag=Measu_RdAndCompData_3Times(Reg,(INT8U *)&RdData);
  if(!Flag)
    return MEASU_RD3TIMES_ERR;
  
  //单位为1,实际是0.0000001,倍数：1000000；系数：1000000/2^23=31250/262144=1/8.388608，精度0.000001
  if(RdData>0x00800000)
    Result=((FP32S)0x01000000-(FP32S)RdData);
  else
    Result=(FP32S)RdData;    
  
  Result=Result*UNIT_FACTOR/pow(2,23);
    
  *DstPower=(INT32U)Result;
  return MEASU_NO_ERR;      
}
/**********************************************************************************
函数功能：读取A/B/C/合相的功率因数。
入口：无
出口：无
**********************************************************************************/	
INT8U GetAllPowerFactorValue_PUCK(void)
{
  INT8U Flag,i;
  INT32U volatile *Ptr;
  
  if(CHECK_STRUCT_SUM(Pri_TempIntantVar)==0)
    ASSERT(A_WARNING,0);
  
  for(i=0;i<4;i++,Ptr++)
  {    
    Flag=GetPowerFactorValue_PerOne(REG_R_A_PF+i,&(Pri_TempIntantVar.PFactor.A)+i);
    if(Flag!=MEASU_NO_ERR)
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get Power Factor Err,Index=%d!",i);  
      return Flag;
    }
  }   
  SET_STRUCT_SUM(Pri_TempIntantVar);
  return MEASU_NO_ERR;      
}
/**********************************************************************************
函数功能：读取线频率。
入口：无
出口：无
**********************************************************************************/	
INT8U GetFreqValue_PUCK(void)
{
  INT8U Flag;
  INT32U RdData;
  FP32S Result;
  
  Flag=Measu_RdAndCompData_3Times(REG_R_SUM_FREQ,(INT8U *)&RdData);
  if(!Flag)
      return MEASU_RD3TIMES_ERR;
  
  if(RdData>=0x00800000)
  {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get Frequecy Err,Value too much(Value=0x%lx)",RdData);  
      return MEASU_FREQU_ERR;
  }
  
  if(CHECK_STRUCT_SUM(Pri_TempIntantVar)==0)
    ASSERT(A_WARNING,0);
  
  Result=(FP32S)RdData*UNIT_HZ/pow(2,13);
  
  //注意:需要增加各式转换代码,调试时加!
  //add code ......................
  Pri_TempIntantVar.Freq.A=(INT32U)Result;
  Pri_TempIntantVar.Freq.B=(INT32U)Result;
  Pri_TempIntantVar.Freq.C=(INT32U)Result; 
  Pri_TempIntantVar.Freq.Sum=(INT32U)Result; 
  
  SET_STRUCT_SUM(Pri_TempIntantVar);
  return MEASU_NO_ERR;
}
/**********************************************************************************
函数功能：相序错误状态字。注：只有电流相序检测功能需要通过相序检测使能控制寄存器EnDtIorder将其开启
入口：无
出口：无
**********************************************************************************/	
INT8U GetSeqErrFlagValue_PUCK(void)
{
  INT8U Flag;
  INT32U RdData;
  
  Flag=Measu_RdAndCompData_3Times(REG_R_SIG,(INT8U *)&RdData);
  if(!Flag)
    return MEASU_RD3TIMES_ERR;
  
  if(CHECK_STRUCT_SUM(Pri_MeasuStatVar)==0)
    ASSERT(A_WARNING,0);
  
  /*
  if(RdData>=0x00800000)
    return 0;
  */  
  
  if(GET_BIT(RdData,7))  //SIG信号为高，异常
    return MEASU_SIGN_ERR;
  
  Pri_MeasuStatVar.V_SeqErr=(GET_BIT(RdData,3))?1:0;
  Pri_MeasuStatVar.I_SeqErr=(GET_BIT(RdData,4))?1:0;
  
  Pri_MeasuStatVar.A_Creep=(GET_BIT(RdData,9))?1:0;
  Pri_MeasuStatVar.B_Creep=(GET_BIT(RdData,10))?1:0;
  Pri_MeasuStatVar.C_Creep=(GET_BIT(RdData,11))?1:0;
  
  SET_STRUCT_SUM(Pri_MeasuStatVar);
  return MEASU_NO_ERR;
}
/**********************************************************************************
函数功能：功率因素校正
入口：无
出口：无
**********************************************************************************/										 											 
void Power_Factor_Modi_PUCK(void)
{
#ifdef  POWER_FACTOR_ADJ

  INT8U i,U_Spec;
  
  if(CHECK_STRUCT_SUM(Pri_TempIntantVar)==0)
    ASSERT(A_WARNING,0);
  
  U_Spec=Get_SysVolt_Mode();
  
  for(i=0;i<3;i++)   //分相功率因数
  {
    if(*(&(Pri_TempIntantVar.Volt.A)+i)<LITTLE_VOLT[U_Spec])
      *(&(Pri_TempIntantVar.PFactor.A)+i)=0;
    if(*(&(Pri_TempIntantVar.Curr.A)+i)<=(Get_Start_Current()*UNIT_A))   //只要有电流
      *(&(Pri_TempIntantVar.PFactor.A)+i)=UNIT_FACTOR;
  }  
  
  //合相功率因数
  //电压均小于最小电压，功率因素置0；
  if(Get_Max_Num(Pri_TempIntantVar.Volt.A,Pri_TempIntantVar.Volt.B,Pri_TempIntantVar.Volt.C)<LITTLE_VOLT[U_Spec])
    Pri_TempIntantVar.PFactor.Sum=0;
  
  //电流均小于启动电流电压，功率因素置1；
  if(Get_Max_Num(Pri_TempIntantVar.Curr.A,Pri_TempIntantVar.Curr.B,Pri_TempIntantVar.Curr.C)<=(Get_Start_Current()*UNIT_A))
    Pri_TempIntantVar.PFactor.Sum=UNIT_FACTOR;
  
  SET_STRUCT_SUM(Pri_TempIntantVar);
  
#endif  
}        
/**********************************************************************************
小电流,置相关标志
**********************************************************************************/	
void Set_Little_Cur_Flag(void)
{
#ifdef LITTLE_CURR_ADJ
  INT8U I_Spec,ParseSpec,i;
  volatile INT32U *Ptr32,*LastCur;
  
  if(CHECK_STRUCT_SUM(Pri_TempIntantVar)==0)
   ASSERT(A_WARNING,0); 
  
  I_Spec=Get_SysCurr_Mode();
  for(i=0;i<3;i++)
  {
    Ptr32=&(Pri_TempIntantVar.Curr.A)+i;
    LastCur=&(Measu_InstantData_ToPub_PUCK.Curr.A)+i;
    if(*Ptr32<=CUR_1_PER_100[I_Spec])  //[0,15]mA
    {
       *(&Curr_1_Per_100.A+i)=1;
       
       if(*Ptr32<=(Get_Start_Current()*UNIT_A))          //小于启动电流的：[0,0.1%In)mA，即[0，0.75mA]
         *(&Curr_Under_Istart.A+i)=1;
       else
         *(&Curr_Under_Istart.A+i)=0; 
       
       if((*Ptr32>=(Get_Start_Current()*UNIT_A))&&(*Ptr32<=Current_down_setup[I_Spec]))   //[0.1%In,6]mA
         *(&Curr_1P100_Bad.A+i)=1;
       else
         *(&Curr_1P100_Bad.A+i)=0;
          
       if((*Ptr32>Current_down_setup[I_Spec])&&(*Ptr32<Current_up_setup[I_Spec]))  //(6,13.5)mA：本次电流
       {
          *(&Curr_In_6_13x5.A+i)=1;
          if((*LastCur>=Current_down_setup[I_Spec])&&(*LastCur<=Current_up_setup[I_Spec]))  //上次电流，也在(6,13.5)mA区域
            *(&Curr_6_13x5_Slipe.A+i)=0;   //一直处于 (6,13.5)mA区域
          else
            *(&Curr_6_13x5_Slipe.A+i)=1;   //刚刚进入 (6,13.5)mA区域
       }
       else                                                                              //[13.5,15]mA：本次电流已退出(6,13.5)mA区域
       {
         *(&Curr_In_6_13x5.A+i)=0;
         if((*LastCur>Current_down_setup[I_Spec])&&(*LastCur<Current_up_setup[I_Spec]))   //上次电流，在(6,13.5)mA区域
            *(&Curr_6_13x5_Slipe.A+i)=1;   //刚退出(6,13.5)mA区域
         else
           *(&Curr_6_13x5_Slipe.A+i)=0;    //一直处于 (6,13.5)mA区域之外
       }
    }
    else                                                                
    {
      *(&Curr_Under_Istart.A+i)=0; 
      *(&Curr_1_Per_100.A+i)=0;   //(15,max]mA
      *(&Curr_1P100_Bad.A+i)=0;
      *(&Curr_In_6_13x5.A+i)=0;
      *(&Curr_6_13x5_Slipe.A+i)=0;
    }
  }
  
  ParseSpec=Get_SysParse_Mode();
  if(ParseSpec==PARSE_331)
    Curr_1_Per_100.B=0; 
  
  SET_STRUCT_SUM(Pri_TempIntantVar);
  SET_STRUCT_SUM(Curr_1_Per_100);
  SET_STRUCT_SUM(Curr_In_6_13x5);
  SET_STRUCT_SUM(Curr_6_13x5_Slipe);  
#endif
}
/**********************************************************************************
函数功能：在小电流下，对电流、有功功率，无功功率，视在功率，电流的校正
入口：无
出口：无
**********************************************************************************/									 											 
void Power_Volt_Curr_Modi_PUCK(void)
{
  INT8U Flag,i;
  INT8U U_Spec,I_Spec;
  INT32U TempCurr;
  FP32S tempPQS,temp;
  
  if(CHECK_STRUCT_SUM(Pri_TempMeasuVar)==0)
    ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(Pri_TempIntantVar)==0)
    ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(Pri_MeasuStatVar)==0)
    ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(Curr_1_Per_100)==0)
    ASSERT(A_WARNING,0);
  
  U_Spec=Get_SysVolt_Mode();
  I_Spec=Get_SysCurr_Mode();
  
  /*
  for(i=0;i<4;i++)  //包括零序电流的处理
  {
    if(*(&Curr_1_Per_100.A+i)==1 && *(&Curr_Under_Istart.A+i)==1)   //小电流，且处于 0.1Ib以下
    {
      ;//  *(&(Pri_TempIntantVar.Curr.A)+i)=0;   //小于启动电流，强制将电流置0
    }
  }
  
  for(i=0;i<4;i++)  //包括零序电流的处理
  {
    if(*(&Curr_1_Per_100.A+i)==1 && *(&Curr_1P100_Bad.A+i)==1)   //小电流，且处于[0.1%In,6]mA区间
    {
      ;//  *(&(Pri_TempIntantVar.Curr.A)+i)=0;   //小于启动电流，强制将电流置0
    }
  }
  

    
  for(i=0;i<4;i++)  //包括零序电流的处理
  {
    if(*(&Curr_1_Per_100.A+i)==1 && *(&Curr_In_6_13x5.A+i)==1  && *(&Curr_6_13x5_Slipe.A+i)==1)   //小电流，刚进入 [6,13.5]mA区间
    {
      ;//  *(&(Pri_TempIntantVar.Curr.A)+i)=8;   //
    }
  }
  
  */
    
  for(i=0;i<3;i++)
  {
    if(*(&Curr_1_Per_100.A+i)==1)
    {
      //小电流情况下，根据P Q算出视在功率
      TempCurr=*(&Pri_TempIntantVar.Curr.A+i); //依次取出各相电流 09-02-05
      temp=pow(*(&Pri_TempIntantVar.AcPower.A+i),2);
      tempPQS=temp+pow(*(&Pri_TempIntantVar.ReacPower.A+i),2);
      tempPQS=sqrt(tempPQS);
      *(&Pri_TempIntantVar.AppPower.A+i)=(INT32U)tempPQS;
      
      if(*(&Pri_TempIntantVar.Volt.A+i)>=LITTLE_VOLT[U_Spec])  //有电压，有小电流
      {
        tempPQS=tempPQS/1000;    //转成单位：w 
        TempCurr=(INT32U)((tempPQS/((FP32S)(*(&Pri_TempIntantVar.Volt.A+i))/(FP32S)UNIT_V))*(FP32S)UNIT_A);     //电压已经确保>0,算出电流
        if(TempCurr>Current_threshold_reset[I_Spec])   //算出的电流超过计量出的电流，置0
          TempCurr=0;
        *(&(Pri_TempIntantVar.Curr.A)+i)=TempCurr; //更新电流 -----09-04-19  PUCK      
      }
      if(*(&Curr_In_6_13x5.A+i)==1)
      ;     
    
      if(TempCurr<=(Get_Start_Current()*UNIT_A))//在启动电流内，有、无功功率,功率方向
      {
        *(&Pri_TempIntantVar.AppPower.A+i)=0;
        *(&Pri_TempIntantVar.ReacPower.A+i)=0;
        if(i==0)
          Pri_MeasuStatVar.A_APwrDir=Pri_MeasuStatVar.A_RPwrDir=0;
        if(i==1)
          Pri_MeasuStatVar.B_APwrDir=Pri_MeasuStatVar.B_RPwrDir=0;
        if(i==2)
          Pri_MeasuStatVar.C_APwrDir=Pri_MeasuStatVar.C_RPwrDir=0;
      }      
    }
  }
  
  
   
 //小电流情况下，合相有功功率，合相无功功率，合相视在功率修正
 if((Curr_1_Per_100.A)||(Curr_1_Per_100.B)||(Curr_1_Per_100.C))
 {
   if((Pri_MeasuStatVar.A_APwrDir==0)&&(Pri_MeasuStatVar.B_APwrDir==0)&&(Pri_MeasuStatVar.C_APwrDir==0))
    Pri_MeasuStatVar.SUM_APwrDir=0;
   
   if((Pri_MeasuStatVar.A_RPwrDir==0)&&(Pri_MeasuStatVar.B_RPwrDir==0)&&(Pri_MeasuStatVar.C_RPwrDir==0))
    Pri_MeasuStatVar.SUM_RPwrDir=0;
   
   Flag=GetSumPowerValue_PerOne();  
   if(Flag!=MEASU_NO_ERR)
   {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Get Ac/Reac/App_Power_Sum Error Under Tiny Current!");  
      return ;
   }
  }
   
  SET_STRUCT_SUM(Curr_1_Per_100);
  SET_STRUCT_SUM(Pri_MeasuStatVar);
  SET_STRUCT_SUM(Pri_TempIntantVar);
  SET_STRUCT_SUM(Pri_TempMeasuVar);
}

/**********************************************************************************
函数功能：功率方向校正
入口：无
出口：无
**********************************************************************************/										 											 
void Power_Direc_Modi_PUCK(void)
{
  INT8U ParseMode;
  FP32S P,Q;
  
  if(CHECK_STRUCT_SUM(Pri_MeasuStatVar)==0)
    ASSERT(A_WARNING,0);
  
  ParseMode=Get_SysParse_Mode();
  
  P=Get_Start_Power();
  Q=P;

  if(ParseMode==PARSE_331)        //331
  { 
    if((FP32S)Pri_TempIntantVar.AcPower.Sum<P) //功率小于 启动功率
      Pri_MeasuStatVar.SUM_APwrDir=0;         //总有功方向置正
    if((FP32S)Pri_TempIntantVar.ReacPower.Sum<Q)//功率小于 启动功率
      Pri_MeasuStatVar.SUM_RPwrDir=0;         //总无功方向置正
  }
  else                             //341
  {   
    if(Pri_TempIntantVar.AcPower.Sum<(1.5L*P))//功率小于 启动功率
      Pri_MeasuStatVar.SUM_APwrDir=0;          //总有功方向置正 
    if(Pri_TempIntantVar.ReacPower.Sum<(1.5L*Q))//功率小于 启动功率
      Pri_MeasuStatVar.SUM_RPwrDir=0;         //总无功方向置正
  }
  
  if((FP32S)Pri_TempIntantVar.AcPower.A<P)//功率小于 启动功率
    Pri_MeasuStatVar.A_APwrDir=0;         //A相有功方向置正 
  if((FP32S)Pri_TempIntantVar.ReacPower.A<Q)//功率小于 启动功率
    Pri_MeasuStatVar.A_RPwrDir=0;        //A相无功方向置正
  
  if((FP32S)Pri_TempIntantVar.AcPower.B<P)//功率小于 启动功率
    Pri_MeasuStatVar.B_APwrDir=0;       //B相有功方向置正
  if((FP32S)Pri_TempIntantVar.ReacPower.B<Q)//功率小于 启动功率
    Pri_MeasuStatVar.B_RPwrDir=0;       //B相无功方向置正
  
  if((FP32S)Pri_TempIntantVar.AcPower.C<P)//功率小于 启动功率
    Pri_MeasuStatVar.C_APwrDir=0;       //C相有功方向置正
  if((FP32S)Pri_TempIntantVar.ReacPower.C<Q)//功率小于 启动功率
    Pri_MeasuStatVar.C_RPwrDir=0;       //C相无功方向置正
  SET_STRUCT_SUM(Pri_MeasuStatVar);
}

/**********************************************************************************        
函数功能：小电流抖动处理。
入口：无
出口：无

void TinyCurrDisProc_PUCK(void)
{
  INT32U volatile *LastPtr,*CurPtr;
  INT32U temp,TempData;
  INT8U i;
  
  i=CHECK_STRUCT_SUM(Pri_TempMeasuVar);
  ASSERT(A_WARNING,1==i);  
  
  
  LastPtr=&(Pri_TempMeasuVar.Last_Curr.A);
  CurPtr=&(Pri_TempIntantVar.Curr.A);
  
  for(i=0;i<3;i++)  //A/B/C三相电流处理
  {
    temp=LastPtr[i];
    if(temp!=0)  //上次读取电流不为0
    {    
      TempData=CurPtr[i]/temp;
      if(TempData>100)  //电流大幅抖动：当前读出电流是上次的100倍：实际电流=0
      {
        LastPtr[i]=CurPtr[i];
        CurPtr[i]=0;      
      }
      else            //电流基本连续：当前读出电流和上次电流基本连续
      {
        LastPtr[i]=CurPtr[i];
      }
    }
    else             //上次读取电流为0
    {
      if(CurPtr[i]>=100)  //电流大幅抖动：上次读取电流为0，当前读取的电流大于10mA
      {
         LastPtr[i]=CurPtr[i];
         //CurPtr[i]=0; 
      }
      else                //电流基本连续：上次读取电流为0，当前读取的电流小于10mA
      {
        LastPtr[i]=CurPtr[i];
      }
    }
  }
 SET_STRUCT_SUM(Pri_TempMeasuVar);
 SET_STRUCT_SUM(Pri_TempIntantVar);
}
**********************************************************************************/	
/**********************************************************************************
函数功能：小电流下启动电流,使之在小电流情况下继续计量
入口：无
出口：无

void Modi_StartCurr_TinyCurr(void)
{
  INT8U Flag;
  
  Flag=CHECK_STRUCT_SUM(Curr_6_13x5_Slipe);
  Flag&=CHECK_STRUCT_SUM(Curr_In_6_13x5);
  ASSERT(A_WARNING,1==Flag); 
  
  if((Curr_6_13x5_Slipe.A==1)||(Curr_6_13x5_Slipe.B==1)||(Curr_6_13x5_Slipe.C==1))
  {
    EnMeasuCal();
    if((Curr_In_6_13x5.A==1)||(Curr_In_6_13x5.B==1)||(Curr_In_6_13x5.C==1))
    {
      Flag=Measu_WrAndCompData_3Times(REG_W_ISTART,0);
      if(Flag)
        DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Clear StartCurr Para OK In Little Curr!");
      else
        DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Clear StartCurr Para Failed In Little Curr!");
    }
    else   
    {
      //Flag=Check_MeasuParaBuf_Sum3();
      //Flag&=Measu_WrAndCompData_3Times(REG_W_ISTART,Measu_ParaBuf[MEASU_PARA_CURSTART]);
      if(Flag)
      {
        DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Restore StartCurr Para Succeed In Little Curr!");  
      }
      else
        DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Restore StartCurr Para Failed In Little Curr!");  
    }
    DisMeasuCal();

    if(Flag)  //设置不成功，下次还要继续设置
    {
      Curr_6_13x5_Slipe.A=0;
      Curr_6_13x5_Slipe.B=0;
      Curr_6_13x5_Slipe.C=0;
    }
  }
  SET_STRUCT_SUM(Curr_6_13x5_Slipe);
}
**********************************************************************************/	
/**********************************************************************************
函数功能：每秒读7022数据，得到当前的瞬时数据
入口：无
出口：无
**********************************************************************************/	
INT8U  GetMeasuData_PerSec(void)
{
  INT8U Flag;
  
  Flag=ChkMeasuICWorkStatus();  //检查SIG信号
  if(Flag!=MEASU_NO_ERR)
    return Flag;
  
  Flag=CheckBothCs();           //检查计量芯片CS和EPPROM cs是否相等
  if(Flag!=MEASU_NO_ERR)
    return Flag;
   
  Flag=GetParseEnergChangeValue_PUCK();  //获取校表请求标志，分相有功，无功增量.24ms
  if(Flag!=MEASU_NO_ERR)
    return Flag;
  
  //OS_TimeDly_Ms(20);  
  
  Flag=GetVoltCurrValue_PUCK();          //获取A/B/C的电压/电流有效值。25ms
  if(Flag!=MEASU_NO_ERR)
    return Flag;  
  Set_Little_Cur_Flag();                //小电流,置相关标志-----------------------PUCK
  
  //OS_TimeDly_Ms(20); 
  
  Flag=Get_PQ_PowerDir_PUCK();           //有功和无功功率方向处理
  if(Flag!=MEASU_NO_ERR)
    return Flag;
  
  
  //OS_TimeDly_Ms(20); 
  
  Flag=GetAllPowerValue_PUCK();         //获取A/B/C/合相有功功率,无功功率,视在功率.30ms
  if(Flag!=MEASU_NO_ERR)
    return Flag;  
  
  Flag=GetAllPowerFactorValue_PUCK();   //获取A/B/C/合相的功率因数.16ms
  if(Flag!=MEASU_NO_ERR)
    return Flag;  
  
  Flag=GetFreqValue_PUCK();             //获取线频率
  if(Flag!=MEASU_NO_ERR)
    return Flag;
  
  Flag=GetSeqErrFlagValue_PUCK();       //相序错误标示
  if(Flag!=MEASU_NO_ERR)
    return Flag;
  
  Power_Volt_Curr_Modi_PUCK();        //小电流下功率、电压、电流修正-----------------------PUCK   
  Power_Factor_Modi_PUCK();           //功率因数修正-----------------------PUCK  
  Power_Direc_Modi_PUCK();            //功率方向修正-----------------------PUCK
  
  //OS_TimeDly_Ms(20);   
   RefreshQuadrant();                 //更新象限   
   GetAllAngel_PUCK();                //获取总,A,B,C的电压电流相角
 
#ifdef Measu_VOLTANGLE_EN
   GetVoltAngle_PUCK();               //获取相邻2相的电压夹角
#endif

  

  
  return Flag;
  
}

/**********************************************************************************
函数功能：检查7022数据上限，将7022数据转换处理，提交应用层
入口：无
出口：MEASU_NO_ERR-------获取成功；----------获取失败
**********************************************************************************/						
INT8U ChkPowerLimitAndConvertData(void)      //test if power value is in the correct range
{
  INT32U Flag,ParseSpec,temp;
  INT8U i;
  INT32U volatile *PtrSrc,*PtrDst;
  
  Flag=MEASU_NO_ERR;

  ParseSpec=Get_SysParse_Mode();
  /*************************以下处理瞬时数据********************************/
  if(CHECK_STRUCT_SUM(Pri_TempIntantVar)==0)
    ASSERT(A_WARNING,0);

  if(CHECK_STRUCT_SUM(Pri_MeasuStatVar)==0)
    ASSERT(A_WARNING,0);
  
  //有功功率，无功功率，视在功率：4*3=12
  PtrSrc=&(Pri_TempIntantVar.AcPower.A);
  PtrDst=&(Measu_InstantData_ToPub_PUCK.AcPower.A);
  for(i=0;i<12;i++) 
  {
    if((i==3)||(i==7)||(i==11))
      temp=3;
    else
      temp=1;
    if(PtrSrc[i]<temp*MAX_POWER_RATE*Get_Un()*Get_In() || Get_Cal_Exsit())  //校表状态下，不判数据逻辑性
      PtrDst[i]=PtrSrc[i];
    else 
      Flag=MEASU_DATA_LOGIC_ERR;
  }
  
  //电压：4项
  PtrSrc=&(Pri_TempIntantVar.Volt.A);
  PtrDst=&(Measu_InstantData_ToPub_PUCK.Volt.A);
  for(i=0;i<4;i++)
  {
    if(3==i &&Pri_MeasuStatVar.V_SeqErr)  //电压逆相序
    {
      PtrDst[i]=PtrSrc[i];
      break;
    }      
    if(PtrSrc[i]<MAX_U_RATE*Get_Un()|| Get_Cal_Exsit())  //校表状态下，不判数据逻辑性
      PtrDst[i]=PtrSrc[i];
    else 
      Flag=MEASU_DATA_LOGIC_ERR;
  } 
  if(ParseSpec==PARSE_331)  //331模式下，B相电压置为0 
    PtrDst[1]=0;
  
  //电流：4项;  
  PtrSrc=&(Pri_TempIntantVar.Curr.A);
  PtrDst=&(Measu_InstantData_ToPub_PUCK.Curr.A);
  for(i=0;i<4;i++)
  {
    if(PtrSrc[i]<MAX_I_RATE*Get_In() || Get_Cal_Exsit())  //校表状态下，不判数据逻辑性
      PtrDst[i]=PtrSrc[i];
    else 
      Flag=MEASU_DATA_LOGIC_ERR;
    
    //if(PtrSrc[i]<=Current_zero[I_Spec])
    //  PtrDst[i]=0;
  } 
  
  //功率因数：4项；频率：4项；象限：4项；夹角：4项；
   mem_cpy((void *)(&(Measu_InstantData_ToPub_PUCK.PFactor.A)),\
                   (void *)(&(Pri_TempIntantVar.PFactor.A)),sizeof(MEASU_DATA)*3+sizeof(MEASUFLAG),\
                   (void *)(&(Measu_InstantData_ToPub_PUCK.PFactor.A)),\
                    sizeof(MEASU_DATA)*3+sizeof(MEASUFLAG));
  
   /*************************以下处理状态数据********************************/   

  mem_cpy((void *)(&Measu_Status_Mode_ToPub_PUCK), &Pri_MeasuStatVar,sizeof(Pri_MeasuStatVar),(void *)(&Measu_Status_Mode_ToPub_PUCK),sizeof(Measu_Status_Mode_ToPub_PUCK));
  
  
  //提交应用层：A/B/C/总四象限指示；
  mem_cpy((void *)(&Measu_InstantData_ToPub_PUCK.Quadrant),\
          (void *)(&Pri_TempIntantVar.Quadrant),\
          sizeof(MEASUFLAG),
          (void *)(&Measu_InstantData_ToPub_PUCK.Quadrant),\
          sizeof(MEASUFLAG));
  
  SET_STRUCT_SUM(Measu_Status_Mode_ToPub_PUCK);
  SET_STRUCT_SUM(Measu_InstantData_ToPub_PUCK);
  
  return Flag;
}
/**********************************************************************************
函数功能：将计量瞬时数据提交应用层
入口：无
出口：无
**********************************************************************************/	
void GetMeasInstantData_PerSec(void)
{
  INT8U Flag;

  Flag=ChkPowerLimitAndConvertData();
  if(Flag!=MEASU_NO_ERR)
  {   
    Pri_MeasuStatVar.DataRecFlg=0;
    SET_STRUCT_SUM(Pri_MeasuStatVar);
    RstCtrlStatus.RstLimiCountr++;
    RstCtrlStatus.LimitErrCountr++;
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->In GetAppInstant(),ErrCode=%d,LimitErrCountr=%d,MeasErrCountrs=%d,Rst=%d",Flag,RstCtrlStatus.LimitErrCountr,RstCtrlStatus.MeasErrCountr,RstCtrlStatus.RstLimiCountr);
    Measure_Error_Statics(Flag,0);

    if(RstCtrlStatus.RstLimiCountr>=MAX_RST_MEASU_IC_TIMES)
    { 
      Measure_Error_Statics(Flag,1);  
      Beep_For_Measu_Alarm_PUCK();    
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->MeasuIC Error Count OverLimited,Reset Measure IC..........");
      InitMeasuAfterPwrOn();
      RstCtrlStatus.RstLimiCountr=0;
    }
  }
  else
  {
    //Pri_MeasuStatVar.DataRecFlg=1;
    SET_STRUCT_SUM(Measu_Status_Mode_ToPub_PUCK);
    RstCtrlStatus.RstLimiCountr=0;
  } 
  SET_STRUCT_SUM(RstCtrlStatus);
}
/**********************************************************************************
函数功能：将计量累计数据提交应用层，每秒累计中间件单元：有/无功总累计值、四象限无功总累计值、正/反无功总累计值等。
入口：无
出口：无
**********************************************************************************/	
void	GetMeasAccmuData_PerSec(void)
{
  INT8U  i,temp1;
  INT8U volatile *Ptr;
  INT32U volatile *Ptr1,*Ptr2,*Ptr3,*Ptr4;
  INT32U CopperLoss,IronLoss,GeneralErr,temp,temp0;
  
  
  temp1=CHECK_STRUCT_SUM(Measu_InstantData_ToPub_PUCK);
  temp1&=CHECK_STRUCT_SUM(Pri_TempMeasuVar);
  temp1&=CHECK_STRUCT_SUM(Measu_Energ_ToPub_PUCK);
  if(!temp1)
    ASSERT(A_WARNING,0); 
  
 //Clr_Energ_Accu();   //清除用于计算电能增量的寄存器,测试增加 0.006的实验。
 
  //获取A/B/C/总正反有功总增量
  Ptr=&(Measu_InstantData_ToPub_PUCK.Quadrant.A);
  Ptr1=&(Pri_TempMeasuVar.Change_AcEnerg.A);
  for(i=0;i<=3;i++)
  {
    temp=Ptr1[i];  //当前各元件有功增量  
    if(Ptr[i]>QUADRANT4)
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Quadrant OverFolw While Getting Acpower Energ!"); 
      break;
    }
    if((Ptr[i]==QUADRANT1)||(Ptr[i]==QUADRANT4))	 //累加A/B/C/总正向有功电量中间单元
    {
        Measu_Energ_ToPub_PUCK.Chang_Plus_AcEnerg[i]+=temp;
    }      
    else				      		//累加总/A/B/C反向有功电量中间单元
    {
        Measu_Energ_ToPub_PUCK.Chang_Nega_AcEnerg[i]+=temp;
    }
  }  
  
  //获取A/B/C/总四象限无功增量;获取A/B/C/总正反无功增量
  Ptr=&(Measu_InstantData_ToPub_PUCK.Quadrant.A);
  Ptr1=&(Pri_TempMeasuVar.Change_ReacEnerg.A);  
  for(i=0;i<=3;i++)
  {
    temp1=Ptr[i];  //当前象限
    temp=Ptr1[i];  //当前各元件无功增量   
    if(temp1>QUADRANT4)
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Quadrant OverFolw While Getting Reacpower Energ!"); 
      break;
    }
    Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[i][temp1-QUADRANT1]+=temp;  //各元件4象限无功：1~4，1作为偏移
    
    //Measu_Energ_ToPub_PUCK.Chang_Plus_ReacEnerg[i]+=temp*GET_BIT(MeasuSysMode.RG1_Spec,temp1-QUADRANT1);   //各元件正向无功，即组合无功1,只处理加，不处理减；
    //Measu_Energ_ToPub_PUCK.Chang_Nega_ReacEnerg[i]+=temp*GET_BIT(MeasuSysMode.RG2_Spec,temp1-QUADRANT1);   //各元件反向无功，即组合无功2,只处理加，不处理减；    
  }  
  
   //累加A/B/C相安时值中间单元，单位为0.0001安培.秒
  Ptr4=&(Measu_InstantData_ToPub_PUCK.Curr.A);
  for(i=0;i<=2;i++)
  {
    temp=Ptr4[i];
    Measu_Energ_ToPub_PUCK.Add_AmpaHour[i]+=(INT32U)((FP32S)temp/UNIT_A*UNIT_AS);      //安时:单位：0.0001An.s
    Measu_Energ_ToPub_PUCK.Add_AmpaHour[3]+=(INT32U)((FP32S)temp/UNIT_A*UNIT_AS);      //合相
  }
  //累加A/B/C相铜损、铁损和互感器合成误差中间单元
  Ptr1=&(Measu_InstantData_ToPub_PUCK.AcPower.A);
  Ptr2=&(Measu_InstantData_ToPub_PUCK.ReacPower.A);
  Ptr3=&(Measu_InstantData_ToPub_PUCK.Volt.A);
  Ptr4=&(Measu_InstantData_ToPub_PUCK.Curr.A);
  
  for(i=0;i<=2;i++)
  {
    temp=Ptr4[i];
    CopperLoss=(INT32U)(((FP32S)temp/UNIT_A)*((FP32S)temp/UNIT_A)*UNIT_KA2S);   //铜损:单位为 0.0000001kA2s
    Measu_Energ_ToPub_PUCK.Add_CopperLoss[i]+=CopperLoss;
    
    temp0=Ptr3[i];
    IronLoss=(INT32U)(((FP32S)temp0/UNIT_V)*((FP32S)temp0/UNIT_V)*UNIT_MV2S);   //铁损:单位为 0.00000001MV2s
    Measu_Energ_ToPub_PUCK.Add_IronLoss[i]+=IronLoss;
    
    if(Ptr1[i]==0)
      GeneralErr=(INT32U)((FP32S)UNIT_TGFAS*Ptr2[i]/UNIT_KWH_KVAR);            //有功功率为0，取无功功率
    else
    {
      temp0=Ptr2[i];
      GeneralErr=(INT32U)(((FP32S)UNIT_TGFAS*temp0)/Ptr1[i]);	//取=单位*无功功率/有功功率,单位为0.01*tgFA*s
    }
    if(GeneralErr>5800)		       //89度附近
      GeneralErr=UNIT_TGFAS*58;
    
    Measu_Energ_ToPub_PUCK.Add_GenaralErr[i]+=GeneralErr;
    
    Measu_Energ_ToPub_PUCK.Add_CopperLoss[3]+=CopperLoss;      //合相
    Measu_Energ_ToPub_PUCK.Add_IronLoss[3]+=IronLoss;          //合相
    Measu_Energ_ToPub_PUCK.Add_GenaralErr[3]+=GeneralErr;       //合相
  } 
  
  //将临时的有、无功增量清0
  mem_set((void *)&Pri_TempMeasuVar.Change_AcEnerg,0,\
                 sizeof(Pri_TempMeasuVar.Change_AcEnerg)*2,\
                 (void *)&Pri_TempMeasuVar.Change_AcEnerg,\
                 sizeof(Pri_TempMeasuVar.Change_AcEnerg)*2);
  SET_STRUCT_SUM(Pri_TempMeasuVar);
  SET_STRUCT_SUM(Measu_Energ_ToPub_PUCK);
}

/**********************************************************************************
函数功能：获取电能累计量,给HUCK调用
入口：
ID----只有HUCK有效！
DstReal----目标缓冲区的实际地址；
Dst_Start----目标缓冲区的最开始地址；
DstLen----目标缓冲区实际数据长度
DstMaxLen----目标缓冲区最大数据长度
出口：0：获取失败；非0：实际的数据长度，将MEASU_ENERG累加数据给Dst
注意：Dst的校验和没有处理
**********************************************************************************/	
INT16U Get_Measu_AcculateData_PUCK(INT8U ID,void *DstReal,void *Dst_Start,INT16U DstLen,INT16U DstMaxLen)
{
  INT8U i,j;
  INT32U tempmod;
  
  //只有HUCK才有权限访问
  if(ID!=HUCK)
    return 0;
  
  if(DstLen > sizeof(MEASU_ENERG))
    ASSERT(A_WARNING,0);
  
  if((INT8U *)DstReal<(INT8U *)Dst_Start || (INT8U *)DstReal+DstLen > (INT8U *)Dst_Start+DstMaxLen)
    ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(Measu_Energ_ToPub_PUCK)==0)
    ASSERT(A_WARNING,0);
  
  mem_cpy(DstReal,(void *)(&Measu_Energ_ToPub_PUCK),sizeof(MEASU_ENERG),DstReal,DstMaxLen);
  
  for(i=0;i<4;i++)   //获取分相、合相正向有功电能
  {
    tempmod=Measu_Energ_ToPub_PUCK.Chang_Plus_AcEnerg[i]%ENERGY_RATIO;  
    Measu_Energ_ToPub_PUCK.Chang_Plus_AcEnerg[i]=tempmod; 
  }
  
  for(i=0;i<4;i++)  //获取分相、合相反向有功电能
  {
    tempmod=Measu_Energ_ToPub_PUCK.Chang_Nega_AcEnerg[i]%ENERGY_RATIO;
    Measu_Energ_ToPub_PUCK.Chang_Nega_AcEnerg[i]=tempmod;    
  }
  
  /*
  for(i=0;i<4;i++)
  {
    tempmod=Measu_Energ_ToPub_PUCK.Chang_Plus_ReacEnerg[i]%ENERGY_RATIO;
    Measu_Energ_ToPub_PUCK.Chang_Plus_ReacEnerg[i]=tempmod;    
  }
  
  for(i=0;i<4;i++)
  {
    tempmod=Measu_Energ_ToPub_PUCK.Chang_Nega_ReacEnerg[i]%ENERGY_RATIO;
    Measu_Energ_ToPub_PUCK.Chang_Nega_ReacEnerg[i]=tempmod;    
  }
  */
 
  for(i=0;i<4;i++)   //获取分相、合相4象限无功电能
  {
    for(j=0;j<4;j++)
    {
      tempmod=Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[i][j]%ENERGY_RATIO;
      Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[i][j]=tempmod;
    }
  }
  
  for(i=0;i<4;i++)   //获取安时累计
  {
    tempmod=Measu_Energ_ToPub_PUCK.Add_AmpaHour[i]%AH_RATIO;
    Measu_Energ_ToPub_PUCK.Add_AmpaHour[i]=tempmod;    
  }

  for(i=0;i<4;i++)  //获取铜损累计
  {
    tempmod=Measu_Energ_ToPub_PUCK.Add_CopperLoss[i]%COPPERLOSS_RATIO;
    Measu_Energ_ToPub_PUCK.Add_CopperLoss[i]=tempmod;    
  }
  
  for(i=0;i<4;i++)   //获取铁损累计
  {
    tempmod=Measu_Energ_ToPub_PUCK.Add_IronLoss[i]%IRONLOSS_RATIO;
    Measu_Energ_ToPub_PUCK.Add_IronLoss[i]=tempmod;    
  }
  
    
  for(i=0;i<4;i++)    //获取合成误差累计
  {
    tempmod=Measu_Energ_ToPub_PUCK.Add_GenaralErr[i]%TRANSERR_RATIO;
    Measu_Energ_ToPub_PUCK.Add_GenaralErr[i]=tempmod;    
  }
 SET_STRUCT_SUM(Measu_Energ_ToPub_PUCK);
 return sizeof(MEASU_ENERG);  
}

/**********************************************************************************
函数功能：每秒对MeasuIC处理函数。
入口：无
出口：无
**********************************************************************************/	
void Deal_PerSec_Main(void)
{
  INT8U Flag;
  
  Flag=GetMeasuData_PerSec(); 
  if(Flag!=MEASU_NO_ERR)
  {
    Pri_MeasuStatVar.DataRecFlg=0;
    SET_STRUCT_SUM(Pri_MeasuStatVar);    
    RstCtrlStatus.RstMeasCountr++;
    RstCtrlStatus.MeasErrCountr++;
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->In Deal_PerSec_Main(),ErrCode=%d,LimitErrCountr=%d,MeasErrCountrs=%d,Rst=%d",Flag,RstCtrlStatus.LimitErrCountr,RstCtrlStatus.MeasErrCountr,RstCtrlStatus.RstMeasCountr);
    Measure_Error_Statics(Flag,0);
    
    if(RstCtrlStatus.RstMeasCountr>=MAX_RST_MEASU_IC_TIMES)
    { 
      Measure_Error_Statics(Flag,1);
      Beep_For_Measu_Alarm_PUCK();
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->MeasuIC Error Count OverLimited,Reset Measure IC..........");
      InitMeasuAfterPwrOn(); 
      RstCtrlStatus.RstMeasCountr=0;      
    }
  }
  else
  {
    Pri_MeasuStatVar.DataRecFlg=1;
    SET_STRUCT_SUM(Pri_MeasuStatVar);
    RstCtrlStatus.RstMeasCountr=0;
  }
  SET_STRUCT_SUM(RstCtrlStatus);
}

/**********************************************************************************
函数功能：每秒对中间件元素处理
入口：无
出口：无
注意：此函数是对计量数据的发布，在数据发布时，不允许让出当前任务，如睡眠等。
**********************************************************************************/	
void Deal_MeasAppData_PerSec_Main(void)
{
  GetMeasInstantData_PerSec();        //获取瞬时量数据和状态量
  GetMeasAccmuData_PerSec();        //获取累加量数据
 
  //Check_Voltage_Threshold();	/*"只有在三相中的最高电压大于75％Un时才点亮背光"*/
}

/**********************************************************************************
函数功能：纠错计量相关的所有Sram数据(只对备份的Sram参数有效)
入口：无
出口：无
**********************************************************************************/	
void ReCorrect_Instant_Sram(void)
{
#ifdef MEASURE_VAR_BAK_EN
  INT8U Flag;
    

  Flag=CHECK_STRUCT_SUM(Pri_TempMeasuVarBak1);
  if(Flag==0)  //Pri_TempMeasuVarBak1有错误发生
  {
    Flag=CHECK_STRUCT_SUM(Pri_TempMeasuVarBak2);
    if(Flag==0)  //Pri_TempMeasuVarBak2有错误发生
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Struct CS Error Pri_TempMeasuVar&&Bak1&&Bak2!");
    }
    else                          //Pri_TempMeasuVarBak2正确,覆盖其他2个
    {
      mem_cpy((void *)(&(Pri_TempMeasuVar)),(void *)(&(Pri_TempMeasuVarBak2)),sizeof(Pri_TempMeasuVarBak2),\
                   (void *)(&(Pri_TempMeasuVar)),sizeof(Pri_TempMeasuVar));
             
      mem_cpy((void *)(&(Pri_TempMeasuVarBak1)),(void *)(&(Pri_TempMeasuVarBak2)),sizeof(Pri_TempMeasuVarBak2),\
                   (void *)(&(Pri_TempMeasuVarBak1)),sizeof(Pri_TempMeasuVarBak1));  
      }          
  }
  else                         //Pri_TempMeasuVarBak1正确,覆盖其他2个
  {
    mem_cpy((void *)(&(Pri_TempMeasuVar)),(void *)(&(Pri_TempMeasuVarBak1)),sizeof(Pri_TempMeasuVarBak1),\
                   (void *)(&(Pri_TempMeasuVar)),sizeof(Pri_TempMeasuVar));
             
    mem_cpy((void *)(&(Pri_TempMeasuVarBak2)),(void *)(&(Pri_TempMeasuVarBak1)),sizeof(Pri_TempMeasuVarBak1),\
                   (void *)(&(Pri_TempMeasuVarBak2)),sizeof(Pri_TempMeasuVarBak2));  
  } 
#endif
}
/**********************************************************************************
函数功能：纠错计量相关的所有Sram数据(只对备份的Sram参数有效)
入口：无
出口：无
**********************************************************************************/	
void ReCorrect_Energ_Sram(void)
{
#ifdef MEASURE_VAR_BAK_EN  
  INT8U Flag;
  Flag=CHECK_STRUCT_SUM(Measu_Energ_ToPub_Bak);
  if(Flag==0)  //备份也发生了错误
  {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Struct CS Error Measu_Energ_ToPub_PUCK&&Bak!");
  }
  else     //备份无错误，用备份覆盖
  {
    mem_cpy((void *)(&(Measu_Energ_ToPub_PUCK)),(void *)(&(Measu_Energ_ToPub_Bak)),sizeof(Measu_Energ_ToPub_Bak),\
                   (void *)(&(Measu_Energ_ToPub_PUCK)),sizeof(Measu_Energ_ToPub_PUCK));             
  }  
#endif
}
/**********************************************************************************
函数功能：检查计量任务中所有结构体的校验和
入口：无
出口：无
**********************************************************************************/	
void Check_ALL_Sram_Sum(void)
{
  if(CHECK_STRUCT_SUM(RstCtrlStatus)==0)
  ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(Sys_Pulse_Var)==0)
  ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(Curr_1_Per_100)==0)
  ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(Curr_6_13x5_Slipe)==0)
  ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(Curr_In_6_13x5)==0)
  ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(SpecChange_CurrFlag)==0)
  ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(MeasuStatusMode)==0)
  ASSERT(A_WARNING,0);
      
  if(CHECK_STRUCT_SUM(Pri_MeasuStatVar)==0)
  ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(Pri_TempIntantVar)==0)
  ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(Measu_Status_Mode_ToPub_PUCK)==0)
  ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(Measu_InstantData_ToPub_PUCK)==0)
  ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(MeasuPara_CS)==0)
  ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_SUM(Measu_Energ_ToPub_PUCK)==0)
  {
    ASSERT(A_WARNING,0);  //CS错误
      ReCorrect_Energ_Sram();
  }
#ifdef MEASURE_VAR_BAK_EN
  else                          //数据正确
  {
    mem_cpy((void *)(&(Measu_Energ_ToPub_Bak)),(void *)(&(Measu_Energ_ToPub_PUCK)),sizeof(Measu_Energ_ToPub_PUCK),\
                   (void *)(&(Measu_Energ_ToPub_Bak)),sizeof(Measu_Energ_ToPub_Bak));  
  }
#endif
    
  
  
  if(CHECK_STRUCT_SUM(Pri_TempMeasuVar)==0)
  {
    ASSERT(A_WARNING,0);  //CS错误
    ReCorrect_Instant_Sram();
  }
#ifdef MEASURE_VAR_BAK_EN
  else                          //数据正确      
  {
      mem_cpy((void *)(&(Pri_TempMeasuVarBak1)),(void *)(&(Pri_TempMeasuVar)),sizeof(Pri_TempMeasuVar),\
                   (void *)(&(Pri_TempMeasuVarBak1)),sizeof(Pri_TempMeasuVarBak1)); 
      mem_cpy((void *)(&(Pri_TempMeasuVarBak2)),(void *)(&(Pri_TempMeasuVar)),sizeof(Pri_TempMeasuVar),\
                   (void *)(&(Pri_TempMeasuVarBak2)),sizeof(Pri_TempMeasuVarBak2)); 
  }
#endif
}
/**********************************************************************************
函数功能：计量参数重新写
入口：3数
出口：最大
**********************************************************************************/
void Measure_IC_Cal(void)                //运行过程中，根据读出芯片的校表请求位，需要重新校表
{  
  if(CHECK_STRUCT_SUM(MeasuStatusMode))
  {
    if(1==MeasuStatusMode.CalReqstErr)   //校表请求
    { 
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Cal Request,Reload Cal Para.....");
      Load_MeasureIC_Para();
      MeasuStatusMode.CalReqstErr=0;
      SET_STRUCT_SUM(MeasuStatusMode);     
    }
  }  
}
/********************************************************************************
函数原型：void Monitor_Para_Modi_PUCK(void)
功能：对所有参数进行维护，包括计量、通道、事件等。

*********************************************************************************/
void MeasuPara_Modi_PUCK(void)
{
  INT16U temp;
  
  while(Get_Para_Modify(&temp))
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Para Modified In Measure Task:0x%x",temp);
    switch(temp)
    {
      case DI_MODE1:   //注：DI_TFH模式字改变后，必须对计量芯片复位操作一次
      case DI_P_U_I_SPEC:
        GetSysModeProc();          //更新T_FH模式字，脉冲常数规格，电压规格，电流规格
        Load_All_Para_To_IC(); //初始化与校表参数相关的参数
        Beep_For_Measu_Alarm_PUCK();
        Set_Event_Instant(ID_EVENT_ADJUST_MEASU);  //校表事件
       break;
       
      case DI_AC_PULSE:      
      case DI_REAC_PULSE:
      case DI_PULSE_WIDTH:  
        Get_Pulse_Para();   //更新有无功脉冲常数，分频系数，脉宽输出,与校表无关，不设置校表事件
       break;
    
    case  DI_CLR_PARA:     //总清
        Init_Para();                //初始化与校表参数无关的参数
        Load_All_Para_To_IC(); //初始化与校表参数相关的参数
        Set_Cal_Event();
      break;
      
    case DI_CAL_STATUS:     //校表事件发生
        Set_Cal_Event();
      break;
      
    default:
      if((temp>=DI_POWER_A_0)&&(temp<=DI_I_GAIN_C))  //校表参数发生修改
      {
        Load_Adj_OnePara_To_IC(temp);
        Set_Event_Instant(ID_EVENT_ADJUST_MEASU);  //校表事件
      }
      break;
    }    
  }   
}
/**********************************************************************************
函数功能：打印输出计量数据,和同步调用
入口：无
出口：无
**********************************************************************************/	
void DebugOutMeasuData(void)
{
  static INT8U Num=0;
  Num++;
  if(Num<240)
    return ;
  
  Num=0;
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|------------------------Measure_Instant_Data------------------------|");
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|               Item           A         B         C     Total       |");
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|    AcPower(0.001w)   %9ld,%9ld,%9ld,%9ld       |",Measu_InstantData_ToPub_PUCK.AcPower.A,Measu_InstantData_ToPub_PUCK.AcPower.B,Measu_InstantData_ToPub_PUCK.AcPower.C,Measu_InstantData_ToPub_PUCK.AcPower.Sum);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|ReacPower(0.001var)   %9ld,%9ld,%9ld,%9ld       |",Measu_InstantData_ToPub_PUCK.ReacPower.A,Measu_InstantData_ToPub_PUCK.ReacPower.B,Measu_InstantData_ToPub_PUCK.ReacPower.C,Measu_InstantData_ToPub_PUCK.ReacPower.Sum);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|  AppPower(0.001vA)   %9ld,%9ld,%9ld,%9ld       |",Measu_InstantData_ToPub_PUCK.AppPower.A,Measu_InstantData_ToPub_PUCK.AppPower.B,Measu_InstantData_ToPub_PUCK.AppPower.C,Measu_InstantData_ToPub_PUCK.AppPower.Sum);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|       Volt(0.0001)   %9ld,%9ld,%9ld,%9ld       |",Measu_InstantData_ToPub_PUCK.Volt.A,Measu_InstantData_ToPub_PUCK.Volt.B,Measu_InstantData_ToPub_PUCK.Volt.C,Measu_InstantData_ToPub_PUCK.Volt.Sum);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|       Curr(0.0001)   %9ld,%9ld,%9ld,%9ld       |",Measu_InstantData_ToPub_PUCK.Curr.A,Measu_InstantData_ToPub_PUCK.Curr.B,Measu_InstantData_ToPub_PUCK.Curr.C,Measu_InstantData_ToPub_PUCK.Curr.Sum);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|  PFactor(0.000001)   %9ld,%9ld,%9ld,%9ld       |",Measu_InstantData_ToPub_PUCK.PFactor.A,Measu_InstantData_ToPub_PUCK.PFactor.B,Measu_InstantData_ToPub_PUCK.PFactor.C,Measu_InstantData_ToPub_PUCK.PFactor.Sum);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|        Angle(0.01)   %9ld,%9ld,%9ld,%9ld       |",Measu_InstantData_ToPub_PUCK.Angle.A,Measu_InstantData_ToPub_PUCK.Angle.B,Measu_InstantData_ToPub_PUCK.Angle.C,Measu_InstantData_ToPub_PUCK.Angle.Sum);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|     Freq(0.000001)   %9ld,%9ld,%9ld                 |",Measu_InstantData_ToPub_PUCK.Freq.A,Measu_InstantData_ToPub_PUCK.Freq.B,Measu_InstantData_ToPub_PUCK.Freq.C,Measu_InstantData_ToPub_PUCK.Freq.Sum);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|           Quadrant   %9d,%9d,%9d,%9d       |",Measu_InstantData_ToPub_PUCK.Quadrant.A,Measu_InstantData_ToPub_PUCK.Quadrant.B,Measu_InstantData_ToPub_PUCK.Quadrant.C,Measu_InstantData_ToPub_PUCK.Quadrant.Sum);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|--------------------------------------------------------------------|"); 
  
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|--------------------------------Measure_Para------------------------|");
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"| ParseMode=%d(PARSE_331/PARSE_341)                                   |",MeasuSysMode.ParseMode);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"| P_Spec=%d(PULSE_TYPE1/PULSE_TYPE2)                                  |",MeasuSysMode.P_Spec);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"| V_Spec=%d(3*100/3*220/3*57.70/3*380)                                |",MeasuSysMode.V_Spec);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"| I_Spec=%d(1.5_6/1_2/5_10/10_40/0.3_1.2/5_6/15_60/20_80/5_20/30_100  |",MeasuSysMode.I_Spec);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"| TF/TH=%d(TH/TF)                                                     |",MeasuSysMode.T_FH);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"| AcPulseConst=%8ld, ReacPulseConst=%8ld,                    |",Sys_Pulse_Var.Ac_Pulse,Sys_Pulse_Var.Rea_Pulse);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|--------------------------------------------------------------------|"); 

  
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|-------------------------Measure_Status_Data------------------------|");
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|               Item           A         B         C     Total       |");
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|      AcEnerg_Value   %9ld,%9ld,%9ld,%9ld       |",Pri_TempMeasuVar.AcEnerg.A,Pri_TempMeasuVar.AcEnerg.B,Pri_TempMeasuVar.AcEnerg.C,Pri_TempMeasuVar.AcEnerg.Sum);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|    ReacEnerg_Value   %9ld,%9ld,%9ld,%9ld       |",Pri_TempMeasuVar.ReacEnerg.A,Pri_TempMeasuVar.ReacEnerg.B,Pri_TempMeasuVar.ReacEnerg.C,Pri_TempMeasuVar.ReacEnerg.Sum);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|--------------------------------------------------------------------|"); 
  
  Print_Measure_Err_Info();
  /*
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"AcPower(0.001w):%ld,%ld,%ld,%ld",Measu_InstantData_ToPub_PUCK.AcPower.A,Measu_InstantData_ToPub_PUCK.AcPower.B,Measu_InstantData_ToPub_PUCK.AcPower.C,Measu_InstantData_ToPub_PUCK.AcPower.Sum);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"ReacPower(0.001var):%ld,%ld,%ld,%ld",Measu_InstantData_ToPub_PUCK.ReacPower.A,Measu_InstantData_ToPub_PUCK.ReacPower.B,Measu_InstantData_ToPub_PUCK.ReacPower.C,Measu_InstantData_ToPub_PUCK.ReacPower.Sum);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"AppPower(0.001vA):%ld,%ld,%ld,%ld",Measu_InstantData_ToPub_PUCK.AppPower.A,Measu_InstantData_ToPub_PUCK.AppPower.B,Measu_InstantData_ToPub_PUCK.AppPower.C,Measu_InstantData_ToPub_PUCK.AppPower.Sum);
  
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Volt(0.0001):%ld,%ld,%ld",Measu_InstantData_ToPub_PUCK.Volt.A,Measu_InstantData_ToPub_PUCK.Volt.B,Measu_InstantData_ToPub_PUCK.Volt.C);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Curr(0.0001):%ld,%ld,%ld",Measu_InstantData_ToPub_PUCK.Curr.A,Measu_InstantData_ToPub_PUCK.Curr.B,Measu_InstantData_ToPub_PUCK.Curr.C);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"PFactor(0.000001):%ld,%ld,%ld,%ld",Measu_InstantData_ToPub_PUCK.PFactor.A,Measu_InstantData_ToPub_PUCK.PFactor.B,Measu_InstantData_ToPub_PUCK.PFactor.C,Measu_InstantData_ToPub_PUCK.PFactor.Sum);
  
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Angle(0.01):%ld,%ld,%ld",Measu_InstantData_ToPub_PUCK.Angle.A,Measu_InstantData_ToPub_PUCK.Angle.B,Measu_InstantData_ToPub_PUCK.Angle.C);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Freq(0.000001):%ld,%ld,%ld",Measu_InstantData_ToPub_PUCK.Freq.A,Measu_InstantData_ToPub_PUCK.Freq.B,Measu_InstantData_ToPub_PUCK.Freq.C,Measu_InstantData_ToPub_PUCK.Freq.Sum);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Quadrant:%d,%d,%d,%d",Measu_InstantData_ToPub_PUCK.Quadrant.A,Measu_InstantData_ToPub_PUCK.Quadrant.B,Measu_InstantData_ToPub_PUCK.Quadrant.C,Measu_InstantData_ToPub_PUCK.Quadrant.Sum);

  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Chang_Plus_AcEnerg(0.001Wh):%ld,%ld,%ld,%ld",Measu_Energ_ToPub_PUCK.Chang_Plus_AcEnerg[0],Measu_Energ_ToPub_PUCK.Chang_Plus_AcEnerg[1],Measu_Energ_ToPub_PUCK.Chang_Plus_AcEnerg[2],Measu_Energ_ToPub_PUCK.Chang_Plus_AcEnerg[3]);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Chang_Nega_AcEnerg(0.001Wh):%ld,%ld,%ld,%ld",Measu_Energ_ToPub_PUCK.Chang_Nega_AcEnerg[0],Measu_Energ_ToPub_PUCK.Chang_Nega_AcEnerg[1],Measu_Energ_ToPub_PUCK.Chang_Nega_AcEnerg[2],Measu_Energ_ToPub_PUCK.Chang_Nega_AcEnerg[3]);
  
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Chang_Quad_ReacEnerg_1(0.001Wh):%ld,%ld,%ld,%ld",Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[0][0],Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[1][0],Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[2][0],Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[3][0]);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Chang_Quad_ReacEnerg_2(0.001Wh):%ld,%ld,%ld,%ld",Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[0][1],Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[1][1],Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[2][1],Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[3][1]);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Chang_Quad_ReacEnerg_3(0.001Wh):%ld,%ld,%ld,%ld",Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[0][2],Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[1][2],Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[2][2],Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[3][2]);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Chang_Quad_ReacEnerg_4(0.001Wh):%ld,%ld,%ld,%ld",Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[0][3],Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[1][3],Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[2][3],Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[3][3]);
 */
}
/**********************************************************************************
函数功能：打印计量出错统计信息
入口：无
出口：无
**********************************************************************************/	
void Print_Measure_Err_Info(void)
{
#ifdef  MEASURE_ERROR_STATICS_EN
  
  INT16U i;
  
  if(CHECK_STRUCT_SUM(Measure_Err_Info)==0)
  {
    mem_set(&Measure_Err_Info,0x00,sizeof(Measure_Err_Info),&Measure_Err_Info,sizeof(Measure_Err_Info));
    SET_STRUCT_SUM(Measure_Err_Info);
    return ;
  }
  

  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|---------------------Measure_Error_Statis_Info----------------------|");
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|          Error_Code              Error_Num        ResetNum         |");
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|                 %3d                                    %3d         |",Measure_Err_Info.AcFlag,Measure_Err_Info.ResetNum);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|           %9ld(Last)       %9ld(Curr)  %9ld(Change) |",Measure_Err_Info.LastValue,Measure_Err_Info.CurrValue,Measure_Err_Info.ChangeValue);
  
  for(i=MEASU_NO_ERR+1;i<=MAX_MEASU_ERR;i++)
  {
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|                 %3d                   %3d                          |",i,Measure_Err_Info.Num[i]);
  } 
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|--------------------------------------------------------------------|"); 
  
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|-------------------------Measure_Status_Data------------------------|");
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|               Item           A         B         C     Total       |");
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|      AcEnerg_Value   %9ld,%9ld,%9ld,%9ld       |",Pri_TempMeasuVar.AcEnerg.A,Pri_TempMeasuVar.AcEnerg.B,Pri_TempMeasuVar.AcEnerg.C,Pri_TempMeasuVar.AcEnerg.Sum);
  OS_TimeDly_Ms(10);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|    ReacEnerg_Value   %9ld,%9ld,%9ld,%9ld       |",Pri_TempMeasuVar.ReacEnerg.A,Pri_TempMeasuVar.ReacEnerg.B,Pri_TempMeasuVar.ReacEnerg.C,Pri_TempMeasuVar.ReacEnerg.Sum);

  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|--------------------------------------------------------------------|"); 
  
#endif
}
/**********************************************************************************
函数功能：计量错误统计
入口：无
出口：无
**********************************************************************************/	
void Measure_Error_Statics(INT8U Code,INT8U RstFlag)
{
#ifdef  MEASURE_ERROR_STATICS_EN
  
  if(CHECK_STRUCT_SUM(Measure_Err_Info)==0)
  {
    mem_set(&Measure_Err_Info,0x00,sizeof(Measure_Err_Info),&Measure_Err_Info,sizeof(Measure_Err_Info)); 
    SET_STRUCT_SUM(Measure_Err_Info);
  }
  
  if(MEASU_NO_ERR==Code || Code>MAX_MEASU_ERR)  //无错误,或者错误代码超限
    return ;
  
  if(RstFlag)
  {
    Measure_Err_Info.ResetNum++;
    Set_Event_Instant(ID_MEASURE_ERROR);  //对计量芯片要复位，算作一次计量错误！
  }
  
  Measure_Err_Info.Num[Code]++;

  SET_STRUCT_SUM(Measure_Err_Info);
  Print_Measure_Err_Info();
#endif
}
/**********************************************************************************
函数功能：检查计量芯片和epprom的校验和是否正确
入口：无
出口：无
**********************************************************************************/	
INT8U CheckBothCs(void)
{
  INT32U CS_IC;
  INT8U Flag;
  
  if(Get_Cal_Exsit())   //发生较表事件时，不部判定2者的检验和
    return MEASU_NO_ERR;
  
  Flag=Get_IC_ParaSum3(&CS_IC);
  if(!Flag)      //取校验和错误，再取一次
  {
    Flag=Get_IC_ParaSum3(&CS_IC);
    if(!Flag)     //取校验和错误，再取一次
    {
      ASSERT(A_WARNING,0);  
      MeasuStatusMode.Retry=1;   //计量芯片参数写错误重试
      SET_STRUCT_SUM(MeasuStatusMode);
      return MEASU_CS_ERR;
    }
  }
  
  if(CS_IC!=MeasuPara_CS.IC_Cs|| CS_IC!=MeasuPara_CS.EppRom_Cs)
  {
    MeasuStatusMode.Retry=1;   //计量芯片参数写错误重试
    SET_STRUCT_SUM(MeasuStatusMode);
    ASSERT(A_WARNING,0); 
    return MEASU_CS_ERR;    
  }  
  return   MEASU_NO_ERR;
}
/**********************************************************************************
函数功能：计量处理主函数
入口：Flag-------1:初始化；0---------------主程序
出口：无
**********************************************************************************/	
void Init_Measu_PUCK(void)
{
  Init_Para();           //计量相关参数初始化
  if(Get_Sys_Status()!=SYS_NORMAL) //只有在正常模式下，才初始化计量任务
    return ;        
   
  Init_Measure_IC(); 
  Deal_PerSec_Main();
  //GetVoltCurrValue_PUCK();
  GetMeasInstantData_PerSec();   //获取瞬时量数据和状态量
}

/**********************************************************************************
函数功能：计量处理主函数
入口：Flag-------1:初始化；0---------------主程序
出口：无
**********************************************************************************/	
void Measu_Main_Puck(INT8U Flag)
{
  static INT32U Sec_Bak=0xFFFFFFFF;  
  
  if(Get_Sys_Status()!=SYS_NORMAL)  //只有在主电源供电情况下，此任务才执行
  {
    while(1)
     Task_Sleep();   
  }
  if(Get_Sys_Status()!=SYS_NORMAL) //只有在正常模式下，才有事件的处理
    return ;
        
  if(Flag && 0==Pri_MeasuStatVar.MeasuIntOk)  //一旦进入，说明任务唤醒或者正常模式
  {
   Init_Para();           //计量相关参数初始化
   Init_Measure_IC();
   return ; 
  }
  else
  {
    if(Sec_Bak==Sec_Timer_Pub)//时间过了一秒
      return ;
    
    //Read_Ext_RTC_Status();          //测试时钟的稳定性
    Check_ALL_Sram_Sum();             //检查计量私/公有校验和
    Deal_PerSec_Main();                //向计量芯片取数据
    //Modi_StartCurr_TinyCurr();
    Deal_MeasAppData_PerSec_Main();   //将计量数据转换并发布,期间数据更新，不允许让出调度
    
    /*Measure_IC_Cal();                //运行过程中，根据读出芯片的校表请求位，需要重新写狡辩参数
    Retry_WritePara_To_IC();         //运行过程中，IC的CS和EPPROM的CS不一致，或者某次写计量参数错误，需要重新写狡辩参数
    */
    
    MeasuPara_Modi_PUCK();
    DebugOutMeasuData();
    Sec_Bak=Sec_Timer_Pub;
  }
}
