#define MEM_ABS_PUCK
#include "Pub_PUCK.h"
#include "Includes.h"


/**********************************************************************************
函数功能：
入口：
出口：1----------------成功；0----------------失败
**********************************************************************************/ 
INT8U Read_PHY_Mem_PUCK(INT8U MemNo,INT32U Offset,void *pDst,INT16U RD_Len,void *pDst_Start,INT16U DstLen)
{
  INT8U Flag;
  
  if((INT8U *)pDst<(INT8U *)pDst_Start || (INT8U *)pDst_Start+RD_Len > (INT8U *)pDst_Start+DstLen)
  ASSERT(A_WARNING,0);
  
  Flag=0;
  switch(MemNo)
  {
  case PHY_MEM0:
    Flag=Read_EEPROM_Dvice_PUCK(ID_MEM_IIC_U14,Offset,RD_Len,pDst);
    break;  
  case PHY_MEM1:
    Flag=Read_EEPROM_Dvice_PUCK(ID_MEM_IIC_U10,Offset,RD_Len,pDst);
    break; 
    
  case PHY_MEM2:
   Flag=Read_EEPROM_Dvice_PUCK(ID_MEM_IIC_U11,Offset,RD_Len,pDst);
   break;   
    
  case PHY_MEM3:
    Flag=Read_EEPROM_Dvice_PUCK(ID_MEM_IIC_U12,Offset,RD_Len,pDst);
   break;
    
  case PHY_MEM4:
   Flag=Read_EEPROM_Dvice_PUCK(ID_MEM_IIC_U15,Offset,RD_Len,pDst);
   break;  
  
  case PHY_MEM5:
  if(POWER_OFF_STATUS==0)  //掉电了
    return 0;
     
   Flag=flash_read_abs(Offset,pDst,RD_Len); 
   break;  
  default:
    return 0;
  }
  
  if(!Flag)
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEM_EN,"Memory Read Error:ID=%d",MemNo);
  
  return Flag; 
}

/**********************************************************************************
函数功能：
入口：
出口：1----------------成功；0----------------失败
**********************************************************************************/ 
INT8U Write_PHY_Mem_PUCK(INT8U MemNo,INT32U Offset,void *pSrc,INT16U SrcLen)
{
  INT8U Flag;
  
  Flag=0;
  switch(MemNo)
  {
  case PHY_MEM0:
    Flag=Write_EEPROM_Dvice_PUCK(ID_MEM_IIC_U14,Offset,SrcLen,pSrc);
    break;  
  case PHY_MEM1:
    Flag=Write_EEPROM_Dvice_PUCK(ID_MEM_IIC_U10,Offset,SrcLen,pSrc);
    break; 
    
  case PHY_MEM2:
   Flag=Write_EEPROM_Dvice_PUCK(ID_MEM_IIC_U11,Offset,SrcLen,pSrc);
   break;   
    
  case PHY_MEM3:
    Flag=Write_EEPROM_Dvice_PUCK(ID_MEM_IIC_U12,Offset,SrcLen,pSrc);
   break;
    
  case PHY_MEM4:
   Flag=Write_EEPROM_Dvice_PUCK(ID_MEM_IIC_U15,Offset,SrcLen,pSrc);
   break;  
  
  case PHY_MEM5: 
   if(POWER_OFF_STATUS==0)  //掉电了
    return 0;
   Flag=flash_writ_abs(Offset,pSrc,SrcLen);
   break;
   
  default:
    return 0;    
  }
  
  if(!Flag)
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEM_EN,"Memory Write Error:ID=%d",MemNo);
  return Flag; 
}