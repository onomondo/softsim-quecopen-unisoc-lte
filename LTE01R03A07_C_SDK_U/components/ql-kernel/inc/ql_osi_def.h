
/**  @file
  ql_osi_def.h

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/
    
#ifndef QUEC_API_EVENT_DEF_H
#define QUEC_API_EVENT_DEF_H

#include "ql_api_common.h"


/***************************      general define      *******************************/


/***************************  app event id defination   *******************************/
//event id, unsigned type
/*
*                       event ID definition
*
* 4 bytes, little endian, unsigned type, as below:
*
*       ------------------------------------------------------------------------------------------------------
*       |            4              |             3            |          2            |          1          |
*       ------------------------------------------------------------------------------------------------------
*       | High byte of component ID | low byte of component ID | High byte of event ID | low byte of event ID|
*       ------------------------------------------------------------------------------------------------------
*
* component ID defined at enum type <ql_errcode_component_e>.
* detail errcode defined by each component.
*
*/
typedef enum
{
    QUEC_APP_EVENT_ID_MIN = 1|(QL_COMPONENT_OSI<<16),
    QUEC_TEST_EVENT_ID_1,
    QUEC_TEST_EVENT_ID_2,
    QUEC_TEST_EVENT_ID_3,
    QUEC_KERNEL_FEED_DOG,

//network
    QUEC_NW_SIGNAL_QUALITY_IND = 1|(QL_COMPONENT_NETWORK<<16),
    QUEC_NW_VOICE_REG_STATUS_IND,
    QUEC_NW_DATA_REG_STATUS_IND,
    QUEC_NW_NITZ_TIME_UPDATE_IND,
    QUEC_NW_JAMMING_DETECT_IND,
    QUEC_NW_CAUSE_IND,
    QUEC_NW_CELL_INFO_REQ,
    QUEC_NW_CELL_INFO_IND,
    QUEC_NW_LTE_AUTO_DISABLE_IND,
//datacall    
    QUEC_DATACALL_ACT_RSP_IND = 1|(QL_COMPONENT_NETWORK_MANAGE<<16),
    QUEC_DATACALL_DEACT_RSP_IND,
    QUEC_DATACALL_PDP_DEACTIVE_IND,

//usbnet
	QUEC_USBNET_START_RSP_IND = 1|(QL_COMPONENT_NETWORK_USBNET<<16),	//response of usbnet start
	QUEC_USBNET_DEACTIVE_IND,											//usbnet down, The cause may be loss of network coverage or detached by the network
	QUEC_USBNET_PORT_CONNECT_IND,										//usb plug in
	QUEC_USBNET_PORT_DISCONNECT_IND,									//usb pull out

//uart
    QUEC_UART_RX_RECV_DATA_IND = 1|(QL_COMPONENT_BSP_UART<<16),
    QUEC_UART_RX_OVERFLOW_IND,
    QUEC_UART_TX_FIFO_COMPLETE_IND,
    QUEC_UART_TX_COMPLETE_APP_IND,
    QUEC_UART_RX_RECV_DATA_APP_IND, 

//usb
	QUEC_USB_LINESTATE_IND = 1|(QL_COMPONENT_BSP_USB<<16),		//only used for kernel, to handle usb linestate


//uac
	QUEC_USB_AUDIO_CREATE_DONE = 2000|(QL_COMPONENT_BSP_USB<<16),	//UAC created ok, only used for kernel
	QUEC_USB_AUDIO_DATA_IN = 2001|(QL_COMPONENT_BSP_USB<<16),	//UAC received data, only used for kernel	
	QUEC_USB_AUDIO_START_PLAY = 2002|(QL_COMPONENT_BSP_USB<<16), //UAC start receive data, only used for kernel	
	QUEC_USB_AUDIO_STOP_PLAY = 2003|(QL_COMPONENT_BSP_USB<<16),  //UAC stop receive data, only used for kernel	
	QUEC_USB_AUDIO_START_RECORD = 2004|(QL_COMPONENT_BSP_USB<<16), //UAC start send data to host, only used for kernel	
	QUEC_USB_AUDIO_STOP_RECORD = 2005|(QL_COMPONENT_BSP_USB<<16), //UAC stop send data to host, only used for kernel	

//usb ccid
    QUEC_USB_CCID_HANDLER_START = 3007|(QL_COMPONENT_BSP_USB<<16),
    
//bt
    QUEC_BT_START_STATUS_IND = 1|(QL_COMPONENT_BSP_BT<<16),     //bt/ble start
    QUEC_BT_STOP_STATUS_IND,                                    //bt/ble stop
    QUEC_BT_BLE_RESET_IND,                                      //bt/ble reset.need to restart bt
//经典蓝牙共用的事件
    QUEC_BT_VISIBILE_IND,                                       //
    QUEC_BT_HIDDEN_IND,
    QUEC_BT_SET_LOCALNAME_IND,
    QUEC_BT_SET_LOCALADDR_IND,
    QUEC_BT_INQUIRY_IND,
    QUEC_BT_INQUIRY_END_IND,
    QUEC_BT_BOND_IND,
    QUEC_BT_BOND_STATE_IND,
    QUEC_BT_CONNECT_IND,
    QUEC_BT_DISCONNECT_IND,

    QUEC_BT_HFP_CONNECTION_IND,

//处理BT BLE共用的一些事件
    QUEC_BT_BLE_ERROR_IND  = 30|(QL_COMPONENT_BSP_BT<<16),    //蓝牙协议栈出现问题，需要针对不同的错误码进行处理
//ble
    //all:server and client can recieve this event
    //server:only server can recieve this event
    //client:only client can recieve this event
    QUEC_BLE_CONNECT_IND = 100|(QL_COMPONENT_BSP_BT<<16),       //all:ble connect
    QUEC_BLE_DISCONNECT_IND,                                    //all:ble disconnect
    QUEC_BLE_UPDATE_CONN_PARAM_IND,                             //all:ble update connection parameter
    
    QUEC_BLESCAN_REPORT_IND,                                    //client:ble gatt cliet scan and report other devices

    QUEC_BLE_GATT_MTU,                                          //all:ble connection mtu
    QUEC_BLE_GATT_RECV_IND,                                     //server:when ble client write characteristic value or descriptor,server get the notice
    QUEC_BLE_GATT_RECV_READ_IND,                                //server:when ble client read characteristic value or descriptor,server get the notice
    QUEC_BLE_GATT_RECV_NOTIFICATION_IND,                        //client:client recieve notification   
    QUEC_BLE_GATT_RECV_INDICATION_IND,                          //client:client recieve indication
    QUEC_BLE_GATT_SEND_END,                                     //server send notification,and recieve send end notice

    QUEC_BLE_GATT_START_DISCOVER_SERVICE_IND,                   //client:start discover service
    QUEC_BLE_GATT_DISCOVER_SERVICE_IND,                         //client:discover service
    QUEC_BLE_GATT_DISCOVER_CHARACTERISTIC_DATA_IND,             //client:discover characteristic
    QUEC_BLE_GATT_DISCOVER_CHARA_DESC_IND,                      //client:discover characteristic descriptor
    QUEC_BLE_GATT_CHARA_WRITE_WITH_RSP_IND,                     //client:write characterisctc value with response
    QUEC_BLE_GATT_CHARA_WRITE_WITHOUT_RSP_IND,                  //client:write characteristic value without response
    QUEC_BLE_GATT_CHARA_READ_IND,                               //client:read characteristic value by hanlde
    QUEC_BLE_GATT_CHARA_READ_BY_UUID_IND,                       //client:read characteristic value by uuid
    QUEC_BLE_GATT_CHARA_MULTI_READ_IND,                         //client:read miltiple characteristic value
    QUEC_BLE_GATT_DESC_WRITE_WITH_RSP_IND,                      //client:wirte characteristic descriptor
    QUEC_BLE_GATT_DESC_READ_IND,                                //client:read characteristic descriptor
    QUEC_BLE_GATT_ATT_ERROR_IND,                                //client:attribute error
    QUEC_BLE_GATT_STATE_CHANGE_IND,                             //server:通道改变
    QUEC_BLE_GATT_SEND_NODIFY_IND,                              //server:通知server线程从缓存里读取数据发送，只在标准版本使用
//BT HFP处理的事件
    QUEC_BT_HFP_CONNECT_IND= 200|(QL_COMPONENT_BSP_BT<<16),     //bt hfp connected
    QUEC_BT_HFP_DISCONNECT_IND,                                 //bt hfp disconnected
    QUEC_BT_HFP_CALL_IND,                                       //bt hfp call state callback
    QUEC_BT_HFP_CALL_SETUP_IND,                                 //bt hfp call setup state callback
    QUEC_BT_HFP_NETWORK_IND,                                    //bt hfp network state callback
    QUEC_BT_HFP_NETWORK_SIGNAL_IND,                             //bt hfp network signal callback
    QUEC_BT_HFP_BATTERY_IND,                                    //bt hfp battery level callback
    QUEC_BT_HFP_CALLHELD_IND,                                   //bt hfp callheld state callback
    QUEC_BT_HFP_AUDIO_IND,                                      //bt hfp audio state callback
    QUEC_BT_HFP_VOLUME_IND,                                     //bt hfp volume type callback
    QUEC_BT_HFP_NETWORK_TYPE_IND,                               //bt hfp network type callback
    QUEC_BT_HFP_RING_IND,                                       //bt hfp ring indication callback
    QUEC_BT_HFP_CODEC_IND,                                      //bt hfp codec type callback
//BT A2DP & AVRCP处理的事件
    QUEC_BT_A2DP_AUDIO_CONFIG_IND = 300|(QL_COMPONENT_BSP_BT<<16),  //bt a2dp audio config callback
    QUEC_BT_A2DP_AUDIO_STATE_START_IND,							    //bt a2dp audio state start callback
    QUEC_BT_A2DP_AUDIO_STATE_STOPPED_IND,						    //bt a2dp audio state stop callback
    QUEC_BT_A2DP_CONNECTION_STATE_CONNECTED_IND,				    //bt a2dp connect callback
    QUEC_BT_A2DP_CONNECTION_STATE_DISCONNECTED_IND,				    //bt a2dp disconnect callback
    
    QUEC_BT_AVRCP_CONNECTION_STATE_CONNECTED_IND  = 400|(QL_COMPONENT_BSP_BT<<16),		//bt avrcp connect callback
    QUEC_BT_AVRCP_CONNECTION_STATE_DISCONNECTED_IND,			                        //bt avrcp disconnect callback
    QUEC_BT_AVRCP_VOLUME_CHANGE_IND,							                        //bt avrcp volume change callback
    
//BT SPP处理的事件
    QUEC_BT_SPP_CONNECT_IND= 500|(QL_COMPONENT_BSP_BT<<16),//bt spp connected  callback
    QUEC_BT_SPP_DISCONNECT_IND,             //bt spp disconnected   callback
    QUEC_BT_SPP_REVDATA_IND,                //bt spp receive data callback

//led_cfg(network status for netlight)
    QUEC_LEDCFG_EVENT_IND = 1|(QL_COMPONENT_BSP_PWM<<16),
    QUEC_LEDCFG_CB_CREATE_IND,
    QUEC_VIRTUAL_PWM_EVENT,
    QUEC_LEDCFG_VIRT_PWM_EVENT,
    QUEC_HR_VIRT_PWM_EVENT,
//sleep
	QUEC_SLEEP_ENETR_AUTO_SLEPP  = 1 | (QL_COMPONENT_PM_SLEEP << 16),
	QUEC_SLEEP_EXIT_AUTO_SLEPP   = 2 | (QL_COMPONENT_PM_SLEEP << 16),

//power
	QUEC_SLEEP_QUICK_POWER_DOWM  = 1 | (QL_COMPONENT_PM << 16),
	QUEC_SLEEP_NORMAL_POWER_DOWM = 2 | (QL_COMPONENT_PM << 16),
	QUEC_SLEEP_QUICK_RESET		 = 3 | (QL_COMPONENT_PM << 16),
	QUEC_SLEEP_NORMAL_RESET 	 = 4 | (QL_COMPONENT_PM << 16),
	QUEC_SLEEP_FAST_POWEROFF 	 = 5 | (QL_COMPONENT_PM << 16),

//sim
    QL_PBK_INIT_OK_IND           = 1 | (QL_COMPONENT_SIM << 16),
    QUEC_SIM_APREADY_CMD_IND     = 2 | (QL_COMPONENT_SIM << 16),
    QUEC_SIM_CRSM_CNF            = 3 | (QL_COMPONENT_SIM << 16),
    QUEC_SIM_CPOL_AT_REQ         = 4 | (QL_COMPONENT_SIM << 16),
    QUEC_SIM_CPOL_AT_CNF         = 5 | (QL_COMPONENT_SIM << 16),

//vsim adapt
    QUEC_VSIM_ADAPT_POWERON_IND  = 1 | (QL_COMPONENT_VSIM_ADAPT << 16),
    QUEC_VSIM_ADAPT_RDY_IND      = 2 | (QL_COMPONENT_VSIM_ADAPT << 16), //FOR standard AT solution, not cared in OPEN project.
    QUEC_VSIM_ADAPT_VSIM_AT_IND  = 3 | (QL_COMPONENT_VSIM_ADAPT << 16), //FOR standard AT solution, not cared in OPEN project. //old feature, don't used

//pwrkey
    QUEC_PWRKEY_SHUTDOWN_START_IND  = 1 | (QL_COMPONENT_BSP_KEYPAD << 16),
    QUEC_PWRKEY_PRESS_IND,
    QUEC_PWRKEY_RELEASE_IND,
    QUEC_PWRKEY_LONGPRESS_IND,

//wifiscan
	QUEC_WIFISCAN_EVENT_DO          = 1 | (QL_COMPONENT_BSP_WIFISCAN << 16),
	QUEC_WIFISCAN_EVENT_ASYNC_IND   = 5 | (QL_COMPONENT_BSP_WIFISCAN << 16),
//gnss
    QUEC_GNSS_OK_CMD_IND = 1|(QL_COMPONENT_BSP_GNSS<<16),	
    QUEC_GNSS_FAIL_0_CMD_IND = 2|(QL_COMPONENT_BSP_GNSS<<16),	
    QUEC_GNSS_FAIL_1_CMD_IND= 3|(QL_COMPONENT_BSP_GNSS<<16),
    QUEC_GPS_INFO_CMD= 4|(QL_COMPONENT_BSP_GNSS<<16),
    QUEC_GNSS_POWER_EVENT_REQ= 5|(QL_COMPONENT_BSP_GNSS<<16),
    QUEC_GNSS_DISABLE_STATUS_IND= 6|(QL_COMPONENT_BSP_GNSS<<16),
    QUEC_GNSS_REPORT_IND = 7|(QL_COMPONENT_BSP_GNSS<<16),
    
//lvgl
    QUEC_LVGL_QUIT_IND= 1|(QL_COMPONENT_BSP_LVGL<<16),

//virt at
    QUEC_VIRT_AT_RX_RECV_DATA_IND = 1 | (QL_COMPONENT_BSP_VIRT_AT<<16),

//sd
    QUEC_SDDET_EVENT_IND = 1 | (QL_COMPONENT_STORAGE_SDEMMC<<16),

//tts
	QUEC_TTS_START_PLAY = 1 | (QL_COMPONENT_AUDIO_TTS<<16),

//headset_det
    QUEC_HEADSET_DET_EVENT_IND = 1 | (QL_COMPONENT_AUDIO_HEADSET<<16),
    QUEC_HEADSET_PLUG_IN_IND   = 2 | (QL_COMPONENT_AUDIO_HEADSET<<16),
    QUEC_HEADSET_PLUG_OUT_IND  = 3 | (QL_COMPONENT_AUDIO_HEADSET<<16),
    QUEC_HEADSET_BTN_DOWN_IND  = 4 | (QL_COMPONENT_AUDIO_HEADSET<<16),
    QUEC_HEADSET_BTN_UP_IND    = 5 | (QL_COMPONENT_AUDIO_HEADSET<<16),

//stk
    QUEC_STK_PROACTIVE_CMD_IND    = 1 | (QL_COMPONENT_STK<<16),
    QUEC_STK_NO_PROACTIVE_CMD_IND = 2 | (QL_COMPONENT_STK<<16),
    QUEC_STK_TIMEOUT_IND          = 3 | (QL_COMPONENT_STK<<16),
    QUEC_STK_SIM_LOST_IND         = 4 | (QL_COMPONENT_STK<<16),

// tcpip app
	QUEC_TCPIP_APP_PDP_EVT_IND	    = 1 | (QL_COMPONENT_LWIP << 16),
//AT Command
	QUEC_RECEIVE_NEW_AT 	= 1|(QL_COMPONENT_AT << 16),
	QUEC_AT_RESP			= 2|(QL_COMPONENT_AT << 16),
	QUEC_AT_RECEIVE_DATA	= 3|(QL_COMPONENT_AT << 16),
	QUEC_AT_FINISH			= 4|(QL_COMPONENT_AT << 16),
	QUEC_AT_RECEIVE_CB		= 5|(QL_COMPONENT_AT << 16),
	QUEC_AT_BYPASS_NEW_DATA = 6|(QL_COMPONENT_AT << 16),
	QUEC_AT_CMUX_NEW_DATA   = 7|(QL_COMPONENT_AT << 16),
	QUEC_AT_CMUX_SEND_DATA	= 8|(QL_COMPONENT_AT << 16),
	QUEC_AT_CMUX_CLOSE		= 9|(QL_COMPONENT_AT << 16),
	
//queclocator
	QUEC_LBS_EVENT_WIFISCAN_REC	    = 1 | (QL_COMPONENT_LWIP_LBS << 16),
//BSP GPIO
    QUEC_GPIOCTRL_EVENT_IND         = 1 | (QL_COMPONENT_BSP_GPIO << 16),
    QUEC_GPIOCTRL_TIMER_EVENT_IND         = 2 | (QL_COMPONENT_BSP_GPIO << 16),
//SS
    QUEC_SS_USSD_IND        = 1|(QL_COMPONENT_SS << 16),

//ETHERNET
    QUEC_ETHERNET_DRV_INT           	= 1 | (QL_COMPONENT_ETHERNET<<16),
    QUEC_ETHERNET_DRV_DATA_OUTPUT  	    = 2 | (QL_COMPONENT_ETHERNET<<16),
    QUEC_ETHERNET_DRV_STATUS_CHECK 	    = 3 | (QL_COMPONENT_ETHERNET<<16),
    QUEC_ETHERNET_DRV_TRY_RESET         = 4 | (QL_COMPONENT_ETHERNET<<16),
    QUEC_ETHERNET_DRV_TRY_CONNECT       = 5 | (QL_COMPONENT_ETHERNET<<16),
    QUEC_ETHERNET_DRV_NET_CREATE        = 6 | (QL_COMPONENT_ETHERNET<<16),

    QUEC_ETHERNET_APP_CONNECTED         = 7 | (QL_COMPONENT_ETHERNET<<16),
    QUEC_ETHERNET_APP_DISCONNECTED      = 8 | (QL_COMPONENT_ETHERNET<<16),

    QUEC_ETHERNET_DRV_NET_UP            = 9 | (QL_COMPONENT_ETHERNET<<16),
    QUEC_ETHERNET_DRV_NET_DOWN          = 10 | (QL_COMPONENT_ETHERNET<<16),

    //l2tp
	QUEC_L2TP_START_RSP_IND = 1|(QL_COMPONENT_L2TP<<16),//response of l2tp start
	QUEC_L2TP_STOP_RSP_IND  = 2|(QL_COMPONENT_L2TP<<16),//response of l2tp stop
    QUEC_L2TP_DEACTIVE_IND	= 3|(QL_COMPONENT_L2TP<<16),//l2tp down, The cause may be loss of network coverage or detached by the network

    //next is reserved for app to add event id!
    //
	QL_EVENT_APP_START           = 1 | (QL_COMPONENT_APP_START<<16),//app event start
	
    QL_EVEN_MAX                  = 0xFFFF | (QL_COMPONENT_MAX<<16),//event max
}ql_api_event_id_e;

/***************************  task priority defination   ******************************/
typedef enum
{
	APP_PRIORITY_IDLE = 1, // reserved
    APP_PRIORITY_LOW = 4,
    APP_PRIORITY_BELOW_NORMAL = 8,
    APP_PRIORITY_NORMAL = 12,
    APP_PRIORITY_ABOVE_NORMAL = 16,
/*
	It is recommended that the task priority on the app side is not higher than 22,
	otherwise it may affect the kernel task operation
*/
    APP_PRIORITY_HIGH = 25,
    APP_PRIORITY_REALTIME = 30
}APP_ThreadPriority_e;



/****************************  error code about osi    ***************************/
typedef enum
{
	QL_OSI_SUCCESS             =     0,
	
	QL_OSI_TASK_PARAM_INVALID  =	 1 | (QL_COMPONENT_OSI << 16),
	QL_OSI_TASK_CREATE_FAIL,   
	QL_OSI_NO_MEMORY,
	QL_OSI_TASK_DELETE_FAIL,
	QL_OSI_TASK_PRIO_INVALID,
	QL_OSI_TASK_NAME_INVALID,
	QL_OSI_TASK_EVENT_COUNT_INVALID,
	QL_OSI_INVALID_TASK_REF,
	QL_OSI_TASK_CNT_REACH_MAX,
	QL_OSI_TASK_BIND_EVENT_FAIL,
	QL_OSI_TASK_UNBIND_EVENT_FAIL,
	QL_OSI_TASK_GET_REF_FAIL,
	QL_OSI_TASK_GET_PRIO_FAIL,
	QL_OSI_TASK_SET_PRIO_FAIL,
	QL_OSI_TASK_GET_STATUS_FAIL,
	QL_OSI_TASK_HAS_BINDED_TIMER,

	QL_OSI_QUEUE_CREATE_FAIL	=   50 | (QL_COMPONENT_OSI << 16), 
	QL_OSI_QUEUE_DELETE_FAIL,
	QL_OSI_QUEUE_IS_FULL,
	QL_OSI_QUEUE_RELEASE_FAIL,
	QL_OSI_QUEUE_RECEIVE_FAIL,
	QL_OSI_QUEUE_GET_CNT_FAIL,
	QL_OSI_QUEUE__FAIL,

	QL_OSI_SEMA_CREATE_FAILE    =  100 | (QL_COMPONENT_OSI << 16), 
	QL_OSI_SEMA_DELETE_FAIL,
	QL_OSI_SEMA_IS_FULL,
	QL_OSI_SEMA_RELEASE_FAIL,
	QL_OSI_SEMA_GET_FAIL,
	QL_OSI_SEMA__FAIL,

	QL_OSI_MUTEX_CREATE_FAIL	=  150 | (QL_COMPONENT_OSI << 16), 
	QL_OSI_MUTEX_DELETE_FAIL,
	QL_OSI_MUTEX_LOCK_FAIL,
	QL_OSI_MUTEX_UNLOCK_FAIL,

	QL_OSI_EVENT_SEND_FAIL		=  200 | (QL_COMPONENT_OSI << 16),
	QL_OSI_EVENT_GET_FAIL,
	QL_OSI_EVENT_REGISTER_FAIL,

	QL_OSI_TIMER_CREATE_FAIL	=  250 | (QL_COMPONENT_OSI << 16),
	QL_OSI_TIMER_START_FAIL,
	QL_OSI_TIMER_STOP_FAIL,
	QL_OSI_TIMER_DELETE_FAIL,
	QL_OSI_TIMER_BIND_TASK_FAIL,

	QL_OSI_SWDOG_REGISTER_FAIL  =  300 | (QL_COMPONENT_OSI << 16),
	QL_OSI_SWDOG_UNREGISTER_FAIL,
	QL_OSI_SWDOG_FEED_DOG_FAIL,
	QL_OSI_SWDOG_ENABLE_FAIL,
	QL_OSI_SWDOG_DISABLE_FAIL,

	QL_OSI_EGROUP_CREATE_FAILE  =  350 | (QL_COMPONENT_OSI << 16), 
	QL_OSI_EGROUP_DELETE_FAIL,
	QL_OSI_EGROUP_CLEAR_BITS_FAIL,
	QL_OSI_EGROUP_SET_BITS_FAIL,
	QL_OSI_EGROUP_GET_BITS_FAIL,
	QL_OSI_EGROUP_WAIT_BITS_FAIL,
	QL_OSI_EGROUP_SYNC_FAIL,
}osi_errcode_e;


#endif
