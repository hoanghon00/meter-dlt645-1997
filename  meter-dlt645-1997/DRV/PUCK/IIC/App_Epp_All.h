


#define DIS_ALL_HARD_I2C_WP       IIC_HARD_WP_DIS            //关闭所有硬件I2C的WP

#define DIS_ALL_SOFT_I2C_WP                  //关闭所有软件I2C的WP



INT8U Get_Modi_Value(INT8U PageNo);
//U10:地址冲突，目前程序没有实现
INT8U Write_EEPROM_Dvice_PUCK(INT8U Mem_ID,INT16U SubAdr,INT16U size,INT8U *src);
INT8U Read_EEPROM_Dvice_PUCK(INT8U Mem_ID,INT16U SubAdr,INT16U size,INT8U *src);