
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_USB_H
#define QL_USB_H


#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define USB_MSC_DEVICE_MAX				2 //USB Mass Storage功能最大支持映射2个存储器
#define USB_MSC_DEVICE_TYPE				6
#define USB_DETECT_TIME_MIN             0
#define USB_DETECT_TIME_MAX             100000000
#define USB_CHARGING_ONFF_TIME_MIN      0
#define USB_CHARGING_ONFF_TIME_MAX      100000

//usb interface id
#define QUEC_USBAT_INTF_NUM				2	//usb at口
#define QUEC_USBDAIG_INTF_NUM			3	//usb diag口
#define QUEC_USBMOS_INTF_NUM			4	//debug host口
#define QUEC_USBCPLOG_INTF_NUM			5	//cp log口
#define QUEC_USBAPLOG_INTF_NUM			6	//ap log口
#define QUEC_USBMODEM_INTF_NUM			7	//usb modem口
#define	QUEC_USBNMEA_INTF_NUM			8	//usb nmea口
#define	QUEC_USBATEXT_INTF_NUM			9	//已废弃不使用
#define	QUEC_UAC_CTRL_INTF_NUM			10	//UAC 控制接口
#define	QUEC_UAC_RX_INTF_NUM			11	//UAC 接收数据接口
#define	QUEC_UAC_TX_INTF_NUM			12	//UAC 发送数据接口
#define	QUEC_USBPRINTER_INTF_NUM		13	//usb打印机
#define	QUEC_USBPRINTER_INTF_MTP		14	//usb mtp
#define	QUEC_USBPRINTER_INTF_MSG		15	//usb mass storage
#define	QUEC_USBPRINTER_INTF_CCID		16	//usb ccid


#define	QUEC_ACM_AT0_NOTIFY_INTF_NUM	20	//ACM at控制接口
#define	QUEC_ACM_AT0_DATA_INTF_NUM		21	//ACM at数据接口

#define USB_FUNC_ITEM_MAX 10
#define QUEC_VOLUME_NAME_MAX 15
#define QUEC_MTP_NAME_MAX 15
#define QUEC_MSC_NAME_MAX 11

#define	USB_HID_EVENT_TYPE	1000
#define USB_HID_SYSETM_PORT	110

#define HID_REPORT_DESC_SIZE_MAX		256
#define HID_STRING_DESC_SIZE_MAX		32

/*===========================================================================
 * Enum
 ===========================================================================*/
typedef enum
{
	QL_USB_SUCCESS						= QL_SUCCESS,							 /*  operating is successful  */
	QL_USB_INVALID_PARAM				= (QL_COMPONENT_BSP_USB << 16) | 1000,   /*  invalid input param  */
	QL_USB_SYS_ERROR					= (QL_COMPONENT_BSP_USB << 16) | 1001,	 /*  system error  */
	QL_USB_DETECT_SAVE_NV_ERR           = (QL_COMPONENT_BSP_USB << 16) | 1002,	 /*  save detect time to NV failed */
	QL_USB_NO_SPACE						= (QL_COMPONENT_BSP_USB << 16) | 1003,   /*  no space to store data  */
	QL_USB_NOT_SUPPORT					= (QL_COMPONENT_BSP_USB << 16) | 1004,   /*  current operation not support  */
	QL_USB_REOPEN_ERR					= (QL_COMPONENT_BSP_USB << 16) | 1005,   /*  usb reopen  */
}ql_errcode_usb_e;

typedef enum
{
	QL_USB_HID_0,	//当前仅支持开启1路USB HID
		
	QL_USB_HID_MAX
}ql_usb_hid_port_e;

typedef enum
{
	QL_USB_RX_ARRIVE	= 0x1,
	QL_USB_TX_COMPLETE	= 0x2
}ql_usb_event_e;

typedef enum
{
	QL_USB_HOTPLUG_OUT = 0,    //USB in plug out state
	QL_USB_HOTPLUG_IN  = 1,   //USB was inserted but not enum
	QL_USB_HOTPLUG_IN_ENUMED =2  //USB insert and enum done
}QL_USB_HOTPLUG_E;

typedef enum
{
	QL_USB_DET_MIN = 0,
	QL_USB_DET_CHARGER = 0,	   //usb will be enabled and disabled by VBUS high level and low level 
    QL_USB_DET_AON = 1,		   //usb will be enabled always, and won't be disabled 
    QL_USB_DET_NO_VBUS = 2,    //usb will be enabled and disabled by GPIO, the GPIO function not support now, so it is same as QL_USB_DET_AON now

	QL_USB_DET_MAX,
}QL_USB_DET_E;


/*
	1. USB端口选择在app_start.c中的quec_usb_serial_create函数中开源,用户可以自行到该函数中选择开启哪些端口,以下配置皆为默认端口组合
	
	2. QL_USB_ENUM_MASS_STORAGE用于将模块的SD卡/内置flash/外置6线flash映射到电脑,用于传输文件由于占用RAM空间较大,开启此模式后,由于端点不够
	   默认会删除USB_NMEA口. 用户若需要用NMEA口,可以在app_start.c中的quec_usb_serial_create函数中自行选择裁剪其他端口,保留NMEA口
	
	3. usb nmea口默认开启,用户可使用宏CONFIG_QUEC_PROJECT_FEATURE_USB_NMEA_AT关闭; 占用300k ram
	4. usb modem口默认开启,用户可使用宏CONFIG_QUEC_PROJECT_FEATURE_USB_MODEM_AT关闭,开启UAC/MASS STORAGE后此端口被关闭; 占用300k ram
	5. usb at口默认开启,用户可在可在quec_usb_serial_create函数中关闭COM0的枚举; 占用300k ram
*/
typedef enum
{
	QL_USB_ENUM_NONE 	= -1,		//NO USB port will be enumerated

/*
	USB Device Mode
*/
	QL_USB_ENUM_USBNET_COM, 		//usbnet(rndis/ecm), USB_AT(ttyUSB0), USB_DIAG, USB_MOS, USB_AP_LOG, USB_CP_LOG, USB_MODEM(ttyUSB5), USB_NMEA(ttyUSB6)				
	QL_USB_ENUM_USBNET_COM_UAC, 	//usbnet(rndis/ecm), USB_AT(ttyUSB0), USB_DIAG, USB_MOS, USB_AP_LOG, USB_CP_LOG, USB_MODEM(ttyUSB5), UAC
	QL_USB_ENUM_USBNET_COM_PRINTER, //usbnet(rndis/ecm), USB_AT(ttyUSB0), USB_DIAG, USB_DebugHost, USB_AP_LOG, USB_CP_LOG, USB_MODEM(ttyUSB5), USB_NMEA / USB_PRINTER
	QL_USB_ENUM_ACM_ONLY,			//ttyACM0			
	QL_USB_ENUM_MASS_STORAGE,		//usbnet(rndis/ecm), USB_AT(ttyUSB0), USB_DIAG, USB_MOS, USB_AP_LOG, USB_CP_LOG, USB_MODEM(ttyUSB5), USB_MTP
	QL_USB_ENMU_HID,				//usbnet(rndis/ecm), USB_AT(ttyUSB0), USB_DIAG, USB_MOS, USB_AP_LOG, USB_CP_LOG, USB_MODEM(ttyUSB5), USB_HID
	QL_USB_ENUM_USBNET_COM_CCID,	//usbnet(rndis/ecm), USB_AT(ttyUSB0), USB_DIAG, USB_MOS, USB_AP_LOG, USB_CP_LOG, USB_MODEM(ttyUSB5), USB_CCID
	QL_USB_ENUM_MAX
}QL_USB_ENMU_MODE_E;

typedef enum
{
	QL_USB_REMOTE_WAKEUP_ENABLE 	= 0,		//使能远程唤醒上位机
	QL_USB_REMOTE_WAKEUP_DISABLE,		        //不使能远程唤醒上位机
}QL_USB_REMOTE_WAKEUP_E;

typedef enum
{
	QL_USB_NV_TYPE_ENUM_MODE = 0,
	QL_USB_NV_TYPE_MASS_STORAGE = 1,
	QL_USB_NV_TYPE_REMOTE_WAKEUP = 2,
	QL_USB_NV_TYPE_HID = 3,

	QL_USB_NV_TYPE_MAX,
}QL_USB_NV_TYPE_E;

typedef enum
{
	QL_USB_MSC_HIDE_NONE 	= 0,	//MSC模式下,且SD卡有2个分区时,2个分区全部显示
	QL_USB_MSC_HIDE_PART1 	= 1,	//MSC模式下,且SD卡有2个分区时,PC上不显示分区1(模块的SD:磁盘)
	QL_USB_MSC_HIDE_PART2 	= 2,	//MSC模式下,且SD卡有2个分区时,PC上不显示分区2(模块的SD1:磁盘)
}QL_USB_MSC_HIDE_E;


/*
	1. 选择映射模块的哪些存储器到PC端, 最大支持映射2个存储器; 例如需要映射sd卡和外部
	   6线flash,可以用 (QL_USB_MSC_SDCARD | QL_USB_MSC_EFS)
	2. 注意: 映射模块外部flash时,外部flash使用的文件系统需要是模块自带的,
	   内核需要用ql_fopen这些API去控制与PC之间的文件传输
*/
typedef enum
{
	QL_USB_MSC_DEFAULT = 0,		//默认设备,默认映射SD卡和内置nor flash
	QL_USB_MSC_SDCARD = 0x01,	//模块的SD卡分区1(未分区则为默认分区)
	QL_USB_MSC_UFS = 0x2,		//模块的内置 nor flash
	QL_USB_MSC_EFS = 0x4,		//模块的外置6线 nor flash
	QL_USB_MSC_EXNSFFS = 0x8,	//模块的外置4线 nor flash
	QL_USB_MSC_EXNANDFS = 0x10,	//外置 nand flash 
    QL_USB_MSC_SDCARD1 = 0x20,	//模块的SD卡分区2
}QL_USB_MSC_E;

typedef enum
{
/*
	MTP: 1. 类似于手机接入PC
		 2. 该协议下支持挂载2个存储器, 可挂载的存储器见QL_USB_MSC_E
		 3. 该模式下PC无法格式化存储器,安全系数较高
		 4. 该模式下支持PC和模组同时向存储器写文件
		 5. MTP模式下卷标名称最大可配置15字节
		 6. 移远使用普通SD卡测试大文件读写,速率约为2.7M/s 2.1M/s,以用户的sd卡以及用户实际测量速率为准
*/
	QL_USB_PROTOCOL_MTP, //Media Transfer Protocol

/*
	MSC: 1. 类似于读卡器接入PC, 此模式仅支持SD卡

		 2. MSC模式下PC会以裸读写的方式访问SD卡,会存与模块文件系统API产生冲突的问题;因此在PC加载出设备后
		 	模块端默认禁止使用文件系统API访问SD卡(SD卡的隐藏分区不受限制),拔出USB或关闭MSC后自动取消保护;
		 	用户可通过ql_usb_msc_cfg_t中的option参数来配置隐藏分区和读写保护功能
		 	
		 3. 该模式下PC可直接格式化SD卡,安全性不如MTP; 用户也可以将SD卡进行分区,配置只显示一个分区到PC上,
		 	这样可以保护另一个分区不被PC访问,以及格式化
		 4. MTP模式下卷标名称最大可配置11字节,并且只支持英文大写字母
		 5. 移远使用普通SD卡测试大文件读写,速率约为8M/s 6M/s,以用户的sd卡以及用户实际测量速率为准
*/
	QL_USB_PROTOCOL_MSC //Mass Storage Class
}ql_usb_protocol_e;

typedef void (*ql_usb_hid_callback)(uint32 ind_type, ql_usb_hid_port_e port, uint32 size);
/*===========================================================================
 * STRUCT
 ===========================================================================*/
typedef struct
{
	QL_USB_DET_E det_mode;
	uint 		 det_pin;    //not used now, but will used feature
	uint 		 reserve[2]; //reserved for futher using
}ql_usb_detmdoe_t;           //this structure will be writed to NV

typedef struct
{
/*
	MSC模式下,且SD卡有2个分区时,PC上显示哪个分区; 默认会将SD卡的两个分区都显示出来,不隐藏
*/
	QL_USB_MSC_HIDE_E hide_option;

/*
	是否取消MSC模式下,PC加载出的SD后模组侧对SD卡的读写保护; 默认PC加载出SD卡后,模组为了避免
	PC与模组同时对SD卡进行访问,会禁止模组的文件系统API读写SD卡
	0 --- 使能SD卡保护
	1 --- 取消SD卡保护
*/	
	bool protect_dsiable;
	char reserve[20]; //无效参数,无需关注
}ql_msc_option_t;

typedef struct
{
    uint32_t usb_detect_time;
    uint32_t charging_on_time;
    uint32_t charging_off_time;
	ql_usb_detmdoe_t usb_det_mode;
} ql_usb_setting_s;

typedef struct
{
	int msc_device;  				  	//需要映射到PC的存储器,参考枚举QL_USB_MSC_E;多个存储之间用'|'连接,最大支持映射2个存储器;仅MTP协议下可配
	char dev_name[QUEC_VOLUME_NAME_MAX];//PC上显示的设备名称,MTP模式下支持15字节,MSC协议下支持11字节
	ql_usb_protocol_e protocol;		  	//当前支持MTP和MSC协议
	ql_msc_option_t	option;				//使用MSC协议时的部分用户配置; 使用MTP协议可忽略此参数
}ql_usb_msc_cfg_t;

typedef struct
{
	bool enabled;								//是否使能USB HID
	char report_desc[HID_REPORT_DESC_SIZE_MAX]; //报告描述符
	uint report_desc_size;						//报告描述符的大小
	char string_desc[HID_STRING_DESC_SIZE_MAX];	//字符串描述符
	uint string_desc_size;						//字符串描述符长度
}ql_usb_hid_cfg_t;

typedef struct
{
	uint8 enum_mode;
	ql_usb_msc_cfg_t msc_cfg;
    uint8 remote_wakeup;            //是否远程唤醒上位机
	ql_usb_hid_cfg_t hid_cfg;
    ql_msc_option_t	msc_option;		//MSC模式下的部分配置值
}ql_usb_nv_t;

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_CCID_DEVICE
typedef int (*ql_ccidXfrBlockCb)(void *buf,uint16_t len);
typedef uint8_t (*ql_ccidIccPowerOnCb)( void );
typedef uint8_t (*ql_ccidIccPowerOffCb)( void );
typedef uint8_t (*ql_ccidGetSlotStatusCb)( void );
typedef struct
{
    /*user power on callback*/
    ql_ccidIccPowerOnCb usrCcidIccPowerOnCb;
    /*user power off callback*/
    ql_ccidIccPowerOffCb usrccidIccPowerOffCb;
    /*user get slot status callback*/
    ql_ccidGetSlotStatusCb usrccidGetSlotStatusCb;
    /*user PC_to_RDR_XfrBlock callback*/
    ql_ccidXfrBlockCb usrccidXfrBlockCb;
}ql_usb_ccid_usr_cb_t;
#endif

/*===========================================================================
 * function
 ===========================================================================*/

/*****************************************************************
	!!!!!   don't  block the callback , as is run in interrupt   !!!!!!
* Function: ql_usb_hotplug_cb
*
* Description: the defination of usb hotplug callback
* 
* Parameters:
* 	state	  		Indicates whether the USB action is unplugged or inserted 
*	ctx         	not used now 
*
* Return:
* 	0
*****************************************************************/
typedef int(*ql_usb_hotplug_cb)(QL_USB_HOTPLUG_E state, void *ctx); //

/*****************************************************************
* 
* Function: ql_usb_bind_hotplug_cb
*
* Description: bind usb hotplug callback to kernel
* 
* Attention:
   1. the callback will be run in interrupt, so it is forbidden to block the callback;
   2. it is forbidden to call Audio start/stop/close， file write/read，CFW（releated to RPC）in interrupt;
   3. it is forbidden to enter critical in interrupt
   4. it is suggested for users to  perform simple operations , or send event(no timeout) to inform your task in interrupt

* Parameters:
* 	hotplug_callback	  [in]callback
*
* Return:
* 	0
*****************************************************************/
ql_errcode_usb_e ql_usb_bind_hotplug_cb(ql_usb_hotplug_cb hotplug_callback);

/*****************************************************************
* Function: ql_usb_get_hotplug_state
*
* Description: get the usb hotplug state
* 
* Parameters:
* 	hotplug_callback	  [in]callback
*
* Return:
* 	QL_USB_HOTPLUG_OUT   : USB is not insrrted
*	QL_USB_HOTPLUG_IN    : USB is inserted
*****************************************************************/
QL_USB_HOTPLUG_E ql_usb_get_hotplug_state(void);

/*****************************************************************
* Function: ql_usb_set_detmode
*
* Description: set usb detect mode
* 
* Parameters:
* 	usb_mode    [in] detected mode, take effect after reset
*
* Return:
*   0:          success
*   others:     ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_set_detmode(ql_usb_detmdoe_t *usb_mode);

/*****************************************************************
* Function: ql_usb_get_detmode
*
* Description: get the usb detect mode
* 
* Parameters:
* 	ql_usb_detmdoe_t	[out]usb detect mode
*
* Return:
*   0:          success
*   others:     ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_get_detmode(ql_usb_detmdoe_t *mode);

/*****************************************************************
* Function: ql_set_usb_detect_max_time
*
* Description: Set the maximum time to detect the connection of 
*              USB DP/DM data line after USB insertion
*              设置USB插入后最大的检测时间，如果过了这个时间，还没有
*              检测到DP/DM线上有数据返回，则关闭检测，省电
* 
* Parameters:
*   ms      [in] the maximum detect time to set
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*
*****************************************************************/
ql_errcode_usb_e ql_set_usb_detect_max_time(uint32_t ms);

/*****************************************************************
* Function: ql_get_usb_detect_max_time
*
* Description: get the maximum USB detection time set
*              
* Parameters:
*   ms      [out]  the maximum detect time
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*
*****************************************************************/
ql_errcode_usb_e ql_get_usb_detect_max_time(uint32_t *ms);

/*****************************************************************
* Function: ql_usb_get_enum_mode
*
* Description: to get the usb enumeration mode             
*
* Return:
*	QL_USB_ENMU_MODE_E
*
*****************************************************************/
QL_USB_ENMU_MODE_E ql_usb_get_enum_mode(void);

/*****************************************************************
* Function: ql_usb_set_enum_mode
*
* Description: to set the enumeration mode of usb port.For UAC or
			   usb printer mode, the USB nmea PORT will not be enumerated		   
*
* Parameters:
*	mode	   [in] see it in QL_USB_ENMU_MODE_E for detail
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_set_enum_mode(QL_USB_ENMU_MODE_E mode);

/*****************************************************************
* Function: ql_usb_get_remote_wakeup
*
* Description: to get the remote wakeup setting           
*
* Return:
*	(void)
*
*****************************************************************/
QL_USB_REMOTE_WAKEUP_E ql_usb_get_remote_wakeup(void);

/*****************************************************************
* Function: ql_usb_set_remote_wakeup
*
* Description: enable or disable remote wakeup.	defaul is enable remote wakeup
*
* Parameters:
*	remote_wakup [in] see it in QL_USB_REMOTE_WAKEUP_E for detail
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_set_remote_wakeup(QL_USB_REMOTE_WAKEUP_E remote_wakup);

/*****************************************************************
* Function: ql_usb_enable
*
* Description: enable usb function, will take effect immediately
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_enable(void);

/*****************************************************************
* Function: ql_usb_enable
*
* Description: disable usb function, will take effect immediately
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_disable(void);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_CCID_DEVICE
/******************************
* Function: ql_usb_ccid_usr_cb_register
* Description: register user data callback funtion when reader recived block message type form PC
* @param param1                       [in] callback funtion point,if equal to NULL unregister
*
* @return OK                        	0   success
* @return others               	        failed
******************************/
ql_errcode_usb_e ql_usb_ccid_usr_cb_register(ql_usb_ccid_usr_cb_t user_cb);

/******************************
* Function: ql_usb_ccid_write
* Description: reader respond block message to PC
* @param param1                       [in] write data point
* @param param2                       [in] write data len , max size 512 bytes
*
* @return write numbers
* @return negative               	  failed
******************************/
int ql_usb_ccid_write(void * buf, uint16_t len);

/******************************
* Function: ql_usb_ccid_read
* Description: reader read block message from PC ,this funtion should call in ql_usb_ccid_data_recv_cb
* @param param1                       [out] read data point
* @param param2                       [in] read data len , max size 512 bytes
*
* @return read numbers
* @return negative               	  failed
******************************/
int ql_usb_ccid_read(void * buf, uint16_t len);

#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_MASS_STORAGE
/*****************************************************************
* Function: ql_usb_msc_config_set
*
* Description: to set the configration of usb mass storage, need to set 
			   the enum mode to usb mass storage by ql_usb_get_enum_mode firstly;
			   Restart to take effect
* Parameters:
*	msc_device [in]see it in QL_USB_MSC_E for detail, to set the mapping storage device
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_msc_config_set(ql_usb_msc_cfg_t *msc_cfg);

/*****************************************************************
* Function: ql_usb_msc_config_get
*
* Description: to get the configration of usb mass storage	   
*
* Parameters:
*	msc_cfg	   [out]the space for getting the configration
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_msc_config_get(ql_usb_msc_cfg_t *msc_cfg);

/*****************************************************************
* Function: ql_usb_mtp_service_refresh
*
* Description: refresh MTP service, update file mapping relationship.
*
* Note:Under the MTP protocol, if files are added or deleted locally, 
*      call this interface, and then refresh the MTP device on the PC 
*      to display the latest local file information
*
* Parameters:
*	None
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_mtp_service_refresh(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_HID_DEVICE

/*****************************************************************
* Function: ql_usb_hid_set_config
*
* Description: to enable hid function and config the descriptors	   
*
* Parameters:
*	cfg	   [In] the configuration structure
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_hid_set_config(ql_usb_hid_cfg_t *cfg);

/*****************************************************************
* Function: ql_usb_hid_set_config
*
* Description: to get the current configuration of hid function
*
* Parameters:
*	cfg	   [Out] the configuration structure
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_hid_get_config(ql_usb_hid_cfg_t *cfg);

/*****************************************************************
* Function: ql_usb_hid_open
*
* Description: to open the hid port, and set the callback

* Parameters:
*	port	   [In] port number, and only QL_USB_HID_0 is aviable now
	callback   [In] when data arrived, or send data done, the callback will be run
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_hid_open(ql_usb_hid_port_e port, ql_usb_hid_callback callback);

/*****************************************************************
* Function: ql_usb_hid_read
*
* Description: to read data from the hid port

* Parameters:
*	port	   [In] port number, and only QL_USB_HID_0 is aviable now
	buffer     [In] the memory to save data
	size	   [In] data size to be read
*
* Return:
*   >=         success
*   other      ql_errcode_usb_e
*****************************************************************/
int ql_usb_hid_read(ql_usb_hid_port_e port, uint8 *buffer, uint size);

/*****************************************************************
* Function: ql_usb_hid_write
*
* Description: to write data from the hid port

* Parameters:
*	port	   [In] port number, and only QL_USB_HID_0 is aviable now
	buffer     [In] the memory to save data
	size	   [In] data size to be write
*
* Return:
*   >=         success
*   other      ql_errcode_usb_e
*****************************************************************/
int ql_usb_hid_write(ql_usb_hid_port_e port, uint8 *buffer, uint size);

/*****************************************************************
* Function: ql_usb_hid_read_aviable
*
* Description: to get how many bytes can be read from hid port now

* Parameters:
*	port	   [In] port number, and only QL_USB_HID_0 is aviable now
*
* Return:
*   >=         success
*   other      ql_errcode_usb_e
*****************************************************************/
int ql_usb_hid_read_aviable(ql_usb_hid_port_e port);

/*****************************************************************
* Function: ql_usb_hid_write_aviable
*
* Description: to get how many bytes can be write to hid port now 

* Parameters:
*	port	   [In] port number, and only QL_USB_HID_0 is aviable now
*
* Return:
*   >=         success
*   other      ql_errcode_usb_e
*****************************************************************/
int ql_usb_hid_write_aviable(ql_usb_hid_port_e port);
#endif

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_USB_H */


