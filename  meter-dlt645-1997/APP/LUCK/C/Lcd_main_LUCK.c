
#define LCD_MAIN_C
#include "Pub_Puck.h"


/********************************************************************************
PUCK:
函数功能：设置取参数修改DI，设置跳屏标志
入口：
返回：
********************************************************************************/
void Set_Para_Modi_DI(INT16U Di)
{  
  INT8U i;
  for(i=0;i<MAX_MODI_NUM;i++)
  {
    if(CHECK_STRUCT_SUM(Para_Dis_Var)==0)
       ASSERT(A_WARNING,0);     
    if(Para_Dis_Var.Para_Modi[i]==0)
    {
      Para_Dis_Var.Para_Modi[i]=Di;
      Para_Dis_Var.Flag=1;
      SET_STRUCT_SUM(Para_Dis_Var);
      return ;
    }     
  }
 
}
/********************************************************************************
PUCK:
函数功能：获取参数修改DI
入口：
返回：
********************************************************************************/
INT16U Get_Para_Modi_DI(void)
{
  INT8U i;
  INT16U  temp=0;
  
  if(Para_Dis_Var.Flag==0)
    return 0;
    
  for(i=0;i<MAX_MODI_NUM;i++)
  {
    if(Para_Dis_Var.Para_Modi[i]!=0)
    {
      if(CHECK_STRUCT_SUM(Para_Dis_Var)==0)
        ASSERT(A_WARNING,0);
      temp=Para_Dis_Var.Para_Modi[i];
      Para_Dis_Var.Para_Modi[i]=0;
      SET_STRUCT_SUM(Para_Dis_Var);      
      break;
    }  
  }  
  return temp;
}
/********************************************************************************
PUCK:
函数功能：获取参数修改DI
入口：
返回：
********************************************************************************/
INT8U Get_Para_Modi_Flag(void)
{
  if(Para_Dis_Var.Flag)
  {
    if(CHECK_STRUCT_SUM(Para_Dis_Var)==0)
      ASSERT(A_WARNING,0);
    Para_Dis_Var.Flag=0;
    SET_STRUCT_SUM(Para_Dis_Var);
    return 1;
  }
    return 0; 
}

/********************************************************************************
PUCK:
函数功能：
入口：
返回：
********************************************************************************/

void Clr_LCDSleep_Timer(void)
{
  Sleep_Sec_Timer.Var=Sec_Timer_Pub;
}
/********************************************************************************
PUCK:
函数功能：
入口：
返回：
********************************************************************************/

void LCD_Task_Monitor(void)
{
  if(Get_Sys_Status()==SYS_SLEEP)//任务在睡眠的情况下，不做任何事情 
  {
    Task_Sleep();
  }
   
  if(Get_Sys_Status()==SYS_RESUME)
  {
    if(Sec_Timer_Pub-Sleep_Sec_Timer.Var>=LCD_SLEEP_SEC)
    {
      DEBUG_PRINT(PUCK,PRINT_PUCK_LCD_EN,"Lcd Task Enter To Sleep!"); 
      Task_Sleep();
      Clr_LCDSleep_Timer();
    }
  }
  
}

/********************************************************************************
PUCK:
函数功能：
入口：
返回：
********************************************************************************/
void LCD_Init_PUCK(void)  //LCD显示初始化，满足速度需要
{
  Init_Event_DIS_PUCK();  //初始化事件，供液晶显示
   //一旦进入，说明任务唤醒或者正常模式,正常上电或者按钮红外唤醒,液晶的初始化已经在Init_ExtDevice_PUCK()完成
  if((Get_Sys_Status()==SYS_NORMAL) || (Check_Resume_Source(IRAD_RESUME|KEY_RESUME)))
  {
    Initial();
    Clr_LCDSleep_Timer();
  }
}
/********************************************************************************
PUCK:
函数功能：
入口：
返回：
********************************************************************************/

void LCD_main_LUCK (INT8U Flag)  //10ms调度一次
{
  if(Flag)   //一旦进入，说明任务唤醒或者正常模式
  {
    if(Get_Sys_Status()==SYS_NORMAL)  //正常上电
      MainHTLCD_Puck_Pub(1);
    else if(Check_Resume_Source(IRAD_RESUME|KEY_RESUME))  //唤醒，是红外或者按钮唤醒
      MainHTLCD_Puck_Pub(0);
    else
      Task_Sleep();
    Initial();
    Clr_LCDSleep_Timer();
    return ;
  }
  
  LCD_Task_Monitor();   //工作模式监控
  Dis_Jump_Para();      //跳屏显示
  Key_Fast_LCD_Proc();  //按钮等快速处理
  Loop_Dis_LCD_Proc();  //循显处理
  Init_LCD_Loop();
}
