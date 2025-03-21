#ifndef __ALIPAY_IOT_KV
#define __ALIPAY_IOT_KV

#include "alipay_iot_kal.h"

#define ALIPAY_KV_ITEM_MAX_SECTION_LEN 32 /* The max key length for key-value item */
#define ALIPAY_KV_ITEM_MAX_KEY_LEN 64 /* The max key length for key-value item */
#define ALIPAY_KV_ITEM_MAX_VAL_LEN 4096 /* The max value length for key-value item */


/*
* @brief: 指定sdk kv的block bits和block number
* @input: blk_bits 每个block的bit数位数, 12 means 4KB, 14 means 16KB, 15 means 32KB.建议不超过过15(32KB).
* @input: blk_num， kv中包含block的个数。建议不超过32。
* @return: true 设置成功。 false 设置失败。
* @note:  此函数只用于告知sdk, kv中每个block的bit数位数，以及block number。
    SDK将用这两个参数胡来初始化存储，若和实际情况不符合，会造成数据错乱，因此使用时需要格外注意。
    如果需要更改，请format sdk kv占用的flash后再启动sdk。
*/
bool alipay_iot_sdk_kv_config(uint8_t blk_bits, int32_t blk_num);



/*
* @brief: 打开block
* @input: block_name block名称
* @return: NULL 打开失败
*          非NULL  打开成功,返回指向block的句柄
* @note:  保证线程安全
*/
extern void *alipay_iot_sdk_kv_open(const char *block_name);

/*
* @brief: 获取某个block中指定key的value值
* @input: h block句柄
* @input: key key字符串
* @output: value 值的地址
* @output: len 返回的实际的value的长度
* @return: <0 失败
*           0 未找到key
*          =1  成功
* @note: 使用完毕后需要人为释放value指针，否则会内存泄漏
* @note: 如果value是字符串，不能保证以'\0'结尾，除非调用alipay_iot_kv_set的时候，将'\0'存入了
* @note: 保证线程安全
*/
extern int alipay_iot_sdk_kv_get(const void *h, const char *key, void **value, size_t *len);

/*
* @brief: 设置某个block中指定key的值为value
* @input: h block句柄
* @input: key key字符串
* @input: value 值的地址
* @input: len 要设置的value的长度
* @return: <0 失败
*          0 没有足够的空间存储
*          =1  成功
* @note:  保证线程安全
*/
extern int alipay_iot_sdk_kv_set(const void *h, const char *key, const void *value, size_t len);

/*
* @brief: 根据key删除某个block中指定的kv
* @input: h block句柄
* @input: key key字符串; 若key为NULL，则删除block对应的所有kv
* @return: <0 失败
*           0 未找到key
*          =1  成功
* @note:  保证线程安全
*/
extern int alipay_iot_sdk_kv_delete(const void *h, const char *key);

/*
* @brief: 关闭block
* @input: h block句柄
* @note:  保证线程安全
*/
extern void alipay_iot_sdk_kv_close(const void *handle);
#endif
