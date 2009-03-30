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
**  Filename :	LVI.c
**  Abstract :	This file implements device driver for LVI module.
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

/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "DrvPub.h"

/* Start user code for include definition. Do not edit comment generated here */
/* End user code for include definition. Do not edit comment generated here */


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
**	This function initializes the low-voltage detector.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void LVI_Init( void )
{
	LVION = 0;				/* stop LVI */	
	LVIMK = 1;				/* INTLVI disabled */;
	LVISEL = 0;				/* detect of supply voltage (VDD) */
	LVIS = LVI_LVIS_LEVEL_09;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts the low-voltage detector.
**
**  Parameters:
**	None
**
**  Returns:
**	MD_OK
**	MD_ERROR
**
**-----------------------------------------------------------------------------
*/
MD_STATUS LVI_Start( void )
{
#ifdef LVI_DETECT_EN
	UCHAR status = MD_OK;
	USHORT  i;
	LVION = 1;				/* enable operation */
	/* Wait 10 us */
	for(i=0; i<=LVI_WAITTIME; i++){	
		NOP();
	}
	if(LVIF == 1)
	{
		status = MD_ERROR;
	}
	else
	{
		LVIMD = 1;
	}
	return status;
#else
    return 1;
#endif
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops the low-voltage detector.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void LVI_Stop( void )
{
	LVIMD = 0;
	LVION = 0;				/* operation disable */
}

/* Start adding user code. Do not edit comment generated here */
/* End user code adding. Do not edit comment generated here */


