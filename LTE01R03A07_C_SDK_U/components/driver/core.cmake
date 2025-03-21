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

set(import_lib ${out_lib_dir}/libdriver_lcd.a)
add_library(driver_combine STATIC IMPORTED GLOBAL)
set_target_properties(driver_combine PROPERTIES IMPORTED_LOCATION ${import_lib} INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include  IMPORTED_LINK_INTERFACE_LIBRARIES driver_camera)

set(import_lib ${out_lib_dir}/libdriver.a)
configure_file(${ql_dm_dir}/libdriver.a ${import_lib} COPYONLY)
add_app_libraries(${import_lib})
add_library(driver STATIC IMPORTED GLOBAL)
set_target_properties(driver PROPERTIES IMPORTED_LOCATION ${import_lib} INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include  IMPORTED_LINK_INTERFACE_LIBRARIES driver_combine)

set(import_lib ${out_lib_dir}/libdriver_core.a)
configure_file(core/${CONFIG_SOC}/libdriver_core.a ${import_lib} COPYONLY)
add_app_libraries(${import_lib})
add_library(driver_core STATIC IMPORTED GLOBAL)
set_target_properties(driver_core PROPERTIES IMPORTED_LOCATION ${import_lib})