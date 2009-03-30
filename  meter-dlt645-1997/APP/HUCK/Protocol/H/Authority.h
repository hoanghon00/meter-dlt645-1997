#ifndef AUTHORITY_H
#define AUTHORITY_H

#undef EXT
#ifdef AUTHORITY_C
#define EXT 
#else
#define EXT extern 
#endif

//定义645协议的控制码
#define C_READ_DATA  0x01       //读数据
#define C_READ_FOLLOW_DATA 0x02 //读后续数据
#define C_SET_DATA 0x04         //写数据
#define C_ADJUST_METER 0x05     //校表
#define C_ADJUST_TIME 0x08      //广播校时
#define C_SET_ADDR 0x0A         //写设备地址
#define C_SET_COM_BAUD 0x0C     //设通信速度
#define C_SET_PSW 0x0F          //修改密码
#define C_CLR_ALL 0x09          //总清0或者冻结命令
#define C_CLR_DEMAND 0x10       //清需量
#define C_CLR_EVENT 0x0B        //清过程(事件)
#define C_CLR_STAT 0x0D         //请电压合格率(统计)

/*
逻辑板J1短接	√	△	△	×	×	×
编程允许	√	√	×	√	△	√
密码级别及密码	△	√	√	√	√	×
用户权限	△	√	√	×	×	√
操作结果	1	1	2	2	2	3

操作结果：
1：正确设置。2：返回错误，但不累加密码错误次数。3：返回错误，且密码错误次数加1。
*/

//数据的密码标志
#define PSW_NULL  0xFF//没有密码类型
#define PSW_FACTORY 0//超级表内跳线跳上才能设置的参数
#define PSW_0 1  //0级密码
#define PSW_1 2  //1级密码
#define PSW_2 3  //2级密码
#define PSW_3 4  //3级密码，协议中暂未用


//根据密码重要级别不一样而进行的数据分类
#define PSW_NULL_FLAG 0xFF
#define PSW_SET_PSW0  1     //0级密码权限
#define PSW_SET_PSW1  2     //1级密码权限
#define PSW_SET_PSW2  3     //2级密码权限
#define PSW_ADJ_TIME  4     //广播校时
#define PSW_SET_DATE  5     //设置日期
#define PSW_SET_TIME  6     //设置时间
#define PSW_CLR_ALL   7     //数据总清楚
#define PSW_CLR_DATA  8     //清需量、过程、电压合格率
#define PSW_SET_ENSTART 9   //设置底度数
#define PSW_SET_PARA 10     //设置普通参数(其他参数)
#define PSW_SET_MODE45 11   //设置模式字4、5
#define PSW_SET_MODE678 12  //设置模式字6、7、8
#define PSW_FACTORY_STATUS 13  //标记编程状态
#define PSW_RD_ONLY 14 //参数只读，没有写权限

//用户权限控制字
typedef struct{
  INT16U PSW[3];
  INT8U CS[CS_BYTES];//校验和  
}S_User_Authority_Ctrl;//用户权限控制字

//定义密码Flag到在用户权限控制字中的位置的映射表
typedef struct{
  INT8U PSW_Flag;
  INT8U Posi;
}S_PSW_Map;

//定义密码权限状态
typedef struct{
  INT8U Head;
  INT8U PSW[3][3];//当前的三级密码，每级密码3个字节
  INT8U CS[CS_BYTES];//校验和
  INT8U Tail;
}S_PSW_Status;

#ifdef AUTHORITY_C
const S_PSW_Map PSW_Map[]=
{
  //第一个用户权限字
  { PSW_SET_PSW0,  7}, //0级密码权限
  { PSW_SET_PSW1,  6}, //1级密码权限
  { PSW_SET_PSW2,  5}, //2级密码权限
  { PSW_ADJ_TIME,  4}, //广播校时
  { PSW_SET_DATE,  3}, //设置日期
  { PSW_SET_TIME,  2}, //设置时间
  //第二个用户权限字  
  { PSW_CLR_ALL,   14},  //数据总清楚
  { PSW_CLR_DATA,  13},  //清需量、过程、电压合格率
  { PSW_SET_ENSTART, 12},//设置底度数
  { PSW_SET_PARA, 10},   //设置普通参数(其他参数)
  { PSW_SET_MODE45, 9},   //设置模式字4、5
  { PSW_SET_MODE678, 8}   //设置模式字6、7、8
};
#endif

EXT volatile S_User_Authority_Ctrl User_Authority_Ctrl;//用户权限控制字
EXT volatile S_PSW_Status Cur_PSW;

EXT INT8U Check_Debug_En();
EXT INT8U Check_Set_ADDR_Key();
EXT void Clear_PSW_Err_Info();
EXT INT8U Check_Clear_Data_Authority();
EXT INT8U Check_Meter_Factory_Status();
EXT INT8U Check_Meter_Prog_Status();
EXT INT8U Get_User_Authority(INT8U Ctrl_Code,INT8U *pSrc,INT8U SrcLen,INT8U *pErr);
EXT INT8U Get_Protocol_Data_Authority(INT8U Ch,INT8U Ctrl_Code,INT8U *pSrc,INT8U SrcLen);
#endif
