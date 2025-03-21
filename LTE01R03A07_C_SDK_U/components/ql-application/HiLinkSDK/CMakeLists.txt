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

set(target ql_app_hilinksdk)

add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_app_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_ALIYUN)
target_include_directories(${target} PUBLIC adapter/include include )
target_include_directories(${target} PUBLIC ${SOURCE_TOP_DIR}/components/newlib/include)
target_link_libraries(${target} PRIVATE ${libc_file_name} ${libm_file_name} ${libgcc_file_name} )
target_link_libraries(${target} PRIVATE ql_newlib)
target_sources(${target} PRIVATE
	adapter/hilink_open_config_adapter.c
	adapter/hilink_open_mem_adapter.c
	adapter/hilink_open_nan_adapter.c
	adapter/hilink_open_network_adapter.c
	adapter/hilink_open_ota_adapter.c
	adapter/hilink_open_socket_adapter.c
	adapter/hilink_open_softap_adapter.c
	adapter/hilink_open_str_adapter.c
	adapter/hilink_open_sys_adapter.c
	adapter/hilink_open_timer_adapter.c
	hilink_device.c
	hilink_ota.c
)
# Use a global property to collect all application libraries
define_property(GLOBAL PROPERTY ql_hilinksdk_lib
    BRIEF_DOCS "hilinksdk libraries"
    FULL_DOCS "hilinksdk libraries"
)
set(target ql_hilinksdk)
set(ql_hilinksdk_out ${out_lib_dir}/ql_hilinksdk.a)
get_property(ql_hilinksdk_lib GLOBAL PROPERTY ql_hilinksdk_lib)

list(APPEND ql_hilinksdk_lib ${libc_file_name} ${libm_file_name} ${libgcc_file_name})
list(APPEND ql_hilinksdk_lib ${CMAKE_CURRENT_SOURCE_DIR}/lib/tl8251/lib/debug/libhilinkbtsdk.a)
list(APPEND ql_hilinksdk_lib ${CMAKE_CURRENT_SOURCE_DIR}/lib/WiFi/ota/debug/libhilinkota.a)
list(APPEND ql_hilinksdk_lib ${CMAKE_CURRENT_SOURCE_DIR}/lib/WiFi/softap/debug/libhilinkdevicesdk.a)

add_custom_command(OUTPUT ${ql_hilinksdk_out}
    COMMAND python3 ${groupgen_py} --base ${BINARY_TOP_DIR} ${ql_hilinksdk_out} ${ql_hilinksdk_lib}
    DEPENDS ${groupgen_py} ${ql_hilinksdk_lib}
    WORKING_DIRECTORY ${BINARY_TOP_DIR}
)
add_custom_target(build_${target} DEPENDS ${ql_hilinksdk_out})
add_library(${target} STATIC IMPORTED GLOBAL)
set_target_properties(${target} PROPERTIES IMPORTED_LOCATION ${ql_hilinksdk_out} 
 IMPORTED_LINK_INTERFACE_LIBRARIES ql_app_hilinksdk)

add_dependencies(${target} build_${target})

 
relative_glob(srcs include/*.h )
beautify_c_code(${target} ${srcs})