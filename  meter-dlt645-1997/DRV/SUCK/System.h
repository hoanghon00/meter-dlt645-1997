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
**  Filename :	System.h
**  Abstract :	This file implements device driver for System module.
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
#ifndef _MDSYSTEM_
#define _MDSYSTEM_
/*
*******************************************************************************
**	Register bit define
*******************************************************************************
*/
/*
	Clock operation mode control register (CMC)
*/
/* High-speed system clock pin operation mode (EXCLK, OSCSEL) */
#define	CG_HISYS_EXTCLK		0x80	/* external clock bit */
#define	CG_HISYS_OSCSEL		0x40	/* clock/port bit */
#define	CG_HISYS_PIN		0xc0
#define	CG_HISYS_PORT		0x00	/* X1, X2 as I/O port */
#define	CG_HISYS_OSC		0x40	/* X1, X2 as crystal/ceramic resonator connection */
#define	CG_HISYS_PORT1		0x80	/* X1, X2 as I/O port */
#define	CG_HISYS_EXT		0xc0	/* X1 as external clock input, X2 as I/O port */
/* Subsystem clock pin operation mode (OSCSELS) */
#define	CG_SUB_OSCSELS		0x10	/* sub clock/port bit */
#define	CG_SUB_PORT			0x00	/* XT1, XT2 as I/O port	*/
#define	CG_SUB_OSC			0x10	/* XT1, XT2 as crystal connection */
/* Control of high-speed system clock oscillation frequency (AMPH) */
#define	CG_SYSOSC_FREQ		0x01
#define	CG_SYSOSC_UNDER10M	0x00	/* 2MHz <=fxh <= 10MHz */
#define	CG_SYSOSC_OVER10M	0x01	/* 10MHz < fxh <= 20MHz */

/*
	Clock operation status control register (CSC)
*/
/* Control of high-speed system clock operation (MSTOP) */
#define	CG_HISYS_MSTOP		0x80
#define	CG_HISYS_OPER		0x00	/* X1 oscillator/external clock operating */
#define	CG_HISYS_STOP		0x80	/* X1 oscillator/external clock stopped */
/* Subsystem clock operation (XTSTOP) */
#define	CG_SUB_XTSTOP		0x40
#define	CG_SUB_OPER			0x00	/* XT1 oscillator operating */
#define	CG_SUB_STOP			0x40	/* XT1 oscillator stopped */
/* Internal high-speed oscillator operation (HIOSTOP) */
#define	CG_HIO_HIOSTOP		0x01
#define	CG_HIO_OPER			0x00	/* internal high-speed oscillator operating */
#define	CG_HIO_STOP			0x01	/* internal high-speed oscillator stopped */

/*
	Oscillation stabilization time counter status register (OSTC)
*/
/* Oscillation stabilization time status ( MOST8,9,10,11,13,15,17,18) */
#define	CG_OSCSTAB_STATUS	0xff
#define	CG_OSCSTAB_STA0		0x00	/* <2^8/fx */
#define	CG_OSCSTAB_STA8		0x80	/* 2^8/fx */
#define	CG_OSCSTAB_STA9		0xc0	/* 2^9/fx */
#define	CG_OSCSTAB_STA10	0xe0	/* 2^10/fx */
#define	CG_OSCSTAB_STA11	0xf0	/* 2^11/fx */
#define	CG_OSCSTAB_STA13	0xf8	/* 2^13/fx */
#define	CG_OSCSTAB_STA15	0xfc	/* 2^15/fx */
#define	CG_OSCSTAB_STA17	0xfe	/* 2^17/fx */
#define	CG_OSCSTAB_STA18	0xff	/* 2^18/fx */

/*
	Oscillation stabilization time select register (OSTS)
*/
/* Oscillation stabilization time selection (OSTS2-0) */
#define	CG_OSCSTAB_SEL	0x07
#define	CG_OSCSTAB_SEL8		0x00	/* 2^8/fx */
#define	CG_OSCSTAB_SEL9		0x01	/* 2^9/fx */
#define	CG_OSCSTAB_SEL10	0x02	/* 2^10/fx */
#define	CG_OSCSTAB_SEL11	0x03	/* 2^11/fx */
#define	CG_OSCSTAB_SEL13	0x04	/* 2^13/fx */
#define	CG_OSCSTAB_SEL15	0x05	/* 2^15/fx */
#define	CG_OSCSTAB_SEL17	0x06	/* 2^17/fx */
#define	CG_OSCSTAB_SEL18	0x07	/* 2^18/fx */

/*
	System clock control register (CKC)
*/
/* Status of CPU/peripheral hardware clock fCLK (CLS) */
#define	CG_CPUCLK_STATUS	0x80
#define	CG_CPUCLK_MAIN		0x00	/* main system clock (fMAIN) */
#define	CG_CPUCLK_SUB		0x80	/* subsystem clock (fSUB) */
/* Selection of CPU/peripheral hardware clock fCLK (CSS) */
#define	CG_CPUCLK_SEL		0x40
#define	CG_CPUCLK_SELMAIN	0x00	/* main system clock (fMAIN) */
#define	CG_CPUCLK_SELSUB	0x40	/* subsystem clock (fSUB) */
/* Status of Main system clock fMAIN (MCS) */
#define	CG_MAINCLK_STATUS	0x20
#define	CG_MAINCLK_HIO		0x00	/* Internal high-speed oscillation clock (fIH) */
#define	CG_MAINCLK_HISYS	0x20	/* High-speed system clock (fMX) */
/* Selection of Main system clock fMAIN (MCM0) */
#define	CG_MAINCLK_SEL		0x10
#define	CG_MAINCLK_SELHIO	0x00	/* Internal high-speed oscillation clock (fIH) */
#define	CG_MAINCLK_SELHISYS	0x10	/* High-speed system clock (fMX) */
/* CPU clock (fcpu) selection (MDIV2-0) */
/** Note: valid when CKC.CSS = 0 **/
#define	CG_CPUCLK			0x07
#define	CG_CPUCLK_MAIN0		0x00	/* fCLK = fMAIN */
#define	CG_CPUCLK_MAIN1		0x01	/* fCLK = fMAIN/2 (default) */
#define	CG_CPUCLK_MAIN2		0x02	/* fCLK = fMAIN/2^2 */
#define	CG_CPUCLK_MAIN3		0x03	/* fCLK = fMAIN/2^3 */
#define	CG_CPUCLK_MAIN4		0x04	/* fCLK = fMAIN/2^4 */
#define	CG_CPUCLK_MAIN5		0x05	/* fCLK = fMAIN/2^5 */

/*
	Peripheral enable registers 0 (PER0)
*/
/* Control of real-time counter input clock (RTCEN) */
#define	CG_PERCLK_RTC		0x80
#define	CG_PERCLK_RTCOFF	0x00	/* stops input clock supply */
#define	CG_PERCLK_RTCON		0x80	/* supplies input clock */
/* Control of D/A converter input clock (DACEN) */
#define	CG_PERCLK_DA		0x40
#define	CG_PERCLK_DAOFF		0x00	/* stops input clock supply */
#define	CG_PERCLK_DAON		0x40	/* supplies input clock */
/* Control of A/D converter input clock (ADCEN) */
#define	CG_PERCLK_AD		0x20
#define	CG_PERCLK_ADOFF		0x00	/* stops input clock supply */
#define	CG_PERCLK_ADON		0x20	/* supplies input clock */
/* Control of serial interface IIC0 input clock (IIC0EN) */
#define	CG_PERCLK_IIC0		0x10
#define	CG_PERCLK_IIC0OFF	0x00	/* stops input clock supply */
#define	CG_PERCLK_IIC0ON	0x10	/* supplies input clock */
/* Control of serial array unit 1 input clock (SAU1EN) */
#define	CG_PERCLK_SAU1		0x08
#define	CG_PERCLK_SAU1OFF	0x00	/* stops input clock supply */
#define	CG_PERCLK_SAU1ON	0x08	/* supplies input clock */
/* Control of serial array unit 0 input clock (SAU0EN) */
#define	CG_PERCLK_SAU0		0x04
#define	CG_PERCLK_SAU0OFF	0x00	/* stops input clock supply */
#define	CG_PERCLK_SAU0ON	0x04	/* supplies input clock */
/* Control of timer array unit input clock (TAU0EN) */
#define	CG_PERCLK_TAU0		0x01
#define	CG_PERCLK_TAU0OFF	0x00	/* stops input clock supply */
#define	CG_PERCLK_TAU0ON	0x01	/* supplies input clock */

/*
	Peripheral enable registers 1 (PER1)
*/
/* Control of external bus interface input clock (EXBEN) */
#define	CG_PERCLK_BUS		0x01
#define	CG_PERCLK_BUSOFF	0x00	/* stops input clock supply */
#define	CG_PERCLK_BUSON		0x01	/* supplies input clock */

/*
	Operation speed mode control register (OSMC)
*/
/* fCLK frequency selection (FSEL) */
#define	CG_FCLK_FREQ		0x01
#define	CG_FCLK_UNDER10M	0x00	/* fCLK <= 10 MHz */
#define	CG_FCLK_OVER10M		0x01	/* fCLK > 10 MHz */

/*
	Internal high-speed oscillator trimming register (HIOTRM)
*/
/* Clock correction value (TTRM4-0) */
#define	CG_HIO_TIMMING	0x1f
#define	CG_HIO_TIMN4D8	0x00	/* -4.8% */
#define	CG_HIO_TIMN4D5	0x01	/* -4.5% */
#define	CG_HIO_TIMN4D2	0x02	/* -4.2% */
#define	CG_HIO_TIMN3D9	0x03	/* -3.9% */
#define	CG_HIO_TIMN3D6	0x04	/* -3.6% */
#define	CG_HIO_TIMN3D3	0x05	/* -3.3% */
#define	CG_HIO_TIMN3D0	0x06	/* -3.0% */
#define	CG_HIO_TIMN2D7	0x07	/* -2.7% */
#define	CG_HIO_TIMN2D4	0x08	/* -2.4% */
#define	CG_HIO_TIMN2D1	0x09	/* -2.1% */
#define	CG_HIO_TIMN1D8	0x0a	/* -1.8% */
#define	CG_HIO_TIMN1D5	0x0b	/* -1.5% */
#define	CG_HIO_TIMN1D2	0x0c	/* -1.2% */
#define	CG_HIO_TIMN0D9	0x0d	/* -0.9% */
#define	CG_HIO_TIMN0D6	0x0e	/* -0.6% */
#define	CG_HIO_TIMN0D3	0x0f	/* -0.3% */
#define	CG_HIO_TIMN0	0x10	/* 0% */
#define	CG_HIO_TIMP0D3	0x11	/* +0.3% */
#define	CG_HIO_TIMP0D6	0x12	/* +0.6% */
#define	CG_HIO_TIMP0D9	0x13	/* +0.9% */
#define	CG_HIO_TIMP1D2	0x14	/* +1.2% */
#define	CG_HIO_TIMP1D5	0x15	/* +1.5% */
#define	CG_HIO_TIMP1D8	0x16	/* +1.8% */
#define	CG_HIO_TIMP2D1	0x17	/* +2.1% */
#define	CG_HIO_TIMP2D4	0x18	/* +2.4% */
#define	CG_HIO_TIMP2D7	0x19	/* +2.7% */
#define	CG_HIO_TIMP3D0	0x1a	/* +3.0% */
#define	CG_HIO_TIMP3D3	0x1b	/* +3.3% */
#define	CG_HIO_TIMP3D6	0x1c	/* +3.6% */
#define	CG_HIO_TIMP3D9	0x1d	/* +3.9% */
#define	CG_HIO_TIMP4D2	0x1e	/* +4.2% */
#define	CG_HIO_TIMP4D5	0x1f	/* +4.5% */

/*
	Temperature correction tables H (TTBLH)
*/
#define	CG_TTBLH_TSADRH	0x00ff
#define	CG_TTBLH_TTRMH	0x1f00

/*
	Temperature correction tables L (TTBLL)
*/
#define	CG_TTBLL_TSADRL	0x00ff
#define	CG_TTBLL_TTRML	0x1f00

/*
	Temperature correction in AD
*/      
#define CG_TEMP_ADM_InitValue	0x00
#define CG_TEMP_AD_TIME4		0x20
#define CG_TEMP_AD_AVREF23V		0x02
#define CG_TEMP_AD_SENSOR1		0x80

/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
#define CG_SUBWAITTIME 240000
#define CG_FIHWAITTIME 40
enum ClockMode { HIOCLK, SYSX1CLK, SYSEXTCLK, SUBCLK };
enum CPUClock { SYSTEMCLOCK, SYSONEHALF, SYSONEFOURTH, SYSONEEIGHTH, 
				SYSONESIXTEENTH, SYSONETHIRTYSECOND };
enum PSLevel { PSSTOP, PSHALT };
enum StabTime { STLEVEL0, STLEVEL1, STLEVEL2, STLEVEL3,
				STLEVEL4, STLEVEL5, STLEVEL6, STLEVEL7 };


#define Get_Main_Clock_Div() ((CSC==0x01 && CKC==0x38)?0:1)

/*
*******************************************************************************
**  Function define
*******************************************************************************
*/
void Clock_Init( INT32U Mode );
void CG_ReadResetSource( void );
/* Start user code for definition. Do not edit comment generated here */
MD_STATUS CG_SelectPowerSaveMode( enum PSLevel level );
/* End user code for definition. Do not edit comment generated here */
#endif
