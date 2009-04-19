#define CLOCK_C
#include "Includes.h"
/*
INT8U Read_PHY_Mem_PUCK(INT8U MemNo,INT32U Offset,void *pDst,INT16U RD_Len,void *pDst_Start,INT16U DstLen);
INT8U Write_PHY_Mem_PUCK(INT8U MemNo,INT32U Offset,void *pSrc,INT16U SrcLen);
INT8U Write_ExtRTC_PUCK(INT8U *Src);
INT8U Read_ExtRTC_PUCK(INT8U *Dst,INT8U DstLen);
*/
/*
void init_DS3231(void);
读ds3231时间，读出数组的值：{秒，分，时，星期，日，月，年}
void Read_DS3231(unsigned char *src);
写ds3231时间，写入数组的值：{秒，分，时，星期，日，月，年}
正确返回1，出错返回err
unsigned char Write_DS3231(unsigned char *src);
*/

//在唤醒的情况下更新时钟
INT8U Resume_Clock_Update()
{
  INT8U Re;
  S_BCD_Time Temp_Time;
  
  Re = CHECK_STRUCT_SUM(Clock_Info);
  ASSERT(A_WARNING, 1==Re);
  
  Re = 0;
  
  if(Get_Time_From_EXT_RTC((S_BCD_Time *)&Temp_Time))
    Re=1;
  
  if(Read_Ext_RTC_Status()!=EXT_RTC_OK)//外部时钟是否复位?
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Read_Ext_RTC_Status Error!");
    ASSERT(A_WARNING,0);
    Re=0;
  }
  
  if(Re==0)//读取外部时钟数据错误或者读取到的年数据是0则读取内部时钟或者内存中的时间数据
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Get_Time_From_EXT_RTC error,Set_INTER_RTC_Flag=%d",Clock_Info.Set_INTER_RTC_Flag);
    Get_Timer_From_CPU_Inter(&Temp_Time);
    Set_Time_To_EXT_RTC((S_BCD_Time *)&Temp_Time);//重置外部时钟
  }
  
  Clock_Info.Set_INTER_RTC_Flag=1;//同步一下内部RTC
  Set_Time_To_INTER_RTC((S_BCD_Time *)&Temp_Time);
 
 
  //将获得的时间数据复制到Cur_Time1,Cur_Time_Bak和Cur_Time0
  mem_cpy((void *)&Cur_Time1,&Temp_Time,sizeof(Temp_Time),(void *)&Cur_Time1,sizeof(Cur_Time1));
  SET_STRUCT_SUM(Cur_Time1);
  
  mem_cpy((void *)&Cur_Time_Bak,&Temp_Time,sizeof(Temp_Time),(void *)&Cur_Time_Bak,sizeof(Cur_Time_Bak));
  SET_STRUCT_SUM(Cur_Time_Bak); 
  
  Convert_BCD_2_HEX_Time((S_BCD_Time *)&Cur_Time1,(S_HEX_Time *)&Cur_Time0);
  
  Clock_Info.Clock_Ready_Flag=1;
  SET_STRUCT_SUM(Clock_Info);//函数出口处设置校验和 
  return Re;
}


//从外部RTC读取时间数据
INT8U Get_Time_From_EXT_RTC(S_BCD_Time *pBCD_Time)
{
  INT8U Re,i;
  INT8U Temp[10];
  
  for(i=0;i<3;i++)
  {
    Re=Read_ExtRTC_PUCK(Temp,7);
    ASSERT(A_WARNING,1==Re);
    DEBUG_PRINT(HUCK,DEBUG_0,"Get_EXT_Time:%x-%x-%x %x:%x:%x,Week:%x",Temp[6],Temp[5],Temp[4],Temp[2],Temp[1],Temp[0],Temp[3]);
    if(1==Re)
    {
      if(Temp[3]>=7)//DLT645和DS3231的星期数据格式 不一样
        Temp[3]=0;
      pBCD_Time->Sec=Temp[0];
      pBCD_Time->Min=Temp[1];
      pBCD_Time->Hour=Temp[2];
      pBCD_Time->Week=Temp[3];
      pBCD_Time->Date=Temp[4];
      pBCD_Time->Month=Temp[5];
      pBCD_Time->Year=Temp[6];
      Set_STRUCT_Sum(pBCD_Time,sizeof(S_BCD_Time),pBCD_Time->CS,sizeof(pBCD_Time->CS));
      if(Check_BCD_Time(pBCD_Time))//检查数据格式?
        return 1;
    }
    OS_TimeDly_Ms(200);
  }
  
  return 0;
}

//将时间设置到外部RTC
INT8U Set_Time_To_EXT_RTC(S_BCD_Time *pBCD_Time)
{
  INT8U i;
  INT8U Re;
  INT8U Temp[10];
  
  Re=Check_BCD_Time(pBCD_Time);
  if(ASSERT(A_WARNING,1==Re))
    return 0;
  
  for(i=0;i<3;i++)
  {
    Temp[0]=pBCD_Time->Sec;
    Temp[1]=pBCD_Time->Min;
    Temp[2]=pBCD_Time->Hour;
    Temp[3]=pBCD_Time->Week;
    Temp[4]=pBCD_Time->Date;
    Temp[5]=pBCD_Time->Month;
    Temp[6]=pBCD_Time->Year;
    if(Temp[3]==0 || Temp[3]>7)//DLT645和DS3231的星期数据格式 不一样
      Temp[3]=7;
    
    DEBUG_PRINT(HUCK,DEBUG_0,"Set_EXT_Time:%x-%x-%x %x:%x:%x,Week:%x",Temp[6],Temp[5],Temp[4],Temp[2],Temp[1],Temp[0],Temp[3]);
    
    if(Write_ExtRTC_PUCK(Temp))
      return 1;
      
    OS_TimeDly_Ms(200);
  }
  ASSERT(A_WARNING,0);
  return 0;
}

//将Time数据设置到pBCD_Time数据中
//Time数据的格式是--sec,min,hour,week,date,month,year
void Set_BCD_Time(INT8U Time[],S_BCD_Time *pBCD_Time)
{
  if(Time[3]>=7)//DLT645和DS3231的星期数据格式 不一样
    Time[3]=0;
  pBCD_Time->Sec=Time[0];
  pBCD_Time->Min=Time[1];
  pBCD_Time->Hour=Time[2];
  pBCD_Time->Week=Time[3];
  pBCD_Time->Date=Time[4];
  pBCD_Time->Month=Time[5];
  pBCD_Time->Year=Time[6];  
  Set_STRUCT_Sum(pBCD_Time,sizeof(S_BCD_Time),pBCD_Time->CS,sizeof(pBCD_Time->CS));  
}

//从内部RTC读取时间数据
INT8U Get_Time_From_INTER_RTC(S_BCD_Time *pBCD_Time)
{
  INT8U Re;
  INT8U Temp[10];
  
  Re=Read_InterRTC_PUCK(Temp,7);
  ASSERT(A_WARNING,1==Re);
  //DEBUG_PRINT(HUCK,DEBUG_0,"Get_INTER_Time:%x-%x-%x %x:%x:%x,Week:%x",Temp[6],Temp[5],Temp[4],Temp[2],Temp[1],Temp[0],Temp[3]);
  if(1==Re)
  {/*
    if(Temp[3]>=7)//DLT645和DS3231的星期数据格式 不一样
      Temp[3]=0;
    pBCD_Time->Sec=Temp[0];
    pBCD_Time->Min=Temp[1];
    pBCD_Time->Hour=Temp[2];
    pBCD_Time->Week=Temp[3];
    pBCD_Time->Date=Temp[4];
    pBCD_Time->Month=Temp[5];
    pBCD_Time->Year=Temp[6];
    Set_STRUCT_Sum(pBCD_Time,sizeof(S_BCD_Time),pBCD_Time->CS,sizeof(pBCD_Time->CS));    
    */
    Set_BCD_Time(Temp,pBCD_Time);

    Re=Check_BCD_Time(pBCD_Time);//检查数据格式?
    if(ASSERT(A_WARNING,1==Re))
      return 0;
    else 
      return 1;
  }
  else
    return 0;
}

//将时间设置到内部RTC
INT8U Set_Time_To_INTER_RTC(S_BCD_Time *pBCD_Time)
{ 
  INT8U Re;
  INT8U Temp[10];
  
  Re=Check_BCD_Time(pBCD_Time);
  if(ASSERT(A_WARNING,1==Re))
    return 0;
  Temp[0]=pBCD_Time->Sec;
  Temp[1]=pBCD_Time->Min;
  Temp[2]=pBCD_Time->Hour;
  Temp[3]=pBCD_Time->Week;
  Temp[4]=pBCD_Time->Date;
  Temp[5]=pBCD_Time->Month;
  Temp[6]=pBCD_Time->Year;
  if(Temp[3]==0 || Temp[3]>7)//DLT645和DS3231的星期数据格式 不一样
    Temp[3]=7;
  
  Re=Write_InterRTC_PUCK(Temp);
  return Re;
}

//判断时间的正确性
INT8U _Check_HEX_Time(INT8U Time[])
{
  if(Time[T_MIN]<60 && Time[T_HOUR]<24 &&\
     Time[T_DATE]>0 && Time[T_DATE]<32 &&\
     Time[T_MONTH]>0 && Time[T_MONTH]<13 &&\
     Time[T_YEAR]<=99)
    return 1;
  else
  {
    //ASSERT(A_WARNING,0);
    return 0;
  }
}

//判断时间的正确性
INT8U _Check_BCD_Time(INT8U Time[])
{
  if(Check_BCD_Data(Time,5)==0)
    return 0;
  
  if(Time[T_MIN]<0x60 && Time[T_HOUR]<0x24 &&\
     Time[T_DATE]>0 && Time[T_DATE]<0x32 &&\
     Time[T_MONTH]>0 && Time[T_MONTH]<0x13 &&\
     Time[T_YEAR]<=0x99)
    return 1;
  else
  {
    ASSERT(A_WARNING,0);
    return 0;
  }
}

//检查一个S_HEX_Time数据是否正确
INT8U Check_HEX_Time(S_HEX_Time *pTime)
{
  if(Check_STRUCT_Sum(pTime,sizeof(S_HEX_Time),pTime->CS,sizeof(pTime->CS)))
  {
    if(_Check_HEX_Time(pTime->Time))
      return 1;
  }
  ASSERT(A_WARNING,0);
  return 0;
}
/*
  INT8U Sec;
  INT8U Min;
  INT8U Hour;
  INT8U Week;
  INT8U Date;
  INT8U Month;
  INT8U Year;

*/
//检查一个S_BCD_Time数据是否正确
INT8U Check_BCD_Time(S_BCD_Time *pTime)
{
  if(Check_STRUCT_Sum(pTime,sizeof(S_BCD_Time),pTime->CS,sizeof(pTime->CS))==1 &&
     Check_BCD_Data(pTime,S_OFF(S_BCD_Time,CS))==1)
  {
    if(pTime->Sec<0x60 && pTime->Min<0x60 && pTime->Hour<0x24 &&\
       pTime->Date>0x00 && pTime->Date<0x32 &&\
       pTime->Month>0x00 && pTime->Month<0x13 &&\
       pTime->Year<=0x99 && pTime->Week<7)
      return 1;
  }
  ASSERT(A_WARNING,0);
  return 0;
}

INT8U Set_Time_EXT_INTER_Ram_RTC(S_BCD_Time * pBCD_Time)
{
  if(Set_Time_To_EXT_RTC(pBCD_Time))
  {
    if(Set_Time_To_INTER_RTC(pBCD_Time))
    {
      //mem_cpy((S_BCD_Time *)&Cur_Time1,&TempTime,sizeof(TempTime),(S_BCD_Time *)&Cur_Time1,sizeof());
      Get_Time_From_INTER_RTC((S_BCD_Time *)&Cur_Time1);
      Convert_BCD_2_HEX_Time((S_BCD_Time *)&Cur_Time1,(S_HEX_Time *)&Cur_Time0);
      return 1;
    }
  } 
  ASSERT(A_WARNING,0);
  return 0;
}
//时钟处理
//每过GET_CLOCK_PERIOD秒刷新一次时钟
void Clock_Proc()
{
  INT8U Re;
  S_BCD_Time TempTime;

  static S_Int32U Sec_Bak={CHK_BYTE,0xFFFFFFFF,CHK_BYTE};
  static S_Int8U Sec={CHK_BYTE,0xFF,CHK_BYTE};
  
  //定时刷新时钟，并模拟秒时钟
  Re=CHECK_STRUCT_VAR(Sec_Bak);
  ASSERT(A_WARNING,1==Re);
  
  if(Sec_Bak.Var!=Sec_Timer_Pub)//时间过了一秒
  {
    Sec_Bak.Var=Sec_Timer_Pub;
    
    Re=Check_BCD_Time((S_BCD_Time *)&Cur_Time1);
    if(ASSERT(A_WARNING,1==Re))//时间不对则刷新时间
      Clock_Update();

    Set_Soft_RTC_PUCK(ID_RAM_RTC);//设置时间到PUCK的时钟数据
    
    Get_Time_From_INTER_RTC((S_BCD_Time *)&TempTime);//读取内部时钟
    if(Check_Time_Diff(&TempTime,(S_BCD_Time *)&Cur_Time1,5)==0)//内存中的数据与内部RTC数据时间差超过5分钟,认为出错
    {
      ASSERT(A_WARNING,0);
      Clock_Update();
      Get_Time_From_INTER_RTC((S_BCD_Time *)&TempTime);//重新读取内部时钟
    }
    
    mem_cpy((void *)&Cur_Time1,&TempTime,sizeof(TempTime),(void *)&Cur_Time1,sizeof(Cur_Time1));
    
    Sec.Var++; //秒时钟+1
    if(Sec.Var>60 ||\
       Bcd2Hex_Byte(Cur_Time1.Min)!=Cur_Time0.Time[T_MIN])//一分钟之内重读一次硬时钟
    {
      Clock_Update();
      Sec.Var=Bcd2Hex_Byte(Cur_Time1.Sec);
    }
  }
}

//从内存中取得一分正确的时钟数据
INT8U Get_Time_From_Ram_Rom(S_BCD_Time *pBCD_Time)
{
  INT8U Re,Err;
  INT8U i;
  
  Re=0;
  if(1==CHECK_STRUCT_SUM(Cur_Time1) &&\
     1==Check_BCD_Time((S_BCD_Time *)&Cur_Time1))
  {
     mem_cpy(pBCD_Time,(void *)&Cur_Time1,sizeof(Cur_Time1),pBCD_Time,sizeof(S_BCD_Time));
     Re=1;
  }
  else if(1==CHECK_STRUCT_SUM(Cur_Time_Bak) &&\
          1==Check_BCD_Time((S_BCD_Time *)&Cur_Time_Bak))
  {
     mem_cpy(pBCD_Time,(void *)&Cur_Time_Bak,sizeof(Cur_Time_Bak),pBCD_Time,sizeof(S_BCD_Time));
     Re=1;
  }
  else if(1==CHECK_STRUCT_SUM(Cur_Time0) &&\
          1==Check_HEX_Time((S_HEX_Time *)&Cur_Time0))
  {
    Convert_HEX_2_BCD_Time((S_HEX_Time *)&Cur_Time0,pBCD_Time);
    Re=1;
  }
  else if(Read_Storage_Data(TIME_PD,pBCD_Time,pBCD_Time,sizeof(S_BCD_Time),&Err))//读取存储时间
  {
    Re = 1;
  }
  else if(Read_Storage_Data(TIME_BAK_PD,pBCD_Time,pBCD_Time,sizeof(pBCD_Time),&Err))//读取存储备份时间
  {
    Re = 1; 
  }
  else//没有任何时间数据是正确的，彻底玩完了~~
  {
    for(i=0; i<3; i++)//重新从Rom中读取时间数据，读3次，防止干扰原因导致读取出错
    {
      if(Read_Storage_Data(TIME_PD,pBCD_Time,pBCD_Time,sizeof(S_BCD_Time),&Err))//读取存储时间
      {
        Re = 1;
        break;
      }
      OS_TimeDly_Ms(100);//延时100ms
      if(Read_Storage_Data(TIME_BAK_PD,pBCD_Time,pBCD_Time,sizeof(pBCD_Time),&Err))//读取存储备份时间
      {
        Re = 1;
        break;
      } 
      OS_TimeDly_Ms(100);//延时100ms
    } 
    
    if(Re == 0)
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"No Correct Time Find,Use Def start Time");
      mem_cpy(pBCD_Time,(void *)&Def_BCD_Time,sizeof(Def_BCD_Time),pBCD_Time,sizeof(S_BCD_Time));
      Re=0;
    }
  }
  
  Set_STRUCT_Sum(pBCD_Time,sizeof(S_BCD_Time),pBCD_Time->CS,sizeof(pBCD_Time->CS));  
  return Re;
}

//从CPU内部获取时间数据
void Get_Timer_From_CPU_Inter(S_BCD_Time *pBCD_Time)
{
    if(1==Clock_Info.Set_INTER_RTC_Flag &&\
       1==Get_Time_From_INTER_RTC(pBCD_Time) &&\
       1==Check_BCD_Time(pBCD_Time))
    {
      return;
    }
    else Get_Time_From_Ram_Rom(pBCD_Time);
  
}

//检查两个时间数据之间的差是否正常?是否在Mins分钟之内?
INT8U Check_Time_Diff(S_BCD_Time *pTime0,S_BCD_Time *pTime1,INT8U Mins)
{
  static S_HEX_Time TempTime;//静态变量，不占用堆栈
  static S_HEX_Time Time0,Time1;
  
  Convert_BCD_2_HEX_Time(pTime0,&Time0);
  Convert_BCD_2_HEX_Time(pTime1,&Time1);
  
  if(Cmp_Time(&Time0,&Time1)==TIME_AFT)
  {
    Time_Dec(&Time0,Mins,UNIT_MIN,&TempTime);
    if(Cmp_Time(&TempTime,&Time1)==TIME_BEF)
      return 1;
  }
  else
  {
    Time_Inc(&Time0,Mins,UNIT_MIN,&TempTime);
    if(Cmp_Time(&TempTime,&Time1)==TIME_AFT)
      return 1; 
  }
  return 0;
  /*
  INT32U Sec0,Sec1;
  Sec0=mktime((INT16U)Bcd2Hex_Byte(pTime0->Year)+2000,\
              Bcd2Hex_Byte(pTime0->Month),\
              Bcd2Hex_Byte(pTime0->Date),\
              Bcd2Hex_Byte(pTime0->Hour),\
              Bcd2Hex_Byte(pTime0->Min),\
              0);
  Sec1=mktime((INT16U)Bcd2Hex_Byte(pTime1->Year)+2000,\
              Bcd2Hex_Byte(pTime1->Month),\
              Bcd2Hex_Byte(pTime1->Date),\
              Bcd2Hex_Byte(pTime1->Hour),\
              Bcd2Hex_Byte(pTime1->Min),\
              0);
  if((Sec0>Sec1 && Sec0-Sec1>180) ||\
    (Sec0<Sec1 && Sec1-Sec0>180))
      return 0;
  else
    return 1;
  */
}

void Set_Err_Time(S_BCD_Time *pBCD_Time)
{
  Err_Time[0]=pBCD_Time->Min;
  Err_Time[1]=pBCD_Time->Hour;
  Err_Time[2]=pBCD_Time->Date;
  Err_Time[3]=pBCD_Time->Month;
  Err_Time[4]=pBCD_Time->Year;  
}
//时钟的刷新
void Clock_Update()
{
  INT8U i;
  INT8U Re,EXT_RTC_Status_Err_Flag;
  S_BCD_Time Temp_Time;
  static S_BCD_Time Temp_Time0;
  static S_BCD_Time Temp_Time1;
  
  EXT_RTC_Status_Err_Flag=0;
  Re=CHECK_STRUCT_SUM(Clock_Info);
  if(ASSERT(A_WARNING,1==Re))//校验正确
  {
    mem_set((void *)&Clock_Info,0,sizeof(Clock_Info),(void *)&Clock_Info,sizeof(Clock_Info));
    SET_STRUCT_SUM(Clock_Info);
  }

  //先读取外部RTC，看能否读回正确数据,不正确的话，连续读3次
  Re=0;//获取到一组正确的时钟数据
  if(Get_Time_From_EXT_RTC((S_BCD_Time *)&Temp_Time))
    Re=1;
  
  if(Read_Ext_RTC_Status()!=EXT_RTC_OK)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Read_Ext_RTC_Status Error!");
    ASSERT(A_WARNING,0);
    
    if(Re==1)//在能够读回外部时钟数据的情况下才置时钟错，否则一旦外部时钟有硬件错误，则可能导致1分钟写一次事件记录
    {
      Set_Err_Time((S_BCD_Time *)&Cur_Time1);
      Set_Event_Instant(ID_EVENT_RTCERR_RUN);//时钟乱事件记录
    }
    
    Re=0;    
    EXT_RTC_Status_Err_Flag=1;//读取外部RTC状态出错!
  }
  
  if(Re==1)
    Clock_Info.EXT_RTC_Err_Counts=0; 
  else
    Clock_Info.EXT_RTC_Err_Counts++;//外部RTC错误计数  
  
  if(Re==0)//读取外部时钟数据错误或者读取到的年数据是0则读取内部时钟或者内存中的时间数据
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Get_Time_From_EXT_RTC error,Set_INTER_RTC_Flag=%d",Clock_Info.Set_INTER_RTC_Flag);
    Get_Timer_From_CPU_Inter(&Temp_Time);
  }

  //两个时间点之间的时间差太大了，超过了10分钟，而且连续读取3次结果都一样
  for(i=0;i<3;i++)
  {
    if(Check_Time_Diff(&Temp_Time,(S_BCD_Time *)&Cur_Time1,10)==0)//如果外部时钟与内部时钟差超过10分钟,则重读外时钟，防止出错
    {
      OS_TimeDly_Ms(200);
      Get_Time_From_EXT_RTC(&Temp_Time);
    }
    else
      break;
  }
  
  if(Check_Time_Diff(&Temp_Time,(S_BCD_Time *)&Cur_Time1,10)==0)//如果外部时钟与内部时钟差超过10分钟！
  {
    if(Clock_Info.Set_INTER_RTC_Flag==1)//内部时钟曾经被设置过
    {
      Get_Time_From_INTER_RTC((S_BCD_Time *)&Temp_Time0);
      Convert_HEX_2_BCD_Time((S_HEX_Time *)&Cur_Time0,(S_BCD_Time *)&Temp_Time1);
      //CPU内4个时钟数据都没有超过5分钟的差，认为内部时钟正确
      if(Check_Time_Diff((S_BCD_Time *)&Temp_Time0,(S_BCD_Time *)&Cur_Time1,5)==1 &&\
         Check_Time_Diff((S_BCD_Time *)&Temp_Time0,(S_BCD_Time *)&Cur_Time_Bak,5)==1 &&\
         Check_Time_Diff((S_BCD_Time *)&Temp_Time0,(S_BCD_Time *)&Temp_Time1,5)==1)
      {
        //Set_Err_Time(&Temp_Time);
        //Set_Event_Instant(ID_EVENT_RTCERR_RUN);//时钟乱事件记录
        mem_cpy(&Temp_Time,(S_BCD_Time *)&Temp_Time0,sizeof(Temp_Time0),(S_BCD_Time *)&Temp_Time,sizeof(Temp_Time));
        EXT_RTC_Status_Err_Flag=1; 
      }
    }
  }
  
  Clock_Info.Set_INTER_RTC_Flag=1;//同步一下内部RTC
  Set_Time_To_INTER_RTC((S_BCD_Time *)&Temp_Time);
  
  //年数据为0或者连续多次读RTC出错,则重置外部RTC
  if(Clock_Info.EXT_RTC_Err_Counts>5 || EXT_RTC_Status_Err_Flag==1)//外部时钟连续错误5次,重置外部时钟
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Reset EXT RTC!!!!!!!!!!!!");
    Set_Time_To_EXT_RTC((S_BCD_Time *)&Temp_Time);//重置外部时钟
    //Set_Event_Instant(ID_EVENT_RTCERR_RUN);//时钟乱事件记录
    Clock_Info.EXT_RTC_Err_Counts=0;
  }   
 
  //将获得的时间数据复制到Cur_Time1,Cur_Time_Bak和Cur_Time0
  mem_cpy((void *)&Cur_Time1,&Temp_Time,sizeof(Temp_Time),(void *)&Cur_Time1,sizeof(Cur_Time1));
  SET_STRUCT_SUM(Cur_Time1);
  
  mem_cpy((void *)&Cur_Time_Bak,&Temp_Time,sizeof(Temp_Time),(void *)&Cur_Time_Bak,sizeof(Cur_Time_Bak));
  SET_STRUCT_SUM(Cur_Time_Bak); 
  
  Convert_BCD_2_HEX_Time((S_BCD_Time *)&Cur_Time1,(S_HEX_Time *)&Cur_Time0);
  
  Clock_Info.Clock_Ready_Flag=1;
  SET_STRUCT_SUM(Clock_Info);//函数出口处设置校验和
}


//S_BCD_Time转换为S_HEX_Time
//转换成功返回1，失败返回0,失败是由于原数据格式或者校验和不对
INT8U Convert_BCD_2_HEX_Time(S_BCD_Time *pBCD_Time,S_HEX_Time *pHEX_Time)
{
  INT8U Re;

  Re=Check_BCD_Time(pBCD_Time);
  ASSERT(A_WARNING,1==Re);

  pHEX_Time->Time[T_MIN]=Bcd2Hex_Byte(pBCD_Time->Min);
  pHEX_Time->Time[T_HOUR]=Bcd2Hex_Byte(pBCD_Time->Hour);
  pHEX_Time->Time[T_DATE]=Bcd2Hex_Byte(pBCD_Time->Date);
  pHEX_Time->Time[T_MONTH]=Bcd2Hex_Byte(pBCD_Time->Month); 
  pHEX_Time->Time[T_YEAR]=Bcd2Hex_Byte(pBCD_Time->Year);
  pHEX_Time->Week=Bcd2Hex_Byte(pBCD_Time->Week);
  Set_STRUCT_Sum(pHEX_Time,sizeof(S_HEX_Time),pHEX_Time->CS,sizeof(pHEX_Time->CS));

  return Re;
}

//S_HEX_Time转换为S_BCD_Time
//1表示转换成功，0表示失败,失败是由于原数据格式或者校验和不对
INT8U Convert_HEX_2_BCD_Time(S_HEX_Time *pHEX_Time,S_BCD_Time *pBCD_Time)
{
  INT8U Re;

  Re=Check_HEX_Time(pHEX_Time);
  ASSERT(A_WARNING,1==Re);

  pBCD_Time->Min=Hex2Bcd_Byte(pHEX_Time->Time[T_MIN]);
  pBCD_Time->Hour=Hex2Bcd_Byte(pHEX_Time->Time[T_HOUR]);
  pBCD_Time->Date=Hex2Bcd_Byte(pHEX_Time->Time[T_DATE]);
  pBCD_Time->Month=Hex2Bcd_Byte(pHEX_Time->Time[T_MONTH]); 
  pBCD_Time->Year=Hex2Bcd_Byte(pHEX_Time->Time[T_YEAR]);
  pBCD_Time->Week=Hex2Bcd_Byte(pHEX_Time->Week);
  Set_STRUCT_Sum(pBCD_Time,sizeof(S_BCD_Time),pBCD_Time->CS,sizeof(pBCD_Time->CS));

  return Re;
}
//初始化时钟相关参数
void Init_Clock_Ram_Para()
{
  mem_set((void *)&Clock_Info,0,sizeof(Clock_Info),(void *)&Clock_Info,sizeof(Clock_Info));
  SET_STRUCT_SUM(Clock_Info);
}


//检查时钟数据的有效性
void Check_Clock_Data_Avail()
{
  INT8U Re;
   
  //Cur_Time0,Cur_Time1和Cur_Time_Bak之间互相纠错，互相备份
  //如果备份都错了，很不幸只能重新读取外部时钟了
  Re=Check_HEX_Time((S_HEX_Time *)&Cur_Time0);
  if(ASSERT(A_WARNING,1==Re))
  {
    if(Check_BCD_Time((S_BCD_Time *)&Cur_Time1))
      Convert_BCD_2_HEX_Time((S_BCD_Time *)&Cur_Time1,(S_HEX_Time *)&Cur_Time0);
    else if(Check_BCD_Time((S_BCD_Time *)&Cur_Time_Bak))
      Convert_BCD_2_HEX_Time((S_BCD_Time *)&Cur_Time_Bak,(S_HEX_Time *)&Cur_Time0);
    else 
      Clock_Update();
  }
  
  Re=Check_BCD_Time((S_BCD_Time *)&Cur_Time1);
  if(ASSERT(A_WARNING,1==Re))
  {
    if(Check_HEX_Time((S_HEX_Time *)&Cur_Time0))
      Convert_HEX_2_BCD_Time((S_HEX_Time *)&Cur_Time0,(S_BCD_Time *)&Cur_Time1);
    else if(Check_BCD_Time((S_BCD_Time *)&Cur_Time_Bak))
      mem_cpy((void *)&Cur_Time1,(void *)&Cur_Time_Bak,sizeof(Cur_Time_Bak),(void *)&Cur_Time1,sizeof(Cur_Time1));
    else 
      Clock_Update();
  }
  
  Re=Check_BCD_Time((S_BCD_Time *)&Cur_Time_Bak);
  if(ASSERT(A_WARNING,1==Re))
  {
    if(Check_HEX_Time((S_HEX_Time *)&Cur_Time0))
      Convert_HEX_2_BCD_Time((S_HEX_Time *)&Cur_Time0,(S_BCD_Time *)&Cur_Time_Bak);
    else if(Check_BCD_Time((S_BCD_Time *)&Cur_Time1))
      mem_cpy((void *)&Cur_Time_Bak,(void *)&Cur_Time1,sizeof(Cur_Time1),(void *)&Cur_Time_Bak,sizeof(Cur_Time_Bak));
    else 
      Clock_Update();
  }
}
//掉电时保存掉电时间
void Save_PD_Time()
{
  if(Check_BCD_Time((S_BCD_Time *)&Cur_Time1))
  {
    Write_Storage_Data(TIME_PD,(void *)&Cur_Time1,sizeof(Cur_Time1));   
    
  }
}

void Save_Bak_PD_Time()
{
  if(Check_BCD_Time((S_BCD_Time *)&Cur_Time1))
  {
    Write_Storage_Data(TIME_BAK_PD,(void *)&Cur_Time1,sizeof(Cur_Time1));   
    
  }  
}

//等待始终准备好,需要时钟的任务需要任务进入流程前调用该函数
void Wait_Clock_Ready()
{
  OS_Waitfor(1==Clock_Info.Clock_Ready_Flag);
}

//初始化时钟
void Init_Clock(INT32U Mode)
{
  //INT16U Len;
  //S_BCD_Time Temp_Time;
  //INT8U Temp[7];
  //INT8U Re,Err;
  
  Clock_Info.Set_INTER_RTC_Flag=0;
  SET_STRUCT_SUM(Clock_Info);
   
  DEBUG_PRINT(HUCK,DEBUG_0,"Init Clock!!");
  if(Resume_Clock_Update()==0)//外部时钟出错的情况下，形成一次事件记录
  {
    Set_Err_Time((S_BCD_Time *)&Def_BCD_Time);//默认时间作为纠错前时间，因为这里外部时钟读不出来，不知道出错前时间
    Event_MultiTimes_Proc(ID_EVENT_RTCERR_PWRON,EVENT_OCCUR,EVENT_REAL);
  }
/*  
  if(SYS_SLEEP==Mode)//当前为睡眠态,是由正常态掉电复位而来，为了省电，从内存中取时间数据设置到内部RTC
  {
     if(Get_Time_From_Ram_Rom(&Temp_Time)==0)//重新设置内部时钟，因为复位后内部时钟清0
       Get_Time_From_EXT_RTC(&Temp_Time);

     Set_Time_To_INTER_RTC((S_BCD_Time *)&Temp_Time);
     Clock_Info.Set_INTER_RTC_Flag=1;
     SET_STRUCT_SUM(Clock_Info);
  }
  else  //不是在睡眠模式下需要读取外部RTC,正常或者唤醒模式
  {
    Re=Get_Time_From_EXT_RTC((S_BCD_Time *)&Temp_Time);
    if(Read_Ext_RTC_Status()!=EXT_RTC_OK)
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Read_Ext_RTC_Status Error!");
      ASSERT(A_WARNING,0);
      Re=0;
    }
    
    if(0==Re)//没有从外部RTC取得正确数据
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"RTC Err,Reset EXT RTC!!!!!!!!!!!!");
      
      Set_Err_Time(&Temp_Time);
      Set_Event_Instant(ID_EVENT_RTCERR_RUN);//时钟乱事件记录
      
      //从内存中取一份正确的时间数据填充到Cur_Time1
      if(Get_Soft_RTC_PUCK(Temp,sizeof(Temp)))//上电前可能是低功耗状态,需要将低功耗时间数据重设到内部RTC中
      {
        Set_BCD_Time(Temp,(S_BCD_Time *)&Temp_Time);
        if(Check_BCD_Time(&Temp_Time))
          mem_cpy((void *)&Cur_Time1,&Temp_Time,sizeof(Temp_Time),(void *)&Cur_Time1,sizeof(Cur_Time1));
        else
          Get_Time_From_Ram_Rom((S_BCD_Time *)&Cur_Time1);
      }
      else
      {
        Len=Read_Storage_Data(TIME_PD,&Temp_Time,&Temp_Time,sizeof(Temp_Time),&Err); 
        if(Len>0 && Check_BCD_Time(&Temp_Time))
        {
          mem_cpy((void *)&(Cur_Time1),&Temp_Time,sizeof(Temp_Time),(void *)&Cur_Time1,sizeof(Cur_Time1));
          DEBUG_PRINT(HUCK,DEBUG_0,"No correct time,use TIME_PD Data!");
        }
        else
        {
          Len=Read_Storage_Data(TIME_BAK_PD,&Temp_Time,&Temp_Time,sizeof(Temp_Time),&Err); 
          if(Len>0 && Check_BCD_Time(&Temp_Time))
          {
            mem_cpy((void *)&(Cur_Time1),&Temp_Time,sizeof(Temp_Time),(void *)&Cur_Time1,sizeof(Cur_Time1));
            DEBUG_PRINT(HUCK,DEBUG_0,"No correct time,use TIME_BAK_PD Data!");
          } 
        }
      }
      
      if(Check_BCD_Time((S_BCD_Time *)&Cur_Time1)==0)//从内存中取一份时钟数据，设置到外部RTC
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Reset EXT RTC 01-01-01 00:00!!!!!!!!!!!!");
        mem_cpy((void *)&Cur_Time1,(void *)&Def_BCD_Time,sizeof(Def_BCD_Time),(void *)&Cur_Time1,sizeof(Cur_Time1));
        SET_STRUCT_SUM(Cur_Time1);
      }
      //此时Cur_Time1是一个正确数据，将此数据设置到外部RTC
      Set_Time_To_EXT_RTC((S_BCD_Time *)&Cur_Time1);
    }
    else
      mem_cpy((void *)&Cur_Time1,(void *)&Temp_Time,sizeof(Temp_Time),(void *)&Cur_Time1,sizeof(Cur_Time1));
   
    Convert_BCD_2_HEX_Time((S_BCD_Time *)&Cur_Time1,(S_HEX_Time *)&Cur_Time0);
    
    Set_Time_To_INTER_RTC((S_BCD_Time *)&Cur_Time1);
    mem_cpy((void *)&Cur_Time_Bak,(void *)&Cur_Time1,sizeof(Cur_Time1),(void *)&Cur_Time_Bak,sizeof(Cur_Time_Bak));
    Clock_Info.Set_INTER_RTC_Flag=1;
    SET_STRUCT_SUM(Clock_Info);       
  }
  */
}
#undef CLOCK_C
