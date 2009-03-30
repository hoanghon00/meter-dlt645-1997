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
**  Filename :	macrodriver.h
**  Abstract :	This file implements general head file.
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
#ifndef _MDSTATUS_
#define _MDSTATUS_
/*
*******************************************************************************
**	Register bit define
*******************************************************************************
*/

/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
	/* device header file */
#include "io78f1166_a0.h"
#include "io78f1166_a0_ext.h"
#include "intrinsics.h"

#define	CG_OPTION 0x84


#define DI		__disable_interrupt
#define EI		__enable_interrupt
#define HALT	__halt
#define NOP		__no_operation
#define STOP	__stop


/* data type defintion */
typedef	unsigned long	ULONG;
typedef	unsigned int	UINT;
typedef	unsigned short	USHORT;
typedef	unsigned char	UCHAR;
typedef	unsigned char	BOOL;

#define	ON		1
#define	OFF		0

#define	TRUE	1
#define	FALSE	0

#define IDLE	0			/* idle status */
#define READ	1			/* read mode */
#define WRITE	2			/* write mode */

#define SET		1
#define CLEAR	0

#define MD_STATUS		unsigned short
#define MD_STATUSBASE	0x0

/* status list definition */
#define MD_OK				MD_STATUSBASE + 0x0		/* register setting OK */
#define MD_RESET			MD_STATUSBASE + 0x1		/* reset input */
#define MD_SENDCOMPLETE		MD_STATUSBASE + 0x2		/* send data complete */
#define MD_OVF				MD_STATUSBASE + 0x3		/* timer count overflow */
#define MD_SPT				MD_STATUSBASE + 0x4		/*IIC stop*/
#define MD_NACK				MD_STATUSBASE + 0x5		/*IIC no ACK*/
#define MD_SLAVE_SEND_END	MD_STATUSBASE + 0x6		/*IIC slave send end*/
#define MD_SLAVE_RCV_END	MD_STATUSBASE + 0x7		/*IIC slave receive end*/
#define MD_MASTER_SEND_END	MD_STATUSBASE + 0x8		/*IIC master send end*/
#define MD_MASTER_RCV_END	MD_STATUSBASE + 0x9		/*IIC master receive end*/
#define MD_BUSY				MD_STATUSBASE + 0xa		/*IIC is busy*/
#define MD_UNDEREXEC		MD_STATUSBASE + 0xb		/* DMA transfer under execute */
#define MD_COMPLETED		MD_STATUSBASE + 0xc		/* DMA transfer completed */

/* error list definition */
#define MD_ERRORBASE		0x80
#define MD_ERROR			MD_ERRORBASE + 0x0		/* error */
#define MD_RESOURCEERROR	MD_ERRORBASE + 0x1		/* no resource available */
#define MD_PARITYERROR		MD_ERRORBASE + 0x2		/* UARTn parity error */
#define MD_OVERRUNERROR		MD_ERRORBASE + 0x3		/* UARTn overrun error */
#define MD_FRAMEERROR		MD_ERRORBASE + 0x4		/* UARTn frame error */
#define MD_ARGERROR			MD_ERRORBASE + 0x5		/* Error agrument input error */
#define MD_TIMINGERROR		MD_ERRORBASE + 0x6		/* Error timing operation error */
#define MD_SETPROHIBITED	MD_ERRORBASE + 0x7		/* setting prohibited */
#define MD_DATAEXISTS		MD_ERRORBASE + 0x8		/* Data to be transferred next exists in TXBn register */

enum INTLevel { HIGHEST, LEVEL1, LEVEL2, LOWEST };

#define	RAMAddr_START	0xcf00
#define	RAMAddr_END		0xfedf

/*
*******************************************************************************
**  Function define
*******************************************************************************
*/
#endif
