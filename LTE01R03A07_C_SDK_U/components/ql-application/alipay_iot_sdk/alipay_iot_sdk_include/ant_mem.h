/**
 * ant_mem.h
 *
 * 内存管理接口
 */
#ifndef __ANT_MEM_H__
#define __ANT_MEM_H__

#include "ant_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 内存模块初始化
 * @return  0： 初始化成功
 *         -1： 初始化失败
 */
ant_s32 ant_malloc_init(void);
#ifdef __MEMLEAK_CHECK_BY_PLATFORM__
#define ant_malloc malloc
#define ant_free   free
#define ant_calloc calloc
#define ant_realloc realloc
#else
/**
 * 分配内存
 *
 * @param size 内存大小
 *
 * @return   非0: 成功，返回分配的内存指针
 *         NULL: 失败，返回空指针
 */
void *  ant_malloc(ant_u32 size);

/**
 * 分配内存块并清0
 *
 * @param n 内存块个数
 * @param size 内存块大小
 * @return   非0: 成功，返回分配的内存指针
 *         NULL: 失败，返回空指针
 */
void *  ant_calloc(ant_u32 n, ant_u32 size);

/**
 * 重新分配内存
 *
 * @param p 已分配内存指针
 * @param new_size 新内存大小
 *
 * @return   非0: 成功，返回分配的内存指针
 *         NULL: 失败，返回空指针
 */
void *  ant_realloc(void *p, ant_u32 new_size);

/**
 * 释放内存
 *
 * @param p 需要释放的内存指针
 */
void    ant_free(void *p);
#endif
/**
 * 分配对齐内存
 *
 * @param size 需分配内存的大小
 * @param align 对齐字节数，必须是4的倍数
 * @return   非0: 成功，返回分配的内存指针
 *         NULL: 失败，返回空指针
 */
void *  ant_malloc_align(ant_u32 size, ant_u32 align);

/**
 * 获取内存状态
 *
 * @param total 返回总内存大小
 * @param avail 返回可用内存大小
 * @param max_block_size 返回可分配最大内存块大小
 *
 * @return 0 : 获取成功
 *        -1 : 获取失败
 */
ant_s32 ant_mem_get_stats(ant_u32 *total, ant_u32 *avail, ant_u32 *max_block_size);

/**
 * 返回内存是否存在泄漏
 *
 *
 * @return ANT_TRUE  : 存在内存泄漏
 *         ANT_FALSE : 不存在内存泄漏
 */
ant_bool ant_mem_is_leak(void);


#define ALLOC_TYPE(p, type) type *p = (type *)ant_malloc(sizeof(type)); memset(p, 0, sizeof(type))
#define ALLOC_TYPE_ARRAY(p, type, n) type *p = (type *)ant_calloc(sizeof(type), (n))

#ifdef __cplusplus
}
#endif

#endif

