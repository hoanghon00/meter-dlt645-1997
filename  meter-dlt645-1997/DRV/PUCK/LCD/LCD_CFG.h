#ifndef LCD_CFG_h
#define LCD_CFG_h

#ifndef LCD_CFG_C
#define LCD_CFG_EXT extern
#else
#define LCD_CFG_EXT
#endif


#if LCD_PROPERTY==LCD_NORMAL
  #include "APP_LCD_Pub.h"
#endif

#if LCD_PROPERTY==LCD_JS_ZXW
  #define HTLCD_JS_ZXW_C
  #include "APP_LCD_Pub_JS_ZXW.h"
#endif

#if LCD_PROPERTY==LCD_JS_JX
  #define HTLCD_JS_JX_C
  #include "APP_LCD_Pub_JS_JX.h"
#endif

#endif
