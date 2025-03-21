# Copyright (C) 2020 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#

set(target ql_app_wifi)

add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_app_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC)
target_include_directories(${target} PUBLIC inc)

include(./inc/wifi_demo_macro.cmake)

configure_file (./inc/wifi_demo_macro.h.in ${out_inc_dir}/wifi_demo_macro.h)

set(SRC_FILES)
#######################################
if(QL_WIFI_FC41D)
# quectel driver
target_include_directories(${target} PUBLIC fc41d/inc)
list(APPEND SRC_FILES "fc41d/src/*.c;fc41d/src/*.C" )
file(GLOB SRC_FILES ${SRC_FILES})
endif()

if(QL_WIFI_FGM842D)
# quectel driver
target_include_directories(${target} PUBLIC fgm842d/inc)
list(APPEND SRC_FILES "fgm842d/src/*.c;fgm842d/src/*.C" )
file(GLOB SRC_FILES ${SRC_FILES})
endif()

if(QL_WIFI_FCM360W)
# quectel driver
target_include_directories(${target} PUBLIC 
	fcm360w/inc
	fcm360w/src/drv_espiwifi
	fcm360w/src/drv_espiwifi/platform/include
	fcm360w/src/drv_espiwifi/driver/include
)

target_sources(${target} PRIVATE
	fcm360w/src/drv_espiwifi/ql_wifi_adapter.c 
	fcm360w/src/drv_espiwifi/platform/platform_mutex.c
	fcm360w/src/drv_espiwifi/platform/platform_memory.c
	fcm360w/src/drv_espiwifi/platform/platform_queue.c
	fcm360w/src/drv_espiwifi/platform/platform_sem.c
	fcm360w/src/drv_espiwifi/platform/platform_os.c
	fcm360w/src/drv_espiwifi/platform/platform_thread.c
	fcm360w/src/drv_espiwifi/driver/drv_espi.c
	fcm360w/src/drv_espiwifi/driver/drvspi_platform.c
)

list(APPEND SRC_FILES "fcm360w/src/*.c;fcm360w/src/*.C" )
file(GLOB SRC_FILES ${SRC_FILES})
endif()
########################################

message("${SRC_FILES}")

target_sources(${target} PRIVATE
	wifi_demo.c
	#wifi_ap_demo.c
	${SRC_FILES}
)

relative_glob(srcs include/*.h src/*.c inc/*.h)
beautify_c_code(${target} ${srcs})
