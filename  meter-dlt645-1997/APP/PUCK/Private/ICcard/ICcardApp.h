#ifndef APP_ICCARD_H
#define APP_ICCARD_H

#ifndef APP_ICCARD_C
#define APP_ICCARD_EXT extern
#else
#define APP_ICCARD_EXT
#endif


#define METER_ICCARD_EN Mode_Word.Mode[1].Bit.Bit6  //模式字2: b6---IC卡功能:1:有 ;0:无


#endif