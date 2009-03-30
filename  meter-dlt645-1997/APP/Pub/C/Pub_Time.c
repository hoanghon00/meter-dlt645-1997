#define TIME_C
#include "string.h"
#include "Includes.h"

//判断某年是否是闰年，是则返回1，否则返回0
INT8U Check_Leep_Year(INT8U Year)
{
  INT16U Temp;
  
  Temp=2000+(INT16U)Year;
  if((Temp%4==0 && Temp%100!=0) || (Temp%400==0))//闰年  
    return 1;
  else
    return 0;
}

/*-----------------------------------------------
作用:两种时间格式之间的转换，将Time的格式转换到Time1中
入口:
Time时间格式
Time[0]年,Time[1]月,Time[2]日,Time[3]时,Time[4]分
TIme1时间格式
Time[0]分,Time[1]时,Time[2]日,Time[3]月,Time[4]年
出口:无
------------------------------------------------*/
void Time_Pre_Format(INT8U Time[],INT8U Time1[])
{
  mem_cpy(Time1,Time,5,Time1,5);
}

void Time_Aft_Format(INT8U Time[],INT8U Time1[])
{
   mem_cpy(Time1,Time,5,Time1,5);
}
/*-----------------------------------------------
作用:Time时间基础上减少 Mins分钟
入口:pCommonTask普通任务的参数z
Time[0]分,Time[1]时,Time[2]日,Time[3]月,Time[4]年
出口:无
------------------------------------------------*/
void Dec_Min(INT8U Time[],INT8U Mins)
{
  ASSERT(A_WARNING,Mins<60);
  
  if(Time[0]>=Mins) //分钟  
    Time[0]=Time[0]-Mins;
  else
  {
    Time[0]=Time[0]+60-Mins;
    Dec_Hour(Time,1);
  }
}
/*-----------------------------------------------
作用:Time时间基础上减少 Hours小时
入口:pCommonTask普通任务的参数z
Time[0]分,Time[1]时,Time[2]日,Time[3]月,Time[4]年
出口:无
------------------------------------------------*/
void Dec_Hour(INT8U Time[],INT8U Hours)
{
  ASSERT(A_WARNING,Hours<24);
  
  if(Time[1]>=Hours)
    Time[1]=Time[1]-Hours;
  else
  {
    Time[1]=Time[1]+24-Hours;
    Dec_Day(Time,1);
  }
}	
/*-----------------------------------------------
作用:Time时间基础上减少 Days天
入口:pCommonTask普通任务的参数z
Time[0]分,Time[1]时,Time[2]日,Time[3]月,Time[4]年
出口:无
------------------------------------------------*/
void Dec_Day(INT8U Time[],INT8U Days)
{
  ASSERT(A_WARNING,Days<31);
  
  if(Time[2]>Days)
    Time[2]=Time[2]-Days;
  else
  { 	
    Dec_Month(Time,1);//前追1月    
    //重新计算日
    ASSERT(A_WARNING,Time[3]>=1 && Time[3]<=12);
    
    if(Check_Leep_Year(Time[4]))//闰年
      Time[2]=Time[2]+Month1[Time[3]-1]-Days;
    else
      Time[2]=Time[2]+Month0[Time[3]-1]-Days;    
  }
}
/*-----------------------------------------------
作用:Time时间基础上减少 Months月
入口:pCommonTask普通任务的参数z
Time[0]分,Time[1]时,Time[2]日,Time[3]月,Time[4]年
出口:无
------------------------------------------------*/
void Dec_Month(INT8U Time[],INT8U Months)
{ 
  ASSERT(A_WARNING,Months<12);//最多可以往前推24月，考虑到历史数据可能超过12月因此超过了12
         
  if(Time[3]>Months)
    Time[3]=Time[3]-Months;
  else
  {	
    Time[3]=12+Time[3]-Months;
    Dec_Year(Time,1);
  }
}

/*-----------------------------------------------
作用:Time时间基础上减少 Years年
入口:pCommonTask普通任务的参数z
Time[0]分,Time[1]时,Time[2]日,Time[3]月,Time[4]年
出口:无
------------------------------------------------*/
void Dec_Year(INT8U Time[],INT8U Years)
{ 
  //ASSERT(A_WARNING,Time[4]>Years);
         
  if(Time[4]>=Years)
    Time[4]=Time[4]-Years;
  else
  {	
    Time[4]=100-(Years-Time[4]);
  }
}
/*-----------------------------------------------
作用:Time时间基础上增加 Mins分钟
入口:pCommonTask普通任务的参数z
Time[0]分,Time[1]时,Time[2]日,Time[3]月,Time[4]年
出口:无
------------------------------------------------*/
void Inc_Min(INT8U Time[],INT8U Mins)
{
  ASSERT(A_WARNING,Mins<60);
  
  if(Time[0]+Mins<60) //分钟  
    Time[0]=Time[0]+Mins;
  else
  {
    Time[0]=Mins+Time[0]-60;
    Inc_Hour(Time,1);
  }
}
/*-----------------------------------------------
作用:Time时间基础上增加 Hours 小时
入口:pCommonTask普通任务的参数z
Time[0]分,Time[1]时,Time[2]日,Time[3]月,Time[4]年
出口:无
------------------------------------------------*/
void Inc_Hour(INT8U Time[],INT8U Hours)
{
  ASSERT(A_WARNING,Hours<24);
  
  if(Time[1]+Hours<24)
    Time[1]=Time[1]+Hours;
  else
  {
    Time[1]=Time[1]+Hours-24;
    Inc_Day(Time,1);
  }
}
/*-----------------------------------------------
作用:Time时间基础上增加 Days天
入口:pCommonTask普通任务的参数z
Time[0]分,Time[1]时,Time[2]日,Time[3]月,Time[4]年
出口:无
------------------------------------------------*/
void Inc_Day(INT8U Time[],INT8U Days)
{
  INT8U Month_Days;
  
  ASSERT(A_WARNING,Days<31);
  ASSERT(A_WARNING,Time[3]>=1 && Time[3]<=12);
  
  //计算当月日数
  if(Check_Leep_Year(Time[4]))//闰年
    Month_Days=Month1[Time[3]-1];
  else
    Month_Days=Month0[Time[3]-1];
  
  if(Time[2]+Days<=Month_Days)
    Time[2]=Time[2]+Days;
  else
  { 	
    Time[2]=Time[2]+Days-Month_Days;

    Inc_Month(Time,1);    
  }
}
/*-----------------------------------------------
作用:Time时间基础上增加 Months月
入口:pCommonTask普通任务的参数z
Time[0]分,Time[1]时,Time[2]日,Time[3]月,Time[4]年
出口:无
------------------------------------------------*/
void Inc_Month(INT8U Time[],INT8U Months)
{
  ASSERT(A_WARNING,Months<12);
  
  if(Time[3]+Months<=12)
    Time[3]=Time[3]+Months;
  else
  {	
    Time[3]=Time[3]+Months-12;
    Inc_Year(Time,1);
  }
}

/*-----------------------------------------------
作用:Time时间基础上增加 Years年
入口:pCommonTask普通任务的参数z
Time[0]分,Time[1]时,Time[2]日,Time[3]月,Time[4]年
出口:无
------------------------------------------------*/
void Inc_Year(INT8U Time[],INT8U Years)
{ 
  ASSERT(A_WARNING,Time[4]+Years<100);//最多可以往前推24月，考虑到历史数据可能超过12月因此超过了12
  
  if(Time[4]+Years<100)
    Time[4]+=Years;
  else
    Time[4]=Time[4]+Years-100;
}
/*-----------------------------------------------
作用:将 Interval 时间转化为 Time，Unit为 Interval 的单位
入口:
Time[0]分,Time[1]时,Time[2]日,Time[3]月,Time[4]年
Interval,时间值
Unit，Interval单位,02分钟,3小时，4日,5月
出口:无
------------------------------------------------*/
void ConvertTime(INT8U Time[],INT16U Interval,INT8U Unit)
{
  mem_set(Time,0,5,Time,5);
  if(Unit==UNIT_MIN)
  {
    //Time[3]=Interval/1440/31;
    Time[2]=Interval/1440;
    Time[1]=(Interval%1440)/60;
    Time[0]=Interval%60;
  }
  else if(Unit==UNIT_HOU)
  {
    Time[2]=Interval/24;
    Time[1]=Interval%24; 
  }
  else if(Unit==UNIT_DAY)
  {
    Time[3]=Interval/31;
    Time[2]=Interval%31;
  } 
  else if(Unit==UNIT_MON)
  {
    Time[4]=Interval/12;
    Time[3]=Interval%12;
  }
}
/*-----------------------------------------------
作用:Time时间基础上增加 Interval 个时间值,Interval的单位是Unit
入口:
Time,需要转化的时间值，同时转化好的时间值也存在Time中
Time[0]年,Time[1]月,Time[2]日,Time[3]时,Time[4]分
出口:无
------------------------------------------------*/
void Time_Inc(S_HEX_Time *pTime0,INT16U Interval,INT8U Unit,S_HEX_Time *pTime1)
{
  INT8U Re,Temp[5],TempTime[5];

  Re=Check_HEX_Time(pTime0);
  ASSERT(A_WARNING,0!=Re);
  
  ASSERT(A_WARNING,sizeof(pTime1->Time)==5);
  Time_Pre_Format(pTime0->Time,TempTime);
  
  ConvertTime(Temp,Interval,Unit);
  Inc_Min(TempTime,Temp[0]);
  Inc_Hour(TempTime,Temp[1]);
  Inc_Day(TempTime,Temp[2]);
  Inc_Month(TempTime,Temp[3]);
  Inc_Year(TempTime,Temp[4]);
  
  Time_Aft_Format(TempTime,pTime1->Time);
  Set_STRUCT_Sum(pTime1,sizeof(S_HEX_Time),pTime1->CS,sizeof(pTime1->CS));
}
/*-----------------------------------------------
作用:Time时间基础上减少 Interval 个时间值,Interval的单位是Unit
入口:
Time,需要转化的时间值，同时转化好的时间值也存在Time中
Time[0]年,Time[1]月,Time[2]日,Time[3]时,Time[4]分
出口:无
------------------------------------------------*/
void Time_Dec(S_HEX_Time *pTime0,INT16U Interval,INT8U Unit,S_HEX_Time *pTime1)
{
  INT8U Re,Temp[5],TempTime[5];

  Re=Check_HEX_Time(pTime0);
  ASSERT(A_WARNING,0!=Re);
  
  ASSERT(A_WARNING,sizeof(pTime1->Time)==5);
  Time_Pre_Format(pTime0->Time,TempTime); 
  
  ConvertTime(Temp,Interval,Unit);
  Dec_Min(TempTime,Temp[0]);
  Dec_Hour(TempTime,Temp[1]);
  Dec_Day(TempTime,Temp[2]);
  Dec_Month(TempTime,Temp[3]);
  Dec_Year(TempTime,Temp[4]);
  
  Time_Aft_Format(TempTime,pTime1->Time);
  Set_STRUCT_Sum(pTime1,sizeof(S_HEX_Time),pTime1->CS,sizeof(pTime1->CS));
}

//时间比较函数,返回TIME_EQU表示两时间相等
//返回TIME_AFT表示pTime1在pTime2之后
//返回TIME_BEF表示pTime1在pTime2之前
INT8U Cmp_Time(S_HEX_Time *pTime1,S_HEX_Time *pTime2)
{
  INT8U i,Re;
 
  Re=Check_HEX_Time(pTime1);
  ASSERT(A_WARNING,0!=Re);
  Re=Check_HEX_Time(pTime2);
  ASSERT(A_WARNING,0!=Re);
  
  for(i=0;i<5;i++)
  {
    if(pTime1->Time[4-i]>pTime2->Time[4-i])
      return TIME_AFT; 
    else if(pTime1->Time[4-i]<pTime2->Time[4-i])
      return TIME_BEF;
    else
      continue;  
  }
  return TIME_EQU;
}

//没有断言的时间比较函数，用于在费率方案的时间比较
INT8U _Cmp_Time(S_HEX_Time *pTime1,S_HEX_Time *pTime2)
{
  INT8U i;
 
  for(i=0;i<5;i++)
  {
    if(pTime1->Time[4-i]>pTime2->Time[4-i])
      return TIME_AFT; 
    else if(pTime1->Time[4-i]<pTime2->Time[4-i])
      return TIME_BEF;
    else
      continue;  
  }
  return TIME_EQU;
}

/******************************************************************************************
*     1. imput: bcd-bcd data                                                              *
*        output:hex data                                                                  *
*******************************************************************************************/
INT8U OB_H(INT8U bcd)
{
  return ((bcd&0xf0)>>4)*10+(bcd&0x0f);
}
/******************************************************************************************/

/******************************************************************************************
*     1. use year and month cal numbers of day                                            *
*     2.                                                                                  *
*******************************************************************************************/	
INT8U CalDay(INT8U year,INT8U month) //hex
{
  INT8U result;
  volatile INT8U secondmonth;

  result=0x00;
  if(!(year%4))
    secondmonth=29;
  else secondmonth=28;
  if(month>12)
    month-=12;
  switch(month)
  {
  case 0x01:
  case 0x03:
  case 0x05:
  case 0x07:
  case 0x08:
  case 0x0A:
  case 0x0C: result=31; break;
  case 0x04:
  case 0x06:
  case 0x09:
  case 0x0B: result=30; break;
  case 0x02: result=secondmonth; break;
  }
  return result;
}

/******************************************************************************************
*     1. cal two time diffenrce,1>2 [年]【月】                                                           *
*     2. ruturn second                                                                                *
*******************************************************************************************/

INT32S ComTime(INT8U *t1,INT8U *t2)
{
  INT8U i,j,flag;
  INT32S data1,data2;
  INT32S result;
  INT8U t11[6],t22[6];

  data1=data2=0;
  if(*(t1+0)>*(t2+0))
    flag=0;
  else if(*(t2+0)>*(t1+0))
    flag=0xff;
  else if(*(t1+1)>*(t2+1))
    flag=0;
  else if(*(t2+1)>*(t1+1))
    flag=0xff;
  else if(*(t1+2)>*(t2+2))
    flag=0;
  else if(*(t2+2)>*(t1+2))
    flag=0xff;
  else if(*(t1+3)>*(t2+3))
    flag=0;
  else if(*(t2+3)>*(t1+3))
    flag=0xff;
  else if(*(t1+4)>*(t2+4))
    flag=0;
  else if(*(t2+4)>*(t1+4))
    flag=0xff;

  if(!flag)
  {
    for(i=0;i<6;i++)
    {
      *(t11+i)=OB_H(*(t1+i));
      *(t22+i)=OB_H(*(t2+i));
    }
  }
  else
  {
    for(i=0;i<6;i++)
    {
      *(t11+i)=OB_H(*(t2+i));
      *(t22+i)=OB_H(*(t1+i));
    }	
  }

  for(i=1;i<*(t22+1);i++)
  {
    data2+=CalDay(*(t22+0),i);
  }
  data2+=*(t22+2);

  for(i=*(t22+0);i<*(t11+0);i++)
  {
    for(j=1;j<=12;j++)
      data1+=CalDay(i,j);
  }

  for(i=1;i<*(t11+1);i++)
  {
    data1+=CalDay(*(t11+0),i);
  }
  data1+=*(t11+2);

  if(!flag)
    result=data1-data2;
  else
    result=data2-data1;

  result=result*86400l+OB_H(*(t1+3))*3600l-OB_H(*(t2+3))*3600l
    +OB_H(*(t1+4))*60l-OB_H(*(t2+4))*60l
      +OB_H(*(t1+5))-OB_H(*(t2+5));
  return result;

}

CONST INT8U WeekTab[] = {//闰年月星期表
  (3 << 5) + 31,//1月
  (6 << 5) + 29,//2月
  (0 << 5) + 31,//3月
  (3 << 5) + 30,//4月
  (5 << 5) + 31,//5月
  (1 << 5) + 30,//6月
  (3 << 5) + 31,//7月
  (6 << 5) + 31,//8月
  (1 << 5) + 30,//9月
  (4 << 5) + 31,//10月
  (0 << 5) + 30,//11月
  (2 << 5) + 31 //12月
};

/*------------------------------------------------------------------------------
----------
                                2000年~2099年星期算法
--------------------------------------------------------------------------------
---------*/
INT8U Week(INT8U y, INT8U m, INT8U d)
{
  INT8U week, day;
  day = WeekTab[m - 1];//月表
  week = day >> 5;//月星期数
  day &= 0x1f;//月天数
  if ((m < 3) && (y & 0x03)){//平年
    if (m == 2) day--;//平年月天数
    week++;//平年月表+1
  }
  y = y + (y >> 2);//年+年/4
  week = (week +  y + d + 2) % 7;//(星期=年+年/4+月表+2日)%7
  //return (week << 5) | day;//返回星期和月天数
  return week;
}

/* Converts Gregorian date to seconds since 1970-01-01 00:00:00.
* Assumes input in normal date format, i.e. 1980-12-31 23:59:59
* => year=1980, mon=12, day=31, hour=23, min=59, sec=59.
*
* [For the Julian calendar (which was used in Russia before 1917,
* Britain & colonies before 1752, anywhere else before 1582,
* and is still in use by some communities) leave out the
* -year/100+year/400 terms, and add 10.]
*
* This algorithm was first published by Gauss (I think).
*
* WARNING: this function will overflow on 2106-02-07 06:28:16 on
* machines were long is 32-bit! (However, as time_t is signed, we
* will already get problems at other places on 2038-01-19 03:14:08)
*/

INT32U mktime(const INT16U year0, const INT16U mon0,\
               const INT16U day, const INT16U hour,\
               const INT16U min, const INT16U sec)
{
        INT16U mon = mon0, year = year0;

        /* 1..12 -> 11,12,1..10 */
        if (0 >= (int) (mon -= 2)) {
                mon += 12;      /* Puts Feb last since it has leap day */
                year -= 1;
        }

        return ((((INT32U)
                  (year/4 - year/100 + year/400 + 367*mon/12 + day) +
                  year*365 - 719499
            )*24 + hour /* now have hours */
          )*60 + min /* now have minutes */
        )*60 + sec; /* finally seconds */
}

/*
void main()
{
    //char wk[15]="天一二三四五六";
    int y,m,d,w;
    cin>>y>>m>>d;
    w=week(y,m,d);
    cout<<y<<"年"<<m<<"月"<<d<<"日 星期"<<wk[w*2]<<wk[w*2+1]<<endl;
}
*/
#undef TIME_C
