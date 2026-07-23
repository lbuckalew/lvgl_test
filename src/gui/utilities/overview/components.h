#ifndef OVERVIEW_COMPONENTS_H
#define OVERVIEW_COMPONENTS_H

#include <lvgl.h>

struct mini_sensor_view {
    // Component state
    bool is_rendered;
    int location_id;

    // Source data and representations
    double data_src;
    int data_repr_int;
    int data_scale_min;
    int data_scale_max;
    char data_repr_str[8];
    const char *data_units_str;

    // Config
    int color_main_hex;
    int color_accent_hex;

    // LVGL objects
    lv_obj_t *lvo_root;
    lv_obj_t *lvo_data_meter;
    lv_obj_t *lvo_data_value;
    lv_obj_t *lvo_data_unit;
};

extern struct mini_sensor_view humidity_view;
extern struct mini_sensor_view temperature_view;
extern struct mini_sensor_view pressure_view;

#endif