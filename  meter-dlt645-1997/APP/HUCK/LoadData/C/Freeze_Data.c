#define FREEZE_DATA_C
#include "includes.h"

void Read_Freeze_Data_Para_From_Rom()
{
  INT8U Temp[3];
  INT8U Err;
  
  //总电量冻结时间
  if(Read_Storage_Data(ENERGY_FREEZE_TIME,Temp,Temp,sizeof(Temp),&Err)==2 && NO_ERR==Err)
  {
    if(ASSERT(A_WARNING,Temp[0]<0x60 && Temp[1]<0x24))
    {
      Energy_Freeze_Time.Min=99;
      Energy_Freeze_Time.Hour=99;
      SET_STRUCT_SUM(Energy_Freeze_Time);
    }
    else
    {
      Energy_Freeze_Time.Min=Bcd2Hex_Byte(Temp[0]);
      Energy_Freeze_Time.Hour=Bcd2Hex_Byte(Temp[1]);
      SET_STRUCT_SUM(Energy_Freeze_Time);
    } 
  }
  /*
  //无功冻结时间
  if(Read_Storage_Data(REACTIVE_FREEZE_TIME,Temp,Temp,sizeof(Temp),&Err)==2 && NO_ERR==Err)
  {
    if(ASSERT(A_WARNING,Temp[0]<0x60 && Temp[1]<0x24))
    {
      Reactive_Freeze_Time.Min=99;
      Reactive_Freeze_Time.Hour=99;
      SET_STRUCT_SUM(Reactive_Freeze_Time);
    }
    else
    {
      Reactive_Freeze_Time.Min=Bcd2Hex_Byte(Temp[0]);
      Reactive_Freeze_Time.Hour=Bcd2Hex_Byte(Temp[1]);
      SET_STRUCT_SUM(Reactive_Freeze_Time);
    } 
  }
  */
}

//冻结数据处理
INT16U Get_Period_Freeze_Data(S_Freeze_Data *pData,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen)
{
  INT8U i,FollowFlag,Err;
  INT16U Len,Len1;
  
  Len=0;
  for(i=0;i<pData->Data_Num;i++)
  {
    Len1=_Get_DLT645_Data(pData->Freeze_DI[i],(void *)0,0,(INT8U *)pDst+Len,(INT8U *)pDst,DstLen,FOR_EVENT,&FollowFlag,&Err); 
    if(ASSERT(A_WARNING,Len1>0 && NO_ERR==Err))
    {
      return 0; 
    }
    else
    {
      Len+=Len1; 
    }
  }
  Len1=Get_Storage_Data_Len(pData->DI);
  DEBUG_PRINT(HUCK,DEBUG_0,"Freeze data,Len=%u,Len1=%u",Len,Len1);
  if(ASSERT(A_WARNING,Len==Len1))
    return 0;
  else
    return Len;
}

//定期冻结数据处理
void Period_Freeze_Data_Proc()
{
  INT8U Re;
  static S_Int8U Min_Bak={CHK_BYTE,0xFF,CHK_BYTE};
 
  Re=CHECK_STRUCT_VAR(Min_Bak);
  ASSERT(A_WARNING,1==Re);
  
  if(Min_Bak.Var!=Cur_Time0.Time[T_MIN])
  {
    Min_Bak.Var=Cur_Time0.Time[T_MIN];
    //每日冻结时间点到？
    //冻结总电量
    if(Energy_Freeze_Time.Min==Cur_Time0.Time[T_MIN] &&
       Energy_Freeze_Time.Hour==Cur_Time0.Time[T_HOUR])
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"----------Period Freeze Data!----------");
      //对Freeze_Data中定义的每个数据项进行冻结
      Freeze_Right_Now(0);//电量和瞬时量
      Freeze_Right_Now(1);//四象限无功
    }
    /*
    //冻结无功电量
    if(Reactive_Freeze_Time.Min==Cur_Time0.Time[T_MIN] &&
       Reactive_Freeze_Time.Hour==Cur_Time0.Time[T_HOUR])
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"----------Period Freeze Data!----------");
      //对Freeze_Data中定义的每个数据项进行冻结
      Freeze_Right_Now(1);
      
    }
    */
  }
}

//立即冻结，用于响应立即冻结命令
void Freeze_Right_Now(INT8U Index)
{
  INT16U Len;
  
 // for(i=0;i<S_NUM(Freeze_Data)-1;i++)
  {
    OS_Sem_Pend(PUB_BUF0_SEM_ID);
    Len=Get_Period_Freeze_Data((S_Freeze_Data *)&Freeze_Data[Index],(INT8U *)Pub_Buf0,(INT8U *)Pub_Buf0,sizeof(Pub_Buf0));
    Write_Storage_Data(Freeze_Data[Index].DI,(INT8U *)Pub_Buf0,Len);
    OS_Sem_Post(PUB_BUF0_SEM_ID);
  }  
}

//收到立即冻结帧处理
INT8U Protocol_Freeze_Data(INT8U Flag)
{
  INT8U Re;

  if(Flag==0x03)
  {
   Freeze_Right_Now(0);//瞬时量和电量冻结
   Re=NO_ERR;
  }
  else if(Flag==0x13)//总电量积极冻结
  {
   Freeze_Right_Now(1);
   Re=NO_ERR;        
  }
  else
    Re=PROTOCOL_DATA_ERR;
  
  return Re;
}

//置冻结数据的默认数据
void Set_Def_Freeze_Data()//置默认的冻结数据
{
  INT8U i;
  INT16U Len;
  
  OS_Sem_Pend(PUB_BUF0_SEM_ID); 
  mem_set((INT8U *)Pub_Buf0,0,sizeof(Pub_Buf0),(INT8U *)Pub_Buf0,sizeof(Pub_Buf0));
  
  for(i=0;i<S_NUM(Freeze_Data);i++)
  {
    Len=Get_Storage_Data_Len(Freeze_Data[i].DI);
    Write_Storage_Data(Freeze_Data[i].DI,(INT8U *)Pub_Buf0,Len);
  }
  OS_Sem_Post(PUB_BUF0_SEM_ID);
}

#undef FREEZE_DATA_C
