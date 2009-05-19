#define INTER_ABS_PUCK
#include "Pub_PUCK.h"


//初始化中断相关变量
void Init_Interr_Sram(void)
{
  
  mem_set((void *)(&Key_Value_Pub),0,sizeof(Key_Value_Pub),\
               (void *)(&Key_Value_Pub),sizeof(Key_Value_Pub));   
  INIT_STRUCT_VAR(Key_Value_Pub);
  
  mem_set((void *)(&Irda_Wake_Ctrl),0,sizeof(Irda_Wake_Ctrl),\
               (void *)(&Irda_Wake_Ctrl),sizeof(Irda_Wake_Ctrl));   
  INIT_STRUCT_VAR(Irda_Wake_Ctrl);

  mem_set((void *)(&Irda_Decode_Ctrl),0,sizeof(Irda_Decode_Ctrl),\
        (void *)(&Irda_Decode_Ctrl),sizeof(Irda_Decode_Ctrl));
  INIT_STRUCT_VAR(Irda_Decode_Ctrl);
  
  mem_set((void *)(&Pulse_Num_Temp),0,sizeof(Pulse_Num_Temp),\
        (void *)(&Pulse_Num_Temp),sizeof(Pulse_Num_Temp));
  INIT_STRUCT_VAR(Pulse_Num_Temp); 
  
  Irda_WkUP_Ms_Timr=Ms_Timer_Pub;
  Key_WkUP_Ms_Timr=Ms_Timer_Pub;
  
}



//有功脉冲计数,中断等级--------INTER_GRADE_LOWERST
void Inter_CF1(void)
{
  EI();
  if(CHECK_STRUCT_VAR(Pulse_Num_Temp))
  {
    if(Pri_MeasuStatVar.MeasuIntOk)
    {
      Pulse_Num_Temp.Ac_Pulse_Num++;  
      if(Pulse_Num_Temp.Ac_Pulse_Num>=Sys_Pulse_Var.Ac_Pulse_Freq)
      {   
        Pulse_Num_Temp.Ac_Pulse_Num-=Sys_Pulse_Var.Ac_Pulse_Freq;
        Send_Acpower_Pulse();
      }
    }
  }
  else                            //头尾不对
  {
    //Port_Out_Pub(INTER_ID_ALARM_BEEP,BEEP_MODE_200);
    Pulse_Num_Temp.Ac_Pulse_Num=0;
    INIT_STRUCT_VAR(Pulse_Num_Temp);  //把头尾填齐
  }
}

//无功脉冲计数,中断等级--------INTER_GRADE_LOWERST
void Inter_CF2(void)
{
  EI();
  if(CHECK_STRUCT_VAR(Pulse_Num_Temp))
  {
    if(Pri_MeasuStatVar.MeasuIntOk)
    {
      Pulse_Num_Temp.Rea_Pulse_Num++;
      if(Pulse_Num_Temp.Rea_Pulse_Num>=Sys_Pulse_Var.Reac_Pulse_Freq)
      {   
        Pulse_Num_Temp.Rea_Pulse_Num-=Sys_Pulse_Var.Reac_Pulse_Freq;
        Send_Reacpower_Pulse();
      }
    }
  }
  else                            //头尾不对
  {
    //Port_Out_Pub(INTER_ID_ALARM_BEEP,BEEP_MODE_300);
    Pulse_Num_Temp.Rea_Pulse_Num=0;
    INIT_STRUCT_VAR(Pulse_Num_Temp);  //把头尾填齐    
  }
}

//编程按钮,中断等级--------INTER_GRADE_LOWERST
void Inter_Prg_Key(void)  
{
  EI();
  Key_Value_Pub.Key.Bit.PrgKey=!Key_Value_Pub.Key.Bit.PrgKey;
}


//left key：实际是功能键,中断等级--------INTER_GRADE_LOWERST
void Inter_Left_Key(void)
{
  EI();
  Key_Value_Pub.Key.Bit.LeftKey=1;  
}


//Down Key,中断等级--------INTER_GRADE_LOWERST
void Inter_Down_Key(void)
{
  EI();
  Key_Value_Pub.Key.Bit.DownKey=1;
}

//Right key,中断等级--------INTER_GRADE_LOWERST
void Inter_Right_Key(void)
{
  EI();
  Key_Value_Pub.Key.Bit.RightKey=1;
}


//UP Key,中断等级--------INTER_GRADE_LOWERST
void Inter_Up_Key(void)
{
  EI();
  Resume_Src.Src_Flag|=KEY_RESUME;
  Key_WkUP_Ms_Timr=Ms_Timer_Pub;
  Key_Value_Pub.Key.Bit.UpKey=1;
}


/********************************************************************************
函数功能：获取当前表的键值；
入口：Id -----只能填写LUCK，其他无效！；
返回：
Bit7	Bit6    Bit5   Bit4	      Bit3	Bit2	Bit1	Bit0
0	0	0      AB间切换键	右	左	下	上
注意：读完后将清除此状态字
在非C模式下，左 右键，即是C模式切换键。
********************************************************************************/
INT8U Get_MeterKey_PUCK(INT8U Id)
{
  INT8U temp;
  if(Id!=LUCK)
    return 0;
  
#ifndef C_MODE_EN   //按钮的C模式不开放！
  Key_Value_Pub.Key.LeftKey=0;
  Key_Value_Pub.Key.RightKey=0;
#endif
  
  temp=Key_Value_Pub.Key.Byte&0x7f;  
  Key_Value_Pub.Key.Byte&=0x80;
  if(Check_AB_Switch())
    SET_BIT(temp,4);   //AB模式切换

  return temp;
}


extern void Power_Down_Proc();
//掉电,中断等级--------INTER_GRADE_LOW
void POWER_DOWN(void)
{
  EI();
  Power_Down_Proc();
}


//激光点亮,中断等级--------INTER_GRADE_LOWERST
void laser_up(void)
{
  EI();
  TURN_ON_LIGHT;
}

//全失压,中断等级--------INTER_GRADE_LOWERST默认(等级：INTTM05_vect)
void Inter_ALL_LOSS(void) //正常模式下，此中断关闭，只有在sleep下中断打开
{
  EI();
  Clear_CPU_Dog();
  //Resume_Src.Src_Flag|=ALL_LOSS_VOL_RESUME;
  All_Loss_Var.Status.Occur=1; //全失压置位，RTC闹铃中清零
  SET_VAR_CS_PUCK(All_Loss_Var.Status); 
  Clear_CPU_Dog();
}



//红外唤醒,中断等级--------INTER_GRADE_LOW(默认：INTP3_vect) 
void Irda_Wake_Up(void)  //正常模式下，此中断关闭，只有在sleep和resume下中断打开
{
  EI();
  if(CHECK_STRUCT_VAR(Irda_Wake_Ctrl)==0)//头尾不对
  {
    //Port_Out_Pub(INTER_ID_ALARM_BEEP,BEEP_MODE_400);  beep无供电，故去掉
    Irda_Wake_Ctrl.Start=0;
    Irda_Wake_Ctrl.WakeUp=0;
    Irda_Wake_Ctrl.PulseNum=0;    
    INIT_STRUCT_VAR(Irda_Wake_Ctrl);  //把头尾填齐
    STOP_FAST_TIMER;
    return ;
  }
  if(1==Irda_Wake_Ctrl.WakeUp)  //已经唤醒
  {
    STOP_IRDA_WAKE;           //确定已经唤醒，以后关闭唤醒脚的中断 
    return ;
  }
  
  if(0==Irda_Wake_Ctrl.Start)  //红外唤醒检测开始
  {
    Irda_Wake_Ctrl.Start=1;
    Irda_Wake_Ctrl.PulseNum=0;
    Fast_Timer_Reg=0;
    START_TIMER_1mS; 
    return ;
  } 
  
  if(1==Irda_Wake_Ctrl.Start)  //红外唤醒检测继续
  {
    Irda_Wake_Ctrl.PulseNum++;
  }
   
  if((Fast_Timer_Reg<=IRDA_WAKE_UP_MS)&&(Irda_Wake_Ctrl.PulseNum>=IRDA_WAKE_UP_NUM))
  {
    Irda_Wake_Ctrl.Start=0;
    Irda_Wake_Ctrl.PulseNum=0;
    Irda_Wake_Ctrl.WakeUp=1;

    STOP_FAST_TIMER;              
    STOP_IRDA_WAKE;           //确定已经唤醒，以后关闭唤醒脚的中断
    Fast_Timer_Reg=0;
    Resume_Src.Src_Flag|=IRAD_RESUME;
    Irda_WkUP_Ms_Timr=Ms_Timer_Pub;
  }
}


//1000us/10us中断脉冲,中断等级--------INTER_GRADE_LOW(默认：INTTM06_vect)
void Fast_Timer(void)
{
  EI();
  if(TDR06==0x07)   //单位：976.5625us,程序慢速晶振跑
  {
    Fast_Timer_Reg++;
    
    if(1==Irda_Wake_Ctrl.WakeUp)  //已经唤醒
    {
      STOP_IRDA_WAKE;           //确定已经唤醒，以后关闭唤醒脚的中断 
      STOP_FAST_TIMER;
      Fast_Timer_Reg=0;
      return ;
    }
  
    if(Fast_Timer_Reg>IRDA_WAKE_UP_MS)   //时间超限,关闭定时器，由唤醒脚来启动
    {
      Fast_Timer_Reg=0;
      STOP_FAST_TIMER;
      Irda_Wake_Ctrl.Start=0;
      Irda_Wake_Ctrl.WakeUp=0;
      Irda_Wake_Ctrl.PulseNum=0;
      return ;
    }
  }
  
  if(TDR06==0x6d || TDR06==0x36)   //单位：9.94647us,程序快速晶振跑
  {
    Fast_Timer_Reg++;
    if(Fast_Timer_Reg>=9000 || (0==INTER_DECODE_EN))  //时间超限，或者解码中断已经关闭！
    {
      Fast_Timer_Reg=0;
      STOP_FAST_TIMER;
      
      Irda_Decode_Ctrl.Start=0;
      Irda_Decode_Ctrl.Done=0;
      Irda_Decode_Ctrl.Index=0;
      Irda_Decode_Ctrl.TrigTimer=0;
    }
  }
}
//遥控器解码:所有碼接收时间 64.02ms左右,中断等级--------INTER_GRADE_LOW(默认：INTP5_vect)
void IR_Decode_Proc(void)
{
  EI();
  if(Irda_Decode_Ctrl.Done)
   return ;
  
  if(Irda_Decode_Ctrl.Start==0 && Irda_Decode_Ctrl.TrigTimer==0)  //启动快速Timer
  {
    START_TIMER_10uS;
    Fast_Timer_Reg=0;
    
    Irda_Decode_Ctrl.TrigTimer=1;
    Irda_Decode_Ctrl.Done=0;
    Irda_Decode_Ctrl.Index=0; 
    return ;
  }

  if(Irda_Decode_Ctrl.TrigTimer && TMMK06)  //意外将Timer关闭，又不能开启
  {
    Irda_Decode_Ctrl.Start=0;
    Irda_Decode_Ctrl.Done=0;
    Irda_Decode_Ctrl.Index=0;
    Irda_Decode_Ctrl.TrigTimer=0;
    return ;
  }
  if(Irda_Decode_Ctrl.Start==0)
  {
    if(Fast_Timer_Reg>=1250 && Fast_Timer_Reg<=1450)
    {
      Irda_Decode_Ctrl.Start=1;
      Fast_Timer_Reg=0;
    }
  }
  else   //已经发现启动信号
  {
    if(Fast_Timer_Reg>=185 && Fast_Timer_Reg<=240)
    {
        if(Irda_Decode_Ctrl.Index<CODE_NUM*8)
        {
          SET_BIT(Irda_Decode_Ctrl.CodeBuf[Irda_Decode_Ctrl.Index/8],Irda_Decode_Ctrl.Index%8);
          Irda_Decode_Ctrl.Index++;
          Fast_Timer_Reg=0;
        }
    }
    else if(Fast_Timer_Reg>=70&&Fast_Timer_Reg<=130)
    {
        if(Irda_Decode_Ctrl.Index<CODE_NUM*8)
        {
          CLR_BIT(Irda_Decode_Ctrl.CodeBuf[Irda_Decode_Ctrl.Index/8],Irda_Decode_Ctrl.Index%8);
          Irda_Decode_Ctrl.Index++;
          Fast_Timer_Reg=0;
        }
     } 
    else  //乱码，解码出错了
    {
      Irda_Decode_Ctrl.Start=0;
      Irda_Decode_Ctrl.Done=0;
      Irda_Decode_Ctrl.Index=0;
      Irda_Decode_Ctrl.TrigTimer=0;
      STOP_FAST_TIMER;
      Fast_Timer_Reg=0;
    }
    if(Irda_Decode_Ctrl.Index>=CODE_NUM*8)	
    {
      Irda_Decode_Ctrl.Start=0;
      Irda_Decode_Ctrl.TrigTimer=0;
      Irda_Decode_Ctrl.Done=1;
      STOP_FAST_TIMER;
      Fast_Timer_Reg=0;
    } 
  }      
}
//1秒脉冲,中断等级--------INTER_GRADE_LOWERST
void INT_1HZ(void)
{
  EI();
  ;//Port_Out_Pub(EXT_ID_SEC_EN,Sys_Pulse_Var.Pulse_Width);
}

//内部RTC闹铃,中断等级--------INTER_GRADE_LOWERST (默认：INTRTC_vect)
void CPU_RTC_Interrupt(void)
{
  EI();
  Clear_CPU_Dog();
  //Resume_Src.Src_Flag|=RTC_RESUME;
  //Refresh_CPU_RTC();
  Count_All_Loss_Proc();
  Clear_CPU_Dog();
}

//系统1ms的Tick,中断等级--------INTER_GRADE_LOWERST(默认：INTTM07_vect)
void count_1ms(void)
{ 
  EI();
  static INT8U Num=0,num1=0,Ms_Timer=0;
  
  Ms_Timer_Pub++;
  
  Ms_Timer++;
  if(Ms_Timer>=OS_MS_PER_TICK)
  {
    Ms_Timer=0;    
    OS_Tick_ISR();
  }
  
  num1++;
  if(num1>=10)
  {
    Ms10_Timer_Pub++;
    num1=0;
  }  
    
  Num++;
  if(Num>=MS_HOOK_NUM)
  {
    ExtPort_xMs_Hook();
    Num=0;
  }     
}
