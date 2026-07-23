#ifndef ENV_SENSOR_H
#define ENV_SENSOR_H

// struct env_sensor_data {
//     double humidity;
//     double temperature;
//     double pressure;
// };

int env_sensor_init();
int env_sensor_fetch();

#endif