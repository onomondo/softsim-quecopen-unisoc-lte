/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_usb.h"
#include "usb_demo.h"
#include "ql_app_feature_config.h"

#define QL_USB_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_USB_LOG(msg, ...)			QL_LOG(QL_USB_LOG_LEVEL, "ql_usb", msg, ##__VA_ARGS__)
#define QL_USB_LOG_PUSH(msg, ...)		QL_LOG_PUSH("ql_usb", msg, ##__VA_ARGS__)

#define QL_USB_MASS_STORAGE	0 //验证mass storage功能打开此宏
#define QL_USB_HID_DEVICE	0 //验证模块作为HID从机功能打开此宏
#define QL_USB_DEVICE_CCID_BLOCK_MSG_TEST     0 //验证USB CCID BLOCK msg

#define HID_PACKET_SIZE		64 //不能超过64字节
/*========================================================================
 *  Global Variable
 *========================================================================*/
#if QL_USB_HID_DEVICE
#include "hid.h"

//usb hid报告描述符
static const unsigned char gQusbHidReportDesc[] =
{
    UsagePage(USB_HID_GENERIC_DESKTOP),
    Usage(0),
    Collection(USB_HID_APPLICATION),
    LogicalMinimum(0),
    LogicalMaximum(255),
    ReportSize(8),					//一个字节
    ReportCount(HID_PACKET_SIZE),	//单条报文长度为HID_PACKET_SIZE字节,
    Usage(1),
    Input(USB_HID_INPUT_DATA | USB_HID_INPUT_VARIABLE | USB_HID_INPUT_ABS),
    Usage(1),
    Output(USB_HID_OUTPUT_DATA | USB_HID_OUTPUT_VARIABLE | USB_HID_OUTPUT_ABS),
    Usage(1),
    Feature(USB_HID_FEATURE_VARIABLE),
    EndCollection
};

//字符串描述符
static const char *gStringDescripter = "QUECTEL_HID_DEVICE";
#endif

#if QL_USB_DEVICE_CCID_BLOCK_MSG_TEST
static int ql_usb_ccid_user_recv_data_cb(void *buff,uint16_t len)
{
    uint8 read_buff[512];
    uint8 write_buff[2] = {0x90,0x00};
    QL_USB_LOG("recive data len:%d",len);
    ql_usb_ccid_read(read_buff,len);
    ql_usb_ccid_write(write_buff,2);
    return 0;
}

static uint8_t ql_usb_ccid_user_poweron(void)
{
    QL_USB_LOG("power on cb");
    return 0;
}

static uint8_t ql_usb_ccid_user_poweroff(void)
{
    QL_USB_LOG("power off cb");
    return 0;
}

#endif
/*========================================================================
 *  Function
 *========================================================================*/
#if QL_USB_HID_DEVICE
static void ql_usb_hid_enable(bool enable, bool effect_now)
{
	ql_usb_hid_cfg_t hid_cfg = {0};
	ql_usb_hid_get_config(&hid_cfg);

	QL_USB_LOG("qhid enable flag: %d", hid_cfg.enabled);

	//对比当前需要配置的参数,是否和nv中保存的参数一致
	if(hid_cfg.enabled == enable &&
	   !memcmp(hid_cfg.report_desc, gQusbHidReportDesc, sizeof(gQusbHidReportDesc)) && \
	   !memcmp(hid_cfg.string_desc, gStringDescripter, strlen(gStringDescripter)))
	{
		QL_USB_LOG("qhid same config");
		return;	//参数一致,不需要再配置
	}

	if(enable)
	{	
		hid_cfg.enabled = true;
		hid_cfg.report_desc_size = sizeof(gQusbHidReportDesc);
		hid_cfg.string_desc_size = strlen(gStringDescripter);

		//用户只需要配置报告描述符,设备描述符和配置描述符内核已经配置好
		memcpy(hid_cfg.report_desc, gQusbHidReportDesc, hid_cfg.report_desc_size);
		memcpy(hid_cfg.string_desc, gStringDescripter, hid_cfg.string_desc_size);
		ql_usb_hid_set_config(&hid_cfg);		//该配置掉电保存
		ql_usb_set_enum_mode(QL_USB_ENMU_HID); 	//配置枚举时加载usb hid设备
	}
	else
	{
		hid_cfg.enabled = false;
		ql_usb_hid_set_config(&hid_cfg); 				//该配置掉电保存
		ql_usb_set_enum_mode(QL_USB_ENUM_USBNET_COM);	//配置枚举默认端口
	}

	//配置立即生效; 代码运行到APP时,USB枚举已经完成了,此时要让USB重新枚举就需要重启生效,或者
	//重新加载USB(注意:重新加载USB时,所有USB口都会断开连接,然后重新加载)
	if(effect_now)
	{
		QL_USB_LOG("hid_flag reset usb......");
		ql_usb_disable();
		ql_rtos_task_sleep_ms(500);
		ql_usb_enable(); //重新枚举USB
	}
}

static void ql_usb_hid_process(uint32 ind_type, ql_usb_hid_port_e port, uint32 size)
{
	int r_size = 0, ret = 0;
	uint8 rx_data_buf[HID_PACKET_SIZE + 1] = {0};
	
	QL_USB_LOG("hid event %d %d %d", ind_type, port, size);

	if(ind_type & QL_USB_RX_ARRIVE)
	{
		r_size = ql_usb_hid_read_aviable(port);
		if(r_size > 0)
		{
			ret = ql_usb_hid_read(port, rx_data_buf, r_size);
			QL_USB_LOG("qhid read %d [%s]", ret, rx_data_buf);

			ret = ql_usb_hid_write(port, rx_data_buf, r_size);
			QL_USB_LOG("qhid write %d bytes", ret, rx_data_buf);
		}
	}

	if(ind_type & QL_USB_TX_COMPLETE)
	{
		QL_USB_LOG("qhid transfer done");
	}
}
#endif

static void ql_usb_demo_thread(void *param)
{
	QL_USB_LOG("enter usb demo");

#if QL_USB_MASS_STORAGE
	ql_usb_msc_cfg_t msc_cfg = {0};
/*
	配置映射模块的内置flash/sd卡/6线flash等存储器到PC端, 模块作为虚拟U盘
	注意：
		  1. 在target.config中,打开CONFIG_QUEC_PROJECT_FEATURE_USB_MASS_STORAGE宏
		  2. protocol参数中支持MTP和MSG两种协议,差异详见ql_usb_protocol_e定义处
		  3. 烧录代码后第一次开机进app可能会比较慢,如果sd卡/外置flash在app挂载,则烧录后第一次开机可能虚拟不出sd卡/外置flash,后续开机不会无法映射
*/
	msc_cfg.msc_device = QL_USB_MSC_SDCARD; //如需映射2个存储器,可用"|"连接,如同时映射外置6线flash和sd卡: QL_USB_MSC_EFS | QL_USB_MSC_SDCARD

/*
	1. MTP协议和MSC协议的差异见ql_usb_protocol_e定义处
	2. 注意MSC协议下PC枚举出虚拟U盘后,模组端将无法访问SD卡,SD卡的读写权限全部交给PC
*/
	msc_cfg.protocol = QL_USB_PROTOCOL_MTP;

/*
	MTP协议下, 已经加载过一次虚拟U盘的PC,如果要修改msc_cfg.dev_name,可能需要卸载重装一次QUECTEL的USB驱动,因为部分windows系统会记住第一次识别到的
	设备名称,只有卸载重装一次USB驱动才会去刷新虚拟U盘设备名称, MSC协议下无此问题
*/
	strcpy(msc_cfg.dev_name, "ANDROID");

	//msc_cfg.option.hide_option = QL_USB_MSC_HIDE_PART2; //MSC协议下,SD卡有2个分区时,选择是否隐藏一个分区,只映射一个分区到PC上
	//msc_cfg.option.protect_dsiable = false; //MSC协议下,是否取消SD卡保护(PC枚举出虚拟U盘后,模组端将无法访问SD卡)

	ql_usb_set_enum_mode(QL_USB_ENUM_MASS_STORAGE); //重启生效
	ql_usb_msc_config_set(&msc_cfg); //重启生效

/*
	//若需要立即生效,可使能此处代码,调用后模块的usb会先断开连接然后重新连接
	//ql_usb_disable();
	//ql_rtos_task_sleep_s(1);
	//ql_usb_enable();
*/
#endif

#if QL_USB_DEVICE_CCID_BLOCK_MSG_TEST

    ql_usb_set_enum_mode(QL_USB_ENUM_USBNET_COM_CCID);

    ql_usb_ccid_usr_cb_t cb = {0};
    cb.usrccidXfrBlockCb = ql_usb_ccid_user_recv_data_cb;
    cb.usrccidGetSlotStatusCb = NULL;
    cb.usrccidIccPowerOffCb = ql_usb_ccid_user_poweroff;
    cb.usrCcidIccPowerOnCb = ql_usb_ccid_user_poweron;
    ql_usb_ccid_usr_cb_register(cb);
    ql_usb_disable();
	ql_rtos_task_sleep_s(1);
	ql_usb_enable();

#endif

#if QL_USB_HID_DEVICE
/*
	1. 模块作为HID从设备
	2. 需要在target.config中,打开CONFIG_QUEC_PROJECT_FEATURE_USB_HID_DEVICE宏
*/
	ql_usb_hid_enable(true, true); //使能USB HID并立即生效
	ql_usb_hid_open(QL_USB_HID_0, ql_usb_hid_process);
#endif

/*
	使用默认的端口组合,配置枚举哪些USB端口,也可以直接在quec_usb_serial_create中配置,该函数开源在app_start.c中
	ql_usb_set_enum_mode(QL_USB_ENUM_USBNET_COM);
*/
	ql_rtos_task_delete(NULL);
}

void ql_usb_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;
	ql_task_t ql_usb_task = NULL;
	
    QL_USB_LOG("usb demo enter");
	
    err = ql_rtos_task_create(&ql_usb_task, QL_USB_TASK_STACK, APP_PRIORITY_NORMAL, "ql_usb_demo", ql_usb_demo_thread, NULL, 5);
	if(err != QL_OSI_SUCCESS)
    {
		QL_USB_LOG("usb task create failed");
	}
}


