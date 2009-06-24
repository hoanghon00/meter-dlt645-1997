
/**
 * @file process.c
 *     { comment block }
 * @brief
 *     { brief description @ref process.cpp }
 * @author li.yao (nardoo@sohu.com)
 * @version v1.00
 * @date 05/28/2008 14:14
 * @note
 *     { paragraph where a note can be entered. }
 */

#include "Pub_PUCK.h"


extern u32 Set_ParaModi_Proc(u16 ModiDI);
extern unsigned char Ser_Support_Code(item_t code);

typedef union {
    struct {
        u32 bit0 : 4;
        u32 bit1 : 4;
        u32 bit2 : 4;
        u32 bit3 : 4;
        u32 bit4 : 4;
        u32 bit5 : 4;
        u32 bit6 : 4;
        u32 bit7 : 4;
    };
    u32 code;
} disp_t;


static volatile curs_t dispcursor;
static volatile offs_t dispoffset;
static volatile disp_t dispcode;


void Refresh_Curr_Offset(void)
{  
  if(modeA==dispmode)
  {
    if(dispoffset>=MODE_A_NUM && dispoffset<0XFFFF)
      dispoffset=0;  
    else if(dispoffset==0XFFFF)
      dispoffset=MODE_A_NUM-1;    
    return ;
  }
  
  if(modeB==dispmode)
  {
    if(dispoffset>=MODE_B_NUM && dispoffset<0XFFFF)
      dispoffset=0;  
    else if(dispoffset==0XFFFF)
      dispoffset=MODE_B_NUM-1;    
    return ;
  }
}

bool modeBorA () {
    if (dispmode != modeA) {
        dispmode = modeA;
    } else {
        dispmode = modeB;
    }
    dispcursor = -1;
    dispoffset = 0;
    lcdshow(dispmode, dispoffset);
    return(TRUE);
}

bool modeBtoA () {
    if (dispmode != modeB) {
        return (FALSE);
    }
    dispmode = modeA;
    dispcursor = -1;
    dispoffset = 0;
    lcdshow(dispmode, dispoffset);
    return(TRUE);
}

bool modeCtoA () {
    if (dispmode != modeC) {
        return (FALSE);
    }
    dispmode = modeA;
    dispcursor = -1;
    dispoffset = 0;
    lcdshow(dispmode, dispoffset);
    return(TRUE);
}

/********************************************************************************
PUCK:
函数功能：判断是否处于循显模式
入口：
返回：
    A模式
bool ModeABLoop(void)
{
  if((dispmode== modeA)||(dispmode==modeB))
    return LoopFlag;
  
  return 0;  
}
********************************************************************************/
/********************************************************************************
PUCK:
函数功能：A模式下的翻屏(其他模式下不具备自动翻屏功能)
入口：
返回：
    A模式
********************************************************************************/
bool disproll () 
{
    if (dispmode != modeA) 
    {
        return (FALSE);
    }
    dispcursor = -1;
    dispoffset++;
    Refresh_Curr_Offset();
    lcdshow(dispmode,dispoffset);  //获取A模式下，对应显示屏的具体显示元素信息-----------PUCK
    return(TRUE);
}

/********************************************************************************
PUCK:右移，只针对C模式有效
函数功能：右移，更新 dispcursor(C模式)
入口：
返回：
********************************************************************************/
void dispright() 
{
  if (dispmode != modeC) //切换至C模式，相关状态初始化---------PUCK
  {
      dispcursor = 5;
      dispmode = modeC;
      dispcode.code = 0;
  }
  if (dispcursor > 0)  //光标右移，减1，因为显示是从右到左-------------PUCK
  {
      --dispcursor;
  } 
  else 
  {
      dispcursor = MAXBITS- 1;
  }
  display(dispmode, dispcode.code);
}
/********************************************************************************
PUCK:左移，只针对C模式有效
函数功能：左移，更新 dispcursor(C模式)
入口：
返回：
********************************************************************************/
void displeft () 
{
  if (dispmode != modeC)  //切换至C模式，相关状态初始化---------PUCK
  {
      dispcursor = 3;
      dispmode = modeC;
      dispcode.code = 0;
  }
  if (dispcursor < MAXBITS-1) //光标左移，加1，因为显示是从右到左-------------PUCK
  {
      ++dispcursor;
  } 
  else 
  {
      dispcursor = 0;
  }
  display(dispmode, dispcode.code);
}

/********************************************************************************
PUCK:后翻
函数功能：
入口：
返回：
********************************************************************************/
void dispnext () 
{
    if(dispmode!= modeC) //不是C模式，则光标不闪烁--------------PUCK
    {
        dispcursor = -1;
        dispoffset++;
        Refresh_Curr_Offset();
        lcdshow(dispmode, dispoffset);  //A/B模式下，获取显示代码------------PUCK        
    } 
    else  //C模式-----------PUCK
    {
        switch (dispcursor) 
        {
            case 0x0 :
                ++dispcode.bit0;
                dispcode.bit0 %= 10;
                break;
            case 0x1 :
                ++dispcode.bit1;
                dispcode.bit1 %= 10;
                break;
            case 0x2 :
                ++dispcode.bit2;
                dispcode.bit2 %= 10;
                break;
            case 0x3 :
                ++dispcode.bit3;
                dispcode.bit3 %= 14;
                break;
            case 0x4 :
                ++dispcode.bit4;
                dispcode.bit4 %= 10;
                break;
            case 0x5 :
                ++dispcode.bit5;
                dispcode.bit5 %= 10;
                break;
            case 0x6 :
                ++dispcode.bit6;
                dispcode.bit6 %= 10;
                break;
            case 0x7 :
                ++dispcode.bit7;
                dispcode.bit7 %= 10;
                break;
        }
        display(dispmode, dispcode.code); //C模式下，根据显示代码来获取显示元素------------PUCK
    }
}

void dispback () {
    if (dispmode!= modeC) 
    {
        dispcursor = -1;
        dispoffset--;
        Refresh_Curr_Offset();
        lcdshow(dispmode, dispoffset);        
    } 
    else 
    {
        switch (dispcursor) {
            case 0x0 :
                if (--dispcode.bit0 > 10) {
                    dispcode.bit0 = 9;
                }
                break;
            case 0x1 :
                if (--dispcode.bit1 > 10) {
                    dispcode.bit1 = 9;
                }
                break;
            case 0x2 :
                if (--dispcode.bit2 > 10) {
                    dispcode.bit2 = 9;
                }
                break;
            case 0x3 :
                if (--dispcode.bit3 > 14) {
                    dispcode.bit3 = 13;
                }
                break;
            case 0x4 :
                if (--dispcode.bit4 > 10) {
                    dispcode.bit4 = 9;
                }
                break;
            case 0x5 :
                if (--dispcode.bit5 > 10) {
                    dispcode.bit5 = 9;
                }
                break;
            case 0x6 :
                if (--dispcode.bit6 > 10) {
                    dispcode.bit6 = 9;
                }
                break;
            case 0x7 :
                if (--dispcode.bit7 > 10) {
                    dispcode.bit7 = 9;
                }
                break;
        }
        display(dispmode, dispcode.code);
    }
}

void Set_C_Mode_DisCode(INT8U dval) 
{
  if(dispmode!=modeC)
    return ;
  
  if (dispcursor == -1) 
  {
        dispcursor = maxcurs() - 1;
  }
  switch (dispcursor) {
      case 0x0 :
          dispcode.bit0 = dval;
          break;
      case 0x1 :
          dispcode.bit1 = dval;
          break;
      case 0x2 :
          dispcode.bit2 = dval;
          break;
      case 0x3 :
          dispcode.bit3 = dval;
          break;
      case 0x4 :
          dispcode.bit4 = dval;
          break;
      case 0x5 :
          dispcode.bit5 = dval;
          break;
      case 0x6 :
          dispcode.bit6 = dval;
          break;
      case 0x7 :
          dispcode.bit7 = dval;
          break;
    }
    display(dispmode, dispcode.code);
    if (dispcursor > 0)  //光标右移，减1，因为显示是从右到左-------------PUCK
    {
        --dispcursor;
    } else 
    {
        dispcursor = maxcurs() - 1;
    }    
    screen(dispmode, dispcursor);
}

// 根据红外接收的按键值进行处理。
/*
void Process (ikey_t key) {
    static u8 flg = 0;
    switch (key) {
        case 0x01 :
        case 0x02 :
        case 0x03 :
        case 0x04 :
        case 0x05 :
        case 0x06 :
        case 0x07 :
        case 0x08 :
        case 0x09 :
        case 0x0A :
        case 0x0B :
        case 0x0C :
        case 0x0D :
        case 0x0E :
        case 0x0F :
            setbits(key);
            break;
        case 0x1A :
            Key_Fast_LCD_Proc(DOWN_KEY_VALUE);
            break;
        case 0x19 :
            Key_Fast_LCD_Proc(UP_KEY_VALUE);
            break;
        case 0x14 :
            Key_Fast_LCD_Proc(LEFT_KEY_VALUE);
            break;
        case 0x1D :
            Key_Fast_LCD_Proc(RIGHT_KEY_VALUE);
            break;
        case 0x10 :
        case 0x1F :
            Key_Fast_LCD_Proc(AB_KEY_VALUE);  //0x10：进入A套显示；0x1F：进入B套显示
            break;
        case 0x18 :
            lcdlight(flg ^= 1);
            break;
    }
}
*/
void Initial (void) 
{
    lcdinit();  //显示参数初始化----------PUCK
    //显示sram变量初始化----------PUCK
    dispoffset = 0;
    dispcursor = -1;
    dispmode = modeA;
    dispcode.code = 0;
    mem_set((void *)(&Para_Dis_Var),0x00,sizeof(Para_Dis_Var),(void *)(&Para_Dis_Var),sizeof(Para_Dis_Var));
    SET_STRUCT_SUM(Para_Dis_Var);
    lcdshow(modeA, 0);
    screen(modeA, -1);
}
/********************************************************************************
PUCK:更新显示内容,周期是 UPDATETIME ms
函数功能：按钮等快速响应的LCD处理
入口：
返回：
********************************************************************************/
INT8U Chk_Light_Condition(void)
{
  FP32S temp;  
  temp=Get_Max_Num(Measu_InstantData_ToPub_PUCK.Volt.A,Measu_InstantData_ToPub_PUCK.Volt.B,Measu_InstantData_ToPub_PUCK.Volt.C);
  temp/=UNIT_V;
  if(temp<Get_Un()*(FP32S)0.75)  //说明三相电压均低于75%Un
    return 1;  
  return 0;
}

/********************************************************************************
PUCK:更新显示内容,周期是 UPDATETIME ms
函数功能：按钮等快速响应的LCD处理
入口：
返回：
********************************************************************************/
void Key_Fast_LCD_Proc(void)
{
  INT8U KeyValue;
  KeyValue=Get_MeterKey_PUCK(LUCK);
  
  if((KeyValue<MIN_KEY_VALUE)||(KeyValue>MAX_KEY_VALUE))
    return ;
  
  switch(KeyValue)    //以下获取具体的显示元素信息------------PUCK
  {
        case UP_KEY_VALUE :
            //DEBUG_PRINT(LUCK,PRINT_PUCK_MEA_EN,"DownKey Pressed!");
            dispnext();  //下翻---------------PUCK
            Clr_LCDSleep_Timer();
            break;
        case DOWN_KEY_VALUE:
            //DEBUG_PRINT(LUCK,PRINT_PUCK_MEA_EN,"UpKey Pressed!");
            dispback();  //上翻---------------PUCK
            Clr_LCDSleep_Timer();
            break;
        case LEFT_KEY_VALUE :
            //DEBUG_PRINT(LUCK,PRINT_PUCK_MEA_EN,"LeftKey Pressed!");
            if(poweroff())  //掉电情况下，此键不起作用
              break;
            displeft();  //左移---------------PUCK
            break;
        case RIGHT_KEY_VALUE :
            //DEBUG_PRINT(LUCK,PRINT_PUCK_MEA_EN,"RightKey Pressed!");
            if(poweroff()) //掉电情况下，此键不起作用
              break;
            dispright();  //右移----------------PUCK
            break;
             
        case AB_KEY_VALUE :   //AB模式切换----------------PUCK
            //DEBUG_PRINT(LUCK,PRINT_PUCK_MEA_EN,"AB Mode Switch Key Pressed!");
            modeBorA();
            break;
              
        case A_KEY_VALUE :   //模式切换到A----------------PUCK
            //09-02-09 PUCK Add: modeAtoB
            dispmode = modeA;
            dispcursor = -1;
            dispoffset = 0;
            lcdshow(dispmode, dispoffset);
            break;
            
        case B_KEY_VALUE :   //模式切换到B----------------PUCK
           //09-02-09 PUCK Add: modeAtoB
            dispmode = modeB;
            dispcursor = -1;
            dispoffset = 0;
            lcdshow(dispmode, dispoffset);
            break;
            
       default:
        return ;
  }
  TURN_ON_LIGHT;
  screen(dispmode, dispcursor);  //刷屏----------PUCK
  LCD_Loop_Ms10Timr.Var=Ms10_Timer_Pub;
  LCD_Loop_Num.Var=0;
  START_LOOP_DIS;
  
}

/********************************************************************************
PUCK:更新显示内容,周期是 UPDATETIME ms
函数功能：循显等慢速显示处理
入口：
返回：
********************************************************************************/
void Loop_Dis_LCD_Proc(void) 
{
  if(LCD_Loop_Ms10Timr.Var!=Ms10_Timer_Pub)
  {
    LCD_Loop_Num.Var+=(Ms10_Timer_Pub-LCD_Loop_Ms10Timr.Var);
    LCD_Loop_Ms10Timr.Var=Ms10_Timer_Pub;
    if(LCD_Loop_Num.Var<UPDATETIME/10)
      return ;
    LCD_Loop_Num.Var=TASK_RUN_TIME/10;
  }
  
   if((LOOP_SEC_TIMER_DIFF>CYCLE)&&disproll()) // A模式下，在1个周期后，没有按键按下，获取A模式下的显示信息--------PUCK
      START_LOOP_DIS;
   if((LOOP_SEC_TIMER_DIFF>2*CYCLE)&&modeBtoA())// B模式下，在2个循显周期后，没有按键按下，进入A模式，获取A模式下的信息--------PUCK
      START_LOOP_DIS;
   if((LOOP_SEC_TIMER_DIFF>120)&&modeCtoA())   // C模式下，在2分钟后无按键按下，进入A模式，获取A模式下的信息--------PUCK
      START_LOOP_DIS;
   if (LCDLightCount== 0) 
   {
     if(poweroff()||Chk_Light_Condition())      //掉电或者三相电压都低于75%Un时，关闭背光
        lcdlight(FALSE);
     else                   //非掉电情况下，打开背光
        lcdlight(TRUE);
   }
   if(++LCDLightCount>SECOND_3(60)) 
   {
      LCDLightCount=1;
      lcdlight(FALSE);
   }
   screen(dispmode, dispcursor);  //刷屏----------PUCK
}

/********************************************************************************
PUCK:更新显示内容,周期是 UPDATETIME ms
函数功能：按钮等快速响应的LCD处理，跳屏
入口：
返回：
********************************************************************************/
void Dis_Jump_Para(void) 
{
   u16 modipara;
   if(modipara=Get_Para_Modi_DI())   //显示参数修改，更新参数，并点亮液晶------PUCK
    {
      if(Ser_Support_Code(modipara))
      {
        if(0xC011==modipara)  //时/分/秒不跳屏
          return ;
        
        START_LOOP_DIS;
        TURN_ON_LIGHT;
        dispmode=modeC; 
        dispcursor = 0;        
        dispcode.code=Set_ParaModi_Proc(modipara);
        display(modeC, dispcode.code);
      }
    }
}
