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
**  Filename :	Port.h
**  Abstract :	This file implements device driver for PORT module.
**  APIlib :	Applilet2 for 78K0R/Kx3 V2.30 [18 Oct. 2007]
**
**  Device :	uPD78F1166_A0
**
**  Compiler :	IAR_C
**
**  Creation date:	2008-6-28
**  
*******************************************************************************
*/
#ifndef _MDPORT_
#define _MDPORT_
/*
*******************************************************************************
**	Register bit define
*******************************************************************************
*/
/* Port mode register(PMn.0 to 7) */
#define PMn0_NOT_USE		0x01	/* not use Pn0 as digital I/O */
#define PMn1_NOT_USE		0x02	/* not use Pn1 as digital I/O */
#define PMn2_NOT_USE		0x04	/* not use Pn2 as digital I/O */
#define PMn3_NOT_USE		0x08	/* not use Pn3 as digital I/O */
#define PMn4_NOT_USE		0x10	/* not use Pn4 as digital I/O */
#define PMn5_NOT_USE		0x20	/* not use Pn5 as digital I/O */
#define PMn6_NOT_USE		0x40	/* not use Pn6 as digital I/O */
#define PMn7_NOT_USE		0x80	/* not use Pn7 as digital I/O */

#define PMn0_MODE_INPUT		0x01	/* Pn0 as input mode */
#define PMn0_MODE_OUTPUT	0x00	/* Pn0 as output mode */
#define PMn1_MODE_INPUT		0x02	/* Pn1 as input mode */
#define PMn1_MODE_OUTPUT	0x00	/* Pn1 as output mode */
#define PMn2_MODE_INPUT		0x04	/* Pn2 as input mode */
#define PMn2_MODE_OUTPUT	0x00	/* Pn2 as output mode */
#define PMn3_MODE_INPUT		0x08	/* Pn3 as input mode */
#define PMn3_MODE_OUTPUT	0x00	/* Pn3 as output mode */
#define PMn4_MODE_INPUT		0x10	/* Pn4 as input mode */
#define PMn4_MODE_OUTPUT	0x00	/* Pn4 as output mode */
#define PMn5_MODE_INPUT		0x20	/* Pn5 as input mode */
#define PMn5_MODE_OUTPUT	0x00	/* Pn5 as output mode */
#define PMn6_MODE_INPUT		0x40	/* Pn6 as input mode */
#define PMn6_MODE_OUTPUT	0x00	/* Pn6 as output mode */
#define PMn7_MODE_INPUT		0x80	/* Pn7 as input mode */
#define PMn7_MODE_OUTPUT	0x00	/* Pn7 as output mode */

#define PMn0_CHANGE_OUTPUT	0xFE	/* Change Pn0 to output mode */
#define PMn1_CHANGE_OUTPUT	0xFD	/* Change Pn1 to output mode */
#define PMn2_CHANGE_OUTPUT	0xFB	/* Change Pn2 to output mode */
#define PMn3_CHANGE_OUTPUT	0xF7	/* Change Pn3 to output mode */
#define PMn4_CHANGE_OUTPUT	0xEF	/* Change Pn4 to output mode */
#define PMn5_CHANGE_OUTPUT	0xDF	/* Change Pn5 to output mode */
#define PMn6_CHANGE_OUTPUT	0xBF	/* Change Pn6 to output mode */
#define PMn7_CHANGE_OUTPUT	0x7F	/* Change Pn7 to output mode */

/* ADPC define */
#define PORT_ADPC_0DIO		0
#define PORT_ADPC_1DIO		1
#define PORT_ADPC_2DIO		2
#define PORT_ADPC_3DIO		3
#define PORT_ADPC_4DIO		4
#define PORT_ADPC_5DIO		5
#define PORT_ADPC_6DIO		6
#define PORT_ADPC_7DIO		7
#define PORT_ADPC_8DIO		8
#define PORT_ADPC_9DIO		9
#define PORT_ADPC_10DIO		10
#define PORT_ADPC_11DIO		11
#define PORT_ADPC_12DIO		12
#define PORT_ADPC_13DIO		13
#define PORT_ADPC_14DIO		14
#define PORT_ADPC_15DIO		15
#define PORT_ADPC_16DIO		16
/* Port register(Pn.0 to 7) */
#define Pn0_OUTPUT_0		0x00	/* Pn0 output 0 */
#define Pn0_OUTPUT_1		0x01	/* Pn0 output 1 */
#define Pn1_OUTPUT_0		0x00	/* Pn1 output 0 */
#define Pn1_OUTPUT_1		0x02	/* Pn1 output 1 */
#define Pn2_OUTPUT_0		0x00	/* Pn2 output 0 */
#define Pn2_OUTPUT_1		0x04	/* Pn2 output 1 */
#define Pn3_OUTPUT_0		0x00	/* Pn3 output 0 */
#define Pn3_OUTPUT_1		0x08	/* Pn3 output 1 */
#define Pn4_OUTPUT_0		0x00	/* Pn4 output 0 */
#define Pn4_OUTPUT_1		0x10	/* Pn4 output 1 */
#define Pn5_OUTPUT_0		0x00	/* Pn5 output 0 */
#define Pn5_OUTPUT_1		0x20	/* Pn5 output 1 */
#define Pn6_OUTPUT_0		0x00	/* Pn6 output 0 */
#define Pn6_OUTPUT_1		0x40	/* Pn6 output 1 */
#define Pn7_OUTPUT_0		0x00	/* Pn7 output 0 */
#define Pn7_OUTPUT_1		0x80	/* Pn7 output 1 */

/* Pull-up resistor option register(PUn.0 to 7) */
#define PUn0_PULLUP_OFF		0x00	/* Pn0 Pull-up not connected */
#define PUn0_PULLUP_ON		0x01	/* Pn0 pull-up connected */
#define PUn1_PULLUP_OFF		0x00	/* Pn1 Pull-up not connected */
#define PUn1_PULLUP_ON		0x02	/* Pn1 pull-up connected */
#define PUn2_PULLUP_OFF		0x00	/* Pn2 Pull-up not connected */
#define PUn2_PULLUP_ON		0x04	/* Pn2 pull-up connected */
#define PUn3_PULLUP_OFF		0x00	/* Pn3 Pull-up not connected */
#define PUn3_PULLUP_ON		0x08	/* Pn3 pull-up connected */
#define PUn4_PULLUP_OFF		0x00	/* Pn4 Pull-up not connected */
#define PUn4_PULLUP_ON		0x10	/* Pn4 pull-up connected */
#define PUn5_PULLUP_OFF		0x00	/* Pn5 Pull-up not connected */
#define PUn5_PULLUP_ON		0x20	/* Pn5 pull-up connected */
#define PUn6_PULLUP_OFF		0x00	/* Pn6 Pull-up not connected */
#define PUn6_PULLUP_ON		0x40	/* Pn6 pull-up connected */
#define PUn7_PULLUP_OFF		0x00	/* Pn7 Pull-up not connected */
#define PUn7_PULLUP_ON		0x80	/* Pn7 pull-up connected */

/* Port input mode resistor option(PIMn.0 to 7) */
#define PIMn2_TTL_OFF		0x00	/* set Pn2 input normal buffer */
#define PIMn2_TTL_ON		0x04	/* set Pn2 input TTL buffer */
#define PIMn3_TTL_OFF		0x00	/* set Pn3 input normal buffer */
#define PIMn3_TTL_ON		0x08	/* set Pn3 input TTL buffer */
#define PIMn4_TTL_OFF		0x00	/* set Pn4 input normal buffer */
#define PIMn4_TTL_ON		0x10	/* set Pn4 input TTL buffer */

/* Port output mode resistor option(POMn.0 to 7) */
#define POMn2_Nch_OFF		0x00	/* set Pn2 output normal mode */
#define POMn2_Nch_ON		0x04	/* set Pn2 output N-ch open-drain mode */
#define POMn3_Nch_OFF		0x00	/* set Pn3 output normal mode */
#define POMn3_Nch_ON		0x08	/* set Pn3 output N-ch open-drain mode */
#define POMn4_Nch_OFF		0x00	/* set Pn4 output normal mode */
#define POMn4_Nch_ON		0x10	/* set Pn4 output N-ch open-drain mode */
#define POMn5_Nch_OFF		0x00	/* set Pn5 output normal mode */
#define POMn5_Nch_ON		0x20	/* set Pn5 output N-ch open-drain mode */

/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
#define	PM0_DEFAULT			0x80	/* PM0 default value */
#define	PM3_DEFAULT			0xFC	/* PM3 default value */
#define	PM11_DEFAULT		0xFC	/* PM11 default value */
#define	PM12_DEFAULT		0xFE	/* PM12 default value */
#define	PM13_DEFAULT		0xFC	/* PM13 default value */
#define	PM14_DEFAULT		0xC0	/* PM14 default value */

/*
*******************************************************************************
**  Function define
*******************************************************************************
*/
void PORT_Init (INT32U);
void PORT_ChangeP10Output( BOOL initialValue );
void PORT_ChangeP15Input( BOOL enablePU );
void PORT_ChangeP15Output( BOOL initialValue );
void  PORT_ChangeP20Input( );
void PORT_ChangeP20Output( BOOL initialValue );
void  PORT_ChangeP21Input( );
void PORT_ChangeP21Output( BOOL initialValue );
void  PORT_ChangeP22Input( );
void PORT_ChangeP22Output( BOOL initialValue );
void  PORT_ChangeP23Input( );
void PORT_ChangeP23Output( BOOL initialValue );
void  PORT_ChangeP24Input( );
void PORT_ChangeP24Output( BOOL initialValue );
void  PORT_ChangeP25Input( );
void PORT_ChangeP25Output( BOOL initialValue );
void PORT_ChangeP51Input( BOOL enablePU );
void PORT_ChangeP51Output( BOOL initialValue );
void PORT_ChangeP53Input( BOOL enablePU );
void PORT_ChangeP53Output( BOOL initialValue );
void PORT_ChangeP54Input( BOOL enablePU );
void PORT_ChangeP54Output( BOOL initialValue );
void PORT_ChangeP55Input( BOOL enablePU );
void PORT_ChangeP55Output( BOOL initialValue );
void PORT_ChangeP56Input( BOOL enablePU );
void PORT_ChangeP56Output( BOOL initialValue );
void PORT_ChangeP57Input( BOOL enablePU );
void PORT_ChangeP57Output( BOOL initialValue );
void PORT_ChangeP63Input( BOOL enablePU );
void PORT_ChangeP05Output( BOOL initialValue );
void PORT_ChangeP64Input( BOOL enablePU );
void PORT_ChangeP64Output( BOOL initialValue );
void PORT_ChangeP65Input( BOOL enablePU );
void PORT_ChangeP65Output( BOOL initialValue );
void PORT_ChangeP66Input( BOOL enablePU );
void PORT_ChangeP66Output( BOOL initialValue );
void PORT_ChangeP67Input( BOOL enablePU );
void PORT_ChangeP67Output( BOOL initialValue );
void PORT_ChangeP70Input( BOOL enablePU );
void PORT_ChangeP70Output( BOOL initialValue );
void PORT_ChangeP80Input( BOOL enablePU );
void PORT_ChangeP80Output( BOOL initialValue );
void PORT_ChangeP81Input( BOOL enablePU );
void PORT_ChangeP81Output( BOOL initialValue );
void PORT_ChangeP82Input( BOOL enablePU );
void PORT_ChangeP82Output( BOOL initialValue );
void PORT_ChangeP83Input( BOOL enablePU );
void PORT_ChangeP83Output( BOOL initialValue );
void PORT_ChangeP84Input( BOOL enablePU );
void PORT_ChangeP84Output( BOOL initialValue );
void PORT_ChangeP85Input( BOOL enablePU );
void PORT_ChangeP85Output( BOOL initialValue );
void PORT_ChangeP86Input( BOOL enablePU );
void PORT_ChangeP86Output( BOOL initialValue );
void  PORT_ChangeP110Input( );
void PORT_ChangeP110Output( BOOL initialValue );
void  PORT_ChangeP111Input( );
void PORT_ChangeP111Output( BOOL initialValue );
void PORT_ChangeP130Output( BOOL initialValue );
void PORT_ChangeP142Input( BOOL enablePU, BOOL enableTTL );
void PORT_ChangeP142Output( BOOL enableNch, BOOL initialValue );
void PORT_ChangeP145Input( BOOL enablePU );
void PORT_ChangeP145Output( BOOL initialValue );
void  PORT_ChangeP150Input( );
void PORT_ChangeP150Output( BOOL initialValue );
void  PORT_ChangeP151Input( );
void PORT_ChangeP151Output( BOOL initialValue );
void  PORT_ChangeP152Input( );
void PORT_ChangeP152Output( BOOL initialValue );
void  PORT_ChangeP153Input( );
void PORT_ChangeP153Output( BOOL initialValue );
void  PORT_ChangeP154Input( );
void PORT_ChangeP154Output( BOOL initialValue );
void  PORT_ChangeP155Input( );
void PORT_ChangeP155Output( BOOL initialValue );
void  PORT_ChangeP156Input( );
void PORT_ChangeP156Output( BOOL initialValue );
void PORT_ChangeP157Output( BOOL initialValue );
/* Start user code for definition. Do not edit comment generated here */
/* End user code for definition. Do not edit comment generated here */
#endif
