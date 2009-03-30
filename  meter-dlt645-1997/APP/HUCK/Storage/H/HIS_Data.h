#ifndef HIS_DATA_H
#define HIS_DATA_H

#undef EXT
#ifdef HIS_DATA_C
#define EXT
#else
#define EXT extern
#endif

EXT INT32U Calc_HIS_Data_DI(INT8U Data_Flag,S_HEX_Time *pTime);
EXT INT16U Read_HIS_Data(INT8U Data_Flag,S_HEX_Time *pTime,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr);
EXT INT8U Write_HIS_Data(INT8U Data_Flag,S_HEX_Time *pTime,void *pSrc,INT16U SrcLen);
EXT INT16U Read_Demand_HIS_Data(INT8U Rate,S_HEX_Time *pTime,void *pDst,void *pDst_Start,INT16U DstLen,INT8U *pErr);
EXT INT8U Write_Demand_HIS_Data(INT8U Rate,S_HEX_Time *pTime,void *pSrc,INT16U SrcLen);

#endif

