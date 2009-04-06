#ifndef DRV_ICCARD_H
#define DRV_ICCARD_H

#ifndef DRV_ICCARD_C
#define DRV_ICCARD_EXT extern
#else
#define DRV_ICCARD_EXT
#endif



void Read_Data_From_ICcard(INT16U Addr,INT8U *Dst,INT8U Len);
INT8U Write_Data_To_ICcard(INT16U Addr,INT8U *Src,INT8U Len);
#endif
