#ifndef CPU_CFG_H
#define CPU_CFG_H



#ifndef CPU_PUB_C
#define CPU_PUB_EXT  extern
#else
#define CPU_PUB_EXT 
#endif




#define RUN_MODE        0    //CPU在主电源下全速运行
#define HALT_MODE       1    //CPU睡眠模式
#define FX1_RUN_MODE     2    //CPU在后备电源下，以fx时钟全速运行
#define FXT_RUN_MODE    3    //CPU在后备电源下，以fxt时钟慢速运行


INT8U  Switch_Main_Osc(INT8U  Mode);
#endif
