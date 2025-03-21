#ifndef __VENDOR_CERT_H__
#define __VENDOR_CERT_H__

#include "alipay_common.h"

/*
 * 先擦除后写数据，即每次调用该接口都要先把原来存储的数据清除再写数据
 * 要求掉电保持，
 * 储存的数据不会在「解绑支付宝」和「恢复出厂设置」的时候清除
 *
 * parametr: in: data: 要写的数据
 *               data_len: 要写的数据长度
 *
 * return: 0: 表示成功
 *         -1: 表示失败
*/
EXTERNC int alipay_secure_save_data(PARAM_IN void *data, PARAM_IN uint32_t data_len);

#endif