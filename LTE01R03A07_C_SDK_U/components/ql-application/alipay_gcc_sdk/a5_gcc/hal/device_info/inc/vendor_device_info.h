#ifndef __VENDOR_DEVICE_INFO_H__
#define __VENDOR_DEVICE_INFO_H__

#include "alipay_common.h"

/*获取设备ID(以冒号分割的16进制mac地址)，要求内容以‘\0’结尾且长度不包含'\0'
 * @param [out] buf_did 存放数据的缓存
 * @param [inout] len_did  入参为允许读取的最大长度
 *                         出参为实际读取的数据长度
 * @return RV_OK 获取成功
 *         其他   获取失败
 */
EXTERNC retval_e alipay_get_mac(PARAM_OUT uint8_t* buf_did,PARAM_INOUT uint32_t *len_did);

/*获取设备SN(厂商印刷在卡片上的设备序列号)，长度不超过32个字符，只能包含大小写字母、数字、下划线
 *要求内容以‘\0’结尾且长度不包含'\0'。
 * @param [out] buf_sn 存放数据的缓存
 * @param [inout] len_sn  入参为允许读取的最大长度
 *                         出参为实际读取的数据长度
 * @return RV_OK 获取成功
 *         其他   获取失败
 */
EXTERNC retval_e alipay_get_sn(PARAM_OUT uint8_t* buf_sn,PARAM_INOUT uint32_t *len_sn);

/*获取设备imei，长度不超过48个字符，只能包含大小写字母、数字、下划线
 *要求内容以‘\0’结尾且长度不包含'\0'。
 *若没有imei，可以用sn作为imei
 * @param [out] buf_imei 存放数据的缓存
 * @param [inout] len_imei  入参为允许读取的最大长度
 *                         出参为实际读取的数据长度
 * @return RV_OK 获取成功
 *         其他   获取失败
 */
EXTERNC retval_e alipay_get_imei(PARAM_OUT uint8_t* buf_imei,PARAM_INOUT uint32_t *len_imei);

/*获取设备类型，根据设备实际情况返回
 * @param 
 * @return 0    成人手表
 *         1    成人手环
 *         2    成人卡片
 *         3    儿童手表
 *         4    儿童手环
 *         5    学生卡
 */
EXTERNC uint32_t alipay_get_productType(void);

/*获取公司名称，公司名称为字符串格式并且字母要求全部大写，长度要求不超过15个字节(不包含结束符)，
 *如果公司全称不超过15个字节，则使用全称比如 "HUAWEI" "GARMIN"；如果超过15个字节，请优先使用中文拼音
 *则第一个字的拼音为完整，后续为拼音首字母的缩写，比如 xiaotiancai应写为 "XIAOTC"；若一定要使用英文
 *的公司名称，请与alipay同学协商。
 * @param [in] buffer  存放公司名称的缓存 
 * @param [in out] len 输入为允许读取的最大长度；输出时为实际读取的长度，长度不包含结束符
 * @return void
 */
EXTERNC retval_e alipay_get_companyName(PARAM_OUT uint8_t* buffer,PARAM_INOUT uint32_t* len);

/*获取产品型号，产品型号与蓝牙中显示的名称相同，长度要求不超过23个字节(不包含结束符)
 * @param [in] buffer  存放产品型号的缓存 
 * @param [in out] len 输入为允许读取的最大长度；输出时为实际读取的长度，长度不包含结束符
 * @return void
 */
EXTERNC retval_e alipay_get_productModel(PARAM_OUT uint8_t* buffer,PARAM_INOUT uint32_t* len);


#endif