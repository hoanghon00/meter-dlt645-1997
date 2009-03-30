
#define USER_DEFINE_C
#define DRV_PUB
#define DRV_CFG

#include "DrvPub.h"

/**********************************************************************
初始化模拟I2C口
***********************************************************************/
void Init_IIC_Soft(void)
{
  INT8U i;
  for(i=IIC_SOFT_MIN_ID;i<=IIC_SOFT_MAX_ID;i++)
    I2cSoft_Open(i);
}

/**********************************************************************
关闭模拟I2C口
***********************************************************************/
void I2cSoft_Close_Port(void)
{
  INT8U i;
  for(i=IIC_SOFT_MIN_ID;i<=IIC_SOFT_MAX_ID;i++)
    I2cSoft_Open(i);
}