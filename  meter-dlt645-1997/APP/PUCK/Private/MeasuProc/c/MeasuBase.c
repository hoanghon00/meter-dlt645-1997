#define     MeasuBase_PUCK
#include    "Pub_PUCK.h"
#include    "MeasuBase.h"

/******************************文件功能************************************************
文件功能：对MeasuIC基本操作，包括：
1：底层读、写
2：底层读比较、写比较
***********************************************************************************/

void Delay_WrRd_3uS(void)
{
  //asm("nop"); asm("nop"); asm("nop");
  //asm("nop"); asm("nop"); asm("nop");
}
/**********************************************************************************
函数功能：读计量数据/参数子程序
入口：    计量数据/参数寄存器地址
出口：    计量数据/参数,实际为3字节，高字节屏蔽
110.592下，测试SCLK频率：一个周波7.2us，即133k左右
**********************************************************************************/
INT32U Measu_RdData(INT8U Reg)
{
  INT8U i,j,temp[3];
  INT32U ReturData;
  
  Measu_IC_Delay;
  MEASU_CS_1;
  MEASU_CLK_0;
  MEASU_CS_0;
  
  for(i=0;i<=7;i++)
  {
    MEASU_CLK_1;
    if((Reg&0x80))
      MEASU_DOUT_1;   
    else   
      MEASU_DOUT_0;
    MEASU_CLK_0;
    Reg<<=1;  
  }  
  MEASU_DOUT_0;  //数据发完后，数据端口保证确定的低电平----------PUCK
  //Delay_WrRd_3uS();                      //SCLK频率低于200k()，不必等待；频率大于200k，等待3us      
  //WAITFOR_DRV_CYCLE_TIMEOUT(50);
  
  temp[0]=temp[1]=temp[2]=0;
  for(i=0;i<3;i++)
  {
    for(j=0;j<=7;j++)
    {
      MEASU_CLK_1;
      if(MEASU_DIN_STAT)
      {
        if(MEASU_DIN_STAT)
          temp[2-i]=temp[2-i]|0x01;     
      }
      MEASU_CLK_0;
      if(j!=7)
        temp[2-i]<<=1;
    }
  }
  //WAITFOR_DRV_CYCLE_TIMEOUT(10);
  MEASU_CS_1;
  
  ReturData=(INT32U)temp[0]+(INT32U)temp[1]*256+(INT32U)temp[2]*65536;  //簇成3字节
  ReturData=ReturData&0x00FFFFFF;
  return(ReturData);
}

/**********************************************************************************
函数功能：写计量参数子程序
入口：    
ParaReg-------------计量参数寄存器地址
paradata_计量-------计量参数
出口：    无
**********************************************************************************/
void Measu_WrData(INT8U ParaReg,INT32U paradata)
{
  INT8U para[3];
  INT8U i,j;
  
  Measu_IC_Delay;
  para[2]=*((INT8U *)&paradata);     //最低字节
  para[1]=*((INT8U *)&paradata+1);   //中间字节
  para[0]=*((INT8U *)&paradata+2);   //最高字节
  
  MEASU_CS_1;
  MEASU_CLK_0;
  MEASU_CS_0;
  for(i=0;i<=7;i++)
  {
    MEASU_CLK_1;
    if((ParaReg&0x80))
      MEASU_DOUT_1;
    else 
      MEASU_DOUT_0;
    //asm("nop");    asm("nop");    asm("nop");   
    MEASU_CLK_0;
    ParaReg<<=1;  
  }
  MEASU_DOUT_0;  //数据发完后，数据端口保证确定的低电平----------PUCK
  for(i=0;i<3;i++)
  {
    for(j=0;j<=7;j++)
    {
      MEASU_CLK_1;
      if((para[i]&0x80))
        MEASU_DOUT_1;
      else
        MEASU_DOUT_0;
      //asm("nop");      asm("nop");      asm("nop");
      MEASU_CLK_0;
      para[i]<<=1;
    }
  }
  MEASU_DOUT_0;  //数据发完后，数据端口保证确定的低电平----------PUCK
  MEASU_CS_1;
}
/**********************************************************************************
函数功能：读数据比较
入口：    
LastData-----参数寄存器地址
出口： 1------表示比对正确；0---------比对错误
**********************************************************************************/
INT8U Measu_RdCompData(INT32U LastData)
{
  INT8U i;
  INT32U tempdata;  
 
  for(i=0;i<3;i++)
  {
    tempdata=Measu_RdData(REG_R_LASTRD);
    if(tempdata == LastData)
      return 1;
  }
  return 0;
}
/**********************************************************************************
函数功能：读参数，并比较
入口：    
ParaReg-----参数寄存器地址
出口： 1------表示比对正确；0---------比对错误
**********************************************************************************/
INT8U Measu_RdParaAndCompData(INT8U ParaReg,INT8U DstBuf[])
{
  INT32U TempData1,TempData2;
  
  TempData1=Measu_RdData(ParaReg);
  TempData2=Measu_RdData(ParaReg);
  
  if(TempData1!=TempData2)
    return 0;
  
  mem_cpy(DstBuf,(INT8U *)(&TempData1),4,DstBuf,4);    //DstBuf强制转换：这样搞可能会出问题，用mem_cpy
  return 1;

}
/**********************************************************************************
函数功能：写数据比较
入口：    
ParaReg-----参数寄存器地址
WrData-------参数
出口： 1------表示比对正确；0---------比对错误
**********************************************************************************/
INT8U Measu_WrAndCompData(INT8U ParaReg,INT32U WrData)
{
  INT32U tempdata;
  
  WrData=WrData&0x00ffffff;
  Measu_WrData(ParaReg,WrData);
  tempdata=Measu_RdData(REG_R_LASTWR);
  if(tempdata==WrData)
    return 1;
   
  return 0;
}
/**********************************************************************************
函数功能：读数据，并比较
入口：    
ParaReg-----参数寄存器地址
出口： 1------表示比对正确；0---------比对错误
**********************************************************************************/
INT8U Measu_RdAndCompData(INT8U ParaReg,INT8U DstBuf[])
{
  INT8U  Flag;
  INT32U TempData;
  

  TempData=Measu_RdData(ParaReg);
  Flag=Measu_RdCompData(TempData);
  
  if(!Flag)
    return 0;
  
  mem_cpy(DstBuf,(INT8U *)(&TempData),4,DstBuf,4);    //DstBuf强制转换：这样搞可能会出问题，用mem_cpy
  return Flag;

}

/**********************************************************************************
函数功能：读参数，并比较3次
入口：    
ParaReg-----参数寄存器地址
出口： 1------表示比对正确；0---------比对错误
**********************************************************************************/
INT8U Measu_RdAndCompData_3Times(INT8U ParaReg,INT8U DstBuf[])
{
  INT8U  i,Flag;
 
  for(i=0;i<3;i++)
  {
    Flag=Measu_RdAndCompData(ParaReg,DstBuf);
    /*if(Flag&&((DstBuf[0]!=0xff)||(DstBuf[1]!=0xff)||(DstBuf[2]!=0xff)))
      return 1;
    if(Flag&&(DstBuf[0]==0xff)&&(DstBuf[1]==0xff)&&(DstBuf[2]==0xff))
    {
      //DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->SPI Rd Data Compare Err,Reg=%d,RD=0xffffff,Read Aganin After 153ms",ParaReg);
      SYS_TimeDly_Ms(153);    //读错误，再等153毫秒后再读
    }*/
    if(Flag)
      return 1;
  }    
  
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Read Data Compare Error With 3 Times,Reg=0x%x\n",ParaReg);  
  MeasuStatusMode.RdCsErr=1;
  SET_STRUCT_SUM(MeasuStatusMode);
  return 0;
}
/**********************************************************************************
函数功能：写数据，并比较3次
入口：    
LastData-----参数寄存器地址
出口： 1------表示比对正确；0---------比对错误
**********************************************************************************/
INT8U Measu_WrAndCompData_3Times(INT8U ParaReg,INT32U WrData)
{
  INT8U  Flag,i;   
  for(i=0;i<3;i++)
  {
    Flag=Measu_WrAndCompData(ParaReg,WrData);
    if(Flag)
      return 1;
  }
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Write Para Compare Error With 3 Times,Reg=0x%x\n",ParaReg); 
  MeasuStatusMode.WrCsErr=1;
  SET_STRUCT_SUM(MeasuStatusMode);
  return 0;

}
/**********************************************************************************
函数功能：读参数，并比较
入口：    
ParaReg-----参数寄存器地址
出口： 1------表示比对正确；0---------比对错误
**********************************************************************************/
INT8U Measu_RdAndCompPara_3Times(INT8U ParaReg,INT8U DstBuf[])
{
  INT8U  i,Flag;
 
  EnMeasuPara();
  
  for(i=0;i<3;i++)
  {
    Flag=Measu_RdParaAndCompData(ParaReg,DstBuf);
    /*if(Flag&&((DstBuf[0]!=0xff)||(DstBuf[1]!=0xff)||(DstBuf[2]!=0xff)))
    {
      EnMeasuData();
      return 1;
    }
    if(Flag&&(DstBuf[0]==0xff)&&(DstBuf[1]==0xff)&&(DstBuf[2]==0xff))
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->SPI Rd Para Compare Err,Reg=%d,RD=0xffffff,Read Aganin After 153ms",ParaReg);
      SYS_TimeDly_Ms(153);    //读错误，再等153毫秒后再读
    }*/
    if(Flag)
    {
      EnMeasuData();
      return 1;
    }
  }    
  
  EnMeasuData();
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->Read Para Compare Error With 3 Times,Reg=0x%x\n",ParaReg);  
  MeasuStatusMode.RdCsErr=1;
  SET_STRUCT_SUM(MeasuStatusMode);
  return 0;
}
