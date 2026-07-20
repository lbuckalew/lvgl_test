#ifndef GUI_THEME_H
#define GUI_THEME_H

#include <lvgl.h>

/* Fonts */
#define GUI_FONT_S &lv_font_montserrat_16
#define GUI_FONT_M &lv_font_montserrat_24
#define GUI_FONT_L &lv_font_montserrat_48

/* Base surfaces */
#define GUI_COLOR_BG              lv_color_hex(0x101418)
#define GUI_COLOR_SURFACE         lv_color_hex(0x181E24)
#define GUI_COLOR_SURFACE_ALT     lv_color_hex(0x202830)
#define GUI_COLOR_SURFACE_RAISED  lv_color_hex(0x29333D)

/* Borders and subdued UI elements */
#define GUI_COLOR_BORDER          lv_color_hex(0x35424D)
#define GUI_COLOR_DIVIDER         lv_color_hex(0x28333C)
#define GUI_COLOR_DISABLED        lv_color_hex(0x56616A)

/* Text */
#define GUI_COLOR_TEXT            lv_color_hex(0xF2F6F8)
#define GUI_COLOR_TEXT_MUTED      lv_color_hex(0xA8B4BC)
#define GUI_COLOR_TEXT_DIM        lv_color_hex(0x71808A)

/* Blue accent family */
#define GUI_COLOR_BLUE_DARK       lv_color_hex(0x123B55)
#define GUI_COLOR_BLUE            lv_color_hex(0x1677A8)
#define GUI_COLOR_BLUE_LIGHT      lv_color_hex(0x43B9E6)
#define GUI_COLOR_BLUE_BRIGHT     lv_color_hex(0x79D7F2)

/* Warm highlight family */
#define GUI_COLOR_ORANGE_DARK     lv_color_hex(0x8A4218)
#define GUI_COLOR_ORANGE          lv_color_hex(0xD46B24)
#define GUI_COLOR_ORANGE_LIGHT    lv_color_hex(0xF49A4A)

/* Additional accents */
#define GUI_COLOR_TEAL            lv_color_hex(0x28B8A7)
#define GUI_COLOR_GREEN           lv_color_hex(0x55B979)
#define GUI_COLOR_PURPLE          lv_color_hex(0x8D75D6)
#define GUI_COLOR_RED             lv_color_hex(0xD95C5C)
#define GUI_COLOR_YELLOW          lv_color_hex(0xD6B84C)

/* Semantic colors */
#define GUI_COLOR_SUCCESS         GUI_COLOR_GREEN
#define GUI_COLOR_WARNING         GUI_COLOR_ORANGE
#define GUI_COLOR_ERROR           GUI_COLOR_RED
#define GUI_COLOR_INFO            GUI_COLOR_BLUE_LIGHT

#endif