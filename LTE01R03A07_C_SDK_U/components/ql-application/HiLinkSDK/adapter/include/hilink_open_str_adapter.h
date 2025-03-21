/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: 系统适配层字符串接口(需设备厂商实现)
 */
#ifndef HILINK_OPEN_STR_ADAPTER_H
#define HILINK_OPEN_STR_ADAPTER_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 用于计算字符串的长度，
 * 输入参数char  *src 待计算长度的字符串。
 * 返回值为字符串长度。
 */
unsigned int HILINK_Strlen(const char *src);

/*
 * 用于在字符串str中查找字符ch
 * 输入参数 const str为待查找字符串
 * 输入参数 ch待查找字符
 * 返回值NULL没有查找到字符，非NULL 指向ch的指针
 */
char *HILINK_Strchr(const char *str, int ch);

/*
 * 在字符串str中逆向查找字符ch
 * 输入参数str 字符串
 * 输入参数c待查找字符
 * 返回值NULL没有查找到字符，非NULL 指向ch的指针
 */
char *HILINK_Strrchr(const char *str, int c);

/*
 * 把字符串转换成int整形数字
 * 输入参数str 传入需要转换成 int 类型字符串
 * 把字符串转换成int整形数字
 */
int HILINK_Atoi(const char *str);

/*
 * 格式化输出字符串
 * 输入参数 format为格式，ap为可变参数
 */
int HILINK_Printf(const char *format, const va_list ap);

/*
 * 在str1中查找是否存在str2
 * 输入参数str1为被查找目标，str2要查找对象
 * 返回0 成功, 其他 失败
 */
char *HILINK_Strstr(char *str1, const char *str2);

/*
 * 比较两个字符串str1和str2
 * 输入参数str1目标串1
 * 输入参数str2目标串2
 * 输入参数len为比较的长度
 * 返回0 成功, 其他 失败
 */
int HILINK_Strncmp(const char *str1, const char *str2, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif
