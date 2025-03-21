# Copyright (C) 2020 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#

set(target ql_app_xlat)

add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_app_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC)
target_include_directories(${target} PUBLIC inc ../ql_api/include ../ql_api/xlat/inc)
IF(EXISTS ql_api_xlat)
target_include_targets(${target} PRIVATE ql_api_xlat)
ENDIF()
set(ql_xlat_out ${out_lib_dir}/libql_api_xlat.a)
target_sources(${target} PRIVATE
	xlat_demo.c
)

relative_glob(srcs include/*.h src/*.c inc/*.h)
beautify_c_code(${target} ${srcs})
