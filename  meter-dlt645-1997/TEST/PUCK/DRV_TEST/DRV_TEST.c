
#define DRV_TEST_C
#include "Pub_PUCK.h"

#ifdef DRV_TEST_EN


#define TEST_TEMP_LEN 20
INT8U Test_Temp_Buf[TEST_TEMP_LEN];
#define TEST_MEM_BUF_LEN MEM_TEST_BYTES

static INT8U KeyNum=0;
INT16U PD_Save_Data_Time;

CONST INT8U TEST_INFO[2][9]={"Failed ","Succeed"};
volatile static INT32U Temp_Timer_Bak=0;
volatile static INT32U Sec_One_Timer;

#define ID_TEST_EXT_U10 ID_MEM_IIC_U10  //0
#define ID_TEST_EXT_U11 ID_MEM_IIC_U11
#define ID_TEST_EXT_U12 ID_MEM_IIC_U12
#define ID_TEST_EXT_U14 ID_MEM_IIC_U14
#define ID_TEST_EXT_U15 ID_MEM_IIC_U15

#ifdef ID_MEM_IIC_CAL
#define ID_TEST_EXT_CAL ID_MEM_IIC_CAL
#define ID_TEST_EXT_U8  (ID_TEST_EXT_CAL+1)
#else
#define ID_TEST_EXT_U8  (ID_TEST_EXT_U15+1)
#endif

#define ID_TEST_MEASURE (ID_TEST_EXT_U8+1)
#define ID_TEST_EXT_RTC (ID_TEST_MEASURE+1)
#define ID_TEST_INTER_RTC (ID_TEST_EXT_RTC+1)
#define ID_TEST_EXT_DOG (ID_TEST_INTER_RTC+1)
#define ID_TEST_PDSAVE_TIME (ID_TEST_EXT_DOG+1)

#define MIN_ID_TEST ID_TEST_EXT_U10
#define MAX_ID_TEST ID_TEST_PDSAVE_TIME
#define MAX_ID_TEST_NUM (MAX_ID_TEST+1)

#define MAX_MEM_NUM (MAX_EPPROM_NUM+1)  //多了个flash


#define   EPPROM_TEST_NUM 10

INT16U const TEST_ADDR[MAX_EPPROM_NUM]={
  U10_SIZE-EPPROM_TEST_NUM,
  U12_SIZE-EPPROM_TEST_NUM,
  U12_SIZE-EPPROM_TEST_NUM,
  U14_SIZE-EPPROM_TEST_NUM,
  U15_SIZE-EPPROM_TEST_NUM
#ifdef ID_MEM_IIC_CAL
 ,UCAL_SIZE-EPPROM_TEST_NUM
#endif
};

#define TEST_FLASH_ADDR  (INT32U)((INT32U)PAGE_SIZE*PAGE_NUMBER-EPPROM_TEST_NUM)

INT8U const  *Const_Drv_Test[MAX_ID_TEST_NUM]=
{
#ifdef ID_TEST_EXT_U10
  #if METER_HARD_TYPE==HARD_TYPE_20081005
    "U10",
  #endif
  #if METER_HARD_TYPE>=HARD_TYPE_20090224
    "U11",
  #endif
#endif
#ifdef ID_TEST_EXT_U11
  #if METER_HARD_TYPE==HARD_TYPE_20081005
    "U11",
  #endif
  #if METER_HARD_TYPE>=HARD_TYPE_20090224
    "U12",
  #endif
#endif
#ifdef ID_TEST_EXT_U12
  #if METER_HARD_TYPE==HARD_TYPE_20081005
    "U12",
  #endif
  #if METER_HARD_TYPE>=HARD_TYPE_20090224
    "U14",
  #endif
#endif
#ifdef ID_TEST_EXT_U14
  #if METER_HARD_TYPE==HARD_TYPE_20081005
    "U14",
  #endif
  #if METER_HARD_TYPE>=HARD_TYPE_20090224
    "U17",
  #endif
#endif
#ifdef ID_TEST_EXT_U15
  "U15",
#endif

#ifdef ID_TEST_EXT_CAL
  "U27",
#endif

#ifdef ID_TEST_EXT_U8
  #if METER_HARD_TYPE==HARD_TYPE_20081005
    "U 8",
  #endif
  #if METER_HARD_TYPE>=HARD_TYPE_20090224
    "U 5",
  #endif
#endif

#ifdef ID_TEST_MEASURE
  "7022",
#endif
#ifdef ID_TEST_EXT_RTC
  "3231",
#endif
#ifdef ID_TEST_INTER_RTC
  "o5C",
#endif
#ifdef ID_TEST_EXT_DOG
  "rST",
#endif
#ifdef ID_TEST_PDSAVE_TIME
  "PoEr"
#endif   
    
};

DECLARE_BUF_TYPE(INT8U,MAX_ID_TEST_NUM,S_Buf_TEST);
static S_Buf_TEST  S_Buf_Test={CHK_BYTE,{0},CHK_BYTE};
#define Drv_Test_Buf S_Buf_Test.Buf


typedef struct 
{
  INT8U Head;
  INT8U Status;  
  INT8U   CS[CS_BYTES];
  INT8U Tail;
}Rst_Test;
NO_INIT volatile Rst_Test Extern_Dog_Test;



/**********************************************************************************
蜂鸣器报警异常
**********************************************************************************/
INT8U Beep_For_Err_Item(INT8U item)
{  
  if(item>MAX_ID_TEST)
    return 0;
  
  if(Drv_Test_Buf[item]==0)
  {
      Beep_For_Test_Alarm_PUCK();
      return 1;
  }
  return 0;
}
/**********************************************************************************
蜂鸣器报警异常
**********************************************************************************/
void Beep_For_Err(void)
{
  INT8U i;
    
  for(i=MIN_ID_TEST;i<=MAX_ID_TEST;i++)
  {
    if(Beep_For_Err_Item(i))
    {
      return ;
    }
  }
}
void Test_RST_Pin(void)
{
  INT32U Temp_Ms_Timer;
  if(CHECK_STRUCT_VAR(Extern_Dog_Test)==0 || CHECK_STRUCT_SUM(Extern_Dog_Test)==0 || Extern_Dog_Test.Status!=0x48)
  {
      DEBUG_PRINT(PUCK,1,"Test Extern Dog ...........................");
      INIT_STRUCT_VAR(Extern_Dog_Test);
      Extern_Dog_Test.Status=0x48;
      SET_STRUCT_SUM(Extern_Dog_Test);
      Temp_Ms_Timer=Ms_Timer_Pub;
      while(Ms_Timer_Pub-Temp_Ms_Timer<3000)     //3秒都不复位！
        Clear_CPU_Dog();    //清CPU内部看门狗
      
      if(Ms_Timer_Pub-Temp_Ms_Timer>=3000)  //怕误取错
      {
        Drv_Test_Buf[ID_TEST_EXT_DOG]=0; 
        DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|-----------------------Test Extern Dog-------------------------------|");
        DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|          Item                                               Result  |");
        DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"| Exter_Rst_Pin                                               Failed  |"); 
        DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|---------------------------------------------------------------------|");         
        mem_set((void *)&Extern_Dog_Test,0,sizeof(Extern_Dog_Test),(void *)&Extern_Dog_Test,sizeof(Extern_Dog_Test));
      }
  }
  else
  {
    Drv_Test_Buf[ID_TEST_EXT_DOG]=1;    
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|-----------------------Test Extern Dog-------------------------------|");
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|           Item                                              Result  |");
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"| Extern_Rst_Pin                                             Succeed  |"); 
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|---------------------------------------------------------------------|");     
  }
  Beep_For_Err_Item(ID_TEST_EXT_DOG);
}
/**********************************************************************************
死延时10毫秒，并喂狗

***********************************************************************************/
/**********************************************************************************/
void Dlyn10MS_ClrDog(INT32U n)
{
  INT32U i;
  for(i=0;i<n;i++)
  {
    if(i%10==0)  //100ms喂狗一次
    {
      Clr_Ext_Inter_Dog();
    }
    WAITFOR_DRV_MS_TIMEOUT(10);
  }
}
/**********************************************************************************
测试1ms内的nop数
**********************************************************************************/
INT32U Get_Counts_1MS(void)
{
  INT32U i;  
  
  for(i=0,Temp_Timer_Bak=Ms_Timer_Pub=0;;i++)  //计算出1ms内的nop数目
  {
    if(Temp_Timer_Bak!=Ms_Timer_Pub)
      break;
  }
  
  i=i*15;
  DEBUG_PRINT(PUCK,1,"NOP Counts=%ld In 1MS",i);
  return i;
}

/**********************************************************************************
测试IO,每秒输出
**********************************************************************************/
void Test_CPU_Output_IO(void)
{
  static INT8U ChkFlag=0;
  ChkFlag=!ChkFlag;
  
  OPEN_MEASU_PWR;           //          计量芯片电源开启
  //置输出口
  SET_AC_POWER_OUT;
  SET_REAC_POWER_OUT;
  SET_AC_DIR_OUT;
  SET_REAC_DIR_OUT;
  SET_AC_LED_OUT;
  SET_REAC_LED_OUT;
  
  
  if(ChkFlag)
  {
    DEBUG_PRINT(PUCK,1,"Trun On All Ext Ports");
    IRDA_NEAR_REC_EN;         //          B_ird_chose_1     //远红外关闭，吸附红外开启
    BAK_POWER_FOR_IRDA;       //        B_ird_pow_1       //远红外电源由低功耗电池供给;
    BAK_POWER_FOR_LCD;        //        LCD_POW_1         //LCD电源由低功耗电池供给;
    BAK_POWER_FOR_MEM;        //        EEPOWER_1         //内卡电源由低功耗电池供给;
    
    
    EXT_ALARM_SET;            //端子排报警
    EXT_SWITCH_SET;
    IRDA_SEND_EN;   //红外发送使能
    IRDA_SEND_DIS;  ///红外发送禁止
    IRDA_NEAR_REC_EN;//          B_ird_chose_1     //远红外关闭，吸附红外开启
    
    //端子排有无功脉冲输出指示
    //有功输出
    A_OUT_0;
    //无功输出
    R_OUT_0;
    //有功正向
    A_DIR_0;
    //无功正向
    R_DIR_0;
    
    
    //有功脉冲 LED 输出口关闭
    MEASU_A_DIR_1;  
   //无功脉冲 LED 输出口关闭    
    MEASU_R_DIR_1;
    
    //LED报警指示灯
    LED_ALARM_CLR;
    
    //需量周期
    EXT_DEMAND_CLR;  

    //时段切换
    EXT_PARSE_CLR;
 
    
    
    DEBUG_PRINT(PUCK,1,"Trun Off All Ext Ports");
  }
  else
  {
    IRDA_FAR_REC_EN;         //          B_ird_chose_0      //远红外开启，吸附红外关闭
    MAIN_POWER_FOR_IRDA;      //       B_ird_pow_0       //远红外电源由主电源供给;
    MAIN_POWER_FOR_LCD;     //       LCD_POW_0         //LCD电源由主电源供给;
    MAIN_POWER_FOR_MEM;     //       EEPOWER_0         //内卡电源由主电源供给;
   
    EXT_ALARM_CLR;
    EXT_SWITCH_CLR;
    IRDA_FAR_REC_EN;//          B_ird_chose_0      //远红外开启，吸附红外关闭
    
    //有功输出
    A_OUT_0;
    //无功输出
    R_OUT_0;
    //有功反向
    A_DIR_1;
    //无功反向
    R_DIR_1;
    
    //有功脉冲 LED 输出口打开
    MEASU_A_DIR_0;
    //无功脉冲 LED 输出口打开    
    MEASU_R_DIR_0;
    
    //LED报警指示灯
    LED_ALARM_SET;
    
    //需量周期
    EXT_DEMAND_SET;  
    
    //时段切换
    EXT_PARSE_SET;
  }
}

/**********************************************************************************
测试液晶
**********************************************************************************/
void Test_CPU_Input_IO(void)
{
 
   DEBUG_PRINT(PUCK,1,"GUMB IO=%d",GUMB_STATUS);
   // UP_COVER_STATUS  DOWN_COVER_STATUS  B_PRG_KEY_STATUS
   DEBUG_PRINT(PUCK,1,"Power_Down IO=%d",POWER_OFF_STATUS); 
}


void Test_All_Port(void)
{
  if(Sec_One_Timer!=Sec_Timer_Pub)
  {
    Test_CPU_Output_IO();
    Test_CPU_Input_IO();
    Sec_One_Timer=Sec_Timer_Pub;    
  }  
    
}
/**********************************************************************************
测试UART
**********************************************************************************/
void Test_UART(void)
{
  INT8U i;
  
  for(i=CHANEL_MIN;i<=CHANEL_MAX;i++)  //注意，内涵调试口的重复初始化！
  {
    Open_Channel_PUCK(i,4800,SAU_PARITY_NONE); 
    Channel_DataSend_PUCK(i,"123456ABCDEF",12);
  }
  Init_DebugChanel_PUCK(1);
}
/**********************************************************************************
测试液晶
**********************************************************************************/
void Test_Lcd(void)
{
  INT8U i;
  
 //DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Test LCD，Please Watch LCD and Judge Result!"); 
  
  mem_set(Temp_Buf_PUCK,0x00,sizeof(Temp_Buf_PUCK),Temp_Buf_PUCK,sizeof(Temp_Buf_PUCK));
  strcpy((char *)Temp_Buf_PUCK,"SELF  TEST");
  
  //BAK_POWER_FOR_LCD;//        LCD_POW_1         //LCD电源由低功耗电池供给;
  MAIN_POWER_FOR_LCD;//       LCD_POW_0         //LCD电源由主电源供给;
  
  InitLcdAllRam();
  Init_HTLCD_Mode(SYS_NORMAL);
  Turn_Light_On();
  
  for(i=0;i<=9;i++)
     SetOnLED8Device_PUCK(N_LED(i+3),Temp_Buf_PUCK[i]);
  UpdataLcdShow();    
}
/**********************************************************************************
测试计量
**********************************************************************************/
#define MEASU_TEST_NUM 3
void Test_Measure(void)
{
  INT32U temp[3];
  INT8U i,Flag=1;
  
  Init_Para();           //计量相关参数初始化
  
  OPEN_MEASU_PWR;//       LCD_POW_0         //LCD电源由主电源供给;
  Dlyn10MS_ClrDog(1);
  MEASU_RST_0;
  Dlyn10MS_ClrDog(1);
  MEASU_RST_1;
  Dlyn10MS_ClrDog(100);
  
 
  for(i=0;i<3;i++)
  {
    Flag&=Measu_RdAndCompData_3Times(REG_R_A_U+i,(INT8U *)&temp[i]);
    temp[i]/=UNIT_V;
    if(((FP32S)temp[i]>Get_Un()*0.2 && (FP32S)temp[i]<Get_Un()*0.5) ||((FP32S)temp[i]>Get_Un()*1.35))
      Flag=0;
  }
  
  Drv_Test_Buf[ID_TEST_MEASURE]=Flag;
  
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|-----------------------Test Measure IC-------------------------------|");
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"| Total Times    Volt_A(V)       Volt_B(V)    Volt_C(V)     Result    |");
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|         %3d          %3ld             %3ld          %3ld     %s   |",\
                          MEASU_TEST_NUM,temp[0],temp[1],temp[2],TEST_INFO[Drv_Test_Buf[ID_TEST_MEASURE]]); 
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|---------------------------------------------------------------------|"); 
  Beep_For_Err_Item(ID_TEST_MEASURE);
}
/**********************************************************************************
RtcFlag:1----------------外部RTC；0--------------内部RTC
测试外部RTC
测试结论：读写测试65536次，实验花费时间为142460ms，即读和写一次，所花时间为2.17ms
**********************************************************************************/
#define RTC_TEST_NUM 10
void Test_All_RTC(INT8U RtcFlag)
{
  INT8U Temp[7],Flag,TempSec;
  INT16U i,RdWrErr=0,CompErr=0;
  INT32U timr;
  
  if(!((RtcFlag==ID_TEST_EXT_RTC)||(RtcFlag==ID_TEST_INTER_RTC)))
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Test RTC ID Error!");
    return ;
  }
  
  if(RtcFlag==ID_TEST_EXT_RTC)   //外部时钟
  {
    Init_DS3231_IIC_Soft();
    Init_ExtRTC_Pulse(1);   //输出秒脉冲
  }

  Temp_Timer_Bak=Ms_Timer_Pub; //用来查看花费的时间
  //读写测试65535次，实验花费时间为Ms10_Timer_Pub=142460ms，即读和写一次，所花时间为2.17ms---------PUCK
  for(i=0;i<RTC_TEST_NUM-1;i++)
  {
    mem_set(Temp,0,sizeof(Temp),Temp,sizeof(Temp));
    if(RtcFlag==ID_TEST_EXT_RTC)
      Flag=!Read_ExtRTC_PUCK(Temp,7);
    else
      Flag=!Read_InterRTC_PUCK(Temp,7);
    
    Flag|=!Check_BCD_Data(Temp,7);//检查数据格式?
    RdWrErr+=Flag; 
    
    if(0==i) //记下第一次读出的秒
    {
      TempSec=Temp[0];    
      Dlyn10MS_ClrDog(100);  //延时2秒，等时钟秒更新
    }
  }
  timr=Ms_Timer_Pub-Temp_Timer_Bak;  
  if(TempSec==Temp[0])    //秒没更新
    CompErr=1;  

   Drv_Test_Buf[RtcFlag]=!(RdWrErr|CompErr);

  if(RtcFlag==ID_TEST_EXT_RTC)
  {
     Beep_For_Err_Item(ID_TEST_EXT_RTC);
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|-----------------------Test Extern RTC-------------------------------|");
  }
  else
  {
    if(CSC!=0x01 ||CKC!=0x38)
      Drv_Test_Buf[RtcFlag]=0;    
    
    Beep_For_Err_Item(ID_TEST_INTER_RTC);
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|------------------------Test Internal RTC----------------------------|");
  }
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"| Total Times    Run Time(MS)      RdWr_Err    Compa_Err    Result    |");
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|         %3d      %9ld            %3d          %3d    %s   |",\
                          RTC_TEST_NUM,timr,RdWrErr,CompErr,TEST_INFO[Drv_Test_Buf[RtcFlag]]); 
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|Current Time(Yea-Mon-Dat_Day_Hou:Min:Sec):%2x-%2x-%2x_%2x_%2x:%2x:%2x       |",\
                          Temp[6],Temp[5],Temp[4],Temp[3],Temp[2],Temp[1],Temp[0]);
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|---------------------------------------------------------------------|"); 
 
}
/**********************************************************************************
测试EPPROM
**********************************************************************************/

void Test_Memory(void)
{
  INT8U j,Flag,RandBuf[TEST_MEM_BUF_LEN];
  INT16U i;
  INT32U timr;  
  static INT16U RdErr[MAX_MEM_NUM],WrErr[MAX_MEM_NUM],CompareErr[MAX_MEM_NUM];
    
  mem_set(Drv_Test_Buf+ID_TEST_EXT_U10,0x01,MAX_MEM_NUM,Drv_Test_Buf+ID_TEST_EXT_U10,MAX_MEM_NUM);
  Temp_Timer_Bak=Ms_Timer_Pub; //用来查看花费的时间
  for(i=0;i<EPPROM_TEST_NUM;i++)
  {
    mem_set(RandBuf,0x55+i,sizeof(RandBuf),RandBuf,sizeof(RandBuf));
    for(j=ID_TEST_EXT_U10;j<ID_TEST_EXT_U8;j++)
    {  
      Flag=!Write_EEPROM_Dvice_PUCK(j,TEST_ADDR[j],TEST_MEM_BUF_LEN,RandBuf);
      WrErr[j]+=Flag;
  
      mem_set(Test_Temp_Buf,0x33,sizeof(Test_Temp_Buf),Test_Temp_Buf,sizeof(Test_Temp_Buf));
      Flag=!Read_EEPROM_Dvice_PUCK(j,TEST_ADDR[j],TEST_MEM_BUF_LEN,Test_Temp_Buf);
      RdErr[j]+=Flag;
     
      if((memcmp(Test_Temp_Buf,RandBuf,TEST_MEM_BUF_LEN)!=0)||(Test_Temp_Buf[TEST_MEM_BUF_LEN]!=0x33))//比较相等，但底层不能多写造成越界！
       CompareErr[j]++;
      
     if(RdErr[j]|WrErr[j]|CompareErr[j])   //有其中一个错误发生，置标志失败标志
      Drv_Test_Buf[j]=0;
    }
    
    Flag=!flash_writ_abs(TEST_FLASH_ADDR,RandBuf,TEST_MEM_BUF_LEN);
    WrErr[j]+=Flag;

    mem_set(Test_Temp_Buf,0x33,sizeof(Test_Temp_Buf),Test_Temp_Buf,sizeof(Test_Temp_Buf));
    Flag=!flash_read_abs(TEST_FLASH_ADDR,Test_Temp_Buf,TEST_MEM_BUF_LEN);
    RdErr[j]+=Flag;
   
    if((memcmp(Test_Temp_Buf,RandBuf,TEST_MEM_BUF_LEN)!=0)||(Test_Temp_Buf[TEST_MEM_BUF_LEN]!=0x33))//比较相等，但底层不能多写造成越界！//比较
    CompareErr[j]++; 
     
   if(RdErr[ID_TEST_EXT_U8]|WrErr[ID_TEST_EXT_U8]|CompareErr[ID_TEST_EXT_U8])  //有其中一个错误发生，置标志失败
    Drv_Test_Buf[ID_TEST_EXT_U8]=0;
   
   Clr_Ext_Inter_Dog();
  }

  timr=Ms_Timer_Pub-Temp_Timer_Bak;
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|-----------------------Test Memory-----------------------------------|");
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"| Total_Times      Read_Err        Write_Err   Compare_Err  Result    |");
  
  for(j=ID_TEST_EXT_U10;j<=ID_TEST_EXT_U8;j++)
  {
    Beep_For_Err_Item(j);
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|%s      %3d           %3d              %3d           %3d  %s   |",\
                          Const_Drv_Test[j],EPPROM_TEST_NUM,RdErr[j],WrErr[j],CompareErr[j],TEST_INFO[Drv_Test_Buf[j]]);
    
  }
 
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|       All Run Time=%9ldMS                                      |",timr); 
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"|---------------------------------------------------------------------|"); 
}

/**********************************************************************************
测试EPPROM
**********************************************************************************/
void Test_Epprom_Err(void)
{  
  INT8U Err;  
  mem_set(Temp_Buf_PUCK,0xa5,sizeof(Temp_Buf_PUCK),Temp_Buf_PUCK,sizeof(Temp_Buf_PUCK));
  I2cReset();
  Read_Storage_Data(0x10003,Temp_Buf_PUCK,Temp_Buf_PUCK,180,&Err);
  
}

/**********************************************************************************
**********************************************************************************/

void Get_Power_Save_Time(void)
{
  INT32U temp;
  INT8U Flag;
  
  Flag=Read_Storage_Data_PUCK(SAVE_PD_DATA_TIME,&temp,4);  //掉电存数时间
  DEBUG_PRINT(PUCK,1,"Power_Save_Time=%ld",temp);
  if(Flag && (temp>10 &&temp<500))
  {
    PD_Save_Data_Time=(INT16U)temp;
    temp=0xfefe;
    Write_Storage_Data(SAVE_PD_DATA_TIME,&temp,4);
    Drv_Test_Buf[ID_TEST_PDSAVE_TIME]=1;
    return ;
  } 
  PD_Save_Data_Time=9999;  
  Drv_Test_Buf[ID_TEST_PDSAVE_TIME]=0;
  Beep_For_Err_Item(ID_TEST_PDSAVE_TIME);
}

/**********************************************************************************
LCD显示每屏结果
**********************************************************************************/
void Dis_Per_Item(INT8U Item)
{
  INT8U i;
  INT16U temp;
  
  if(Item>MAX_ID_TEST)
    return ;
  
  if(Drv_Test_Buf[Item]>1)  //只允许0/1
    return ;
  
  mem_set(Temp_Buf_PUCK,0x00,sizeof(Temp_Buf_PUCK),Temp_Buf_PUCK,sizeof(Temp_Buf_PUCK));
  strcpy((char *)Temp_Buf_PUCK,(char*)Const_Drv_Test[Item]);
   
  if(Drv_Test_Buf[Item]==0)
    strcpy((char *)Temp_Buf_PUCK+5,"Error");
  else
    strcpy((char *)Temp_Buf_PUCK+5," 6ood");
       
  for(i=3;i<=12;i++)
    SetOnLED8Device_PUCK(N_LED(i),Temp_Buf_PUCK[i-3]);

  mem_set(Temp_Buf_PUCK,0x00,sizeof(Temp_Buf_PUCK),Temp_Buf_PUCK,sizeof(Temp_Buf_PUCK));
  //0 不显示
  Temp_Buf_PUCK[6]=(Item+1)/10+'0';
  Temp_Buf_PUCK[7]=(Item+1)%10+'0';
  
  if(ID_TEST_PDSAVE_TIME==Item)
  {
    temp=PD_Save_Data_Time; 
    for(i=0;(i<3) && (temp!=0);i++)
    {
      Temp_Buf_PUCK[2-i]=(temp%10)+'0';
      temp=(temp-(temp%10))/10;
    }
  }  
   // GUMB_STATUS  UP_COVER_STATUS  DOWN_COVER_STATUS  B_PRG_KEY_STATUS
  if(GUMB_STATUS)
    SetOnDevice_PUCK(S_BUTTON);
  if(UP_COVER_STATUS)
    SetOnDevice_PUCK(S_HOUSE);
  if(DOWN_COVER_STATUS)
    SetOnDevice_PUCK(S_SHOT);
  if(B_PRG_KEY_STATUS)
    SetOnDevice_PUCK(S_KEY);

  
  
  for(i=14;i<=21;i++)
    SetOnLED8Device_PUCK(N_LED(i),Temp_Buf_PUCK[i-14]);
  
  UpdataLcdShow();
}
/**********************************************************************************
LCD显示输出结果
**********************************************************************************/
void LCD_Dis_Result(void)
{  
  INT8U KeyValue;
  
  KeyValue=Key_Value_Pub.Key.Byte;
  Key_Value_Pub.Key.Byte=0;
  switch(KeyValue)    //以下获取具体的现实元素信息------------PUCK
  {
        case UP_KEY_VALUE :
        case RIGHT_KEY_VALUE:
            Turn_Light_On();
            if(KeyNum>MAX_ID_TEST)
              KeyNum=0;
            Dis_Per_Item(KeyNum);
            Temp_Timer_Bak=Sec_Timer_Pub;
            KeyNum++;
              break;
 
        case DOWN_KEY_VALUE:
        case LEFT_KEY_VALUE:
            Turn_Light_On();
            if(KeyNum==0)
              KeyNum=MAX_ID_TEST;            
            Dis_Per_Item(KeyNum);
            Temp_Timer_Bak=Sec_Timer_Pub;
            KeyNum--;
             break;
        default:
          if(Sec_Timer_Pub-Temp_Timer_Bak>3)  //3秒循环显示
          {
            if(KeyNum>MAX_ID_TEST)
              KeyNum=0;
            
            if(KeyNum%2)
              Turn_Light_Off();
            else
              Turn_Light_On();              
            Dis_Per_Item(KeyNum);
            Temp_Timer_Bak=Sec_Timer_Pub;
            Beep_For_Err();  
            KeyNum++;
          }
          break;            
   }  
}
/**********************************************************************************
LCD显示输出结果
**********************************************************************************/
void Test_Sleep(void)
{
  /* while(1)
   {
      //Clear_CPU_Dog();    //清CPU内部看门狗
      Clear_Ext_Dog();    //清CPU外部看门狗
      
   }*/
   
  
  /*TAU_Init();
  START_IRDA;
  START_ALL_LOSS;
  Init_ExtRTC_Pulse(1);        //使能秒脉冲----PUCK
  START_1HZ;
  while(1)
  DEBUG_PRINT(PUCK,1,"Enter Into Halt Mode!");
  Init_All_UART(SYS_SLEEP);       //睡眠模式下的串口初始化,打印调试信息的方便
  Init_Inter_Abs(SYS_SLEEP);
  STOP_ALL_LOSS;
  STOPT_RTC_COUNTER; STOP_RTC_ALARM;
  STOP_IRDA;
  Clr_Ext_Inter_Dog();
  //Init_All_IO_Sleep();
  Goto_Sleep_PUCK();
  //P5|=0x08;
  //PM5|=0x08;
  if(Switch_Main_Osc(RUN_MODE))  
  {
    
    //Init_All_UART(SYS_NORMAL);
    //DEBUG_PRINT(PUCK,1,"Wake Up From Halt Mode!");
    while(1)
    {
      Clear_CPU_Dog();    //清CPU内部看门狗
      //Clear_Ext_Dog();    //清CPU外部看门狗
      
    }
    START_1MS;
    START_PMW;
  };*/
}
#endif

void Enable_Key(void)
{
  START_UP_KEY;
  START_DOWN_KEY;
  START_LEFT_KEY;
  START_RIGHT_KEY;
  START_IR_DECODE  //检测遥控解码，实际也是检测红外的收
}

#ifdef ID_MEM_IIC_CAL
void Test_Sumi_I2c_Epprom(void)
{
  INT16U i; 
  INT16U len;
  INT16U Addr;
  
 //Temp_Buf_PUCK长度实际可小！！！！！！！！！！1
    
  Addr=0;
  len=sizeof(Temp_Buf_PUCK);
  //写：
  /*mem_set((void *)Temp_Buf_PUCK,0,len,(void *)Temp_Buf_PUCK,len);
  for(i=0;i<len;i++)
    Temp_Buf_PUCK[i]=255-i; 
  Write_EEPROM_Dvice_PUCK(ID_MEM_IIC_CAL,Addr,len,Temp_Buf_PUCK);
  */
  KeyNum=0;
  while(1)
  {
    Clr_Ext_Inter_Dog();
    Addr=12;
    len=180;//sizeof(Temp_Buf_PUCK);
    mem_set((void *)Temp_Buf_PUCK,Addr,len,(void *)Temp_Buf_PUCK,len);  
    Read_EEPROM_Dvice_PUCK(ID_MEM_IIC_CAL,Addr,len,Temp_Buf_PUCK);  //1:0/260:ok;

    for(i=0;i<len;i++)
      if(Temp_Buf_PUCK[i]!=(INT8U)(255-i-Addr))
        KeyNum++;   //错误计数
    /*
    0/260:ok
    20/100:error
    0/10:ok
    0/17:ok
    0/50:ok
    0/60:ok，255－－196
    0/63:ok
    0/64:error,翻转：255－－192；189－－126
    0/65:error,翻转：255－－191；188－－124
    0/70:error,翻转：255－－186；183－－114
    0/80:error,翻转：255－－176；173－－94
    
    0/100:error
    */
  }  
}
#endif
/**********************************************************************************
测试EPPROM
**********************************************************************************/
void Test_HardWare_PUCK(void)
{  
#ifdef DRV_TEST_EN

 
  //START_MIN_ALARM;
  /*Init_CPU_PUCK(SYS_RESUME);
  Init_ExtDevice_PUCK(SYS_RESUME);
  while(1)
  {
    Clr_Ext_Inter_Dog(); 
  }
  */
  if(Get_Meter_Hard_Mode()!=MODE_TEST)
    return ;  
  
  if(POWER_OFF_STATUS==0)  //掉电了
    return ;
  
  Init_DebugChanel_PUCK(0);
  //Test_Sumi_I2c_Epprom();

  
  DEBUG_PRINT(PUCK,1,">>>>>>>>>>>>>>>>>>>> Self Test Mode <<<<<<<<<<<<<<<<<<<<< ");
  Init_All_Port(SYS_NORMAL); 
  
  Test_RST_Pin();
  Clr_Ext_Inter_Dog();
  
  Test_Lcd();
  Clr_Ext_Inter_Dog();
  
  Test_Measure();
  Clr_Ext_Inter_Dog();
  
  Test_All_RTC(ID_TEST_EXT_RTC); 
  Clr_Ext_Inter_Dog();
  
  Test_Memory();  
  Clr_Ext_Inter_Dog();
  
  Test_All_RTC(ID_TEST_INTER_RTC);
  Test_CPU_Output_IO();
  
  Clr_Ext_Inter_Dog();   
  Get_Power_Save_Time();
  Chk_Table_Conflict();
  //Test_UART();
  //START_ALL_LOSS;         //打开全失压
  //START_FAST_FX_TIMER;
  //START_IRDA_WAKE;
  Enable_Key();   //使能按钮检测和遥控器接收检测
  
  while(1)
  {
    if(Get_Meter_Hard_Mode()==MODE_TEST)    //&&(POWER_OFF_STATUS==1)) //自检模式，喂狗，否则等看门狗复位！
    {
      //Test_Sleep();
      //Test_Epprom_Err();
      LCD_Dis_Result();  //液晶显示测试结果
      Test_All_Port();
      Clr_Ext_Inter_Dog();
      Channel_Data_Proc_PUCK(SYS_NORMAL);
      //Read_Ext_RTC_Status();
    }
  }
#endif
}

