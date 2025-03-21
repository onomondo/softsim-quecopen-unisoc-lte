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

configure_file(include/net_config.h.in ${out_inc_dir}/net_config.h)

set(target net)
include(core.cmake)

#quectel add start === Open source into the SDK
set(target net_src_lwip)
add_app_libraries($<TARGET_FILE:${target}>)

add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_NET)
target_include_directories(${target} PUBLIC include)
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(${target} PRIVATE lwip/include)
target_include_directories(${target} PRIVATE lwip/src/include)
target_include_directories(${target} PRIVATE lwip/src/include/lwip)
target_include_directories(${target} PRIVATE lwip/src/include/lwip/priv)
target_include_directories(${target} PRIVATE ../ats/include)
target_include_directories(${target} PRIVATE ../hal/include)
target_include_directories(${target} PRIVATE ../net/mupnp/src/mupnp/include)
target_include_directories(${target} PRIVATE ../net/mbedtls/include)
target_include_directories(${target} PRIVATE http/include)
target_include_directories(${target} PRIVATE ../json/cJSON)
target_include_directories(${target} PRIVATE coap/include)
target_include_directories(${target} PRIVATE ctiotcryplib/include)
target_include_directories(${target} PRIVATE ../fs/include)

target_include_targets(${target} PRIVATE kernel driver calclib lwip atr cfw json fs nvm fupdate service)

target_sources(${target} PRIVATE
    src/lwipports.c
    src/cfw_sockets_api.c
    )
#quectel add end

add_subdirectory_if_exist(lwip)
if (NOT CONFIG_QUEC_PROJECT_FEATURE)
add_subdirectory_if_exist(libmqttsn)
add_subdirectory_if_exist(libcoap)
endif()
add_subdirectory_if_exist(json)
if (CONFIG_QUEC_PROJECT_FEATURE_SSL)
add_subdirectory_if_exist(mbedtls)
endif()
if (CONFIG_QUEC_PROJECT_FEATURE_XLAT)
add_subdirectory_if_exist(xlat)
endif()
if (NOT CONFIG_QUEC_PROJECT_FEATURE)
add_subdirectory_if_exist(libexpat)
add_subdirectory_if_exist(edp)
add_subdirectory_if_exist(http)
add_subdirectory_if_exist(mupnp)
add_subdirectory_if(CONFIG_NET_LWM2M_SUPPORT THEN liblwm2m)
add_subdirectory_if(CONFIG_FTP_MODULE_SUPPORT THEN ftp)
add_subdirectory_if_exist(aliotx)
add_subdirectory_if(CONFIG_NET_CIS_SDK_SUPPORT THEN cis_sdk)
add_subdirectory_if(CONFIG_NET_PAHO_MQTT_SUPPORT THEN pahomqtt)
add_subdirectory_if(CONFIG_NET_ALI_SDK_SUPPORT THEN ali_c_sdk)
endif()
