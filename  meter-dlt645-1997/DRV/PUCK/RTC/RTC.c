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
**  Filename :	RTC.c
**  Abstract :	This file implements device driver for RTC module.
**  APIlib :	Applilet2 for 78K0R/Kx3 V2.30 [18 Oct. 2007]
**
**  Device :	uPD78F1166_A0
**
**  Compiler :	IAR_C
**
**  Creation date:	2008-6-29
**  
*******************************************************************************
*/

/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "Pub_PUCK.h"
/* Start user code for include definition. Do not edit comment generated here */
/* End user code for include definition. Do not edit comment generated here */

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
const UCHAR convert24hto12h[36] = {0x12, 0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0,0,0,0,0,0,0x10,0x11,0x32,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0,0,0,0,0,0,0x28,0x29,0x30,0x31};
const UCHAR convert12hto24h[51] = {0, 0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0,0,0,0,0,0,0x10,0x11,0x0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x20,0x21,0,0,0,0,0,0,0x22,0x23,0x12};
/* Start user code for global definition. Do not edit comment generated here */
/* End user code for global definition. Do not edit comment generated here */


/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes the RTC module.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_Init( void )
{
	RTCEN = 1;		/* RTC clock supply */
	/* Real-time counter setting */
	RTCE = 0;		/* RTC counter disable */
	RTCC0 = RTC_RTCC0_INITIALVALUE | RTC_24HOUR_SYSTEM | RTC_INTRTC_CLOCK_1 | RTC_RTC1HZ_DISABLE;
	RTCC1 = RTC_RTCC1_INITIALVALUE
			& ( ~RTC_ALARM_ENABLE & ~RTC_ALARM_INT_ENABLE & ~RTC_ALARM_MATCH & ~RTC_INTC_GENERATE_FLAG & ~RTC_COUNTER_PAUSE );
	/* Correction function setting */
	SUBCUD = RTC_CORRECTION_EVERY60S | RTC_CORRECTION_INCREASE | RTC_CORRECTION_VALUE;
	RTCIF = 0;		/* clear INTRTC interrupt flag */
	RTCMK = 1;		/* INTRTC disable */
        
         /* INTRTC priority H */
        
        RTCPR1 = 1;
	RTCPR0 = 1;//ÖÐ¶ÏµÈ¼¶£º3
        
        
        
	RTCC2 = RTC_RTCC2_INITIALVALUE & ( ~RTC_INTRTCI_GENERATE & ~RTC_RTCDIV_ENABLE); 
	RTCIIF = 0;		/* clear INTRTCI interrupt flag */
	RTCIMK = 1;		/* INTRTCI disable */
        
        START_RTC_COUNTER;
        STOP_RTC_ALARM;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function enables the real-time counter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_CounterEnable( void )
{
	RTCE = 1;	/* counter enable */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function disables the real-time counter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_CounterDisable( void )
{
	RTCE = 0;	/* counter disable */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function chooses 12-hour system or 24-hour system.
**
**  Parameters:
**	ampm :	the selection of real-time counter hour system
**
**  Returns:
**	MD_OK
**	MD_ARGERROR
**	MD_BUSY
**
**-----------------------------------------------------------------------------
*/
MD_STATUS RTC_SetHourSystem( enum RTCHourSystem hoursystem )
{
	int i;
	RWAIT = 1;
	for ( i = 0; i < RTC_WAITTIME; i++ ) {
		NOP();
	}
	if ( RWST == 0 )
	{
		return MD_BUSY;
	}
	switch (hoursystem)
	{
	case HOUR12:
		if((RTCC0 & RTC_RTCC0_AMPM) == 0x08)
		{
			HOUR = convert24hto12h[HOUR];
			RTCC0 &= ~RTC_HOURSYSTEM_CLEAR;			
		}
		break;
	case HOUR24:
		if((RTCC0 & RTC_RTCC0_AMPM) == 0x00)
		{
			HOUR = convert12hto24h[HOUR];
			RTCC0 |= RTC_24HOUR_SYSTEM;			
		}
		break;
	default:
		RWAIT = 0;
		for ( i = 0; i < RTC_WAITTIME; i++ ) {
			NOP();
		}
		if ( RWST == 1 ) 
		{
			return MD_BUSY;
		}
		return MD_ARGERROR;
	}
	RWAIT = 0;
	for ( i = 0; i < RTC_WAITTIME; i++ ) {
		NOP();
	}
	if ( RWST == 1 )
	{
		return MD_BUSY;
	}
	return MD_OK;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function reads the results of real-time counter and store them in the variables.
**
**  Parameters:
**	*CounterReadVal :	the current real-time counter value(BCD code)
**
**  Returns:
**	MD_OK
**	MD_BUSY
**
**-----------------------------------------------------------------------------
*/
MD_STATUS RTC_CounterGet( struct RTCCounterValue* CounterReadVal )
{
	int i;
	RWAIT = 1;
	for ( i = 0; i < RTC_WAITTIME; i++ ) 
        {
          if(RWST)
              break;
	}
	if ( i == RTC_WAITTIME-1) 
        {
		return MD_BUSY;
	}
	
	CounterReadVal->Sec = SEC;
	CounterReadVal->Min = MIN;
	CounterReadVal->Hour = HOUR;
	CounterReadVal->Week = WEEK;
	CounterReadVal->Day = DAY;
	CounterReadVal->Month = MONTH;
	CounterReadVal->Year = YEAR;
	
	RWAIT = 0;
	for ( i = 0; i < RTC_WAITTIME; i++ ) 
        {
          if(RWST==0)
            break;
	}
	if ( i == RTC_WAITTIME-1 ) 
        {
		return MD_BUSY;
	}
	return MD_OK;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function changes the real-time counter value.
**
**  Parameters:
**	CounterWriteVal :	the expected real-time counter value(BCD code)
**
**  Returns:
**	MD_OK
**	MD_BUSY
**
**-----------------------------------------------------------------------------
*/
MD_STATUS RTC_CounterSet( struct RTCCounterValue CounterWriteVal )
{
	int i;
	RWAIT = 1;
	for ( i = 0; i < RTC_WAITTIME; i++ ) 
        {
          if(RWST)
            break;
	}
	if ( i == RTC_WAITTIME-1 )
        {
		return MD_BUSY;
	}
	
	SEC = CounterWriteVal.Sec;
	MIN = CounterWriteVal.Min;
	HOUR = CounterWriteVal.Hour;
	WEEK = CounterWriteVal.Week;
	DAY = CounterWriteVal.Day;
	MONTH = CounterWriteVal.Month;
	YEAR = CounterWriteVal.Year;

	RWAIT = 0;
	for ( i = 0; i < RTC_WAITTIME; i++ ) 
        {
          if(RWST==0)
            break;
	}
	if ( i == RTC_WAITTIME-1 ) 
        {
		return MD_BUSY;
	}
	return MD_OK;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts constant-period interrupt.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_IntervalStart( void )
{
	RINTE = 1;
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
void RTC_IntervalStop( void )
{
	RINTE = 0;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts the alarm operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_AlarmEnable( void )
{
	RTCMK = 0;	/* INTRTC enable */
	WALE = 1;	/* alarm enable */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops the alarm operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_AlarmDisable( void )
{
	WALE = 0;	/* alarm disable */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function sets RTC alarm value.
**
**  Parameters:
**	AlarmVal :	the expected RTC alarm value(BCD code)
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_AlarmSet( struct RTCAlarmValue AlarmVal )
{
	WALE = 0;
	ALARMWM = AlarmVal.Alarmwm;
	ALARMWH = AlarmVal.Alarmwh;
	ALARMWW = AlarmVal.Alarmww;
	WALE = 1;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function gets RTC alarm value.
**
**  Parameters:
**	*AlarmVal :	the address to save RTC alarm value(BCD code)
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_AlarmGet( struct RTCAlarmValue* AlarmVal )
{
	WALE = 0;
	AlarmVal->Alarmwm = ALARMWM;
	AlarmVal->Alarmwh = ALARMWH;
	AlarmVal->Alarmww = ALARMWW;
	WALE = 1;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function enables period interrupt.
**
**  Parameters:
**	period:	the constant period of INTRTC
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_ConstPeriodInterruptEnable( enum RTCINTPeriod period )
{
 	RTCMK = 0;		/* INTRTC enable */
	RTCC0 = (RTCC0 & 0xB8 ) | period;
	RTCIF = 0;		/* clear INTRTC interrupt flag */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function disables period interrupt.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_ConstPeriodInterruptDisable( void )
{
	RTCC0 &= 0xB8;
	RTCIF = 0;		/* clear INTRTC interrupt flag */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function enables RTC interval interrupt.
**
**  Parameters:
**	interval:	the interval value of RTC interval interrupt
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_IntervalInterruptEnable( enum RTCINTInterval interval )
{
	RTCC2 = (RTCC2 & 0x60) | interval;
	RINTE = 1;
	RTCIIF = 0;
	RTCIMK = 0;		/* INTRTCI enable */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function disables RTC interval interrupt.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_IntervalInterruptDisable( void )
{
	RINTE = 0;
	RTCIMK = 1;
	RTCIIF = 0;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function enables RTC RTCCL output.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_RTCCL_OutputEnable( void )
{
	RCLOE0 = 1;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function disables RTC RTCCL output.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_RTCCL_OutputDisable( void )
{
	RCLOE0 = 0;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function enables RTC RTCDIV output.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_RTCDIV_OutputEnable( void )
{
	RCLOE2 = 1;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function disables RTC RTCDIV output.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_RTCDIV_OutputDisable( void )
{
	RCLOE2 = 0;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function enables RTC RTC1HZ output.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_RTC1HZ_OutputEnable( void )
{
	RCLOE1 = 1;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function disables RTC RTC1HZ output.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_RTC1HZ_OutputDisable( void )
{
	RCLOE1 = 0;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function changes the correction value.
**
**  Parameters:
**	timing :	watch error correction timing
**	correctVal :	7-bit correction register value
**
**  Returns:
**	MD_OK
**	MD_ARGERROR
**
**-----------------------------------------------------------------------------
*/
MD_STATUS RTC_ChangeCorrectionValue( enum RTCCorectionTiming timing, UCHAR correctVal )
{
	switch (timing)
	{
		case EVERY20S:
			SUBCUD = correctVal;
			break;
		case EVERY60S:
			SUBCUD = correctVal | 0x80;
			break;
		default:
			return MD_ARGERROR;
	}
	return MD_OK;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops the clock supplied for RTC.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void RTC_PowerOff( void )
{
	RTCEN = 0;	/* RTC clock stop */
}

/* Start adding user code. Do not edit comment generated here */
/* End user code adding. Do not edit comment generated here */
