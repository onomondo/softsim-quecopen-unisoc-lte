/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: 蓝牙SDK配网API头文件
 */

#ifndef HILINK_BT_NETCFG_API_H
#define HILINK_BT_NETCFG_API_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*HILINK_BT_ProcessNetCfgCb)(const unsigned char *in, unsigned int inLen,
    unsigned char *out, unsigned int outLen);

/* 启动配网 */
int HILINK_BT_StartNetCfg(void);

/* 停止配网 */
int HILINK_BT_StopNetCfg(void);

/* 通知配网状态 */
int HILINK_BT_NotifyNetCfgStatus(int status);

/* 注册接收到配网信息的处理函数 */
int HILINK_BT_RegisterNetCfgDataCb(HILINK_BT_ProcessNetCfgCb callback);

#ifdef __cplusplus
}
#endif
#endif

