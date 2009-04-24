#define UART_ABS_PUCK
#include "Pub_PUCK.h"



typedef struct
{ 
  INT16U MaxBufLen;
  INT16U *CurRecLen;
  INT8U  *RecBufPtr;
  void (* const Init_Uart)(INT32U,INT16U);
  void (* const Start_Uart)(void);
  void (* const Stop_Uart)(void);
  INT8U (* const Ready_Uart)(void);
  INT8U (* const Send_Uart)(INT8U *,USHORT);
}CONST_UART_ATTRIB;


//以下按channel.h中定义的通道顺序，描述与UART的应用
const CONST_UART_ATTRIB  Const_Uart_Attib[MAX_UART_NUM]={
  {UART0_RX_BUFF_MAXSIZE,&Uart0_Rx_Count,Uart0_RecBuf,&UART0_Init_PUCK,\
    &UART0_Start,&UART0_Stop,&UART0_RecData_pre_PUCK,&UART0_SendData_PUCK},
   
  {UART2_RX_BUFF_MAXSIZE,&Uart2_Rx_Count,Uart2_RecBuf,&UART2_Init_PUCK,\
    &UART2_Start,&UART2_Stop,&UART2_RecData_pre_PUCK,&UART2_SendData_PUCK},
    
  {UART3_RX_BUFF_MAXSIZE,&Uart3_Rx_Count,Uart3_RecBuf,&UART3_Init_PUCK,\
    &UART3_Start,&UART3_Stop,&UART3_RecData_pre_PUCK,&UART3_SendData_PUCK},
    
#ifdef UART1    
  {UART1_RX_BUFF_MAXSIZE,&Uart1_Rx_Count,Uart1_RecBuf,&UART1_Init_PUCK,\
    &UART1_Start,&UART1_Stop,&UART1_RecData_pre_PUCK,&UART1_SendData_PUCK},
#endif 
};


/********************************************************************************
函数原型：void IRDA_Rec_Enable(void)
功能：根据干簧管状态,使能相应的红外功能
Enable---0:禁止相应的红外接收功能
注意：在低功耗情况下，干簧管的状态判定状态前提，必须是红外的电源开启下调用此函数，否则误判！
*********************************************************************************/
void IRDA_Rec_Enable(void)
{
  if(GUMB_STATUS)  //目前是远红外
  {
     IRDA_FAR_REC_EN;
  }
  else             //目前是吸附红外
  {
     IRDA_NEAR_REC_EN;
  }  
}
/********************************************************************************
函数原型：void IRDA_Rec_Disable(void)
功能：根据干簧管状态,禁止相应的红外功能
Enable---0:禁止相应的红外接收功能
*********************************************************************************/
void IRDA_Rec_Disable(void)
{
  if(GUMB_STATUS)  //目前是远红外
  {
     IRDA_NEAR_REC_EN;
  }
  else             //目前是吸附红外
  {
     IRDA_FAR_REC_EN;
  }  
}
/**********************************************************************************
函数功能：打开表计所有通道
入口：
Type-------------通道类型，见Pub_PUCK.h定义
Baurd----------波特率
出口：1---------成功；0--------失败
**********************************************************************************/
INT8U Open_Channel_PUCK(INT8U Type,INT32U Baurd,INT16U Checkbit )
{
  if(Type>CHANEL_MAX)
    return 0;
  
  Const_Uart_Attib[Type].Init_Uart(Baurd,Checkbit);
  Const_Uart_Attib[Type].Start_Uart();
  Const_Uart_Attib[Type].Ready_Uart();
  if(Type==CHANEL_IRDA)
    IRDA_Rec_Enable();   //根据不同类别选择不同的红外类型
  return 1;
}
/**********************************************************************************
函数功能：关闭表计所有通道
入口：
Type-------------通道类型，见Pub_PUCK.h定义
Baurd----------波特率
出口：1---------成功；0--------失败
**********************************************************************************/
INT8U Close_Channel_PUCK(INT8U Type)
{
   if(Type>CHANEL_MAX)  
    return 0;
  
  Const_Uart_Attib[Type].Stop_Uart();
  if(Type==CHANEL_IRDA)
    IRDA_Rec_Disable();   //根据不同类别禁止不同的红外类型
  return 1;
}
/**********************************************************************************
函数功能：向某通道发送数据
入口：
Type-------------通道类型，见Pub_PUCK.h定义
DataSrc-------------数据
DataLen----------数据长度
出口：1---------发送成功；0--------发送失败
**********************************************************************************/
INT8U Channel_DataSend_PUCK(INT8U Type,INT8U *DataSrc,INT16U DataLen)
{
    if(Type>CHANEL_MAX)  
      return 0;
    if(Type==CHANEL_IRDA)
    {      
      IRDA_Rec_Disable();  //只要是红外口，发送前，关闭接收
      if((CHANEL_IRDA!=CHANEL_DEBG)||Check_Debug_En()==0)  //是红外口，但不做调试口，或者不允许输出调试信息
      {
        IRDA_SEND_EN;       //红外允许发送 
        START_PMW;
      }
      else             //调试口不是红外口，或者不是工厂状态
      {
        IRDA_SEND_DIS;
      }
      Const_Uart_Attib[Type].Send_Uart(DataSrc,DataLen);
      if((CHANEL_IRDA!=CHANEL_DEBG)||Check_Debug_En()==0) //是红外口，但不做调试口，或者不允许输出调试信息
      {
        IRDA_SEND_DIS;    //红外禁止发送
        STOP_PMW;
      }      
      IRDA_Rec_Enable();   //根据当前的状态，打开不同的红外通道      
      return 1;
    }    
    Const_Uart_Attib[Type].Send_Uart(DataSrc,DataLen);
    return 1;
}

/**********************************************************************************
函数功能：查询通道数据的接收
入口：
Type------------当前的数据通道类型

**********************************************************************************/
void Channel_DataReceive_PUCK(INT8U Type,INT32U Status)
{

  INT16U MaxRecLen,Len,offset;
  INT8U *RecBufPtr;
  
  if(Type>CHANEL_MAX)
    return ;
 
  if(Status!=SYS_NORMAL)   //resume模式下
  {
    if(Type!=CHANEL_IRDA)
      return ;
    
    if(IRDA_READ_METER_EN==0)  //唤醒模式下，红外抄表禁止
      return ;
    
  }
    
  RecBufPtr=Const_Uart_Attib[Type].RecBufPtr;
  MaxRecLen=Const_Uart_Attib[Type].MaxBufLen;
  
  Len=Search_Protocol_Frame(RecBufPtr,MaxRecLen,&offset);
  if((Len>=MIN_FRAME_PROCO_SIZE)&&(Len<=MAX_UART_REC_SIZE)&&(offset<=MaxRecLen))  //确实有645数据,645最少长度为12字节
  {
    if(CHECK_STRUCT_VAR(S_Buf_PUCK)==0)
      ASSERT(A_WARNING,0);
    
    DEBUG_PRINT(PUCK,PRINT_PUCK_CHANNEL_EN,"<---------Receive Data,Channel_Id=%d",Type);
    DEBUG_BUF_PRINT(RecBufPtr,Len,PRINT_HEX,30);
    Len=Rcv_PROTOCOL_Frame_Proc(Type,RecBufPtr+offset,Len,(void *)Temp_Buf_PUCK, (void *)Temp_Buf_PUCK, TEMP_BUF_LEN);
    if(Len)   //需要应答
      Send_HostData_Public_Puck(Type,(INT8U *)Temp_Buf_PUCK,Len);  //1200bps,160字节发送不喂狗，估计有问题
#ifdef CHANNEL_FAST_EN
    if(CHECK_STRUCT_VAR(Rec_Frame_Status)==0 || REC_FRAME_CONDITION)
    {
      CLR_REC_FRAME;  
    }
#endif
    //先发送后打印调试信息，确保速度
    DEBUG_PRINT(PUCK,PRINT_PUCK_CHANNEL_EN,"--------->Send Data,Channel_Id=%d",Type);
    DEBUG_BUF_PRINT((INT8U *)Temp_Buf_PUCK,Len,PRINT_HEX,30);
    Chanel_Para[Type].ExitFlag=1;
    SET_STRUCT_SUM(Chanel_Para[Type]);
    Const_Uart_Attib[Type].Ready_Uart();    //重置接收区
    if(CHECK_STRUCT_VAR(S_Buf_PUCK)==0)
      ASSERT(A_WARNING,0);    
    Clr_LCDSleep_Timer();   //清除唤醒后计数器
  }
  else
  {
      Len=*Const_Uart_Attib[Type].CurRecLen;
      if(Len>=(MaxRecLen-10))  //乱码长度接近整个缓冲区
        Const_Uart_Attib[Type].Ready_Uart();  //重置接收区
  }
}
