# Copyright (C) 2020 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#

set(target cnktd_demo)

add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_app_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC)
target_include_directories(${target} PUBLIC inc)
target_link_libraries(${target} PRIVATE ql_cnktd)

target_sources(${target} PRIVATE
	cnktd_demo.c
    cnktd_sms_demo.c
    cnktd_nw_demo.c
    cnktd_socket_demo.c
    cnktd_http_demo.c
)

relative_glob(srcs *.c inc/*.h)
beautify_c_code(${target} ${srcs})
