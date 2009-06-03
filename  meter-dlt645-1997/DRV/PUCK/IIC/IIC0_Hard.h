
#ifndef _MDIIC0_
#define _MDIIC0_


#ifdef  HARD_I2C_C
#define EXT_I2C
#else
#define EXT_I2C extern
#endif

#define WPPORT          P6//#define WPPORT      PORTC
#define SDAPORT		P6//#define SDAPORT		PORTC
#define SCLPORT		P6//#define SCLPORT		PORTC
#define SDADDR		PM6//#define SDADDR		DDRC
#define SCLDDR		PM6//#define SCLDDR		DDRC
#define WPDDR		PM6//#define SCLDDR		DDRC
#define SCLPIN		P6//#define SCLPIN		PINC
#define SDAPIN		P6//#define SDAPIN		PINC


#define WP		    2
#define SDA		    1
#define SCL		    0


#define SET_SCL_DATA   SCLPORT|=(1<<SCL)        //输出为1  
#define CLR_SCL_DATA   SCLPORT&=~(1<<SCL)       //输出为0 

#define SET_SDA_DATA   SDAPORT|=(1<<SDA)        //输出为1
#define CLR_SDA_DATA   SDAPORT&=~(1<<SDA)       //输出为0 

#define IIC_HARD_WP_EN  (WPPORT&=~(1<<WP))                  //WP开启
#define IIC_HARD_WP_DIS (WPPORT|=(1<<WP))                   //WP关闭

#define HARD_I2C_ENV //宏开启：使能setjmp绝对跳转
#ifdef HARD_I2C_ENV
typedef struct
{
  INT8U Head;
  jmp_buf Env;
  INT8U CS[CS_BYTES];
  INT8U Tail;
}Enviornment;

EXT_I2C Enviornment Hard_I2c_Env;
#endif

void I2cInit( void );
INT8U I2cStartCondition(void);
INT8U I2cPutAddress(INT8U data );
INT8U I2cGetDataNackSet(INT8U *Dst);
void I2cStopCondition( void);
INT8U I2cPutData( INT8U data);
INT8U I2cGetDataAckSet(INT8U *Dst);
void I2cStart(void);
void I2cReset(void);
#endif