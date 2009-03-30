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
**  Filename :	Int.c
**  Abstract :	This file implements device driver for INT module.
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
**	This function initializes the INT module.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void INTP_Init( void )
{
	EGP0 = INTP_EGP_RESET_VALUE;
	EGN0 = INTP_EGN_RESET_VALUE;
	EGP1 = INTP_EGP_RESET_VALUE;
	EGN1 = INTP_EGN_RESET_VALUE;
	PMK0 = 1;			/* INTP0 disable */
	PIF0 = 0;			/* INTP0 IF clear */
	PMK1 = 1;			/* INTP1 disable */
	PIF1 = 0;			/* INTP1 IF clear */
	PMK3 = 1;			/* INTP3 disable */
	PIF3 = 0;			/* INTP3 IF clear */
	PMK4 = 1;			/* INTP4 disable */
	PIF4 = 0;			/* INTP4 IF clear */
        PMK5 = 1;			/* INTP5 disable */
	PIF5 = 0;			/* INTP5 IF clear */
	PMK6 = 1;			/* INTP6 disable */
	PIF6 = 0;			/* INTP6 IF clear */
	PMK7 = 1;			/* INTP7 disable */
	PIF7 = 0;			/* INTP7 IF clear */
	PMK8 = 1;			/* INTP8 disable */
	PIF8 = 0;			/* INTP8 IF clear */
	PMK9 = 1;			/* INTP9 disable */
	PIF9 = 0;			/* INTP9 IF clear */
	PMK10 = 1;			/* INTP10 disable */
	PIF10 = 0;			/* INTP10 IF clear */
	PMK11 = 1;			/* INTP11 disable */
	PIF11 = 0;			/* INTP11 IF clear */
	PM12 |= 0x01;		/* INTP0 pin setting */
	PM3 |= 0x01;		/* INTP3 pin setting */
	PM3 |= 0x02;		/* INTP4 pin setting */
        PM1 |= 0x40;		/* INTP5 pin setting */
	PM14 |= 0x01;		/* INTP6 pin setting */
	PM14 |= 0x02;		/* INTP7 pin setting */
	PM7 |= 0x10;		/* INTP8 pin setting */
	PM7 |= 0x20;		/* INTP9 pin setting */
	PM7 |= 0x40;		/* INTP10 pin setting */
	PM7 |= 0x80;		/* INTP11 pin setting */

#if METER_HARD_TYPE==HARD_TYPE_20081005
        EGN0 = INTP7_EDGE_FALLING_SEL | INTP6_EDGE_FALLING_SEL | INTP5_EDGE_FALLING_SEL   | INTP4_EDGE_FALLING_SEL | INTP3_EDGE_FALLING_SEL | INTP0_EDGE_FALLING_SEL;
	EGN1 = INTP11_EDGE_FALLING_SEL | INTP10_EDGE_FALLING_SEL | INTP9_EDGE_FALLING_SEL | INTP8_EDGE_FALLING_SEL;

#endif
        
#if METER_HARD_TYPE==HARD_TYPE_20090224      
        EGN0 = INTP7_EDGE_FALLING_SEL | INTP6_EDGE_FALLING_SEL | INTP5_EDGE_FALLING_SEL   | INTP4_EDGE_FALLING_SEL | INTP3_EDGE_FALLING_SEL | INTP0_EDGE_FALLING_SEL;
	EGN1 = INTP11_EDGE_FALLING_SEL | INTP10_EDGE_FALLING_SEL | INTP9_EDGE_FALLING_UNSEL | INTP8_EDGE_FALLING_SEL;
#endif    

      //INTP_User_Init();
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes the key return module.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/

void KEY_Init( void )
{
	KRMK = 1;			/* INTKR disable */
	KRIF = 0;			/* INTKR IF clear */
	//PM7 |= 0x02;		/* KR1 pin setting */
	//PM7 |= 0x04;		/* KR2 pin setting */
	PM7 |= 0x08;		/* KR3 pin setting */
        PM7 |= 0x01;		// P70:编程按钮，由查询改为中断方式----------PUCK
      //KRM = KR0_SIGNAL_DETECT_OFF | KR1_SIGNAL_DETECT_ON | KR2_SIGNAL_DETECT_ON | KR5_SIGNAL_DETECT_OFF | KR7_SIGNAL_DETECT_OFF | KR4_SIGNAL_DETECT_OFF | KR3_SIGNAL_DETECT_ON | KR6_SIGNAL_DETECT_OFF;
#if METER_HARD_TYPE==HARD_TYPE_20081005
        KRM = KR0_SIGNAL_DETECT_ON | KR1_SIGNAL_DETECT_OFF | KR2_SIGNAL_DETECT_OFF | KR5_SIGNAL_DETECT_OFF | KR7_SIGNAL_DETECT_OFF | KR4_SIGNAL_DETECT_OFF | KR3_SIGNAL_DETECT_ON | KR6_SIGNAL_DETECT_OFF;
#endif
        
#if METER_HARD_TYPE==HARD_TYPE_20090224
        KRM = KR0_SIGNAL_DETECT_OFF | KR1_SIGNAL_DETECT_ON | KR2_SIGNAL_DETECT_ON | KR5_SIGNAL_DETECT_OFF | KR7_SIGNAL_DETECT_OFF | KR4_SIGNAL_DETECT_OFF | KR3_SIGNAL_DETECT_ON | KR6_SIGNAL_DETECT_OFF;
#endif
	/* INTKR priority level2 */
	
       
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function sets maskable interrupt status.
**
**  Parameters:
**	name : Interrupt source name { INT_xxx }
**	enableflag :	Enable status
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/

void INT_MaskableInterruptEnable( enum MaskableSource name, BOOL enableflag )
{
	if( !enableflag ){
		if( name < 8){
			MK0L |= ( UCHAR )( 0x01 << name );
		}
		else if( name < 16 ){
			MK0H |= ( UCHAR )( 0x01 << ( name - 8 ) );
		}
		else if( name < 24 ){
			MK1L |= ( UCHAR )( 0x01 << ( name - 16 ) );
		}
		else if( name < 32 ){
			MK1H |= ( UCHAR )( 0x01 << ( name - 24 ) );
		}
		else if( name < 40 ){
			MK2L |= ( UCHAR )( 0x01 << ( name - 32 ) );
		}
		else{
			MK2H |= ( UCHAR )( 0x01 << ( name - 40 ) );
		}
	}
	else{	
		if( name < 8){
			MK0L &= ~( UCHAR )( 0x01 << name );
		}
		else if( name < 16 ){
			MK0H &= ~( UCHAR )( 0x01 << ( name - 8 ));
		}
		else if( name < 24 ){
			MK1L &= ~( UCHAR )( 0x01 << ( name - 16 ));
		}
		else if( name < 32 ){
			MK1H &= ~( UCHAR )( 0x01 << ( name - 24 ));
		}
		else if( name < 40 ){
			MK2L &= ~( UCHAR )( 0x01 << ( name - 32 ));
		}
		else{
			MK2H &= ~( UCHAR )( 0x01 << ( name - 40 ));
		}
	}	
}

/* Start adding user code. Do not edit comment generated here */
/* End user code adding. Do not edit comment generated here */





