#ifndef __CH395_H__
#define __CH395_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "ql_api_ethernet.h"

typedef enum
{
    CH395_APP_CB_TYPE_RESET,
    CH395_APP_CB_TYPE_NOTIFY,
    CH395_APP_CB_TYPE_MAX
}ch395_app_cb_type_e;

typedef enum
{
    CH395_APP_NET_NONE,
    CH395_APP_NET_RESETING,
    CH395_APP_NET_CONNECTING,
    CH395_APP_NET_CONNECTED,
    CH395_APP_NET_DISCONNECTED,
    CH395_APP_NET_MAX
}ch395_app_net_status_e;

typedef void (*ch395_app_reset_cb_t)(void* ctx);
typedef void (*ch395_app_notify_cb_t)(void* ctx);
typedef void (*ch395_app_gpio_cb_t)(void* ctx);

ch395_app_gpio_cb_t ch395_app_get_gpio_cb(void);
void ch395_app_reset(void);

bool ch395_app_init(void* argv);
bool ch395_app_deinit(void* argv);
bool ch395_app_cb_register(ch395_app_cb_type_e type,void* cb);
#ifdef __cplusplus
}
#endif
#endif

/************************** endfile **************************************/
