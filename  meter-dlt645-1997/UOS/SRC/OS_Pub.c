#define OS_PUB_C
#include "OS_Core.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "Pub_Debug.h"

CONST OS_INT16U Crc_Table0[16]={0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
                                0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef};

CONST OS_INT16U Crc_Table1[256]={ 
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
  0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
  0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
  0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
  0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
  0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
  0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
  0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
  0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
  0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
  0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
  0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
  0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
  0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
  0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
  0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
  0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
  0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
  0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
  0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
  0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
  0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
  0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0};

extern void Clr_Ext_Inter_Dog(void);
#if OS_ASSERT_HOOK_EN
extern INT8U OS_Assert_Hook();
INT8U OS_ASSERT_Hook()
{
  INT8U Temp;
  INT8U Re;
  Temp=Assert_Info.On_Off_Flag;
  OS_Assert_Ctrl(0);
  Re=OS_Assert_Hook();
  OS_Assert_Ctrl(Temp);
  return Re;  
}
#else
#define OS_ASSERT_Hook()
#endif

extern INT8U Check_Debug_En();



//根据format和ap参数表输出调试信息
void OS_vsPrintf(CONST OS_INT8S *format, va_list ap)
{
  
  static const OS_INT8S Chr[] = "0123456789ABCDEF";

  static const OS_INT32U Oct[12]=
  {
    01,010,0100,01000,010000,0100000,01000000,010000000,
    0100000000,01000000000,010000000000,010000000000,
  }; //八进制
   static const OS_INT32U Dec[11]=
  {
    1,10,100,1000,10000,100000,1000000,
    10000000,100000000,1000000000,1000000000,
  }; //十进制
  static const OS_INT32U Hex[8]=
  {
    0x1,0x10,0x100,0x1000,0x10000,
    0x100000,0x1000000,0x10000000,
  }; //十六进制
  
  OS_INT8U LintFlag;
  OS_INT32U const *p;
  OS_INT8S *ptr;
  
  unsigned long int uParaValue;//无符号数
  long int sParaValue;//有符号数
  
  OS_INT8U c;
  OS_INT8U i,Width,Len;
 
  for(;;)    //遍历整个参数表
  {
    while((c=*format++)!='%')//%符号之前全是字符串
    {
      if(!c)
        return;
      Debug_Out_Public_Puck(&c,1);
    }

    c=*format++;
    //非数值类型处理
    switch(c)
    {
      case 'c':
        c = va_arg(ap, int);
        Debug_Out_Public_Puck(&c,1);
        break;
      case 's':
        ptr= va_arg(ap,OS_INT8S *);
        while((c = *ptr++)!=0)
          Debug_Out_Public_Puck(&c,1);
        break;
    }
    
    //数值类型处理
    if(c>='1' && c<='9')
    {
      Width=c-'0';
      c=*format++;
    }
    else
      Width=1;//至少打印一个字符
    
    if(c=='l' || c=='L')//是否是一个长整形?
    {
      LintFlag=1;
      c=*format++;
    }
    else
      LintFlag=0;
    
    switch(c)//打印数据类型
    {
      case 'o':
        p=Oct;
        Len=12;
        break;
      case 'd':
      case 'u':
        p=Dec;
        Len=11;
        break;
      case 'x':
        p=Hex;
        Len=8;
        break;

      default:
        continue;
    }
    
    //获取参数数值
    if(c=='d')//有符号数
    {
      if(LintFlag)//长整型
        sParaValue=va_arg(ap,long int);
      else
        sParaValue=va_arg(ap,int);
      
      if(sParaValue<0)
      {
        c='-';
        Debug_Out_Public_Puck(&c,1);
        sParaValue=0-sParaValue;
      }
      uParaValue=(unsigned long int)sParaValue;//赋值到uParaValue,统一计算
    }
    else//无符号数
    {
      if(LintFlag)//长整型
        uParaValue=va_arg(ap,unsigned long int);
      else
        uParaValue=va_arg(ap,unsigned int);  
    }
    
    //计算数据本身的长度
    for(i=0;i<Len;i++)
    {
       if(uParaValue<*(p+i))
         break;
    }
    
    if(Width<i)//i表示数据本身长度，Width表示需要打印的长度
      Width=i;
    
    //分别打印每个字符
    i=0;//表示是否已经找到了一个非0位
    for(;Width!=0;Width--)
    {
      c=uParaValue/(*(p+Width-1));
      uParaValue=uParaValue%(*(p+Width-1));
      //前面的0全部打成空格

      if(c==0 && i==0 && Width>1)//该位为0，且之前的位也都为0,且不是最后一位(数值本身为0)
        c=' ';
      else
      {
        i=1;//表示已经有一个非0位了
        c=Chr[c];
      }
      Debug_Out_Public_Puck(&c,1);
    }
  }
}

//调试信息输出函数
//注意目前只支持%c,%s,%d,%u,%o,%x。暂不支持浮点数
void OS_Debug_Print(CONST OS_INT8S *format, ...)
{
  va_list ap;
  va_start(ap,format);
  OS_vsPrintf(format,ap);
  va_end(ap);
}

//断言打开关闭的控制功能
void OS_Assert_Ctrl(OS_INT32U Flag)
{
  Assert_Info.On_Off_Flag=Flag;//打开断言
}

//定义断言
//Condition表示条件
//File断言所在的文件
//Function断言所在的函数
//Line断言所在的行号
//条件成立返回0，不成立返回1
#if OS_ASSERT_FUNC_EN>0
OS_INT8U OS_Assert(OS_INT8U Condition,CONST OS_INT8U File[],CONST OS_INT8S Function[],OS_INT16U Line)
{
#else
OS_INT8U OS_Assert(OS_INT8U Condition,CONST OS_INT8U File[],OS_INT16U Line)
{
  const INT8U Function[]="Func";
#endif 
  
  OS_INT8U i,j;
  OS_INT16U Len;  
  //static OS_INT8U Flag=0;
  
  Condition=((Condition!=0)?0:1);
  if(Check_Debug_En()==0)//不是调试状态不打印
    return Condition;

  //断言关闭的情况下不打印调试信息和记录调用流程
#if OS_ASSERT_CTRL_EN==1
  if(GET_BIT(Assert_Info.On_Off_Flag,Task_Info.Cur_Task_ID)==0)
    return Condition;   
#endif 

  //Flag=1;//表示已经在Assert流程中了，防止重复进入  
  if(1==Assert_Info.Flag)
  {
    DEBUG_PRINT(ASSERT_ID,DEBUG_0,"ASSERT Reentry!\r\n");
    return Condition;
  }
  
  Assert_Info.Flag=1;//表示已经进入
  OS_ASSERT_Hook();

  Len=strlen((char *)File)+1;
  
  if(Len>OS_ASSERT_BUF_LEN)
    File=File+Len-OS_ASSERT_BUF_LEN;//取文件名的起始位置
    
  if(Condition)//Conditon已经转化，0表示条件真，1表示假
  {
    //打印任务名字或者ID
    DEBUG_PRINT(ASSERT_ID,DEBUG_0,"ASSERT:-----------------");
#if OS_USE_TASK_NAME    
    DEBUG_PRINT(ASSERT_ID,DEBUG_0,"ASSERT:Task Name:%s",Tcb[Task_Info.Cur_Task_ID].Name);
#else
    DEBUG_PRINT(ASSERT_ID,DEBUG_0,"ASSERT:Task ID:%d",Task_Info.Cur_Task_ID);
#endif
  
    //打印出错文件
   // if(Condition)
      DEBUG_PRINT(ASSERT_ID,DEBUG_0,"ASSERT:Err File:%s,Function:%s,Line:%d",File,Function,Line);

    //打印前几次调用的函数
    if(Assert_Info.Index>=OS_ASSERT_NUM)
    {
      DEBUG_PRINT(ASSERT_ID,DEBUG_0,"ASSERT:ASSERT Index error %d",Assert_Info.Index);
      Assert_Info.Index=0;
    }
    
    j=Assert_Info.Index; //最近一次调用信息索引   
    for(i=0;i<OS_ASSERT_NUM;i++)//打印出前AS_BUF_NUM次调用的函数
    {
      Clr_Ext_Inter_Dog();
      Assert_Info.File[j][OS_ASSERT_BUF_LEN-1]=0;
      
      DEBUG_PRINT(ASSERT_ID,DEBUG_0,"ASSERT:Call:Task:%u,Line:%u,Counts:%u,File:%s",\
        *(OS_INT8U *)&Assert_Info.Task_ID[j],\
        *(OS_INT16U *)&Assert_Info.Line[j],\
        *(OS_INT16U *)&Assert_Info.Counts[j],\
        Assert_Info.File[j]);
      //前一次调用信息
      if(j==0)
        j=OS_ASSERT_NUM-1;
      else
        j--;
    } 
  }

  //记录下当此调用的函数
  //该次调用和上次调用的行号不相等,或者两次调用的函数名字不一样,或者不是在同一个任务中,认为是两次不同的调用
  if(Assert_Info.Line[Assert_Info.Index]!=Line ||\
     strcmp(Assert_Info.File[Assert_Info.Index],(char *)File)!=0 ||\
     Assert_Info.Task_ID[Assert_Info.Index]!=Task_Info.Cur_Task_ID)
  {
    Assert_Info.Index++;
    if(Assert_Info.Index>=OS_ASSERT_NUM)
      Assert_Info.Index=0;
    
    //记录当前任务号,与行号
    Assert_Info.Task_ID[Assert_Info.Index]=Task_Info.Cur_Task_ID;//保存任务ID
    Assert_Info.Line[Assert_Info.Index]=Line;//保存行号
    Assert_Info.Counts[Assert_Info.Index]=1;//调用次数
    //记录文件名
    memcpy(Assert_Info.File[Assert_Info.Index],File,OS_ASSERT_BUF_LEN-1);//记录下该次调用的函数名字
    //memcpy(Assert_Info.Function[Assert_Info.Index],(void *)Function,OS_ASSERT_BUF_LEN-1);//记录下该次调用的函数名字
    //最后一个字符填0
    Assert_Info.File[Assert_Info.Index][OS_ASSERT_BUF_LEN-1]=0;
  }
  else
  {
    Assert_Info.Counts[Assert_Info.Index]++;//认为在反复调用同一个函数，增加调用次数的记录
  }
  
  //Flag=1表示已经在Assert流程中了，防止重复进入
  Assert_Info.Flag=0;
  return Condition;
}

/*****************************************
函数原型：
OS_INT16U OS_Debug_Print(char *format,...)
函数功能：
打印调试信息，可以将调试信息打印到串口或IAR的i/o窗口中
入口参数：参数列表
出口参数：打印信息长度
******************************************/
/*
OS_INT16U OS_Debug_Print(OS_INT8S *format,...)
{
  va_list ap;
  OS_INT16U n=0;

  if(OS_PRINT_TYPE==OS_PRINT_NULL)//不输出
    return 0;
  else if(OS_PRINT_TYPE==OS_PRINT_COMM)//通信口输出
  {
    va_start(ap,format);
    n=vsprintf(OS_Debug_Buf,format,ap);
    
    //检查边界
    if(n>=sizeof(OS_Debug_Buf))
      Debug_Out_Public_Puck("OS_Deubg_Buf Overflow!",strlen("OS_Deubg_Buf Overflow"));//串口输出错误
    //调用串口输出
    Debug_Out_Public_Puck(OS_Debug_Buf,n);
    va_end(ap);
    return n;
  }
  else if(OS_PRINT_TYPE==OS_PRINT_STDIO)//仿真器标准IO输出
  {
    va_start(ap,format);
    OS_Debug_Buf[0]=OS_Debug_Buf[sizeof(OS_Debug_Buf)-1]=CHK_BYTE;
    n=vsprintf(OS_Debug_Buf,format,ap);
    
    //检查边界
    if(n>=sizeof(OS_Debug_Buf))
      printf("OS_Deubg_Buf Overfow!");
    
    printf("%s",OS_Debug_Buf);
    va_end(ap);
    return n;
  }
  else
    return 0;
}
*/

//memcpy函数
//pDst表示目标缓冲区
//pSrc表示源缓冲区
//SrcLen表示拷贝长度
//pDst_Start和DstLen一起限制pDst和SrcLen的范围
//必须满足ASSERT(A_WARNING,(OS_INT8U *)pDst>=(OS_INT8U *)pDst_Start && (OS_INT8U *)pDst+SrcLen<=(OS_INT8U *)pDst_Start+DstLen);
void OS_memcpy(void *pDst,void *pSrc,OS_INT32U SrcLen,void *pDst_Start,OS_INT32U DstLen)
{
  if(!((OS_INT8U *)pDst>=(OS_INT8U *)pDst_Start && (OS_INT8U *)pDst+SrcLen<=(OS_INT8U *)pDst_Start+DstLen))//pDst必须满足的条件
  {
    ASSERT(A_WARNING,0);
    return;
  }
  memcpy(pDst,pSrc,SrcLen);
}

//memset函数
//pDst表示目标缓冲区
//Value表示需要设置的值
//SetLen表示置数长度
//pDst_Start和DstLen一起限制pDst和SetLen的范围
//必须满足ASSERT(A_WARNING,(OS_INT8U *)pDst>=(OS_INT8U *)pDst_Start && (OS_INT8U *)pDst+SetLen<=(OS_INT8U *)pDst_Start+DstLen);
void OS_memset(void *pDst, OS_INT8U Value,OS_INT32U SetLen,void *pDst_Start,OS_INT32U DstLen)
{
  if(!((OS_INT8U *)pDst>=(OS_INT8U *)pDst_Start && (OS_INT8U *)pDst+SetLen<=(OS_INT8U *)pDst_Start+DstLen))//pDst必须满足的条件
  {
    ASSERT(A_WARNING,0);
    return;
  }
  memset(pDst,Value,SetLen);
}


//CRC校验算法0，用于空间比较小的情况,但速度要求不高的情况
OS_INT16U OS_Crc16_0(OS_INT8U *pSrc, OS_INT16U Len) 
{
  OS_INT16U Crc;
  OS_INT8U Temp;
  
  Crc=0;
  while(Len--!=0)
   {
     Temp=(OS_INT8U)(Crc>>12); 
     Crc<<=4; 
     Crc^=Crc_Table0[Temp^(*pSrc/16)]; 
                                
     Temp=(OS_INT8U)(Crc>>12); 
     Crc<<=4; 
     Crc^=Crc_Table0[Temp^(*pSrc&0x0f)]; 
     pSrc++;
  }
  return(Crc);
}

//CRC校验算法1，用于空间比较大的情况,但速度要求较高的情况
OS_INT16U OS_Crc16_1(OS_INT8U *pSrc, OS_INT16U Len)
{
  OS_INT16U Crc=0x00;
  OS_INT16U i,Temp;
  
  for(i=0;i<Len;i++)
  {
    Temp=Crc;
    Crc=Crc_Table1[(Temp>>8)^*pSrc++];
    Crc=Crc^(Temp<<8);
  }
  return(Crc);
}

//通用检查校验和函数
//pSrc表示需要检查的数据缓冲区
//SrcLen表示数据缓冲区长度
//pCS表示校验和的起始地址
//CS_Bytes表示校验和长度
//注意：这个函数要和Set_Sum成对使用，因为内部不是计算简单的校验和
OS_INT8U OS_Check_Sum(void *pSrc,OS_INT16U SrcLen,void *pCS,OS_INT8U CS_Bytes)
{
  OS_INT32U Sum=0;
  
  if(!(CS_Bytes==1 || CS_Bytes==2 || CS_Bytes==4))//字节数必须是1或2或3
    OS_ASSERT(0);
  
#if OS_CS_TYPE==CS_CRC16_0
  Sum=(OS_INT32U)OS_Crc16_0((OS_INT8U *)pSrc,SrcLen);
#elif OS_CS_TYPE==CS_CRC16_1
  Sum=(OS_INT32U)OS_Crc16_1((OS_INT8U *)pSrc,SrcLen);
#else
  while(SrcLen!=0)
  {
    SrcLen--;
    Sum+=*((OS_INT8U *)pSrc+SrcLen);
  }
#endif
  
  Sum=Sum^CHK_CS_BYTES;//进行简单变换,某些位取反  
  if(memcmp(&Sum,pCS,CS_Bytes)==0)
    return 1;
  else
    return 0;
}

//通用设置校验和函数
//pSrc表示需要计算校验和的数据缓冲区
//SrcLen表示数据缓冲区长度
//pDst表示校验和的起始地址
//CS_Bytes表示校验和长度
//pDst_Start对pDst范围的限定
//DstLen对pDst范围的限定,
//需要满足ASSERT(A_WARNING,(OS_INT8U *)pDst>=(OS_INT8U *)pDst_Start && (OS_INT8U *)pDst+CS_Bytes<=(OS_INT8U *)pDst_Start+DstLen);
//这个函数要和Check_Sum成对使用，因为生成的校验不是简单的校验和，而是进行了简单的变换
void OS_Set_Sum(void *pSrc,OS_INT16U SrcLen,void *pDst,OS_INT8U CS_Bytes,void *pDst_Start,OS_INT16U DstLen)
{
  OS_INT32U Sum=0;
  
  if(!(CS_Bytes==1 || CS_Bytes==2 || CS_Bytes==4))
    OS_ASSERT(0);
  
  if(!((OS_INT8U *)pDst>=(OS_INT8U *)pDst_Start && (OS_INT8U *)pDst+CS_Bytes<=(OS_INT8U *)pDst_Start+DstLen))
  {
    OS_ASSERT(0);
    return;
  }
  
#if OS_CS_TYPE==CS_CRC16_0
  Sum=(OS_INT32U)OS_Crc16_0((OS_INT8U *)pSrc,SrcLen);
#elif OS_CS_TYPE==CS_CRC16_1
  Sum=(OS_INT32U)OS_Crc16_1((OS_INT8U *)pSrc,SrcLen);
#else
  while(SrcLen!=0)
  {
    SrcLen--;
    Sum+=*((OS_INT8U *)pSrc+SrcLen);
  }
#endif
  
  Sum=Sum^CHK_CS_BYTES;//进行简单变换,某些位取反  
  OS_memcpy(pDst,&Sum,CS_Bytes,pDst_Start,DstLen);
}

//检查某个结构体的校验和是否正确,该结构体的定义必须满足如下方式
//
//typedef struct{
//     ...
//     ...
//     OS_INT8U CS[OS_CS_BYTES];//最后一个域必须是校验和，且长度为OS_CS_BYTES
//    }sturct_name;
//
//pSrc为结构体变量的起始指针
//SrcLen为结构体变量长度
//CS为结构体变量的校验和,注意该变量必须为结构体中的一个域
//注意：这个函数要和Set_STRUCT_Sum成对使用，因为校验计算算法一致
OS_INT8U OS_Check_STRUCT_Sum(void *pSrc,OS_INT16U SrcLen,OS_INT8U *pCS,OS_INT8U CS_Bytes)
{
  OS_INT16U Len;
  
  if(!(pCS>(OS_INT8U *)pSrc && pCS+CS_Bytes<=(OS_INT8U *)pSrc+SrcLen))//判断CS的位置是否合法
  {
    OS_ASSERT(0);
    return 0;
  }
  
  Len=(OS_INT16U)((OS_INT8U *)pCS-(OS_INT8U *)pSrc);//计算需要计算校验和的数据长度
  return OS_Check_Sum(pSrc,Len,pCS,CS_Bytes);
}

//设置结构体变量的校验和,该结构体的定义必须满足如下方式
//
//typedef struct{
//     ...
//     ...
//     OS_INT8U CS[OS_CS_BYTES];//最后一个域必须是校验和，且长度为OS_CS_BYTES
//    }sturct_name;
//
//pSrc为结构体变量的起始指针
//SrcLen为结构体变量长度
//CS为结构体变量的校验和,注意该变量必须为结构体中的一个域
//注意：这个函数要和Check_STRUCT_Sum成对使用，因为校验计算算法一致
void OS_Set_STRUCT_Sum(void *pSrc,OS_INT16U SrcLen,OS_INT8U *pCS,OS_INT8U CS_Bytes)
{
  OS_INT16U Len;

  if(!(pCS>(OS_INT8U *)pSrc && pCS+CS_Bytes<=(OS_INT8U *)pSrc+SrcLen))//判断CS的位置是否合法
  {
    OS_ASSERT(0);
    return;
  }
  
  Len=(OS_INT16U)((OS_INT8U *)pCS-(OS_INT8U *)pSrc);//计算需要计算校验和的数据长度
  OS_Set_Sum(pSrc,Len,pCS,CS_Bytes,pSrc,SrcLen);
}

#undef OS_PUB_C
