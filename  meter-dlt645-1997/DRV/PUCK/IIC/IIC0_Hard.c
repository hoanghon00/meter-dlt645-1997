#define HARD_I2C_C
#include "DrvPub.h"

/********************************************************************************
********************************************************************************/
INT8U I2cWait(void)
{
   IF_WAITFOR_DRV_CYCLE_CONDITION(IICIF0,100)
   {
      ASSERT(A_WARNING,0);   //直接断言，可能影响速度！----------PUCK
#ifdef HARD_I2c_ENV
      longjmp(Hard_I2c_Env.Env,1);
#else
      return 0;
#endif 
   }
  IICIF0=0;
  return 1;
}

/********************************************************************************
********************************************************************************/
void I2cStart(void)     //产生起始条件------------PUCK
{
  STT0 = 1;  
}

/********************************************************************************
********************************************************************************/
INT8U I2cGetDataNackSet( INT8U *src )
{
    if(!TRC0)  //总线处于接收状态---------PUCK
    {
       WREL0 = 1;			//取消等待，发送NACK------------PUCK
       // WTIM0=0;
       ACKE0 = 0;                 //发送NACK
       I2cWait();                 //等待数据发送完毕--------PUCK
       *src = IIC0;
       return 1;
    }
    ASSERT(A_WARNING,0);   
#ifdef HARD_I2c_ENV
    longjmp(Hard_I2c_Env.Env,1);
#endif 
    return 0;
}
/********************************************************************************
********************************************************************************/
INT8U I2cGetDataAckSet( INT8U *src )
{	
    if(!TRC0)  //总线处于接收状态---------PUCK
    {
    WREL0 = 1;				//取消等待，发送 ACK------------PUCK
    //WTIM0=0;
     ACKE0 = 1;                    //发送 ACK
     I2cWait();
     *src = IIC0;
     return 1;
    }
    ASSERT(A_WARNING,0);
#ifdef HARD_I2c_ENV
      longjmp(Hard_I2c_Env.Env,1);
#endif 
    return 0;
}
/********************************************************************************
********************************************************************************/
INT8U I2cPutData( INT8U data )    //发送数据后等待ACK回应---------PUCK
{
  IIC0 = data;
  if(I2cWait())
    return 1;
  
  ASSERT(A_WARNING,0);
#ifdef HARD_I2c_ENV
      longjmp(Hard_I2c_Env.Env,1);
#endif 
  return 0;
}
/********************************************************************************
********************************************************************************/
INT8U I2cPutAddress( INT8U data )   //检测启动信号，发送总线数据，并检测ACK信号----------PUCK
{
  if(STD0)//STDO：1------表示检测到起始条件，0----------未检测到起始条件
  {
    if(I2cPutData(data))
      return 1;
  }
  ASSERT(A_WARNING,0);
#ifdef HARD_I2c_ENV
      longjmp(Hard_I2c_Env.Env,1);
#endif
  return 0;    
}
 
/********************************************************************************
********************************************************************************/
INT8U I2cStartCondition( void )    //检测总线状态，并产生启动信号---------PUCK
{
  if(IICBSY||SPT0||STT0)  //IICBSY----0:总线释放状态，1----------总线通信状态
  {
   I2cInit();
   IF_WAITFOR_DRV_CYCLE_CONDITION((!IICBSY),100)
   {
    ASSERT(A_WARNING,0);
#ifdef HARD_I2c_ENV
      longjmp(Hard_I2c_Env.Env,1);
#else
      return 0;
#endif       
   }
  }
  
  I2cStart();   //产生起始条件------------PUCK
  return 1;
}
/********************************************************************************
********************************************************************************/

void I2cStopCondition( void )
{
  //INT8U Rd;
   SPT0 = 1;
   //WAITFOR_DRV_CYCLE_TIMEOUT(100);
   //I2cInit();
  // Rd=STT0;
  // Rd=SPT0;
   //Rd=Rd;
}
/********************************************************************************
********************************************************************************/
void I2cReset(void)
{
  INT8U i;
  DEBUG_PRINT(PUCK,1,"IIC_Error----->Reset I2c_Hard Device!"); 
  IICE0=0;  //关闭IIC时钟
  
  SCLDDR&=~(1<<SCL);          //输出  
  SDADDR&=~(1<<SDA);          //输出  
  
  SET_SCL_DATA;        //输出为1
  SET_SDA_DATA;        //输出为1 
  CLR_SDA_DATA;       //输出为0 
      
  CLR_SCL_DATA;        //输出为0     
  SET_SDA_DATA;        //输出为1 
  for(i=0;i<=9;i++)
  {
    SET_SCL_DATA;         //输出为1 
    CLR_SCL_DATA;        //输出为0      
  }
  SET_SCL_DATA;        //输出为1
  SET_SDA_DATA;        //输出为1 
  CLR_SDA_DATA;       //输出为0 
  
  SET_SCL_DATA;              //输出为1
  CLR_SDA_DATA;        //输出为0 
  SET_SDA_DATA;         //输出为1  
  
  WAITFOR_DRV_CYCLE_TIMEOUT(10);
  I2cInit();
  WAITFOR_DRV_CYCLE_TIMEOUT(20);
} 
/********************************************************************************
********************************************************************************/
void I2cInit( void )
{
  IIC0EN = 1;	              //启动IIC时钟---------PUCK
  IICE0=0;                    //关闭IIC0的操作------PUCK
  IICMK0 = 1;	              //IIC中断关闭，使用查询方式-----------PUCK
  
  
  WPDDR&=~(1<<WP);            //设置WP口为输出---------PUCK
  WPPORT|=(1<<WP);            //设置WP口为高，禁止写,P62---------PUCK
  
  SCLDDR&=~(1<<SCL);          //将此口作为IIC的时钟口,P60---------PUCK
  SCLPORT&=~(1<<SCL);         //SCL置低，待IICE0为1时，自动将SCK置高
  
  SDADDR&=~(1<<SDA);          //将此口作为IIC的数据口,P61---------PUCK
  SDAPORT&=~(1<<SDA);         //SDA置低，待IICE0为1时，自动将SDA置高
  
  IICCL0=IIC0_CLOCK3;        //将IIC设置为标准模式(不需数字滤波)，同时设置速率为IIC0_CLOCK0:fclk/88(测试为125k)，IIC0_CLOCK3测试为250k----------PUCK
  IICX0=IIC0_EXPENSION0;     //和上面的寄存器共同设置速率----------PUCK
  
  STCEN=1;     //1-----停止条件不必检测，允许产生起始条件,0---------停止条件检测后允许产生起始条件，-----------PUCK
  IICRSV=1;    //1---------禁止通讯预约;0---------允许通讯预约
  
  SPIE0 = 0;   //0---------当检测到停止条件时，禁止产生中断请求,1---------允许
  WTIM0 = 1;   //1----------主设备模式：输出9 个时钟之后，将时钟输出设置为低电平且设置等待状态,在第9 个时钟的下降沿产生中断请求。
  ACKE0=1;     //1----------允许应答。在第九个时钟周期， SDA0 设置为低电平
  STT0=0;
  SPT0=0;
  IICIF0 = 0;    //IIC中断请求标志清0(SPIE0决定stop条件产生中断请求；或者WTIM0在第9个时钟产生中断请求)---------PUCK
}
