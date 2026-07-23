#ifndef GUI_THEME_H
#define GUI_THEME_H

#include <lvgl.h>

/* Fonts */
#define GUI_FONT_S &lv_font_montserrat_16
#define GUI_FONT_M &lv_font_montserrat_24
#define GUI_FONT_L &lv_font_montserrat_48

/* Raw RGB values */
#define GUI_RGB_BG              0x101418
#define GUI_RGB_SURFACE         0x181E24
#define GUI_RGB_SURFACE_ALT     0x202830
#define GUI_RGB_SURFACE_RAISED  0x29333D

#define GUI_RGB_TEXT            0xF2F6F8
#define GUI_RGB_TEXT_MUTED      0xA8B4BC
#define GUI_RGB_TEXT_DIM        0x71808A

#define GUI_RGB_BLUE_DARK        0x123B55
#define GUI_RGB_BLUE             0x1677A8
#define GUI_RGB_BLUE_LIGHT       0x43B9E6
#define GUI_RGB_BLUE_BRIGHT      0x79D7F2

#define GUI_RGB_GREEN_DARK       0x184B2D
#define GUI_RGB_GREEN            0x23995A
#define GUI_RGB_GREEN_LIGHT      0x53D98C
#define GUI_RGB_GREEN_BRIGHT     0x8EF2BE

#define GUI_RGB_ORANGE_DARK      0x6A3B12
#define GUI_RGB_ORANGE           0xD96C17
#define GUI_RGB_ORANGE_LIGHT     0xF5A03A
#define GUI_RGB_ORANGE_BRIGHT    0xFFD07A

#define GUI_RGB_TEAL            0x28B8A7
#define GUI_RGB_PURPLE          0x8D75D6
#define GUI_RGB_RED             0xD95C5C
#define GUI_RGB_YELLOW          0xD6B84C

/* Runtime LVGL color conversion */
#define GUI_COLOR(rgb) lv_color_hex(rgb)

#endif