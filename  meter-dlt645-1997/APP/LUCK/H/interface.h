
/**                                                                                                
 * @file interface.h                                                                                    
 *     { comment block }
 * @brief                                                                                           
 *     { brief description @ref interface.cpp }                                                                              
 * @author li.yao (nardoo@sohu.com)                                                                                  
 * @version v1.00                                                                               
 * @date 05/29/2008 08:40                                                                                   
 * @note 
 *     { paragraph where a note can be entered. }                                                   
 */                                                                                                 


#ifndef __INTERFACE__
#define __INTERFACE__

#define  MAXDLEN  32  //显示数据最大字符数目
#define  MAXBITS  6   //显示代码位数


typedef signed   char      s8;  ///< unsigned  8 bit quantity.
typedef unsigned char      u8;  ///< unsigned  8 bit quantity.
typedef unsigned short     u16; ///< unsigned 16 bit quantity.
typedef signed   short     s16; ///< signed   16 bit quantity.
typedef unsigned long      u32; ///< unsigned 32 bit quantity.
typedef signed   long      s32; ///< signed   32 bit quantity.
//typedef _ULonglong         u64; ///< unsigned 64 bit quantity.
//typedef _Longlong          s64; ///< signed   64 bit quantity.

#define TRUE 1
#define FALSE 0
typedef  u8 bool;

typedef u32  code_t; ///< display code type.


typedef u16  item_t; ///< dataitem type.


typedef u16  offs_t; ///< dataitem numbers.
typedef s8   curs_t; ///< cursor position.


/// mode element.
typedef struct {
    u8 bit0 : 1; ///< mode2.bit0 ? "停电时间" : "当前时间";
    u8 bit1 : 1; ///< mode4.bit1 ? "告警显示" : "告警关闭";
    u8 bit2 : 1; ///< mode3.bit2 ? 失压标志"闪烁" : "消失";
    u8 bit3 : 1; ///< mode4.bit3 ? "高位补0" : "高位不补0";
    u8 bit4 : 1; ///< mode4.bit4 ? "输入", "输出" : "正向", "反向";
    u8 bit5 : 1; ///< mode4.bit5 ? "上月", "上上月" : "上1月", "上2月";
    u8 bit6 : 1; ///< mode8.bit6 ? 需量不带年 : 需量带年
    u8 bit7 : 1; ///< mode4.bit7 ? "逆时针" : "顺时针";
} mode_t;

/// status element.
typedef union 
{
    struct 
    {
        u32 loss_voltage  : 3; ///< 电压状态(失压/断相,熄灭)(Ua,Ub,Uc)
        u32 over_voltage  : 3; ///< 过压(Ua,Ub,Uc)(闪烁)
        u32 loss_current  : 3; ///< 电流状态(失流/全失流)(Ia,Ib,Ic)
        u32 Cur_reverse   : 3; ///< 电流反极性(-Ia,-Ib,-Ic)(闪烁)
        u32 cell_lowvolt  : 3; ///< 电池电压低(常亮改为闪烁)
        u32 stat_communi  : 4; ///< 通信状态(RS1,RS2,红外)(常亮)
        u32 volt_reverse  : 1; ///< 电压"逆相序"(闪烁)
        u32 event_warning : 1; ///< 事件告警(闪烁)
        u32 switch_opened : 1; ///< 编程键打开(常亮)
        u32 fac_status    : 1; ///< 工厂(常亮)
        u32 jumper_short  : 1; ///< 跳线短接(常亮)
        u32 reset_demand  : 1; ///< 需量复位(常亮)        
        u32 signal_scale  : 3; ///< 通讯模块信号强度指示(常亮)
        u32 status_tariff : 4; ///< 显示当前费率，"尖, 峰, 平, 谷, T5, ...Tx",
        
        u32 meter_locked  : 1; ///< 电表上锁(常亮)
        u32 status_online : 1; ///< 通讯模块在线指示(常亮)
        u32  num_tariff   : 2; //显示主副时段1：主时段；2：副时段
        u32  CalMeter     : 1; //校表事件
        u32  reserved     : 27; //预留
    };
    struct 
    {
        u32 bit1maps; //< 事件状态位图
        u32 bit2maps; //< 事件状态位图
    };    
} stat_t;


/*
typedef enum {
    none = 0x00, 
    key1 = 0x01, 
    key2 = 0x02, 
    key3 = 0x04,
    key4 = 0x08,
    key5 = 0x10,
} pkey_t;
*/
typedef enum {
    modeA, modeB, modeC,
} type_t;

// 获取按键值
u8 getpkey (void);

// 开关液晶背光
void lcdlight (u8 stat);

// 获取模式字
mode_t getmode (void);
// mode_t : 模式字结构。

// 判断是否停电
bool poweroff (void);

// 判断是否为实时数据
bool rtvalue (item_t item);

// 参数初始化
void lcdinit (void);

// A屏轮显时间
s16 rolltime ();

// 获取协议数据内容
s16 getdata (item_t id, void* buff, void* start, s16 maxlen);
// id    : 对应数据的标识码
// buff  : 输入缓冲的首地址
// start : 数据输入起始地址
// maxlen: 输入缓冲的最大长度
// return: 返回写入的实际长度

// 常量显示元素，仅仅显示，不需要闪烁。
void lcd_total     (u8 flg); ///< "总",
void lcd_phase     (u8 flg); ///< "A/B/C相",
void lcd_voltage   (u8 flg); ///< "电压",
void lcd_current   (u8 flg); ///< "电流",
void lcd_positive  (u8 flg); ///< mode4.bit4 ? "输入" : "正向";
void lcd_negative  (u8 flg); ///< mode4.bit4 ? "输出" : "反向";
void lcd_reactive  (u8 flg); ///< "无功",
void lcd_active    (u8 flg); ///< "有功",
void lcd_power     (u8 flg); ///< "功率",
void lcd_factor    (u8 flg); ///< "因数",
void lcd_angle     (u8 flg); ///< "相角",
void lcd_energy    (u8 flg); ///< "电量",
void lcd_demand    (u8 flg); ///< "需量",
void lcd_time      (u8 flg); ///< "时间",
void lcd_charge    (u8 flg); ///< "电费",
void lcd_lostvolt  (u8 flg); ///< "失压",
void lcd_lostcurt  (u8 flg); ///< "失流",
void lcd_periods   (u8 flg); ///< "时段",
void lcd_feerate   (u8 flg); ///< "费率", 
void lcd_thismonth (u8 flg); ///< "本月",
void lcd_lastmonth (u8 flg); ///< mode4.bit5 ? "上月", "上上月" : "上1月", "上2月";
void lcd_assemble  (u8 flg); ///< "无功组合方式1 or 2",
void lcd_quadrant  (u8 flg); ///< "XX象限",
void lcd_tariff    (u8 flg); ///< 显示"费率Tx",
void lcd_times     (u8 flg); ///< 显示"上XX次",

// 输出显示数据的内容
void lcd_data (item_t item, const char* frmt,u8 signpos);

// 输出显示代码的内容
void lcd_code (code_t code, curs_t curs);

// 输出显示方案的内容
void lcd_mode (u8 type);

// 显示缓冲区内容
void lcd_update (void);

// 输出显示事件状态
void lcd_events (void);

bool Lcd_Para_Modi(item_t item);
#endif
