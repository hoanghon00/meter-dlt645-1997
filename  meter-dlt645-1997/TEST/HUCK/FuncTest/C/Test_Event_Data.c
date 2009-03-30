#define TEST_EVENT_DATA_C
#include "Includes.h"
#ifdef TEST_MODE

extern void Event_Energy_Cumu(MEASU_ENERG *pEnergy);
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
}MEASU_ENERG;                             //计量累计数据和增量数据

*/
void Set_Cumu_Energy()
{
  INT8U i,j;
  
  for(i=0;i<4;i++)
  {
    Measu_Energ_ToPub_PUCK.Chang_Plus_AcEnerg[i]=i*111111;
    Measu_Energ_ToPub_PUCK.Chang_Nega_AcEnerg[i]=i*222222;
    
    for(j=0;j<4;j++)
      Measu_Energ_ToPub_PUCK.Chang_Quad_ReacEnerg[i][j]=i*100000+j*10000;
    
    if(i<3)
      Measu_Energ_ToPub_PUCK.Add_AmpaHour[i]=i*33333333;
    Measu_Energ_ToPub_PUCK.Add_CopperLoss[i]=i*44444444;
    Measu_Energ_ToPub_PUCK.Add_IronLoss[i]=i*55555555;
    Measu_Energ_ToPub_PUCK.Add_GenaralErr[i]=i*66666666;
  }
  
}

void Set_Energy_0()
{
  INT8U i,j;
  
  mem_set((void *)&Cur_Energy,0,sizeof(Cur_Energy),(void *)&Cur_Energy,sizeof(Cur_Energy));
  for(i=1;i<=MAX_RATES;i++)
  {
    Cur_Energy.Pos_Active[i]=111;//总以及分费率正向有功,三位小数,单位KWh，下同
    Cur_Energy.Pos_Active[0]+=Cur_Energy.Pos_Active[i];//888
    Cur_Energy.Neg_Active[i]=222;//总以及分费率反向有功
    Cur_Energy.Neg_Active[0]+=Cur_Energy.Neg_Active[i];//1776
    
    Cur_Energy.Quad_Reactive[0][i]=111;//四象限分费率无功
    Cur_Energy.Quad_Reactive[0][0]+=Cur_Energy.Quad_Reactive[0][i];//888
    Cur_Energy.Quad_Reactive[1][i]=222;//四象限分费率无功
    Cur_Energy.Quad_Reactive[1][0]+=Cur_Energy.Quad_Reactive[1][i];//1776
    Cur_Energy.Quad_Reactive[2][i]=333;//四象限分费率无功
    Cur_Energy.Quad_Reactive[2][0]+=Cur_Energy.Quad_Reactive[2][i];//2664
    Cur_Energy.Quad_Reactive[3][i]=444;//四象限分费率无功
    Cur_Energy.Quad_Reactive[3][0]+=Cur_Energy.Quad_Reactive[3][i];//3552
  }  

  for(i=0;i<3;i++)
  {
    Cur_Energy.Phase_Pos_Active[i]=(i+1)*1000;//三相正向有功,//0,1000,2000
    Cur_Energy.Phase_Neg_Active[i]=(i+1)*2000;//三相反向有功,//
    Cur_Energy.Phase_AH[i]=(i+1)*3000;//三相总AH
  }
  
  for(i=0;i<3;i++)
  {
    for(j=0;j<4;j++)
      Cur_Energy.Phase_Quad_Reactive[i][j]=i*1000+j*100;
  } 

  for(i=0;i<4;i++)
  {
    Cur_Energy.CopperLoss[i]=(i+1)*1000;//总ABC铜损
    Cur_Energy.IronLoss[i]=(i+1)*2000;  //总ABC铁损
    Cur_Energy.TransErr[i]=(i+1)*3000;  //总ABC互感器误差  
  } 
  SET_STRUCT_SUM(Cur_Energy);
}

void Set_Energy_1()
{
  INT8U i,j;
  
  //mem_set((void *)&Cur_Energy,0,sizeof(Cur_Energy),(void *)&Cur_Energy,sizeof(Cur_Energy));
  Cur_Energy.Pos_Active[0]+=1111;
  Cur_Energy.Neg_Active[0]+=2222;
  Cur_Energy.Quad_Reactive[0][0]+=3333;
  Cur_Energy.Quad_Reactive[1][0]+=4444;
  Cur_Energy.Quad_Reactive[2][0]+=5555;
  Cur_Energy.Quad_Reactive[3][0]+=6666;
  for(i=1;i<=MAX_RATES;i++)
  {
    Cur_Energy.Pos_Active[i]+=1111;//总以及分费率正向有功,三位小数,单位KWh，下同

    Cur_Energy.Neg_Active[i]=2222;//总以及分费率反向有功

    
    Cur_Energy.Quad_Reactive[0][i]+=3333;//四象限分费率无功

    Cur_Energy.Quad_Reactive[1][i]+=4444;//四象限分费率无功

    Cur_Energy.Quad_Reactive[2][i]+=5555;//四象限分费率无功

    Cur_Energy.Quad_Reactive[3][i]+=6666;//四象限分费率无功

  }  

  for(i=0;i<3;i++)
  {
    Cur_Energy.Phase_Pos_Active[i]+=7777;//三相正向有功
    Cur_Energy.Phase_Neg_Active[i]+=8888;//三相反向有功
    Cur_Energy.Phase_AH[i]+=9999;//三相总AH
  }
  
  for(i=0;i<3;i++)
  {
    for(j=0;j<4;j++)
      Cur_Energy.Phase_Quad_Reactive[i][j]+=11111;
  } 

  for(i=0;i<4;i++)
  {
    Cur_Energy.CopperLoss[i]+=22222;//总ABC铜损
    Cur_Energy.IronLoss[i]+=33333;  //总ABC铁损
    Cur_Energy.TransErr[i]+=44444;  //总ABC互感器误差  
  } 
  SET_STRUCT_SUM(Cur_Energy);
}

//测试电压统计数据
INT8U Test_Freeze_VolStat_Data()
{
  INT8U Re,i;
  INT8U Err;
  S_VolStat *p;
  INT16U Len;
  
  Set_Def_VolStat_Data();//先清电量数据
  Cur_VolStat.Total_Time[0]=5555;
  SET_STRUCT_SUM(Cur_VolStat);
  
  Cur_Time0.Time[T_YEAR]=8;
  Cur_Time0.Time[T_MONTH]=11;
  Cur_Time0.Time[T_DATE]=1;
  Cur_Time0.Time[T_HOUR]=0;
  Cur_Time0.Time[T_MIN]=0;
  SET_STRUCT_SUM(Cur_Time0);
  

  Re=1;
  for(i=0;i<HIS_VOLSTAT_DATA_MONTHS*2;i++)
  {
    Time_Inc((S_HEX_Time *)&Cur_Time0,1,UNIT_MON,(S_HEX_Time *)&Cur_Time0);
    Cur_VolStat.Total_Time[0]=i;
    SET_STRUCT_TIME(Cur_VolStat,Cur_Time0);
    Freeze_VolStat_Data((S_HEX_Time *)&Cur_Time0);

    OS_Sem_Pend(PUB_BUF_SEM_ID);
    mem_set((void *)Pub_Buf,0,sizeof(Pub_Buf),(void *)Pub_Buf,sizeof(Pub_Buf));
    Len=Read_HIS_Data(HIS_VOLSTAT_DATA,(S_HEX_Time *)&Cur_Time0,(void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),&Err);
    p=(S_VolStat *)Pub_Buf;
    OS_Sem_Post(PUB_BUF_SEM_ID);//释放Pub_Buf的信号量
    Re&=~ASSERT(A_WARNING,Len>0 && i==p->Total_Time[0]);
  }  
  return Re;  
  
}

//测试需量统计数据
INT8U Test_Freeze_Demand_Data()
{
  INT8U Re,i,j;
  INT8U Err;
  S_One_Demand *p;
  INT16U Len;
  INT32U Off;
  
  //Set_Def_Energy_Data();//先清电量数据
  //Cur_Energy.Pos_Active[0]=5555;
  //SET_STRUCT_SUM(Cur_Energy);
  
  Cur_Time0.Time[T_YEAR]=7;
  Cur_Time0.Time[T_MONTH]=12;
  Cur_Time0.Time[T_DATE]=1;
  Cur_Time0.Time[T_HOUR]=0;
  Cur_Time0.Time[T_MIN]=0;
  SET_STRUCT_SUM(Cur_Time0);
  

  Re=1;
  for(i=0;i<HIS_DEMAND_DATA_MONTHS*2;i++)
  {
    Time_Inc((S_HEX_Time *)&Cur_Time0,1,UNIT_MON,(S_HEX_Time *)&Cur_Time0);
    mem_cpy((S_HEX_Time *)&Cur_Demand.Demand[0].Time,(S_HEX_Time *)&Cur_Time0,sizeof(Cur_Time0),\
      (S_HEX_Time *)&Cur_Demand.Demand[0].Time,sizeof(Cur_Demand.Demand[0].Time));
    mem_cpy((S_HEX_Time *)&Cur_Demand.Demand[1].Time,(S_HEX_Time *)&Cur_Time0,sizeof(Cur_Time0),\
      (S_HEX_Time *)&Cur_Demand.Demand[1].Time,sizeof(Cur_Demand.Demand[1].Time));
    SET_STRUCT_TIME(Cur_Demand.Demand[0],Cur_Time0);
    SET_STRUCT_TIME(Cur_Demand.Demand[1],Cur_Time0);
    SET_STRUCT_SUM(Cur_Demand);
    
    //先将需量数据写入当前数据区，因为冻结需要从该数据区取数
    for(j=0;j<=Multi_Rate_Para.Rates && j<=MAX_RATES;j++)
    {
      Off=DEMAND_RATE_OFF(j);
      Cur_Demand.Demand[0].Pos_Active=j;
      SET_STRUCT_SUM(Cur_Demand.Demand[0].Time);
      SET_STRUCT_SUM(Cur_Demand.Demand[0]);
      SET_STRUCT_SUM(Cur_Demand);
      
      Write_Storage_Data_Fix_Len(CUR_DEMAND,Off,(void *)&Cur_Demand.Demand[0],ONE_DEMAND_SAVE_SIZE);
      Write_Storage_Data_Fix_Len(CUR_DEMAND_BAK,Off,(void *)&Cur_Demand.Demand[0],ONE_DEMAND_SAVE_SIZE);      
    }
    
    //置当前费率为一个不可能值,下面的冻结函数用到内存中需量
    Cur_Demand.Rate=0xFF;
    Cur_Demand.Demand[0].Pos_Active=0;
    SET_STRUCT_SUM(Cur_Demand.Demand[0]);
    SET_STRUCT_SUM(Cur_Demand);
    //
    
    SET_STRUCT_SUM(Cur_Demand);    
    Freeze_Demand_Data((S_HEX_Time *)&Cur_Time0);

    OS_Sem_Pend(PUB_BUF_SEM_ID);
    for(j=0;j<=Multi_Rate_Para.Rates && j<=MAX_RATES;j++)
    {
      mem_set((void *)Pub_Buf,0,sizeof(Pub_Buf),(void *)Pub_Buf,sizeof(Pub_Buf));
      Len=Read_Demand_HIS_Data(j,(S_HEX_Time *)&Cur_Time0,(void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),&Err);
      p=(S_One_Demand *)Pub_Buf;
      Re&=~ASSERT(A_WARNING,NO_ERR==Err && Len==ONE_DEMAND_SAVE_SIZE &&\
                           p->Pos_Active==j && (p->Time).Time[T_MONTH]==Cur_Time0.Time[T_MONTH]);

    }
    OS_Sem_Post(PUB_BUF_SEM_ID);//释放Pub_Buf的信号量
  }  
  return Re;
  
  
}

//数据冻结的测试
INT8U Test_Freeze_Energy_Data()
{
  INT8U Re,i;
  INT8U Err;
  S_Energy *p;
  INT16U Len;
  
  Set_Def_Energy_Data();//先清电量数据
  Cur_Energy.Pos_Active[0]=5555;
  SET_STRUCT_SUM(Cur_Energy);
  
  Cur_Time0.Time[T_YEAR]=8;
  Cur_Time0.Time[T_MONTH]=11;
  Cur_Time0.Time[T_DATE]=1;
  Cur_Time0.Time[T_HOUR]=0;
  Cur_Time0.Time[T_MIN]=0;
  SET_STRUCT_SUM(Cur_Time0);
  

  Re=1;
  for(i=0;i<HIS_ENERGY_DATA_MONTHS*2;i++)
  {
    Time_Inc((S_HEX_Time *)&Cur_Time0,1,UNIT_MON,(S_HEX_Time *)&Cur_Time0);
    SET_STRUCT_TIME(Cur_Energy,Cur_Time0);
    Freeze_Energy_Data((S_HEX_Time *)&Cur_Time0);

    OS_Sem_Pend(PUB_BUF_SEM_ID);
    mem_set((void *)Pub_Buf,0,sizeof(Pub_Buf),(void *)Pub_Buf,sizeof(Pub_Buf));
    Len=Read_HIS_Data(HIS_ENERGY_DATA,(S_HEX_Time *)&Cur_Time0,(void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),&Err);
    p=(S_Energy *)Pub_Buf;
    OS_Sem_Post(PUB_BUF_SEM_ID);//释放Pub_Buf的信号量
    Re&=~ASSERT(A_WARNING,Len>0 && Cur_Energy.Pos_Active[0]==p->Pos_Active[0]);
  }  
  return Re;
}

void Test_Cur_Demand_Rate_Data()
{
  INT8U i;

  for(i=0;i<MAX_RATES;i++)
  {
    Cur_Demand.Rate=i+1;
    Save_Cur_Demand_Data();    
  }

  for(i=0;i<MAX_RATES;i++)
  {
    Cur_Demand.Rate=i+1;
    Read_Demand_Rate_Data(i+1,(void *)&Cur_Demand.Demand[0],(void *)&Cur_Demand[0],sizeof(Cur_Demand[0]));    
  }  
}

//全失压数据的测试
INT8U Test_All_Loss_Vol_Data_Proc()
{
  INT8U i;
  INT8U Start_Time[5]={0x00,0x00,0x01,0x01,0x08};
  INT8U End_Time[5]={0x10,0x00,0x01,0x01,0x08};
  
  All_Loss_Vol_Counts_Time_Proc(11,1140);
 
  for(i=0;i<15;i++)
  {
    All_Loss_Vol_Data_Proc(10,Start_Time,End_Time);
    Check_Event_Cumu_Avail();
    Start_Time[1]++;
    End_Time[1]++;
  }

  return 1;
}

INT8U Test_Event_Data_Proc()
{
  INT8U i,j;
  INT16U Num;
 
  Set_Def_Event_Data();
  Num=Get_Event_Data_Info_Num();
  //for(j=0;j<20;j++)
  {
    //Set_Energy_0();
    Read_Event_Cumu_Data_From_Rom();
    
    //memset((void *)&Cur_Time0,0,sizeof(Cur_Time0));
    Cur_Time0.Time[T_YEAR]=0;
    Cur_Time0.Time[T_MONTH]=1;
    Cur_Time0.Time[T_DATE]=1;//j+1;
    Cur_Time0.Time[T_HOUR]=0;
    Cur_Time0.Time[T_MIN]=0;
    SET_STRUCT_SUM(Cur_Time0);
  
    for(i=0;i<Num;i++)
    {
      Time_Inc((S_HEX_Time *)&Cur_Time0,1,UNIT_MIN,(S_HEX_Time *)&Cur_Time0);
      Convert_HEX_2_BCD_Time((S_HEX_Time *)&Cur_Time0,(S_BCD_Time *)&Cur_Time1);
      Event_Data_Proc(Event_Data_Info[i].Event_ID,EVENT_OCCUR);
    }

    Set_Cumu_Energy();
    Event_Energy_Cumu((MEASU_ENERG *)&Measu_Energ_ToPub_PUCK);
    Set_Energy_1(); 
    
    Cur_Time0.Time[T_YEAR]=8;
    Cur_Time0.Time[T_MONTH]=1;
    Cur_Time0.Time[T_DATE]=1;//j+1;
    Cur_Time0.Time[T_HOUR]=0;
    Cur_Time0.Time[T_MIN]=0;
    SET_STRUCT_SUM(Cur_Time0);
    for(i=0;i<Num;i++)
    {
      Time_Inc((S_HEX_Time *)&Cur_Time0,1,UNIT_MIN,(S_HEX_Time *)&Cur_Time0);
      Convert_HEX_2_BCD_Time((S_HEX_Time *)&Cur_Time0,(S_BCD_Time *)&Cur_Time1);
      Event_Data_Proc(Event_Data_Info[i].Event_ID,EVENT_END); 
    }
  }
  return 1;
 //mem_set((void *)&Cur_Energy,0,sizeof(Cur_Energy),(void *)&Cur_Energy,sizeof(Cur_Energy));
}
/*
typedef struct
{
 MEASU_DATA AcPower;       //有功功率:UNIT:0.000001kW
 MEASU_DATA ReacPower;     //无功功率:UNIT:0.000001kvar
 MEASU_DATA AppPower;      //视在功率:UNIT:0.000001kva
 MEASU_DATA Volt;          //电压有效值：无合相 UNIT:0.0001V
 MEASU_DATA Curr;          //电流有效值: UNIT:0.0001A	
 MEASU_DATA PFactor;       //功率因数: UNIT:0.000001
 MEASU_DATA Freq;          //频率:UNIT:0.000001Hz
 MEASUFLAG  Quadrant;      //各相象限：QUADRANT1～QUADRANT4代表1～4象限。
 MEASU_DATA Angle;         //电压电流夹角：UINT:0.01度
 INT32U     Temprate;      //温度
 INT8U CS[CS_BYTES];
}MEASU_INSTANT;   //MeasuIC瞬时量计量数据
*/
//A失压测试
INT8U Test_A_Loss_Vol()
{
  INT8U Re,Err;
  INT16U Data0,Data1,Len;
  
  Re=1;
  /*
  mem_set((void *)&Measu_InstantData_ToPub_PUCK,0,sizeof(Measu_InstantData_ToPub_PUCK),(void *)&Measu_InstantData_ToPub_PUCK,sizeof(Measu_InstantData_ToPub_PUCK));
  Measu_InstantData_ToPub_PUCK.Volt.A=0;
  Measu_InstantData_ToPub_PUCK.Curr.A=10000;

  Measu_InstantData_ToPub_PUCK.Volt.B=2200000;
  Measu_InstantData_ToPub_PUCK.Curr.B=10000;

  Measu_InstantData_ToPub_PUCK.Volt.C=2200000;
  Measu_InstantData_ToPub_PUCK.Curr.C=10000;
  
  SET_STRUCT_SUM(Measu_InstantData_ToPub_PUCK);
  */  

  Len=Get_DLT645_Data(0xB310,(INT8U *)Test_Buf0,(INT8U *)Test_Buf0,sizeof(Test_Buf0),&Err);//总失压次数
  Re&=~ASSERT(A_WARNING,2==Len);
  Len=Get_DLT645_Data(0xB311,(INT8U *)Test_Buf1,(INT8U *)Test_Buf1,sizeof(Test_Buf1),&Err);//A失压次数
  Re&=~ASSERT(A_WARNING,2==Len);
  Len=Get_DLT645_Data(0xB710,(INT8U *)Test_Buf2,(INT8U *)Test_Buf2,sizeof(Test_Buf2),&Err);//A失压期间累加
  Re&=~ASSERT(A_WARNING,82==Len);
  Len=Get_DLT645_Data(0xE910,(INT8U *)Test_Buf3,(INT8U *)Test_Buf3,sizeof(Test_Buf3),&Err);//前一次A失压累加 
  Re&=~ASSERT(A_WARNING,86==Len);  
/*  
  OS_Waitfor_Sec(Check_Cumu_Event_Occuring(ID_EVENT_A_LOSS_VOLT),5);
  if(Check_Cumu_Event_Occuring(ID_EVENT_A_LOSS_VOLT)==1 &&\
     Check_Cumu_Event_Occuring(ID_EVENT_B_LOSS_VOLT)==0 &&\
     Check_Cumu_Event_Occuring(ID_EVENT_C_LOSS_VOLT)==0 &&\
     Check_Cumu_Event_Occuring(ID_EVENT_AB_LOSS_VOLT)==0 &&\
     Check_Cumu_Event_Occuring(ID_EVENT_AC_LOSS_VOLT)==0 &&\
     Check_Cumu_Event_Occuring(ID_EVENT_BC_LOSS_VOLT)==0 &&\
     Check_Cumu_Event_Occuring(ID_EVENT_ABC_LOSS_VOLT)==0 &&\
     Check_Cumu_Event_Occuring(ID_EVENT_ALL_LOSS_VOLT)==0)
  {
      ASSERT(A_WARNING,0);
      Re=0; 
  }
  OS_TimeDly_Min(2);//延时2分钟
*/
  Event_Data_Proc(ID_EVENT_A_LOSS_VOLT,EVENT_OCCUR);
  //重新读总失压次数
  Len=Get_DLT645_Data(0xB310,(INT8U *)Test_Buf0+sizeof(Test_Buf0)/2,(INT8U *)Test_Buf0,sizeof(Test_Buf0),&Err);//总失压次数
  Re&=~ASSERT(A_WARNING,2==Len);
  if(memcmp(Test_Buf0,Test_Buf0+sizeof(Test_Buf0)/2,Len)!=0)
  {
    ASSERT(A_WARNING,0);
    Re=0;
  }
  Data0=Bcd2Hex((INT8U *)Test_Buf0,2);
  Data1=Bcd2Hex((INT8U *)Test_Buf0+sizeof(Test_Buf0)/2,2);
  Re&=~ASSERT(A_WARNING,Data0+1==Data1);  
  
  Len=Get_DLT645_Data(0xB311,(INT8U *)Test_Buf1+sizeof(Test_Buf1)/2,(INT8U *)Test_Buf1,sizeof(Test_Buf1),&Err);//A失压次数
  Re&=~ASSERT(A_WARNING,2==Len);
  if(memcmp(Test_Buf0,(INT8U *)Test_Buf0+sizeof(Test_Buf0)/2,Len)!=0)
  {
    ASSERT(A_WARNING,0);
    Re=0;
  }
 
  Data0=Bcd2Hex((INT8U *)Test_Buf1,2);
  Data1=Bcd2Hex((INT8U *)Test_Buf0+sizeof(Test_Buf1)/2,2);
  Re&=~ASSERT(A_WARNING,Data0+1==Data1); 
  
  Len=Get_DLT645_Data(0xB710,(INT8U *)Test_Buf2+sizeof(Test_Buf2)/2,(INT8U *)Test_Buf2,sizeof(Test_Buf2),&Err);//A失压期间累加
  Re&=~ASSERT(A_WARNING,82==Len);
  if(memcmp(Test_Buf0,Test_Buf0+sizeof(Test_Buf0)/2,Len)!=0)
  {
    ASSERT(A_WARNING,0);
    Re=0;
  }
  Data0=Bcd2Hex((INT8U *)Test_Buf2+4,2);
  Data1=Bcd2Hex((INT8U *)Test_Buf0+sizeof(Test_Buf1)/2+4,2);
  Re&=~ASSERT(A_WARNING,Data0+1==Data1);
  
  Len=Get_DLT645_Data(0xE910,(INT8U *)Test_Buf3+sizeof(Test_Buf3)/2,(INT8U *)Test_Buf3,sizeof(Test_Buf3),&Err);//前一次A失压累加 
  Re&=~ASSERT(A_WARNING,86==Len);
  if(memcmp(Test_Buf0,Test_Buf0+sizeof(Test_Buf0)/2,Len)!=0)
  {
    ASSERT(A_WARNING,0);
    Re=0;
  }  
  return Re;
}

//B失压测试
INT8U Test_B_Loss_Vol()
{
  return 1;
}

//C失压测试
INT8U Test_C_Loss_Vol()
{
  return 1;
}

//AB失压测试
INT8U Test_AB_Loss_Vol()
{
  return 1;
  //Measu_InstantData_ToPub_PUCK.
  
}

//AC失压测试
INT8U Test_AC_Loss_Vol()
{
  return 1;
}

//BC失压测试
INT8U Test_BC_Loss_Vol()
{
  return 1;
}

//ABC失压测试
INT8U Test_ABC_Loss_Vol()
{
  return 1;
  //Measu_InstantData_ToPub_PUCK.
  
}
#endif
#undef TEST_EVENT_DATA_C
