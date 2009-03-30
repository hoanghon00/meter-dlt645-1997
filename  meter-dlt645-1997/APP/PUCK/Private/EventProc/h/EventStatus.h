#ifndef EVENTSTATUS_VAR
#define EVENTSTATUS_VAR  


#ifndef EVENTSTATUS_C
#define EVENTSTATUS_C_EXT  extern
#else
#define EVENTSTATUS_C_EXT 
#endif


const INT16U Const_End_Time[]=
{
  14400,
  600,
  0
};

#define MAX_END_TIME_NUM (sizeof(Const_End_Time)/sizeof(INT16U))

typedef struct{
  INT8U  DoubleFlag:1;      //事件清除属性，0-------不成对事件,由外部访问清除或者定时强制清除;1-------成对出现的事件(事件成对出现,可能会统计该时间段的相关量)
  INT8U  StartSave:1;       //事件起始存储使能
  INT8U  EndSave:1;         //事件起始存储使能
  INT8U  StartUnit:1;       //事件发生计数器，单位：0-----秒；1---------分
  INT8U  EndUnit:1;         //事件结束计数器，单位：0-----秒；1---------分
  INT8U  ForbitEnd:1;       //对于成对事件，事件发生后，按照EndCount的值，隔一段时间强行清除。
  INT8U  EndIndex:1;        //强行结束事件时间索引
  INT8U  DisEvent;
  INT8U StartCount;         //事件发生计数器，单位：秒
  INT8U EndCount;           //对成事件，0表示事件一旦发生后就马上结束，表示事件结束计数器；对于非成对事件，定时强制清除计数器，单位：秒
  void (* const GetInstStatus)(void);
  INT8U const *EventName;
}EventProperty;           //事件和报警属性结构体


/*********************表参数配置需注意的几个问题*********************************************************
1：对于合相判定失压、失流等事件，发生时间>结束时间，且发生时间=1.5×结束时间
2：

*********************************************************************************************************/
EVENTSTATUS_C_EXT const EventProperty EventPropertyVar[MAX_EVENT_NUM]={
  {.DoubleFlag=0,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=0,.EndCount=0,.GetInstStatus=0,.EventName="NONE",\
  },
   
#ifdef ID_EVENT_A_LOSS_VOLT                               //A相失压------------------1
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Volt,.EventName="A Parse Loss Volt",\
  },
#endif

#ifdef ID_EVENT_B_LOSS_VOLT                               //B相失压------------------2
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Volt,.EventName="B Parse Loss Volt",\
  },
#endif  

#ifdef ID_EVENT_C_LOSS_VOLT                             //C相失压------------------3
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Volt,.EventName="C Parse Loss Volt",\
  },
#endif
#ifdef ID_EVENT_A_LOSS_CURR                                //A相失流--------------4
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Curr,.EventName="A Parse Loss Curr",\
  },
  #endif  
  
#ifdef ID_EVENT_B_LOSS_CURR                               //B相失流--------------5
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Curr,.EventName="B Parse Loss Curr",\
  },
  #endif
  
#ifdef ID_EVENT_C_LOSS_CURR                               //C相失流--------------6
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Curr,.EventName="C Parse Loss Curr",\
  },
  #endif
  
#ifdef ID_EVENT_CUR_NEG_SEQ                               //电流逆相序-----------7
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Volt_Curr_Seq,.EventName="Curr Neg Sequence",\
  },
  #endif  
  
#ifdef ID_EVENT_VOLT_NEG_SEQ                              //电压逆相序-----------8
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=10,.EndCount=10,.GetInstStatus=&Check_Volt_Curr_Seq,.EventName="Volt Neg Sequence",\
  },
  #endif  
  
#ifdef ID_EVENT_A_VOLT_MAX
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=0,.EventName="A Parse Volt OverLimted",\
   },    //A相电压超上压--------9----HUCK Will do
  #endif  
  
#ifdef ID_EVENT_B_VOLT_MAX
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=0,.EventName="B Parse Volt OverLimted",\
   },    //B相电压超上压--------10----HUCK Will do
  #endif  
  
#ifdef ID_EVENT_C_VOLT_MAX
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=0,.EventName="C Parse Volt OverLimted",\
    },    //C相电压超上压--------11----HUCK Will do
  #endif  
  
#ifdef ID_EVENT_A_VOLT_MIN
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=0,.EventName="A Parse Volt LowerLimted",\
   },    //A相电压超下压--------12----HUCK Will do
  #endif  
  
#ifdef ID_EVENT_B_VOLT_MIN
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=0,.EventName="B Parse Volt LowerLimted",\
   },    //B相电压超下压-------13----HUCK Will do
  #endif  
  
#ifdef ID_EVENT_C_VOLT_MIN
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=0,.EventName="C Parse Volt LowerLimted",\
   },    //C相电压超下压--------14----HUCK Will do
  #endif  
  
#ifdef ID_EVENT_DEMAND_ALARM
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=1,.GetInstStatus=0,.EventName="OverLimted Demand Alarm",\
   },    //超需量报警-----------15----HUCK  done
  #endif  
  
#ifdef ID_EVENT_DEMAND_SWITCH
  {.DoubleFlag=1,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=1,.GetInstStatus=0,.EventName="OverLimted Demand Switch Toggle",\
   },    //超需量跳闸-----------16----HUCK  done
  #endif
  
#ifdef ID_EVENT_NC
  {.DoubleFlag=0,.StartSave=0,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=0,.GetInstStatus=0,.EventName="Meter NC Event",\
   },       //空空空--------17
  #endif
  
#ifdef ID_EVENT_RESET
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=1,.GetInstStatus=0,.EventName="Meter Reset Event",\
   },      //复位-----------------18----需要增加函数----PUCK
  #endif 
  
#ifdef ID_EVENT_CLR_DEMAND_COM
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=1,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=10,.GetInstStatus=0,.EventName="Clr Demand With COM",\
   },    //通信清需量-----------19----HUCK Will do
  #endif  
#ifdef ID_EVENT_CLR_DEMAND_MAN
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=1,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=10,.GetInstStatus=0,.EventName="Clr Demand With Manul",\
   },    //手动清需量-----------20----HUCK Will do
  #endif  
  
#ifdef ID_EVENT_CLR_PROCESS
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=0,.GetInstStatus=0,.EventName="Clr Process",\
    },    //清过程---------------21----HUCK Will do
  #endif  
  
#ifdef ID_EVENT_CLR_VOLT_PASS
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=0,.GetInstStatus=0,.EventName="Clr Volt Rate",\
   },    //清电压合格率---------22----HUCK Will do
  #endif  
  
#ifdef ID_EVENT_SET_ENERG
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=0,.GetInstStatus=0,.EventName="Set Init Energ",\
   },    //设置初始电量---------23----HUCK not do
  #endif  
  
#ifdef ID_EVENT_UP_COVER_OPEN
  {.DoubleFlag=1,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=3,.EndCount=3,.GetInstStatus=&Check_UpCover,.EventName="Up Cover Opened",\
   },    //开大盖(前端盖)--------24
  #endif  
  
#ifdef ID_EVENT_LEAD_COVER_OPEN
  {.DoubleFlag=1,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=3,.EndCount=3,.GetInstStatus=&Check_DownCover,.EventName="Lead Door Opened",\
   },    //开铅封门-------------25  
  #endif  
  
#ifdef ID_EVENT_RTC_BAT_LOW                   //时钟电池欠压---------26
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=1,.GetInstStatus=&Check_RTCBat_Low,.EventName="RTC Bat Low",\
  },
  #endif  
  
#ifdef ID_EVENT_LOWPWR_BAT_LOW                    //低功耗电池欠压-------27
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=1,.GetInstStatus=&Check_LPWBat_Low,.EventName="Low_Power Bat Low",\
  },
  #endif  
  
#ifdef ID_EVENT_ADJUST_TIME
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=1,.GetInstStatus=0,.EventName="Adjust RTC",\
   },    //校时-----------------28----HUCK Will do
  #endif  
  
#ifdef ID_EVENT_RTCERR_RUN                          //运行时钟乱-----------29----HUCK Will do
  {.DoubleFlag=1,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=1,.GetInstStatus=0,.EventName="RTC Error During Running",\
  },
#endif  
  
#ifdef ID_EVENT_RTCERR_PWRON                        //上电时钟乱-----------30----HUCK Will do
  {.DoubleFlag=1,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=1,.GetInstStatus=0,.EventName="RTC Error With Power On",\
  },
#endif

#ifdef ID_EVENT_EPPROM_FAULT                //EEPROM坏-------------31----HUCK Will do(只是上电时候查)
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=20,.EndCount=0,.GetInstStatus=0,.EventName="Inter Memory Error",\
  },
#endif 

#ifdef ID_EVENT_ADJUST_MEASU
  {.DoubleFlag=1,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=1,.GetInstStatus=0,.EventName="Adjust Measure",\
  },          //校表-----------------32----PUCK Will do
#endif

#ifdef ID_EVENT_MODI_ENERG  
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=20,.EndCount=0,.GetInstStatus=0,.EventName="Remedy Energ Error",\
  },           //电量纠错-------------33----HUCK Will do
#endif

#ifdef ID_EVENT_A_VOLT_OVERLOAD                     //A相电压过压----------34
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Overload_Volt,.EventName="A Parse Overload",\
  },
#endif

#ifdef ID_EVENT_B_VOLT_OVERLOAD                     //B相电压过压----------35
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Overload_Volt,.EventName="B Parse Overload",\
  },
#endif

#ifdef ID_EVENT_C_VOLT_OVERLOAD                     //C相电压过压----------36
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Overload_Volt,.EventName="C Parse Overload",\
  },
#endif

#ifdef ID_EVENT_ENERG_FREEZ  
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=0,.GetInstStatus=0,.EventName="Energ Freeze",\
  },    //电量冻结-------------37----HUCK Will do
#endif

#ifdef ID_EVENT_POWER_ALARM  
  {.DoubleFlag=1,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=0,.EventName="Overlimted Power Alarm",\
  },    //超功率-----------38-同超需量、超功率（HUCK)
#endif

#ifdef ID_EVENT_POWER_SWITCH  
  {.DoubleFlag=1,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=0,.EventName="Overlimted Power Switch Toggle",\
  },    //超功率跳闸-----------39  
#endif

#ifdef ID_EVENT_A_LOSS_PARSE                               //A相断相--------------40  
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Parse,.EventName="A Parse Loss Parse",\
  },
#endif

#ifdef ID_EVENT_B_LOSS_PARSE                                //B相断相--------------41  
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Parse,.EventName="B Parse Loss Parse",\
  },
#endif

#ifdef ID_EVENT_C_LOSS_PARSE                           //C相断相--------------42  
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Parse,.EventName="C Parse Loss Parse",\
  },
#endif

#ifdef ID_EVENT_A_NEG_CURR                        //A相电流反向----------43 
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Neg_Curr,.EventName="A Curr Neg Direction",\
  },
#endif

#ifdef ID_EVENT_B_NEG_CURR               //B相电流反向----------44  
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Neg_Curr,.EventName="B Curr Neg Direction",\
  },
#endif

#ifdef ID_EVENT_C_NEG_CURR                  //C相电流反向----------45 
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Neg_Curr,.EventName="C Curr Neg Direction",\
  },
#endif 

#ifdef ID_EVENT_KEY_SWITCH
  {.DoubleFlag=1,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=2,.EndCount=2,.GetInstStatus=&Check_Key_Switch,.EventName="Key Switch On",\
  },    //跳线短接-------------46
#endif 

#ifdef ID_EVENT_ERR_DI  
  {.DoubleFlag=0,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=20,.EndCount=0,.GetInstStatus=0,.EventName="Illegal Data Identification",\
  },    //数据标识错-----------47----HUCK Will do
#endif  
#ifdef ID_EVENT_ERR_PASSWD
  {.DoubleFlag=1,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=1,.GetInstStatus=0,.EventName="Password Error",\
  },    //密码错---------------48----HUCK Will do
#endif  

#ifdef ID_EVENT_YEAR_FLOW
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=0,.GetInstStatus=0,.EventName="Time Zone Overflow",\
  },    //年时区数超-----------49----HUCK Will do 
#endif

#ifdef ID_EVENT_DATE_FLOW  
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=0,.GetInstStatus=0,.EventName="Date Zone Overflow",\
  },    //日时段数超---------------50----HUCK Will do
#endif  

#ifdef ID_EVENT_TRRI_FLOW
  {.DoubleFlag=0,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=0,.GetInstStatus=0,.EventName="Trri Num Error",\
  },    //费率数超-----------------51----HUCK Will do  
#endif  

#ifdef ID_EVENT_PRGKEY_ON
  {.DoubleFlag=1,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=1,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=1,.GetInstStatus=&Check_Prg_Key,.EventName="Prog Key Pressed",\
  },    //编程键打开-----------52--此按钮需要中断支持
#endif  

#ifdef ID_EVENT_FAC_ON
  {.DoubleFlag=1,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=1,.EndIndex=0,.DisEvent=0,.StartCount=2,.EndCount=2,.GetInstStatus=&Check_Fac_Status,.EventName="Switch Key Shutcircuit",\
  },  //跳线短接(非现场运行模式)-----------53 
#endif 

#ifdef ID_EVENT_METER_LOCK
  {.DoubleFlag=1,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=2,.EndCount=2,.GetInstStatus=0,.EventName="Prg Meter Event",\
  },     //对电表编程(设置重要参数)时，若密码连续出错次数大于等于5次后--------54
#endif  

#ifdef ID_EVENT_AB_FUNC_KEY
  {.DoubleFlag=1,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=3,.EndCount=1,.GetInstStatus=&Check_AB_Func_Key,.EventName="Func AB Mode Switch Key",\
  },    //功能键识别，AB模式切换按钮--------55
#endif    

#ifdef ID_EVENT_POWER_OFF                                    //上电事件--------56----PUCK 
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=1,.GetInstStatus=&Get_Meter_PowerOn,.EventName="Meter Power On Or Power Off",\
  },
#endif    

#ifdef ID_DEMAND_ARRIVE                                    //需量到--------57----HUCK Will do 
  {.DoubleFlag=0,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=0,.GetInstStatus=0,.EventName="Demand Time Expired!",\
  },
#endif  
  
#ifdef ID_OVER_LOAD                                    //负荷控制报警--------58-同超需量、超功率（HUCK)
  {.DoubleFlag=0,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=20,.EndCount=0,.GetInstStatus=0,.EventName="OverLoad Ctrol Alarm",\
  },
#endif  
  
#ifdef ID_NOT_CHARGE                                   //欠费报警--------59----HUCK Will do 
  {.DoubleFlag=0,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=20,.EndCount=0,.GetInstStatus=0,.EventName="Fees Not Enough",\
  },
#endif

#ifdef ID_PARSE_SWITCH                                 //时段切换--------60----HUCK Will do 
  {.DoubleFlag=0,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=0,.GetInstStatus=0,.EventName="Time Prase Switch",\
  },
#endif

#ifdef ID_MEASURE_ERROR                               //计量错误(瞬态字)--------61
  {.DoubleFlag=0,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=20,.EndCount=0,.GetInstStatus=0,.EventName="Measure Error Event",\
  },
#endif

#ifdef ID_METER_ADDR_EN  
  {.DoubleFlag=1,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=1,.GetInstStatus=&Check_MeterAddr_Key,.EventName="Set Meter Addr Key",\
  },    //设置表地址按钮----------62
#endif

#ifdef ID_EVENT_ALL_LOSS_VOLT                           //全失压--------63
  {.DoubleFlag=1,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=30,.EndCount=20,.GetInstStatus=0,.EventName="A Parse Loss Volt",\
  },
#endif  

#ifdef ID_EVENT_AB_LOSS_VOLT                               //AB失压------------------64
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Volt,.EventName="A_B Parse Loss Volt",\
  },
#endif

#ifdef ID_EVENT_AC_LOSS_VOLT                               //AC失压------------------65
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Volt,.EventName="A_C Parse Loss Volt",\
  },
#endif

#ifdef ID_EVENT_BC_LOSS_VOLT                               //BC失压------------------66
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Volt,.EventName="B_C Parse Loss Volt",\
  },
#endif

#ifdef ID_EVENT_ABC_LOSS_VOLT                               //ABC失压------------------67
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Volt,.EventName="A_B_C Parse Loss Volt",\
  },
#endif

#ifdef ID_EVENT_PROG                                      //编程事件：设置参数-----------68
  {.DoubleFlag=0,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=1,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=10,.GetInstStatus=0,.EventName="Prog Data Event",\
  },
#endif


#ifdef ID_EVENT_CLR_ALL_DATA                                   //数据总清-----------69
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=1,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=5,.GetInstStatus=0,.EventName="All Data Clear Event",\
  },
#endif


#ifdef ID_EVENT_UP_COVER_CLOSE                                   //关大盖(前端盖)--------70(HUCK需求) 
  {.DoubleFlag=1,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=3,.EndCount=3,.GetInstStatus=&Check_UpCover,.EventName="Up Cover Closed",\
  },
#endif

#ifdef ID_EVENT_LEAD_COVER_CLOSE
  {.DoubleFlag=1,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=3,.EndCount=3,.GetInstStatus=&Check_DownCover,.EventName="Lead Door Closed",\
   },    //关铅封门------------71(HUCK需求)  
#endif 

#ifdef ID_EVENT_AB_LOSS_CURR                                //AB相失流--------------72
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Curr,.EventName="A_B Parse Loss Curr",\
  },
#endif  
  
#ifdef ID_EVENT_AC_LOSS_CURR                                //AC相失流--------------73
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Curr,.EventName="A_C Parse Loss Curr",\
  },
#endif  
  
#ifdef ID_EVENT_BC_LOSS_CURR                                //BC相失流--------------74
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Curr,.EventName="B_C Parse Loss Curr",\
  },
#endif  

#ifdef ID_EVENT_ALL_LOSS_CURR                                //全失流--------75
 {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=30,.EndCount=20,.GetInstStatus=&Check_Loss_Curr,.EventName="Loss All Cur",\
 },
#endif 

#ifdef ID_EVENT_DOWN_COVER_OPEN                           //开端盖(后端盖)--------76
  {.DoubleFlag=1,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=3,.EndCount=3,.GetInstStatus=&Check_DownCover,.EventName="Down Cover Opened",\
  },
#endif

#ifdef ID_EVENT_DOWN_COVER_CLOSE                         //关端盖(后端盖)--------77(HUCK需求)
  {.DoubleFlag=1,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=3,.EndCount=3,.GetInstStatus=&Check_DownCover,.EventName="Down Cover Closed",\
  },
#endif


#ifdef ID_EVENT_DOOR_OPEN                                  //开计量装置门--------78
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=20,.EndCount=0,.GetInstStatus=0,.EventName="Door Opened",\
  },
#endif

#ifdef ID_EVENT_DOOR_CLOSE                                //关计量装置门--------79(HUCK需求)
  {.DoubleFlag=0,.StartSave=1,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=20,.EndCount=0,.GetInstStatus=0,.EventName="Door Closed",\
  },
#endif
  
#ifdef ID_SLIP_ARRIVE                                 //滑差到--------80
  {.DoubleFlag=0,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=0,.GetInstStatus=0,.EventName="Slipe Time Arrived",\
  },
#endif 

#ifdef ID_ALARM_OUT               //是否有报警事件输出：如声音报警，LCD_LED报警，端子排报警--------81
  {.DoubleFlag=1,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=3,.EndCount=1,.GetInstStatus=&Sound_LED_LCD_Port_Alarm,.EventName="Event Alarm Exist",\
  },
#endif

#ifdef ID_CLR_DEMAND_KEY
  {.DoubleFlag=1,.StartSave=0,.EndSave=0,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=3,.EndCount=1,.GetInstStatus=&Check_ClrDemand_Key,.EventName="Clr Demand Key Press",\
  },                          //手动清需量按钮--------81
#endif
  
#ifdef ID_EVENT_EXT_PROG
  {.DoubleFlag=0,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=1,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=10,.GetInstStatus=0,.EventName="Extern Prog Data Event",\
  },                           //编程事件：设置参数--------82(同 ID_EVENT_PROG,应HUCKGD需求增加)
#endif
  
#ifdef ID_EVENT_LOSS_VOL           //失压变化：--------83(应HUCKGD需求增加，瞬态字由HUCK维护)
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=1,.GetInstStatus=&Get_Loss_Volt_Change,.EventName="Loss Volt Change",\
  },
#endif

#ifdef ID_EVENT_LOSS_CUR    //失流变化：--------84(应HUCKGD需求增加，瞬态字由HUCK维护)
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=1,.GetInstStatus=&Get_Loss_Curr_Change,.EventName="Loss Curr Change",\
  }, 
#endif
  
#ifdef ID_EVENT_NEG_CUR      //功率反向变化：--------85(应HUCKGD需求增加，瞬态字由HUCK维护)
  {.DoubleFlag=0,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=1,.StartCount=1,.EndCount=1,.GetInstStatus=&Get_Curr_Neg_Change,.EventName="Curr Negative Change",\
  },
#endif
 

#ifdef ID_EVENT_OSC_ERR      //单片机时钟系统故障--------86
  {.DoubleFlag=1,.StartSave=1,.EndSave=1,.StartUnit=0,.EndUnit=0,.ForbitEnd=0,.EndIndex=0,.DisEvent=0,.StartCount=1,.EndCount=1,.GetInstStatus=&Monitor_Meter_System_Osc,.EventName="MCU Osc System Error",\
  }                     
#endif  
};

#endif