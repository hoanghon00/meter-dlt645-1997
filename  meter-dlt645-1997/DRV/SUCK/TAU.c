/*
*******************************************************************************
**
**  This device driver was created by Applilet2 for 78K0R/Kx3
**  16-Bit Single-Chip Microcontrollers
**
**  Copyright(C) NEC Electronics Corporation 2002 - 2008
**  All rights reserved by NEC Electronics Corporation.
**
**  This program should be used on your own responsibility.
**  NEC Electronics Corporation assumes no responsibility for any losses
**  incurred by customers or third parties arising from the use of this file.
**
**  Filename :	TAU.c
**  Abstract :	This file implements device driver for TAU module.
**  APIlib :	Applilet2 for 78K0R/Kx3 V2.30 [18 Oct. 2007]
**
**  Device :	uPD78F1166_A0
**
**  Compiler :	IAR_C
**
**  Creation date:	2008-6-10
**  
*******************************************************************************
*/

/*
*******************************************************************************
** Include files
*******************************************************************************
*/
/* Start user code for include definition. Do not edit comment generated here */
/* End user code for include definition. Do not edit comment generated here */
#include "Pub_PUCK.h"

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
/* Start user code for global definition. Do not edit comment generated here */
/* End user code for global definition. Do not edit comment generated here */

/* Channel 6 used as interval timer */
void Set_Fast_Timer(INT8U  Mode)
{
  if(RUN_MODE==Mode)
  {
   //FX1,10us中断一次，实际9.94647
    TMR06 = TAU_TMR0_INITIALVALUE | TAU_CLOCK_SELECT_CK00 | TAU_CLOCK_MODE_CKS | TAU_MODE_INTERVAL_TIMER | TAU_START_INT_USED;
    TIS0 &= ~TAU_CH6_INPUT_FXT;
    if(Get_Main_Clock_Div()==0)
      TDR06 = 0x6d;	
    else
      TDR06 = 0x36;	
    TOM0 &= ~TAU_CH6_OUTPUT_COMBIN;
    TOL0 &= ~TAU_CH6_OUTPUT_LEVEL_L;
    TOE0 &= ~TAU_CH6_OUTPUT_ENABLE;	
    return ;    
  }
  if(FXT_RUN_MODE==Mode)
  {
    //FXT/4,1000us中断一次，实际976.5625us
    TMR06 = TAU_TMR0_INITIALVALUE | TAU_CLOCK_SELECT_CK00 | TAU_CLOCK_MODE_TI0N | TAU_MODE_INTERVAL_TIMER | TAU_START_INT_USED;
    TIS0 |= TAU_CH6_INPUT_FXT;
    TDR06 = 0x7;	
    TOM0 &= ~TAU_CH6_OUTPUT_COMBIN;
    TOL0 &= ~TAU_CH6_OUTPUT_LEVEL_L;
    TOE0 &= ~TAU_CH6_OUTPUT_ENABLE;    
  }    
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initiate the TAU module.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
/*********************************************************************
PUCK：TAU用法
TIO_00：TI00:开大盖查询检测；TO00:开端盖查询检测
TIO_01：与INTP5复用，遥控器解码，做外部中断用；
TIO_02：IRDA的PWM；
TIO_03：与INTP4复用，掉电检测，做外部中断用；
TIO_04：做dataflash的CS用；
TIO_05：全失压外部计数器；
TIO_06：TI06/TO06共用，做100us/10us定时器；
TIO_07：系统1ms时隙输出，优先级最高。
**********************************************************************/
void TAU_Init(void)
{
	TAU0EN = 1;			/* supplies input clock */
	TPS0 = TAU_CK00_FCLK_0 | TAU_CK01_FCLK_15;
	/* Stop all channels */
	TT0 = TAU_CH0_STOP_TRG_ON | TAU_CH1_STOP_TRG_ON | TAU_CH2_STOP_TRG_ON | TAU_CH3_STOP_TRG_ON | TAU_CH4_STOP_TRG_ON | TAU_CH5_STOP_TRG_ON | TAU_CH6_STOP_TRG_ON | TAU_CH7_STOP_TRG_ON;

       	/* Mask channel 1 interrupt */
	TMMK01 = 1;		/* INTTM01 disabled */
	TMIF01 = 0;		/* INTTM01 interrupt flag clear */
        
	/* Mask channel 2 interrupt */
	TMMK02 = 1;		/* INTTM02 disabled */
	TMIF02 = 0;		/* INTTM02 interrupt flag clear */
        
        /* Mask channel 5 interrupt */
	TMMK05 = 1;		/* INTTM05 disabled */
	TMIF05 = 0;		/* INTTM05 interrupt flag clear */
	
        /* Mask channel 6 interrupt */
	TMMK06 = 1;		/* INTTM06 disabled */
	TMIF06 = 0;		/* INTTM06 interrupt flag clear */
        
        /* Mask channel 7 interrupt */
	TMMK07 = 1;		/* INTTM07 disabled */
	TMIF07 = 0;		/* INTTM07 interrupt flag clear */
        
        /* Set INTTM01 low priority */
	TMPR001 = 1;
	TMPR101 = 1;
        
	/* Set INTTM02 low priority */
	TMPR002 = 1;
	TMPR102 = 1;
        
        /* Set INTTM05 low priority */
	TMPR005 = 1;
	TMPR105 = 1;
        
        /* Set INTTM06 low priority */
	TMPR006 = 0;
	TMPR106 = 1;
        
	/* Set INTTM07 low priority */  //1MS中断优先级最高-------PUCK
	TMPR007 = 0;
	TMPR107 = 0;
        
        
	/* Channel 1 is used to measure input pulse interval 
	TMR01 = TAU_TMR0_INITIALVALUE | TAU_CLOCK_SELECT_CK00 | TAU_CLOCK_MODE_CKS | TAU_TRIGGER_TI0N_VALID | TAU_TI0N_EDGE_FALLING | TAU_MODE_CAPTURE | TAU_START_INT_USED;
	TOM0 &= ~TAU_CH1_OUTPUT_COMBIN;
	TOL0 &= ~TAU_CH1_OUTPUT_LEVEL_L;
	TOE0 &= ~TAU_CH1_OUTPUT_ENABLE;
	NFEN1 |= TAU_CH1_NOISE_ON;*/
        
	/* Channel 2 used as square output function */
	TMR02 = TAU_TMR0_INITIALVALUE | TAU_CLOCK_SELECT_CK00 | TAU_CLOCK_MODE_CKS | TAU_MODE_INTERVAL_TIMER | TAU_START_INT_UNUSED;
        if(Get_Main_Clock_Div()==0)
	  TDR02 = TAU_TDR02_VALUE;	
        else
          TDR02 = TAU_TDR02_VALUE/2;
	TOM0 &= ~TAU_CH2_OUTPUT_COMBIN;
	TOL0 &= ~TAU_CH2_OUTPUT_LEVEL_L;	
	TO0 &= ~TAU_CH2_OUTPUT_VALUE_1;
	TOE0 |= TAU_CH2_OUTPUT_ENABLE;

        /* Channel 5 used as external event counter */
	TMR05 = TAU_TMR0_INITIALVALUE | TAU_CLOCK_SELECT_CK00 | TAU_CLOCK_MODE_TI0N | TAU_TI0N_EDGE_FALLING | TAU_MODE_EVENT_COUNT;
	TDR05 = TAU_TDR05_VALUE;	
	TOM0 &= ~TAU_CH5_OUTPUT_COMBIN;
	TOL0 &= ~TAU_CH5_OUTPUT_LEVEL_L;
	TOE0 &= ~TAU_CH5_OUTPUT_ENABLE;	
	NFEN1 |= TAU_CH5_NOISE_ON;
        
        
	/* Channel 7 used as interval timer */
	TMR07 = TAU_TMR0_INITIALVALUE | TAU_CLOCK_SELECT_CK00 | TAU_CLOCK_MODE_CKS | TAU_MODE_INTERVAL_TIMER | TAU_START_INT_USED;
	if(Get_Main_Clock_Div()==0)
          TDR07 = TAU_TDR07_VALUE;
        else
          TDR07 = TAU_TDR07_VALUE/2;
            
	TOM0 &= ~TAU_CH7_OUTPUT_COMBIN;
	TOL0 &= ~TAU_CH7_OUTPUT_LEVEL_L;
	TOE0 &= ~TAU_CH7_OUTPUT_ENABLE;	

        /* TI00(P00) input */
	PM0 |= 0x01;

        /* TI01(P16) input */
	PM1 |= 0x40;
        
	/* TO02(P17) output */
	PM1 &= (UCHAR)~0x80;
	P1 &= (UCHAR)~0x80;
        
        /* TI05(P46) input */
	PM4 |= 0x40;

}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts TAU channel 1 counter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void TAU_Channel1_Start( void )
{
	TMMK01 = 0;		/* INTTM01 enabled */
	TS0 |= TAU_CH1_START_TRG_ON;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops TAU channel 1 counter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void TAU_Channel1_Stop( void )
{
	TT0 |= TAU_CH1_STOP_TRG_ON;
	/* Mask channel 1 interrupt */
	TMMK01 = 1;		/* INTTM01 disabled */
	TMIF01 = 0;		/* INTTM01 interrupt flag clear */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts TAU channel 2 counter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void TAU_Channel2_Start( void )
{
	TOE0 |= TAU_CH2_OUTPUT_ENABLE;
	TS0 |= TAU_CH2_START_TRG_ON;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops TAU channel 2 counter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void TAU_Channel2_Stop( void )
{
	TT0 |= TAU_CH2_STOP_TRG_ON;
	TOE0 &= ~TAU_CH2_OUTPUT_ENABLE;
	/* Mask channel 2 interrupt */
	TMMK02 = 1;		/* INTTM02 disabled */
	TMIF02 = 0;		/* INTTM02 interrupt flag clear */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts TAU channel 5 counter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void TAU_Channel5_Start( void )
{
	TMMK05 = 0;		/* INTTM05 enabled */
	TS0 |= TAU_CH5_START_TRG_ON;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops TAU channel 5 counter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void TAU_Channel5_Stop( void )
{
	TT0 |= TAU_CH5_STOP_TRG_ON;
	/* Mask channel 5 interrupt */
	TMMK05 = 1;		/* INTTM05 disabled */
	TMIF05 = 0;		/* INTTM05 interrupt flag clear */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts TAU channel 6 counter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void TAU_Channel6_Start( void )
{
	TMMK06 = 0;		/* INTTM06 enabled */
	TS0 |= TAU_CH6_START_TRG_ON;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops TAU channel 6 counter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void TAU_Channel6_Stop( void )
{
	TT0 |= TAU_CH6_STOP_TRG_ON;
	/* Mask channel 6 interrupt */
	TMMK06 = 1;		/* INTTM06 disabled */
	TMIF06 = 0;		/* INTTM06 interrupt flag clear */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts TAU channel 7 counter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void TAU_Channel7_Start( void )
{
	TMMK07 = 0;		/* INTTM07 enabled */
	TS0 |= TAU_CH7_START_TRG_ON;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops TAU channel 7 counter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void TAU_Channel7_Stop( void )
{
	TT0 |= TAU_CH7_STOP_TRG_ON;
	/* Mask channel 7 interrupt */
	TMMK07 = 1;		/* INTTM07 disabled */
	TMIF07 = 0;		/* INTTM07 interrupt flag clear */
}
/* Start adding user code. Do not edit comment generated here */
/* End user code adding. Do not edit comment generated here */


