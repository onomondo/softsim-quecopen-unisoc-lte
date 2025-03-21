#ifndef __CSI_DEVICE_INFO_H__
#define __CSI_DEVICE_INFO_H__

#include "alipay_common.h"
#include "csi_common.h"

/*○ description:
    ■ get device id(mac addres format in hex separated ':')，id must ended with ‘\0’, and len_did not include '\0'.All letters should be capital，len_did must be 17.For example："AA:BB:CC:00:11:22"
  ○ param
    ■ buf_did - buffer to store device id
    ■ len_did - the length of device id
  ○ return
    ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_get_deviceid(uint8_t* buf_did, uint32_t *len_did);

/*○ description:
    ■ get serial number of device(the sn which factory printed on the card)，len of sn <= 32 bytes,sn could only include letters or numbers or underline.Only product(type of card) which need to show sn on small routine to realize this function，otherwise set buf_sn to "" and len_sn to 0
  ○ param
    ■ buf_sn - buffer to store device serial number
    ■ len_sn - length of device serial number
  ○ return
    ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_get_sn(uint8_t* buf_sn, uint32_t *len_sn);

/*○ description:
    ■ get device company name
  ○ param
    ■ buffer - buffer to store company name
    ■ len - length of company name
  ○ return
    ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_get_companyname(uint8_t* buffer, uint32_t* len);

/*○ description:
    ■ get device product model
  ○ param
    ■ buffer - buffer to store product model
    ■ len - length of product model
  ○ return
    ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_get_productmodel(uint8_t* buffer, uint32_t* len);


/*○ description:
  ■ 获取加密芯片固件版本
  ○ param:
  ■ version 版本号
  ○ return:
  ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_get_fw_version(char *version);



#endif
