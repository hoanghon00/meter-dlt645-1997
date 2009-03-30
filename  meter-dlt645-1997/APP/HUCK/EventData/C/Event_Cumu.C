#define EVENT_CUMU0_C
#include "Includes.h"

//#define INIT(x,y) y
CONST S_DI_Array Event_DI_Array[]=
{
  {.DI_Num=19,{0x9010,0x9110,0x9020,0x9120,0xE701,0xE704,0xE707,0xE70A,0xE702,0xE705,0xE708,0xE70B,0xE703,0xE706,0xE709,0xE70C,DI_A_AH,DI_B_AH,DI_C_AH}},//失压
  {.DI_Num=16,{0x9010,0x9110,0x9020,0x9120,0xE701,0xE704,0xE707,0xE70A,0xE702,0xE705,0xE708,0xE70B,0xE703,0xE706,0xE709,0xE70C}},//失流
  {.DI_Num=8,{0x9010,0x9020,0xE701,0xE707,0xE702,0xE708,0xE703,0xE709}},//反向
  {.DI_Num=0,{0}}//全失压、全失流、断相只计次数和时间
};


CONST INT8U Single_Total_Event_Map[][2]=
{
  //失压
  {ID_EVENT_A_LOSS_VOLT, ID_EVENT_TOTAL_LOSS_VOL},  
  {ID_EVENT_B_LOSS_VOLT, ID_EVENT_TOTAL_LOSS_VOL},
  {ID_EVENT_C_LOSS_VOLT, ID_EVENT_TOTAL_LOSS_VOL},
  {ID_EVENT_AB_LOSS_VOLT, ID_EVENT_TOTAL_LOSS_VOL},
  {ID_EVENT_AC_LOSS_VOLT, ID_EVENT_TOTAL_LOSS_VOL},
  {ID_EVENT_BC_LOSS_VOLT, ID_EVENT_TOTAL_LOSS_VOL},
  {ID_EVENT_ABC_LOSS_VOLT, ID_EVENT_TOTAL_LOSS_VOL},
  {ID_EVENT_ALL_LOSS_VOLT, ID_EVENT_TOTAL_LOSS_VOL},
  //失流
  {ID_EVENT_A_LOSS_CURR, ID_EVENT_TOTAL_LOSS_CUR},
  {ID_EVENT_B_LOSS_CURR, ID_EVENT_TOTAL_LOSS_CUR},
  {ID_EVENT_C_LOSS_CURR, ID_EVENT_TOTAL_LOSS_CUR},
  {ID_EVENT_AB_LOSS_CURR, ID_EVENT_TOTAL_LOSS_CUR},
  {ID_EVENT_AC_LOSS_CURR, ID_EVENT_TOTAL_LOSS_CUR},
  {ID_EVENT_BC_LOSS_CURR, ID_EVENT_TOTAL_LOSS_CUR},
  {ID_EVENT_ALL_LOSS_CURR, ID_EVENT_TOTAL_LOSS_CUR}, 
  //反向
  {ID_EVENT_A_NEG_CURR, ID_EVENT_TOTAL_NEG_CUR},
  {ID_EVENT_B_NEG_CURR, ID_EVENT_TOTAL_NEG_CUR},
  {ID_EVENT_C_NEG_CURR, ID_EVENT_TOTAL_NEG_CUR},
  //断相
  {ID_EVENT_A_LOSS_PARSE, ID_EVENT_TOTAL_BREAK_VOL},
  {ID_EVENT_B_LOSS_PARSE, ID_EVENT_TOTAL_BREAK_VOL},
  {ID_EVENT_C_LOSS_PARSE, ID_EVENT_TOTAL_BREAK_VOL},
};


CONST S_Event_Cumu_Data_Info Event_Cumu_Data_Info[]=
{
  //失压数据
  {.Event_ID=ID_EVENT_TOTAL_LOSS_VOL, .Occur_Stora_DI=OCCUR_TOTAL_LOSS_VOL, .Total_Stora_DI=TOTAL_TOTAL_LOSS_VOL, .pDI_Array=&Event_DI_Array[0]},  
  {.Event_ID=ID_EVENT_A_LOSS_VOLT,     .Occur_Stora_DI=OCCUR_A_LOSS_VOL,     .Total_Stora_DI=TOTAL_A_LOSS_VOL,     .pDI_Array=&Event_DI_Array[0]},
  {.Event_ID=ID_EVENT_B_LOSS_VOLT,     .Occur_Stora_DI=OCCUR_B_LOSS_VOL,     .Total_Stora_DI=TOTAL_B_LOSS_VOL,     .pDI_Array=&Event_DI_Array[0]},
  {.Event_ID=ID_EVENT_C_LOSS_VOLT,     .Occur_Stora_DI=OCCUR_C_LOSS_VOL,     .Total_Stora_DI=TOTAL_C_LOSS_VOL,     .pDI_Array=&Event_DI_Array[0]},
  {.Event_ID=ID_EVENT_AB_LOSS_VOLT,    .Occur_Stora_DI=OCCUR_AB_LOSS_VOL,     .Total_Stora_DI=TOTAL_AB_LOSS_VOL,    .pDI_Array=&Event_DI_Array[0]},
  {.Event_ID=ID_EVENT_AC_LOSS_VOLT,    .Occur_Stora_DI=OCCUR_AC_LOSS_VOL,     .Total_Stora_DI=TOTAL_AC_LOSS_VOL,    .pDI_Array=&Event_DI_Array[0]}, 
  {.Event_ID=ID_EVENT_BC_LOSS_VOLT,    .Occur_Stora_DI=OCCUR_BC_LOSS_VOL,     .Total_Stora_DI=TOTAL_BC_LOSS_VOL,    .pDI_Array=&Event_DI_Array[0]},
  {.Event_ID=ID_EVENT_ABC_LOSS_VOLT,   .Occur_Stora_DI=OCCUR_ABC_LOSS_VOL,     .Total_Stora_DI=TOTAL_ABC_LOSS_VOL,   .pDI_Array=&Event_DI_Array[0]},
  {.Event_ID=ID_EVENT_ALL_LOSS_VOLT, .Occur_Stora_DI=OCCUR_ALL_LOSS_VOL,  .Total_Stora_DI=TOTAL_ALL_LOSS_VOL, .pDI_Array=&Event_DI_Array[3]},
  //失流
  {.Event_ID=ID_EVENT_TOTAL_LOSS_CUR, .Occur_Stora_DI=OCCUR_TOTAL_LOSS_CUR, .Total_Stora_DI=TOTAL_TOTAL_LOSS_CUR, .pDI_Array=&Event_DI_Array[1]},  
  {.Event_ID=ID_EVENT_A_LOSS_CURR,     .Occur_Stora_DI=OCCUR_A_LOSS_CUR,     .Total_Stora_DI=TOTAL_A_LOSS_CUR,     .pDI_Array=&Event_DI_Array[1]},
  {.Event_ID=ID_EVENT_B_LOSS_CURR,     .Occur_Stora_DI=OCCUR_B_LOSS_CUR,     .Total_Stora_DI=TOTAL_B_LOSS_CUR,     .pDI_Array=&Event_DI_Array[1]},
  {.Event_ID=ID_EVENT_C_LOSS_CURR,     .Occur_Stora_DI=OCCUR_C_LOSS_CUR,     .Total_Stora_DI=TOTAL_C_LOSS_CUR,     .pDI_Array=&Event_DI_Array[1]},
  {.Event_ID=ID_EVENT_AB_LOSS_CURR,    .Occur_Stora_DI=OCCUR_AB_LOSS_CUR,     .Total_Stora_DI=TOTAL_AB_LOSS_CUR,    .pDI_Array=&Event_DI_Array[1]},
  {.Event_ID=ID_EVENT_AC_LOSS_CURR,    .Occur_Stora_DI=OCCUR_AC_LOSS_CUR,     .Total_Stora_DI=TOTAL_AC_LOSS_CUR,    .pDI_Array=&Event_DI_Array[1]}, 
  {.Event_ID=ID_EVENT_BC_LOSS_CURR,    .Occur_Stora_DI=OCCUR_BC_LOSS_CUR,     .Total_Stora_DI=TOTAL_BC_LOSS_CUR,    .pDI_Array=&Event_DI_Array[1]},
  {.Event_ID=ID_EVENT_ALL_LOSS_CURR,   .Occur_Stora_DI=OCCUR_ABC_LOSS_CUR,     .Total_Stora_DI=TOTAL_ABC_LOSS_CUR,   .pDI_Array=&Event_DI_Array[3]},
  //反向
  {.Event_ID=ID_EVENT_TOTAL_NEG_CUR, .Occur_Stora_DI=OCCUR_TOTAL_NEG_CUR, .Total_Stora_DI=TOTAL_TOTAL_NEG_CUR, .pDI_Array=&Event_DI_Array[2]},  
  {.Event_ID=ID_EVENT_A_NEG_CURR,     .Occur_Stora_DI=OCCUR_A_NEG_CUR,     .Total_Stora_DI=TOTAL_A_NEG_CUR,     .pDI_Array=&Event_DI_Array[2]},
  {.Event_ID=ID_EVENT_B_NEG_CURR,     .Occur_Stora_DI=OCCUR_B_NEG_CUR,     .Total_Stora_DI=TOTAL_B_NEG_CUR,     .pDI_Array=&Event_DI_Array[2]},
  {.Event_ID=ID_EVENT_C_NEG_CURR,     .Occur_Stora_DI=OCCUR_C_NEG_CUR,     .Total_Stora_DI=TOTAL_C_NEG_CUR,     .pDI_Array=&Event_DI_Array[2]},
  //断相
  {.Event_ID=ID_EVENT_TOTAL_BREAK_VOL, .Occur_Stora_DI=OCCUR_TOTAL_BREAK_VOL, .Total_Stora_DI=TOTAL_TOTAL_BREAK_VOL, .pDI_Array=&Event_DI_Array[3]},  
  {.Event_ID=ID_EVENT_A_LOSS_PARSE,     .Occur_Stora_DI=OCCUR_A_BREAK_VOL,     .Total_Stora_DI=TOTAL_A_BREAK_VOL,     .pDI_Array=&Event_DI_Array[3]},
  {.Event_ID=ID_EVENT_B_LOSS_PARSE,     .Occur_Stora_DI=OCCUR_B_BREAK_VOL,     .Total_Stora_DI=TOTAL_B_BREAK_VOL,     .pDI_Array=&Event_DI_Array[3]},
  {.Event_ID=ID_EVENT_C_LOSS_PARSE,     .Occur_Stora_DI=OCCUR_C_BREAK_VOL,     .Total_Stora_DI=TOTAL_C_BREAK_VOL,     .pDI_Array=&Event_DI_Array[3]},

  //全失压 
}; 

INT8U Check_Cumu0_Event_Occuring(INT8U Event_ID)
{
  INT8U i;
  
  for(i = 0; i < S_NUM(Single_Total_Event_Map); i ++)
  {
    if(Single_Total_Event_Map[i][0] == Event_ID ||\
       Single_Total_Event_Map[i][1] == Event_ID)
    {
      if(Read_Event_Status(Single_Total_Event_Map[i][0]))
         return 1;
    }
  }
  return 0;
}

INT8U Get_Total_Event_ID(INT8U Event_ID)
{
  INT8U i;
  
  for(i = 0; i < S_NUM(Single_Total_Event_Map); i ++)
  {
    if(Single_Total_Event_Map[i][0] == Event_ID ||\
       Single_Total_Event_Map[i][1] == Event_ID)
    {
       return Single_Total_Event_Map[i][1];
    }
  }
  return NULL_EVENT_ID;  
  
}

//获取某个事件在Event_Cumu_Data_Info中的索引 
//Event_ID表示时间的ID
INT16U Get_Event_Cumu_Data_Index(INT8U Event_ID)
{
  INT16U i;
  
  for(i = 0; i<S_NUM(Event_Cumu_Data_Info); i++)
  {
    if(Event_Cumu_Data_Info[i].Event_ID EQ Event_ID)
      return i;
  }
  
  return NULL_2BYTES;
}

//获取某个累计事件的累计数据在Event_Cumu_Data_Info中的索引
//DI表示协议中的索引值
INT16U Get_Event_Cumu_Stora_Index(STORA_DI DI)
{
  INT16U i;
  
  for(i = 0; i<S_NUM(Event_Cumu_Data_Info); i++)
  {
    if(Event_Cumu_Data_Info[i].Total_Stora_DI EQ DI)
      return i;
  }
  
  return NULL_2BYTES;
}

//获取协议数据，专给事件提供，电量等有3位小数，防止误差产生.
//DI标识码
//DI_Num标识个数
//pDst目标缓冲区
//pDst_Start和DstLen一起限定目标缓冲区
INT16U Get_Multi_Proto_Data(PROTO_DI DI[],INT8U DI_Num,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag)
{
  INT8U Err;
  INT8U i,Next_Flag;
  INT16U Len,TempLen;
  
  if(ASSERT(A_WARNING,pDst >= pDst_Start && pDst < pDst_Start + DstLen))
    return 0;
  
  Len = 0;  
  for(i = 0; i < DI_Num; i ++)
  {
    TempLen =_Get_DLT645_Data(DI[i], (void *)0, 0, pDst + Len, pDst_Start, DstLen, Data_Flag, &Next_Flag, &Err);
    if(TempLen EQ 0)
      ASSERT(A_WARNING,0);
    Len += TempLen;
  }
  
  return Len;
}

//void Convert_S_Proto_Data(INT32U Data, INT8U Div, INT8U *pDst, INT8U *pDst_Start, INT8U DstLen)
//void Convert_U_Proto_Data(INT32S Data, INT8U Div, INT8U *pDst, INT8U *pDst_Start, INT8U DstLen)
//协议数据的加减
//DI数据对应的标识码数组
//DI_Num标识码个数
//pData0,通过Get_DLT645_Data 获得的数据，FOR_SRC标志
//pData1,,通过Get_DLT645_Data 获得的数据，FOR_SRC标志
//BCD数据,要计算的就是pData0-pData1
//pDst,目标缓冲区,pDst_Start和DstLen一起限定返回长度
INT16U Proto_Data_Add_Dec(INT8U Data_Flag,INT8U Add_Or_Dec,PROTO_DI DI[], INT8U DI_Num, INT8U *pData0, INT8U *pData1,INT8U *pDst, INT8U *pDst_Start, INT16U DstLen)
{
  INT8U i;
  INT16U Index,Len;
  INT32U U32_Data0,U32_Data1;
  INT32S S32_Data0,S32_Data1;
  INT16U S16_Data0,S16_Data1;
  
  Len = 0;
  for(i = 0; i < DI_Num; i ++)
  {
    Index = Get_Protocol_Data_Index(DI[i]);//通过Index0获取源数据格式
    
    if(Index EQ NULL_2BYTES)
    {
      ASSERT(A_WARNING,0);
      return 0;
    }
    
    if(Protocol_Data[Index].Src_Len != Protocol_Data[Index].Dst_Len)
      ASSERT(A_WARNING,0);
    
    if(Protocol_Data[Index].Src_Format EQ S_INTU)
    {
      U32_Data0 = 0;
      U32_Data1 = 0;
      
      mem_cpy(&U32_Data0, pData0 + Len, Protocol_Data[Index].Src_Len, &U32_Data0, sizeof(U32_Data0));
      mem_cpy(&U32_Data1, pData1 + Len, Protocol_Data[Index].Src_Len, &U32_Data1, sizeof(U32_Data1));

      DEBUG_PRINT(HUCK,DEBUG_0,"Proto_Data_Add_Dec:DI=0x%x,Data0=%lu,Data1=%lu,Len=%d",DI[i],U32_Data0,U32_Data1,Protocol_Data[Index].Src_Len);
        
      if(Add_Or_Dec EQ DATA_ADD)
        U32_Data0 += U32_Data1;
      else
        U32_Data0 = (U32_Data0 > U32_Data1)? (U32_Data0 - U32_Data1):0;
        
        if(Data_Flag EQ FOR_SRC)
          mem_cpy(pDst + Len, &U32_Data0, Protocol_Data[Index].Dst_Len, pDst_Start, DstLen);
        else
        {
          U32_Data0 = U32_Data0/Pow_10x(Protocol_Data[Index].Dst_Start);//计算差
          Hex2Bcd(U32_Data0, pDst + Len, Protocol_Data[Index].Dst_Len, pDst_Start, DstLen);
        }          
        
        Len += Protocol_Data[Index].Src_Len;
    }
    else if(Protocol_Data[Index].Src_Format EQ S_INTS)
    {
      if(Protocol_Data[Index].Src_Len EQ sizeof(INT16S))
      {
        S16_Data0 = 0;
        S16_Data1 = 0;
        
        mem_cpy(&S16_Data0, pData0 + Len, Protocol_Data[Index].Src_Len, &S16_Data0, sizeof(S16_Data0));
        mem_cpy(&S16_Data1, pData1 + Len, Protocol_Data[Index].Src_Len, &S16_Data1, sizeof(S16_Data1));
 
        //DEBUG_PRINT(HUCK,DEBUG_0,"Proto_Data_Add_Dec:DI=0x%x,S16_Data0=%ld,S16_Data1=%ld,Len=%d",DI[i],S16_Data0,S16_Data1,Protocol_Data[Index].Src_Len);
        
        if(Add_Or_Dec  EQ  DATA_ADD)
          S16_Data0 += S16_Data1;
        else
          S16_Data0 = S16_Data0 - S16_Data1;
           
        if(Data_Flag EQ FOR_SRC)
          mem_cpy(pDst + Len, &S16_Data0, Protocol_Data[Index].Dst_Len, pDst_Start, DstLen);
        else 
        {
          S16_Data0 = S16_Data0/(INT16S)(Pow_10x(Protocol_Data[Index].Dst_Start));//计算差          
          S_Hex2Bcd(S16_Data0, pDst + Len, Protocol_Data[Index].Dst_Len, pDst_Start, DstLen);
        }
        
        Len += Protocol_Data[Index].Src_Len;
      }
      else if(Protocol_Data[Index].Src_Len EQ sizeof(INT32S))
      {
        S32_Data0 = 0;
        S32_Data1 = 0;
        
        mem_cpy(&S32_Data0, pData0 + Len, Protocol_Data[Index].Src_Len, &S32_Data0, sizeof(S32_Data0));
        mem_cpy(&S32_Data1, pData1 + Len, Protocol_Data[Index].Src_Len, &S32_Data1, sizeof(S32_Data1));
        
        //DEBUG_PRINT(HUCK,DEBUG_0,"Proto_Data_Add_Dec:DI=0x%x,S32_Data0=%ld,S32_Data1=%ld,Len=%d",DI[i],S32_Data0,S32_Data1,Protocol_Data[Index].Src_Len);
        
        if(Add_Or_Dec  EQ  DATA_ADD)
          S32_Data0 += S32_Data1;
        else
          S32_Data0 = S32_Data0 - S32_Data1;
 
        if(Data_Flag EQ FOR_SRC)
          mem_cpy(pDst + Len, &S32_Data0, Protocol_Data[Index].Dst_Len, pDst_Start, DstLen);
        else  
        {
          //DEBUG_PRINT(HUCK,DEBUG_0,"S32_Data0=%ld",S32_Data0);
          S32_Data0 = S32_Data0/(INT32S)(Pow_10x(Protocol_Data[Index].Dst_Start));//计算差
          //DEBUG_PRINT(HUCK,DEBUG_0,"S32_Data0/10=%ld",S32_Data0);
          S_Hex2Bcd(S32_Data0, pDst + Len, Protocol_Data[Index].Dst_Len, pDst_Start, DstLen);
          DEBUG_PRINT(HUCK,DEBUG_0,"Buf=%x %x %x %x",pDst[Len], pDst[Len+1], pDst[Len+2], pDst[Len+3]);
        }
        
        Len += Protocol_Data[Index].Src_Len;
      }
      else
        ASSERT(A_WARNING,0);
    }
    else
      ASSERT(A_WARNING,0);
    
  }
  return Len;
}

INT8U Check_Total_Event_ID(INT8U Event_ID)
{
 INT8U i;

  for(i = 0; i < S_NUM(Single_Total_Event_Map); i ++)
  {
    if(Single_Total_Event_Map[i][1] EQ Event_ID)
    {
       return 1;
    }
  }
  return 0; 
}
//获取某个总事件对应的单独事件正在发生的个数
INT8U Get_Event_Total_Occuring_Singles(INT8U Event_ID)
{
  INT8U i;
  INT8U Counts;
  
  Counts = 0;
  for(i = 0; i < S_NUM(Single_Total_Event_Map); i ++)
  {
    if(Single_Total_Event_Map[i][1] EQ Event_ID)//总事件
    {
      if(Read_Event_Status(Single_Total_Event_Map[i][0]))
        Counts ++;
    }
  }  
  return Counts;
}

//检查当此是否需要处理总的事件的发生或者结束
INT8U Check_Event_Total_Proc(INT8U Event_ID, INT8U Occur_Or_End)
{
  INT8U Counts;
  
  Counts = Get_Event_Total_Occuring_Singles(Event_ID);
  
  if(Occur_Or_End EQ EVENT_OCCUR)//总事件对应的独立事件还没有发生过，需要处理!
    return (Counts EQ 0)?1:0;
  else
    return (Counts EQ 1)?1:0;//总时间对应的独立事件只有当前在处理的这个了，需要处理总事件
  
}

//事件发生时的累计处理
//Index为事件在Event_Cumu_Data_Info数组中的索引
INT16U Event_Cumu_Start_Proc(INT16U Index, INT8U Flag)
{
  INT32U Counts,Mins;
  INT16U Len;
  INT8U Err;
  
  if(Event_Cumu_Data_Info[Index].Occur_Stora_DI EQ NULL_STORA_DI)
    return 0;
  
  Len = Read_Storage_Data(Event_Cumu_Data_Info[Index].Occur_Stora_DI, (INT8U *)Pub_Buf0, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0), &Err);
  if(Len > 0)//没有读到累计次数，则从发生时存储区获得累加次数
  {
    Counts = 0;
    mem_cpy(&Counts, (INT8U *)Pub_Buf0 + 5, 4, &Counts, sizeof(Counts));          
    Mins = 0;
    mem_cpy(&Mins, (INT8U *)Pub_Buf0 + 9, 4, &Mins, sizeof(Mins));
  }
  else
  {    
    Len = Read_Storage_Data(Event_Cumu_Data_Info[Index].Total_Stora_DI, (INT8U *)Pub_Buf0, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0), &Err);
    if(Len > 0)
    {
      Counts = 0;
      mem_cpy(&Counts, (INT8U *)Pub_Buf0 + 5, 4, &Counts, sizeof(Counts));          
      Mins = 0;
      mem_cpy(&Mins, (INT8U *)Pub_Buf0 + 9, 4, &Mins, sizeof(Mins));
    }
    else
    {
      ASSERT(A_WARNING, 0);
      Mins = 0;//实在是没有办法读到正确的次数数据了,则置0
      Counts = 0;
      mem_set((INT8U *)Pub_Buf0, 0, sizeof(Pub_Buf0), (INT8U *)Pub_Buf0, sizeof(Pub_Buf0));      
    } 
  }
  
  Counts ++;
 
  //当前是总累加事件，且已经有独立事件发生了的，只对发生次数进行增加处理
  if(Check_Total_Event_ID(Event_Cumu_Data_Info[Index].Event_ID) &&\
    Get_Event_Total_Occuring_Singles(Event_Cumu_Data_Info[Index].Event_ID) != 0 &&\
    EVENT_REAL EQ Flag)//真实事件才存储
  {
    DEBUG_PRINT(HUCK, DEBUG_0, "Total_Event Occuring...");
    mem_cpy((INT8U *)Pub_Buf0 + 5, &Counts, 4, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0));//4字节累计次数
    Len = Get_Storage_Data_Len(Event_Cumu_Data_Info[Index].Occur_Stora_DI);    
    Write_Storage_Data(Event_Cumu_Data_Info[Index].Occur_Stora_DI, (INT8U *)Pub_Buf0, Len);//将发生时的数据存入ROM)
    return Len;
  } 
 
  DEBUG_PRINT(HUCK,DEBUG_0,"Event Occur Index=%d,Counts=%lu,Mins=%lu,Start_Min=%lu",Index,Counts,Mins,Event_Cumu_Data.Min_Timer);
  
  Pub_Buf0[0] = EVENT_OCCUR;//1字节头，表示事件进入发生态
  mem_cpy((INT8U *)Pub_Buf0 + 1, (INT8U *)&Event_Cumu_Data.Min_Timer, 4, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0));//4字节当前分计数
  mem_cpy((INT8U *)Pub_Buf0 + 5, &Counts, 4, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0));//4字节累计次数
  mem_cpy((INT8U *)Pub_Buf0 + 9, &Mins, 4, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0));//4字节累计分钟
  Len = 13;
  //除了次数和时间还需要记录其他的累加量，则需要在事件发生时存储瞬时量
  Len += Get_Multi_Proto_Data((PROTO_DI *)Event_Cumu_Data_Info[Index].pDI_Array -> DI, Event_Cumu_Data_Info[Index].pDI_Array -> DI_Num, (INT8U *)Pub_Buf0 + Len, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0),FOR_SRC|FOR_EVENT);		
  
  if(EVENT_REAL EQ Flag)//真实事件才存储
    Write_Storage_Data(Event_Cumu_Data_Info[Index].Occur_Stora_DI, (INT8U *)Pub_Buf0, Len);//将发生时的数据存入ROM
  return Len;
}

//事件结束时的累计处理
//Flag:0表示真实的处理一次事件结束，1表示虚拟一次结束，但是不存储到EEROM中，而是将累计数据存储到Pub_Buf0开始的缓冲区中
INT16U Event_Cumu_End_Proc(INT16U Index,INT8U Flag)
{
  INT8U *p, Err;
  INT16U Len,Len1;
  INT32U Counts,Mins;
  INT32U Timer;
  
  //OS_Sem_Pend(PUB_BUF0_SEM_ID);
  //读取发生时存储的瞬时量
  if(Event_Cumu_Data_Info[Index].Occur_Stora_DI EQ NULL_STORA_DI)
    return 0;

  //当前是总累加事件，且该累加还有某些独立事件在发生，则不进行处理
  if(Check_Total_Event_ID(Event_Cumu_Data_Info[Index].Event_ID) &&\
    Get_Event_Total_Occuring_Singles(Event_Cumu_Data_Info[Index].Event_ID) > 1 &&\
    EVENT_REAL EQ Flag)//真实事件才存储
  {
    DEBUG_PRINT(HUCK, DEBUG_0, "Total_Event Occuring...");
    return 0;
  } 
  
  //先读取最近一次发生时的记录  
  Len = Read_Storage_Data(Event_Cumu_Data_Info[Index].Occur_Stora_DI, (INT8U *)Pub_Buf0, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0), &Err);
  if(Len EQ 0 || Pub_Buf0[0] != EVENT_OCCUR)//没有事件发生记录
  {
    ASSERT(A_WARNING,0);
    if(Len EQ 0)
      DEBUG_PRINT(HUCK, DEBUG_0, "Read Event_Cumu_Data_Info error Len=0, Index=%d", Index);
    else
      DEBUG_PRINT(HUCK, DEBUG_0, "Read Event_Cumu_Data_Info Occcur_Flag=0x%x error ", Pub_Buf0[0]);
    
    return 0;
  }
  else//将档次累计数据存入总累计
  {
    Pub_Buf0[0] = EVENT_END;//置事件结束标志
    
    //总的次数
    Counts = 0;
    mem_cpy(&Counts, (INT8U *)Pub_Buf0 + 5, 4, &Counts, sizeof(Counts));
    
    //总的累计分钟
    Mins = 0;
    mem_cpy(&Mins, (INT8U *)Pub_Buf0 + 9, 4, &Mins, sizeof(Mins));
    //读取发生时的分钟计数,当前分钟计数 - 发生时的分钟计数 = 当次发生的分钟数
    Timer = 0;
    mem_cpy(&Timer, (INT8U *)Pub_Buf0 + 1, 4, &Timer, sizeof(Timer));//发生时的分钟计数
    
    if(Event_Cumu_Data.Min_Timer >= Timer && Timer != 0)
    {
      Mins += (Event_Cumu_Data.Min_Timer > Timer) ? (Event_Cumu_Data.Min_Timer-Timer):1;
      if(Mins >= 999999)
        Mins = 0;
    }
    else
    {
      Mins = 0;
    }
    mem_cpy((INT8U *)Pub_Buf0 + 9, &Mins, 4, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0));
    
    DEBUG_PRINT(HUCK,DEBUG_0,"Event End Index=%d,Counts=%lu,Mins=%lu,Start_Min=%lu,End_Min=%lu",Index,Counts,Mins,Timer,Event_Cumu_Data.Min_Timer);
    
    Len = 13;
    
    p = (INT8U *)Pub_Buf0 + (sizeof(Pub_Buf0)/2);
    
    //读取当前时刻的电量等数据, 放入p开始的缓冲区
    Get_Multi_Proto_Data((PROTO_DI *)Event_Cumu_Data_Info[Index].pDI_Array ->DI, Event_Cumu_Data_Info[Index].pDI_Array ->DI_Num, p, \
      (INT8U *)Pub_Buf0, sizeof(Pub_Buf0), FOR_SRC);
    
    //将当前电量与事件发生时的电量相减,并将结果放入Pub_Buf0+13开始的目标缓冲区中
    Proto_Data_Add_Dec(FOR_SRC, DATA_DEC,(PROTO_DI *)Event_Cumu_Data_Info[Index].pDI_Array ->DI, Event_Cumu_Data_Info[Index].pDI_Array -> DI_Num, p, (INT8U *)Pub_Buf0 + 13, \
      (INT8U *)Pub_Buf0 + 13, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0));
    
    //至此所有的差值都计算完毕, 并放在了 Pub_Buf0+13 开始的缓冲区中
    
    //读出总的累计量到p 开始的buf,然后将当此的差量计入累加量
    Len1 = Read_Storage_Data(Event_Cumu_Data_Info[Index].Total_Stora_DI, p, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0), &Err);
    if(ASSERT(A_WARNING,Len1 > 0))
    {
      Len1 = Get_Storage_Data_Len(Event_Cumu_Data_Info[Index].Total_Stora_DI);
      mem_set(p, 0, Len1, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0));
    }
    
    //将当次事件累加量计入总的累加量,放到Pub_Buf+13 开始的buf
    Len += Proto_Data_Add_Dec(FOR_SRC, DATA_ADD, (PROTO_DI *)Event_Cumu_Data_Info[Index].pDI_Array ->DI, Event_Cumu_Data_Info[Index].pDI_Array ->DI_Num, p + 13, (INT8U *)Pub_Buf0 + 13, \
      (INT8U *)Pub_Buf0 + Len, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0));
    
    if(Flag EQ EVENT_REAL)//虚拟事件结束时不存储事件
    {
      Write_Storage_Data(Event_Cumu_Data_Info[Index].Occur_Stora_DI, (INT8U *)Pub_Buf0, Len);//更新发生时的数据，发生时数据也应该存储累计次数和时间
      Write_Storage_Data(Event_Cumu_Data_Info[Index].Total_Stora_DI, (INT8U *)Pub_Buf0, Len);
    } 
  }
  return Len;
  
}

//事件累加处理
//Event_ID表示事件ID
//Start_End_Flag表示事件发生或者结束标志,0表示发生,1表示结束
INT16U Event_Cumu_Data_Proc(INT8U Event_ID,INT8U Start_End_Flag,INT8U Flag)
{
  INT8U Return_Flag;
  INT16U Index;
  INT16U Len;
  
  //事件在Event_Cumu_Info结构体中的索引
  DEBUG_PRINT(HUCK,DEBUG_0,"Event_Cumu_Data_Proc ID:%d",Event_ID);
  
  Len = Extend_Event_Cumu_Proc(Event_ID, Start_End_Flag, Flag, &Return_Flag);
  if(Return_Flag EQ 1)
    return Len; 
  
  Index=Get_Event_Cumu_Data_Index(Event_ID);
  if(Index EQ NULL_2BYTES)
    return 0;
  
  if(Start_End_Flag EQ EVENT_OCCUR)//事件发生时,保存好事件发生时的某些瞬时量，待事件结束时，计算其累加量
    return Event_Cumu_Start_Proc(Index,Flag);
  else//事件结束时
    return Event_Cumu_End_Proc(Index,Flag);
}

//转换源数据到协议数据
INT16U Convert_Src_2_Proto_Data(PROTO_DI DI[], INT8U DI_Num, INT8U *pSrc, INT8U *pDst, INT8U *pDst_Start, INT16U DstLen)
{
  INT8U i;
  INT16U Src_Len,Dst_Len;
  INT16U Index;
  INT32U U32_Data;
  INT32S S32_Data;
  INT16U S16_Data;
  
  Src_Len = 0;
  Dst_Len = 0;
  for(i = 0; i < DI_Num; i ++)
  {
    Index = Get_Protocol_Data_Index(DI[i]);//通过Index0获取源数据格式
    
    if(Index EQ NULL_2BYTES)
    {
      ASSERT(A_WARNING,0);
      return 0;
    }
    
    if(Protocol_Data[Index].Src_Format EQ S_INTU)
    {
      U32_Data = 0;
      mem_cpy(&U32_Data, pSrc + Src_Len, Protocol_Data[Index].Src_Len, &U32_Data, sizeof(U32_Data));
      
      if(Protocol_Data[Index].Dst_Start > 0)
        U32_Data = U32_Data / Pow_10x(Protocol_Data[Index].Dst_Start);				
      
      Hex2Bcd(U32_Data, pDst + Dst_Len, Protocol_Data[Index].Dst_Len, pDst_Start, DstLen);
      Src_Len += Protocol_Data[Index].Src_Len;
      Dst_Len += Protocol_Data[Index].Dst_Len;
    }
    else if(Protocol_Data[Index].Src_Format EQ S_INTS)
    {
      if(Protocol_Data[Index].Src_Len EQ sizeof(INT16S))
      {
        S16_Data = 0;
        mem_cpy(&S16_Data, pSrc + Src_Len, Protocol_Data[Index].Src_Len, &S16_Data, sizeof(S16_Data));
        
        if(Protocol_Data[Index].Dst_Start > 0)
          S16_Data = S16_Data / Pow_10x(Protocol_Data[Index].Dst_Start);
        
        S_Hex2Bcd(S16_Data, pDst + Dst_Len, Protocol_Data[Index].Dst_Len, pDst_Start, DstLen);
      }
      else if(Protocol_Data[Index].Src_Len EQ sizeof(INT32S))
      {
        S32_Data = 0;
        mem_cpy(&S32_Data, pSrc + Src_Len, Protocol_Data[Index].Src_Len, &S32_Data, sizeof(S32_Data));
        
        if(Protocol_Data[Index].Dst_Start > 0)
          S32_Data = S32_Data / Pow_10x(Protocol_Data[Index].Dst_Start);
        
        S_Hex2Bcd(S32_Data, pDst + Dst_Len, Protocol_Data[Index].Dst_Len, pDst_Start, DstLen);
      }
      else
        ASSERT(A_WARNING,0);
      
      Src_Len += Protocol_Data[Index].Src_Len;
      Dst_Len += Protocol_Data[Index].Dst_Len;
    }
    else if(Protocol_Data[Index].Src_Format EQ S_BCD)
    {
      if(Protocol_Data[Index].Src_Len != Protocol_Data[Index].Dst_Len)
        ASSERT(A_WARNING,0);
      
      mem_cpy(pDst + Dst_Len, pSrc + Src_Len, Protocol_Data[Index].Dst_Len, pDst_Start, DstLen);
      
      Src_Len += Protocol_Data[Index].Src_Len;
      Dst_Len += Protocol_Data[Index].Dst_Len;
    }
    else
      ASSERT(A_WARNING,0);
  }
  return Dst_Len;
  
}
//获取事件累计的协议数据
//pDst目标缓冲区
//pDst_Start和DstLen一起限定目标的范围
INT16U Get_Event_Cumu_Proto_Data(STORA_DI DI, INT8U *pDst, INT8U *pDst_Start, INT16U DstLen)
{
  INT8U Err;
  INT16U Index,Len;
  INT32U Counts,Mins;
  
  Index = Get_Event_Cumu_Stora_Index(DI);
  if(ASSERT(A_WARNING,Index  !=  NULL_2BYTES))
    return 0;

  OS_Sem_Pend(PUB_BUF0_SEM_ID);
  
  Len = 0;
  if(Check_Cumu0_Event_Occuring(Event_Cumu_Data_Info[Index].Event_ID))//当前事件在发生?
    Len = Event_Cumu_Data_Proc(Event_Cumu_Data_Info[Index].Event_ID, EVENT_END, EVENT_VIRTRUAL);//虚拟一次事件结束,将当前累加数据存储到了Pub_Buf0中	
  else 
    Len = Read_Storage_Data(Event_Cumu_Data_Info[Index].Total_Stora_DI, (INT8U *)Pub_Buf0, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0)/2, &Err);

  if(Len > 0)
  {
  
    Len = 0;
    
    Counts = 0;
    Mins = 0;
    mem_cpy(&Counts, (INT8U *)Pub_Buf0 + 5, 4, &Counts, sizeof(Counts));
    mem_cpy(&Mins, (INT8U *)Pub_Buf0 + 9, 4, &Mins, sizeof(Mins));
   
    Hex2Bcd(Mins, pDst, 4, pDst_Start, DstLen);  //累计时间  
    Hex2Bcd(Counts, pDst + 4, 4, pDst_Start, DstLen); //累计次数
   
    Len += 8;
    
    Len += Convert_Src_2_Proto_Data((PROTO_DI *)Event_Cumu_Data_Info[Index].pDI_Array->DI, Event_Cumu_Data_Info[Index].pDI_Array->DI_Num, \
      (INT8U *)Pub_Buf0 + 13, pDst + Len, pDst_Start, DstLen);
    
  }
  OS_Sem_Post(PUB_BUF0_SEM_ID);
  
  return Len;
  
}

//读取累计事件
INT16U Get_Cumu0_Event_Data(PROTO_DI DI, INT8U *pDst, INT8U *pDst_Start, INT16U DstLen,INT8U *pErr)
{
  INT16U Len, TempLen;
  INT8U i;
  
  *pErr=NO_ERR;
  
  Len = 0;
  if(DI >= 0xB710 && DI <= 0xB716)//失压
     Len = Get_Event_Cumu_Proto_Data(TOTAL_A_LOSS_VOL + (DI - 0xB710), pDst, pDst_Start, DstLen);
  else if(DI >= 0xB720 && DI <= 0xB725)//失流
     Len = Get_Event_Cumu_Proto_Data(TOTAL_A_LOSS_CUR + (DI - 0xB720), pDst, pDst_Start, DstLen);
  else if(DI >= 0xB730 && DI <= 0xB733)//反向
     Len = Get_Event_Cumu_Proto_Data(TOTAL_TOTAL_NEG_CUR + (DI - 0xB730), pDst, pDst_Start, DstLen);
  else if(DI >= 0xB740 && DI <= 0xB742)//断相
     Len = Get_Event_Cumu_Proto_Data(TOTAL_A_BREAK_VOL + (DI - 0xB740), pDst, pDst_Start, DstLen);
 
  if(Len >= 8)
  {
     mem_cpy(pDst + 6, (INT8U *)pDst + 8, Len - 8, pDst_Start, DstLen);//电能等增量
     return Len - 2;
  }
  
  //-----------------------------------------------------------
  //全失压、全失流累计时间和次数
  Len = 0; 
  if(DI EQ 0xB743)//全失压
    Len = Get_Event_Cumu_Proto_Data(TOTAL_ALL_LOSS_VOL, pDst, pDst_Start, DstLen);
  else if(DI EQ 0xB744)//全失流
    Len = Get_Event_Cumu_Proto_Data(TOTAL_ABC_LOSS_CUR, pDst, pDst_Start, DstLen);    
  if(Len >= 8)
    return 6;


 //--------------------------------------------------------------------------------- 
 //读取总失压累计次数
  Len = 0;
  if(DI >= 0xB310 && DI <= 0xB313)
     Len = Get_Event_Cumu_Proto_Data(TOTAL_TOTAL_LOSS_VOL + (DI - 0xB310), pDst, pDst_Start, DstLen);   
  
  if(Len >= 8)
  {
    mem_cpy(pDst, pDst + 4, 2, pDst_Start, DstLen);//后四字节是累计次数
    return 2;
  }
  //--------------------------------------------------------------------------------
  //读取失压次数数据块
  Len = 0;
  
  if(DI EQ 0xB31F)
  {
    for(i = 0; i < 4; i ++)
    {
     TempLen = Get_Event_Cumu_Proto_Data(TOTAL_TOTAL_LOSS_VOL + i, pDst + Len, pDst_Start, DstLen); 
    
      if(TempLen <= 8)
        mem_set(pDst + Len, 0, 2, pDst_Start, DstLen);
      else
        mem_cpy(pDst + Len, pDst + Len + 4, 2, pDst_Start, DstLen);
      
      Len += 2;     
    }

    return Len;
  }
  
  //-------------------------------------------------------------------------------
  //失压、失流累计时间
  Len = 0;  
  if(DI >= 0xB320 && DI <= 0xB323)
     Len = Get_Event_Cumu_Proto_Data(TOTAL_TOTAL_LOSS_VOL + (DI - 0xB320), pDst, pDst_Start, DstLen);
  else if(DI >= 0xB328 && DI <= 0xB32B)//失流累计时间
     Len = Get_Event_Cumu_Proto_Data(TOTAL_TOTAL_LOSS_CUR + (DI - 0xB328), pDst, pDst_Start, DstLen);
    
  if(Len >= 8)
    return 3;
  //------------------------------------------------------------------------------
  //失压累计时间数据块
  Len = 0;
  if(DI EQ 0xB32F)
  {
    for(i = 0; i < 4; i ++)
    {
     TempLen = Get_Event_Cumu_Proto_Data(TOTAL_TOTAL_LOSS_VOL + i, pDst + Len, pDst_Start, DstLen); 
    
      if(TempLen <= 8)
        mem_set(pDst + Len, 0, 3, pDst_Start, DstLen);
      
      Len += 3;     
    }

    return Len;    
  }
  //----------------------------------------------------------
  //全失压累计时间
  Len = 0;
  if(DI == 0xB324)//全失压累计时间
     Len = Get_Event_Cumu_Proto_Data(TOTAL_ALL_LOSS_VOL, pDst, pDst_Start, DstLen);
    
  if(Len >= 8)
    return 3;
  
  *pErr=PROTOCOL_DI_ERR;
  return 0;

  //OS_Sem_Post(PUB_BUF0_SEM_ID);
}

//全失压次数和时间统计
//上电后调用该函数一次，Counts表示掉电期间失压总次数，Mins表示掉电期间失压总分钟数
void All_Loss_Vol_Counts_Time_Proc(INT32U Counts,INT32U Mins)
{
  INT8U Re,Err;
  INT8U Temp[20];
  INT32U Counts0,Mins0;
  INT16U Len;
  
  DEBUG_PRINT(HUCK,DEBUG_0,"All_Loss_Vol Counts:%ld, Mins:%ld", Counts, Mins);
  Re=Read_Storage_Data(TOTAL_ALL_LOSS_VOL,Temp,Temp,sizeof(Temp),&Err);
  if(0==Re)
  {
    ASSERT(A_WARNING,0);
    Counts0 = Counts;
    Mins0 = Mins;
  }
  else
  {
    mem_cpy(&Counts0, Temp + 5, 4, &Counts0, sizeof(Counts0));
    mem_cpy(&Mins0, Temp + 9, 4, &Mins0, sizeof(Mins0));
    
    Counts0 += Counts;//Bcd2Hex(Temp + 5, 4);
    Mins0 += Mins;//Bcd2Hex(Temp + 9, 4);
    
    mem_cpy(Temp + 5, &Counts0, 4, Temp, sizeof(Temp));
    mem_cpy(Temp + 9, &Mins0, 4, Temp, sizeof(Temp));
  }
  

  Re=Write_Storage_Data(TOTAL_ALL_LOSS_VOL, (void *)&Temp, ALL_LOSS_VOL_DATA_LEN);
  ASSERT(A_WARNING,NO_ERR==Re);
  
  //----------------------------------
  //对总的失压次数和时间进行统计
  if(Get_Total_Event_ID(ID_EVENT_ALL_LOSS_VOLT) EQ ID_EVENT_TOTAL_LOSS_VOL)//全失压的时间和次数是否进入总失压和次数?
  {
    OS_Sem_Pend(PUB_BUF0_SEM_ID);
    Len = Read_Storage_Data(TOTAL_TOTAL_LOSS_VOL, (void *)Pub_Buf0, (void *)Pub_Buf0, sizeof(Pub_Buf0), &Err);
    if(Len > 0)
    {
      mem_cpy(&Counts0, (INT8U *)Pub_Buf0 + 5, 4, &Counts0, sizeof(Counts0));
      mem_cpy(&Mins0, (INT8U *)Pub_Buf0 + 9, 4, &Mins0, sizeof(Mins0));
      
      Counts0 += Counts;
      Mins0 += Mins;

      DEBUG_PRINT(HUCK,DEBUG_0,"Total_Loss_Vol Counts:%ld, Mins:%ld", Counts0, Mins0);
    
      mem_cpy((INT8U *)Pub_Buf0 + 5, &Counts0, 4, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0));
      mem_cpy((INT8U *)Pub_Buf0 + 9, &Mins0, 4, (INT8U *)Pub_Buf0, sizeof(Pub_Buf0));
      
      Write_Storage_Data(TOTAL_TOTAL_LOSS_VOL, (INT8U *)Pub_Buf0, Len);
    }
    OS_Sem_Post(PUB_BUF0_SEM_ID);
  }
}

//事件分钟计数
void Event_Cumu_Mins_Proc()
{
  INT8U Re;
  static S_Int8U Min_Bak = {CHK_BYTE,0xFF,CHK_BYTE};
  static S_Int32U Sec_Bak = {CHK_BYTE,0xFF,CHK_BYTE}; 
    
  Re = 1;
  Re &= CHECK_STRUCT_VAR(Sec_Bak);
  Re &= CHECK_STRUCT_VAR(Min_Bak); 
  ASSERT(A_WARNING, 1 EQ Re);
  
  if(Sec_Bak.Var EQ Sec_Timer_Pub)
    return;
  
  Sec_Bak.Var = Sec_Timer_Pub;
  
  Re = CHECK_STRUCT_SUM(Event_Cumu_Data);
  ASSERT(A_WARNING,1==Re);

  mem_cpy((void *)&Event_Cumu_Data.Time, (void *)&Cur_Time1, sizeof(Cur_Time1), (void *)&Event_Cumu_Data.Time, sizeof(Event_Cumu_Data.Time));
  SET_STRUCT_SUM(Event_Cumu_Data);
  
  if(Min_Bak.Var != Cur_Time1.Min)
  {
     Min_Bak.Var = Cur_Time1.Min;
     Event_Cumu_Data.Min_Timer ++ ;
     SET_STRUCT_SUM(Event_Cumu_Data);
  }
}

void Save_Event_Cumu_PD_Data()
{
  PwrOff_Proc_PUCK();
  
  if(CHECK_STRUCT_SUM(Meter_Instant_Status))
    mem_cpy((void *)&Event_Cumu_Data.Meter_Instant_Status, (void *)&Meter_Instant_Status, sizeof(Meter_Instant_Status),\
            (void *)&Event_Cumu_Data.Meter_Instant_Status, sizeof(Event_Cumu_Data.Meter_Instant_Status));
  else
  {
    mem_set((void *)&Event_Cumu_Data.Meter_Instant_Status, 0, sizeof(Event_Cumu_Data.Meter_Instant_Status),\
            (void *)&Event_Cumu_Data.Meter_Instant_Status, sizeof(Event_Cumu_Data.Meter_Instant_Status));
    SET_STRUCT_SUM(Event_Cumu_Data.Meter_Instant_Status);
  }
  
  mem_cpy((void *)&Event_Cumu_Data.Time, (void *)&Cur_Time1, sizeof(Cur_Time1),\
          (void *)&Event_Cumu_Data.Time, sizeof(Event_Cumu_Data.Time));

  if(CHECK_STRUCT_SUM(PSW_Err_Info))
    mem_cpy((void *)&Event_Cumu_Data.PSW_Err_Info, (void *)&PSW_Err_Info, sizeof(PSW_Err_Info),\
            (void *)&Event_Cumu_Data.PSW_Err_Info, sizeof(Event_Cumu_Data.PSW_Err_Info));
  else
  {
    mem_set((void *)&Event_Cumu_Data.PSW_Err_Info, 0, sizeof(Event_Cumu_Data.PSW_Err_Info),\
            (void *)&Event_Cumu_Data.PSW_Err_Info, sizeof(Event_Cumu_Data.PSW_Err_Info));
    SET_STRUCT_SUM(Event_Cumu_Data.PSW_Err_Info);
  }

  SET_STRUCT_SUM(Event_Cumu_Data);

  Write_Storage_Data(CUMU_EVENT_PD,(void *)&Event_Cumu_Data,sizeof(Event_Cumu_Data));
}

//设置默认的累计事件数据
void Set_Def_Event_Cumu0_Data()
{
  INT8U i;
  INT16U Len;
  
  DISP_CLR_DATA_INFO;
  OS_Sem_Pend(PUB_BUF0_SEM_ID);
  mem_set((INT8U *)Pub_Buf0,0,sizeof(Pub_Buf0),(INT8U *)Pub_Buf0,sizeof(Pub_Buf0));
  for(i = 0; i < S_NUM(Event_Cumu_Data_Info); i ++)
  {
    Len = Get_Storage_Data_Len(Event_Cumu_Data_Info[i].Occur_Stora_DI);
    Write_Storage_Data(Event_Cumu_Data_Info[i].Occur_Stora_DI, (INT8U *)Pub_Buf0,Len);
    Len = Get_Storage_Data_Len(Event_Cumu_Data_Info[i].Total_Stora_DI);
    Write_Storage_Data(Event_Cumu_Data_Info[i].Total_Stora_DI, (INT8U *)Pub_Buf0,Len);    
  }
  
  Len = Get_Storage_Data_Len(TOTAL_ALL_LOSS_VOL);//全失压
  Write_Storage_Data(TOTAL_ALL_LOSS_VOL, (INT8U *)Pub_Buf0,Len); 
  OS_Sem_Post(PUB_BUF0_SEM_ID);
  
  Extend_Set_Def_Event_Cumu_Data();
}

//上电事件处理
void Power_On_Event_Proc()
{
  INT8U Err;
  
  DEBUG_PRINT(HUCK,DEBUG_0,"----------PowerOn Event Proc Start----------");
  
  if(Read_Storage_Data(CUMU_EVENT_PD,(void *)&Event_Cumu_Data,(void *)&Event_Cumu_Data,sizeof(Event_Cumu_Data),&Err)==sizeof(Event_Cumu_Data)\
    && NO_ERR==Err && CHECK_STRUCT_SUM(Event_Cumu_Data))
  {
    mem_cpy((void *)&Meter_Instant_Status,(void *)&Event_Cumu_Data.Meter_Instant_Status,sizeof(Event_Cumu_Data.Meter_Instant_Status),\
            (void *)&Meter_Instant_Status,sizeof(Meter_Instant_Status));
    Save_Event_Data();//保存上电前的事件记录
  }
  
  if(CHECK_STRUCT_SUM(Event_Cumu_Data.PSW_Err_Info))
  {
    mem_cpy((void *)&PSW_Err_Info, (void *)&Event_Cumu_Data.PSW_Err_Info, sizeof(Event_Cumu_Data.PSW_Err_Info),\
            (void *)&PSW_Err_Info, sizeof(PSW_Err_Info));
    PSW_Err_Info.Set_En_Mins = 0;
    SET_STRUCT_SUM(PSW_Err_Info);
  }
  else
  {
    ASSERT(A_WARNING,0);
    mem_set((void *)&PSW_Err_Info, 0, sizeof(PSW_Err_Info),(void *)&PSW_Err_Info, sizeof(PSW_Err_Info));
    SET_STRUCT_SUM(PSW_Err_Info);
  }       
  
  if(Check_BCD_Time((S_BCD_Time *)&Event_Cumu_Data.Time))//记录前次掉电时间,暂时未用
  {
    Last_PD_Time[0]=Event_Cumu_Data.Time.Min;
    Last_PD_Time[1]=Event_Cumu_Data.Time.Hour;
    Last_PD_Time[2]=Event_Cumu_Data.Time.Date;
    Last_PD_Time[3]=Event_Cumu_Data.Time.Month;
    Last_PD_Time[4]=Event_Cumu_Data.Time.Year;    
  }
  
  mem_set((void *)&Event_Cumu_Data,0,sizeof(Event_Cumu_Data),(void *)&Event_Cumu_Data,sizeof(Event_Cumu_Data)); 
  SET_STRUCT_SUM(Event_Cumu_Data.Prog_Flag);
  SET_STRUCT_SUM(Event_Cumu_Data.Meter_Instant_Status);

  mem_cpy((void *)&Event_Cumu_Data.Time,(void *)&Cur_Time1,sizeof(Cur_Time1),(void *)&Event_Cumu_Data.Time,sizeof(Event_Cumu_Data.Time));
  SET_STRUCT_SUM(Event_Cumu_Data);
  Write_Storage_Data(CUMU_EVENT_PD,(void *)&Event_Cumu_Data,sizeof(Event_Cumu_Data)); 
  DEBUG_PRINT(HUCK,DEBUG_0,"----------PowerOn Event Proc End----------");   
}

//检查事件累加数据是否正确
void Check_Event_Cumu_Avail()
{
  INT8U Re;

  Re = CHECK_STRUCT_SUM(Event_Cumu_Data);
  if(ASSERT(A_WARNING,1==Re))
  {
    mem_set((void *)&Event_Cumu_Data, 0, sizeof(Event_Cumu_Data), (void *)&Event_Cumu_Data, sizeof(Event_Cumu_Data));
    mem_cpy((void *)&Event_Cumu_Data, (void *)&Cur_Time1, sizeof(Cur_Time1), (void *)&Event_Cumu_Data, sizeof(Event_Cumu_Data));
    SET_STRUCT_SUM(Event_Cumu_Data);
  }

}
#undef EVENT_CUMU0_C
