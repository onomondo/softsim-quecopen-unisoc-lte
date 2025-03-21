/*================================================================
  Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
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
#include "ble_hid_demo.h"
#include "ql_api_ble_gatt.h"
#include "ql_power.h"
#include "ql_api_dev.h"

#define QL_BLE_HID_LOG_LEVEL         	            QL_LOG_LEVEL_INFO
#define QL_BLE_HID_LOG(msg, ...)	                QL_LOG(QL_BLE_HID_LOG_LEVEL, "ql_BLE_HID_DEMO", msg, ##__VA_ARGS__)
#define QL_BLE_HID_LOG_PUSH(msg, ...)	            QL_LOG_PUSH("ql_BLE_HID_DEMO", msg, ##__VA_ARGS__)

#define BLE_CHECK(err, lable, str)					\
			do															\
			{															\
				if(0 != (err))											\
				{														\
					QL_BLE_HID_LOG(str);								\
					goto lable; 										\
				}														\
			}while(0)


#define HID_MOUSE_OR_KEYBRD                 1       //1:mouse 0:keybrd
#define HID_INPUT_HANDLE                    20

extern ql_errcode_bt_e ql_bt_demo_start();
extern ql_errcode_bt_e ql_bt_demo_stop();
extern ql_errcode_bt_e ql_bt_demo_get_state();

ql_task_t ble_demo_task = NULL;
ql_bt_addr_s ble_connection_addr;
ql_ble_sys_service_mode_e ql_ble_gatt_sys_service = QL_RESERVED_SERVICE_KEEP; // 保留系统默认的GAP和GATT服务,建议不删除否则可能出现异常

#if HID_MOUSE_OR_KEYBRD
    uint8_t g_hid_report[3] = {0};
#else
    uint8_t g_hid_report[8] = {0};
#endif
uint8_t g_rpt_01_in_ref[2] = {0x00, HID_REPORT_TYPE_INPUT};
uint8_t g_rpt_01_out_ref[2] = {0x00, HID_REPORT_TYPE_OUTPUT};
uint8_t g_rpt_01_feature_ref[2] = {0x00, HID_REPORT_TYPE_FEATURE};
uint8_t g_rpt_client_cfg[2] = {0x03, 0x00};

uint8_t g_hid_protocol_mode = HID_PROTOCOL_MODE_REPORT;
uint8_t g_hid_ctl = 1;
uint8_t g_hid_key_input = 0;
uint8_t g_hid_key_output = 0;
uint8_t g_hid_mouse_input = 0;

static const uint8_t g_hid_information[] =
{
    0x00,
    0x00,        /**<HID version*/
    0x01,       /**< b_country_code*/
    0x00       /**< flags remote wake + normally connectable*/
};

ql_bt_ble_local_name_s ble_name = 
{
    .name = "QUEC_HID",
    .code_type = QL_BT_BLE_NAME_CODE_UTF8,
};

#if HID_MOUSE_OR_KEYBRD
static const uint8_t ql_hid_report_map[] =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
};
#else
static const uint8_t ql_hid_report_map[] =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};
#endif

ql_ble_gatt_uuid_s hid_service_uuid = 
{
    .uuid_type = 1,
    .uuid_l = {0x00},
    .uuid_s = ATT_UUID_HIDS,
};

ql_ble_gatt_uuid_s hid_report_uuid[8] = 
{
    {
        .uuid_type = 1,
        .uuid_l = {0x00},
        .uuid_s = ATT_UUID_HID_PROTOCOL_MODE,
    },
    {
        .uuid_type = 1,
        .uuid_l = {0x00},
        .uuid_s = ATT_UUID_HID_REPORT,
    },
    {
        .uuid_type = 1,
        .uuid_l = {0x00},
        .uuid_s = ATT_UUID_HID_REPORT_MAP,
    },
    {
        .uuid_type = 1,
        .uuid_l = {0x00},
        .uuid_s = ATT_UUID_HID_KEY_INPUT,
    },
    {
        .uuid_type = 1,
        .uuid_l = {0x00},
        .uuid_s = ATT_UUID_HID_KEY_OUTPUT,
    },
    {
        .uuid_type = 1,
        .uuid_l = {0x00},
        .uuid_s = ATT_UUID_HID_MOUSE_INPUT,
    },
    {
        .uuid_type = 1,
        .uuid_l = {0x00},
        .uuid_s = ATT_UUID_HID_INFORMATION,
    },
    {
        .uuid_type = 1,
        .uuid_l = {0x00},
        .uuid_s = ATT_UUID_HID_CONTROL,
    }
};

ql_ble_gatt_uuid_s att_client_uuid = 
{
    .uuid_type = 1,
    .uuid_l = {0x00},
    .uuid_s = ATT_UUID_CLIENT,
};

ql_ble_gatt_uuid_s att_report_uuid = 
{
    .uuid_type = 1,
    .uuid_l = {0x00},
    .uuid_s = ATT_UUID_REPORT_RE,
};

ql_bleadv_set_data_s ble_adv_data = 
{
    .date_len = QL_BLE_HID_ADV_DATA_ORIGAL_LEN,
#if HID_MOUSE_OR_KEYBRD
    .data = {0x02,0x01,0x05,0x03,0x02,0x18,0x12,0x03,0x19,(unsigned char)(HID_DEVICE_MOUSE_ID&0xff),(unsigned char)(HID_DEVICE_MOUSE_ID>>8)},
#else
    .data = {0x02,0x01,0x05,0x03,0x02,0x18,0x12,0x03,0x19,(unsigned char)(HID_DEVICE_KEYBOARD_ID&0xff),(unsigned char)(HID_DEVICE_KEYBOARD_ID>>8)},
#endif
};

ql_errcode_bt_e ql_ble_demo_get_state()
{
    ql_errcode_bt_e ret;
    ql_bt_state_e bt_state;
    
    ret = ql_bt_get_state(&bt_state);
    if (ret != QL_BT_SUCCESS)
    {
        QL_BLE_HID_LOG("error=%x", ret);
    }
    else 
    {
        QL_BLE_HID_LOG("bt_state=%d",(int)bt_state);
    }

    return ret;
}

ql_errcode_bt_e ql_ble_demo_get_public_addr()
{
    ql_errcode_bt_e ret;
    ql_bt_addr_s bt_public_addr;
    
    ret = ql_ble_get_public_addr(&bt_public_addr);
    if (ret == QL_BT_SUCCESS)
    {
        unsigned char addr_string[QL_BLE_HID_DEMO_ADDR_MAX_SIZE + 1] = {0};

        memset(addr_string,0x00,sizeof(addr_string));
        sprintf((char *)addr_string, "%02x:%02x:%02x:%02x:%02x:%02x", bt_public_addr.addr[0], bt_public_addr.addr[1], bt_public_addr.addr[2], bt_public_addr.addr[3], bt_public_addr.addr[4], bt_public_addr.addr[5]);
        QL_BLE_HID_LOG("addr_string=%s",addr_string);
    }
    else 
    {
        QL_BLE_HID_LOG("error=%x", ret);
    }

    return ret;
}

ql_errcode_bt_e ql_ble_demo_get_version()
{
    ql_errcode_bt_e ret;
    char version[QL_BLE_VERSION_SIZE];
    
    ret = ql_ble_get_version(version,sizeof(version));
    if (ret == QL_BT_SUCCESS)
    {
        QL_BLE_HID_LOG("version=%s", version);
    }
    else 
    {
        QL_BLE_HID_LOG("error=%x", ret);
    }

    return ret;
}

ql_errcode_bt_e ql_ble_demo_get_random_addr()
{
    ql_errcode_bt_e ret;
    ql_bt_addr_s bt_random_addr;
    
    ret = ql_ble_get_random_addr(&bt_random_addr);
    if (ret == QL_BT_SUCCESS)
    {
        unsigned char addr_string[QL_BLE_HID_DEMO_ADDR_MAX_SIZE + 1] = {0};
       
        memset(addr_string,0x00,sizeof(addr_string));
        sprintf((char *)addr_string, "%02x:%02x:%02x:%02x:%02x:%02x", bt_random_addr.addr[0], bt_random_addr.addr[1], bt_random_addr.addr[2], bt_random_addr.addr[3], bt_random_addr.addr[4], bt_random_addr.addr[5]);
        QL_BLE_HID_LOG("addr_string=%s",addr_string); 
    }
    else 
    {
        QL_BLE_HID_LOG("error=%x", ret);
    }

    return ret;
}

ql_errcode_bt_e ql_ble_demo_get_name()
{
    ql_errcode_bt_e ret;
    ql_bt_ble_local_name_s bt_name;
    
    ret = ql_bt_ble_get_localname(&bt_name);
    if (ret == QL_BT_SUCCESS)
    {
        QL_BLE_HID_LOG("bt_name.name=%s",bt_name.name);
    }
    else 
    {
        QL_BLE_HID_LOG("error=%x", ret);
    }

    return ret;
}

ql_errcode_bt_e ql_ble_demo_set_name()
{
    ql_errcode_bt_e ret;
    
    ret = ql_bt_ble_set_localname(ble_name);
    if (ret == QL_BT_SUCCESS)
    {
        QL_BLE_HID_LOG("sucess");
    }
    else 
    {
        QL_BLE_HID_LOG("error=%x", ret);
    }

    return ret;
}

ql_errcode_bt_e ql_bleadv_demo_set_data()
{
    ql_errcode_bt_e ret = QL_BT_SUCCESS;
    unsigned char init_len = QL_BLE_HID_ADV_DATA_ORIGAL_LEN;//ble_adv_data.date_len;
    unsigned char length = strlen((char *)ble_name.name);
    
    ble_adv_data.date_len = init_len+length+2;
    ble_adv_data.data[init_len] = length+1;
    ble_adv_data.data[init_len+1] = 0x09;
    memcpy(&ble_adv_data.data[init_len+2], ble_name.name, length);

    ret = ql_bleadv_set_data(ble_adv_data);
    if (ret == QL_BT_SUCCESS)
    {
        QL_BLE_HID_LOG("sucess");
    }
    else 
    {
        QL_BLE_HID_LOG("error=%x", ret);
    }

    return ret;
}

ql_errcode_bt_e ql_ble_demo_send_data(uint16_t handle, uint8_t *value)
{
    return ql_ble_send_notification_data(0, handle, sizeof(g_hid_report), value);
}

#if HID_MOUSE_OR_KEYBRD
ql_errcode_bt_e ql_hid_mouse_control(uint8_t key, uint8_t x, uint8_t y)
{
    g_hid_report[0] = key;
    g_hid_report[1] = x;
    g_hid_report[2] = y;
    return ql_ble_demo_send_data(HID_INPUT_HANDLE, g_hid_report);
}
#else
ql_errcode_bt_e ql_hid_keybrd_key(uint8_t *key_map)
{
    memset(g_hid_report, 0x00, sizeof(g_hid_report));
    memcpy(g_hid_report, key_map, sizeof(g_hid_report));
    return ql_ble_demo_send_data(HID_INPUT_HANDLE, g_hid_report);
}
#endif

static ql_errcode_bt_e ql_bleadv_demo_start()
{
    ql_errcode_bt_e ret;
    
    ret = ql_bleadv_start();
    if (ret == QL_BT_SUCCESS)
    {
        QL_BLE_HID_LOG("sucess");
    }
    else 
    {
        QL_BLE_HID_LOG("error=%x", ret);
    }

    return ret;
}

static void ql_ble_notify_cb(void *ind_msg_buf, void *ctx)
{
    ql_event_t *event_temp = NULL;
    ql_event_t test_event = {0};
    
    if (ind_msg_buf == NULL)
    {
        return ;
        
    }

    event_temp = (ql_event_t *)ind_msg_buf;
    
    switch (event_temp->id)
    {
        case QUEC_BLE_CONNECT_IND:
        case QUEC_BLE_DISCONNECT_IND:
        {
            ql_bt_addr_s * temp = (ql_bt_addr_s *)event_temp->param2;
            ql_bt_addr_s *ql_addr = (ql_bt_addr_s *)calloc(1,sizeof(ql_bt_addr_s));
            if (ql_addr)
            {
                memcpy(ql_addr->addr, temp->addr, sizeof(ql_bt_addr_s));
                
                test_event.id = event_temp->id;
                test_event.param2 = (uint32)ql_addr;
                test_event.param3 = event_temp->param3;
            }            
        }
        break;
        case QUEC_BLE_UPDATE_CONN_PARAM_IND:
        {
            ql_ble_update_conn_infos_s *temp = (ql_ble_update_conn_infos_s *)event_temp->param2;
            
            ql_ble_update_conn_infos_s *ql_conn_param = (ql_ble_update_conn_infos_s *)malloc(sizeof(ql_ble_update_conn_infos_s));
            if (ql_conn_param)
            {
                ql_conn_param->conn_id = temp->conn_id;
                ql_conn_param->min_interval = temp->min_interval;
                ql_conn_param->max_interval = temp->max_interval;
                ql_conn_param->latency = temp->latency;
                ql_conn_param->timeout = temp->timeout;

                test_event.id = event_temp->id;
                test_event.param2 = (uint32)ql_conn_param;

            }            
        }
        break;
        case QUEC_BLE_GATT_RECV_IND:
        case QUEC_BLE_GATT_RECV_READ_IND:
        {
            ql_ble_gatt_data_s *temp = (ql_ble_gatt_data_s *)event_temp->param2;         
            ql_ble_gatt_data_s *gatt = (ql_ble_gatt_data_s *)malloc(sizeof(ql_ble_gatt_data_s));
            if (gatt)
            {
                memset(gatt, 0x00, sizeof(ql_ble_gatt_data_s));
                gatt->data = malloc(temp->len);
                if (gatt->data == NULL)
                {
                    free(gatt);
                    return;
                }
                
                gatt->len = temp->len;
                gatt->att_handle = temp->att_handle;
           
                memcpy(gatt->data, temp->data, temp->len);
                gatt->uuid_s = temp->uuid_s;
         
                test_event.id = event_temp->id;
                test_event.param2 = (uint32)gatt;
            }
        }
        break;
        case QUEC_BLESCAN_REPORT_IND:
        {
            ql_ble_scan_report_info_s *temp = (ql_ble_scan_report_info_s *)event_temp->param2;
            
            ql_ble_scan_report_info_s *ql_scan_report = malloc(sizeof(ql_ble_scan_report_info_s));
            
            if (ql_scan_report)
            {
                memset(ql_scan_report, 0x00, sizeof(ql_ble_scan_report_info_s));
                
                ql_scan_report->name_length = temp->name_length;
                memcpy(ql_scan_report->name, temp->name, temp->name_length);      
                ql_scan_report->addr_type = temp->addr_type;
                memcpy(ql_scan_report->addr.addr, temp->addr.addr, QL_BT_MAC_ADDRESS_SIZE);
                ql_scan_report->event_type = temp->event_type;
                ql_scan_report->data_length = temp->data_length;
                ql_scan_report->rssi = temp->rssi;
                memcpy(ql_scan_report->raw_data, temp->raw_data, temp->data_length);
                 
                test_event.id = event_temp->id;
                test_event.param2 = (uint32)ql_scan_report;

            }        
        }
        break;
        case QUEC_BLE_GATT_DISCOVER_SERVICE_IND:
        {
            ql_blegatt_prime_service_s *temp = (ql_blegatt_prime_service_s *)event_temp->param2;
            
            ql_blegatt_prime_service_s *ql_param = (ql_blegatt_prime_service_s *)malloc(sizeof(ql_blegatt_prime_service_s));
            if (ql_param)
            {
                memcpy(ql_param, temp, sizeof(ql_blegatt_prime_service_s));
  
                test_event.id = event_temp->id;
                test_event.param2 = (uint32)ql_param;
    
            }

        }
        break; 
        case QUEC_BLE_GATT_DISCOVER_CHARACTERISTIC_DATA_IND:
        case QUEC_BLE_GATT_CHARA_READ_BY_UUID_IND:
        case QUEC_BLE_GATT_DISCOVER_CHARA_DESC_IND:
        case QUEC_BLE_GATT_CHARA_READ_IND:
        case QUEC_BLE_GATT_DESC_READ_IND:
        case QUEC_BLE_GATT_CHARA_MULTI_READ_IND:
        case QUEC_BLE_GATT_RECV_NOTIFICATION_IND:
        case QUEC_BLE_GATT_RECV_INDICATION_IND:
        {
            ql_att_general_rsp_s * temp = (ql_att_general_rsp_s *)event_temp->param2;
            ql_att_general_rsp_s *ql_server = malloc(sizeof(ql_att_general_rsp_s));
            if (ql_server)
            {
                ql_server->pay_load = malloc(temp->length);
                if (ql_server->pay_load == NULL)
                {
                    free(ql_server);
                    return;
                }
                
                ql_server->length = temp->length;
                memcpy(ql_server->pay_load, temp->pay_load, ql_server->length);
                
                test_event.id = event_temp->id;
                test_event.param2 = (uint32)ql_server;
            }            
        }
        break;
        default:
        {
                test_event.id = event_temp->id;
                test_event.param1 = event_temp->param1;
                test_event.param2 = event_temp->param2;
                test_event.param3 = event_temp->param3;
        }
        break;
    }

    if (test_event.id != 0)
    {
        ql_rtos_event_send(ble_demo_task,&test_event);
    }
}

/************************************************************************
* QUEC_BLE_CONNECT_IND
* QUEC_BLE_DISCONNECT_IND
* QUEC_BLE_UPDATE_CONN_PARAM_IND
* QUEC_BLE_GATT_RECV_IND
* QUEC_BLE_GATT_RECV_READ_IND
************************************************************************/
ql_errcode_bt_e ql_ble_gatt_server_handle_event()
{
    ql_event_t test_event;
    ql_errcode_bt_e ret = QL_BT_SUCCESS;
    QL_BT_STATUS status = 0;
    
    if (ql_event_try_wait(&test_event) == 0)
	{
        if(test_event.id == 0)
        {
            return ret;
        }
        
        status = (QL_BT_STATUS)(test_event.param1);
        switch (test_event.id)
        {
            case QUEC_BT_START_STATUS_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BLE_HID_LOG("start sucess");
                    
                    ret = ql_ble_demo_get_state();
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "get state err");
#if 1
                    ret = ql_ble_demo_get_public_addr();
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "get public addr");

                    ret = ql_ble_demo_get_version();
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "set adv data err");

                    ret = ql_ble_demo_get_random_addr();
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "set adv data err");

                    ret = ql_ble_demo_set_name();
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "set adv data err");

                    ret = ql_ble_demo_get_name();
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "set adv data err");
#endif
                    ret = ql_bleadv_demo_set_data();
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "set adv data err");
                    
                    //add HID service
                    ret = ql_ble_gatt_add_service(0x01, hid_service_uuid, 1);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add ser err");

                    //PROTOCOL_MODE
                    ret = ql_ble_gatt_add_chara(0x01, 0x01, QL_ATT_CHARA_PROP_READ | QL_ATT_PM_WRITEABLE | QL_ATT_CHARA_PROP_WWP, hid_report_uuid[0]);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char err");
                    ret = ql_ble_gatt_add_chara_value(0x01, 0x01, QL_ATT_PM_READABLE | QL_ATT_PM_WRITEABLE, hid_report_uuid[0], sizeof(g_hid_protocol_mode), (unsigned char *)&g_hid_protocol_mode);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char val err");
                    
                    // Bluetooth HID devices支持三种Report：Input, Output, Feature。uuid:0x2A4D
                    // Input report
                    ret = ql_ble_gatt_add_chara(0x01, 0x02, QL_ATT_CHARA_PROP_READ | QL_ATT_CHARA_PROP_WRITE | QL_ATT_CHARA_PROP_NOTIFY, hid_report_uuid[1]);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char err");
                    ret = ql_ble_gatt_add_chara_value(0x01, 0x02, QL_ATT_PM_READABLE | QL_ATT_PM_WRITEABLE, hid_report_uuid[1], sizeof(g_hid_report), (unsigned char *)g_hid_report);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char val err");
                    ret = ql_ble_gatt_add_chara_desc(0x01, 0x02, QL_ATT_PM_READABLE | QL_ATT_PM_WRITEABLE, att_client_uuid, sizeof(g_rpt_client_cfg), (unsigned char *)g_rpt_client_cfg);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char desc err");
                    ret = ql_ble_gatt_add_chara_desc(0x01, 0x02, QL_ATT_PM_READABLE, att_report_uuid, sizeof(g_rpt_01_in_ref), (unsigned char *)g_rpt_01_in_ref);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char desc err");

                    //Output report
                    ret = ql_ble_gatt_add_chara(0x01, 0x03, QL_ATT_CHARA_PROP_READ | QL_ATT_CHARA_PROP_WRITE | QL_ATT_CHARA_PROP_WWP, hid_report_uuid[1]);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char err");
                    ret = ql_ble_gatt_add_chara_value(0x01, 0x03, QL_ATT_PM_READABLE | QL_ATT_PM_WRITEABLE, hid_report_uuid[1], sizeof(g_hid_report), (unsigned char *)g_hid_report);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char val err");
                    ret = ql_ble_gatt_add_chara_desc(0x01, 0x03, QL_ATT_PM_READABLE | QL_ATT_PM_WRITEABLE, att_client_uuid, sizeof(g_rpt_client_cfg), (unsigned char *)g_rpt_client_cfg);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char desc err");
                    ret = ql_ble_gatt_add_chara_desc(0x01, 0x03, QL_ATT_PM_READABLE, att_report_uuid, sizeof(g_rpt_01_out_ref), (unsigned char *)g_rpt_01_out_ref);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char desc err");

                    //Feature report
                    ret = ql_ble_gatt_add_chara(0x01, 0x04, QL_ATT_CHARA_PROP_READ | QL_ATT_CHARA_PROP_WRITE, hid_report_uuid[1]);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char err");
                    ret = ql_ble_gatt_add_chara_value(0x01, 0x04, QL_ATT_PM_READABLE | QL_ATT_PM_WRITEABLE, hid_report_uuid[1], sizeof(g_hid_report), (unsigned char *)g_hid_report);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char val err");
                    ret = ql_ble_gatt_add_chara_desc(0x01, 0x04, QL_ATT_PM_READABLE | QL_ATT_PM_WRITEABLE, att_client_uuid, sizeof(g_rpt_client_cfg), (unsigned char *)g_rpt_client_cfg);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char desc err");
                    ret = ql_ble_gatt_add_chara_desc(0x01, 0x04, QL_ATT_PM_READABLE, att_report_uuid, sizeof(g_rpt_01_feature_ref), (unsigned char *)g_rpt_01_feature_ref);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char desc err");

                    //report map
                    ret = ql_ble_gatt_add_chara(0x01, 0x05, QL_ATT_CHARA_PROP_READ, hid_report_uuid[2]);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char err");
                    ret = ql_ble_gatt_add_chara_value(0x01, 0x05, QL_ATT_PM_READABLE | QL_ATT_PM_R_ENCRYPTION_REQUIRED, hid_report_uuid[2], sizeof(ql_hid_report_map), (unsigned char *)ql_hid_report_map);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char val err");

                    //keyboard input report
                    ret = ql_ble_gatt_add_chara(0x01, 0x06, QL_ATT_CHARA_PROP_READ | QL_ATT_PM_WRITEABLE | QL_ATT_CHARA_PROP_NOTIFY, hid_report_uuid[3]);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char err");
                    ret = ql_ble_gatt_add_chara_value(0x01, 0x06, QL_ATT_PM_READABLE | QL_ATT_PM_WRITEABLE, hid_report_uuid[3], sizeof(g_hid_key_input), (unsigned char *)&g_hid_key_input);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char val err");
                    ret = ql_ble_gatt_add_chara_desc(0x01, 0x06, QL_ATT_PM_READABLE | QL_ATT_PM_WRITEABLE, att_client_uuid, sizeof(g_rpt_client_cfg), (unsigned char *)g_rpt_client_cfg);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char desc err");

                    //keyboard output report
                    ret = ql_ble_gatt_add_chara(0x01, 0x07, QL_ATT_CHARA_PROP_READ | QL_ATT_CHARA_PROP_WRITE | QL_ATT_CHARA_PROP_WWP, hid_report_uuid[4]);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char err");
                    ret = ql_ble_gatt_add_chara_value(0x01, 0x07, QL_ATT_PM_READABLE | QL_ATT_PM_WRITEABLE, hid_report_uuid[4], sizeof(g_hid_key_output), (unsigned char *)&g_hid_key_output);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char val err");

                    //mouse_input_char
                    ret = ql_ble_gatt_add_chara(0x01, 0x08, QL_ATT_CHARA_PROP_READ | QL_ATT_CHARA_PROP_WRITE, hid_report_uuid[5]);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char err");
                    ret = ql_ble_gatt_add_chara_value(0x01, 0x08, QL_ATT_PM_READABLE | QL_ATT_PM_WRITEABLE, hid_report_uuid[5], sizeof(g_hid_mouse_input), (unsigned char *)&g_hid_mouse_input);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char val err");
                    ret = ql_ble_gatt_add_chara_desc(0x01, 0x08, QL_ATT_PM_READABLE | QL_ATT_PM_WRITEABLE, att_client_uuid, sizeof(g_rpt_client_cfg), (unsigned char *)g_rpt_client_cfg);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char desc err");

                    //hid info char
                    ret = ql_ble_gatt_add_chara(0x01, 0x09, QL_ATT_CHARA_PROP_READ, hid_report_uuid[6]);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char err");
                    ret = ql_ble_gatt_add_chara_value(0x01, 0x09, QL_ATT_PM_READABLE, hid_report_uuid[6], sizeof(g_hid_information), (unsigned char *)g_hid_information);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char val err");

                    //hid control char
                    ret = ql_ble_gatt_add_chara(0x01, 0x0a, QL_ATT_CHARA_PROP_WRITE | QL_ATT_CHARA_PROP_WWP, hid_report_uuid[7]);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char err");
                    ret = ql_ble_gatt_add_chara_value(0x01, 0x0a, QL_ATT_PM_WRITEABLE, hid_report_uuid[7], sizeof(g_hid_ctl), (unsigned char *)&g_hid_ctl);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "add char val err");

                    ret = ql_ble_gatt_add_or_clear_service_complete(QL_BLE_SERVICE_ADD_COMPLETE, ql_ble_gatt_sys_service);
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "complete ser err");
                    
                    ret = ql_bleadv_demo_start();
                    BLE_CHECK(ret, QL_BLE_ADV_DEMO_STOP, "start adv err");
                }
                else
                {
                    QL_BLE_HID_LOG("start failed"); 
                }
            }
            break;
            case QUEC_BT_STOP_STATUS_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BLE_HID_LOG("stop sucess");
                    ret = QL_BT_ALREADY_STOPED_ERR;
                }
                else
                {
                    QL_BLE_HID_LOG("stop failed");
                    ret = QL_BT_ALREADY_STOPED_ERR;
                }
            }
            break;
            case QUEC_BT_BLE_RESET_IND:
            {
                QL_BLE_HID_LOG("bt reset");
                ret = QL_BT_ALREADY_STOPED_ERR;   
            }
            break;
            case QUEC_BLE_CONNECT_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BLE_HID_LOG("connect sucess");

                    ql_bt_addr_s * addr = (ql_bt_addr_s *)test_event.param2;
                    if (addr)
                    {
                        //addr need to free
                        unsigned char addr_string[QL_BLE_HID_DEMO_ADDR_MAX_SIZE + 1] = {0};
                        memset(addr_string,0x00,sizeof(addr_string));
                        sprintf((char *)addr_string, "%02x:%02x:%02x:%02x:%02x:%02x", addr->addr[0], addr->addr[1], addr->addr[2], addr->addr[3], addr->addr[4], addr->addr[5]);
                        QL_BLE_HID_LOG("addr_string=%s", addr_string); 
                        memcpy(ble_connection_addr.addr, addr->addr, QL_BT_MAC_ADDRESS_SIZE);
                        free(addr);
                    }
                    QL_BLE_HID_LOG("ble_conn_handle=%d", test_event.param3);
                }
                else
                {
                    QL_BLE_HID_LOG("connect failed");
                    goto QL_BLE_ADV_DEMO_STOP; 
                }
            }
            break;
            case QUEC_BLE_GATT_MTU:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BLE_HID_LOG("connect mtu sucess:handle=%d,mtu=%d", test_event.param2, test_event.param3);
                }
                else
                {
                    QL_BLE_HID_LOG("connect mtu failed");
                    goto QL_BLE_ADV_DEMO_STOP;
                }
            }
            break;
            case QUEC_BLE_DISCONNECT_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BLE_HID_LOG("disconnect sucess"); 
                    ql_bt_addr_s * addr = (ql_bt_addr_s *)test_event.param2;
                    if (addr)
                    {
                        unsigned char addr_string[QL_BLE_HID_DEMO_ADDR_MAX_SIZE + 1] = {0};
                        memset(addr_string,0x00,sizeof(addr_string));
                        sprintf((char *)addr_string, "%02x:%02x:%02x:%02x:%02x:%02x", addr->addr[0], addr->addr[1], addr->addr[2], addr->addr[3], addr->addr[4], addr->addr[5]);
                        QL_BLE_HID_LOG("addr_string=%s", addr_string); 
                        free(addr);

                    }
                }
                else
                {
                    QL_BLE_HID_LOG("disconnect failed");
                }
            }
            break;
            case QUEC_BLE_UPDATE_CONN_PARAM_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BLE_HID_LOG("update conn param sucess");
                    ql_ble_update_conn_infos_s *conn_param = (ql_ble_update_conn_infos_s *)test_event.param2;
                    if (conn_param)
                    {
                        QL_BLE_HID_LOG("conn_id=%d,min_interval=%d,max_interval=%d,latency=%d,timeout=%d", conn_param->conn_id, \
                            conn_param->min_interval, conn_param->max_interval, conn_param->latency, conn_param->timeout);
                        free(conn_param);
                    }
                }
                else
                {
                    QL_BLE_HID_LOG("update conn param failed");
                    goto QL_BLE_ADV_DEMO_STOP;
                }
            }
            break;
            case QUEC_BLE_GATT_SEND_END:
            {
                //send notification complete
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BLE_HID_LOG("send data sucess"); 
                }
                else
                {
                    QL_BLE_HID_LOG("send data failed");
                    goto QL_BLE_ADV_DEMO_STOP; 
                }
            }
            break;
            case QUEC_BLE_GATT_RECV_IND:
            {
                //client write characteristic
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BLE_HID_LOG("ble write sucess");
                    ql_ble_gatt_data_s *ble_data = (ql_ble_gatt_data_s *)test_event.param2;
                    if (ble_data && ble_data->data)
                    {
                        unsigned char * data=calloc(1,ble_data->len+1);
                        if(data)
                        {
                            memcpy(data,ble_data->data,ble_data->len);
                            QL_BLE_HID_LOG("ble_data->len=%d,data=%s", ble_data->len, data);
                            free(data);
                        }
                        free(ble_data->data);
                        free(ble_data);
                    }
                }
                else
                {
                    QL_BLE_HID_LOG("ble recv failed");
                    goto QL_BLE_ADV_DEMO_STOP;
                }
            }
            break;
            case QUEC_BLE_GATT_RECV_READ_IND:
            {
                //client read characterisc
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BLE_HID_LOG("ble read sucess");
                    ql_ble_gatt_data_s *ble_data = (ql_ble_gatt_data_s *)test_event.param2;
                    if (ble_data && ble_data->data)
                    {
                        unsigned char * data=calloc(1,ble_data->len+1);
                        if(data)
                        {
                            memcpy(data,ble_data->data,ble_data->len);
                            QL_BLE_HID_LOG("ble_data->len=%d,handle=%d,uuid=%4x", ble_data->len, ble_data->att_handle, ble_data->uuid_s);
                            free(data);
                        }
                        free(ble_data->data);
                        free(ble_data);
                    }
                }
                else
                {
                    QL_BLE_HID_LOG("ble recv failed");
                    goto QL_BLE_ADV_DEMO_STOP;
                }
            }
            break;
            default:
            break;
        }
        return ret;
QL_BLE_ADV_DEMO_STOP:
        ret = ql_bt_demo_stop();
	}

    return ret;
}

void ql_ble_hid_task_pthread(void *ctx)
{
    QlOSStatus err = 0;
    ql_errcode_bt_e ret = QL_BT_SUCCESS;

GATT_SERVER_RETRY:
    ret = ql_ble_gatt_server_init(ql_ble_notify_cb);
    if (ret != QL_BT_SUCCESS)
    {
        goto QL_BLT_GATT_SERVER_NOT_INIT_EXIT;
    }

    ret = ql_bt_demo_start();
    if (ret != QL_BT_SUCCESS)
    {
       goto QL_BLT_GATT_SERVER_INIT_EXIT;
    }

    while(1)
    {
        ret = ql_ble_gatt_server_handle_event();
        if (ret != QL_BT_SUCCESS)
        {
            break;
        }
    }

QL_BLT_GATT_SERVER_INIT_EXIT:
    ql_ble_gatt_server_release();
    QL_BLE_HID_LOG("gatt release");
    ql_rtos_task_sleep_s(5);
    goto GATT_SERVER_RETRY;
QL_BLT_GATT_SERVER_NOT_INIT_EXIT:
    err = ql_rtos_task_delete(NULL);
	if(err != QL_OSI_SUCCESS)
	{
		QL_BLE_HID_LOG("task deleted failed");
	}
}


QlOSStatus ql_ble_hid_demo_init(void)
{	
	QlOSStatus err = QL_OSI_SUCCESS;

    QL_BLE_HID_LOG("enter hid demo init");
	err = ql_rtos_task_create(&ble_demo_task, BT_BLE_HID_DEMO_TASK_STACK_SIZE, BT_BLE_HID_DEMO_TASK_PRIO, "gatt_hid", ql_ble_hid_task_pthread, NULL, BT_BLE_HID_DEMO_TASK_EVENT_CNT);
	
    return err;
}
