/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------
Note: Don't delete BT firmware from the file system
=================================================================*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_api_bt.h"

#include "ql_log.h"


#define QL_BT_LOG_LEVEL         	QL_LOG_LEVEL_INFO
#define QL_BT_LOG(msg, ...)			QL_LOG(QL_BT_LOG_LEVEL, "ql_bt_DEMO", msg, ##__VA_ARGS__)
#define QL_BT_LOG_PUSH(msg, ...)	QL_LOG_PUSH("ql_bt_DEMO", msg, ##__VA_ARGS__)



ql_bt_visible_mode_e scan_mode = QL_BT_PAGE_AND_INQUIRY_ENABLE;
ql_bt_ble_local_name_s bt_local_name = 
{
    .name = "移远BT",
    .code_type = QL_BT_BLE_NAME_CODE_UTF8,
};



ql_errcode_bt_e ql_bt_demo_get_state()
{
    ql_errcode_bt_e ret;
    ql_bt_state_e bt_state;
    
    ret = ql_bt_get_state(&bt_state);
    if (ret != QL_BT_SUCCESS)
    {
        QL_BT_LOG("error=%x", ret);
    }
    else 
    {
        QL_BT_LOG("bt_state=%d",(int)bt_state);
    }

    return ret;
}

ql_errcode_bt_e ql_bt_demo_start()
{
    ql_errcode_bt_e ret;
    
    ret = ql_bt_start();
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_LOG("sucess");
    }
    else if (ret == QL_BT_ALREADY_STARTED_ERR)
    {
        QL_BT_LOG("already started");
    }
    else
    {
        QL_BT_LOG("error=%x", ret);
    }

    return ret;
}

ql_errcode_bt_e ql_bt_demo_get_scanmde()
{
    ql_errcode_bt_e ret;
    ql_bt_visible_mode_e scan_mode;
    
    ret = ql_classic_bt_get_scanmode(&scan_mode);
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_LOG("scan_mode=%d",(int)scan_mode); 
    }
    else 
    {
        QL_BT_LOG("error=%x", ret);
    } 

    return ret;
}

ql_errcode_bt_e ql_be_demo_set_scanmode()
{
    ql_errcode_bt_e ret;
    
    ret = ql_classic_bt_set_scanmode(scan_mode);
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_LOG("sucess");
    }
    else 
    {
        QL_BT_LOG("error=%x", ret);
    }

    return ret;
}

ql_errcode_bt_e ql_bt_demo_get_local_name()
{
    ql_errcode_bt_e ret;
    ql_bt_ble_local_name_s bt_local_name;
    
    memset(bt_local_name.name,0x00,sizeof(bt_local_name.name));
    ret = ql_bt_ble_get_localname(&bt_local_name);
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_LOG("bt_local_name:%s",bt_local_name.name);
    }
    else 
    {
        QL_BT_LOG("error=%x", ret);
    }

    return ret;
}

ql_errcode_bt_e ql_bt_demo_set_local_name()
{
    ql_errcode_bt_e ret;
    
    ret = ql_bt_ble_set_localname(bt_local_name);
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_LOG("sucess");
    }
    else 
    {
        QL_BT_LOG("error=%x", ret);
    }

    return ret;
}

ql_errcode_bt_e ql_bt_demo_stop()
{
    ql_errcode_bt_e ret;
    
    ret = ql_bt_stop();
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_LOG("sucess");
    }
    else 
    {
        QL_BT_LOG("error=%x", ret);
    }

    return ret;
}

