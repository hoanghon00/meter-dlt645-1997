#ifndef OS_PUB_H
#define OS_PUB_H

#undef OS_EXT
#ifdef OS_PUB_C
#define OS_EXT
#else
#define OS_EXT extern
#endif

//定义断言,多多使用，有益身心
#if OS_ASSERT_FUNC_EN>0
#define OS_ASSERT(x) OS_Assert(x,__FILE__,__FUNCTION__,__LINE__)
#else
#define OS_ASSERT(x) OS_Assert(x,__FILE__,__LINE__)
#endif

#define OS_PRINT_NULL  0//不打印
#define OS_PRINT_STDIO 1//标准IO
#define OS_PRINT_COMM  2//通信口打印

#define CS_SUM         0//简单和校验
#define CS_CRC16_0     1//CRC16算法0
#define CS_CRC16_1     2//CRC16算法1

#define OS_CHECK_STRUCT_SUM(Var) OS_Check_STRUCT_Sum((void *)&(Var),sizeof(Var),(OS_INT8U *)(Var.CS),sizeof(Var.CS))
#define OS_SET_STRUCT_SUM(Var) OS_Set_STRUCT_Sum((void *)&(Var),sizeof(Var),(OS_INT8U *)(Var.CS),sizeof(Var.CS))

#define OS_CHECK_STRUCT_VAR(Var) ((CHK_BYTE==Var.Head  && CHK_BYTE==Var.Tail)?1:0)
#define OS_INIT_STRUCT_VAR(Var)  do{Var.Head=CHK_BYTE;Var.Tail=CHK_BYTE;}while(0)

//打印缓冲区
//#define OS_DEBUG_BUF_LEN 500
//OS_EXT OS_INT8S OS_Debug_Buf[OS_DEBUG_BUF_LEN];

#define CHK_CS_BYTES 0xAAAAAAAA//用于生成校验的特征字

//将打印相关变量设置成__no_init类型，可以在非正常复位后追溯复位前的调用流程
typedef struct
{
  OS_INT8U Head;
  
  OS_INT8S File[OS_ASSERT_NUM][OS_ASSERT_BUF_LEN];
  OS_INT8U Task_ID[OS_ASSERT_NUM];//调用的任务号
  OS_INT16U Line[OS_ASSERT_NUM];//调用的行号
  OS_INT16U Counts[OS_ASSERT_NUM];//调用的次数
  OS_INT8U Index;//当前调用索引
  OS_INT8U Flag;//是否进入了断言函数
  OS_INT32U On_Off_Flag;//断言开关标志
  
  OS_INT8U Tail;
}S_Assert_Info;

OS_EXT NO_INIT S_Assert_Info Assert_Info;

//外部完成调试信息输出函数
extern OS_INT8U Debug_Out_Public_Puck(OS_INT8U *Srcbuf,OS_INT16U Src_Len);

#if OS_ASSERT_FUNC_EN>0
OS_EXT OS_INT8U OS_Assert(OS_INT8U Condition,CONST OS_INT8U File[],CONST OS_INT8S Function[],OS_INT16U Line);
#else
OS_EXT OS_INT8U OS_Assert(OS_INT8U Condition,CONST OS_INT8U File[],OS_INT16U Line);
#endif

OS_EXT void OS_Debug_Print(CONST OS_INT8S *format, ...);
OS_EXT void OS_Assert_Ctrl(OS_INT32U Flag);
//OS_EXT OS_INT8U OS_Assert(OS_INT8U Conditon,CONST OS_INT8U File[],CONST OS_INT8S Function[],OS_INT16U Line);
OS_EXT void OS_memcpy(void *pDst, void *pSrc,OS_INT32U SrcLen,void *pDst_Start,OS_INT32U DstLen);
OS_EXT void OS_memset(void *pDst, OS_INT8U Value,OS_INT32U SetLen,void *pDst_Start,OS_INT32U DstLen);

OS_EXT OS_INT8U OS_Check_Sum(void *pSrc,OS_INT16U SrcLen,void *pCS,OS_INT8U CS_Bytes);
OS_EXT void OS_Set_Sum(void *pSrc,OS_INT16U SrcLen,void *pDst,OS_INT8U CS_Bytes,void *pDst_Start,OS_INT16U DstLen);

OS_EXT OS_INT8U OS_Check_STRUCT_Sum(void *pSrc,OS_INT16U SrcLen,OS_INT8U *pCS,OS_INT8U CS_Bytes);
OS_EXT void OS_Set_STRUCT_Sum(void *pSrc,OS_INT16U SrcLen,OS_INT8U *pCS,OS_INT8U CS_Bytes);

#endif
