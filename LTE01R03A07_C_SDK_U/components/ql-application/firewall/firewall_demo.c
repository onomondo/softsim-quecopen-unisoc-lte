#include "ql_firewall.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ql_api_osi.h"
#include "ql_api_nw.h"
#include "ql_log.h"

#define QL_SOCKET_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_FIREWALL_LOG(msg, ...) QL_LOG(QL_SOCKET_LOG_LEVEL, "firewall ", msg, ##__VA_ARGS__)


void ql_firewall_app_init(void)
{
#ifdef QL_APP_FEATURE_FRIEWALL
    int value = 0;
    ql_firewall_enable(1); //开启IDS检测功能
    value = ql_firewall_get_config(QL_FIREWALL_ENABLE);
    QL_FIREWALL_LOG("firewall enable=%d",value);
#endif
}
