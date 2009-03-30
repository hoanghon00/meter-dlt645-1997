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
**  Filename :	WatchdogTimer.h
**  Abstract :	This file implements device driver for WatchdogTimer module.
**  APIlib :	Applilet2 for 78K0R/Kx3 V2.30 [18 Oct. 2007]
**
**  Device :	uPD78F1166_A0
**
**  Compiler :	IAR_C
**
**  Creation date:	2008-12-7
**  
*******************************************************************************
*/
#ifndef _MDWATCHDOGTIMER_
#define _MDWATCHDOGTIMER_
/*
*******************************************************************************
**	Register bit define
*******************************************************************************
*/

/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/

/*
*******************************************************************************
**  Function define
*******************************************************************************
*/

#define Clear_CPU_Dog()   {WDTE = 0xac;}


void WDT_Init( void );
void WDT_Restart( void );
/* Start user code for definition. Do not edit comment generated here */
/* End user code for definition. Do not edit comment generated here */
#endif
