# Copyright (C) 2020 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#

set(target ql_cnktd)

add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_app_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC)
target_include_directories(${target} PUBLIC inc)
target_link_libraries(${target} PRIVATE ${libc_file_name} ${libm_file_name} ${libgcc_file_name})
target_include_targets(${target} PRIVATE ql_app_peripheral)

target_sources(${target} PRIVATE
	ql_cnktd_device.c
	ql_cnktd_uart.c
	ql_cnktd_fs.c
	ql_cnktd_spi.c
	ql_cnktd_nw.c
	ql_cnktd_sms.c
	ql_cnktd_coap.c
	ql_cnktd_socket.c
	ql_cnktd_http.c
)

relative_glob(srcs *.c inc/*.h)
beautify_c_code(${target} ${srcs})

add_subdirectory_if_exist(demo)
