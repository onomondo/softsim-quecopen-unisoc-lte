# Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.

configure_file(include/drv_config.h.in ${out_inc_dir}/drv_config.h)

if(NOT ${CONFIG_KEYPAD_CSV_PATH} STREQUAL "")
    set(keypadcfg_h ${CMAKE_CURRENT_BINARY_DIR}/drv_keypad_def.h)
    set(csv_keypad ${SOURCE_TOP_DIR}/${CONFIG_KEYPAD_CSV_PATH})
    add_custom_command(OUTPUT ${keypadcfg_h}
        COMMAND python3 ${keypadgen_py} ${csv_keypad} ${keypadcfg_h}
        DEPENDS ${keypadgen_py} ${csv_keypad}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

set(target driver_lcd)
add_app_libraries($<TARGET_FILE:${target}>)
add_library(${target} STATIC)

set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
target_include_directories(${target} PUBLIC include)
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_targets(${target} PUBLIC driver hal fs nvm) 
target_link_libraries(${target} PRIVATE hal)
target_sources(${target} PRIVATE 
	src/drv_lcd_cfg.c
    src/lcd/drv_lcd_gc9305.c
    src/lcd/drv_lcd_st7735s.c
    src/lcd/drv_lcd_st7567a.c
	src/lcd/drv_lcd_st7796s.c
	src/rda_fb.c
	src/lcd/st7701s_2lane_mipi.c
)

set(target driver_camera)
add_app_libraries($<TARGET_FILE:${target}>)
add_library(${target} STATIC)

set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
target_include_directories(${target} PUBLIC include)
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_targets(${target} PUBLIC driver hal fs nvm) 
target_link_libraries(${target} PRIVATE hal)
target_sources(${target} PRIVATE 
	src/image_sensor/image_sensor.c
    src/image_sensor/camera_gc032a.c
    src/image_sensor/camera_gc0310.c
    src/image_sensor/camera_bf20a6.c
	src/image_sensor/camera_bf3095.c
)

set(target drv_host)
add_app_libraries($<TARGET_FILE:${target}>)

add_library(${target} STATIC ${dummy_c_file})
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_targets(${target} PUBLIC calclib apploader)
target_link_libraries(${target} PRIVATE hal) 

target_sources(${target} PRIVATE
	src/hostcmd/drv_host_cmd_sys.c
)

set(target driver_charge_adc)
add_app_libraries($<TARGET_FILE:${target}>)

add_library(${target} STATIC )
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_DRIVER)
target_compile_options(${target} PRIVATE "-Wnull-dereference")
target_include_directories(${target} PUBLIC include src)#quec update
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR} src)
target_include_targets(${target} PRIVATE hal kernel fs fsmount diag nvm)
target_link_libraries(${target} PRIVATE hal)#quec update
target_sources(${target} PRIVATE
	src/drv_charger.c
	src/drv_adc.c
)

set(target driver)
include(core.cmake)
