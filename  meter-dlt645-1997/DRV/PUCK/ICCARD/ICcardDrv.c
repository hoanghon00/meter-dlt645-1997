#define DRV_ICCARD_C
#include "Pub_PUCK.h"

#if METER_HARD_TYPE>=HARD_TYPE_20090224
/**********************************************************************************
函数功能：初始化IC卡口
入口：
出口：
**********************************************************************************/ 
void Init_ICcard_Port(void)
{   
  if(METER_ICCARD_EN)   //IC卡
  {    
    SET_IC_CARD_CLK_DIR;
    SET_IC_CARD_RST_DIR;
    SET_IC_CARD_OUT_DIR;
    
    SET_IC_CARD_IN_DIR;
    SET_IC_CARD_SW_DIR;  
    SET_IC_CARD_PWR_DIR;
    SET_IC_CARD_FAIL_DIR;
    
    IC_CARD_POWR_OFF;
    IC_CARD_OUT_1;
  }    
}
/**********************************************************************************
函数功能：上电初始化 IC卡
入口：
出口：
**********************************************************************************/ 
void Power_On_Card(void)
{
  INT8U i;
  
  IC_CARD_RST_0;
  IC_CARD_CLK_0;
  
  //tr_show_wait(10);
  IC_CARD_RST_1;
  //tr_show_wait(10);
  IC_CARD_CLK_1;
  
  //tr_show_wait(10);
  IC_CARD_CLK_0;
  //tr_show_wait(10);
  IC_CARD_RST_0;
  
  //tr_show_wait(10);
  IC_CARD_OUT_1;
  for(i=0;i<=20;i++)
  {
          //tr_show_wait(10);
          IC_CARD_CLK_1;
          //tr_show_wait(10);
          IC_CARD_CLK_0;
  }
    //tr_show_wait(10);
}
/**********************************************************************************
函数功能：向IC卡写一个字节
入口：
出口：
**********************************************************************************/ 
void Write_IC_Card_Byte(INT8U byte)
{
    INT8U i;

    for(i=0;i<8;i++)
    {
        IC_CARD_CLK_0;
        
        if(byte&0x01)
          IC_CARD_OUT_1;
        else
          IC_CARD_OUT_0;
        
        //tr_show_wait(10);
        IC_CARD_CLK_1;
        //tr_show_wait(10);
        IC_CARD_CLK_0;
        
        byte=byte>>1;
    }
    IC_CARD_OUT_1;
}

/**********************************************************************************
函数功能：写IC卡命令
入口：
出口：
**********************************************************************************/ 
void Write_Command(INT8U byte1,INT8U byte2,INT8U byte3)
{
    IC_CARD_RST_1;
    //tr_show_wait(10);
    //tr_show_wait(10);
    Write_IC_Card_Byte(byte1);
    Write_IC_Card_Byte(byte2);
    Write_IC_Card_Byte(byte3);
    //tr_show_wait(10);
    IC_CARD_RST_0;
    //tr_show_wait(10);
}
/**********************************************************************************
函数功能：读IC卡数据到指定区域
入口：
出口：
**********************************************************************************/ 
void Read_Data_From_ICcard(INT16U Addr,INT8U *Dst,INT8U Len)
{
    INT8U command1,command2,command3;
    INT8U temp,i;

    temp=Addr>>2;
    temp=temp&0xc0;
    command1=0x0e;
    command1=command1|temp;
    command2=Addr&0x00ff;
    command3=Addr&0x0001;
    Write_Command(command1,command2,command3);

    //tr_show_wait(10);
    //tr_show_wait(10);
    IC_CARD_CLK_1;
    //tr_show_wait(10);
    IC_CARD_CLK_0;

    IC_CARD_OUT_1;
    for(temp=0;Len!=0;Len--)
    {
        for(i=0;i<8;i++)
        {
            //tr_show_wait(10);
            IC_CARD_CLK_1;
            temp=temp>>1;
            temp=temp|(IC_CARD_IN<<7);
            //tr_show_wait(10);
            IC_CARD_CLK_0;
        }
        *Dst=temp;
        Dst++;
        temp=0;
    }
    //tr_show_wait(10);
    IC_CARD_RST_1;
    //tr_show_wait(10);
    //tr_show_wait(10);
    //tr_show_wait(10);
    IC_CARD_RST_0;
    //tr_show_wait(10);
}
/**********************************************************************************
函数功能：写IC卡数据到指定区域
入口：
出口：
**********************************************************************************/ 
INT8U Write_Data_To_ICcard(INT16U Addr,INT8U *Src,INT8U Len)
{
    INT8U command1,command2,command3;
    INT8U temp,tempflag=0x00;

    command2=Addr&0x00ff;
    Addr=Addr>>2;
    temp=Addr&0xc0;
    command1=0x33;
    command1=command1|temp;


    for(temp=0;Len!=0;Len--)
    {
        if(IC_CARD_SW==1)
        {
            tempflag=0x01;
            break;
        }
        command3=*Src;
        Write_Command(command1,command2,command3);
        Src++;
        command2++;
        if(command2==0x00)
        command1=command1+0x40;//A9 A8 add1 if A7~A0=00H
        //tr_show_wait(10);
        for(temp=0;temp<=203;temp++)
        {
            //tr_show_wait(10);
            IC_CARD_CLK_1;
            //tr_show_wait(10);
            IC_CARD_CLK_0;
        }
        //tr_show_wait(10);
    }
    return tempflag;
}
/**********************************************************************************
函数功能：
入口：
出口：
**********************************************************************************/ 
void psc_verification(INT8U psc_fe,INT8U psc_ff, char task_number)
{
    INT8U command3;
    //INT8U command1,command2;
    INT8U temp;

    //command1=0xf2;
    //command2=0xfd;
    //command3=0xff;
    //command3=command3>>task_number;
    Write_Command(0xf2,0xfd,task_number);//write error counter
    //tr_show_wait(10);
    for(temp=0;temp<=103;temp++)
    {
        //tr_show_wait(10);
        IC_CARD_CLK_1;
        //tr_show_wait(10);
        IC_CARD_CLK_0;
    }
    //tr_show_wait(10);

    //command1=0xcd;
    //command2=0xfe;
    command3=psc_fe;
    Write_Command(0xcd,0xfe,command3);//verify 1st PS Cbyte
    //tr_show_wait(10);
    //tr_show_wait(10);
    IC_CARD_CLK_1;
    //tr_show_wait(10);
    IC_CARD_CLK_0;
    //tr_show_wait(10);
    IC_CARD_CLK_1;
    //tr_show_wait(10);
    IC_CARD_CLK_0;
    //tr_show_wait(10);

    //command1=0xcd;
    //command2=0xff;
    command3=psc_ff;
    Write_Command(0xcd,0xff,command3);//verify 2nd PS Cbyte
    //tr_show_wait(10);
    //tr_show_wait(10);
    IC_CARD_CLK_1;
    //tr_show_wait(10);
    IC_CARD_CLK_0;
    //tr_show_wait(10);
    IC_CARD_CLK_1;
    //tr_show_wait(10);
    IC_CARD_CLK_0;
    //tr_show_wait(10);

    //command1=0xf3;
    //command2=0xfd;
    //command3=0xff;
    Write_Command(0xf3,0xfd,0xff);//erase error counter
    //tr_show_wait(10);
    for(temp=0;temp<=203;temp++)
    {
        //tr_show_wait(10);
        IC_CARD_CLK_1;
        //tr_show_wait(10);
        IC_CARD_CLK_0;
    }
    //tr_show_wait(10);
}
/**********************************************************************************
函数功能：
入口：
出口：
**********************************************************************************/ 
INT8U password_bit_number(char* count)
{
    INT8U password_count,tempcount,temp;
    int i_temp;
    char flag;
    
    Read_Data_From_ICcard(1021,&password_count,1);	
    
    temp=0x01;
    tempcount=0;
    flag=0;
    for(i_temp=0;i_temp<8;i_temp++) 
    {
        if((password_count&temp)!=0)
        {
            tempcount++;
            if(flag==0)
            {
                  *count=password_count&(~temp);
            }			
        }
        temp<<=1;
    }
    

    /*if(password_count>=0x80)
            tempcount=1;
    else if(password_count>=0x40)
            tempcount=2;
    else if(password_count>=0x20)
            tempcount=3;
    else if(password_count>=0x10)
            tempcount=4;
    else if(password_count>=0x08)
            tempcount=5;
    else 
            tempcount=6;*/

    return tempcount;
}
#endif