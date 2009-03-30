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
**  Filename :	Int.h
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
#ifndef _MDINT_
#define _MDINT_
/*
*******************************************************************************
**	Register bit define
*******************************************************************************
*/
/*
 External Interrupt Rising Edge Enable Register (EGP)
*/
/* INTPn pin valid edge selection(EGP.EGPn) */
#define INTP_EGP_RESET_VALUE		0x00	/* external interrupt rising edge enable register value after reset */
#define INTP0_EDGE_RISING_SEL		0x01	/* rising edge selected for INTP0 pin */
#define INTP0_EDGE_RISING_UNSEL		0x00	/* rising edge not selected for INTP0 pin */
#define INTP1_EDGE_RISING_SEL		0x02	/* rising edge selected for INTP1 pin */
#define INTP1_EDGE_RISING_UNSEL		0x00	/* rising edge not selected for INTP1 pin*/
#define INTP2_EDGE_RISING_SEL		0x04	/* rising edge selected for INTP2 pin */
#define INTP2_EDGE_RISING_UNSEL		0x00	/* rising edge not selected for INTP2 pin */
#define INTP3_EDGE_RISING_SEL		0x08	/* rising edge selected for INTP3 pin */
#define INTP3_EDGE_RISING_UNSEL		0x00	/* rising edge not selected for INTP3 pin */
#define INTP4_EDGE_RISING_SEL		0x10	/* rising edge selected for INTP4 pin */
#define INTP4_EDGE_RISING_UNSEL		0x00	/* rising edge not selected for INTP4 pin */
#define INTP5_EDGE_RISING_SEL		0x20	/* rising edge selected for INTP5 pin */
#define INTP5_EDGE_RISING_UNSEL		0x00	/* rising edge not selected for INTP5 pin */
#define INTP6_EDGE_RISING_SEL		0x40	/* rising edge selected for INTP6 pin */
#define INTP6_EDGE_RISING_UNSEL		0x00	/* rising edge not selected for INTP6 pin */
#define INTP7_EDGE_RISING_SEL		0x80	/* rising edge selected for INTP7 pin */
#define INTP7_EDGE_RISING_UNSEL		0x00	/* rising edge not selected for INTP7 pin */
#define INTP8_EDGE_RISING_SEL		0x01	/* rising edge selected for INTP8 pin */
#define INTP8_EDGE_RISING_UNSEL		0x00	/* rising edge not selected for INTP8 pin */
#define INTP9_EDGE_RISING_SEL		0x02	/* rising edge selected for INTP9 pin */
#define INTP9_EDGE_RISING_UNSEL		0x00	/* rising edge not selected for INTP9 pin */
#define INTP10_EDGE_RISING_SEL		0x04	/* rising edge selected for INTP10 pin */
#define INTP10_EDGE_RISING_UNSEL	0x00	/* rising edge not selected for INTP10 pin */
#define INTP11_EDGE_RISING_SEL		0x08	/* rising edge selected for INTP11 pin */
#define INTP11_EDGE_RISING_UNSEL	0x00	/* rising edge not selected for INTP11 pin */

/*
 External Interrupt Falling Edge Enable Register (EGN)
*/
/* INTPn pin valid edge selection(EGN.EGNn) */
#define INTP_EGN_RESET_VALUE		0x00	/* external interrupt falling edge enable register value after reset */
#define INTP0_EDGE_FALLING_SEL		0x01	/* falling edge selected for INTP0 pin */
#define INTP0_EDGE_FALLING_UNSEL	0x00	/* falling edge not selected for INTP0 pin */
#define INTP1_EDGE_FALLING_SEL		0x02	/* falling edge selected for INTP1 pin */
#define INTP1_EDGE_FALLING_UNSEL	0x00	/* falling edge not selected for INTP1 pin */
#define INTP2_EDGE_FALLING_SEL		0x04	/* falling edge selected for INTP2 pin */
#define INTP2_EDGE_FALLING_UNSEL	0x00	/* falling edge not selected for INTP2 pin */
#define INTP3_EDGE_FALLING_SEL		0x08	/* falling edge selected for INTP3 pin */
#define INTP3_EDGE_FALLING_UNSEL	0x00	/* falling edge not selected for INTP3 pin */
#define INTP4_EDGE_FALLING_SEL		0x10	/* falling edge selected for INTP4 pin */
#define INTP4_EDGE_FALLING_UNSEL	0x00	/* falling edge not selected for INTP4 pin */
#define INTP5_EDGE_FALLING_SEL		0x20	/* falling edge selected for INTP5 pin */
#define INTP5_EDGE_FALLING_UNSEL	0x00	/* falling edge not selected for INTP5 pin */
#define INTP6_EDGE_FALLING_SEL		0x40	/* falling edge selected for INTP6 pin */
#define INTP6_EDGE_FALLING_UNSEL	0x00	/* falling edge not selected for INTP6 pin */
#define INTP7_EDGE_FALLING_SEL		0x80	/* falling edge selected for INTP7 pin */
#define INTP7_EDGE_FALLING_UNSEL	0x00	/* falling edge not selected for INTP7 pin */
#define INTP8_EDGE_FALLING_SEL		0x01	/* falling edge selected for INTP8 pin */
#define INTP8_EDGE_FALLING_UNSEL	0x00	/* falling edge not selected for INTP8 pin */
#define INTP9_EDGE_FALLING_SEL		0x02	/* falling edge selected for INTP9 pin */
#define INTP9_EDGE_FALLING_UNSEL	0x00	/* falling edge not selected for INTP9 pin */
#define INTP10_EDGE_FALLING_SEL		0x04	/* falling edge selected for INTP10 pin */
#define INTP10_EDGE_FALLING_UNSEL	0x00	/* falling edge not selected for INTP10 pin */
#define INTP11_EDGE_FALLING_SEL		0x08	/* falling edge selected for INTP11 pin */
#define INTP11_EDGE_FALLING_UNSEL	0x00	/* falling edge not selected for INTP11 pin */

/* Key interrupt mode control(KRM.KRMn)*/
#define KR_KRM_RESET_VALUE			0x00	/* KRM value after reset */
#define KR0_SIGNAL_DETECT_ON		0x01	/* Detects KR0 signal */
#define KR0_SIGNAL_DETECT_OFF		0x00	/* Does not detect KR0 signal */
#define KR1_SIGNAL_DETECT_ON		0x02	/* Detects KR1 signal */        
#define KR1_SIGNAL_DETECT_OFF		0x00	/* Does not detect KR1 signal */
#define KR2_SIGNAL_DETECT_ON		0x04	/* Detects KR2 signal */        
#define KR2_SIGNAL_DETECT_OFF		0x00	/* Does not detect KR2 signal */
#define KR3_SIGNAL_DETECT_ON		0x08	/* Detects KR3 signal */        
#define KR3_SIGNAL_DETECT_OFF		0x00	/* Does not detect KR3 signal */
#define KR4_SIGNAL_DETECT_ON		0x10	/* Detects KR4 signal */        
#define KR4_SIGNAL_DETECT_OFF		0x00	/* Does not detect KR4 signal */
#define KR5_SIGNAL_DETECT_ON		0x20	/* Detects KR5 signal */        
#define KR5_SIGNAL_DETECT_OFF		0x00	/* Does not detect KR5 signal */
#define KR6_SIGNAL_DETECT_ON		0x40	/* Detects KR6 signal */        
#define KR6_SIGNAL_DETECT_OFF		0x00	/* Does not detect KR6 signal */
#define KR7_SIGNAL_DETECT_ON		0x80	/* Detects KR7 signal */        
#define KR7_SIGNAL_DETECT_OFF		0x00	/* Does not detect KR7 signal */

/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
enum MaskableSource {
	INT_WDTI, INT_LVI, INT_INTP0, INT_INTP1, 
	INT_INTP2, INT_INTP3, INT_INTP4, INT_INTP5,
	INT_ST3, INT_SR3, INT_SRE3, INT_DMA0,
	INT_DMA1, INT_ST0_CSI00, INT_SR0_CSI01, INT_SRE0,
	INT_CSI10_IIC1_ST1, INT_SR1, INT_SRE1, INT_IIC0,
	INT_TM00, INT_TM01, INT_TM02, INT_TM03,
	INT_AD, INT_RTC, INT_RTCI, INT_KR,
	INT_ST2_CSI20_IIC2, INT_SR2, INT_SRE2, INT_TM04,
	INT_TM05, INT_TM06, INT_TM07, INT_INTP6,
	INT_INTP7, INT_INTP8, INT_INTP9, INT_INTP10,
	INT_INTP11
	};

/*
*******************************************************************************
**  Function define
*******************************************************************************
*/
void INTP_Init( void );
void KEY_Init( void );
void INT_MaskableInterruptEnable( enum MaskableSource name, BOOL enableflag );
/* Start user code for definition. Do not edit comment generated here */
/* End user code for definition. Do not edit comment generated here */
#endif
