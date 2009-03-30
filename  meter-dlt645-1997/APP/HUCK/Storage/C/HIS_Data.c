#define HIS_DATA_C
#include <includes.h>

//计算历史电量、需量、和电压统计数据的存储DI
//因为一般都是存储上N月的数据，所以存储的索引是根据总月数生成的
STORA_DI Calc_HIS_Data_DI(INT8U Data_Flag,S_HEX_Time *pTime)
{
  INT8U Re;
  STORA_DI DI;
 
  if(ASSERT(A_WARNING,HIS_ENERGY_DATA==Data_Flag ||\
            HIS_DEMAND_DATA==Data_Flag ||\
            HIS_VOLSTAT_DATA==Data_Flag))
    return NULL_4BYTES;
  
  Re=Check_HEX_Time(pTime);
  if(ASSERT(A_WARNING,Re!=0))
    return NULL_4BYTES;    
  
  //计算电量和需量数据的Index
  if(HIS_ENERGY_DATA==Data_Flag)
  {
    DI=HIS_ENERGY+(pTime->Time[T_MONTH]+(pTime->Time[T_YEAR])*12)%HIS_ENERGY_DATA_MONTHS;
  }
  else if(HIS_DEMAND_DATA==Data_Flag)
  {
    DI=HIS_DEMAND+(pTime->Time[T_MONTH]+(pTime->Time[T_YEAR])*12)%HIS_DEMAND_DATA_MONTHS;
  }
  else if(HIS_VOLSTAT_DATA==Data_Flag)
  {
    DI=HIS_VOLSTAT+(pTime->Time[T_MONTH]+(pTime->Time[T_YEAR])*12)%HIS_VOLSTAT_DATA_MONTHS;  
  }
  else
  {
    return NULL_4BYTES;
  }
  return DI;
}

//获取前几个月的历史数据的DI
//Data_Flag数据标志
//HIS_ENERGY_DATA;
//HIS_DEMAND_DATA;
//HIS_VOLSTAT_DATA;
//Bef_Month前几个月
//返回数据的存储DI
STORA_DI Calc_HIS_Data_DI_Bef_Month(INT8U Data_Flag,INT8U Bef_Month)
{
  S_HEX_Time Temp_Time;
  Time_Dec((S_HEX_Time *)&Cur_Time0,Bef_Month,UNIT_MON,(S_HEX_Time *)&Temp_Time);//将时间向前推移Bef_Mon个月
  return Calc_HIS_Data_DI(Data_Flag,&Temp_Time);
}

//读历史电量电压统计等数据,需量历史数据在Read_Demand_HIS_Data函数中单独处理
//pTime电量，电压统计等数据的存储时间
//pDst读数据目标缓冲区
//pDst_Start和DstLen一起限定pDst和返回长度的范围
//pErr错误标志字
//返回读取到的数据长度
//当返回为0是，*pErr表示错误原因
INT16U Read_HIS_Data(INT8U Data_Flag,S_HEX_Time *pTime,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr)
{
  INT8U *p;
  INT16U Len;
  STORA_DI DI;
 
  DEBUG_PRINT(HUCK,DEBUG_0,"Read_HIS_Data Time:");
  Debug_Print_HEX_Time(pTime->Time);
  DI=Calc_HIS_Data_DI(Data_Flag,pTime);
  if(ASSERT(A_WARNING,NULL_4BYTES!=DI))
  {
    *pErr=RD_HIS_DATA_DI_ERR;
    return 0; 
  }

  Len=Get_Storage_Data_Len(DI); //数据长度
  //数据长度必须满足在参数规定范围内
  if(ASSERT(A_WARNING,0!=Len && (INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst+Len<=(INT8U *)pDst_Start+DstLen))
  {
    *pErr=RD_HIS_DATA_LEN_ERR;
    return 0;    
  }
  //读出数据
  if(Read_Storage_Data(DI,pDst,pDst_Start,DstLen,pErr)!=Len)
  {
    *pErr=RD_HIS_DATA_LEN_ERR;
    return 0;        
  }

  if(NO_ERR==*pErr)
  {
    if(HIS_ENERGY_DATA==Data_Flag)
      p=(((S_Energy *)pDst)->Time).Time;
    //else if(HIS_DEMAND==DI)
      //p=((S_Demand *)pDst)->Time;
    else if(HIS_VOLSTAT_DATA==Data_Flag)
      p=(((S_VolStat *)pDst)->Time).Time;
    else
    {
      *pErr=RD_HIS_DATA_DI_ERR;
      return 0;
    }
    //比较时间是否是需要的时间,日、月、年都相等，认为时间相等
    //if(p[T_DATE]==pTime->Time[T_DATE] &&
     if(p[T_MONTH]==pTime->Time[T_MONTH] &&
        p[T_YEAR]==pTime->Time[T_YEAR])
      return Len;
    else 
    {
      *pErr=RD_HIS_DATA_TIME_ERR;
      return 0;
    }
  }
  else
  {
    ASSERT(A_WARNING,0);
    return 0;
  }
}


//写历史电量电压统计等数据,写需量历史数据是在Write_Demand_HIS_Data函数中单独处理 
//pTime电量，电压统计等数据的存储时间
//pSrc写数据缓冲区
//SrcLen数据长度
//返回值
//返回NO_ERR表示写成功
INT8U Write_HIS_Data(INT8U Data_Flag,S_HEX_Time *pTime,void *pSrc,INT16U SrcLen)
{
  //INT8U Index;
  INT8U Re;
  INT16U Len;
  STORA_DI DI;
 
  DI=Calc_HIS_Data_DI(Data_Flag,pTime);
  if(ASSERT(A_WARNING,NULL_4BYTES!=DI))
    return WR_HIS_DATA_DI_ERR;

  Len=Get_Storage_Data_Len(DI); //数据长度
  //数据长度必须满足在参数规定范围内
  if(ASSERT(A_WARNING,Len==SrcLen))
    return WR_HIS_DATA_LEN_ERR;    
 
  //写存储数据
  Re=Write_Storage_Data(DI,pSrc,Len);
  if(ASSERT(A_WARNING,NO_ERR==Re))//写入数据
    return Re;
  else
    return NO_ERR;
}

//读历史电量需量数据
//Rate 需要读取的是哪个费率的需量数据
//pTime电量，需量数据的存储时间
//pDst读数据目标缓冲区
//pDst_Start和DstLen一起限定pDst和返回长度的范围
//pErr错误标志字
//返回读取到的数据长度
//当返回为0是，*pErr表示错误原因
INT16U Read_Demand_HIS_Data(INT8U Rate,S_HEX_Time *pTime,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr)
{
  STORA_DI DI;
  INT32U Off;

  //判断该需量数据的费率的合法性
  if(ASSERT(A_WARNING,Rate<=Multi_Rate_Para.Rates && Rate<=MAX_RATES))
  {
    *pErr=RD_HIS_DATA_DATA_ERR;
    return 0;
  }
  //计算历史数据的DI
  DI=Calc_HIS_Data_DI(HIS_DEMAND_DATA,pTime);
  if(ASSERT(A_WARNING,NULL_4BYTES!=DI))
  {
    *pErr=RD_HIS_DATA_DI_ERR;
    return 0;
  }

  //读出数据
  Off=DEMAND_RATE_OFF(Rate);
  if(Read_Storage_Data_Fix_Len(DI,Off,ONE_DEMAND_SAVE_SIZE,pDst,pDst_Start,DstLen,pErr)!=ONE_DEMAND_SAVE_SIZE)
  {
    ASSERT(A_WARNING,0);
    *pErr=RD_HIS_DATA_LEN_ERR;
    return 0;        
  }

  //比较时间是否是需要的时间,月、年都相等，认为时间相等
  if((((S_One_Demand *)pDst)->Time.Time[T_MONTH]==pTime->Time[T_MONTH] &&
     (((S_One_Demand *)pDst)->Time.Time[T_YEAR]==pTime->Time[T_YEAR])))
    return ONE_DEMAND_SAVE_SIZE;
  else 
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Read Demand HIS Data Time !=:");
    Debug_Print_Time(pTime->Time);
    Debug_Print_Time(((S_One_Demand *)pDst)->Time.Time);
    *pErr=RD_HIS_DATA_TIME_ERR;
    return 0;
  }
}


//写历史电量需量数据
//pTime电量，需量数据的存储时间
//pSrc写数据缓冲区
//SrcLen数据长度
//返回值
//返回NO_ERR表示写成功
INT8U Write_Demand_HIS_Data(INT8U Rate,S_HEX_Time *pTime,void *pSrc,INT16U SrcLen)
{
  INT8U Re;
  STORA_DI DI;
  INT32U Off;

  //判断该需量数据的费率的合法性
  if(ASSERT(A_WARNING,Rate<=Multi_Rate_Para.Rates && Rate<=MAX_RATES &&\
            sizeof(S_One_Demand)==SrcLen))
  {
    return RD_HIS_DATA_DATA_ERR;
  }

  DI=Calc_HIS_Data_DI(HIS_DEMAND_DATA,pTime);
  if(ASSERT(A_WARNING,NULL_4BYTES!=DI))
    return WR_HIS_DATA_DI_ERR;

  //数据长度必须满足在参数规定范围内
  if(ASSERT(A_WARNING,ONE_DEMAND_SAVE_SIZE==SrcLen))
  {
    return WR_HIS_DATA_LEN_ERR;    
  }  
  //写存储数据,先计算数据在该DI内的存储偏移
  Off=DEMAND_RATE_OFF(Rate);
  Re=Write_Storage_Data_Fix_Len(DI,Off,pSrc,ONE_DEMAND_SAVE_SIZE);
  if(ASSERT(A_WARNING,NO_ERR==Re))//写入数据
    return Re;
  else
    return NO_ERR;
}
#undef HIS_DATA_C

        
