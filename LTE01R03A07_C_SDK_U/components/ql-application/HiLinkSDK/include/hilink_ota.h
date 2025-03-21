/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: 此文件提供了升级相关的接口实现示例;
 *              其中升级相关功能的接口需要开发者或厂商进行实现，HiLink SDK在收到升级相关命令时会调用对应接口;
 *              请开发者仔细阅读文件中的注释说明，参考或修改实现。
 */
#ifndef HILINK_OTA_H
#define HILINK_OTA_H

#ifdef __cplusplus
extern "C" {
#endif

// 返回正常
#define RETURN_OK               0
// 没有MCU
#define RETURN_ERROR_NO_MCU     (-1)
// MCU返回其他错误
#define RETURN_ERROR            (-2)
// MCU不需要升级
#define RETURN_MCU_NO_NEED_OTA  (-3)
// 模组可以重启
#define MODULE_CAN_REBOOT       1
// 模组不能重启
#define MODULE_CANNOT_REBOOT    0
// 手动升级
#define UPDATE_TYPE_MANUAL      0
// 自动升级
#define UPDATE_TYPE_AUTO        1
// 开始发送MCU数据
#define START_SEND_DATA         0
// 发送MCU数据结束
#define STOP_SEND_DATA          1
// 异常终止
#define SEND_DATA_ERROR         2

typedef enum {
    SUB_DEV_UPDATE_SUCCESS = 0,
    SUB_DEV_UPDATE_FAIL
} SubDevUpdateStatus;

typedef void (*GetOtaVerCb)(const char *version);

/*
 * 获取MCU当前版本
 * version表示版本字符串
 * inLen表示输入字符串长度
 * outLen表示输出字符串长度
 * 返回值是RETURN_OK时，表示获取成功
 * 返回值是RETURN_ERROR_NO_MCU时，表示没有MCU
 * 返回值是RETURN_ERROR时，表示获取失败
 * 注意：如果获取不到MCU的版本，则不对MCU进行升级。
 * 建议厂商在MCU正常启动后，或升级启动后，就将MCU的版本号传递给模组，确保模组可以获取到MCU的版本。
 */
int HilinkGetMcuVersion(char *version, unsigned int inLen, unsigned int *outLen);

/*
 * 开始模组升级
 * type表示升级类型
 * 当type是UPDATE_TYPE_MANUAL时，表示本次升级流程是由用户主动发起的手动升级
 * 当type是UPDATE_TYPE_AUTO时，表示本次升级流程是经过用户同意的自动升级
 * 返回值是RETURN_OK时，表示处理成功，HiLink SDK将开始启动升级流程
 * 返回值是RETURN_ERROR时，表示处理不成功，HiLink SDK将终止本次升级流程
 * 注意：在手动场景场景下，HiLink SDK在接收到用户发出的升级指令后，将直接调用此接口；
 * 在自动升级场景下，当HiLink SDK在调用HilinkGetRebootFlag接口返回值是MODULE_CAN_REBOOT时，HiLink SDK将调用此接口。
 * 厂商可在此接口中完成和升级流程相关的处理。
 * 开机后10分钟到1小时内随机时间检测一次是否有新版本，之后以当前时间为起点，23小时加1小时内随机值周期性检测新版本。
 * 如果用户打开了自动升级开关，检测到有新版本并且是可以重启的情况下，就进行新版本的下载，下载完成后自动重启。
 * 自动升级流程可能在凌晨进行，因此厂商在实现升级流程相关功能时，确保在升级的下载安装固件和重启设备时避免对用户产生
 * 影响，比如发出声音，光亮等。
 */
int HilinkOtaStartProcess(int type);

/*
 * 模组升级结束
 * status表示升级结果
 * 当status是100时，表示升级成功
 * 当status不是100时，表示升级失败
 * 返回值是RETURN_OK时，表示处理成功，HiLink SDK将置升级标志或切换运行区标志
 * 返回值不是RETURN_OK时，表示处理不成功，HiLink SDK将终止本次升级流程
 * 注意：HiLink SDK在将固件写入到OTA升级区后，且完整性校验通过后，将调用厂商适配的此接口；
 * 厂商可在此接口中完成和升级流程相关的处理。
 * 开机后10分钟到1小时内随机时间检测一次是否有新版本，之后以当前时间为起点，23小时加1小时内随机值周期性检测新版本。
 * 如果用户打开了自动升级开关，检测到有新版本并且是可以重启的情况下，就进行新版本的下载，下载完成后自动重启。
 * 自动升级流程可能在凌晨进行，因此厂商在实现升级流程相关功能时，确保在升级的下载安装固件和重启设备时避免对用户产生
 * 影响，比如发出声音，光亮等；升级类型是否为自动升级可参考接口HilinkOtaStartProcess的参数type的描述。
 */
int HilinkOtaEndProcess(int status);

/*
 * 判断模组是否能立即升级并重启
 * 返回值是MODULE_CAN_REBOOT时，表示模组可以立即升级并重启，HiLink SDK将开始自动升级流程。
 * 返回值是MODULE_CANNOT_REBOOT时，表示模组不能立即升级并重启，HiLink SDK将不进行本次自动升级流程。
 * 注意：在用户同意设备可以自动升级的情况下，HiLink SDK调用此接口获取设备当前业务状态下，模组是否可以立即升级并重启的标志。
 * 只有当设备处于业务空闲状态时，接口才可以返回MODULE_CAN_REBOOT。
 * 当设备处于业务非空闲状态时，接口返回MODULE_CANNOT_REBOOT。
 */
int HilinkGetRebootFlag();

/*
 * HiLink SDK调用厂商适配的此接口通知MCU固件传输的状态
 * flag表示升级流程标志
 * 当flag是START_SEND_DATA时，表示通知模组即将开始发送MCU固件数据包
 * 当flag是STOP_SEND_DATA时，表示通知模组完整的MCU固件包已发送完成
 * 当flag是SEND_DATA_ERROR时，表示通知模组本次MCU固件升级异常终止
 * len表示MCU固件包的大小
 * type表示升级类型
 * 当type是UPDATE_TYPE_MANUAL时，表示本次升级流程是由用户主动发起的手动升级
 * 当type是UPDATE_TYPE_AUTO时，表示本次升级流程是经过用户同意的自动升级
 * 返回值是RETURN_OK时，表示处理成功，HiLink SDK继续正常处理后续流程
 * 返回值是RETURN_ERROR时，表示处理失败，HiLink SDK将终止本次MCU升级流程
 * 注意：当flag是STOP_SEND_DATA时，此接口需返回MCU侧固件升级的结果；当flag是其它值时，需返回接口接收到此消息后的处理结果。
 * 开机后10分钟到1小时内随机时间检测一次是否有新版本，之后以当前时间为起点，23小时加1小时内随机值周期性检测新版本。
 * 如果用户打开了自动升级开关，检测到有新版本并且是可以重启的情况下，就进行新版本的下载，下载完成后自动重启。
 * 自动升级流程可能在凌晨进行，因此厂商在实现升级流程相关功能时，确保在升级的下载安装固件和重启设备时避免对用户产生
 * 影响，比如发出声音，光亮等。
 */
int HilinkNotifyOtaStatus(int flag, unsigned int len, unsigned int type);

/*
 * HiLink SDK调用厂商适配的此接口通知厂商发送MCU固件文件数据
 * data表示发送的数据
 * len表示发送的数据的长度
 * offset表示发送的数据起始位置相对于完整固件包的偏移量
 * 此接口需要返回MCU接收这部分数据的处理结果
 * 返回值是RETURN_OK时，模组将通知的数据正确发送给MCU，且MCU正确处理发送的数据，HiLink SDK将继续正常处理后续流程
 * 返回值是RETURN_ERROR时，模组或MCU未能正常处理通知的MCU的固件文件数据，HiLink SDK将终止本次MCU固件升级流程
 */
int HilinkNotifyOtaData(const unsigned char *data, unsigned int len, unsigned int offset);

/*
 * 获取设备升级文件保存路径
 * filePath表示设备升级文件保存路径
 * len表示设备升级文件保存路径的长度
 * 返回值是RETURN_OK时，表示获取成功
 * 返回值是RETURN_ERROR时，表示获取失败
 * 注意：Linux系统和安卓系统需要适配此接口，其他操作系统不用适配此接口。
 * HiLink SDK下载的设备升级文件需保存在设备上的某个文件路径下；
 * 此文件路径，需由厂商适配提供，例如“/var/update.bin”。
 */
int HILINK_GetUpdateFilePath(char *filePath, unsigned int len);

/*
 * 厂商适配此接口提供设备上能够保存的升级文件的最大大小，
 * 避免当HiLink SDK下载的升级文件大小大于分区大小时，导致升级失败。
 * 注意：Linux系统和安卓系统需要适配此接口，其他操作系统不用适配此接口。
 */
unsigned int HILINK_GetMaxUpdateFileSize(void);

/*
 * 启动软件安装
 * 返回值是RETURN_OK时，表示启动成功
 * 返回值是RETURN_ERROR时，表示启动失败
 * 注意：Linux系统和安卓系统需要适配此接口，其他操作系统不用适配此接口。
 * HiLink SDK完成新版本的检测和下载流程后，由厂商实现软件安装流程。
 * HiLink SDK调用此接口后便会退出升级流程，交由厂商的安装程序来完成软件的安装。
 */
int HILINK_StartSoftwareIntall(void);

/*
 * 获取桥下子设备的当前版本
 * version返回同一proId品类的最低版本号
 * 返回值是RETURN_OK时，表示获取成功，反之失败
 */
int HILINK_GetBrgSubDevVersion(const char *proId, unsigned int proIdLen,
    char *version, unsigned int versionLen);

/*
 * HiLink SDK调用厂商适配的此接口通知厂商桥下子设备的hota最新版本号
 * version表示同一proId品类的最新版本号
 * 返回值是RETURN_OK时，表示获取成功，反之失败
 */
int HILINK_NotifyBrgSubDevNewVersion(const char *proId, unsigned int proIdLen, const char *version,
    unsigned int versionLen);

/*
 * HiLink SDK调用厂商适配的此接口通知厂商发送桥下子设备固件数据
 * data表示发送的数据
 * len表示发送的数据的长度
 * offset表示发送的数据起始位置相对于完整固件包的偏移量
 * 返回值是RETURN_OK时，表示接收成功，反之失败
 */
int HILINK_NotifyBrgSubDevOtaData(const char *proId, unsigned int proIdLen,
    const unsigned char *data, unsigned int len, unsigned int offset);
/*
 * HiLink SDK调用厂商适配的此接口通知桥下子设备固件传输的状态
 * flag表示升级流程标志
 * 当flag是START_SEND_DATA时，表示通知模组即将开始发送桥下子设备固件数据包
 * 当flag是STOP_SEND_DATA时，表示通知模组完整的桥下子设备固件包已发送完成
 * 当flag是SEND_DATA_ERROR时，表示通知模组本次桥下子设备固件升级异常终止
 * len表示MCU固件包的大小
 * type表示升级类型
 * 当type是UPDATE_TYPE_MANUAL时，表示本次升级流程是由用户主动发起的手动升级
 * 当type是UPDATE_TYPE_AUTO时，表示本次升级流程是经过用户同意的自动升级
 * 返回值是RETURN_OK时，表示处理成功，HiLink SDK继续正常处理后续流程
 * 返回值是RETURN_ERROR时，表示处理失败，HiLink SDK将终止本次桥下子设备升级流程
 * 注意:当flag是STOP_SEND_DATA时，此接口需返回桥下子设备侧固件升级的结果；当flag是其它值时，需返回接口接收到此消息后的处理结果。
 * 开机后10分钟到1小时内随机时间检测一次是否有新版本，如果用户打开了自动升级开关，检测到有新版本并且是可以重启的情况下，
 * 就进行新版本的下载，下载完成后自动重启。
 */
int HILINK_NotifyBrgSubDevOtaStatus(const char *proId, unsigned int proIdLen, int flag,
    unsigned int len, unsigned int type);
/*
 * 上报给模组桥下子设备的升级进度
 * progress合法值是[0,100]，建议5%递增上报，无需频繁上报。
 * 注意:该接口只能在支持子设备升级的SDK版本中调用
 */
void HILINK_ReportBrgSubDevUpdateProgress(const char *proId, unsigned int proIdLen, int progress);
/*
 * 上报给模组桥下子设备的升级状态
 * 上报的状态在SubDevUpdateStatus枚举中
 * 无论是成功还是失败，都需要确保升级结束后将升级状态上报给模组。
 */
void HILINK_ReportBrgSubDevUpdateStatus(const char *proId, unsigned int proIdLen, SubDevUpdateStatus status);

/*
 * 触发设备的ota新版本检测
 * 检测到的版本，sdk将通过cb回调函数上报。如果不需要获取版本号，则可将cb置空。
 * 上报的version如果为空，则未成功获取到新版本。
 * 注意:1、建议调用频次最多一天一次
 * 2、首次通过手机配网成功后不能调用，建议连云成功后至少5s后调用
 */
void HILINK_TrigOtaVersionCheck(GetOtaVerCb cb);
#ifdef __cplusplus
}
#endif

#endif