#include "alipay_common.h"
#include <stdio.h>
#include <string.h>
#include "hal_device_info.h"
#include "csi_device_info.h"

retval_e alipay_get_mac(uint8_t* buf_did, uint32_t *len_did) {
    hal_error_t hal_ret = hal_get_deviceid(buf_did, len_did);
    retval_e ret = ALIPAY_RV_OK;
    if(hal_ret == HAL_INVALID_PARAM) ret = ALIPAY_RV_WRONG_PARAM;
    if(hal_ret == HAL_ERROR) ret = ALIPAY_RV_HAL_ERROR;
    return ret;
}

retval_e alipay_get_sn(uint8_t* buf_sn, uint32_t *len_sn) {
    hal_error_t hal_ret = hal_get_sn(buf_sn, len_sn);
    retval_e ret = ALIPAY_RV_OK;
    if(hal_ret == HAL_INVALID_PARAM) ret = ALIPAY_RV_WRONG_PARAM;
    if(hal_ret == HAL_ERROR) ret = ALIPAY_RV_HAL_ERROR;
    return ret;
}

retval_e alipay_get_imei(uint8_t* buf_imei, uint32_t *len_imei) {
    return alipay_get_sn(buf_imei, len_imei);
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
uint32_t alipay_get_productType(void) {
    #warning alipay_get_productType demo
    return g_productType;
}

retval_e alipay_get_companyName(uint8_t* buffer, uint32_t* len) {
    hal_error_t hal_ret = hal_get_companyname(buffer, len);
    retval_e ret = ALIPAY_RV_OK;
    if(hal_ret == HAL_INVALID_PARAM) ret = ALIPAY_RV_WRONG_PARAM;
    if(hal_ret == HAL_ERROR) ret = ALIPAY_RV_HAL_ERROR;
    return ret;
}

retval_e alipay_get_productModel(uint8_t* buffer, uint32_t* len) {
    hal_error_t hal_ret = hal_get_productmodel(buffer, len);
    retval_e ret = ALIPAY_RV_OK;
    if(hal_ret == HAL_INVALID_PARAM) ret = ALIPAY_RV_WRONG_PARAM;
    if(hal_ret == HAL_ERROR) ret = ALIPAY_RV_HAL_ERROR;
    return ret;
}

/*-------------------------------    HAL     --------------------------------------*/

hal_error_t hal_get_deviceid(uint8_t *buf_did, uint32_t *len_did)
{
    csi_error_t ret;

    if(!buf_did || !len_did) {
        return HAL_INVALID_PARAM;
    }

    ret = csi_get_deviceid(buf_did, len_did);
    if(ret || !(*len_did)) {
        return HAL_ERROR;
    }

    return HAL_OK;
}


hal_error_t hal_get_sn(uint8_t* buf_sn, uint32_t *len_sn)
{
    csi_error_t ret;

    if(!buf_sn || !len_sn) {
        return HAL_INVALID_PARAM;
    }

    ret = csi_get_sn(buf_sn, len_sn);
    if(ret || !(*len_sn)) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

hal_error_t hal_get_companyname(uint8_t* buffer, uint32_t* len)
{
    csi_error_t ret;

    if(!buffer || !len) {
        return HAL_INVALID_PARAM;
    }

    ret = csi_get_companyname(buffer, len);
    if(ret || !(*len)) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

hal_error_t hal_get_productmodel(uint8_t* buffer, uint32_t* len)
{
    csi_error_t ret;

    if(!buffer || !len) {
        return HAL_INVALID_PARAM;
    }

    ret = csi_get_productmodel(buffer, len);
    if(ret) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

hal_error_t hal_get_fw_version(char *version)
{
    csi_error_t ret;

    if(version == NULL) {
        return HAL_INVALID_PARAM;
    }

    ret = csi_get_fw_version(version);
    if(ret) {
        return HAL_ERROR;
    }

    return HAL_OK;
}
