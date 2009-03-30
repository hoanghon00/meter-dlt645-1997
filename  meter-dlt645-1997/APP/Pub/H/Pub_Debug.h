#ifndef PUB_DEBUG_H
#define PUB_DEBUG_H

#include "Pub.h"

#undef EXT
#ifdef PUB_DEBUG_C
#define EXT
#else
#define EXT extern
#endif

//定义调试信息输出方式
#define PRINT_NULL  0 //不输出
#define PRINT_STDIO 1 //标准I/O输出
#define PRINT_UART  2 //串口输出

#define DEBUG_PRINT(x,y,...) Debug_Print(__VA_ARGS__)

#define PRINT_TYPE PRINT_UART //暂定为标准IO输出

//定义打印缓冲时的打印格式
#define PRINT_HEX 0 //16进制打印
#define PRINT_DEC 1 //10进制打印

//定义调试信息的重要等级
#define DEBUG_0 0
#define DEBUG_1 1
#define DEBUG_2 2
#define DEBUG_3 3 

//定义调试信息输出者ID,修改此处时请注意修改PARA_ID_NUM
#define ASSERT_ID 0
#define TEST 1
#define HUCK 2
#define SUCK 3
#define PUCK 4
#define LUCK 5

#define A_RST 0 //表示断言中的条件不成立的话，复位
#define A_WARNING 1//表示断言中条件不成立的话，只输出调试信息，然后返回1

//定义断言,多多使用，有益身心
//#define ASSERT(x,y) Assert(x,(y),__FILE__,__FUNCTION__,__LINE__)


EXT INT16U DEBUG_PRINT(INT8U ID,INT8U Debug_Flag,char *format,...);
EXT INT8U DEBUG_BUF_PRINT(INT8U Src[],INT16U SrcLen,INT8U Flag,INT8U Num);
#endif
