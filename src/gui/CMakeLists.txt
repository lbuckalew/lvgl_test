add_subdirectory(assets)
add_subdirectory(utilities)

target_include_directories(app PRIVATE .)

target_sources(app PRIVATE
    gui_app.c
)