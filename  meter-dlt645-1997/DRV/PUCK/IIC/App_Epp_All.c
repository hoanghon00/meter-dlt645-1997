#include "DrvPub.h"



/**********************************************************************************
函数功能：根据从器件页数号，获取从器件中高字节地址
入口：
出口：1----------------成功；0----------------失败
**********************************************************************************/  
INT8U Get_Modi_Value(INT8U PageNo)
{  
  switch(PageNo)
  {
    case 1:   //当前操作的是第2大页
        return 0x20;
    case 2:   //当前操作的是第3大页
       return 0x60;
    case 3:   //当前操作的是第4大页
       return 0xe0;
    default:   //当前操作的是第1大页
       return 0x00;;
  }
}
/**********************************************************************************
函数功能：
入口：
出口：1-------------成功；0----------失败
**********************************************************************************/   
INT8U Write_EEPROM_Dvice_PUCK(INT8U Mem_ID,INT16U SubAdr,INT16U size,INT8U *src)
{
  INT8U CmdCode,Type;

  if(size==0)return 0;
        
        switch(Mem_ID)
        {
#ifdef ID_MEM_IIC_U10
        case ID_MEM_IIC_U10:
          Type=U10_TYPE;
          CmdCode=U10_Slvadr;
        break;
#endif
#ifdef ID_MEM_IIC_U11
        case ID_MEM_IIC_U11:
          Type=U11_TYPE;
          CmdCode=U11_Slvadr;
        break;
#endif
#ifdef ID_MEM_IIC_U12
        case ID_MEM_IIC_U12:
          Type=U12_TYPE;
          CmdCode=U12_Slvadr;
        break;   
#endif
#ifdef ID_MEM_IIC_U14
        case ID_MEM_IIC_U14:
          Type=U14_TYPE;
          CmdCode=U14_Slvadr;
        break;
#endif
#ifdef ID_MEM_IIC_U15
        case ID_MEM_IIC_U15:
          Type=U15_TYPE;
          CmdCode=U15_Slvadr;
        break; 
#endif
#ifdef ID_MEM_IIC_CAL   //模拟的校表I2C口 
       case ID_MEM_IIC_CAL: 
        Type=Write_Soft_EEPROM(IIC_SOFT_ID1,UCAL_Slvadr,SubAdr,size,UCAL_TYPE,src);
        return Type;
#endif
        default:
          return 0;
        }        
        
        Type=Write_Hard_EEPROM(CmdCode,SubAdr,size,Type,src);
        return Type;
}
/**********************************************************************************
函数功能：
入口：
出口：1-------------成功；0----------失败
**********************************************************************************/   
INT8U Read_EEPROM_Dvice_PUCK(INT8U Mem_ID,INT16U SubAdr,INT16U size,INT8U *dst)
{
  INT8U CmdCode,Type;

  if(size==0)return 0;
        
        switch(Mem_ID)
        {
#ifdef ID_MEM_IIC_U10
        case ID_MEM_IIC_U10:
          Type=U10_TYPE;
          CmdCode=U10_Slvadr;
        break;
#endif
#ifdef ID_MEM_IIC_U11
        case ID_MEM_IIC_U11:
          Type=U11_TYPE;
          CmdCode=U11_Slvadr;
        break;
#endif
#ifdef ID_MEM_IIC_U12
        case ID_MEM_IIC_U12:
          Type=U12_TYPE;
          CmdCode=U12_Slvadr;
        break;   
#endif
#ifdef ID_MEM_IIC_U14
        case ID_MEM_IIC_U14:
          Type=U14_TYPE;
          CmdCode=U14_Slvadr;
        break;
#endif
#ifdef ID_MEM_IIC_U15
        case ID_MEM_IIC_U15:
          Type=U15_TYPE;
          CmdCode=U15_Slvadr;
        break; 
#endif
        
#ifdef ID_MEM_IIC_CAL         //模拟的校表I2C口 
        case ID_MEM_IIC_CAL:
          Type=Read_Soft_EEPROM(IIC_SOFT_ID1,UCAL_Slvadr,SubAdr,UCAL_TYPE,size,dst);
          return Type;
#endif       
        default:
          return 0;
        }
      
        Type=Read_Hard_EEPROM(CmdCode,SubAdr,Type,size,dst);
        return Type;
}
