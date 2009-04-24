#ifndef METER_CFG_H
#define METER_CFG_H

#ifdef METER_CFG_C
#define EXT
#else
#define EXT extern
#endif

//#define TEST_MODE

#define CS_BYTES          2//内存中重要数据结构体的CS字节数  
#define ROM_CS_BYTES      2//ROM中存储数据的校验和字节数
#define MAX_RATES         8///最大费率数
#define HIS_ENERGY_DATA_MONTHS  13//保存历史数据的月份数,最好不要超过12！
#define HIS_DEMAND_DATA_MONTHS  13
#define HIS_VOLSTAT_DATA_MONTHS 3//历史电压统计数据的保存月数
#define LOAD_DATA_NUM        8000//存储的负荷曲线条数

#define USE_ENERGY_RAM_BAK   1//是否在内存中使用一份电量数据的备份

#define MAX_YEAR_PERIODS   12//最大年时区数,年时区表示将一年分为几个时间段
#define MAX_DATE_TABLES     8//最大日时段表数,日时段表示将一天分成几个时间段
#define MAX_DATE_PERIODS   14//每个时段表最大时段数
#define MAX_YEAR_HOLIDAYS  90//每个年时区表对应的最多节假日数,应该为15的倍数

#define ADJ_TIME_EVENT_NUM 50
#define ALL_LOSS_CUR_EVENT_NUM 50

#define SOFT_VERSION {0x11,0x14,0x24,0x04,0x09}
#endif
