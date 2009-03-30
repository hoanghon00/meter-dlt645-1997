#define VOLSTAT_C
#include "Includes.h"

void Read_VolStat_Para_From_Rom()
{
  INT8U Err;
  INT8U Temp[12];

  if(Read_Storage_Data(0xC213,(void *)Temp,(void *)Temp,sizeof(Temp),&Err)!=12 || Err!=NO_ERR)
    DEBUG_PRINT(HUCK,DEBUG_0,"Read 0xC213 Para Err");    
  //规约电压参数为2位小数，puck提供电压参数为4位小数，因此下面要X100
  VolStat_Para.Qual_Upper_Limit=Bcd2Hex(Temp,3)*VOL_RATIO;
  VolStat_Para.Qual_Lower_Limit=Bcd2Hex(Temp+3,3)*VOL_RATIO;
  VolStat_Para.Exam_Upper_Limit=Bcd2Hex(Temp+6,3)*VOL_RATIO;
  VolStat_Para.Exam_Lower_Limit=Bcd2Hex(Temp+9,3)*VOL_RATIO;
  
  SET_STRUCT_SUM(VolStat_Para); 
}

//电压统计处理过程
void VolStat_Proc()
{
  INT8U i,Re;
  static S_Int8U Min_Bak={CHK_BYTE,0xFF,CHK_BYTE};
  static S_Int32U Sec_Bak={CHK_BYTE,0xFFFFFFFF,CHK_BYTE};
  
  MEASU_INSTANT *p;//电压是4位小数,VolStat_Para中的电压参数也已经转化为了4位小数

  Re=1;
  Re&=CHECK_STRUCT_VAR(Min_Bak);
  Re&=CHECK_STRUCT_VAR(Sec_Bak);
  
  if(Min_Bak.Var==0xFF)//第一次进入该函数，需要等1分钟后进入分钟计数
    Min_Bak.Var=Cur_Time0.Time[T_MIN];
  
  //每分钟有个结算
  if(Min_Bak.Var!=Cur_Time0.Time[T_MIN])
  {
    DIS_PD_INT;//关掉电中断
    Min_Bak.Var=Cur_Time0.Time[T_MIN];
    for(i=0;i<3;i++)//三相分别考核
    {
      if(i==1 && Get_SysParse_Mode()!=PARSE_341)//331情况下不对B相进行考核
      {
        //对B相统计情况清0
        Cur_VolStat.Total_Time[i]=0;
        Cur_VolStat.Upper_Time[i]=0;
        Cur_VolStat.Lower_Time[i]=0;
        Cur_VolStat.Qual[i]=0;
          
        Total_VolStat.Total_Time[i]=0;
        Total_VolStat.Upper_Time[i]=0;
        Total_VolStat.Lower_Time[i]=0; 
        Total_VolStat.Qual[i]=0;
        continue;
      }
      //该分钟内电压都发生在考核范围内才进行统计
      if(VolStat_Count.Exam_Upper[i]==0 && VolStat_Count.Exam_Lower[i]==0)
      {  
        //该分钟内没有发生一次电压超考核上限也没有发生一次电压超考核下限
        //这样才认为电压在考核范围内
        Cur_VolStat.Total_Time[i]++;//检测总时间
        Total_VolStat.Total_Time[i]++;//总监测时间
        
        if(0!=VolStat_Count.Qual_Upper[i] ||\
           0!=VolStat_Count.Qual_Lower[i])//电压不合格
        {
          if(VolStat_Count.Qual_Upper[i]>VolStat_Count.Qual_Lower[i])//电压超上
          {
            Cur_VolStat.Upper_Time[i]++;
            Total_VolStat.Upper_Time[i]++;
          }
          else
          {
            Cur_VolStat.Lower_Time[i]++;
            Total_VolStat.Lower_Time[i]++;
          }
        }
      }
      //计算分相当月电压合格率
      if(Cur_VolStat.Total_Time[i]!=0 &&\
         Cur_VolStat.Total_Time[i] > Cur_VolStat.Upper_Time[i] + Cur_VolStat.Lower_Time[i])
        Cur_VolStat.Qual[i]=(Cur_VolStat.Total_Time[i]-Cur_VolStat.Upper_Time[i]-Cur_VolStat.Lower_Time[i])*10000/Cur_VolStat.Total_Time[i];
      else
         Cur_VolStat.Qual[i]=0;
      //计算总电压合格率
      if(Total_VolStat.Total_Time[i]!=0 &&\
         Total_VolStat.Total_Time[i] > Total_VolStat.Upper_Time[i] + Total_VolStat.Lower_Time[i])
        Total_VolStat.Qual[i]=(Total_VolStat.Total_Time[i]-Total_VolStat.Upper_Time[i]-Total_VolStat.Lower_Time[i])*10000/Total_VolStat.Total_Time[i];
      else
        Total_VolStat.Qual[i]=0;      
    }
    

    SET_STRUCT_SUM(Cur_VolStat);
    SET_STRUCT_SUM(Total_VolStat);
    //每分钟过去将该分钟内的统计数据清0
    mem_set((void *)&VolStat_Count,0,sizeof(VolStat_Count),(void *)&VolStat_Count,sizeof(VolStat_Count));
    EN_PD_INT;//开掉电中断
  }
  
  //每秒比较一下当前电压值
  if(Sec_Bak.Var!=Sec_Timer_Pub)
  {
    Sec_Bak.Var=Sec_Timer_Pub;

    p=(MEASU_INSTANT *)&Measu_InstantData_ToPub_PUCK;//瞬时量数据
  /////////电压合格统计///////////
    //A相
   if(p->Volt.A<VolStat_Para.Exam_Upper_Limit &&\
      p->Volt.A>VolStat_Para.Exam_Lower_Limit)
    {
      if(p->Volt.A>VolStat_Para.Qual_Upper_Limit)//A相超上限
      {
        VolStat_Count.Qual_Upper[0]++;
        Event_Vol_Cur.Qual_Up_Vol[0]=p->Volt.A;
        SET_STRUCT_SUM(Event_Vol_Cur);
        Set_Event_Instant(ID_EVENT_A_VOLT_MAX);//通知事件处理
        Clr_Event_Instant(ID_EVENT_A_VOLT_MIN);
      }
      else if(p->Volt.A<VolStat_Para.Qual_Lower_Limit)//A相超下限
      {
        VolStat_Count.Qual_Lower[0]++;
        Event_Vol_Cur.Qual_Low_Vol[0]=p->Volt.A;
        SET_STRUCT_SUM(Event_Vol_Cur);
        Set_Event_Instant(ID_EVENT_A_VOLT_MIN);//通知事件处理  
        Clr_Event_Instant(ID_EVENT_A_VOLT_MAX);
      }
      else
      {
        Clr_Event_Instant(ID_EVENT_A_VOLT_MAX);//通知事件处理
        Clr_Event_Instant(ID_EVENT_A_VOLT_MIN);      
      }
    }
   else
   {
        Clr_Event_Instant(ID_EVENT_A_VOLT_MAX);//通知事件处理
        Clr_Event_Instant(ID_EVENT_A_VOLT_MIN);     
   }
    
    if(Get_SysParse_Mode()==PARSE_341)//341表才对B相进行统计
    {
       if(p->Volt.B<VolStat_Para.Exam_Upper_Limit &&\
          p->Volt.B>VolStat_Para.Exam_Lower_Limit)
      {      
        //B相
        if(p->Volt.B>VolStat_Para.Qual_Upper_Limit)//B相超上限
        {
          VolStat_Count.Qual_Upper[1]++;
          Event_Vol_Cur.Qual_Up_Vol[1]=p->Volt.B;
          SET_STRUCT_SUM(Event_Vol_Cur);
          Set_Event_Instant(ID_EVENT_B_VOLT_MAX);//通知事件处理
          Clr_Event_Instant(ID_EVENT_B_VOLT_MIN);//通知事件处理
        }
        else if(p->Volt.B<VolStat_Para.Qual_Lower_Limit)//B相超下限
        {
          VolStat_Count.Qual_Lower[1]++;
          Event_Vol_Cur.Qual_Low_Vol[1]=p->Volt.B;
          SET_STRUCT_SUM(Event_Vol_Cur);
          Set_Event_Instant(ID_EVENT_B_VOLT_MIN);//通知事件处理
          Clr_Event_Instant(ID_EVENT_B_VOLT_MAX);//通知事件处理
        }
        else
        {
          Clr_Event_Instant(ID_EVENT_B_VOLT_MIN);//通知事件处理
          Clr_Event_Instant(ID_EVENT_B_VOLT_MAX);//通知事件处理      
        }
      }
      else
      {
          Clr_Event_Instant(ID_EVENT_B_VOLT_MIN);//通知事件处理
          Clr_Event_Instant(ID_EVENT_B_VOLT_MAX);//通知事件处理        
      }
    }
    //C相
    if(p->Volt.B<VolStat_Para.Exam_Upper_Limit &&\
        p->Volt.B>VolStat_Para.Exam_Lower_Limit)
    {   
      if(p->Volt.C>VolStat_Para.Qual_Upper_Limit)//C相超上限
      {
        VolStat_Count.Qual_Upper[2]++;
        Event_Vol_Cur.Qual_Up_Vol[2]=p->Volt.C;
        SET_STRUCT_SUM(Event_Vol_Cur);
        Set_Event_Instant(ID_EVENT_C_VOLT_MAX);//通知事件处理
        Clr_Event_Instant(ID_EVENT_C_VOLT_MIN);//通知事件处理      
      }
      else if(p->Volt.C<VolStat_Para.Qual_Lower_Limit)//C相超下限
      {
        VolStat_Count.Qual_Lower[2]++;
        Event_Vol_Cur.Qual_Low_Vol[2]=p->Volt.C;
        SET_STRUCT_SUM(Event_Vol_Cur);
        Set_Event_Instant(ID_EVENT_C_VOLT_MIN);//通知事件处理
        Clr_Event_Instant(ID_EVENT_C_VOLT_MAX);//通知事件处理
      }
      else
      {
        Clr_Event_Instant(ID_EVENT_C_VOLT_MIN);//通知事件处理
        Clr_Event_Instant(ID_EVENT_C_VOLT_MAX);//通知事件处理     
      }
    }
    else
    {
        Clr_Event_Instant(ID_EVENT_C_VOLT_MIN);//通知事件处理
        Clr_Event_Instant(ID_EVENT_C_VOLT_MAX);//通知事件处理      
    }
    
  /////////电压考核统计///////////////
    if(p->Volt.A>VolStat_Para.Exam_Upper_Limit)
      VolStat_Count.Exam_Upper[0]++;
    else if(p->Volt.A<VolStat_Para.Exam_Lower_Limit)
      VolStat_Count.Exam_Lower[0]++;
    
    if(p->Volt.B>VolStat_Para.Exam_Upper_Limit)
      VolStat_Count.Exam_Upper[1]++;
    else if(p->Volt.B<VolStat_Para.Exam_Lower_Limit)
      VolStat_Count.Exam_Lower[1]++;
    
    if(p->Volt.C>VolStat_Para.Exam_Upper_Limit)
      VolStat_Count.Exam_Upper[2]++;
    else if(p->Volt.C<VolStat_Para.Exam_Lower_Limit)
      VolStat_Count.Exam_Lower[2]++;  
   } 
}

//请当月电压统计数据
void Set_Def_Cur_VolStat_Data()
{
  //if(Check_Meter_Factory_Status()==0)//当前不是工厂状态则推出
    //return;
  //将初始需量写入存储器中  
  DEBUG_PRINT(HUCK,DEBUG_0,"Set Def VolStat Data");
  mem_set((void *)&Cur_VolStat,0,sizeof(Cur_VolStat),(void *)&Cur_VolStat,sizeof(Cur_VolStat));
 
  INIT_STRUCT_VAR(Cur_VolStat);
  SET_DATA_READY_FLAG(Cur_VolStat);   
  SET_STRUCT_SUM(Cur_VolStat);
  
  mem_set((void *)&Total_VolStat,0,sizeof(Total_VolStat),(void *)&Total_VolStat,sizeof(Total_VolStat));
  
  INIT_STRUCT_VAR(Total_VolStat);
  SET_DATA_READY_FLAG(Total_VolStat); 
  SET_STRUCT_SUM(Total_VolStat); 
  
  if(Check_Meter_Factory_Status()==0)//当前不是工厂状态则推出
    return;  
  Save_Cur_VolStat_Data();
  Save_Cur_VolStat_PD_Data();  
  
}

//设置默认的电压统计数据
void Set_Def_VolStat_Data()
{
  INT8U i;
  Set_Def_Cur_VolStat_Data();
  
  for(i=0;i<HIS_VOLSTAT_DATA_MONTHS;i++)
  {
    Write_Storage_Data(HIS_VOLSTAT+i,(void *)&Cur_VolStat,sizeof(Cur_VolStat)); 
  }
}

//保存当前电压统计数据
void Save_Cur_VolStat_Data()
{  
  if(CHECK_STRUCT_SUM(Cur_VolStat))
    Write_Storage_Data(CUR_VOLSTAT,(void *)(&Cur_VolStat),sizeof(Cur_VolStat));
  else
    ASSERT(A_WARNING,0);
  
  OS_TimeDly_Ms(100);
  if(CHECK_STRUCT_SUM(Total_VolStat))
    Write_Storage_Data(TOTAL_VOLSTAT,(void *)(&Total_VolStat),sizeof(Total_VolStat));
  else
    ASSERT(A_WARNING,0);
  
  OS_TimeDly_Ms(100);    
}

//保存当前电压统计数据
void Save_Cur_VolStat_PD_Data()
{ 
  if(Check_VolStat_Data((S_VolStat *)&Cur_VolStat))
    Write_Storage_Data(CUR_VOLSTAT_PD,(void *)(&Cur_VolStat),sizeof(Cur_VolStat));
  else
    ASSERT(A_WARNING,0);
  
  if(Check_VolStat_Data((S_VolStat *)&Total_VolStat))
    Write_Storage_Data(TOTAL_VOLSTAT_PD,(void *)(&Total_VolStat),sizeof(Total_VolStat));
  else
    ASSERT(A_WARNING,0);
}

//保存当前电压统计数据
//此函数因为是掉电存储，因此没有延时函数
void PowerDown_Save_VolStat_Data()
{  
  Write_Storage_Data(CUR_VOLSTAT_PD,(void *)(&Cur_VolStat),sizeof(Cur_VolStat));
  Write_Storage_Data(TOTAL_VOLSTAT_PD,(void *)(&Total_VolStat),sizeof(Total_VolStat));
}

//检查电压统计数据是否正确
INT8U Check_VolStat_Data(S_VolStat *pVolStat)
{
  INT8U Re;
  
  Re=1;
  Re&=(pVolStat->Head==CHK_BYTE && pVolStat->Tail==CHK_BYTE)?1:0;  
  Re&=Check_STRUCT_Sum(pVolStat,sizeof(S_VolStat),pVolStat->CS,sizeof(pVolStat->CS));
  Re&=(pVolStat->Data_Ready_Flag==DATA_READY)?1:0;
  if(ASSERT(A_WARNING,0!=Re))
    return 0;  
  return Re;
}

//从ROM中读取电压统计数据
void Read_VolStat_Data_From_Rom()
{
  INT8U Err;
  S_VolStat *p;
  
  //先读取掉电存储区数据
  //再读普通数据
  //读当前电压检测数据
  OS_Sem_Pend(PUB_BUF0_SEM_ID); 
  p=(S_VolStat *)Pub_Buf0;
  
  if(Read_Storage_Data(CUR_VOLSTAT_PD,p,p,sizeof(Pub_Buf0),&Err)\
    !=sizeof(Cur_VolStat) || Check_VolStat_Data(p)!=1)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Read CUR_VOLSTAT_PD data error");
    OS_TimeDly_Ms(500);
    if(Read_Storage_Data(CUR_VOLSTAT,p,p,sizeof(Pub_Buf0),&Err)\
      !=sizeof(Cur_VolStat) || Check_VolStat_Data(p)!=1)//该参数有默认参数项,实际上不会进入下面的括号
    {
        DEBUG_PRINT(HUCK,DEBUG_0,"Read CUR_VOLSTAT data error");
        //如果所有的备份区数据都没读出来则认为是第一次上电，电量清0
        mem_cpy(p,(void *)Def_VolStat,sizeof(Cur_VolStat),p,sizeof(Pub_Buf0));
    }
  }
  DIS_PD_INT;
  mem_cpy((void *)&Cur_VolStat,p,sizeof(Cur_VolStat),(void *)&Cur_VolStat,sizeof(Cur_VolStat));
  INIT_STRUCT_VAR(Cur_VolStat);
  SET_DATA_READY_FLAG(Cur_VolStat);
  SET_STRUCT_SUM(Cur_VolStat);
  EN_PD_INT;
  //读总电压检测数据
  if(Read_Storage_Data(TOTAL_VOLSTAT_PD,p,p,sizeof(Total_VolStat),&Err)\
    !=sizeof(Total_VolStat) || Check_VolStat_Data(p)!=1)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Read TOTAL_VOLSTAT_PD data error");
    OS_TimeDly_Ms(500);
    if(Read_Storage_Data(TOTAL_VOLSTAT,p,p,sizeof(Total_VolStat),&Err)\
      !=sizeof(Total_VolStat) || Check_VolStat_Data(p)!=1)//该参数有默认参数项,实际上不会进入下面的括号
    {
        DEBUG_PRINT(HUCK,DEBUG_0,"Read TOTAL_VOLSTAT data error");
        //如果所有的备份区数据都没读出来则认为是第一次上电，电量清0
        mem_cpy(p,(void *)Def_VolStat,sizeof(Total_VolStat),p,sizeof(Pub_Buf0));
        //SET_STRUCT_SUM(Total_VolStat);         
    }
  } 
  DIS_PD_INT;
  mem_cpy((void *)&Total_VolStat,p,sizeof(Total_VolStat),(void *)&Total_VolStat,sizeof(Total_VolStat));
  INIT_STRUCT_VAR(Total_VolStat);
  SET_DATA_READY_FLAG(Total_VolStat);
  SET_STRUCT_SUM(Total_VolStat);
  EN_PD_INT;  
  
  OS_Sem_Post(PUB_BUF0_SEM_ID);
}

//将当前电压统计数据冻结，pTime传入冻结时刻，一般就是当前时间
void Freeze_VolStat_Data(S_HEX_Time *pTime)
{
  INT8U Re;
  S_HEX_Time TempTime;
  INT8U Err;
  
  DEBUG_PRINT(HUCK,DEBUG_0,"----------Freeze VolStat Data %d-%d-%d %d:%d----------",\
                           pTime->Time[T_YEAR],pTime->Time[T_MONTH],pTime->Time[T_DATE],pTime->Time[T_HOUR],pTime->Time[T_MIN]);
  
  DEBUG_PRINT(HUCK,DEBUG_0,"VolStat Data Freeze, Data Time: ");
  Debug_Print_HEX_Time(Cur_VolStat.Time.Time);
      
  if((Cur_VolStat.Time.Time[T_YEAR]==0 && Cur_VolStat.Time.Time[T_MONTH]==0) ||\
     Check_HEX_Time((S_HEX_Time *)&(Cur_VolStat.Time))==0)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"----------Freeze VolStat Data End----------");
    return;
  }
  //<=pTime的最近一个冻结时间点
  Get_Last_Freeze_Time(pTime,&TempTime);//pTime以前的最近一个冻结时间点,包括pTime本身
  //现在TempTime就是最近一次的冻结时间点
  Re=Cmp_Time(&(Cur_VolStat.Time),&TempTime);//比较数据冻结时间点与最近一次冻结时间点
  if(TIME_AFT==Re)//需量时间点在最近一次结算时间点之后，是一条当前数据，不需要补存到历史数据中
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"VolStat Time > FreezeTime");
    Debug_Print_HEX_Time(Cur_VolStat.Time.Time);
    Debug_Print_HEX_Time(TempTime.Time);    
    DEBUG_PRINT(HUCK,DEBUG_0,"----------Freeze VolStat Data End----------");
    return;
  }
  //是一条历史数据
  //>=p->Time的最近一个冻结时间点
  Get_Next_Freeze_Time(&(Cur_VolStat.Time),&TempTime);//历史数据的冻结时间点 
  OS_Sem_Pend(PUB_BUF0_SEM_ID);//申请Pub_Buf0信号量
  //该月没有冻结数据则进行冻结
  if(Read_HIS_Data(HIS_VOLSTAT_DATA,&TempTime,(void *)Pub_Buf0,(void *)Pub_Buf0,sizeof(Pub_Buf0),&Err) EQ sizeof(S_VolStat))
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"VolStat Time Data Freezed!");
    Debug_Print_HEX_Time(TempTime.Time); 
  }
  else
  {
    //mem_cpy((void *)&Cur_VolStat.Time,&TempTime,sizeof(TempTime),(void *)&Cur_VolStat,sizeof(Cur_VolStat));
    SET_STRUCT_TIME(Cur_VolStat,TempTime);
    Write_HIS_Data(HIS_VOLSTAT_DATA,(S_HEX_Time *)&(Cur_VolStat.Time),(void *)&Cur_VolStat,sizeof(Cur_VolStat));
  }
  
  OS_Sem_Post(PUB_BUF0_SEM_ID);//释放Pub_Buf0信号量  
  //因为是历史数据，所以需要重置变为当月数据
  //清当月电压统计数据
  mem_set((void *)&Cur_VolStat,0,sizeof(Cur_VolStat),(void *)&Cur_VolStat,sizeof(Cur_VolStat));//将数据全部清0存入当前存储区，下次读出全0数据，表示尚未生成新数据

  INIT_STRUCT_VAR(Cur_VolStat);
  SET_DATA_READY_FLAG(Cur_VolStat);   
  SET_STRUCT_SUM(Cur_VolStat);
  
  //更新存储区数据  
  Save_Cur_VolStat_PD_Data();
  Save_Cur_VolStat_Data();
  DEBUG_PRINT(HUCK,DEBUG_0,"----------Freeze VolStat Data End----------");
}

//上电补冻电压统计数据
void PowerOn_VolStat_Data_Freeze()
{
  Freeze_VolStat_Data((S_HEX_Time *)&Cur_Time0);
}

//检查电压统计相关参数的合法性
void Check_VolStat_Para_Avail()
{
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(VolStat_Para);  
  if(0==Re)
  {
    ASSERT(A_WARNING,0); 
    Read_VolStat_Para_From_Rom();
  }
}

//检查电压统计相关数据的合法性
void Check_VolStat_Data_Avail()
{
  INT8U Re;
  
  Re=1;
  Re&=CHECK_STRUCT_SUM(Cur_VolStat);
  Re&=CHECK_STRUCT_SUM(Total_VolStat);
  if(0==Re)
  {
    ASSERT(A_WARNING,0);
    Read_VolStat_Data_From_Rom();
  }
  
}

//清电压统计数据
void Clear_VolStat_Data()
{ 
  if(Check_Clear_Data_Authority()==0)
    return;
  
  if(CLEAR_VOLSTAT_FLAG!=Check_Clear_Data_Flag() &&
     CLEAR_ALL_FLAG!=Check_Clear_Data_Flag())
    return;
  DEBUG_PRINT(HUCK,DEBUG_0,"Clear_VolStat_Data!!!");  
  Set_Def_VolStat_Data();
  /*
  INT8U i;
  S_HEX_Time Time;
  INT8U Re;
  
  DEBUG_PRINT(HUCK,DEBUG_0,"Clear_VolStat_Data!!!");
  //清电量,检查是否在编程状态，以及用户权限是否够
  if(Check_Meter_Prog_Status()==0 ||\
     CHECK_STRUCT_SUM(User_Authority_Flag)==0 ||\
    User_Authority_Flag.Flag!=AUTHORITY_FLAG)
    return;
  
  if(CLEAR_VOLSTAT_FLAG!=Check_Clear_Data_Flag())
    return;
  
  //清内存中变量
  mem_set((void *)&Cur_VolStat,0,sizeof(Cur_VolStat),(void *)&Cur_VolStat,sizeof(Cur_VolStat));
  SET_STRUCT_SUM(Cur_VolStat);
  mem_set((void *)&Total_VolStat,0,sizeof(Total_VolStat),(void *)&Total_VolStat,sizeof(Total_VolStat));
  SET_STRUCT_SUM(Total_VolStat);

  
  //清电量,检查是否在编程状态，以及用户权限是否够
  if(Check_Meter_Prog_Status()==0 ||\
     CHECK_STRUCT_SUM(User_Authority_Flag)==0 ||\
    User_Authority_Flag.Flag!=AUTHORITY_FLAG)
    return;
  
  //检查清楚标志是否合法
  if(CLEAR_VOLSTAT_FLAG!=Check_Clear_Data_Flag())
    return;
  //存当月数据
  Write_Storage_Data(CUR_VOLSTAT_PD,(void *)(&Cur_VolStat),sizeof(Cur_VolStat));
  Write_Storage_Data(CUR_VOLSTAT,(void *)(&Cur_VolStat),sizeof(Cur_VolStat));
  //存总数据
  Write_Storage_Data(TOTAL_VOLSTAT_PD,(void *)(&Total_VolStat),sizeof(Total_VolStat));
  Write_Storage_Data(TOTAL_VOLSTAT,(void *)(&Total_VolStat),sizeof(Total_VolStat));
  
  Re=CHECK_STRUCT_SUM(Cur_Time0);
  if(ASSERT(A_WARNING,1==Re))
    Check_Data_Avail();
  
  //将前0-HIS_VOLSTAT_DATA_MONTHS个月的统计数据全部清0
  mem_cpy((void *)&Time,(void *)&Cur_Time0,sizeof(Time),(void *)&Time,sizeof(Time));//获取当前时间
  Re=CHECK_STRUCT_SUM(Data_Freeze_Time);
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();
  
  Time.Time[T_HOUR]=Data_Freeze_Time.DDHH[0];
  Time.Time[T_DATE]=Data_Freeze_Time.DDHH[1];
  SET_STRUCT_SUM(Time);
  //将前0-HIS_VOLSTAT_DATA_MONTHS个月的统计数据全部清0
  if(CLEAR_VOLSTAT_FLAG!=Check_Clear_Data_Flag())
    return;
  
  Re=1;
  Re&=CHECK_STRUCT_SUM(User_Authority_Flag);
  Re&=CHECK_STRUCT_SUM(Clear_Flag);
  for(i=0;i<=HIS_VOLSTAT_DATA_MONTHS;i++)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Clear Freeze VolStat Data,Time:%d-%d-%d %d:%d",Time.Time[T_YEAR],Time.Time[T_MONTH],Time.Time[T_DATE],Time.Time[T_HOUR],Time.Time[T_MIN]);
    //严格判断，防止程序乱跑到这里
    if(Check_Meter_Prog_Status()==0 || 1!=Re ||\
       AUTHORITY_FLAG!=User_Authority_Flag.Flag ||\
       CLEAR_DEMAND_FLAG!=Clear_Flag.Flag)
      return;
    Freeze_VolStat_Data(&Time);
    Time_Dec((S_HEX_Time *)&Time,1,UNIT_MON,(S_HEX_Time *)&Time);
  } 
  */
}

//初始化电压统计参数
void Init_VolStat_Ram_Para()
{
  mem_set((void *)&VolStat_Count,0,sizeof(VolStat_Count),(void *)&VolStat_Count,sizeof(VolStat_Count));
  SET_STRUCT_SUM(VolStat_Count);
  
}
#undef VOLSTAT_C

