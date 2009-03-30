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
**  Filename :	TAU.h
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
#ifndef _MDTAU_
#define _MDTAU_
/*
*******************************************************************************
**	Register bit define
*******************************************************************************
*/
/*
	Peripheral Enable Register 0 (PER0)
*/
/* Control of timer array unit input clock(TAU0EN) */
#define TAU_CLOCK_CLR				0x01	/* for clear the bits */
#define TAU_CLOCK_STOP				0x00	/* stops supply of input clock */
#define TAU_CLOCK_SUPPLY			0x01	/* supplies input clock */

/*
	Timer Clock Select Register 0 (TPS0)
*/
/* Initial Value */
#define TAU_TPS0_INITIALVALUE		0x0000
/* Operating mode and clear mode selection(PRS003 & PRS002 & PRS001 & PRS000) */
#define TAU_CK00_FCLK_0				0x0000	/* ck00-fclk */
#define TAU_CK00_FCLK_1				0x0001	/* ck00-fclk/2^1 */
#define TAU_CK00_FCLK_2				0x0002	/* ck00-fclk/2^2 */
#define TAU_CK00_FCLK_3				0x0003	/* ck00-fclk/2^3 */
#define TAU_CK00_FCLK_4				0x0004	/* ck00-fclk/2^4 */
#define TAU_CK00_FCLK_5				0x0005	/* ck00-fclk/2^5 */
#define TAU_CK00_FCLK_6				0x0006	/* ck00-fclk/2^6 */
#define TAU_CK00_FCLK_7				0x0007	/* ck00-fclk/2^7 */
#define TAU_CK00_FCLK_8				0x0008	/* ck00-fclk/2^8 */
#define TAU_CK00_FCLK_9				0x0009	/* ck00-fclk/2^9 */
#define TAU_CK00_FCLK_10			0x000A	/* ck00-fclk/2^10 */
#define TAU_CK00_FCLK_11			0x000B	/* ck00-fclk/2^11 */
#define TAU_CK00_FCLK_12			0x000C	/* ck00-fclk/2^12 */
#define TAU_CK00_FCLK_13			0x000D	/* ck00-fclk/2^13 */
#define TAU_CK00_FCLK_14			0x000E	/* ck00-fclk/2^14 */
#define TAU_CK00_FCLK_15			0x000F	/* ck00-fclk/2^15 */
/* Operating mode and clear mode selection(PRS013 & PRS012 & PRS011 & PRS010) */
#define TAU_CK01_FCLK_0				0x0000	/* ck01-fclk */
#define TAU_CK01_FCLK_1				0x0010	/* ck01-fclk/2^1 */
#define TAU_CK01_FCLK_2				0x0020	/* ck01-fclk/2^2 */
#define TAU_CK01_FCLK_3				0x0030	/* ck01-fclk/2^3 */
#define TAU_CK01_FCLK_4				0x0040	/* ck01-fclk/2^4 */
#define TAU_CK01_FCLK_5				0x0050	/* ck01-fclk/2^5 */
#define TAU_CK01_FCLK_6				0x0060	/* ck01-fclk/2^6 */
#define TAU_CK01_FCLK_7				0x0070	/* ck01-fclk/2^7 */
#define TAU_CK01_FCLK_8				0x0080	/* ck01-fclk/2^8 */
#define TAU_CK01_FCLK_9				0x0090	/* ck01-fclk/2^9 */
#define TAU_CK01_FCLK_10			0x00A0	/* ck01-fclk/2^10 */
#define TAU_CK01_FCLK_11			0x00B0	/* ck01-fclk/2^11 */
#define TAU_CK01_FCLK_12			0x00C0	/* ck01-fclk/2^12 */
#define TAU_CK01_FCLK_13			0x00D0	/* ck01-fclk/2^13 */
#define TAU_CK01_FCLK_14			0x00E0	/* ck01-fclk/2^14 */
#define TAU_CK01_FCLK_15			0x00F0	/* ck01-fclk/2^15 */

/*
	Timer Mode Register 0n (TMR0n)
*/
/* Initial Value */
#define TAU_TMR0_INITIALVALUE		0x0000
/* Selection of macro clock (MCK) of channel n(CKS0n) */
#define TAU_CLOCK_SELECT_CLR		0x8000	/* for clear the bits */
#define TAU_CLOCK_SELECT_CK00		0x0000	/* operation clock CK0 set by PRS register */ 
#define TAU_CLOCK_SELECT_CK01		0x8000	/* operation clock CK1 set by PRS register */
/* Selection of count clock (CCK) of channel n(CCS0n) */
#define TAU_CLOCK_MODE_CLR			0x1000	/* for clear the bits */
#define TAU_CLOCK_MODE_CKS			0x0000	/* macro clock MCK specified by CKS0n bit */  
#define TAU_CLOCK_MODE_TI0N			0x1000	/* valid edge of input signal input from TI0n pin */
/* Selection of slave/master of channel n(MASTER0n) */
#define TAU_COMBINATION_CLR			0x0800	/* for clear the bits */
#define TAU_COMBINATION_SLAVE		0x0000	/* operates as slave channel with combination operation function */  
#define TAU_COMBINATION_MASTER		0x0800	/* operates as master channel with combination operation function */
/* Setting of start trigger or capture trigger of channel n(STS0n2 & STS0n1 & STS0n0) */
#define TAU_TRIGGER_CLR				0x0700	/* for clear the bits */
#define TAU_TRIGGER_SOFTWARE		0x0000	/* only software trigger start is valid */
#define TAU_TRIGGER_TI0N_VALID		0x0100	/* valid edge of TI0n pin input is used as both the start trigger and capture trigger */
#define TAU_TRIGGER_TI0N_BOTH		0x0200	/* both the edges of TI0n pin input are used as a start trigger and a capture trigger */
#define TAU_TRIGGER_MASTER_INT		0x0400	/* interrupt signal of the master channel is used */
/* Selection of TI0n pin input valid edge(CIS0n1 & CIS0n0) */
#define TAU_TI0N_EDGE_CLR			0x00C0	/* for clear the bits */
#define TAU_TI0N_EDGE_FALLING		0x0000	/* falling edge */
#define TAU_TI0N_EDGE_RISING		0x0040	/* rising edge */
#define TAU_TI0N_EDGE_BOTH_LOW		0x0080	/* both edges (when low-level width is measured) */
#define TAU_TI0N_EDGE_BOTH_HIGH		0x00C0	/* both edges (when high-level width is measured) */
/* Operation mode of channel n(MD0n3 & MD0n2 & MD0n1 & MD0n0) */
#define TAU_MODE_CLR				0x000F	/* for clear the bits */
#define TAU_MODE_INTERVAL_TIMER		0x0000	/* interval timer mode */
#define TAU_MODE_EVENT_COUNT 		0x0006	/* event counter mode */
#define TAU_MODE_CAPTURE			0x0004	/* capture mode */
#define TAU_MODE_HIGHLOW_MEASURE	0x000C	/* high-/low-level width measurement mode */
#define TAU_MODE_PWM_MASTER			0x0001	/* PWM Function (Master Channel) mode */
#define TAU_MODE_PWM_SLAVE			0x0009	/* PWM Function (Slave Channel) mode */
#define TAU_MODE_ONESHOT			0x0008	/* one-shot pulse output mode */
/* Setting of starting counting and interrupt(MD0n0) */
#define TAU_START_INT_CLR			0x0001	/* for clear the bits */
#define TAU_START_INT_UNUSED		0x0000	/* timer interrupt is not generated when counting is started (timer output does not change, either) */  
#define TAU_START_INT_USED			0x0001	/* timer interrupt is generated when counting is started (timer output also changes) */

/*
	Timer Status Register 0n (TSR0n)
*/
/* Initial Value */
#define TAU_TSR0_INITIALVALUE		0x0000
/* Counter overflow status of channel n(OVF) */
#define TAU_OVERFLOW_STATUS			0x0001
#define TAU_OVERFLOW_NOT_OCCURS		0x0000	/* overflow does not occur */
#define TAU_OVERFLOW_OCCURS			0x0001	/* overflow occurs */

/*
	Timer Channel Enable Status Register 0 (TE0)
*/
/* Initial Value */
#define TAU_TE0_INITIALVALUE		0x0000
/* Indication of operation enable/stop status of channel 0(TE00) */
#define TAU_CH0_OPERATION_STOP		0x0000	/* operation is stopped */
#define TAU_CH0_OPERATION_ENABLE	0x0001	/* operation is enabled */
/* Indication of operation enable/stop status of channel 1(TE01) */
#define TAU_CH1_OPERATION_STOP		0x0000	/* operation is stopped */
#define TAU_CH1_OPERATION_ENABLE	0x0002	/* operation is enabled */
/* Indication of operation enable/stop status of channel 2(TE02) */
#define TAU_CH2_OPERATION_STOP		0x0000	/* operation is stopped */
#define TAU_CH2_OPERATION_ENABLE	0x0004	/* operation is enabled */
/* Indication of operation enable/stop status of channel 3(TE03) */
#define TAU_CH3_OPERATION_STOP		0x0000	/* operation is stopped */
#define TAU_CH3_OPERATION_ENABLE	0x0008	/* operation is enabled */
/* Indication of operation enable/stop status of channel 4(TE04) */
#define TAU_CH4_OPERATION_STOP		0x0000	/* operation is stopped */
#define TAU_CH4_OPERATION_ENABLE	0x0010	/* operation is enabled */
/* Indication of operation enable/stop status of channel 5(TE05) */
#define TAU_CH5_OPERATION_STOP		0x0000	/* operation is stopped */
#define TAU_CH5_OPERATION_ENABLE	0x0020	/* operation is enabled */
/* Indication of operation enable/stop status of channel 6(TE06) */
#define TAU_CH6_OPERATION_STOP		0x0000	/* operation is stopped */
#define TAU_CH6_OPERATION_ENABLE	0x0040	/* operation is enabled */
/* Indication of operation enable/stop status of channel 7(TE07) */
#define TAU_CH7_OPERATION_STOP		0x0000	/* operation is stopped */
#define TAU_CH7_OPERATION_ENABLE	0x0080	/* operation is enabled */

/*
	Timer Channel Start Register 0 (TS0)
*/
/* Initial Value */
#define TAU_TS0_INITIALVALUE		0x0000
/* Operation enable (start) trigger of channel 0(TS00) */
#define TAU_CH0_START_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH0_START_TRG_ON		0x0001	/* operation is enabled (start software trigger is generated) */
/* Operation enable (start) trigger of channel 1(TS01) */
#define TAU_CH1_START_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH1_START_TRG_ON		0x0002	/* operation is enabled (start software trigger is generated) */
/* Operation enable (start) trigger of channel 2(TS02) */
#define TAU_CH2_START_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH2_START_TRG_ON		0x0004	/* operation is enabled (start software trigger is generated) */
/* Operation enable (start) trigger of channel 3(TS03) */
#define TAU_CH3_START_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH3_START_TRG_ON		0x0008	/* operation is enabled (start software trigger is generated) */
/* Operation enable (start) trigger of channel 4(TS04) */
#define TAU_CH4_START_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH4_START_TRG_ON		0x0010	/* operation is enabled (start software trigger is generated) */
/* Operation enable (start) trigger of channel 5(TS05) */
#define TAU_CH5_START_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH5_START_TRG_ON		0x0020	/* operation is enabled (start software trigger is generated) */
/* Operation enable (start) trigger of channel 6(TS06) */
#define TAU_CH6_START_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH6_START_TRG_ON		0x0040	/* operation is enabled (start software trigger is generated) */
/* Operation enable (start) trigger of channel 7(TS07) */
#define TAU_CH7_START_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH7_START_TRG_ON		0x0080	/* operation is enabled (start software trigger is generated) */

/*
	Timer Channel Stop Register 0 (TT0)
*/
/* Initial Value */
#define TAU_TT0_INITIALVALUE		0x0000
/* Operation stop trigger of channel 0(TT00) */
#define TAU_CH0_STOP_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH0_STOP_TRG_ON			0x0001	/* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 1(TT01) */
#define TAU_CH1_STOP_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH1_STOP_TRG_ON			0x0002	/* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 2(TT02) */
#define TAU_CH2_STOP_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH2_STOP_TRG_ON			0x0004	/* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 3(TT03) */
#define TAU_CH3_STOP_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH3_STOP_TRG_ON			0x0008	/* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 4(TT04) */
#define TAU_CH4_STOP_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH4_STOP_TRG_ON			0x0010	/* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 5(TT05) */
#define TAU_CH5_STOP_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH5_STOP_TRG_ON			0x0020	/* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 6(TT06) */
#define TAU_CH6_STOP_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH6_STOP_TRG_ON			0x0040	/* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 7(TT07) */
#define TAU_CH7_STOP_TRG_OFF		0x0000	/* no trigger operation */
#define TAU_CH7_STOP_TRG_ON			0x0080	/* operation is stopped (stop trigger is generated) */

/*
	Timer Input Select Register 0 (TIS0)
*/
/* Initial Value */
#define TAU_TIS0_INITIALVALUE		0x00
/* Selection of timer input/subsystem clock used with channel 0(TIS00) */
#define TAU_CH0_INPUT_TI0N			0x00	/* input signal of timer input pin (TI00) */
#define TAU_CH0_INPUT_FXT			0x01	/* subsystem clock divided by 4 (fXT/4) */
/* Selection of timer input/subsystem clock used with channel 1(TIS01) */
#define TAU_CH1_INPUT_TI0N			0x00	/* input signal of timer input pin (TI01) */
#define TAU_CH1_INPUT_FXT			0x02	/* subsystem clock divided by 4 (fXT/4) */
/* Selection of timer input/subsystem clock used with channel 2(TIS02) */
#define TAU_CH2_INPUT_TI0N			0x00	/* input signal of timer input pin (TI02) */
#define TAU_CH2_INPUT_FXT			0x04	/* subsystem clock divided by 4 (fXT/4) */
/* Selection of timer input/subsystem clock used with channel 3(TIS03) */
#define TAU_CH3_INPUT_TI0N			0x00	/* input signal of timer input pin (TI03) */
#define TAU_CH3_INPUT_FXT			0x08	/* subsystem clock divided by 4 (fXT/4) */
/* Selection of timer input/subsystem clock used with channel 4(TIS04) */
#define TAU_CH4_INPUT_TI0N			0x00	/* input signal of timer input pin (TI04) */
#define TAU_CH4_INPUT_FXT			0x10	/* subsystem clock divided by 4 (fXT/4) */
/* Selection of timer input/subsystem clock used with channel 5(TIS05) */
#define TAU_CH5_INPUT_TI0N			0x00	/* input signal of timer input pin (TI05) */
#define TAU_CH5_INPUT_FXT			0x20	/* subsystem clock divided by 4 (fXT/4) */
/* Selection of timer input/subsystem clock used with channel 6(TIS06) */
#define TAU_CH6_INPUT_TI0N			0x00	/* input signal of timer input pin (TI06) */
#define TAU_CH6_INPUT_FXT			0x40	/* subsystem clock divided by 4 (fXT/4) */
/* Selection of timer input/subsystem clock used with channel 7(TIS07) */
#define TAU_CH7_INPUT_TI0N			0x00	/* input signal of timer input pin (TI07) */
#define TAU_CH7_INPUT_FXT			0x80	/* subsystem clock divided by 4 (fXT/4) */

/*
	Timer Output Enable Register 0 (TOE0)
*/
/* Initial Value */
#define TAU_TOE0_INITIALVALUE		0x0000
/* Timer output enable/disable of channel 0(TOE00) */
#define TAU_CH0_OUTPUT_ENABLE		0x0001	/* the TO00 operation enabled by count operation (timer channel output bit) */
#define TAU_CH0_OUTPUT_DISABLE		0x0000	/* the TO00 operation stopped by count operation (timer channel output bit) */
/* Timer output enable/disable of channel 1(TOE01) */
#define TAU_CH1_OUTPUT_ENABLE		0x0002	/* the TO01 operation enabled by count operation (timer channel output bit) */
#define TAU_CH1_OUTPUT_DISABLE		0x0000	/* the TO01 operation stopped by count operation (timer channel output bit) */
/* Timer output enable/disable of channel 2(TOE02) */
#define TAU_CH2_OUTPUT_ENABLE		0x0004	/* the TO02 operation enabled by count operation (timer channel output bit) */
#define TAU_CH2_OUTPUT_DISABLE		0x0000	/* the TO02 operation stopped by count operation (timer channel output bit) */
/* Timer output enable/disable of channel 3(TOE03) */
#define TAU_CH3_OUTPUT_ENABLE		0x0008	/* the TO03 operation enabled by count operation (timer channel output bit) */
#define TAU_CH3_OUTPUT_DISABLE		0x0000	/* the TO03 operation stopped by count operation (timer channel output bit) */
/* Timer output enable/disable of channel 4(TOE04) */
#define TAU_CH4_OUTPUT_ENABLE		0x0010	/* the TO04 operation enabled by count operation (timer channel output bit) */
#define TAU_CH4_OUTPUT_DISABLE		0x0000	/* the TO04 operation stopped by count operation (timer channel output bit) */
/* Timer output enable/disable of channel 5(TOE05) */
#define TAU_CH5_OUTPUT_ENABLE		0x0020	/* the TO05 operation enabled by count operation (timer channel output bit) */
#define TAU_CH5_OUTPUT_DISABLE		0x0000	/* the TO05 operation stopped by count operation (timer channel output bit) */
/* Timer output enable/disable of channel 6(TOE06) */
#define TAU_CH6_OUTPUT_ENABLE		0x0040	/* the TO06 operation enabled by count operation (timer channel output bit) */
#define TAU_CH6_OUTPUT_DISABLE		0x0000	/* the TO06 operation stopped by count operation (timer channel output bit) */
/* Timer output enable/disable of channel 7(TOE07) */
#define TAU_CH7_OUTPUT_ENABLE		0x0080	/* the TO07 operation enabled by count operation (timer channel output bit) */
#define TAU_CH7_OUTPUT_DISABLE		0x0000	/* the TO07 operation stopped by count operation (timer channel output bit) */

/*
	Timer Output Register 0 (TO0)
*/
/* Initial Value */
#define TAU_TO0_INITIALVALUE		0x0000
/* Timer output of channel 0(TO00) */
#define TAU_CH0_OUTPUT_VALUE_0		0x0000	/* timer output value is 0 */
#define TAU_CH0_OUTPUT_VALUE_1		0x0001	/* timer output value is 1 */
/* Timer output of channel 1(TO01) */
#define TAU_CH1_OUTPUT_VALUE_0		0x0000	/* timer output value is 0 */
#define TAU_CH1_OUTPUT_VALUE_1		0x0002	/* timer output value is 1 */
/* Timer output of channel 2(TO02) */
#define TAU_CH2_OUTPUT_VALUE_0		0x0000	/* timer output value is 0 */
#define TAU_CH2_OUTPUT_VALUE_1		0x0004	/* timer output value is 1 */
/* Timer output of channel 3(TO03) */
#define TAU_CH3_OUTPUT_VALUE_0		0x0000	/* timer output value is 0 */
#define TAU_CH3_OUTPUT_VALUE_1		0x0008	/* timer output value is 1 */
/* Timer output of channel 4(TO04) */
#define TAU_CH4_OUTPUT_VALUE_0		0x0000	/* timer output value is 0 */
#define TAU_CH4_OUTPUT_VALUE_1		0x0010	/* timer output value is 1 */
/* Timer output of channel 5(TO05) */
#define TAU_CH5_OUTPUT_VALUE_0		0x0000	/* timer output value is 0 */
#define TAU_CH5_OUTPUT_VALUE_1		0x0020	/* timer output value is 1 */
/* Timer output of channel 6(TO06) */
#define TAU_CH6_OUTPUT_VALUE_0		0x0000	/* timer output value is 0 */
#define TAU_CH6_OUTPUT_VALUE_1		0x0040	/* timer output value is 1 */
/* Timer output of channel 7(TO07) */
#define TAU_CH7_OUTPUT_VALUE_0		0x0000	/* timer output value is 0 */
#define TAU_CH7_OUTPUT_VALUE_1		0x0080	/* timer output value is 1 */

/*
	Timer Output Level Register 0 (TOL0)
*/
/* Initial Value */
#define TAU_TOL0_INITIALVALUE		0x0000
/* Control of timer output level of channel 0(TOL00) */
#define TAU_CH0_OUTPUT_LEVEL_H		0x0000	/* positive logic output (active-high) */
#define TAU_CH0_OUTPUT_LEVEL_L		0x0001	/* inverted output (active-low) */
/* Control of timer output level of channel 1(TOL01) */
#define TAU_CH1_OUTPUT_LEVEL_H		0x0000	/* positive logic output (active-high) */
#define TAU_CH1_OUTPUT_LEVEL_L		0x0002	/* inverted output (active-low) */
/* Control of timer output level of channel 2(TOL02) */
#define TAU_CH2_OUTPUT_LEVEL_H		0x0000	/* positive logic output (active-high) */
#define TAU_CH2_OUTPUT_LEVEL_L		0x0004	/* inverted output (active-low) */
/* Control of timer output level of channel 3(TOL03) */
#define TAU_CH3_OUTPUT_LEVEL_H		0x0000	/* positive logic output (active-high) */
#define TAU_CH3_OUTPUT_LEVEL_L		0x0008	/* inverted output (active-low) */
/* Control of timer output level of channel 4(TOL04) */
#define TAU_CH4_OUTPUT_LEVEL_H		0x0000	/* positive logic output (active-high) */
#define TAU_CH4_OUTPUT_LEVEL_L		0x0010	/* inverted output (active-low) */
/* Control of timer output level of channel 5(TOL05) */
#define TAU_CH5_OUTPUT_LEVEL_H		0x0000	/* positive logic output (active-high) */
#define TAU_CH5_OUTPUT_LEVEL_L		0x0020	/* inverted output (active-low) */
/* Control of timer output level of channel 6(TOL06) */
#define TAU_CH6_OUTPUT_LEVEL_H		0x0000	/* positive logic output (active-high) */
#define TAU_CH6_OUTPUT_LEVEL_L		0x0040	/* inverted output (active-low) */
/* Control of timer output level of channel 7(TOL07) */
#define TAU_CH7_OUTPUT_LEVEL_H		0x0000	/* positive logic output (active-high) */
#define TAU_CH7_OUTPUT_LEVEL_L		0x0080	/* inverted output (active-low) */

/*
	Timer Output Mode Register 0 (TOM0)
*/
/* Initial Value */
#define TAU_TOM0_INITIALVALUE		0x0000
/* Control of timer output mode of channel 0(TOM00) */
#define TAU_CH0_OUTPUT_TOGGLE		0x0000	/* toggle operation mode */
#define TAU_CH0_OUTPUT_COMBIN		0x0001	/* combination operation mode */
/* Control of timer output mode of channel 1(TOM01) */
#define TAU_CH1_OUTPUT_TOGGLE		0x0000	/* toggle operation mode */
#define TAU_CH1_OUTPUT_COMBIN		0x0002	/* combination operation mode */
/* Control of timer output mode of channel 2(TOM02) */
#define TAU_CH2_OUTPUT_TOGGLE		0x0000	/* toggle operation mode */
#define TAU_CH2_OUTPUT_COMBIN		0x0004	/* combination operation mode */
/* Control of timer output mode of channel 3(TOM03) */
#define TAU_CH3_OUTPUT_TOGGLE		0x0000	/* toggle operation mode */
#define TAU_CH3_OUTPUT_COMBIN		0x0008	/* combination operation mode */
/* Control of timer output mode of channel 4(TOM04) */
#define TAU_CH4_OUTPUT_TOGGLE		0x0000	/* toggle operation mode */
#define TAU_CH4_OUTPUT_COMBIN		0x0010	/* combination operation mode */
/* Control of timer output mode of channel 5(TOM05) */
#define TAU_CH5_OUTPUT_TOGGLE		0x0000	/* toggle operation mode */
#define TAU_CH5_OUTPUT_COMBIN		0x0020	/* combination operation mode */
/* Control of timer output mode of channel 6(TOM06) */
#define TAU_CH6_OUTPUT_TOGGLE		0x0000	/* toggle operation mode */
#define TAU_CH6_OUTPUT_COMBIN		0x0040	/* combination operation mode */
/* Control of timer output mode of channel 7(TOM07) */
#define TAU_CH7_OUTPUT_TOGGLE		0x0000	/* toggle operation mode */
#define TAU_CH7_OUTPUT_COMBIN		0x0080	/* combination operation mode */

/*
	Input Switch Control Register (ISC)
*/
/* Initial Value */
#define TAU_ISC_INITIALVALUE		0x00
/* Switching channel 7 input of timer array unit(ISC1) */
#define TAU_CH7_STATUS				0x02	/* for clear the bits */
#define TAU_CH7_NO_INPUT			0x00	/* timer input is not used */
#define TAU_CH7_RXD3_INPUT			0x02	/* input signal of RxD3 pin is used as timer input*/

/*
	Noise Filter Enable Register 1 (NFEN1)
*/
/* Initial Value */
#define TAU_NFEN1_INITIALVALUE		0x00
/* Enable/disable using noise filter of TI07/TO07/P145 pin input signal(TNFEN07) */
#define TAU_CH7_NOISE_CLR			0x80	/* for clear the bits */
#define TAU_CH7_NOISE_OFF			0x00	/* noise filter OFF */
#define TAU_CH7_NOISE_ON			0x80	/* noise filter ON */
/* Enable/disable using noise filter of TI06/TO06/P06 pin input signal(TNFEN06) */
#define TAU_CH6_NOISE_CLR			0x40	/* for clear the bits */
#define TAU_CH6_NOISE_OFF			0x00	/* noise filter OFF */
#define TAU_CH6_NOISE_ON			0x40	/* noise filter ON */
/* Enable/disable using noise filter of TI05/TO05/P05 pin input signal(TNFEN05) */
#define TAU_CH5_NOISE_CLR			0x20	/* for clear the bits */
#define TAU_CH5_NOISE_OFF			0x00	/* noise filter OFF */
#define TAU_CH5_NOISE_ON			0x20	/* noise filter ON */
/* Enable/disable using noise filter of TI04/TO04/P42 pin input signal(TNFEN04) */
#define TAU_CH4_NOISE_CLR			0x10	/* for clear the bits */
#define TAU_CH4_NOISE_OFF			0x00	/* noise filter OFF */
#define TAU_CH4_NOISE_ON			0x10	/* noise filter ON */
/* Enable/disable using noise filter of TI03/TO03/INTP4/P31 pin input signal(TNFEN03) */
#define TAU_CH3_NOISE_CLR			0x08	/* for clear the bits */
#define TAU_CH3_NOISE_OFF			0x00	/* noise filter OFF */
#define TAU_CH3_NOISE_ON			0x08	/* noise filter ON */
/* Enable/disable using noise filter of TI02/TO02/P17 pin input signal(TNFEN02) */
#define TAU_CH2_NOISE_CLR			0x04	/* for clear the bits */
#define TAU_CH2_NOISE_OFF			0x00	/* noise filter OFF */
#define TAU_CH2_NOISE_ON			0x04	/* noise filter ON */
/* Enable/disable using noise filter of TI01/TO01/INTP5/P16 pin input signal(TNFEN01) */
#define TAU_CH1_NOISE_CLR			0x02	/* for clear the bits */
#define TAU_CH1_NOISE_OFF			0x00	/* noise filter OFF */
#define TAU_CH1_NOISE_ON			0x02	/* noise filter ON */
/* Enable/disable using noise filter of TI00/P00 pin input signal(TNFEN00) */
#define TAU_CH0_NOISE_CLR			0x01	/* for clear the bits */
#define TAU_CH0_NOISE_OFF			0x00	/* noise filter OFF */
#define TAU_CH0_NOISE_ON			0x01	/* noise filter ON */

/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
/* 16-bit timer data register 00 (TDR00) */
#define TAU_TDR00_VALUE	0x20
/* 16-bit timer data register 02 (TDR02) */
#define TAU_TDR02_VALUE	0x91
//0x20e/2
/* 16-bit timer data register 05 (TDR05) */
#define TAU_TDR05_VALUE	   0x545  //0x0865

/* 16-bit timer data register 06 (TDR06) */
#define TAU_TDR06_VALUE	0x7

/* 16-bit timer data register 07 (TDR07) */
#define TAU_TDR07_VALUE	0x2B32  
//0x4e1f

/* Clock divisor for channel2 */
#define TAU_CHANNEL2_DIVISOR	1
/* Clock divisor for channel7 */
#define TAU_CHANNEL7_DIVISOR	1

/*
*******************************************************************************
**  Function define
*******************************************************************************
*/
void TAU_Init( void );

void Set_Fast_Timer(INT8U  Mode);
void TAU_Channel0_Start( void );
void TAU_Channel0_Stop( void );

void TAU_Channel1_Start( void );
void TAU_Channel1_Stop( void );

void TAU_Channel2_Start( void );
void TAU_Channel2_Stop( void );

void TAU_Channel5_Start( void );
void TAU_Channel5_Stop( void );

void TAU_Channel6_Start( void );
void TAU_Channel6_Stop( void );

void TAU_Channel7_Start( void );
void TAU_Channel7_Stop( void );

/* Start user code for definition. Do not edit comment generated here */
/* End user code for definition. Do not edit comment generated here */
#endif
