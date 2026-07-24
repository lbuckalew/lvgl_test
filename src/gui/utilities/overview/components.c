#include "../../gui_theme.h"
#include "components.h"

struct mini_sensor_view humidity_view = {
    .data_scale_min = 0,
    .data_scale_max = 100,
    .data_units_str = "%rh",
    .color_main_hex = GUI_RGB_BLUE_LIGHT,
    .color_accent_hex = GUI_RGB_BLUE,
};

struct mini_sensor_view temperature_view = {
    .data_scale_min = 0,
    .data_scale_max = 100,
    .data_units_str = "°C",
    .color_main_hex = GUI_RGB_ORANGE_BRIGHT,
    .color_accent_hex = GUI_RGB_ORANGE,
};

struct mini_sensor_view pressure_view = {
    .data_scale_min = 90,
    .data_scale_max = 110,
    .data_units_str = "kPa",
    .color_main_hex = GUI_RGB_GREEN_BRIGHT,
    .color_accent_hex = GUI_RGB_GREEN,
};