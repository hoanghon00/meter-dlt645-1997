#define EVENTINSTANT_C
#include "Pub_PUCK.h"
/*
typedef struct
{
  INT32U Qual_Up_Vol[3];//越合格上限电压值,0-2分别表示A-B-C相
  INT32U Qual_Low_Vol[3];//越合格下限电压值
  
  INT32U Over_Vol[3];//电压超上限电压值
}S_Event_Vol_Cur;
黄智颖 17:25:37
EXT S_Event_Vol_Cur Event_Vol_Cur;
黄智颖 17:26:00
Over_Vol[3] 是你的
*/

/***********************************************************************
函数功能：初始化获取瞬态字的全部参数
入口：无
出口：无
***********************************************************************/
void Init_Event_Instant_Para(void)
{
  INT8U Flag,temp[4];
  Flag=Read_Storage_Data_PUCK(DI_LOSS_VOLT,temp,4); 
  if(Flag)
  {
    EventInsParaVar.LossVolt_I=Bcd2Hex(temp+1,3)/(FP32S)10000;   //读的是整数1000，4位小数
    EventInsParaVar.LossVolt_Per=Bcd2Hex(temp,1)/(FP32S)100;   //读的是整数，如78
  }
  
  Flag&=Read_Storage_Data_PUCK(DI_LOSS_CURR,temp,4);
  if(Flag)
  {
    EventInsParaVar.LossCurr_I=Bcd2Hex(temp+1,3)/(FP32S)10000;
    EventInsParaVar.LossCurr_Per=Bcd2Hex(temp,1)/(FP32S)100;
    
   /* if(EventInsParaVar.LossCurr_I<=ALL_LOSS_CURR_VALUE)    //全失流阀值必须比失流电流阀值小
      EventInsParaVar.LossCurr_I=ALL_LOSS_CURR_VALUE*5;
    */
  }  
  if(!Flag)
  {
      DEBUG_PRINT(PUCK,PRINT_PUCK_EVENT_EN,"Get Instant Para Error!");  
  }

#ifdef LOSS_VOLT_ONLY           //失压只判定电压 
    EventInsParaVar.LossVolt_I=0;      //失压不判断电流
    EventInsParaVar.LossVolt_Per=0.5;   //失压电压阀值50%Un
#endif

#ifdef LOSS_CURR_ONLY           //失流只判定电流
    EventInsParaVar.LossCurr_I=0.3;       //失流参数不判不平衡率
    EventInsParaVar.LossCurr_Per=0.1;     //失流参数无效，只判定 0.001In作为阀值
#endif
  SET_STRUCT_SUM(EventInsParaVar);
}


/***********************************************************************
函数功能：对变位事件的判定，如失压、失流、功率反向
入口：无
出口：无
***********************************************************************/
void Get_Change_Status(INT8U CurrStat,INT8U StartBit,INT8U EventId,volatile EVENT_CHANGE *Result)
{
 
  if(GET_BIT(CurrStat,StartBit))        					//A相
    SET_BIT(Result->NewStatus,StartBit);
  else
    CLR_BIT(Result->NewStatus,StartBit);
  
  if(GET_BIT(CurrStat,StartBit+1))        					//B相
    SET_BIT(Result->NewStatus,StartBit+1);
  else
    CLR_BIT(Result->NewStatus,StartBit+1);
    
  if(GET_BIT(CurrStat,StartBit+2))        					//C相
    SET_BIT(Result->NewStatus,StartBit+2);
  else
    CLR_BIT(Result->NewStatus,StartBit+2);
  
  
  if(Result->OldStatus==Result->NewStatus)  //无变位，不做处理
  {
    Result->Counts=0;
    return ;
  }
  
  //发生变位
  if(0==Result->OldStatus)   //发生
  {
    Set_Event_Instant(EventId);
    Result->Counts++;
    if(MAX_CHANGE_SEC)  //确定已经存储等判定完毕
    {
      Result->OldStatus=Result->NewStatus;
      Result->Counts=0;
    }    
  }
  else if(0!=Result->OldStatus && 0==Result->NewStatus)  //结束
  {    
    Clr_Event_Instant(EventId);
    Result->Counts++;
    if(MAX_CHANGE_SEC)   //确定已经存储等判定完毕
    {
      Result->OldStatus=Result->NewStatus;
      Result->Counts=0;
    }
  }
  else if(0!=Result->OldStatus && 0!=Result->NewStatus)  //都不为0，但不相等，事件按照 结束/发生 处理
  {
    Clr_Event_Instant(EventId);
    Result->Counts++;
    if(MAX_CHANGE_SEC)   //确定已经存储等判定完毕
    {
      Result->OldStatus=0;         //本次结束，将其清0，但还需处理发生，作为下次事件的开始
      Result->Counts=0;
    }     
  }  
}
/********************************************************************************
函数原型：void Event_Para_Modi(void)
功能：对所有参数进行维护，包括计量、通道、事件等。

*********************************************************************************/
void Event_Para_Modi(void)
{
  INT16U temp;
  
  while(Get_Para_Modify(&temp))
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_CHANNEL_EN,"Para Modified In Event Task:%x",temp);
    switch(temp)
    {
      case DI_LOSS_VOLT:      
      case DI_LOSS_CURR:
          Init_Event_Instant_Para();
       break;  
    }    
  }   
}
/***********************************************************************
函数功能：电网事件发生前提检查
入口：无
出口：0-----------所有事件需要近一步检查电网事件；1-------------所有事件可视为结束
三相电压均低于70%Un；
或者
***********************************************************************/
INT8U Check_Power_Grid_Event(void)
{
  FP32S temp;
  INT8U Flag1,Flag2;
  
  if(Get_Sys_Status()!=SYS_NORMAL)
    return 1;
  
  Flag1=Flag2=0;
  temp=Get_Max_Num(Measu_InstantData_ToPub_PUCK.Volt.A,Measu_InstantData_ToPub_PUCK.Volt.B,Measu_InstantData_ToPub_PUCK.Volt.C);
  temp/=UNIT_V;
  if(temp<Get_Un()*(FP32S)0.7)        //所有电压都低于0.7Un, 所有电压都低于AUXIL_POWER_RATIO*Un时，被认为是辅助电源
    Flag1=1;
  
  temp=Measu_InstantData_ToPub_PUCK.Volt.A+Measu_InstantData_ToPub_PUCK.Volt.B+Measu_InstantData_ToPub_PUCK.Volt.C;
  temp/=UNIT_V;
  if((temp-10)<Get_Un()*(FP32S)0.7)   //(所有电压之和-10)<0.7Un   
    Flag2=1;
  
  if(Flag1&&Flag2)      //所有事件结束
    return 1;
  
  return 0;
}

/***********************************************************************
函数功能：331判B相失压处理
入口：无
出口：
***********************************************************************/
INT8U B_331_LossVolt(void)
{
  INT8U Result=0;
  FP32S temp;
  
  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.A/UNIT_V<=10 || \
     (FP32S)Measu_InstantData_ToPub_PUCK.Volt.B/UNIT_V<=10)
    Result=0;
       
  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.A/UNIT_V!=0 && \
     (FP32S)Measu_InstantData_ToPub_PUCK.Volt.B/UNIT_V!=0 &&(FP32S)Measu_InstantData_ToPub_PUCK.Volt.C/UNIT_V!=0)
  {
    Result=0;  
    //(Ua＋Uc)/Ub
    temp=((FP32S)Measu_InstantData_ToPub_PUCK.Volt.A/UNIT_V+(FP32S)Measu_InstantData_ToPub_PUCK.Volt.C/UNIT_V)/(FP32S)Measu_InstantData_ToPub_PUCK.Volt.B/UNIT_V;
    if(temp>=0.9 && temp<=1.1)
    {
      temp=(FP32S)Measu_InstantData_ToPub_PUCK.Volt.A/(FP32S)Measu_InstantData_ToPub_PUCK.Volt.B; //Ua/Ub
      if(temp>0.1)
      {
        temp=(FP32S)Measu_InstantData_ToPub_PUCK.Volt.C/(FP32S)Measu_InstantData_ToPub_PUCK.Volt.B; //Uc/Ub
        if(temp>0.1)
          Result=1;        
      }      
    }
  }
  
  if( (Result||(FP32S)Measu_InstantData_ToPub_PUCK.Angle.B/UNIT_DEGREE>=74) && \
      ((FP32S)Measu_InstantData_ToPub_PUCK.Curr.A/UNIT_A >EventInsParaVar.LossVolt_I ||
       (FP32S)Measu_InstantData_ToPub_PUCK.Curr.C/UNIT_A >EventInsParaVar.LossVolt_I) )
    return 1;
  
  return 0;
  
  
}
/***********************************************************************
函数功能：获取失压瞬时状态字
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_A_LOSS_VOLT
void Check_Loss_Volt(void)
{
  INT8U Flag;
  FP32S temp;
  
  if(Check_Power_Grid_Event())
  {
    Clr_Event_Instant(ID_EVENT_A_LOSS_VOLT);  //A电压失压清除
    Clr_Event_Instant(ID_EVENT_B_LOSS_VOLT);  //B电压失压清除
    Clr_Event_Instant(ID_EVENT_C_LOSS_VOLT);  //C电压失压清除
    Clr_Event_Instant(ID_EVENT_AB_LOSS_VOLT);  //AB电压失压清除
    Clr_Event_Instant(ID_EVENT_BC_LOSS_VOLT);  //BC电压失压清除
    Clr_Event_Instant(ID_EVENT_AC_LOSS_VOLT);  //AC电压失压清除
    Clr_Event_Instant(ID_EVENT_ABC_LOSS_VOLT);  //ABC电压失压清除
    
   return ;
  }
    
  temp=Get_Un();
  if((((FP32S)Measu_InstantData_ToPub_PUCK.Volt.A/UNIT_V)<=(temp*EventInsParaVar.LossVolt_Per))&&(((FP32S)Measu_InstantData_ToPub_PUCK.Curr.A/UNIT_A)>=EventInsParaVar.LossVolt_I))
  {
    Set_Event_Instant(ID_EVENT_A_LOSS_VOLT);  //A相失压
  }
  else
  {
    Clr_Event_Instant(ID_EVENT_A_LOSS_VOLT);  //A相没有失压
  }
  
  if((((FP32S)Measu_InstantData_ToPub_PUCK.Volt.B/UNIT_V)<=(temp*EventInsParaVar.LossVolt_Per))&&(((FP32S)Measu_InstantData_ToPub_PUCK.Curr.B/UNIT_A)>=EventInsParaVar.LossVolt_I))
  {
    Set_Event_Instant(ID_EVENT_B_LOSS_VOLT);  //B相失压
  }
  else
  {
    Clr_Event_Instant(ID_EVENT_B_LOSS_VOLT);  //B相没有失压
  }
  
  if((((FP32S)Measu_InstantData_ToPub_PUCK.Volt.C/UNIT_V)<=(temp*EventInsParaVar.LossVolt_Per))&&(((FP32S)Measu_InstantData_ToPub_PUCK.Curr.C/UNIT_A)>=EventInsParaVar.LossVolt_I))
  {
    Set_Event_Instant(ID_EVENT_C_LOSS_VOLT);  //C相失压
  }
  else
  {
     Clr_Event_Instant(ID_EVENT_C_LOSS_VOLT);  //C相没有失压
  }

  Flag=Get_SysParse_Mode();
  if(Flag==PARSE_331)  
  {
    if(B_331_LossVolt())
    {
      Set_Event_Instant(ID_EVENT_B_LOSS_VOLT);  //B相失压
      Clr_Event_Instant(ID_EVENT_A_LOSS_VOLT);  //A相不判断失压
      Clr_Event_Instant(ID_EVENT_C_LOSS_VOLT);  //C相不判断失压        
    }
    else
    {
      Clr_Event_Instant(ID_EVENT_B_LOSS_VOLT);  //B相没有失压      
    }
#ifdef MULTI_LOSS_VOLT_EN   //判定合相失压
    if(Get_Event_Instant(ID_EVENT_A_LOSS_VOLT)&&(Get_Event_Instant(ID_EVENT_C_LOSS_VOLT)))   //A和C失压
    {
      Set_Event_Instant(ID_EVENT_AC_LOSS_VOLT);  //AC相失压
      Clr_Event_Instant(ID_EVENT_A_LOSS_VOLT);  //A相没有失压
      Clr_Event_Instant(ID_EVENT_C_LOSS_VOLT);  //C相没有失压
    }
    else
    {
      Clr_Event_Instant(ID_EVENT_AC_LOSS_VOLT);  //AC相失压
    }
    //331不判 AB/BC/ABC失压
    Clr_Event_Instant(ID_EVENT_AB_LOSS_VOLT);  //AB相不判失压
    Clr_Event_Instant(ID_EVENT_BC_LOSS_VOLT);  //BC相不判失压
    Clr_Event_Instant(ID_EVENT_ABC_LOSS_VOLT);  //ABC相失压 
#endif
  }

  else                          //341                               
  {
#ifdef MULTI_LOSS_VOLT_EN    //判定合相失压
    if(Get_Event_Instant(ID_EVENT_A_LOSS_VOLT)&&(Get_Event_Instant(ID_EVENT_B_LOSS_VOLT)))
      Set_Event_Instant(ID_EVENT_AB_LOSS_VOLT);  //AB相失压
    else
      Clr_Event_Instant(ID_EVENT_AB_LOSS_VOLT);  //AB相没失压
    
    if(Get_Event_Instant(ID_EVENT_A_LOSS_VOLT)&&(Get_Event_Instant(ID_EVENT_C_LOSS_VOLT)))
      Set_Event_Instant(ID_EVENT_AC_LOSS_VOLT);  //AC相失压
    else
      Clr_Event_Instant(ID_EVENT_AC_LOSS_VOLT);  //AC相没失压
    
    if(Get_Event_Instant(ID_EVENT_B_LOSS_VOLT)&&(Get_Event_Instant(ID_EVENT_C_LOSS_VOLT)))
      Set_Event_Instant(ID_EVENT_BC_LOSS_VOLT);  //BC相失压
    else
      Clr_Event_Instant(ID_EVENT_BC_LOSS_VOLT);  //BC相没失压      
    
    if(Get_Event_Instant(ID_EVENT_AB_LOSS_VOLT)&&(Get_Event_Instant(ID_EVENT_AC_LOSS_VOLT)))
      Set_Event_Instant(ID_EVENT_ABC_LOSS_VOLT);  //ABC相失压
    else
      Clr_Event_Instant(ID_EVENT_ABC_LOSS_VOLT);  //ABC相没失压
    
    if(Get_Event_Instant(ID_EVENT_AB_LOSS_VOLT))
    {
      Clr_Event_Instant(ID_EVENT_A_LOSS_VOLT);  //A相没有失压
      Clr_Event_Instant(ID_EVENT_B_LOSS_VOLT);  //B相没有失压
    }
    if(Get_Event_Instant(ID_EVENT_AC_LOSS_VOLT))
    {
      Clr_Event_Instant(ID_EVENT_A_LOSS_VOLT);  //A相没有失压
      Clr_Event_Instant(ID_EVENT_C_LOSS_VOLT);  //C相没有失压
    }
    if(Get_Event_Instant(ID_EVENT_BC_LOSS_VOLT))
    {
      Clr_Event_Instant(ID_EVENT_B_LOSS_VOLT);  //B相没有失压
      Clr_Event_Instant(ID_EVENT_C_LOSS_VOLT);  //C相没有失压
    }
    if(Get_Event_Instant(ID_EVENT_ABC_LOSS_VOLT))
    {
      Clr_Event_Instant(ID_EVENT_A_LOSS_VOLT);  //A相没有失压
      Clr_Event_Instant(ID_EVENT_B_LOSS_VOLT);  //B相没有失压
      Clr_Event_Instant(ID_EVENT_C_LOSS_VOLT);  //C相没有失压
      Clr_Event_Instant(ID_EVENT_AB_LOSS_VOLT);  //AB相失压
      Clr_Event_Instant(ID_EVENT_BC_LOSS_VOLT);  //BC相失压
      Clr_Event_Instant(ID_EVENT_AC_LOSS_VOLT);  //AC相失压
    }
#else            //只判定单相失压
    Clr_Event_Instant(ID_EVENT_AB_LOSS_VOLT);  //AB电压失压清除
    Clr_Event_Instant(ID_EVENT_BC_LOSS_VOLT);  //BC电压失压清除
    Clr_Event_Instant(ID_EVENT_AC_LOSS_VOLT);  //AC电压失压清除
    Clr_Event_Instant(ID_EVENT_ABC_LOSS_VOLT);  //ABC电压失压清除
#endif    
  }

}
#endif

/***********************************************************************
函数功能：获取失流瞬时状态字
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_A_LOSS_CURR
void Check_Loss_Curr(void)
{

  INT8U Flag;
  FP32S max;
  
  if(Check_Power_Grid_Event())
  {
    Clr_Event_Instant(ID_EVENT_A_LOSS_CURR);  //A相没有失流
    Clr_Event_Instant(ID_EVENT_B_LOSS_CURR);  //B相没有失流
    Clr_Event_Instant(ID_EVENT_C_LOSS_CURR);  //C相没有失流
    Clr_Event_Instant(ID_EVENT_AB_LOSS_CURR);  //AB相没有失流
    Clr_Event_Instant(ID_EVENT_AC_LOSS_CURR);  //AC相没有失流
    Clr_Event_Instant(ID_EVENT_BC_LOSS_CURR);  //BC相没有失流
    Clr_Event_Instant(ID_EVENT_ALL_LOSS_CURR);  //ABC相没有失流    
 
    return ;
  }
  
  max=Get_Max_Num(Measu_InstantData_ToPub_PUCK.Curr.A,Measu_InstantData_ToPub_PUCK.Curr.B,Measu_InstantData_ToPub_PUCK.Curr.C);
  max/=UNIT_A;
  
  //if(max<=(Get_In()*ALL_LOSS_CURR_VALUE))  //三相电流同时小于2%In,全失流
  if(max<=Get_Start_Current())  //三相电流同时小于0.001*In,全失流
  {
#ifdef MULTI_LOSS_CURR_EN    //判定合相失流
    Set_Event_Instant(ID_EVENT_ALL_LOSS_CURR);  //ABC相失流
    Clr_Event_Instant(ID_EVENT_A_LOSS_CURR);  //A相没有失流
    Clr_Event_Instant(ID_EVENT_B_LOSS_CURR);  //B相没有失流
    Clr_Event_Instant(ID_EVENT_C_LOSS_CURR);  //C相没有失流
#else
    Clr_Event_Instant(ID_EVENT_ALL_LOSS_CURR);  //ABC相没失流
    Set_Event_Instant(ID_EVENT_A_LOSS_CURR);  //A相失流
    Set_Event_Instant(ID_EVENT_B_LOSS_CURR);  //B相失流
    Set_Event_Instant(ID_EVENT_C_LOSS_CURR);  //C相失流    
#endif
    Clr_Event_Instant(ID_EVENT_AB_LOSS_CURR);  //AB相没有失流
    Clr_Event_Instant(ID_EVENT_AC_LOSS_CURR);  //AC相没有失流
    Clr_Event_Instant(ID_EVENT_BC_LOSS_CURR);  //BC相没有失流
      
    if(Get_SysParse_Mode()==PARSE_331)           //331不判组合失流, B相不判断失流,同一时刻，A和C不能同时失流
      Clr_Event_Instant(ID_EVENT_B_LOSS_CURR);
    return;  
  } 
  else      //判定分相失流
  {
#ifdef LOSS_CURR_ONLY           //失流只判定电流，与电流平衡率无关
    if(((FP32S)Measu_InstantData_ToPub_PUCK.Curr.A/UNIT_A)<=Get_Start_Current())
       Set_Event_Instant(ID_EVENT_A_LOSS_CURR);  //A相失流
    else
       Clr_Event_Instant(ID_EVENT_A_LOSS_CURR);  //A相没有失流
    
    if(((FP32S)Measu_InstantData_ToPub_PUCK.Curr.B/UNIT_A)<=Get_Start_Current())
       Set_Event_Instant(ID_EVENT_B_LOSS_CURR);  //B相失流
    else
       Clr_Event_Instant(ID_EVENT_B_LOSS_CURR);  //B相没有失流
    
    if(((FP32S)Measu_InstantData_ToPub_PUCK.Curr.C/UNIT_A)<=Get_Start_Current())
       Set_Event_Instant(ID_EVENT_C_LOSS_CURR);  //C相失流
    else
       Clr_Event_Instant(ID_EVENT_C_LOSS_CURR);  //C相没有失流
    
    if(Get_SysParse_Mode()==PARSE_331)           //331不判组合失流, B相不判断失流,同一时刻，A和C不能同时失流
      Clr_Event_Instant(ID_EVENT_B_LOSS_CURR);
    
#else                             //失流需判定电流，同时也与电流平衡率有关
    FP32S temp;
    temp=(max-(FP32S)Measu_InstantData_ToPub_PUCK.Curr.A/UNIT_A)/max;  //电流不平衡率
    if(temp<0)  temp*=-1;
    if((temp>=EventInsParaVar.LossCurr_Per)&&(max>=EventInsParaVar.LossCurr_I))
    {
      Set_Event_Instant(ID_EVENT_A_LOSS_CURR);  //A相失流
    }
    else
    {
      Clr_Event_Instant(ID_EVENT_A_LOSS_CURR);  //A相没有失流
    }
    
    temp=(max-(FP32S)Measu_InstantData_ToPub_PUCK.Curr.B/UNIT_A)/max;   //电流不平衡率
    if(temp<0)  temp*=-1;
    if((temp>=EventInsParaVar.LossCurr_Per)&&(max>=EventInsParaVar.LossCurr_I))
    {
      Set_Event_Instant(ID_EVENT_B_LOSS_CURR);  //B相失流
    }
    else
    {
      Clr_Event_Instant(ID_EVENT_B_LOSS_CURR);  //B相没有失流
    }      
    
    temp=(max-(FP32S)Measu_InstantData_ToPub_PUCK.Curr.C/UNIT_A)/max;   //电流不平衡率
    if(temp<0)  temp*=-1;
    if((temp>=EventInsParaVar.LossCurr_Per)&&(max>=EventInsParaVar.LossCurr_I))
    {
      Set_Event_Instant(ID_EVENT_C_LOSS_CURR);  //C相失流
    }
    else
    {
      Clr_Event_Instant(ID_EVENT_C_LOSS_CURR);  //C相没有失流
    }
#endif
  }
  
  Flag=Get_SysParse_Mode();
  if(Flag==PARSE_331)           //331不判组合失流, B相不判断失流,同一时刻，A和C不能同时失流
  {
    Clr_Event_Instant(ID_EVENT_B_LOSS_CURR);
  }
  else                          //341                             
  {
#ifdef MULTI_LOSS_CURR_EN    //判定合相失流
    if(Get_Event_Instant(ID_EVENT_A_LOSS_CURR)&&(Get_Event_Instant(ID_EVENT_B_LOSS_CURR)))
      Set_Event_Instant(ID_EVENT_AB_LOSS_CURR);  //AB相失流     
    else
      Clr_Event_Instant(ID_EVENT_AB_LOSS_CURR);  //AB相失流
    
    if(Get_Event_Instant(ID_EVENT_A_LOSS_CURR)&&(Get_Event_Instant(ID_EVENT_C_LOSS_CURR)))
      Set_Event_Instant(ID_EVENT_AC_LOSS_CURR);  //AC相失流
    else
      Clr_Event_Instant(ID_EVENT_AC_LOSS_CURR);  //AC相失流
    
    if(Get_Event_Instant(ID_EVENT_B_LOSS_CURR)&&(Get_Event_Instant(ID_EVENT_C_LOSS_CURR)))
      Set_Event_Instant(ID_EVENT_BC_LOSS_CURR);  //BC相失流      
    else
      Clr_Event_Instant(ID_EVENT_BC_LOSS_CURR);  //BC相失流
    
    if(Get_Event_Instant(ID_EVENT_AB_LOSS_CURR)&&(Get_Event_Instant(ID_EVENT_BC_LOSS_CURR)))
      Set_Event_Instant(ID_EVENT_ALL_LOSS_CURR);   //ABC相失流 
    else
      Clr_Event_Instant(ID_EVENT_ALL_LOSS_CURR);   //ABC相失流
     
    if(Get_Event_Instant(ID_EVENT_AB_LOSS_CURR))
    {
      Clr_Event_Instant(ID_EVENT_A_LOSS_CURR);  //A相没有失流
      Clr_Event_Instant(ID_EVENT_B_LOSS_CURR);  //B相没有失流
    }
    if(Get_Event_Instant(ID_EVENT_AC_LOSS_CURR))
    {
      Clr_Event_Instant(ID_EVENT_A_LOSS_CURR);  //A相没有失流
      Clr_Event_Instant(ID_EVENT_C_LOSS_CURR);  //C相没有失流
    }
    if(Get_Event_Instant(ID_EVENT_BC_LOSS_CURR))
    {
      Clr_Event_Instant(ID_EVENT_B_LOSS_CURR);  //B相没有失流
      Clr_Event_Instant(ID_EVENT_C_LOSS_CURR);  //C相没有失流
    }
    if(Get_Event_Instant(ID_EVENT_ALL_LOSS_CURR))
    {
      Clr_Event_Instant(ID_EVENT_A_LOSS_CURR);  //A相没有失流
      Clr_Event_Instant(ID_EVENT_B_LOSS_CURR);  //B相没有失流
      Clr_Event_Instant(ID_EVENT_C_LOSS_CURR);  //C相没有失流
      Clr_Event_Instant(ID_EVENT_AB_LOSS_CURR);  //AB相失流
      Clr_Event_Instant(ID_EVENT_BC_LOSS_CURR);  //BC相失流
      Clr_Event_Instant(ID_EVENT_AC_LOSS_CURR);  //AC相失流
    }   
#else
    Clr_Event_Instant(ID_EVENT_AB_LOSS_CURR);  //AB相失流
    Clr_Event_Instant(ID_EVENT_BC_LOSS_CURR);  //BC相失流
    Clr_Event_Instant(ID_EVENT_AC_LOSS_CURR);  //AC相失流
    Clr_Event_Instant(ID_EVENT_ALL_LOSS_CURR);  //ABC相没有失流    
#endif
  }

}
#endif

/***********************************************************************
函数功能：获取断相瞬时状态字
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_A_LOSS_PARSE
void Check_Loss_Parse(void)
{
  INT8U Flag;
  FP32S temp;
  
  Flag=Check_Power_Grid_Event();
  if(Flag)
  {
    Clr_Event_Instant(ID_EVENT_A_LOSS_PARSE);  //A相没有断相
    Clr_Event_Instant(ID_EVENT_B_LOSS_PARSE);  //B相没有断相
    Clr_Event_Instant(ID_EVENT_C_LOSS_PARSE);  //C相没有断相
    return ;
  }
    
  temp=Get_Un();
  if((((FP32S)Measu_InstantData_ToPub_PUCK.Volt.A/UNIT_V)<=(temp*EventInsParaVar.LossVolt_Per))&&(((FP32S)Measu_InstantData_ToPub_PUCK.Curr.A/UNIT_A)<=EventInsParaVar.LossVolt_I))
    Set_Event_Instant(ID_EVENT_A_LOSS_PARSE);  //A相断相
  else
     Clr_Event_Instant(ID_EVENT_A_LOSS_PARSE);  //A相没有断相
  
  if((((FP32S)Measu_InstantData_ToPub_PUCK.Volt.B/UNIT_V)<=(temp*EventInsParaVar.LossVolt_Per))&&(((FP32S)Measu_InstantData_ToPub_PUCK.Curr.B/UNIT_A)<=EventInsParaVar.LossVolt_I))
    Set_Event_Instant(ID_EVENT_B_LOSS_PARSE);  //B相断相
  else
     Clr_Event_Instant(ID_EVENT_B_LOSS_PARSE);  //B相没有断相
  
  if((((FP32S)Measu_InstantData_ToPub_PUCK.Volt.C/UNIT_V)<=(temp*EventInsParaVar.LossVolt_Per))&&(((FP32S)Measu_InstantData_ToPub_PUCK.Curr.C/UNIT_A)<=EventInsParaVar.LossVolt_I))
    Set_Event_Instant(ID_EVENT_C_LOSS_PARSE);  //C相断相
  else
     Clr_Event_Instant(ID_EVENT_C_LOSS_PARSE);  //C相没有断相
  
  Flag=Get_SysParse_Mode();
  if(Flag==PARSE_331)
  {
    if((((FP32S)Measu_InstantData_ToPub_PUCK.Curr.B/UNIT_A)<=EventInsParaVar.LossVolt_I)&&(((FP32S)Measu_InstantData_ToPub_PUCK.Curr.C/UNIT_A)<=EventInsParaVar.LossVolt_I))
      Set_Event_Instant(ID_EVENT_B_LOSS_PARSE);  //B相断相
    else
      Clr_Event_Instant(ID_EVENT_B_LOSS_PARSE);  //B相没有断相
  }
}
#endif
/***********************************************************************
函数功能：获取反向(电流)瞬态字
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_A_NEG_CURR
void Check_Neg_Curr(void)
{
  FP32S temp;
  
  if(Check_Power_Grid_Event())
  {
    Clr_Event_Instant(ID_EVENT_A_NEG_CURR); //A电流反向
    Clr_Event_Instant(ID_EVENT_B_NEG_CURR);  //B电流反向
    Clr_Event_Instant(ID_EVENT_C_NEG_CURR);  //C电流反向
    return ;
  }
  temp=Get_In();
  
  //电流>0.1%In,电压大于10v，且有功功率反向
  if(((FP32S)Measu_InstantData_ToPub_PUCK.Curr.A/UNIT_A>temp/1000)&&\
     ((FP32S)Measu_InstantData_ToPub_PUCK.Volt.A/UNIT_V>10)&&\
     (Measu_Status_Mode_ToPub_PUCK.A_APwrDir))
    Set_Event_Instant(ID_EVENT_A_NEG_CURR); ///A电流反向
  else
    Clr_Event_Instant(ID_EVENT_A_NEG_CURR); //A电流反未向
  

    //电流>0.1%In,电压大于10v，且有功功率反向
  if(((FP32S)Measu_InstantData_ToPub_PUCK.Curr.B/UNIT_A>temp/1000)&&\
     ((FP32S)Measu_InstantData_ToPub_PUCK.Volt.B/UNIT_V>10)&&\
     (Measu_Status_Mode_ToPub_PUCK.B_APwrDir))
    Set_Event_Instant(ID_EVENT_B_NEG_CURR); //B电流反向
  else
    Clr_Event_Instant(ID_EVENT_B_NEG_CURR); //B电流反未向
  
    //电流>0.1%In,电压大于10v，且有功功率反向
  if(((FP32S)Measu_InstantData_ToPub_PUCK.Curr.C/UNIT_A>temp/1000)&&\
     ((FP32S)Measu_InstantData_ToPub_PUCK.Volt.C/UNIT_V>10)&&\
     (Measu_Status_Mode_ToPub_PUCK.C_APwrDir))
    Set_Event_Instant(ID_EVENT_C_NEG_CURR); //C电流反向
  else
    Clr_Event_Instant(ID_EVENT_C_NEG_CURR); //C电流反未向
}
#endif



/***********************************************************************
函数功能：获取失压变化状态字
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_LOSS_VOL
void Get_Loss_Volt_Change(void)
{
 Get_Change_Status(Meter_Run_Status.Volt_LossMiss.Byte,4,ID_EVENT_LOSS_VOL,&Loss_Volt_Change); 
}
#endif

/***********************************************************************
函数功能：获取失流变化状态字
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_LOSS_CUR
void Get_Loss_Curr_Change(void)
{
  Get_Change_Status(Meter_Run_Status.Meter_Stat3.Byte,4,ID_EVENT_LOSS_CUR,&Loss_Curr_Change); 
}
#endif
/***********************************************************************
函数功能：获取电流反向变化状态字
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_NEG_CUR
void Get_Curr_Neg_Change(void)
{ 
  Get_Change_Status(Meter_Run_Status.Meter_Stat2.Byte,0,ID_EVENT_NEG_CUR,&Curr_Neg_Change);  
}
#endif

/***********************************************************************
函数功能：获取电压电流逆相序瞬态字
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_VOLT_NEG_SEQ
void Check_Volt_Curr_Seq(void)
{

  if(Check_Power_Grid_Event())
  {
    Clr_Event_Instant(ID_EVENT_VOLT_NEG_SEQ); //电压相序正确，事件结束
    Clr_Event_Instant(ID_EVENT_CUR_NEG_SEQ);  //电流相序正确，事件结束
    return ;
  }
  
  if(Measu_Status_Mode_ToPub_PUCK.V_SeqErr)
    Set_Event_Instant(ID_EVENT_VOLT_NEG_SEQ); //电压相序错
  else
    Clr_Event_Instant(ID_EVENT_VOLT_NEG_SEQ); //电压相序正确
 
    
  //断相，不判逆相序
  if((FP32S)Get_Min_Num(Measu_InstantData_ToPub_PUCK.Volt.A,Measu_InstantData_ToPub_PUCK.Volt.B,Measu_InstantData_ToPub_PUCK.Volt.C)<UNIT_V*60.0)
     Clr_Event_Instant(ID_EVENT_VOLT_NEG_SEQ); //电压相序清除
  
  //331表ABC失压时，不判逆相序
  if(Get_SysParse_Mode()==PARSE_331)  //331不判逆相序
  {
     if(((FP32S)Measu_InstantData_ToPub_PUCK.Volt.A/UNIT_V)<=65  || \
                         Get_Event_Instant(ID_EVENT_B_LOSS_VOLT) || \
        ((FP32S)Measu_InstantData_ToPub_PUCK.Volt.C/UNIT_V)<=65)
      Clr_Event_Instant(ID_EVENT_VOLT_NEG_SEQ); //电压相序清除
  }
  
  if(Measu_Status_Mode_ToPub_PUCK.I_SeqErr)
    Set_Event_Instant(ID_EVENT_CUR_NEG_SEQ);  //电流相序错
  else
    Clr_Event_Instant(ID_EVENT_CUR_NEG_SEQ);  //电流相序正确
}
#endif
/***********************************************************************
函数功能：获取电压超限瞬时状态字
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_A_VOLT_OVERLOAD
void Check_Overload_Volt(void)
{
  
  if(Check_Power_Grid_Event())
  {
    Clr_Event_Instant(ID_EVENT_A_VOLT_OVERLOAD);  //A电压过压清除
    Clr_Event_Instant(ID_EVENT_B_VOLT_OVERLOAD);  //B电压过压清除
    Clr_Event_Instant(ID_EVENT_C_VOLT_OVERLOAD);  //C电压过压清除
    return ;
  }
  
  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.A/(FP32S)UNIT_V>Get_Un()*(FP32S)1.25)
  {
    Set_Event_Instant(ID_EVENT_A_VOLT_OVERLOAD);  //A电压过压
    Event_Vol_Cur.Over_Vol[0]=Measu_InstantData_ToPub_PUCK.Volt.A;
  }
  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.A/(FP32S)UNIT_V<Get_Un()*(FP32S)1.20)  //或者掉电,需加上掉电条件
    Clr_Event_Instant(ID_EVENT_A_VOLT_OVERLOAD);  //A电压过压清除

  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.B/(FP32S)UNIT_V>Get_Un()*(FP32S)1.25)
  {
    Set_Event_Instant(ID_EVENT_B_VOLT_OVERLOAD);  //B电压过压
    Event_Vol_Cur.Over_Vol[1]=Measu_InstantData_ToPub_PUCK.Volt.B;
  }
  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.B/(FP32S)UNIT_V<Get_Un()*(FP32S)1.20)  //或者掉电,需加上掉电条件
    Clr_Event_Instant(ID_EVENT_B_VOLT_OVERLOAD);  //B电压过压清除

  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.C/(FP32S)UNIT_V>Get_Un()*(FP32S)1.25)
  {
    Set_Event_Instant(ID_EVENT_C_VOLT_OVERLOAD);  //B电压过压
    Event_Vol_Cur.Over_Vol[2]=Measu_InstantData_ToPub_PUCK.Volt.C;
  }
  if((FP32S)Measu_InstantData_ToPub_PUCK.Volt.C/(FP32S)UNIT_V<Get_Un()*(FP32S)1.20)  //或者掉电,需加上掉电条件
    Clr_Event_Instant(ID_EVENT_C_VOLT_OVERLOAD);  //B电压过压清除

  if(Get_SysParse_Mode()==PARSE_331)
    Clr_Event_Instant(ID_EVENT_B_VOLT_OVERLOAD);  //B相不判断
}
#endif

/***********************************************************************
函数功能：判断是否是工厂模式
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_FAC_ON
void Check_Fac_Status(void)
{
  if(Get_Sys_Status()!=SYS_NORMAL)
  {
    Clr_Event_Instant(ID_EVENT_FAC_ON);
    return ;
  }
  if(Get_Meter_Hard_Mode()==MODE_FAC)
    Set_Event_Instant(ID_EVENT_FAC_ON);
  else
    Clr_Event_Instant(ID_EVENT_FAC_ON);  
}
#endif

/***********************************************************************
函数功能：判断是否跳线短接
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_KEY_SWITCH
void Check_Key_Switch(void)
{
  if(Get_Sys_Status()!=SYS_NORMAL)
  {
    Clr_Event_Instant(ID_EVENT_KEY_SWITCH);
    return ;
  }
  if(Get_Meter_Hard_Mode()!=MODE_RUN)
    Set_Event_Instant(ID_EVENT_KEY_SWITCH);
  else
    Clr_Event_Instant(ID_EVENT_KEY_SWITCH);  
}
#endif
/***********************************************************************
函数功能：判断编程开关是否打开
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_PRGKEY_ON
void Check_Prg_Key(void)
{
  if(Get_Sys_Status()!=SYS_NORMAL)   //VCC
  {
    Clr_Event_Instant(ID_EVENT_PRGKEY_ON);
    return ;
  }
  if(B_PRG_KEY_STATUS)  //曾经高电平
   KeyInitStatus.Prg=KEY_INIT_STATUS;
  
  if(KEY_INIT_STATUS==KeyInitStatus.Prg)
  {
    if(Key_Value_Pub.Key.Bit.PrgKey)
      Set_Event_Instant(ID_EVENT_PRGKEY_ON);
    else
      Clr_Event_Instant(ID_EVENT_PRGKEY_ON);
  }
  else
  {
    Clr_Event_Instant(ID_EVENT_PRGKEY_ON);
  }
}
#endif

/***********************************************************************
函数功能：判断手动清需量按钮是否打开
入口：无
出口：无
***********************************************************************/
#ifdef ID_CLR_DEMAND_KEY
void Check_ClrDemand_Key(void)
{
 if(Get_Sys_Status()!=SYS_NORMAL)    //VCC
 {
    Clr_Event_Instant(ID_CLR_DEMAND_KEY);
    return ;
 }
 
 if(B_PRG_KEY_STATUS)  //曾经高电平
   KeyInitStatus.Prg=KEY_INIT_STATUS;
 
 if(KEY_INIT_STATUS==KeyInitStatus.Prg)
 {
  if(B_PRG_KEY_STATUS==0)
    Set_Event_Instant(ID_CLR_DEMAND_KEY);
  else
    Clr_Event_Instant(ID_CLR_DEMAND_KEY);
 }
 else
 {
   Clr_Event_Instant(ID_CLR_DEMAND_KEY);   
 }
}
#endif

/***********************************************************************
函数功能：判断设置表地址开关是否打开
入口：无
出口：无
***********************************************************************/
#ifdef ID_METER_ADDR_EN
void Check_MeterAddr_Key(void)
{
  if(Inter_Right_Key_STATUS)                  //设置表地址按钮
    Clr_Event_Instant(ID_METER_ADDR_EN);
  else
    Set_Event_Instant(ID_METER_ADDR_EN);  
}
#endif
/***********************************************************************
函数功能：判断上盖是否打开
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_UP_COVER_OPEN
void Check_UpCover(void)
{
 if(Get_Sys_Status()!=SYS_NORMAL)    //VCC
 {
    Clr_Event_Instant(ID_EVENT_UP_COVER_OPEN);
    Clr_Event_Instant(ID_EVENT_UP_COVER_CLOSE); 
    return ;
 }
  
  if(UP_COVER_STATUS)   //正常情况下是按下去的
  {
    Set_Event_Instant(ID_EVENT_UP_COVER_OPEN);
    Clr_Event_Instant(ID_EVENT_UP_COVER_CLOSE); 
  }
  else
  {
    Clr_Event_Instant(ID_EVENT_UP_COVER_OPEN); 
    Set_Event_Instant(ID_EVENT_UP_COVER_CLOSE);
  }
}
#endif
/***********************************************************************
函数功能：判断后盖(铅封)是否打开
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_UP_COVER_OPEN
void Check_DownCover(void)
{
 if(Get_Sys_Status()!=SYS_NORMAL)    //VCC
 {
    Clr_Event_Instant(ID_EVENT_LEAD_COVER_OPEN);
    Clr_Event_Instant(ID_EVENT_LEAD_COVER_CLOSE); 
    
    Clr_Event_Instant(ID_EVENT_DOWN_COVER_OPEN);
    Clr_Event_Instant(ID_EVENT_DOWN_COVER_CLOSE); 
    
    return ;
 }
 
  if(DOWN_COVER_STATUS)  //正常情况下是按下去的,此盖是安装在后盖上的
  {
    Set_Event_Instant(ID_EVENT_LEAD_COVER_OPEN);
    Clr_Event_Instant(ID_EVENT_LEAD_COVER_CLOSE);  
    
    Set_Event_Instant(ID_EVENT_DOWN_COVER_OPEN);
    Clr_Event_Instant(ID_EVENT_DOWN_COVER_CLOSE);  
  }
  else
  {
    Clr_Event_Instant(ID_EVENT_LEAD_COVER_OPEN);  
    Set_Event_Instant(ID_EVENT_LEAD_COVER_CLOSE);
    
    Set_Event_Instant(ID_EVENT_DOWN_COVER_OPEN);
    Clr_Event_Instant(ID_EVENT_DOWN_COVER_CLOSE);  
  }
}
#endif
/***********************************************************************
函数功能：获取时钟电池欠压状态字
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_RTC_BAT_LOW
void Check_RTCBat_Low(void)
{
  if(Get_Sys_Status()!=SYS_NORMAL)
  {
    Clr_Event_Instant(ID_EVENT_RTC_BAT_LOW);
    return ;
  }
  if(B_BATLOWRTC_TEST_STATUS)
    Clr_Event_Instant(ID_EVENT_RTC_BAT_LOW);
  else
    Set_Event_Instant(ID_EVENT_RTC_BAT_LOW);  
}
#endif
/***********************************************************************
函数功能：获取低功耗电池欠压状态字
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_RTC_BAT_LOW
void Check_LPWBat_Low(void)
{
  if(Get_Sys_Status()!=SYS_NORMAL)
  {
    Clr_Event_Instant(ID_EVENT_LOWPWR_BAT_LOW);
    return ;
  }
  
  if(B_BATLOW_TEST_STATUS)
    Clr_Event_Instant(ID_EVENT_LOWPWR_BAT_LOW);
  else
    Set_Event_Instant(ID_EVENT_LOWPWR_BAT_LOW);  
}
#endif
/***********************************************************************
函数功能：获取AB切换模式按钮
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_AB_FUNC_KEY
void Check_AB_Func_Key(void)
{
  if((Inter_Up_Key_STATUS==1)&&(Inter_Down_Key_STATUS==1))  //没有按钮按下
    Clr_Event_Instant(ID_EVENT_AB_FUNC_KEY);
  else                         //有一个按钮按下
    Set_Event_Instant(ID_EVENT_AB_FUNC_KEY);  
}
#endif
/***********************************************************************
函数功能：获取AB切换模式按钮
入口：无
出口：无
***********************************************************************/
#ifdef ID_EVENT_ADJUST_MEASU
void Set_Cal_Event(void)
{
  INT8U FLag,temp;
  INT32U bootflag;
  
  FLag=Read_Storage_Data_PUCK(DI_CAL_STATUS,&temp,1);
  if(0==FLag)
  {
    ASSERT(A_WARNING,0);
    return ;
  }
  if(2!=temp)   //校表事件发生。没发生：0或者1
  {
    Set_Event_Instant(ID_EVENT_ADJUST_MEASU);
    return ;
  }
    
  FLag=Read_Storage_Data_PUCK(BOOT_FLAG,&bootflag,4);   //系统参数初始化标志
  if(0==FLag || FIRST_BOOT_ON_FLAG!=bootflag)  //读不出，或者读出的值不为 FIRST_BOOT_ON_FLAG
  {
   Set_Event_Instant(ID_EVENT_ADJUST_MEASU);
    return ;
  } 
  Clr_Event_Instant(ID_EVENT_ADJUST_MEASU);
}
#endif
/********************************************************************************
函数原型：
功能：检测系统时钟故障

*********************************************************************************/
void Monitor_Meter_System_Osc(void)
{
  if(SYS_NORMAL==Sys_Status.Status)
  {
    if(CSC!=0x01 ||CKC!=0x38)
    {
      Beep_For_OSC_Alarm_PUCK();
      DEBUG_PRINT(PUCK,1,"MCU Osc System Error:MSTOP=%d,HIOSTOP=%d,XTSTOP=%d",MSTOP,HIOSTOP,XTSTOP);  
      Set_Event_Instant(ID_EVENT_OSC_ERR); 
      return ;
    }
    Clr_Event_Instant(ID_EVENT_OSC_ERR);
    return ;
  }
  
  if(CSC!=0x01 ||CKC!=(0x38|RSUME_CLOCK_DIV))
  {
    Beep_For_OSC_Alarm_PUCK();
    DEBUG_PRINT(PUCK,1,"MCU Osc System Error:MSTOP=%d,HIOSTOP=%d,XTSTOP=%d",MSTOP,HIOSTOP,XTSTOP);  
    Set_Event_Instant(ID_EVENT_OSC_ERR); 
    return ;
  }
  Clr_Event_Instant(ID_EVENT_OSC_ERR);
}

/********************************************************************************
函数原型：
功能：获取表计上电状态字

*********************************************************************************/
void Get_Meter_PowerOn(void)
{    
  if(Get_Sys_Status()==SYS_NORMAL)
  {
    if(POWER_OFF==Power_Status.Power)
    {
      Clr_Event_Instant(ID_EVENT_POWER_OFF);  //掉电事件结束
      Power_Status.Power=POWER_ON;           //上电事件发生
      SET_STRUCT_SUM(Power_Status);  
    }
    if(ABNORMAL_RESET==Power_Status.Power)
    {
      Set_Event_Instant(ID_EVENT_RESET);     //复位事件
      Power_Status.Power=POWER_ON;           //上电事件发生
      SET_STRUCT_SUM(Power_Status);  
    }
  }
}