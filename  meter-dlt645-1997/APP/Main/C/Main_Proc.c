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
**  Filename :	 main.c
**  Abstract :	This file implements main function.
**  APIlib :	Applilet2 for 78K0R/Kx3 V2.30 [18 Oct. 2007]
**
**  Device :	uPD78F1166_A0
**
**  Compiler :	IAR_C
**
**  Creation date:	2008-5-22
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


/* Start user code for include definition. Do not edit comment generated here */
/* End user code for include definition. Do not edit comment generated here */

#define MAIN_C
#include "includes.h"
#include "TaskProc.h"
#include "Pub_PUCK.h"
/*
*******************************************************************************
**  Global define
*******************************************************************************
*/

/* Option bytes */
//0x6e:关闭看门狗；0x7e:开启看门狗(halt和stop除外)，溢出时间：4.369s；0x7f：看门狗在所有模式下都工作
#pragma location = "OPTBYTE"
__root const unsigned char opbyte0=0x7e; 

//LVI设置字：0xfe：LVI默认开启；0xff：LVI默认关闭
#pragma location = "OPTBYTE"
__root const unsigned char opbyte1=0xFE;

#pragma location = "OPTBYTE"
__root const unsigned char opbyte2=0xFF;
#pragma location = "OPTBYTE"
__root const unsigned char opbyte3=CG_OPTION;
/* Start user code for global definition. Do not edit comment generated here */
/* End user code for global definition. Do not edit comment generated here */


/********************************************************************************
函数原型：void main(void)
*********************************************************************************/
void  main( void )
{
  OS_Assert_Ctrl(0x00);//关闭所有任务断言

  OS_Init();
  Init_Hard_PUCK();
  
  OS_Create_Task(&Main_Task_PUB,&AppStartTaskStk_main[TASK_STK_SIZE_main-1],sizeof(AppStartTaskStk_main),"Main_Task_PUB");
  OS_Start();
}
