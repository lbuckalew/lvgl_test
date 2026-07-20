#include "../gui_theme.h"
#include "../assets/gui_assets.h"
#include "../gui_utility.h"
#include <zephyr/kernel.h>
#include <lvgl.h>

struct overview_branch {
    double meas;
    char meas_val_str[8];
    const char *meas_unit_str;
    lv_obj_t *lv_container;
    lv_obj_t *lv_icon;
    lv_obj_t *lv_val;
    lv_obj_t *lv_unit;
    int x_offs;
    int y_offs;
    void (*style_overlay)(void);
};

static struct overview_branch rh_branch = {
    .meas = (double)0.0,
    .meas_val_str = "0.0",
    .meas_unit_str = " %rh",
    .x_offs = 0,
    .y_offs = -80,
};

static struct overview_branch temp_branch = {
    .meas = (double)0.0,
    .meas_val_str = "0.0",
    .meas_unit_str = " °C",
    .x_offs = 69,
    .y_offs = 40,
};

static struct overview_branch press_branch = {
    .meas = (double)0.0,
    .meas_val_str = "0.0",
    .meas_unit_str = " Bar",
    .x_offs = -69,
    .y_offs = 40,
};

static int branch_create(lv_obj_t *parent, struct overview_branch *branch)
{
    branch->lv_container = lv_obj_create(parent);
    branch->lv_icon = lv_img_create(branch->lv_container);
    branch->lv_val = lv_label_create(branch->lv_container);
    branch->lv_unit = lv_label_create(branch->lv_container);

    // Container
    lv_obj_remove_style_all(branch->lv_container);
    lv_obj_set_size(branch->lv_container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(branch->lv_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(
        branch->lv_container,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER
    );
    // lv_obj_set_style_pad_column(branch->lv_container, 3, LV_PART_MAIN);
    lv_obj_align(branch->lv_container, LV_ALIGN_CENTER, branch->x_offs, branch->y_offs);

    // Icon
    lv_img_set_src(branch->lv_icon, &img_temperature_small);
    lv_obj_align(branch->lv_icon, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_img_recolor(branch->lv_icon, GUI_COLOR_BLUE_BRIGHT, LV_PART_MAIN);
    lv_obj_set_style_img_recolor_opa(branch->lv_icon, LV_OPA_COVER, LV_PART_MAIN);

    // Value
    lv_obj_set_style_text_font(branch->lv_val, GUI_FONT_M, LV_PART_MAIN);
    lv_label_set_text(branch->lv_val, branch->meas_val_str);

    // Unit
    lv_obj_set_style_text_font(branch->lv_unit, GUI_FONT_S, LV_PART_MAIN);
    lv_label_set_text(branch->lv_unit, branch->meas_unit_str);

    return 0;
}

static void branch_render(struct overview_branch *branch)
{
    snprintf(branch->meas_val_str, sizeof(branch->meas_val_str), "%.1f", branch->meas);
    lv_label_set_text(branch->lv_val, branch->meas_val_str);
}

static void branch_destroy(struct overview_branch *branch)
{
    branch->lv_container = NULL;
    branch->lv_icon = NULL;
    branch->lv_val = NULL;
    branch->lv_unit = NULL;
}

static int util_create(lv_obj_t *parent)
{
    if (parent == NULL) {
        return -EINVAL;
    }

    int rc = branch_create(parent, &rh_branch);
    rc = branch_create(parent, &temp_branch);
    rc = branch_create(parent, &press_branch);

    return 0;
}

static void util_destroy(void)
{
    branch_destroy(&rh_branch);
    branch_destroy(&temp_branch);
    branch_destroy(&press_branch);

    return;
}

static int util_render(void)
{
    bool has_changed = true;
    if (!has_changed) {
        return 0;
    }

    branch_render(&rh_branch);
    branch_render(&temp_branch);
    branch_render(&press_branch);

    return 0;
}

void overview_set_rh(double rh)
{
    rh_branch.meas = rh;
}

void overview_set_press(double press)
{
    press_branch.meas = press;
}

void overview_set_temp(double temp)
{
    temp_branch.meas = temp;
}

const struct gui_utility overview_utility = {
    .title = "Environment Overview",
    .create = util_create,
    .render = util_render,
    .destroy = util_destroy,
};