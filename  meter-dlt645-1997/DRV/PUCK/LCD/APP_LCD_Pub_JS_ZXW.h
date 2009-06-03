#ifndef HTLCDPUB_JS_ZXW_h
#define HTLCDPUB_JS_ZXW_h

#ifndef HTLCDPUB_JS_ZXW_EXT
#define HTLCDPUB_C_JS_ZXW extern
#else
#define HTLCDPUB_JS_ZXW_EXT
#endif




#define MAX_SEG                 32                 //最大的SEG序号:0~MAX_SEG
#define MAX_COL                 7                  //最大的COL序号:0~MAX_COL



#define SEG(x)                  x                 //0~MAX_SEG
#define COM(y)                  y            //0~MAX_COL

#define Sx       1  
#define Dx       2  
#define Nx       3  


//显示设备标示------Sx---------1~255
#define S_DANGQIAN             1         //当前
#define S_SHANG                2         //上
#define S_SHIYUE               3         //1
#define S_YUE                  4         //月
#define S_CI                   5        //次
#define S_I1                   6        //I
#define S_I2                   7        //II
#define S_I3                   8        //III
#define S_V                    9        //IV
#define S_FAN                  10        //反
#define S_ZHENG                11        //正
#define S_XIANG                12        //向
#define S_WU                   13        //无
#define S_YOU                  14        //有
#define S_GONG                 15        //功
#define S_ZONG                 16        //总
#define S_FEIlLV               17        //费率
#define S_XU                   18        //需
#define S_DIAN                 19        //电
#define S_LIANG                20        //量
#define S_XIANGXIAN1           21        //第1象限符号
#define S_XIANGXIAN2           22        //第2象限符号
#define S_XIANGXIAN3           23        //第3象限符号
#define S_XIANGXIAN4           24        //第4象限符号
#define S_COS                  25        //
#define S_FAI                  26        //
#define S_A                    27         //A
#define S_B                    28         //B
#define S_C                    29         //C
#define S_N                    30         //N
#define S_JIAN_DL             31        //尖电量
#define S_FENG_DL             32        //峰电量
#define S_PIN_DL             33        //平电量
#define S_GU_DL             34        //谷电量
#define S_RI_QI                35        //日期
#define S_SHI1                 36        //失
#define S_DIAN1                37        //电
#define S_YA                   38        //压
#define S_LIU                  39        //流
#define S_SHIJIAN              40        //时间
#define S_GONGLV               41        //功率
#define S_DU                   42        //度
#define S_JIAN1                43        //尖
#define S_FENG                 44        //峰
#define S_PING                 45        //平
#define S_GU                   46        //谷
#define S_1                    47        //①
#define S_2                    48        //②
#define S_K1                   49        //③
#define S_K2                   50        //④
#define S_K3                   51        //③
#define S_TT                   52        //费率
#define S_Ua_UP                53        //Ua超上限
#define S_Ub_UP                54        //Ub超上限

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

#define S_Uc_UP                70        //Uc超上限
#define S_UPCOVERP             71        //上端盖
#define S_DOWNCOVERP           72        //下端盖
#define S_RS1                  73        //
#define S_RS2                  74        //
#define S_HONGWAI              75        //红外
#define S_NIXIANGXU            76        //逆相序
#define S_Ua                   77        //Ua
#define S_Ua_DOWN              78        //Ua超下限
#define S_Ub                   79        //Ub
#define S_Ub_DOWN              80        //Ub超下限
#define S_Uc                   81         //Uc
#define S_Uc_DOWN              82        //Uc超下限
#define S_BUTTON               83       //编程
#define S_LOCK                 84       //电表上锁
#define S_LIGHT                85     //报警
#define S_SHOT                 86     //短接
#define S_KEY                  87     //钥匙
#define S_HOUSE                88     //工厂状态
#define S_BAT1                 89    
#define S_BAT2                 90
#define S_U_NOT_EVEN           91    //平衡
#define S_BASE_TOWER           92    //基站
#define S_CSQ1                 93     //信号1
#define S_CSQ2                 94     //信号2
#define S_CSQ3                 95     //信号3
#define S_CSQ4                 96     //信号4
#define S_Ia_UP                97     //Ua超上限
#define S_Ia_                  98     //-
#define S_Ia                   99     //Ia
#define S_Ib_UP                100    //Ua超上限
#define S_Ib_                  101    //-
#define S_Ib                   102    //Ia
#define S_Ic_UP                103    //Ua超上限
#define S_Ic_                  104    //-
#define S_Ic                   105    //Ia



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
