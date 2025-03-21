#ifndef __VENDOR_SE_V1_H__
#define __VENDOR_SE_V1_H__

#include "alipay_common.h"

/*
*发送请求
*/
EXTERNC int32_t csi_user_cmd_process_sync(uint8_t *req, int32_t len_req, uint8_t *rsp, int32_t *len_rsp );

/*
*进入低功耗模式
*/
EXTERNC void csi_enter_lpm(void *ctx);

/*
*退出低功耗模式
*/
EXTERNC void csi_exit_lpm(void *ctx);


typedef enum {
    CSI_OK          =  0,
    CSI_ERROR       = -1,
    CSI_BUSY        = -2,
    CSI_TIMEOUT     = -3,
    CSI_UNSUPPORTED = -4
 } csi_error_t;

/*○ description:
    ■ init sha context
  ○ param
    ■ context - sha context to init
  ○ return
    ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_sha_init(void *context);

/*○ description:
    ■ uninit sha context
  ○ param
    ■ context - sha context to uninit
  ○ return
    ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_sha_uninit(void *context);

/*○ description:
    ■ start sha operate
  ○ param
    ■ context - sha context
		■ mode - sha mode(sha256)
  ○ return
    ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_sha_start(void *context, uint32_t mode);

/*○ description:
    ■ finish sha operate
  ○ param
    ■ context - sha context
		■ in - hash data buffer
    ■ size - hash data len
		■ digest - digest data buffer
  ○ return
    ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_sha_finish(void *context, void *in, uint32_t size, void *digest);

/*○ description:
    ■ open asset and get asset handler
  ○ param
    ■ fd  - asset handler
    ■ asset_name - asset name
  ○ return
    ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_open_asset(void **fd, char asset_name[36]);

/*○ description:
    ■ read asset data
  ○ param
    ■ fd  - asset handler
    ■ offset - asset offset
    ■ buffer - buffer which store data to read
    ■ data_len - length of data read to buffer
  ○ return
    ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_read_asset(void* fd,uint32_t offset,void *buffer,uint32_t* data_len);

/*○ description:
    ■ close asset
  ○ param
    ■ fd - asset handler
  ○ return
    ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_close_asset(void* fd);

#endif