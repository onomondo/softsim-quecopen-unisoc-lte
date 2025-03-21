/**
 * @file bot_adapter.h
 * @brief 适配接口头文件
 *
 * @copyright Copyright (C) 2015-2023 Ant Group Holding Limited
 */

#ifndef __BOT_ADAPTER_H__
#define __BOT_ADAPTER_H__


#ifdef __cplusplus
extern "C"
{
#endif
/***********************************************************************************************************************
 * Including File
***********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/***********************************************************************************************************************
 * Macro Definition
***********************************************************************************************************************/
#define BOT_USER_ASSET_STRING_MAX_SIZE          600u
#define BOT_ASSET_INFO_PRINT_FMT                "{\"DEVICE-ID\":\"%s\", \"PRODUCT-TIME\":\"%s\", \"VENDOR\":\"%04x\"}"

#define DEVICE_ID_MAX_LEN                       40u
#define DEVICE_MAC_MAX_LEN                      20u
#define DEVICE_TIME_MAX_LEN                     14u


/***********************************************************************************************************************
 * Enumeration Definition
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Type & Structure Definition
***********************************************************************************************************************/

typedef struct _bot_asset_reg_t{
    char device_id[DEVICE_ID_MAX_LEN];
    char mac[DEVICE_MAC_MAX_LEN];
    char product_time[DEVICE_TIME_MAX_LEN];
    uint16_t vendor;
}bot_asset_reg_t;

/***********************************************************************************************************************
 * Global Variable Definition
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Fuction Declaration
***********************************************************************************************************************/
/**
 * @brief Write to the character string
 *
 * @param[in]  str buff to store the data
 * @param[in]  len length of the buffer
 * @param[in]  fmt format
 *
 * @return the number of characters printed
 */
int bot_snprintf(char *str, const int len, const char *fmt, ...);
/**
 * @brief Printf log output
 *
 * @param[in]  fmt format
 *
 * @return void
 */
void bot_printf(const char *fmt, ...);

/**
 * @brief get device id
 *
 * @return the device id string.
 */
const char *bot_get_device_id(void);

/**
 * @brief get device's time
 *
 * @return the timestamp string.
 */
const char *bot_get_timestamp(void);


/**
 * @brief get device's asset or register info
 *
 * @param[out]  reg_data   the info of the device's asset or register info.
 *
 * @return 0 on success, otherwise negative error code indicating the cause of error will be returned.
 */
int user_assetInfo_get(uint8_t *reg_data);

/**
 * @brief Get meta module type, it must be consistent with the ANT IoT trust platform
 *
 * @return meta module type.
 */
int bot_get_meta_id(void);

/**
 * @brief get device's vendor
 *
 * @return the vendor number.
 */
uint16_t bot_get_vendor(void);



#ifdef __cplusplus
}
#endif


#endif	 /* __BOT_ADAPTER_H__ */

