#ifndef PUB_ERR_H
#define PUB_ERR_H

#define NO_ERR 0

#define PROTOCOL_PSW_ERR (NO_ERR+0x01)    //协议密码错
#define PROTOCOL_AUTH_ERR (PROTOCOL_PSW_ERR+0x01)   //协议权限错
#define PROTOCOL_DATA_ERR (PROTOCOL_AUTH_ERR+0x01)   //协议非法数据
#define PROTOCOL_DI_ERR (PROTOCOL_DATA_ERR+0x01)     //协议数据标识错

//#define YEAR_PERIOD_ERR 0x05     //年时区数超
#define PHY_MEM_ERR (PROTOCOL_DI_ERR+0x01)
#define LOG_MEM_MAP_ERR (PHY_MEM_ERR+0x01)         //内存映射错
#define IMP_DATA_STORAGE_ERR (LOG_MEM_MAP_ERR+1)       //IMP数据存储错误
#define PROTOCOL_DATA_STORAGE_ERR (IMP_DATA_STORAGE_ERR+1) //协议数据的存储错误

#define LOG_MEM_ADDR_ERR (PROTOCOL_DATA_STORAGE_ERR+0x01) //逻辑存储器地址错
#define PHY_MEM_ADDR_ERR (LOG_MEM_ADDR_ERR+0x01)    //物理存储器地址错

//#define RD_LOG_MEM_ERR (PHY_MEM_ADDR_ERR+0x01)
//#define WR_LOG_MEM_ERR (RD_LOG_MEM_ERR+0x01)

//#define RD_PHY_MEM_ERR (WR_LOG_MEM_ERR+0x01)
//#define WR_PHY_MEM_ERR (RD_PHY_MEM_ERR+0x01)

#define LOG_MEM_NO_ERR   (PHY_MEM_ADDR_ERR+0x01)    //逻辑存储器编号错
#define PHY_MEM_NO_ERR   (LOG_MEM_NO_ERR+0x01)    //物理存储器编号错

#define RD_IMP_DATA_CS_ERR  (PHY_MEM_NO_ERR+0x01)    //读存储器数据校验和错
#define RD_IMP_DATA_ERR  (RD_IMP_DATA_CS_ERR+0x01)     //读取IMP数据失败,任何一个logmem都没有成功读到该数据
#define WR_IMP_DATA_ERR  (RD_IMP_DATA_ERR+0x01)    //写IMP失败，任何一个logmem都没有写成功该数据

#define RD_FIX_DATA_ERR (WR_IMP_DATA_ERR+0x01) //读取固定数据长度错误
#define WR_FIX_DATA_ERR (RD_FIX_DATA_ERR+0x01) //写固定数据长度错误

#define RD_HIS_DATA_LEN_ERR (WR_FIX_DATA_ERR+0x01)  //读取历史数据是的错误
#define RD_HIS_DATA_DI_ERR  (RD_HIS_DATA_LEN_ERR+0x01)     
#define RD_HIS_DATA_TIME_ERR (RD_HIS_DATA_DI_ERR+0x01)
#define RD_HIS_DATA_DATA_ERR (RD_HIS_DATA_TIME_ERR+0x01)

#define WR_HIS_DATA_LEN_ERR (RD_HIS_DATA_DATA_ERR+0x01) //写历史数据时的错误
#define WR_HIS_DATA_DI_ERR  (WR_HIS_DATA_LEN_ERR+0x01)
#define WR_HIS_DATA_TIME_ERR (WR_HIS_DATA_DI_ERR+0x01)

#define RD_STORAGE_DATA_DI_ERR (WR_HIS_DATA_TIME_ERR+0x01) //读存储数据时的错误
#define WR_STORAGE_DATA_DI_ERR (RD_STORAGE_DATA_DI_ERR+0x01) //写存储数据时的错误
#define WR_STORAGE_DATA_LEN_ERR (WR_STORAGE_DATA_DI_ERR+0x01)
#define WR_STORAGE_DATA_AUTH_ERR (WR_STORAGE_DATA_LEN_ERR+0x01)//写数据权限错误

#define WR_LOAD_DATA_ERR (WR_STORAGE_DATA_AUTH_ERR+0x01) //写负荷曲线数据格式错误
#define RD_LOAD_DATA_ERR (WR_LOAD_DATA_ERR+0x01)

#define EVENT_ID_ERR (RD_LOAD_DATA_ERR+0x01)//事件的ID错误
#define EVENT_DATA_ERR (EVENT_ID_ERR+0x01)

#define ADJ_TIME_ERR (EVENT_DATA_ERR+1)
#define SET_PSW_ERR (ADJ_TIME_ERR+1)//修改密码错误
#define SET_ADDR_ERR (SET_PSW_ERR+1)
#endif

