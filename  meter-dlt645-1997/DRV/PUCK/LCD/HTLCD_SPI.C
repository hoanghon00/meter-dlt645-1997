
#include "Pub_PUCK.h"


/**********************************************************************************
函数功能：初始化LCDIO口
入口：    无
出口：    无
**********************************************************************************/
void InitHTLCDIOPort(void)
{
  /*
  GPIO_InitTypeDef  GPIO0_InitStructure;
  GPIO_InitTypeDef  GPIO4_InitStructure;
  GPIO_InitTypeDef  GPIO5_InitStructure;
 
  //P0x
  CFG_PeripheralClockConfig(CFG_CLK_GPIO0, ENABLE);   
  GPIO0_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO0_InitStructure.GPIO_Pins = TO_LCD_PWR | TO_LCD_LED;
  GPIO_Init(GPIO0, &GPIO0_InitStructure);
  
  //P4x
  CFG_PeripheralClockConfig(CFG_CLK_GPIO4, ENABLE);  
  GPIO4_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO4_InitStructure.GPIO_Pins = TO_LCD_CS|TO_LCD_CLK;  
  GPIO_Init(GPIO4, &GPIO4_InitStructure);
  
  
  //P5x
  CFG_PeripheralClockConfig(CFG_CLK_GPIO5, ENABLE);  
  GPIO5_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO5_InitStructure.GPIO_Pins = TO_LCD_DATA;  
  GPIO_Init(GPIO5, &GPIO5_InitStructure);  
  */
  /*
   while(1)
  {
    CLOSE_LCD_BAT;
    OPEN_LCD_BAT;
      
    LIGHT_ON;
    LIGHT_OFF; 
    
    HTLCD_CS_Ctrl(Bit_SET);
    HTLCD_CS_Ctrl(Bit_RESET);
    
    HTLCD_CLK_Ctrl(Bit_SET);  
    HTLCD_CLK_Ctrl(Bit_RESET);
    
    HTLCD_DATA_Ctrl(Bit_SET);
    HTLCD_DATA_Ctrl(Bit_RESET);    
  }
  */
}
/**********************************************************************************
函数功能：写1个指令到LCD
入口：    指令内容
出口：    无
测试SCLK频率：一个周波5.2us
**********************************************************************************/
void	HTLCDWriteACommand(INT32U command)
{
  INT8U i;
  HTLCD_CS_0;	//CS的数据口置低
  for(i=0;i<=11;i++)		//命令只取最低12位
  {
    HTLCD_CLK_0;	//CLK的数据口
    if((command&0x0800))	//先高位再低位
      HTLCD_DATA_1;
    else
      HTLCD_DATA_0;

    HTLCD_CLK_1;		//CLK的数据口

    command<<=1;
  }
  HTLCD_CS_1;
  HTLCD_DATA_1;
}

/**********************************************************************************
函数功能：使LCD写数据模式
入口：    指令内容
出口：    无
**********************************************************************************/
void	HTLCDInWriteMode(INT8U LCD_RAM_ADDR)
{
  INT32U i,command;
  
  command=WRITE_ID|((INT32U)LCD_RAM_ADDR);
  for(i=0;i<=9;i++)			//只取最低10位
  {
    HTLCD_CLK_0;		//CLK的数据口
    if((command&0x0200))		//先高位再低位
      HTLCD_DATA_1;
    else
      HTLCD_DATA_0;

    HTLCD_CLK_1;		//CLK的数据口

    command<<=1;
  }
  HTLCD_DATA_1;
}

/**********************************************************************************
函数功能：写数据到LCD
入口:
        LCD_RAM_ADDR----------HTLCD LCD RAM 地址(0-95)
        DATA_ADDR-------------显示数据的缓冲区地址
        DATA_LENGTH-----------显示数据的长度
出口：  无
**********************************************************************************/
void	HTLCDWriteMultiData(INT8U LCD_RAM_ADDR,INT8U *DATA_ADDR,INT32U	DATA_LENGTH)
{
  INT32U i,j;
  INT8U DATA_BYTE;
  HTLCD_CS_0;
  HTLCDInWriteMode(LCD_RAM_ADDR); //使HTLCD进入写数据模式
  for(i=0;i<DATA_LENGTH;i++)
  {
    DATA_BYTE=*(DATA_ADDR+i);
    for(j=0;j<=7;j++)
    {
      HTLCD_CLK_0;	//WR的数据口
      if((DATA_BYTE&0x01))	//显示数据顺序：D0/D1/D2/D3
        HTLCD_DATA_1;	//DATA的数据口
      else
        HTLCD_DATA_0;	//DATA的数据口
      HTLCD_CLK_1;	
      DATA_BYTE>>=1;
    }
  }
  HTLCD_CS_1;
  HTLCD_DATA_1;
}
/**********************************************************************************
函数功能：工作模式下初始化LCDIO口
入口：    无
出口：    无
**********************************************************************************/
void Init_HTLCD_Mode(INT32U Mode)
{

  switch(Mode)
  {
    case SYS_NORMAL:
        MAIN_POWER_FOR_LCD; 
        //InitHTLCDIOPort();
        HTLCD_SYS_DIS();	//关振荡器和偏压发生器
        HTLCD_LCD_OFF();	//关LCD显示
        HTLCD_RC_32K();	//选择片上RC振荡器
        HTLCD_SYS_EN();	//开振荡器"*/
        HTLCD_LCD_ON();	//开LCD显示  
    break;
    
    case SYS_RESUME:
        BAK_POWER_FOR_LCD;  
        //InitHTLCDIOPort();
        HTLCD_SYS_DIS();	//关振荡器和偏压发生器
        HTLCD_LCD_OFF();	//关LCD显示
        HTLCD_RC_32K();	//选择片上RC振荡器
        HTLCD_SYS_EN();	//开振荡器
        HTLCD_LCD_ON();	//开LCD显示 
        Turn_Light_Off();  
    break;
    
    case SYS_SLEEP:
        //InitHTLCDIOPort(); 
        HTLCD_SYS_DIS();		//关振荡器和偏压发生器
        HTLCD_LCD_OFF();		//关LCD显示
        HTLCD_RC_32K();		//选择片上RC振荡器	
        Turn_Light_Off();
    break;    
  }   
}
/**********************************************************************************
函数功能：定时初始化LCD，避免LCD死机
入口：    无
出口：    无
**********************************************************************************/
void Init_LCD_Loop(void)
{
  static volatile S_Int32U Sec_Timer_Bak={CHK_BYTE,0xFFFFFFFF,CHK_BYTE};
 
  if(Sec_Timer_Pub-Sec_Timer_Bak.Var>=15)
  {
    if(Get_Sys_Status()==SYS_NORMAL)
    {
      MAIN_POWER_FOR_LCD; 
      HTLCD_SYS_DIS();	//关振荡器和偏压发生器
      HTLCD_LCD_OFF();	//关LCD显示
      HTLCD_RC_32K();	//选择片上RC振荡器
      HTLCD_SYS_EN();	//开振荡器"*/
      HTLCD_LCD_ON();	//开LCD显示  
    }
    else if(Get_Sys_Status()==SYS_RESUME)
    {
      BAK_POWER_FOR_LCD; 
      HTLCD_SYS_DIS();	//关振荡器和偏压发生器
      HTLCD_LCD_OFF();	//关LCD显示
      HTLCD_RC_32K();	//选择片上RC振荡器
      HTLCD_SYS_EN();	//开振荡器"*/
      HTLCD_LCD_ON();	//开LCD显示  
    }
    else
    {
      
      
    }
    Sec_Timer_Bak.Var=Sec_Timer_Pub;
  } 
}