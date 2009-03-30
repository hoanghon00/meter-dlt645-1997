
/**
 * @file support.h
 *     { comment block }
 * @brief
 *     { brief description @ref support.cpp }
 * @author li.yao (nardoo@sohu.com)
 * @version v1.00
 * @date 05/29/2008 08:40
 * @note
 *     { paragraph where a note can be entered. }
 */

#ifndef __SUPPORT__
#define __SUPPORT__


// 按位置显示内容。
void lcdshow (u8 type, offs_t offs);
// type: 显示方案(modeA, modeB, modeC)
// offs: 显示偏移位置(0 ~ 根据配置参数定)
// return: true(正常显示) : false(参数出错)

// 按编码显示内容。
void display (u8 type, code_t code);
// type: 显示方案(modeA, modeB, modeC)
// code: 显示的代码(根据配置参数定)
// return: true(正常显示) : false(参数出错)

// 屏幕显示函数(定时调用产生闪烁)
void screen (u8 type, curs_t curs);
// type: 显示方案(modeA, modeB, modeC)
// curs: 光标所在位置，-1(不显示光标)

curs_t maxcurs (); 

#endif
