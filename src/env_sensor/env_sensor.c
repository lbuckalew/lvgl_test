#include "../sensor_msgq.h"
#include "env_sensor.h"

#include <zephyr/kernel.h>
// #include <zephyr/device.h>
// #include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor_data_types.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/logging/log.h>
#include <math.h>

LOG_MODULE_REGISTER(env_sensor, CONFIG_LVGL_APP_LOG_LEVEL);

const struct device *const env_dev = DEVICE_DT_GET_ANY(bosch_bme280);
const struct sensor_decoder_api *decoder;
uint8_t sensor_buf[128];

RTIO_DEFINE(ctx, 1, 1);
SENSOR_DT_READ_IODEV(
    iodev,
    DT_COMPAT_GET_ANY_STATUS_OKAY(bosch_bme280),
    {SENSOR_CHAN_AMBIENT_TEMP, 0},
    {SENSOR_CHAN_HUMIDITY, 0},
    {SENSOR_CHAN_PRESS, 0}
);

static int sensor_init(void)
{
    if (env_dev == NULL) {
        LOG_ERR("Could not fetch environment sensor.");
        return -ENODEV;
    }

    if (!device_is_ready(env_dev)) {
        LOG_ERR("Environment sensor device was not ready.");
        return -EBUSY;
    }

    return 0;
}

static double sensor_q31_to_double(const struct sensor_q31_data *data)
{
    return ldexp((double)data->readings[0].value, data->shift - 31);
}

int env_sensor_fetch()
{
    int rc = sensor_read(&iodev, &ctx, sensor_buf, 128);
    if (rc != 0) {
        LOG_WRN("%s: sensor_read() failed: %d", env_dev->name, rc);
        return rc;
    }

    uint32_t temp_fit = 0;
    struct sensor_q31_data temp_data = {0};
    decoder->decode(
        sensor_buf,
        (struct sensor_chan_spec) {SENSOR_CHAN_AMBIENT_TEMP, 0},
        &temp_fit,
        1,
        &temp_data
    );

    uint32_t press_fit = 0;
    struct sensor_q31_data press_data = {0};
    decoder->decode(sensor_buf,
        (struct sensor_chan_spec) {SENSOR_CHAN_PRESS, 0},
        &press_fit,
        1,
        &press_data
    );

    uint32_t hum_fit = 0;
    struct sensor_q31_data hum_data = {0};
    decoder->decode(sensor_buf,
        (struct sensor_chan_spec) {SENSOR_CHAN_HUMIDITY, 0},
        &hum_fit,
        1,
        &hum_data
    );

    gui_app_set_relative_humidity(sensor_q31_to_double(&hum_data));
    gui_app_set_temperature(sensor_q31_to_double(&temp_data));
    gui_app_set_pressure(sensor_q31_to_double(&press_data));

    return 0;
}

int env_sensor_init()
{
    LOG_INF("Initializing environment sensor...");

    int rc = sensor_init();
    if (rc != 0) {
        LOG_WRN("Could not get sensor device.");
        goto fail;
    }
    
    rc = sensor_get_decoder(env_dev, &decoder);
    if (rc != 0) {
        LOG_WRN("%s: sensor_get_decode() failed: %d", env_dev->name, rc);
        goto fail;
    }

    LOG_INF("Done initializing.");
    
    return 0;

fail:
    LOG_ERR("Failed initialization.");
    return rc;
}