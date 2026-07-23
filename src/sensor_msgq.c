#include "sensor_msgq.h"
#include <zephyr/kernel.h>

K_MSGQ_DEFINE(sensor_msgq, sizeof(struct gui_msg), SENSOR_MSGQ_DEPTH, 4);