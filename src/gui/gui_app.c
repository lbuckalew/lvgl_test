#include "gui_theme.h"
#include "gui_app.h"
#include "../sensor_msgq.h"
#include "utilities/overview/overview_utility.h"
#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <zephyr/logging/log.h>
#include <lvgl.h>

LOG_MODULE_REGISTER(gui_app, CONFIG_LVGL_APP_LOG_LEVEL);

#define GUI_THREAD_STACK_SIZE 2048
#define GUI_THREAD_PRIORITY   5

K_THREAD_STACK_DEFINE(gui_thread_stack, GUI_THREAD_STACK_SIZE);
static struct k_thread gui_thread_data;
static k_tid_t gui_thread_id;

static const struct gui_utility *current_utility;

const struct device *display;
lv_obj_t *screen;
lv_obj_t *content;

static int _init_content(void)
{
    if (screen == NULL) {
        LOG_WRN("Screen pointer was NULL when trying to create container container.");
        return -EFAULT;
    }
    
    content = lv_obj_create(screen);
    lv_obj_set_size(content, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_text_color(content, GUI_COLOR(GUI_RGB_TEXT), LV_PART_MAIN);
    lv_obj_center(content);

    return 0;
}

static int _init_screen(void)
{
    screen = lv_scr_act();
    if (screen == NULL) {
        LOG_WRN("Screen pointer returned was NULL.");
        return -EFAULT;
    }

    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(screen, GUI_COLOR(GUI_RGB_BG), LV_PART_MAIN);

    return 0;
}

static int _init_display(void)
{
    display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (display == NULL) {
        LOG_WRN("Could not retrieve display device.");
        return -ENODEV;
    }

    if (!device_is_ready(display)) {
        LOG_WRN("Display device was not ready.");
        return -ENODEV;
    }

    return 0;
}

static int gui_app_load_utility(const struct gui_utility *utility)
{
    if (utility == NULL || utility->create == NULL) {
        return -EINVAL;
    }

    if (current_utility != NULL && current_utility->destroy != NULL) {
        current_utility->destroy();
    }

    lv_obj_clean(content);

    int rc = utility->create(content);
    if (rc != 0) {
        current_utility = NULL;
        return rc;
    }

    current_utility = utility;

    return 0;
}

static void gui_process_message(const struct gui_msg *msg)
{
    switch (msg->type) {
    case GUI_MSG_SET_RELATIVE_HUMIDITY:
        overview_set_rh(msg->data.meas);
        break;

    case GUI_MSG_SET_TEMPERATURE:
        overview_set_temp(msg->data.meas);
        break;

    case GUI_MSG_SET_PRESSURE:
        overview_set_press(msg->data.meas);
        break;

    case GUI_MSG_LOAD_UTILITY:
        LOG_ERR("Add ability to change utilities.");
        break;

    case GUI_MSG_BTN_1:
        LOG_INF("Button 1 triggered <gui next widget>.");
        overview_focus_next();
        break;

    default:
        LOG_WRN("Unknown GUI message type: %d", msg->type);
        break;
    }
}

static void gui_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    int rc = _init_display();
    if (rc != 0) {
        LOG_ERR("Failed to init display device.");
        return;
    }

    rc = _init_screen();
    if (rc != 0) {
        LOG_ERR("Failed to init screen controller.");
        return;
    }

    rc = _init_content();
    if (rc != 0) {
        LOG_ERR("Failed to initialize GUI.");
        return;
    }

    display_blanking_off(display);
    lv_task_handler();

    rc = gui_app_load_utility(&overview_utility);
    if (rc != 0) {
        LOG_ERR("Failed to load humidity utility: %d", rc);
        return;
    }

    struct gui_msg msg;
    for (;;) {
        if (k_msgq_get(&sensor_msgq, &msg, K_MSEC(10)) == 0) {
            gui_process_message(&msg);
        }

        current_utility->render();

        lv_task_handler();
    }
}

int gui_app_start(void)
{
    if (gui_thread_id != NULL) {
        return -EALREADY;
    }

    gui_thread_id = k_thread_create(
        &gui_thread_data,
        gui_thread_stack,
        K_THREAD_STACK_SIZEOF(gui_thread_stack),
        gui_thread,
        NULL,
        NULL,
        NULL,
        GUI_THREAD_PRIORITY,
        0,
        K_NO_WAIT
    );

    k_thread_name_set(gui_thread_id, "gui_app");

    return 0;
}