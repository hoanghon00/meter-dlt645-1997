#define REMOTE_PUCK
#include "Pub_PUCK.h"


#define IR_NONE        0x00
#define IR_MIN_NUM     0X00
#define IR_MAX_NUM     0X0F
#define IR_LIGHT_ON    0x11
#define IR_UP_KEY      0x12
#define IR_DOWN_KEY    0x13
#define IR_LEFT_KEY    0x17
#define IR_RIGHT_KEY   0x18
#define IR_ENTER_A     0x14
#define IR_ENTER_B     0x15

CONST INT8U REMOTOR_CMD[32]=
{
0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
0x09,0x00,0x0A,0x0B,0x0C,IR_NONE,0x16,IR_ENTER_B,
IR_ENTER_A,IR_NONE,IR_NONE,IR_NONE,IR_LEFT_KEY,IR_NONE,0x0F,IR_NONE,
IR_LIGHT_ON,IR_DOWN_KEY,IR_UP_KEY,0x0D,IR_NONE,IR_RIGHT_KEY,0x10,0X0E
};

void Remoter_Cmd_Decode(INT8U Cmd)
{
  static INT8U LightCtrl=0;
  
  switch(Cmd)
  {     
    case IR_LIGHT_ON:
      if(LightCtrl)
        TURN_OFF_LIGHT
      else            
        TURN_ON_LIGHT;
      LightCtrl=!LightCtrl;      
      break;
    case IR_UP_KEY:
      Key_Value_Pub.Key.Bit.UpKey=1;
      break;
    case IR_DOWN_KEY:
      Key_Value_Pub.Key.Bit.DownKey=1;
      break;
    case IR_LEFT_KEY:
      Key_Value_Pub.Key.Bit.LeftKey=1;
      break;
    case IR_RIGHT_KEY:
      Key_Value_Pub.Key.Bit.RightKey=1;
      break;
    case IR_ENTER_A:
      Key_Value_Pub.Key.Bit.A_Key=1;
      break; 
    case IR_ENTER_B:
      Key_Value_Pub.Key.Bit.B_Key=1;
      break;
  default:
    if(Cmd<=IR_MAX_NUM && Get_Sys_Status()==SYS_NORMAL)
    {
      Set_C_Mode_DisCode(Cmd);      
    }
  }
}
void Remote_Key_Proc(void)
{  
  if(CHECK_STRUCT_VAR(Irda_Decode_Ctrl)==0)    //头尾不对
  {
    mem_set((void *)(&Irda_Decode_Ctrl),0,sizeof(Irda_Decode_Ctrl),\
            (void *)(&Irda_Decode_Ctrl),sizeof(Irda_Decode_Ctrl));
    INIT_STRUCT_VAR(Irda_Decode_Ctrl);
    ASSERT(A_WARNING,0);
    return ;
  }
  
  if(Irda_Decode_Ctrl.Done)  //此情况下，肯定是 不能输出调试信息的，所以不必断言
  {
    if((Irda_Decode_Ctrl.CodeBuf[0]==(INT8U)(~Irda_Decode_Ctrl.CodeBuf[1]))     //用户识别码
     &&(Irda_Decode_Ctrl.CodeBuf[2]==(INT8U)(~Irda_Decode_Ctrl.CodeBuf[3])))    //操作码
    {
      if(Irda_Decode_Ctrl.CodeBuf[2]<sizeof(REMOTOR_CMD)) //避免越界
        Remoter_Cmd_Decode(REMOTOR_CMD[Irda_Decode_Ctrl.CodeBuf[2]]);
    }
    mem_set((void *)(&Irda_Decode_Ctrl),0,sizeof(Irda_Decode_Ctrl),\
            (void *)(&Irda_Decode_Ctrl),sizeof(Irda_Decode_Ctrl));
    INIT_STRUCT_VAR(Irda_Decode_Ctrl);
  }
}