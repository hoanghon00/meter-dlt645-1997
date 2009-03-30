#define MEMORY_C
#include "Includes.h"

//物理存储器测试,对每个物理存储器读写一个字节进行测试
INT8U Check_PHY_Mem()
{
  INT8U Re,i,Test[5];
  INT8U Bak;
  
  Re=NO_ERR;
  DEBUG_PRINT(HUCK,DEBUG_0,"PHY_MEM Write & Read Check Start");
  for(i=0;i<S_NUM(Sys_PHY_Mem);i++)
  {
    Read_PHY_Mem(Sys_PHY_Mem[i].MemNo,Sys_PHY_Mem[i].Mem_Size-1,Test,1,Test,1);//先将原字节备份
    Bak=Test[0];
    Test[0]=0x5A;
    Write_PHY_Mem(Sys_PHY_Mem[i].MemNo,Sys_PHY_Mem[i].Mem_Size-1,Test,1);//写入测试字节
    Test[0]=0x00;
    Read_PHY_Mem(Sys_PHY_Mem[i].MemNo,Sys_PHY_Mem[i].Mem_Size-1,Test,1,Test,1);//读出测试字节
    if(Test[0]!=0x5A)
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"PHY Mem%d Write && Read Test failed!",Sys_PHY_Mem[i].MemNo);
      Re=PHY_MEM_ERR;
    }
    Test[0]=Bak;
    Write_PHY_Mem(Sys_PHY_Mem[i].MemNo,Sys_PHY_Mem[i].Mem_Size-1,Test,1);
  }
  
  //if(Re==NO_ERR)
    //DEBUG_PRINT(HUCK,DEBUG_0,"PHY_MEM Write & Read Test Succeed!");
  
  DEBUG_PRINT(HUCK,DEBUG_0,"PHY_MEM Write & Read Check End");
  return Re;
}

//检查内存分布是否合理,主要是检查逻辑存储器与物理存储器之间的映射
//NO_ERR表示检查成功
//MEM_MAP_ERR表示检查失败
INT8U Check_LOG_Mem_Map()
{
  INT8U i,j,Re;
  INT32U Size;
  
  Re=NO_ERR;
  //逐个物理内存遍历
  DEBUG_PRINT(HUCK,DEBUG_0,"----PHY_LOG_MEM Map Check Start!----");
  
  for(i=0;i<sizeof(Sys_PHY_Mem)/sizeof(S_PHY_Mem_Info);i++)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"PHY Mem %d size=%ld",i,Sys_PHY_Mem[i].Mem_Size);    
    Size=0;
    //检查每个逻辑内存对应的物理内存分布是否合理
    for(j=0;j<sizeof(Sys_LOG_Mem)/sizeof(S_LOG_Mem_Info);j++)
    {
       //第i个逻辑内存使用了该物理内存
       if(Sys_LOG_Mem[j].PHY_Mem_Info.MemNo==Sys_PHY_Mem[i].MemNo) 
       {
         DEBUG_PRINT(HUCK,DEBUG_0,"----LOG Mem %2d Size:%5ld,Addr:%5ld-->%5ld ",\
                 Sys_LOG_Mem[j].LOG_MemNo,Sys_LOG_Mem[j].PHY_Mem_Info.Mem_Size,
                 Sys_LOG_Mem[j].PHY_Mem_Info.Mem_Addr,Sys_LOG_Mem[j].PHY_Mem_Info.Mem_Addr + Sys_LOG_Mem[j].PHY_Mem_Info.Mem_Size);
 
         //分配的物理内存起始地址应该在，已分配的空间外
         if(Sys_LOG_Mem[j].PHY_Mem_Info.Mem_Addr != Size)//起始地址不应在已经分配的范围内
         {
           //ASSERT(A_WARNING,0);
           DEBUG_PRINT(HUCK,DEBUG_0,"LOG Mem %d Map Error",j);
           Re |= LOG_MEM_MAP_ERR;
         }
         //计算该物理内存内已分配的空间总和
         Size+=Sys_LOG_Mem[j].PHY_Mem_Info.Mem_Size;
       }     
    }
    if(Size>Sys_PHY_Mem[i].Mem_Size)//总的大小不应该超过物理内存大小
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"PHY Mem %d not enough space",i);
      Re |= LOG_MEM_MAP_ERR;
    }
  }
  DEBUG_PRINT(HUCK,DEBUG_0,"----PHY_LOG_MEM Map Check End!----");
  return Re;
}

//获取某个逻辑存储器的大小
INT32U Get_LOG_Mem_Size(INT8U MemNo)
{
  INT8U i;

  for(i=0;i<S_NUM(Sys_LOG_Mem);i++)
  {
    if(Sys_LOG_Mem[i].LOG_MemNo==MemNo)
      return Sys_LOG_Mem[i].PHY_Mem_Info.Mem_Size;
  }
  ASSERT(A_WARNING,0);
  return 0;
  
}

//获取某个物理存储器的大小
INT32U Get_PHY_MEM_Size(INT8U MemNo)
{
  INT8U i;

  for(i=0;i<sizeof(Sys_PHY_Mem)/sizeof(S_PHY_Mem_Info);i++)
  {
    if(Sys_PHY_Mem[i].MemNo==MemNo)
      return Sys_PHY_Mem[i].Mem_Size;
  }
  ASSERT(A_WARNING,0);
  return 0;  
}

//从物理存储器读数据
//MemNo，物理存储器编号
//Offset,物理存储器内的偏移
//pDst, 写的源数据
//RD_Len,需要读取的数据长度 
//pDst_Start,目标缓冲区的界定起始地址
//DstLen，目标缓冲区的界定长度
//返回结果:NO_ERR表示读取成功
INT16U Read_PHY_Mem(INT8U MemNo,INT32U Offset,void *pDst,INT16U RD_Len,void *pDst_Start,INT16U DstLen)
{
  INT8U i,j;
  INT8U Re;
  INT8U Head,Tail;
  
  ASSERT(A_WARNING,(INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst_Start+RD_Len<=(INT8U *)pDst_Start+DstLen);
  
  for(i=0;i<S_NUM(Sys_PHY_Mem);i++)
  {
    if(Sys_PHY_Mem[i].MemNo==MemNo)
    {
      if(Offset+RD_Len<=Sys_PHY_Mem[i].Mem_Size)
      {
        Head=*((INT8U *)pDst-1);//先保存目标缓冲区的头和尾,读完后再判断头和尾是否发生变化,保证读不会产生溢出
        Tail=*((INT8U *)pDst+RD_Len);
        
        for(j=0;j<3;j++)
        {
          Re=Read_PHY_Mem_PUCK(MemNo,Offset,pDst,RD_Len,pDst_Start,DstLen);
          if(1==Re)
            break;
          OS_TimeDly_Ms(50);
          ASSERT(A_WARNING,0);
        }
        
        if(Head!=*((INT8U *)pDst-1) || Tail!=*((INT8U *)pDst+RD_Len))
        {
          ASSERT(A_WARNING,0);
          DEBUG_PRINT(HUCK,DEBUG_0,"Read_PHY_Mem Error,Mem_ID=%d,Offset=%d,RD_Len=%d,%s",MemNo,Offset,RD_Len,(Head!=*((INT8U *)pDst-1))?"Head Error":"Tail Error");
          DEBUG_PRINT(HUCK,DEBUG_0,"Bef_Head=0x%x,Bef_Tail=0x%x,Aft_Head=0x%x,Aft_Tail=0x%x",Head,Tail,*((INT8U *)pDst-1),*((INT8U *)pDst+RD_Len));
          DEBUG_BUF_PRINT((INT8U *)pDst+RD_Len,20,PRINT_HEX,0);
        }
        if(1==Re)
          return NO_ERR;
      }
    }
  }
  ASSERT(A_WARNING,0);
  return PHY_MEM_NO_ERR;
}

//向物理存储器写数据
//MemNo，物理存储器编号
//Offset,物理存储器内的偏移
//pSrc, 写的源数据
//写的源数据长度
//返回结果:
//NO_ERR表示写成功
INT8U Write_PHY_Mem(INT8U MemNo,INT32U Offset,void *pSrc,INT16U SrcLen)
{
  INT8U i;
  INT8U Re;
  INT8U Head,Tail;
  
  for(i=0;i<S_NUM(Sys_PHY_Mem);i++)
  {
    if(Sys_PHY_Mem[i].MemNo==MemNo)
    {
      if(Offset+SrcLen<=Sys_PHY_Mem[i].Mem_Size) 
      {
        Head=*((INT8U *)pSrc);//先保存源缓冲区的头和尾,读完后再判断头和尾是否发生变化,保证写不会破坏源数据
        Tail=*((INT8U *)pSrc+SrcLen-1);
        Re=Write_PHY_Mem_PUCK(MemNo,Offset,pSrc,SrcLen);
        if(Head!=*((INT8U *)pSrc) || Tail!=*((INT8U *)pSrc+SrcLen-1))
        {
          ASSERT(A_WARNING,0);
          DEBUG_PRINT(HUCK,DEBUG_0,"Write_PHY_Mem Error,Mem_ID=%d,Offset=%d,SrcLen=%d,%s",MemNo,Offset,SrcLen,(Head!=*((INT8U *)pSrc))?"Head Error":"Tail Error");
          DEBUG_PRINT(HUCK,DEBUG_0,"Bef_Head=0x%x,Bef_Tail=0x%x,Aft_Head=0x%x,Aft_Tail=0x%x",Head,Tail,*(INT8U *)pSrc,*((INT8U *)pSrc+SrcLen-1));
          DEBUG_BUF_PRINT((INT8U *)pSrc,20,PRINT_HEX,0);
        }
        if(1==Re)
          return NO_ERR;
      }
    }
    
  }
  ASSERT(A_WARNING,0);
  return PHY_MEM_NO_ERR;//物理存储器序号错
}

//读写存储函数中的喂狗
void WR_Memory_Clear_Dog()
{
  INT8U Re; 
  static S_Int16U Clr_Dog_Counts={CHK_BYTE,0,CHK_BYTE};  
  static S_Int32U Clr_Dog_Counts0={CHK_BYTE,0,CHK_BYTE};
  static S_Int32U Clr_Dog_Counts1={CHK_BYTE,0,CHK_BYTE};
  static S_Int8U Sec={CHK_BYTE,0xFF,CHK_BYTE};
  static S_Int8U Task_ID={CHK_BYTE,0xFF,CHK_BYTE};
  
  Re=1;
  Re &= CHECK_STRUCT_VAR(Clr_Dog_Counts);
  Re &= CHECK_STRUCT_VAR(Clr_Dog_Counts0);
  Re &= CHECK_STRUCT_VAR(Clr_Dog_Counts1);
  Re &= CHECK_STRUCT_VAR(Sec);
  Re &= CHECK_STRUCT_VAR(Task_ID);
  
  if(Re!=1)
    ASSERT(A_WARNING,0);
  
  if(Get_Cur_Task_Num()==1 || CLEAR_END_FLAG!=Clear_Flag.Flag)//当前只有一个任务，也就是只有主任务在进行初始化的情况下清狗
  {
    if(Clr_Dog_Counts.Var<=MAX_INIT_CLR_DOG_COUTS)
    {
      Clr_Ext_Inter_Dog();
      Clr_Dog_Counts.Var++;
    }
    else
    {
      ASSERT(A_WARNING,0);
      Soft_Reset();
    }
  }
  else//所有任务都已经创建运行，此时判断一分钟内喂狗多少次?
  {
    if(Sec.Var!=Cur_Time1.Sec)//时间有变化，说明程序让出来了，给其他任务执行
    {
      Clr_Dog_Counts0.Var=0;
      Sec.Var=Cur_Time1.Sec;
    }
    
    Clr_Ext_Inter_Dog();    //清CPU外部看门狗 
    Clear_Task_Dog();  
    //一分钟以内连续读写超过1000次，说明已经锁死在读写函数中了,
    Clr_Dog_Counts0.Var++;

    if(Clr_Dog_Counts0.Var > MAX_INIT_CLR_DOG_COUTS)
    {
      ASSERT(A_WARNING,0);
      DEBUG_PRINT(HUCK,DEBUG_0,"WR_Memory_Clear_Dog Error,soft reset");
      Soft_Reset();
    }
  }
  
  //同一个任务内连续读写超过MAX_INIT_CLR_DOG_COUTS次，说明已经锁死了
  if(Get_Cur_Task_ID()!=Task_ID.Var)//不同的任务执行读写操作，说明没有锁死
  {
    Task_ID.Var=Get_Cur_Task_ID();
    Clr_Dog_Counts1.Var=0;
  }
  else
  {
    Clr_Dog_Counts1.Var++;
    if(Clr_Dog_Counts1.Var > MAX_INIT_CLR_DOG_COUTS)
    {
      ASSERT(A_WARNING,0);
      Soft_Reset();
    }
  }
}

//从逻辑存储器读数据
//MemNo，逻辑存储器编号
//Offset,逻辑存储器内的偏移
//pSrc, 写的源数据
//写的源数据长度
//返回结果:1表示写成功，0表示写失败
INT16U Read_LOG_Mem(INT8U MemNo,INT32U Offset,void *pDst,INT16U RD_Len,void *pDst_Start,INT16U DstLen)
{
  INT8U i,Re;

  WR_Memory_Clear_Dog();
  
  ASSERT(A_WARNING,(INT8U *)pDst>=(INT8U *)pDst_Start && (INT8U *)pDst_Start+RD_Len<=(INT8U *)pDst_Start+DstLen);
  
  for(i=0;i<sizeof(Sys_LOG_Mem)/sizeof(S_LOG_Mem_Info);i++)
  {
    if(Sys_LOG_Mem[i].LOG_MemNo==MemNo)
    {
      if(Offset+RD_Len<=Sys_LOG_Mem[i].PHY_Mem_Info.Mem_Size)
      {
        //读物理存储器中的数据
        Power_Down_Check();
        Re=Read_PHY_Mem(Sys_LOG_Mem[i].PHY_Mem_Info.MemNo,\
                        Sys_LOG_Mem[i].PHY_Mem_Info.Mem_Addr+Offset,\
                        pDst,\
                        RD_Len,\
                        pDst_Start,\
                        DstLen);
        if(ASSERT(A_WARNING,Re==NO_ERR))
         return Re;
        else
         return NO_ERR;
      }
      else
      {
         ASSERT(A_WARNING,0);
         return LOG_MEM_ADDR_ERR;//逻辑存储器地址错
      }
    }
  }
  ASSERT(A_WARNING,0);
  return LOG_MEM_NO_ERR;//逻辑存储器序号错
}

//向逻辑存储器写数据
//MemNo，逻辑存储器编号
//Offset,逻辑存储器内的偏移
//pSrc, 写的源数据
//写的源数据长度
//返回结果:Re表示写成功，其他表示写失败
INT8U Write_LOG_Mem(INT8U MemNo,INT32U Offset,void *pSrc,INT16U SrcLen)
{
  INT8U i,Re;
  
  WR_Memory_Clear_Dog();
  
  for(i=0;i<S_NUM(Sys_LOG_Mem);i++)
  {
    if(Sys_LOG_Mem[i].LOG_MemNo==MemNo)
    {
      if(Offset+SrcLen<Sys_LOG_Mem[i].PHY_Mem_Info.Mem_Size)
      {
        Power_Down_Check();
        Re=Write_PHY_Mem(Sys_LOG_Mem[i].PHY_Mem_Info.MemNo,\
                        Sys_LOG_Mem[i].PHY_Mem_Info.Mem_Addr+Offset,\
                        pSrc,\
                        SrcLen);
        if(ASSERT(A_WARNING,Re==NO_ERR))
         return Re;
        else
         return NO_ERR;
      }
      else
      {
         ASSERT(A_WARNING,0);
         return LOG_MEM_ADDR_ERR;
      }
    }
  }
  return LOG_MEM_NO_ERR;
}
#undef MEMORY_C

