#define TEST_PUB_C
#include "Includes.h"
#ifdef TEST_MODE
void Test_Func(INT8U (*pFunc)(),INT8U Func_Name[])
{
  DEBUG_PRINT(TEST,DEBUG_0,"%s Test Start!",Func_Name);
  if((*pFunc)()!=0)
    DEBUG_PRINT(TEST,DEBUG_0,"%s Test OK!",Func_Name);
  else
    DEBUG_PRINT(TEST,DEBUG_0,"%s Test Failed!",Func_Name);
  
}

void Test_Unit()
{
 //TEST_FUNC(&Test_Event_Data_Proc);
 //TEST_FUNC(&Test_Freeze_Energy_Data);
 //TEST_FUNC(&Test_Freeze_Demand_Data);
 //TEST_FUNC(&Test_Freeze_VolStat_Data);
 TEST_FUNC(&Test_All_Loss_Vol_Data_Proc); 
}

void Test_Proc(INT8U Flag)
{
  if(Flag==1)
    return;
  
  //while(1)
  {
    //TEST_FUNC(&Test_A_Loss_Vol); 
    //TEST_FUNC(&Test_Freeze_Energy_Data);
    OS_TimeDly_Ms(100);
  }
  
}
#endif
#undef TEST_PUB_C
