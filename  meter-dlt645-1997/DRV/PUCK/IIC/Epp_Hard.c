#include "DrvPub.h"
#include "Epp_Hard.h"

#pragma pack(1)
typedef struct
{ 
  INT8U   EppType:5;
  INT8U   Single:1;
  INT8U   BitFill:2;
  INT16U  PageSize;               //页内字节数
  INT32U  MaxSize;                //IC最大字节数
  INT8U   PageDlyFlg:1;           //写页内延时标志
  INT8U   PageDelay:7;            //跨页内延时时间：ms
}CONST_EPPROM_PROPERTY;

#define MAX_EPPROM_TYPE  (M24512+1)

CONST CONST_EPPROM_PROPERTY  Const_Epprom_Property[MAX_EPPROM_TYPE]={
  {M2401,1,0,8,EPP_2401_SIZE,0,0},
  {M2402,1,0,8,EPP_2402_SIZE,0,0},
  {M2404,1,1,16,EPP_2404_SIZE,1,8},
  {M2408,1,2,16,EPP_2408_SIZE,0,0},
  {M2416,1,3,16,EPP_24016_SIZE,0,0},
  {M2432,0,0,32,EPP_24032_SIZE,0,0},
  {M2464,0,0,32,EPP_24064_SIZE,0,1},
  {M24128,0,0,64,EPP_240128_SIZE,0,0},
  {M24256,0,0,64,EPP_240256_SIZE,1,8},
  {M24512,0,0,128,EPP_240512_SIZE,1,8}
};
#pragma pack()

/**********************************************************************************
函数功能：
入口：
出口：1----------------成功；0----------------失败
**********************************************************************************/   
INT8U Write_Hard_EEPROM(INT8U SlvAdr,INT16U SubAddr,INT16U size,INT8U Type,INT8U *Src)
{
    INT16U WrNum;
    INT16U Page;
    INT8U AddrH,AddrL;
    INT8U OkFlag;
    
    if(Type>MAX_EPPROM_TYPE)                              return 0;
    if(size>Const_Epprom_Property[Type].MaxSize)          return 0;
    if(SubAddr>Const_Epprom_Property[Type].MaxSize)        return 0;        
    if((size+SubAddr)>Const_Epprom_Property[Type].MaxSize) return 0;  
    
    
    IICE0=1;               //打开II2C操作时钟，因有可能会关闭-----------PUCK
#ifdef HARD_I2C_ENV
    if(setjmp(Hard_I2c_Env.Env)==0)
    {
#endif     
    IIC_HARD_WP_EN;       //开启写操作
    WAITFOR_DRV_CYCLE_TIMEOUT(100);
    WrNum=0;
    AddrH=(INT8U)(SubAddr/256);
    AddrL=(INT8U)(SubAddr%256);
    do{
        OkFlag=I2cStartCondition();	
        Page=AddrL+WrNum; 
        if(Const_Epprom_Property[Type].Single) //地址为一个字节的器件------PUCK
        {
          if(Const_Epprom_Property[Type].BitFill==0) //此器件地址是固定的
            I2cPutAddress(SlvAdr); 
          else                                      //地址需要在slave上修改------PUCK
          { 
            switch((SubAddr+WrNum)/256)
            {
              case 1:
                  if(Const_Epprom_Property[Type].BitFill>=1)
                    OkFlag&=I2cPutAddress(SlvAdr|0x20); 
                  break;
              case 2:
                if(Const_Epprom_Property[Type].BitFill>=2)
                    OkFlag&=I2cPutAddress(SlvAdr|0x60); 
                  break;
              case 3:
                if(Const_Epprom_Property[Type].BitFill>=3)
                    OkFlag&=I2cPutAddress(SlvAdr|0xe0); 
                  break;
              default:
                  OkFlag&=I2cPutAddress(SlvAdr); 
                  break;
            }
          }          
        }
        else
        {
          OkFlag&=I2cPutAddress(SlvAdr); 
          OkFlag&=I2cPutData(AddrH+(Page>>8));          
        }       
        
        OkFlag&=I2cPutData((Page&0x00FF)) ;	
        do
        {
           OkFlag&=I2cPutData(*Src); 					
           Src++;WrNum++;
        }while((WrNum<size)&&(((AddrL+WrNum)&(Const_Epprom_Property[Type].PageSize-1))!=0));
        
        I2cStopCondition();
        if(Const_Epprom_Property[Type].PageDlyFlg)
          WAITFOR_DRV_MS_TIMEOUT(Const_Epprom_Property[Type].PageDelay)
        else
          WAITFOR_DRV_CYCLE_TIMEOUT(100)  //没有延时标志，延时100个nop
        }while(WrNum<size);
        IIC_HARD_WP_DIS;     //置WP口线为高，禁止写--------PUCK
#ifdef HARD_I2C_ENV
    return 1;
    }
   else
#else
   if(!OkFlag)
#endif
   {
      IIC_HARD_WP_DIS;     //置WP口线为高，禁止写--------PUCK
      I2cReset();
      OkFlag=0;
   }
    return OkFlag;
}

/**********************************************************************************
函数功能：
入口：
出口：1----------------成功；0----------------失败
**********************************************************************************/
INT8U Read_ChangeAddr_EEPROM( INT8U SlvAdr,INT16U SubAddr,INT8U Type,INT16U size,INT8U *Dst)
 {
   INT8U NackData;
   INT8U AddrL,OkFlag;
  
   AddrL=(INT8U)(SubAddr%256);
#ifdef HARD_I2C_ENV
   if(setjmp(Hard_I2c_Env.Env)==0)
   { 
#endif   
   IICE0 = 1;  //打开II2C操作时钟，因有可能会关闭-----------PUCK
   WAITFOR_DRV_CYCLE_TIMEOUT(50);
   OkFlag=I2cStartCondition();
   OkFlag&=I2cPutAddress(SlvAdr);     
   OkFlag&=I2cPutData(AddrL);
   I2cStart();   //产生起始条件------------PUCK
   OkFlag&=I2cPutData(SlvAdr|0x1);  //读操作-----------PUCK
   for(;size>0;size--)
   {
     OkFlag&=I2cGetDataAckSet(Dst);
      Dst++;
   }	  
   OkFlag&=I2cGetDataNackSet(&NackData); 
   I2cStopCondition();
#ifdef HARD_I2C_ENV
    return 1;
   }
   else
#else
    if(!OkFlag)
#endif
  {
     I2cReset();
     OkFlag=0;
   }
   return OkFlag;
}


/**********************************************************************************
函数功能：
入口：
出口：1----------------成功；0----------------失败
**********************************************************************************/
INT8U Read_FixAddr_EEPROM( INT8U SlvAdr,INT16U SubAddr,INT8U Type,INT16U size,INT8U *Dst)
{
    INT8U NackData;
    INT8U AddrH,AddrL,OkFlag;
     
    AddrH=(INT8U)(SubAddr/256);
    AddrL=(INT8U)(SubAddr%256);
  
#ifdef HARD_I2C_ENV
   if(setjmp(Hard_I2c_Env.Env)==0)
   { 
#endif    
    
    IICE0 = 1;  //打开II2C操作时钟，因有可能会关闭-----------PUCK
    WAITFOR_DRV_CYCLE_TIMEOUT(50);
    OkFlag=I2cStartCondition();
    OkFlag&=I2cPutAddress(SlvAdr);
    
    if(Const_Epprom_Property[Type].Single==0) 
      OkFlag&=I2cPutData(AddrH);
    OkFlag&=I2cPutData(AddrL);
    I2cStart();   //产生起始条件------------PUCK
    OkFlag&=I2cPutData(SlvAdr+1);  //读操作-----------PUCK
    for(;size>0;size--)
    {
      OkFlag&=I2cGetDataAckSet(Dst);
       Dst++;
    }	  
    OkFlag&=I2cGetDataNackSet(&NackData); 
    I2cStopCondition();
#ifdef HARD_I2C_ENV
    return 1;
   }
   else
#else
    if(!OkFlag)
#endif
    {
      I2cReset();
      OkFlag=0;
    }
    return OkFlag;
}

/**********************************************************************************
函数功能：
入口：
出口：1----------------成功；0----------------失败
**********************************************************************************/
INT8U Read_Hard_EEPROM( INT8U SlvAdr,INT16U SubAddr,INT8U Type,INT16U size,INT8U *Dst)
{
    INT8U Flag;

  
   if(Type>MAX_EPPROM_TYPE)                              return 0;
   if(size>Const_Epprom_Property[Type].MaxSize)          return 0;
   if(SubAddr>Const_Epprom_Property[Type].MaxSize)        return 0;        
   if((size+SubAddr)>Const_Epprom_Property[Type].MaxSize) return 0;  
   
   if((Const_Epprom_Property[Type].Single)&&(Const_Epprom_Property[Type].BitFill))  //M2404/M2408/M2416,目前只实现M2404----------PUCK
   {
     if(SubAddr+size<256) //第一页内零头
     {
       Flag=Read_ChangeAddr_EEPROM(SlvAdr,SubAddr,Type,size,Dst);
       return Flag;
     }
     else if(SubAddr>=256) //第二页内零头
     {
       SlvAdr|=0x20;
       Flag=Read_ChangeAddr_EEPROM(SlvAdr,SubAddr,Type,size,Dst);
       return Flag;
     }
     else
     {
       Flag=Read_ChangeAddr_EEPROM(SlvAdr,SubAddr,Type,256-SubAddr,Dst);
       Flag&=Read_ChangeAddr_EEPROM(SlvAdr|0x20,256,Type,size-256+SubAddr,Dst+256-SubAddr);
       return Flag;
     }
   }
   else
     Flag=Read_FixAddr_EEPROM(SlvAdr,SubAddr,Type,size,Dst);
  
     return Flag; 
}