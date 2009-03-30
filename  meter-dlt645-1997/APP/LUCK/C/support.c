
/**
 * @file support.c
 *     { comment block }
 * @brief
 *     { brief description @ref support.cpp }
 * @author li.yao (nardoo@sohu.com)
 * @version v1.00
 * @date 05/29/2008 08:41
 * @note
 *     { paragraph where a note can be entered. }
 */

#include "Pub_PUCK.h"
#include "config.h"

// screen model words.
typedef struct {
    code_t user;     //A/B模式下的用户显示代码-----PUCK
    code_t code;     //C模式下的显示代码-----PUCK
    u8     dlen;
} list_t;

#define BYTE0(x)      (INT8U)(x&0x000000ff)
#define BYTE1(x)      (INT8U)((x&0x0000ff00)>>8)
#define BYTE2(x)      (INT8U)((x&0x00ff0000)>>16)
#define BYTE3(x)      (INT8U)((x&0xff000000)>>24)

#if PROC_645_TYPE==PROC_645_2007
INT8U Get_Code (scrn_t *temp,INT32U DI)
{
  INT8U Modi;
  
  if(DI==temp->item)
  {
    Modi=0;
  }
  else
  {
    if(BYTE3(DI)==BYTE3(temp->item) && BYTE2(DI)==BYTE2(temp->item) && BYTE1(DI)==BYTE1(temp->item))
    {
      if(0!=temp->BytePos)
        return 0;
      Modi=BYTE0(DI);
    }
    
    if(BYTE3(DI)==BYTE3(temp->item) && BYTE2(DI)==BYTE2(temp->item) && BYTE0(DI)==BYTE0(temp->item))
    {
      if(1!=temp->BytePos)
        return 0;
      Modi=BYTE1(DI);
    }
    
    if(BYTE3(DI)==BYTE3(temp->item) && BYTE1(DI)==BYTE1(temp->item) && BYTE0(DI)==BYTE0(temp->item))
    {
      if(2!=temp->BytePos)
        return 0;
      Modi=BYTE2(DI);
    }
    
    if(BYTE2(DI)==BYTE2(temp->item) && BYTE1(DI)==BYTE1(temp->item) && BYTE0(DI)==BYTE0(temp->item))
     {
      if(3!=temp->BytePos)
        return 0;
      Modi=BYTE3(DI);
     }
  }
  temp->item=DI;
  switch(temp->GroupFlag)
  {
    case GROUP_TARIFF:      
      if(Modi)
      {
        temp->elem.feerate=1;           //显示 "费率"
        temp->elem.tariff=Modi;         //显示 "Txx"
      }
      else
        temp->elem.total=1;      
    break;
    case GROUP_SETTLE:      
      if(Modi)
      {
        temp->elem.lastmonth=Modi;         //显示 "上xx日"
      }
      else
        temp->elem.thismonth=1;          //当前  
    break;
    
    
    
  }
  
  return 1;
}
#endif


// 显示屏缓冲
static volatile list_t vlist;

/********************************************************************************
PUCK:
函数功能：获取显示代码的个数，也就是光标的范围
入口：
返回：
********************************************************************************/
curs_t maxcurs () 
{
    return (curs_t)vlist.dlen;
}
/********************************************************************************
PUCK:
函数功能：获取对应模式（A或者B）下的具体显示项目
入口：
     item-----显示项目，同645标识;offs-----该项目下的后续项目，及偏移
返回：
********************************************************************************/
list_t getlist (item_t item, offs_t offs) 
{
  INT8U Err;
  INT16U Len;
  list_t list = {0, 0, 0};

#if PROC_645_TYPE==PROC_645_1997
  if(item&0xFF00!=0xC600)
    ASSERT(A_WARNING,0); 
#endif
 
#if PROC_645_TYPE==PROC_645_2007
    ;  //别忘了添加代码
#endif
  
  Len=Read_Storage_Data(item+(offs/10)*16+(offs%10),&list,&list,sizeof(list),&Err);
  if(0==Len || Err!=NO_ERR)
    ASSERT(A_WARNING,0);
  
#if PROC_645_TYPE==PROC_645_2007
  list.code=list.user;  //显示代码用协议代码DI替代
#endif
  return list;

  //getdata(item + (offs / 10) * 16 + (offs % 10), &list, &list, sizeof(list));

}
/********************************************************************************
PUCK:
函数功能：根据修改的DI值，设置C模式下的显示code
入口：
返回：
********************************************************************************/
u32 Set_ParaModi_Proc(u16 ModiDI) 
{
  const scrn_t* p;
  u32 DisCode=0;
  
   for(p=&table[0];p!= &table[MAXCOUNT];++p) 
   {
     if (ModiDI==p->item)
     {
       DisCode=p->code;
       break;
      }
   }
   return DisCode;
}
/********************************************************************************
PUCK:
函数功能：按位置显示内容：实际只适用A/B模式
入口：
     type-----模式参数;offs-----该模式下显示的项目顺序
返回：
********************************************************************************/
void lcdshow (u8 type, offs_t offs) 
{
    switch (type) 
    {
        case modeA: //A模式下，根据645标识及其偏移获取显示元素-----------PUCK
        {
            vlist = getlist(ITEMMODEA, offs % MODE_A_NUM);
            vlist.dlen = 2;
            break;
        }
        case modeB: //B模式下，根据645标识及其偏移获取显示元素-----------PUCK
        {
            vlist = getlist(ITEMMODEB, offs % MODE_B_NUM);
            vlist.dlen = 2;
            break;
        }
        default :   //C模式下，根据表的偏移获取显示元素-----------PUCK
        {
            vlist.user = table[offs % MAXCOUNT].code; 
            vlist.dlen = MAXBITS;
            break;
        }
    }
}
/********************************************************************************
PUCK:
函数功能：根据模式参宿,显示编码，获取显示信息。
入口：
     type-----模式参数;code-----该模式下显示的项目顺序
返回：
********************************************************************************/
unsigned char Ser_Support_Code(item_t code)
{
  const scrn_t* p;
  for(p=&table[0];p!=&table[MAXCOUNT];++p) 
  {
    if (code==p->item) 
    {
      return 1;
    }
 }
 return 0;
}
/********************************************************************************
PUCK:
函数功能：按编码显示内容：实际C模式显示
入口：
     type-----模式参数;code-----该模式下显示的项目顺序
返回：
********************************************************************************/
void display(u8 type, code_t code) 
{
    vlist.user = code;
    switch (type) 
    {
        case modeA: 
        {
            for (offs_t offs = MODE_A_NUM; offs > 0; --offs) 
            {
                list_t list = getlist(ITEMMODEA, offs);
                if (code == list.user) 
                {
                    vlist = list;
                    break;
                }
            }
            break;
         }
        case modeB: 
         {
            for (offs_t offs = MODE_B_NUM; offs > 0; --offs) 
            {
                list_t list = getlist(ITEMMODEB, offs);
                if (code == list.user) 
                {
                    vlist = list;
                    break;
                }
            }
            break;
        }
        default : 
        {
            vlist.code = code;
            vlist.dlen = MAXBITS;
            break;
        }
    }
}
/********************************************************************************
PUCK:
函数功能：获取屏幕显示信息，并显示
入口：
     type-----模式参数;curs-----光标位置
返回：
********************************************************************************/
void screen (u8 type, curs_t curs) 
{
   const scrn_t* p;
  //在此前，vlist的信息已经更新-----PUCK
#if PROC_645_TYPE==PROC_645_1997 
    static scrn_t scrn={0xff,0xff,"",{0}};   //不为0，保证第一次就能更新，因为0x000000有可能第一次就是0---PUCK
    if(scrn.code != vlist.code)              //按照全显代码检索：编码已经更新，需要根据vlist的编码获取新的显示元素-----------PUCK
#endif
      
#if PROC_645_TYPE==PROC_645_2007
    static scrn_t scrn={0xff,0xff,GROUP_NO,0,"",{0}};   //不为0，保证第一次就能更新，因为0x000000有可能第一次就是0---PUCK
    if (scrn.item != vlist.code)     //按照命令代码检索：A/B模式下为协议DI代码，C模式下还是显示代码
#endif
    {
        for (p=&table[0];p!=&table[MAXCOUNT];++p) 
        {
#if PROC_645_TYPE==PROC_645_1997
            if ( p->code==vlist.code)  //按照显示代码检索
            {
                scrn = *p;
                break;
            }  
#endif
            
#if PROC_645_TYPE==PROC_645_2007
            scrn=*p;        
            {
              if(Get_Code(&scrn,vlist.code))  //按照命令代码检索：
                break;
            }
#endif                  
        }
    }
    if(p==&table[MAXCOUNT]) //找不到显示的代码,只显示代码，事件，其他不显示-------PUCK
    {
      lcd_code     (vlist.user, curs);
      lcd_events();
      lcd_update();  
      return ;
    }
    
    curs %= vlist.dlen;
    lcd_data     (scrn.item, scrn.frmt,scrn.elem.signpos);   //18ms
    lcd_code     (vlist.user, curs);       //26ms
    lcd_mode     (type);                ///A/B/C 3种模式
    lcd_total    (scrn.elem.total    ); ///< "总",
    lcd_phase    (scrn.elem.phase    ); ///< "A/B/C相",
    lcd_voltage  (scrn.elem.voltage  ); ///< "电压",
    lcd_current  (scrn.elem.current  ); ///< "电流",
    lcd_positive (scrn.elem.positive ); ///< mode4.bit4 ? "输入" : "正向";
    lcd_negative (scrn.elem.negative ); ///< mode4.bit4 ? "输出" : "反向";
    lcd_reactive (scrn.elem.reactive ); ///< "无功",
    lcd_active   (scrn.elem.active   ); ///< "有功",
    lcd_power    (scrn.elem.power    ); ///< "功率",
    lcd_factor   (scrn.elem.factor   ); ///< "因数",
    lcd_angle    (scrn.elem.angle    ); ///< "相角",
    lcd_energy   (scrn.elem.energy   ); ///< "电量",
    lcd_demand   (scrn.elem.demand   ); ///< "需量",
    lcd_time     (scrn.elem.time     ); ///< "时间",
    lcd_charge   (scrn.elem.charge   ); ///< "电费",
    lcd_lostvolt (scrn.elem.lostvolt ); ///< "失压",
    lcd_lostcurt (scrn.elem.lostcurt ); ///< "失流",
    lcd_periods  (scrn.elem.periods  ); ///< "时段",
    lcd_feerate  (scrn.elem.feerate  ); ///< "费率",    
    lcd_thismonth(scrn.elem.thismonth); ///< "本月",
    lcd_lastmonth(scrn.elem.lastmonth); ///< mode4.bit5 ? "上月", "上上月" : "上1月", "上2月";
    lcd_assemble (scrn.elem.assemble ); ///< "无功组合方式1 or 2", mode4.bit7 ? "逆时针" : "顺时针";
    lcd_quadrant (scrn.elem.quadrant ); ///< "XX象限",
    lcd_tariff   (scrn.elem.tariff   ); ///< 显示"费率Tx",
    lcd_times    (scrn.elem.times    ); ///< 显示"上XX次",
    lcd_events   ();    //8ms
    lcd_update   ();
}
