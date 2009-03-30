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
**  Filename :	LVI.h
**  Abstract :	This file implements device driver for LVI module.
**  APIlib :	Applilet2 for 78K0R/Kx3 V2.30 [18 Oct. 2007]
**
**  Device :	uPD78F1166_A0
**
**  Compiler :	IAR_C
**
**  Creation date:	2008-12-7
**  
*******************************************************************************
*/
#ifndef _MDLVI_
#define _MDLVI_
/*
*******************************************************************************
**	Register bit define
*******************************************************************************
*/
/*
	Low-voltage detection register (LVIM)
*/
/* Low-voltage detection operation (LVION) */
#define LVI_OPERATION			0x80
#define LVI_OPERATION_DISABLE	0x00	/* disable operation */
#define LVI_OPERATION_ENABLE	0x80	/* enable operation */
/* Voltage detection selection (LVISEL) */
#define LVI_SELECTION			0x04
#define LVI_SELECTION_VDD		0x00	/* detection voltage=VDD */  
#define LVI_SELECTION_EXLVI		0x04	/* detection voltage=EXLVI */
/* Operation mode selection (LVIMD) */
#define LVI_MODE				0x02
#define LVI_MODE_INT			0x00	/* generates interrupt signal */
#define LVI_MODE_RESET			0x02	/* generates reset signal */
/* Low-voltage detection flag (LVIF) */
#define LVI_FLAG				0x01
#define LVI_FLAG_OVER			0x00	/* voltage ?? detection voltage */
#define LVI_FLAG_LESS			0x01	/* voltage < detection voltage */
/*
	Low-voltage detection level selection register (LVIS)
*/
/* Detection level (LVIS0-3) */
#define LVI_LVIS_LEVEL			0x0f
#define LVI_LVIS_LEVEL_00		0x00	/*  4.22 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_01		0x01	/*  4.07 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_02		0x02	/*  3.92 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_03		0x03	/*  3.76 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_04		0x04	/*  3.61 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_05		0x05	/*  3.45 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_06		0x06	/*  3.30 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_07		0x07	/*  3.15 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_08		0x08	/*  2.99 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_09		0x09	/*  2.84 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_10		0x0a	/*  2.68 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_11		0x0b	/*  2.53 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_12		0x0c	/*  2.38 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_13		0x0d	/*  2.22 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_14		0x0e	/*  2.07 V +/- 0.1 V */
#define LVI_LVIS_LEVEL_15		0x0f	/*  1.91 V +/- 0.1 V */

/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
#define LVI_WAITTIME 100
enum LVILevel { 
	LVILEVEL0, LVILEVEL1, LVILEVEL2, LVILEVEL3, 
	LVILEVEL4, LVILEVEL5, LVILEVEL6, LVILEVEL7,
	LVILEVEL8, LVILevel9, LVILevel10, LVILevel11,
	LVILEVEL12, LVILEVEL13, LVILEVELl14,LVILEVEL15
	};

/*
*******************************************************************************
**  Function define
*******************************************************************************
*/
void LVI_Init( void );
MD_STATUS LVI_Start( void );
void LVI_Stop( void );
/* Start user code for definition. Do not edit comment generated here */
/* End user code for definition. Do not edit comment generated here */
#endif
