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
**  Filename :	Int_user.c
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
**	This function is INTP0 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTP0_vect
__interrupt void MD_INTP0(void)
{
	/* Start user code. Do not edit comment generated here */
          Inter_CF2();
	/* End user code. Do not edit comment generated here */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTP6 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTP6_vect
__interrupt void MD_INTP6(void)
{
	/* Start user code. Do not edit comment generated here */
           Inter_CF1();
	/* End user code. Do not edit comment generated here */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTP8 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTP8_vect
__interrupt void MD_INTP8(void)
{
	/* Start user code. Do not edit comment generated here */
          Inter_Down_Key();
	/* End user code. Do not edit comment generated here */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTP9 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTP9_vect
__interrupt void MD_INTP9(void)
{
	/* Start user code. Do not edit comment generated here */
#if METER_HARD_TYPE==HARD_TYPE_20081005 
          Inter_Right_Key();
#endif
	/* End user code. Do not edit comment generated here */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTP10 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/

#pragma vector = INTP1_vect
__interrupt void MD_INTP1(void)
{
	/* Start user code. Do not edit comment generated here */
           //INT_1HZ();
	/* End user code. Do not edit comment generated here */
}

#pragma vector = INTP3_vect
__interrupt void MD_INTP3(void)
{
	/* Start user code. Do not edit comment generated here */
#if METER_HARD_TYPE==HARD_TYPE_20081005
           Irda_Wake_Up();
#endif
	/* End user code. Do not edit comment generated here */
}

#pragma vector = INTP4_vect
__interrupt void MD_INTP4(void)
{
	/* Start user code. Do not edit comment generated here */
           POWER_DOWN();
	/* End user code. Do not edit comment generated here */
}



#pragma vector = INTP5_vect
__interrupt void MD_INTP5(void)
{
	/* Start user code. Do not edit comment generated here */
        IR_Decode_Proc();
	/* End user code. Do not edit comment generated here */
}

#pragma vector = INTP7_vect
__interrupt void MD_INTP7(void)
{
	/* Start user code. Do not edit comment generated here */
           //Inter_ALL_LOSS();
          INT_1HZ();
	/* End user code. Do not edit comment generated here */
}


#pragma vector = INTP10_vect
__interrupt void MD_INTP10(void)
{
	/* Start user code. Do not edit comment generated here */
#if METER_HARD_TYPE==HARD_TYPE_20081005
           Inter_Up_Key();
#endif
        
#if METER_HARD_TYPE==HARD_TYPE_20090224
           Inter_Prg_Key();
#endif
	/* End user code. Do not edit comment generated here */
}


#pragma vector = INTP11_vect
__interrupt void MD_INTP11(void)
{
	/* Start user code. Do not edit comment generated here */
#if METER_HARD_TYPE==HARD_TYPE_20081005
       laser_up();   
#else
       Irda_Wake_Up();
#endif
	/* End user code. Do not edit comment generated here */
}


/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTKR interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTKR_vect
__interrupt void MD_INTKR(void)
{
	/* Start user code. Do not edit comment generated here */
       INT8U key_flag=P7;

#if METER_HARD_TYPE==HARD_TYPE_20081005    
       if((key_flag&0x01)==0)//KR0
       {
         Inter_Prg_Key();         
       }
       if((key_flag&0x08)==0)//KR3
       {
         Inter_Left_Key();
       }
#endif
       
#if METER_HARD_TYPE==HARD_TYPE_20090224        
       if((key_flag&0x02)==0)//KR1
       {
         Inter_Right_Key();         
       }
       if((key_flag&0x04)==0)//KR2
       {
         Inter_Left_Key();         
       }
       if((key_flag&0x08)==0)//KR3
       {
         Inter_Up_Key();
       }
#endif
       
       
       /* End user code. Do not edit comment generated here */
}


/* Start adding user code. Do not edit comment generated here */

/* End user code adding. Do not edit comment generated here */


