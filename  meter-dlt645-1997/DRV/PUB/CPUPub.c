
#define CPU_PUB_C
#include "DrvPub.h"


/**************************************************************
根据不同的工作模式，切换主时钟系统，关闭相应的振荡
**************************************************************/
INT8U  Switch_Main_Osc(INT8U  Mode)
{
  switch(Mode)
  {
    case RUN_MODE:
    case FX1_RUN_MODE:
      if(CG_ChangeClockMode(SYSX1CLK)==MD_OK)
      {/*
        HIOSTOP=1;      //关闭fih
        XTSTOP=0;       //开启fxt*/
        return 1;
      }
      break;
      
    case HALT_MODE:
      if(CG_ChangeClockMode(SUBCLK)==MD_OK)
      {
        MSTOP=1;           //关闭fx
        HIOSTOP=1;         //关闭fih
        /*
        XTSTOP=1;       //关闭fxt
        */
       return 1;        
      }
    case FXT_RUN_MODE:
     if(CG_ChangeClockMode(SUBCLK)==MD_OK)
     {
       MSTOP=1;           //关闭fx
       return 1;
     }
  }
  return 0;
}

