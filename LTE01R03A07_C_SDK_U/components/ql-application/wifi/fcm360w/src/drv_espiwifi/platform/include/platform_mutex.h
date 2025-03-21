#ifndef __PLATFORM_MUTEX_H__
#define __PLATFORM_MUTEX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_wifi_adapter.h"

typedef struct platform_mutex {
    aos_mutex_t mutex;
} platform_mutex_t;

void platform_mutex_init(platform_mutex_t *m);
int platform_mutex_lock(platform_mutex_t *m);
int platform_mutex_trylock(platform_mutex_t *m);
void platform_mutex_unlock(platform_mutex_t *m);
void platform_mutex_destroy(platform_mutex_t *m);

#ifdef __cplusplus
}
#endif

#endif

