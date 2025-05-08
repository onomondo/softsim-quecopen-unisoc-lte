# Copyright (c) 2024 Onomondo ApS. All rights reserved.
# SPDX-License-Identifier: GPL-3.0-only

set(target onomondo_vsim_adapt)

add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_app_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC)
target_include_directories(${target} PUBLIC inc)
target_include_directories(${target} PUBLIC include)

target_sources(${target} PRIVATE
    ss_fs.c
    ss_bootstrap.c
    ss_static_files_hex.c
    ss_provisioning.c
)
target_link_libraries(${target} INTERFACE ${libc_file_name} ${libm_file_name} ${libgcc_file_name})
relative_glob(srcs include/*.h src/*.c inc/*.h)
beautify_c_code(${target} ${srcs})

add_library(ss_log STATIC)
target_sources(ss_log PRIVATE log.c)
target_include_directories(ss_log PUBLIC inc)
target_link_libraries(ss_log INTERFACE ${libc_file_name} ${libm_file_name} ${libgcc_file_name})

add_library(ss_mem STATIC)
target_sources(ss_mem PRIVATE mem.c)
target_include_directories(ss_mem PUBLIC inc)
target_include_directories(ss_mem PUBLIC include)

add_library(softsim_utils STATIC IMPORTED GLOBAL)
set_target_properties(softsim_utils PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libutils.a)
set_target_properties(softsim_utils PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(softsim_storage STATIC IMPORTED GLOBAL)
set_target_properties(softsim_storage PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libstorage.a)
set_target_properties(softsim_storage PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(softsim_crypto STATIC IMPORTED GLOBAL)
set_target_properties(softsim_crypto PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libcrypto.a)
set_target_properties(softsim_crypto PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(softsim_milenage STATIC IMPORTED GLOBAL)
set_target_properties(softsim_milenage PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libmilenage.a)
set_target_properties(softsim_milenage PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(softsim_uicc STATIC IMPORTED GLOBAL)
set_target_properties(softsim_uicc PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libuicc.a)
set_target_properties(softsim_uicc PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include)
