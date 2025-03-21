#ifndef ALIPAY_IOT_TYPE_H
#define ALIPAY_IOT_TYPE_H
#define __USE_TOOLCHAIN_INCLUDE__
#if defined(__USE_TOOLCHAIN_INCLUDE__)
#include <stdint.h>
#include <stddef.h>
#else
typedef signed char         int8_t;
typedef unsigned char       uint8_t;
typedef short               int16_t;
typedef unsigned short      uint16_t;
typedef int                 int32_t;
#ifndef uint32_t
typedef unsigned            uint32_t;
#endif
typedef long long           int64_t;
typedef unsigned long long  uint64_t;
typedef unsigned int        size_t;
typedef long                ssize_t;
#endif

#ifndef bool
typedef unsigned char       bool;
#endif
#ifndef false
#define false ((bool)0)
#define true  ((bool)1)
#endif

#ifndef NULL
#define NULL                0
#endif

#ifndef __func__
#ifdef __FUNC__
#define __func__ __FUNC__
#else
#define __func__ __FUNCTION__
#endif
#endif
#if defined(WIN32) && !defined(__cplusplus)
#define inline __inline
#endif

#define _EQ_ ==
#endif