#define SOFT_I2C_C
#include "DrvPub.h"

        

typedef struct
{ 
  void (* const Set_Sclk_Dir)(INT8U);
  void (* const Set_Sda_Dir)(INT8U);
  void (* const Set_Sclk_Pin)(INT8U);
  void (* const Set_Sda_Pin)(INT8U);
  INT8U (* const Get_Sda_Pin)(void);
}CONST_SOFT_IIC;

CONST_SOFT_IIC  CONST_I2C_PORT[IIC_SOFT_MAX_ID+1]=
{
#ifdef IIC_SOFT_ID0
  {&Set_3231_Sclk_Dir,&Set_3231_Sda_Dir,&Set_3231_Sclk_Pin,&Set_3231_Sda_Pin,&Get_3231_Sda_Pin},    //----------------0
#endif
#ifdef IIC_SOFT_ID1
  {&Set_Cal_Sclk_Dir,&Set_Cal_Sda_Dir,&Set_Cal_Sclk_Pin,&Set_Cal_Sda_Pin,&Get_Cal_Sda_Pin}    //----------------0
#endif
};

/**********************************************************************************
函数功能：初始化模拟IIC口
入口：
出口：
**********************************************************************************/ 
void I2cSoft_Open(INT8U Id)   //将总线置为空闲状态
{
  if(Id>IIC_SOFT_MAX_ID)  //直接断言，可能影响速度！----------PUCK
  {
    ASSERT(A_WARNING,0);
    return ;
  }
  
  CONST_I2C_PORT[Id].Set_Sclk_Dir(DIR_OUTPUT);
  CONST_I2C_PORT[Id].Set_Sclk_Pin(1);             //改为输出为1
  CONST_I2C_PORT[Id].Set_Sda_Dir(DIR_OUTPUT);
  CONST_I2C_PORT[Id].Set_Sda_Pin(1);              //改为输出为1
}
/**********************************************************************************
函数功能：关闭模拟IIC口
入口：
出口：
**********************************************************************************/ 
void I2cSoft_Close(INT8U Id)   //将总线置为空闲状态
{
  if(Id>IIC_SOFT_MAX_ID)  //直接断言，可能影响速度！----------PUCK
  {
    ASSERT(A_WARNING,0);
    return ;
  }
  
  CONST_I2C_PORT[Id].Set_Sclk_Dir(DIR_INPUT);
  CONST_I2C_PORT[Id].Set_Sclk_Pin(1);         //改为输出为1
  CONST_I2C_PORT[Id].Set_Sda_Dir(DIR_INPUT);
  CONST_I2C_PORT[Id].Set_Sda_Pin(1);           //改为输出为1
}

/**********************************************************************************
函数功能：启动软件模拟I2C口,11.0592下，测试SCLK频率为73.5k
入口：
出口：
**********************************************************************************/ 
INT8U I2cSoft_Start(INT8U Id)
{
  if(Id>IIC_SOFT_MAX_ID)  //直接断言，可能影响速度！----------PUCK
  {
    ASSERT(A_WARNING,0);
#ifdef Soft_I2c_Env
    longjmp(Soft_I2c_Env.Env,1);
#else
    return 0;
#endif 
  }
  
  CONST_I2C_PORT[Id].Set_Sda_Pin(1);
  CONST_I2C_PORT[Id].Set_Sclk_Pin(1);

  CONST_I2C_PORT[Id].Set_Sda_Pin(0);        //SDA在SCLK为高时，产生一个下降沿,起始条件产生---------PUCK
  return 1;
}
/**********************************************************************************
函数功能：停止3231
入口：
出口：
**********************************************************************************/ 
INT8U I2cSoft_Stop(INT8U Id)    //在SCLK为高的情况下，SDA产生一个上升沿，作为停止条件
{
  if(Id>IIC_SOFT_MAX_ID)  //直接断言，可能影响速度！----------PUCK
  {
#ifdef Soft_I2c_Env
    longjmp(Soft_I2c_Env.Env,1);
#else
    return 0;
#endif 
  }
  
  CONST_I2C_PORT[Id].Set_Sda_Dir(DIR_OUTPUT);
  CONST_I2C_PORT[Id].Set_Sda_Pin(0);  

  CONST_I2C_PORT[Id].Set_Sclk_Pin(1);
  CONST_I2C_PORT[Id].Set_Sda_Pin(1);
  return 1;
}
/**********************************************************************************
函数功能：回应ACK或者NACK
入口：
出口：
**********************************************************************************/ 
INT8U I2cSoft_Send_N_Ack(INT8U Id,INT8U Ack_Nack)
{
  if(Id>IIC_SOFT_MAX_ID)  //直接断言，可能影响速度！----------PUCK
  {
#ifdef Soft_I2c_Env
    longjmp(Soft_I2c_Env.Env,1);
#else
    return 0;
#endif 
  }
  
  CONST_I2C_PORT[Id].Set_Sclk_Pin(0);  //清0，握住总线-----------------PUCK
  CONST_I2C_PORT[Id].Set_Sda_Dir(DIR_OUTPUT);
  CONST_I2C_PORT[Id].Set_Sda_Pin(Ack_Nack);  //Ack_Nack为0表示ACK，为1表示NACK    
  
  CONST_I2C_PORT[Id].Set_Sclk_Pin(1);  //SCLK为高时，SDA为低，表示发送ACK
  CONST_I2C_PORT[Id].Set_Sclk_Pin(0);
  return 1;
}
/**********************************************************************************
函数功能：检测是否有ACK信号
入口：
出口：1-------------成功；0----------失败
**********************************************************************************/ 
INT8U I2cSoft_Check_Ack(INT8U Id)
{
  INT8U count;
    
  if(Id>IIC_SOFT_MAX_ID)  //直接断言，可能影响速度！----------PUCK
  {
#ifdef Soft_I2c_Env
    longjmp(Soft_I2c_Env.Env,1);
#else
    return 0;
#endif 
  }
  
  CONST_I2C_PORT[Id].Set_Sda_Pin(1);        //设置SDA,释放总线-------------PUCK
  CONST_I2C_PORT[Id].Set_Sda_Dir(DIR_INPUT);
  
  CONST_I2C_PORT[Id].Set_Sclk_Pin(1);     //握总线，查询ACK

  for(count=0;count<100;count++)
  {
    if(CONST_I2C_PORT[Id].Get_Sda_Pin()==0)  //ACK回应,为0，表示是ACK
    {
      CONST_I2C_PORT[Id].Set_Sclk_Pin(0);
      CONST_I2C_PORT[Id].Set_Sda_Dir(DIR_OUTPUT);
      return 1;
    }    
  }  
  CONST_I2C_PORT[Id].Set_Sclk_Pin(0);
  CONST_I2C_PORT[Id].Set_Sda_Dir(DIR_OUTPUT);
    
   ASSERT(A_WARNING,0);   //直接断言，可能影响速度！----------PUCK
  
#ifdef HARD_I2c_ENV    
    longjmp(Soft_I2c_Env.Env,1);
#endif 
    return 0;
}

/**********************************************************************************
函数功能：iic总线上发数据:8bits
入口：
出口：1-------------成功；0----------失败
**********************************************************************************/ 
INT8U I2cSoft_Send_Byte(INT8U Id,INT8U sbyte)
{	
  INT8U i;

  if(Id>IIC_SOFT_MAX_ID)  //直接断言，可能影响速度！----------PUCK
  {
#ifdef Soft_I2c_Env
    longjmp(Soft_I2c_Env.Env,1);
#else
    return 0;
#endif 
  }
  
  CONST_I2C_PORT[Id].Set_Sclk_Pin(0);
  for (i=0;i<8;i++)
  {
      if(sbyte&0x80)
          CONST_I2C_PORT[Id].Set_Sda_Pin(1);
      else
          CONST_I2C_PORT[Id].Set_Sda_Pin(0);
      CONST_I2C_PORT[Id].Set_Sclk_Pin(1);    //SCL变高，锁存数据
      CONST_I2C_PORT[Id].Set_Sclk_Pin(0);
      sbyte <<=1;
  }
  return(I2cSoft_Check_Ack(Id));	 	    
}
/********************************************************************************
********************************************************************************/
void I2cSoft_Reset(INT8U Id)
{
  INT8U i;
  
  DEBUG_PRINT(PUCK,1,"IIC_Error----->Reset I2c_Soft Device!"); 
  ASSERT(A_WARNING,0);
  I2cSoft_Start(Id);
  
  CONST_I2C_PORT[Id].Set_Sclk_Pin(0);
  CONST_I2C_PORT[Id].Set_Sda_Pin(1);  //SDA置1,SCL脉冲，释放总线
  for(i=0;i<=9;i++)
  { 
    CONST_I2C_PORT[Id].Set_Sclk_Pin(1);    //SCL变高，锁存数据
    CONST_I2C_PORT[Id].Set_Sclk_Pin(0);
  }  
  I2cSoft_Start(Id);
  I2cSoft_Stop(Id);
  I2cSoft_Open(Id);
} 
/**********************************************************************************
函数功能：iic总线上读数据:8bits
入口：
出口：1-------------成功；0----------失败
**********************************************************************************/ 
INT8U I2cSoft_Read_Byte(INT8U Id,INT8U *Dst)
{
  INT8U i;
  INT8U temp;

  if(Id>IIC_SOFT_MAX_ID)  //直接断言，可能影响速度！----------PUCK
  {
#ifdef Soft_I2c_Env
    longjmp(Soft_I2c_Env.Env,1);
#else
    return 0;
#endif     
  }
  
  CONST_I2C_PORT[Id].Set_Sda_Pin(1);        //设置SDA,释放总线-------------PUCK
  CONST_I2C_PORT[Id].Set_Sda_Dir(DIR_INPUT);
  
  for(i=0,temp=0;i<8;i++)
  {
      temp<<=1;
      CONST_I2C_PORT[Id].Set_Sclk_Pin(1);
      if(CONST_I2C_PORT[Id].Get_Sda_Pin())
        temp|=1;
      CONST_I2C_PORT[Id].Set_Sclk_Pin(0);
  }
  
  *Dst=temp;
  return  1;
}
