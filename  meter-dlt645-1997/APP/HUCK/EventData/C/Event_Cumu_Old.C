#define EVENT_CUMU_C
#include "Includes.h"

//计算总失压累加数据
void Calc_Total_Loss_Vol()
{
  INT8U i;
  
  Event_Cumu.Phase_Loss_Vol[0].Counts=0;
  Event_Cumu.Phase_Loss_Vol[0].Time=0;
  for(i=1;i<S_NUM(Event_Cumu.Phase_Loss_Vol);i++)
  {
    Event_Cumu.Phase_Loss_Vol[0].Counts+=Event_Cumu.Phase_Loss_Vol[i].Counts; 
    Event_Cumu.Phase_Loss_Vol[0].Time+=Event_Cumu.Phase_Loss_Vol[i].Time; 
  }
  SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[0]);
  SET_STRUCT_SUM(Event_Cumu);
}

//计算总失流累加数据,包括时间和次数的累加，就是更新Event_Cumu.Phase_Loss_Cur[0]的数据      
void Calc_Total_Loss_Cur()
{
  INT8U i;
  
  Event_Cumu.Phase_Loss_Cur[0].Counts=0;
  Event_Cumu.Phase_Loss_Cur[0].Time=0;
  for(i=1;i<S_NUM(Event_Cumu.Phase_Loss_Cur);i++)
  {
    Event_Cumu.Phase_Loss_Cur[0].Counts+=Event_Cumu.Phase_Loss_Cur[i].Counts; 
    Event_Cumu.Phase_Loss_Cur[0].Time+=Event_Cumu.Phase_Loss_Cur[i].Time; 
  }
  SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[0]);
  SET_STRUCT_SUM(Event_Cumu);
}

//计算断相总累加数据
void Calc_Total_Phase_Break()
{
  INT8U i;
  
  Event_Cumu.Phase_Break[0].Counts=0;
  Event_Cumu.Phase_Break[0].Time=0;
  for(i=1;i<S_NUM(Event_Cumu.Phase_Break);i++)
  {
    Event_Cumu.Phase_Break[0].Counts+=Event_Cumu.Phase_Break[i].Counts; 
    Event_Cumu.Phase_Break[0].Time+=Event_Cumu.Phase_Break[i].Time; 
  }
  SET_STRUCT_SUM(Event_Cumu.Phase_Break[0]);
  SET_STRUCT_SUM(Event_Cumu);
}

//计算反向总累加数据
void Calc_Total_Phase_Reverse()
{
  INT8U i;
  
  Event_Cumu.Phase_Reverse[0].Counts=0;
  Event_Cumu.Phase_Reverse[0].Time=0;
  for(i=1;i<S_NUM(Event_Cumu.Phase_Reverse);i++)
  {
    Event_Cumu.Phase_Reverse[0].Counts+=Event_Cumu.Phase_Reverse[i].Counts; 
    Event_Cumu.Phase_Reverse[0].Time+=Event_Cumu.Phase_Reverse[i].Time; 
  } 
  SET_STRUCT_SUM(Event_Cumu.Phase_Reverse[0]);
  SET_STRUCT_SUM(Event_Cumu);
}
      
//读或者写失压的累计数据
void Update_Loss_Vol_Cumu_Data(INT8U Event_ID,INT8U Occur_Or_End)
{
  STORA_DI DI;
  INT8U Err,Re;

  static S_Int8U Last_Event_ID={CHK_BYTE,0xFF,CHK_BYTE};
  static S_Int8U Last_Occur_Or_End={CHK_BYTE,0xFF,CHK_BYTE};

  if(Event_ID==ID_EVENT_A_LOSS_VOLT)//A失压
    DI=_A_LOSS_VOL;
  else if(Event_ID==ID_EVENT_B_LOSS_VOLT)//B失压
    DI=_B_LOSS_VOL;
  else if(Event_ID==ID_EVENT_C_LOSS_VOLT)//C失压
    DI=_C_LOSS_VOL;
  else if(Event_ID==ID_EVENT_AB_LOSS_VOLT)//AB失压
    DI=_AB_LOSS_VOL;
  else if(Event_ID==ID_EVENT_AC_LOSS_VOLT)//AC失压
    DI=_AC_LOSS_VOL;
  else if(Event_ID==ID_EVENT_BC_LOSS_VOLT)//BC失压
    DI=_BC_LOSS_VOL;
  else if(Event_ID==ID_EVENT_ABC_LOSS_VOLT)//ABC失压
    DI=_ABC_LOSS_VOL;
  else if(Event_ID==ID_EVENT_ALL_LOSS_VOLT)//全失压
    DI=_ALL_LOSS_VOL;
  else
  {
    return;
  }
  //如果本次的事件和上次的事件完全一样，说明参数传入有错
  if(Last_Event_ID.Var==Event_ID && Last_Occur_Or_End.Var==Occur_Or_End)
  {
    ASSERT(A_WARNING,0);
    return;
  }
  
  Last_Event_ID.Var=Event_ID;
  Last_Occur_Or_End.Var=Occur_Or_End;
  
  if(EVENT_OCCUR==Occur_Or_End)//失压读取统计数据
  {
    //OS_Sem_Pend(PUB_BUF0_SEM_ID);//函数外部申请和释放该信号量
    Re=Read_Storage_Data(DI,(void *)&Pub_Buf0,(void *)&Pub_Buf0,sizeof(Pub_Buf0),&Err);
    
    DIS_PD_INT;//更新Event_Cumu数据，关闭掉电中断
    Event_Cumu.Event_Flag.Loss_Vol_Flag=0;//更新数据之前，先清发生标志
    if(Re>0 && NO_ERR==Err)
    {
      mem_cpy((void *)&Event_Cumu.Loss_Vol,(void *)Pub_Buf0,sizeof(Event_Cumu.Loss_Vol),(void *)&Event_Cumu.Loss_Vol,sizeof(Event_Cumu.Loss_Vol)); 
    }
    else
    {
      ASSERT(A_WARNING,0);
      mem_set((void *)&Event_Cumu.Loss_Vol,0,sizeof(Event_Cumu.Loss_Vol),(void *)&Event_Cumu.Loss_Vol,sizeof(Event_Cumu.Loss_Vol)); 
    }
    //OS_Sem_Post(PUB_BUF0_SEM_ID);


    Event_Cumu.Event_Flag.Loss_Vol_Flag=Event_ID;
    Event_Cumu.Event_Flag.Loss_Vol_Flag_Bak=Event_ID;
    SET_STRUCT_SUM(Event_Cumu.Event_Flag);  
    
    if(ID_EVENT_ALL_LOSS_VOLT!=Event_ID)//全失压的时间累加式特殊处理的 
    {
      Event_Cumu.Loss_Vol.Counts++;
      SET_STRUCT_SUM(Event_Cumu.Loss_Vol);
    }
    //更新内存中的分相数据
    SET_VAR(Event_Cumu.Phase_Loss_Vol[DI-_TOTAL_LOSS_VOL].Counts,Event_Cumu.Loss_Vol.Counts,&Event_Cumu.Phase_Loss_Vol,sizeof(Event_Cumu.Phase_Loss_Vol));
    SET_VAR(Event_Cumu.Phase_Loss_Vol[DI-_TOTAL_LOSS_VOL].Time,Event_Cumu.Loss_Vol.Time,&Event_Cumu.Phase_Loss_Vol,sizeof(Event_Cumu.Phase_Loss_Vol));
    SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[DI-_TOTAL_LOSS_VOL]);
    SET_STRUCT_SUM(Event_Cumu);
    
    EN_PD_INT;//重新打开掉电中断
  }
  else if(EVENT_END==Occur_Or_End)//失压恢复 
  {
    DIS_PD_INT;
    Event_Cumu.Event_Flag.Loss_Vol_Flag=0;
    //Event_Cumu.Event_Flag.Loss_Vol_Flag_Bak不予更新，因为此数保留在内存中，便于抄读
    SET_STRUCT_SUM(Event_Cumu.Event_Flag);
    SET_STRUCT_SUM(Event_Cumu);
    EN_PD_INT;
    Write_Storage_Data(DI,(void *)&Event_Cumu.Loss_Vol,sizeof(Event_Cumu.Loss_Vol)); 
  }
  else
    ASSERT(A_WARNING,0);
  
  Calc_Total_Loss_Vol();
}

//读或者写失流的累计数据
void Update_Loss_Cur_Cumu_Data(INT8U Event_ID,INT8U Occur_Or_End)
{
  STORA_DI DI;
  INT8U Err,Re;
  
  static INT8U Last_Event_ID=0xFF;
  static INT8U Last_Occur_Or_End=0xFF;

  if(Event_ID==ID_EVENT_A_LOSS_CURR)//A失流
    DI=_A_LOSS_CUR;
  else if(Event_ID==ID_EVENT_B_LOSS_CURR)//B失流
    DI=_B_LOSS_CUR;
  else if(Event_ID==ID_EVENT_C_LOSS_CURR)//C失流
    DI=_C_LOSS_CUR;
  else if(Event_ID==ID_EVENT_AB_LOSS_CURR)//AB失流
    DI=_AB_LOSS_CUR;
  else if(Event_ID==ID_EVENT_AC_LOSS_CURR)//AC失流
    DI=_AC_LOSS_CUR;
  else if(Event_ID==ID_EVENT_BC_LOSS_CURR)//BC失流
    DI=_BC_LOSS_CUR;
  else if(Event_ID==ID_EVENT_ALL_LOSS_CURR)//全失流不需要累计电能等，只需要统计次数和时间
    DI=_ALL_LOSS_CUR;  
  else
  {
    return;
  }
  
  if(Last_Event_ID==Event_ID && Last_Occur_Or_End==Occur_Or_End)//如果本次的事件和上次的事件完全一样，说明参数传入有错
    ASSERT(A_WARNING,0);
  
  Last_Event_ID=Event_ID;
  Last_Occur_Or_End=Occur_Or_End;
  
  if(EVENT_OCCUR==Occur_Or_End)//失流读取统计数据
  {
    //OS_Sem_Pend(PUB_BUF0_SEM_ID);//函数外部申请和释放该信号量
    Re=Read_Storage_Data(DI,(void *)&Pub_Buf0,(void *)&Pub_Buf0,sizeof(Pub_Buf0),&Err); 
    
    DIS_PD_INT;//更新Event_Cumu数据，关掉电中断
    Event_Cumu.Event_Flag.Loss_Cur_Flag=0;//更新数据之前，先清发生标志
    if(Re>0 && NO_ERR==Err)
    {
       mem_cpy((void *)&Event_Cumu.Loss_Cur,(void *)Pub_Buf0,sizeof(Event_Cumu.Loss_Cur),(void *)&Event_Cumu.Loss_Cur,sizeof(Event_Cumu.Loss_Cur)); 
    }
    else
    {
      ASSERT(A_WARNING,0);
      mem_set((void *)&Event_Cumu.Loss_Cur,0,sizeof(Event_Cumu.Loss_Cur),(void *)&Event_Cumu.Loss_Cur,sizeof(Event_Cumu.Loss_Cur)); 
    }
    //OS_Sem_Post(PUB_BUF0_SEM_ID);

    
    Event_Cumu.Event_Flag.Loss_Cur_Flag=Event_ID;
    Event_Cumu.Event_Flag.Loss_Cur_Flag_Bak=Event_ID;
    SET_STRUCT_SUM(Event_Cumu.Event_Flag);
    
    Event_Cumu.Loss_Cur.Counts++;
    SET_STRUCT_SUM(Event_Cumu.Loss_Cur);
    //更新内存中的分相数据
    SET_VAR(Event_Cumu.Phase_Loss_Cur[DI-_TOTAL_LOSS_CUR].Counts,Event_Cumu.Loss_Cur.Counts,&Event_Cumu.Phase_Loss_Cur,sizeof(Event_Cumu.Phase_Loss_Cur));    
    SET_VAR(Event_Cumu.Phase_Loss_Cur[DI-_TOTAL_LOSS_CUR].Time,Event_Cumu.Loss_Cur.Time,&Event_Cumu.Phase_Loss_Cur,sizeof(Event_Cumu.Phase_Loss_Cur));    
    SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[DI-_TOTAL_LOSS_CUR]);
    SET_STRUCT_SUM(Event_Cumu);
    
    EN_PD_INT;
  }
  else if(EVENT_END==Occur_Or_End) 
  {
    DIS_PD_INT;
    
    Event_Cumu.Event_Flag.Loss_Cur_Flag=0;
    //Event_Cumu.Event_Flag.Loss_Cur_Flag_Bak不予更新，因为此数保留在内存中，便于抄读
    SET_STRUCT_SUM(Event_Cumu.Event_Flag);
    SET_STRUCT_SUM(Event_Cumu);
    
    EN_PD_INT;
    Write_Storage_Data(DI,(void *)&Event_Cumu.Loss_Cur,sizeof(Event_Cumu.Loss_Cur));
  }
  else
    ASSERT(A_WARNING,0);
}

//获取当前BCD时间
void Get_Event_BCD_Time(INT8U *pDst,INT8U *pDst_Start,INT16U DstLen)
{
  ASSERT(A_WARNING,pDst>=pDst_Start && pDst+4<=pDst_Start+DstLen);
  *pDst=Event_Cumu.Event_Time.Min;
  *(pDst+1)=Event_Cumu.Event_Time.Hour;
  *(pDst+2)=Event_Cumu.Event_Time.Date;
  *(pDst+3)=Event_Cumu.Event_Time.Month;
  return;  
}

//判断某相发生事件标志
INT8U Check_Phase_Event(INT8U Flag,INT8U Phase)
{
  if(Phase==PHASE_ALL)
  {
    if((Flag & PHASE_ALL)==1)
      return 1;
    else
      return 0;
  }
  else
  {
    if(Flag==Phase)
      return 1;
    else 
      return 0;
  }
}

//失压处理
//在事件发生时刻存储其发生次数和发生时间，恢复时刻存储其累计时间和恢复时间
void Event_Cumu_Loss_Vol_Proc(INT8U Event_ID,INT8U Occur_Or_End)
{
  INT8U Temp[5];

  Get_Event_BCD_Time(Temp,Temp,sizeof(Temp));//获取当前BCD时间,分时日月  
  if(EVENT_END==Occur_Or_End)
  {  
    if(Event_ID==ID_EVENT_ALL_LOSS_VOLT)//全失压恢复
    {
      mem_cpy(Temp,(void *)All_Loss_Vol_Time.End_Time,4,Temp,sizeof(Temp));//复制恢复时间
      //Write_Storage_Data(ALL_LOSS_VOL,(void *)&Event_Cumu.Phase_Loss_Vol[8],sizeof(Event_Cumu.Phase_Loss_Vol[8]));//全失压累计次数和时间 
    }
    else if(Event_ID==ID_EVENT_A_LOSS_VOLT)//A失压恢复
    {
      //Write_Storage_Data(A_LOSS_VOL,(void *)&Event_Cumu.Phase_Loss_Vol[1],sizeof(Event_Cumu.Phase_Loss_Vol[1]));//A失压累计时间
      Write_Storage_Data(A_LOSS_VOL_END_TIME,Temp,4);//失压恢复时间
    }
    else if(Event_ID==ID_EVENT_B_LOSS_VOLT)//B失压恢复
    {
      //Write_Storage_Data(B_LOSS_VOL,(void *)&Event_Cumu.Phase_Loss_Vol[2],sizeof(Event_Cumu.Phase_Loss_Vol[2]));//B失压累计时间
      Write_Storage_Data(B_LOSS_VOL_END_TIME,Temp,4);//失压恢复时间
    }
    else if(Event_ID==ID_EVENT_C_LOSS_VOLT)//C失压恢复
    {
      //Write_Storage_Data(C_LOSS_VOL,(void *)&Event_Cumu.Phase_Loss_Vol[3],sizeof(Event_Cumu.Phase_Loss_Vol[3]));//C失压累计时间
      Write_Storage_Data(C_LOSS_VOL_END_TIME,Temp,4);//失压恢复时间
    }
    else if(Event_ID==ID_EVENT_AB_LOSS_VOLT)//AB失压恢复
    {
      //Write_Storage_Data(AB_LOSS_VOL,(void *)&Event_Cumu.Phase_Loss_Vol[4],sizeof(Event_Cumu.Phase_Loss_Vol[4]));//AB失压累计时间
      Write_Storage_Data(AB_LOSS_VOL_END_TIME,Temp,4);//失压恢复时间
    }
    else if(Event_ID==ID_EVENT_AC_LOSS_VOLT)//AC失压恢复
    {
      //Write_Storage_Data(AC_LOSS_VOL,(void *)&Event_Cumu.Phase_Loss_Vol[5],sizeof(Event_Cumu.Phase_Loss_Vol[5]));//AC失压累计时间
      Write_Storage_Data(AC_LOSS_VOL_END_TIME,Temp,4);//失压恢复时间
    }
    else if(Event_ID==ID_EVENT_BC_LOSS_VOLT)//BC失压恢复
    {
      //Write_Storage_Data(BC_LOSS_VOL,(void *)&Event_Cumu.Phase_Loss_Vol[6],sizeof(Event_Cumu.Phase_Loss_Vol[6]));//BC失压累计时间
      Write_Storage_Data(BC_LOSS_VOL_END_TIME,Temp,4);//失压恢复时间
    }
    else if(Event_ID==ID_EVENT_ABC_LOSS_VOLT)//ABC失压恢复
    {
      //Write_Storage_Data(ABC_LOSS_VOL,(void *)&Event_Cumu.Phase_Loss_Vol[7],sizeof(Event_Cumu.Phase_Loss_Vol[7]));//ABC失压累计时间
      Write_Storage_Data(ABC_LOSS_VOL_END_TIME,Temp,4);//失压恢复时间
    }
    else
      return;
    
    Write_Storage_Data(TOTAL_LOSS_VOL_END_TIME,Temp,4); //最近一次失压恢复时间   
    //Write_Storage_Data(TOTAL_LOSS_VOL,(void *)&Event_Cumu.Phase_Loss_Vol[0],sizeof(Event_Cumu.Phase_Loss_Vol[0]));//总失压累计次数和时间
  }
  else if(EVENT_OCCUR==Occur_Or_End)//失压恢复时才保存失压数据
  {
    //发生标志
    if(Event_ID==ID_EVENT_ALL_LOSS_VOLT)//全失压发生
    {
      mem_cpy(Temp,(void *)All_Loss_Vol_Time.Start_Time,4,Temp,sizeof(Temp));//复制发生时间
    }
    else if(Event_ID==ID_EVENT_A_LOSS_VOLT)//A失压发生
    {
      //Event_Cumu.Phase_Loss_Vol[1].Counts++; 
      //SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[1]);
      Write_Storage_Data(A_LOSS_VOL_OCCUR_TIME,Temp,4);//最近一次发生时刻
    }
    else if(Event_ID==ID_EVENT_B_LOSS_VOLT)//B失压发生
    {
      //Event_Cumu.Phase_Loss_Vol[2].Counts++; 
      //SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[2]);
      Write_Storage_Data(B_LOSS_VOL_OCCUR_TIME,Temp,4);//最近一次发生时刻
    }
    else if(Event_ID==ID_EVENT_C_LOSS_VOLT)//C失压发生
    {
      //Event_Cumu.Phase_Loss_Vol[3].Counts++;  
      //SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[3]);
      Write_Storage_Data(C_LOSS_VOL_OCCUR_TIME,Temp,4);//最近一次发生时刻
    }
    else if(Event_ID==ID_EVENT_AB_LOSS_VOLT)//AB失压发生
    {
      //Event_Cumu.Phase_Loss_Vol[4].Counts++;
      //SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[4]);
      Write_Storage_Data(AB_LOSS_VOL_OCCUR_TIME,Temp,4);//最近一次发生时刻
    }
    else if(Event_ID==ID_EVENT_AC_LOSS_VOLT)//AC失压发生
    {
      //Event_Cumu.Phase_Loss_Vol[5].Counts++;
      //SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[5]);
      Write_Storage_Data(AC_LOSS_VOL_OCCUR_TIME,Temp,4);//最近一次发生时刻
    }
    else if(Event_ID==ID_EVENT_BC_LOSS_VOLT)//BC失压发生
    {
      //Event_Cumu.Phase_Loss_Vol[6].Counts++;
      //SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[6]);
      Write_Storage_Data(BC_LOSS_VOL_OCCUR_TIME,Temp,4);//最近一次发生时刻
    }
    else if(Event_ID==ID_EVENT_ABC_LOSS_VOLT)//ABC失压发生
    {
      //Event_Cumu.Phase_Loss_Vol[7].Counts++;
      //SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[7]);
      Write_Storage_Data(ABC_LOSS_VOL_OCCUR_TIME,Temp,4);//最近一次发生时刻
    }
    else
      return;
    
    Write_Storage_Data(TOTAL_LOSS_VOL_OCCUR_TIME,Temp,4);//最近一次失压发生时刻    
    
    //SET_STRUCT_SUM(Event_Cumu);
    
  }
  else
  {
    ASSERT(A_WARNING,0);
    return;
  }
   
  Update_Loss_Vol_Cumu_Data(Event_ID,Occur_Or_End);//保存前一次失压事件的数据
}

//失流处理,失流发生的次数不用再这里记录
//在事件发生时刻存储其发生次数和发生时间，恢复时刻存储其累计时间和恢复时间
void Event_Cumu_Loss_Cur_Proc(INT8U Event_ID,INT8U Occur_Or_End)
{
#if 0  //这段程序用于在全失压与分相失压同时发生的情况下用
  INT8U Err;
  
  if(Event_ID==ID_EVENT_ALL_LOSS_CURR)//全失流不需要累计电能等，只需要统计次数和时间
  {
    if(Occur_Or_End==EVENT_END)//事件结束
    {
      Write_Storage_Data(ALL_LOSS_CUR,(void *)&Event_Cumu.Phase_Loss_Cur[7],sizeof(Event_Cumu.Phase_Loss_Cur[7]));//全失流累计次数和时间
      
      Event_Cumu.Event_Flag.All_Loss_Cur_Flag=0;//清除事件发生标志
      SET_STRUCT_SUM(Event_Cumu.Event_Flag);
      
      SET_STRUCT_SUM(Event_Cumu);
    }
    else//事件发生
    {
      Read_Storage_Data(ALL_LOSS_CUR,(void *)&Event_Cumu.Phase_Loss_Cur[7],(void *)&Event_Cumu.Phase_Loss_Cur[7],sizeof(Event_Cumu.Phase_Loss_Cur[7]),&Err); 
      
      Event_Cumu.Event_Flag.All_Loss_Cur_Flag=ID_EVENT_ALL_LOSS_CURR;//设置事件发生标志
      SET_STRUCT_SUM(Event_Cumu.Event_Flag);
      
      Event_Cumu.Phase_Loss_Cur[7].Counts++;
      SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[7]);
      SET_STRUCT_SUM(Event_Cumu);
    }
    return;
  }
#endif
  
  Update_Loss_Cur_Cumu_Data(Event_ID,Occur_Or_End);//保存老的失流数据
}

//断相累加处理
void Event_Cumu_Phase_Break_Proc(INT8U Event_ID,INT8U Occur_Or_End)
{
  if(Occur_Or_End==EVENT_END)
  {
    if(Event_ID==ID_EVENT_A_LOSS_PARSE)//A断相
    {
      CLR_BIT(Event_Cumu.Event_Flag.Phase_Break_Flag,0);
      Write_Storage_Data(A_BREAK_VOL,(void *)&Event_Cumu.Phase_Break[1],sizeof(Event_Cumu.Phase_Break[1]));
    }
    else if(Event_ID==ID_EVENT_B_LOSS_PARSE)//B断相
    {
      CLR_BIT(Event_Cumu.Event_Flag.Phase_Break_Flag,1);
      Write_Storage_Data(B_BREAK_VOL,(void *)&Event_Cumu.Phase_Break[2],sizeof(Event_Cumu.Phase_Break[2]));
    }
    else if(Event_ID==ID_EVENT_C_LOSS_PARSE)//C断相
    {
      CLR_BIT(Event_Cumu.Event_Flag.Phase_Break_Flag,2);
      Write_Storage_Data(C_BREAK_VOL,(void *)&Event_Cumu.Phase_Break[3],sizeof(Event_Cumu.Phase_Break[3]));
    }
    else
      return;

    //写总断相次数与时间
    SET_STRUCT_SUM(Event_Cumu.Event_Flag);
    SET_STRUCT_SUM(Event_Cumu);
    Write_Storage_Data(TOTAL_BREAK_VOL,(void *)&Event_Cumu.Phase_Break[0],sizeof(Event_Cumu.Phase_Break[0]));   
  }
  else if(Occur_Or_End==EVENT_OCCUR)
  {
    if(Event_ID==ID_EVENT_A_LOSS_PARSE)//A断相
    {
      SET_BIT(Event_Cumu.Event_Flag.Phase_Break_Flag,0);
      Event_Cumu.Phase_Break[1].Counts++;
      SET_STRUCT_SUM(Event_Cumu.Phase_Break[1]);
    }
    else if(Event_ID==ID_EVENT_B_LOSS_PARSE)//B断相
    {
      SET_BIT(Event_Cumu.Event_Flag.Phase_Break_Flag,1);
      Event_Cumu.Phase_Break[2].Counts++;
      SET_STRUCT_SUM(Event_Cumu.Phase_Break[2]);
    }
    else if(Event_ID==ID_EVENT_C_LOSS_PARSE)//C断相
    {
      SET_BIT(Event_Cumu.Event_Flag.Phase_Break_Flag,2);
      Event_Cumu.Phase_Break[3].Counts++;
      SET_STRUCT_SUM(Event_Cumu.Phase_Break[3]);
    }
    else
      return;

    SET_STRUCT_SUM(Event_Cumu.Event_Flag);
    Event_Cumu.Phase_Break[0].Counts++;//总断相次数,断相恢复时才保存断相数据
    SET_STRUCT_SUM(Event_Cumu.Phase_Break[0]);
    SET_STRUCT_SUM(Event_Cumu);
  }
}

//反向累加处理
void Event_Cumu_Phase_Reverse_Proc(INT8U Event_ID,INT8U Occur_Or_End)
{
  if(Occur_Or_End==EVENT_END)//事件恢复
  {
    if(Event_ID==ID_EVENT_A_NEG_CURR)//A反向恢复
    {
      CLR_BIT(Event_Cumu.Event_Flag.Phase_Reverse_Flag,0);
      Write_Storage_Data(A_REVERSE,(void *)&Event_Cumu.Phase_Reverse[1],sizeof(Event_Cumu.Phase_Reverse[1]));
    }
    else if(Event_ID==ID_EVENT_B_NEG_CURR)//B反向恢复
    {
      CLR_BIT(Event_Cumu.Event_Flag.Phase_Reverse_Flag,1);
      Write_Storage_Data(B_REVERSE,(void *)&Event_Cumu.Phase_Reverse[2],sizeof(Event_Cumu.Phase_Reverse[2]));
    }
    else if(Event_ID==ID_EVENT_C_NEG_CURR)//C反向恢复
    {
      CLR_BIT(Event_Cumu.Event_Flag.Phase_Reverse_Flag,2);
      Write_Storage_Data(C_REVERSE,(void *)&Event_Cumu.Phase_Reverse[3],sizeof(Event_Cumu.Phase_Reverse[3]));
    }
    else
      return;

    //写总反向次数与时间
    SET_STRUCT_SUM(Event_Cumu.Event_Flag);
    SET_STRUCT_SUM(Event_Cumu);
    Write_Storage_Data(TOTAL_REVERSE,(void *)&Event_Cumu.Phase_Reverse[0],sizeof(Event_Cumu.Phase_Reverse[0]));//总反向次数和时间   
  }
  else if(Occur_Or_End==EVENT_OCCUR)
  {
    if(Event_ID==ID_EVENT_A_NEG_CURR)//A反向发生
    {
      SET_BIT(Event_Cumu.Event_Flag.Phase_Reverse_Flag,0);
      Event_Cumu.Phase_Reverse[1].Counts++;
      SET_STRUCT_SUM(Event_Cumu.Phase_Reverse[1]);
    }
    else if(Event_ID==ID_EVENT_B_NEG_CURR)//B反向发生
    {
      SET_BIT(Event_Cumu.Event_Flag.Phase_Reverse_Flag,1);
      Event_Cumu.Phase_Reverse[2].Counts++;
      SET_STRUCT_SUM(Event_Cumu.Phase_Reverse[2]);
    }
    else if(Event_ID==ID_EVENT_C_NEG_CURR)//C反向发生
    {
      SET_BIT(Event_Cumu.Event_Flag.Phase_Reverse_Flag,2);
      Event_Cumu.Phase_Reverse[3].Counts++;
      SET_STRUCT_SUM(Event_Cumu.Phase_Reverse[3]);
    }
    else
      return;

    SET_STRUCT_SUM(Event_Cumu.Event_Flag);
    Event_Cumu.Phase_Reverse[0].Counts++;//总反向次数
    SET_STRUCT_SUM(Event_Cumu.Phase_Reverse[0]);
    SET_STRUCT_SUM(Event_Cumu);
  }
}

//事件累加处理
void Event_Cumu_Proc(INT8U Event_ID,INT8U Occur_Or_End)
{
  Event_Cumu_Loss_Vol_Proc(Event_ID,Occur_Or_End);  //失压处理
  Event_Cumu_Loss_Cur_Proc(Event_ID,Occur_Or_End);  //失流处理
  Event_Cumu_Phase_Break_Proc(Event_ID,Occur_Or_End);//断相处理
  Event_Cumu_Phase_Reverse_Proc(Event_ID,Occur_Or_End);//反向处理
}

/*
typedef struct
{
  INT32U Chang_Plus_AcEnerg[4];            //A/B/C/SUM各元件正向有功增量:单位:0.01wh
  INT32U Chang_Nega_AcEnerg[4];            //A/B/C/SUM各元件反向有功增量:单位:0.01wh
  
  //INT32U Chang_Plus_ReacEnerg[4];          //A/B/C/SUM各元件正向无功增量(无功组合1):单位:0.01warh  
  //INT32U Chang_Nega_ReacEnerg[4];          //A/B/C/SUM各元件反向无功增量(无功组合2):单位:0.01warh
  
  INT32U Chang_Quad_ReacEnerg[4][4];       //A/B/C/SUM各元件四象限无功增量,Chang_Quad_ReacEnerg[1][2]表示B向3象限无功
  
  INT32U Add_AmpaHour[3];                 //AH累计:0.0001As,累计量
  INT32U Add_CopperLoss[4];		  //A,B,C,sum铜损,单位为0.0000001kA2s,累计量
  INT32U Add_IronLoss[4];		  //A,B,C,sum铁损,单位为0.0000001KV2s，累计量
  INT32U Add_GenaralErr[4];		  //A,B,C,sum合成误差,单位:0.01*tgFA*s,累计量
  INT8U CS[CS_BYTES];
}MEASU_ENERG;                             //计量累计数据和增量数据
*/

//计算累加量中的正向无功量
void Calc_Phase_Reactive(MEASU_ENERG *pEnergy,S_Phase_Reactive *pPhase_Reactive)
{
  INT32S Pos_Reactive;//正向无功，也是组合无功1
  INT32S Neg_Reactive;//反省无功，也是组合无功2
  INT8U i,j;
  
  for(i=0;i<4;i++)//A、B、C、总分别计算组合无功
  {
    Pos_Reactive=0;
    Neg_Reactive=0;
    for(j=0;j<4;j++)//四象限
    {
      if(GET_BIT(Mode_Word.Mode[5].Byte,j))//分相正向无功，参与计算? 
      {
        if(GET_BIT(Mode_Word.Mode[5].Byte,4+j)==0)//加
          Pos_Reactive+=pEnergy->Chang_Quad_ReacEnerg[i][j];
        else//减
          Pos_Reactive-=pEnergy->Chang_Quad_ReacEnerg[i][j];
      }
      
      if(GET_BIT(Mode_Word.Mode[6].Byte,j))//分相反向无功，参与计算? 
      {
        if(GET_BIT(Mode_Word.Mode[6].Byte,4+j)==0)//加
          Neg_Reactive+=pEnergy->Chang_Quad_ReacEnerg[i][j];
        else//减
          Neg_Reactive-=pEnergy->Chang_Quad_ReacEnerg[i][j];
      }
    }
    pPhase_Reactive->Pos_Reactive[i]=Pos_Reactive;
    pPhase_Reactive->Neg_Reactive[i]=Neg_Reactive;
  }
}

//检查某事件是否处于发生状态
INT8U Check_Cumu_Event_Occuring(INT8U Event_ID)
{
  switch(Event_ID)
  {
    //失压
    case ID_EVENT_A_LOSS_VOLT:
    case ID_EVENT_B_LOSS_VOLT:
    case ID_EVENT_C_LOSS_VOLT:
    case ID_EVENT_AB_LOSS_VOLT:
    case ID_EVENT_AC_LOSS_VOLT:
    case ID_EVENT_BC_LOSS_VOLT:
    case ID_EVENT_ABC_LOSS_VOLT:
      return (Event_ID==Event_Cumu.Event_Flag.Loss_Vol_Flag)?1:0;
    //失流  
    case ID_EVENT_A_LOSS_CURR:
    case ID_EVENT_B_LOSS_CURR:
    case ID_EVENT_C_LOSS_CURR:
    case ID_EVENT_AB_LOSS_CURR:
    case ID_EVENT_AC_LOSS_CURR:
    case ID_EVENT_BC_LOSS_CURR:
    case ID_EVENT_ALL_LOSS_CURR:
      return (Event_ID==Event_Cumu.Event_Flag.Loss_Cur_Flag)?1:0;
    //断相  
    case ID_EVENT_A_LOSS_PARSE:
      return GET_BIT(Event_Cumu.Event_Flag.Phase_Break_Flag,0);
    case ID_EVENT_B_LOSS_PARSE:
      return GET_BIT(Event_Cumu.Event_Flag.Phase_Break_Flag,1);   
    case ID_EVENT_C_LOSS_PARSE:
      return GET_BIT(Event_Cumu.Event_Flag.Phase_Break_Flag,2);
    //反向
    case ID_EVENT_A_NEG_CURR:
      return GET_BIT(Event_Cumu.Event_Flag.Phase_Reverse_Flag,0);
    case ID_EVENT_B_NEG_CURR:
      return GET_BIT(Event_Cumu.Event_Flag.Phase_Reverse_Flag,1);   
    case ID_EVENT_C_NEG_CURR:
      return GET_BIT(Event_Cumu.Event_Flag.Phase_Reverse_Flag,2);
    default:
      ASSERT(A_WARNING,0);
      return 0;
  }
}

//失压数据累计
void Event_Loss_Vol_Cumu(MEASU_ENERG *pEnergy)
{
  INT8U i;
  static S_Int8U Min_Bak={CHK_BYTE,0xFF,CHK_BYTE};
  
  if(Event_Cumu.Event_Flag.Loss_Vol_Flag==0)//当前没有失压事件发生
    return;
  
  if(Min_Bak.Var!=Cur_Time0.Time[T_MIN])//每分钟进行次数累加
  {
    Min_Bak.Var=Cur_Time0.Time[T_MIN];
    Event_Cumu.Loss_Vol.Time++;
    SET_STRUCT_SUM(Event_Cumu.Loss_Vol);
    if(Check_Cumu_Event_Occuring(ID_EVENT_A_LOSS_VOLT))
    {
      Event_Cumu.Phase_Loss_Vol[1].Time=Event_Cumu.Loss_Vol.Time;
      SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[1]);
    }
    else if(Check_Cumu_Event_Occuring(ID_EVENT_B_LOSS_VOLT))
    {
      Event_Cumu.Phase_Loss_Vol[2].Time=Event_Cumu.Loss_Vol.Time;
      SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[2]);
    }
    else if(Check_Cumu_Event_Occuring(ID_EVENT_C_LOSS_VOLT))
    {
      Event_Cumu.Phase_Loss_Vol[3].Time=Event_Cumu.Loss_Vol.Time;
      SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[3]);
    }
    else if(Check_Cumu_Event_Occuring(ID_EVENT_AB_LOSS_VOLT))
    {
      Event_Cumu.Phase_Loss_Vol[4].Time=Event_Cumu.Loss_Vol.Time;
      SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[4]);
    }
    else if(Check_Cumu_Event_Occuring(ID_EVENT_AC_LOSS_VOLT))
    {
      Event_Cumu.Phase_Loss_Vol[5].Time=Event_Cumu.Loss_Vol.Time;
      SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[5]);
    }
    else if(Check_Cumu_Event_Occuring(ID_EVENT_BC_LOSS_VOLT))
    {
      Event_Cumu.Phase_Loss_Vol[6].Time=Event_Cumu.Loss_Vol.Time;
      SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[6]);
    } 
    else if(Check_Cumu_Event_Occuring(ID_EVENT_ABC_LOSS_VOLT))
    {
      Event_Cumu.Phase_Loss_Vol[7].Time=Event_Cumu.Loss_Vol.Time;
      SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[7]);      
    }
    else
      return;//当前没有失压发生

    Calc_Total_Loss_Vol();
  }
  
  Calc_Phase_Reactive(pEnergy,(S_Phase_Reactive *)&Cumu_Phase_Reactive);//计算累加量的组合无功
  //失压期间电能累计
  //Cumu_Phase_Reactive中数据的顺序是A、B、C、总
  for(i=0;i<3;i++)
  {
    //[0]表示总，与秒累加量顺序不一样
    Event_Cumu.Loss_Vol.Phase_Pos_Active[i+1]+=pEnergy->Chang_Plus_AcEnerg[i]/ENERGY_RATIO;
    Event_Cumu.Loss_Vol.Phase_Neg_Active[i+1]+=pEnergy->Chang_Nega_AcEnerg[i]/ENERGY_RATIO;
    Event_Cumu.Loss_Vol.Phase_Pos_Reactive[i+1]+=Cumu_Phase_Reactive.Pos_Reactive[i]/ENERGY_RATIO;
    Event_Cumu.Loss_Vol.Phase_Neg_Reactive[i+1]+=Cumu_Phase_Reactive.Neg_Reactive[i]/ENERGY_RATIO;
    Event_Cumu.Loss_Vol.Phase_AH[i]+=pEnergy->Add_AmpaHour[i]/AH_RATIO;
  } 
  Event_Cumu.Loss_Vol.Phase_Pos_Active[0]+=pEnergy->Chang_Plus_AcEnerg[3]/ENERGY_RATIO;
  Event_Cumu.Loss_Vol.Phase_Neg_Active[0]+=pEnergy->Chang_Nega_AcEnerg[3]/ENERGY_RATIO;
  Event_Cumu.Loss_Vol.Phase_Pos_Reactive[0]+=Cumu_Phase_Reactive.Pos_Reactive[3]/ENERGY_RATIO;
  Event_Cumu.Loss_Vol.Phase_Neg_Reactive[0]+=Cumu_Phase_Reactive.Neg_Reactive[3]/ENERGY_RATIO;
  
  SET_STRUCT_SUM(Event_Cumu.Loss_Vol);
  SET_STRUCT_SUM(Event_Cumu);
}

//失流数据累计
void Event_Loss_Cur_Cumu(MEASU_ENERG *pEnergy)
{
  INT8U i;
  static S_Int8U Min_Bak={CHK_BYTE,0xFF,CHK_BYTE};
  
  if(Event_Cumu.Event_Flag.Loss_Cur_Flag!=0)//有分相或合相事件发生，但不包括全失流
  {  
    if(Min_Bak.Var!=Cur_Time0.Time[T_MIN])//每分钟进行次数累加
    {
      Event_Cumu.Loss_Cur.Time++;  
      SET_STRUCT_SUM(Event_Cumu.Loss_Cur);
      if(Check_Cumu_Event_Occuring(ID_EVENT_A_LOSS_CURR))
      {
        Event_Cumu.Phase_Loss_Cur[1].Time=Event_Cumu.Loss_Cur.Time;
        SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[1]); 
      }
      else if(Check_Cumu_Event_Occuring(ID_EVENT_B_LOSS_CURR))
      {
        Event_Cumu.Phase_Loss_Cur[2].Time=Event_Cumu.Loss_Cur.Time;
        SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[2]); 
      }      
      else if(Check_Cumu_Event_Occuring(ID_EVENT_C_LOSS_CURR))
      {
        Event_Cumu.Phase_Loss_Cur[3].Time=Event_Cumu.Loss_Cur.Time;
        SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[3]); 
      }
      else if(Check_Cumu_Event_Occuring(ID_EVENT_AB_LOSS_CURR))
      {
        Event_Cumu.Phase_Loss_Cur[4].Time=Event_Cumu.Loss_Cur.Time;
        SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[4]); 
      }
      else if(Check_Cumu_Event_Occuring(ID_EVENT_AC_LOSS_CURR))
      {
        Event_Cumu.Phase_Loss_Cur[5].Time=Event_Cumu.Loss_Cur.Time;
        SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[5]); 
      }
      else if(Check_Cumu_Event_Occuring(ID_EVENT_BC_LOSS_CURR))
      {
        Event_Cumu.Phase_Loss_Cur[6].Time=Event_Cumu.Loss_Cur.Time;
        SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[6]); 
      }
      else if(Check_Cumu_Event_Occuring(ID_EVENT_ALL_LOSS_CURR))
      {
        Event_Cumu.Phase_Loss_Cur[7].Time=Event_Cumu.Loss_Cur.Time;
        SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[7]); 
      }
      else
        return;//当前没有失流发生
      
      Calc_Total_Loss_Cur();
  
    }
    //每秒电能数据累加
    Calc_Phase_Reactive(pEnergy,(S_Phase_Reactive *)&Cumu_Phase_Reactive);//计算累加量的组合无功
    //A、B、C三相
    for(i=0;i<3;i++)
    {
      //排列顺序不一致，总、A、B、C与A、B、C、总
      Event_Cumu.Loss_Cur.Phase_Pos_Active[i+1]+=pEnergy->Chang_Plus_AcEnerg[i]/ENERGY_RATIO;
      Event_Cumu.Loss_Cur.Phase_Neg_Active[i+1]+=pEnergy->Chang_Nega_AcEnerg[i]/ENERGY_RATIO;
      Event_Cumu.Loss_Cur.Phase_Pos_Reactive[i+1]+=Cumu_Phase_Reactive.Pos_Reactive[i]/ENERGY_RATIO;
      Event_Cumu.Loss_Cur.Phase_Neg_Reactive[i+1]+=Cumu_Phase_Reactive.Neg_Reactive[i]/ENERGY_RATIO;
    }  
    //总
    Event_Cumu.Loss_Cur.Phase_Pos_Active[0]+=pEnergy->Chang_Plus_AcEnerg[3]/ENERGY_RATIO;
    Event_Cumu.Loss_Cur.Phase_Neg_Active[0]+=pEnergy->Chang_Nega_AcEnerg[3]/ENERGY_RATIO;
    Event_Cumu.Loss_Cur.Phase_Pos_Reactive[0]+=Cumu_Phase_Reactive.Pos_Reactive[3]/ENERGY_RATIO;
    Event_Cumu.Loss_Cur.Phase_Neg_Reactive[0]+=Cumu_Phase_Reactive.Neg_Reactive[3]/ENERGY_RATIO;
  
    SET_STRUCT_SUM(Event_Cumu.Loss_Cur); 
    SET_STRUCT_SUM(Event_Cumu);
  }
  
#if 0  //这段程序用于在全失压与分相失压同时发生的情况下用  //全失流？
  if(Event_Cumu.Event_Flag.All_Loss_Cur_Flag!=0)//全失压事件发生
  {
    if(Min_Bak.Var!=Cur_Time0.Time[T_MIN])//每分钟进行次数累加
    {
      Event_Cumu.Phase_Loss_Cur[7].Time++;
      SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[7]);
      SET_STRUCT_SUM(Event_Cumu);
      
      Calc_Total_Loss_Cur();      
    }  
  }
#endif
  
  if(Min_Bak.Var!=Cur_Time0.Time[T_MIN])//每分钟进行次数累加
    Min_Bak.Var=Cur_Time0.Time[T_MIN];
}


//反向数据累计
void Event_Phase_Reverse_Cumu(MEASU_ENERG *pEnergy)
{
  INT8U Flag;
  INT8U i,j;
  INT8U Min_Flag;
  
  static S_Int8U Min_Bak={CHK_BYTE,0xFF,CHK_BYTE};

  if(Event_Cumu.Event_Flag.Phase_Reverse_Flag==0)
    return;
 
  Min_Flag = 0;
  
  if(Min_Bak.Var!=Cur_Time0.Time[T_MIN])//分钟累加
  {
    Min_Bak.Var=Cur_Time0.Time[T_MIN];
    Min_Flag = 1;
  } 
  
  //总反向累计数据
  if(Min_Flag != 0)//分钟累加
  {
    Event_Cumu.Phase_Reverse[0].Time++;//0表示总
    SET_STRUCT_SUM(Event_Cumu.Phase_Reverse[0]);
  }
  
  //三相累加
  for(j=0;j<3;j++)//每秒电能累加
  {
    Event_Cumu.Phase_Reverse[0].Phase_Pos_Active[j+1]+=pEnergy->Chang_Plus_AcEnerg[i]/ENERGY_RATIO;
    Event_Cumu.Phase_Reverse[0].Phase_Neg_Active[j+1]+=pEnergy->Chang_Nega_AcEnerg[i]/ENERGY_RATIO;
  }

  //总累加
  Event_Cumu.Phase_Reverse[0].Phase_Pos_Active[0]+=pEnergy->Chang_Plus_AcEnerg[3]/ENERGY_RATIO;
  Event_Cumu.Phase_Reverse[0].Phase_Neg_Active[0]+=pEnergy->Chang_Nega_AcEnerg[3]/ENERGY_RATIO;
  SET_STRUCT_SUM(Event_Cumu.Phase_Reverse[0]);
    
  //三相累加  
  for(i=0;i<3;i++)
  {
    if(i==0)
      Flag=ID_EVENT_A_NEG_CURR;
    else if(i==1)
      Flag=ID_EVENT_B_NEG_CURR;
    else if(i==2)
      Flag=ID_EVENT_C_NEG_CURR;
      
    if(Check_Cumu_Event_Occuring(Flag))//第i个相反向了，计入累加
    {
      if(Min_Flag != 0)//分钟累加
      {
        Event_Cumu.Phase_Reverse[i+1].Time++;//0表示总
        SET_STRUCT_SUM(Event_Cumu.Phase_Reverse[i+1]);
      }
      //分相累加
      for(j=0;j<3;j++)//每秒电能累加
      {
        Event_Cumu.Phase_Reverse[i+1].Phase_Pos_Active[j+1]+=pEnergy->Chang_Plus_AcEnerg[i]/ENERGY_RATIO;
        Event_Cumu.Phase_Reverse[i+1].Phase_Neg_Active[j+1]+=pEnergy->Chang_Nega_AcEnerg[i]/ENERGY_RATIO;
      }

      //总累加
      Event_Cumu.Phase_Reverse[i+1].Phase_Pos_Active[0]+=pEnergy->Chang_Plus_AcEnerg[3]/ENERGY_RATIO;
      Event_Cumu.Phase_Reverse[i+1].Phase_Neg_Active[0]+=pEnergy->Chang_Nega_AcEnerg[3]/ENERGY_RATIO;
      SET_STRUCT_SUM(Event_Cumu.Phase_Reverse[i+1]);
    }
  }
  Calc_Total_Phase_Reverse();
}

//断相数据累计
void Event_Phase_Break_Cumu(MEASU_ENERG *pEnergy)
{
  INT8U Flag;
  INT8U i;
  static S_Int8U Min_Bak={CHK_BYTE,0xFF,CHK_BYTE};

  if(Min_Bak.Var!=Cur_Time0.Time[T_MIN])//每分钟数据累加
  {
    Min_Bak.Var=Cur_Time0.Time[T_MIN];
    
    for(i=0;i<3;i++)
    {
      if(i==0)
        Flag=ID_EVENT_A_LOSS_PARSE;
      else if(i==1)
        Flag=ID_EVENT_B_LOSS_PARSE;
      else if(i==2)
        Flag=ID_EVENT_C_LOSS_PARSE;
        
      if(Check_Cumu_Event_Occuring(Flag))//第i个相断相了，计入累加
      {  
          Event_Cumu.Phase_Break[i+1].Time++;//0表示总
          SET_STRUCT_SUM(Event_Cumu.Phase_Break[i+1]);
      }
    }
    Calc_Total_Phase_Break();
  }
}

//事件的电能累计,上层函数每秒调用一次该函数
void Event_Energy_Cumu(MEASU_ENERG *pEnergy)
{
  mem_cpy((void *)&Event_Cumu.Event_Time,(void *)&Cur_Time1,sizeof(Cur_Time1),(void *)&Event_Cumu.Event_Time,sizeof(Event_Cumu.Event_Time));
  SET_STRUCT_SUM(Event_Cumu);
  
  Event_Loss_Vol_Cumu(pEnergy);
  Event_Loss_Cur_Cumu(pEnergy);
  Event_Phase_Reverse_Cumu(pEnergy);
  Event_Phase_Break_Cumu(pEnergy);
  //Check_Event_Cumu_Avail();
}

//从ROM中读出累加的数据，主要是标准645中的失压和失流、断相、反向的次数统计
void Read_Event_Cumu_Data_From_Rom()
{
  INT8U i;
  INT16U Len;
  INT8U Err=NO_ERR;
  
  mem_set((void *)&Event_Cumu,0,sizeof(Event_Cumu),(void *)&Event_Cumu,sizeof(Event_Cumu));
  INIT_STRUCT_VAR(Event_Cumu); 
  //初始化事件时间
  mem_cpy((void *)&Event_Cumu.Event_Time,(void *)&Cur_Time1,sizeof(Cur_Time1),(void *)&Event_Cumu.Event_Time,sizeof(Event_Cumu.Event_Time));
  //初始化事件标志
  mem_set((void *)&Event_Cumu.Event_Flag,0,sizeof(Event_Cumu.Event_Flag),(void *)&Event_Cumu.Event_Flag,sizeof(Event_Cumu.Event_Flag));
  SET_STRUCT_SUM(Event_Cumu.Event_Flag); 

  //初始化编程记录
  mem_set((void *)&Event_Cumu.Prog_Flag,0,sizeof(Event_Cumu.Prog_Flag),(void *)&Event_Cumu.Prog_Flag,sizeof(Event_Cumu.Prog_Flag));
  SET_STRUCT_SUM(Event_Cumu.Prog_Flag); 
  
  //失压数据初始化
  for(i=0;i<S_NUM(Event_Cumu.Phase_Loss_Vol);i++)
  {
    //失压数据
    Len=Read_Storage_Data(_TOTAL_LOSS_VOL+i,(void *)&Event_Cumu.Loss_Vol,(void *)&Event_Cumu.Loss_Vol,sizeof(Event_Cumu.Loss_Vol),&Err);
    if(Len!=sizeof(S_Loss_Vol_Cumu) || Err!=NO_ERR)
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Read LOSS_VOL %lx data error",_TOTAL_LOSS_VOL+i);
      ASSERT(A_WARNING,0);
      mem_set((void *)&Event_Cumu.Loss_Vol,0,sizeof(Event_Cumu.Loss_Vol),(void *)&Event_Cumu.Loss_Vol,sizeof(Event_Cumu.Loss_Vol));
    }
    //将失压的时间和次数长驻内存
    Event_Cumu.Phase_Loss_Vol[i].Counts=Event_Cumu.Loss_Vol.Counts;
    Event_Cumu.Phase_Loss_Vol[i].Time=Event_Cumu.Loss_Vol.Time;
    SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[i]);
  }
  
  //失流数据初始化
  for(i=0;i<S_NUM(Event_Cumu.Phase_Loss_Cur);i++)
  {
    //失流数据    
    Len=Read_Storage_Data(_TOTAL_LOSS_CUR+i,(void *)&Event_Cumu.Loss_Cur,(void *)&Event_Cumu.Loss_Cur,sizeof(Event_Cumu.Loss_Cur),&Err);
    if(Len!=sizeof(S_Loss_Cur_Cumu) || Err!=NO_ERR)
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Read LOSS_CUR %lx data error",_TOTAL_LOSS_CUR+i);
      ASSERT(A_WARNING,0);      
      mem_set((void *)&Event_Cumu.Loss_Cur,0,sizeof(Event_Cumu.Loss_Cur),(void *)&Event_Cumu.Loss_Cur,sizeof(Event_Cumu.Loss_Cur));
    }
    //将失流的时间和次数长驻内存      
    Event_Cumu.Phase_Loss_Cur[i].Counts=Event_Cumu.Loss_Cur.Counts;
    Event_Cumu.Phase_Loss_Cur[i].Time=Event_Cumu.Loss_Cur.Time;
    SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[i]);
  }
  
  //断相数据    
  for(i=0;i<S_NUM(Event_Cumu.Phase_Break);i++)
  {
    Len=Read_Storage_Data(TOTAL_BREAK_VOL+i,(void *)&Event_Cumu.Phase_Break[i],(void *)&Event_Cumu.Phase_Break[i],sizeof(Event_Cumu.Phase_Break[i]),&Err);
    if(Len!=sizeof(S_Phase_Cumu) || Err!=NO_ERR)
    {
      ASSERT(A_WARNING,0);      
      mem_set((void *)&Event_Cumu.Phase_Break[i],0,sizeof(Event_Cumu.Phase_Break[i]),(void *)&Event_Cumu.Phase_Break[i],sizeof(Event_Cumu.Phase_Break[i]));
    }
    SET_STRUCT_SUM(Event_Cumu.Phase_Break[i]);
  }
  
  //反向数据
  for(i=0;i<S_NUM(Event_Cumu.Phase_Reverse);i++)
  {
    Len=Read_Storage_Data(TOTAL_REVERSE+i,(void *)&Event_Cumu.Phase_Reverse[i],(void *)&Event_Cumu.Phase_Reverse[i],sizeof(Event_Cumu.Phase_Reverse[i]),&Err);
    if(Len!=sizeof(S_Reverse_Cumu) || Err!=NO_ERR)
    {
      ASSERT(A_WARNING,0);
      mem_set((void *)&Event_Cumu.Phase_Reverse[i],0,sizeof(Event_Cumu.Phase_Reverse[i]),(void *)&Event_Cumu.Phase_Reverse[i],sizeof(Event_Cumu.Phase_Reverse[i]));
    }
    SET_STRUCT_SUM(Event_Cumu.Phase_Reverse[i]);
  }                                                            

  SET_STRUCT_SUM(Event_Cumu.Loss_Vol);
  SET_STRUCT_SUM(Event_Cumu.Loss_Cur);
  SET_STRUCT_SUM(Event_Cumu);
  
  //统计总的次数和时间
  Calc_Total_Loss_Vol();
  Calc_Total_Loss_Cur();
  Calc_Total_Phase_Break();
  Calc_Total_Phase_Reverse();
}

/*
//保存当前数据
void Save_Event_Cumu_Data()
{
  INT8U i;
  
  for(i=0;i<8;i++)
  {
    Write_Storage_Data(A_LOSS_VOL+i,(void *)&Event_Cumu.Phase_Loss_Vol[i],sizeof(Event_Cumu.Phase_Loss_Vol[i]));   
    Write_Storage_Data(A_LOSS_CUR+i,(void *)&Event_Cumu.Phase_Loss_Cur[i],sizeof(Event_Cumu.Phase_Loss_Cur[i]));
    if(i<3)
    {
      Write_Storage_Data(A_BREAK_VOL,(void *)&Event_Cumu.Phase_Break[i],sizeof(Event_Cumu.Phase_Break[i]));    
      Write_Storage_Data(A_REVERSE,(void *)&Event_Cumu.Phase_Reverse[i],sizeof(Event_Cumu.Phase_Reverse[i]));
    }
  }
}

//保存备份数据
void Save_Event_Cumu_Bak_Data()
{
  INT8U Re;
  INT8U i;
  
  Re=CHECK_STRUCT_VAR(Event_Cumu);
  ASSERT(A_WARNING,1==Re);
 
  for(i=0;i<S_NUM(Event_Cumu.Phase_Loss_Vol);i++)
    Write_Storage_Data(TOTAL_LOSS_VOL+i,(void *)&Event_Cumu.Phase_Loss_Vol[i],sizeof(Event_Cumu.Phase_Loss_Vol[i]));
  
  for(i=0;i<S_NUM(Event_Cumu.Phase_Loss_Cur);i++)
    Write_Storage_Data(TOTAL_LOSS_CUR+i,(void *)&Event_Cumu.Phase_Loss_Cur[i],sizeof(Event_Cumu.Phase_Loss_Cur[i]));
 
  for(i=0;i<S_NUM(Event_Cumu.Phase_Break);i++)
    Write_Storage_Data(TOTAL_BREAK_VOL+i,(void *)&Event_Cumu.Phase_Break[i],sizeof(Event_Cumu.Phase_Break[i]));    

  for(i=0;i<S_NUM(Event_Cumu.Phase_Reverse);i++)
    Write_Storage_Data(TOTAL_REVERSE+i,(void *)&Event_Cumu.Phase_Reverse[i],sizeof(Event_Cumu.Phase_Reverse[i]));
}
*/

//设置默认的累计数据
void Set_Def_Event_Cumu_Data()
{
  INT16U Len;
  INT8U i;
  STORA_DI DI;
  
  OS_Sem_Pend(PUB_BUF0_SEM_ID);
  mem_set((void *)Pub_Buf0,0,sizeof(Pub_Buf0),(void *)Pub_Buf0,sizeof(Pub_Buf0));
  mem_set((void *)&Event_Cumu,0,sizeof(Event_Cumu),(void *)&Event_Cumu,sizeof(Event_Cumu));
  INIT_STRUCT_VAR(Event_Cumu);

  
  for(i=0;i<S_NUM(Event_Cumu.Phase_Loss_Vol);i++)
    SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[i]);
  
  for(i=0;i<S_NUM(Event_Cumu.Phase_Loss_Cur);i++)
    SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[i]);

  for(i=0;i<S_NUM(Event_Cumu.Phase_Break);i++)
    SET_STRUCT_SUM(Event_Cumu.Phase_Break[i]);

  for(i=0;i<S_NUM(Event_Cumu.Phase_Reverse);i++)
    SET_STRUCT_SUM(Event_Cumu.Phase_Reverse[i]);
  
  SET_STRUCT_SUM(Event_Cumu.Meter_Instant_Status);
  SET_STRUCT_SUM(Event_Cumu.Event_Flag);
  SET_STRUCT_SUM(Event_Cumu.Loss_Vol);
  SET_STRUCT_SUM(Event_Cumu.Loss_Cur);  

  SET_STRUCT_SUM(Event_Cumu);
  
  for(i=0;i<9;i++)
  {
    DI=TOTAL_LOSS_VOL+i;  //失压时间和次数累加数据
    Len=Get_Storage_Data_Len(DI);
    Write_Storage_Data(DI,(void *)Pub_Buf0,Len);

    DI=_TOTAL_LOSS_VOL+i; //失压电能累加数据
    Len=Get_Storage_Data_Len(DI);
    Write_Storage_Data(DI,(void *)Pub_Buf0,Len);
    
    if(i<8)//失压发生和结束时刻
    {
      DI=TOTAL_LOSS_VOL_OCCUR_TIME+i;
      Len=Get_Storage_Data_Len(DI);
      Write_Storage_Data(DI,(void *)Pub_Buf0,Len);

      DI=TOTAL_LOSS_VOL_END_TIME+i;
      Len=Get_Storage_Data_Len(DI);
      Write_Storage_Data(DI,(void *)Pub_Buf0,Len);      
    }
    
    if(i<8)//失流
    {
      DI=TOTAL_LOSS_CUR+i;
      Len=Get_Storage_Data_Len(DI);
      Write_Storage_Data(DI,(void *)Pub_Buf0,Len);
      
      DI=_TOTAL_LOSS_CUR+i;
      Len=Get_Storage_Data_Len(DI);
      Write_Storage_Data(DI,(void *)Pub_Buf0,Len);
    }
    
    if(i<4)//断相和反向
    {
      DI=TOTAL_BREAK_VOL+i;
      Len=Get_Storage_Data_Len(DI);
      Write_Storage_Data(DI,(void *)Pub_Buf0,Len);      
      
      DI=TOTAL_REVERSE+i;
      Len=Get_Storage_Data_Len(DI);
      Write_Storage_Data(DI,(void *)Pub_Buf0,Len);    
    }
    
  }
  Write_Storage_Data(CUR_EVENT_PD,(void *)&Event_Cumu,sizeof(Event_Cumu));
  
  //失压开始和结束时间
  Write_Storage_Data(TOTAL_LOSS_VOL_OCCUR_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(A_LOSS_VOL_OCCUR_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(B_LOSS_VOL_OCCUR_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(C_LOSS_VOL_OCCUR_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(AB_LOSS_VOL_OCCUR_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(AC_LOSS_VOL_OCCUR_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(BC_LOSS_VOL_OCCUR_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(ABC_LOSS_VOL_OCCUR_TIME,(void *)Pub_Buf0,4);
  
  Write_Storage_Data(TOTAL_LOSS_VOL_END_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(A_LOSS_VOL_END_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(B_LOSS_VOL_END_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(C_LOSS_VOL_END_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(AB_LOSS_VOL_END_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(AC_LOSS_VOL_END_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(BC_LOSS_VOL_END_TIME,(void *)Pub_Buf0,4);
  Write_Storage_Data(ABC_LOSS_VOL_END_TIME,(void *)Pub_Buf0,4);
  
  OS_Sem_Post(PUB_BUF0_SEM_ID); 

}





//初始化事件累加数据
void Init_Event_Cumu_Data()
{
  INT8U i;
  
  mem_set((void *)&Event_Cumu,0,sizeof(Event_Cumu),(void *)&Event_Cumu,sizeof(Event_Cumu));
  INIT_STRUCT_VAR(Event_Cumu); 
  //初始化事件标志
  mem_set((void *)&Event_Cumu.Event_Flag,0,sizeof(Event_Cumu.Event_Flag),(void *)&Event_Cumu.Event_Flag,sizeof(Event_Cumu.Event_Flag));
  SET_STRUCT_SUM(Event_Cumu.Event_Flag);
  //初始化分相累计
  for(i=0;i<S_NUM(Event_Cumu.Phase_Loss_Vol);i++)
    SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[i]);
  for(i=0;i<S_NUM(Event_Cumu.Phase_Loss_Cur);i++)
    SET_STRUCT_SUM(Event_Cumu.Phase_Loss_Cur[i]);
  for(i=0;i<S_NUM(Event_Cumu.Phase_Break);i++)
    SET_STRUCT_SUM(Event_Cumu.Phase_Break[i]);
  for(i=0;i<S_NUM(Event_Cumu.Phase_Reverse);i++)
    SET_STRUCT_SUM(Event_Cumu.Phase_Reverse[i]);
  //初始化失压失流累计
  SET_STRUCT_SUM(Event_Cumu.Loss_Vol);
  SET_STRUCT_SUM(Event_Cumu.Loss_Cur);
}



INT8U OS_Assert_Hook()
{
  INT8U Re;

  Re=CHECK_STRUCT_SUM(Event_Cumu.Phase_Loss_Vol[1]);

  return Re;
}
#undef EVENT_CUMU_C

