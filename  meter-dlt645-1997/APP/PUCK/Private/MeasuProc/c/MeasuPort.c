#define MEASU_PORT_C
#include "Pub_PUCK.h"


void Init_Pulse_Port(INT8U Flag)
{  
  if(Flag)
  {
    SET_AC_POWER_OUT;
    SET_REAC_POWER_OUT;
    SET_AC_DIR_OUT;    
    SET_REAC_DIR_OUT;
    SET_AC_LED_OUT;
    SET_REAC_LED_OUT;   
    START_CF1;
    START_CF2;
    return ;    
  }
  STOP_CF1;
  STOP_CF2;
  SET_AC_POWER_IN;
  SET_REAC_POWER_IN;
  SET_AC_DIR_IN;
  SET_REAC_DIR_IN;
  SET_AC_LED_IN;
  SET_REAC_LED_IN;
}
