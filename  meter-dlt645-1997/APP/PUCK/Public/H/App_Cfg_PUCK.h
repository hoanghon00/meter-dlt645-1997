
#ifndef APP_CFG_PUCK
#define APP_CFG_PUCK




#define SHOW_ENERG_BIT     3   //显示的电能小数位
#define MEASURE_ERROR_STATICS_EN //开启，打开计量错误统计，同时开辟相应的 Sram资源和Flash资源
#define OS_DEBG_EN       //OS功能打开 ！

#define PRINT_PUCK_MEA_EN  1        //1:使能PUCK的计量调试信息，0:禁止
#define PRINT_PUCK_LCD_EN  1        //1:使能LUCK的LCD调试信息，0:禁止
#define PRINT_PUCK_CHANNEL_EN  1    //1:使能通道调试信息，0:禁止
#define PRINT_PUCK_EVENT_EN  1    //1:使能通道调试信息，0:禁止
#define PRINT_PUCK_MEM_EN  1    //1:使能通道调试信息，0:禁止

#define MEASURE_VAR_BAK_EN       //开启，使能电能变量的备份
//#define MEASU_ANGLE_RD           //开启：角度由7022给出；关闭：由自己算出,建议关闭该宏，角度自己算
#define DRV_TEST_EN             //开启:使能驱动测试功能
//#define CONFLICT_TAB_CHKEN     //检查所有配置表是否冲突
#define C_MODE_EN               //宏打开，使能C模式功能


//操作配置宏
#define CHANNEL_FAST_EN               //宏打开，通信通道快速响应





#if METER_PLACE == METER_GD   //HUCK ver
  #define GD_VERSION
#endif

#ifdef  GD_VERSION                      
  #define LOSS_VOLT_ONLY           //判定依据：失压只判定电压，与电流无关
  #define LOSS_CURR_ONLY           //判定依据：失流只判定电流，与电流平衡率无关
#else                              //通用版本
  #define MULTI_LOSS_VOLT_EN       //合相判定：宏打开，失压合相判定，宏关闭：分相判定
  #define MULTI_LOSS_CURR_EN       //合相判定：宏打开，失流合相判定，宏关闭：分相判定
#endif

//计量参数配置宏
#define DEFAULT_PULSE_SPEC  PULSE_TYPE2
#define DEFAULT_VOLT_SPEC   VOLT_3220
#define DEFAULT_CURR_SPEC   CURR_1x5_6
#define DEFAULT_TFH_SPEC    TH_MODE
#if DEFAULT_VOLT_SPEC==VOLT_3220 || DEFAULT_VOLT_SPEC==VOLT_3577
  #define DEFAULT_PARSE_SPEC  PARSE_341
#else
  #define DEFAULT_PARSE_SPEC  PARSE_331
#endif
#define DEFAULT_AC_PULSE        5000
#define DEFAULT_REAC_PULSE      5000

//硬件版本配置
#define HARD_TYPE_20081005     1   //第一版驱动：
#define HARD_TYPE_20090224     2   //第二版驱动：最终版，中间的乱七八糟版本废除！！！

#define METER_HARD_TYPE   HARD_TYPE_20090224   //-------PUCK


//表计属性配置宏
#define PARSE_METER_3   0               //通用三相多功能
#define IC_METER        1               //IC卡预付费表
#define NETMETER_METER  2               //网络表基表
#define MWRWE_HARD_TYPE NETMETER_METER

//LCD属性
#define LCD_NORMAL           0         //通用液晶
#define LCD_JS_ZXW           1          //江苏液晶---中显威
#define LCD_JS_JX            2           //江苏液晶---晶像

#define LCD_PROPERTY  LCD_JS_ZXW


//串口属性
#if MWRWE_HARD_TYPE>=NETMETER_METER
  #define UART1_EN                         //串口1使能
#else
  #undef UART1_EN                          //串口1禁止
#endif



#endif
