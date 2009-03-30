#ifndef REMOTE_H
#define REMOTE_H



#ifndef REMOTE_PUCK
#define REMOTE_PUCK_EXT  extern
#else
#define REMOTE_PUCK_EXT 
#endif



#define CODE_NUM 4
#pragma pack(1)
typedef struct
{
  INT8U Head;
  INT8U Start:1;              //接收起始信号
  INT8U Done:1;              //解码bit流结束
  INT8U Index:6;              //收取的bit命令序列
  INT8U TrigTimer;
  INT8U CodeBuf[CODE_NUM];    //是否曾经唤醒
  INT8U Tail;
}IRDA_DECODE;
INTER_ABS_PUCK_EXT volatile IRDA_DECODE Irda_Decode_Ctrl;
#pragma pack()

void Remote_Key_Proc(void);
#endif