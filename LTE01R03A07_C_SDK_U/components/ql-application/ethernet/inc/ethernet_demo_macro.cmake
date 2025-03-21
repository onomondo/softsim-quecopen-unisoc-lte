# Copyright (C) 2020 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#

if(CONFIG_QL_OPEN_EXPORT_PKG)

################################################################################################################
# Quectel open sdk feature config
################################################################################################################
if(QL_APP_FEATURE_ETHERNET)
option(ETHERNET_PHY_CH395   "Enable ETHERNET_PHY_CH395 " ON)
else()
option(ETHERNET_PHY_CH395   "Enable ETHERNET_PHY_CH395 " OFF)
#message(STATUS "ETHERNET_PHY_CH395 ${ETHERNET_PHY_CH395}")
endif()

endif()