#include "../../gui_theme.h"
#include "../../assets/gui_assets.h"
#include "../../gui_utility.h"
#include "components.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <lvgl.h>
#include <math.h>

LOG_MODULE_DECLARE(lvgl_app, CONFIG_LVGL_APP_LOG_LEVEL);

#define COMP_SIZE 64 // Component size in pixels

enum component_positions {
    COMP_POS_NEXT,
    COMP_POS_ACTIVE,
    COMP_POS_LAST,
};

struct lvgl_point {
    lv_coord_t x;
    lv_coord_t y;
};

struct next_comp_anim {
    struct mini_sensor_view *component;

    lv_coord_t start_x;
    lv_coord_t start_y;

    lv_coord_t end_x;
    lv_coord_t end_y;
};

const char *dummy_str = ":V";
static bool next_comp_anim_active = false;
static struct mini_sensor_view *const components[] = {
    &humidity_view,
    &temperature_view,
    &pressure_view,
};
static const struct lvgl_point component_locations[] = {
    [COMP_POS_ACTIVE] = {.x = -75,  .y = 0  },
    [COMP_POS_NEXT] =   {.x = 38,   .y = 65 },
    [COMP_POS_LAST] =   {.x = 38,   .y = -65},
};
static struct next_comp_anim comp_focus_anims[ARRAY_SIZE(components)];

static enum component_positions next_position(enum component_positions current_position)
{
    if (current_position == COMP_POS_LAST) {
        return COMP_POS_NEXT;
    }
    return ++current_position;

}

static struct mini_sensor_view* get_active_component()
{
    for (int i=0; i<ARRAY_SIZE(components); i++) {
        if (components[i]->location_id == COMP_POS_ACTIVE) {
            return components[i];
        }
    }

    return components[0];
}

static void next_comp_anim_exec(void *var, int32_t progress)
{
    struct next_comp_anim *move = var;
    lv_coord_t x = move->start_x + ((move->end_x - move->start_x) * progress) / 1000;
    lv_coord_t y = move->start_y + ((move->end_y - move->start_y) * progress) / 1000;
    lv_obj_align(move->component->lvo_root, LV_ALIGN_CENTER, x, y);
}

static void create_screen_glow(lv_obj_t *parent, lv_color_t accent_color)
{
    const lv_color_t bg_color = GUI_COLOR(GUI_RGB_BG);
    // const lv_color_t accent_color = GUI_COLOR(GUI_RGB_BLUE);
    const lv_color_t center_color = lv_color_mix(accent_color, bg_color, LV_OPA_30);

    lv_coord_t width = lv_obj_get_width(parent);
    lv_coord_t height = lv_obj_get_height(parent);
    lv_coord_t half_height = height / 4;
    lv_coord_t quarter_height = half_height / 2;

    lv_obj_t *top = lv_obj_create(parent);
    lv_obj_remove_style_all(top);
    lv_obj_set_size(top, width, half_height);
    lv_obj_align(top, LV_ALIGN_CENTER, 0, -quarter_height);

    lv_obj_set_style_bg_opa(top, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(top, bg_color, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(top, center_color, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(top, LV_GRAD_DIR_VER, LV_PART_MAIN);

    lv_obj_t *bottom = lv_obj_create(parent);
    lv_obj_remove_style_all(bottom);
    lv_obj_set_size(bottom, width, half_height);
    lv_obj_align(bottom, LV_ALIGN_CENTER, 0, quarter_height);

    lv_obj_set_style_bg_opa(bottom, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(bottom, center_color, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(bottom, bg_color, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(bottom, LV_GRAD_DIR_VER, LV_PART_MAIN);

    lv_obj_move_background(top);
    lv_obj_move_background(bottom);
}

static void next_comp_anim_done(lv_anim_t *anim)
{
    ARG_UNUSED(anim);
    next_comp_anim_active = false;


    lv_obj_t *p = lv_obj_get_parent(humidity_view.lvo_root);
    struct mini_sensor_view *active = get_active_component();
    // create_screen_glow(p, GUI_COLOR(active->color_main_hex));
}

static int comp_create(struct mini_sensor_view *component, lv_obj_t *lvo_parent)
{
    if ((component == NULL) || (lvo_parent == NULL)) {
        return -EINVAL;
    }

    // Root
    lv_obj_t *_root = lv_obj_create(lvo_parent);
    component->lvo_root = _root;
    lv_obj_remove_style_all(_root);
    lv_obj_set_size(_root, COMP_SIZE, COMP_SIZE);
    lv_obj_clear_flag(_root, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(
        _root,
        LV_ALIGN_CENTER,
        component_locations[component->location_id].x,
        component_locations[component->location_id].y
    );

    // Meter
    lv_obj_t * _meter = lv_arc_create(_root);
    component->lvo_data_meter = _meter;
    lv_obj_set_size(_meter, 64, 64);
    lv_obj_clear_flag(_meter, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_arc_rounded(_meter, true, LV_PART_INDICATOR);
    lv_arc_set_range(_meter, component->data_scale_min, component->data_scale_max);
    lv_arc_set_bg_angles(_meter, 145, 35);
    lv_obj_remove_style(_meter, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_width(_meter, 5, LV_PART_MAIN);
    lv_obj_set_style_arc_width( _meter, 5, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(_meter, GUI_COLOR(GUI_RGB_SURFACE_RAISED), LV_PART_MAIN);
    lv_obj_set_style_arc_color(_meter, GUI_COLOR(component->color_accent_hex), LV_PART_INDICATOR);
    lv_obj_center(_meter);

    // Data Value label
    lv_obj_t * _value_label = lv_label_create(_root);
    component->lvo_data_value = _value_label;
    lv_obj_align(_value_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(_value_label, GUI_FONT_M, LV_PART_MAIN);
    lv_obj_set_style_text_color(_value_label, GUI_COLOR(component->color_main_hex), LV_PART_MAIN);
    lv_label_set_text(_value_label, "0");

    // Data units label
    lv_obj_t * _units_label = lv_label_create(_meter);
    lv_obj_set_style_text_font(_units_label, GUI_FONT_S, LV_PART_MAIN);
    lv_obj_set_style_text_color(_units_label, GUI_COLOR(GUI_RGB_TEXT_MUTED), LV_PART_MAIN);
    if (component->data_units_str == NULL) {
        LOG_WRN("Tried to build component, but data units string was not set yet.");
        component->data_units_str = dummy_str;
    }
    lv_label_set_text(_units_label, component->data_units_str);
    lv_obj_align(_units_label, LV_ALIGN_BOTTOM_MID, 0, 0);

    return 0;
}

static void comp_render(struct mini_sensor_view *component)
{
    if ((component == NULL) || component->is_rendered) {
        return;
    }

    component->data_repr_int = CLAMP(
        (int)lround(component->data_src),
        component->data_scale_min,
        component->data_scale_max
    );
    snprintf(
        component->data_repr_str,
        sizeof(component->data_repr_str),
        "%2d",
        component->data_repr_int
    );
    lv_label_set_text(component->lvo_data_value, component->data_repr_str);

    lv_arc_set_value(component->lvo_data_meter, component->data_repr_int);

    component->is_rendered = true;
}

static void comp_destroy(struct mini_sensor_view *component)
{
    component->lvo_root = NULL;
    component->lvo_data_meter = NULL;
    component->lvo_data_value = NULL;
}

static int util_create(lv_obj_t *parent)
{
    if (parent == NULL) {
        return -EINVAL;
    }

    for (size_t i=0; i<ARRAY_SIZE(components); i++) {
        components[i]->location_id = i;
        comp_create(components[i], parent);
    }

    struct mini_sensor_view *active = get_active_component();
    create_screen_glow(parent, GUI_COLOR(active->color_main_hex));

    return 0;
}

static int util_render(void)
{
    for (size_t i=0; i<ARRAY_SIZE(components); i++) {
        comp_render(components[i]);
    }
    
    return 0;
}

static void util_destroy(void)
{
    for (size_t i=0; i<ARRAY_SIZE(components); i++) {
        comp_destroy(components[i]);
    }
}

void overview_focus_next(void)
{
    if (next_comp_anim_active) {
        LOG_WRN("Rotation blocked - animation already in progress.");
        return;
    }
    next_comp_anim_active = true;

    int num_components = ARRAY_SIZE(components);
    for (size_t i = 0; i < num_components; i++) {
        struct mini_sensor_view *component = components[i];

        int old_location = component->location_id;
        int new_location = next_position(old_location);

        struct next_comp_anim *move = &comp_focus_anims[i];

        move->component = component;
        move->start_x = component_locations[old_location].x;
        move->start_y = component_locations[old_location].y;
        move->end_x = component_locations[new_location].x;
        move->end_y = component_locations[new_location].y;

        lv_anim_t anim;
        lv_anim_init(&anim);

        lv_anim_set_var(&anim, move);
        lv_anim_set_exec_cb(&anim, next_comp_anim_exec);

        lv_anim_set_values(&anim, 0, 1000);
        lv_anim_set_time(&anim, 400);

        lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out);

        if (i == ARRAY_SIZE(components) - 1) {
            lv_anim_set_ready_cb(&anim, next_comp_anim_done);
        }

        lv_anim_start(&anim);

        component->location_id = new_location;
    }
}

void overview_set_rh(double rh)
{
    humidity_view.data_src = rh;
    humidity_view.is_rendered = false;
}

void overview_set_press(double press)
{
    pressure_view.data_src = press;
    pressure_view.is_rendered = false;
}

void overview_set_temp(double temp)
{
    temperature_view.data_src = temp;
    temperature_view.is_rendered = false;
}

const struct gui_utility overview_utility = {
    .title = "Overview",
    .create = util_create,
    .render = util_render,
    .destroy = util_destroy,
};