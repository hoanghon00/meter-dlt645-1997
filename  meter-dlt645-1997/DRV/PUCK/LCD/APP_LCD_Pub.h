#ifndef HTLCDPUB_h
#define HTLCDPUB_h

#ifndef HTLCDPUB_C
#define HTLCDPUB_EXT extern
#else
#define HTLCDPUB_EXT
#endif




#define MAX_SEG                 32                 //最大的SEG序号:0~MAX_SEG
#define MAX_COL                 7                  //最大的COL序号:0~MAX_COL



#define SEG(x)                  x                 //0~MAX_SEG
#define COM(y)                  y            //0~MAX_COL

#define Sx       1  
#define Dx       2  
#define Nx       3  


//显示设备标示------Sx---------1~255
#define S_A                    1         //A
#define S_B                    2         //B
#define S_C                    3         //C
#define S_N                    4         //N
#define S_SHANG                5         //上
#define S_BEN                  6         //本
#define S_SHANG1               7         //上
#define S_SHIYUE               8         //1
#define S_YUE                  9         //月
#define S_CI                   10        //次
#define S_SHURU                11        //输入
#define S_FAN                  12        //反
#define S_ZHENG                13        //正
#define S_XIANG                14        //向
#define S_XU                   15        //需
#define S_DIAN                 16        //电
#define S_LIANG                17        //量
#define S_SHI                  18        //时
#define S_JIAN                 19        //间
#define S_FEI                  20        //费
#define S_XIANGXIAN1           21        //第1象限符号
#define S_XIANGXIAN2           22        //第2象限符号
#define S_XIANGXIAN3           23        //第3象限符号
#define S_XIANGXIAN4           24        //第4象限符号
#define S_COS                  25        //
#define S_FAI                  26        //
#define S_I1                   27        //I
#define S_I2                   28        //II
#define S_I3                   29        //III
#define S_V                    30        //IV
#define S_ZONG                 103       //总
#define S_T                    31        //T
#define S_GUO                  32        //过
#define S_SHUCHU               33        //输出
#define S_WU                   34        //无
#define S_YOU                  35        //有
#define S_GONG0                36        //功
#define S_SHI1                  37        //失
#define S_YA                   38        //压
#define S_LIU                  39        //流
#define S_DUAN                 40        //段
#define S_GONG                 41        //功
#define S_LV                   42        //率
#define S_DU                   43        //度
#define S_JIAN1                44        //尖
#define S_FENG                 45        //峰
#define S_PING                 46        //平
#define S_GU                   47        //谷
#define S_1                    48        //①
#define S_2                    49        //②
#define S_3                    50        //③
#define S_4                    51        //④

#define S_1A1                  52        //A
#define S_1B1                  53        //B

#define S_1C1                  54        //C


#define S_M_55                 55        //MVKWHz
#define S_M_56                 56        //MVKWH

#define S_M_57                 57        //MVKWHz
#define S_M_58                 58        //MVKWHz
#define S_M_59                 59        //MVKWHz

#define S_M_60                 60        //MVKWHz

#define S_M_61                 61        //MVKWHz
#define S_M_62                 62        //MVKWHz

#define S_M_63                 63        //MVKWHz
#define S_M_64                 64        //MVKWHz
#define S_M_65                 65        //MVKWHz

#define S_M_66                 66        //MVKWHz
#define S_M_67                 67        //MVKWHz
#define S_M_68                 68        //MVKWHz

#define S_M_69                 69        //MVKWHz

#define S_CUOFENG              70        //错峰

#define S_QIANFEI              71        //欠费
#define S_DONG                 72        //冬
#define S_RS1                  73        //欠费
#define S_RS2                  74        //欠费
#define S_HONGWAI              75        //红外
#define S_NIXIANGXU            76        //逆相序

#define S_BAT1                 77 
#define S_BAT2                 78
#define S_BAT3                 79

#define S_Ua                   80     //Ua
#define S_Ub                   81     //Ub
#define S_Uc                   82     //Uc

#define S_CIRCLE               83     //圆饼

#define S_ARROW_LEFT           84     //左箭头
#define S_CSQ4                 85     //信号4
#define S_CSQ3                 86     //信号3
#define S_CSQ2                 87     //信号2

#define S_CSQ1                 88     //信号1
#define S_ARROW_RIGHT          89     //左箭头


#define S_Ia_                 90     //-
#define S_Ia                  91     //Ia
#define S_Ib_                 92     //-
#define S_Ib                  93     //Ib
#define S_Ic_                 94     //-
#define S_Ic                  95     //Ic

#define S_TT                  96     //费率
#define S_BUTTON              97     //编程
#define S_HOUSE               98     //工厂状态
#define S_KEY                 99     //钥匙
#define S_LOCK                100     //电表上锁
#define S_LIGHT               101     //报警
#define S_SHOT                102     //短接


//显示设备标示------Nx--------'8'-----256~511
#define N_LED(x)              x						//((x&0xff)|0x100)     //某一个'8'字

//显示设备标示------Dx--------'.',":"-----512~767
#define D_(x)                 x						//((x&0xff)|0x200)


void MainHTLCD_Puck_Pub(INT8U Flag);
void UpdataLcdShow(void);

//以下3个函数是LCD抽象后的函数
INT8U SetOnDevice_PUCK(INT16U Device);       //Sx
INT8U SetOnDevice_D_PUCK(INT16U Device);      //Dx
INT8U SetOnLED8Device_PUCK(INT16U DeviceID,INT8U DisChar);    //Nx
void Main_Dis_Info(char *Str);
void Init_LCD_Loop(void);
#endif
