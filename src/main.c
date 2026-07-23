#include "sensor_msgq.h"
#include "env_sensor/env_sensor.h"
#include "gui/gui_app.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <math.h>

LOG_MODULE_REGISTER(lvgl_app, CONFIG_LVGL_APP_LOG_LEVEL);

// #include <zephyr/dsp/print_format.h>

int main(void)
{
    int rc = env_sensor_init();
    rc = gui_app_start();

    int counter = 0;
    for (;;) {
        
        rc =  env_sensor_fetch();
        counter++;
        if (counter == 20) {
            gui_app_trigger_btn1();
            counter = 0;
        }

        k_msleep(1000);
    }
    
    LOG_WRN("Main thread is terminating");
	return 0;
}