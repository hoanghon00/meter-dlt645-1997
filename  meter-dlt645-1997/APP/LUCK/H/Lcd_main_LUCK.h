#ifndef __LCDMAIN__
#define __LCDMAIN__


#ifdef LCD_MAIN_C
#define LCD_MAIN_EXT
#else
#define LCD_MAIN_EXT extern
#endif


//#include "pub.h"
//#include "Pub_PUCK.h"


#define  NUMBMODEA  0xC114
#define  ITEMMODEA  0xC610
#define  NUMBMODEB  0xC128
#define  ITEMMODEB  0xC650




#define  VARHMODE1  0xC04C    //模式字6(无功组合1) 
#define  VARHMODE2  0xC04D    //模式字7(无功组合2) 
#define  DISENHBITS 0xC115    //电能[协议和显示]小数位[范围 ：2或3]
#define  ROLLCYCLE  0xC113    //循显时间
#define  ENKWHBITS  DISENHBITS    //电能[协议和显示]小数位[范围 ：2或3]
#define  POWERBITS  0xC116    //功率&需量[显示]小数位



#define LCD_SLEEP_SEC 60


DECLARE_BUF_TYPE(INT8U,6,S_Buf_BIT8_LEN5);
#ifdef LCD_MAIN_C
  LCD_MAIN_EXT  S_Buf_BIT8_LEN5 Pub_Bit8_Len5_PUCK={CHK_BYTE,{0},CHK_BYTE};
#endif
LCD_MAIN_EXT S_Buf_BIT8_LEN5 Pub_Bit8_Len5_PUCK;
#define  varhmode1 Pub_Bit8_Len5_PUCK.Buf[0]
#define  varhmode2 Pub_Bit8_Len5_PUCK.Buf[1]
#define  rollcycle Pub_Bit8_Len5_PUCK.Buf[2]
#define  dispmode   Pub_Bit8_Len5_PUCK.Buf[3]

DECLARE_BUF_TYPE(INT16U,4,S_Buf_BIT16_LEN4);
#ifdef LCD_MAIN_C
  LCD_MAIN_EXT S_Buf_BIT16_LEN4  Pub_Bit16_Len4_PUCK={CHK_BYTE,{0},CHK_BYTE};
#endif
LCD_MAIN_EXT S_Buf_BIT16_LEN4 Pub_Bit16_Len4_PUCK;
#define  MODE_A_NUM  Pub_Bit16_Len4_PUCK.Buf[0]
#define  MODE_B_NUM  Pub_Bit16_Len4_PUCK.Buf[1]
#define  LCDLoopCount  Pub_Bit16_Len4_PUCK.Buf[2]
#define  LCDLightCount Pub_Bit16_Len4_PUCK.Buf[3]

#ifdef LCD_MAIN_C
LCD_MAIN_EXT S_Int32U Sleep_Sec_Timer={CHK_BYTE,0x00,CHK_BYTE};
LCD_MAIN_EXT S_Int8U LCD_Loop_Num={CHK_BYTE,0x00,CHK_BYTE};
LCD_MAIN_EXT S_Int32U LCD_Loop_Ms10Timr={CHK_BYTE,0xFFFFFFFF,CHK_BYTE};
#endif
LCD_MAIN_EXT S_Int32U Sleep_Sec_Timer;
LCD_MAIN_EXT S_Int8U LCD_Loop_Num;
LCD_MAIN_EXT S_Int32U LCD_Loop_Ms10Timr;


#define TURN_ON_LIGHT   {LCDLightCount=0;}
#define TURN_OFF_LIGHT   {LCDLightCount=(0xFFFF-0X10);}
#define START_LOOP_DIS  {LCDLoopCount=0;}

#define MAX_MODI_NUM 5
typedef struct
{
 INT8U Flag;
 INT16U Para_Modi[MAX_MODI_NUM];
 INT8U CS[CS_BYTES];
}PARA_MODI_DIS;

LCD_MAIN_EXT PARA_MODI_DIS Para_Dis_Var;

#define DIS_BUF_LEN  268
DECLARE_BUF_TYPE(INT8U,DIS_BUF_LEN,S_Buf_Dis);
#ifdef LCD_MAIN_C
  LCD_MAIN_EXT S_Buf_Dis  _Dis_GetData_Buf={CHK_BYTE,{0},CHK_BYTE};
#endif
LCD_MAIN_EXT S_Buf_Dis _Dis_GetData_Buf;
#define  Dis_GetData_Buf  _Dis_GetData_Buf.Buf


void LCD_main_LUCK (INT8U Flag);
void Clr_LCDSleep_Timer(void);
void Dis_Jump_Para(void);
void Set_Para_Modi_DI(INT16U Di);
INT16U Get_Para_Modi_DI(void);
INT8U Get_Para_Modi_Flag(void);
void Check_LCDVar_Cs(void);
void Key_Fast_LCD_Proc(void);
void Loop_Dis_LCD_Proc(void);
void LCD_Init_PUCK(void);
void Set_C_Mode_DisCode(INT8U dval);
#endif