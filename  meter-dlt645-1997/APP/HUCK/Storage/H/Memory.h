#ifndef MEMORY_H
#define MEMORY_H

#undef EXT
#ifdef MEMORY_C
#define EXT
#else
#define EXT extern
#endif

//#ifdef MEMORY_C
///EXT volatile INT8U Rom_Err_Flag=0;
//#endif

//#ifdef MEMORY_C
//EXT S_Int16U Clr_Dog_Counts={CHK_BYTE,0x00,CHK_BYTE};
//#endif
//extern S_Int16U Clr_Dog_Counts;

EXT volatile INT8U Rom_Err_Flag;//用来标识哪个ROM发生了错误

EXT INT8U Check_PHY_Mem();
EXT INT8U Check_LOG_Mem_Map();
EXT INT32U Get_LOG_Mem_Size(INT8U MemNo);
EXT INT32U Get_PHY_MEM_Size(INT8U MemNo);
EXT INT16U Read_PHY_Mem(INT8U MemNo,INT32U Offset,void *pDst,INT16U RD_Len,void *pDst_Start,INT16U DstLen);
EXT INT8U Write_PHY_Mem(INT8U MemNo,INT32U Offset,void *pSrc,INT16U SrcLen);
EXT INT16U Read_LOG_Mem(INT8U MemNo,INT32U Offset,void *pDst,INT16U RD_Len,void *pDst_Start,INT16U DstLen);
EXT INT8U Write_LOG_Mem(INT8U MemNo,INT32U Offset,void *pSrc,INT16U SrcLen);
#endif
