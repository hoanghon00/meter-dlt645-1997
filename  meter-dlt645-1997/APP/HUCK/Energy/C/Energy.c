#define ENERGY_C
#include "Includes.h"

volatile MEASU_ENERG Cumu_Energy;
extern void Event_Energy_Cumu(MEASU_ENERG *pEnergy);

//表征内存是否掉过电，掉过返回POWER_ON否则返回RESET
//检查当前是上电还是复位,实际上该函数是判断内存中的数据在复位后是否有变化，即是否发生了掉电之类
//上电返回POWER_ON,复位返回RESET
//该函数检查Power_On_Flag.Flag[]是不是每个都是0x55
INT8U Check_Rst_Flag()
{
  INT8U i,Re;

  static S_Int8U Flag={CHK_BYTE,0x55,CHK_BYTE};//0x55表示第一次进入该函数,0xAA表示不是第一次进入,其他值表示错误,记录是否是上电后第一次进入该函数
  static S_Int8U Power_Flag={CHK_BYTE,SOFT_RST,CHK_BYTE};//复位或上电标志
  
  Re=CHECK_STRUCT_VAR(Flag);
  ASSERT(A_WARNING,1==Re);
  
  if(0x55==Flag.Var)//第一次进入该函数
  {
    Flag.Var=0xAA;//置0xAA，以后再调用该函数就不会再进入该流程了
    Power_Flag.Var=SOFT_RST;//默认是复位
    for(i=0;i<S_NUM(Power_On_Flag.Flag);i++)
    {
      if(Power_On_Flag.Flag[i]!=0x55)//有一个不是0x55说明是上电
      {
        Power_Flag.Var=POWER_ON_RST;
        break;
      }
    }
    //Power_Flag==RESET而且Power_On_Flag的校验和正确,认为是软复位
    if(Power_Flag.Var==SOFT_RST && CHECK_STRUCT_SUM(Power_On_Flag))
      Power_Flag.Var=SOFT_RST;
    else
      Power_Flag.Var=POWER_ON_RST;
    
    //Power_On_Flag.Flag都置成0x55，下次复位或上电可查看该标志
    mem_set((void *)Power_On_Flag.Flag,0x55,sizeof(Power_On_Flag.Flag),(void *)Power_On_Flag.Flag,sizeof(Power_On_Flag.Flag));
    SET_STRUCT_SUM(Power_On_Flag);
    
    return Power_Flag.Var; 
  }
  else if(0xAA==Flag.Var)
  {
    Re=CHECK_STRUCT_VAR(Power_Flag);
    ASSERT(A_WARNING,1==Re);
    
    if(Power_Flag.Var==SOFT_RST || Power_Flag.Var==POWER_ON_RST)
      return Power_Flag.Var;
    else
    {
      ASSERT(A_WARNING,0);
      return POWER_ON_RST;
    }
  }
  else
  {
    ASSERT(A_WARNING,0);
    return POWER_ON_RST;
  }
}

//获取BCD时间
//Dst[0]分
//Dst[1]时
//Dst[2]日
//Dst[3]月
//Dst[4]年
void Get_BCD_Time(S_BCD_Time *pSrc,INT8U Dst[],INT8U Dst_Start[],INT8U DstLen)
{
  ASSERT(A_WARNING,Dst>=Dst_Start && Dst+5<=Dst_Start+DstLen);
  
  Dst[T_MIN]=pSrc->Min;
  Dst[T_HOUR]=pSrc->Hour;
  Dst[T_DATE]=pSrc->Date;
  Dst[T_MONTH]=pSrc->Month;
  Dst[T_YEAR]=pSrc->Year;  
}

//检查电量参数的合法性
void Check_Energy_Para_Avil()
{
  INT8U Re;
  
  Re=1;
  Re&=CHECK_STRUCT_SUM(Mode_Word);
  Re&=CHECK_STRUCT_SUM(Rate_Status);
  Re&=CHECK_STRUCT_SUM(Rate_Scheme_Switch_Time);
  Re&=CHECK_STRUCT_SUM(Multi_Rate_Para);
  Re&=CHECK_STRUCT_SUM(Year_Table);
  Re&=CHECK_STRUCT_SUM(Date_Table);
  Re&=CHECK_STRUCT_SUM(Data_Freeze_Time);
  Re&=CHECK_STRUCT_SUM(Cur_Rate_Info);

  if(ASSERT(A_WARNING,1==Re))
    Read_Energy_Para_From_Rom();//重新读取电量参数!!
}

//任务中查询重要数据的校验和不对时，调用此函数
void Check_Data_Avail()
{
  if(Get_Cur_Task_ID()!=0)//非主任务下才请求数据检测!
  {  
    Chk_Data_Flag.Chk_Flag=1;//通知主任务进行一次数据检查操作
  
    OS_Waitfor_Sec(Chk_Data_Flag.Chk_Flag==0,5);
    if(Chk_Data_Flag.Chk_Flag==1)
    {
      ASSERT(A_WARNING,0);
      OS_Waitfor(Chk_Data_Flag.Chk_Flag==0);
    }
  }
}
//检查数据与参数的主流程
void Main_Chk_Data_Avail()
{
  if(Chk_Data_Flag.Chk_Flag==1)
  {
    Check_Energy_Demand_Task_Data_Avail();
    Chk_Data_Flag.Chk_Flag=0; 
  }
}

//检查所有数据的可用性
void Check_Energy_Demand_Task_Data_Avail()
{
  INT8U Re;
  static S_Int8U Flag={CHK_BYTE,0,CHK_BYTE};
  
  Re=CHECK_STRUCT_VAR(Flag);
  ASSERT(A_WARNING,1==Re);
  
  if(Flag.Var==1)//防止下面的函数里面在调用Check_Data_Avail(),从而导致嵌套
    return;
  
  Flag.Var=1;
  //下面的函数最好不要再别的流程中调用，防止发生嵌套调用
  ASSERT(A_WARNING,PD_INT_EN_Flag==1);
  ASSERT(A_WARNING,CHECK_STRUCT_VAR(Assert_Info));
  
  Check_Energy_Data_Avail();//检查当前电量需量数据是否合法
  //OS_TimeDly_Ms(20);
  Check_Energy_Para_Avil();//检查电量相关参数是否合法
  //OS_TimeDly_Ms(20);
  Check_Demand_Data_Avail();//检查需量数据是否合法
  OS_TimeDly_Ms(20);
  Check_Demand_Para_Avail();//检查需量参数是否合法
  //OS_TimeDly_Ms(20);
  Check_Clock_Data_Avail();//检查时钟数据是否有效
  //OS_TimeDly_Ms(20);
  Check_LoadData_Index_Avail();//检查负荷曲线索引信息是否合法
  OS_TimeDly_Ms(20);
  Check_LoadData_Para_Avail();//检查负荷曲线参数是否合法
  //OS_TimeDly_Ms(20);
  Check_Power_Ctrl_Para_Avail();//检查负荷控制相关参数是否正确
  //OS_TimeDly_Ms(20);
  Check_Sys_Status_Avail();//检查当前系统状态是否合法
  OS_TimeDly_Ms(20);
  Check_Pub_Var_Avail();//检查全局buf或者变量的是否合法
  //OS_TimeDly_Ms(20);
  Check_Event_Cumu_Avail();//检查事件累加数据是否正确
  //OS_TimeDly_Ms(20);
  Check_Data_Trans_Data_Avail();//检查协议传输相关数据是否正确
  OS_TimeDly_Ms(20);
  Check_Low_Power_Data_Avail();//检查低功耗相关变量是否正确
  //OS_TimeDly_Ms(20);
  Check_VolStat_Para_Avail();//检查电压统计参数的合法性
  //OS_TimeDly_Ms(20);
  Check_VolStat_Data_Avail();//检查电压统计数据的合法性
  Flag.Var=0;
  return;
}

//从ROM空间中读取模式字
void Read_Mode_Word()
{
  INT8U i,Err;
  INT16U Len;
  PROTO_DI DI=0xC047;
  
  //读取模式字1-8
  for(i=0;i<8;i++)
  {
    //如果读取模式字失败了则采用默认的模式字
    Len=Read_Storage_Data(DI+i,(INT8U *)&Mode_Word.Mode[i],(INT8U *)&Mode_Word.Mode[i],1,&Err);
    ASSERT(A_WARNING,1==Len && NO_ERR==Err);
  }
  //模式字9暂未用
/*  
  //读取模式字9
  Len=Read_Storage_Data(0xCA1A,(INT8U *)&Mode_Word.Mode[8],(INT8U *)&Mode_Word.Mode[8],1,&Err);
  ASSERT(A_WARNING,1==Len && NO_ERR==Err);  
*/
  SET_STRUCT_SUM(Mode_Word);

  //读取信号输出控制字
  //Len=Read_Storage_Data(0xCA12,(INT8U *)&Output_Mode.Mode,(INT8U *)&Output_Mode.Mode,1,&Err);
  //ASSERT(A_WARNING,1==Len && NO_ERR==Err); 
  
  //Output_Mode.Mode=(Output_Mode.Mode & 0x03);//4和1信号输出模式字只有最低三位有意义
  //SET_STRUCT_SUM(Output_Mode);
}

//获取当前费率方案,
//返回RATE_SCHEME0表示使用主费率方案,返回RATE_SCHEME1表示当前使用副费率方案
INT8U Get_Using_Rate_Scheme()
{
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(Rate_Status);
  //检查费率状态字校验和   
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();
  //费率方案错，默认为方案0
  if(ASSERT(A_WARNING,RATE_SCHEME0==Rate_Status.Status[0].Byte ||\
    RATE_SCHEME1==Rate_Status.Status[0].Byte))
    return RATE_SCHEME0;
  else
    return Rate_Status.Status[0].Byte;
}

//设置当前费率方案
//将当前费率方案信息设置到当前费率状态字中
void Set_Using_Rate_Scheme(INT8U Rate_Scheme)
{
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(Rate_Status);
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();
  
  if(ASSERT(A_WARNING,RATE_SCHEME0==Rate_Scheme ||\
     RATE_SCHEME1==Rate_Scheme))
    Rate_Status.Status[0].Byte=RATE_SCHEME0;
  else
    Rate_Status.Status[0].Byte=Rate_Scheme;
  
  SET_STRUCT_SUM(Rate_Status);//重算校验和  
}

//从当前费率状态字中读取当前的费率，如果当前费率不在默认范围内则返回费率1
//主要是设置费率状态字
INT8U Get_Using_Rate()
{
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(Rate_Status);
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();
  
  if(ASSERT(A_WARNING,Rate_Status.Status[1].Byte<=MAX_RATES &&\
    Rate_Status.Status[1].Byte>=1))
  {
    Rate_Status.Status[1].Byte=1;//如果当前费率错，默认使用费率1
    SET_STRUCT_SUM(Rate_Status);
    return 1;
  }
  else
    return Rate_Status.Status[1].Byte;  
}

//设置当前费率为Rate,主要是设置费率状态字
void Set_Using_Rate(INT8U Rate)
{
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(Rate_Status);
  ASSERT(A_WARNING,0!=Re);
  if(ASSERT(A_WARNING,Rate>=1 && Rate<=MAX_RATES))
    Rate_Status.Status[1].Byte=1;
  else
    Rate_Status.Status[1].Byte=Rate;
  
  SET_STRUCT_SUM(Rate_Status);//重算校验和  
}

//计算当前应该使用的费率方案,如果Flag==CALC_ROM则重新从ROM中读取参数计算但前的费率方案
//同时如果费率方按发生了切换，则函数内部进行切换相关的处理
// 当前应该采用主费率方案则返回RATE_SCHEME0,否则返回RATE_SCHEME1
//该函数的主要作用是判断是否产生费率切换
//费率方案切换的话调用Rate_Scheme_Changed_Proc进行相关处理
INT8U Calc_Cur_Rate_Scheme(INT8U Flag)
{
  INT8U Re,Scheme,Err;
  INT16U Len;
  
  //读取主副套费率切换时刻
  if(CALC_ROM==Flag)//表示从ROM中重新读取相关数据，进行计算
  {
    //读取模式字
    Read_Mode_Word();
    //读取主负费率切换时刻
    Len=Read_Storage_Data(0xCA13,(INT8U *)Rate_Scheme_Switch_Time.Time,(INT8U *)Rate_Scheme_Switch_Time.Time,sizeof(Rate_Scheme_Switch_Time.Time),&Err);
    if(ASSERT(A_WARNING,5==Len))
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Read Rate_Scheme_Switch_Time Error"); 
      //清成0xFF，不使用第二套费率方案
      mem_set((INT8U *)Rate_Scheme_Switch_Time.Time,0xFF,sizeof(Rate_Scheme_Switch_Time.Time),\
            (INT8U *)Rate_Scheme_Switch_Time.Time,sizeof(Rate_Scheme_Switch_Time.Time));
    }
    if(Check_BCD_Data((INT8U *)Rate_Scheme_Switch_Time.Time,5)==1)//确实是BCD时间数据,否则可能是全FF(默认参数)
      Bcd2Hex_Buf((INT8U *)Rate_Scheme_Switch_Time.Time,sizeof(Rate_Scheme_Switch_Time.Time),(INT8U *)Rate_Scheme_Switch_Time.Time,(INT8U *)Rate_Scheme_Switch_Time.Time,sizeof(Rate_Scheme_Switch_Time.Time));
    else
      mem_set((INT8U *)Rate_Scheme_Switch_Time.Time,0xFF,sizeof(Rate_Scheme_Switch_Time.Time),\
            (INT8U *)Rate_Scheme_Switch_Time.Time,sizeof(Rate_Scheme_Switch_Time.Time));
      
    SET_STRUCT_SUM(Rate_Scheme_Switch_Time); 
  }

  //检查内存中相关变量的正确性
  Re=1;
  Re&=Check_HEX_Time((S_HEX_Time *)&Cur_Time0);//检查Cur_Time0时间
  Re&=CHECK_STRUCT_SUM(Mode_Word);//检查模式字是否允许费率切换
  Re&=CHECK_STRUCT_SUM(Rate_Scheme_Switch_Time);//检查费率切换时间
  Re&=CHECK_STRUCT_SUM(Cur_Rate_Info);//检查当前费率信息
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();
  
  if(Mode_Word.Mode[2].Bit.Bit5==1)//是否允许切换费率方案?
  {
    if(_Check_HEX_Time((INT8U *)&Rate_Scheme_Switch_Time.Time))
    {
      Re=_Cmp_Time((S_HEX_Time *)&Cur_Time0,(S_HEX_Time *)&Rate_Scheme_Switch_Time);//当前时间是否在费率切换时间之后?
      if(Re!=TIME_BEF)//当前时间在费率方案切换时间之后
        Scheme=RATE_SCHEME1;//当前应采用副费率方案
      else
        Scheme=RATE_SCHEME0;
    }
    else
      Scheme=RATE_SCHEME0;
  }
  else//不允许的情况下，默认使用套费率方案0
    Scheme=RATE_SCHEME0;
  
  //和当前使用的费率方案不一致，应该切换费率方案
  //使用ROM中参数计算费率方案时，强制认为发生了一次费率切换,以免出错
  if(Cur_Rate_Info.Scheme!=Scheme || CALC_ROM==Flag)
  {
    Cur_Rate_Info.Scheme=Scheme;
    SET_STRUCT_SUM(Cur_Rate_Info); 
    Rate_Scheme_Changed_Proc();//费率方案切换处理
  }
  return Scheme;
}
/*
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
*/

//检查复费率参数
void Check_Multi_Rate_Para()
{
  if(ASSERT(A_WARNING,Multi_Rate_Para.Year_Period<=MAX_YEAR_PERIODS))
  {
    Multi_Rate_Para.Year_Period=MAX_YEAR_PERIODS; 
    SET_STRUCT_SUM(Multi_Rate_Para);
  }
  if(ASSERT(A_WARNING,Multi_Rate_Para.Date_Table<=MAX_DATE_TABLES))
  {
    Multi_Rate_Para.Date_Table=MAX_DATE_TABLES; 
    SET_STRUCT_SUM(Multi_Rate_Para);    
  }
  if(ASSERT(A_WARNING,Multi_Rate_Para.Date_Period<=MAX_DATE_PERIODS))
  {
    Multi_Rate_Para.Date_Table=MAX_DATE_PERIODS; 
    SET_STRUCT_SUM(Multi_Rate_Para);    
  }
  if(ASSERT(A_WARNING,Multi_Rate_Para.Holidays<=MAX_YEAR_HOLIDAYS))
  {
    Multi_Rate_Para.Holidays=MAX_YEAR_HOLIDAYS; 
    SET_STRUCT_SUM(Multi_Rate_Para);      
  }
}


//获取多费率相关参数
//Rate_Scheme表示费率方案号,为RATE_SCHEME0或者RATE_SCHEME1
void Read_Multi_Rate_Para(INT8U Rate_Scheme)
{
  INT16U i,Len;
  INT8U Err;
  
  if(ASSERT(A_WARNING,RATE_SCHEME0==Rate_Scheme ||\
                      RATE_SCHEME1==Rate_Scheme))
    Rate_Scheme=RATE_SCHEME0;
  
  if(RATE_SCHEME0==Rate_Scheme)//使用第一套费率方案
    i=0;
  else
    i=0x0800;//第二套方案参数从CB1F开始,是第一套费率参数标识符+0x0800
  
  //第一套费率方案参数
  Len=S_OFF(S_Multi_Rate_Para,CS);
  if(Get_DLT645_Data(0xC31F+i,(void *)&Multi_Rate_Para,(void *)&Multi_Rate_Para,Len,&Err)!=Len ||\
    Err!=NO_ERR)
  {
     DEBUG_PRINT(HUCK,DEBUG_0,"Read 0xC31F Len Error"); 
  }
  
  Len=S_OFF(S_Multi_Rate_Para,WeekRest_Status);//周休日状态字前的数据要BCD->HEX
  Bcd2Hex_Buf((void *)&Multi_Rate_Para,Len,(void *)&Multi_Rate_Para,(void *)&Multi_Rate_Para,Len);
  SET_STRUCT_SUM(Multi_Rate_Para);
  Check_Multi_Rate_Para();//检查复费率参数的合法性
  
  //年时区表
  if(Read_Storage_Data(0xC32F+i,(void *)Year_Table.MDN,(void *)Year_Table.MDN,MAX_YEAR_PERIODS*3,&Err)!=MAX_YEAR_PERIODS*3 ||\
    Err!=NO_ERR)
    DEBUG_PRINT(HUCK,DEBUG_0,"Read 0xC32F Len Error"); 

  Len=S_OFF(S_Year_Table,CS);
  Bcd2Hex_Buf((void *)&Year_Table,Len,(void *)&Year_Table,(void *)&Year_Table,Len);
  SET_STRUCT_SUM(Year_Table); 
}

//用于计算两个时间的时间差日数,年时区时间差,返回结果为日数
//用于计算年时区表中两项的时间差,Time0-Time1，该函数用于在年时区表中寻找当前时间的时段
//如果Time0本省在Time1之前，那么结果是Time0+12*31-Time1
//Time0[0]表示日,Time0[1]表示月,Time1表示方法与Time0相同
INT16U Year_Period_Dec(INT8U Time0[],INT8U Time1[])
{
  INT16U Days[2];

  ASSERT(A_WARNING, Time0[0]<32 && Time0[0]>0 && Time0[1]<13 && Time0[1]>0);//判断两个参数的格式
  ASSERT(A_WARNING, Time1[0]<32 && Time0[0]>0 && Time1[1]<13 && Time1[1]>0);//判断两个参数的格式
  
  Days[0]=(INT16U)(Time0[1])*31+(INT16U)(Time0[0]);
  Days[1]=(INT16U)(Time1[1])*31+(INT16U)(Time1[0]);
  
  if(Days[0]<Days[1])
    return Days[0]+12*31-Days[1];
  else
    return Days[0]-Days[1];
}

//日时段时间差,返回为分钟数
//用于计算日时段表中两项的时间差,Time0-Time1,如果Time0在Time1之前，那么结果是
//Time0+24*60-Time1
//Time0[0]表示分，Time0[1]表示时，Time1表示方法与Time0相同
INT16U Date_Period_Dec(INT8U Time0[],INT8U Time1[])
{
  INT16U Mins[2];
  
  //分时数据必须合法
  ASSERT(A_WARNING,Time0[0]<60 && Time0[1]<24);//判断两个参数的格式
  ASSERT(A_WARNING,Time1[0]<60 && Time1[1]<24);//判断两个参数的格式
  
  Mins[0]=(INT16U)(Time0[1])*60+(INT16U)(Time0[0]);
  Mins[1]=(INT16U)(Time1[1])*60+(INT16U)(Time1[0]);
  
  if(Mins[0]<Mins[1])
    return Mins[0]+24*60-Mins[1];
  else
    return Mins[0]-Mins[1];  
}

//检查当前是否是节假日
//该函数需要从内存中读出节假日表并进行比较,一般每日调用该函数一次
//返回0表示不是节假日，返回>0表示是节假日，同时返回值代表是第多少个节假日，*pDate_Table_Index返回日时段表号
INT8U Check_Today_Holiday(INT8U *pDate_Table_Index)
{
  INT8U Num,i,j,Counts,Err,Re;
  PROTO_DI DI;

  Re=1;
  Re&=CHECK_STRUCT_SUM(Cur_Rate_Info);
  Re&=CHECK_STRUCT_SUM(Multi_Rate_Para);
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();

  if(Multi_Rate_Para.Holidays>MAX_YEAR_HOLIDAYS)
  {
    Multi_Rate_Para.Holidays=MAX_YEAR_HOLIDAYS;
    SET_STRUCT_SUM(Multi_Rate_Para);
  }

  //每个节假日参数DI对应15个节假日，计算需要读取多少个节假日DI参数
  if(0==Multi_Rate_Para.Holidays%15)
    Num=Multi_Rate_Para.Holidays/15;
  else
    Num=Multi_Rate_Para.Holidays/15+1;

  OS_Sem_Pend(PUB_BUF0_SEM_ID);//下面函数内部会用到Pub_Buf0,因此使用信号量互斥
  Counts=0;//已经检索过的节假日个数
  Re=0;//返回结果
  for(i=0;i<Num;i++)
  {
    if(RATE_SCHEME1==Cur_Rate_Info.Scheme)
      DI=0xCC1F+(i<<4);
    else
      DI=0xC41F+(i<<4);
    
    if(Get_DLT645_Data(DI,(void *)Pub_Buf0,(void *)Pub_Buf0,15*3,&Err)!=15*3 || Err!=NO_ERR)
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Read 0x%x Len Error",DI);
      Counts+=15;
      if(Counts>=Multi_Rate_Para.Holidays || Counts>=MAX_YEAR_HOLIDAYS)
        break;//此时Re==0
    }
    else
    {
      Bcd2Hex_Buf((void *)&Pub_Buf0,15*3,(void *)&Pub_Buf0,(void *)&Pub_Buf0,sizeof(Pub_Buf0));
      for(j=0;j<15;j++)//一个F项有15个节假日
      {
        if(Cur_Time0.Time[T_MONTH]==Pub_Buf0[j*3+2] &&\
           Cur_Time0.Time[T_DATE]==Pub_Buf0[j*3+1])
        {
          *pDate_Table_Index=Pub_Buf0[j*3];//节假日采用的日时段表号
          Re=1;//确实是个节假日
          break;
        }
        Counts++;
        if(Counts>=Multi_Rate_Para.Holidays || Counts>=MAX_YEAR_HOLIDAYS)
          break;//此时Re==0
      }
      //是节假日，跳出外部循环
      if(1==Re)
        break;
    }
  }
  OS_Sem_Post(PUB_BUF0_SEM_ID);//释放Pub_Buf0
  if(0==Re)
    return Re;
  else
    return Counts+1;//第多少个节假日?+1保证该结果>0
}

//计算当前应该使用的日时段表号,如果Flag==CALC_ROM则强制判断一次当前的日时段表号
//如果日时段表号变化了，将当前日时段表读入Date_Table中
INT8U Cal_Cur_Date_Table(INT8U Flag)
{
  INT8U i,Re;//Data_Index为使用的日时段表号索引
  INT8U Date_Table_Flag;//1表示找到日时段表号
  INT16U Min=0xFFFF;//最小值,先置一个极大值
  INT16U Dec;//差
  
  static S_HEX_Time Last_Chk_Time={{0,0,0,0,0},0,{0^((INT8U)CHK_CS_BYTES)}};
  static S_Int8U Date_Table_Index={CHK_BYTE,1,CHK_BYTE};
  
  //在当前节假日中查找是否是节假日?
  //时间发生了变化，或者Flag标志为CALC_ROM表示强制判断一次
  //每天只进入这个流程一次,或者CALC_ROM==Flag时进入该流程
  Re=CHECK_STRUCT_SUM(Last_Chk_Time);
  if(Last_Chk_Time.Time[T_DATE]!=Cur_Time0.Time[T_DATE] ||\
     Last_Chk_Time.Time[T_MONTH]!=Cur_Time0.Time[T_MONTH] ||\
     Last_Chk_Time.Time[T_YEAR]!=Cur_Time0.Time[T_YEAR]||\
     CALC_ROM==Flag)//跨日或者CALC_ROM情况下重新判断当日是否节假日,每天只判一次，或者CALC_ROM的情况下判断
  {
    Re=1;
    Re&=CHECK_STRUCT_SUM(Cur_Time0);//当前时间
    Re&=CHECK_STRUCT_SUM(Multi_Rate_Para);//当前复费率参数
    Re&=CHECK_STRUCT_SUM(Year_Table);//年时区表
    Re&=CHECK_STRUCT_SUM(Cur_Rate_Info);//当前费率信息
    
    if(ASSERT(A_WARNING,0!=Re))
      Check_Data_Avail();
  
    Date_Table_Flag=0;

    //本非当日时间，上面的判断保证每日只进入该函数一次
    mem_cpy(&Last_Chk_Time,(void *)&Cur_Time0,sizeof(Cur_Time0),&Last_Chk_Time,sizeof(Last_Chk_Time));

    //先判断当前是否是节假日
    Re=Check_Today_Holiday(&Date_Table_Index.Var);//返回>0标识表示是节假日
    if(Re>0)//
    {
      Date_Table_Flag=1;//==1表示找到日时段表索引号了
      Cur_Rate_Info.Date_Flag=HOLIDAY;
      Cur_Rate_Info.Holiday_Index=Re;    
    }

    //不是公共假日，是否是周休日?
    if(0==Date_Table_Flag)
    {
      if(GET_BIT(Multi_Rate_Para.WeekRest_Status,Cur_Time0.Week)==0)//为0表示该日是周休日
      {
        Date_Table_Index.Var=Multi_Rate_Para.WeekRest_DataTable;//周休日日时段表号
        Date_Table_Flag=1;//==1表示找到日时段表索引号了      
        Cur_Rate_Info.Date_Flag=WEEK_REST_DAY;    
      }
    }
    
    //不是公共假日，也不是周休日，在年时区表中查找
    if(0==Date_Table_Flag)
    {
      Cur_Rate_Info.Date_Flag=NORMAL_DAY;//这是一个平常的日子，既不是节假日，也不是周休日
      for(i=0;i<Multi_Rate_Para.Year_Period && i<MAX_YEAR_PERIODS;i++)
      {                                    //日、月
        Dec=Year_Period_Dec((INT8U *)&Cur_Time0.Time[T_DATE],(INT8U *)&Year_Table.MDN[i][1]);//计算当前时间和年时区表中某项时间的时间差
        if(Dec<Min)//寻找时间差最小的一个认为是当前年时区
        {
          Date_Table_Index.Var=Year_Table.MDN[i][0];//日时段表索引
          Date_Table_Flag=1;
          Cur_Rate_Info.Year_Period=i+1;//年时区号
          Min=Dec;
        }
      }
    }
  
    //没有得到合适的日时段表号，则采用1时段表
    if(ASSERT(A_WARNING,1==Date_Table_Flag &&\
                        Date_Table_Index.Var>0 &&\
                        Date_Table_Index.Var<=Multi_Rate_Para.Date_Table &&\
                        Date_Table_Index.Var<=MAX_DATE_TABLES))
      Date_Table_Index.Var=1;
    //判断时段表号是否发生变化,变化了则重新读取时段表参数,或者强制认为日时段发生了变化的话也重新读取
    if(Cur_Rate_Info.Date_Table!=Date_Table_Index.Var || CALC_ROM==Flag)//当前时段表号发生变化？
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Date Table Changed,%d-->%d",Cur_Rate_Info.Date_Table,Date_Table_Index.Var);
      Cur_Rate_Info.Date_Table=Date_Table_Index.Var;//日时段表索引号
      SET_STRUCT_SUM(Cur_Rate_Info);    
      Get_Cur_Date_Table(Cur_Rate_Info.Scheme,Date_Table_Index.Var);//获取当前时段表参数到Date_Table中 
    }
  }
  return Cur_Rate_Info.Date_Table;//Date_Table_Index;
}

//读取Rate_Scheme套费率方案的Date_Table_Index号时段表到Date_Table结构体变量中
void Get_Cur_Date_Table(INT8U Rate_Scheme,INT8U Date_Table_Index)
{
  INT8U Re;
  INT8U Err;
  INT16U Len;

  Re=CHECK_STRUCT_SUM(Date_Table);
  if(ASSERT(A_WARNING,1==Re))
    Check_Data_Avail();

  ASSERT(A_WARNING,(RATE_SCHEME0==Rate_Scheme || RATE_SCHEME1==Rate_Scheme));
  if(ASSERT(A_WARNING,Date_Table_Index>0 && Date_Table_Index<=MAX_DATE_TABLES))
    Date_Table_Index=1;

  Len=0;
  if(Rate_Scheme==RATE_SCHEME0)
    Len=Read_Storage_Data(0xC33F+((Date_Table_Index-1)<<4),(INT8U *)Date_Table.HMN,(INT8U *)Date_Table.HMN,sizeof(Date_Table.HMN),&Err);
  else
    Len=Read_Storage_Data(0xCB3F+((Date_Table_Index-1)<<4),(INT8U *)Date_Table.HMN,(INT8U *)Date_Table.HMN,sizeof(Date_Table.HMN),&Err);
  //日时段表数据转化为hex码
  Bcd2Hex_Buf((void *)&Date_Table,S_OFF(S_Date_Table,CS),(void *)&Date_Table,(void *)&Date_Table,S_OFF(S_Date_Table,CS));
  ASSERT(A_WARNING,MAX_DATE_PERIODS*3==Len && NO_ERR==Err);
  
  SET_STRUCT_SUM(Date_Table);
}


//计算当前应该使用的费率编号
//主要是从当前日时段表中找到当前费率号
INT8U Calc_Cur_Rate(INT8U Flag)
{
  INT8U Rate,Re;//Data_Index为使用的日时段表号索引
  INT16U Min=0xFFFF,i;
  INT16U Dec;
  INT8U Period;//日时段号
  
  Re=1;
  Re&=CHECK_STRUCT_SUM(Cur_Time0);
  Re&=CHECK_STRUCT_SUM(Cur_Rate_Info);
  Re&=CHECK_STRUCT_SUM(Multi_Rate_Para);
  
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();//检查数据的有效性

  Period=Cur_Rate_Info.Date_Period;
  //如果日时段表号发生变化，则重新读取到Date_Table变量中
  //套费率方按发生变化的时候，会强制切换Date_Table
  //平时计算费率的时候，只是每天计算一次Date_Table
  Cal_Cur_Date_Table(CALC_RAM);
  //此处没有用Flag，而用CALC_RAM，因为当Flag==CACL_ROM时，之前肯定调用了Calc_Cur_Rate_Scheme(CACL_ROM)，这里面已经对日时段表进行了处理

  //在日时段表中找当前费率
  Rate=0;
  Min=0xFFFF;//置一个极大值
  for(i=0;i<Multi_Rate_Para.Date_Period && i<MAX_DATE_PERIODS;i++)
  {                                
    Dec=Date_Period_Dec((INT8U *)&Cur_Time0.Time[T_MIN],(INT8U *)&Date_Table.HMN[i][1]);
    if(Dec<Min)
    {
      Rate=Date_Table.HMN[i][0];//费率
      Cur_Rate_Info.Date_Period=i+1;//日时段号
      Min=Dec;
    }
  }
  
  if(ASSERT(A_WARNING,Rate<=MAX_RATES && Rate<=Multi_Rate_Para.Rates && Rate>0))//费率号错，默认使用费率1
    Rate=1;
  
  if(Cur_Rate_Info.Rate!=Rate || CALC_ROM==Flag)//需要切换费率了
  {
    Cur_Rate_Info.Rate=Rate;
    SET_STRUCT_SUM(Cur_Rate_Info);//下面函数会CHECK_STRUCT_SUM哦
    Rate_Changed_Proc();//费率切换相关处理
  }
  
  if(Period!=Cur_Rate_Info.Date_Period)
  {
    Set_Event_Instant(ID_PARSE_SWITCH);//时段切换
    Port_Out_Pub(EXT_ID_PARSE,85);
    DEBUG_PRINT(HUCK,DEBUG_0,"Date Period Switch!!!");
  }
  SET_STRUCT_SUM(Cur_Rate_Info);  
  return Rate;//返回费率号
}
/*
typedef struct
{
  INT32U Chang_Plus_AcEnerg[4];            //A/B/C/SUM各元件正向有功增量:单位:0.01wh
  INT32U Chang_Nega_AcEnerg[4];            //A/B/C/SUM各元件反向有功增量:单位:0.01wh
  
  //INT32U Chang_Plus_ReacEnerg[4];          //A/B/C/SUM各元件正向无功增量(无功组合1):单位:0.01warh  
  //INT32U Chang_Nega_ReacEnerg[4];          //A/B/C/SUM各元件反向无功增量(无功组合2):单位:0.01warh
  
  INT32U Chang_Quad_ReacEnerg[4][4];       //A/B/C/SUM各元件四象限无功增量,Chang_Quad_ReacEnerg[1][2]表示B向3象限无功
  
  INT32U Add_AmpaHour[3];                 //AH累计:0.0001As,累计量
  INT32U Add_CopperLoss[4];		  //A,B,C,sum铜损,单位为0.0000001kA2s,累计量
  INT32U Add_IronLoss[4];		  //A,B,C,sum铁损,单位为0.0000001KV2s，累计量
  INT32U Add_GenaralErr[4];		  //A,B,C,sum合成误差,单位:0.01*tgFA*s,累计量
  INT8U CS[CS_BYTES];
}MEASU_ENERG; 
*/
//检查累积电量数据是否正确
INT8U Check_Cumulative_Energy_Data(MEASU_ENERG *pEnergy)
{
  INT8U Re,i,j;
  
  Re=Check_STRUCT_Sum(pEnergy,sizeof(MEASU_ENERG),pEnergy->CS,sizeof(pEnergy->CS));
  if(ASSERT(A_WARNING,1==Re))
    return 0;
  
  for(i=0;i<4;i++)
  {
    if(pEnergy->Chang_Plus_AcEnerg[i]>=(INT32U)MAX_ACTIVE_INC*ENERGY_RATIO ||\
      pEnergy->Chang_Nega_AcEnerg[i]>=(INT32U)MAX_ACTIVE_INC*ENERGY_RATIO)
    {
      ASSERT(A_WARNING,0);
      return 0;
    }
    else
    {
      for(j=0;j<4;j++)
      {
        if(pEnergy->Chang_Quad_ReacEnerg[i][j]>=(INT32U)MAX_REACTIVE_INC*ENERGY_RATIO) 
        {
          ASSERT(A_WARNING,0);
          return 0;
        }        
      }
    }
  }

  return 1;
}

//检查电量增长是否正常
INT8U Check_Energy_Data_Inc()
{
  INT8U i;
  INT32U Temp;
  
  //正向有功电量
  if(Cur_Energy.Pos_Active[0]>=Cur_Energy_Bak.Pos_Active[0])
  {
    Temp=Cur_Energy.Pos_Active[0]-Cur_Energy_Bak.Pos_Active[0];
    if(Temp>MAX_ACTIVE_INC)//电量飞走
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Pos Energy Data Inc too much:%ld",Temp);
      ASSERT(A_WARNING,0);
      return 0;
    }
  }
  else//电量倒走
  {
    ASSERT(A_WARNING,0);
    DEBUG_PRINT(HUCK,DEBUG_0,"Pos Energy Data Dec!");
    return 0;
  }
  
  //反向有功电量
  if(Cur_Energy.Neg_Active[0]>=Cur_Energy_Bak.Neg_Active[0])
  {
    Temp=Cur_Energy.Neg_Active[0]-Cur_Energy_Bak.Neg_Active[0];
    if(Temp>MAX_ACTIVE_INC)//电量飞走
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Neg Energy Data Inc too much:%ld",Temp);
      ASSERT(A_WARNING,0);
      return 0;
    }
  }
  else//电量倒走
  {
    ASSERT(A_WARNING,0);
    DEBUG_PRINT(HUCK,DEBUG_0,"Neg Energy Data Dec!");
    return 0;
  }
  
  //四象限无功
  for(i=0;i<4;i++)
  {
    if(Cur_Energy.Quad_Reactive[i][0]>=Cur_Energy_Bak.Quad_Reactive[i][0])
    {
      Temp=Cur_Energy.Quad_Reactive[i][0]-Cur_Energy_Bak.Quad_Reactive[i][0];
      if(Temp>MAX_REACTIVE_INC)//电量飞走
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Quad %d Reactive Energy Data Inc too much:%ld",i,Temp);
        ASSERT(A_WARNING,0);
        return 0;
      }
    }
    else//电量倒走
    {
      ASSERT(A_WARNING,0);
      DEBUG_PRINT(HUCK,DEBUG_0,"Quad %d Reactive Energy Data Dec!",i);
      return 0;
    }
  }
  
  //三相的正反有功
  for(i=0;i<3;i++)
  {
    //正向有功
    if(Cur_Energy.Phase_Pos_Active[i]>=Cur_Energy_Bak.Phase_Pos_Active[i])
    {
      Temp=Cur_Energy.Phase_Pos_Active[i]-Cur_Energy_Bak.Phase_Pos_Active[i];
      if(Temp>MAX_ACTIVE_INC)//电量飞走
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Phase %c Pos_Active Energy Data Inc too much:%d",'A'+i,Temp);
        ASSERT(A_WARNING,0);
        return 0;
      }
    }
    else//电量倒走
    {
      ASSERT(A_WARNING,0);
      DEBUG_PRINT(HUCK,DEBUG_0,"Phase %c Pos_Active Energy Data Dec!",'A'+i);
      return 0;
    }
    
    //反向有功
    if(Cur_Energy.Phase_Neg_Active[i]>=Cur_Energy_Bak.Phase_Neg_Active[i])
    {
      Temp=Cur_Energy.Phase_Neg_Active[i]-Cur_Energy_Bak.Phase_Neg_Active[i];
      if(Temp>MAX_REACTIVE_INC)//电量飞走
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Phase %c Neg_Active Energy Data Inc too much:%ld",'A'+i,Temp);
        ASSERT(A_WARNING,0);
        return 0;
      }
    }
    else//电量倒走
    {
      ASSERT(A_WARNING,0);
      DEBUG_PRINT(HUCK,DEBUG_0,"Phase %c Neg_Active Energy Data Dec!",'A'+i);
      return 0;
    }
  }
  return 1;
}

//当前电量累加
void Cur_Energy_Cumu(MEASU_ENERG *pEnergy)
{
  INT8U Rate;
  INT8U i,j;
  
  Rate=Cur_Rate_Info.Rate;//Get_Using_Rate();//当前使用费率
  //保证当前费率数是正确的,如果超过了最大费率数，则默认使用费率1
  if(ASSERT(A_WARNING,Rate>0 && Rate<=Multi_Rate_Para.Rates && Rate<=MAX_RATES))
     Rate=1;
  //正有累加  
  Cur_Energy.Pos_Active[0]+=(pEnergy->Chang_Plus_AcEnerg[3]/ENERGY_RATIO);
  Cur_Energy.Pos_Active[Rate]+=(pEnergy->Chang_Plus_AcEnerg[3]/ENERGY_RATIO);
  //反有累加
  Cur_Energy.Neg_Active[0]+=(pEnergy->Chang_Nega_AcEnerg[3]/ENERGY_RATIO);
  Cur_Energy.Neg_Active[Rate]+=(pEnergy->Chang_Nega_AcEnerg[3]/ENERGY_RATIO);
  //正无累加
  //Cur_Energy.Pos_Reactive[0]+=(pEnergy->Chang_Plus_ReacEnerg[3]/ENERGY_RATIO);
  //Cur_Energy.Pos_Reactive[Rate]+=(pEnergy->Chang_Plus_ReacEnerg[3]/ENERGY_RATIO);
  //反无累加
  //Cur_Energy.Neg_Reactive[0]+=(pEnergy->Chang_Nega_ReacEnerg[3]/ENERGY_RATIO);
  //Cur_Energy.Neg_Reactive[Rate]+=(pEnergy->Chang_Nega_ReacEnerg[3]/ENERGY_RATIO);
  
  //四象限无功累加
  for(i=0;i<4;i++)
  {
    Cur_Energy.Quad_Reactive[i][0]+=pEnergy->Chang_Quad_ReacEnerg[3][i]/ENERGY_RATIO;      
    Cur_Energy.Quad_Reactive[i][Rate]+=pEnergy->Chang_Quad_ReacEnerg[3][i]/ENERGY_RATIO;
  }
  
  //三相有无功
  for(i=0;i<3;i++)
  {
    Cur_Energy.Phase_Pos_Active[i]+=(pEnergy->Chang_Plus_AcEnerg[i]/ENERGY_RATIO);
    Cur_Energy.Phase_Neg_Active[i]+=(pEnergy->Chang_Nega_AcEnerg[i]/ENERGY_RATIO);

    for(j=0;j<4;j++)//三相四象限分别计算
      Cur_Energy.Phase_Quad_Reactive[i][j]+=pEnergy->Chang_Quad_ReacEnerg[i][j]/ENERGY_RATIO;
  }
  
  //三相安培时
  for(i=0;i<3;i++)
    Cur_Energy.Phase_AH[i+1]+=(pEnergy->Add_AmpaHour[i]/AH_RATIO);
  
   Cur_Energy.Phase_AH[0]+=(pEnergy->Add_AmpaHour[3]/AH_RATIO);
   
  //总以及三相铜损  
  Cur_Energy.CopperLoss[0]+=(pEnergy->Add_CopperLoss[3]/COPPERLOSS_RATIO);
  Cur_Energy.CopperLoss[1]+=(pEnergy->Add_CopperLoss[0]/COPPERLOSS_RATIO);
  Cur_Energy.CopperLoss[2]+=(pEnergy->Add_CopperLoss[1]/COPPERLOSS_RATIO);
  Cur_Energy.CopperLoss[3]+=(pEnergy->Add_CopperLoss[2]/COPPERLOSS_RATIO);
  //总以及三相铁损  
  Cur_Energy.IronLoss[0]+=(pEnergy->Add_IronLoss[3]/IRONLOSS_RATIO);
  Cur_Energy.IronLoss[1]+=(pEnergy->Add_IronLoss[0]/IRONLOSS_RATIO);
  Cur_Energy.IronLoss[2]+=(pEnergy->Add_IronLoss[1]/IRONLOSS_RATIO);
  Cur_Energy.IronLoss[3]+=(pEnergy->Add_IronLoss[2]/IRONLOSS_RATIO);
  //总以及三相合成误差
  Cur_Energy.TransErr[0]+=(pEnergy->Add_GenaralErr[3]/TRANSERR_RATIO);
  Cur_Energy.TransErr[1]+=(pEnergy->Add_GenaralErr[0]/TRANSERR_RATIO);
  Cur_Energy.TransErr[2]+=(pEnergy->Add_GenaralErr[1]/TRANSERR_RATIO);
  Cur_Energy.TransErr[3]+=(pEnergy->Add_GenaralErr[2]/TRANSERR_RATIO);
  //更新Cur_Energy.Time的时间
  //mem_cpy((S_HEX_Time *)&Cur_Energy.Time,(S_HEX_Time *)&Cur_Time0,sizeof(Cur_Time0),(S_HEX_Time *)&Cur_Energy.Time,sizeof(Cur_Energy.Time));
  SET_STRUCT_SUM(Cur_Energy);   
  
}
//累加电量
void Cumulative_Energy()
{
  static S_Int32U Sec_Bak={CHK_BYTE,0,CHK_BYTE};
  MEASU_ENERG *pEnergy;
  INT8U Re;
  
  Re=CHECK_STRUCT_VAR(Sec_Bak);
  ASSERT(A_WARNING,1==Re);
  
  //保证每秒才进入一次该函数
  if(Sec_Bak.Var==Sec_Timer_Pub)
    return;
  else
    Sec_Bak.Var=Sec_Timer_Pub;    
  
  Re=1;
  Re&=Check_Energy_Data((S_Energy *)&Cur_Energy);
  Re&=CHECK_STRUCT_SUM(Cur_Rate_Info);
  if(ASSERT(A_WARNING,1==Re))
    Check_Data_Avail();
  
  //OS_Sem_Pend(PUB_BUF_SEM_ID);//申请Pub_Buf的信号量

  Get_Measu_AcculateData_PUCK(HUCK,(void *)&Cumu_Energy,(void *)&Cumu_Energy,sizeof(Cumu_Energy),sizeof(Cumu_Energy));
  pEnergy=(MEASU_ENERG *)&Cumu_Energy;
  Re=Check_Cumulative_Energy_Data(pEnergy);
  ASSERT(A_WARNING,1==Re);
  if(1==Re)//pEnergy指向的数据合法
  { 
    DIS_PD_INT;//关掉电中断
    Cur_Energy_Cumu(pEnergy); //当前电量累加   
    //Event_Energy_Cumu(pEnergy);//报警相关电量数据的累加
    Energy_Cumu_Counts.Var++; //电量累加次数增加
    EN_PD_INT;//开掉电中断
  }
  //OS_Sem_Post(PUB_BUF_SEM_ID);//申请Pub_Buf的信号量
  
  Check_Energy_Data_Inc();//检查电量增长是否合法
  

  
#if USE_ENERGY_RAM_BAK//将数据备份到Cur_Energy_Bak变量中
  mem_cpy((void *)&Cur_Energy_Bak,(void *)&Cur_Energy,sizeof(Cur_Energy_Bak),(void *)&Cur_Energy_Bak,sizeof(Cur_Energy_Bak));
#endif
}

//检查复费率电量数据分费率电量和是否等于总电量
INT8U Check_Multi_Rate_Data(INT32U Data[])
{
  INT16U i;
  INT32U Sum;
  
  Sum=0;
  for(i=1;i<=MAX_RATES;i++)
    Sum+=Data[i];
  if(Sum==Data[0])
    return 1;
  else
    return 0;
}

//检查电量数据是否合法正确
//返回1表示数据正确,0表示数据有错
INT8U Check_Energy_Data(S_Energy *pEnergy)
{
  INT8U Re=1;

  Re&=(pEnergy->Head==CHK_BYTE && pEnergy->Tail==CHK_BYTE)?1:0;  
  Re&=Check_STRUCT_Sum(pEnergy,sizeof(S_Energy),pEnergy->CS,sizeof(pEnergy->CS));
  Re&=(pEnergy->Data_Ready_Flag==DATA_READY)?1:0;
  if(ASSERT(A_WARNING,0!=Re))
    return 0;
  
  Re&=Check_Multi_Rate_Data((INT32U *)(pEnergy->Pos_Active));
  ASSERT(A_WARNING,0!=Re);
  Re&=Check_Multi_Rate_Data((INT32U *)(pEnergy->Neg_Active));
  ASSERT(A_WARNING,0!=Re);
  Re&=Check_Multi_Rate_Data((INT32U *)(pEnergy->Neg_Active));
  ASSERT(A_WARNING,0!=Re);
  Re&=Check_Multi_Rate_Data((INT32U *)(pEnergy->Quad_Reactive[0]));
  ASSERT(A_WARNING,0!=Re);
  Re&=Check_Multi_Rate_Data((INT32U *)(pEnergy->Quad_Reactive[1]));
  ASSERT(A_WARNING,0!=Re);
  Re&=Check_Multi_Rate_Data((INT32U *)(pEnergy->Quad_Reactive[2]));
  ASSERT(A_WARNING,0!=Re);
  Re&=Check_Multi_Rate_Data((INT32U *)(pEnergy->Quad_Reactive[3]));
  ASSERT(A_WARNING,0!=Re);
  Re&=Check_STRUCT_Sum(pEnergy,sizeof(S_Energy),pEnergy->CS,sizeof(pEnergy->CS));  
  ASSERT(A_WARNING,0!=Re);
  return Re;
}

//检查全局公共buf是否合法
void Check_Pub_Var_Avail()
{
  INT8U Re;
  Re=CHECK_STRUCT_VAR(_Pub_Buf);
  ASSERT(A_WARNING,1==Re);
  Re=CHECK_STRUCT_VAR(_Pub_Buf0);
  ASSERT(A_WARNING,1==Re);
  Re=CHECK_STRUCT_VAR(Pub_Timer);
  ASSERT(A_WARNING,1==Re);
  Re=CHECK_STRUCT_VAR(Assert_Info);
  ASSERT(A_WARNING,1==Re);
}

//检查当前电量和需量数据,并进行容错处理
void Check_Energy_Data_Avail()//检查当前电量与需量数据的正确性,总=和
{
  INT8U Re;
  static S_Flag Energy_Err_Flag={0,{0}};
 
  //Energy_Err_Flag用来记录电量数据是否发生过错误（分费率和！=总）,如果已经发生过且没有纠错成功
  //则该标志置1，标识纠错不成功,从而下次进入该函数后不再进行纠错了,防止重复不断的纠错该数据
  
  //检查当前电量,同时检查分费率是否等于总，以及校验和
  //检查分费率是否等于总时，如果内存中数据不对，则读取ROM中数据，如果读出的数据还不对
  //则默认该次的错误,以后不再进行检查，此时Energy_Err_Flag==1
  Re=Check_Energy_Data((S_Energy *)&Cur_Energy);
  if(0==Re)
  {
    ASSERT(A_WARNING,0);
#if USE_ENERGY_RAM_BAK//该宏为1表示定义了Cur_Energy_Bak变量作为Cur_Energy的内存备份
    DEBUG_PRINT(HUCK,DEBUG_0,"Cur_Energy Data Error,Copy from Cur_Energy_Bak Data");
    Re=Check_Energy_Data((S_Energy *)&Cur_Energy_Bak);//使用内存备份电量
    if(1==Re)//Cur_Energy_Bak中的数据正确
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Copy Cur_Energy_Bak Data to Cur_Energy");//将Cur_Energy_Bak中的数据复制到Cur_Energy中
      mem_cpy((void *)&Cur_Energy,(void *)&Cur_Energy_Bak,sizeof(Cur_Energy),(void *)&Cur_Energy,sizeof(Cur_Energy));
    }
    else//Cur_Energy_Bak中数据不正确的情况下直接从rom中读取数据
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Cur_Energy_Bak Data Error,Read_Energy_Data_From_Rom");
      Read_Energy_Data_From_Rom(); 
      Re=Check_Energy_Data((S_Energy *)&Cur_Energy);//ROM中的数据也不对的话，完全没有办法了，默认这个错误,将Energy_Err_Flag=1
      if(0==Re)
        Energy_Err_Flag.Flag=1;//表示纠错没有成功
      SET_STRUCT_SUM(Energy_Err_Flag);//同时也可纠正上面的CHECK_STRUCT_SUM(Energy_Err_Flag)!=1
    }
#else
    ASSERT(A_WARNING,0);
    DEBUG_PRINT(HUCK,DEBUG_0,"Cur_Energy Data Error,Read_Energy_Data_From_Rom");
    Read_Energy_Data_From_Rom(); 
    Re=Check_Energy_Data((S_Energy *)&Cur_Energy);//ROM中的数据也不对的话，完全没有办法了，默认这个错误,将Energy_Err_Flag=1
    if(0==Re)
      Energy_Err_Flag.Flag=1;//表示纠错没有成功
    SET_STRUCT_SUM(Energy_Err_Flag);//同时也可纠正上面的CHECK_STRUCT_SUM(Energy_Err_Flag)!=1
#endif
  }
}

//保存当前电量和需量数据，主流程定时调用该函数保存电量等
void Save_Cur_Energy_Data()
{
  INT8U Re;
  Re=Check_Energy_Data((S_Energy *)&Cur_Energy);
  if(ASSERT(A_WARNING,1==Re))
    Check_Data_Avail();
 
  Re=Check_Energy_Data((S_Energy *)&Cur_Energy);
  if(1==Re)
  {
    Re=Write_Storage_Data(CUR_ENERGY,(void *)(&Cur_Energy),ENERGY_SAVE_SIZE);
    ASSERT(A_WARNING,NO_ERR==Re);
    OS_TimeDly_Ms(100);
  }
  else
    ASSERT(A_WARNING,0);
}

//掉电时调用该函数保存电量需量等
void Save_Cur_Energy_PD_Data()
{
  INT8U Re;
  Re=Check_Energy_Data((S_Energy *)&Cur_Energy);
  if(0==Re)//判断Cur_Energy中的数据是否正确，不正确则采用Cur_Energy_Bak中的数据，如果两个数据都不正确，则不存
  {
    ASSERT(A_WARNING,0);
#if USE_ENERGY_RAM_BAK==1
    Re=Check_Energy_Data((S_Energy *)&Cur_Energy_Bak);
    if(1==Re)
      mem_cpy((S_Energy *)&Cur_Energy,(S_Energy *)&Cur_Energy_Bak,sizeof(Cur_Energy_Bak),(S_Energy *)&Cur_Energy,sizeof(Cur_Energy));
    else
    {
      ASSERT(A_WARNING,0);
      return;
    }
#else
    return;
#endif
  }
  Re=Write_Storage_Data(CUR_ENERGY_PD,(void *)(&Cur_Energy),ENERGY_SAVE_SIZE);
  if(NO_ERR!=Re)
    ASSERT(A_WARNING,0);
}

//保存当前电量和需量数据，主流程定时调用该函数保存电量等
void Save_Cur_Energy_Bak0_Data()
{
  INT8U Re;
  
  Re=Check_Energy_Data((S_Energy *)&Cur_Energy);
  if(1==Re)
  {
    Re=Write_Storage_Data(CUR_ENERGY_BAK0,(void *)(&Cur_Energy),ENERGY_SAVE_SIZE);
    ASSERT(A_WARNING,NO_ERR==Re);  
    OS_TimeDly_Ms(100);
  }
  else
    ASSERT(A_WARNING,0);
}

//保存当前电量和需量数据，主流程定时调用该函数保存电量等
void Save_Cur_Energy_Bak1_Data()
{
  INT8U Re;

  Re=Check_Energy_Data((S_Energy *)&Cur_Energy);
  if(1==Re)
  {  
    Re=Write_Storage_Data(CUR_ENERGY_BAK1,(void *)(&Cur_Energy),ENERGY_SAVE_SIZE);
    ASSERT(A_WARNING,NO_ERR==Re);  
    OS_TimeDly_Ms(100);
  }
  else
    ASSERT(A_WARNING,0);
}

//保存当前电量和需量数据，主流程定时调用该函数保存电量等
void Save_Cur_Energy_Bak2_Data()
{
  INT8U Re;
  Re=Check_Energy_Data((S_Energy *)&Cur_Energy);
  if(1==Re)
  {
    Re=Write_Storage_Data(CUR_ENERGY_BAK2,(void *)(&Cur_Energy),ENERGY_SAVE_SIZE);
    ASSERT(A_WARNING,NO_ERR==Re);  
    OS_TimeDly_Ms(100);
  }
  else
    ASSERT(A_WARNING,0);
}

//设置默认的电量数据
void Set_Def_Energy_Data()
{
  INT8U i,Re;
  S_HEX_Time TempTime;
  
  DISP_CLR_DATA_INFO;
  DEBUG_PRINT(HUCK,DEBUG_0,"Set Def Energy Data");
  mem_set((void *)&Cur_Energy,0,sizeof(Cur_Energy),(void *)&Cur_Energy,sizeof(Cur_Energy));
  SET_STRUCT_SUM(Cur_Energy);
  
#if USE_ENERGY_RAM_BAK==1
  mem_set((void *)&Cur_Energy_Bak,0,sizeof(Cur_Energy),(void *)&Cur_Energy_Bak,sizeof(Cur_Energy_Bak));
  SET_STRUCT_SUM(Cur_Energy_Bak);
#endif
  
  INIT_STRUCT_VAR(Cur_Energy);
  SET_DATA_READY_FLAG(Cur_Energy);
  SET_STRUCT_SUM(Cur_Energy);
  
  Save_Cur_Energy_PD_Data();
  Save_Cur_Energy_Data();
  Save_Cur_Energy_Bak0_Data();  
  //Save_Cur_Energy_Bak1_Data();  
  //Save_Cur_Energy_Bak2_Data();
  
  Re=CHECK_STRUCT_SUM(Cur_Time0);//检查当前时间
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();

  //<=Cur_Time0的最近一个冻结时间点
  Get_Last_Freeze_Time((S_HEX_Time *)&Cur_Time0,&TempTime);
  //将前i个月的数据读出，不能读出或读出失败的，进行冻结
  //mem_cpy(&TempTime1,&TempTime,sizeof(S_HEX_Time),&TempTime1,sizeof(TempTime1));//备份TempTime到TempTime1
  for(i=0;i<HIS_ENERGY_DATA_MONTHS;i++)
  {
    SET_STRUCT_TIME(Cur_Energy,TempTime);//设置电量数据时间为TempTime
    DEBUG_PRINT(HUCK,DEBUG_0,"Set Energy Data Time: ");
    Debug_Print_HEX_Time((INT8U *)Cur_Energy.Time.Time);
    Write_HIS_Data(HIS_ENERGY_DATA,&TempTime,(void *)&Cur_Energy,ENERGY_SAVE_SIZE);
    
    Time_Dec(&TempTime,1,UNIT_MON,&TempTime);//时间向前推1个月
    OS_TimeDly_Ms(20);//延时给其他任务
  }
  SET_STRUCT_TIME(Cur_Energy,Cur_Time0);//恢复电量时间 
  
  Extend_Set_Def_Energy_Data();
}

//每隔一段时间自动保存当前电量、需量、需量时间等数据
void Auto_Save_Cur_Data()
{
  DECLARE_BUF_TYPE(INT8U,6,S_Bak_Hour);
  static S_Bak_Hour Bak_Hour={CHK_BYTE,{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},CHK_BYTE};
  static S_Int8U Min_Bak={CHK_BYTE,0xFF,CHK_BYTE};
  INT8U Re;
  
  Re=1;
  Re&=CHECK_STRUCT_VAR(Min_Bak);
  Re&=CHECK_STRUCT_VAR(Bak_Hour);
  ASSERT(A_WARNING,1==Re);
  
  Re=CHECK_STRUCT_SUM(Cur_Time0);
  ASSERT(A_WARNING,0!=Re);
  
  if(Min_Bak.Var!=Cur_Time0.Time[T_MIN])//每分钟判断一次
  {
    Min_Bak.Var=Cur_Time0.Time[T_MIN];
    
    if(Min_Bak.Var==0)//分钟为0的情况下不存数!因为这个时刻可能是月冻结时刻，此时不应该存数，以免引起混乱
      return;
 
    Re=CHECK_STRUCT_VAR(Bak_Hour);
    ASSERT(A_WARNING,1==Re);
  
    SET_STRUCT_TIME(Cur_Energy,Cur_Time0);
    SET_STRUCT_TIME(Cur_Demand.Demand[0],Cur_Time0);
    SET_STRUCT_TIME(Cur_Demand.Demand[1],Cur_Time0);
    SET_STRUCT_SUM(Cur_Demand);
    SET_STRUCT_TIME(Cur_VolStat,Cur_Time0);
    
    //每半小时保存掉电数据,该数据存放在铁电中
    if(Cur_Time0.Time[T_MIN]==17 || Cur_Time0.Time[T_MIN]==47)
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Every 30 Mins Save_Cur_Energy_PD_Data,Save_Cur_Demand_PD_Data,Save_Bak_PD_Time");
      Save_Cur_Energy_PD_Data();
      Save_Cur_Demand_PD_Data();
      Save_Bak_PD_Time();
    }
    
    //每小时43分保存数据,该数据存放在EEROM中
    if(Cur_Time0.Time[T_MIN]==43 && Cur_Time0.Time[T_HOUR]!=Bak_Hour.Buf[0])
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Every Hour Save_Cur_Energy_Data,Save_Cur_Demand_Data,Save_PD_Time,Save_LoadData_PD_Data");
      Bak_Hour.Buf[0]=Cur_Time0.Time[T_HOUR];
      Save_Cur_Energy_Data();
      Save_Cur_Demand_Data();
      Save_PD_Time();
      Save_LoadData_PD_Data();      
    }
    
    //每6小时保存一次电量需量数据到备份的DI中,该数据存放在EEROM中
    if(Cur_Time0.Time[T_HOUR]%6==5 && Cur_Time0.Time[T_HOUR]!=Bak_Hour.Buf[1])
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Every 6 Hours Save_Cur_Energy_Bak0_Data && Save_Cur_Demand_Bak_Data");
      Bak_Hour.Buf[1]=Cur_Time0.Time[T_HOUR];
      Save_Cur_Energy_Bak0_Data();
      Save_Cur_Demand_Bak_Data();   
    }
    
    //每12小时保存一次电量需量数据到备份的DI中,该数据存放在EEROM中
    if(Cur_Time0.Time[T_HOUR]%12==1 && Cur_Time0.Time[T_HOUR]!=Bak_Hour.Buf[2])
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Every 6 Hours Save_Cur_Energy_Bak1_Data,Save_LoadData_Index_Bak");
      Bak_Hour.Buf[2]=Cur_Time0.Time[T_HOUR];
      Save_Cur_Energy_Bak1_Data();      
      Save_LoadData_Index_Bak();
    }

    //每12小时保存一次电量需量数据到备份的DI中,因为该数据在DataFlash中,因此12小时存一次
    if(Cur_Time0.Time[T_HOUR]%12==2 && Cur_Time0.Time[T_HOUR]!=Bak_Hour.Buf[3])
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Every 12 hours Save_Cur_Energy_Bak2_Data");
      Bak_Hour.Buf[3]=Cur_Time0.Time[T_HOUR];
      Save_Cur_Energy_Bak2_Data(); 
    }
    
    //每12小时保存电压统计掉电数据,该数据存放在铁电中
    if(Cur_Time0.Time[T_HOUR]%12==3 && Cur_Time0.Time[T_HOUR]!=Bak_Hour.Buf[4])
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Every hour Save_Cur_VolStat_PD_Data");
      Bak_Hour.Buf[4]=Cur_Time0.Time[T_HOUR];    
      Save_Cur_VolStat_PD_Data();
    }
    
    //每12小时保存电压统计数据,以及负荷曲线索引参数,该数据存放在EEROM中
    if(Cur_Time0.Time[T_HOUR]%12==4 && Cur_Time0.Time[T_HOUR]!=Bak_Hour.Buf[5])
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Every 12 hour Save_Cur_VolStat_Data,Save_LoadData_Index");
      Bak_Hour.Buf[5]=Cur_Time0.Time[T_HOUR];    
      Save_Cur_VolStat_Data();
      Save_LoadData_Index(); 
    }
  }
}

//费率方案切换处理,主要是重新读取部分参数
//New_Scheme,表示切换到该费率方案
//New_Scheme==RATE_SCHEME0或者New_Scheme==RATE_SCHEME1
//该函数的主要作用是更新套费率方案相关参数
void Rate_Scheme_Changed_Proc()
{
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(Cur_Rate_Info);
  if(ASSERT(A_WARNING,1==Re))
    Check_Data_Avail();
  
  if(ASSERT(A_WARNING,Cur_Rate_Info.Scheme==RATE_SCHEME0 || Cur_Rate_Info.Scheme==RATE_SCHEME1))
  {
    Cur_Rate_Info.Scheme=RATE_SCHEME0;//默认使用主费率方案
    SET_STRUCT_SUM(Cur_Rate_Info);
  }
  DEBUG_PRINT(HUCK,DEBUG_0,"Rate_Scheme Changed! Use %dth Scheme",Cur_Rate_Info.Scheme);
  
  Set_Using_Rate_Scheme(Cur_Rate_Info.Scheme);//重新设置当前费率方案到费率状态字
  Read_Multi_Rate_Para(Cur_Rate_Info.Scheme);//从存储器中读取当前费率方案相关的参数 
  
  Cal_Cur_Date_Table(CALC_ROM);//重新计算时段表号,并且重新读取日时段表参数到Date_Table中  
}

//费率切换处理
void Rate_Changed_Proc()
{
  INT8U Re;
 
  Re=CHECK_STRUCT_SUM(Cur_Rate_Info);
  if(ASSERT(A_WARNING,1==Re))
    Check_Data_Avail();
  
  DEBUG_PRINT(HUCK,DEBUG_0,"Rate Changed! Use %dth Rate",Cur_Rate_Info.Rate);
  Set_Using_Rate(Cur_Rate_Info.Rate);//设置费率状态字
}

//计算当前费率方案和费率，以及是否发生费率切换，如果发生费率切换
//就重新从rom中读取相关参数等
void Check_Cur_Rate()
{
  S_Int8U  static Min={CHK_BYTE,0,CHK_BYTE};
  INT8U Re;  
 
  Re = 1;
  Re &= CHECK_STRUCT_VAR(Min);
  Re &= CHECK_STRUCT_VAR(Time_Modify_Flag);
  ASSERT(A_WARNING, 1==Re);
  
  if(Min.Var!=Cur_Time0.Time[T_MIN] || Time_Modify_Flag.Var==1)//每隔一分钟进入一次下面的流程
  {
    Time_Modify_Flag.Var = 0;
    Min.Var=Cur_Time0.Time[T_MIN];
    //检查当前费率信息
    Re=CHECK_STRUCT_SUM(Cur_Rate_Info);
    if(ASSERT(A_WARNING,0!=Re))
      Check_Data_Avail();
  
    //Cur_Rate_Info中的费率信息应该和Rate_Status中的费率信息应该一致
    ASSERT(A_WARNING,Cur_Rate_Info.Rate==Rate_Status.Status[1].Byte);
    ASSERT(A_WARNING,Cur_Rate_Info.Scheme==Rate_Status.Status[0].Byte);
  
    //计算当前应该使用的费率方案,判断当前套费率方案，如果方案发生了切换，则更新复费率参数Multi_Rate_Para,以及更新当前日时段表     
    Calc_Cur_Rate_Scheme(CALC_RAM);//计算当前费率方案
    Calc_Cur_Rate(CALC_RAM);//计算当前应该使用的费率
  }
}

//将当前电能数据按pTime时间存储起来
void _Freeze_Energy_Data(S_HEX_Time *pTime)
{
  //SET_STRUCT_TIME(Cur_Energy,*pTime);//将结构体中的时间设为数据存储时间
  Write_HIS_Data(HIS_ENERGY_DATA,pTime,(void *)&Cur_Energy,ENERGY_SAVE_SIZE);
  //SET_STRUCT_TIME(Cur_Energy,Cur_Time0);//结构体中时间恢复为当前时间
  OS_TimeDly_Ms(100);//让出CPU 

}

//每月抄表日冻结处理
void Month_Data_Freeze()
{
  INT8U Re;
  static S_Int8U Min_Bak={CHK_BYTE,0xFF,CHK_BYTE};
  S_HEX_Time TempTime;
  
  //保证每分钟只进来一次
  Re=CHECK_STRUCT_VAR(Min_Bak);
  ASSERT(A_WARNING,1==Re);
  
  if(Min_Bak.Var==Cur_Time0.Time[T_MIN])
    return;
  else
    Min_Bak.Var=Cur_Time0.Time[T_MIN];
  
  Re=1;
  Re&=Check_HEX_Time((S_HEX_Time *)&Cur_Time0);
  Re&=CHECK_STRUCT_SUM(Data_Freeze_Time);
  if(ASSERT(A_WARNING,1==Re))
    Check_Data_Avail();
  
  //时间数据不是BCD码
  //Re=C((INT8U *)(Data_Freeze_Time.DDHH),2);
  //Re=CHECK_STRUCT_SUM(Data_Freeze_Time);
  //if(ASSERT(A_WARNING,1==Re))
    //Check_Data_Avail();
  
  //如果自动抄表时间设置错误
  if(ASSERT(A_WARNING,Data_Freeze_Time.DDHH[0]<=23))
  {
    Data_Freeze_Time.DDHH[0]=0;
    SET_STRUCT_SUM(Data_Freeze_Time);
  }
  if(ASSERT(A_WARNING,Data_Freeze_Time.DDHH[1]>=1 &&\
                      Data_Freeze_Time.DDHH[1]<=31))
  {
    Data_Freeze_Time.DDHH[1]=1;
    SET_STRUCT_SUM(Data_Freeze_Time);
  }
  
  //判断冻结日时间是否到了
  if(Cur_Time0.Time[T_MIN] == 0 &&\
     Cur_Time0.Time[T_HOUR]==Data_Freeze_Time.DDHH[0] &&\
     Cur_Time0.Time[T_DATE]==Data_Freeze_Time.DDHH[1])
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"----------Month Data Freeze Start!----------");
    //获取当月冻结时间点
    Get_NowMonth_Freeze_Time((S_HEX_Time *)&Cur_Time0,&TempTime);//防止直接使用Cur_Time0产生的分钟误差

    //将需要冻结的数据的时间域置为冻结时间点
    SET_STRUCT_TIME(Cur_Energy,TempTime);
    SET_STRUCT_TIME(Cur_Demand.Demand[0],TempTime);
    SET_STRUCT_TIME(Cur_Demand.Demand[1],TempTime);
    SET_STRUCT_SUM(Cur_Demand);
    SET_STRUCT_TIME(Cur_VolStat,TempTime);
    
    Freeze_Energy_Data(&TempTime);//冻结电量数据
    Freeze_Demand_Data(&TempTime);//冻结需量数据
    Freeze_VolStat_Data(&TempTime);//冻结电压统计数据

  }
}

//初始化电量数据
void Read_Energy_Data_From_Rom()
{
  INT8U Err=NO_ERR;
  S_Energy *p;
  
  //先读取掉电存储区数据
  //再读普通数据
  //最后读备份区数据
  OS_Sem_Pend(PUB_BUF0_SEM_ID); 
  p=(S_Energy *)Pub_Buf0;
  
  if(Read_Storage_Data(CUR_ENERGY_PD,p,p,sizeof(Pub_Buf0),&Err)!=ENERGY_SAVE_SIZE ||\
    Check_Energy_Data((S_Energy *)p)!=1)
  {
    //需要保证存储的校验和正确且数据本身正确才认为读取成功
    DEBUG_PRINT(HUCK,DEBUG_0,"Read CUR_ENERGY_PD data error");
    OS_TimeDly_Ms(500);
    if(Read_Storage_Data(CUR_ENERGY,p,p,sizeof(Pub_Buf0),&Err)!=ENERGY_SAVE_SIZE ||\
       Check_Energy_Data((S_Energy *)p)!=1)
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Read CUR_ENERGY data error");
      OS_TimeDly_Ms(500);
      if(Read_Storage_Data(CUR_ENERGY_BAK0,p,p,sizeof(Pub_Buf0),&Err)!=ENERGY_SAVE_SIZE ||\
         Check_Energy_Data((S_Energy *)p)!=1)
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Read CUR_ENERGY_BAK0 data error");
        OS_TimeDly_Ms(500);
        if(Read_Storage_Data(CUR_ENERGY_BAK1,p,p,sizeof(Pub_Buf0),&Err)!=ENERGY_SAVE_SIZE ||\
           Check_Energy_Data((S_Energy *)p)!=1)
        {
          DEBUG_PRINT(HUCK,DEBUG_0,"Read CUR_ENERGY_BAK1 data error");
          if(Read_Storage_Data(CUR_ENERGY_BAK2,p,p,sizeof(Pub_Buf0),&Err)!=ENERGY_SAVE_SIZE ||\
             Check_Energy_Data((S_Energy *)p)!=1)
          {
            DEBUG_PRINT(HUCK,DEBUG_0,"Read CUR_ENERGY_BAK2 data error");
            mem_set(p,0,sizeof(Cur_Energy),p,sizeof(Pub_Buf0));
            Write_Storage_Data(CUR_ENERGY_BAK2,p,ENERGY_SAVE_SIZE);
          }
          Write_Storage_Data(CUR_ENERGY_BAK1,p,ENERGY_SAVE_SIZE);
        }
        Write_Storage_Data(CUR_ENERGY_BAK0,p,ENERGY_SAVE_SIZE);
      }  
      Write_Storage_Data(CUR_ENERGY,p,ENERGY_SAVE_SIZE);
    }
    Write_Storage_Data(CUR_ENERGY_PD,p,ENERGY_SAVE_SIZE);
  }
 
  DIS_PD_INT;
  mem_cpy((void *)&Cur_Energy,p,sizeof(Cur_Energy),(void *)&Cur_Energy,sizeof(Cur_Energy));
  INIT_STRUCT_VAR(Cur_Energy);  
  SET_DATA_READY_FLAG(Cur_Energy);
  SET_STRUCT_SUM(Cur_Energy);
  
#if USE_ENERGY_RAM_BAK
  mem_cpy((void *)&Cur_Energy_Bak,(void *)&Cur_Energy,sizeof(Cur_Energy_Bak),(void *)&Cur_Energy_Bak,sizeof(Cur_Energy_Bak)); 
#endif
  
  EN_PD_INT;
  
  OS_Sem_Post(PUB_BUF0_SEM_ID);   
}
//该函数主要是为液晶显示第一屏准备参数和数据
//后面的流程还会重新读这些数据
void Read_Energy_Data_Para_For_Show()
{
  INT8U Rst_Flag;  
  INT8U Err;
  
  //读取掉电时存储的电量数据,没有读取成功则读取备份存储的数据 
  Rst_Flag=Check_Rst_Flag();
  if(Rst_Flag==POWER_ON_RST || Check_Energy_Data((S_Energy *)&Cur_Energy)==0)//电源上电或者校验和错误的情况下都必须读取ROM中的数据
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"PowerOn Read Energy Data");
    Read_Energy_Data_From_Rom();
  }

  Read_Mode_Word();//读取模式字，用于计算正向有功的模式字
  //电能显示小数位数
  if(Read_Storage_Data(0xC115,(void *)(&Disp_Digs.Energy_Digs),(void *)(&Disp_Digs.Energy_Digs),1,&Err)!=1 || Err!=NO_ERR)
    DEBUG_PRINT(HUCK,DEBUG_0,"Read 0xC125 Para Err");    
  Disp_Digs.Energy_Digs=Bcd2Hex_Byte(Disp_Digs.Energy_Digs);
  SET_STRUCT_SUM(Disp_Digs);
}

//初始化电量和需量数据
void PowerOn_Read_Data_From_Rom()
{
  INT8U Rst_Flag;
  //读取掉电时存储的电量数据,没有读取成功则读取备份存储的数据 
  Rst_Flag=Check_Rst_Flag();
  //显示第一屏以前已经读取到总电量
  if(Rst_Flag==POWER_ON_RST || Check_Energy_Data((S_Energy *)&Cur_Energy)==0)//电源上电或者校验和错误的情况下都必须读取ROM中的数据
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"PowerOn Read Energy Data");
    Read_Energy_Data_From_Rom();
  }
  
  //读取掉电时存储的需量数据，没有读取成功则读取备份存储的数据 
  if(Rst_Flag==POWER_ON_RST || Check_Demand_Data((S_Demand *)&Cur_Demand)==0)//电源上电或者校验和错误的情况下都必须读取ROM中的数据
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"PowerOn Read Demand Data");
    Read_Demand_Data_From_Rom();
  }
  
  //读取掉电时存储的需量时间数据，没有读取成功则读取备份存储的数据
  if(Rst_Flag==POWER_ON_RST || Check_VolStat_Data((S_VolStat *)&Cur_VolStat)==0 ||\
                           Check_VolStat_Data((S_VolStat *)&Total_VolStat)==0)//电源上电或者校验和错误的情况下都必须读取ROM中的数据 
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"PowerOn Read VolStat Data");
    Read_VolStat_Data_From_Rom();
  } 
  
  //读取负荷曲线存储信息
  if(Rst_Flag==POWER_ON_RST || CHECK_STRUCT_SUM(LoadData_Index)==0)//电源上电或者校验和错误的情况下都必须读取ROM中的数据 
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"PowerOn Read LoadData Index");
    Read_LoadData_Index_Data_From_Rom();
  }
  
  //if(Rst_Flag==POWER_ON_RST || CHECK_STRUCT_SUM(Event_Cumu)==0)//电源上电或者校验和错误的情况下都必须读取ROM中的数据 
  {
    //DEBUG_PRINT(HUCK,DEBUG_0,"PowerOn Read Event_Cumu Data");
    //Read_Event_Cumu_Data_From_Rom();
  }
  
}

//上电后月数据的冻结，前溯HIS_ENERGY_DATA_MONTHS个月
void PowerOn_Energy_Data_Freeze()
{
  Freeze_Energy_Data((S_HEX_Time *)&Cur_Time0);
}

void Freeze_Energy_Data(S_HEX_Time *pTime)
{
  INT8U i,Err,Re;
  INT16U Len;
  S_HEX_Time TempTime;
  
  Re=Check_HEX_Time(pTime);//检查当前时间
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();
  
  DEBUG_PRINT(HUCK,DEBUG_0,"----------Freeze Energy Data %d-%d-%d %d:%d----------",\
                           pTime->Time[T_YEAR],pTime->Time[T_MONTH],pTime->Time[T_DATE],pTime->Time[T_HOUR],pTime->Time[T_MIN]);
  
  DEBUG_PRINT(HUCK,DEBUG_0,"Energy Data Freeze, Data Time: ");
  Debug_Print_HEX_Time((INT8U *)Cur_Energy.Time.Time);  
  //<=Cur_Time0的最近一个冻结时间点
  Get_Last_Freeze_Time(pTime,&TempTime);
  //将前i个月的数据读出，不能读出或读出失败的，进行冻结
  //mem_cpy(&TempTime1,&TempTime,sizeof(S_HEX_Time),&TempTime1,sizeof(TempTime1));//备份TempTime到TempTime1
  for(i=0;i<HIS_ENERGY_DATA_MONTHS;i++)
  {
    //读电量数据,知道读取到一份正确电量，认为不需要补冻数据了    
    OS_Sem_Pend(PUB_BUF_SEM_ID);//申请Pub_Buf的信号量
    Len=Read_HIS_Data(HIS_ENERGY_DATA,&TempTime,(void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),&Err);
    OS_Sem_Post(PUB_BUF_SEM_ID);//释放Pub_Buf的信号量
    
    if(ENERGY_SAVE_SIZE!=Len)
    {
      SET_STRUCT_TIME(Cur_Energy,TempTime);//设置电量数据时间为TempTime
      DEBUG_PRINT(HUCK,DEBUG_0,"Freeze Energy Data,Time:");
      Debug_Print_HEX_Time(TempTime.Time);
      _Freeze_Energy_Data(&TempTime);     
    }
    else
      break;
    
    Time_Dec(&TempTime,1,UNIT_MON,&TempTime);//时间向前推1个月
    OS_TimeDly_Ms(20);//延时给其他任务
  }
  SET_STRUCT_TIME(Cur_Energy,Cur_Time0);//恢复电量时间
  DEBUG_PRINT(HUCK,DEBUG_0,"----------Freeze Energy Data End----------");   
}

//上电补结算(补冻)数据
void PowerOn_MonthData_Freeze()
{
  PowerOn_Energy_Data_Freeze();//电量数据
  PowerOn_Demand_Data_Freeze();//需量数据
  PowerOn_VolStat_Data_Freeze();//电压统计数据
}

//掉电保存电量和需量数据等
//此函数因为是掉电存储，因此没有延时函数
void PowerDown_Save_Energy_Data()
{
  //保存掉电时存储的电量数据,没有读取成功则读取备份存储的数据 
  Write_Storage_Data(CUR_ENERGY_PD,(void *)(&Cur_Energy),ENERGY_SAVE_SIZE);

}

//掉电存数
void PowerDown_Save_Data()
{
  PowerDown_Save_Energy_Data();//保存电量数据
  PowerDown_Save_Demand_Data();//保存需量数据
  PowerDown_Save_VolStat_Data(); //保存电压统计数据
}

#pragma optimize=none
//将当前费率信息等打印出来
void Debug_Print_Cur_Energy_Info()
{ 
  static S_Int8U Min={CHK_BYTE,0,CHK_BYTE};
  static S_Int8U Counts={CHK_BYTE,0,CHK_BYTE};
  INT8U i,Re;
  
  if(Check_Debug_En()==0)//非工厂状态不打印
    return;
  
  if(Cur_Time0.Time[T_MIN]==Min.Var)//每分钟执行一次该函数
    return;

  Re=CHECK_STRUCT_VAR(Min);
  ASSERT(A_WARNING,1==Re);
  Re=CHECK_STRUCT_VAR(Counts);
  ASSERT(A_WARNING,1==Re);
  
  Min.Var=Cur_Time0.Time[T_MIN];
  
  Clr_Ext_Inter_Dog();
  DEBUG_PRINT(HUCK,DEBUG_0,"----------Cur_Rate_Info-----------");
  DEBUG_PRINT(HUCK,DEBUG_0,"Cur_Time:%d-%d-%d %d:%d,Week:%d",Cur_Time0.Time[T_YEAR],Cur_Time0.Time[T_MONTH],\
              Cur_Time0.Time[T_DATE],Cur_Time0.Time[T_HOUR],Cur_Time0.Time[T_MIN],Cur_Time0.Week);
  DEBUG_PRINT(HUCK,DEBUG_1,"Cur Rate Scheme:%d",Cur_Rate_Info.Scheme);
  DEBUG_PRINT(HUCK,DEBUG_1,"Cur Rate:%d",Cur_Rate_Info.Rate);
  
  if(Cur_Rate_Info.Date_Flag==HOLIDAY)
    DEBUG_PRINT(HUCK,DEBUG_1,"Today is Holiday,Holiday_Index=%d",Cur_Rate_Info.Holiday_Index);
  else if(Cur_Rate_Info.Date_Flag==WEEK_REST_DAY)
    DEBUG_PRINT(HUCK,DEBUG_1,"Today is Week_Rest_Day");
  else if(Cur_Rate_Info.Date_Flag==NORMAL_DAY)
    DEBUG_PRINT(HUCK,DEBUG_1,"Today is Normal_Day,Year_Peirod=%d",Cur_Rate_Info.Year_Period);
  else
    DEBUG_PRINT(HUCK,DEBUG_1,"Today Date_Flag Err");
  
  //当前日时段表号和当前日时段
  DEBUG_PRINT(HUCK,DEBUG_1,"Cur Date_Table:%d Date_Period:%d",\
    Cur_Rate_Info.Date_Table,Cur_Rate_Info.Date_Period);
  //当前正向有功总
  DEBUG_PRINT(HUCK,DEBUG_1,"Cur_Pos_Active:%ld",Cur_Energy.Pos_Active[0]);
 
  Counts.Var++;
  if(Counts.Var>=5)//每隔5分钟打印一次当前电能值
  {
    Counts.Var=0;
    
    Clr_Ext_Inter_Dog();
      DEBUG_PRINT(HUCK,DEBUG_1,"|-----------------------------Total_Energy-------------------------|"); 
      DEBUG_PRINT(HUCK,DEBUG_1,"| Rate Pos_Activ Neg_Activ Quad0_Rea Quad1_Rea Quad2_Rea Quad3_Rea |");
    for(i=0;i<=Multi_Rate_Para.Rates;i++)
    {
      DEBUG_PRINT(HUCK,DEBUG_1,"|   %2d %9ld %9ld %9ld %9ld %9ld %9ld |",\
        i,Cur_Energy.Pos_Active[i],Cur_Energy.Neg_Active[i],\
        Cur_Energy.Quad_Reactive[0][i],Cur_Energy.Quad_Reactive[1][i],Cur_Energy.Quad_Reactive[2][i],Cur_Energy.Quad_Reactive[3][i]);
    }
  
    //打印三相电能
      DEBUG_PRINT(HUCK,DEBUG_1,"|------------------------------------------------------------------|"); 
      
    OS_TimeDly_Ms(50);
   
    Clr_Ext_Inter_Dog();    
      DEBUG_PRINT(HUCK,DEBUG_1,"|-----------------------------Phase_Energy-------------------------|"); 
      DEBUG_PRINT(HUCK,DEBUG_1,"| Phas Pos_Activ Neg_Activ Quad0_Rea Quad1_Rea Quad2_Rea Quad3_Rea |");

    for(i=0;i<3;i++)
    {
      DEBUG_PRINT(HUCK,DEBUG_1,"|    %c %9ld %9ld %9ld %9ld %9ld %9ld |",\
        (INT8S)('A'+i),Cur_Energy.Phase_Pos_Active[i],Cur_Energy.Phase_Neg_Active[i],\
        Cur_Energy.Phase_Quad_Reactive[i][0],Cur_Energy.Phase_Quad_Reactive[i][1],\
        Cur_Energy.Phase_Quad_Reactive[i][2],Cur_Energy.Phase_Quad_Reactive[i][3]);
         
    }
      DEBUG_PRINT(HUCK,DEBUG_1,"|------------------------------------------------------------------|"); 

   }

}

//初始化电量和需量相关参数
void Read_Energy_Para_From_Rom()
{
  INT8U Err;
  INT16U Len;
  
  //读取自动抄表日
  Len=Read_Storage_Data(0xC117,(INT8U *)&Data_Freeze_Time,(INT8U *)&Data_Freeze_Time,sizeof(Data_Freeze_Time.DDHH),&Err);
  if(ASSERT(A_WARNING,2==Len && NO_ERR==Err))
    DEBUG_PRINT(HUCK,DEBUG_0,"Read Data_Freeze_Time Error"); 
  
  if(!(Check_BCD_Data((INT8U *)Data_Freeze_Time.DDHH,2) &&
       Data_Freeze_Time.DDHH[0]<0x24 && Data_Freeze_Time.DDHH[1]>0 && Data_Freeze_Time.DDHH[1]<0x32))
  {
    Read_Def_Para(0xC117,(void *)Data_Freeze_Time.DDHH,2,(void *)Data_Freeze_Time.DDHH,sizeof(Data_Freeze_Time.DDHH));
    //mem_cpy((void *)Data_Freeze_Time.DDHH,Def_C117,2,(void *)Data_Freeze_Time.DDHH,sizeof(Data_Freeze_Time.DDHH));
  }
  Bcd2Hex_Buf((INT8U *)&Data_Freeze_Time,S_OFF(S_Data_Freeze_Time,CS),(INT8U *)&Data_Freeze_Time,(INT8U *)&Data_Freeze_Time,S_OFF(S_Data_Freeze_Time,CS));

  SET_STRUCT_SUM(Data_Freeze_Time);  

  //以下部分读取套费率相关参数,读取了Multi_Rate_Para,Year_Table，Holiday_Table
  Calc_Cur_Rate_Scheme(CALC_ROM);//计算当前套费率方案,CALC_ROM标志表示从ROM中读取参数重新判断
  OS_TimeDly_Ms(100);
  Calc_Cur_Rate(CALC_ROM);//计算当前应该使用的费率
}

void Clear_All_Data()
{
  Set_Def_Energy_Data();
  Set_Def_Demand_Data();  
}

//清电量数据
void Clear_Energy_Data()
{
  if(Check_Clear_Data_Authority()==0)
    return;

  if(CLEAR_ENERGY_FLAG!=Check_Clear_Data_Flag() &&
     CLEAR_ALL_FLAG!=Check_Clear_Data_Flag())
    return;
  
  DEBUG_PRINT(HUCK,DEBUG_0,"Clear_Energy_Data!!!");   
  Set_Def_Energy_Data();
}

//检查某参数是否是电能任务相关参数
INT8U Check_If_Energy_Task_Para(PROTO_DI DI)
{
  if(DI==0xC010 || DI==0xC011)//日期或时间独立处理了Time_Modify_Flag
    return 0;
  else if(DI>=0xC030 && DI<=0xC034)//有功脉冲常数,无功脉冲常数 表号 用户号设备码
    return 0;
  else if(0xC211==DI || 0xC215==DI || 0xC216==DI)//跳合闸延时
    return 0;
  //else if((DI & 0xFF00)==0xC400 && DI!=0xC41E)//第一套公共节假日(但不包括周休日对应日时段表0xC41E)
    //return 0;
  //else if((DI & 0xFF00)==0xCC00 && DI!=0xCC1E)//第二套公共节假日(但不包括周休日对应日时段表0xCC1E)
    //return 0;
  else if(ADJ_METER_PUL_VOL_CUR_SPEC<=DI && DI<=ADJ_METER_C_CUR)
    return 0;
  else if((0xC113<=DI && DI<=0xC116) || (0xC123<=DI && DI<=0xC126) || 0xC128==DI)//循显时间和显示项目数
    return 0;
  else if(0xC030==DI || 0xC031==DI)//脉冲常数
    return 0;
  else if((DI & 0xFF00)==0xC600)//A、B屏显示项
    return 0;
  
  return 1;
}


//检查参数是否发生修改
void Check_Energy_Para_Modified()
{
  PROTO_DI DI;
  INT8U Re;
  static S_Int32U Sec_Bak0={CHK_BYTE,0xFF,CHK_BYTE};
  static S_Int32U Sec_Bak={CHK_BYTE,0xFF,CHK_BYTE};
  static S_Int8U Count={CHK_BYTE,0,CHK_BYTE};
  static S_Int8U Init_Para_Flag={CHK_BYTE,0,CHK_BYTE};

  if(Sec_Bak0.Var==Sec_Timer_Pub)//每秒才进来查一次
    return;

  Sec_Bak0.Var=Sec_Timer_Pub;
  Re=1;
  Re&=CHECK_STRUCT_VAR(Sec_Bak);
  Re&=CHECK_STRUCT_VAR(Count);
  Re&=CHECK_STRUCT_VAR(Init_Para_Flag);
  ASSERT(A_WARNING,1==Re);
  
  OS_TimeDly_Ms(20);
//对设置过的参数逐个处理  
  while(Get_Para_Modify(&DI)==1)
  {
    if(Check_If_Energy_Task_Para(DI))//如果是当前任务相关的参数则需要重新初始化参数
    {
      Init_Para_Flag.Var=1;//需要重新初始化参数 
      Sec_Bak.Var=Sec_Timer_Pub;//秒计时器    
      Count.Var=0;//重新开始计数,10S后仍无参数设置则，重新初始化电量相关参数
    }
  }
  
  //是否需要重新初始化?
  if(1==Init_Para_Flag.Var && Sec_Bak.Var!=Sec_Timer_Pub)
  {
    Sec_Bak.Var=Sec_Timer_Pub;
    Count.Var++;//秒计数
  }
  
  //修改当前任务相关参数后10S，重新初始化参数
  if(Count.Var>=1 && 1==Init_Para_Flag.Var)
  {
    //设置了电量相关参数
    Init_Para_Flag.Var=0;
    Count.Var=0;
    Refresh_Para_From_Rom();//重新将
  }
  
}

//初始化电量和需量相关的参数
void Init_Energy_Demand_Ram_Para()
{
  //初始化信号量

  //初始化费率状态字
  Rate_Status.Status[0].Byte=0x01;//套费率方案0
  Rate_Status.Status[1].Byte=0x01;//当前费率为费率1
  SET_STRUCT_SUM(Rate_Status);//费率状态字
  //初始化费率信息
  mem_set((void *)&Cur_Rate_Info,0,sizeof(Cur_Rate_Info),(void *)&Cur_Rate_Info,sizeof(Cur_Rate_Info));
  SET_STRUCT_SUM(Cur_Rate_Info);//费率信息
  //初始化日时段表
  mem_set((void *)&Date_Table,0,sizeof(Date_Table),(void *)&Date_Table,sizeof(Date_Table));
  SET_STRUCT_SUM(Date_Table);//日时段表
  
  //初始化参数修改标志
  //Clear_Flag.Flag=CLEAR_END_FLAG;//清除标志,清电量需量等的标志
  //SET_STRUCT_SUM(Clear_Flag);
  
  //两个全局缓冲区的头尾检验字节
  INIT_STRUCT_VAR(_Pub_Buf);
  INIT_STRUCT_VAR(_Pub_Buf0);
  INIT_STRUCT_VAR(Pub_Timer);
  Energy_Cumu_Counts.Var=0; //电量累加次数,供测试程序用
  INIT_STRUCT_VAR(Energy_Cumu_Counts);
  
  Chk_Data_Flag.Chk_Flag=0;
  SET_STRUCT_SUM(Chk_Data_Flag);
}

void Init_Sem()
{
  //初始化信号量
  OS_Sem_Clr();
  OS_Sem_Init(PUB_BUF_SEM_ID,0);//初始化Pub_Buf公共信号量
  OS_Sem_Init(PUB_RW_ROM_ID,0);//初始化读写ROM的信号量  
}
//初始化内存的相关参数
/*
void Init_Ram_Para_()
{
  Init_Sem();//初始化信号量
  Init_Clock_Ram_Para();//初始化始终内存
  Init_Energy_Demand_Ram_Para();//初始化电量需量相关内存
  Init_VolStat_Ram_Para();
  Init_Data_Trans_Ram_Para();//数据传输相关变量初始化
}
*/

//初始化电量相关参数
void Read_Para_From_Rom()
{
  Read_Energy_Para_From_Rom();//初始化电能累计相关参数
  Read_Demand_Para_From_Rom();//初始化需量相关参数
  Read_Data_Trans_Para_From_Rom();//数据转换需要的相关参数初始化 
  Read_VolStat_Para_From_Rom();//电压统计相关参数
  Read_LoadData_Para_From_Rom();//负荷曲线相关参数
  Read_Power_Ctrl_Para_From_Rom();//读取负荷控制参数
  Read_Freeze_Data_Para_From_Rom();
}

void Refresh_Para_From_Rom()
{
  Read_Energy_Para_From_Rom();//初始化电能累计相关参数
  OS_TimeDly_Ms(100);
  Read_Demand_Para_From_Rom();//初始化需量相关参数
  OS_TimeDly_Ms(100);
  Read_Data_Trans_Para_From_Rom();//数据转换需要的相关参数初始化
  OS_TimeDly_Ms(100);
  Read_VolStat_Para_From_Rom();//电压统计相关参数
  OS_TimeDly_Ms(100);
  Read_LoadData_Para_From_Rom();//负荷曲线相关参数
  OS_TimeDly_Ms(100);
  Read_Power_Ctrl_Para_From_Rom();//读取负荷控制参数
  OS_TimeDly_Ms(100);
  Read_Freeze_Data_Para_From_Rom();  
  
}

//密码锁定计时处理
void PSW_Lock_Proc()
{
  INT8U Re;
  static S_Int8U Min_Bak={CHK_BYTE,0xFF,CHK_BYTE};
  
  if(Min_Bak.Var!=Cur_Time0.Time[T_MIN])
  {
    Min_Bak.Var=Cur_Time0.Time[T_MIN];
    
    Re=CHECK_STRUCT_SUM(PSW_Err_Info);
    if(1!=Re || PSW_Err_Info.Err_Times>99 ||\
      PSW_Err_Info.Lock_Mins>1440 || PSW_Err_Info.Clr_Err_Times_Mins>1440)
      Clear_PSW_Err_Info();

    if(Check_Meter_Factory_Status())//在工厂状态清除密码锁
    {
      Clear_PSW_Err_Info();
      Clr_Event_Instant(ID_EVENT_METER_LOCK);
    }
    
    if(PSW_Err_Info.Lock_Mins>0)//锁定时间递减
    {
      Set_Event_Instant(ID_EVENT_METER_LOCK);  //密码锁事件发生
      PSW_Err_Info.Lock_Mins--;
      if(PSW_Err_Info.Lock_Mins==0)
      {
        PSW_Err_Info.Err_Times=0;//锁定次数清0
        Clr_Event_Instant(ID_EVENT_METER_LOCK);
      }
    }
    
    if(PSW_Err_Info.Clr_Err_Times_Mins>0)//1440分钟后错误次数自动清0
    {
      PSW_Err_Info.Clr_Err_Times_Mins--;
      if(PSW_Err_Info.Clr_Err_Times_Mins==0)
        PSW_Err_Info.Err_Times=0;
    }
    
    if(PSW_Err_Info.Set_En_Mins>0)//开放设置功能时间
    {
      PSW_Err_Info.Set_En_Mins--;
    }
    
    SET_STRUCT_SUM(PSW_Err_Info);
  }
} 
//检查是否有清数据需要
//清数据前需要调用Set_Clear_Data_Flag函数进行设置
void Check_Clear_Data()
{
  INT32U Flag;
  
  Check_Man_Clr_Demand();//手动清需量检查
  
  Flag=Check_Clear_Data_Flag();//CHECK_STRUCT_SUM(Clear_Flag);
  //清电量
  if(CLEAR_ALL_FLAG==Flag)
  {
    Clear_Energy_Data();
    Clear_All_Demand_Data();
    //Clear_VolStat_Data();
    //Clear_Event_Data();
    Clear_Data_End();//清数据完成
    OS_TimeDly_Ms(100);//让出CPU给通信任务    
  }
  else if(CLEAR_ENERGY_FLAG==Flag)
  {
    Clear_Energy_Data();
    Clear_Data_End();//清数据完成
    OS_TimeDly_Ms(100);//让出CPU给通信任务
    //DEBUG_PRINT(HUCK,DEBUG_0,"Clear_Energy_Data");
  }
  //清需量
  else if(CLEAR_DEMAND_FLAG==Flag)
  {
    Clear_Cur_Demand_Data();
    Clear_Data_End();//清数据完成
    OS_TimeDly_Ms(100);//让出CPU给通信任务 
  }
  //清电压统计
  else if(CLEAR_VOLSTAT_FLAG==Flag)
  {
    Clear_VolStat_Data();
    Clear_Data_End();//清数据完成
    OS_TimeDly_Ms(100);//让出CPU给通信任务
    //DEBUG_PRINT(HUCK,DEBUG_0,"Clear_VolStat_Data");     
  }
  else if(CLEAR_EVENT_FLAG==Flag)
  {
    Clear_Event_Data(); 
    Clear_Data_End();//清数据完成
    OS_TimeDly_Ms(100);//让出CPU给通信任务
  }
}

void Energy_Task_Ram_Init()
{
  Init_Sem();//初始化信号量
  Init_Clock_Ram_Para();//初始化始终内存
  Init_Energy_Demand_Ram_Para();//初始化电量需量相关内存
  Init_VolStat_Ram_Para();//电压统计变量初始化
  Init_Data_Trans_Ram_Para();//数据传输相关变量初始化  
  Init_Low_Power_Ram();
}

extern INT8U Check_Imp_Data_Storage();
//电量和需量的任务的初始化,该初始化只在主任务中调用，在各自任务创建前调用
void Energy_Demand_Task_Init()
{
  if(Check_Sys_Status()==SYS_NORMAL)//系统电源正常的情况下才进行数据冻结和需量处理
  {
   //Check_Imp_Data_Storage();
    //Check_Sys_Storage();//存储器自检
    //Check_Boot_On_Flag();//判断是否是第一次上电，是则设置默认参数
    //Init_Clock();//初始化时钟
    Read_Para_From_Rom();//读取存储在ROM中的参数,此函数前需要先读取到时钟!
    Clock_Update();//重新获取时间,因为前面的读函数会用掉很多时间
  }
  else
  {
    //在唤醒的情况下没有读取参数，但是有如下两类参数需要读取
    Read_Mode_Word();//模式字
    Read_Data_Trans_Para_From_Rom();//数据转换相关参数
  }
}

void Task_Switch_Hook()
{
  static INT32U Ms10_Timer=0;
  static INT32U Max=0;
  static INT32U Temp=1000;

  if(Ms10_Timer_Pub>Ms10_Timer && Ms10_Timer_Pub-Ms10_Timer>Max)
    Max=Ms10_Timer_Pub-Ms10_Timer;
  
  if(Max>Temp)
    Max=Max;
  Ms10_Timer=Ms10_Timer_Pub;
}

//打印电量需量信息
void Print_Energy_Demand_Info()
{
  Debug_Print_Cur_Energy_Info(); 
  Debug_Print_Cur_Demand_Info();
}
/*
void Test_Cur_Demand_Rate_Data()
{
  INT8U i;

  for(i=0;i<4;i++)
  {
    Cur_Demand.Rate=i+1;
    Save_Cur_Demand_Data();    
  }

  for(i=0;i<4;i++)
  {
    Cur_Demand.Rate=i+1;
    Read_Demand_Rate_Data(i+1,(void *)&Cur_Demand.Demand[0],(void *)&Cur_Demand.Demand[0],sizeof(Cur_Demand.Demand[0]));    
  }  
}
*/
//电量需量任务
//其实除了计算电量、需量等还做了电压统计、负荷曲线、时钟处理等工作
void Energy_Demand_Task(INT8U Flag)
{
  if(Flag!=0)
  {
    PowerOn_Read_Data_From_Rom();//电量数据读取初始化
    if(Check_Sys_Status()==SYS_NORMAL)//系统电源正常的情况下才进行数据冻结和需量处理
    {
      PowerOn_MonthData_Freeze();//进行月数据结算，前HIS_ENERGY_ENERGY_DATA_MONTHS个月的数据  
      PowerOn_Demand_Proc(); //上电后的需量计算
        
      OS_Waitfor_Sec(1==Measu_Status_Mode_ToPub_PUCK.DataRecFlg,5);//等待计量数据准备好！
      if(1!=Measu_Status_Mode_ToPub_PUCK.DataRecFlg)
      {
        ASSERT(A_WARNING,0);
        OS_Waitfor(1==Measu_Status_Mode_ToPub_PUCK.DataRecFlg);
      }
    }

////////////////////////
/*
    INT8U a=0;  
    while(a<=50)
    {
      Event_Data_Proc(a,EVENT_OCCUR);
      Clear_Task_Dog();
      OS_TimeDly_Ms(100);
      Event_Data_Proc(a,EVENT_END);
      DEBUG_PRINT(HUCK,DEBUG_0,"a=%d",a);
      a++;      
    }
*/
/////////////////////////    
    return;
  }
  else
  {
    if(Check_Sys_Status()!=SYS_NORMAL)//任务在唤醒的情况下，不做任何事情 
    {
      while(1)
      Task_Sleep();
    }
    else
    {
      //Clear_Task_Dog();//清任务软狗
  
      Clock_Proc();//时钟处理，定时刷新时钟
      Check_Energy_Para_Modified();//检查参数是否发生修改
  
      PSW_Lock_Proc();//密码锁定计时
      Check_Energy_Demand_Task_Data_Avail();//检查本任务所用数据的合法性
      
      //Check_Cur_Rate应该放在Month_Data_Freeze和Auto_Save_Cur_Data函数的后面，保证的跨分时，该函数先执行
      Check_Cur_Rate();//检查当前费率
      
      Month_Data_Freeze();//每月自动抄表结算
      Auto_Save_Cur_Data();//定期自动保存当前数据
      
      Cumulative_Energy();//每秒累加电量
  
      Demand_Proc();//需量数据处理

      VolStat_Proc();//电压统计处理
      LoadData_Proc();//负荷曲线处理
      Period_Freeze_Data_Proc();//周期冻结数据
      Check_Clear_Data();//检查是否清除电量需量数据

      Power_Ctrl_Proc();//负荷控制处理
      
      Event_Cumu_Mins_Proc();//事件分累计
      Extend_Main_Proc();
      //Debug_Print_Cur_Energy_Info();//将当前费率信息输出到调试口
      Print_Energy_Demand_Info();
    }
  }
 
}

//检查当此是否是掉电后复位的，如果是则进行掉电数据保存
/*
void Last_Power_Down_Proc()
{
  //INT32U Status;

  if(Check_Sys_Status()==SYS_NORMAL_TO_SLEEP)
  {
    PowerDown_Save_Data();//掉电存数
    //保存报警数据,PUCK提供函数
  }
}
*/
/*
INT8U Temp[1000];
void Energy_Demand_Task()
{
  INT16U Len,DI;
  INT8U Err;
  
  DI=0x7000;
  Multi_Rate_Para.Rates=8;
  INT8U i;
  for(i=0;i<=8;i++)
  {
    Cur_Energy.Pos_Active[i]=i*111;
    Cur_Energy.Neg_Active[i]=i*1111; 
    Cur_Energy.Quad_Reactive[0][i]=i*1111;
    Cur_Energy.Quad_Reactive[1][i]=i*2222;
    Cur_Energy.Quad_Reactive[2][i]=i*3333;
    Cur_Energy.Quad_Reactive[3][i]=i*4444;
  }
  
  for(i=0;i<3;i++)
  {
    Cur_Energy.Phase_Pos_Active[i]=i*111;
    Cur_Energy.Phase_Neg_Active[i]=i*222;
    Cur_Energy.Phase_Pos_Reactive[i]=i*333; 
    Cur_Energy.Phase_Neg_Reactive[i]=i*444;
  }
  Len=sizeof(Cur_Energy.Time);
  printf("Len=%d",Len);
  while(1)
  {
    
    Len=Get_DLT645_Data(DI,Temp,Temp,sizeof(Temp),&Err);
    if(Len>0)
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Get 0x%x Data Len=%d",Len);
      DEBUG_BUF_PRINT(Temp,Len,PRINT_HEX,8);
    }
  }
}
*/

#undef ENERGY_C
