#ifndef EVENT_DATA_H
#define EVENT_DATA_H

//#include "Include_PUCK.h"

#undef EXT
#ifndef EVENT_DATA_C
#define EXT extern
#else
#define EXT
#endif

EXT INT8U Check_Event_Occuring(INT8U Event_ID);
EXT void Event_Data_Proc(INT8U Event_ID,INT8U Occur_Or_End);
EXT void Clear_Event_Data();
EXT void Set_Def_Event_Data();
EXT void Save_Event_PD_Data();
EXT void PowerOn_Event_Proc();
#endif

