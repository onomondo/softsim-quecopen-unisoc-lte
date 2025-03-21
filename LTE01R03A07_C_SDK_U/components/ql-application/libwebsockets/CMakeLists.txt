# Copyright (C) 2020 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#

set(target ql_app_websocket)

add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_app_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC)
target_include_directories(${target} PUBLIC inc ../ql_api/include ../ql_api/libwebsockets/api ../ql_api/libwebsockets/config ../ql_api/libwebsockets/inc ../ql_api/libwebsockets/inc/libwebsockets)
IF(EXISTS ql_api_websocket)
target_include_targets(${target} PRIVATE ql_api_websocket)
ENDIF()
set(ql_websocket_out ${out_lib_dir}/libql_api_websocket.a)
target_sources(${target} PRIVATE
	websocket_demo.c
)

relative_glob(srcs include/*.h src/*.c inc/*.h)
beautify_c_code(${target} ${srcs})
