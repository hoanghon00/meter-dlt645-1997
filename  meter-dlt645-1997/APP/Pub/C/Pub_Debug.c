#define PUB_DEBUG_C
#include "includes.h"
#include "OS_Port.h"

extern void OS_vsPrintf(CONST OS_INT8S *format, va_list ap);
//断言
//x条件不成立时返回1，成立时返回0
//这样可以方便函数中做这样的判断:
//
// if(ASSERT(A_WARNING,x))
//   return 0;
//

//调试信息输出
/*****************************************
函数原型：
INT8U DEBUG_BUF_PRINT(INT8U Src[],INT16U SrcLen,INT8U Flag)
函数功能：
打印调试信息，可以将Src缓冲区中的变量打print出来
入口参数：
Src,源数据缓冲区
SrcLen,源数据长度
Flag,0表示按十六进制方式打印,1表示按十进制方式打印
Num,没Num个数字打一个换行符
出口参数：打印信息长度
******************************************/
INT8U DEBUG_BUF_PRINT(INT8U Src[],INT16U SrcLen,INT8U Flag,INT8U Num)
{
#ifdef PRINT_TYPE
  INT16U i;

  if(Check_Debug_En()==0)//不是工厂状态不打印
    return 0;
  
  if(PRINT_TYPE==PRINT_NULL)
    return 0;

  i=0;
  if(Flag==PRINT_HEX) //hex打印
  {
    for(i=0;i<SrcLen;i++)
    {
      if(Num!=0 && i%Num==0)
        OS_Debug_Print("\r\n");
      
      OS_Debug_Print("%2x ",Src[i]);
    }
  }
  else//%d
  {
    for(i=0;i<SrcLen;i++)
    {
      if(Num!=0 && i%Num==0)
        OS_Debug_Print("\r\n");
      
      OS_Debug_Print("%3d ",Src[i]);
    }
  }
  return 1;
#endif

}

/*****************************************
函数原型：
INT16U DEBUG_PRINT(char *format,...)
函数功能：
打印调试信息，可以将调试信息打印到串口或IAR的i/o窗口中
入口参数：参数列表
出口参数：打印信息长度
******************************************/
INT16U Debug_Print(char *format,...)
{

#ifdef PRINT_TYPE
  va_list ap;

  if(Check_Debug_En()==0)//不是工厂状态不打印
    return 0;
  
  if(Task_Info.Cur_Task_Num==0)
    OS_Debug_Print("\r\nMAIN:");
  else
    OS_Debug_Print("\r\n%s:",Tcb[Task_Info.Cur_Task_ID].Name);
/*    
  if(ID==HUCK)
    ;//Debug_Out_Public_Puck("HUCK:",5);
  else if(ID==PUCK)
    ;//Debug_Out_Public_Puck("PUCK:",5);
  else if(ID==SUCK)
    ;//Debug_Out_Public_Puck("SUCK:",5);
  else if(ID==LUCK)
    ;//Debug_Out_Public_Puck("LUCK:",5);
  else if(ID==ASSERT_ID)
    Debug_Out_Public_Puck("ASSERT:",7);
  else if(ID==TEST)
    Debug_Out_Public_Puck("TEST:",5);
*/  
  if(PRINT_TYPE==PRINT_NULL)
    return 0;
  else
  {
    va_start(ap,format);
    OS_vsPrintf(format,ap);
    va_end(ap);
    return 1;
  }
#else
  return 0;
#endif
}

#undef PUB_DEBUG_C

