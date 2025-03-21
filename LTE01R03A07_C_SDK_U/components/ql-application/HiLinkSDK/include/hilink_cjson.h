/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: json头文件，厂家不用实现，直接调用
 */
#ifndef HILINK_CJSON_H
#define HILINK_CJSON_H

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/*
 * 将字符串形式的json数据解析为json结构体类型的数据
 * 返回NULL-解析失败,非NULL-解析成功，返回值为json结构体指针
 */
void *hilink_json_parse(const char *value);

/*
 * 获取json结构体中的字符串value
 * 返回NULL-获取失败,非NULL-获取成功，返回值为字符串value首地址
 */
char *hilink_json_get_string_value(const void *object, const char *name, unsigned int *len);

/*
 * 获取json结构体中的数值value
 * 返回0-成功,-1-失败
 */
int hilink_json_get_number_value(const void *object, const char *name, int *value);

/*
 * 获取json结构体中的object
 * 返回NULL-解析失败,非NULL-解析成功，返回值为json object结构体指针
 */
void *hilink_json_get_object(const void *object, const char *name);

/*
 * 获取json结构体中的array元组
 * 返回NULL-解析失败, 非NULL-解析成功，返回值为json object结构体指针
 */
void *hilink_json_get_arrayitem(const void *array, int item);

/*
 * 获取json结构体中的array的大小
 * 返回元组数量
 */
int hilink_json_get_arraysize(const void *array);

/* 释放json结构体所占用的空间 */
void hilink_json_delete(void *object);

/* json对象内字符串数据清0 */
void hilink_json_clear_all_string(void *object);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif