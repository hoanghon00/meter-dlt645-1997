#ifndef DATA_FLASH_H
#define DATA_FLASH_H


#define PAGE_SIZE 528
#define PAGE_NUMBER 4096

//      cpu   <---> DataFlash
// +---------------+
// |          P4.7 | ---> DATA_FLASH_RDY
// |          P4.4 | <--- DATA_FLASH_SO
// |          P4.5 | ---> DATA_FLASH_SI
// |          P4.3 | ---> DATA_FLASH_SCL
// |          P4.2 | ---> DATA_FLASH_CS
// +---------------+
//#define DATA_FLASH_WP                              BIT2
//#define DATA_FLASH_WP_OUT                          P5
//#define  DATA_FLASH_WP_DIR                     PM5

#define DIR_FLASH_SO                               (PM4_bit.no4)
#define DATA_FLASH_SO                              (P4_bit.no4)
#define DATA_FLASH_SO_IN                           (P4_bit.no4)

#define DIR_FLASH_SI                               (PM4_bit.no5)
#define DATA_FLASH_SI                              (P4_bit.no5)
#define DATA_FLASH_SI_OUT                          (P4_bit.no5)

#define DIR_FLASH_SCL                               (PM4_bit.no3)
#define DATA_FLASH_SCL                             (P4_bit.no3)
#define DATA_FLASH_SCL_OUT                         (P4_bit.no3)

#define DIR_FLASH_CS                               (PM4_bit.no2)
#define DATA_FLASH_CS                              (P4_bit.no2)
#define DATA_FLASH_CS_OUT                          (P4_bit.no2)


//PUCK ADD:09-02-04
#define DIR_FLASH_RDY                               (PM4_bit.no7)
#define DATA_FLASH_RDY                              (P4_bit.no7)
#define DATA_FLASH_RDY_OUT                          (P4_bit.no7)

//-----------------------------------------------------------

#define DF_READ_CMD_ARRAY 0xe8
#define DF_READ_STATUS1 0xd7
#define DF_WRITE_BUFFER1 0x84
#define DF_READ_BUFFER1 0xd4
#define DF_WMM_WBIE_BUF1 0x83
#define DF_MAIN_TO_BUFFER1 0x53

#define DF_READ_STATUS2 0xD7

#define DF_READ_BUFFER2 0x56
#define DF_READ_BUFFER1_ 0xD4
#define DF_READ_BUFFER2_ 0xD6

#define DF_WRITE_BUFFER2 0x87

#define DF_WMM_WBIE_BUF2 0x86

#define DF_MAIN_TO_BUFFER2 0x55
#define DF_ARRAY_READ 0x68

#define DATA_FLASH_REFRESH_CNT 9000
#define DATA_FLASH_SECTOR_CNT 10

#define DF_ENTER_SLEEP        0xB9
#define DF_ENTER_ACTIVE       0xAB
#define DF_SLEEP 0
#define DF_ACTIVE 1

INT8U flash_read_abs(INT32U addr_abs,INT8U *buf,INT32U size);
INT8U flash_writ_abs(INT32U addr_abs,INT8U *buf,INT32U size);

#endif

