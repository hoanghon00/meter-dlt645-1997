#include "DrvPub.h"
#include "I2C_Soft.h"

#ifdef DRV_SIMU_DS3231

/**********************************************************************************/
/**********************************************************************************/ 
void Set_3231_Sclk_Dir(INT8U Dir)
{
  if(Dir==DIR_OUTPUT)
    SET_3231_SCLK_OUTPUT;
  else
    SET_3231_SCLK_INPUT;  
}
/**********************************************************************************
**********************************************************************************/ 
void Set_3231_Sda_Dir(INT8U Dir)
{
  if(Dir==DIR_OUTPUT)
    SET_3231_SDA_OUTPUT;
  else
    SET_3231_SDA_INPUT; 
}
/**********************************************************************************
**********************************************************************************/ 
void Set_3231_Sclk_Pin(INT8U Level)
{
  if(Level)
    SET_3231_SCLK_1; 
  else
    CLR_3231_SCLK_0; 
}
/**********************************************************************************
**********************************************************************************/ 
void Set_3231_Sda_Pin(INT8U Level)
{
  if(Level)
    SET_3231_SDA_1; 
  else
    CLR_3231_SDA_0; 
}
/**********************************************************************************
**********************************************************************************/ 
INT8U Get_3231_Sda_Pin(void)
{
  return (GET_SDA_STATUS);  
}

#endif
/**********************************************************************************
**********************************************************************************/ 
void Init_DS3231_IIC_Soft(void)
{
#ifdef DRV_SIMU_DS3231
  I2cSoft_Open(IIC_SOFT_ID0);  
#endif 
}
/**********************************************************************************
**********************************************************************************/ 
void Close_DS3231_IIC_Soft(void)
{
#ifdef DRV_SIMU_DS3231
  I2cSoft_Close(IIC_SOFT_ID0);  
#endif 
}

#ifdef DRV_SIMU_DS3231
/**********************************************************************************
函数功能：
入口：
出口：1------------------读取成功；0----------读取失败
**********************************************************************************/   
INT8U DS3231_Read_Buf(INT8U addr,INT8U Len,INT8U *Dst)
{
  INT8U OkFlag;

#ifdef SOFT_I2c_ENV
   if(setjmp(Soft_I2c_Env.Env)==0)
   { 
#endif  

   I2cSoft_Start(IIC_SOFT_ID0);
   OkFlag=I2cSoft_Send_Byte(IIC_SOFT_ID0,0xd0);
   OkFlag&=I2cSoft_Send_Byte(IIC_SOFT_ID0,addr);
   I2cSoft_Start(IIC_SOFT_ID0);
   OkFlag&=I2cSoft_Send_Byte(IIC_SOFT_ID0,0xd1);   
   OkFlag&=I2cSoft_Read_nByteS(IIC_SOFT_ID0,Len,Dst);
   OkFlag&=I2cSoft_Stop(IIC_SOFT_ID0);
#ifdef SOFT_I2c_ENV
    return 1;
   }
   else
#else
    if(!OkFlag)
#endif
    {
      I2cSoft_Reset(IIC_SOFT_ID0);
      OkFlag=0;
    }
    return OkFlag;
}
/**********************************************************************************
函数功能：
入口：
出口：出口：1------------------写入成功；0----------写入失败
**********************************************************************************/   
INT8U DS3231_Write_Buf(INT8U addr,INT8U Len,INT8U *Src)
{
  INT8U i,OkFlag;

#ifdef SOFT_I2c_ENV
   if(setjmp(Soft_I2c_Env.Env)==0)
   { 
#endif     
    I2cSoft_Start(IIC_SOFT_ID0);
    OkFlag=I2cSoft_Send_Byte(IIC_SOFT_ID0,0xd0);	
    OkFlag&=I2cSoft_Send_Byte(IIC_SOFT_ID0,addr);
    for(i=0;i<Len;i++)
      OkFlag&=I2cSoft_Send_Byte(IIC_SOFT_ID0,Src[i]);
    OkFlag&=I2cSoft_Stop(IIC_SOFT_ID0);
#ifdef SOFT_I2c_ENV
    return 1;
   }
   else
#else
    if(!OkFlag)
#endif
    {
      I2cSoft_Reset(IIC_SOFT_ID0);
      OkFlag=0;
    }
    return OkFlag;  
}
#endif
