#define AUTHORITY_C
#include "Includes.h"

//extern const S_Protocol_Data_Info Protocol_Data[];
//检查表计内部的跳线是否跳上了
INT8U Check_Meter_Factory_Status()
{
  if(Get_Meter_Hard_Mode()!=MODE_FAC)
    return 0;  //非工厂状态
  else
    return 1;  //工厂状态  
}

//检查是否能否输出调试信息
INT8U Check_Debug_En()
{
  if(Get_Meter_Hard_Mode()==MODE_RUN)
    return 0;
  else
    return 1;
}

//检查表计是否处于编程状态，是的返回1，否则返回0
INT8U Check_Meter_Prog_Status()
{
  //return 1;//test
  return Check_Prog_Status();
}

//检查设置通信地址的案件是否按下，按下返回1，否则返回0
INT8U Check_Set_ADDR_Key()
{
  return Check_MeterAddr_Status();
}

//检查清除数据的权限,要处于工厂状态或者编程状态，权限标志
INT8U Check_Clear_Data_Authority()
{
  if((Check_Meter_Prog_Status()==0 && Check_Meter_Factory_Status()==0) ||\
     CHECK_STRUCT_VAR(User_Authority_Flag)==0 ||\
     CHECK_STRUCT_SUM(User_Authority_Flag)==0 ||\
    User_Authority_Flag.Flag!=AUTHORITY_FLAG)
    return 0;
  else
    return 1;
  
}

//清除密码错误信息
void Clear_PSW_Err_Info()
{
  mem_set((void *)&PSW_Err_Info,0,sizeof(PSW_Err_Info),(void *)&PSW_Err_Info,sizeof(PSW_Err_Info));
  INIT_STRUCT_VAR(PSW_Err_Info);
  SET_STRUCT_SUM(PSW_Err_Info);
}

//获取当前的用户权限,也就是密码,如果发送的数据不需要密码，则直接返回权限PSW_0
INT8U Get_User_Authority(INT8U Ctrl_Code,INT8U *pSrc,INT8U SrcLen,INT8U *pErr)
{
  INT8U Posi;//密码在数据缓冲中的起始位置
  INT8U Level;//当前密码级别
 
  *pErr=NO_ERR;
  
  //暂时屏蔽 test
  if(Check_Meter_Factory_Status())//表内跳线跳上了，具备超级权限
    return PSW_FACTORY;
  
  if(PSW_Err_Info.Set_En_Mins>0)//在权限开放的时段内认为用户具备0级权限
    return PSW_0;
  
//以下都没有条上表内跳线  
  if(Ctrl_Code==C_READ_DATA || Ctrl_Code==C_READ_FOLLOW_DATA)
  {
    return PSW_0;//不需要密码的情况下，认为具备了0级密码
  }
  else if(Ctrl_Code==C_SET_DATA)//写数据
  {
    if(Check_Meter_Prog_Status()==0)//没有处于编程状态，不具备任何写权限
    {
      *pErr=PROTOCOL_AUTH_ERR;
      return PSW_NULL;
    }
    Level=pSrc[2];//密码级别
    Posi=3;//密码开始位置
  }
  else if(Ctrl_Code==C_ADJUST_METER)//校表,校表必须在工厂状态
  {
    *pErr=PROTOCOL_AUTH_ERR;
    return PSW_NULL;//此时没有处于跳线状态
  }
  else if(Ctrl_Code==C_ADJUST_TIME)//远程校时
  {
    if(Mode_Word.Mode[0].Bit.Bit4==1)//需要密码？
    {
      Level=pSrc[0];//密码级别
      Posi=1;//密码起始位置
    }
    else
    {
      return PSW_0;//不需要密码的情况下，认为具备了0级密码
    }
  }
  else if(Ctrl_Code==C_SET_ADDR)//设置表地址不需要密码权限，只需要按下某个按键
  {
    if(Check_Set_ADDR_Key()==0)//按键没有按下，返回权限错
    {
      *pErr=PROTOCOL_AUTH_ERR;
      return PSW_NULL;
    }
    else 
      return PSW_0;//不需要密码的情况下，认为具备了0级密码
  }
  else if(Ctrl_Code==C_SET_COM_BAUD)//设置通信波特率
  {
    return PSW_0;//不需要密码
  }
  else if(Ctrl_Code==C_SET_PSW)//修改密码
  { 
    if(Check_Meter_Prog_Status()==0)//没有处于编程状态，不具备任何写权限
    {
      *pErr=PROTOCOL_AUTH_ERR;
      return PSW_NULL;
    }
    
    Level=pSrc[0];//数据域第0字节就是密码级别
    Posi=1;    
  }
  else if((Ctrl_Code==C_CLR_ALL && 4==SrcLen) || Ctrl_Code==C_CLR_DEMAND ||\
          Ctrl_Code==C_CLR_EVENT || Ctrl_Code==C_CLR_STAT)
  {
    if(Check_Meter_Prog_Status()==0)//没有处于编程状态，不具备任何写权限
    {
      *pErr=PROTOCOL_AUTH_ERR;
      return PSW_NULL;
    }
    
    Level=pSrc[0];
    Posi=1;         
  }
  else if(Ctrl_Code==C_CLR_ALL && 3==SrcLen)
  {
    if(*pSrc==(0x36-0x33) || *pSrc==(0x46-0x33))//数据长度为4是总清，长度3是冻结
      return PSW_0;//冻结命令
  }
  else
  {
    *pErr=PROTOCOL_AUTH_ERR;   
    return PSW_NULL;
  }
  //密码级别在0-2之间才是正确的
  if(Level>2)
  {
    *pErr=PROTOCOL_PSW_ERR;//密码错误
    return PSW_NULL;
  }
  
  //比较密码是否相同
  if(memcmp(pSrc+Posi,(void *)Cur_PSW.PSW[Level],3)!=0)
  {
    *pErr=PROTOCOL_PSW_ERR;//密码错误
    return PSW_NULL;
  }
  
  if(Level==0)
    return PSW_0;
  else if(Level==1)
    return PSW_1;
  else
    return PSW_2;    
}

//检查修改某个数据需要的权限
INT8U Get_Protocol_Data_Authority(INT8U Ch,INT8U Ctrl_Code,INT8U *pSrc,INT8U SrcLen)//INT16U DI)
{
  INT8U i,PSW_Flag,Posi;
  INT16U DI;
  
  //先获得这个数据的密码标记
  if(Ctrl_Code==C_READ_DATA || Ctrl_Code==C_READ_FOLLOW_DATA)
  {
    return PSW_3;//读取数据和后续数据不需要密码，因此默认返回密码3,最低等级
  }
  else//以下都是写参数相关
  {
    /*
    Ch=CHANEL_485;//待改
    if(CHANEL_MOUDEL==Ch)//模式字1的bit3是否允许从串口2设参数？0表示不允许
    {
      if(Ctrl_Code!=C_ADJUST_TIME && 0==Mode_Word.Mode[0].Bit.Bit3)
        return PSW_NULL;     
    }
    */
    
    if(Ctrl_Code==C_SET_DATA)
    {
       DI=0;
       mem_cpy(&DI,pSrc,2,&DI,sizeof(DI));//DI=*(INT16U *)pSrc;
       PSW_Flag=Get_Protocol_Data_PSW_Flag(DI);
       
       //没有找到该数据相应的密码标记
       if(ASSERT(A_WARNING,PSW_Flag!=PSW_NULL_FLAG && PSW_Flag!=PSW_RD_ONLY))
         return PSW_NULL;
       
       //如果密码标记为跳线,直接返回PSW_SUPPER
       if(PSW_Flag==PSW_FACTORY_STATUS)
         return PSW_FACTORY;
      
       //在PSW_Map表中寻找该数据的密码在密码状态字中的位置
       Posi=0xFF;
       for(i=0;i<S_NUM(PSW_Map);i++)
       {
         if(PSW_Map[i].PSW_Flag==PSW_Flag)
         {
           Posi=PSW_Map[i].Posi;
           break;
         }
       }
       if(ASSERT(A_WARNING,Posi<15))//此时Posi==0xFF,表示没有找到位置
         return PSW_NULL;
    }
    else if(Ctrl_Code==C_ADJUST_METER)//校表
    {
      /*
      if(Mode_Word.Mode[0].Bit.Bit2==0)//不允许出厂较表,则需要在工厂状态
        return PSW_FACTORY;//需要超级密码,即表内跳线跳上，同时处于编程状态
      else
        Posi=14;//和数据总清具备需要同样的权限?是此Posi?此处待改
      */
      return PSW_FACTORY;//需要超级密码,即表内跳线跳上，同时处于编程状态
    }
    else if(Ctrl_Code==C_ADJUST_TIME)//广播校时
    {
      if(Mode_Word.Mode[0].Bit.Bit4==1)//需要密码？
        Posi=0x04;
      else
        return PSW_3;//因为不需要密码，所以给出一个最低级密码(在判断用户密码时会给一个较高级密码) 
    }
    else if(Ctrl_Code==C_SET_ADDR)
    {
      return PSW_3; //设表地址，因为不需要密码，所以给出一个最低级密码(在判断用户密码时会给一个较高级密码) 
    }
    else if(Ctrl_Code==C_SET_COM_BAUD)
    {
      return PSW_3; //设波特率，因为不需要密码，所以给出一个最低级密码     
    }
    else if(Ctrl_Code==C_SET_PSW)
    {
      if(pSrc[4]==0)//设置的密码级别
        Posi=0x07; //在密码权限控制字中的位置
      else if(pSrc[4]==1)
        Posi=0x06;
      else if(pSrc[4]==2)
        Posi=0x05;
      else
        return PSW_NULL;//不存在这个权限 
      
      if(pSrc[0]>pSrc[4])//如果用户权限比设置的密码权限低，当然也不能设置
        return PSW_NULL;
    }
    else if(Ctrl_Code==C_CLR_ALL)//总清
    {
      if(SrcLen==4)//源数据4字节是总清
      {
        if(Mode_Word.Mode[0].Bit.Bit2==0)//不允许出厂总清，则需要在工厂状态
          return PSW_FACTORY;
        else
          Posi=14;
      }
      else//源数据3字节是冻结命令,不需要密码操作，所以给出一个最低级的密码要求
        return PSW_3;
    }
    else if(Ctrl_Code==C_CLR_DEMAND || Ctrl_Code==C_CLR_EVENT || Ctrl_Code==C_CLR_STAT)
    {
      Posi=13;
    }
    else 
      return PSW_NULL;
  }
  
 //找到哪级密码权限控制字中设置了该数据类的权限
 for(i=3;i>=1;i--)
 {
   if(GET_BIT(User_Authority_Ctrl.PSW[i-1],Posi))
   {
     if(i==1)
       return PSW_0;
     else if(i==2)
       return PSW_1;
     else if(i==3)
       return PSW_2;
   }
 }
 return PSW_0;//如果0-2级密码权限都没有设置该项，则默认为0级密码可设
}
#undef AUTHORITY_C
