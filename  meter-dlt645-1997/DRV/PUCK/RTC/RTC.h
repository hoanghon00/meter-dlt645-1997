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
**  Filename :	RTC.h
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
#ifndef _MDRTC_
#define _MDRTC_
/*
*******************************************************************************
**	Register bit define
*******************************************************************************
*/
/*
	Peripheral Enable Register 0 (PER0)
*/
/* Control of real-time counter(RTC) input clock(RTCEN) */
#define RTC_CLOCK_CLR				0x01	/* for clear the bits */
#define RTC_CLOCK_STOP				0x00	/* stops supply of input clock */
#define RTC_CLOCK_SUPPLY			0x01	/* supplies input clock */

/*
	Real-time counter control register 0 (RTCC0)
*/
#define RTC_RTCC0_INITIALVALUE		0x00	/* RTCC0 initial value */
/* Real-time counter operation control(RTCE) */
#define RTC_COUNTER_STOP			0x00	/* stops counter operation */
#define RTC_COUNTER_START			0x80	/* starts counter operation */
/* RTC1HZ pin output control(RCLOE1) */
#define RTC_RTC1HZ_DISABLE			0x00	/* disables output of RTC1HZ pin(1kHz) */
#define RTC_RTC1HZ_ENABLE			0x20	/* enable output of RTC1HZ pin(1kHz) */
/* RTCCL pin output control(RCLOE0) */
#define RTC_RTCCL_DISABLE			0x00	/* disables output of RTCCL pin(32kHz) */
#define RTC_RTCCL_ENABLE			0x10	/* enable output of RTCCL pin(32kHz) */
/* selection of 12-/24-hour system(AMPM) */
#define RTC_HOURSYSTEM_CLEAR		0x08	/* clear the AMPM bit */
#define RTC_RTCC0_AMPM				0x08	/* AMPM bit status detect */
#define RTC_12HOUR_SYSTEM			0x00	/* 12-hour system(a.m. and p.m.. are displayed.) */
#define RTC_24HOUR_SYSTEM			0x08	/* 24-hour system */
/* Interrupt(INTRTC)selection(CT2,CT1,CT0) */
#define RTC_INTRTC_NOT_GENERATE		0x00	/* does not use constant-period interrupt function */
#define RTC_INTRTC_CLOCK_0			0x01	/* once per 0.5s(synchronized with second count up) */
#define RTC_INTRTC_CLOCK_1			0x02	/* once per 1 s(same time as second count up) */
#define RTC_INTRTC_CLOCK_2			0x03	/* once per 1 m(second 00 of every minute) */
#define RTC_INTRTC_CLOCK_3			0x04	/* once per 1 hour(minute 00 and second 00 of every hour) */
#define RTC_INTRTC_CLOCK_4			0x05	/* once per 1 day(hour 00,minute 00,and second 00 of every day) */
#define RTC_INTRTC_CLOCK_5			0x06	/* once per 1 month(Day 1,hour 00a.m., minute 00, and second 00 of every month) */

/*
	Real-time counter control register 1 (RTCC1)
*/
#define RTC_RTCC1_INITIALVALUE		0x00	/* RTCC1 initial value */
/* Alarm operation control(WALE) */
#define RTC_ALARM_DISABLE			0x00	/* alarm match operation is invalid */
#define RTC_ALARM_ENABLE			0x80	/* alarm match operation is valid */
/* Control of alarm interrupt function operation(WALIE) */
#define RTC_ALARM_INT_DISABLE		0x00	/* does not generate interrupt on matching of alarm */
#define RTC_ALARM_INT_ENABLE		0x40	/* generates interrupt on matching of alarm */
/* Alarm detection status flag(WAFG) */
#define RTC_ALARM_MISMATCH			0x00	/* alarm mismatch */
#define RTC_ALARM_MATCH				0x10	/* detection of matching of alarm */
/* Constant-period interrupt status flag(RIFG) */
#define RTC_INTC_NOTGENERATE_FALG	0x00	/* constant-period interrupt is not generated */
#define RTC_INTC_GENERATE_FLAG		0x08	/* constant-period interrupt is generated */
/* Wait control of real-time counter(RWAIT) */
#define RTC_COUNTER_SET				0x00	/* sets counter operation */
#define RTC_COUNTER_PAUSE			0x01	/* stops SEC to YEAR counters. Mode to read or write counter value */

/*
	Real-time counter control register 2 (RTCC2)
*/
#define RTC_RTCC2_INITIALVALUE		0x00	/* RTCC2 initial value */
/* Interrupt(INTRTCI)selection(RINTE,ICT2,ICT1,ICT0) */
#define RTC_INTRTCI_NOT_GENERATE	0x00	/* constant-period interrupt is not generated */
#define RTC_INTRTCI_GENERATE        0x80	/* generate constant-period interrupt */
#define RTC_INTRTCI_CLOCK_0			0x80	/* 2^6/fXT (1.953125ms) */
#define RTC_INTRTCI_CLOCK_1			0x81	/* 2^7/fXT (3.90625ms) */
#define RTC_INTRTCI_CLOCK_2			0x82	/* 2^8/fXT (7.8125ms) */
#define RTC_INTRTCI_CLOCK_3			0x83	/* 2^9/fXT (15.625ms) */
#define RTC_INTRTCI_CLOCK_4			0x84	/* 2^10/fXT (31.25ms) */
#define RTC_INTRTCI_CLOCK_5			0x85	/* 2^11/fXT (62.5ms) */
#define RTC_INTRTCI_CLOCK_6			0x86	/* 2^12/fXT (125ms) */
/* RTCDIV pin output control(RCLOE2) */
#define RTC_RTCDIV_DISABLE			0x00	/* output of RTCDIV pin is disabled */
#define RTC_RTCDIV_ENABLE			0x40	/* output of RTCDIV pin is enabled */
/* Selection of RTCDIV pin output(RCKDIV) */
#define RTC_RTCDIV_512HZ			0x00	/* RTCDIV pin outputs 512Hz */
#define RTC_RTCDIV_16KHZ			0x20	/* RTCDIV pin outputs 16.384kHz */

/*
	Watch error correction register (SUBCUD)
*/
/* Setting of watch error correction timing(DEV) */
#define RTC_CORRECTION_EVERY20S		0x00	/* corrects watch error when the second digits are at 00,20, or 40 */
#define RTC_CORRECTION_EVERY60S		0x80	/* corrects watch error when the second digits are at 00 */
/* Setting of watch error correction method(F6) */
#define RTC_CORRECTION_INCREASE		0x00	/* increase */
#define RTC_CORRECTION_DECREASE		0x40	/* decrease */
/* Setting of watch error correction timing(F5,F4,F3,F2,F1) */
#define RTC_CORRECTION_NULL			0x00	/* setting of watch error correction method */

/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
#define	RTC_CORRECTION_VALUE	0x02
#define	RTC_WAITTIME		140
struct RTCCounterValue 
{
	UCHAR Sec;
	UCHAR Min;
	UCHAR Hour;
	UCHAR Day;
	UCHAR Week;
	UCHAR Month;
	UCHAR Year;
};
struct RTCAlarmValue 
{
	UCHAR Alarmwm;
	UCHAR Alarmwh;
	UCHAR Alarmww;
};
enum RTCHourSystem	{ HOUR12, HOUR24 };
enum RTCCorectionTiming	{ EVERY20S, EVERY60S };
enum RTCINTPeriod	{HALFSEC=1, ONESEC,ONEMIN,ONEHOUR, ONEDAY,ONEMONTH};
enum RTCINTInterval	{INTERVAL0, INTERVAL1,INTERVAL2, INTERVAL3, INTERVAL4, INTERVAL5, INTERVAL6};




/*
*******************************************************************************
**  Function define
*******************************************************************************
*/
void RTC_Init( void );
void RTC_CounterEnable( void );
void RTC_CounterDisable( void );
MD_STATUS RTC_SetHourSystem( enum RTCHourSystem hoursystem );
MD_STATUS RTC_CounterGet( struct RTCCounterValue* CounterReadVal );
MD_STATUS RTC_CounterSet( struct RTCCounterValue CounterWriteVal );
void RTC_IntervalStart( void );
void RTC_IntervalStop( void );
void RTC_AlarmEnable( void );
void RTC_AlarmDisable( void );
void RTC_AlarmSet( struct RTCAlarmValue AlarmVal );
void RTC_AlarmGet( struct RTCAlarmValue* AlarmVal );
void RTC_ConstPeriodInterruptEnable(enum RTCINTPeriod period);
void RTC_ConstPeriodInterruptDisable( void );
void RTC_IntervalInterruptEnable( enum RTCINTInterval interval );
void RTC_IntervalInterruptDisable( void );
void RTC_RTCCL_OutputEnable( void );
void RTC_RTCCL_OutputDisable( void );
void RTC_RTCDIV_OutputEnable( void );
void RTC_RTCDIV_OutputDisable( void );
void RTC_RTC1HZ_OutputEnable( void );
void RTC_RTC1HZ_OutputDisable( void );
MD_STATUS RTC_ChangeCorrectionValue( enum RTCCorectionTiming timing, UCHAR correctVal );
void RTC_PowerOff( void );
void RTC_User_Init( void );
void CALL_RTC_AlarmINT( void );
void CALL_RTC_ConstantPeriodINT( void );
/* Start user code for definition. Do not edit comment generated here */
/* End user code for definition. Do not edit comment generated here */
#endif

