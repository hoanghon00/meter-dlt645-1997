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
**  Filename :	RTC_user.c
**  Abstract :	This file implements device driver for RTC module.
**  APIlib :	Applilet2 for 78K0R/Kx3 V2.30 [18 Oct. 2007]
**
**  Device :	uPD78F1166_A0
**
**  Compiler :	IAR_C
**
**  Creation date:	2008-6-27
**  
*******************************************************************************
*/

/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "macrodriver.h"
#include "RTC.h"
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


/* Start adding user code. Do not edit comment generated here */
/*
//写内部的RTC,bcd
void set_rtc_built_in(unsigned char sec,unsigned char min,unsigned char hour,
                 unsigned char day,unsigned char week,unsigned char month,
                 unsigned char year)

{ struct RTCCounterValue CwriteVal;
      
  CwriteVal.Sec = sec;

  CwriteVal.Min = min;

  CwriteVal.Hour = hour;                

  CwriteVal.Day = day;

  CwriteVal.Week= week;                

  CwriteVal.Month = month;

  CwriteVal.Year = year;                

   RTC_CounterEnable( );
   //RTC_CounterDisable( );
   //RTC_IntervalInterruptDisable( );

   RTC_CounterSet(CwriteVal );
   //RTC_CounterWrite(CwriteVal);                
   RTC_CounterEnable( );
   
  
}

//读内部的RTC,数据将被放入全局变量rtc_built_in_struct中
void get_rtc_built_in(void)

{                


  
   RTC_CounterGet( &rtc_built_in_struct );
   
   
  
}
*/
/* End user code adding. Do not edit comment generated here */

#pragma vector = INTRTC_vect
__interrupt void MD_INTRTC(void)
{
	CALL_RTC_ConstantPeriodINT();
	/* Start user code. Do not edit comment generated here */
        
	/* End user code. Do not edit comment generated here */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is  real-time counter constant-period interrupt service handler.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void CALL_RTC_ConstantPeriodINT( void )
{
	/* Start user code. Do not edit comment generated here */
       CPU_RTC_Interrupt();
	/* End user code. Do not edit comment generated here */
}



/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops constant-period interrupt.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/


/* Start adding user code. Do not edit comment generated here */
/* End user code adding. Do not edit comment generated here */
