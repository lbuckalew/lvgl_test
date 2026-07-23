#include "sensor_msgq.h"
#include <zephyr/kernel.h>

K_MSGQ_DEFINE(sensor_msgq, sizeof(struct gui_msg), SENSOR_MSGQ_DEPTH, 4);

int gui_app_trigger_btn1(void)
{
    const struct gui_msg msg = {.type = GUI_MSG_BTN_1,};
    return k_msgq_put(&sensor_msgq, &msg, K_NO_WAIT);
}

int gui_app_set_relative_humidity(double relative_humidity)
{
    const struct gui_msg msg = {
        .type = GUI_MSG_SET_RELATIVE_HUMIDITY,
        .data.meas = relative_humidity,
    };

    return k_msgq_put(&sensor_msgq, &msg, K_NO_WAIT);
}

int gui_app_set_temperature(double temperature)
{
    const struct gui_msg msg = {
        .type = GUI_MSG_SET_TEMPERATURE,
        .data.meas = temperature,
    };

    return k_msgq_put(&sensor_msgq, &msg, K_NO_WAIT);
}

int gui_app_set_pressure(double pressure)
{
    const struct gui_msg msg = {
        .type = GUI_MSG_SET_PRESSURE,
        .data.meas = pressure,
    };

    return k_msgq_put(&sensor_msgq, &msg, K_NO_WAIT);
}