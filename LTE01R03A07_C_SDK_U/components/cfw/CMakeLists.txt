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

set(LTE_SUPPORT ${CONFIG_LTE_SUPPORT})
set(LTE_NBIOT_SUPPORT ${CONFIG_LTE_NBIOT_SUPPORT})
configure_file(include/cfw_config.h.in ${out_inc_dir}/cfw_config.h)

if(CONFIG_SOC_8910)
    configure_file(include/modem_config_ext.h.in ${out_inc_dir}/modem_config.h)
endif()

set(target cfw)

include(core.cmake)

