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
**  Filename :	TAU_user.c
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
volatile ULONG gTauCh1Width = 0;
/* End user code for global definition. Do not edit comment generated here */

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTTM01 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTTM01_vect
__interrupt void MD_INTTM01(void)
{
	if( ( TSR01 & TAU_OVERFLOW_STATUS ) != 0 )		/* Overflow occurs */
	{			
		gTauCh1Width = ( ULONG )TDR01 + 0x10000;
	}
	else
	{
		gTauCh1Width = ( ULONG )TDR01;
	}
        //IR_Decode_Proc(gTauCh1Width);
        //gTauCh1Width=0;
        
	/* Start user code. Do not edit comment generated here */
	/* End user code. Do not edit comment generated here */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTTM02 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTTM02_vect
__interrupt void MD_INTTM02(void)
{
	/* Start user code. Do not edit comment generated here */
	/* End user code. Do not edit comment generated here */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTTM07 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTTM05 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTTM05_vect
__interrupt void MD_INTTM05(void)
{
	/* Start user code. Do not edit comment generated here */
      Inter_ALL_LOSS();
	/* End user code. Do not edit comment generated here */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTTM06 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTTM06_vect
__interrupt void MD_INTTM06(void)
{
	/* Start user code. Do not edit comment generated here */
        Fast_Timer();
	/* End user code. Do not edit comment generated here */
}

#pragma vector = INTTM07_vect
__interrupt void MD_INTTM07(void)
{
	/* Start user code. Do not edit comment generated here */
        count_1ms();
	/* End user code. Do not edit comment generated here */
}

/* Start adding user code. Do not edit comment generated here */
/* End user code adding. Do not edit comment generated here */


