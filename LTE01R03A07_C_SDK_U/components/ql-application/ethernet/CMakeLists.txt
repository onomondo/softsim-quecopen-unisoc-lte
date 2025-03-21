# Copyright (C) 2020 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#

set(target ql_app_ethernet)

add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_app_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC)
target_include_directories(${target} PUBLIC inc)

include(./inc/ethernet_demo_macro.cmake)

configure_file (./inc/ethernet_demo_macro.h.in ${out_inc_dir}/ethernet_demo_macro.h)

#######################################
if(ETHERNET_PHY_CH395)
target_include_directories(${target} PUBLIC 
	./ch395/inc
	./ch395/driver/include
)

target_sources(${target} PRIVATE
	./ch395/ch395.c
	./ch395/driver/CH395CMD.C
)

endif()
########################################


target_sources(${target} PRIVATE
	#ethernet_demo.c
	#ethernet_http_demo.c
	#ethernet_router_demo.c
	ethernet_ap_demo.c
)

relative_glob(srcs include/*.h src/*.c inc/*.h)
beautify_c_code(${target} ${srcs})
