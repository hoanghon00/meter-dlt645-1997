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
**  Filename :	Serial.h
**  Abstract :	This file implements device driver for Serial module.
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
#ifndef _MDSERIAL_
#define _MDSERIAL_



#ifndef SEIRAL_C
#define SEIRAL_EXT  extern
#else
#define SEIRAL_EXT 
#endif


/*
*******************************************************************************
**	Register bit define
*******************************************************************************
*/
/*
	Peripheral Enable Register 0 (PER0)
*/
/* Control of serial array unit 0 input clock(SAU0EN) */
#define SAU0_CLOCK_CLR				0x04	/* for clear the bits */
#define SAU0_CLOCK_STOP				0x00	/* stops supply of input clock */
#define SAU0_CLOCK_SUPPLY			0x04	/* supplies input clock */

/* Control of serial array unit 1 input clock(SAU1EN) */
#define SAU1_CLOCK_CLR				0x08	/* for clear the bits */
#define SAU1_CLOCK_STOP				0x00	/* stops supply of input clock */
#define SAU1_CLOCK_SUPPLY			0x08	/* supplies input clock */

/*
	Serial Clock Select Register m (SPSm)
*/
/* Operating mode and clear mode selection(PRS003~PRS000) */
#define SAU_CK00_FCLK_0				0x0000	/* ck00-fclk */
#define SAU_CK00_FCLK_1				0x0001	/* ck00-fclk/2^1 */
#define SAU_CK00_FCLK_2				0x0002	/* ck00-fclk/2^2 */
#define SAU_CK00_FCLK_3				0x0003	/* ck00-fclk/2^3 */
#define SAU_CK00_FCLK_4				0x0004	/* ck00-fclk/2^4 */
#define SAU_CK00_FCLK_5				0x0005	/* ck00-fclk/2^5 */
#define SAU_CK00_FCLK_6				0x0006	/* ck00-fclk/2^6 */
#define SAU_CK00_FCLK_7				0x0007	/* ck00-fclk/2^7 */
#define SAU_CK00_FCLK_8				0x0008	/* ck00-fclk/2^8 */
#define SAU_CK00_FCLK_9				0x0009	/* ck00-fclk/2^9 */
#define SAU_CK00_FCLK_10			0x000A	/* ck00-fclk/2^10 */
#define SAU_CK00_FCLK_11			0x000B	/* ck00-fclk/2^11 */
/* Operating mode and clear mode selection(PRS013~PRS010) */
#define SAU_CK01_FCLK_0				0x0000	/* ck01-fclk */
#define SAU_CK01_FCLK_1				0x0010	/* ck01-fclk/2^1 */
#define SAU_CK01_FCLK_2				0x0020	/* ck01-fclk/2^2 */
#define SAU_CK01_FCLK_3				0x0030	/* ck01-fclk/2^3 */
#define SAU_CK01_FCLK_4				0x0040	/* ck01-fclk/2^4 */
#define SAU_CK01_FCLK_5				0x0050	/* ck01-fclk/2^5 */
#define SAU_CK01_FCLK_6				0x0060	/* ck01-fclk/2^6 */
#define SAU_CK01_FCLK_7				0x0070	/* ck01-fclk/2^7 */
#define SAU_CK01_FCLK_8				0x0080	/* ck01-fclk/2^8 */
#define SAU_CK01_FCLK_9				0x0090	/* ck01-fclk/2^9 */
#define SAU_CK01_FCLK_10			0x00A0	/* ck01-fclk/2^10 */
#define SAU_CK01_FCLK_11			0x00B0	/* ck01-fclk/2^11 */

/*
	Serial Mode Register mn (SMRmn)
*/

#define	SAU_SMRMN_INITIALVALUE		0x0020 

/* Selection of macro clock (MCK) of channel n(CKSmn) */
#define SAU_CLOCK_SELECT_CLR		0x8000	/* for clear the bits */
#define SAU_CLOCK_SELECT_CK00		0x0000	/* operation clock CK0 set by PRS register */ 
#define SAU_CLOCK_SELECT_CK01		0x8000	/* 0peration clock CK1 set by PRS register */
/* Selection of transfer clock (TCLK) of channel n(CCSmn) */
#define SAU_CLOCK_MODE_CLR			0x4000	/* for clear the bits */
#define SAU_CLOCK_MODE_CKS			0x0000	/* divided operation clock MCK specified by CKSmn bit */  
#define SAU_CLOCK_MODE_TI0N			0x4000	/* clock input from SCK pin (slave transfer in CSI mode) */
/* Selection of start trigger source(STSmn) */
#define SAU_TRIGGER_CLR				0x0100	/* for clear the bits */
#define SAU_TRIGGER_SOFTWARE		0x0000	/* only software trigger is valid */
#define SAU_TRIGGER_RXD				0x0100	/* valid edge of RXD pin */
/* Controls inversion of level of receive data of channel n in UART mode(SISmn0) */
#define SAU_EDGE_FALL				0x0000	/* falling edge is detected as the start bit */
#define SAU_EDGE_RISING				0x0040	/* rising edge is detected as the start bit */
/* Setting of operation mode of channel n(MDmn2,MDmn1) */
#define SAU_CSI						0x0000	/* CSI mode */
#define SAU_UART					0x0002	/* UART mode */
#define SAU_IIC						0x0004	/* simplified IIC mode */
/* Selection of interrupt source of channel n(MDmn0) */
#define SAU_TRANSFER_END			0x0000	/* transfer end interrupt */
#define SAU_BUFFER_EMPTY			0x0001	/* buffer empty interrupt */

/*
	Serial Communication Operation Setting Register mn (SCRmn)
*/
/* Setting of operation mode of channel n(TXEmn,RXEmn) */
#define SAU_NOT_COMMUNICATION		0x0000	/* does not start communication */
#define SAU_RECEPTION				0x4000	/* reception only */
#define SAU_TRANSMISSION			0x8000	/* transmission only */
#define SAU_RECEPTION_TRANSMISSION	0xc000	/* reception and transmission */
/* Selection of data and clock phase in CSI mode(DAPmn,CKPmn) */
#define SAU_TIMING_1				0x0000	/* type 1 */
#define SAU_TIMING_2				0x1000	/* type 2 */
#define SAU_TIMING_3				0x2000	/* type 3 */
#define SAU_TIMING_4				0x3000	/* type 4 */
/* Selection of masking of error interrupt signal(EOCmn) */
#define SAU_INTSRE_MASK				0x0000	/* masks error interrupt INTSREx */
#define SAU_INTSRE_ENABLE			0x0400	/* enables generation of error interrupt INTSREx */
/* Setting of parity bit in UART mode(PTCmn1,PTCmn0) */
#define SAU_PARITY_NONE				0x0000	/* none parity */
#define SAU_PARITY_ZERO				0x0100	/* zero parity */
#define SAU_PARITY_EVEN				0x0200	/* even parity */
#define SAU_PARITY_ODD				0x0300	/* odd parity */
/* Selection of data transfer sequence in CSI and UART modes(DIRmn) */
#define SAU_MSB						0x0000	/* MSB */
#define SAU_LSB						0x0080	/* LSB */
/* Setting of stop bit in UART mode(SLCmn1,SLCmn0) */
#define SAU_STOP_NONE				0x0000	/* none stop bit */
#define SAU_STOP_1					0x0010	/* 1 stop bit */
#define SAU_STOP_2					0x0020	/* 2 stop bits */
/* Setting of data length in CSI and UART modes(DLSmn2~DLSmn0) */
#define SAU_LENGTH_5				0x0004	/* 5-bit data length */
#define SAU_LENGTH_7				0x0006	/* 7-bit data length */
#define SAU_LENGTH_8				0x0007	/* 8-bit data length */

/*
	Serial Output Level Register m (SOLm)
*/
/* Selects inversion of the level of the transmit data of channel n in UART mode */
#define SAU_CHANNEL0_NORMAL			0x0000	/* normal bit level */
#define SAU_CHANNEL0_INVERTED		0x0001	/* inverted bit level */
#define SAU_CHANNEL1_NORMAL			0x0000	/* normal bit level */  
#define SAU_CHANNEL1_INVERTED		0x0002	/* inverted bit level */
#define SAU_CHANNEL2_NORMAL			0x0000	/* normal bit level */  
#define SAU_CHANNEL2_INVERTED		0x0004	/* inverted bit level */
#define SAU_CHANNEL3_NORMAL			0x0000	/* normal bit level */  
#define SAU_CHANNEL3_INVERTED		0x0008	/* inverted bit level */

/*
	Noise Filter Enable Register 0 (NFEN0)
*/
/* Use of noise filter of RXD3/P14 pin */
#define SAU_RXD3_FILTER_OFF			0x00	/* noise filter off */
#define SAU_RXD3_FILTER_ON			0x40	/* noise filter on */
#define SAU_RXD2_FILTER_OFF			0x00	/* noise filter off */
#define SAU_RXD2_FILTER_ON			0x10	/* noise filter on */
#define SAU_RXD1_FILTER_OFF			0x00	/* noise filter off */
#define SAU_RXD1_FILTER_ON			0x04	/* noise filter on */
#define SAU_RXD0_FILTER_OFF			0x00	/* noise filter off */
#define SAU_RXD0_FILTER_ON			0x01	/* noise filter on */

/*
	Format of Serial Status Register mn (SSRmn)
*/
/* Communication status indication flag of channel n(TSFmn) */
#define SAU_UNDER_EXECUTE			0x0040	/* communication is under execution */
#define SAU_DATA_STORED				0x0020	/* valid data is stored SDRmn register */

/* Parity error detection flag of channel n(PEFmn) */
#define SAU_PARITY_ERROR			0x0002	/* a parity error occurs during UART reception or ACK is not detected during I2C transmission */

/*
	Serial Channel Start Register m (SSm)
*/
/* Operation start trigger of channel 0(SSm0) */
#define SAU_CH0_START_TRG_OFF		0x0000	/* no trigger operation */
#define SAU_CH0_START_TRG_ON		0x0001	/* sets SEm0 to 1 and enters the communication wait status */
/* Operation start trigger of channel 1(SSm1) */
#define SAU_CH1_START_TRG_OFF		0x0000	/* no trigger operation */
#define SAU_CH1_START_TRG_ON		0x0002	/* sets SEm1 to 1 and enters the communication wait status */
/* Operation start trigger of channel 2(SSm2) */
#define SAU_CH2_START_TRG_OFF		0x0000	/* no trigger operation */
#define SAU_CH2_START_TRG_ON		0x0004	/* sets SEm2 to 1 and enters the communication wait status */
/* Operation start trigger of channel 3(SSm3) */
#define SAU_CH3_START_TRG_OFF		0x0000	/* no trigger operation */
#define SAU_CH3_START_TRG_ON		0x0008	/* sets SEm3 to 1 and enters the communication wait status */

/*
	Serial Channel Stop Register m (STm)
*/
/* Operation stop trigger of channel 0(STm0) */
#define SAU_CH0_STOP_TRG_OFF		0x0000	/* no trigger operation */
#define SAU_CH0_STOP_TRG_ON			0x0001	/* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 1(STm1) */
#define SAU_CH1_STOP_TRG_OFF		0x0000	/* no trigger operation */
#define SAU_CH1_STOP_TRG_ON			0x0002	/* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 2(STm2) */
#define SAU_CH2_STOP_TRG_OFF		0x0000	/* no trigger operation */
#define SAU_CH2_STOP_TRG_ON			0x0004	/* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 3(STm3) */
#define SAU_CH3_STOP_TRG_OFF		0x0000	/* no trigger operation */
#define SAU_CH3_STOP_TRG_ON			0x0008	/* operation is stopped (stop trigger is generated) */

/*
	Format of Serial Flag Clear Trigger Register mn (SIRmn)
*/
/* Clear trigger of overrun error flag of channel n(OVCTmn) */
#define	SAU_SIRMN_OVCTMN		0x0001
/* Clear trigger of parity error flag of channel n(PECTmn) */
#define	SAU_SIRMN_PECTMN		0x0002
/* Clear trigger of framing error of channel n(FECTMN) */
#define	SAU_SIRMN_FECTMN		0x0004

/*
	Serial Output Enable Register m (SOEm)
*/
/* Serial output enable/disable of channel 0(SOEm0) */
#define SAU_CH0_OUTPUT_ENABLE		0x0001	/* stops output by serial communication operation */
#define SAU_CH0_OUTPUT_DISABLE		0x0000	/* enables output by serial communication operation */
/* Serial output enable/disable of channel 1(SOEm1) */
#define SAU_CH1_OUTPUT_ENABLE		0x0002	/* stops output by serial communication operation */
#define SAU_CH1_OUTPUT_DISABLE		0x0000	/* enables output by serial communication operation */
/* Serial output enable/disable of channel 2(SOEm2) */
#define SAU_CH2_OUTPUT_ENABLE		0x0004	/* stops output by serial communication operation */
#define SAU_CH2_OUTPUT_DISABLE		0x0000	/* enables output by serial communication operation */
/* Serial output enable/disable of channel 3(SOEm3) */
#define SAU_CH3_OUTPUT_ENABLE		0x0008	/* stops output by serial communication operation */
#define SAU_CH3_OUTPUT_DISABLE		0x0000	/* enables output by serial communication operation */

/*
	Serial Output Register m (SOm)
*/
/* Serial data output of channel 0(SOm0) */
#define SAU_CH0_DATA_OUTPUT_0		0x0000	/* Serial data output value is "0" */
#define SAU_CH0_DATA_OUTPUT_1		0x0001	/* Serial data output value is "1" */
/* Serial data output of channel 1(SOm1) */
#define SAU_CH1_DATA_OUTPUT_0		0x0000	/* Serial data output value is "0" */
#define SAU_CH1_DATA_OUTPUT_1		0x0002	/* Serial data output value is "1" */
/* Serial data output of channel 2(SOm2) */
#define SAU_CH2_DATA_OUTPUT_0		0x0000	/* Serial data output value is "0" */
#define SAU_CH2_DATA_OUTPUT_1		0x0004	/* Serial data output value is "1" */
/* Serial data output of channel 3(SOm3) */
#define SAU_CH3_DATA_OUTPUT_0		0x0000	/* Serial data output value is "0" */
#define SAU_CH3_DATA_OUTPUT_1		0x0008	/* Serial data output value is "1" */
/* Serial clock output of channel 0(CKOm0) */
#define SAU_CH0_CLOCK_OUTPUT_0		0x0000	/* Serial clock output value is "0" */
#define SAU_CH0_CLOCK_OUTPUT_1		0x0100	/* Serial clock output value is "1" */
/* Serial clock output of channel 1(CKOm1) */
#define SAU_CH1_CLOCK_OUTPUT_0		0x0000	/* Serial clock output value is "0" */
#define SAU_CH1_CLOCK_OUTPUT_1		0x0200	/* Serial clock output value is "1" */
/* Serial clock output of channel 2(CKOm2) */
#define SAU_CH2_CLOCK_OUTPUT_0		0x0000	/* Serial clock output value is "0" */
#define SAU_CH2_CLOCK_OUTPUT_1		0x0400	/* Serial clock output value is "1" */
/* Serial clock output of channel 3(CKOm3) */
#define SAU_CH3_CLOCK_OUTPUT_0		0x0000	/* Serial clock output value is "0" */
#define SAU_CH3_CLOCK_OUTPUT_1		0x0800	/* Serial clock output value is "1" */

#define SAU_IIC_MASTER_FLAG_CLEAR	0x00
#define SAU_IIC_SEND_FLAG			0x01
#define SAU_IIC_RECEIVE_FLAG		0x02
#define SAU_IIC_SENDED_ADDRESS_FLAG	0x04

#define	SAU_IIC_HOLDTIME				0x0fa
/*
	Peripheral Enable Register 0 (PER0)
*/
/* Control of timer array unit input clock(IIC0EN) */
#define IIC0_CLOCK_CLR				0x10	/* for clear the bits */
#define IIC0_CLOCK_STOP				0x00	/* stops supply of input clock */
#define IIC0_CLOCK_SUPPLY			0x10	/* supplies input clock */

/*
	IIC control register 0 (IICC0)
*/
/* IIC operation enable	(IICE0) */
#define	IIC0_OPERATION				0x80
#define	IIC0_OPERATION_DISABLE		0x00	/* stop	operation */
#define	IIC0_OPERATION_ENABLE		0x80	/* enable operation */
/* Exit	from communications	(LREL0) */
#define	IIC0_COMMUNICATION			0x40
#define	IIC0_COMMUNICATION_NORMAL	0x00	/* normal operation */
#define	IIC0_COMMUNICATION_EXIT		0x40	/* exit	from current communication */
/* Wait	cancellation (WREL0) */
#define	IIC0_WAITCANCEL				0x20
#define	IIC0_WAIT_NOTCANCEL			0x00	/* do not cancel wait */
#define	IIC0_WAIT_CANCEL			0x20	/* cancel wait */
/* Generation of interrupt when	stop condition (SPIE0) */
#define	IIC0_STOPINT				0x10
#define	IIC0_STOPINT_DISABLE		0x00	/* disable */
#define	IIC0_STOPINT_ENABLE			0x10	/* enable */
/* Wait	and	interrupt generation (WTIM0) */
#define	IIC0_WAITINT				0x08
#define	IIC0_WAITINT_CLK8FALLING	0x00	/* generate at the eighth clocks falling edge */
#define	IIC0_WAITINT_CLK9FALLING	0x08	/* generated at the ninth clocks falling edge */
/* Acknowledgement control (ACKE0) */
#define	IIC0_ACK					0x04
#define	IIC0_ACK_DISABLE			0x00	/* enable acknowledgement */
#define	IIC0_ACK_ENABLE				0x04	/* disable acknowledgement */
/* Start condition trigger (STT0) */
#define	IIC0_STARTCONDITION			0x02
#define	IIC0_START_NOTGENERATE		0x00	/* do not generate start condition */
#define	IIC0_START_GENERATE			0x02	/* generate start condition */
/* Stop	condition trigger (SPT0) */
#define	IIC0_STOPCONDITION			0x01
#define	IIC0_STOP_NOTGENERATE		0x00	/* do not generate stop condition */
#define	IIC0_STOP_GENERATE			0x01	/* generate stop condition */

/*
	IIC Status Register 0 (IICS0)
*/
/* Master device status	(MSTS0) */
#define	IIC0_MASTERSTATUS			0x80
#define	IIC0_STATUS_NOTMASTER		0x00	/* slave device status or communication standby status */
#define	IIC0_STATUS_MASTER			0x80	/* master device communication status */
/* Detection of	arbitration	loss (ALD0) */
#define	IIC0_ARBITRATION			0x40
#define	IIC0_ARBITRATION_NO			0x00	/* arbitration win or no arbitration */
#define	IIC0_ARBITRATION_LOSS		0x40	/* arbitration loss */
/* Detection of	extension code reception (EXC0) */
#define	IIC0_EXTENSIONCODE			0x20
#define	IIC0_EXTCODE_NOT			0x00	/* extension code not received */
#define	IIC0_EXTCODE_RECEIVED		0x20	/* extension code received */
/* Detection of	matching addresses (COI0) */
#define	IIC0_ADDRESSMATCH			0x10
#define	IIC0_ADDRESS_NOTMATCH		0x00	/* addresses do not match */
#define	IIC0_ADDRESS_MATCH			0x10	/* addresses match */
/* Detection of	transmit/receive status	(TRC0) */
#define	IIC0_STATUS					0x08
#define	IIC0_STATUS_RECEIVE			0x00	/* receive status */
#define	IIC0_STATUS_TRANSMIT		0x08	/* transmit status */
/* Detection of	acknowledge	signal (ACKD0) */
#define	IIC0_ACKDETECTION			0x04
#define	IIC0_ACK_NOTDETECTED		0x00	/* ACK signal was not detected */
#define	IIC0_ACK_DETECTED			0x04	/* ACK signal was detected */
/* Detection of	start condition	(STD0) */
#define	IIC0_STARTDETECTION			0x02
#define	IIC0_START_NOTDETECTED		0x00	/* start condition not detected */
#define	IIC0_START_DETECTED			0x02	/* start condition detected */
/* Detection of	stop condition (SPD0) */
#define	IIC0_STOPDETECTION			0x01
#define	IIC0_STOP_NOTDETECTED		0x00	/* stop condition not detected */
#define	IIC0_STOP_DETECTED			0x01	/* stop condition detected */

/*
	IIC Flag Register 0 (IICF0)
*/
/* STT0	clear flag (STCF) */
#define	IIC0_STARTFLAG				0x80
#define	IIC0_STARTFLAG_GENERATE		0x00	/* generate start condition */
#define	IIC0_STARTFLAG_UNSUCCESSFUL	0x80	/* start condition generation unsuccessful */
/* IIC bus status flag (IICBSY) */
#define	IIC0_BUSSTATUS				0x40
#define	IIC0_BUS_RELEASE			0x00	/* bus release status */
#define	IIC0_BUS_COMMUNICATION		0x40	/* bus communication status */
/* Initial start enable	trigger	(STCEN) */
#define	IIC0_STARTWITHSTOP			0x02
#define	IIC0_START_WITHSTOP			0x00	/* generation of a start condition without detecting a stop condition */
#define	IIC0_START_WITHOUTSTOP		0x02	/* generation of a start condition upon detection of a stop condition */
/* Communication reservation function disable bit (IICRSV) */
#define	IIC0_RESERVATION			0x01
#define	IIC0_RESERVATION_ENABLE		0x00	/* enable communication reservation */
#define	IIC0_RESERVATION_DISABLE	0x01	/* disable communication reservation */

/*
	IIC clock selection	register 0 (IICCL0)
*/

#define IICCL0_INITIALVALUE				0x00
/* Detection of	SCL0 pin level (CLD0) */
#define	IIC0_SCLLEVEL				0x20
#define	IIC0_SCL_LOW				0x00	/* clock line at low level */
#define	IIC0_SCL_HIGH				0x20	/* clock line at high level */
/* Detection of	SDA0 pin level (DAD0) */
#define	IIC0_SDALEVEL				0x10
#define	IIC0_SDA_LOW				0x00	/* data line at low level */
#define	IIC0_SDA_HIGH				0x10	/* data line at high level */
/* Operation mode switching (SMC0) */
#define	IIC0_OPERATIONMODE			0x08
#define	IIC0_MODE_STANDARD			0x00	/* operates in standard mode */
#define	IIC0_MODE_HIGHSPEED			0x08	/* operates in high-speed mode */
/* Digital filter operation	control (DFC0) */
#define	IIC0_DIGITALFILTER			0x04
#define	IIC0_FILTER_OFF				0x00	/* digital filter off */
#define	IIC0_FILTER_ON				0x04	/* digital filter on */
/* Operation mode switching	(CL01, CL00) */
#define	IIC0_CLOCKSELECTION			0x03
/*	Combine	of (SMC0, CL01,	CL00)*/
#define	IIC0_CLOCK0					0x00
#define	IIC0_CLOCK1					0x01
#define	IIC0_CLOCK2					0x02
#define	IIC0_CLOCK3					0x03
#define	IIC0_CLOCK4					0x08
#define	IIC0_CLOCK5					0x09
#define	IIC0_CLOCK6					0x0a
#define	IIC0_CLOCK7					0x0b

/*
	IIC function expansion register 0 (IICX0)
*/
/* IIC clock expension (CLX0) */
#define	IIC0_CLOCKEXPENSION			0x01
#define	IIC0_EXPENSION0				0x00
#define	IIC0_EXPENSION1				0x01
/*	Operation clock	(CLX0, SMC0, CL01, CL00)

				|	IIC0_EXPENSION0	|	IIC0_EXPENSION1	|
	------------|-------------------|-------------------|----------------------
	IIC0_CLOCK0	|		fCLK/88		|					|	transfer clock
				|		normal		|					|	mode
	------------|-------------------|-------------------|----------------------
	IIC0_CLOCK1	|		fCLK/172	|					|	transfer clock
				|		normal		|					|	mode
	------------|-------------------|-------------------|----------------------
	IIC0_CLOCK2	|		fCLK/344	|					|	transfer clock
				|		normal		|					|	mode
	------------|-------------------|-------------------|----------------------
	IIC0_CLOCK3	|		fCLK/44		|					|	transfer clock
				|		normal		|					|	mode
	------------|-------------------|-------------------|----------------------
	IIC0_CLOCK4	|		fCLK/48		|		fCLK/24		|	transfer clock
				|	  	fast	 	|		fast		|	mode
	------------|-------------------|-------------------|----------------------
	IIC0_CLOCK5	|		fCLK/48		|		fCLK/24		|	transfer clock
				|	  	fast		|		fast		|	mode
	------------|-------------------|-------------------|----------------------
	IIC0_CLOCK6	|		fCLK/96		|		fCLK/48		|	transfer clock
				|	  	fast		|		fast		|	mode
	------------|-------------------|-------------------|----------------------
	IIC0_CLOCK7	|		fCLK/24		|		fCLK/24		|	transfer clock
				|	  	fast		|		fast		|	mode
	------------|-------------------|-------------------|----------------------
*/

#define ADDRESS_COMPLETE			0x80
#define IIC0_MASTER_FLAG_CLEAR		0x00

#define False 0
#define Ture  1
/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/

enum TransferMode { SEND, RECEIVE };


//关闭UART0口的接收，及其中断
#define DIS_UART0_REC   {ST0|=SAU_CH1_STOP_TRG_ON;SRMK0 = 1;SREMK0 = 1;}	/* UART0 receive disable */
        
/*
*******************************************************************************
**  Function define
*******************************************************************************
*/
void SAU0_Init( void );
void UART0_Init( void );
void UART0_Start( void );
void UART0_Stop( void );
MD_STATUS UART0_SendData( UCHAR* txbuf, USHORT txnum );
MD_STATUS UART0_ReceiveData( UCHAR* rxbuf, USHORT rxnum );
void CALL_UART0_Error( UCHAR err_type );
void CALL_UART0_Send( void );

void UART1_Init( void );
void UART1_Start( void );
void UART1_Stop( void );
MD_STATUS UART1_SendData( UCHAR* txbuf, USHORT txnum );
MD_STATUS UART1_ReceiveData( UCHAR* rxbuf, USHORT rxnum );
void CALL_UART1_Error( UCHAR err_type );
void CALL_UART1_Send( void );

void SAU1_Init( void );
void UART2_Init( void );
void UART2_Start( void );
void UART2_Stop( void );
MD_STATUS UART2_SendData( UCHAR* txbuf, USHORT txnum );
MD_STATUS UART2_ReceiveData( UCHAR* rxbuf, USHORT rxnum );
void CALL_UART2_Error( UCHAR err_type );
void CALL_UART2_Send( void );
void UART3_Init( void );
void UART3_Start( void );
void UART3_Stop( void );
MD_STATUS UART3_SendData( UCHAR* txbuf, USHORT txnum );
MD_STATUS UART3_ReceiveData( UCHAR* rxbuf, USHORT rxnum );
void CALL_UART3_Error( UCHAR err_type );
void CALL_UART3_Send( void );


/* Start user code for definition. Do not edit comment generated here */
#define UART0_RECEIVE_DIVISOR	17<<9
#define UART0_TRANSMIT_DIVISOR	17<<9

#define UART1_RECEIVE_DIVISOR	0x400
#define UART1_TRANSMIT_DIVISOR	0x400
#define UART2_RECEIVE_DIVISOR	0x600
#define UART2_TRANSMIT_DIVISOR	0x600
#define UART3_RECEIVE_DIVISOR	0x600
#define UART3_TRANSMIT_DIVISOR	0x600

/* End user code for definition. Do not edit comment generated here */
#endif
