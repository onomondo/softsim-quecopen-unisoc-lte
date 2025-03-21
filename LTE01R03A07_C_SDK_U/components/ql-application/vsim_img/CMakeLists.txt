# Copyright (C) 2020 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#

if (QL_APP_PACK_FILE)
if (QL_CCSDK_BUILD)
set(CONFIG_PACKAGE_FILE_APPIMG_JSON_PATH ${QL_APP_PACK_FILE_JSON_PATH})
else()
set(CONFIG_PACKAGE_FILE_APPIMG_JSON_PATH $ENV{prepack_json_path})
endif()
endif()

set(target ${QL_APP_BUILD_VER})
if(CONFIG_APPIMG_LOAD_FLASH)
	add_appimg_flash_ql_example(${target} vsim_img_init.c)
endif()
if(CONFIG_APPIMG_LOAD_FILE)
	add_appimg_file_ql_example(${target} vsim_img_init.c)
endif()

target_link_libraries(${target}
    PRIVATE
        onomondo_vsim_adapt
        softsim_uicc
        softsim_milenage
        softsim_crypto
        softsim_storage
        softsim_utils
        ss_log
        ss_mem
)
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../onomondo/inc)
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../onomondo/include)

target_link_libraries(${target} PRIVATE ${libc_file_name} ${libm_file_name} ${libgcc_file_name})
target_include_directories(${target} PUBLIC inc)
target_include_targets(${target} PRIVATE kernel driver atr)

relative_glob(srcs *.h *.c inc/*.h)
beautify_c_code(${target} ${srcs})
