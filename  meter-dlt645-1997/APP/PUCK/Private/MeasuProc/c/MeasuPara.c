
#define EEPROM_PUCK
#include "Pub_PUCK.h"

/**********************************************************************************
函数功能：根据buf，按照1/2/4自己的选择得出校验和
入口：
Src-------------源buf
SrcLen----------源buf的字节长度
ByteGroup-------以1/2/4自己为整体算出校验和
Dst-------------目标缓冲区
DstLen-----------目标缓冲区的字节长度
出口：1---------读成功，读比较正确;0--------表示失败
**********************************************************************************/
INT8U Get_Buf_MulitByte_Sum(INT8U *Src,INT16U SrcLen,INT8U ByteGroup,INT8U *Dst,INT8U DstLen)
{
  //INT8U Flag;
  INT8U temp1;
  INT16U i;
 
  /*
  Flag=ASSERT(A_WARNING,(ByteGroup==1)); 
  Flag|=ASSERT(A_WARNING,ByteGroup==DstLen);  
  Flag|=ASSERT(A_WARNING,(SrcLen%ByteGroup)==0);    
  */
  if(ByteGroup!=1 || ByteGroup!=DstLen || (SrcLen%ByteGroup)) //整数字节
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Get_Buf_MulitByte_Sum Function Error!");   

  
  temp1=0;
   for(i=0;i<SrcLen/ByteGroup;i++)
  {
    switch(ByteGroup)
    {
    case 1:
      temp1+=*(Src+i);
      break;
    case 2:
      break;
    case 4:
      break;
    }    
  }
  
  switch(ByteGroup)
  {
    case 1:
      *Dst=temp1;
      break;
    case 2:
      //mem_cpy(Dst,Dst,2,(INT8U *)(&temp2),2);
      break;
    case 4:
      //mem_cpy(Dst,Dst,4,(INT8U *)(&temp3),4);
      break;
  }    
  return 1;
}

/**********************************************************************************
函数功能：读取参数
入口：Flag-------1:初始化；0---------------主程序
出口：无
**********************************************************************************/	
INT8U Read_Storage_Data_PUCK(INT32U DI,void *pDst,INT16U RealLen)
{
  INT8U Err;
  INT16U Len;
  
  Len=Read_Storage_Data(DI,pDst,pDst,RealLen,&Err);
  if((Len==0)||(Len!=RealLen)||(Err!=NO_ERR))
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->0X%x Get Failed!",DI); 
    return 0;
  }
  
  mem_cpy(pDst,pDst,Len,pDst,Len);
  return 1;
}
/**********************************************************************************
函数功能：检查IC校表参数和实际使用的校表参数校验和是否正确；
入口：无
出口：
CS_IC_EPPROM_GEIFAILED---内卡CS和IC的CS均获取失败,CS也想等,CS均无
CS_IC_GET_FAILED---------计量IC参数获取失败，CS无效；
CS_EPPROM_GET_FAILED-----内卡参数获取失败，CS无效；
CS_IC_EPPROM_NO_EQUAL------CS均获取成功，但是2者不相等,以内卡为准！
CS_IC_EPPROM_EQUAL-------内卡CS和IC的CS均获取成功,CS也想等
**********************************************************************************/
INT8U Chk_IC_User_ParaSum(void)
{
  INT8U Flag1,Flag2;
  INT32U CS_User,CS_IC;
  
  Flag1=Get_IC_ParaSum3(&CS_IC);
  Flag2=Get_User_All_ParaSum3(&CS_User);
  
  if((!Flag1)&&(!Flag2))
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->IC Cs And Epprom Para Read Error");
    return CS_IC_EPPROM_GEIFAILED;
  }
  
  if((!Flag1)&&(Flag2))
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->IC Cs Read Error!");
    return CS_IC_GET_FAILED;
  }
  
  if((Flag1)&&(!Flag2))
  {
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->IC Cs And Epprom Para Read Error");
    return CS_EPPROM_GET_FAILED;
  }
  
  if(CS_IC==CS_User)
  {
    MeasuPara_CS.IC_Cs=CS_IC;
    MeasuPara_CS.EppRom_Cs=CS_IC;
    SET_STRUCT_SUM(MeasuPara_CS);
    DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Info----->IC Cs And Epprom Para Read All OK");
    return CS_IC_EPPROM_EQUAL;
  }
  
  DEBUG_PRINT(PUCK,PRINT_PUCK_MEA_EN,"Measure_Error----->IC Cs And Epprom Cs Read OK,But Not Equal");
  return CS_IC_EPPROM_NO_EQUAL;
}
