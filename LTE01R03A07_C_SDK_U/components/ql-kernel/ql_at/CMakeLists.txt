# Copyright (C) 2021 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#

configure_file(../inc/quec_proj_config_at.h.in ${out_inc_dir}/quec_proj_config_at.h)

#生成AT命令总列表,AT命令来源于3个头文件:
#quec_atcmd_def.h --- 存放移远的AT
#quec_atcmd_def_unisoc.h --- 存放展锐的原始AT
#ql_atcmd_def.h --- 存放OPEN客户自定义AT
set(target at_cmd_table_std)
set(gperf_in src/ql_at_cmd_table_std.gperf)
set(gperf_h ${CMAKE_CURRENT_BINARY_DIR}/ql_at_cmd_table_std.h)
cpp_only(${target} ${gperf_in})

add_custom_command(OUTPUT ${gperf_h}
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/src/gperf2h.py
        $<TARGET_OBJECTS:at_cmd_table_std> ${gperf_h}
    DEPENDS $<TARGET_OBJECTS:at_cmd_table_std>
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gperf2h.py
)

set(target ql_at_list)
add_app_libraries($<TARGET_FILE:${target}>)
add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_quec_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC)
target_include_directories(${target} PUBLIC inc)
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${target} PRIVATE kernel ats atr)
target_sources(${target} PRIVATE
	${gperf_h}
    src/quec_atc_search.c
)

set(target ql_open_at)
add_app_libraries($<TARGET_FILE:${target}>)
add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC)
target_include_directories(${target} PUBLIC inc)
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${target} PRIVATE kernel driver hal fs fsmount audio ml ats atr)

if (CONFIG_QUEC_PROJECT_FEATURE_OHOS_AT)
target_link_libraries(${target} PRIVATE ../libs/ql_ohos_at)
endif()

target_sources(${target} PRIVATE
    src/ql_atcmd_handler.c
)

relative_glob(srcs include/*.h src/*.c inc/*.h)
beautify_c_code(${target} ${srcs})
