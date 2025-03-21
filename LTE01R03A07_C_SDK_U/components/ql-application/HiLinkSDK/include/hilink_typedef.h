/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: 类型定义头文件
 */
#ifndef HILINK_TYPEDEF_H
#define HILINK_TYPEDEF_H

#ifdef __cplusplus
extern "C" {
#endif

#define HILINK_IF_OPER(condition, operation) do { \
    if ((condition)) { \
        operation; \
    } \
} while (0)

#ifndef bool
#define bool int
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifdef __cplusplus
}
#endif

#endif /* HILINK_TYPEDEF_H */
