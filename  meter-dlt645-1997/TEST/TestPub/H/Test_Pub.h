#ifndef TEST_PUB_H
#define TEST_PUB_H

#ifdef TEST_MODE

#include "Include_PUCK.h"

#undef EXT
#ifdef TEST_PUB_C
#define EXT
#else
#define EXT extern
#endif

#define Test_Buf0 _Test_Buf0.Buf
#define Test_Buf1 _Test_Buf1.Buf
#define Test_Buf2 _Test_Buf2.Buf
#define Test_Buf3 _Test_Buf3.Buf

DECLARE_BUF_TYPE(INT16U,100,S_Test_Buf);

EXT S_Test_Buf _Test_Buf0;
EXT S_Test_Buf _Test_Buf1;
EXT S_Test_Buf _Test_Buf2;
EXT S_Test_Buf _Test_Buf3;

#define TEST_FUNC(Func) Test_Func(Func,#Func)
EXT void Test_Unit();
EXT void Test_Proc(INT8U Flag);
#endif
#endif
