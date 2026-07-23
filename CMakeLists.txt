cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(lvgl_test)

add_subdirectory(src/gui)
add_subdirectory(src/env_sensor)

target_sources(app PRIVATE
    src/sensor_msgq.c
    src/main.c
)