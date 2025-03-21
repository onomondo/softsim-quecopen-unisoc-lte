#ifndef __HAL_DEVICE_INFO_H__
#define __HAL_DEVICE_INFO_H__

#include "hal_common.h"

hal_error_t hal_get_deviceid(uint8_t* buf_did, uint32_t *len_did);

hal_error_t hal_get_sn(uint8_t* buf_sn, uint32_t *len_sn);

hal_error_t hal_get_companyname(uint8_t* buffer, uint32_t* len);

hal_error_t hal_get_productmodel(uint8_t* buffer, uint32_t* len);

hal_error_t hal_get_fw_version(char *version);


#endif
