#define EVENT_MULTITIMES_C
#include <includes.h>

CONST S_Event_DI Event_DI[]=
{
//0,注1：失压事件记录的数据格式为：起始时间(年月日时分)，结束时间(年月日时分)，总、A、B和C相的总正向有功电量、总组合无功1电量、总反向有功电量、总组合无功2电量、ABC相的总安培乘时间（分辨率为0.1Ah）。数据长度为：5＋5＋4×（4＋4＋4＋4）＋4＋4＋4。（计量数据为"期间增量数据"）
{.Occur_Mask=0x1FFFFD,.End_Mask=0x1FFFFE,.Diff_Mask=0x1FFFFC,.Data_Num=21,.Data_Len=86+1,.DI={DI_EVENT_TIME,DI_EVENT_TIME,0x9010,0x9110,0x9020,0x9120,0xE701,0xE704,0xE707,0xE70A,0xE702,0xE705,0xE708,0xE70B,0xE703,0xE706,0xE709,0xE70C,DI_A_AH,DI_B_AH,DI_C_AH}},//失压记录
//1,注2 ：失流事件记录的数据格式为：起始时间(年月日时分)，结束时间(年月日时分)，总、A、B和C相的总正向有功电量、总组合无功1电量、总反向有功电量、总组合无功2电量。数据长度为：5＋5＋4×（4＋4＋4＋4）。  （计量数据为"期间增量数据"）
{.Occur_Mask=0x1FFFFD,.End_Mask=0x1FFFFE,.Diff_Mask=0x1FFFFC,.Data_Num=18,.Data_Len=74+1,.DI={DI_EVENT_TIME,DI_EVENT_TIME,0x9010,0x9110,0x9020,0x9120,0xE701,0xE704,0xE707,0xE70A,0xE702,0xE705,0xE708,0xE70B,0xE703,0xE706,0xE709,0xE70C}},//失流记录
//2,注3 ：反向事件记录的数据格式为：起始时间(年月日时分)，结束时间(年月日时分)，总、A、B和C相的总正向有功电量、总反向有功电量。数据长度为：5＋5＋4×（4＋4）。  （计量数据为"期间增量数据"）
{.Occur_Mask=0x3FD,.End_Mask=0x3FE,.Diff_Mask=0x3FC,.Data_Num=10,.Data_Len=42+1,.DI={DI_EVENT_TIME,DI_EVENT_TIME,0x9010,0x9020,0xE701,0xE707,0xE702,0xE708,0xE703,0xE709}},//反向记录
//3 注4：全失压、全失流、断相、逆相序、上下电、时钟电池欠压、低功耗电池欠压和校时事件记录的数据格式为：起始(前)时间(年月日时分),结束(后)时间(年月日时分)。数据长度为：5＋5。
{.Occur_Mask=0x01,.End_Mask=0x02,.Diff_Mask=0x00,.Data_Num=2,.Data_Len=10+1,.DI={DI_EVENT_TIME,DI_EVENT_TIME}},
//4 注5：A相电压合格率、过压事件记录,电压合格率、过压事件记录的数据格式为：起始时间(年月日时分)，结束时间(年月日时分)，电压（XXXX.XX（V））。数据长度为：5＋5＋3。 
{.Occur_Mask=0x05,.End_Mask=0x02,.Diff_Mask=0x00,.Data_Num=3,.Data_Len=13+1,.DI={DI_EVENT_TIME,DI_EVENT_TIME,DI_EVENT_VOL}},
//5 B相电压合格率、过压事件记录,电压合格率、过压事件记录的数据格式为：起始时间(年月日时分)，结束时间(年月日时分)，电压（XXXX.XX（V））。数据长度为：5＋5＋3。 
{.Occur_Mask=0x05,.End_Mask=0x02,.Diff_Mask=0x00,.Data_Num=3,.Data_Len=13+1,.DI={DI_EVENT_TIME,DI_EVENT_TIME,DI_EVENT_VOL}},
//6 C相电压合格率、过压事件记录,电压合格率、过压事件记录的数据格式为：起始时间(年月日时分)，结束时间(年月日时分)，电压（XXXX.XX（V））。数据长度为：5＋5＋3。 
{.Occur_Mask=0x05,.End_Mask=0x02,.Diff_Mask=0x00,.Data_Num=3,.Data_Len=13+1,.DI={DI_EVENT_TIME,DI_EVENT_TIME,DI_EVENT_VOL}},
//7 注6 ：超需量报警事件记录的数据格式为：起始时间(年月日时分)，结束时间(年月日时分)，当时需量（单位0.0001kW）。数据长度为：5＋5＋3。
{.Occur_Mask=0x05,.End_Mask=0x02,.Diff_Mask=0x00,.Data_Num=3,.Data_Len=13+1,.DI={DI_EVENT_TIME,DI_EVENT_TIME,DI_CUR_DEMAND}},
//8 注7 ：超需量跳闸事件记录的数据格式为：起始时间(年月日时分)，当时需量（单位0.0001kW）。数据长度为：5＋3。
{.Occur_Mask=0x03,.End_Mask=0x00,.Diff_Mask=0x00,.Data_Num=2,.Data_Len=8+1,.DI={DI_EVENT_TIME,DI_CUR_DEMAND}},
//9 注8 ：清过程、清电压合格率、复位、校表、开端盖、开大盖、开铅封门、开计量柜门事件记录的数据格式为：时间(年月日时分)。数据长度为：5。
{.Occur_Mask=0x01,.End_Mask=0x00,.Diff_Mask=0x00,.Data_Num=1,.Data_Len=5+1,.DI={DI_EVENT_TIME}},
//10 注9 ：清需量事件记录的数据格式为：清需时间(年月日时分)、清需前的正有功总需量、反有功总需量、输入无功总需量、输出无功总需量及对应的需量发生时间和标识字。数据长度为：5＋4×3＋4×5＋1。标识字为：1=按键清需，2=通讯清需。（该标识字保留，暂时没用）
{.Occur_Mask=0x03FF,.End_Mask=0x00,.Diff_Mask=0x00,.Data_Num=10,.Data_Len=38+1,.DI={DI_EVENT_TIME,0xA010,0xA020,0xA110,0xA120,0xB010,0xB020,0xB110,0xB120,DI_CLR_DEMAND_WAY}},
//11 注11 ：编程事件记录数据格式为：起始时间(年月日时分)、编程标识字1、编程标识字2。数据长度为5＋1＋1。
//         标识字1：0位：设置电表常数
//              1位：设置周休日状态
//                  2位：设置表号、设备号、用户号
//                  3位：设置最大需量周期及滑差步进时间
//                  4位：设置循环显示参数：循环显示间隔，循环显示项目及总数
//                  5位：设置电表的电池使用时间
//                  6位：设置结算日
//                  7位：其他参数
//        标识字2： 0位：设置密码
//                  1位：设置时段参数
//                  2位：设置负荷曲线记录参数
//                  3位：设置费率，剩余电费报警限额
//                  4位：设置功率限额
//                  5位：设置跳合闸延时，用户级别
//                  6位：设置电压合格率参数
//    7位：设置电表模式字
{.Occur_Mask=0x01,.End_Mask=0x06,.Diff_Mask=0x00,.Data_Num=3,.Data_Len=7+1,.DI={DI_PROG_TIME,DI_PROG_FLAG0,DI_PROG_FLAG1}},
//12 注12 ：运行时钟乱和上电时钟乱事件记录的数据格式为：后时间(年月日时分)，原时间(年月日时分)，标识字(暂时无定义,保留)。数据长度为：5＋5＋1。
{.Occur_Mask=0x02,.End_Mask=0x00,.Diff_Mask=0x00,.Data_Num=3,.Data_Len=11+1,.DI={DI_CUR_TIME,DI_ERR_TIME,DI_NULL_1BYTE}},
//13 注13 ：EEPROM坏事件记录的数据格式为：起始时间(年月日时分)，标识字。数据长度为：5＋1
{.Occur_Mask=0x02,.End_Mask=0x00,.Diff_Mask=0x00,.Data_Num=2,.Data_Len=6+1,.DI={DI_EVENT_TIME,DI_ERR_ROM}},
//14,注17 ：总清零事件记录的数据格式为：时间(年月日时分)，清前总及8费率的正有功电量、组合无功1电量、反有功电量、组合无功2电量。数据长度为：5＋4×9×4。
{.Occur_Mask=0x1F,.End_Mask=0x00,.Diff_Mask=0,.Data_Num=5,.Data_Len=149+1,.DI={DI_EVENT_TIME,0x901F,0x911F,0x902F,0x912F}},
//15 注18 ：总电量立即冻结事件记录的数据格式为：冻结时间(年月日时分)，冻结正有功总电量、冻结组合无功1总电量、冻结反有功总电量、冻结组合无功2总电量。数据长度为：5＋4×4。
{.Occur_Mask=0x01F,.End_Mask=0x00,.Diff_Mask=0x00,.Data_Num=5,.Data_Len=21+1,.DI={DI_EVENT_TIME,0x9010,0x9110,0x9020,0x9120}},
//16 全失压特殊处理
{.Occur_Mask=0x01,.End_Mask=0x02,.Diff_Mask=0x00,.Data_Num=2,.Data_Len=10+1,.DI={DI_ALL_LOSS_VOL_START_TIME,DI_ALL_LOSS_VOL_END_TIME}},
//17 校表记录
{.Occur_Mask=0x01,.End_Mask=0x00,.Diff_Mask=0x00,.Data_Num=2,.Data_Len=6+1,.DI={DI_EVENT_TIME,DI_ADJ_FLAG}},

EVENT_MULTITIMES_DI_EXTEND
};

//每种事件的分次事件记录
CONST S_Event_Data_Info Event_Data_Info[]=
{
//标识编码	数据格式	功能	单位	数据长度	数据项名称
//E900		R		149	前1次总清零记录（注17）
//┅	┅	┅	┅	┅	┅
//E902		R		149	前3次总清零记录（注17）
{.Event_ID=ID_EVENT_CLR_ALL_DATA,.Property=0,.pData=&Event_DI[14],.Last_Storage_DI=E_E900,.Storage_DI=0xE900,.Storage_Num=3,.Clr_Flag=0},
//E910		R		86	前1次A相失压记录（注1）
//┅	┅	┅	┅	┅	┅
//E919		R		86	前10次A相失压记录（注1）
{.Event_ID=ID_EVENT_A_LOSS_VOLT,.Property=0,.pData=&Event_DI[0],.Last_Storage_DI=E_E910,.Storage_DI=0xE910,.Storage_Num=10,.Clr_Flag=1},
//E920		R		86	前1次B相失压记录（注1）
//┅	┅	┅	┅	┅	┅
//E929		R		86	前10次B相失压记录（注1）
{.Event_ID=ID_EVENT_B_LOSS_VOLT,.Property=0,.pData=&Event_DI[0],.Last_Storage_DI=E_E920,.Storage_DI=0xE920,.Storage_Num=10,.Clr_Flag=1},
//E930		R		86	前1次C相失压记录（注1）
//┅	┅	┅	┅	┅	┅
//E939		R		86	前10次C相失压记录（注1）
{.Event_ID=ID_EVENT_C_LOSS_VOLT,.Property=0,.pData=&Event_DI[0],.Last_Storage_DI=E_E930,.Storage_DI=0xE930,.Storage_Num=10,.Clr_Flag=1},
//E940		R		86	前1次AB相失压记录（注1）
//┅	┅	┅	┅	┅	┅
//E949		R		86	前10次AB相失压记录（注1）
{.Event_ID=ID_EVENT_AB_LOSS_VOLT,.Property=0,.pData=&Event_DI[0],.Last_Storage_DI=E_E940,.Storage_DI=0xE940,.Storage_Num=10,.Clr_Flag=1},
//E950		R		86	前1次AC相失压记录（注1）
//┅	┅	┅	┅	┅	┅
//E959		R		86	前10次AC相失压记录（注1）
{.Event_ID=ID_EVENT_AC_LOSS_VOLT,.Property=0,.pData=&Event_DI[0],.Last_Storage_DI=E_E950,.Storage_DI=0xE950,.Storage_Num=10,.Clr_Flag=1},
//E960		R		86	前1次BC相失压记录（注1）
//┅	┅	┅	┅	┅	┅
//E969		R		86	前10次BC相失压记录（注1）
{.Event_ID=ID_EVENT_BC_LOSS_VOLT,.Property=0,.pData=&Event_DI[0],.Last_Storage_DI=E_E960,.Storage_DI=0xE960,.Storage_Num=10,.Clr_Flag=1},
//E970		R		86	前1次ABC相失压记录（注1）
//┅	┅	┅	┅	┅	┅
//E979		R		86	前10次ABC相失压记录（注1）
{.Event_ID=ID_EVENT_ABC_LOSS_VOLT,.Property=0,.pData=&Event_DI[0],.Last_Storage_DI=E_E970,.Storage_DI=0xE970,.Storage_Num=10,.Clr_Flag=1},
//E980		R		74	前1次A失流记录（注2）
//┅	┅	┅	┅	┅	┅
//E989		R		74	前10次A失流记录（注2）
{.Event_ID=ID_EVENT_A_LOSS_CURR,.Property=0,.pData=&Event_DI[1],.Last_Storage_DI=E_E980,.Storage_DI=0xE980,.Storage_Num=10,.Clr_Flag=1},
//E990		R		74	前1次B失流记录（注2）
//┅	┅	┅	┅	┅	┅
//E999		R		74	前10次B失流记录（注2）
{.Event_ID=ID_EVENT_B_LOSS_CURR,.Property=0,.pData=&Event_DI[1],.Last_Storage_DI=E_E990,.Storage_DI=0xE990,.Storage_Num=10,.Clr_Flag=1},
//E9A0		R		74	前1次C失流记录（注2）
//┅	┅	┅	┅	┅	┅
//E9A9		R		74	前10次C失流记录（注2）
{.Event_ID=ID_EVENT_C_LOSS_CURR,.Property=0,.pData=&Event_DI[1],.Last_Storage_DI=E_E9A0,.Storage_DI=0xE9A0,.Storage_Num=10,.Clr_Flag=1},
//E9B0		R		74	前1次AB失流记录（注2）
//┅	┅	┅	┅	┅	┅
//E9B9		R		74	前10次AB失流记录（注2）
{.Event_ID=ID_EVENT_AB_LOSS_CURR,.Property=0,.pData=&Event_DI[1],.Last_Storage_DI=E_E9B0,.Storage_DI=0xE9B0,.Storage_Num=10,.Clr_Flag=1},
//E9C0		R		74	前1次AC失流记录（注2）
//┅	┅	┅	┅	┅	┅
//E9C9		R		74	前10次AC失流记录（注2）
{.Event_ID=ID_EVENT_AC_LOSS_CURR,.Property=0,.pData=&Event_DI[1],.Last_Storage_DI=E_E9C0,.Storage_DI=0xE9C0,.Storage_Num=10,.Clr_Flag=1},
//E9D0		R		74	前1次BC失流记录（注2）
//┅	┅	┅	┅	┅	┅
//E9D9		R		74	前10次BC失流记录（注2）
{.Event_ID=ID_EVENT_BC_LOSS_CURR,.Property=0,.pData=&Event_DI[1],.Last_Storage_DI=E_E9D0,.Storage_DI=0xE9D0,.Storage_Num=10,.Clr_Flag=1},
//EA00		R		10	前1次A相反向记录（注3）
//┅	┅	┅	┅	┅	┅
//EA09		R		10	前10次A相反向记录（注3）
{.Event_ID=ID_EVENT_A_NEG_CURR,.Property=0,.pData=&Event_DI[2],.Last_Storage_DI=E_EA00,.Storage_DI=0xEA00,.Storage_Num=10,.Clr_Flag=1},
//EA10		R		10	前1次B相反向记录（注3）
//┅	┅	┅	┅	┅	┅
//EA19		R		10	前10次B相反向记录（注3）
{.Event_ID=ID_EVENT_B_NEG_CURR,.Property=0,.pData=&Event_DI[2],.Last_Storage_DI=E_EA10,.Storage_DI=0xEA10,.Storage_Num=10,.Clr_Flag=1},
//EA20		R		10	前1次C相反向记录（注3）
//┅	┅	┅	┅	┅	┅
//EA29		R		10	前10次C相反向记录（注3）
{.Event_ID=ID_EVENT_C_NEG_CURR,.Property=0,.pData=&Event_DI[2],.Last_Storage_DI=E_EA20,.Storage_DI=0xEA20,.Storage_Num=10,.Clr_Flag=1},
//EA30		R		10	前1次全失压记录（注4）
//┅	┅	┅	┅	┅	┅
//EA39		R		10	前10次全失压记录（注4）
{.Event_ID=ID_EVENT_ALL_LOSS_VOLT,.Property=0,.pData=&Event_DI[16],.Last_Storage_DI=E_EA30,.Storage_DI=0xEA30,.Storage_Num=10,.Clr_Flag=1},
//EA40		R		74	前1次A相断相记录（注4）
//┅	┅	┅	┅	┅	┅
//EA49		R		74	前10次A相断相记录（注4）
{.Event_ID=ID_EVENT_A_LOSS_PARSE,.Property=0,.pData=&Event_DI[3],.Last_Storage_DI=E_EA40,.Storage_DI=0xEA40,.Storage_Num=10,.Clr_Flag=1},
//EA50		R		74	前1次B相断相记录（注4）
//┅	┅	┅	┅	┅	┅
//EA59		R		74	前10次B相断相记录（注4）
{.Event_ID=ID_EVENT_B_LOSS_PARSE,.Property=0,.pData=&Event_DI[3],.Last_Storage_DI=E_EA50,.Storage_DI=0xEA50,.Storage_Num=10,.Clr_Flag=1},
//EA60		R		74	前1次C相断相记录（注4）
//┅	┅	┅	┅	┅	┅
//EA69		R		74	前10次C相断相记录（注4）
{.Event_ID=ID_EVENT_C_LOSS_PARSE,.Property=0,.pData=&Event_DI[3],.Last_Storage_DI=E_EA60,.Storage_DI=0xEA60,.Storage_Num=10,.Clr_Flag=1},
//EA70		R		10	前1次电压逆相序记录（注4）
//┅	┅	┅	┅	┅	┅
//EA79		R		10	前10次电压逆相序记录（注4）
{.Event_ID=ID_EVENT_VOLT_NEG_SEQ,.Property=0,.pData=&Event_DI[3],.Last_Storage_DI=E_EA70,.Storage_DI=0xEA70,.Storage_Num=10,.Clr_Flag=0},
//EA80		R		13	前1次A相电压超合格上限记录（注5）
//┅	┅	┅	┅	┅	┅
//EA89		R		13	前10次A相电压超合格上限记录（注5）
{.Event_ID=ID_EVENT_A_VOLT_MAX,.Property=0,.pData=&Event_DI[4],.Last_Storage_DI=E_EA80,.Storage_DI=0xEA80,.Storage_Num=10,.Clr_Flag=0},
//EA90		R		13	前1次B相电压超合格上限记录（注5）
//┅	┅	┅	┅	┅	┅
//EA99		R		13	前10次B相电压超合格上限记录（注5）
{.Event_ID=ID_EVENT_B_VOLT_MAX,.Property=0,.pData=&Event_DI[5],.Last_Storage_DI=E_EA90,.Storage_DI=0xEA90,.Storage_Num=10,.Clr_Flag=0},
//EAA0		R		13	前1次C相电压超合格上限记录（注5）
//┅	┅	┅	┅	┅	┅
//EAA9		R		13	前10次C相电压超合格上限记录（注5）
{.Event_ID=ID_EVENT_C_VOLT_MAX,.Property=0,.pData=&Event_DI[6],.Last_Storage_DI=E_EAA0,.Storage_DI=0xEAA0,.Storage_Num=10,.Clr_Flag=0},
//EAB0		R		13	前1次A相电压超合格下限记录（注5）
//┅	┅	┅	┅	┅	┅
//EAB9		R		13	前10次A相电压超合格下限记录（注5）
{.Event_ID=ID_EVENT_A_VOLT_MIN,.Property=0,.pData=&Event_DI[4],.Last_Storage_DI=E_EAB0,.Storage_DI=0xEAB0,.Storage_Num=10,.Clr_Flag=0},
//EAC0		R		13	前1次B相电压超合格下限记录（注5）
//┅	┅	┅	┅	┅	┅
//EAC9		R		13	前10次B相电压超合格下限记录（注5）
{.Event_ID=ID_EVENT_B_VOLT_MIN,.Property=0,.pData=&Event_DI[5],.Last_Storage_DI=E_EAC0,.Storage_DI=0xEAC0,.Storage_Num=10,.Clr_Flag=0},
//EAD0		R		13	前1次C相电压超合格下限记录（注5）
//┅	┅	┅	┅	┅	┅
//EAD9		R		13	前10次C相电压超合格下限记录（注5）
{.Event_ID=ID_EVENT_C_VOLT_MIN,.Property=0,.pData=&Event_DI[6],.Last_Storage_DI=E_EAD0,.Storage_DI=0xEAD0,.Storage_Num=10,.Clr_Flag=0},
//EAE0		R		13	前1次超需量报警记录（注6）
//┅	┅	┅	┅	┅	┅
//EAE9		R		13	前10次超需量报警记录（注6）
{.Event_ID=ID_EVENT_DEMAND_ALARM,.Property=0,.pData=&Event_DI[7],.Last_Storage_DI=E_EAE0,.Storage_DI=0xEAE0,.Storage_Num=10,.Clr_Flag=0},
//EAF0		R		8	前1次超需量跳闸记录（注7）
//┅	┅	┅	┅	┅	┅
//EAF9		R		8	前10次超需量跳闸记录（注7）
{.Event_ID=ID_EVENT_DEMAND_SWITCH,.Property=0,.pData=&Event_DI[8],.Last_Storage_DI=E_EAF0,.Storage_DI=0xEAF0,.Storage_Num=10,.Clr_Flag=0},
//EB20		R		10	前1次上下电记录（注4）
//┅	┅	┅	┅	┅	┅
//EB29		R		10	前10次上下电记录（注4）
{.Event_ID=ID_EVENT_POWER_OFF,.Property=0,.pData=&Event_DI[3],.Last_Storage_DI=E_EB20,.Storage_DI=0xEB20,.Storage_Num=10,.Clr_Flag=0},
//EB30		R		5	前1次复位记录(注8)
//┅	┅	┅	┅	┅	┅
//EB39		R		5	前10次复位记录(注8)
{.Event_ID=ID_EVENT_RESET,.Property=0,.pData=&Event_DI[9],.Last_Storage_DI=E_EB30,.Storage_DI=0xEB30,.Storage_Num=10,.Clr_Flag=0},
//EB40		R		38	前1次通讯清需量记录（注9）
//┅	┅	┅	┅	┅	┅
//EB49		R		38	前10次通讯清需量记录（注9）
{.Event_ID=ID_EVENT_CLR_DEMAND_COM,.Property=0,.pData=&Event_DI[10],.Last_Storage_DI=E_EB40,.Storage_DI=0xEB40,.Storage_Num=10,.Clr_Flag=0},
//EB50		R		38	前1次手动清需量记录（注9）
//┅	┅	┅	┅	┅	┅
//EB59		R		38	前10次手动清需量记录（注9）
{.Event_ID=ID_EVENT_CLR_DEMAND_MAN,.Property=0,.pData=&Event_DI[10],.Last_Storage_DI=E_EB50,.Storage_DI=0xEB50,.Storage_Num=10,.Clr_Flag=0},
//EB60		R		5	前1次清过程记录（注8） 
//┅	┅	┅	┅	┅	┅        
//EB69		R		5	前10次清过程记录（注8）
{.Event_ID=ID_EVENT_CLR_PROCESS,.Property=0,.pData=&Event_DI[9],.Last_Storage_DI=E_EB60,.Storage_DI=0xEB60,.Storage_Num=10,.Clr_Flag=0},
//EB70		R		5	前1次清电压合格率记录（注8）
//┅	┅	┅	┅	┅	┅
//EB79		R		5	前10次清电压合格率记录（注8）
{.Event_ID=ID_EVENT_CLR_VOLT_PASS,.Property=0,.pData=&Event_DI[9],.Last_Storage_DI=E_EB70,.Storage_DI=0xEB70,.Storage_Num=10,.Clr_Flag=0},
//EB80		R		78	前1次设置初始电量记录（注10）
//┅	┅	┅	┅	┅	┅
//EB89		R		78	前10次设置初始电量记录（注10）
//EB90		R		7	前1次编程记录（注11）
//┅	┅	┅	┅	┅	┅
//EB99		R		7	前10次编程记录（注11）
{.Event_ID=ID_EVENT_PROG,.Property=0,.pData=&Event_DI[11],.Last_Storage_DI=E_EB90,.Storage_DI=0xEB90,.Storage_Num=10,.Clr_Flag=0},
//EBA0		R		5	前1次开端盖起始记录（注8）--->修改为开大盖，也就是开上盖！！！！！,规约描述有错，修改之
//┅	┅	┅	┅	┅	┅
//EBA9		R		5	前10次开端盖起始记录（注8）
{.Event_ID=ID_EVENT_UP_COVER_OPEN,.Property=0,.pData=&Event_DI[9],.Last_Storage_DI=E_EBA0,.Storage_DI=0xEBA0,.Storage_Num=10,.Clr_Flag=0},
//ED90		R		5	前1次开端盖结束记录（注8）--->修改为开大盖，也就是开上盖！！！！！,规约描述有错，修改之
//┅	┅	┅	┅	┅	┅
//ED99		R		5	前10次开端盖结束记录（注8）
{.Event_ID=ID_EVENT_UP_COVER_CLOSE,.Property=0,.pData=&Event_DI[9],.Last_Storage_DI=E_ED90,.Storage_DI=0xED90,.Storage_Num=10,.Clr_Flag=0},
//EBB0		R		5	前1次开铅封门起始记录（注8）
//┅	┅	┅	┅	┅	┅
//EBB9		R		5	前10次开铅封门起始记录（注8）
{.Event_ID=ID_EVENT_LEAD_COVER_OPEN,.Property=0,.pData=&Event_DI[9],.Last_Storage_DI=E_EBB0,.Storage_DI=0xEBB0,.Storage_Num=10,.Clr_Flag=0},
//EDA0		R		5	前1次开铅封门结束记录（注8）
//┅	┅	┅	┅	┅	┅
//EDA9		R		5	前10次开铅封门结束记录（注8）
{.Event_ID=ID_EVENT_LEAD_COVER_CLOSE,.Property=0,.pData=&Event_DI[9],.Last_Storage_DI=E_EDA0,.Storage_DI=0xEDA0,.Storage_Num=10,.Clr_Flag=0},

//EDB0		R		5	前1次开大盖起始记录（注8）---->修改为开端盖,规约描述有错，修改之
//┅	┅	┅	┅	┅	┅
//EDB9		R		5	前10次开大盖起始记录（注8）
{.Event_ID=ID_EVENT_DOWN_COVER_OPEN,.Property=0,.pData=&Event_DI[9],.Last_Storage_DI=E_EDB0,.Storage_DI=0xEDB0,.Storage_Num=10,.Clr_Flag=0},
//EDC0		R		5	前1次开大盖结束记录（注8）---->修改为开端盖,规约描述有错，修改之
//┅	┅	┅	┅	┅	┅
//EDC9		R		5	前10次开大盖结束记录（注8）
{.Event_ID=ID_EVENT_DOWN_COVER_CLOSE,.Property=0,.pData=&Event_DI[9],.Last_Storage_DI=E_EDC0,.Storage_DI=0xEDC0,.Storage_Num=10,.Clr_Flag=0},
//EDD0		R		5	前1次开计量柜门起始记录（注8）
//┅	┅	┅	┅	┅	┅
//EDD9		R		5	前10次开计量柜门起始记录（注8）
{.Event_ID=ID_EVENT_DOOR_OPEN,.Property=0,.pData=&Event_DI[9],.Last_Storage_DI=E_EDD0,.Storage_DI=0xEDD0,.Storage_Num=10,.Clr_Flag=0},
//EDE0		R		5	前1次开计量柜门结束记录（注8）
//┅	┅	┅	┅	┅	┅
//EDE9		R		5	前10次开计量柜门结束记录（注8）
{.Event_ID=ID_EVENT_DOOR_CLOSE,.Property=0,.pData=&Event_DI[9],.Last_Storage_DI=E_EDE0,.Storage_DI=0xEDE0,.Storage_Num=10,.Clr_Flag=0},

//EBC0		R		10	前1次时钟电池欠压记录（注4）
//┅	┅	┅	┅	┅	┅
//EBC9		R		10	前10次时钟电池欠压记录（注4）
{.Event_ID=ID_EVENT_RTC_BAT_LOW,.Property=0,.pData=&Event_DI[3],.Last_Storage_DI=E_EBC0,.Storage_DI=0xEBC0,.Storage_Num=10,.Clr_Flag=0},
//EBD0		R		10	前1次低功耗电池欠压记录（注4）
//┅	┅	┅	┅	┅	┅
//EBD9		R		10	前10次低功耗电池欠压记录（注4）
{.Event_ID=ID_EVENT_LOWPWR_BAT_LOW,.Property=0,.pData=&Event_DI[3],.Last_Storage_DI=E_EBD0,.Storage_DI=0xEBD0,.Storage_Num=10,.Clr_Flag=0},
//E400		R		10	前1次校时事件记录（注4）
//┅		R			
//E409		R		10	前10次校时事件记录（注4）
//┅		R			
//E499		R		10	前100次校时事件记录（注4）
{.Event_ID=ID_EVENT_ADJUST_TIME,.Property=0,.pData=&Event_DI[3],.Last_Storage_DI=E_E400,.Storage_DI=0xE400,.Storage_Num=ADJ_TIME_EVENT_NUM,.Clr_Flag=0},
//EBF0		R		11	前1次运行时钟乱记录（注12）
//┅	┅	┅	┅	┅	┅
//EBF9		R		11	前10次运行时钟乱记录（注12）
{.Event_ID=ID_EVENT_RTCERR_RUN,.Property=0,.pData=&Event_DI[12],.Last_Storage_DI=E_EBF0,.Storage_DI=0xEBF0,.Storage_Num=10,.Clr_Flag=0},
//EC00		R		11	前1次上电时钟乱记录（注12）
//┅	┅	┅	┅	┅	┅
//EC09		R		11	前10次上电时钟乱记录（注12）
{.Event_ID=ID_EVENT_RTCERR_PWRON,.Property=0,.pData=&Event_DI[12],.Last_Storage_DI=E_EC00,.Storage_DI=0xEC00,.Storage_Num=10,.Clr_Flag=0},
//EC20		R		6	前1次EEPROM坏记录（注13）
//┅	┅	┅	┅	┅	┅
//EC29		R		6	前10次EEPROM坏记录（注13）
{.Event_ID=ID_EVENT_EPPROM_FAULT,.Property=0,.pData=&Event_DI[13],.Last_Storage_DI=E_EC20,.Storage_DI=0xEC20,.Storage_Num=10,.Clr_Flag=0},
//EC30		R		6	前1次校表记录(注14)
//┅	┅	┅	┅	┅	┅
//EC39		R		6	前10次校表记录(注14)
{.Event_ID=ID_EVENT_ADJUST_MEASU,.Property=0,.pData=&Event_DI[17],.Last_Storage_DI=E_EC30,.Storage_DI=0xEC30,.Storage_Num=10,.Clr_Flag=0},
//EC40		R		10	前1次全失流记录（注4）
//┅	┅	┅	┅	┅	┅
//EC49		R			前10次全失流记录（注4）
//EC50		R			前11次全失流记录（注4）
//┅	┅	┅	┅	┅	┅
//EC59		R		10	前20次全失流记录（注4）
//┅ ┅	┅ ┅	┅	┅	┅ ┅	┅ ┅
//ECD0		R		10	前91次全失流记录（注4）
//┅	┅	┅	┅	┅	┅
//ECD9		R		10	前100次全失流记录（注4）
{.Event_ID=ID_EVENT_ALL_LOSS_CURR,.Property=0,.pData=&Event_DI[3],.Last_Storage_DI=E_EC40,.Storage_DI=0xEC40,.Storage_Num=ALL_LOSS_CUR_EVENT_NUM,.Clr_Flag=1},
//ED00		R		6	前1次电量纠错时间及类型（注16）
//ED01		R		36	前1次电量纠错前正向有功总及8费率电量（注15）
//ED02		R		36	前1次电量纠错前反向有功总及8费率电量（注15）
//ED03		R		36	前1次电量纠错前一象限无功总及8费率电量（注15）
//ED04		R		36	前1次电量纠错前二象限无功总及8费率电量（注15）
//ED05		R		36	前1次电量纠错前三象限无功总及8费率电量（注15）
//ED06		R		36	前1次电量纠错前四象限无功总及8费率电量（注15）
//ED07		R		36	前1次电量纠错后正向有功总及8费率电量（注15）
//ED08		R		36	前1次电量纠错后反向有功总及8费率电量（注15）
//ED09		R		36	前1次电量纠错后一象限无功总及8费率电量（注15）
//ED0A		R		36	前1次电量纠错后二象限无功总及8费率电量（注15）
//ED0B		R		36	前1次电量纠错后三象限无功总及8费率电量（注15）
//ED0C		R		36	前1次电量纠错后四象限无功总及8费率电量（注15）
//ED10		R		6	前2次电量纠错时间及类型（注16）
//ED11		R		36	前2次电量纠错前正向有功总及8费率电量（注15）
//┅	┅	┅	┅	┅	┅
//ED1C		R		36	前2次电量纠错后四象限无功总及8费率电量（注15）
//ED20		R		6	前3次电量纠错时间及类型（注16）
//ED21		R		36	前3次电量纠错后正向有功总及8费率电量（注15）
//┅	┅	┅	┅	┅	┅
//ED2C		R		36	前3次电量纠错后四象限无功总及8费率电量（注15）
//ED30		R		13	前1次A相过压记录（注5）
//┅	┅	┅	┅	┅	┅
//ED39		R		13	前10次A相过压记录（注5）
{.Event_ID=ID_EVENT_A_VOLT_OVERLOAD,.Property=0,.pData=&Event_DI[6],.Last_Storage_DI=E_ED30,.Storage_DI=0xED30,.Storage_Num=10,.Clr_Flag=0},
//ED40		R		13	前1次B相过压记录（注5）
//┅	┅	┅	┅	┅	┅
//ED49		R		13	前10次B相过压记录（注5）
{.Event_ID=ID_EVENT_B_VOLT_OVERLOAD,.Property=0,.pData=&Event_DI[6],.Last_Storage_DI=E_ED40,.Storage_DI=0xED40,.Storage_Num=10,.Clr_Flag=0},
//ED50		R		13	前1次C相过压记录（注5）
//┅	┅	┅	┅	┅	┅
//ED59		R		13	前10次C相过压记录（注5）
{.Event_ID=ID_EVENT_C_VOLT_OVERLOAD,.Property=0,.pData=&Event_DI[6],.Last_Storage_DI=E_ED50,.Storage_DI=0xED50,.Storage_Num=10,.Clr_Flag=0},
//ED60		R		21	前1次总电量立即冻结记录（注18）
//┅	┅	┅	┅	┅	┅
//ED69		R		21	前10次总电量立即冻结记录（注18）
//ED70		R		21	前11次总电量立即冻结记录（注18）
//┅	┅	┅	┅	┅	┅
//ED79		R		21	前20次总电量立即冻结记录（注18）
{.Event_ID=ID_EVENT_ENERG_FREEZ,.Property=0,.pData=&Event_DI[15],.Last_Storage_DI=E_ED60,.Storage_DI=0xED60,.Storage_Num=20,.Clr_Flag=0},

EVENT_MULTITIMES_EXTEND
};
//获取某个事件在事件信息表中的索引值
//Event_ID表示该事件的ID值
INT16U Get_Event_Data_Index(INT8U Event_ID)
{
  INT16U i;
  for(i=0;i<S_NUM(Event_Data_Info);i++)
  {
    if(Event_ID==Event_Data_Info[i].Event_ID)
      return i;
  }
  return NULL_2BYTES;
}

//DI表示事件记录的DI
//返回该DI对应的事件在Event_Data_Info中的索引值
//返回NULL_2BYTES表示没有该DI对应的事件
INT16U Get_Event_Data_Index_From_DI(PROTO_DI DI)
{
  INT16U i;
  
  for(i=0;i<S_NUM(Event_Data_Info);i++)
  {
    //if(DI>=Event_Data_Info[i].Storage_DI && DI<Event_Data_Info[i].Storage_DI+Event_Data_Info[i].Storage_Num)
    if((DI & 0xFF00) == (Event_Data_Info[i].Storage_DI & 0xFF00) && (DI & 0x00FF) >= (Event_Data_Info[i].Storage_DI & 0x00FF) &&\
       Bcd2Hex_Byte((INT8U)(DI & 0xFF))-Bcd2Hex_Byte((INT8U)(Event_Data_Info[i].Storage_DI & 0xFF))<Event_Data_Info[i].Storage_Num)  
      return i;
  }
  //ASSERT(A_WARNING,0);
  return NULL_2BYTES;
}

//读取事件Event_ID的最近一次事件的事件记录存储DI
STORA_DI Read_Last_Event_Storage_DI(INT8U Event_ID)
{
  INT16U Index,Len;
  STORA_DI Storage_DI;
  INT8U Temp[6],Err;
  
  Index=Get_Event_Data_Index(Event_ID);//获取事件在Event_Data_Info中的索引值 
  if(ASSERT(A_WARNING,NULL_2BYTES!=Index))
    return NULL_STORA_DI;
  
  Len=Read_Storage_Data(Event_Data_Info[Index].Last_Storage_DI,Temp,Temp,sizeof(Temp),&Err);
  if(ASSERT(A_WARNING,Len==4 && Err==NO_ERR)) //没有读取到上次发生事件的记录
    return NULL_STORA_DI;//返回一个不存在的DI
  
  //Storage_DI=*(STORA_DI *)Temp;//读取到的4个字节是上次存储事件的存储DI
  mem_cpy(&Storage_DI,Temp,4,&Storage_DI,sizeof(Storage_DI));
  
  if(Storage_DI == NULL_EVENT_STORAGE_DI)
    return Storage_DI;
  
  //判断Storage_DI是否在合理范围内
  if(!(Event_Data_Info[Index].Storage_Num>1 &&\
      Storage_DI>=Event_Data_Info[Index].Storage_DI &&\
      Storage_DI<Event_Data_Info[Index].Storage_DI+Event_Data_Info[Index].Storage_Num))
  {
    ASSERT(A_WARNING,0);
    Storage_DI=NULL_STORA_DI;
  }
  return Storage_DI;
}

//该函数用来读取上次发生的某事件的记录,只有MULTI_RECORD_EVENT类事件才需要调用该函数
//Event_ID表示事件的ID
//Storage_DI,最近一条记录的存储DI
//pDst表示目标缓冲区的起始
//pDst_Start和DstLen一起限定pDst和DstLen的范围
//返回值，数据长度，为0表示没有读取到，可能是校验和错误或者是根本没有存储
INT16U Read_Last_Event_Data(INT8U Event_ID,STORA_DI Storage_DI,void *pDst,void *pDst_Start,INT16U DstLen)
{
  INT8U Err;
  INT16U i,Len;
  
  //Storage_DI=Read_Last_Event_Storage_DI(EVENT_ID);
  ASSERT(A_WARNING,Storage_DI!=NULL_4BYTES);
  //读取最近发生的一次事件记录
  Len=Read_Storage_Data(Storage_DI,pDst,pDst_Start,DstLen,&Err);
  
  //判断数据长度是否正确
  i=Get_Event_Data_Index(Event_ID);
  //检查返回数据的合法性
  if(ASSERT(A_WARNING,NULL_2BYTES!=i && Event_Data_Info[i].pData->Data_Len==Len && NO_ERR==Err))
    return 0;
  else
    return Len; 
}

//写入最近一次失压时间
void Event_Loss_Vol_Spec_Proc(INT8U Event_ID,INT8U Occur_Or_End,INT8U Flag)
{
  STORA_DI DI;
  INT8U Temp[5];
  
  if(Flag != EVENT_REAL)
    return;

  switch(Event_ID)
  {
      case ID_EVENT_A_LOSS_VOLT:
      DI = 0xB331;
      break;
      case ID_EVENT_B_LOSS_VOLT:
      DI = 0xB332;  
      break;
      case ID_EVENT_C_LOSS_VOLT:
      DI = 0xB333;  
      break;
      case ID_EVENT_AB_LOSS_VOLT:
      DI = 0xB334;
      break;
      case ID_EVENT_AC_LOSS_VOLT:
      DI = 0xB335;
      break;
      case ID_EVENT_BC_LOSS_VOLT:
      DI = 0xB336; 
      break;
      case ID_EVENT_ABC_LOSS_VOLT:
      DI = 0xB337;
      break;
      case ID_EVENT_ALL_LOSS_VOLT:
      DI = 0xB338;
      
      default:
       return;      
  }
  if(Occur_Or_End EQ EVENT_END)
    DI = DI + 0x0010;
 

  Temp[0] = Cur_Time1.Min;//事件发生时间
  Temp[1] = Cur_Time1.Hour;
  Temp[2] = Cur_Time1.Date;
  Temp[3] = Cur_Time1.Month;
    
  if(Event_ID EQ ID_EVENT_ALL_LOSS_VOLT)//全失压起始结束时间需要特殊处理
  {
     if(Occur_Or_End EQ EVENT_OCCUR)
       mem_cpy(Temp,(INT8U *)All_Loss_Vol_Time.Start_Time, 5, Temp, sizeof(Temp));//全失压的起始和终止时间需要特殊处理 
     else
       mem_cpy(Temp,(INT8U *)All_Loss_Vol_Time.End_Time, 5, Temp, sizeof(Temp));
  }  
  
  Write_Storage_Data(DI, Temp, 4);
  
  if(Get_Total_Event_ID(Event_ID)==ID_EVENT_TOTAL_LOSS_VOL)//失压数据需要计入总?
    Write_Storage_Data((DI & 0xFFF0), Temp, 4);//最近一次失压起始或结束时刻
}

INT16U Event_MultiTimes_Proc(INT8U Event_ID, INT8U Occur_Or_End, INT8U Flag)
{
  INT8U Return_Flag;
  INT16U Len;
  
  //if(MAX_MULTI_TIMES_DI_NUM != S_NUM(Event_DI))
    //ASSERT(A_WARNING,0);
  DEBUG_PRINT(HUCK,DEBUG_0,"Event_MultiTimes_Proc ID:%d",Event_ID);
  Len = Extend_Event_MultiTimes_Proc(Event_ID, Occur_Or_End, Flag, &Return_Flag);
  if(Return_Flag EQ 1)
    return Len;  
  
  return Event_MultiTimes_Normal_Proc(Event_ID, Occur_Or_End, Flag);
}
//该函数用来生成并存储报警事件数据
//Event_ID表示事件的ID
//Occur_Or_End 表示是该事件的发生还是结束,用两个宏表示EVENT_OCCUR和EVENT_END
//返回NO_ERR表示成功，其他表示失败
//Flag:EVENT_REAL,EVENT_VIRTRUAL
INT16U Event_MultiTimes_Normal_Proc(INT8U Event_ID,INT8U Occur_Or_End,INT8U Flag)
{
  INT8U i,Last_Data_Flag,FollowFlag,Err,Data_Flag;
  INT16U Index;
  INT16U Len,Len1;
  STORA_DI Storage_DI;
  
  Index=Get_Event_Data_Index(Event_ID);
  if(NULL_2BYTES==Index) //没有找到该事件在Event_Data_Info中的索引，返回0
  {
    DEBUG_PRINT(HUCK,DEBUG_0,"No such Event_ID");
    return 0;    
  }
  
  Event_Loss_Vol_Spec_Proc(Event_ID,Occur_Or_End,Flag);
  
  Event_Vol_Cur_Var_Pre_Set(Event_ID,Occur_Or_End);//过压过流等事件的数据预处理
  if(Occur_Or_End==EVENT_OCCUR)
  {
    //DEBUG_PRINT(HUCK,DEBUG_0,"Event_MultiTimes_Proc ID:%d,Occur!",Event_ID);
    if((Event_Data_Info[Index].pData)->Occur_Mask==0)
    {
      //ASSERT(A_WARNING,0);
      DEBUG_PRINT(HUCK,DEBUG_0,"Event no need to save!");
      return 0;
    }
  }
  else
  {
    //DEBUG_PRINT(HUCK,DEBUG_0,"Event_MultiTimes_Proc ID:%d,End!",Event_ID);
    if((Event_Data_Info[Index].pData)->End_Mask==0)
    {
      //ASSERT(A_WARNING,0); 
      DEBUG_PRINT(HUCK,DEBUG_0,"Event no need to save!");
      return 0;
    }
  }
  
  //下面先生成事件记录的数据
  Len=0;
  Last_Data_Flag=0;//是否成功读取到上次事件发生时的数据,1表示读取到，0表示失败
  
  //如果事件是多条记录的事件,需要读出最近一次事件的存储位置
  if(Event_Data_Info[Index].Storage_Num>1)
    Storage_DI=Read_Last_Event_Storage_DI(Event_ID);
  else
    Storage_DI=Event_Data_Info[Index].Storage_DI;
  
  if(Storage_DI == NULL_EVENT_STORAGE_DI)//还没有发生过该事件或者已经清0过
    Storage_DI=Event_Data_Info[Index].Storage_DI;
  
  //判断Storage_DI的合法性
  if(!(Storage_DI>=Event_Data_Info[Index].Storage_DI &&\
    Storage_DI<Event_Data_Info[Index].Storage_DI+Event_Data_Info[Index].Storage_Num))
  {
    ASSERT(A_WARNING,0);
    Storage_DI=Event_Data_Info[Index].Storage_DI;  
  }
  //如果是事件结束，且是多记录事件，且在事件发生和结束时都要记录数据,则需要读取最近一次发生的事件记录
  if(EVENT_END==Occur_Or_End &&\
    Event_Data_Info[Index].Storage_Num>1 &&\
    (Event_Data_Info[Index].pData)->Occur_Mask!=0 &&\
    (Event_Data_Info[Index].pData)->End_Mask!=0)
  {
    if(Storage_DI!=NULL_STORA_DI)
    {
      //读出事件发生时的数据，没有读出则返回错误
      Len=Read_Last_Event_Data(Event_ID,Storage_DI,(INT8U *)Pub_Buf0,(INT8U *)Pub_Buf0,sizeof(Pub_Buf0));
      if(ASSERT(A_WARNING,Len==(Event_Data_Info[Index].pData)->Data_Len && Len<=(sizeof(Pub_Buf0)/2)))
      {
        return 0;
      }
      
      if(Pub_Buf0[0]!=EVENT_OCCUR)//不是事件发生时数据?
      {
        ASSERT(A_WARNING,0);
        return 0;
      }
      Last_Data_Flag=1;
      //此时Storage_DI就是该次事件需要存储的DI
    }		
  }
  //如果是事件发生，且是多记录事件,则需要在原记录DI基础上+1
  else if(EVENT_OCCUR==Occur_Or_End &&\
    Event_Data_Info[Index].Storage_Num>1)
  {
    if(Storage_DI!=NULL_STORA_DI)//当前事件的存储DI，应该在上条事件的基础上增加
    {
      Storage_DI++;
      if(!(Storage_DI>=Event_Data_Info[Index].Storage_DI &&\
        Storage_DI<Event_Data_Info[Index].Storage_DI+Event_Data_Info[Index].Storage_Num))
        Storage_DI=Event_Data_Info[Index].Storage_DI;
    }
    else
      Storage_DI=Event_Data_Info[Index].Storage_DI;
    
  }
  

  Pub_Buf0[0]=Occur_Or_End;  //第0字节记录当前是发生或者结束
  Len=1;
  
  //生成当前事件的数据
  for(i=0;i<(Event_Data_Info[Index].pData)->Data_Num;i++)
  {
    //如果是需要计算差量的数据项目，则生成新的数据后计算差量
    if(Occur_Or_End==EVENT_OCCUR)//事件发生
    {
      if((Event_Data_Info[Index].pData)->Occur_Mask!=0)//发生时需要记录数据项
      {
        Data_Flag=FOR_EVENT;
        if(GET_BIT((Event_Data_Info[Index].pData)->Diff_Mask,i)!=0)//需要计算差的数据，获取数据为原始数据
          Data_Flag|=FOR_SRC;
        
        //获取对应的645数据项
        Len1=_Get_DLT645_Data((Event_Data_Info[Index].pData)->DI[i],(void *)0,0,(INT8U *)Pub_Buf0+Len,(INT8U *)Pub_Buf0,sizeof(Pub_Buf0),Data_Flag,&FollowFlag,&Err);
        if(!(Len1>0 && Err==NO_ERR))
        {
          ASSERT(A_WARNING,0);
          return 0;
        }
        
        //某数据项在事件发生时不填充数据，则发生时置0
        if(GET_BIT((Event_Data_Info[Index].pData)->Occur_Mask,i)==0)
          mem_set((INT8U *)Pub_Buf0+Len,0,Len1,(INT8U *)Pub_Buf0,sizeof(Pub_Buf0));
        Len+=Len1;
        if(!(Len<=(sizeof(Pub_Buf0)/2)))
        {
          ASSERT(A_WARNING,0);
          return 0;
        }
      }
    }
    else//事件结束
    {
      if((Event_Data_Info[Index].pData)->End_Mask!=0)//结束时需要记录数据项
      {
        Data_Flag=FOR_EVENT;
        if(GET_BIT((Event_Data_Info[Index].pData)->Diff_Mask,i)!=0)//需要计算差数据，获取数据为原始数据
          Data_Flag|=FOR_SRC;
        
        //前面已经将数据发生时的数据读取到Pub_Buf0的前半部分，这里读取结束时的数据到后半部分
        Len1=_Get_DLT645_Data((Event_Data_Info[Index].pData)->DI[i],(void *)0,0,(INT8U *)Pub_Buf0+(sizeof(Pub_Buf0)/2)+Len,(INT8U *)Pub_Buf0,sizeof(Pub_Buf0),Data_Flag,&FollowFlag,&Err);
        if(!(Len1>0 && NO_ERR==Err))
        {
          ASSERT(A_WARNING,0);
          return 0;
        }
        
        //计算发生和结束之间的数据差
        if(GET_BIT((Event_Data_Info[Index].pData)->End_Mask,i)!=0)
        {
          //Last_Data_Flag==1说明已经读取到了上次发生时的数据
          if(1==Last_Data_Flag)
          {
            if(GET_BIT((Event_Data_Info[Index].pData)->Diff_Mask,i)!=0)//需要计算差的数据在获取源数据时都是原始数据
            {
              
              Proto_Data_Add_Dec(FOR_COMM, DATA_DEC, (PROTO_DI *)&((Event_Data_Info[Index].pData)->DI[i]), 1, \
                                 (INT8U *)Pub_Buf0 + (sizeof(Pub_Buf0)/2) + Len, (INT8U *)Pub_Buf0 + Len, (INT8U *)Pub_Buf0 + Len,\
                                 (INT8U *)Pub_Buf0, sizeof(Pub_Buf0));

            }
            else
            {
              if(EVENT_REAL==Flag)
                mem_cpy((INT8U *)Pub_Buf0+Len,(INT8U *)Pub_Buf0+(sizeof(Pub_Buf0)/2)+Len,Len1,(INT8U *)Pub_Buf0,sizeof(Pub_Buf0)); 
              else//虚拟事件结束时，需要将结束时数据置0
                mem_set((INT8U *)Pub_Buf0+Len,0,Len1,(INT8U *)Pub_Buf0,sizeof(Pub_Buf0));
            }
          }
        }
        Len+=Len1;
      }
    }
  }
  if(Storage_DI!=NULL_STORA_DI && Len>0 && EVENT_REAL==Flag)//必须是真实事件结束才存数据
  {
    //保存时间内容并将最近一次的存储DI保存到Last_Storage_DI指向的存储区域
    if(Write_Storage_Data(Storage_DI,(INT8U *)Pub_Buf0,Len)==NO_ERR)
      Write_Storage_Data(Event_Data_Info[Index].Last_Storage_DI,&Storage_DI,sizeof(STORA_DI));
  }
  return Len;
}

//获取事件记录信息条数
INT16U Get_Event_Data_Info_Num()
{
  return S_NUM(Event_Data_Info);
}

//过压过流等事件的数据预处理
void Event_Vol_Cur_Var_Pre_Set(INT8U Event_ID,INT8U Occur_Or_End)
{
  if(EVENT_OCCUR==Occur_Or_End)
  {
    switch(Event_ID)
    {
    case ID_EVENT_A_VOLT_MAX:
      Event_Vol_Cur.Pub_Vol=Event_Vol_Cur.Qual_Up_Vol[0];
      break;
    case ID_EVENT_B_VOLT_MAX:
      Event_Vol_Cur.Pub_Vol=Event_Vol_Cur.Qual_Up_Vol[1];
      break;    
    case ID_EVENT_C_VOLT_MAX:
      Event_Vol_Cur.Pub_Vol=Event_Vol_Cur.Qual_Up_Vol[2];
      break;
    case ID_EVENT_A_VOLT_MIN:
      Event_Vol_Cur.Pub_Vol=Event_Vol_Cur.Qual_Low_Vol[0];
      break;
    case ID_EVENT_B_VOLT_MIN:
      Event_Vol_Cur.Pub_Vol=Event_Vol_Cur.Qual_Low_Vol[1];
      break;
    case ID_EVENT_C_VOLT_MIN:
      Event_Vol_Cur.Pub_Vol=Event_Vol_Cur.Qual_Low_Vol[2];
      break;
    case ID_EVENT_A_VOLT_OVERLOAD:
      Event_Vol_Cur.Pub_Vol=Event_Vol_Cur.Over_Vol[0];
      break;
    case ID_EVENT_B_VOLT_OVERLOAD:
      Event_Vol_Cur.Pub_Vol=Event_Vol_Cur.Over_Vol[1];
      break;
    case ID_EVENT_C_VOLT_OVERLOAD:
      Event_Vol_Cur.Pub_Vol=Event_Vol_Cur.Over_Vol[2];
      break;
    }
  }
}

//全失压数据累加处理
//Start_Time表示起始时间,Start_Time[0]->Start_Time[4]表示分、时、日、月、年,BCD码
//End_Time表示终止时间,End_Time[0]->End_Time[4]表示分、时、日、月、年，BCD码
//上电后，对上电前的前N(N<=10)次全失压，调用该函数N次
void All_Loss_Vol_Data_Proc(INT8U Start_Time[],INT8U End_Time[])
{
  DEBUG_PRINT(HUCK,DEBUG_0,"All_Loss_Vol Start_Time:%x-%x-%x %x:%x, End_Time:%x-%x-%x %x:%x",\
              Start_Time[4],Start_Time[3],Start_Time[2],Start_Time[1],Start_Time[0],\
              End_Time[4],End_Time[3],End_Time[2],End_Time[1],End_Time[0]);
  
  mem_cpy((void *)All_Loss_Vol_Time.Start_Time,Start_Time,5,(void *)All_Loss_Vol_Time.Start_Time,sizeof(All_Loss_Vol_Time.Start_Time));
  mem_cpy((void *)All_Loss_Vol_Time.End_Time,End_Time,5,(void *)All_Loss_Vol_Time.End_Time,sizeof(All_Loss_Vol_Time.End_Time));
  //All_Loss_Vol_Time.Mins=Mins;
  SET_STRUCT_SUM(All_Loss_Vol_Time);
  
  //Event_Data_Proc(ID_EVENT_ALL_LOSS_VOLT,EVENT_OCCUR); //全失压的分次处理 
  //Event_Data_Proc(ID_EVENT_ALL_LOSS_VOLT,EVENT_END); //全失压的分次处理
  OS_Sem_Pend(PUB_BUF0_SEM_ID);//申请PUB_BUF0，下面的函数需要用到这个信号量
  Event_MultiTimes_Proc(ID_EVENT_ALL_LOSS_VOLT, EVENT_OCCUR, EVENT_REAL);
  Event_MultiTimes_Proc(ID_EVENT_ALL_LOSS_VOLT, EVENT_END, EVENT_REAL);  
  OS_Sem_Post(PUB_BUF0_SEM_ID);//释放PUB_BUF0  
}

//读取事件记录数据项
//DI表示事件记录的标识码
//pDst表示目标缓冲区
//pDst_Start表示目标缓冲区的起始
//DstLen和pDst_Start一起限定pDst和返回长度
//pErr表示返回为0时对应的原因，如果是NO_ERR表示该DI不是一个事件DI
//返回值为读到的报警数据长度
/*
INT16U Get_MultiTimes_Event_Data(PROTO_DI DI,INT8U *pDst,INT8U *pDst_Start,INT16U DstLen,INT8U *pErr)
{
  //INT8U Err;
  INT16U i,Len;
  STORA_DI Storage_DI;
  
  //获取数据的Index
  *pErr=NO_ERR;
  i=Get_Event_Data_Index_From_DI(DI);
  if(NULL_2BYTES==i)
    return 0;
  
  if(Event_Data_Info[i].Storage_Num==1)//ONE_RECORD_EVENT)
    return Read_Storage_Data(Event_Data_Info[i].Storage_DI,pDst,pDst_Start,DstLen,pErr);
  else
  {
    //先读出最近一次事件的存储DI到Storage_DI
    Len=Read_Storage_Data(Event_Data_Info[i].Last_Storage_DI,pDst,pDst_Start,DstLen,pErr);
    if(ASSERT(A_WARNING,Len==sizeof(STORA_DI) && *pErr==NO_ERR)) //没有读取到上次发生事件的记录
    {
      *pErr=EVENT_DATA_ERR;
      return 0;
    }
    else
      mem_cpy(&Storage_DI,pDst,4,&Storage_DI,sizeof(Storage_DI));//Storage_DI=*(INT32U *)pDst;//读取到的4个字节是上次存储事件的存储DI
    //Storage_DI认为是最后一次存储数据的DI
    if(!(Storage_DI>=Event_Data_Info[i].Storage_DI &&\
      Storage_DI<Event_Data_Info[i].Storage_DI+Event_Data_Info[i].Storage_Num))
    {
      *pErr=EVENT_DATA_ERR;
      return 0;
    }
    
    //DI转换成最近第几次的事件数据?
    //Proto_DI是最近一次的DI
    if(DI>=Event_Data_Info[i].Storage_DI && DI<Event_Data_Info[i].Storage_DI+Event_Data_Info[i].Storage_Num)
      DI=DI-Event_Data_Info[i].Storage_DI;
    else
    {
      *pErr=PROTOCOL_DI_ERR;
      return 0;
    }
    
    if((Storage_DI-DI)<Event_Data_Info[i].Storage_DI)
      Storage_DI=Event_Data_Info[i].Storage_Num+Storage_DI-DI;
    else
      Storage_DI=Storage_DI-DI;
    
    return Read_Storage_Data(Storage_DI,pDst,pDst_Start,DstLen,pErr);
  }
}
*/
//设置默认的分次事件数据
void Set_Def_Event_MultiTimes_Data()
{
  INT16U i,j,Len;
  STORA_DI DI;
  
  DEBUG_PRINT(HUCK,DEBUG_0,"Set Def Event Data");
  DISP_CLR_DATA_INFO;
  OS_Sem_Pend(PUB_BUF0_SEM_ID);
  mem_set((void *)Pub_Buf0,0,sizeof(Pub_Buf0),(void *)Pub_Buf0,sizeof(Pub_Buf0));
  for(i=0;i<S_NUM(Event_Data_Info);i++)
  {
    if(ID_EVENT_PROG==Event_Data_Info[i].Event_ID)//编程记录不清除
      continue;
    
    //Write_Storage_Data(Event_Data_Info[i].Last_Storage_DI,(void *)&Event_Data_Info[i].Storage_DI,sizeof(Event_Data_Info[i].Storage_DI));
    //对每条分次记录清除
    for(j=0;j<Event_Data_Info[i].Storage_Num;j++)
    {
      DI=Event_Data_Info[i].Storage_DI+j;// & 0xFFFFFF00)+Hex2Bcd_Byte(j);
      Len=Get_Storage_Data_Len(DI);
      Pub_Buf0[0]=EVENT_END;//置事件结束字，以免读取事件记录时断言
      Write_Storage_Data(DI,(void *)Pub_Buf0,Len); 
    }
    
    DI=NULL_EVENT_STORAGE_DI;
    Write_Storage_Data(Event_Data_Info[i].Last_Storage_DI,(void *)&DI,sizeof(DI));    
  }
  
 Write_Storage_Data(0xB330,(INT8U *)Pub_Buf0,4),//最近一次失压起始时刻     
 Write_Storage_Data(0xB331,(INT8U *)Pub_Buf0,4),//A相最近一次失压起始时刻  
 Write_Storage_Data(0xB332,(INT8U *)Pub_Buf0,4),//B相最近一次失压起始时刻  
 Write_Storage_Data(0xB333,(INT8U *)Pub_Buf0,4),//C相最近一次失压起始时刻  
 Write_Storage_Data(0xB334,(INT8U *)Pub_Buf0,4),//AB相最近一次失压起始时刻 
 Write_Storage_Data(0xB335,(INT8U *)Pub_Buf0,4),//AC相最近一次失压起始时刻 
 Write_Storage_Data(0xB336,(INT8U *)Pub_Buf0,4),//BC相最近一次失压起始时刻 
 Write_Storage_Data(0xB337,(INT8U *)Pub_Buf0,4),//ABC相最近一次失压起始时刻
 Write_Storage_Data(0xB338,(INT8U *)Pub_Buf0,4),//全失压最近一次起始时刻

 Write_Storage_Data(0xB340,(INT8U *)Pub_Buf0,4),//最近一次失压结束时刻     
 Write_Storage_Data(0xB341,(INT8U *)Pub_Buf0,4),//A相最近一次失压结束时刻  
 Write_Storage_Data(0xB342,(INT8U *)Pub_Buf0,4),//B相最近一次失压结束时刻  
 Write_Storage_Data(0xB343,(INT8U *)Pub_Buf0,4),//C相最近一次失压结束时刻  
 Write_Storage_Data(0xB344,(INT8U *)Pub_Buf0,4),//AB相最近一次失压结束时刻 
 Write_Storage_Data(0xB345,(INT8U *)Pub_Buf0,4),//AC相最近一次失压结束时刻 
 Write_Storage_Data(0xB346,(INT8U *)Pub_Buf0,4),//BC相最近一次失压结束时刻 
 Write_Storage_Data(0xB347,(INT8U *)Pub_Buf0,4),//ABC相最近一次失压结束时刻
 Write_Storage_Data(0xB348,(INT8U *)Pub_Buf0,4),//全失压最近一次结束时刻
 
 Extend_Def_Event_MultiTimes_Data();
 OS_Sem_Post(PUB_BUF0_SEM_ID);
}

//清除分次事件,清事件记录时调用该函数
void Clear_Event_MultiTimes_Data()
{
  INT8U Err;
  INT16U i,j,Len;
  STORA_DI DI;
  
  DEBUG_PRINT(HUCK,DEBUG_0,"Set Def Event Data");
  
  OS_Sem_Pend(PUB_BUF0_SEM_ID);
  mem_set((void *)Pub_Buf0,0,sizeof(Pub_Buf0),(void *)Pub_Buf0,sizeof(Pub_Buf0));
  for(i=0;i<S_NUM(Event_Data_Info);i++)
  {
    if(Event_Data_Info[i].Clr_Flag == 0)//不允许清该记录
      continue;
    
    DI=0;
    Read_Storage_Data(Event_Data_Info[i].Last_Storage_DI,&DI,&DI,sizeof(DI),&Err);//读出最近一次的事件的存储DI
    if(NULL_EVENT_STORAGE_DI == DI)//该事件还没有生成过,不需要清除分次记录
      continue;
   
    DI = NULL_EVENT_STORAGE_DI;
    Write_Storage_Data(Event_Data_Info[i].Last_Storage_DI,(void *)&DI,sizeof(DI));
    
    //对每条分次记录清除
    for(j=0;j<Event_Data_Info[i].Storage_Num;j++)
    {
      DI=Event_Data_Info[i].Storage_DI+j;// & 0xFFFFFF00)+Hex2Bcd_Byte(j);
      Len=Get_Storage_Data_Len(DI);
      Write_Storage_Data(DI,(void *)Pub_Buf0,Len); 
    }
  }

 Write_Storage_Data(0xB330,(INT8U *)Pub_Buf0,4),//最近一次失压起始时刻     
 Write_Storage_Data(0xB331,(INT8U *)Pub_Buf0,4),//A相最近一次失压起始时刻  
 Write_Storage_Data(0xB332,(INT8U *)Pub_Buf0,4),//B相最近一次失压起始时刻  
 Write_Storage_Data(0xB333,(INT8U *)Pub_Buf0,4),//C相最近一次失压起始时刻  
 Write_Storage_Data(0xB334,(INT8U *)Pub_Buf0,4),//AB相最近一次失压起始时刻 
 Write_Storage_Data(0xB335,(INT8U *)Pub_Buf0,4),//AC相最近一次失压起始时刻 
 Write_Storage_Data(0xB336,(INT8U *)Pub_Buf0,4),//BC相最近一次失压起始时刻 
 Write_Storage_Data(0xB337,(INT8U *)Pub_Buf0,4),//ABC相最近一次失压起始时刻
 Write_Storage_Data(0xB338,(INT8U *)Pub_Buf0,4),//全失压最近一次起始时刻

 Write_Storage_Data(0xB340,(INT8U *)Pub_Buf0,4),//最近一次失压结束时刻     
 Write_Storage_Data(0xB341,(INT8U *)Pub_Buf0,4),//A相最近一次失压结束时刻  
 Write_Storage_Data(0xB342,(INT8U *)Pub_Buf0,4),//B相最近一次失压结束时刻  
 Write_Storage_Data(0xB343,(INT8U *)Pub_Buf0,4),//C相最近一次失压结束时刻  
 Write_Storage_Data(0xB344,(INT8U *)Pub_Buf0,4),//AB相最近一次失压结束时刻 
 Write_Storage_Data(0xB345,(INT8U *)Pub_Buf0,4),//AC相最近一次失压结束时刻 
 Write_Storage_Data(0xB346,(INT8U *)Pub_Buf0,4),//BC相最近一次失压结束时刻 
 Write_Storage_Data(0xB347,(INT8U *)Pub_Buf0,4),//ABC相最近一次失压结束时刻
 Write_Storage_Data(0xB348,(INT8U *)Pub_Buf0,4),//全失压最近一次结束时刻
 
 Extend_Def_Event_MultiTimes_Data();  
  OS_Sem_Post(PUB_BUF0_SEM_ID);  
}
#undef EVENT_MULTITIMES_C


