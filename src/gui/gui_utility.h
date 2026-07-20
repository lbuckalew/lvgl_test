#ifndef GUI_UTILITY_H
#define GUI_UTILITY_H

#include <lvgl.h>

struct gui_utility {
    const char *title;

    int (*create)(lv_obj_t *parent);
    void (*destroy)(void);
    int (*render)(void);
};

#endif