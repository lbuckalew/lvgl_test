#include "sensor_msgq.h"
#include "env_sensor/env_sensor.h"
#include "gui/gui_app.h"

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(lvgl_app, CONFIG_LVGL_APP_LOG_LEVEL);

#define SW0_NODE DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS_OKAY(SW0_NODE)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif

static const struct gpio_dt_spec btn1 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback btn1_cb_data;

static void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    if (cb == &btn1_cb_data) {
        gui_app_trigger_btn1();
    } else {
        LOG_WRN("Button pressed that doesn't own a callback.");
    }
}

static int setup_input()
{
    if (!gpio_is_ready_dt(&btn1)) {
		LOG_WRN("Error: button device %s is not ready", btn1.port->name);
		return -ENODEV;
	}

	int rc = gpio_pin_configure_dt(&btn1, GPIO_INPUT);
	if (rc != 0) {
		LOG_WRN("Error %d: failed to configure %s pin %d",
            rc,
            btn1.port->name,
            btn1.pin
        );
		return rc;
	}

	rc = gpio_pin_interrupt_configure_dt(&btn1, GPIO_INT_EDGE_TO_ACTIVE);
	if (rc != 0) {
		LOG_WRN("Error %d: failed to configure interrupt on %s pin %d",
            rc,
            btn1.port->name,
            btn1.pin
        );
		return rc;
	}

	gpio_init_callback(&btn1_cb_data, button_pressed, BIT(btn1.pin));
	gpio_add_callback(btn1.port, &btn1_cb_data);
	LOG_INF("Set up devkit Button 1.");

    return 0;
}

int main(void)
{
    int rc = env_sensor_init();

    rc = gui_app_start();

    rc = setup_input();
    if (rc != 0) {
        LOG_ERR("Failed to configure input.");
        return -1;
    }

    for (;;) {
        rc =  env_sensor_fetch();
        k_msleep(1000);
    }
    
    LOG_WRN("Main thread is terminating");
	return 0;
}