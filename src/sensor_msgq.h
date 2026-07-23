#ifndef SENSOR_MSGQ_H
#define SENSOR_MSGQ_H

#define SENSOR_MSGQ_DEPTH 8

enum gui_msg_type {
    GUI_MSG_SET_RELATIVE_HUMIDITY,
    GUI_MSG_SET_TEMPERATURE,
    GUI_MSG_SET_PRESSURE,
    GUI_MSG_LOAD_UTILITY,
    GUI_MSG_BTN_1,
};

struct gui_msg {
    enum gui_msg_type type;

    union {
        double meas;
        int utility_id;
    } data;
};

extern struct k_msgq sensor_msgq;
int gui_app_trigger_btn1(void);
int gui_app_set_relative_humidity(double relative_humidity);
int gui_app_set_temperature(double temperature);
int gui_app_set_pressure(double pressure);

#endif