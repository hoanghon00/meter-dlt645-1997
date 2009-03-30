#define DEMAND_C
#include "Includes.h"

#define DEMAND_PERIOD_OUTPUT 0
#define SLIDE_PERIOD_OUTPUT 1
//需量输出是按需量周期还是按滑差周期输出?
#define DEMAND_OUTPUT_MODE (Mode_Word.Mode[2].Bit.Bit4==1?DEMAND_PERIOD_OUTPUT:SLIDE_PERIOD_OUTPUT)
/*
需量和电压统计等月数据冻结的相关问题
1 冻结完成后，将当前数据存储区的数据全部写为0，表示当前数据区的数据为空，尚未写入当月（新的一月）的数据
2 冻结时间点，不自动存数，避免可能的数据乱(实际上应该不可能出问题)
3 如果在冻结后马上复位了，那么重启后，读取当前数据区全为0，不会补冻，如果不慎进入了补冻流程，由于可以读出前月历史数据，所以还是不会补冻
4 如果在冻结前一刻复位了，重启后，仍然会按照历史数据进行冻结。
5 如果在冻结的过程中复位了,数据不能形成校验和,当前数据也没有重置为0，所以上电后还是会补冻
*/

//初始化需量相关参数
void Read_Demand_Para_From_Rom()
{
  INT8U Err;
  INT16U Len;
  //读取需量周期
  Len=Read_Storage_Data(0xC111,(INT8U *)&Demand_Para.Period,(INT8U *)&Demand_Para.Period,1,&Err);
  ASSERT(A_WARNING,1==Len && NO_ERR==Err);
  Demand_Para.Period=Bcd2Hex_Byte(Demand_Para.Period);
  //读取需量滑差时间
  Len=Read_Storage_Data(0xC112,(INT8U *)&Demand_Para.Slide_Time,(INT8U *)&Demand_Para.Slide_Time,1,&Err);
  ASSERT(A_WARNING,1==Len && NO_ERR==Err);
  Demand_Para.Slide_Time=Bcd2Hex_Byte(Demand_Para.Slide_Time);
  
  SET_STRUCT_SUM(Demand_Para);  
}

//检查需量参数是否合法
void Check_Demand_Para_Avail()
{
  INT8U Re;
  Re=CHECK_STRUCT_SUM(Demand_Para);//需量参数
  if(ASSERT(A_WARNING,1==Re))
  {
     Read_Demand_Para_From_Rom();    
  }
}

//清除某一个需量单元数据
void Clr_Demand_Unit(INT8U Index)
{
  //将该功率累加单元清0
  Demand_Accu.Pos_Active[Index]=0;
  Demand_Accu.Neg_Active[Index]=0;
  Demand_Accu.Pos_Reactive[Index]=0;
  Demand_Accu.Neg_Reactive[Index]=0;     
  Demand_Accu.Counts[Index]=0;
  Demand_Accu.Mins[Index]=0;
  SET_STRUCT_SUM(Demand_Accu);
}

//判断需量周期和滑差时间必须满足的条件
//必须满足滑差时间=1,2,3,5,10,需量周期/滑差时间<MAX_PERIOD_SLIDE_RATIO(15)
void Check_Demand_Para()
{
  if(ASSERT(A_WARNING,
            (Demand_Para.Period%Demand_Para.Slide_Time==0 &&\
             Demand_Para.Period/Demand_Para.Slide_Time>=1 &&\
             Demand_Para.Period/Demand_Para.Slide_Time<=MAX_PERIOD_SLIDE_RATIO)))
  {
    //如果条件不成立，默认为1分钟滑差时间和15分钟需量周期
    //ASSERT(A_WARNING,0);
    DEBUG_PRINT(HUCK,DEBUG_0,"Demand_Para error");
    Demand_Para.Slide_Time=1;
    Demand_Para.Period=MAX_PERIOD_SLIDE_RATIO; 
    SET_STRUCT_SUM(Demand_Para);
  }  
  
}

//需量累加计算,每秒调用一次该函数
//该函数对每个功率累加单元进行累加，并根据当前时间决定是否产生一次需量
void Demand_Calc()
{
  INT32U Counts;
  INT16U Mins;
  INT8U i,j,Re;
  static S_Int8U Min_Bak={CHK_BYTE,0xFF,CHK_BYTE};

  Re=CHECK_STRUCT_SUM(Demand_Para);
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();
  
  Re=CHECK_STRUCT_SUM(Demand_Accu);
  if(ASSERT(A_WARNING,0!=Re))//检查已经统计过的需量数据是否正确
  {
    mem_set((void *)&Demand_Accu,0,sizeof(Demand_Accu),(void *)&Demand_Accu,sizeof(Demand_Accu));
    SET_STRUCT_SUM(Demand_Accu);
  }

  if(ASSERT(A_WARNING,Demand_Para.Slide_Time!=0))//保证Demand_Accu.Slide_Time!=0
  {
    Demand_Para.Slide_Time=1;
    SET_STRUCT_SUM(Demand_Para);
  }
  
  Check_Demand_Para();

  Re=1;
  Re &= CHECK_STRUCT_SUM(Measu_Status_Mode_ToPub_PUCK);
  Re &= CHECK_STRUCT_SUM(Measu_InstantData_ToPub_PUCK);
  ASSERT(A_WARNING,1==Re);
  if(1==Re)
  {
    //对每个滑动窗口(功率累加单元)进行累加
    for(i=0;i<MAX_PERIOD_SLIDE_RATIO;i++)
    {
      //累加功率,原数据是6位小数，实际使用4为小数，因此要除以POWER_DATA_RATIO
      if(Mode_Word.Mode[0].Bit.Bit6==1)//单向表,如果是单向表，正反功率全部计入正向
      {
        Demand_Accu.Pos_Active[i]+=(Measu_InstantData_ToPub_PUCK.AcPower.Sum/POWER_DATA_RATIO);
      }
      else//双向表,正反功率独立计
      {
        if(Measu_Status_Mode_ToPub_PUCK.SUM_APwrDir==POWER_DIRECT_N)//正向有功功率
          Demand_Accu.Pos_Active[i]+=(Measu_InstantData_ToPub_PUCK.AcPower.Sum/POWER_DATA_RATIO);
        else   //反向有功功率
          Demand_Accu.Neg_Active[i]+=(Measu_InstantData_ToPub_PUCK.AcPower.Sum/POWER_DATA_RATIO);
      }
      //1,2象限无功计入正向无功
      if(QUADRANT1==Measu_InstantData_ToPub_PUCK.Quadrant.Sum ||\
         QUADRANT2==Measu_InstantData_ToPub_PUCK.Quadrant.Sum)//正向无功功率
        Demand_Accu.Pos_Reactive[i]+=(Measu_InstantData_ToPub_PUCK.ReacPower.Sum/POWER_DATA_RATIO);
      //3,4象限无功计如反向无功
      else if(QUADRANT3==Measu_InstantData_ToPub_PUCK.Quadrant.Sum ||\
              QUADRANT4==Measu_InstantData_ToPub_PUCK.Quadrant.Sum)//正向无功功率
        Demand_Accu.Neg_Reactive[i]+=(Measu_InstantData_ToPub_PUCK.ReacPower.Sum/POWER_DATA_RATIO);
      else if(NO_QUADRANT!=Measu_InstantData_ToPub_PUCK.Quadrant.Sum)
        ASSERT(A_WARNING,0);
      
      Demand_Accu.Counts[i]++;
      SET_STRUCT_SUM(Demand_Accu);
    }
  }
  
  //每分钟判下是否到了出需量时间
  Re=CHECK_STRUCT_VAR(Min_Bak);
  ASSERT(A_WARNING,1==Re);
  if(Min_Bak.Var!=Cur_Time0.Time[T_MIN])
  {
    Min_Bak.Var=Cur_Time0.Time[T_MIN];  
    Mins=(INT16U)Cur_Time0.Time[T_HOUR]*60+(INT16U)Cur_Time0.Time[T_MIN];
    
    if(Demand_Para.Period!=0 && (Mins%Demand_Para.Period)==0)//需量周期到
    {
      Set_Event_Instant(ID_DEMAND_ARRIVE);//需量周期
      //if(DEMAND_OUTPUT_MODE EQ DEMAND_PERIOD_OUTPUT)
      {
        Port_Out_Pub(EXT_ID_DEMAND,85);
        DEBUG_PRINT(HUCK,DEBUG_0,"Demand Period Arrived!!!"); 
      }
    }
    
    //每过一个滑差时间出一次需量,如果不是滑差周期，则清除一个最大功率单元，保证不会技术
    Counts=0;
    i=0;
    
    if(0==Demand_Para.Slide_Time)
    {
      ASSERT(A_WARNING,0);
      Demand_Para.Slide_Time=1;
      SET_STRUCT_SUM(Demand_Para);
    }
    
    for(j=0;j<MAX_PERIOD_SLIDE_RATIO && j<(Demand_Para.Period/Demand_Para.Slide_Time);j++)
    {
      Demand_Accu.Mins[j]++;//分钟计数
      if(Counts<Demand_Accu.Counts[j])//找到当前需量计数最大的那个单元来出需量
      {
        i=j;
        Counts=Demand_Accu.Counts[j];
      }
    }
    SET_STRUCT_SUM(Demand_Accu);
    
    if(Demand_Para.Slide_Time!=0 && (Mins%Demand_Para.Slide_Time)==0)//滑差周期到,出需量时间到
    {
      Set_Event_Instant(ID_SLIP_ARRIVE);//滑差周期
      //if(DEMAND_OUTPUT_MODE EQ SLIDE_PERIOD_OUTPUT)
      {      
        Port_Out_Pub(EXT_ID_SLIPE,85);
        DEBUG_PRINT(HUCK,DEBUG_0,"Demand Slide Period Arrived!!!"); 
      }
      //计算平均值，也就是需量
      //if(Counts>=(INT32U)Demand_Para.Period*(60-10))//防止内部秒计数器的偏差故减去10S
      if(Demand_Accu.Mins[i]>=((INT32U)Demand_Para.Period))
      {
        Demand_Accu.Cur_Pos_Active=Demand_Accu.Pos_Active[i]/Counts;
        Demand_Accu.Cur_Neg_Active=Demand_Accu.Neg_Active[i]/Counts; 
        Demand_Accu.Cur_Pos_Reactive=Demand_Accu.Pos_Reactive[i]/Counts;
        Demand_Accu.Cur_Neg_Reactive=Demand_Accu.Neg_Reactive[i]/Counts;
        
        Mins=Demand_Accu.Mins[i];
        
        //将该功率累加单元清0        
        Clr_Demand_Unit(i);
        
        //打印需量相关调试信息
        DEBUG_PRINT(HUCK,DEBUG_0,"------Demand Update-----");
        DEBUG_PRINT(HUCK,DEBUG_0,"The %dth Unit make demand,Mins=%d,Counts=%d",\
                                  i,Mins,Counts);
        DEBUG_PRINT(HUCK,DEBUG_0,"Demand:Pos_Act=%d",Demand_Accu.Cur_Pos_Active);
        DEBUG_PRINT(HUCK,DEBUG_0,"Demand:Neg_Act=%d",Demand_Accu.Cur_Neg_Active);
        DEBUG_PRINT(HUCK,DEBUG_0,"Demand:Pos_React=%d",Demand_Accu.Cur_Pos_Reactive);
        DEBUG_PRINT(HUCK,DEBUG_0,"Demand:Neg_React=%d",Demand_Accu.Cur_Neg_Reactive);
      }
      else
        Clr_Demand_Unit(i);//此时应该是刚上电阶段
    }
    else//清除掉最大功率单元，防止连续计数超过一个需量周期
    {
        Clr_Demand_Unit(i);
    }
  }
}

//需量数据清0处理,在复位时调用该函数
//跨月时需要清需量累加中间单元？暂定为不清
void Demand_Accu_Clear()
{
  //INT8U i;
  //Mins=(INT16U)Cur_Time0.Time[T_HOUR]*60+(INT16U)Cur_Time0.Time[T_MIN];
  //i=(Mins/Demand_Para.Slide_Time)%(Demand_Para.Period/Demand_Para.Slide_Time);//第几个中间单元出需量

  mem_set((void *)&Demand_Accu,0,sizeof(Demand_Accu),(void *)&Demand_Accu,sizeof(Demand_Accu));
  
  //for(i=0;i<MAX_PERIOD_SLIDE_RATIO;i++)
    //Demand_Accu.Init_Flag[i]=1;//初始化标志置1，计算需量时可能使用该标志
  //重新根据当前时间计算每个功率累加单元应该走过的时间
  //Mins=(INT16U)Cur_Time0.Time[T_HOUR]*60+(INT16U)Cur_Time0.Time[T_MIN];
  //i=(Mins/Demand_Para.Slide_Time)%(Demand_Para.Period/Demand_Para.Slide_Time);//第几个中间单元出需量
 
  SET_STRUCT_SUM(Demand_Accu);

}

//需量清0，入口参数pTime表示清除的时间点,一般是当前时间
//清需量后要根据该时间点计算下次需量数据冻结的时间
void Demand_Clear(S_HEX_Time *pTime)
{
  //Demand_Accu_Clear();//清累加,跨月的情况下一定要清累加?
  mem_set((void *)&Cur_Demand,0,sizeof(Cur_Demand),(void *)&Cur_Demand,sizeof(Cur_Demand));//清需量
  SET_STRUCT_SUM(Cur_Demand.Demand[0]);
  SET_STRUCT_SUM(Cur_Demand.Demand[1]);
  SET_STRUCT_SUM(Cur_Demand);

}

//需量上电时的处理，主要是当表计睡眠时间少于一个需量周期时的处理
//初次上电时调用该函数，可以将Sleep_Secs值放入一个最大值
//待改
void PowerOn_Demand_Proc()
{
  /*
  if(Check_Rst_Flag()==SOFT_RST)//如果是软件复位，则不清楚中间累加单元
    Demand_Accu_Init(0);
  else
  {
    //此处需要获得睡眠时间的秒计数,并作为Demand_Accu_Init的入口参数
    Demand_Accu_Init(0xFFFFFFFF);//上电则清除累加单元

  }
  */
  Demand_Accu_Clear();
}

//功率累加单元初始化
//Sleep_Secs表示距离上一次需量累加已经走过了多长时间
/*
void Demand_Accu_Init(INT32U Sleep_Secs)
{
  INT32U Period;
  INT8U Re,i;
     
  Re=CHECK_STRUCT_SUM(Demand_Para);
  if(ASSERT(A_WARNING,Re>0))
    Check_Data_Avail();
  
  Period=(INT32U)(Demand_Para.Period)*60;//计算需量周期
  
  if(Sleep_Secs>=Period)//掉电时间超过一个需量周期，则要将需量数据清0
  {
    Demand_Accu_Clear();
  }
  else
  {   
    Re=CHECK_STRUCT_SUM(Mode_Word);
    //模式字校验和是否正确
    if(ASSERT(A_WARNING,0!=Re))
      Check_Data_Avail();

    //掉电需量不连续的话，请需量累加单元
    if(Mode_Word.Mode[2].Bit.Bit3==0)//掉电需量不连续
    {
      Demand_Accu_Clear();//清中间累加
    }
    else
    {
      Re=CHECK_STRUCT_SUM(Demand_Accu);
      if(ASSERT(A_WARNING,0!=Re))
         Demand_Accu_Clear();
      else
      {
        Re=CHECK_STRUCT_SUM(Demand_Para);
        if(ASSERT(A_WARNING,1==Re))
          Check_Data_Avail();
          
        for(i=0;i<MAX_PERIOD_SLIDE_RATIO;i++)//此处有待改，主要是看休眠情况下程序是否在跑
        {
          Demand_Accu.Counts[i]+=Sleep_Secs;
          
          //比较每个中间单元的计数，如果该计数超过了一个需量周期的时间，则认为
          //在停电期间，该中间单元会出需量一次
          //+2是为了防止出现掉电和上电正好在一个单元的出需量周期内,上电时间靠近出需量时刻
          //0-------------------------15
          //                  |pd    |po
          //这种情况很容易出现判断错误,因此增加2分钟,确定在掉电期间出过一次需量
          if(Demand_Accu.Counts[i]>=((INT32U)Demand_Para.Period+2)*60)//掉电期间肯定出过一次需量
          {
            Demand_Accu.Counts[i]=Demand_Accu.Counts[i]-(INT32U)Demand_Para.Period*60;
            Demand_Accu.Pos_Active[i]=0;//清中间单元
            Demand_Accu.Neg_Active[i]=0;
            Demand_Accu.Pos_Reactive[i]=0;
            Demand_Accu.Neg_Reactive[i]=0;
            //Demand_Accu.Init_Flag[i]=1;//初始化标志
          }
        }
        SET_STRUCT_SUM(Demand_Accu);
      }
    }
  }
}
*/

//需量周期的累加
void Demand_Counts_Init()
{
  INT8U i;
  
  for(i=0;i<MAX_PERIOD_SLIDE_RATIO;i++)
  {
    Demand_Accu.Counts[i]=(INT16U)Demand_Para.Slide_Time*60*i;//每个需量累加中间单元的个数累加 
  } 
}

//检查当前需量是否是最大需量
void Demand_Check_Max()
{
  INT8U i,Re,Cur_Rate,Max_Flag;
  INT8U TempTime[5];

  Re=1;
  Re&=CHECK_STRUCT_SUM(Cur_Time0);//检查当前需量累加单元
  Re&=CHECK_STRUCT_SUM(Demand_Accu);//检查当前需量累加单元
  Re&=CHECK_STRUCT_SUM(Cur_Demand);//检查当前需量
  Re&=CHECK_STRUCT_SUM(Cur_Rate_Info);//当前费率相关信息 
  Re&=CHECK_STRUCT_SUM(Multi_Rate_Para);//检查复费率相关参数
  
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();

  Max_Flag=0;//该标志为1表示产生了一次最大需量
  
  //正向有功总最大需量以其发生时间
  DIS_PD_INT;//更新需量数据时关掉电中断
  for(i=0;i<2;i++)
  {
    if(i==0)
      Cur_Rate=0;//计算总的最大值
    else
    {
      Cur_Rate=Cur_Rate_Info.Rate;//Get_Using_Rate();//计算当前费率的最大值
      //检查Cur_Rate的正确性
      if(ASSERT(A_WARNING,Cur_Rate>0 && Cur_Rate<=Multi_Rate_Para.Rates && Cur_Rate<=MAX_RATES))
        Cur_Rate=1;
    }
    //得到当前BCD时间
    Hex2Bcd_Buf((void *)Cur_Time0.Time,sizeof(Cur_Time0.Time),TempTime,TempTime,sizeof(TempTime));
    //当前需量比各费率最大需量大
    //正向有功总需量及其发生时间
    if(Demand_Accu.Cur_Pos_Active>Cur_Demand.Demand[i].Pos_Active)
    {
      Cur_Demand.Demand[i].Pos_Active=Demand_Accu.Cur_Pos_Active;
      mem_cpy((void *)Cur_Demand.Demand[i].Pos_Active_Time,\
              TempTime,\
              5,\
              (void *)Cur_Demand.Demand[i].Pos_Active_Time,\
              sizeof(Cur_Demand.Demand[i].Pos_Active_Time));
      SET_STRUCT_SUM(Cur_Demand);
      Max_Flag=1;
    }
    //反向有功总最大需量及其发生时间  
    if(Demand_Accu.Cur_Neg_Active>Cur_Demand.Demand[i].Neg_Active)
    {
      Cur_Demand.Demand[i].Neg_Active=Demand_Accu.Cur_Neg_Active;
      mem_cpy((void *)Cur_Demand.Demand[i].Neg_Active_Time,\
              TempTime,\
              5,\
              (void *)Cur_Demand.Demand[i].Neg_Active_Time,\
              sizeof(Cur_Demand.Demand[i].Neg_Active_Time));
      SET_STRUCT_SUM(Cur_Demand);
      Max_Flag=1;
    }
  //正向无功总最大需量及其发生时间  
    if(Demand_Accu.Cur_Pos_Reactive>Cur_Demand.Demand[i].Pos_Reactive)
    {
      Cur_Demand.Demand[i].Pos_Reactive=Demand_Accu.Cur_Pos_Reactive;
      mem_cpy((void *)Cur_Demand.Demand[i].Pos_Reactive_Time,\
              TempTime,\
              5,\
              (void *)Cur_Demand.Demand[i].Pos_Reactive_Time,\
              sizeof(Cur_Demand.Demand[i].Pos_Reactive_Time));
      SET_STRUCT_SUM(Cur_Demand);
      Max_Flag=1;
    } 
  //反向无功总最大需量及其发生时间 
    if(Demand_Accu.Cur_Neg_Reactive>Cur_Demand.Demand[i].Neg_Reactive)
    {
      Cur_Demand.Demand[i].Neg_Reactive=Demand_Accu.Cur_Neg_Reactive;
      mem_cpy((void *)Cur_Demand.Demand[i].Neg_Reactive_Time,\
              TempTime,\
              5,\
              (void *)Cur_Demand.Demand[i].Neg_Reactive_Time,\
              sizeof(Cur_Demand.Demand[i].Neg_Reactive_Time));
      SET_STRUCT_SUM(Cur_Demand);
      Max_Flag=1;
    }
  }
  EN_PD_INT;//开掉电中断
  
  //生成了一个最大需量数据
  if(Max_Flag==1)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Demand max data generated");
    //Save_Cur_Demand_PD_Data(); 
  }
}
//需量处理,包括每秒累加需量单元，以及与当前最大需量比较，生成最大值 
void Demand_Proc()
{
  INT8U Re;
  static S_Int32U Sec_Timer={CHK_BYTE,0x00,CHK_BYTE};
  
  Re=CHECK_STRUCT_VAR(Sec_Timer);
  ASSERT(A_WARNING,1==Re);
  
  if(Sec_Timer_Pub==Sec_Timer.Var)//时间还没有走过1S，该函数只允许1S进入一次下面的流程
    return;  
  Sec_Timer.Var=Sec_Timer_Pub;//更新秒计数器
  
  Demand_Calc();//每秒的需量累加以及每分钟的当前需量计算,以及每个滑差周期到后出需量 
  Demand_Check_Rate_Change();//检查当前费率是否发生了修改，如果修改了则要重新读取当前费率的需量数据
  Demand_Check_Max();//检查当前需量是否是最大需量
}

//保存当前需量数据
void Save_Cur_Demand_Data()
{
  INT8U Re;
  INT32U Off;

  Re=Check_Demand_Data((S_Demand *)&Cur_Demand);
  if(1==Re)
  {
    Re=Write_Storage_Data_Fix_Len(CUR_DEMAND,0,(void *)&Cur_Demand.Demand[0],ONE_DEMAND_SAVE_SIZE);
    ASSERT(A_WARNING,NO_ERR==Re);
    //再保存当前费率需量
    Off=DEMAND_RATE_OFF(Cur_Demand.Rate);
    Write_Storage_Data_Fix_Len(CUR_DEMAND,Off,(void *)&Cur_Demand.Demand[1],ONE_DEMAND_SAVE_SIZE);
    ASSERT(A_WARNING,NO_ERR==Re);
  }
  else
    ASSERT(A_WARNING,0);
}

//保存当前需量备份数据
void Save_Cur_Demand_Bak_Data()
{
  INT8U Re;
  INT32U Off;

  Re=Check_Demand_Data((S_Demand *)&Cur_Demand);
  if(1==Re)
  {
    Re=Write_Storage_Data_Fix_Len(CUR_DEMAND_BAK,0,(void *)&Cur_Demand.Demand[0],ONE_DEMAND_SAVE_SIZE);
    ASSERT(A_WARNING,NO_ERR==Re);
    //再保存当前费率需量
    Off=DEMAND_RATE_OFF(Cur_Demand.Rate);
    Write_Storage_Data_Fix_Len(CUR_DEMAND_BAK,Off,(void *)&Cur_Demand.Demand[1],ONE_DEMAND_SAVE_SIZE);
    ASSERT(A_WARNING,NO_ERR==Re);
  }
  else
    ASSERT(A_WARNING,0);
}

//保存当前需量掉电数据
void Save_Cur_Demand_PD_Data()
{
  INT8U Re;

  Re=Check_Demand_Data((S_Demand *)&Cur_Demand);
  if(1==Re)//检查需量数据的校验和，校验和错误则重新从ROM中读取
  {
    //mem_cpy((void *)Cur_Demand.Time,(void *)Cur_Time0.Time,sizeof(Cur_Time0.Time),(void *)Cur_Demand.Time,sizeof(Cur_Demand.Time));
    Re=Write_Storage_Data(CUR_DEMAND_PD,(void *)&Cur_Demand,sizeof(S_Demand));
    ASSERT(A_WARNING,NO_ERR==Re);
  }
  else
    ASSERT(A_WARNING,0);
}

//设置默认的需量数据
void Set_Def_Cur_Demand_Data()
{
  INT8U i,Re;
  INT32U Off;
  //将初始需量写入存储器中  
  DEBUG_PRINT(HUCK,DEBUG_0,"Set Def Demand Data");
  mem_set((void *)&Cur_Demand,0,sizeof(Cur_Demand),(void *)&Cur_Demand,sizeof(Cur_Demand));
  SET_STRUCT_SUM(Cur_Demand.Demand[0]);
  SET_STRUCT_SUM(Cur_Demand.Demand[1]);
  
  INIT_STRUCT_VAR(Cur_Demand);
  SET_DATA_READY_FLAG(Cur_Demand);
  
  SET_STRUCT_SUM(Cur_Demand); 
  
  //将各个费率的需量数据清0
  for(i=0;i<=MAX_RATES;i++)
  {
    Cur_Demand.Rate=i;
    SET_STRUCT_SUM(Cur_Demand);
    
    Off=DEMAND_RATE_OFF(Cur_Demand.Rate);
    Re=Write_Storage_Data_Fix_Len(CUR_DEMAND,Off,(void *)&Cur_Demand.Demand[0],ONE_DEMAND_SAVE_SIZE);
    ASSERT(A_WARNING,NO_ERR==Re);
    
    Off=DEMAND_RATE_OFF(Cur_Demand.Rate);
    Re=Write_Storage_Data_Fix_Len(CUR_DEMAND_BAK,Off,(void *)&Cur_Demand.Demand[0],ONE_DEMAND_SAVE_SIZE);
    ASSERT(A_WARNING,NO_ERR==Re);    
  }

  mem_set((void *)&Cur_Demand,0,sizeof(Cur_Demand),(void *)&Cur_Demand,sizeof(Cur_Demand)); 
  Cur_Demand.Rate=0;
  SET_STRUCT_SUM(Cur_Demand.Demand[0]);
  SET_STRUCT_SUM(Cur_Demand.Demand[1]);

  INIT_STRUCT_VAR(Cur_Demand);
  SET_DATA_READY_FLAG(Cur_Demand);

  SET_STRUCT_SUM(Cur_Demand);
  
  mem_set((void *)&Demand_Accu,0,sizeof(Demand_Accu),(void *)&Demand_Accu,sizeof(Demand_Accu));//清中间累加单元
  SET_STRUCT_SUM(Demand_Accu);
  
  Save_Cur_Demand_PD_Data();  
}

//设置默认的需量数据
void Set_Def_Demand_Data()
{
  INT8U i,j;
  INT32U Off;

  //设置当前需量数据
  Set_Def_Cur_Demand_Data();

  //设置历史需量数据
  for(i=0;i<HIS_DEMAND_DATA_MONTHS;i++)
  {
    for(j=0;j<MAX_RATES+1;j++)
    {
      Off=DEMAND_RATE_OFF(j);
      Write_Storage_Data_Fix_Len(HIS_DEMAND+i,Off,(void *)&Cur_Demand.Demand[0],ONE_DEMAND_SAVE_SIZE);    
    }
  }
}

//读取存在当前需量存储区中的总或分费率需量数据
//Rate:需要读取的需量的费率,0表示总
//pDst:目标缓冲区
//pDst_Start和DstLen一起限定pDst和返回长度的范围
//返回值:读取数据长度,读取失败返回0，成功返回ONE_DEMAND_SAVE_SIZE
INT16U Read_Demand_Rate_Data(INT8U Rate,void *pDst,void *pDst_Start,INT16U DstLen)
{
  INT8U Err;
  INT32U Off;

  Off=DEMAND_RATE_OFF(Rate);
  //先读取当前存储区
  if(Read_Storage_Data_Fix_Len(CUR_DEMAND,Off,ONE_DEMAND_SAVE_SIZE,pDst,pDst_Start,\
                               DstLen,&Err)!=ONE_DEMAND_SAVE_SIZE)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Read CUR_DEMAND CurRate data error");
    //再读取备份存储区
    OS_TimeDly_Ms(500);
    if(Read_Storage_Data_Fix_Len(CUR_DEMAND_BAK,Off,ONE_DEMAND_SAVE_SIZE,pDst,pDst_Start,\
                                 DstLen,&Err)!=ONE_DEMAND_SAVE_SIZE)
    {
        //该参数有默认参数项,实际上不会进入下面的括号
        DEBUG_PRINT(HUCK,DEBUG_0,"Read CUR_DEMAND_BAK Rate %d data Error",Rate);
        //如果所有的备份区数据都没读出来则认为是第一次上电，需量清0
        return 0;
    }
  }
  return ONE_DEMAND_SAVE_SIZE;
}

//检查需量数据是否合法
INT8U Check_Demand_Data(S_Demand *pDemand)
{
  INT8U i,Re;
  INT8U Temp[5]={0,0,0,0,0};

  Re=1;
  Re&=(pDemand->Head==CHK_BYTE && pDemand->Tail==CHK_BYTE)?1:0;  
  Re&=Check_STRUCT_Sum(pDemand,sizeof(S_Demand),pDemand->CS,sizeof(pDemand->CS));
  Re&=(pDemand->Data_Ready_Flag==DATA_READY)?1:0;  
  if(ASSERT(A_WARNING,0!=Re))
    return 0;
  
  if(!(pDemand->Rate<=Multi_Rate_Para.Rates && pDemand->Rate<=MAX_RATES))
  {
    ASSERT(A_WARNING,0);
    return 0;
  }
  
  for(i=0;i<2;i++)
  {
    if(!(memcmp(pDemand->Demand[i].Pos_Active_Time,Temp,5)==0 || _Check_BCD_Time(pDemand->Demand[i].Pos_Active_Time)==1) ||
       !(memcmp(pDemand->Demand[i].Neg_Active_Time,Temp,5)==0 || _Check_BCD_Time(pDemand->Demand[i].Neg_Active_Time)==1) ||
       !(memcmp(pDemand->Demand[i].Pos_Reactive_Time,Temp,5)==0 || _Check_BCD_Time(pDemand->Demand[i].Pos_Reactive_Time)==1) ||
       !(memcmp(pDemand->Demand[i].Neg_Reactive_Time,Temp,5)==0 || _Check_BCD_Time(pDemand->Demand[i].Neg_Reactive_Time)==1))
    { 
      ASSERT(A_WARNING,0);
      return 0;
    }  
  }
  return 1; 
}

//从ROM中读出当前需量数据
void Read_Demand_Data_From_Rom()
{
  INT8U Err,Re,Rate,i;
  S_Demand *p;
  
  //先读取掉电存储区数据
  //再读普通数据
  //最后读备份区数据
  Re=CHECK_STRUCT_SUM(Cur_Rate_Info);
  if(ASSERT(A_WARNING,0!=Re))
    Check_Data_Avail();
  
  OS_Sem_Pend(PUB_BUF0_SEM_ID); 
  p=(S_Demand *)Pub_Buf0;
  if(Read_Storage_Data(CUR_DEMAND_PD,p,p,sizeof(Pub_Buf0),&Err)!=sizeof(Cur_Demand) ||\
     Check_Demand_Data((S_Demand *)p)==0)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Read CUR_DEMAND_PD data error");
    OS_TimeDly_Ms(500);
    //读取总和当前费率需量
    for(i=0;i<2;i++)
    {
      if(i==0)
        Rate=0;//读总需量时偏移为0
      else
        Rate=Cur_Rate_Info.Rate;//非总时根据费率计算偏移

      p->Rate=Cur_Rate_Info.Rate;
      //从当前数据区读取当前和总费率的需量数据
      if(Read_Demand_Rate_Data(Rate,(void *)&p->Demand[i],(void *)&p->Demand[i],sizeof(S_One_Demand))!=ONE_DEMAND_SAVE_SIZE)
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Read CUR_DEMAND Rate %d data error",Rate);
        mem_set((void *)&p->Demand[i],0,sizeof(p->Demand[i]),(void *)&p->Demand[i],sizeof(p->Demand[i]));
      }
    }
  }

  
  if(Check_Demand_Data((S_Demand *)p)==0)
  {
    ASSERT(A_WARNING,0);
    mem_set(p,0,sizeof(Cur_Demand),p,sizeof(Pub_Buf0));
  }
  
  DIS_PD_INT;//更新Cur_Demand数据前关掉电中断
  mem_cpy((void *)&Cur_Demand,p,sizeof(Cur_Demand),(void *)&Cur_Demand,sizeof(Cur_Demand));
  INIT_STRUCT_VAR(Cur_Demand);
  SET_DATA_READY_FLAG(Cur_Demand);
  SET_STRUCT_SUM(Cur_Demand);
  EN_PD_INT;
  
  OS_Sem_Post(PUB_BUF0_SEM_ID);  
  //SET_STRUCT_SUM(Cur_Demand); 
  //更新当前存储区和备份存储区，补冻函数可能要用到这两个区，因此要保证这两个区的数据更新
  Save_Cur_Demand_Data();
  Save_Cur_Demand_Bak_Data();
}

/*
//根据历史数据时间计算其应该存储的DI位置
//Flag==
INT8U Calc_HIS_Data_Time_Index(INT8U Flag,S_HEX_Time *pTime)
{
  INT8U Index;
  //需量时间数据不能在当前时间之前HIS_ENERGY_DATA_MONTHS
  if((INT16U)(Cur_Time0.Time[T_YEAR])*12+Cur_Time0.Time[T_MONTH]>\
     (INT16U)(pTime->Time[T_YEAR])*12+pTime->Time[T_MONTH]+HIS_ENERGY_DATA_MONTHS)
  {
    return NULL_1BYTES;
  }
  //该时间在当月冻结时间之后
  if(pTime->Time[T_DATE]>=Data_Freeze_Time.DDHH[1] ||\
     (pTime->Time[T_DATE]==Data_Freeze_Time.DDHH[1] && pTime->Time[T_HOUR]>=Data_Freeze_Time.DDHH[0]))
  {
    Index=((INT16U)(pTime->Time[T_DATE])*12+pTime->Time[T_MONTH]) %HIS_ENERGY_DATA_MONTH;
  }
  else
  {
    Index=((INT16U)(pTime->Time[T_DATE])*12+pTime->Time[T_MONTH])%HIS_ENERGY_DATA_MONTH;
  }
  return Index;

}
*/

//获取pSrc时间点后的下一个冻结时间,包括pSrc本身所指向的时间点
//即>=pSrc指向时间的最近一个冻结时间点
void Get_Next_Freeze_Time(S_HEX_Time *pSrc,S_HEX_Time *pDst)
{
  S_HEX_Time TempTime;

  Get_NowMonth_Freeze_Time(pSrc,&TempTime);//获取当月冻结时间
  if(Cmp_Time(pSrc,&TempTime)==TIME_AFT)
  {
    Time_Inc(&TempTime,1,UNIT_MON,pDst);
  }
  else
    mem_cpy(pDst,&TempTime,sizeof(TempTime),pDst,sizeof(S_HEX_Time));
}

//获取pSrc时间点或该时间点之前的最近一个冻结时间点,包括pSrc本身所指向的时间点
//即<=pSrc指向时间的最近一个冻结时间点时间点
void Get_Last_Freeze_Time(S_HEX_Time *pSrc,S_HEX_Time *pDst)
{
  S_HEX_Time TempTime;

  Get_NowMonth_Freeze_Time(pSrc,&TempTime);//获取当月冻结时间
  if(Cmp_Time(pSrc,&TempTime)==TIME_BEF)
  {
    Time_Dec(&TempTime,1,UNIT_MON,pDst);
  }
  else
    mem_cpy(pDst,&TempTime,sizeof(TempTime),pDst,sizeof(S_HEX_Time));
}

//获取pSrc所指向的时间的当月的冻结时间点,当月指自然月
void Get_NowMonth_Freeze_Time(S_HEX_Time *pSrc,S_HEX_Time *pDst)
{
  INT8U Re;

  Re=1;
  Re=Check_STRUCT_Sum(pSrc,sizeof(S_HEX_Time),pSrc->CS,sizeof(pSrc->CS));
  ASSERT(A_WARNING,1==Re);
  mem_cpy(pDst->Time,pSrc->Time,sizeof(pSrc->Time),pDst->Time,sizeof(pDst->Time));
  pDst->Time[T_HOUR]=Data_Freeze_Time.DDHH[0];
  pDst->Time[T_DATE]=Data_Freeze_Time.DDHH[1];
  pDst->Time[T_MIN]=0;
  Set_STRUCT_Sum(pDst,sizeof(S_HEX_Time),pDst->CS,sizeof(pDst->CS));
}

//设置某条需量数据的时间域
void Set_One_Demand_Time(S_One_Demand *pSrc,S_HEX_Time *pTime)
{
  mem_cpy(&(pSrc->Time),pTime,sizeof(S_HEX_Time),&(pSrc->Time),sizeof(pSrc->Time));
  Set_STRUCT_Sum(pSrc,sizeof(S_One_Demand),pSrc->CS,sizeof(pSrc->CS));
}

//函数作用主要是从CUR_DEMAND存储区或者CUR_DEMAND_BAK存储区读出数据,然后进行月数据冻结
//在上电补冻或者跨越冻结时可以调用该函数，但是注意：
//pTime表示冻结时的时间,一般为当前时间
//如果冻结完后，马上就停电了，那么重上电后，仍在冻结时刻，继续补冻的话，发现已经有冻结数据了，便不会再冻了
void Freeze_Demand_Data(S_HEX_Time *pTime)//S_HEX_Time *pTime)
{
  INT8U i,Re,Err;
  S_One_Demand *p;
  S_HEX_Time TempTime;//,TempTime1;
  INT32U Off;

  DEBUG_PRINT(HUCK,DEBUG_0,"----------Freeze Demand Data %d-%d-%d %d:%d----------",\
                           pTime->Time[T_YEAR],pTime->Time[T_MONTH],pTime->Time[T_DATE],pTime->Time[T_HOUR],pTime->Time[T_MIN]);
  
  Re=CHECK_STRUCT_SUM(Cur_Demand);
  if(ASSERT(A_WARNING,1==Re))
    Check_Data_Avail();
  
  OS_Sem_Pend(PUB_BUF0_SEM_ID);//申请Pub_Buf信号量
  //分别读取每个份费率的数据，并将其转存到相应的历史数据区
  for(i=0;i<=Multi_Rate_Para.Rates && i<=MAX_RATES;i++)
  {
    //p指向该费率的需量数据
    //当前费率与Cur_Demand中的数据相符
    if(i==0 || i==Cur_Demand.Rate)
    {
      if(i==0)
       p=(S_One_Demand *)&Cur_Demand.Demand[0];
      else if(i==Cur_Demand.Rate)
       p=(S_One_Demand *)&Cur_Demand.Demand[1];
      
      Re=1;
    }
    else if(Read_Demand_Rate_Data(i,(INT8U *)Pub_Buf0,(INT8U *)Pub_Buf0,sizeof(Pub_Buf0))==ONE_DEMAND_SAVE_SIZE)
    {
      p=(S_One_Demand *)Pub_Buf0; 
      Re=1;
    }
    else
      Re=0;
    
    if(1==Re)//得到了正确的原始数据才进行冻结
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Demand Data Rate %u Freeze, Data Time: ",i);
      Debug_Print_HEX_Time((p->Time).Time);
      //先找到pTime之前的上次结算的时间点(包括pTime本身)
      if(((p->Time).Time[T_YEAR]==0 && (p->Time).Time[T_MONTH]==0) ||\
          Check_HEX_Time(&(p->Time))!=1)//必须保证时间格式正确才进行补冻,因为里面也可能是全0的数据,设置默认参数或者清0了
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Demand Rate %u Time EQ Zero or format error",i);
        continue;
      }
      //<=pTime的最近一个冻结时间点,pTime可能就是冻结时间点
      Get_Last_Freeze_Time(pTime,&TempTime);//pTime以前的最近一个冻结时间点,包括pTime本身
      
      //现在TempTime就是最近一次的冻结时间点,p->Time是需量数据的时间点
      Re=Cmp_Time(&(p->Time),&TempTime);//比较需量数据时间点与最近一次冻结时间点
      if(TIME_AFT==Re)//需量时间点在最近一次结算时间点之后，是一条当前数据，不需要补存到历史数据中,时间相等也有可能需要保存，因为需量生成时间可能正好是结算时间点
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Demand Rate %u Time > FreezeTime",i);
        Debug_Print_HEX_Time((p->Time).Time);
        Debug_Print_HEX_Time(TempTime.Time);
        continue;
      }
        //>=p->Time的最近一个冻结时间点,可能就是p->Time,因为p->Time可能就是当前时间,同时当前时间就是冻结时间点
      Get_Next_Freeze_Time(&(p->Time),&TempTime);//历史数据的冻结时间点,需量数据需要找到其时间之后的一个冻结时间点
      //这个月的这个费率的数据是否冻结过?没有冻结过才冻结
      if(Read_Demand_HIS_Data(i,&TempTime,(INT8U *)Pub_Buf0+sizeof(S_Demand),(INT8U *)Pub_Buf0,sizeof(Pub_Buf0),&Err) EQ ONE_DEMAND_SAVE_SIZE)
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Demand Rate %u Time Data Freezed!",i);
        Debug_Print_HEX_Time(TempTime.Time);        
        //continue;
      } 
      else
      {
        mem_cpy(p->Time.Time,&TempTime,sizeof(TempTime),p,sizeof(S_One_Demand));
        Set_STRUCT_Sum(p,sizeof(S_One_Demand),p->CS,sizeof(p->CS));
        Write_Demand_HIS_Data(i,&TempTime,p,ONE_DEMAND_SAVE_SIZE);
      }
      //因为是历史数据，所以需要重置变为当月数据
      //需要重置本月的需量初值以及需量冻结时间
      mem_set(p,0,sizeof(S_One_Demand),p,sizeof(S_One_Demand));//将数据全部清0存入当前存储区，下次读出全0数据，表示尚未生成新数据
      Set_STRUCT_Sum(p,sizeof(S_One_Demand),p->CS,sizeof(p->CS));
      //确保当前数据域时间点不会是冻结时间点
      //确保分域不为0

      //重新写入当前需量存储区包括备份区
      Off=DEMAND_RATE_OFF(i);
      Write_Storage_Data_Fix_Len(CUR_DEMAND,Off,p,ONE_DEMAND_SAVE_SIZE);
      Write_Storage_Data_Fix_Len(CUR_DEMAND_BAK,Off,p,ONE_DEMAND_SAVE_SIZE);
      
      Demand_Accu_Clear();
      //将修改后的Cur_Demand数据保存到掉电数据区
      if(i==0 || i==Cur_Demand.Rate)
      {
        SET_STRUCT_SUM(Cur_Demand);
        Save_Cur_Demand_PD_Data();
      }
    }
  }
  OS_Sem_Post(PUB_BUF0_SEM_ID);//释放Pub_Buf信号量
  DEBUG_PRINT(HUCK,DEBUG_0,"----------Freeze Demand Data End----------");
}

//需量检查当前费率是否发生了变化，如果变化了，则读出当前费率的最大需量值
void Demand_Check_Rate_Change()
{
  INT8U Re;

  Re=1;
  Re=CHECK_STRUCT_SUM(Cur_Rate_Info);
  Re=CHECK_STRUCT_SUM(Cur_Demand);
  if(ASSERT(A_WARNING,1==Re))
    Check_Data_Avail();

  if(Cur_Rate_Info.Rate!=Cur_Demand.Rate)//费率已经发生变化了
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Rate_Changed!Save Old Rate Demand && Read New Rate Demand");
    Save_Cur_Demand_Data();//先将Cur_Demand数据保存起来
    Save_Cur_Demand_Bak_Data();

    //重新根据当前费率读取需量数据
    Cur_Demand.Rate=Cur_Rate_Info.Rate;
    //读出该分费率数据
    if(Read_Demand_Rate_Data(Cur_Demand.Rate,(void *)&Cur_Demand.Demand[1],(void *)&Cur_Demand.Demand[1],sizeof(S_One_Demand))!=ONE_DEMAND_SAVE_SIZE)
    {
      mem_set((void *)&Cur_Demand.Demand[1],0,sizeof(Cur_Demand.Demand[1]),(void *)&Cur_Demand.Demand[1],sizeof(Cur_Demand.Demand[1]));
      SET_STRUCT_SUM(Cur_Demand.Demand[1]);
    }
    SET_STRUCT_SUM(Cur_Demand);
    
    Save_Cur_Demand_PD_Data();//更新掉电存储区的需量数据
  }
}

//检查当前需量数据的有效性
void Check_Demand_Data_Avail()
{
  INT8U Re;

  Re=Check_Demand_Data((S_Demand *)&Cur_Demand);
  if(ASSERT(A_WARNING,1==Re))//检查需量数据的校验和，校验和错误则重新从ROM中读取
  {
    Read_Demand_Data_From_Rom();
  }
  
  Re=CHECK_STRUCT_SUM(Demand_Accu);
  if(ASSERT(WARNING,1==Re))
  {
     mem_set((void *)&Demand_Accu,0,sizeof(Demand_Accu),(void *)&Demand_Accu,sizeof(Demand_Accu));
     SET_STRUCT_SUM(Demand_Accu);
  }
}

//清最大需量及其发生时间,清需量只清当月需量
void Clear_Cur_Demand_Data()
{
  if(Check_Clear_Data_Authority()==0)
    return;

  if(CLEAR_DEMAND_FLAG!=Check_Clear_Data_Flag())
    return;
  DEBUG_PRINT(HUCK,DEBUG_0,"Clear_Cur_Demand_Data!!!");  
  Set_Def_Cur_Demand_Data();
  Clear_Demand_Record();//清需量记录
  /*
  INT8U i,Re;
  INT32U Off;
  //S_HEX_Time TempTime;
  
  DEBUG_PRINT(HUCK,DEBUG_0,"Clear_Demand_Data!!!");
  //清需量

  //检查编程状态、用户权限状态
  if(Check_Meter_Prog_Status()==0 ||\
     CHECK_STRUCT_SUM(User_Authority_Flag)==0 ||\
     User_Authority_Flag.Flag!=AUTHORITY_FLAG)
    return;
  
  //检查清除标志
  if(CLEAR_DEMAND_FLAG!=Check_Clear_Data_Flag())
    return;

  //将需量数据清0，并置需量数据时间为下次冻结时间
  mem_set((void *)&Cur_Demand,0,sizeof(Cur_Demand),(void *)&Cur_Demand,sizeof(Cur_Demand));

  Re=CHECK_STRUCT_SUM(Cur_Rate_Info);//下面要用到当前费率
  if(ASSERT(A_WARNING,1==Re))
    Check_Data_Avail();

  Cur_Demand.Rate=Cur_Rate_Info.Rate;//重置当前需量费率
  SET_STRUCT_SUM(Cur_Demand);
  
  //检查编程状态、用户权限状态
  if(Check_Meter_Prog_Status()==0 ||\
     CHECK_STRUCT_SUM(User_Authority_Flag)==0 ||\
     User_Authority_Flag.Flag!=AUTHORITY_FLAG)
    return;
  
  //检查清除标志
  if(CLEAR_DEMAND_FLAG!=Check_Clear_Data_Flag())
    return;
  //先保存掉电存储区数据
  Write_Storage_Data(CUR_DEMAND_PD,(void *)(&Cur_Demand),sizeof(Cur_Demand));
  //再保存数据区和备份数据区数据,数据区和备份数据区格式不一样,需要按每个费率独立存储
  for(i=0;i<MAX_RATES;i++)
  {
    Off=DEMAND_RATE_OFF(i);
    Write_Storage_Data_Fix_Len(CUR_DEMAND,Off,(void *)(&Cur_Demand.Demand[0]),ONE_DEMAND_SAVE_SIZE);
    Write_Storage_Data_Fix_Len(CUR_DEMAND_BAK,Off,(void *)(&Cur_Demand.Demand[0]),ONE_DEMAND_SAVE_SIZE);
    OS_TimeDly_Ms(20);
  }
  */
}

//清当前需量记录处理
void Clear_Demand_Record()
{
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(Clr_Demand_Record);
  if(0==Re)
  {
    ASSERT(A_WARNING,0);
    mem_set((void *)(&Clr_Demand_Record),0,sizeof(Clr_Demand_Record),(void *)(&Clr_Demand_Record),sizeof(Clr_Demand_Record));
    SET_STRUCT_SUM(Clr_Demand_Record);
  }
  
  Clr_Demand_Record.Time[0]=Cur_Time1.Min;
  Clr_Demand_Record.Time[1]=Cur_Time1.Hour;
  Clr_Demand_Record.Time[2]=Cur_Time1.Date;
  Clr_Demand_Record.Time[3]=Cur_Time1.Month;
  Clr_Demand_Record.Counts++;
  SET_STRUCT_SUM(Clr_Demand_Record);
  Write_Storage_Data(CLR_DEMAND_RECORD,(void *)&Clr_Demand_Record,sizeof(Clr_Demand_Record));
}

//手动清需量的检查执行
void Check_Man_Clr_Demand()
{
  static S_Int8U Mins = {CHK_BYTE,0x00,CHK_BYTE};
  static S_Int8U Min_Bak = {CHK_BYTE,0x00,CHK_BYTE};
  INT8U Re;
  
  Re = CHECK_STRUCT_VAR(Mins);
  ASSERT(A_WARNING,1==Re);
 
  if(Mins.Var == 0)
  {
    if(Check_Man_ClrDemd_En())//手动清需量
    {
      Mins.Var ++; 
      Set_Event_Instant(ID_EVENT_CLR_DEMAND_MAN);//手动清需量事件
      Wait_Event_Data_Proc_End(ID_EVENT_CLR_DEMAND_MAN);//等待事件处理完成
      
      DEBUG_PRINT(HUCK,DEBUG_0,"Man_Clear_Cur_Demand_Data!!!");  
      Set_Def_Cur_Demand_Data();
      Clear_Demand_Record();//清需量记录      
    }
  }
  else
  {
    if(Min_Bak.Var == Cur_Time0.Time[T_MIN])//每分钟进来一次
       return;
       
    Min_Bak.Var = Cur_Time0.Time[T_MIN];
       
    Mins.Var ++;
    if(Mins.Var > Demand_Para.Period)
      Mins.Var = 0;
  }
}

//清所有的需量数据,包括当月和前HIS_ENERGY_DATA_MONTHS个月
void Clear_All_Demand_Data()
{
  if(Check_Clear_Data_Authority()==0)
    return;
  
  DEBUG_PRINT(HUCK,DEBUG_0,"Clear_All_Demand_Data!!!"); 
  if(CLEAR_DEMAND_FLAG!=Check_Clear_Data_Flag() &&
     CLEAR_ALL_FLAG!=Check_Clear_Data_Flag())
    return;
  Set_Def_Demand_Data();
  /*
  INT8U i,j;
  INT8U Re;
  INT32U Off;
  
  //将前0-HIS_ENERGY_DATA_MONTHS月的数据全部清0
  //检查编程状态、用户权限状态
  if(Check_Meter_Prog_Status()==0 ||\
     CHECK_STRUCT_SUM(User_Authority_Flag)==0 ||\
     User_Authority_Flag.Flag!=AUTHORITY_FLAG)
    return;
  
  //检查清除标志

  
  Re=1;
  Re&=CHECK_STRUCT_SUM(User_Authority_Flag);
  Re&=CHECK_STRUCT_SUM(Clear_Flag);
  if(ASSERT(A_WARNING,1==Re))
    Check_Data_Avail();

  mem_set((void *)&Cur_Demand,0,sizeof(Cur_Demand),(void *)&Cur_Demand,sizeof(Cur_Demand));
  //清前0-HIS_ENERGY_DATA_MONTHS月的需量
  for(i=0;i<=HIS_DEMAND_DATA_MONTHS;i++)
  {
    //DEBUG_PRINT(HUCK,DEBUG_0,"Clear Freeze Demand Data,Time:%d-%d-%d %d:%d",Time.Time[T_YEAR],Time.Time[T_MONTH],Time.Time[T_DATE],Time.Time[T_HOUR],Time.Time[T_MIN]);
    //严格判断，防止程序乱跑到这里
    if(Check_Meter_Prog_Status()==0 || 1!=Re ||\
       AUTHORITY_FLAG!=User_Authority_Flag.Flag ||\
       CLEAR_DEMAND_FLAG!=Clear_Flag.Flag)
      return;

      for(j=0;j<MAX_RATES;j++)
      {
        Off=DEMAND_RATE_OFF(i);//直接写历史数据存储缓冲区
        Write_Storage_Data_Fix_Len(HIS_DEMAND+i,Off,(void *)&Cur_Demand.Demand[0],ONE_DEMAND_SAVE_SIZE);
      }
  }
  Clear_Demand_Data();//先清当月需量
  */
}

//上电补冻需量数据
void PowerOn_Demand_Data_Freeze()
{
  Freeze_Demand_Data((S_HEX_Time *)&Cur_Time0);
}

//掉电存储需量数据
void PowerDown_Save_Demand_Data()
{
  Save_Cur_Demand_PD_Data();
}

//打印时间
void Debug_Print_Time(INT8U Time[])
{
  INT8U i;
  
  if(Check_Debug_En()==0)//非调试状态不打印
    return;
  
  OS_Debug_Print("  ");
  for(i=0;i<5;i++)
  {
    if(Time[4-i]<10)
      OS_Debug_Print("0");
    
    OS_Debug_Print("%x",Time[4-i]);
    if(i==0 || i==1)
      OS_Debug_Print("-");
    else if(i==2)
      OS_Debug_Print(" ");
    else if(i==3)
      OS_Debug_Print(":");
  }
  //OS_Debug_Print(" "); 
}

//打印时间
void Debug_Print_HEX_Time(INT8U Time[])
{
  INT8U i;
  
  if(Check_Debug_En()==0)//非调试状态不打印
    return;
  
  OS_Debug_Print("  ");
  for(i=0;i<5;i++)
  {
    if(Time[4-i]<10)
      OS_Debug_Print("0");
    
    OS_Debug_Print("%d",Time[4-i]);
    if(i==0 || i==1)
      OS_Debug_Print("-");
    else if(i==2)
      OS_Debug_Print(" ");
    else if(i==3)
      OS_Debug_Print(":");
  }
  //OS_Debug_Print(" "); 
}

//输出当前需量信息
void Debug_Print_Cur_Demand_Info()
{
  static S_Int8U Min={CHK_BYTE,0,CHK_BYTE};
  static S_Int8U Counts={CHK_BYTE,0,CHK_BYTE};
  INT8U i,Rate,Re;

  if(Check_Debug_En()==0)//非工厂状态不打印
    return;
  
  if(Cur_Time0.Time[T_MIN]==Min.Var)//每分钟执行一次该函数
    return;
  
  Re=CHECK_STRUCT_VAR(Min);
  ASSERT(A_WARNING,1==Re);
  Re=CHECK_STRUCT_VAR(Counts);
  ASSERT(A_WARNING,1==Re);
  
  Min.Var=Cur_Time0.Time[T_MIN];
  Counts.Var++;
  if(Counts.Var>=5)//每隔5分钟打印一次当前电能值
  {
    Counts.Var=0;
    
    Clr_Ext_Inter_Dog();
      DEBUG_PRINT(HUCK,DEBUG_1,"|-----------------------------Max_Demand-------------------------------|"); 
      DEBUG_PRINT(HUCK,DEBUG_1,"| Rate Pos_Act Neg_Act Pos_Rea Neg_Rea Quad0_R Quad1_R Quad2_R Quad3_R |");
    for(i=0;i<2;i++)
    {
      if(i==0)
        Rate=0;
      else
        Rate=Cur_Demand.Rate;
      
      DEBUG_PRINT(HUCK,DEBUG_1,"|   %2d %7ld %7ld %7ld %7ld %7ld %7ld %7ld %7ld |",\
        Rate,Cur_Demand.Demand[i].Pos_Active,Cur_Demand.Demand[i].Neg_Active,Cur_Demand.Demand[i].Pos_Reactive,Cur_Demand.Demand[i].Neg_Reactive,\
        Cur_Demand.Demand[i].Quad_Reactive[0],Cur_Demand.Demand[i].Quad_Reactive[1],Cur_Demand.Demand[i].Quad_Reactive[2],Cur_Demand.Demand[i].Quad_Reactive[3]);
    }
      DEBUG_PRINT(HUCK,DEBUG_1,"|----------------------------------------------------------------------|");
    
    OS_TimeDly_Ms(50);
    Clr_Ext_Inter_Dog();
      DEBUG_PRINT(HUCK,DEBUG_1,"|----------------------------Demand_Time-------------------------------|"); 
      DEBUG_PRINT(HUCK,DEBUG_1,"| Rate      Pos_Active      Neg_Active      Pos_Reacti      Neg_Reacti |");
    
    for(i=0;i<2;i++)
    {
      if(i==0)
        Rate=0;
      else
        Rate=Cur_Demand.Rate;
      
      DEBUG_PRINT(HUCK,DEBUG_1,"|   %2d",Rate);
      Debug_Print_Time((INT8U *)Cur_Demand.Demand[i].Pos_Active_Time);
      Debug_Print_Time((INT8U *)Cur_Demand.Demand[i].Neg_Active_Time);
      Debug_Print_Time((INT8U *)Cur_Demand.Demand[i].Pos_Reactive_Time);
      Debug_Print_Time((INT8U *)Cur_Demand.Demand[i].Neg_Reactive_Time);
      OS_Debug_Print(" |"); 
    }
    DEBUG_PRINT(HUCK,DEBUG_1,"|----------------------------------------------------------------------|");
    
    //暂时没有四象限无功
    /*
    Clr_Ext_Inter_Dog();
    //DEBUG_PRINT(HUCK,DEBUG_1,"|-------------------------------Demand_Time_0--------------------------|"); 
    DEBUG_PRINT(HUCK,DEBUG_1,"| Rate     Quad0_Reati     Quad1_Reati     Quad2_Reati     Quad3_Reati |");
    
    for(i=0;i<2;i++)
    {
      if(i==0)
        Rate=0;
      else
        Rate=Cur_Demand.Rate;
      
      DEBUG_PRINT(HUCK,DEBUG_1,"|   %2d",Rate);
      Debug_Print_Time((INT8U *)Cur_Demand.Demand[i].Quad_Reactive_Time[0]);
      Debug_Print_Time((INT8U *)Cur_Demand.Demand[i].Quad_Reactive_Time[1]);
      Debug_Print_Time((INT8U *)Cur_Demand.Demand[i].Quad_Reactive_Time[2]);
      Debug_Print_Time((INT8U *)Cur_Demand.Demand[i].Quad_Reactive_Time[3]);
      OS_Debug_Print(" |"); 
    }
      DEBUG_PRINT(HUCK,DEBUG_1,"|----------------------------------------------------------------------|");
    */
  } 
}

#undef DEMAND_C
