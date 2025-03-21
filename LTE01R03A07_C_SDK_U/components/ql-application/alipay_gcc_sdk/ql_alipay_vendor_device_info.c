/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------
=================================================================*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_api_dev.h"
#include "ql_api_bt.h"
#include "ql_log.h"
#include "alipay_common.h"

retval_e alipay_get_mac(uint8_t* buf_did, uint32_t *len_did)
{
    ql_bt_addr_s mac_addr = {0};
    if (QL_BT_SUCCESS != ql_bt_ble_get_nv_addr(&mac_addr))
    {
        return ALIPAY_RV_HAL_ERROR;
    }
    *len_did = 17;
    snprintf((char *)buf_did, 18, "%02X:%02X:%02X:%02X:%02X:%02X", mac_addr.addr[0], mac_addr.addr[1], mac_addr.addr[2], 
        mac_addr.addr[3], mac_addr.addr[4], mac_addr.addr[5]);
    return ALIPAY_RV_OK;
}

retval_e alipay_get_sn(uint8_t* buf_sn, uint32_t *len_sn) 
{
    if (QL_DEV_SUCCESS != ql_dev_get_sn((char *)buf_sn, 32, 0))
    {
        return ALIPAY_RV_HAL_ERROR;
    }

    *len_sn = strlen((char *)buf_sn);
    return ALIPAY_RV_OK;
}

retval_e alipay_get_imei(uint8_t* buf_imei, uint32_t *len_imei) 
{
    if (QL_DEV_SUCCESS != ql_dev_get_imei((char *)buf_imei, 32, 0))
    {
        return ALIPAY_RV_HAL_ERROR;
    }

    *len_imei = strlen((char *)buf_imei);
    return ALIPAY_RV_OK;
}

/*获取设备类型，根据设备实际情况返回
 * @param 
 * @return 0    成人手表
 *         1    成人手环
 *         2    成人卡片
 *         3    儿童手表
 *         4    儿童手环
 *         5    学生卡
 */
// 默认0, 测试商家聚合码需要设置为3
int g_productType = 0;
uint32_t alipay_get_productType(void)
{
    return g_productType;
}

//company name需要客户自己设配，不能超过15个字节
#define COMPANY_NAME "ZHIFUBAO"
retval_e alipay_get_companyName(uint8_t* buffer, uint32_t* len) 
{
    if(*len < (strlen(COMPANY_NAME)+1))
    {
        return 1;
    }
    memcpy((char *)buffer, COMPANY_NAME, strlen(COMPANY_NAME));
    *len = strlen((char *)buffer);
    return ALIPAY_RV_OK;
}

//product model需要客户自己设配，不能超过23个字节
#define PRODUCT_MODEL "MODEL_IN_BLUTOOTH"
retval_e alipay_get_productModel(uint8_t* buffer, uint32_t* len) 
{
    if(*len < (strlen(PRODUCT_MODEL)+1))
    {
        return 1;
    }
    memcpy((char *)buffer, PRODUCT_MODEL, strlen(PRODUCT_MODEL));
    *len = strlen((char *)buffer);
    return ALIPAY_RV_OK;
}

