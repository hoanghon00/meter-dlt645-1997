#ifndef IIC_SOFT_H
#define IIC_SOFT_H


#ifdef  SOFT_I2C_C
#define EXT_SOFT_I2C
#else
#define EXT_SOFT_I2C extern
#endif



#ifdef DRV_SIMU_DS3231
#define IIC_SOFT_ID0          0                      //3231
#endif

#ifdef ID_MEM_IIC_CAL
  #define IIC_SOFT_ID1        1       //模拟I2C
#endif

#ifdef IIC_SOFT_ID0
#define IIC_SOFT_MIN_ID      IIC_SOFT_ID0
#endif

#ifdef IIC_SOFT_ID1
#define IIC_SOFT_MAX_ID      IIC_SOFT_ID1
#endif

#ifndef IIC_SOFT_MIN_ID
  #define IIC_SOFT_MIN_ID    0
#endif

#ifndef IIC_SOFT_MAX_ID
  #define IIC_SOFT_MAX_ID      0
#endif


#define SOFT_I2c_ENV //宏开启：使能setjmp绝对跳转
#ifdef SOFT_I2c_ENV
EXT_SOFT_I2C Enviornment Soft_I2c_Env;
#endif

void I2cSoft_Open(INT8U Id);
void I2cSoft_Close(INT8U Id);

INT8U I2cSoft_Start(INT8U Id);
INT8U I2cSoft_Stop(INT8U Id) ;
INT8U I2cSoft_Send_N_Ack(INT8U Id,INT8U Ack_Nack);
INT8U I2cSoft_Send_Byte(INT8U Id,INT8U sbyte);
INT8U I2cSoft_Read_Byte(INT8U Id,INT8U *Dst);
void I2cSoft_Reset(INT8U Id);
#endif