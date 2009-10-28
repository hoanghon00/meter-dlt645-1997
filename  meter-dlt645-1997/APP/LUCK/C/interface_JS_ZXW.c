
/**
 * @file interface.c
 *     { comment block }
 * @brief
 *     { brief description @ref interface.cpp }
 * @author li.yao (nardoo@sohu.com)
 * @version v1.00
 * @date 05/29/2008 08:41
 * @note
 *     { paragraph where a note can be entered. }
 */

//#include <ctype.h>
//#include <string.h>

#include "Pub_PUCK.h"

#if LCD_PROPERTY==LCD_JS_ZXW
void Check_LCDVar_Cs(void)
{
  if(CHECK_STRUCT_VAR(Pub_Bit8_Len5_PUCK)==0)//头尾不对
    ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_VAR(Pub_Bit16_Len4_PUCK)==0)//头尾不对
    ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_VAR(Sleep_Sec_Timer)==0)//头尾不对
  ASSERT(A_WARNING,0);
  
  if(CHECK_STRUCT_VAR(Key_Value_Pub)==0)//头尾不对
  {
    mem_set((void *)(&Key_Value_Pub),0,sizeof(Key_Value_Pub),\
                 (void *)(&Key_Value_Pub),sizeof(Key_Value_Pub));   
    ASSERT(A_WARNING,0);
  }  
}


// 获取协议数据内容
s16 getdata (item_t item, void* dst, void* dststart, s16 maxlen) 
{ 
  s16 nlen = 0,sign=0;
  void *buff=Dis_GetData_Buf; 
  void *start=Dis_GetData_Buf;
  u8 temp;
  
  if(maxlen>DIS_BUF_LEN)  //越界保护
    return 0;
  
  if (item == DI_COMM_BAUD0)   //第一路通信参数
  {
      u32 type = Chanel_Para[CHANEL_485_1].Baud;
      Hex2Bcd(type, (u8*)buff, sizeof(type), start, maxlen);
      nlen = sizeof(u32);
  }
  else if (item == DI_COMM_BAUD1)   //第二路通信参数
  {
      u32 type = Chanel_Para[CHANEL_485_2].Baud;
      Hex2Bcd(type, (u8*)buff, sizeof(type), start, maxlen);
      nlen = sizeof(u32);
  } 
  else if((item>=0xB630)&&(item<=0xB633))  //有功功率
  {
      nlen = Get_DLT645_Data_For_Disp(item,(u8*)buff,(u8*)start, maxlen);
      if(item==0xB630&& Measu_Status_Mode_ToPub_PUCK.SUM_APwrDir)
        sign=-1;
      if(item==0xB631&& Measu_Status_Mode_ToPub_PUCK.A_APwrDir)
        sign=-1;        
      if(item==0xB632&& Measu_Status_Mode_ToPub_PUCK.B_APwrDir)
        sign=-1;        
      if(item==0xB633&& Measu_Status_Mode_ToPub_PUCK.C_APwrDir)
        sign=-1;
  }
  else if((item>=DI_REACTIVE_ALL)&&(item<=DI_REACTIVE_C))  //无功功率 
  {
      nlen = Get_DLT645_Data_For_Disp(item,(u8*)buff,(u8*)start, maxlen);
      if(item==DI_REACTIVE_ALL&& Measu_Status_Mode_ToPub_PUCK.SUM_RPwrDir)
        sign=-1;
      if(item==DI_REACTIVE_A&& Measu_Status_Mode_ToPub_PUCK.A_RPwrDir)
        sign=-1;        
      if(item==DI_REACTIVE_B&& Measu_Status_Mode_ToPub_PUCK.B_RPwrDir)
        sign=-1;        
      if(item==DI_REACTIVE_C&& Measu_Status_Mode_ToPub_PUCK.C_RPwrDir)
        sign=-1;
  } 
  else if(((item>=0xC331)&&(item<=0xC36E))|| ((item>=0xCB21)&&(item<=0xCB6E)))  // 第1/2套费率第n日时段表第n时段起始时分及其费率号
  {
      nlen = Get_DLT645_Data_For_Disp(item,(u8*)buff,(u8*)start, maxlen);
      temp=Dis_GetData_Buf[0];
      Dis_GetData_Buf[0]=Dis_GetData_Buf[2];
      Dis_GetData_Buf[2]=temp;
      
      temp=Dis_GetData_Buf[0];
      Dis_GetData_Buf[0]=Dis_GetData_Buf[1];
      Dis_GetData_Buf[1]=temp;
  } 
  else 
  {
      nlen = Get_DLT645_Data_For_Disp(item,(u8*)buff,(u8*)start, DIS_BUF_LEN);
  }
  nlen=(nlen>maxlen)?maxlen:nlen;
  mem_cpy(dst, start, nlen, dststart, maxlen);
  mem_set((u8*)dst + nlen, 0, maxlen - nlen, dststart, maxlen);
  
  if(CHECK_STRUCT_VAR(_Dis_GetData_Buf)==0)
     ASSERT(A_WARNING,0);
  
  if(sign==0)
    return nlen;
    
  return (sign);
}

// 判断是否停电
bool poweroff (void) {
    return  (Get_MeterPowerStatus_PUCK());
}

// 判断是否为实时数据
bool rtvalue (item_t item) 
{
  if(DI_COMM_BAUD0==item || DI_COMM_BAUD1==item)
     return 1;
     
  return (S_RAM == Get_DLT645_Data_Storage_Type(item));
}

/********************************************************************************
PUCK:
函数功能：内卡参数读出置sram，参数初始化
入口：
返回：
********************************************************************************/
void lcdinit (void) {
    u8     stat,Err;
    u8     cycle;
    s16    temp;
    offs_t numb;

    temp=Read_Storage_Data(VARHMODE1,&stat, &stat, sizeof(stat),&Err);
    if(!temp)
      stat = 0x03;   //默认参数0x03，无功组合1-------PUCK_LCD
    varhmode1 = stat;
    
    temp=Read_Storage_Data(VARHMODE2,&stat, &stat, sizeof(stat),&Err);
    if(!temp)
      stat = 0x0c;    //默认参数0x0c，无功组合2-------PUCK_LCD
    varhmode2 = stat;
    
    cycle = 0;
    temp=Read_Storage_Data(ROLLCYCLE,&cycle, &cycle, sizeof(cycle),&Err); 
    if((!temp)||(cycle<5)||(cycle>180))  //最小为5秒，最大180秒，循显时间------PUCK_LCD
      cycle=5;
    rollcycle = (u8)Bcd2Hex((u8*)&cycle, 1);     
    
    numb=0;
    temp=Read_Storage_Data(NUMBMODEA,&numb, &numb, sizeof(numb),&Err); 
    if((!temp) ||(!numb))
      numb=0x13;   //BCD码,1字节------PUCK_LCD
    MODE_A_NUM = (u16)Bcd2Hex((u8*)&numb, 1);
    
    numb=0;
    temp=Read_Storage_Data(NUMBMODEB,&numb, &numb, sizeof(numb),&Err); 
     if((!temp)||(!numb))
      numb=0x14;   //BCD码，2字节------PUCK_LCD
    MODE_B_NUM = (u16)Bcd2Hex((u8*)&numb, 2);
}

// A屏轮显时间
s16 rolltime () {
    return (rollcycle);
}

// 开关液晶背光
void lcdlight (u8 stat) {
    if (stat > 0) {
        Turn_Light_On();
    } else {
        Turn_Light_Off();
    }
}


// 获取状态字
stat_t getstat (void) 
{
    static INT8U disflag=0;
    static INT32U SecTimerBak=0;
  
    u16 data;
    stat_t stat;
    stat.bit1maps = 0;
    stat.bit2maps = 0;
    
    data = 0;
    
    if(0==disflag)
    {
      SecTimerBak=Sec_Timer_Pub;
      disflag=1;
    }
    else if(disflag==1 &&Sec_Timer_Pub-SecTimerBak<50)  //事件判定机制还没有走完
      Init_Event_DIS_PUCK();
    else
      disflag=2;
    
    //getdata(DI_LOW_VOL, &data, &data, sizeof(data));
    data=Meter_Run_Status.Volt_LossMiss.Byte;
    stat.loss_voltage  = (data&0x07)|(data >> 0x4); ///< 电压状态(失压/断相)(Ua,Ub,Uc)
    data = 0;
    
    //getdata(0xC044, &data, &data, sizeof(data));            //12ms
    data=Meter_Run_Status.Grid_Stat.Byte;
    stat.over_voltage  = (data >> 0x4); ///< 过压(Ua,Ub,Uc)(闪烁)
    data = 0;
       
    //getdata(0xC042, &data, &data, sizeof(data));            //12ms
    data=Meter_Run_Status.Meter_Stat3.Byte;
    stat.loss_current  = (data >> 0x4); ///< 电流状态(失流/全失流)(Ia,Ib,Ic)
    stat.volt_reverse  = (data>>0x1); ///< 电压"逆相序"(闪烁)
    data = 0;
    
    //getdata(DI_CURRENT_QUADRANT, &data, &data, sizeof(data));
    data=Meter_Run_Status.NegCurr_Qudrant.Byte;
    stat.Cur_reverse  = (data >> 0x4); ///< 电流反极性(-Ia,-Ib,-Ic)(闪烁)
    stat.stat_communi  = Get_Curr_ComType_PUCK(LUCK); ///< 通信状态(RS1,RS2,红外)(常亮)

    data = 0;
    //getdata(0xC043, &data, &data, sizeof(data));       //13ms
    data=Meter_Run_Status.Meter_Stat4.Byte;
    stat.cell_lowvolt  = (data >> 0x5) | (data >> 0x2); ///< 电池电压低(常亮)
    
    data = 0;
    //getdata(DI_METER_PERIPHERY, &data, &data, sizeof(data));
    data=Meter_Run_Status.Key_Event.Byte;
    stat.switch_opened = (data & 0x1); ///< 编程键打开(常亮)
    stat.fac_status  = (data>>1);    // 工厂模式(常亮)
    stat.reset_demand  = (data>>2); ///< 需量复位(常亮)
    stat.meter_locked  = (data>>3); ///< 电表上锁(常亮)
    stat.event_warning = (data>>4); ///< 事件告警(闪烁)
    stat.jumper_short  = (data>>5);    // 跳线短接(闪烁)
    
    data = 0;
    //getdata(DI_MODULE_STATUS, &data, &data, sizeof(data));
    data=Meter_Run_Status.Comm_Status.Byte;
    stat.status_online = (data &  0x1); ///< 通讯模块在线指示(常亮)
    stat.signal_scale  = (data >> 0x1); ///< 通讯模块信号强度指示(常亮)
    data = 0;
    //getdata(0xCA14, &data, &data, sizeof(data));   //18ms
    data=Rate_Status.Status[0].Byte+Rate_Status.Status[1].Byte*((INT16U)256);
    stat.status_tariff = (data >> 0x8); ///< 显示当前费率，"尖, 峰, 平, 谷, T5, ...Tx",
    stat.num_tariff = (data&0x03); ///显示主副时段1：主时段；2：副时段",
    
    
    stat.CalMeter=Meter_Run_Status.Key_Event.Bit.Bit6;
    return (stat);
}

// 获取模式字
mode_t getmode (void) {
    mode_t mode;
    mode.bit0 = Mode_Word.Mode[1].Bit.Bit0;
    mode.bit1 = Mode_Word.Mode[3].Bit.Bit1;
    mode.bit2 = Mode_Word.Mode[2].Bit.Bit2;
    mode.bit3 = Mode_Word.Mode[3].Bit.Bit3;
    mode.bit4 = Mode_Word.Mode[3].Bit.Bit4;
    mode.bit5 = Mode_Word.Mode[3].Bit.Bit5;
    mode.bit6 = Mode_Word.Mode[7].Bit.Bit6;
    mode.bit7 = Mode_Word.Mode[3].Bit.Bit7;
    return (mode);
}

// 常量显示元素，仅仅显示，不需要闪烁。
void lcd_total (u8 flg) { ///< 显示"总",
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_ZONG);

}

void lcd_phase (u8 flg) { ///< 显示"A/B/C/N相",
    if (flg & 0x01) {
        SetOnDevice_PUCK(S_A);
    }
    if (flg & 0x02) {
        SetOnDevice_PUCK(S_B);
    }
    if (flg & 0x04) {
        SetOnDevice_PUCK(S_C);
    }
    //无N相显示，ABCB
    /*
    if (flg & 0x08) {
        SetOnDevice_PUCK(S_N);
    }
    */
}

void lcd_voltage (u8 flg) { ///< 显示"电压",
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_DIAN);
    SetOnDevice_PUCK(S_YA);

}

void lcd_current (u8 flg) { ///< 显示"电流",
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_DIAN);
    SetOnDevice_PUCK(S_LIU);

}

void lcd_positive (u8 flg) { ///< 显示"正向",
    if (flg == 0) {
        return;
    }
    /*
    if (getmode().bit4) {
        SetOnDevice_PUCK(S_SHURU);
    } else {
        SetOnDevice_PUCK(S_ZHENG);
        SetOnDevice_PUCK(S_XIANG);
    }
    */
     SetOnDevice_PUCK(S_ZHENG);
     SetOnDevice_PUCK(S_XIANG);

}

void lcd_negative (u8 flg) { ///< 显示"反向",
    if (flg == 0) {
        return;
    }
    /*
    if (getmode().bit4) {
        SetOnDevice_PUCK(S_SHUCHU);
    } else {
        SetOnDevice_PUCK(S_FAN);
        SetOnDevice_PUCK(S_XIANG);
    }
    */
    SetOnDevice_PUCK(S_FAN);
    SetOnDevice_PUCK(S_XIANG);

}

void lcd_reactive (u8 flg) { ///< 显示"无功",
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_WU);
    SetOnDevice_PUCK(S_GONG);

}

void lcd_active (u8 flg) { ///< 显示"有功",
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_YOU);
    SetOnDevice_PUCK(S_GONG);

}

void lcd_power (u8 flg) { ///< 显示"功率",
    if (flg == 0) {
        return;
    }
    /*
    SetOnDevice_PUCK(S_GONG);
    SetOnDevice_PUCK(S_LV);
    */
    SetOnDevice_PUCK(S_GONGLV);
}

void lcd_factor (u8 flg) { ///< 显示"因数",
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_COS);
    SetOnDevice_PUCK(S_FAI);

}

void lcd_angle (u8 flg) { ///< "相角",
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_FAI);
}

void lcd_energy (u8 flg) { ///< 显示"电量",
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_DIAN);
    SetOnDevice_PUCK(S_LIANG);

}

void lcd_demand (u8 flg) { ///< 显示"需量",
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_XU);
    SetOnDevice_PUCK(S_LIANG);

}

void lcd_time (u8 flg) { ///< 显示"时间",
    if (flg == 0) {
        return;
    }
    /*
    SetOnDevice_PUCK(S_SHI);
    SetOnDevice_PUCK(S_JIAN);
    */
    SetOnDevice_PUCK(S_SHIJIAN);
    
}

void lcd_charge (u8 flg) { ///< 显示"电费",
    if (flg == 0) {
        return;
    }
    /*
    SetOnDevice_PUCK(S_DIAN);
    SetOnDevice_PUCK(S_FEI);
    */
}

void lcd_lostvolt  (u8 flg) { ///< "失压",
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_SHI1);
    SetOnDevice_PUCK(S_YA);
}

void lcd_lostcurt  (u8 flg) { ///< "失流",
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_SHI1);
    SetOnDevice_PUCK(S_LIU);
}

void lcd_periods   (u8 flg) { ///< "时段"
    if (flg == 0) {
        return;
    }
    /*
    SetOnDevice_PUCK(S_SHI);
    SetOnDevice_PUCK(S_DUAN);
    */
}

void lcd_feerate   (u8 flg) { ///< "费率", 
    if (flg == 0) {
        return;
    }
    /*
    SetOnDevice_PUCK(S_FEI);
    SetOnDevice_PUCK(S_LV);
    */
    SetOnDevice_PUCK(S_FEIlLV);
}

void lcd_thismonth (u8 flg) { ///< 显示"本月",
    if (flg == 0) {
        return;
    }
    /*
    SetOnDevice_PUCK(S_BEN);
    SetOnDevice_PUCK(S_YUE);
    */
    SetOnDevice_PUCK(S_DANGQIAN);

}

void lcd_lastmonth (u8 flg) { ///< mode4.bit5 ? "上月", "上上月" : "上1月", "上2月";
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_SHANG);
    if (getmode().bit5) {
        if (flg == 1) {
            SetOnDevice_PUCK(S_YUE);
            return;
        }
        if (flg == 2) {
            SetOnDevice_PUCK(S_SHANG);
            SetOnDevice_PUCK(S_YUE);
            return;
        }
    }
    if (flg > 9) {
        SetOnDevice_PUCK(S_SHIYUE);
    }
    SetOnLED8Device_PUCK(N_LED(1), '0' + (flg % 10));
    SetOnDevice_PUCK(S_YUE);
}


void lcd_assemble (u8 flg) 
{
    static u8 flag=0,num=0,SwitFlg=0;
    static u8 stat;
    
    
    if (flg > 0)  //无功组合模式字，闪烁
    {
      num++;
      if(num>SECOND_2(1))
      {        
        if(SwitFlg!=flg)
          flag=0;
        
        SwitFlg=flg;
        
        stat = (flg == 1) ? varhmode1 : varhmode2;
        stat&=0x0f;
        
         // 此液晶只支持逆时针
        switch(stat&0x0f)
        {
          case 3:
            stat=9;
            break;
          case 5:
            stat=5;
            break;
          case 9:
            stat=3;
            break;
          case 6:
            stat=6;
            break;
          case 10:
            stat=10;
            break;
          case 12:
            stat=6;
            break;  
         }

        flag=(flag&stat)^stat;  //闪烁
        stat = flag; 
        num=0;
      }
    }
    else                  //有功
    {
      getdata(DI_CURRENT_QUADRANT, &stat, &stat, sizeof(stat));
      if(stat&0x0f)   //1/2/4/8
      {
          // 此液晶只支持逆时针
          switch(stat&0x0f)   //长亮或者常灭
          {
            case 1:
              stat=14;
              break;
            case 2:
              stat=7;
              break;
            case 4:
              stat=11;
              break;
            case 8:
              stat=13;
              break;   
          default:
            return;
         }
      }
      else
        return ;
    }
    if (stat & 0x01) {
        SetOnDevice_PUCK(S_XIANGXIAN1);
    }
    if (stat & 0x02) {
        SetOnDevice_PUCK(S_XIANGXIAN2);
    }
    if (stat & 0x04) {
        SetOnDevice_PUCK(S_XIANGXIAN3);
    }
    if (stat & 0x08) {
        SetOnDevice_PUCK(S_XIANGXIAN4);
    }
}

void lcd_quadrant (u8 flg) { ///< 显示"I象限",
    switch (flg) {
        case 0x04 : {
           SetOnDevice_PUCK(S_I3);
           SetOnDevice_PUCK(S_V);
           break;
        }
        case 0x03 : {
           SetOnDevice_PUCK(S_I3);
        }
        case 0x02 : {
           SetOnDevice_PUCK(S_I2);
        }
        case 0x01 : {
           SetOnDevice_PUCK(S_I1);
           break;
        }
    }
}

void lcd_tariff (u8 flg) { ///< 显示"费率Tx",
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_FEIlLV);
    SetOnLED8Device_PUCK(N_LED(2), '0' + (flg % 10));
}

void lcd_times (u8 flg) { ///< 显示"上XX次",
    if (flg == 0) {
        return;
    }
    SetOnDevice_PUCK(S_SHANG);
    if (flg > 9) {
        SetOnDevice_PUCK(S_SHIYUE);
    }
    SetOnLED8Device_PUCK(N_LED(1), '0' + (flg % 10));
    SetOnDevice_PUCK(S_CI);
}

// 十六进制转字符串
static const char hex2cha[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'b', 'C', 'd', 'E', 'F',
};

// 显示单位"kMVAWvarHhz",
const char* lcd_units (const char* s) {
    const char* p;
    for (p = s + strlen(s); p >= s; --p) {
        switch (*p) {
            case '2': {
                break;
            }
            case 'k': {
                SetOnDevice_PUCK(S_M_55);
                SetOnDevice_PUCK(S_M_56);
                break;
            }
            case 'M': {
                SetOnDevice_PUCK(S_M_55);
                SetOnDevice_PUCK(S_M_57);
                break;
            }
            case 'V': {
                SetOnDevice_PUCK(S_M_58);
                break;
            }
            case 'A': {
                SetOnDevice_PUCK(S_M_61);
                SetOnDevice_PUCK(S_M_62);
                SetOnDevice_PUCK(S_M_63);
                SetOnDevice_PUCK(S_M_64);
                break;
            }
            case 'W': {
                SetOnDevice_PUCK(S_M_58);
                SetOnDevice_PUCK(S_M_59);
                SetOnDevice_PUCK(S_M_60);
                SetOnDevice_PUCK(S_M_61);
                SetOnDevice_PUCK(S_M_62);
                break;
            }
            case 'H': {
                SetOnDevice_PUCK(S_M_67);
                SetOnDevice_PUCK(S_M_68);
                break;
            }
            case 'h': {
                SetOnDevice_PUCK(S_M_67);
                 break;
            }
            case 'z': {
                SetOnDevice_PUCK(S_M_69);
                break;
            }
            case 'v': {
                SetOnDevice_PUCK(S_M_60);
                SetOnDevice_PUCK(S_M_62);
                break;
            }
            case 'a': {
                SetOnDevice_PUCK(S_M_64);
                SetOnDevice_PUCK(S_M_65);
                break;
            }
            case 'r': {
                SetOnDevice_PUCK(S_M_66);
                break;
            }
            case 'o': {
                SetOnDevice_PUCK(S_DU);
                break;
            }
            case '|': {
                return (--p);
            }
        }
    }
    return (p);
}

// 数据显示接口，采用字符串格式
void lcd_value (const char* s) {
    typedef struct {
        u16 id;
        u16 dp1;
        u16 dp2;
    } addr_t;

    static const addr_t addr[] = {
        {N_LED(12), D_( 0), D_( 0)}, {N_LED(11), D_( 0), D_( 0)}, {N_LED(10), D_(10), D_(11)},
        {N_LED( 9), D_( 9), D_( 0)}, {N_LED( 8), D_( 8), D_( 7)}, {N_LED( 7), D_( 0), D_( 0)},
        {N_LED( 6), D_( 4), D_( 4)}, {N_LED( 5), D_( 0), D_( 0)}, {N_LED( 4), D_( 4), D_( 4)},
        {N_LED( 3), D_( 0), D_( 0)}
    };
    s16 n = 0;
    for (const char* p = lcd_units(s); p >= s   //显示分隔符|后面的单位。并获取分割符前面的位置--------PUCK
        && (n < sizeof(addr) / sizeof(addr_t)); --p) {
        switch (*p) {
            case '?': {
                break;
            }
            case ' ': {
                n++;
                break;
            }
            case '.': {
                SetOnDevice_D_PUCK(addr[n].dp1);
                break;
            }
            case ':': {
                SetOnDevice_D_PUCK(addr[n].dp1);
                SetOnDevice_D_PUCK(addr[n].dp2);
                break;
            }
            default: {
                SetOnLED8Device_PUCK(addr[n++].id, *p);     //根据字符，点亮相应的LED---------PUCK
            }
        }
    }
}

typedef struct {
    u32 low;
    u32 high;
    u32 higher;
} dval_t;

bool isnull (dval_t* val) {
    return ((val->low == 0) && (val->high == 0)&& (val->higher == 0));
}

char dshift (dval_t* val) {
	  u8 bits = val->low % 16;
	  val->low = (val->high << 28) | (val->low >> 4);    //更新半字节---------PUCK
	  val->high >>= 4;
		return hex2cha[bits];
}

//bit显示方式
char bshift (dval_t* val) 
{
  u8 bits=val->low&0x01;
  val->low=(val->low)>>1;
  return hex2cha[bits];
}

/********************************************************************************
PUCK:
函数功能：输出显示数据的内容
入口：
     type-----模式参数;curs-----光标位置
返回：
********************************************************************************/
// 输出显示数据的内容
// mode4.bit3 1 "高位补0" : 为0，"高位不补0";
// 电能[协议和显示]小数位[范围: 2或3]
// 功率&需量[显示]小数位[范围: 2或3或4; 协议固定为4]
void lcd_data (item_t item, const char* s,u8 singpos) 
{
  INT8S frmt[32];

  if(strlen(s)<=sizeof(frmt))
    strcpy(frmt,(INT8S *)s);
  else
  {
    ASSERT(A_WARNING,0);
    return ;
  }

 //电能显示位数：有功电能，无功电能 HUCK按3位小数给出，5个字节，显示按照 SHOW_ENERG_BIT来显示
 //功率显示位数：有功/视在功率，显示时取标准ID，HUCK给出4位，标准也是4位，无功功率有标准ID(2位)，扩充的3位
    if(SHOW_ENERG_BIT == 3 && ((item&0xf000)==0x6000 || (item&0xf000)==0x9000|| (item&0xff00)==0xE700)) 
    {
      if(strstr((const INT8S *)frmt, "kvarh"))  //电能显示位数，表格按2位
      {          
         strcpy(frmt,"XXXXXX#.###|kvarh");
      }  
      if(strstr((const INT8S *)frmt, "kWh"))  //电能显示位数，按照宏来控制
      {
           strcpy(frmt,"XXXXXX#.###|kWh");
      } 
    }
     //其他显示，按照配置表默认来显示
   if (strstr((const INT8S *)frmt, "tm")) 
   {
        if (!getmode().bit6)  //需量和负荷曲线的时标方式，HUCK总给年，5字节，4字节时(显示不带年)，需要处理-----09-03-12
        {
          strcpy(frmt,"##:##:##:##:##|tm");
        }
    }

    if(((item>=0xC331)&&(item<=0xC36E))|| ((item>=0xCB21)&&(item<=0xCB6E)))  // 第1/2套费率第n日时段表第n时段起始时分及其费率号
    {
       strcpy(frmt,"##-##:##|");
    }

    if (poweroff() && getmode().bit0 && (item == 0xC010)) //低功耗显示时间
    {
        item = DI_POWER_DOWN_TIME;
        strcpy(frmt,"##:##:##:##:##|tm");
    } 
    
    u8   NegFlag=0;
    static s16    _dlen = 0;
    static item_t _item = 0;
    static dval_t _dval = {0,0,0};
    //显示项目发生改变;上次获取不正确，需要再次获取;是SRAM数据；参数发生修改-------------及时获取显示数据
    if((_item!= item)||(_dlen == 0)||rtvalue(item)||Get_Para_Modi_Flag())
    { 
        _dlen = getdata(item, &_dval, &_dval, sizeof(_dval));     //获取需要显示的变量数据
        _item = item;
    }    
    if (_dlen == 0) {
        lcd_value("rEAd Error|");
        return;
    }

    if(_dlen>0 && singpos==4)  //负数处理
    {
      if(_dval.high&0x00000080)         //最高位位符号位
      {
        _dlen=-1;
        _dval.high=_dval.high&0xffffff7f;         //去掉最高位位符号位    
      }      
    }
    char   buff[MAXDLEN] = {0};
    size_t nlen = strlen(frmt);
    dval_t dval = _dval;
    mem_cpy(buff, (char*)frmt, nlen, buff, sizeof(buff));    //将显示格式拷贝到buff---------PUCK
    for (char* next = buff + nlen; next >= buff; --next)   //反向查找
    {
        switch (*next) 
        {
            case 'X':    //最高位标识
                if (!getmode().bit3 &&isnull(&dval))   //高位不补0，且当前数据已是0了，用空格替代-----PUCK
                {
                  if(-1==_dlen&&0==NegFlag)   //显示符号
                  {
                    *next = '-';
                    NegFlag=1;
                  }
                  else
                    *next = ' ';
                } 
                else    //高位需补0，按照实际填写 
                {
                    *next = dshift(&dval);  //获取显示的半个字符，逆序放入buff--------PUCK
                }
                break;
            case '#': 
                *next = dshift(&dval);      //获取显示的半个字符，逆序放入buff--------PUCK
                break;
               
            case 'B':
                *next=  bshift(&dval);
                break;
            case '?': 
                dshift(&dval);
                 break;
        }
    }
    lcd_value(buff); //此时buff的数据格式：-12.3:-4|单位
}

/********************************************************************************
PUCK: 输出显示代码的内容
函数功能：根据显示模式,及模式偏移，获取具体的显示元素等信息
入口：
     type-----模式参数;offs-----该模式下显示的项目顺序
返回：
********************************************************************************/
void lcd_code (code_t code, curs_t curs,u8 dlen) 
{
    typedef struct {
        u16 id;
        u16 dp;
    } addr_t;

    static const addr_t addr[] = {
        {N_LED(21), D_( 0)}, {N_LED(20), D_(20)}, 
        {N_LED(19), D_(19)}, {N_LED(18), D_(22)},
        {N_LED(17), D_(21)}, {N_LED(16), D_( 0)}, 
     // {N_LED(15), D_( 0)}, {N_LED(14), D_( 0)},
    };
    
    char ch = ' ';
    static u8 flag = 0;
    for (curs_t n = 0; n < sizeof(addr) / sizeof(addr_t); ++n) 
    {
      if(curs==-1)  //不闪烁
      {
        if(n>=dlen)
          break;
        ch = hex2cha[code % 16];
        code /= 16;
        if((n>=2)&&(n%2==0))  //显示格式00.01
          SetOnDevice_D_PUCK(addr[n].dp);
      }
      else  //闪烁
      {
        if ((n == curs) && (flag ^= 1)) //是光标所在位置，将显示代码点灭，下次再点亮，形成闪烁效果----------PUCK
        {
            ch = ' ';
            code /= 16;
        } 
        else if (code > 0)              //将显示代码点亮----------PUCK
        {
            ch = hex2cha[code % 16];
            code /= 16;
        } else 
        {
            ch = '0';
        }
      }
      SetOnLED8Device_PUCK(addr[n].id, ch);
    }
}

// 输出显示方案的内容
void lcd_mode (u8 type) {
    switch (type) {
        case modeA: {
            SetOnDevice_PUCK(S_K1);
            break;
        }
        case modeB: {
            SetOnDevice_PUCK(S_K2);
            break;
        }
        default : {
            SetOnDevice_PUCK(S_K3);
            break;
        }
    }
}

// 显示缓冲区内容
void lcd_update (void) 
{
  UpdataLcdShow();
}

// 输出显示事件状态
void lcd_events (void) 
{
  static stat_t flag;
  static u8 num=0;
  
  stat_t stat = getstat();
    
  stat.event_warning &= getmode().bit1; // mode4.bit1 ,1-------告警"显示" :0------ 告警"关闭";
  flag.bit1maps &= stat.bit1maps;
  flag.bit2maps &= stat.bit2maps;
  num++;
 
  if(num>=SECOND_2(1))
  {
      num=0;
      flag.bit1maps ^= stat.bit1maps;     //取反，形成闪烁效果------PUCK
      flag.bit2maps ^= stat.bit2maps;     //取反，形成闪烁效果------PUCK
  }
  
  //电压无过压：正常状态(常亮)
  if(!GET_BIT(stat.loss_voltage,0)&& !GET_BIT(stat.over_voltage,0))  //即没失压也没过压，电压正常,常亮
    SetOnDevice_PUCK(S_Ua);
  if(!GET_BIT(stat.loss_voltage,0)&&GET_BIT(stat.over_voltage,0)&& !GET_BIT(flag.over_voltage,0))  //没失压,但过压，闪烁
  {
      SetOnDevice_PUCK(S_Ua_UP);
      SetOnDevice_PUCK(S_Ua);
  }
  
  if(!GET_BIT(stat.loss_voltage,1)&& !GET_BIT(stat.over_voltage,1))  //即没失压也没过压，电压正常,常亮
    SetOnDevice_PUCK(S_Ub);
  if(!GET_BIT(stat.loss_voltage,1)&& GET_BIT(stat.over_voltage,1)&& !GET_BIT(flag.over_voltage,1))  //没失压,但过压，闪烁
  {
      SetOnDevice_PUCK(S_Ub_UP);
      SetOnDevice_PUCK(S_Ub);
  }
      
  if(!GET_BIT(stat.loss_voltage,2)&& !GET_BIT(stat.over_voltage,2))  //即没失压也没过压，电压正常,常亮
    SetOnDevice_PUCK(S_Uc);
  if(!GET_BIT(stat.loss_voltage,2)&& GET_BIT(stat.over_voltage,2)&& !GET_BIT(flag.over_voltage,2))  //没失压,但过压，闪烁
  {
      SetOnDevice_PUCK(S_Uc_UP);
      SetOnDevice_PUCK(S_Uc);
  }

  //电流反相：显示或者熄灭--------------PUCK
  if (stat.Cur_reverse&0x01 && !GET_BIT(stat.loss_current,0))  //A相反极性,且没有失流
      SetOnDevice_PUCK(S_Ia_);
  if (stat.Cur_reverse&0x02 && !GET_BIT(stat.loss_current,1))  //B相反极性,且没有失流
      SetOnDevice_PUCK(S_Ib_);
  if (stat.Cur_reverse&0x04 && !GET_BIT(stat.loss_current,2))  //C相反极性,且没有失流
      SetOnDevice_PUCK(S_Ic_);
  
   //电流状态(常亮或者闪烁)
  if(!GET_BIT(stat.loss_current,0)&&!GET_BIT(stat.Cur_reverse,0))//没有失流，且没有反相，常亮
      SetOnDevice_PUCK(S_Ia);
  if(!GET_BIT(stat.loss_current,0)&&GET_BIT(stat.Cur_reverse,0)&& !GET_BIT(flag.Cur_reverse,0))//没有失流，且反相，闪烁
      SetOnDevice_PUCK(S_Ia);
  
  if(!GET_BIT(stat.loss_current,1)&&!GET_BIT(stat.Cur_reverse,1))//没有失流，且没有反相，常亮
      SetOnDevice_PUCK(S_Ib);
  if(!GET_BIT(stat.loss_current,1)&&GET_BIT(stat.Cur_reverse,1)&& !GET_BIT(flag.Cur_reverse,1))//没有失流，且反相，闪烁
      SetOnDevice_PUCK(S_Ib);
  
  if(!GET_BIT(stat.loss_current,2)&&!GET_BIT(stat.Cur_reverse,2))//没有失流，且没有反相，常亮
      SetOnDevice_PUCK(S_Ic);
  if(!GET_BIT(stat.loss_current,2)&&GET_BIT(stat.Cur_reverse,2)&& !GET_BIT(flag.Cur_reverse,2))//没有失流，且反相，闪烁
      SetOnDevice_PUCK(S_Ic);
  

  ///< 电池电压低(常亮)
  if (flag.cell_lowvolt & 0x01) {  //时钟电池----闪烁
      SetOnDevice_PUCK(S_BAT2);
  }
  if (flag.cell_lowvolt & 0x02) {  //低功耗电池----闪烁
      SetOnDevice_PUCK(S_BAT1);
  }
  
  if (flag.cell_lowvolt & 0x04)
  {
    /*    
    SetOnDevice_PUCK(S_BAT3);
   */
  }
  ///< 通信状态(RS1,RS2,红外)(常亮)
  if (stat.stat_communi &(1<<CHANEL_IRDA)) 
  {
      SetOnDevice_PUCK(S_HONGWAI);
  }
  
  if (stat.stat_communi &(1<<CHANEL_485_1)) 
  {
      SetOnDevice_PUCK(S_RS1);
  }
  
  if (stat.stat_communi &(1<<CHANEL_485_2)) 
  {
      SetOnDevice_PUCK(S_RS2);
  }
  
#ifdef CHANEL_MOUDEL
  if (stat.stat_communi &(1<<CHANEL_MOUDEL)) 
  {
    /*  
      SetOnDevice_PUCK(S_ARROW_LEFT);
      SetOnDevice_PUCK(S_ARROW_RIGHT);
    */
    SetOnDevice_PUCK(S_BASE_TOWER);    
  }
#endif
  
  ///< 电压"逆相序"(闪烁)
  if (flag.volt_reverse > 0x00)
  {
      SetOnDevice_PUCK(S_NIXIANGXU);
  }
  ///< 事件告警(闪烁)
  if (flag.event_warning & 0x01) 
  {
      SetOnDevice_PUCK(S_LIGHT);
  }
  ///< 编程键打开(常亮)
  if (stat.switch_opened & 0x01) 
  {
      SetOnDevice_PUCK(S_BUTTON);
  }    
  //工厂模式(常亮)
  if (stat.fac_status & 0x01) 
  {
      SetOnDevice_PUCK(S_HOUSE);
  }
  ///< 需量复位(常亮)
  if (stat.reset_demand & 0x01) 
  {
      SetOnDevice_PUCK(S_KEY);
  }    
    
  //跳线短接(闪烁)
  if (flag.jumper_short & 0x01) 
  {
      SetOnDevice_PUCK(S_SHOT);
  }
  ///< 电表上锁(常亮)
  if (stat.meter_locked & 0x01) 
  {
      SetOnDevice_PUCK(S_LOCK);
  }
  ///< 通讯模块在线指示(常亮)
  if (stat.status_online & 0x01)
  {
      //SetOnDevice_PUCK(S_CIRCLE);
    SetOnDevice_PUCK(S_BASE_TOWER);
  }
  ///< 通讯模块信号强度指示
  switch (stat.signal_scale) 
  {   
    case 0x00 :
          break;
    case 0x01 :
          SetOnDevice_PUCK(S_CSQ1);
          break;
    case 0x02 :
          SetOnDevice_PUCK(S_CSQ2);
          SetOnDevice_PUCK(S_CSQ1);
          break;
    case 0x03 :
          SetOnDevice_PUCK(S_CSQ3);
          SetOnDevice_PUCK(S_CSQ2);
          SetOnDevice_PUCK(S_CSQ1);
          break;      
      default:
          SetOnDevice_PUCK(S_CSQ4);
          SetOnDevice_PUCK(S_CSQ3);
          SetOnDevice_PUCK(S_CSQ2);
          SetOnDevice_PUCK(S_CSQ1);
         break;
  }
  ///< 显示当前费率，"尖, 峰, 平, 谷, T5, ...Tx",
  switch (stat.status_tariff) 
  {
      case 0x00 : {
          break;
      }
      case 0x01 : {
          SetOnDevice_PUCK(S_JIAN1);
          break;
      }
      case 0x02 : {
          SetOnDevice_PUCK(S_FENG);
          break;
      }
      case 0x03 : {
          SetOnDevice_PUCK(S_PING);
          break;
      }
      case 0x04 : {
          SetOnDevice_PUCK(S_GU);
          break;
      }
      default : {
          SetOnDevice_PUCK(S_TT);
          SetOnLED8Device_PUCK(N_LED(13), '0'+stat.status_tariff);
          break;
      }
  }
  
  ///显示主副时段1：主时段；2：副时段",
  if(1==stat.num_tariff)
    SetOnDevice_PUCK(S_1);
  if(2==stat.num_tariff)
    SetOnDevice_PUCK(S_2);
  
  if(Read_Event_Status(ID_METER_ADDR_EN))
    SetOnDevice_PUCK(S_SHOT);
  
  //if (flag.CalMeter & 0x01)   //发现未校表，闪烁小数点
  //  SetOnDevice_PUCK(S_BASE_TOWER);
  
  if(Read_Event_Status(ID_EVENT_UP_COVER_OPEN))  //上端盖
    SetOnDevice_PUCK(S_UPCOVERP);

  if(Read_Event_Status(ID_EVENT_DOWN_COVER_OPEN))  //上端盖
    SetOnDevice_PUCK(S_DOWNCOVERP);
  
}
#endif
