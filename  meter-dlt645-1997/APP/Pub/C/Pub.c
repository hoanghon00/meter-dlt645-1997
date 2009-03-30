#define PUB_C
#include "includes.h"
#include "OS_Port.h"


//检查BCD数据缓冲区的数据是否都是BCD码,是BCD的缓冲区则返回1，否则返回0
INT8U Check_BCD_Data(void *pBCD,INT16U Len)
{
  INT16U i;
  INT8U *p;
  
  p=(INT8U *)pBCD;
  
  for(i=0;i<Len;i++)
  {
     if(!((p[i] & 0x0F)<0x0A && (p[i] & 0xF0)<0xA0))
     return 0;
  }
  return 1;
}

/*
//memcpy函数
//pDst表示目标缓冲区
//pSrc表示源缓冲区
//SrcLen表示拷贝长度
//pDst_Start和DstLen一起限制pDst和SrcLen的范围
//必须满足ASSERT(A_WARNING,(INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst+SrcLen<=(INT8U *)pDst_Start+DstLen);
void mem_cpy(void *pDst,void *pSrc,INT32U SrcLen,void *pDst_Start,INT32U DstLen)
{
  ASSERT(A_WARNING,(INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst+SrcLen<=(INT8U *)pDst_Start+DstLen);//pDst必须满足的条件
  memcpy(pDst,pSrc,SrcLen);
}

//memset函数
//pDst表示目标缓冲区
//Value表示需要设置的值
//SetLen表示置数长度
//pDst_Start和DstLen一起限制pDst和SetLen的范围
//必须满足ASSERT(A_WARNING,(INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst+SetLen<=(INT8U *)pDst_Start+DstLen);
void mem_set(void *pDst, INT8U Value,INT32U SetLen,void *pDst_Start,INT32U DstLen)
{
  ASSERT(A_WARNING,(INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst+SetLen<=(INT8U *)pDst_Start+DstLen);//pDst必须满足的条件
  memset(pDst,Value,SetLen);
}

//通用检查校验和函数
//pSrc表示需要检查的数据缓冲区
//SrcLen表示数据缓冲区长度
//pCS表示校验和的起始地址
//CS_Bytes表示校验和长度
//注意：这个函数要和Set_Sum成对使用，因为内部不是计算简单的校验和
INT8U Check_Sum(void *pSrc,INT16U SrcLen,void *pCS,INT8U CS_Bytes)
{
  INT16U i;
  INT32U Sum=0;
  
  ASSERT(A_WARNING,CS_Bytes==1 || CS_Bytes==2 || CS_Bytes==4);
  for(i=0;i<SrcLen;i++)
  {
    Sum+=*((INT8U *)pSrc+i); 
  }
  
  Sum=Sum^0xAAAAAAAA;//进行简单变换,某些位取反
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
//需要满足ASSERT(A_WARNING,(INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst+CS_Bytes<=(INT8U *)pDst_Start+DstLen);
//这个函数要和Check_Sum成对使用，因为生成的校验不是简单的校验和，而是进行了简单的变换
void Set_Sum(void *pSrc,INT16U SrcLen,void *pDst,INT8U CS_Bytes,void *pDst_Start,INT16U DstLen)
{
  INT16U i;
  INT32U Sum=0;
  
  ASSERT(A_WARNING,CS_Bytes==1 || CS_Bytes==2 || CS_Bytes==4);
  ASSERT(A_WARNING,(INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst+CS_Bytes<=(INT8U *)pDst_Start+DstLen);
  
  for(i=0;i<SrcLen;i++)
  {
    Sum+=*((INT8U *)pSrc+i); 
  }
  
  Sum=Sum^0xAAAAAAAA;//进行简单变换,某些位取反
  mem_cpy(pDst,&Sum,CS_Bytes,pDst_Start,DstLen);
}

//检查某个结构体的校验和是否正确,该结构体的定义必须满足如下方式
//
//typedef struct{
//     ...
//     ...
//     INT8U CS[CS_BYTES];//最后一个域必须是校验和，且长度为CS_BYTES
//    }sturct_name;
//
//pSrc为结构体变量的起始指针
//SrcLen为结构体变量长度
//CS为结构体变量的校验和,注意该变量必须为结构体中的一个域
//注意：这个函数要和Set_STRUCT_Sum成对使用，因为校验计算算法一致
INT8U Check_STRUCT_Sum(void *pSrc,INT16U SrcLen,INT8U CS[])
{
  INT32U Sum;
  INT16U i;
  
  Sum=0;
  i=0;
  ASSERT(A_WARNING,CS>(INT8U *)pSrc && CS+CS_BYTES<=(INT8U *)pSrc+SrcLen);//判断CS的位置是否合法
  while(((INT8U *)pSrc+i)!=CS)//计算校验和
  {
    Sum+=*((INT8U *)pSrc+i);
    i++;    
  }
  
  Sum=Sum^0xAAAAAAAA;//进行简单变换,某些位取反
  if(memcmp(&Sum,CS,CS_BYTES)==0)//比较校验和
    return SYS_TRUE;
  else
    return SYS_FALSE;
}

//设置结构体变量的校验和,该结构体的定义必须满足如下方式
//
//typedef struct{
//     ...
//     ...
//     INT8U CS[CS_BYTES];//最后一个域必须是校验和，且长度为CS_BYTES
//    }sturct_name;
//
//pSrc为结构体变量的起始指针
//SrcLen为结构体变量长度
//CS为结构体变量的校验和,注意该变量必须为结构体中的一个域
//注意：这个函数要和Check_STRUCT_Sum成对使用，因为校验计算算法一致
void Set_STRUCT_Sum(void *pSrc,INT16U SrcLen,INT8U CS[])
{
  INT32U Sum;
  INT16U i;

  Sum=0;
  i=0;
  ASSERT(A_WARNING,CS>(INT8U *)pSrc && CS+CS_BYTES<=(INT8U *)pSrc+SrcLen);//判断CS的位置是否合法
  while(((INT8U *)pSrc+i)!=CS)//计算校验和
  {
    Sum+=*((INT8U *)pSrc+i);
    i++;    
  }
  
  Sum=Sum^0xAAAAAAAA;//进行简单变换,某些位取反
  mem_cpy(CS,&Sum,CS_BYTES,pSrc,SrcLen);
}
*/

//无符号数的Hex到BCD的转换
//Data表示源数据
//pDst表示目标数据缓冲区
//Len表示需要转换出的BCD的字节数
//pDst_Start和DstLen一起限制pDst和Len的范围必须满足:
//ASSERT(A_WARNING,pDst>=pDst_Start && pDst+Len<=pDst_Start+DstLen);
void Hex2Bcd(INT32U Data,INT8U *pDst,INT16U Len,INT8U *pDst_Start,INT16U DstLen)
{
  INT8U Temp,i;
  
  ASSERT(A_WARNING,pDst>=pDst_Start && pDst+Len<=pDst_Start+DstLen);//判断内存区域的合法性
  
  i=0;
  mem_set(pDst,0,Len,pDst_Start,DstLen);//置缓冲区为0
  while(Data!=0 && i<Len)
  {
    Temp=(INT8U)(Data%10);
    pDst[i]|=Temp;
    Temp=(INT8U)((Data/10)%10);
    pDst[i]|=(Temp<<4);
    Data=Data/100;
    i++;
  }
}

//有符号数的Hex到BCD的转换
//Data表示源数据
//pDst表示目标数据缓冲区
//Len表示需要转换出的BCD的字节数
//pDst_Start和DstLen一起限制pDst和Len的范围必须满足:
//ASSERT(A_WARNING,pDst>=pDst_Start && pDst+Len<=pDst_Start+DstLen);
void S_Hex2Bcd(INT32S Data,INT8U *pDst,INT16U Len,INT8U *pDst_Start,INT16U DstLen)
{
  INT8U Temp,i;
  INT8U S_Flag=0;
  
  ASSERT(A_WARNING,pDst>=pDst_Start && pDst+Len<=pDst_Start+DstLen);//判断内存区域的合法性
  
  i=0;
  if(Data<0)
  {
    Data=0-Data;
    S_Flag=1;//源数据是负数
  }
  
  mem_set(pDst,0,Len,pDst_Start,DstLen);//置缓冲区为0
  while(Data!=0 && i<Len)
  {
    Temp=Data%10;
    pDst[i]|=Temp;
    Temp=(Data/10)%10;
    pDst[i]|=(Temp<<4);
    Data=Data/100;
    i++;
  }
  if(1==S_Flag)
   *(pDst+Len-1)|=0x80;//最高位置1表示是负数
}

//BCD转换为有符号数，最高位为符号位
INT32S S_Bcd2Hex(INT8U Src[],INT8U SrcLen)
{
  INT32S S_Data;
  INT8U Sign=0,Temp;
  
  Temp=Src[SrcLen-1];
  if((Src[SrcLen-1] & 0x80)!=0)//是否是负数?
    Sign=1;
  
  Src[SrcLen-1]=Src[SrcLen-1] & 0x7F;//将最高位置为0
  
  S_Data=(INT32S)Bcd2Hex(Src,SrcLen);
  if(Sign==1)
    S_Data=0-S_Data;
  Src[SrcLen-1]=Temp;
  
  return S_Data;
}

//无符号的BCD到HEX的转换
//Src为BCD缓冲区
//SrcLen表示BCD缓冲区的长度,SrcLen必须<=5，也就是BCD数据必须小于0x9999999999
//返回值表示转换后的HEX值
INT32U Bcd2Hex(INT8U Src[],INT8U SrcLen)
{
  INT32U Data,i;
  INT8U Re;
  
  ASSERT(A_WARNING,SrcLen<=5);//判断SrcLen的合法性
  Re=Check_BCD_Data(Src,SrcLen);
  ASSERT(A_WARNING,Re!=0);//检查BCD数据的合法性
  
  Data=0;
  i=0;
  while(i<SrcLen)
  {
    Data=Data*100;
    Data+=(Src[SrcLen-i-1] & 0x0F)+((Src[SrcLen-i-1] & 0xF0)>>4)*10;
    i++;
  }
  return Data;
}

//字节Hex2Bcd转换
INT8U Hex2Bcd_Byte(INT8U Byte)
{
  ASSERT(A_WARNING,Byte<100);
  return (Byte%10)+((Byte/10)<<4);
}

//字节Hex2Bcd转换
INT8U Bcd2Hex_Byte(INT8U Byte)
{
  INT8U Re;
  Re=Check_BCD_Data(&Byte,1);
  ASSERT(A_WARNING,Re!=0);
  return (Byte & 0x0F)+((Byte & 0xF0)>>4)*10; 
}

//缓冲区的Hex2Bcd转换
void Hex2Bcd_Buf(void *pSrc,INT16U SrcLen,void *pDst,void *pDst_Start,INT16U DstLen)
{
  INT16U i;
  ASSERT(A_WARNING,(INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst+SrcLen<=(INT8U *)pDst_Start+DstLen);
  for(i=0;i<SrcLen;i++)
    *((INT8U *)pDst+i)=Hex2Bcd_Byte(*((INT8U *)pSrc+i));
}

//缓冲区的Hex2Bcd转换
void Bcd2Hex_Buf(void *pSrc,INT16U SrcLen,void *pDst,void *pDst_Start,INT16U DstLen)
{
  INT16U i;
  
  ASSERT(A_WARNING,(INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst+SrcLen<=(INT8U *)pDst_Start+DstLen);
  for(i=0;i<SrcLen;i++)
    *((INT8U *)pDst+i)=Bcd2Hex_Byte(*((INT8U *)pSrc+i));
}

//两个BCD数减
//pData0指向第一个BCD数的缓冲区
//pData1指向第二个BCD数的缓冲区
//pDst指向目标缓冲区,结果pData0-pData1=pDst
//Len表示BCD码的字节数
//pDst_Start和DstLen一起限定pDst和Len的范围
void Bcd_Dec(INT8U Type,INT8U *pData0,INT8U *pData1,INT8U *pDst,INT8U Len,INT8U *pDst_Start,INT16U DstLen)
{
  INT32U Data0,Data1;
  INT32S S_Data0,S_Data1;

  if(Type==S_INTU)
  {
    Data0=Bcd2Hex(pData0,Len);
    Data1=Bcd2Hex(pData1,Len);
  
    if(Data0>=Data1)//
    {
      Hex2Bcd(Data0-Data1,pDst,Len,pDst_Start,DstLen);
    }
    else
    {
      ASSERT(A_WARNING,0);
      Hex2Bcd(0,pDst,Len,pDst_Start,DstLen);
    }
  }
  else if(Type==S_INTS)
  {
    S_Data0=S_Bcd2Hex(pData0,Len);
    S_Data1=S_Bcd2Hex(pData1,Len);
  
    S_Hex2Bcd(S_Data0-S_Data1,pDst,Len,pDst_Start,DstLen);
  }
  else
  {
    ASSERT(A_WARNING,0); 
  }
}

/*****************************************
函数原型：INT8U GetBufSum_Pub(INT8U Src[],INT16U Src_Len)
函数功能：获取buf 的单字节累加和
入口参数：Src,源缓冲区起始指针
         Src_Len,源数据的长度
出口参数：0：校验和错误；1：校验和正确
******************************************/
INT8U GetBufSum_Pub(INT8U Src[],INT16U Src_Len)
{
  INT8U ResultSum;
  INT16U i;
  
  for(ResultSum=0,i=0;i<Src_Len;i++)
  {
    ResultSum+=Src[i];
  }  
 
  return ResultSum;  
}


#undef PUB_C
