/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: 生成安全随机数，厂家不用实现，直接调用
 */
#ifndef HILINK_SEC_RANDOM_H
#define HILINK_SEC_RANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*RandEntropy)(unsigned int *entropyValue);

/*
 * 注册硬件熵源,厂家不用实现该接口，只需调用该接口
 */
int HILINK_RegisterRandomEntropy(RandEntropy randEntropy);

/*
 * 如果模组有硬件熵源，在调用HILINK_RegisterRandomEntropy接口注册熵源后，
 * 调用该接口生成安全随机数, 其范围为[0,2^32),厂家不用实现该接口，只需调用该接口
 * 参数securityRandom用于保存生成的安全随机数
 * 返回值: 成功返回0, 失败返回非0值
 */
int HILINK_SecRandom(unsigned int *securityRandom);

#ifdef __cplusplus
}
#endif

#endif