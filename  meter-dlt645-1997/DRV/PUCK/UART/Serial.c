#define SEIRAL_C
#include "Pub_PUCK.h"

#pragma pack(1)
typedef struct
{ 
  INT16U  FclkDiv;
  INT8U   Divisor;
  INT32U  Baud;
}CONST_UART_SET;

const CONST_UART_SET Const_Uart_PUCK[]={
  {0,0,0},
  {SAU_CK00_FCLK_10,17,300},
  {SAU_CK00_FCLK_9,17,600},
  {SAU_CK00_FCLK_8,17,1200},
  {SAU_CK00_FCLK_7,17,2400},
  {SAU_CK00_FCLK_6,17,4800},
  {SAU_CK00_FCLK_5,17,9600},
  {SAU_CK00_FCLK_4,17,19200},
  {SAU_CK00_FCLK_3,17,38400},
  {SAU_CK00_FCLK_3,11,57600},
  {SAU_CK00_FCLK_2,11,115200}  
};
#pragma pack()

#define MAX_BUAD_NUM (sizeof(Const_Uart_PUCK)/sizeof(CONST_UART_SET))


INT8U Get_Baud_Pos(INT32U Baud)
{
  INT8U i;
  for(i=0;i<MAX_BUAD_NUM;i++)
  {
    if(Baud==Const_Uart_PUCK[i].Baud)
      return i;
    
  }
  return 0;  
}

/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void SAU0_PowerOff( void )
{
	SAU0EN = 0;	/* SAU0 clock stop */
}

/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void SAU1_PowerOff( void )
{
	SAU1EN = 0;	/* SAU1 clock stop */
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void SAU0_Init( void )
{
	SAU0EN = 1;	/* SAU0 clock supply */
	WAITFOR_DRV_CYCLE_TIMEOUT(4);
	SPS0 = SAU_CK00_FCLK_2 | SAU_CK01_FCLK_8;//SAU_CK01_FCLK_2=156250
        
	UART0_Init( );
#ifdef UART1
	UART1_Init();
#endif
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART0_Init( void )
{
    /* UART0 initial setting */
    ST0 |= SAU_CH1_STOP_TRG_ON;	/* UART0 receive disable */
    ST0 |= SAU_CH0_STOP_TRG_ON;	/* UART0 transmit disable */
    SRMK0 = 1;		/* INTSR0 disable */
    SREMK0 = 1;		/* INTSRE0 disable */
    STMK0 = 1;		/* INTST0 disable */
    SRIF0 = 0;		/* INTSR0 IF clear */
    SREIF0 = 0;		/* INTSRE0 IF clear */
    STIF0 = 0;		/* INTST0 IF clear */
    SIR01 = SAU_SIRMN_FECTMN | SAU_SIRMN_PECTMN | SAU_SIRMN_OVCTMN;
    NFEN0 |= SAU_RXD0_FILTER_ON;
    SMR01 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK01 | SAU_TRIGGER_RXD | SAU_UART | SAU_TRANSFER_END;
    SCR01 = SAU_RECEPTION | SAU_INTSRE_ENABLE | SAU_PARITY_EVEN | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
    SDR01 = UART0_RECEIVE_DIVISOR;
    SOL0 |= SAU_CHANNEL0_NORMAL;
    SMR00 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK01 | SAU_TRIGGER_SOFTWARE | SAU_UART | SAU_TRANSFER_END;
    SCR00 = SAU_TRANSMISSION | SAU_INTSRE_MASK | SAU_PARITY_EVEN | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
    SDR00 = UART0_TRANSMIT_DIVISOR;
    SOL0 |= SAU_CHANNEL0_NORMAL;
    SO0 |= SAU_CH0_DATA_OUTPUT_1;	/* output level */
    SOE0 |= SAU_CH0_OUTPUT_ENABLE;	/* UART0 output enable */
    /* INTSR0 priority low */
    SRPR10 = 1;
    SRPR00 = 1;
    /* INTSRE0 priority low */
    SREPR10 = 1;
    SREPR00 = 1;
    /* INTST0 priority low */
    STPR10 = 1;
    STPR00 = 1;
    /* RXD0 pin setting */
    PM1 |= 0x02;
    /* TxD0 pin setting */
    P1 |= 0x04;
    PM1 &= ~0x04;
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART0_Init_sub_suck( INT16U divisor,INT16U CheckBit)
{
    /* UART0 initial setting */
    ST0 |= SAU_CH1_STOP_TRG_ON;	/* UART0 receive disable */
    ST0 |= SAU_CH0_STOP_TRG_ON;	/* UART0 transmit disable */
    SRMK0 = 1;		/* INTSR0 disable */
    SREMK0 = 1;		/* INTSRE0 disable */
    STMK0 = 1;		/* INTST0 disable */
    SRIF0 = 0;		/* INTSR0 IF clear */
    SREIF0 = 0;		/* INTSRE0 IF clear */
    STIF0 = 0;		/* INTST0 IF clear */
    SIR01 = SAU_SIRMN_FECTMN | SAU_SIRMN_PECTMN | SAU_SIRMN_OVCTMN;
    NFEN0 |= SAU_RXD0_FILTER_ON;
    SMR01 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK01 | SAU_TRIGGER_RXD | SAU_UART | SAU_TRANSFER_END;
    SCR01 = SAU_RECEPTION | SAU_INTSRE_ENABLE | SAU_PARITY_NONE | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
    SDR01 = divisor<<9;
    SOL0 |= SAU_CHANNEL0_NORMAL;
    SMR00 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK01 | SAU_TRIGGER_SOFTWARE | SAU_UART | SAU_TRANSFER_END;
    SCR00 = SAU_TRANSMISSION | SAU_INTSRE_MASK | CheckBit | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
    SDR00 = divisor<<9;
    SOL0 |= SAU_CHANNEL0_NORMAL;
    SO0 |= SAU_CH0_DATA_OUTPUT_1;	/* output level */
    SOE0 |= SAU_CH0_OUTPUT_ENABLE;	/* UART0 output enable */
    /* INTSR0 priority low */
    SRPR10 = 1;
    SRPR00 = 1;
    /* INTSRE0 priority low */
    SREPR10 = 1;
    SREPR00 = 1;
    /* INTST0 priority low */
    STPR10 = 1;
    STPR00 = 1;
    /* RXD0 pin setting */
    PM1 |= 0x02;
    /* TxD0 pin setting */
    P1 |= 0x04;
    PM1 &= ~0x04;
}

/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART0_Start( void )
{
	SRMK0 = 0;			/* INTSR0 enable */
	SREMK0 = 0;			/* INTSRE0 enable */
	STMK0 = 1;			/* INTST0 enable */
	SS0 |= SAU_CH1_START_TRG_ON;	/* UART0 receive enable */
	SS0 |= SAU_CH0_START_TRG_ON;	/* UART0 transmit enable */
}

/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART0_Stop( void )
{
	ST0 |= SAU_CH1_STOP_TRG_ON;	/* UART0 receive disable */
	ST0 |= SAU_CH0_STOP_TRG_ON;	/* UART0 transmit disable */
	SRMK0 = 1;		/* INTSR0 disable */
	SREMK0 = 1;		/* INTSRE0 disable */
	STMK0 = 1;		/* INTST0 disable */
	SRIF0 = 0;		/* INTSR0 IF clear */
	SREIF0 = 0;		/* INTSRE0 IF clear */
	STIF0 = 0;		/* INTST0 IF clear */
        //SAU0_PowerOff();       //关闭串口0时钟-------PUCK
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
MD_STATUS UART0_SendData( UCHAR* txbuf, USHORT txnum )
{
	MD_STATUS status = MD_OK;
	
	if( txnum < 1 ){
		status = MD_ARGERROR;
	}
        else 
        {
          for(;txnum>0;)
          {                
            TXD0 = *txbuf;
            txbuf++;
            txnum--; 
            if(txnum%15==0)
              Clr_Ext_Inter_Dog();
          while((SSR00 & SAU_DATA_STORED));         
          }        
        }
	while((SSR00 & SAU_UNDER_EXECUTE));
	return (status);
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART1_Init( void )
{
	/* UART1 initial setting */
	ST0 |= SAU_CH3_STOP_TRG_ON;	/* UART1 receive disable */
	ST0 |= SAU_CH2_STOP_TRG_ON;	/* UART1 transmit disable */
	SRMK1 = 1;		/* INTSR1 disable */
	SREMK1 = 1;		/* INTSRE1 disable */
	STMK1 = 1;		/* INTST1 disable */
	SRIF1 = 0;		/* INTSR1 IF clear */
	SREIF1 = 0;		/* INTSRE1 IF clear */
	STIF1 = 0;		/* INTST1 IF clear */
	SIR03 = SAU_SIRMN_FECTMN | SAU_SIRMN_PECTMN | SAU_SIRMN_OVCTMN;
	NFEN0 |= SAU_RXD1_FILTER_ON;
	SMR03 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK00 | SAU_TRIGGER_RXD | SAU_UART | SAU_TRANSFER_END;
	SCR03 = SAU_RECEPTION | SAU_INTSRE_ENABLE | SAU_PARITY_EVEN | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
	SDR03 = UART1_RECEIVE_DIVISOR;
	SOL0 |= SAU_CHANNEL1_NORMAL;
	SMR02 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK00 | SAU_TRIGGER_SOFTWARE | SAU_UART | SAU_TRANSFER_END;
	SCR02 = SAU_TRANSMISSION | SAU_INTSRE_MASK | SAU_PARITY_EVEN | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
	SDR02 = UART1_TRANSMIT_DIVISOR;
	SOL0 |= SAU_CHANNEL1_NORMAL;
	SO0 |= SAU_CH2_DATA_OUTPUT_1;	/* output level */
	SOE0 |= SAU_CH2_OUTPUT_ENABLE;	/* UART1 output enable */
	/* INTSR1 priority low */
	SRPR11 = 1;
	SRPR01 = 1;
	/* INTSRE1 priority low */
	SREPR11 = 1;
	SREPR01 = 1;
	/* INTST1 priority low */
	STPR11 = 1;
	STPR01 = 1;
	/* RXD1 pin setting */
	PM0 |= 0x08;
	/* TxD1 pin setting */
	P0 |= 0x04;
	PM0 &= ~0x04;
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART1_Init_sub_suck( INT16U divisor,INT16U CheckBit)
{
	/* UART1 initial setting */
	ST0 |= SAU_CH3_STOP_TRG_ON;	/* UART1 receive disable */
	ST0 |= SAU_CH2_STOP_TRG_ON;	/* UART1 transmit disable */
	SRMK1 = 1;		/* INTSR1 disable */
	SREMK1 = 1;		/* INTSRE1 disable */
	STMK1 = 1;		/* INTST1 disable */
	SRIF1 = 0;		/* INTSR1 IF clear */
	SREIF1 = 0;		/* INTSRE1 IF clear */
	STIF1 = 0;		/* INTST1 IF clear */
	SIR03 = SAU_SIRMN_FECTMN | SAU_SIRMN_PECTMN | SAU_SIRMN_OVCTMN;
	NFEN0 |= SAU_RXD1_FILTER_ON;
	SMR03 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK00 | SAU_TRIGGER_RXD | SAU_UART | SAU_TRANSFER_END;
	SCR03 = SAU_RECEPTION | SAU_INTSRE_ENABLE | SAU_PARITY_EVEN | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
	SDR03 = divisor<<9;
	SOL0 |= SAU_CHANNEL1_NORMAL;
	SMR02 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK00 | SAU_TRIGGER_SOFTWARE | SAU_UART | SAU_TRANSFER_END;
	SCR02 = SAU_TRANSMISSION | SAU_INTSRE_MASK | SAU_PARITY_EVEN | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
	SDR02 = divisor<<9;
	SOL0 |= SAU_CHANNEL1_NORMAL;
	SO0 |= SAU_CH2_DATA_OUTPUT_1;	/* output level */
	SOE0 |= SAU_CH2_OUTPUT_ENABLE;	/* UART1 output enable */
	/* INTSR1 priority low */
	SRPR11 = 1;
	SRPR01 = 1;
	/* INTSRE1 priority low */
	SREPR11 = 1;
	SREPR01 = 1;
	/* INTST1 priority low */
	STPR11 = 1;
	STPR01 = 1;
	/* RXD1 pin setting */
	PM0 |= 0x08;
	/* TxD1 pin setting */
	P0 |= 0x04;
	PM0 &= ~0x04;
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART1_Start( void )
{
	SRMK1 = 0;			/* INTSR1 enable */
	SREMK1 = 0;			/* INTSRE1 enable */
	STMK1 = 0;			/* INTST1 enable */
	SS0 |= SAU_CH3_START_TRG_ON;	/* UART1 receive enable */
	SS0 |= SAU_CH2_START_TRG_ON;	/* UART1 transmit enable */
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART1_Stop( void )
{
	ST0 |= SAU_CH3_STOP_TRG_ON;	/* UART1 receive disable */
	ST0 |= SAU_CH2_STOP_TRG_ON;	/* UART1 transmit disable */
	SRMK1 = 1;		/* INTSR1 disable */
	SREMK1 = 1;		/* INTSRE1 disable */
	STMK1 = 1;		/* INTST1 disable */
	SRIF1 = 0;		/* INTSR1 IF clear */
	SREIF1 = 0;		/* INTSRE1 IF clear */
	STIF1 = 0;		/* INTST1 IF clear */
        //SAU0_PowerOff();       //关闭串口0时钟-------PUCK
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
MD_STATUS UART1_SendData( UCHAR* txbuf, USHORT txnum )
{
	MD_STATUS status = MD_OK;
	
	if( txnum < 1 ){
		status = MD_ARGERROR;
	}
        else 
        {
          for(;txnum>0;)
          {                
            TXD1=*txbuf;
            txbuf++;
            txnum--; 
            if(txnum%15==0)
              Clr_Ext_Inter_Dog();
          while((SSR02 & SAU_DATA_STORED));         
          }        
        }
	while((SSR02 & SAU_UNDER_EXECUTE));
	return (status);
}

/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void SAU1_Init( void )
{
    SAU1EN = 1;	/* SAU1 clock supply */
    WAITFOR_DRV_CYCLE_TIMEOUT(4);
    SPS1 = SAU_CK00_FCLK_11 | SAU_CK01_FCLK_11;
    
    UART2_Init( );
    UART3_Init( );
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART2_Init( void )
{
    /* UART2 initial setting */
    ST1 |= SAU_CH1_STOP_TRG_ON;	/* UART2 receive disable */
    ST1 |= SAU_CH0_STOP_TRG_ON;	/* UART2 transmit disable */
    SRMK2 = 1;		/* INTSR2 disable */
    SREMK2 = 1;		/* INTSRE2 disable */
    STMK2 = 1;		/* INTST2 disable */
    SRIF2 = 0;		/* INTSR2 IF clear */
    SREIF2 = 0;		/* INTSRE2 IF clear */
    STIF2 = 0;		/* INTST2 IF clear */
    SIR11 = SAU_SIRMN_FECTMN | SAU_SIRMN_PECTMN | SAU_SIRMN_OVCTMN;
    NFEN0 |= SAU_RXD2_FILTER_ON;
    SMR11 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK00 | SAU_TRIGGER_RXD | SAU_UART | SAU_TRANSFER_END;
    SCR11 = SAU_RECEPTION | SAU_INTSRE_ENABLE | SAU_PARITY_EVEN | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
    SDR11 = UART2_RECEIVE_DIVISOR;
    SOL1 |= SAU_CHANNEL2_NORMAL;
    SMR10 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK00 | SAU_TRIGGER_SOFTWARE | SAU_UART | SAU_TRANSFER_END;
    SCR10 = SAU_TRANSMISSION | SAU_INTSRE_MASK | SAU_PARITY_EVEN | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
    SDR10 = UART2_TRANSMIT_DIVISOR;
    SOL1 |= SAU_CHANNEL2_NORMAL;
    SO1 |= SAU_CH0_DATA_OUTPUT_1;	/* output level */
    SOE1 |= SAU_CH0_OUTPUT_ENABLE;	/* UART2 output enable */
            /* INTSR2 priority low */
    SRPR12 = 1;
    SRPR02 = 1;
    /* INTSRE2 priority low */
    SREPR12 = 1;
    SREPR02 = 1;
    /* INTST2 priority low */
    STPR12 = 1;
    STPR02 = 1;
    /* RXD2 pin setting */
    PM14 |= 0x08;
    /* TxD2 pin setting */
    P14 |= 0x10;
    PM14 &= ~0x10;
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART2_Start( void )
{
	SRMK2 = 0;			/* INTSR2 enable */
	SREMK2 = 0;			/* INTSRE2 enable */
	STMK2 = 1;			/* INTST2 enable */
	SS1 |= SAU_CH1_START_TRG_ON;	/* UART2 receive enable */
	SS1 |= SAU_CH0_START_TRG_ON;	/* UART2 transmit enable */
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART2_Stop( void )
{
	ST1 |= SAU_CH1_STOP_TRG_ON;	/* UART2 receive disable */
	ST1 |= SAU_CH0_STOP_TRG_ON;	/* UART2 transmit disable */
	SRMK2 = 1;		/* INTSR2 disable */
	SREMK2 = 1;		/* INTSRE2 disable */
	STMK2 = 1;		/* INTST2 disable */
	SRIF2 = 0;		/* INTSR2 IF clear */
	SREIF2 = 0;		/* INTSRE2 IF clear */
	STIF2 = 0;		/* INTST2 IF clear */
        //SAU1_PowerOff();       //关闭串口1时钟-------PUCK
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
MD_STATUS UART2_SendData( UCHAR* txbuf, USHORT txnum )
{
	MD_STATUS status = MD_OK;
	
	if( txnum < 1 ){
		status = MD_ARGERROR;
	}
	else {
		//gpUart2TxAddress = txbuf;
		//gUart2TxCnt = txnum;
		for(;txnum>0;)
		{
		//DI();
		TXD2 = *txbuf;
		txbuf++;
		txnum--;
                if(txnum%15==0)
                  Clr_Ext_Inter_Dog();
		//EI();
		while((SSR10 & SAU_DATA_STORED));
	}
	}
	while((SSR10 & SAU_UNDER_EXECUTE)); 
	
	return (status);
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART3_Init( void )
{
    /* UART3 initial setting */
    ST1 |= SAU_CH3_STOP_TRG_ON;	/* UART3 receive disable */
    ST1 |= SAU_CH2_STOP_TRG_ON;	/* UART3 transmit disable */
    SRMK3 = 1;		/* INTSR3 disable */
    SREMK3 = 1;		/* INTSRE3 disable */
    STMK3 = 1;		/* INTST3 disable */
    SRIF3 = 0;		/* INTSR3 IF clear */
    SREIF3 = 0;		/* INTSRE3 IF clear */
    STIF3 = 0;		/* INTST3 IF clear */
    SIR13 = SAU_SIRMN_FECTMN | SAU_SIRMN_PECTMN | SAU_SIRMN_OVCTMN;
    NFEN0 |= SAU_RXD3_FILTER_ON;
    SMR13 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK00 | SAU_TRIGGER_RXD | SAU_UART | SAU_TRANSFER_END;
    SCR13 = SAU_RECEPTION | SAU_INTSRE_ENABLE | SAU_PARITY_NONE | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
    SDR13 = UART3_RECEIVE_DIVISOR;
    SOL1 |= SAU_CHANNEL3_NORMAL;
    SMR12 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK00 | SAU_TRIGGER_SOFTWARE | SAU_UART | SAU_TRANSFER_END;
    SCR12 = SAU_TRANSMISSION | SAU_INTSRE_MASK | SAU_PARITY_EVEN | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
    SDR12 = UART3_TRANSMIT_DIVISOR;
    SOL1 |= SAU_CHANNEL3_NORMAL;
    SO1 |= SAU_CH2_DATA_OUTPUT_1;	/* output level */
    SOE1 |= SAU_CH2_OUTPUT_ENABLE;	/* UART3 output enable */
    /* INTSR3 priority low */
    SRPR13 = 1;
    SRPR03 = 1;
    /* INTSRE3 priority low */
    SREPR13 = 1;
    SREPR03 = 1;
    /* INTST3 priority low */
    STPR13 = 1;
    STPR03 = 1;
    /* RXD3 pin setting */
    P1 |= 0x10;
    /* TxD3 pin setting */
    P1 |= 0x08;
    PM1 &= ~0x08;
}/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART3_Start( void )
{
    SRMK3 = 0;			/* INTSR3 enable */
    SREMK3 = 0;			/* INTSRE3 enable */
    STMK3 = 1;			/* INTST3 enable */
    SS1 |= SAU_CH3_START_TRG_ON;	/* UART3 receive enable */
    SS1 |= SAU_CH2_START_TRG_ON;	/* UART3 transmit enable */
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART3_Stop( void )
{
    ST1 |= SAU_CH3_STOP_TRG_ON;	/* UART3 receive disable */
    ST1 |= SAU_CH2_STOP_TRG_ON;	/* UART3 transmit disable */
    SRMK3 = 1;		/* INTSR3 disable */
    SREMK3 = 1;		/* INTSRE3 disable */
    STMK3 = 1;		/* INTST3 disable */
    SRIF3 = 0;		/* INTSR3 IF clear */
    SREIF3 = 0;		/* INTSRE3 IF clear */
    STIF3 = 0;		/* INTST3 IF clear */
    //SAU1_PowerOff();       //关闭串口1时钟-------PUCK
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
MD_STATUS UART3_SendData( UCHAR* txbuf, USHORT txnum )
{
    MD_STATUS status = MD_OK;
    
    if( txnum < 1 ){
      status = MD_ARGERROR;
    }
    else {	
      for(;txnum>0;)
      {               
        //DI();  
        TXD3 = *txbuf;
        txbuf++;
        txnum--;
        if(txnum%15==0)
          Clr_Ext_Inter_Dog();
        //EI();
      while((SSR12 & SAU_DATA_STORED));
      }
    }        
    while((SSR12 & SAU_UNDER_EXECUTE));     
    
    return (status);
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/

/* Start adding user code. Do not edit comment generated here */
UART2_Init_sub_suck(INT8U divisor,INT16U CheckBit)
{
  /* UART2 initial setting */
    ST1 |= SAU_CH1_STOP_TRG_ON;	/* UART2 receive disable */
    ST1 |= SAU_CH0_STOP_TRG_ON;	/* UART2 transmit disable */
    SRMK2 = 1;		/* INTSR2 disable */
    SREMK2 = 1;		/* INTSRE2 disable */
    STMK2 = 1;		/* INTST2 disable */
    SRIF2 = 0;		/* INTSR2 IF clear */
    SREIF2 = 0;		/* INTSRE2 IF clear */
    STIF2 = 0;		/* INTST2 IF clear */
    SIR11 = SAU_SIRMN_FECTMN | SAU_SIRMN_PECTMN | SAU_SIRMN_OVCTMN;
    NFEN0 |= SAU_RXD2_FILTER_ON;
    SMR11 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK00 | SAU_TRIGGER_RXD | SAU_UART | SAU_TRANSFER_END;
    SCR11 = SAU_RECEPTION | SAU_INTSRE_ENABLE | SAU_PARITY_EVEN | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
    SDR11 = divisor<<9;
    SOL1 |= SAU_CHANNEL2_NORMAL;
    SMR10 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK00 | SAU_TRIGGER_SOFTWARE | SAU_UART | SAU_TRANSFER_END;
    SCR10 = SAU_TRANSMISSION | SAU_INTSRE_MASK | CheckBit | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
    SDR10 = divisor<<9;
    SOL1 |= SAU_CHANNEL2_NORMAL;
    SO1 |= SAU_CH0_DATA_OUTPUT_1;	/* output level */
    SOE1 |= SAU_CH0_OUTPUT_ENABLE;	/* UART2 output enable */
    /* INTSR2 priority low */
    SRPR12 = 1;
    SRPR02 = 1;
    /* INTSRE2 priority low */
    SREPR12 = 1;
    SREPR02 = 1;
    /* INTST2 priority low */
    STPR12 = 1;
    STPR02 = 1;
    /* RXD2 pin setting */
    PM14 |= 0x08;
    /* TxD2 pin setting */
    P14 |= 0x10;
    PM14 &= ~0x10;
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
UART3_Init_sub_suck(INT8U divisor,INT16U CheckBit)
{
    /* UART3 initial setting */
    ST1 |= SAU_CH3_STOP_TRG_ON;	/* UART3 receive disable */
    ST1 |= SAU_CH2_STOP_TRG_ON;	/* UART3 transmit disable */
    SRMK3 = 1;		/* INTSR3 disable */
    SREMK3 = 1;		/* INTSRE3 disable */
    STMK3 = 1;		/* INTST3 disable */
    SRIF3 = 0;		/* INTSR3 IF clear */
    SREIF3 = 0;		/* INTSRE3 IF clear */
    STIF3 = 0;		/* INTST3 IF clear */
    SIR13 = SAU_SIRMN_FECTMN | SAU_SIRMN_PECTMN | SAU_SIRMN_OVCTMN;
    NFEN0 |= SAU_RXD3_FILTER_ON;
    SMR13 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK01 | SAU_TRIGGER_RXD | SAU_UART | SAU_TRANSFER_END;
    SCR13 = SAU_RECEPTION | SAU_INTSRE_ENABLE | SAU_PARITY_EVEN | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
    //SDR13 = UART3_RECEIVE_DIVISOR_SUCK;
    SDR13 = divisor<<9;
    SOL1 |= SAU_CHANNEL3_NORMAL;
    SMR12 = SAU_SMRMN_INITIALVALUE | SAU_CLOCK_SELECT_CK01 | SAU_TRIGGER_SOFTWARE | SAU_UART | SAU_TRANSFER_END;
    SCR12 = SAU_TRANSMISSION | SAU_INTSRE_MASK | CheckBit | SAU_LSB | SAU_STOP_1 | SAU_LENGTH_8;
    SDR12 = divisor<<9;
    SOL1 |= SAU_CHANNEL3_NORMAL;
    SO1 |= SAU_CH2_DATA_OUTPUT_1;	/* output level */
    SOE1 |= SAU_CH2_OUTPUT_ENABLE;	/* UART3 output enable */
    /* INTSR3 priority low */
    SRPR13 = 1;
    SRPR03 = 1;
    /* INTSRE3 priority low */
    SREPR13 = 1;
    SREPR03 = 1;
    /* INTST3 priority low */
    STPR13 = 1;
    STPR03 = 1;
    /* RXD3 pin setting */
    P1 |= 0x10;
    /* TxD3 pin setting */
    P1 |= 0x08;
    PM1 &= ~0x08;
 }

/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART2_Init_suck(ULONG baud_rate,INT16U CheckBit)
{
  INT8U Pos;  
  Pos=Get_Baud_Pos(baud_rate);
  if(0==Pos)
    return ;  
  UART2_Stop( ); 
  B_1_485_1;
  SAU1EN = 1;	/* SAU1 clock supply */
  WAITFOR_DRV_CYCLE_TIMEOUT(4);
  SPS1=Const_Uart_PUCK[Pos].FclkDiv|(SPS1&0xf0); //CK10提供给UART2,如115200，第一次分频：11059200/2^2=2764800，第二次分频：2764800/(2n+2)=2764800/24=115200
  UART2_Init_sub_suck(Const_Uart_PUCK[Pos].Divisor,CheckBit);
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART3_Init_suck(ULONG baud_rate,INT16U CheckBit)
{
  INT8U Pos;  
  Pos=Get_Baud_Pos(baud_rate);
  if(0==Pos)
    return ;  
 
  UART3_Stop( );
  B_2_485_1;
 
  SAU1EN = 1;	/* SAU1 clock supply */
  WAITFOR_DRV_CYCLE_TIMEOUT(4);
  SPS1=(Const_Uart_PUCK[Pos].FclkDiv<<4)|(SPS1&0x0f);   //CK11提供给UART2和UART3；
  UART3_Init_sub_suck(Const_Uart_PUCK[Pos].Divisor,CheckBit);  
}

/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
void UART0_Init_suck(ULONG baud_rate,INT16U CheckBit)
{
  INT8U Pos; 
  INT8U temp;
  
  Pos=Get_Baud_Pos(baud_rate);
  if(0==Pos)
    return ;  
  
  UART0_Stop( );
  SAU0EN = 1;
  WAITFOR_DRV_CYCLE_TIMEOUT(4);
  if(Get_Main_Clock_Div()==0)
    temp=Const_Uart_PUCK[Pos].FclkDiv;
  else
    temp=Const_Uart_PUCK[Pos].FclkDiv-1;
  
  SPS0 = (SPS0&0x0f)|(temp<<4); //CK01提供给UART0
  UART0_Init_sub_suck(Const_Uart_PUCK[Pos].Divisor,CheckBit);  
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
INT8U UART0_SendData_suck(UCHAR* txbuf, USHORT txnum)
{
    UART0_SendData(txbuf, txnum );
    return SUCESS;
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
INT8U UART0_RecData_pre_suck(void)
{
  Uart0_Rx_Count=0;//清除计数标志
  memset(Uart0_RecBuf,0x00,sizeof(Uart0_RecBuf));
  return 1;
}

/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/

void UART1_Init_suck(ULONG baud_rate,INT16U CheckBit)
{
  INT8U Pos;  
  Pos=Get_Baud_Pos(baud_rate);
  if(0==Pos)
    return ;  
  
  UART1_Stop( );
  SAU0EN = 1;
  WAITFOR_DRV_CYCLE_TIMEOUT(4);
  SPS0 = (SPS0&0xf0)|Const_Uart_PUCK[Pos].FclkDiv;        //CK00提供给UART0和UART1；
  UART1_Init_sub_suck(Const_Uart_PUCK[Pos].Divisor,CheckBit);  
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
INT8U UART1_SendData_suck(UCHAR* txbuf, USHORT txnum)
{
    UART1_SendData(txbuf, txnum );    
    return SUCESS;

}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
INT8U UART1_RecData_pre_suck(void)
{
#ifdef UART1
  Uart1_Rx_Count=0;//清除计数标志
  memset(Uart1_RecBuf,0x00,sizeof(Uart1_RecBuf));
#endif
  return 1;
}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
INT8U UART2_SendData_suck(UCHAR* txbuf, USHORT txnum)
{
    //UART2_send_finish_flag =0;
    B_1_485_0;
    UART2_SendData(txbuf, txnum );    
    //while (!UART2_send_finish_flag ){}; 
    B_1_485_1;//
    
    return SUCESS;

}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
INT8U UART2_RecData_pre_suck(void)
{

  Uart2_Rx_Count=0;//清除计数标志
  memset(Uart2_RecBuf,0x00,sizeof(Uart2_RecBuf));
  return 1;
}

/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
INT8U UART3_SendData_suck(UCHAR* txbuf, USHORT txnum)
{    
     B_2_485_0;
    UART3_SendData(txbuf, txnum );    
    //while (!UART3_send_finish_flag ){}; 
     B_2_485_1;
     
    return SUCESS;

}
/********************************************************************************
PUCK:
入口：
返回：
********************************************************************************/
INT8U UART3_RecData_pre_suck(void)
{
  
  Uart3_Rx_Count=0;//清除计数标志
  memset(Uart3_RecBuf,0x00,sizeof(Uart3_RecBuf));
  return 1;
}



