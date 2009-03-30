#ifndef TEST_EVENT_DATA_H
#define TEST_EVENT_DATA_H

#undef EXT
#ifdef TEST_EVENT_DATA_C
#define EXT
#else
#define EXT extern
#endif

EXT INT8U Test_Event_Data_Proc();
EXT INT8U Test_A_Loss_Vol();
EXT INT8U Test_Freeze_Energy_Data();
EXT INT8U Test_Freeze_Demand_Data();
EXT INT8U Test_Freeze_VolStat_Data();
EXT INT8U Test_All_Loss_Vol_Data_Proc();
EXT void Test_Cur_Demand_Rate_Data();
#endif
