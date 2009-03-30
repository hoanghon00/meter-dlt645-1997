#ifndef TIME_H
#define TIME_H

#include "Energy.h"

#undef EXT
#ifndef TIME_C
#define EXT extern
#else
#define EXT
#endif

#define UNIT_MIN 2
#define UNIT_HOU 3
#define UNIT_DAY 4
#define UNIT_MON 5

#ifdef TIME_C
CONST INT8U Month0[12]={31,28,31,30,31,30,31,31,30,31,30,31};//平年每月日数
CONST INT8U Month1[12]={31,29,31,30,31,30,31,31,30,31,30,31};//润年每月日数
#endif

EXT void Dec_Min(INT8U Time[],INT8U Mins);
EXT void Dec_Hour(INT8U Time[],INT8U Hours);
EXT void Dec_Day(INT8U Time[],INT8U Days);
EXT void Dec_Month(INT8U Time[],INT8U Months);
EXT void Dec_Year(INT8U Time[],INT8U Years);

EXT void Inc_Min(INT8U Time[],INT8U Mins);
EXT void Inc_Hour(INT8U Time[],INT8U Hours);
EXT void Inc_Day(INT8U Time[],INT8U Days);
EXT void Inc_Month(INT8U Time[],INT8U Months);
EXT void Inc_Year(INT8U Time[],INT8U Years);

EXT void ConvertTime(INT8U Time[],INT16U Interval,INT8U Unit);
EXT INT8U Cmp_Time(S_HEX_Time *pTime1,S_HEX_Time *pTime2);
EXT INT8U _Cmp_Time(S_HEX_Time *pTime1,S_HEX_Time *pTime2);

EXT void Time_Inc(S_HEX_Time *pTime0,INT16U Interval,INT8U Unit,S_HEX_Time *pTime1);
EXT void Time_Dec(S_HEX_Time *pTime0,INT16U Interval,INT8U Unit,S_HEX_Time *pTime1);

EXT INT8U Week(INT8U y,INT8U m,INT8U d);
INT32U mktime(const INT16U year0, const INT16U mon0,\
       const INT16U day, const INT16U hour,\
       const INT16U min, const INT16U sec);
#endif
