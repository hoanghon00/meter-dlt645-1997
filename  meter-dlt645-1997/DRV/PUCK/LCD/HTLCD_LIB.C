#include "Pub_PUCK.h"


#if LCD_PROPERTY==LCD_NORMAL
  #define HTLCD_C
  #include "HTLCD_lib.h"
#endif

#if LCD_PROPERTY==LCD_JS_ZXW
  #define HTLCD_C_JS_ZXW
  #include "HTLCD_lib_JS_ZXW.h"
#endif

#if LCD_PROPERTY==LCD_JS_JX
  #define HTLCD_C_JS_JX
  #include "HTLCD_lib_JS.h"
#endif



/**********************************************************************************
函数功能：获取Sx的seg位置
入口：    ShouID----显示设备ID
出口：    0--------不存在
**********************************************************************************/
INT16U GetPixySegPos_SxNxDx(INT16U DeviceID,INT8U Type)
{
  INT16U i,MaxNum;
  //void *Ptr;
  
  switch(Type)
  {
    case Sx:
      MaxNum=MAX_S_NUM;
      for(i=0;i<MaxNum;i++)
      {
         if(LCD_S_Const[i].Sign==DeviceID)
          {
            return i;
          } 
      }
    break;
    
    case Dx:
      MaxNum=MAX_D_NUM;
      for(i=0;i<MaxNum;i++)
      {
         if(LCD_D_Const[i].Sign==DeviceID)
          {
            return i;
          } 
      }
    break;
    
    case Nx:
      MaxNum=MAX_N_NUM;
      for(i=0;i<MaxNum;i++)
      {
         if(LCD_N_Const[i].Sign==DeviceID)
          {
            return i;
          } 
      }
    break;
    
    default:
    return 0;
  }
  
  /*for(i=0;i<MaxNum;i++)
  {
     if((*)(Ptr[i].Sign==DeviceID)
      {
        return i;
      } 
  }*/
    return 0;
}
/**********************************************************************************
函数功能：点亮或者熄灭某一个显示标志
入口：    Flag----1:显示;0:熄灭
出口：    1---------有此设备；0------找不到设备
**********************************************************************************/
INT8U SetOrClrAndChkSEGCOM(INT16U Seg,INT8U Com,INT8U Type,INT8U ClrOrSet)
{
  if((Seg<=MAX_SEG)&&(Com<=MAX_COL))
  {    
    if(ClrOrSet)
    {
      if(GET_BIT(Show_Lcd_Ram[Seg],Com))  //检查内存是否已经置位或者 seg/com配置表错误
      return 0;
      
      Show_Lcd_Flag=1;
      SET_BIT(Show_Lcd_Ram[Seg],Com);
      return 1;
    }
    else
    {
    	CLR_BIT(Show_Lcd_Ram[Seg],Com);
    	return 1;
    }  
  }
  return 0;
}
/**********************************************************************************
函数功能：点亮或者熄灭某一个显示标志
入口：    Flag----1:显示;0:熄灭
出口：    1---------有此设备；0------找不到设备
**********************************************************************************/
INT8U SetOrClrPixPiyDot(INT16U ShouID,INT8U ClrOrSet,INT8U Type)
{
  INT16U Pos,Seg,Com;
  INT8U Flag,i,temp;
  
  Pos=GetPixySegPos_SxNxDx(ShouID,Type);
  if(!Pos)
    return 0;
  
  switch(Type)
  {
    case Sx:
       Seg=SEG(LCD_S_Const[Pos].x);
       Com=COM(LCD_S_Const[Pos].y);
       Flag=SetOrClrAndChkSEGCOM(Seg,Com,Type,ClrOrSet);
       return Flag;
  
    case Dx:
       Seg=SEG(LCD_D_Const[Pos].x);
       Com=COM(LCD_D_Const[Pos].y);
       Flag=SetOrClrAndChkSEGCOM(Seg,Com,Type,ClrOrSet);
       return Flag;

  case Nx: 
//#ifdef CONFLICT_TAB_CHKEN
//	temp=7;
//#else
	temp=1;
//#endif
      for(i=0;i<temp;i++)
      { 
        Seg=SEG(LCD_N_Const[Pos+i].x);
        Com=COM(LCD_N_Const[Pos+i].y);
      	Flag=SetOrClrAndChkSEGCOM(Seg,Com,Type,ClrOrSet);
      	if(!Flag)
      	return 0;
      }
      return 1;

			
      
	}

  return 0;
}
/**********************************************************************************
函数功能：点亮某一个显示设备
入口：    无
出口：    无
**********************************************************************************/
INT8U SetOnDevice_PUCK(INT16U Device)
{
  INT8U Flag;
  Flag=SetOrClrPixPiyDot(Device,1,Sx);
  return Flag;
}
/**********************************************************************************
函数功能：熄灭某一个显示设备
入口：    无
出口：    无
**********************************************************************************/
INT8U SetOnDevice_D_PUCK(INT16U Device)
{
  INT8U Flag;
  Flag=SetOrClrPixPiyDot(Device,1,Dx);
  return Flag;
}
/**********************************************************************************
函数功能：点亮或者熄灭某'8'中的某一个元素:'A'~'G'
入口：    
         offset----具体的设备对应的偏移
         Bit---------------清除还是显示的具体位:0~6('A'~'G')
出口：   1-----------成功;0-------------失败
**********************************************************************************/
INT8U SetOnPixyLED8(INT16U Device,INT8U DisChar)
{
  INT16U Seg,Com,Pos;
  
    //找合理位置
  Pos=GetPixySegPos_SxNxDx(Device,Nx);
  if(!Pos)
    return 0;
    
  if(Pos>=MAX_N_NUM) //防止越界
    return 0;
  
  //检查显示的字段是否合法
  if(DisChar<'A')
    return 0;
  
  if(DisChar>'G')
    return 0;
  
  DisChar=DisChar-'A';
  
  Seg=SEG(LCD_N_Const[Pos+DisChar].x);
  Com=COM(LCD_N_Const[Pos+DisChar].y);
    
  if((Seg<=MAX_SEG)&&(Com<=MAX_COL)) //防止越界
  {
     Show_Lcd_Flag=1;
     SET_BIT(Show_Lcd_Ram[Seg],Com);
  }
  return 1;
}

/**********************************************************************************
函数功能：点亮0~9,A,b,C,d,E,F,...等具有意义的字符，如"9:","2."
入口：    
         DeviceID----具体的'8'设备名称
         DisChar----------点亮0~9,A,b,C,d,E,F,...等具有意义的字符
出口：   1-----------成功;0-------------失败
**********************************************************************************/
INT8U SetOnLED8Device_PUCK(INT16U DeviceID,INT8U DisChar)
{
  INT8U i,CharPos;
  
  //检索LED8显示的内容是否合法：0~9，A,b,C,d,E,F,o,O,...
  for(CharPos=0,i=0;i<MAX_LED_NUM;i++)
  {
    if(LED_Pixy8_Const[i].Type_8==DisChar)
    {
      CharPos=i;
      break;
    }
  }
  //没有找到显示的字符
  if(!CharPos)
    return 0;
  
  //根据 LED_Pixy8_Const 描笔
  for(i=0;i<LED_Pixy8_Const[CharPos].Num;i++)
  {
    SetOnPixyLED8(DeviceID,LED_Pixy8_Const[CharPos].Pxy8[i]);
  }
  return 1;
}

/**********************************************************************************
函数功能：更新显示屏
入口：    Flag----1:表示初始化;0:表示进入主程序调用
出口：    无
**********************************************************************************/
void UpdataLcdShow(void)
{
  if(Show_Lcd_Flag)
  {
    
    //mem_set(Show_Lcd_Ram,0xff,sizeof(Show_Lcd_Ram),Show_Lcd_Ram,sizeof(Show_Lcd_Ram));    
    //HTLCDWriteMultiData(0,Show_Lcd_Ram,sizeof(Show_Lcd_Ram));
    //mem_set(Show_Lcd_Ram,0x0f,sizeof(Show_Lcd_Ram),Show_Lcd_Ram,sizeof(Show_Lcd_Ram));
    HTLCDWriteMultiData(0,Show_Lcd_Ram,sizeof(Show_Lcd_Ram));
    Show_Lcd_Flag=0;
    mem_set(Show_Lcd_Ram,0x00,sizeof(Show_Lcd_Ram),Show_Lcd_Ram,sizeof(Show_Lcd_Ram));    
  }
}
/**********************************************************************************
函数功能：清除整屏
入口：    无
出口：    无
**********************************************************************************/
void ClearAllScreen(void)
{
  mem_set(Show_Lcd_Ram,0x00,sizeof(Show_Lcd_Ram),Show_Lcd_Ram,sizeof(Show_Lcd_Ram));    
  Show_Lcd_Flag=1;
  UpdataLcdShow();
}
/**********************************************************************************
函数功能：填满整屏
入口：    无
出口：    无
**********************************************************************************/
void FillAllScreen(void)
{
  mem_set(Show_Lcd_Ram,0xff,sizeof(Show_Lcd_Ram),Show_Lcd_Ram,sizeof(Show_Lcd_Ram));    
  Show_Lcd_Flag=1;
  UpdataLcdShow();
  //OS_TimeDly_Ms(150);//500ms睡眠
}
/**********************************************************************************
void Show_9_To_0(void)
{  
  INT16U i;
  for(i=1;i<43;i++)
      SetOnDevice_PUCK(i); 
      
  for(i=3;i<13;i++)   
  {
  	SetOnLED8Device_PUCK(N_LED(i),'1'+i-3);
  }
  
  for(i=14;i<22;i++)   
  {
  	SetOnLED8Device_PUCK(N_LED(i),'2'+21-i);
  	if(i==20)
  		SetOnDevice_D_PUCK(D_(20));
  } 
  UpdataLcdShow();  
}
**********************************************************************************/

/**********************************************************************************
函数功能：初始化显示RAM
入口：    无
出口：    无
**********************************************************************************/
void InitLcdAllRam(void)
{
  
 //mem_set(Show_Lcd_Ram,0x00,MAX_SEG+1,Show_Lcd_Ram,MAX_SEG+1);
  //mem_set(Show_Lcd_Ram,0x00,sizeof(Show_Lcd_Ram),Show_Lcd_Ram,sizeof(Show_Lcd_Ram));
  //mem_set(void *pDst, INT8U Value,INT32U SetLen,void *pDst_Start,INT32U Dst_Len)
   mem_set(Show_Lcd_Ram,0x00,sizeof(Show_Lcd_Ram),Show_Lcd_Ram,sizeof(Show_Lcd_Ram)); 
}
/**********************************************************************************
函数功能：主窗口显示
入口：    无
出口：    无
**********************************************************************************/
void Main_Dis_Info(char *Str)
{
    INT8U i,len;
    len=strlen(Str);
    len=(len>10)?10:len;
      
    for(i=3;i<=len+2;i++)
      SetOnLED8Device_PUCK(N_LED(i),Str[i-3]);
    
    UpdataLcdShow();
}
/**********************************************************************************
函数功能：检查配置表是否配置矛盾
入口：    无
出口：    无
**********************************************************************************/
void Chk_Table_Conflict(void)
{
  
#ifdef CONFLICT_TAB_CHKEN
  INT16U i;
  INT8U Flag,Result=1;

  DEBUG_PRINT(PUCK,1,"Check LCD Soft Table Conflict.......................");
  for(i=1;i<MAX_S_NUM;i++)
  {
    Flag=SetOrClrPixPiyDot(LCD_S_Const[i].Sign,1,Sx);
    if(!Flag)
    {
      Result=0;
     DEBUG_PRINT(PUCK,1,"LCD_TABLE Error---->Config Table Error,Sign=S%d\n",LCD_S_Const[i].Sign); 
    }
  }
  
  for(i=1;i<MAX_D_NUM;i++)
  {      
    Flag=SetOrClrPixPiyDot(LCD_D_Const[i].Sign,1,Dx);
    if(!Flag)
    {
      Result=0;
     DEBUG_PRINT(PUCK,1,"LCD_TABLE Error---->Config Table Error,Sign=D%d\n",LCD_D_Const[i].Sign); 
    }
  } 
  
  for(i=1;i<MAX_N_NUM;i+=7)  //7个seg为一个单位
  {      
    Flag=SetOrClrPixPiyDot(LCD_N_Const[i].Sign,1,Nx);
    if(!Flag)
    {
      Result=0;
     DEBUG_PRINT(PUCK,1,"LCD_TABLE Error---->Config Table Error,Sign=N%d\n",LCD_N_Const[i].Sign); 
    }
  }
  if(Result)
    DEBUG_PRINT(PUCK,1,"LCD Soft Table no Conflicet,Succeed!");
  else
    DEBUG_PRINT(PUCK,1,"LCD Soft Table Conflicet,Failed!");
  
  InitLcdAllRam();
#endif
}
/**********************************************************************************
**********************************************************************************/
void Test_Lcd_Proc(void)
{
  INT8U i,j;  
  for(i=0;i<=9;i++)
  {
    for(j=0;j<=i;j++)
    {
      SetOnLED8Device_PUCK(N_LED(12-j),'0'+i-j);
      //SetOnLED8Device_PUCK(N_LED(13+j),'0'+i-j);
    }  
    UpdataLcdShow();
    OS_TimeDly_Ms(80);
  }
  OS_TimeDly_Sec(1);
}
/**********************************************************************************
函数功能：HTLCD处理程序
入口：    Flag----1:表示初始化;0:表示进入主程序调用
出口：    无
**********************************************************************************/
void MainHTLCD_Puck_Pub(INT8U Flag)
{ 
    //InitLcdAllRam();
    Chk_Table_Conflict();
   
    if(Flag)
    {
      Turn_Light_On();
      FillAllScreen();
      //Test_Lcd_Proc();
    }
}
/**********************************************************************************
函数功能：上电后，强行刷掉LCD显示
入口：
出口：
**********************************************************************************/
void Clr_Screen_PUCK(void)
{ 
  //Turn_Light_Off();
  Init_HTLCD_Mode(SYS_SLEEP);
  //ClearAllScreen();  
}
