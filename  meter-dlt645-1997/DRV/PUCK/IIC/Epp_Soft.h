#ifndef SOFT_I2C_CFG_H
#define SOFT_I2C_CFG_H

#ifdef ID_MEM_IIC_CAL     //宏开启，打开校表口的模拟 I2C功能
    #define SET_CAL_SCLK_OUTPUT  PM0_bit.no2=0
    #define SET_CAL_SCLK_INPUT   PM0_bit.no2=1
    
    #define SET_CAL_SDA_OUTPUT   PM0_bit.no1=0
    #define SET_CAL_SDA_INPUT    PM0_bit.no1=1
    
    #define CLR_CAL_SCLK_0       P0_bit.no2=0    //将SCL总线置0---------PUCK   
    #define SET_CAL_SCLK_1       P0_bit.no2=1    //将SCL总线置1---------PUCK
    
    #define CLR_CAL_SDA_0        P0_bit.no1=0     //将SDA总线置0---------PUCK
    #define SET_CAL_SDA_1        P0_bit.no1=1     //将SDA总线置1---------PUCK
    
    #define GET_CAL_STATUS       (P0_bit.no1)
    
    void Set_Cal_Sclk_Dir(INT8U Dir);
    void Set_Cal_Sda_Dir(INT8U Dir);
    void Set_Cal_Sclk_Pin(INT8U Level);
    void Set_Cal_Sda_Pin(INT8U Level);
    INT8U Get_Cal_Sda_Pin(void);
#endif

#endif





INT8U I2cSoft_Read_nByteS(INT8U Id,INT16U Len,INT8U *Dst);
INT8U  Write_Soft_EEPROM(INT8U VirtualID,INT8U SlvAdr,INT16U SubAddr,INT16U size,INT8U Type,INT8U *Src);
INT8U  Read_Soft_EEPROM(INT8U VirtualID,INT8U SlvAdr,INT16U SubAddr,INT8U Type,INT16U size,INT8U *Dst);