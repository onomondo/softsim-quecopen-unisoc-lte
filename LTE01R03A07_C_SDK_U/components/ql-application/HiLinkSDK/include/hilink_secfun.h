/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: HiLink安全函数，厂家不用实现，直接调用
 */
#ifndef HILINK_SECFUN_H
#define HILINK_SECFUN_H

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/*
 * 设置目的缓冲区为特定值
 * 返回0表示成功, 非0表示失败
 * 注意：1）确保count <= dest所指向的内存区域大小。
 *       2）这个接口是已经提供给三方厂商使用安全函数接口，
 *       已经在软工代码TMG评审过，为了版本兼容，可以继续提供。
 */
int hilink_memset_s(void *dest, unsigned int destMax, int c, unsigned int count);

/*
 * 复制源缓冲区的数据到目的缓冲区
 * 返回0表示成功, 非0表示失败
 * 注意：1）确保count <= destMax所指向的内存区域大小
 *       2）确保dest和src指向的内存没有重叠区域
 *       3）这个接口是已经提供给三方厂商使用安全函数接口，
 *       已经在软工代码TMG评审过，为了版本兼容，可以继续提供。
 */
int hilink_memcpy_s(void *dest, unsigned int destMax, const void *src, unsigned int count);

/*
 * 复制指定长度的源字符串到目的缓冲区
 * 返回0表示成功, 非0表示失败
 * 注意：1）确保strDest所指内存空间足够容纳拷贝的字符内容
 *       2）这个接口是已经提供给三方厂商使用安全函数接口，
 *       已经在软工代码TMG评审过，为了版本兼容，可以继续提供。
 */
int hilink_strncpy_s(char *strDest, unsigned int destMax, const char *strSrc, unsigned int count);

/*
 * 将指定长度的源字符串连接到目的字符串后面
 * 返回0表示成功, 非0表示失败
 * 注意：1）确保strDest所指内存空间足够容纳strDest原有字符及追加的strSrc字符内容
 *       2）这个接口是已经提供给三方厂商使用安全函数接口，
 *       已经在软工代码TMG评审过，为了版本兼容，可以继续提供。
 */
int hilink_strncat_s(char *strDest, unsigned int destMax, const char *strSrc, unsigned int count);

/*
 * 将数据格式化输出到目的缓冲区
 * 返回-1表示失败, 其他表示实际写入strDest的字节数
 * 注意：1）strDest要有足够的空间容纳格式化的字符内容
 *       2）这个接口是已经提供给三方厂商使用安全函数接口，
 *       已经在软工代码TMG评审过，为了版本兼容，可以继续提供。
 */
int hilink_sprintf_s(char *strDest, unsigned int destMax, const char *format, ...);

/*
 * 将数据按照指定长度格式化输出到目的缓冲区
 * 返回-1表示失败, 其他表示欲写入strDest的字节数
 * 注意：这个接口是已经提供给三方厂商使用安全函数接口，
 *       已经在软工代码TMG评审过，为了版本兼容，可以继续提供。
 */
int hilink_snprintf_s(char *strDest, unsigned int destMax, unsigned int count, const char *format, ...);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif