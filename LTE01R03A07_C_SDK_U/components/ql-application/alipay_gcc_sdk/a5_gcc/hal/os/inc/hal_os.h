#ifndef __HAL_OS_H__
#define __HAL_OS_H__

#include "hal_common.h"

hal_error_t hal_get_timestamp(uint32_t *tm);
hal_error_t hal_get_compile_timestamp(uint32_t *timestamp);


void hal_log_ext(const char *format, ...);


void *hal_malloc(uint32_t size);
void *hal_calloc(uint32_t nblock, uint32_t size);
void hal_free(void *pt);
void *hal_realloc(void *pt, uint32_t size);


hal_error_t hal_rng_get_data(void *random, uint32_t size); 

#endif
