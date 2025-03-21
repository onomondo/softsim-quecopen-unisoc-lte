# Copyright (C) 2020 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#

if(CONFIG_QL_OPEN_EXPORT_PKG)

################################################################################################################
# Quectel open sdk feature config
################################################################################################################
if(QL_APP_FEATURE_WIFI)
option(QL_WIFI_FC41D     "Enable QL_WIFI_FC41D " OFF)
option(QL_WIFI_FCM360W   "Enable QL_WIFI_FCM360W " OFF)
option(QL_WIFI_FGM842D   "Enable QL_WIFI_FGM842D " OFF)
endif()

# 判断只能开启一个宏
if(QL_WIFI_FC41D AND (QL_WIFI_FCM360W OR QL_WIFI_FGM842D))
    message(FATAL_ERROR "Only one of the macros QL_WIFI_FC41D, QL_WIFI_FCM360W, QL_WIFI_FGM842D can be enabled!")
elseif(QL_WIFI_FCM360W AND (QL_WIFI_FC41D OR QL_WIFI_FGM842D))
    message(FATAL_ERROR "Only one of the macros QL_WIFI_FC41D, QL_WIFI_FCM360W, QL_WIFI_FGM842D can be enabled!")
elseif(QL_WIFI_FGM842D AND (QL_WIFI_FC41D OR QL_WIFI_FCM360W))
    message(FATAL_ERROR "Only one of the macros QL_WIFI_FC41D, QL_WIFI_FCM360W, QL_WIFI_FGM842D can be enabled!")
endif()

message(STATUS "QL_WIFI_FC41D ${QL_WIFI_FC41D}")
message(STATUS "QL_WIFI_FCM360W ${QL_WIFI_FCM360W}")
message(STATUS "QL_WIFI_FGM842D ${QL_WIFI_FGM842D}")

endif()