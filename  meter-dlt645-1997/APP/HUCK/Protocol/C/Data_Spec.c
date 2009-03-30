#define DATA_SPEC_C
#include <includes.h>

//预处理一些特殊数据项
INT16U Get_Freeze_Data(PROTO_DI DI,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr)
{
  INT16U Len;
  
  *pErr=NO_ERR;
  if(0xEF00==DI)
  {
    Len=Get_Period_Freeze_Data((S_Freeze_Data *)&Freeze_Data[0],(INT8U *)pDst,(INT8U *)pDst,DstLen);
    if(ASSERT(A_WARNING,Len==127))
      return 0;
    return Len-5;
  }
  else if(0xEF02==DI)
  {
    Len=Get_Period_Freeze_Data((S_Freeze_Data *)&Freeze_Data[1],(INT8U *)pDst,(INT8U *)pDst,DstLen);
    if(ASSERT(A_WARNING,Len==149))
      return 0;
    return Len-5;
  }
  else
    *pErr=PROTOCOL_DI_ERR;

  return 0;
}

//pSrc所指向的时间的前Bef_Mon个月的时间,返回的时间放到pDst中
//前几个月的时间计算不是以自然月为基准的，而是以结算时间点为月分介线的
void Calc_HIS_Data_Time(S_HEX_Time *pSrc,INT8U Bef_Mon,S_HEX_Time *pDst)
{
  //S_HEX_Time Temp_Time;
  //if(pSrc->Time[T_MIN])
  //先找到pSrc当月的冻结时间点
  Get_Next_Freeze_Time(pSrc,pDst);
  Time_Dec(pDst,Bef_Mon,UNIT_MON,pDst);//当月的冻结时间点

}

//计算组合有功电能
void Calc_Active_Comm_Energy(S_Energy *pEnergy)
{
  INT8U Re;
  INT8U i;
  
  Re=CHECK_STRUCT_SUM(Mode_Word);
  ASSERT(A_WARNING,1==Re);//不要Check_Data_Avail()，防止Get_DLT645_Data重入
  
  Re=Check_STRUCT_Sum(pEnergy,sizeof(S_Energy),pEnergy->CS,sizeof(pEnergy->CS));
  ASSERT(A_WARNING,1==Re);
  
  if(GET_BIT(Mode_Word.Mode[1].Byte,2))//有功正=正+反
  {
    for(i=0;i<=MAX_RATES;i++)//总有功以及分费率有功
      pEnergy->Pos_Active[i]=pEnergy->Pos_Active[i]+pEnergy->Neg_Active[i];

    for(i=0;i<3;i++)//三相有功
      pEnergy->Phase_Pos_Active[i]=pEnergy->Phase_Pos_Active[i]+pEnergy->Phase_Neg_Active[i];

    Set_STRUCT_Sum(pEnergy,sizeof(S_Energy),pEnergy->CS,sizeof(pEnergy->CS));
  }
}

//计算各种电量数据中的各种组合量
INT8U Calc_Reac_Comm_Energy(INT8U Type,S_Energy *pEnergy,INT8U Rate,void *pDst,void *pDst_Start,INT8U DstLen)
{
  INT32S S_Data=0;
  INT8U i;
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(Mode_Word);
  ASSERT(A_WARNING,1==Re);//不要Check_Data_Avail()，防止Get_DLT645_Data重入
  
  if(POS_REACTIVE==Type)//组合无功1，也就是正向无功
  {
    for(i=0;i<4;i++)//从1-4象限逐个计算
    {
      if(GET_BIT(Mode_Word.Mode[5].Byte,i))//该象限参与计算 
      {
        if(GET_BIT(Mode_Word.Mode[5].Byte,4+i)==0)//加
          S_Data+=pEnergy->Quad_Reactive[i][Rate];
        else
          S_Data-=pEnergy->Quad_Reactive[i][Rate];
      }
    }
  }
  else if(NEG_REACTIVE==Type)//组合无功2,也就是反向
  {
    for(i=0;i<4;i++)//从1-4象限逐个计算
    {
      if(GET_BIT(Mode_Word.Mode[6].Byte,i))//该象限参与计算 
      {
        if(GET_BIT(Mode_Word.Mode[6].Byte,4+i)==0)//加
          S_Data+=pEnergy->Quad_Reactive[i][Rate];
        else
          S_Data-=pEnergy->Quad_Reactive[i][Rate];
      }
    }    
  }
  else if(PHASE_A_POS_REACTIVE==Type || PHASE_B_POS_REACTIVE==Type || PHASE_C_POS_REACTIVE==Type)//ABC三相组合无功1
  {
    for(i=0;i<4;i++)//从1-4象限逐个计算
    {
      if(GET_BIT(Mode_Word.Mode[5].Byte,i))//该象限参与计算 
      {
        if(GET_BIT(Mode_Word.Mode[5].Byte,4+i)==0)//加
          S_Data+=pEnergy->Phase_Quad_Reactive[Type-PHASE_A_POS_REACTIVE][i];
        else
          S_Data-=pEnergy->Phase_Quad_Reactive[Type-PHASE_A_POS_REACTIVE][i];
      }
    }    
  }
  else if(PHASE_A_NEG_REACTIVE==Type || PHASE_B_NEG_REACTIVE==Type || PHASE_C_NEG_REACTIVE==Type)//ABC三相组合无功2
  {
    for(i=0;i<4;i++)//从1-4象限逐个计算
    {
      if(GET_BIT(Mode_Word.Mode[6].Byte,i))//该象限参与计算 
      {
        if(GET_BIT(Mode_Word.Mode[6].Byte,4+i)==0)//加
          S_Data+=pEnergy->Phase_Quad_Reactive[Type-PHASE_A_NEG_REACTIVE][i];
        else
          S_Data-=pEnergy->Phase_Quad_Reactive[Type-PHASE_A_NEG_REACTIVE][i];
      }
    }    
  }
  mem_cpy(pDst,&S_Data,sizeof(S_Data),pDst_Start,DstLen);
  return 4;  
}

//将标准645的当前和历史电量、需量数据的DI转化为自己扩充的DI
//就是将 0x9XXX、0xAXXX、0xBXXX转换为0x6XXX、0x7XXX、0x8XXX
INT16U Convert_645_HIS_DI(PROTO_DI DI,INT8U *pErr)
{
  INT16U TempDI;
  
  *pErr=NO_ERR;
  
  TempDI=0;
  if((DI & 0xF000)==0x9000)//读电量
    TempDI|=0x6000;
  else if((DI & 0xF000)==0xA000)//读需量
    TempDI|=0x7000;
  else if((DI & 0xF000)==0xB000)//读需量时间
    TempDI|=0x8000;
  else
  {
    *pErr=PROTOCOL_DI_ERR;
    return 0;
  }
  
  TempDI|=(DI & 0x0C00)>>2;//时间
  switch((DI & 0x03F0)>>4)//数据类型
  {
  case 0x01://正向有功
    TempDI|=0x0000;
    break;
  case 0x02://反向有功
    TempDI|=0x0020;
    break;
  case 0x11://组合无功1,正向无功
    TempDI|=0x0010;
    break;
  case 0x12://组合无功2,反向无功
    TempDI|=0x0030;
    break;
  case 0x13://一象限无功
    TempDI|=0x0040;
    break;
  case 0x14://四象限无功
    TempDI|=0x0050;
    break;
  case 0x15://二象限无功
    TempDI|=0x0060;
    break;
  case 0x16://三象限无功
    TempDI|=0x0070;
    break;
  default:
    *pErr=PROTOCOL_DI_ERR;
    return 0;
  }
  TempDI|=(DI & 0x000F);//费率号
  return TempDI; 
  
}

//获取历史的电压统计数据
INT16U Get_HIS_VolStat_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U *pErr)
{
  INT8U Bef_Mon,i;//前？月数据
  INT16U Len;
  S_HEX_Time Temp_Time;
  S_VolStat *pSrc;
  
  //读出历史电压统计数据
  if(DI>=0xB220 && DI<=0xB224)
  {
    if(DI>=0xB222 && DI<=0xB224)//上1-3月
    {
      Bef_Mon=(INT8U)(DI-0xB221);//读前几月数据？
      Calc_HIS_Data_Time((S_HEX_Time *)&Cur_Time0,Bef_Mon,&Temp_Time);
      Len=Read_HIS_Data(HIS_VOLSTAT_DATA,&Temp_Time,(void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),pErr);
      if(sizeof(S_VolStat)!=Len || NO_ERR!=*pErr)//没有读出来
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Read_HIS_Data return len=%d,Err_Flag=%d",Len,*pErr);
        mem_set((void *)Pub_Buf,0,sizeof(S_VolStat),(void *)Pub_Buf,sizeof(Pub_Buf));
      } 
      
      pSrc=(S_VolStat *)Pub_Buf;
    }
    else if(DI==0xB221)//本月    
      pSrc=&Cur_VolStat;
    else if(DI==0xB220)//总数据
      pSrc=&Total_VolStat;
    
    //三相电压检测数据
    for(i=0;i<3;i++)
    {
      Hex2Bcd(pSrc->Total_Time[i],pDst+i*12,4,pDst_Start,DstLen);//检测总时间
      Hex2Bcd(pSrc->Upper_Time[i],pDst+i*12+4,4,pDst_Start,DstLen);//超上时间
      Hex2Bcd(pSrc->Lower_Time[i],pDst+i*12+8,4,pDst_Start,DstLen);//超下时间
    }
    *pErr=NO_ERR;
    return 36;
  }
  //电压合格率数据,本月->上3月
  else if((DI>=0xE501 && DI<=0xE503) ||\
          (DI>=0xE511 && DI<=0xE513) ||\
          (DI>=0xE521 && DI<=0xE523) ||\
          (DI>=0xE531 && DI<=0xE533))
  {
    Bef_Mon=((DI & 0x00F0)>>4);

    if(Bef_Mon==0)//当月
      pSrc=(S_VolStat *)&Cur_VolStat;
    else
    {
      //Time_Dec((S_HEX_Time *)&Cur_Time0,Bef_Mon,UNIT_MON,(S_HEX_Time *)&Temp_Time);//将时间向前推移Bef_Mon个月
      Calc_HIS_Data_Time((S_HEX_Time *)&Cur_Time0,Bef_Mon,&Temp_Time);
      Len=Read_HIS_Data(HIS_VOLSTAT_DATA,&Temp_Time,(void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),pErr);
      if(sizeof(S_VolStat)!=Len || NO_ERR!=*pErr)//没有读出来
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Read_HIS_Data return len=%d,Err_Flag=%d",Len,*pErr);
        mem_set((void *)Pub_Buf,0,sizeof(S_VolStat),(void *)Pub_Buf,sizeof(Pub_Buf));
      }
      
      pSrc=(S_VolStat *)Pub_Buf;      
    }
    
    i=(DI & 0x000F)-1;//A\B\C三相索引0-2
    
    Hex2Bcd(pSrc->Qual[i],pDst,3,pDst_Start,DstLen);    
    *pErr=NO_ERR;
    return 3;    
  }
  return 0;
}

//获取当前->上HIS_ENERGY_DATA_MONTHS的历史数据
INT16U Get_HIS_Energy_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pErr)
{
  INT8U *pSrc,Bef_Mon,Comm_Flag;//Comm_Flag表示组合无功标志，1表示是组合无功数据
  INT32U Data;
  INT32S S_Data;//有符号数
  S_HEX_Time Temp_Time;
  INT16U Len;

  //读取上0->上HIS_ENERGY_DATA_MONTHS月电量数据
  Comm_Flag=0;
  if((DI & 0xF000)==0x6000) //|| (DI & 0xF000)==0x7000 || (DI & 0xF000)==0x8000)
  {
    if(ASSERT(A_WARNING,((DI & 0x0F00)>>8)<=HIS_ENERGY_DATA_MONTHS &&\
                        ((DI & 0x00F0)>>4)<=7 &&\
                        (DI & 0x000F)<=MAX_RATES))
    {
      *pErr=PROTOCOL_DI_ERR;
      return 0;
    }
    //////////////////////从ROM中读取历史电量数据、或从RAM中读取当前数据////////////////////
    Bef_Mon=(INT8U)((DI & 0x0F00)>>8);//读前几月数据？
    //Time_Dec((S_HEX_Time *)&Cur_Time0,Bef_Mon,UNIT_MON,(S_HEX_Time *)&Temp_Time);//将时间向前推移Bef_Mon个月
    //读历史数据
    if(Bef_Mon!=0)//非本月
    {
      Calc_HIS_Data_Time((S_HEX_Time *)&Cur_Time0,Bef_Mon,&Temp_Time);
      Len=Read_HIS_Data(HIS_ENERGY_DATA,&Temp_Time,(void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),pErr);
      if(ENERGY_SAVE_SIZE!=Len || NO_ERR!=*pErr)
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Read_HIS_Data return len=%d,Err_Flag=%d",Len,*pErr);
        if(NO_ERR==*pErr)
          *pErr=PROTOCOL_DATA_ERR;
        return 0;
      }
      pSrc=(INT8U *)Pub_Buf;
    }
    else//当前数据，直接从内存中取
    {
      mem_cpy((void *)Pub_Buf,(void *)&Cur_Energy,sizeof(Cur_Energy),(void *)Pub_Buf,sizeof(Pub_Buf));
      pSrc=(INT8U *)Pub_Buf;
    }
    
    Calc_Active_Comm_Energy((S_Energy *)pSrc);//计算组合有功
      
    switch(((DI & 0x00F0)>>4))
    {
      case 0:
        pSrc+=S_OFF(S_Energy,Pos_Active[0]);//正向有功
        break;
      case 1:
        Calc_Reac_Comm_Energy(POS_REACTIVE,(S_Energy *)pSrc,(INT8U)(DI & 0x000F),&S_Data,&S_Data,sizeof(S_Data));//组合无功1
        Comm_Flag=1;
        break;
      case 2:
        pSrc+=S_OFF(S_Energy,Neg_Active[0]);//反向有功
        break;
      case 3:
        Calc_Reac_Comm_Energy(NEG_REACTIVE,(S_Energy *)pSrc,(INT8U)(DI & 0x000F),&S_Data,&S_Data,sizeof(S_Data));//组合无功2
        Comm_Flag=1;
        break;
      case 4:
        pSrc+=S_OFF(S_Energy,Quad_Reactive[0][0]);//一象限无功
        break;
      case 5:
        pSrc+=S_OFF(S_Energy,Quad_Reactive[3][0]);//四象限无功
        break;
      case 6:
        pSrc+=S_OFF(S_Energy,Quad_Reactive[1][0]);//二象限无功
        break;
      case 7:
        pSrc+=S_OFF(S_Energy,Quad_Reactive[2][0]);//三象限无功
        break;
      default:
        *pErr=PROTOCOL_DI_ERR;
        return 0;
    }
    if(0==Comm_Flag)
      pSrc+=(DI & 0x000F)*4;//计算费率偏移
  }
  ///////////////////////分相历史电量数据////////////////////
  else if((DI & 0xFF00)==0xE700 && ((DI & 0x00F0)>>4)<=HIS_ENERGY_DATA_MONTHS)
  {
    Bef_Mon=(INT8U)((DI & 0x00F0)>>4);//读前几月数据？
    
    if(Bef_Mon==0)//本月
    {
      mem_cpy((void *)Pub_Buf,(void *)&Cur_Energy,sizeof(Cur_Energy),(void *)Pub_Buf,sizeof(Pub_Buf));
      pSrc=(INT8U *)Pub_Buf;
    }
    else//非本月
    {
      Calc_HIS_Data_Time((S_HEX_Time *)&Cur_Time0,Bef_Mon,&Temp_Time);
      Len=Read_HIS_Data(HIS_ENERGY_DATA,&Temp_Time,(void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),pErr);
      if(ENERGY_SAVE_SIZE!=Len || NO_ERR!=*pErr)
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Read_HIS_Data return len=%d,Err_Flag=%d",Len,*pErr);
        if(NO_ERR==*pErr)
          *pErr=PROTOCOL_DATA_ERR;
        return 0;
      }
      
      pSrc=(INT8U *)Pub_Buf;      
    }

    Calc_Active_Comm_Energy((S_Energy *)pSrc);//计算组合有功
    
    switch(DI & 0x000F)
    {
    case 1:
      pSrc+=S_OFF(S_Energy,Phase_Pos_Active[0]);//A相正有
      break;
    case 2:
      pSrc+=S_OFF(S_Energy,Phase_Pos_Active[1]);//B相正有
      break;
    case 3:
      pSrc+=S_OFF(S_Energy,Phase_Pos_Active[2]);//C相正有
      break;
    case 4:
      Calc_Reac_Comm_Energy(PHASE_A_POS_REACTIVE,(S_Energy *)pSrc,(INT8U)(DI & 0x000F),&S_Data,&S_Data,sizeof(S_Data));////A相正无
      Comm_Flag=1;
      break;
    case 5:
      Calc_Reac_Comm_Energy(PHASE_B_POS_REACTIVE,(S_Energy *)pSrc,(INT8U)(DI & 0x000F),&S_Data,&S_Data,sizeof(S_Data));////B相正无
      Comm_Flag=1;
      break;
    case 6:
      Calc_Reac_Comm_Energy(PHASE_C_POS_REACTIVE,(S_Energy *)pSrc,(INT8U)(DI & 0x000F),&S_Data,&S_Data,sizeof(S_Data));////C相正无
      Comm_Flag=1;
      break;
    case 7:
      pSrc+=S_OFF(S_Energy,Phase_Neg_Active[0]);//A相反有
      break;
    case 8:
      pSrc+=S_OFF(S_Energy,Phase_Neg_Active[1]);//B相反有
      break;
    case 9:
      pSrc+=S_OFF(S_Energy,Phase_Neg_Active[2]);//C相反有
      break;
    case 0x0A:
      Calc_Reac_Comm_Energy(PHASE_A_NEG_REACTIVE,(S_Energy *)pSrc,(INT8U)(DI & 0x000F),&S_Data,&S_Data,sizeof(S_Data));////A相反无
      Comm_Flag=1;
      break;
    case 0x0B:
      Calc_Reac_Comm_Energy(PHASE_B_NEG_REACTIVE,(S_Energy *)pSrc,(INT8U)(DI & 0x000F),&S_Data,&S_Data,sizeof(S_Data));////B相反无
      Comm_Flag=1;
      break;
    case 0x0C:
      Calc_Reac_Comm_Energy(PHASE_C_NEG_REACTIVE,(S_Energy *)pSrc,(INT8U)(DI & 0x000F),&S_Data,&S_Data,sizeof(S_Data));////C相反无
      Comm_Flag=1;
      break;
    default:
      *pErr=PROTOCOL_DI_ERR;
      return 0;
    }    
  }
  
  ////////////////////////////////将数据转换为BCD码///////////////////////////////////////////  
  if((FOR_DISP & Data_Flag)==FOR_DISP) //显示电能小数位固定为3//&& 3==Get_Energy_Disp_Digs())//显示电能小数位数为3
  {
    if(0==Comm_Flag)//不是无功组合电能数据
    {
      mem_cpy(&Data,pSrc,4,&Data,sizeof(Data));//Data=*(INT32U *)pSrc;
      Hex2Bcd(Data,pDst,5,pDst_Start,DstLen);//转换出BCD数
    }
    else
      S_Hex2Bcd(S_Data,pDst,5,pDst_Start,DstLen);
    
    *pErr=NO_ERR;
    return 5;    
  }
  else//显示和协议小数位一致,都是2位
  {
    if(0==Comm_Flag)//不是无功组合电能数据
    {
      if((FOR_SRC & Data_Flag)==FOR_SRC)//获取源数据
        mem_cpy(pDst,pSrc,4,pDst_Start,DstLen);
      else
      {
        mem_cpy(&Data,pSrc,4,&Data,sizeof(Data));
        Data=Data/10;//原数据是3位小数，协议置需2位，故/10
        Hex2Bcd(Data,pDst,4,pDst_Start,DstLen);//转换出BCD数
      }      
    }
    else
    {
      if((FOR_SRC & Data_Flag)==FOR_SRC)//获取源数据
        mem_cpy(pDst,&S_Data,4,pDst_Start,DstLen);
      else
      {
        S_Data=S_Data/10;
        S_Hex2Bcd(S_Data,pDst,4,pDst_Start,DstLen);//转换出BCD数      );
      }
    }
    
    *pErr=NO_ERR;
    return 4;    
  }

}

//获取历史需量数据
//DI数据标识
//pDst目标缓冲区
//pDst_Start和DstLen一起限定返回值长度和pDst
//Data_Flag表示是否给显示
//pErr错误标识字,当返回为0时用
//返回读取到的数据长度
INT16U Get_HIS_Demand_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pErr)
{
  INT8U *pSrc,Bef_Mon,Re,Rate;
  INT32U Off,Data;
  S_HEX_Time Temp_Time;
  INT16U Len;
  //读取上0->上HIS_ENERGY_DATA_MONTHS月电量数据
  if((DI & 0xF000)==0x7000 || (DI & 0xF000)==0x8000)
  {
    if((DI & 0xF000)==0x7000 || (DI & 0xF000)==0x8000)
    {
      if(ASSERT(A_WARNING,((DI & 0x0F00)>>8)<=HIS_DEMAND_DATA_MONTHS &&\
                          ((DI & 0x00F0)>>4)<=7 &&\
                          (DI & 0x000F)<=MAX_RATES))
      {
        *pErr=PROTOCOL_DI_ERR;
        return 0;
      }
    }
/*
    if((DI & 0xF000)==0x7000)
      Storage_DI=HIS_DEMAND;//需量
    else
      Storage_DI=HIS_DEMAND_TIME;//需量时间
*/    
    //////////////////////从ROM中读取历史电量数据、或从RAM中读取当前数据////////////////////
    
    Bef_Mon=(INT8U)((DI & 0x0F00)>>8);//读前几月数据？
    Rate=(INT8U)(DI & 0x000F);//读取哪个费率的数据?

    //Time_Dec((S_HEX_Time *)&Cur_Time0,Bef_Mon,UNIT_MON,(S_HEX_Time *)&Temp_Time);//将时间向前推移Bef_Mon个月

    //读历史数据
    if(Bef_Mon!=0)//非本月
    {
      Calc_HIS_Data_Time((S_HEX_Time *)&Cur_Time0,Bef_Mon,&Temp_Time);
      Len=Read_Demand_HIS_Data(Rate,&Temp_Time,(void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),pErr);
      if(ONE_DEMAND_SAVE_SIZE!=Len || NO_ERR!=*pErr)
      {
        DEBUG_PRINT(HUCK,DEBUG_0,"Read_Demand_HIS_Data return len=%d,Err_Flag=%d",Len,*pErr);
        //没有读到，则默认需量为0，需量时间也为0
        mem_set((void *)Pub_Buf,0,sizeof(S_One_Demand),(void *)Pub_Buf,sizeof(Pub_Buf));
      }
      pSrc=(INT8U *)Pub_Buf;
      
      //如果读出的数据时间不符合，则默认费率最大需量为0,需量发生时间也为0
      if(((S_One_Demand *)pSrc)->Time.Time[T_YEAR]!=Temp_Time.Time[T_YEAR] ||\
         ((S_One_Demand *)pSrc)->Time.Time[T_MONTH]!=Temp_Time.Time[T_MONTH])
      {
        //没有读到，则默认需量为0，需量时间也为0
         DEBUG_PRINT(HUCK,DEBUG_0,"Demand Data Time error:%d-%d != %d-%d",\
           ((S_One_Demand *)pSrc)->Time.Time[T_YEAR],((S_One_Demand *)pSrc)->Time.Time[T_MONTH],\
           Temp_Time.Time[T_YEAR],Temp_Time.Time[T_MONTH]);
         
         mem_set((void *)pSrc,0,sizeof(S_One_Demand),(INT8U *)Pub_Buf,sizeof(Pub_Buf));
      }      
    }
    else//当前数据
    {
      if(Rate!=Cur_Demand.Rate && Rate!=0)//非当前费率也非总，则数据不在RAM内存中
      {
        Off=DEMAND_RATE_OFF(Rate);
        //先读取当前需量数据区，读取不到则读取当前需量备份数据区
        Len=Read_Storage_Data_Fix_Len(CUR_DEMAND,Off,ONE_DEMAND_SAVE_SIZE,\
                                      (void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),pErr);
        if(ASSERT(A_WARNING,0!=Len && NO_ERR==*pErr))
        {
          //主存储区没有读到则读备份存储区
          Len=Read_Storage_Data_Fix_Len(CUR_DEMAND_BAK,Off,ONE_DEMAND_SAVE_SIZE,\
                                        (void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),pErr);
          if(ASSERT(A_WARNING,0!=Len && NO_ERR==*pErr))
          {
            //没有读到，则默认需量为0，需量时间也为0
            mem_set((void *)Pub_Buf,0,sizeof(S_One_Demand),(void *)Pub_Buf,sizeof(Pub_Buf));
          }
        }
        pSrc=(INT8U *)Pub_Buf;
      }
      else if(Rate==0)//总最大需量数据在内存中
      {
        mem_cpy((void *)Pub_Buf,(INT8U *)&Cur_Demand.Demand[0],sizeof(S_One_Demand),(void *)Pub_Buf,sizeof(Pub_Buf));
        pSrc=(INT8U *)Pub_Buf;//总需量缓冲区
      }
      else//当前费率需量数据也在内存中
      {
        mem_cpy((void *)Pub_Buf,(INT8U *)&Cur_Demand.Demand[1],sizeof(S_One_Demand),(void *)Pub_Buf,sizeof(Pub_Buf));
        pSrc=(INT8U *)Pub_Buf;//总需量缓冲区        
      }
    }


    if((DI & 0xF000)==0x7000)//需量数据
    {
      switch(((DI & 0x00F0)>>4))
      {
      case 0:
        pSrc+=S_OFF(S_One_Demand,Pos_Active);//正向有功
        break;
      case 1:
        pSrc+=S_OFF(S_One_Demand,Pos_Reactive);//组合无功1
        break;
      case 2:
        pSrc+=S_OFF(S_One_Demand,Neg_Active);//反向有功
        break;
      case 3:
        pSrc+=S_OFF(S_One_Demand,Neg_Reactive);//组合无功2
        break;
      case 4:
        pSrc+=S_OFF(S_One_Demand,Quad_Reactive);//一象限无功
        break;
      case 5:
        pSrc+=S_OFF(S_One_Demand,Quad_Reactive[3]);//四象限无功
        break;
      case 6:
        pSrc+=S_OFF(S_One_Demand,Quad_Reactive[1]);//二象限无功
        break;
      case 7:
        pSrc+=S_OFF(S_One_Demand,Quad_Reactive[2]);//三象限无功
        break;
      default:
        *pErr=PROTOCOL_DI_ERR;
        return 0;
      }      
      //pSrc+=(DI & 0x000F)*4;//计算费率偏移    
    }
    else//需量时间数据
    {
      switch(((DI & 0x00F0)>>4))
      {
      case 0:
        pSrc+=S_OFF(S_One_Demand,Pos_Active_Time[0]);//正向有功
        break;
      case 1:
        pSrc+=S_OFF(S_One_Demand,Pos_Reactive_Time[0]);//组合无功1
        break;
      case 2:
        pSrc+=S_OFF(S_One_Demand,Neg_Active_Time[0]);//反向有功
        break;
      case 3:
        pSrc+=S_OFF(S_One_Demand,Neg_Reactive_Time[0]);//组合无功2
        break;
      case 4:
        pSrc+=S_OFF(S_One_Demand,Quad_Reactive_Time[0][0]);//一象限无功
        break;
      case 5:
        pSrc+=S_OFF(S_One_Demand,Quad_Reactive_Time[3][0]);//四象限无功
        break;
      case 6:
        pSrc+=S_OFF(S_One_Demand,Quad_Reactive_Time[1][0]);//二象限无功
        break;
      case 7:
        pSrc+=S_OFF(S_One_Demand,Quad_Reactive_Time[2][0]);//三象限无功
        break;
      default:
        *pErr=PROTOCOL_DI_ERR;
        return 0;
      }       
    }
  }
   
  ////////////////////////////////将数据转换为BCD码///////////////////////////////////////////  
  if((DI & 0xF000)==0x7000)//需量数据
  {
    mem_cpy(&Data,pSrc,4,&Data,sizeof(Data));
    Hex2Bcd(Data,pDst,3,pDst_Start,DstLen);//转换出BCD数

    *pErr=NO_ERR;
    return 3;    
    
  }
  else//需量时间
  {
    Re=CHECK_STRUCT_SUM(Mode_Word);
    if(ASSERT(A_WARNING,0!=Re))
      Check_Data_Avail();
    
    if(Mode_Word.Mode[7].Bit.Bit6 EQ 0 ||\
      (Data_Flag & FOR_DISP)==FOR_DISP ||\
      (Data_Flag & FOR_EVENT)==FOR_EVENT)//根据模式字判断需量是否带年,或者数据给显示和事件的话，要带年
    {
      mem_cpy(pDst,pSrc,5,pDst_Start,DstLen);
      *pErr=NO_ERR;
      return 5;
    }
    else
    {
      mem_cpy(pDst,pSrc,4,pDst_Start,DstLen);
      *pErr=NO_ERR;
      return 4;
    }
  }  
}
//DI必须是节假日日0xC411->0xC41D或者0xCC11->0xCC1D
//*pDst目标缓冲区
//*pDst_Start目标缓冲区的起始地址
//DstLen和*pDst_Start一起限制*pDst
//注意:该函数内部使用了Pub_Buf变量
//函数外部必须OS_Sem_Pend和OS_Sem_Post两个函数
INT16U Get_Holiday_Spec_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U *pErr)
{
  if(DI>=0xC411 && DI<=0xC41D)
  {
    if(Read_Storage_Data(0xC41F,(void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),pErr)==45)
    {
      mem_cpy(pDst,(INT8U *)Pub_Buf+(DI-0xC411)*3,3,pDst_Start,DstLen);
      return 3;
    }
    else
    {
      if(NO_ERR==*pErr)
        *pErr=PROTOCOL_DATA_ERR;        
      return 0;
    }
  }
  else if(DI>=0xCC11 && DI<=0xCC1D)
  {
    if(Read_Storage_Data(0xCC1F,(void *)Pub_Buf,(void *)Pub_Buf,sizeof(Pub_Buf),pErr)==45)
    {
      mem_cpy(pDst,(INT8U *)Pub_Buf+(DI-0xCC11)*3,3,pDst_Start,DstLen);
      return 3;
    }
    else
    {
      if(NO_ERR==*pErr)
        *pErr=PROTOCOL_DATA_ERR;        
      return 0;
    }   
  }  
  *pErr=PROTOCOL_DI_ERR;        
  return 0; 
}
/*
  {0xB710,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,4,S_INTU,0,4,0x1117,SPEC_CUMU_EVENT},	
  {0xB720,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,4,S_INTU,0,4,0x1116,SPEC_CUMU_EVENT},
  {0xB730,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,4,S_INTU,0,4,0x1114,SPEC_CUMU_EVENT},
  {0xB740,DI_EQU_NULL,1,1,PSW_RD_ONLY,S_RAM,(void *)0,0,4,S_INTU,0,4,0x1115,SPEC_CUMU_EVENT},
*/


INT16U Get_MultiTimes_Spec_Event_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen)
{
  if(0xEB90==DI)
  {
    if(Read_Event_Status(ID_EVENT_PROG))
    {
      if(!(pDst>=pDst_Start && pDst+7<=pDst_Start+DstLen))
      {
        ASSERT(A_WARNING,0);
        return 0;
      }
      
      mem_cpy(pDst,(INT8U *)Prog_Record.Time,5,pDst_Start,DstLen);
      mem_cpy(pDst+5,(INT8U *)Event_Cumu_Data.Prog_Flag.Flag,2,pDst_Start,DstLen);
      return 7;
    }
  }

  return 0;
}

INT16U Get_Spec_Multi_Event_Data(PROTO_DI DI, INT8U *pDst, INT8U *pDst_Start, INT16U DstLen)
{
  
  return 0;
}
//分次事件记录数据的读取
INT16U Get_MultiTimes_Event_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U *pErr)
{
  INT16U i,j,Len,Index;
  STORA_DI Storage_DI;
  INT8U Event_Flag;
  
  //获取数据的Index
  *pErr=NO_ERR;
  
  if(DI >= 0xEBE0 && DI <= 0xEBE9)//校时记录同时支持0xEBE0->0xEBE9和0xE400->0xE499
    DI = 0xE400 + (DI - 0xEBE0);
  
  i=Get_Event_Data_Index_From_DI(DI);
  if(NULL_2BYTES==i)
  {
    *pErr=PROTOCOL_DI_ERR;//表示不是分次事件的ID
    return 0;
  }
  
  Len=Get_MultiTimes_Spec_Event_Data(DI,pDst,pDst_Start,DstLen);
  if(Len>0)
    return Len;

  //读取最近一条，且当前事件在发生,且是差量类型的事
  if(Event_Data_Info[i].Storage_DI==DI && Check_Event_Occuring(Event_Data_Info[i].Event_ID) && Event_Data_Info[i].pData -> Diff_Mask != 0)
  {
    OS_Sem_Pend(PUB_BUF0_SEM_ID);
    Len=Event_MultiTimes_Proc(Event_Data_Info[i].Event_ID,EVENT_END,EVENT_VIRTRUAL);//事件还没有真正结束，虚拟一次结束
    if(Len > 1)
      Len --;
    
    mem_cpy(pDst,(INT8U *)Pub_Buf0+1,Len,pDst_Start,DstLen);
    OS_Sem_Post(PUB_BUF0_SEM_ID);
    return Len;
  }
  
  if(Event_Data_Info[i].Storage_Num==1)//ONE_RECORD_EVENT)
  {
    Len = Read_Storage_Data(Event_Data_Info[i].Storage_DI,pDst,pDst_Start,DstLen,pErr);
    if(Len > 0)
      Len --;
    
    mem_cpy(pDst, pDst + 1, Len, pDst_Start, DstLen);
    return Len;
  }
  else
  {
    //先读出最近一次事件的存储DI到Storage_DI
    Len=Read_Storage_Data(Event_Data_Info[i].Last_Storage_DI,pDst,pDst_Start,DstLen,pErr);
    if(ASSERT(A_WARNING,Len==sizeof(STORA_DI) && *pErr==NO_ERR)) //没有读取到上次发生事件的记录
    {
      //*pErr=EVENT_DATA_ERR;
      return 0;
    }
    else
      mem_cpy(&Storage_DI,pDst,4,&Storage_DI,sizeof(Storage_DI));//Storage_DI=*(INT32U *)pDst;//读取到的4个字节是上次存储事件的存储DI
    
    if(NULL_EVENT_STORAGE_DI == Storage_DI)//还没有生成过事件
    {
      mem_set(pDst,0,Event_Data_Info[i].pData->Data_Len,pDst_Start,DstLen);
      return Event_Data_Info[i].pData->Data_Len - 1;
    }
    
    //Storage_DI认为是最后一次存储数据的DI
    if(!(Storage_DI>=Event_Data_Info[i].Storage_DI &&\
      Storage_DI<Event_Data_Info[i].Storage_DI+Event_Data_Info[i].Storage_Num))
    {
      //*pErr=EVENT_DATA_ERR;
      return 0;
    }
    
    if(Event_Data_Info[i].Storage_Num>10)
    {
      DI=Event_Data_Info[i].Storage_DI+Bcd2Hex_Byte((INT8U)(DI & 0xFF))-Bcd2Hex_Byte((INT8U)(Event_Data_Info[i].Storage_DI) & 0xFF);
      //DI=(DI & 0xFF00)+Bcd2Hex_Byte((INT8U)(DI & 0xFF));//地位字节转化为
    }
    //DI转换成最近第几次的事件数据?
    //Proto_DI是最近一次的DI
    if(DI>=Event_Data_Info[i].Storage_DI && DI<Event_Data_Info[i].Storage_DI+Event_Data_Info[i].Storage_Num)
      DI=DI-Event_Data_Info[i].Storage_DI;
    else
    {
      //*pErr=PROTOCOL_DI_ERR;
      return 0;
    }
    
    if((Storage_DI-DI)<Event_Data_Info[i].Storage_DI)
      Storage_DI=Event_Data_Info[i].Storage_Num+Storage_DI-DI;
    else
      Storage_DI=Storage_DI-DI;
    
    Len = Read_Storage_Data(Storage_DI,pDst,pDst_Start,DstLen,pErr);
    if(0 == Len)
      return 0;
    
    Len --;
    Event_Flag = *pDst;//事件发生或者结束标志
    mem_cpy(pDst, pDst + 1, Len, pDst_Start, DstLen);//去掉第一个字节的事件发生或者结束标志

    if(Event_Data_Info[i].pData->Diff_Mask!=0 && Event_Flag!=EVENT_END)//差量事件，但是没有结束事件,应该将差量数据清0
    {
       DEBUG_PRINT(HUCK,DEBUG_0,"Read Event Data, Event_Flag=0x%x!=EVENT_END",Event_Flag);
       ASSERT(A_WARNING,0);
       Len = 0;
       for(j = 0; j < Event_Data_Info[i].pData -> Data_Num; j ++)
       {
         Index = Get_Protocol_Data_Index(Event_Data_Info[i].pData -> DI[j]);
         if(Index == NULL_2BYTES)
         {
           ASSERT(A_WARNING,0);
           return 0;
         }
         //DEBUG_PRINT(HUCK,DEBUG_0,"Event %d, DataLen=%d", i, Protocol_Data[Index].Dst_Len);
         if(GET_BIT(Event_Data_Info[i].pData -> Diff_Mask,j))//第i个数据是差量数据，则清零之 
           mem_set(pDst + Len, 0, Protocol_Data[Index].Dst_Len, pDst_Start, DstLen); 
         Len += Protocol_Data[Index].Dst_Len; 
         
         if(Protocol_Data[Index].Dst_Len == 0)
           ASSERT(A_WARNING,0);
       } 
    }
  }
  return Len;
}
//获取645数据里面数据项，需要特殊处理的项，主要是电量、需量、和需量时间等
//DI数据标识符
//pPara入口参数，主要是S_HEX_Time类型参数，即历史时间
//pDst目标缓冲区
//pDst_Start和DstLen一起限定pDst和返回长度
//Data_Flag,FOR_DISP或FOR_COMM，表示该函数提供给显示还是通信
//函数返回1表示读取成功
//函数返回0且NO_ERR==*pErr表示该数据不是特殊数据项
//函数返回0且*pErr!=NO_ERR表示该数据是特殊数据项,但是获取有错误
INT16U Get_DLT645_Spec_Single_Data(PROTO_DI DI,INT8U Spec_Flag,void *pPara,INT8U ParaLen,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U Data_Flag,INT8U *pErr)
{
  INT16U Len;
  INT8U Re;
  INT32U Temp;
  S_BCD_Time TempTime;
  
  *pErr=NO_ERR;  
  //读电量、需量、需量时间数据项
  //将0x9xxx,0xAxxx和0xBxxx映射到0x6xxx、0x7xxx和0x8xxx上去
  if(((DI & 0xF000)==0x9000 || (DI & 0xF000)==0xA000 || (DI & 0xF000)==0xB000) &&\
    ((DI & 0x0C00)==0x0000 || (DI & 0x0C00)==0x0400 || (DI & 0x0C00)==0x0800) &&\
    ((DI & 0x0300)==0x0000 || (DI & 0x0300)==0x0100))
  {
    //DEBUG_PRINT(HUCK,DEBUG_0,"Get_Spec_Data Bef_Convert DI=%x\r\n",DI);
    DI=Convert_645_HIS_DI(DI,pErr);//转换到0x6000、0x7000、0x8000
    if(*pErr!=NO_ERR)
    {
      *pErr=PROTOCOL_DI_ERR;
      return 0;
    }
  }
  
  //获取当月->上HIS_ENERGY_DATA_MONTHS月历史数据
  if((DI & 0xF000)==0x6000 ||\
    ((DI & 0xFF00)==0xE700 && ((DI & 0x00F0)>>4)<=HIS_ENERGY_DATA_MONTHS))//分相电能
  { 
    //DEBUG_PRINT(HUCK,DEBUG_0,"Get_HIS_Energy_Data DI=%x\r\n",DI);
    Len=Get_HIS_Energy_Data(DI,pDst,pDst_Start,DstLen,Data_Flag,pErr);
    if(ASSERT(A_WARNING,Len>0))
    {
      *pErr=PROTOCOL_DATA_ERR;
      return 0;
    }
    else
    {
      *pErr=NO_ERR;
      return Len;      
    }
  }
  else if((DI & 0xF000)==0x7000 || (DI & 0xF000)==0x8000)//历史需量
  { 
    //DEBUG_PRINT(HUCK,DEBUG_0,"Get_HIS_Demand_Data DI=%x\r\n",DI);
    Len=Get_HIS_Demand_Data(DI,pDst,pDst_Start,DstLen,Data_Flag,pErr);
    if(ASSERT(A_WARNING,Len>0))
    {
      *pErr=PROTOCOL_DATA_ERR;
      return 0;
    }
    else
    {
      *pErr=NO_ERR;
      return Len;      
    }
  }
  else if((DI>=0xC411 && DI<=0xC41D) || (DI>=0xCC11 && DI<=0xCC1D))//节假日特殊处理,两套费率方案的前1-13个节假日需要特殊处理
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Get_Holiday_Spec_Data DI=%x\r\n",DI);
    Len=Get_Holiday_Spec_Data(DI,pDst,pDst_Start,DstLen,pErr);      
    if(ASSERT(A_WARNING,Len>0))
    {
      *pErr=PROTOCOL_DATA_ERR;
      return 0;
    }
    else
    {
      *pErr=NO_ERR;
      return Len;      
    }
  }
  else if((DI>=0xB220 && DI<=0xB224) || (DI>=0xE501 && DI<=0xE533))//读出历史电压统计数据
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Get_HIS_VolStat_Data DI=%x\r\n",DI);
    Len=Get_HIS_VolStat_Data(DI,pDst,pDst_Start,DstLen,pErr);      
    if(ASSERT(A_WARNING,Len>0))
    {
      *pErr=PROTOCOL_DATA_ERR;
      return 0;
    }
    else
    {
      *pErr=NO_ERR;
      return Len;     
    } 
  }
  else if(DI_ACTIVE_DIRECT==DI || DI_REACTIVE_DIRECT==DI)  //负荷曲线需要的有无功方向数据
  {
    if(DI==DI_ACTIVE_DIRECT)
     *(INT8U *)pDst=(Meter_Run_Status.Meter_Stat2.Byte & 0x0F);
    else if(DI==DI_REACTIVE_DIRECT)
     *(INT8U *)pDst=(Meter_Run_Status.Meter_Stat2.Byte & 0xF0)>>4; 
    *pErr=NO_ERR;
    return 1;
  }
  else if(DI>=0xD110 && DI<=0xD115)  //读负荷曲线
  {
    Len=Read_Load_Data(DI,(INT8U *)pPara,ParaLen,pDst,pDst_Start,DstLen,pErr);
    return Len;
  }
  else if(DI_CUR_TIME==DI)//当前BCD时间
  {
    ASSERT(A_WARNING,pDst>=pDst_Start && pDst+5<=pDst_Start+DstLen);
    //此处不能直接读取内部时钟，因为上电处理掉电前事件时会将掉电前时间复制到Cur_Time1
    *(INT8U *)pDst=Cur_Time1.Min;//Hex2Bcd_Byte(Cur_Time0.Time[T_MIN);
    *((INT8U *)pDst+1)=Cur_Time1.Hour;//Hex2Bcd_Byte(Cur_Time0.Time[T_HOUR);
    *((INT8U *)pDst+2)=Cur_Time1.Date;//Hex2Bcd_Byte(Cur_Time0.Time[T_DATE);
    *((INT8U *)pDst+3)=Cur_Time1.Month;//Hex2Bcd_Byte(Cur_Time0.Time[T_MONTH);
    *((INT8U *)pDst+4)=Cur_Time1.Year;//Hex2Bcd_Byte(Cur_Time0.Time[T_YEAR);
    return 5;
  }
  else if(DI_EVENT_TIME==DI)//事件发生BCD时间
  {
    ASSERT(A_WARNING,pDst>=pDst_Start && pDst+5<=pDst_Start+DstLen);
    //此处不能直接读取内部时钟，因为上电处理掉电前事件时会将掉电前时间复制到Cur_Time1
    *(INT8U *)pDst=Event_Cumu_Data.Time.Min;//Hex2Bcd_Byte(Cur_Time0.Time[T_MIN);
    *((INT8U *)pDst+1)=Event_Cumu_Data.Time.Hour;//Hex2Bcd_Byte(Cur_Time0.Time[T_HOUR);
    *((INT8U *)pDst+2)=Event_Cumu_Data.Time.Date;//Hex2Bcd_Byte(Cur_Time0.Time[T_DATE);
    *((INT8U *)pDst+3)=Event_Cumu_Data.Time.Month;//Hex2Bcd_Byte(Cur_Time0.Time[T_MONTH);
    *((INT8U *)pDst+4)=Event_Cumu_Data.Time.Year;//Hex2Bcd_Byte(Cur_Time0.Time[T_YEAR);
    return 5;
  }
  else if(0xC010==DI || 0xC011==DI)//日期和时间因为在低功耗的情况下，需要特殊处理
  {
    Get_Time_From_INTER_RTC((S_BCD_Time *)&TempTime);
    
    if(0xC010==DI)
    {
      mem_cpy(pDst,(void *)&TempTime.Week,4,pDst_Start,DstLen);
      return 4;
    }
    else
    {
      mem_cpy(pDst,(void *)&TempTime.Sec,3,pDst_Start,DstLen);
      return 3;
    }
  }
  /*
  else if(Spec_Flag==SPEC_CUMU_EVENT)//累加事件记录
  {
    return Get_Cumu0_Event_Data(DI,pDst,pDst_Start,DstLen);
  }
  */
  else if(0xB7A0==DI)//自锁状态字
  {
    Hex2Bcd(PSW_Err_Info.Lock_Mins,(INT8U *)pDst,3,pDst_Start,DstLen);
    *((INT8U *)pDst+3)=Hex2Bcd_Byte(PSW_Err_Info.Err_Times);
    return 4;
  }
  else if(0xC046==DI)//波特率状态字
  {
    Len=0;
    if(CH_IRAD==Cur_Comm_Ch.Ch)//红外波特率固定为0x01
    {
      *pDst=0x01;
      Len=1;
    }
    else if(CH_RS485_1==Cur_Comm_Ch.Ch)
      Len=Read_Storage_Data(DI_COMM_BAUD0,pDst,pDst_Start,DstLen,pErr);
    else if(CH_RS485_2==Cur_Comm_Ch.Ch)
      Len=Read_Storage_Data(DI_COMM_BAUD1,pDst,pDst_Start,DstLen,pErr);
    
    return Len;
  }
  else if(0xB214==DI)
  {
    Len=Read_Storage_Data(0xB214,&Temp,&Temp,sizeof(Temp),pErr);
    Hex2Bcd(Temp,pDst,3,pDst_Start,DstLen);
    return Len;  
  }
  else if(0xCDE0==DI)//读取ROM数据
  {
    //Temp = Bcd2Hex((INT8U *)pPara+1,4);//地址
    //Len = Bcd2Hex((INT8U *)pPara+5,2);//长度
    Temp=0;
    mem_cpy(&Temp,(INT8U *)pPara+1,4,&Temp,sizeof(Temp));
    Len=0;
    mem_cpy(&Len,(INT8U *)pPara+5,2,&Len,sizeof(Len));
    if(Len > 200)
      Len = 200;
    
    if((INT8U *)pDst+Len<=(INT8U *)pDst_Start+DstLen)
    {
      if(*(INT8U *)pPara==0)//MEM_ID==0表示读取内存数据
      {
        mem_cpy(pDst,(INT8U *)Temp,Len,pDst_Start,DstLen);
        Re = 1;
      }
      else
      {
        OS_Sem_Pend(PUB_RW_ROM_ID);
        Re = Read_PHY_Mem_PUCK(*(INT8U *)pPara - 1,Temp,pDst,Len,pDst_Start,DstLen);
        OS_Sem_Post(PUB_RW_ROM_ID);
      }
      return (Re==1)?Len:0;
    }
    else
    {
      ASSERT(A_WARNING,0);
      return 0;
    }
  }
  else if(Spec_Flag == SPEC_YEAR_PERIOD)
  {
     Len = Read_Storage_Data((DI | 0x0F),(INT8U *)Pub_Buf,(INT8U *)Pub_Buf,sizeof(Pub_Buf),pErr);
     if(Len > 0 && (DI & 0x0F)>0)
     {
       mem_cpy(pDst,(INT8U *)Pub_Buf+((DI & 0x0F) -1)*3,3,pDst_Start,DstLen);
       return 3;
     }
     else
       return 0;
  }
  else if(Spec_Flag == SPEC_DATE_PERIOD)
  {
     Len = Read_Storage_Data((DI | 0x0F),(INT8U *)Pub_Buf,(INT8U *)Pub_Buf,sizeof(Pub_Buf),pErr);
     if(Len > 0 && (DI & 0x0F)>0)
     {
       mem_cpy(pDst,(INT8U *)Pub_Buf+((DI & 0x0F) -1)*3,3,pDst_Start,DstLen);
       return 3;
     }
     else
       return 0;
  }
  else if(Spec_Flag == SPEC_EXTEND)
  {
    Len = Extend_Get_Single_Proto_Data_Proc(DI,pDst,pDst_Start,DstLen,pErr);
    return Len;
  }
  *pErr=NO_ERR;//不是特殊数据项，同时也没有错误
  return 0;  
}


//设置节假日的特殊处理
INT8U Set_Spec_Holiday_Data(PROTO_DI DI,INT8U *pSrc,INT8U SrcLen,INT8U *pErr)
{
  //PROTO_DI DI;
  INT8U Re;
  
  Re=0;
  if(ASSERT(A_WARNING,SrcLen==3))
  {
    *pErr=PROTOCOL_DATA_ERR;        
    return 0;
  }

  if(DI>=0xC411 && DI<=0xC41D)
  {
    OS_Sem_Pend(PUB_BUF0_SEM_ID);//先读出C41F,修改后再重新写入
    if(Read_Storage_Data(0xC41F,(void *)Pub_Buf0,(void *)Pub_Buf0,sizeof(Pub_Buf0),pErr)==45)
    {
      mem_cpy((INT8U *)Pub_Buf0+(DI-0xC411)*3,pSrc,3,(void *)Pub_Buf0,sizeof(Pub_Buf0));
      *pErr=Write_Storage_Data(0xC41F,(void *)Pub_Buf0,45);
      if(NO_ERR==*pErr)
        Re=1;
      else
        Re=0;
    }
    else
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Read_Storage_Data 0x%x error,Err=%d",DI,*pErr);
      if(NO_ERR==*pErr)
        *pErr=PROTOCOL_DI_ERR;        
      Re=0;
    }
    OS_Sem_Post(PUB_BUF0_SEM_ID);
    return Re;
  }
  else if(DI>=0xCC11 && DI<=0xCC1D)
  {
    OS_Sem_Pend(PUB_BUF0_SEM_ID);//先读出CC1F,修改后再重新写入
    if(Read_Storage_Data(0xCC1F,(void *)Pub_Buf0,(void *)Pub_Buf0,sizeof(Pub_Buf0),pErr)==45)
    {
      mem_cpy((INT8U *)Pub_Buf0+(DI-0xCC11)*3,pSrc,3,(void *)Pub_Buf0,sizeof(Pub_Buf0));
      *pErr=Write_Storage_Data(0xCC1F,(void *)Pub_Buf0,45);
      if(NO_ERR==*pErr)
        Re=1;
      else
        Re=0;
    }
    else
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Read_Storage_Data 0x%x error,Err=%d",DI,*pErr);
      if(NO_ERR==*pErr)
        *pErr=PROTOCOL_DI_ERR;        
      Re=0;
    }
    OS_Sem_Post(PUB_BUF0_SEM_ID);
    return Re;
  }
  
  //数据标识不是特殊数据项标识
  *pErr=NO_ERR;        
  Re=0;      

  return Re;
}

INT8U Set_Energy_Start(PROTO_DI DI,INT8U *pSrc, INT16U SrcLen)
{
  INT32U *p;
  INT8U i;
  
  if(Check_Meter_Factory_Status()==0)
    return PROTOCOL_DI_ERR;
  
  DEBUG_PRINT(HUCK,DEBUG_0,"Set Start Energy------!!!!,DI = 0x%x, SrcLen = %d",DI,SrcLen);
  if(DI EQ 0xC91F)
    p = (INT32U *)Cur_Energy.Pos_Active;
  else if(DI EQ 0xC92F)
    p = (INT32U *)Cur_Energy.Neg_Active;
  else if(DI EQ 0xC93F)
    p = (INT32U *)Cur_Energy.Quad_Reactive[0];
  else if(DI EQ 0xC94F)
    p = (INT32U *)Cur_Energy.Quad_Reactive[1];
  else if(DI EQ 0xC95F)
    p = (INT32U *)Cur_Energy.Quad_Reactive[2];
  else if(DI EQ 0xC96F)
    p = (INT32U *)Cur_Energy.Quad_Reactive[3];
  else
    return PROTOCOL_DI_ERR;
  
  for(i = 0; i <= MAX_RATES; i ++)
  {
    //mem_cpy(&Data, pSrc + i*4, 4, &Data, sizeof(Data)); 
    
    *(p + i) = Bcd2Hex(pSrc + i*4, 4)*10;
  }
  
  SET_STRUCT_SUM(Cur_Energy);
  return NO_ERR;//PROTOCOL_DI_ERR;
}

//判断是否应该特殊处理的数据项
INT8U Set_Spec_Data_Proc(PROTO_DI DI,INT8U Spec_Flag,INT8U *pSrc,INT8U SrcLen)
{
  S_BCD_Time TempTime;
  INT8U Err,Re;
  INT16U Len;
  INT32U Temp;
  
  //DI=*(INT16U *)pSrc;  //节假日特殊处理,两套费率方案的前1-13个节假日需要特殊处理
  if((DI>=0xC411 && DI<=0xC41D) || (DI>=0xCC11 && DI<=0xCC1D))
  {
    if(Set_Spec_Holiday_Data(DI,pSrc,SrcLen,&Err)==1)
      return NO_ERR;
    else
    {
       return PROTOCOL_DI_ERR;      
    }
  } 
  else if(DI==0xCEEE)//校表清0
  {
    if(*pSrc==1)
     Set_Def_Adj_Para(); 
    else if(*pSrc==2)
     Set_Def_Para_Except_Adj(); 
    else if(*pSrc==3)
      Set_All_Def_Data();
    else
      return PROTOCOL_DATA_ERR;
     return NO_ERR;
  }
  else if(DI==0xC010)//日期
  {
    mem_cpy(&TempTime,(void *)&Cur_Time1,sizeof(Cur_Time1),&TempTime,sizeof(TempTime));
    TempTime.Week=*pSrc;
    TempTime.Date=*(pSrc+1);
    TempTime.Month=*(pSrc+2);
    TempTime.Year=*(pSrc+3);
    SET_STRUCT_SUM(TempTime);
   
    return Adj_Time(&TempTime);
  }
  else if(DI==0xC011)//时间
  {
    mem_cpy(&TempTime,(void *)&Cur_Time1,sizeof(Cur_Time1),&TempTime,sizeof(TempTime));
    TempTime.Sec=*pSrc;
    TempTime.Min=*(pSrc+1);
    TempTime.Hour=*(pSrc+2);
    SET_STRUCT_SUM(TempTime);
    
    return Adj_Time(&TempTime);       
  }
  else if(DI==0xB214)
  {
    Temp=Bcd2Hex(pSrc,3);
    return Write_Storage_Data(0xB214,&Temp,sizeof(Temp));
  }
  else if(Spec_Flag == SPEC_YEAR_PERIOD)
  {
     Re = NO_ERR;
     OS_Sem_Pend(PUB_BUF_SEM_ID);
     Len = Read_Storage_Data((DI | 0x0F),(INT8U *)Pub_Buf,(INT8U *)Pub_Buf,sizeof(Pub_Buf),&Err);
     if(Len > 0 && (DI & 0x0F)>0)
     {
       mem_cpy((INT8U *)Pub_Buf+((DI & 0x0F) -1)*3,pSrc,3,(INT8U *)Pub_Buf,sizeof(Pub_Buf));
       Re = Write_Storage_Data((DI | 0x0F),(INT8U *)Pub_Buf,Len);
     }
     else
       Re = PROTOCOL_DI_ERR;
     OS_Sem_Post(PUB_BUF_SEM_ID);
     return Re;
  }
  else if(Spec_Flag == SPEC_DATE_PERIOD)
  {
     Re = NO_ERR;
     OS_Sem_Pend(PUB_BUF_SEM_ID);
     Len = Read_Storage_Data((DI | 0x0F),(INT8U *)Pub_Buf,(INT8U *)Pub_Buf,sizeof(Pub_Buf),&Err);
     if(Len > 0 && (DI & 0x0F)>0)
     {
       mem_cpy((INT8U *)Pub_Buf+((DI & 0x0F) -1)*3,pSrc,3,(INT8U *)Pub_Buf,sizeof(Pub_Buf));
       Re = Write_Storage_Data((DI | 0x0F),(INT8U *)Pub_Buf,Len);
     }
     else
       Re = PROTOCOL_DI_ERR;
     OS_Sem_Post(PUB_BUF_SEM_ID);
     return Re;    
  }
  else if(Spec_Flag == SPEC_SET_ENSTART)
  {
     return Set_Energy_Start(DI,pSrc,SrcLen);   
  }

  return PROTOCOL_DI_ERR;
}

#undef DATA_SPEC_C


