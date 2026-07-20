#include "../gui_theme.h"
#include "../assets/gui_assets.h"
#include "humidity_utility.h"

#include <zephyr/kernel.h>
#include <lvgl.h>

#define METER_PADDING 40

lv_obj_t *rh_meter;
lv_obj_t *rh_drop_icon;
lv_obj_t *rh_val_display;
lv_obj_t *rh_val_num;
lv_obj_t *rh_val_unit;

double rh_measurement = 0.0;
char rh_string[25];

static int double_to_int(double d)
{
    return (int)(d + 0.5);
}

void humidity_update(double measurement)
{
    if (measurement > 100.0) {
        measurement = 100.0;
    } else if (measurement < 0.0) {
        measurement = 0.0;
    }
    rh_measurement = measurement;
    snprintf(rh_string, sizeof(rh_string), "%.1f", rh_measurement);

    lv_label_set_text(rh_val_num, rh_string);
    lv_arc_set_value(rh_meter, double_to_int(rh_measurement));
}

static int humidity_create(lv_obj_t *parent)
{
    if (parent == NULL) {
        return -EINVAL;
    }

    rh_meter = lv_arc_create(parent);
    lv_arc_set_range(rh_meter, 0, 100);
    lv_arc_set_bg_angles(rh_meter, 135, 45);
    lv_obj_remove_style(rh_meter, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(rh_meter, LV_OBJ_FLAG_CLICKABLE);

    lv_coord_t rh_meter_size = LV_MIN(
        lv_obj_get_content_width(parent),
        lv_obj_get_content_height(parent)
    ) - METER_PADDING;
    lv_obj_set_size(rh_meter, rh_meter_size, rh_meter_size);
    lv_obj_center(rh_meter);

    // Meter scale
    lv_obj_set_style_arc_width(rh_meter, 6, LV_PART_MAIN);
    lv_obj_set_style_arc_color(rh_meter, lv_color_hex(0x454545), LV_PART_MAIN);

    // Meter indicator
    lv_obj_set_style_arc_width(rh_meter, 10, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(rh_meter, lv_color_hex(0x48BFE3), LV_PART_INDICATOR);

    // Icon
    rh_drop_icon = lv_img_create(rh_meter);
    lv_img_set_src(rh_drop_icon, &img_water_drop);
    lv_obj_align(rh_drop_icon, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_img_recolor(rh_drop_icon, lv_color_hex(0x164A70), LV_PART_MAIN);
    lv_obj_set_style_img_recolor_opa(rh_drop_icon, LV_OPA_COVER, LV_PART_MAIN);

    // Value display
    rh_val_display = lv_obj_create(rh_meter);
    lv_obj_remove_style_all(rh_val_display);
    lv_obj_set_size(rh_val_display, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(rh_val_display, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(
        rh_val_display,
        LV_FLEX_ALIGN_CENTER,  /* Horizontal arrangement */
        LV_FLEX_ALIGN_END,     /* Align bottoms of the labels */
        LV_FLEX_ALIGN_CENTER
    );
    lv_obj_set_style_pad_column(rh_val_display, 3, LV_PART_MAIN);
    lv_obj_align(rh_val_display, LV_ALIGN_BOTTOM_RIGHT, -16, -24);

    rh_val_num = lv_label_create(rh_val_display);
    lv_obj_set_style_text_font(rh_val_num, GUI_FONT_L, LV_PART_MAIN);

    rh_val_unit = lv_label_create(rh_val_display);
    lv_obj_set_style_text_font(rh_val_unit, GUI_FONT_M, LV_PART_MAIN);
    lv_obj_set_style_translate_y(rh_val_unit, -4, LV_PART_MAIN);
    // lv_obj_set_style_translate_y(rh_val_unit, -4, LV_PART_MAIN);
    lv_label_set_text(rh_val_unit, " %");

    return 0;
}

static void humidity_destroy(void)
{
    rh_meter = NULL;
    rh_val_display = NULL;
    rh_val_num = NULL;
    rh_val_unit = NULL;
    rh_drop_icon = NULL;
}

const struct gui_utility humidity_utility = {
    .title = "Relative Humidity",
    .create = humidity_create,
    .destroy = humidity_destroy,
};