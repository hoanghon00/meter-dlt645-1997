
#include "DrvPub.h"

#ifndef DRV_SIMU_DS3231
/**********************************************************************************
函数功能：
入口：
出口：出口：1------------------读取成功；0----------读取失败
**********************************************************************************/   
INT8U DS3231_Read_Buf(INT8U addr,INT8U Len,INT8U *Dst)
{
 INT8U i,Flag;
 
#ifdef HARD_I2C_ENV
   if(setjmp(Hard_I2c_Env.Env)==0)
   {
#endif 
     
    IICE0=1;               //打开II2C操作时钟，因有可能会关闭-----------PUCK
    Flag=I2cStartCondition();	
    Flag&=I2cPutAddress(0xd0);	
    Flag&=I2cPutData(addr);
    I2cStart();
    Flag&=I2cPutData(0xd1);	 //读操作
    for(i=0;i<Len-1;i++)
        Flag&=I2cGetDataAckSet(Dst+i);
    Flag&=I2cGetDataNackSet(Dst+Len-1);
    I2cStopCondition();
#ifdef HARD_I2C_ENV
    return 1;
   }
   else
#else
    if(!OkFlag)
#endif
   {
      DEBUG_PRINT(PUCK,1,"IIC_Error----->Reset I2c_Hard Device!");  
      I2cReset();
   }
  return Flag;
}
/**********************************************************************************
函数功能：
入口：
出口：出口：1------------------写入成功；0----------写入失败
**********************************************************************************/   
INT8U DS3231_Write_Buf(INT8U Addr,INT8U Len,INT8U *Src)
{
  INT8U i,Flag;
  IICE0=1;               //打开II2C操作时钟，因有可能会关闭-----------PUCK
  Flag=I2cStartCondition();	
  Flag&=I2cPutAddress(0xd0);	
  Flag&=I2cPutData(Addr);
  for(i=0;i<Len;i++)
    Flag&=I2cPutData(Src[i]);
  I2cStopCondition();
  if(!OkFlag)
  {
    DEBUG_PRINT(PUCK,1,"IIC_Error----->Reset I2c_Hard Device!");  
    I2cReset();
  }
  return Flag;
}
#endif
