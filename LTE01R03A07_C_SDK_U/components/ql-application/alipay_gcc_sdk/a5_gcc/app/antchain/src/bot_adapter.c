/***********************************************************************************************************************
 * Copyright (C) 2021-2022 Alibaba Group Holding Limited
 * description: source file of bot hal iot. It defines the function of iot hal adpter interface.
 *              Customers are requested to implement all functions befor integrating it into your own project.
 * author:      Bot Team
 * date:        2021-11-08
***********************************************************************************************************************/
/***********************************************************************************************************************
 * Including File
***********************************************************************************************************************/
#include "bot_adapter.h"
#include "bot_device_api.h"

/***********************************************************************************************************************
 * Macro Definition
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Enumeration Definition
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Type & Structure Definition
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variable Definition
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Fuction Declaration
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Fuction Implementation
***********************************************************************************************************************/
uint16_t bot_get_vendor(void){
    return 0x8601;
}

int bot_get_meta_id(void){
    return 1;
}

int user_assetInfo_get(uint8_t *reg_data){
    if (reg_data == NULL) {
        bot_printf("Input data is NULL\r\n");
        return -1;
    }

    bot_get_timestamp();

    bot_asset_reg_t asset_reg = {0};

    const char *device_id = bot_get_device_id();
    const char *timestamp = bot_get_timestamp();
    const uint16_t vendor = bot_get_vendor();
    
    memcpy(asset_reg.device_id, device_id, strlen(device_id));
    memcpy(asset_reg.product_time, timestamp, strlen(timestamp));
    asset_reg.vendor = vendor;

    asset_reg.device_id[strlen(device_id)] = '\0';
    asset_reg.product_time[strlen(timestamp)] = '\0';

    bot_snprintf((char*)reg_data, BOT_USER_ASSET_STRING_MAX_SIZE, BOT_ASSET_INFO_PRINT_FMT, \
                  asset_reg.device_id, \
                  asset_reg.product_time, \
                  asset_reg.vendor);

    bot_printf("asset_data: %s, len is: %d\r\n", reg_data, strlen((const char*)reg_data));

    return 0;

}

