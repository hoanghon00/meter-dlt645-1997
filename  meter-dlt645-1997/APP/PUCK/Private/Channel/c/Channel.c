#define CHANNEL_C
#include "Pub_PUCK.h"


/********************************************************************************

*********************************************************************************/
INT32U Get_Baud_Spec(INT8U Baud)
{
  if(Baud&0x01)
    return 1200;
  
  if(Baud&0x02)
    return 300;
  
  if(Baud&0x04)
    return 600;
  
  if(Baud&0x10)
    return 2400;
  
  if(Baud&0x20)
    return 4800;
  
  if(Baud&0x40)
    return 9600;
  
    return 1200;  //默认1200
}


/********************************************************************************
函数原型：void Init_All_Cannel(void)
功能：初始化所有通道
*********************************************************************************/
void Init_All_Cannel(void)
{
  INT16U Len;
  INT8U temp,Err;
  
  Len=Read_Storage_Data(DI_CHANEL_485_1,&temp,&temp,1,&Err);
  if((Len==1)&&(NO_ERR==Err))
  {
      Chanel_Para[CHANEL_485_1].Baud=Get_Baud_Spec(temp);
      SET_STRUCT_SUM(Chanel_Para[CHANEL_485_1]);
  }
    
  Len=Read_Storage_Data(DI_CHANEL_485_2,&temp,&temp,1,&Err);
  if((Len==1)&&(NO_ERR==Err))
  {
    Chanel_Para[CHANEL_485_2].Baud=Get_Baud_Spec(temp);
    SET_STRUCT_SUM(Chanel_Para[CHANEL_485_2]);
  }
  
  if(CHANEL_IRDA!=CHANEL_DEBG)    //调试口单独初始化了
    Open_Channel_PUCK(CHANEL_IRDA,Chanel_Para[CHANEL_IRDA].Baud,SAU_PARITY_EVEN);
    
  if(CHANEL_485_1!=CHANEL_DEBG)     //调试口单独初始化了
    Open_Channel_PUCK(CHANEL_485_1,Chanel_Para[CHANEL_485_1].Baud,SAU_PARITY_EVEN);  
    
  if(DI_CHANEL_485_2!=CHANEL_DEBG)  //调试口单独初始化了
     Open_Channel_PUCK(CHANEL_485_2,Chanel_Para[CHANEL_485_2].Baud,SAU_PARITY_EVEN); 
  
#ifdef CHANEL_MOUDEL 
  Chanel_Para[CHANEL_MOUDEL].Baud=BAUD_MOUDLE;
  SET_STRUCT_SUM(Chanel_Para[CHANEL_MOUDEL]);
  if(CHANEL_MOUDEL!=CHANEL_DEBG)  //调试口单独初始化了
     Open_Channel_PUCK(CHANEL_MOUDEL,Chanel_Para[CHANEL_MOUDEL].Baud,SAU_PARITY_EVEN); 
#endif
}
/********************************************************************************
函数原型：INT8U  Send_HostData_Public_Puck(INT8U Channl,INT8U *Srcbuf,INT16U len)
函数功能：向某通道发送数据
入口参数：Channl,通道号
          Srcbuf，发送的数据起始地址 
          Src_Len,源数据的长度
出口参数：1：发送成功；0：发送失败
*********************************************************************************/
INT8U  Send_HostData_Public_Puck(INT8U Channl,INT8U *Srcbuf,INT16U Src_Len)
{
  INT8U Flag;
  
  /*645一包最小不能低于10字节
  Flag=ASSERT(A_WARNING,Src_Len>=10);
  if(Flag)
    return 0;
  */
  
  Flag=Channel_DataSend_PUCK(Channl,Srcbuf,Src_Len);  
  return Flag;
}
/********************************************************************************
函数原型：INT8U  Send_HostData_Public_Puck(INT8U Channl,INT8U *Srcbuf,INT16U len)
函数功能：向某通道发送数据
入口参数：Channl,通道号
          Srcbuf，发送的数据起始地址 
          Src_Len,源数据的长度
出口参数：1：发送成功；0：发送失败
*********************************************************************************/
INT8U  Debug_Out_Public_Puck(INT8U *Srcbuf,INT16U Src_Len)
{
 INT8U Flag;
 Flag=Channel_DataSend_PUCK(CHANEL_DEBG,Srcbuf,Src_Len);  
  return Flag;
}
/********************************************************************************
函数原型：void DataRec_Monitor_PUCK(void)
功能：提取所有通道的645数据

*********************************************************************************/
void Channel_Data_Proc_PUCK(INT32U Status)
{
  INT8U i;
  
  for(i=CHANEL_MIN;i<=CHANEL_MAX;i++)
  {      
      Channel_DataReceive_PUCK(i,Status);     
  }
  Remote_Key_Proc();   //resume模式下，还可以工作
}

void Debug_Channel_Switch(INT32U Mode)
{    
  if((Check_Debug_En()==1)&&(Chanel_Para[CHANEL_IRDA].Baud!=BAUD_DEBG))  //将该口切换至调试口
  {
    Chanel_Para[CHANEL_DEBG].Baud=BAUD_DEBG;
    Open_Channel_PUCK(CHANEL_DEBG,BAUD_DEBG,SAU_PARITY_NONE);
    Debug_Out_Public_Puck("\r\nPUCK:Switch To Debug CHannel!",31);
    DIS_UART0_REC;        //调试模式下，关闭UART0口的接收，及其中断
    SET_STRUCT_SUM(Chanel_Para[CHANEL_IRDA]);
  }
  else
  {
    if((Check_Debug_En()==0)&&(Chanel_Para[CHANEL_IRDA].Baud!=BAUD_IRDA))  //将该口切换至工作口
    {
      if(SYS_NORMAL==Mode)
      {        
        Chanel_Para[CHANEL_IRDA].Baud=BAUD_IRDA;
        Open_Channel_PUCK(CHANEL_IRDA,BAUD_IRDA,SAU_PARITY_EVEN);
        SET_STRUCT_SUM(Chanel_Para[CHANEL_IRDA]); 
      }
      if(SYS_RESUME==Mode &&Check_Resume_Source(IRAD_RESUME)) //红外唤醒模式
      {
        if(IRDA_READ_METER_EN)   //在key已经唤醒下，红外唤醒，HUCK不会调用 Init_ExtDevice_PUCK()
        {
          //IRDA_FAR_REC_EN;              //红外接收开启
          BAK_POWER_FOR_IRDA;            //远红外电源由低功耗电池供给;
          //Debug_Out_Public_Puck("\r\nPUCK:Switch To IRDA CHannel!",30);
          Chanel_Para[CHANEL_IRDA].Baud=BAUD_IRDA;
          Open_Channel_PUCK(CHANEL_IRDA,BAUD_IRDA,SAU_PARITY_EVEN);
          SET_STRUCT_SUM(Chanel_Para[CHANEL_IRDA]); 
        }
      }
    }  
  } 
}
/********************************************************************************
函数原型：void Channel_Switch_PUCK(INT32U Mode)
功能：通道的切换：根据工厂状态或出厂状态，调整串口的作用。

*********************************************************************************/
void Channel_Switch_PUCK(INT32U Mode)
{ 
  if(CHANEL_DEBG==CHANEL_IRDA)
  {
     Debug_Channel_Switch(Mode);
  }  
}
/********************************************************************************
函数原型：void Monitor_Para_Modi_PUCK(void)
功能：对所有参数进行维护，包括计量、通道、事件等。

*********************************************************************************/
void Para_Modi_Monitor_PUCK(void)
{
  INT16U temp;
  
  while(Get_Para_Modify(&temp))
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_CHANNEL_EN,"Para Modified In Channel Task:0x%x",temp);
    switch(temp)
    {
      case DI_CHANEL_485_1:  //第一路485参数修改
      case DI_CHANEL_485_2:   //第二路485参数修改
          Init_All_Cannel();
          DEBUG_PRINT(PUCK,PRINT_PUCK_CHANNEL_EN,"CHANEL_485_1/CHANEL_485_2 Baud Changed");
      break; 
      default:   //以下参数做优化
      if(((temp>=VARHMODE1)&&(temp<=VARHMODE2))||((temp>=ROLLCYCLE)&&(temp<=POWERBITS))||(temp==NUMBMODEB))
        Lcd_Para_Modi(temp);                              //液晶参数发生修改  
        break;
    }    
  }   
}

/********************************************************************************
函数原型：void Init_All_UART(void)
功能：初始化所有通道
*********************************************************************************/
void Init_All_UART(INT32U Mode)
{
  INT8U i;
  switch(Mode)
  {
  case SYS_NORMAL:
    Init_DebugChanel_PUCK(0);         //调试口初始化
    Init_All_Cannel();
    break;
    
  case SYS_RESUME:    //只可能是红外、按钮、红外光唤醒，其他唤醒不会进入此函数----------PUCK
    for(i=CHANEL_MIN;i<=CHANEL_MAX;i++)
    {
      if(i!=CHANEL_DEBG)   //不是调试口
        Close_Channel_PUCK(i); 
      else             //是调试口
        Init_DebugChanel_PUCK(1);
    }
      
    break;    
  case SYS_SLEEP:
   //关闭所有串口
    for(i=CHANEL_MIN;i<=CHANEL_MAX;i++)
       Close_Channel_PUCK(i);   //关闭全部串口------------PUCK
    break;
    
  }
}

/********************************************************************************
函数原型：void Init_DebugChanel_PUCK(INT8U FacFlag)
功能：初始化调试口通道
FacFlag-----1:需要判定工厂状态，0--------不需要
*********************************************************************************/
void Init_DebugChanel_PUCK(INT8U FacFlag)
{
  if(POWER_OFF_STATUS==0)  //掉电了
    return ;
  
  if(CHANEL_DEBG==CHANEL_IRDA)
  {
    Chanel_Para[CHANEL_DEBG].Baud=BAUD_DEBG;
    SET_STRUCT_SUM(Chanel_Para[CHANEL_DEBG]);    
    if(FacFlag)         //需要判定工厂状态
    {
      if(Check_Debug_En())  //是工厂状态打印)
        Open_Channel_PUCK(CHANEL_DEBG,BAUD_DEBG,SAU_PARITY_NONE); 
    }
    else               //不需要判定工厂状态     
        Open_Channel_PUCK(CHANEL_DEBG,BAUD_DEBG,SAU_PARITY_NONE); 
  }  
}
/********************************************************************************
函数原型：void Close_DebugChanel_PUCK(INT8U FacFlag)
功能：初始化调试口通道
FacFlag-----1:需要判定工厂状态，0--------不需要
*********************************************************************************/
void Close_DebugChanel_PUCK(INT8U FacFlag)
{
  if(CHANEL_DEBG==CHANEL_IRDA)
  {
    Chanel_Para[CHANEL_DEBG].Baud=0;
    SET_STRUCT_SUM(Chanel_Para[CHANEL_DEBG]);    
    if(FacFlag)         //需要判定工厂状态
    {
      if(Check_Debug_En())  //是工厂状态打印)
        Close_Channel_PUCK(CHANEL_DEBG); 
    }
    else               //不需要判定工厂状态     
        Close_Channel_PUCK(CHANEL_DEBG); 
  }  
}
/**********************************************************************************
函数功能：检查通信参数结构体的校验和
入口：无
出口：无
**********************************************************************************/	
void Check_Channel_ParaCS(void)
{
  INT8U i,Flag;
  
  Flag=0;
  
  for(i=CHANEL_MIN;i<=CHANEL_MAX;i++)
  {
    Flag=CHECK_STRUCT_SUM(Chanel_Para[i]); 
    ASSERT(A_WARNING,1==Flag);
    if(!Flag)
    DEBUG_PRINT(PUCK,PRINT_PUCK_CHANNEL_EN,"Channel_Error----->Channel_Para Struct CS Error In Channel Proc!");
  }

  Flag=CHECK_STRUCT_VAR(S_Buf_PUCK);
  ASSERT(A_WARNING,1==Flag);
}

/**********************************************************************************
函数功能：
入口：无
出口：无
**********************************************************************************/	
void Init_Channel_Sram(void)
{
#ifdef CHANNEL_FAST_EN
  INIT_STRUCT_VAR(Rec_Frame_Status);
#endif
}
/********************************************************************************
函数原型：void Channel_Main_PUCK(INT8U Flag)
函数功能：通信通道处理主函数
*********************************************************************************/
void Channel_Main_PUCK(INT8U Flag)  //10ms调度一次
{
  static volatile S_Int32U Sec_Timer_Bak={CHK_BYTE,0xFFFFFFFF,CHK_BYTE};
  INT32U Status;
  
  Status=Get_Sys_Status();
  if(Status==SYS_SLEEP) //只有在正常模式下，才有事件的处理
    return ;
  
  if(Flag)
  {
    Init_Channel_Sram();
    return ;
  }
  
  Channel_Data_Proc_PUCK(Status);  //10ms调度一次  
  if(Sec_Timer_Bak.Var!=Sec_Timer_Pub)  //正常模式下，1秒搞一次
  {
    if(Status==SYS_NORMAL)
    {
      Para_Modi_Monitor_PUCK();   //1s
      Check_Channel_ParaCS();    //1s
      Check_LCDVar_Cs();    //1s查一次
    }
    Channel_Switch_PUCK(Status);     //1s 
    Sec_Timer_Bak.Var=Sec_Timer_Pub;
  }
}
