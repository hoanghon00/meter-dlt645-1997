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
**  Filename :	systeminit.c
**  Abstract :	This file implements system initializing function.
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


/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function  initializes every macro.
**
**  Parameters:
**	Mode:0----------表示normal下的初始化；1---------------表示resume下的初始化；2-------------表示sleep下的初始化
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void SystemInit(INT32U Mode)
{
	/*Process of reset*/
	CG_ReadResetSource();
	/*Initialize the I/O ports*/
	PORT_Init(Mode);
	/*Initialize the clock generator*/
	Clock_Init(Mode); 
	/*Initialize the external interrupts*/
	INTP_Init();
	/*Initialize the key interrupt*/
	KEY_Init();
	/*Initialize SAU0*/
	//SAU0_Init();
	/*Initialize SAU1*/
	//SAU1_Init();
	/*Initialize the IIC0*/
	//IIC0_Init();
	/*Initialize TAU function*/
	TAU_Init();
	/*Initialize the watchdog timer*/
	WDT_Init();
	/*Initialize the real-time counter*/
	RTC_Init();     
       /*Initialize the low-voltage detector*/
	LVI_Init();
        I2cInit();        
        Init_IIC_Soft();  //开启IIC的软件模拟端口初始化------------PUCK
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function  initializes hardware setting.
**
**  Parameters:
**	Mode:0----------表示normal下的初始化；1---------------表示resume下的初始化；2-------------表示sleep下的初始化
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void hdwinit(INT32U Mode)
{
	DI( );
	SystemInit(Mode);        
	EI( );
}

/* Start adding user code. Do not edit comment generated here */
/* End user code adding. Do not edit comment generated here */


