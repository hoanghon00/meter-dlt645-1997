#define LOAD_DATA_C
#include "Includes.h"

#define LOAD_DATA_SEARCH_NUM 16
/*****************************************
函数原型：INT8U Count_Mid_Index(INT8U Task_ID,INT16U Start,INT16U Last)
函数功能：计算某任务的两个索引的中间索引值
入口参数：Start，起始的Index
Last,终止Index
Ratio，二分法的分比
出口参数：中间索引
******************************************/
/*
INT16U Count_Mid_Index(INT8U Ratio,INT16U Start,INT16U Last)
{
  INT16U MidIndex;
  
  if(ASSERT(A_WARNING,Ratio!=0))//分比不能为0！，否则就会除0了！
    return 0;
  
  if(Start<=Last)
    MidIndex=(Last+Start)/Ratio;
  else//此时是倒追的形式
  {
    MidIndex=(Last+LOAD_DATA_NUM+Start)/Ratio;//为什么要这样算？自己推算一下
    if(MidIndex>=LOAD_DATA_NUM)
      MidIndex=MidIndex%LOAD_DATA_NUM;
  }
  return MidIndex;
}
*/

//读取索引为Index的负荷曲线的时间
//Index表示这条负荷曲线数据的存储Index，其值为0->LOAD_DATA_NUM-1
//pTime,读出的时间数据放到该指针指向的缓冲区中
//返回1表示读回了数据，0表示没有读回数据
INT8U Read_LoadData_Time(INT16U Index,S_HEX_Time *pTime)
{
  INT8U Temp[7+ROM_CS_BYTES];

  INT8U Err;
  INT16U Len;
  
  //读取负荷曲线数据的起始处LOAD_DATA_HEAD_SIZE个字节
  //主要是起始字符和时间
  //前两个字节是0xA0和0xA0表示是负荷曲线,后面5个字节是时间
  Len=Read_Storage_Data_Fix_Len(LOAD_DATA+Index,\
                                0,\
                                LOAD_DATA_HEAD_SIZE,\
                                Temp,\
                                Temp,\
                                sizeof(Temp),\
                                &Err);
  if(LOAD_DATA_HEAD_SIZE!=Len)//读出的长度不对?见鬼(一般是内部校验和不对)
  {  
    DEBUG_PRINT(HUCK,DEBUG_0,"Read LoadData Index=%d error",Index);
    return 0;
  }
  //判断数据格式
  if(Temp[0]==0xA0 && Temp[1]==0xA0)
  {
    mem_cpy(pTime->Time,Temp+2,5,pTime,sizeof(S_HEX_Time));
    Set_STRUCT_Sum(pTime,sizeof(S_HEX_Time),pTime->CS,sizeof(pTime->CS));
    if(Check_HEX_Time(pTime)==1)//时间格式正确
      return 1;
    else
    {
      ASSERT(A_WARNING,0);
      return 0;
    }
  }
  else
  {
    ASSERT(A_WARNING,0);
    return 0;
  }
}

/*****************************************
函数原型：NT16U Get_LoadData_Index(S_HEX_Time *pTime)
函数功能：获取某个时间点的数据的索引,采用二分法查询负荷曲线数据存储区
入口参数：某个时间点
出口参数：索引值,失败时返回NULL_LOAD_INDEX
******************************************/
INT16U Get_LoadData_Index(S_HEX_Time *pTime)//获取某个时间点的数据的索引
{
  S_HEX_Time Time;
  INT8U Temp[6],Count,Re,Err;
  INT16U Index,Len;
  
  Re=CHECK_STRUCT_SUM(LoadData_Index);
  if(ASSERT(A_WARNING,Re==1))//检查数据的校验和
    Check_Data_Avail();

  Index=Get_LoadData_Hour_Index(pTime->Time);//先找出其小时索引位置
  Len=Read_Storage_Data(LOAD_DATA_HOUR_INDEX+Index,Temp,Temp,sizeof(Temp),&Err);//读出该小时内第一个数据的存储位置
  if(2==Len)
  {
    Index=0;
    mem_cpy(&Index,Temp,2,&Index,sizeof(Index));//Index=*(INT16U *)Temp;
    Count=0;
    while(Count<LOAD_DATA_SEARCH_NUM)//每15分钟存储一次索引，因此最多可能需要读15次
    {
      if(Read_LoadData_Time(Index,&Time)!=0) //如果获得时间数据有效
      {
        if(Cmp_Time(&Time,pTime)==TIME_EQU)//两个时间相等，找到数据
          break;
      }
      Index++;
      OS_TimeDly_Ms(10);
    }
  
    if(Count!=LOAD_DATA_SEARCH_NUM)
      return Index;
    else
      return NULL_2BYTES;
  }
  else
    return NULL_2BYTES;
}

INT32U Get_Time_Mins(INT8U Time[])
{
  //INT8U i;
  //INT8U Temp[5];
  
  //for(i = 0; i < 5 ; i ++)
    //Temp[i] = Bcd2Hex_Byte(Time[i]);
  
  return (INT32U)Time[0] + (INT32U)Time[1]*60 + (INT32U)(Time[2]-1)*1440;
  
}
//读取制定索引位置的数据块,该函数主要是为了剔除掉S_HEX_TIme中的CS域,并将时间转BCD马
//Index,索引值,0-LOAD_DATA_NUM-1
//pDst,目标缓冲区
//pDst_Start,
//格式:0xA0,2字节
//Cur_Time0,sizeof(S_HEX_Time)
//数据
INT16U Read_Load_Data_With_Index(INT16U Index,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U *pErr)
{
  INT16U Len,SrcPosi,DstPosi;
  INT8U Err,i,Re,Sum;
  INT32U Mins;
  
  Len=Read_Storage_Data(LOAD_DATA+Index,pDst,pDst,DstLen,&Err);
  if(Get_LoadData_All_Len()==Len && Len>(2+sizeof(S_HEX_Time)))//长度合法
  {
    if(pDst[0]==0xA0 && pDst[1]==0xA0)//头2字节正确
    {
      Mins = Get_Time_Mins(pDst + 2);
      //存储的时间数据是HEX码格式,转化为BCD码
      //Hex2Bcd_Buf(pDst+2,5,pDst+2,pDst,DstLen);
      SrcPosi=LOAD_DATA_HEAD_SIZE+ROM_CS_BYTES;//将数据向前移，去掉ROM_CS;
      
      //if(GET_BIT(Mode_Word.Mode[7].Byte,6))//负荷曲线数据带年
      {
        mem_cpy(pDst,pDst+SrcPosi,9,pDst_Start,DstLen);//A0之前有一个字节是数据长度的高位
        DstPosi=9;
        SrcPosi+=9;
      }
      /*
      else
      {
        mem_cpy(pDst,pDst+SrcPosi,4,pDst_Start,DstLen);//长度-A0-A0-长度
        DstPosi=4;
        SrcPosi+=5;     
        mem_cpy(pDst+DstPosi,pDst+SrcPosi,4,pDst_Start,DstLen);//月、日、时、分
        DstPosi+=4;
        SrcPosi+=4;           
      }
      */
      
      //对6类负荷曲线数据，分别判断其是否需要读出
      for(i=0;i<LOAD_DATA_TYPE_NUM;i++)
      {
        Len=Get_LoadData_Type_Len(i);//获取第i类数据的长度
        if(GET_BIT(Mode_Word.Mode[7].Byte,i)==1 &&\
          LoadData_Para.Period[i] != 0 &&\
          (Mins % LoadData_Para.Period[i])==0)//模式字8表示表示是否记录该数据
        {
          mem_cpy(pDst+DstPosi,pDst+SrcPosi,Len,pDst_Start,DstLen);
          SrcPosi+=Len;
          DstPosi+=Len;
        }
        else
        {
          *(pDst+DstPosi)=0xAA;
          DstPosi++;
          SrcPosi+=Len;
        }
      }
      
      *(pDst)=DstPosi/256;
      *(pDst+3)=DstPosi%256;
      
      Sum=0;
      for(i=0;i<DstPosi;i++)
        Sum+=*(pDst+i); 
      //还有一个字节校验和一个字节结束码   
      pDst[DstPosi++]=Sum;//检验和
      pDst[DstPosi++]=0xE5;      

      //*(pDst+DstPosi++)=*(pDst+SrcPosi++);
      //*(pDst+DstPosi++)=*(pDst+SrcPosi++);

      /////更新最近一次读的索引值
      Re=CHECK_STRUCT_SUM(LoadData_Index);
      if(ASSERT(A_WARNING,1==Re))
        mem_cpy((void *)&LoadData_Index,(void *)&LoadData_Index_Bak,sizeof(LoadData_Index_Bak),(void *)&LoadData_Index,sizeof(LoadData_Index));
      
      LoadData_Index.Last_Read=Index;//最近一次读取的索引
      SET_STRUCT_SUM(LoadData_Index);
      ///////////////////////////
      return DstPosi;
    }
  }
  *pErr=RD_LOAD_DATA_ERR;
  return 0;
}

//读负荷曲线
//pTime电量，需量数据的存储时间
//pSrc写数据缓冲区
//SrcLen数据长度
INT16U Read_Load_Data_With_Time(S_HEX_Time *pTime,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr)
{
  INT16U Index;
  
  Index=Get_LoadData_Index(pTime);//获取该数据的存储索引
  
  if(NULL_LOAD_INDEX!=Index)
  {  
    return Read_Load_Data_With_Index(Index,(INT8U *)pDst,(INT8U *)pDst_Start,DstLen,pErr);
  }
  else
  {
    ASSERT(A_WARNING,0);
    *pErr=RD_LOAD_DATA_ERR;
    return 0;
  }
}

//读负荷曲线数据
//DI,数据标示,主要是D110-D115
//Para，D110时表示需要读取的负荷曲线的时间
//pDst目标缓冲区
//pDst_Start目标的起始地址，和DstLen一起限定pDst和返回数据长度
INT16U Read_Load_Data(PROTO_DI DI,INT8U Para[],INT8U ParaLen,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr)
{
  INT8U Err;
  INT16U Len,Index;
  S_HEX_Time TempTime;

  if(LoadData_Index.Start==0 && LoadData_Index.Last==0)
  {
    //return 0;
    DEBUG_PRINT(HUCK,DEBUG_0,"No Load_Data Now!");
    return 0;
  }
  
  if(0xD110==DI)//读取给定时间数据块
  {
    Index=0;
    if(ParaLen==6)//可能发5字节，也可能发6字节
      Index++;
    TempTime.Time[T_MIN]=Bcd2Hex_Byte(Para[Index]);
    TempTime.Time[T_HOUR]=Bcd2Hex_Byte(Para[Index+1]);
    TempTime.Time[T_DATE]=Bcd2Hex_Byte(Para[Index+2]);
    TempTime.Time[T_MONTH]=Bcd2Hex_Byte(Para[Index+3]);
    TempTime.Time[T_YEAR]=Bcd2Hex_Byte(Para[Index+4]);
    SET_STRUCT_SUM(TempTime);
    Len=Read_Load_Data_With_Time(&TempTime,pDst,pDst_Start,DstLen,&Err);
    return Len;
  }
  else if(0xD111==DI)//读下一快
  {
    Index=LoadData_Index.Last_Read+1;
    
    if(Index>=LOAD_DATA_NUM)
      Index=0;
    
    if(Index==LoadData_Index.Last)//已经读完最后一条数据了，没有数据可读了！！！
    {
      *pErr=RD_LOAD_DATA_ERR;
      return 0;
    }
    //按Index读取某条负荷曲线数据
    //Len=Read_Storage_Data(LOAD_DATA+LoadData_Index.Last_Read,pDst,pDst,DstLen,&Err);
    Len=Read_Load_Data_With_Index(Index,pDst,pDst,DstLen,&Err);
    return Len;
  }
  else if(0xD112==DI)//读取最早块
  {
    Len=Read_Load_Data_With_Index(LoadData_Index.Start,pDst,pDst,DstLen,&Err);
    return Len;    
  }
  else if(0xD113==DI)//读最近块
  {
    //LoadData_Index.Last的前一个才是最后一个哦!
    if(LoadData_Index.Last==0)
      Index=LOAD_DATA_NUM-1;
    else
      Index=LoadData_Index.Last-1;

    Len=Read_Load_Data_With_Index(Index,pDst,pDst,DstLen,&Err);
    return Len; 
  }
  else if(0xD114==DI)//重读上次块
  {
    Index=LoadData_Index.Last_Read;//(INT16U)Bcd2Hex(Para,2);//(INT16U)Bcd2Hex_Byte(Para[0])+(INT16U)Bcd2Hex_Byte(Para[1])*100;
    if(Index>=LOAD_DATA_NUM)
      Index=0;
    
    Len=Read_Load_Data_With_Index(Index,pDst,pDst,DstLen,&Err);
    return Len; 
  }
  else if(0xD115==DI)//读制定索引块,两个入口参数作为索引值
  {
    Index=(INT16U)Bcd2Hex(Para,2);//(INT16U)Bcd2Hex_Byte(Para[0])+(INT16U)Bcd2Hex_Byte(Para[1])*100;
    if(Index>=LOAD_DATA_NUM)
      Index=0;
    
    Len=Read_Load_Data_With_Index(Index,pDst,pDst,DstLen,&Err);
    return Len; 
  }
  return 0;
}

//设置默认的负荷曲线索引值
void Set_Def_Load_Data_Index()
{
  mem_cpy((void *)&LoadData_Index, (void *)Def_LoadData_Index, sizeof(LoadData_Index),(void *)&LoadData_Index,sizeof(LoadData_Index));
  mem_cpy((void *)&LoadData_Index_Bak, (void *)Def_LoadData_Index, sizeof(LoadData_Index),(void *)&LoadData_Index_Bak,sizeof(LoadData_Index_Bak));

  SET_STRUCT_SUM(LoadData_Index);
  SET_STRUCT_SUM(LoadData_Index_Bak);
  
  Save_LoadData_PD_Data();
  Save_LoadData_Index(); 
  Save_LoadData_Index_Bak();
}

void Save_LoadData_PD_Data()
{
  if(CHECK_STRUCT_SUM(LoadData_Index))
     Write_Storage_Data(LOAD_DATA_INDEX_PD,(void *)&LoadData_Index,sizeof(LoadData_Index));
  else if(CHECK_STRUCT_SUM(LoadData_Index_Bak))
     Write_Storage_Data(LOAD_DATA_INDEX_PD,(void *)&LoadData_Index_Bak,sizeof(LoadData_Index));
}

//存储负荷曲线信息的备份,该函数每小时调用一次
void Save_LoadData_Index()
{
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(LoadData_Index);
  if(ASSERT(A_WARNING,Re!=0))
    Check_Data_Avail();
  
  Re=Write_Storage_Data(LOAD_DATA_INDEX,(void *)&LoadData_Index,sizeof(LoadData_Index));
  ASSERT(A_WARNING,NO_ERR==Re);
}

//存储负荷曲线信息的备份,该函数每小时调用一次
void Save_LoadData_Index_Bak()
{
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(LoadData_Index);
  if(ASSERT(A_WARNING,Re!=0))
    Check_Data_Avail();
  
  Re=Write_Storage_Data(LOAD_DATA_INDEX_BAK,(void *)&LoadData_Index,sizeof(LoadData_Index));
  ASSERT(A_WARNING,NO_ERR==Re);
}


//读取负荷曲线参数，从ROM中
void Read_LoadData_Para_From_Rom()
{
  INT8U Err,i;
  //读取起始时间
  if(Read_Storage_Data(0xC510,(void *)(LoadData_Para.Time),(void *)(LoadData_Para.Time),4,&Err)!=4 || Err!=NO_ERR)
    DEBUG_PRINT(HUCK,DEBUG_0,"Read 0xC510 Para Err"); 
  Bcd2Hex_Buf((void *)LoadData_Para.Time,sizeof(LoadData_Para.Time),(void *)LoadData_Para.Time,(void *)LoadData_Para.Time,sizeof(LoadData_Para.Time));
  //读取时间间隔
  for(i=0;i<6;i++)
  {
    if(Read_Storage_Data(0xC511+i,(void *)(&LoadData_Para.Period[i]),(void *)(&LoadData_Para.Period[i]),2,&Err)!=2 || Err!=NO_ERR)
      DEBUG_PRINT(HUCK,DEBUG_0,"Read 0x%x Para Err",0xC511+i); 

    LoadData_Para.Period[i]=(INT16U)Bcd2Hex((INT8U *)(&LoadData_Para.Period[i]),2);    
  }
  SET_STRUCT_SUM(LoadData_Para);  
  
}

//从Rom中读取LoadData_Index
void Read_LoadData_Index_Data_From_Rom()
{
  INT8U Err;
  if(Read_Storage_Data(LOAD_DATA_INDEX_PD,(void *)&LoadData_Index,(void *)&LoadData_Index,sizeof(LoadData_Index),&Err)!=sizeof(LoadData_Index))
  {  
    DEBUG_PRINT(HUCK,DEBUG_0,"Read LOAD_DATA_INDEX_PD data error");
    OS_TimeDly_Ms(100);    
    if(Read_Storage_Data(LOAD_DATA_INDEX,(void *)&LoadData_Index,(void *)&LoadData_Index,sizeof(LoadData_Index),&Err)!=sizeof(LoadData_Index))
    {
       DEBUG_PRINT(HUCK,DEBUG_0,"Read LOAD_DATA_INDEX data error");
       OS_TimeDly_Ms(100);
       if(Read_Storage_Data(LOAD_DATA_INDEX_BAK,(void *)&LoadData_Index,(void *)&LoadData_Index,sizeof(LoadData_Index),&Err)!=sizeof(LoadData_Index))
       {
          DEBUG_PRINT(HUCK,DEBUG_0,"Read LOAD_DATA_INDEX_BAK data error");
          OS_TimeDly_Ms(100);
          mem_cpy((void *)&LoadData_Index,(void *)&Def_LoadData_Index,sizeof(LoadData_Index),(void *)&LoadData_Index,sizeof(LoadData_Index)); 
       }
    }
  }
  SET_STRUCT_SUM(LoadData_Index);
  mem_cpy((void *)&LoadData_Index_Bak,(void *)&LoadData_Index,sizeof(LoadData_Index_Bak),(void *)&LoadData_Index_Bak,sizeof(LoadData_Index_Bak));
}

//检查S_LoadData_Index变量是否合法
INT8U Check_LoadData_Index(S_LoadData_Index *pInfo)
{
  INT8U Re;
  Re=Check_STRUCT_Sum(pInfo,sizeof(S_LoadData_Index),pInfo->CS,sizeof(pInfo->CS));
  if(Re!=0)
  {
     if(pInfo->Last<LOAD_DATA_NUM &&\
        pInfo->Start<LOAD_DATA_NUM)
       return 1;
     else
       return 0;
  }
  else
    return 0;
}

//检查许亮索引数据的合法性
void Check_LoadData_Index_Avail()
{
  INT8U Re=1;

  //先读主区数据，校验和不对则读取备份区数据
  Re=Check_LoadData_Index((S_LoadData_Index *)&LoadData_Index);
  if(Re==0)
  {
    Re=Check_LoadData_Index((S_LoadData_Index *)&LoadData_Index_Bak);
    if(Re==0)
      Read_LoadData_Index_Data_From_Rom();//从ROM中读取该参数
    else
      mem_cpy((void *)&LoadData_Index,(void *)&LoadData_Index_Bak,sizeof(LoadData_Index),(void *)&LoadData_Index,sizeof(LoadData_Index));
  } 
}

//检查负荷曲线相关参数的合法性
void Check_LoadData_Para_Avail()
{
  INT8U Re;
  
  Re=CHECK_STRUCT_SUM(LoadData_Para);
  if(ASSERT(A_WARNING,1==Re))
    Read_LoadData_Para_From_Rom();
}

//获取负荷曲线数据在某个时间点的小时索引，通过小时索引可以读出该小时的第一个数据的存储索引
INT16U Get_LoadData_Hour_Index(INT8U Time[])
{
  ASSERT(A_WARNING,Time[T_MIN]<60 && Time[T_HOUR]<24 &&\
                   Time[T_DATE]>0 && Time[T_DATE]<32 &&\
                   Time[T_MONTH]>0 && Time[T_MONTH]<13);

  //修改为15分钟的索引，因此下表达式*4
  //表示走过了多少个15分钟
  return (INT16U)Time[T_MIN]/15+((INT16U)Time[T_HOUR]+((INT16U)Time[T_DATE]-1)*24+((INT16U)Time[T_MONTH]-1)*31*24)*4;//%LOAD_DATA_HOUR_INDEX_NUM;
}

//写负荷曲线
//pSrc写数据缓冲区
//pSrc格式必须满足:第一二个字节分别是0xA0和0xA0,表示是负荷曲线，后面紧跟一条S_HEX_Time类型的时间数据
//表示这条负荷曲线数据的时间
//SrcLen表示数据长度
//返回NO_ERR表示写成功，其他表示写失败
INT8U Write_Load_Data(INT8U *pSrc,INT16U SrcLen)
{
  INT8U Re;
  INT16U Index,Hour_Index;
  //static INT16U Old_Index=NULL_2BYTES;
  
  if(0xA0==*pSrc && 0xA0==*((INT8U *)pSrc+1))//存储的前两个字节一定要是两个0xA0
  {
    Re=CHECK_STRUCT_SUM(LoadData_Index);//检查索引LoadData_Index的校验和正确
    if(ASSERT(A_WARNING,1==Re))
      Check_Data_Avail();

    Re=_Check_HEX_Time(pSrc+2);  
    if(ASSERT(A_WARNING,1==Re))//时间不对处理
      return WR_LOAD_DATA_ERR;
    
    Index=LoadData_Index.Last;//Index就是当此应该写入的索引
    if(ASSERT(A_WARNING,Index<LOAD_DATA_NUM))//Index不对？不可能吧？
      Index=0;
    
    Re=Write_Storage_Data(LOAD_DATA+Index,pSrc,SrcLen);//直接往里面写数据了！
    if(NO_ERR!=Re)
    {
      ASSERT(A_WARNING,0);
      DEBUG_PRINT(HUCK,DEBUG_0,"Write_Load_Data Error");
    }
    
    Re=CHECK_STRUCT_SUM(LoadData_Index);//检查校验和
    if(ASSERT(A_WARNING,1==Re))
      Check_Data_Avail();
      
    LoadData_Index.Last++;//增加Last索引
    if(LOAD_DATA_NUM<=LoadData_Index.Last)
      LoadData_Index.Last=0;
    
    if(LoadData_Index.Last==LoadData_Index.Start)//Last==Start表示最后一条数据已经追上了第一条数据
      LoadData_Index.Start++;//增加第一条数据的索引
    
    if(LOAD_DATA_NUM<=LoadData_Index.Start)//LoadData_Index.Start的范围也在0->LOAD_DATA_NUM-1之间
      LoadData_Index.Start=0;
    
    SET_STRUCT_SUM(LoadData_Index);//重设校验和
    //将Index数据备份到Bak中去
    mem_cpy((void *)&LoadData_Index_Bak,(void *)&LoadData_Index,sizeof(LoadData_Index_Bak),(void *)&LoadData_Index_Bak,sizeof(LoadData_Index_Bak));
    
    Hour_Index=Get_LoadData_Hour_Index(pSrc+2);//小时索引
    //当前小时索引发生了变化则保存老的，并更新LoadData_Index.Hour_Index
    if(Hour_Index!=LoadData_Index.Hour_Index)//一个新的小时，或者当此的小时或者日与上次不同-->修改为一个新的15分钟 090311,详见Get_LoadData_Hour_Index函数内的修改
    {
      LoadData_Index.Hour_Index=Hour_Index;
      SET_STRUCT_SUM(LoadData_Index);
      mem_cpy((void *)&LoadData_Index_Bak,(void *)&LoadData_Index,sizeof(LoadData_Index_Bak),(void *)&LoadData_Index_Bak,sizeof(LoadData_Index_Bak));
      //Save_LoadData_Index();
      //Save_LoadData_Index_Bak();
      DEBUG_PRINT(HUCK, DEBUG_0, "Save_Load_Data Index, Time_Index=%u, Index=%u",Hour_Index, Index);
      Re=Write_Storage_Data(LOAD_DATA_HOUR_INDEX+Hour_Index,&Index,2);//将当前存储位置存储小时索引中
      ASSERT(A_WARNING,NO_ERR==Re);
    }
    return NO_ERR;
  }
  else
  {
    ASSERT(A_WARNING,0);
    return WR_LOAD_DATA_ERR; 
  }
}

//获取负荷曲线总长
//每个数据块后面有一个0xAA哦
INT16U Get_LoadData_All_Len()
{
  INT8U i;
  INT16U Len;

  //Len=2+sizeof(S_HEX_Time);//2字节0xAA和一个Cur_Time0
  Len=LOAD_DATA_HEAD_SIZE+ROM_CS_BYTES+4+5;//2字节A0+5字节时间+ROM_CS_BYTES校验和,还有两个AA和一个块字节数,还有时间
  for(i=0;i<LOAD_DATA_TYPE_NUM;i++)//有6大类数据哦,分别用0-5表示
  {
    Len+=Get_LoadData_Type_Len(i);//pDst+Len,pDst,DstLen,&Err);包含了一个AA
  }
  return Len+2;//还有2个字节的块累加校验和和块结束码和两个字节的头，还有块字节数
}

//获取某类负荷曲线数据的长度 
//Type:类别，从0-5分别表示6类数据，具体应该看LoadData_Info[i].Type的定义
INT16U Get_LoadData_Type_Len(INT8U Type)
{
  INT16U Len=0;
  INT8U i;
  
  for(i=0;i<S_NUM(LoadData_Info);i++)
  {
    if(LoadData_Info[i].Type==Type)
      Len+=LoadData_Info[i].Len;
  }
  if(Len!=0)
    return Len+1;//包括一个0xAA
  else
  {
    ASSERT(A_WARNING,0);
    return 0;
  }
}

//生成负荷曲线数据
//具体格式:
/*
0:0xA0
1:0xA0
2:Time[0]分(Hex)
3:Time[1]时
4:Time[2]日
5:Time[3]月
6:Time[4]年
7:ROM_CS_BYTES:校验和
8:ROM_CS_BYTES:校验和
9:数据长度高位-----------此处开始是真正的负荷曲线数据
10:0xA0
11:0xA0
11:数据长度低位
12-16:年月日时分
17:
   电压电流频率等
   0xAA
   有无功功率等
   0xAA
   ......
*/
INT16U Make_LoadData(S_HEX_Time *pTime,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen)
{
  INT8U i,Err,Sum;
  INT16U Len,TempLen;

  pDst[0]=0xA0;//数据头是两个0xA0
  pDst[1]=0xA0;
  Len=2;
  //当前时间复制,分、时、日、月、年五个字节
  mem_cpy(pDst+Len,&(pTime->Time[T_MIN]),5,pDst_Start,DstLen);
  //生成前7个字节的校验和
  Set_Sum(pDst,LOAD_DATA_HEAD_SIZE,pDst+LOAD_DATA_HEAD_SIZE,ROM_CS_BYTES,pDst_Start,DstLen);
  Len=LOAD_DATA_HEAD_SIZE+ROM_CS_BYTES;
  
  //以下是真实负荷曲线数据
  Len++;//预留一个字节作为数据长度的高位
  pDst[Len++]=0xA0;//数据头是两个0xA0
  pDst[Len++]=0xA0;
  Len++;
  //负荷曲线时间
  /*
  Len+=Get_DLT645_Data(DI_CUR_TIME,pDst+Len,pDst_Start,DstLen,&Err);
  */
  *(pDst+Len)=Hex2Bcd_Byte(pTime->Time[T_YEAR]);
  *(pDst+Len+1)=Hex2Bcd_Byte(pTime->Time[T_MONTH]);
  *(pDst+Len+2)=Hex2Bcd_Byte(pTime->Time[T_DATE]);
  *(pDst+Len+3)=Hex2Bcd_Byte(pTime->Time[T_HOUR]);
  *(pDst+Len+4)=Hex2Bcd_Byte(pTime->Time[T_MIN]);
  Len+=5;
  
  //分别获取LoadData_Info中定义的每个数据项目
  for(i=0;i<S_NUM(LoadData_Info);i++)
  {
    TempLen=Get_DLT645_Data(LoadData_Info[i].DI,pDst+Len,pDst_Start,DstLen,&Err);
    ASSERT(A_WARNING,LoadData_Info[i].Len==TempLen);
    Len+=TempLen;
    //每类数据后面都应该加一个0xAA作为
    if(i<S_NUM(LoadData_Info)-1 && LoadData_Info[i].Type!=LoadData_Info[i+1].Type)
    {
      pDst[Len]=0xAA;
      Len++;
    }
  }
  
  pDst[Len++]=0xAA;//最后一个0xAA
  //填充两个长度域
  pDst[LOAD_DATA_HEAD_SIZE+ROM_CS_BYTES]=Len/256;//数据长度的高位
  pDst[LOAD_DATA_HEAD_SIZE+ROM_CS_BYTES+3]=Len+2-(LOAD_DATA_HEAD_SIZE+ROM_CS_BYTES);//后面校验和+结束码-数据头=块字节数 
  
  Sum=0;
  for(i=LOAD_DATA_HEAD_SIZE+ROM_CS_BYTES+1;i<Len;i++)
    Sum+=*(pDst+i); 

  pDst[Len++]=Sum;//检验和
  pDst[Len++]=0xE5;//结束码

  return Len;
}

//负荷曲线的存储处理流程
void LoadData_Proc()
{
  INT8U Re,i;
  INT16U Len;
  INT32U Mins;
  INT8U LoadData_Flag;//是否需要记录负荷曲线数据,1表示需要,0表示不需要
  S_HEX_Time TempTime;
  static S_Int8U Min_Bak={CHK_BYTE,0xFF,CHK_BYTE};

  if(Min_Bak.Var==Cur_Time0.Time[T_MIN])//每分钟才进入一次该函数
    return;

  Min_Bak.Var=Cur_Time0.Time[T_MIN];

  Re=CHECK_STRUCT_SUM(LoadData_Para);//负荷曲线参数对否
  if(ASSERT(A_WARNING,1==Re))
    Check_Data_Avail();
  
  LoadData_Flag=0;
  mem_cpy(&TempTime,(S_HEX_Time *)&Cur_Time0,sizeof(Cur_Time0),&TempTime,sizeof(TempTime));
  //计算当前走过的分
  Mins=(INT32U)(TempTime.Time[T_DATE]-1)*24*60+(INT32U)(TempTime.Time[T_HOUR]-1)*60+TempTime.Time[T_MIN];
  //比较每类数据，看该类数据的时间是否到
  for(i=0;i<LOAD_DATA_TYPE_NUM;i++)
  {
    //某类数据的存储时间点到了，则存储整个数据
    if(LoadData_Para.Period[i]!=0 &&\
       Mins%(LoadData_Para.Period[i])==0)
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Load Data %dth type data save now!",i);
      LoadData_Flag=1; 
      break;
    }  
  }
  
  //需要生成并存储负荷曲线
  if(1==LoadData_Flag)
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"Load Data Time!");
    OS_Sem_Pend(PUB_BUF0_SEM_ID);//使用Pub_Buf0用于生成负荷曲线数据
    Len=Make_LoadData(&TempTime,(INT8U *)Pub_Buf0,(INT8U *)Pub_Buf0,sizeof(Pub_Buf0));//生成负荷曲线，并返回其长度
    if(Get_LoadData_All_Len()!=Len)//咋回事？长度不对?只有在长度正确的情况下才存储哦
    {
      ASSERT(A_WARNING,0);
      DEBUG_PRINT(HUCK,DEBUG_0,"Make_LoadData Len error"); 
    }
    else
    {
      DEBUG_PRINT(HUCK,DEBUG_0,"Write Load Data!");
      Re=Write_Load_Data((INT8U *)Pub_Buf0,Len);
      ASSERT(A_WARNING,NO_ERR==Re);
    }
    OS_Sem_Post(PUB_BUF0_SEM_ID);
  } 
}
#undef LOAD_DATA_C
