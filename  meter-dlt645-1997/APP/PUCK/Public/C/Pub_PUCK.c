#define PUB_PUCK_C
#include "Pub_PUCK.h"


/**********************************************************************************
函数功能：取3数中最大数字
入口：3数
出口：最大
**********************************************************************************/
INT32U Get_Max_Num(INT32U a,INT32U b,INT32U c)
{
			
      if((a>=b)&&(a>=c))	return a;
      if((b>=a)&&(b>=c))	return b;
            
      return c;
}
/**********************************************************************************
函数功能：取3数中最大数字
入口：3数
出口：最大
**********************************************************************************/
INT32U Get_Min_Num(INT32U a,INT32U b,INT32U c)
{
			
      if((a<=b)&&(a<=c))	return a;
      if((b<=a)&&(b<=c))	return b;
      
      return c;
}
/********************************************************************************

INT8U Get_RemoteCtrlKey_PUCK(INT8U Id);
函数功能：遥控器的键值；
入口：Id -----只能填写LUCK，其他无效！；
返回：0~255，定义见技术说明书
********************************************************************************/
INT8U Get_RemoteCtrlKey_PUCK(INT8U Id)
{
  return 1;  
}

/********************************************************************************
INT8U Get_MeterPowerStatus_PUCK(void);
函数功能：判断是否停电
入口：无；
返回：1------------停电(不是主电源供电)；0------------没停电(是主电源供电)
********************************************************************************/
INT8U Get_MeterPowerStatus_PUCK(void)
{
  
  if(Check_Sys_Status()==SYS_NORMAL)
    return 0;
  
  return 1;
  
  
}

/********************************************************************************
INT8U Get_Curr_ComType_PUCK(INT8U Id);
函数功能：获取当前的通信通道类型；
入口：ID-----只能填写LUCK，其他无效！
返回：
Bit7	Bit6	Bit5	Bit4	Bit3	Bit2	Bit1	Bit0
1：表示该通道有数据通信;0：表示该通道无数据通信
保留	保留	保留	遥控器	GPRS模块	第1路485通道	第1路485通道	红外通道
注意：读完后将清除此状态字

********************************************************************************/
INT8U Get_Curr_ComType_PUCK(INT8U Id)
{  
  INT8U i;
  
  for(i=CHANEL_MIN;i<=CHANEL_MAX;i++)
  {
    if(Chanel_Para[i].ExitFlag)
    {
      Chanel_Para[i].ExitFlag++;
      if(Chanel_Para[i].ExitFlag>=SECOND_2(2))
      {
        Chanel_Para[i].ExitFlag=0;  
      }
      if(i==CHANEL_IRDA)
        TURN_ON_LIGHT;
      SET_STRUCT_SUM(Chanel_Para[i]);
      return (1<<i);
    }
  }      
  return 0;  
  
}
/**********************************************************************************************
函数名称:SFLOAT BCD_To_Real_Convert(INT8U Src[],INT8U Src_Len,INT8U Decimal_Digits,INT8U Sign)
函数功能:将国电数据转化为真实数据
入口参数:Src,原始国电数据缓冲区,BCD码!!!!!!!!!!
Src_Len,原始数据长度
Decimal_Digits,小数位数
Sign,1表示是由符号数,0表示无符号数
出口参数:无

FP32S BCD_To_Real_Convert(INT8U Src[],INT8U Src_Len,INT8U Decimal_Digits,INT8U Sign)
{
  INT8U i,Temp;//i表示处理整个Src的第i个4位,j表示记录处理Src的第j个字节
  
  FP32S Integral,Decimal,Result;
  
  Integral=0; //记录整数部分的值
  Decimal=0; //记录小数部分的值
  
  for(i=0;i<Decimal_Digits;i++)
  {
    Temp=Src[i/2]; //给一个零时变量
    
    if(Sign==1 && i/2==Src_Len-1 ) //如果处理到最后一个字节且是由符号数,最高位为符号位
      Temp=Temp&0x7F;//先将符号位去掉
    
    if(i%2==1) //处理的是高4位
      Temp=Temp>>4;
    
    Decimal=Decimal/10; //每计算一位都除10,等价于小数点移位
    Decimal+=(FP32S)(Temp&0x0F)/10; //每次循环取低四位作为小数点后第一位   		
    
  }
  
  for(i=0;i<Src_Len*2-Decimal_Digits;i++)//处理整数部分
  {
    Temp=Src[Src_Len-1-i/2]; //给一个零时变量
    
    if(Sign==1 && i/2==0) //如果处理到最后一个字节且是由符号数,最高位为符号位
      Temp=Temp&0x7F;//先将符号位去掉
    
    if(i%2==0) //处理的是高4位
      Temp=Temp>>4;
    
    Integral=Integral*10; //每计算一位都乘10,等价于小数点移位      		
    Integral+=(FP32S)(Temp&0x0F); //每次循环取低四位作为个位    	
    
  }  
  Result=Integral+Decimal;//整数部分与小数部分相加得到最后结果
  
  if(Sign==1 && (Src[Src_Len-1] & 0x80)!=0) //如果是有符号数,且最高位为1,表示是负数
    Result=0-Result;
  return Result;	
}
*******************************************************************************/

/***********************************************************************
函数功能：检查Src的Len长度数据，全和Byte相等？
入口：无
出口：1------Src内容都是Byte；0--------不全是
***********************************************************************/
INT8U CHeck_Same_Byte(INT8U *Src,INT8U Byte,INT16U Len)
{
  INT16U i;
  
  for(i=0;i<Len;i++)
  {
    if(Src[i]!=Byte)
      return 0;    
  }  
  return 1;   
}
void SYS_TimeDly_Sec(INT32U n)
{
#ifndef OS_DEBG_EN
  Delayn10MS(n*100);
#else
  OS_TimeDly_Sec(1);
#endif  
}

void SYS_TimeDly_Ms(INT32U n)
{  
#ifndef OS_DEBG_EN
  if(n<10)
    Delayn10MS(1);
  else
    Delayn10MS(n/10);
#else
  OS_TimeDly_Ms(n);
#endif  
}