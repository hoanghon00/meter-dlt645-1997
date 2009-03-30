#ifndef OS_CFG_H
#define OS_CFG_H

//#ifdef TEST_MODE
#define OS_TASK_NUM 5 //最大任务个数,最大不能超过255
//#else
//#define OS_TASK_NUM 4 //最大任务个数,最大不能超过255
//#endif

#define OS_SEM_NUM 16 //最大信号量个数,最大不能超过32个

#define OS_USE_LIB        1 //0表示使用自己定义的跳转函数,1表示使用库函数setjmp和longjmp
#define OS_USE_TICK_ISR   1 //0表示使用OS_Check_Tick()查询定时器的方式产生时基，1表示使用中断OS_Tick_ISR产生时基
#define OS_USE_IDLE_TASK  0 //0表示不使用空闲任务，1表示使用空闲任务,该任务可以用来统计cpu的利用率
#define OS_USE_TASK_NAME  1 //0表示不增加任务名字功能,1表示增加
#define OS_USE_CS         1 //操作系统是否采用内存变量CS校验
#define OS_RST_CHK_STK    1 //操作系统复位后是否检查复位前各任务的堆栈剩余情况,该功能需要将任务堆栈空间申明成__no_init类型
                            //主要是用于分析异常复位前的函数堆栈使用情况
                            //在创建任务的函数中，会将该任务复位前的堆栈使用情况计算放到Task_Stk_Left全局变量中
#define OS_SEM_LOCK_CHK   1 //是否对信号量锁死情况分析和报警
#define OS_SEM_CHK_LOCK_TIME 1//当超过这么长时间没有申请到信号量，则进行锁死分析，单位为妙

#define OS_STK_GROWTH     1 //设置堆栈增长方向，1表示由高地址向低地址递减，0相反

#define OS_MS_PER_TICK    10 //每个Tick间隔多少ms
#define OS_CS_BYTES       2//校验和大小
#define OS_CS_TYPE        CS_CRC16_1//快速CRC校验
#define OS_TASK_NAME_SIZE 7 //任务名字大小

#define OS_ASSERT_CTRL_EN 1//1表示断言可通过函数开关，0 表示断言功能不可通过函数开关，一直打开。正式发布时应该
#define OS_ASSERT_FUNC_EN 1
#define OS_ASSERT_HOOK_EN 0
#define OS_PRINT_TYPE     OS_PRINT_COMM//定义调试信息输出方式，暂设为标准IO输出(软仿真)
#define OS_ASSERT_NUM     10//断言函数跟踪的个数
#define OS_ASSERT_BUF_LEN 15//每个断言记录内容长度
#endif
