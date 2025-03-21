/**
 * ant_kv.h
 *
 * KV存储访问接口
 */
#ifndef __ANT_KV_H__
#define __ANT_KV_H__

#include "ant_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif
    
typedef void* kv_handle;

/**
 * kv存储初始化
 */
void      ant_kv_init(void);

/**
 * 打开KV存储
 *
 * @return 非0值： 打开成功，返回kv访问句柄
 *           -1： 打开失败
 */
kv_handle ant_kv_open(void);

/**
 * 获取kv中的字符串值
 * @param h kv访问句柄
 * @param key 键值
 * @param value 字符串缓存，返回键值对应的字符串
 * @param len 字符串缓存长度
 * 
 * @return  0： 获取成功
 *         -1： 获取失败
 */
ant_s32   ant_kv_get_string(void *h, const char *key, char *value, ant_u32 *len);

/**
 * 设置kv中的字符串值
 * @param h kv访问句柄
 * @param key 键值
 * @param value 字符串缓存，键值对应的字符串值
  * 
 * @return  0： 设置成功
 *         -1： 设置失败
 */
ant_s32   ant_kv_set_string(void *h, const char *key, char *value);

/**
 * 获取kv中的整型值
 * @param h kv访问句柄
 * @param key 键值
 * @param value 整型缓存，返回键值对应的整型数据
  * 
 * @return  0： 获取成功
 *         -1： 获取失败
 */
ant_s32   ant_kv_get_u32(void *h, const char *key, ant_u32 *value);

/**
 * 设置kv中的整型值
 * @param h kv访问句柄
 * @param key 键值
 * @param value 整型缓存，键值对应的整型值
  * 
 * @return  0： 设置成功
 *         -1： 设置失败
 */
ant_s32   ant_kv_set_u32(void *h, const char *key, ant_u32 value);

/**
 * 删除kv中的键值对
 * 
 * @param h kv访问句柄 
 * @param key 键值
 * 
 * @return  0： 删除成功
 *         -1： 删除失败 
 */
ant_s32   ant_kv_erase(void *h, const char *key);

/**
 * 关闭kv句柄
 * 
 * @param h kv访问句柄 
 */
void      ant_kv_close(kv_handle h);

void ant_iot_kv_init(void);
void *ant_iot_kv_open(const char *block_name);
int ant_iot_kv_get(const void *h, const char *key, void **value, size_t *len);
int ant_iot_kv_set(const void *h, const char *key, const void *value, size_t len);
int ant_iot_kv_delete(const void *h, const char *key);
void ant_iot_kv_close(const void *handle);
#ifdef __cplusplus
} // extern "C"
#endif

#endif

