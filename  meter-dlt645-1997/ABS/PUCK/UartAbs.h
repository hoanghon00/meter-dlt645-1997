
#ifndef UART_ABS_H
#define UART_ABS_H



#ifndef UART_ABS_PUCK
#define UART_ABS_PUCK_EXT  extern
#else
#define UART_ABS_PUCK_EXT 
#endif


#define UART0 0
#define UART2 1
#define UART3 2

#ifdef UART1_EN    //UART1使能
  #define UART1 3   //1144CPU无此口
#endif

#ifdef UART1
  #define MAX_UART_NUM  (UART1+1)
#else
  #define MAX_UART_NUM  (UART3+1)
#endif

#ifdef CHANNEL_FAST_EN
  #define REC_FRAME_FLAG   0x36
  typedef struct
  { 
    INT8U Head;
    INT8U RecFrameFlag;           //接收到帧标志  
    INT8U Tail;
  }REC_FRAME_STATUS;
  UART_ABS_PUCK_EXT volatile REC_FRAME_STATUS Rec_Frame_Status;
  
  #define REC_FRAME_CONDITION    ((REC_FRAME_FLAG==Rec_Frame_Status.RecFrameFlag)?1:0)
  #define CLR_REC_FRAME          do{Rec_Frame_Status.RecFrameFlag=0; INIT_STRUCT_VAR(Rec_Frame_Status);}while(0)
  #define SWITCH_TO_CHANNEL_TASK  do{ if(REC_FRAME_CONDITION) OS_TimeDly_Ms(10);}while(0)
#endif

INT8U Open_Channel_PUCK(INT8U Type,INT32U Baurd,INT16U CheckBit);
INT8U Close_Channel_PUCK(INT8U Type);
void Channel_DataReceive_PUCK(INT8U Type,INT32U Status);
INT8U Channel_DataSend_PUCK(INT8U Type,INT8U *DataSrc,INT16U DataLen);

INT8U UART0_SendData_suck(INT8U *txbuf, unsigned short txnum);
INT8U UART0_RecData_pre_suck(void);

INT8U Channel_BufReady_PUCK(INT8U Type);
void IRDA_Rec_Select(void);
void IRDA_Rec_Disable(void);

#endif