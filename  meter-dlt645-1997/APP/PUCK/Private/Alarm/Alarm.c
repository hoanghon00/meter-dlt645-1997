#define ALARM_C
#include "Pub_PUCK.h"
#include "Alarm.h"
/***********************************************************************
函数功能：根据位或者字节单位，获取对应的位信息或字节信息
入口：无
出口：无
***********************************************************************/
void Sound_LED_LCD_Port_Alarm(void)
{
  INT8U  i,j,flag,num;
   
   for(i=0,flag=0;i<ALARM_EVENT_NUM;i++)
   {
     if(GET_BIT(*(Alarm_Event_Property[i].EventEn),Alarm_Event_Property[i].EventBit))   //是否允许事件报警输出
     {
       if(Read_Event_Status(Alarm_Event_Property[i].EventID))  //确认该事件发生
       {
         for(j=0,num=0;j<ALARM_MODE_NUM;j++)  //声音报警，LED报警，端子排报警
         {
           if(GET_BIT(*(Alarm_Mode_Property[j].AlarmEn),Alarm_Mode_Property[j].AlarmBit))  //何种方式报警
           {
             Port_Out_Pub(Alarm_Mode_Property[j].PortID,Alarm_Mode_Property[j].AlarmWidth);
             Set_Event_Instant(ID_ALARM_OUT);
             num++;
             flag=1;
           }
           if(num>=ALARM_MODE_NUM)
             return ;
         }       
       }     
     }  
   }
   if(0==flag)
   Clr_Event_Instant(ID_ALARM_OUT);
}
