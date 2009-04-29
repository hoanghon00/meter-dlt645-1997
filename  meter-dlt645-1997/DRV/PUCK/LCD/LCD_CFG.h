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

#if LCD_PROPERTY==LCD_NORMAL_JS
  #define HTLCD_JS_C
  #include "APP_LCD_Pub_JS.h"
#endif


#endif
