/*
*******************************************************************************
**
**  This device driver was created by Applilet2 for 78K0R/Kx3
**  16-Bit Single-Chip Microcontrollers
**
**  Copyright(C) NEC Electronics Corporation 2002 - 2006
**  All rights reserved by NEC Electronics Corporation.
**
**  This program should be used on your own responsibility.
**  NEC Electronics Corporation assumes no responsibility for any losses
**  incurred by customers or third parties arising from the use of this file.
**
**  Filename :	System.c
**  Abstract :	This file implements device driver for System module.
**  APIlib :	Applilet2 for 78K0R/Kx3 V2.30 [18 Oct. 2007]
**
**  Device :	uPD78F1166_A0
**
**  Compiler :	IAR_C
**
**  Creation date:	2006-6-27
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
**	This function initializes the clock generator.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void Clock_Init(INT32U SysMode)
{
	ULONG i;
	
        Clr_Ext_Inter_Dog();
        
	UCHAR temp_stabset, temp_stabwait;
	OSTS = CG_OSCSTAB_SEL18;        //振荡稳定时间选择寄存器------------PUCK
	CMC = CG_HISYS_OSC | CG_SYSOSC_OVER10M | CG_SUB_OSC;
	MSTOP = 0;  //开启fx，等待稳定
	temp_stabset = CG_OSCSTAB_STA18;   //振荡稳定时间计数器------------PUCK
	do{
		temp_stabwait = OSTC;
		temp_stabwait &= temp_stabset;
	}while(temp_stabwait != temp_stabset);
              
        MCM0 = 1;   while (MCS == 0) Clr_Ext_Inter_Dog();  //主时钟系统振荡处于fih？//主系统时钟由 fih 切换至 外部 fMX------------PUCK
	        
	HIOSTOP = 1; //关闭fih，前，必须确认 MCS=1，或者CLS=1------------PUCK
	
	XTSTOP = 0; //开启fxt
	
	for(i = 0; i <= CG_SUBWAITTIME/10; i++){
		//NOP();
          Clr_Ext_Inter_Dog();
	}
	
	CSS = 0;

	CKC &= (UCHAR)~CG_CPUCLK;         //将分频系数归0------------PUCK
        if(SYS_NORMAL==SysMode)
	{
          CKC |= CG_CPUCLK_MAIN0;           //获取分频系数:fMAIN=晶振------------PUCK
          OSMC = CG_FCLK_OVER10M;           //10M以上频率
        }
        else
        {
          CKC |= RSUME_CLOCK_DIV;           //获取分频系数:fMAIN/n------------PUCK
          OSMC = CG_FCLK_UNDER10M;           //10M以下频率
        }	
}

void Clock_Init_1( void )
{
//	ULONG i;
	/* Set fMX */
//	UCHAR temp_stabset, temp_stabwait;
	MEM = 0x00;			/*  EXEN=0: single-chip mode (port function is valid.)
					   		EXWEN=0: external wait pin is not used
					   		EW1=EW0=0: CLKOUT=fclk
					  		MM3=0: multiplexed bus mode
					   		MM2=0: 8-bit bus mode
					   		MM1=MM0=0: 256-byte extension mode */
	/*****************************************
	 X1 clock oscillation stabilization wait time when STOP mode is released */
	OSTS = 0x07;			/* OSTS2=OSTS1=OSTS0=1: 2^18/fx */
	/****************************************
	 clock generator setting */
	CMC = 0X51;		/* EXCLK=0, OSCSEL=1: X1 oscillation mode
					   		   OSCSELS=1: XT1 oscillation mode
					   		   AMPH=1 (10MHz亙fmx亝20MHz) */
	CSC = 0X00;		/* MSTOP=0: X1 operating
								XTSTOP=0: XT1 operating
								HIOSTOP=0: internal high-speed oscillator operating */

	MSTOP = 0;
        while(OSTC != 0xff);		/* oscillation stabilization ? */
	MCM0 = 1;
	MCM0 = 1;				/* fCLK: fIH -> fMX */

	while (MCS == 0) {
	}						/* wait until MCS=1 (fCLK=fMX) */

	HIOTRM = 0x10;			/* TTRM4=TTRM3=TTRM2=TTRM1=TTRM0=0: clock correction value = 亇0% (default) */

	/****************************************
	 low-voltage detector */
	LVIM = 0x00;			/* LVION=0: disable operation
					   LVISEL=0: detects level of supply voltage (VDD)
					   LVIMD=0: generates an interrupt signal when VDD drops lower than VLVI
					   LVIF=0: LVI operation is disabled */

	LVIS = 0x0e;			/* LVIS3=LVIS2=LVIS1=1, LVIS0=0: 2.07亇0.1V */

	/*****************************************
	 flash memory */
	BECTL = 0x00;			/* FLMDPUP=0: selects pul-down */

	/*****************************************
	 enable or disable each peripheral hardware */
	PER0 = 0XFD;		/* RTCEN=DACEN=ADCEN=IIC0EN=SAU1EN=SAU0EN=TAU0EN=1: supplies input clock to RTC, DAC, ADC, IIC0, SAU0, SAU1 and TAU0 */

	PER1 = 0X00;		/* EXBEN=0: stops input clock supply to external bus interface */
	/* Set fCLK */
	CSS = 0;
	CKC = 0X08;		/* CLS=0: fCLK = main system clock (fMAIN)
					   MCS=0: fMAIN = internal high-speed oscillation clock (fIH)
					   CSS=MCM0=MDIV2=MDIV1=MDIV0=0: fCLK = fIH */
	OSMC = 0x01;			/* FSEL=1: operates at a frequency over higher than 10MHz */
}
/* Start adding user code. Do not edit comment generated here */

MD_STATUS CG_SelectPowerSaveMode( enum PSLevel level )
{
	switch( level ){
	case PSHALT:
		/* Halt mode */
		HALT( );
		break;
	case PSSTOP:
		/* STOP mode */
		if(CKC & CG_CPUCLK_SUB)
			return MD_ERROR;		/* sub clock mode */
		STOP( );
		break;
	default:
		return MD_ARGERROR;
	}
        return MD_OK;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function changes clock generator operation mode.
**
**  Parameters:
**	mode :	clock generator operation mode
**
**  Returns:
**	MD_OK
**	MD_ARGERROR
**	MD_ERROR
**
**-----------------------------------------------------------------------------
*/
MD_STATUS CG_ChangeClockMode( enum ClockMode mode )
{
	enum ClockMode oldmode;
	UCHAR temp_stabset, temp_stabwait;
	int i;

	if(CKC & CG_CPUCLK_SUB){
		oldmode = SUBCLK;
	}
	else{
		if(CKC & CG_MAINCLK_HISYS){
			if(CMC & CG_HISYS_EXTCLK){
				oldmode = SYSEXTCLK;
			}
			else{
				oldmode = SYSX1CLK;
			}
		}
		else{
			oldmode = HIOCLK;
		}
	}
	if(mode != oldmode){
		switch( mode ){
		case HIOCLK:
			if(CSC & CG_HIO_STOP){
				HIOSTOP = 0;
				/* Software wait 10us */
				for(i = 0; i <= CG_FIHWAITTIME; i++){
					NOP();
				}
			}
			MCM0 = 0;
			CSS = 0;
			break;
		case SYSX1CLK:
			if((oldmode == SYSEXTCLK) || ((CMC & CG_HISYS_PIN) != CG_HISYS_OSC)){
				return MD_ERROR;
			}
			if(CSC & CG_HISYS_STOP){
				MSTOP = 0;
				temp_stabset = (UCHAR)~(0x7f >> OSTS);
				do{
					temp_stabwait = OSTC;
					temp_stabwait &= temp_stabset;
				}while(temp_stabwait != temp_stabset);
			}
			MCM0 = 1;
			CSS = 0;
			break;
		case SYSEXTCLK:
			if((oldmode == SYSX1CLK) || ((CMC & CG_HISYS_PIN) != CG_HISYS_EXT)){
				return MD_ERROR;
			}
			if(CSC & CG_HISYS_STOP){
				MSTOP = 0;
			}
			MCM0 = 1;
			CSS = 0;
			break;
		case SUBCLK:
			if(!(CMC & CG_SUB_OSCSELS)){
				return MD_ERROR;
			}
			if(CSC & CG_SUB_STOP){
				XTSTOP = 0;
			}
			CSS = 1;
			break;
		default:
			return MD_ARGERROR;			/* input data error */
		}
	}
	return MD_OK;
}

/* End user code adding. Do not edit comment generated here */


