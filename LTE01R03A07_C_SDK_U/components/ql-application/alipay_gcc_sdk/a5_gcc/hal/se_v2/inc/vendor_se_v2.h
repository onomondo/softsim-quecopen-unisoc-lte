#ifndef __VENDOR_SE_V2_H__
#define __VENDOR_SE_V2_H__

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


#endif