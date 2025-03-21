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

=================================================================*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_api_nw.h"
#include "ql_api_dev.h"
#include "ql_api_datacall.h"
#include "quec_pin_index.h"


#include "ql_api_ntrip_rtk.h"
#include "ql_uart.h"
#include "ql_gpio.h"

#include "sockets.h"
#include "lwip/ip_addr.h"
#include "lwip/ip6_addr.h"

#include "lwip/netdb.h"
#include "lwip/netif.h"
#include "lwip/inet.h"
#include "lwip/tcp.h"


#include "ql_log.h"
#include "ntrip_rtk_demo.h"


#define QL_NTRIP_RTK_DEMO_LOG_LEVEL       	        QL_LOG_LEVEL_INFO
#define QL_NTRIP_RTK_DEMO_LOG(msg, ...)			    QL_LOG(QL_NTRIP_RTK_DEMO_LOG_LEVEL, "ql_SPI_DEMO", msg, ##__VA_ARGS__)
#define QL_NTRIP_RTK_DEMO_LOG_PUSH(msg, ...)	    QL_LOG_PUSH("ql_SPI_DEMO", msg, ##__VA_ARGS__)


#define QL_GNSS_UART_RX_BUFF_SIZE                2048
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define GNSS_NMEA_END_CHAR_1 '\r'
#define GNSS_NMEA_END_CHAR_2 '\n'

#define QL_NTRIP_RTK_APP_START_SOCKET               (1001 | (QL_COMPONENT_APP_START<<16))
#define QL_NTRIP_RTK_APP_REAC_DATA                  (1002 | (QL_COMPONENT_APP_START<<16))
#define QL_NTRIP_RTK_APP_STATE_CHANGE               (1003 | (QL_COMPONENT_APP_START<<16))

#define QL_NTRIP_RTK_SERVER_IP                      "120.24.149.249"       
#define QL_NTRIP_RTK_SERVER_PORT                    7001                   
#define QL_NTRIP_RTK_SERVER_USER                    "HDcv6x5mc9t1nr"       
#define QL_NTRIP_RTK_SERVER_PWD                     "ce10b1c4";            


ql_task_t ntrip_rtk_demo_task = NULL;
static ql_ntrip_rtk_state_e ql_ntrip_demo_state = QL_NTRIP_RTK_IDLE;

void ql_gnss_uart_notify_cb(unsigned int ind_type, ql_uart_port_number_e port, unsigned int size)
{
    QL_NTRIP_RTK_DEMO_LOG("UART port %d receive ind type:0x%x, receive data size:%d", port, ind_type, size);
    switch(ind_type)
    {
        case QUEC_UART_RX_OVERFLOW_IND:  //rx buffer overflow
        case QUEC_UART_RX_RECV_DATA_IND:
        {
            ql_event_t ql_event = {0};
            ql_event.id = ind_type;
            ql_event.param1 = port;
            ql_event.param2 = size;
            ql_rtos_event_send(ntrip_rtk_demo_task,&ql_event);
            
            break;
        }
    }
}

void ql_ntrip_rtk_handle_cb(ql_ntrip_rtk_state_e state, char *data, int len)
{
    QL_NTRIP_RTK_DEMO_LOG("state=%d", state);
    ql_event_t ql_event = {0};

    if (QL_NTRIP_RTK_RECV_DATA == state)
    {
        //只有为QL_NTRIP_RTK_RECV_DATA时，需要处理数据
        if (data && len)
        {
            if (ntrip_rtk_demo_task)
            {
                char *rtk_data = (char *)malloc(len);
                if (rtk_data)
                {
                    ql_event.id = QL_NTRIP_RTK_APP_REAC_DATA;
                    ql_event.param1 = (int)rtk_data;
                    ql_event.param2 = len;
                    ql_rtos_event_send(ntrip_rtk_demo_task, &ql_event);
                } 
            } 
        }
    }
    else 
    {
        ql_event.id = QL_NTRIP_RTK_APP_STATE_CHANGE;
        ql_event.param1 = state;
        ql_rtos_event_send(ntrip_rtk_demo_task, &ql_event);
    }
}

char *ql_gnss_get_lastest_char(char *str, int len, char ch)
{
    char *get_char = NULL;

    if (NULL == str)
    {
        return NULL;
    }

    //QL_NTRIP_RTK_DEMO_LOG("len = %d", strlen(str));

    while (len--)
    {
        if (*str == ch)
        {
            get_char = str;
        }
        str++;
    }
    
    return get_char;
}

char *ql_gnss_get_typical_char(char *str, int len, char ch, int num)
{
    char *get_char = NULL;
    int get_num = 0;

    if (NULL == str)
    {
        return NULL;
    }

    //QL_NTRIP_RTK_DEMO_LOG("len = %d", strlen(str));

    while (len--)
    {
        if (*str == ch)
        {
            get_num++;
            if (get_num == num)
            {
                get_char = str;
                break;
            }
            
        }
        str++;
    }
    
    return get_char;
}


static void ql_ntrip_rtk_demo_pthread(void *arg)
{
    ql_errcode_ntrip_rtk_e ret = QL_NTRIP_RTK_SUCCESS;
    int profile_idx = 1;
    int i = 0;
    uint8_t nSim = 0;
    char ip4_addr_str[16] = {0};
    ql_data_call_info_s info;
    ql_uart_errcode_e uart_ret = 0;
    ql_errcode_gpio gpio_ret = 0;
    ql_uart_config_s uart_cfg = {0};
    ql_event_t ql_event = {0};
    unsigned char *recv_buff = calloc(1, QL_GNSS_UART_RX_BUFF_SIZE+1);
    unsigned int real_size = 0;
    int read_len = 0;
    unsigned int size = 0;
    unsigned char nmea_buff[256];
	char *start = NULL, *end = NULL;
    static int total_bytes = 0;
    char *gnss_nmea_buff = calloc(1, QL_GNSS_UART_RX_BUFF_SIZE+1);
    int len = 0;
    int ntrip_rtk_id = 0;
    ql_ntrip_rtk_config_s config = {0};
 
    ql_rtos_task_sleep_s(10);
        
	QL_NTRIP_RTK_DEMO_LOG("========== ntrip_rtk demo start ==========");
	QL_NTRIP_RTK_DEMO_LOG("wait for network register done");
		
	while((ret = ql_network_register_wait(nSim, 120)) != 0 && i < 10)
    {
    	i++;
		ql_rtos_task_sleep_s(1);
	}
    
	if(ret == 0)
    {
		i = 0;
		QL_NTRIP_RTK_DEMO_LOG("====network registered!!!!====");
	}
    else
    {
		QL_NTRIP_RTK_DEMO_LOG("====network register failure!!!!!====");
		goto exit;
	}

	ql_set_data_call_asyn_mode(nSim, profile_idx, 0);

	QL_NTRIP_RTK_DEMO_LOG("===start data call====");
	ret=ql_start_data_call(nSim, profile_idx, QL_PDP_TYPE_IP, NULL, NULL, NULL, 0); 
	QL_NTRIP_RTK_DEMO_LOG("===data call result:%d", ret);
	if(ret != 0)
    {
		QL_NTRIP_RTK_DEMO_LOG("====data call failure!!!!=====");
	}
	memset(&info, 0x00, sizeof(ql_data_call_info_s));
	
	ret = ql_get_data_call_info(nSim, profile_idx, &info);
	if(ret != 0)
    {
		QL_NTRIP_RTK_DEMO_LOG("ql_get_data_call_info ret: %d", ret);
		ql_stop_data_call(nSim, profile_idx);
		goto exit;
	}
    QL_NTRIP_RTK_DEMO_LOG("info->profile_idx: %d", info.profile_idx);
	QL_NTRIP_RTK_DEMO_LOG("info->ip_version: %d", info.ip_version);
            
	QL_NTRIP_RTK_DEMO_LOG("info->v4.state: %d", info.v4.state); 
	inet_ntop(AF_INET, &info.v4.addr.ip, ip4_addr_str, sizeof(ip4_addr_str));
	QL_NTRIP_RTK_DEMO_LOG("info.v4.addr.ip: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_NTRIP_RTK_DEMO_LOG("info.v4.addr.pri_dns: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip4_addr_str, sizeof(ip4_addr_str));
	QL_NTRIP_RTK_DEMO_LOG("info.v4.addr.sec_dns: %s\r\n", ip4_addr_str);

    /************************************************************/
    uart_cfg.baudrate = QL_UART_BAUD_115200;
    uart_cfg.flow_ctrl = QL_FC_NONE;
    uart_cfg.data_bit = QL_UART_DATABIT_8;
    uart_cfg.stop_bit = QL_UART_STOP_1;
    uart_cfg.parity_bit = QL_UART_PARITY_NONE;

    uart_ret = ql_uart_set_dcbconfig(QL_UART_PORT_2, &uart_cfg);
    QL_NTRIP_RTK_DEMO_LOG("ret: 0x%x", uart_ret);
	if(QL_UART_SUCCESS != uart_ret)
	{
		goto exit;
	}
	
	/***********************************************************
	Note start:
		1. If QL_UART_PORT_1 is selected for use, there is no need to set TX and RX pin and function
		2. According to the QuecOpen GPIO table, user should select the correct PIN to set function
		3. CTS and RTS pins (UART2 and UART3) also need to be initialized if hardware flow control function is required
	************************************************************/
	gpio_ret = ql_pin_set_func(QUEC_PIN_UART2_TXD, 0X01);
	if(QL_GPIO_SUCCESS != gpio_ret)
	{
		goto exit;
	}
	gpio_ret = ql_pin_set_func(QUEC_PIN_UART2_RXD, 0X01);
	if(QL_GPIO_SUCCESS != gpio_ret)
	{
		goto exit;
	}
    
    uart_ret = ql_uart_register_cb(QL_UART_PORT_2, ql_gnss_uart_notify_cb);
	
    uart_ret = ql_uart_open(QL_UART_PORT_2);
    QL_NTRIP_RTK_DEMO_LOG("ret: 0x%x", uart_ret);

    while(1)
    {
        if (ql_event_try_wait(&ql_event) == 0)
    	{
            if(ql_event.id == 0)
            {
                continue;
            }

            switch(ql_event.id)
            {
                case QUEC_UART_RX_OVERFLOW_IND:
                case QUEC_UART_RX_RECV_DATA_IND:
                {
                    size = ql_event.param2;
                    while(size > 0)
                    {
                        memset(recv_buff, 0, QL_GNSS_UART_RX_BUFF_SIZE+1);
                        real_size= MIN(size, QL_GNSS_UART_RX_BUFF_SIZE);
                        
                        read_len = ql_uart_read(ql_event.param1, recv_buff, real_size);
                        //QL_NTRIP_RTK_DEMO_LOG("read_len=%d, recv_data=%s", read_len, recv_buff);
                        if((read_len > 0) && (size >= read_len))
                        {
                            size -= read_len;
                        }
                        else
                        {
                            break;
                        }
                    }

                    total_bytes += ql_event.param2;
                    if (total_bytes > QL_GNSS_UART_RX_BUFF_SIZE-150) 
                    {
                        QL_NTRIP_RTK_DEMO_LOG("nmea data Overflow");
                        total_bytes = 0;
                        memset(gnss_nmea_buff, 0x00, QL_GNSS_UART_RX_BUFF_SIZE+1);
                        continue;
                    }  

                    QL_NTRIP_RTK_DEMO_LOG("total_bytes=%d", total_bytes);
                    memcpy(gnss_nmea_buff + total_bytes - ql_event.param2, recv_buff, ql_event.param2);
                    //QL_NTRIP_RTK_DEMO_LOG("gnss_nmea_buff=%s", gnss_nmea_buff);
                    if(ql_event.param2 > 0)
                    {  
                        //找NMEA语句
                        start = strstr(gnss_nmea_buff, "$GNGGA");
                        if (NULL == start) 
                        {
                            start = ql_gnss_get_lastest_char(gnss_nmea_buff, total_bytes, '$');
                            if (start)
                            {
                                len = start - gnss_nmea_buff;
                                //QL_NTRIP_RTK_DEMO_LOG("len = %d", len);
                                memmove(gnss_nmea_buff, start, total_bytes - len);
                                total_bytes = total_bytes - len;
                                //QL_NTRIP_RTK_DEMO_LOG("total_bytes=%d", total_bytes);
                                memset(gnss_nmea_buff+total_bytes, 0x00, QL_GNSS_UART_RX_BUFF_SIZE + 1 - total_bytes);
                            }
                            else 
                            {
                                //不存在$字符
                                total_bytes = 0;
                                memset(gnss_nmea_buff, 0x00, QL_GNSS_UART_RX_BUFF_SIZE+1);
                            }
                           
                            continue;
                        }
                        
                        //找\r，\n
                        end = memchr(start, GNSS_NMEA_END_CHAR_1, total_bytes - (start - gnss_nmea_buff ));
                        if (end == NULL)
                        {
                            //有$号，但是没有\r
                            continue;
                        }
                        if (GNSS_NMEA_END_CHAR_2 != *(++end)) 
                        {
                            //有$号，有\r，但是没有\n
                            continue;
                        }

                        //找到对应的nmea语句
                        memset(nmea_buff, 0x00, sizeof(nmea_buff));
                        len = end - start - 1;
                        memcpy(nmea_buff, start, len );
                        QL_NTRIP_RTK_DEMO_LOG("real_nmea_buff=%s", nmea_buff);
                        //判断是否已定位
                        start =  ql_gnss_get_typical_char((char *)nmea_buff, len, ',', 6);
                        QL_NTRIP_RTK_DEMO_LOG("locate=%c", start[1]);
                        if ('1' == start[1] && QL_NTRIP_RTK_IDLE == ql_ntrip_demo_state)
                        {
                            ql_ntrip_demo_state = QL_NTRIP_RTK_CONNECTING;
                            ql_event.id = QL_NTRIP_RTK_APP_START_SOCKET;
                            ql_rtos_event_send(ntrip_rtk_demo_task, &ql_event);
                        }

                        if (QL_NTRIP_RTK_POINTED == ql_ntrip_demo_state || QL_NTRIP_RTK_RECV_DATA == ql_ntrip_demo_state)
                        {
                            ql_ntrip_rtk_send(ntrip_rtk_id, (char *)nmea_buff, len);
                        }
                        
                        total_bytes = 0;
                        memset(gnss_nmea_buff, 0x00, QL_GNSS_UART_RX_BUFF_SIZE+1);   
                    }

                    QL_NTRIP_RTK_DEMO_LOG("total_bytes=%d", total_bytes);
                }
                break;
                case QL_NTRIP_RTK_APP_START_SOCKET:
                {
                    config.sim_id = 0;
                    config.pdp_cid = 1;
                    config.ip = QL_NTRIP_RTK_SERVER_IP;
                    config.port = QL_NTRIP_RTK_SERVER_PORT;
                    config.user = QL_NTRIP_RTK_SERVER_USER;
                    config.pwd = QL_NTRIP_RTK_SERVER_PWD;
                    config.point = "RTCM32";
                    config.gnss_type = "GPGGA";
                    config.func = ql_ntrip_rtk_handle_cb;
                    ret = ql_ntrip_rtk_init(ntrip_rtk_id, &config);
                    if (ret != QL_NTRIP_RTK_SUCCESS)
                    {
                        QL_NTRIP_RTK_DEMO_LOG("init err=%x", ret);
                        goto exit;
                    }
                }
                break;
                case QL_NTRIP_RTK_APP_REAC_DATA:
                {
                    ql_ntrip_demo_state = QL_NTRIP_RTK_RECV_DATA;
                    char *rtk_data = (char *)ql_event.param1;
                    if (rtk_data)
                    {
                        ql_uart_write(QL_UART_PORT_2, (unsigned char *)rtk_data, ql_event.param2);
                        free(rtk_data);
                    }
#if 0
                    //断开差分服务器连接，只是为了测试ql_ntrip_rtk_close接口功能，实际应用需要屏蔽掉
                    static int count = 0;
                    if (count++ > 20)
                    {
                        count = 0;
                        ql_ntrip_rtk_close(ntrip_rtk_id);
                    }
                    QL_NTRIP_RTK_DEMO_LOG("test_count=%d", count);
#endif
                }
                break;
                case QL_NTRIP_RTK_APP_STATE_CHANGE:
                {
                    ql_ntrip_demo_state = ql_event.param1;
                    if (QL_NTRIP_RTK_IDLE == ql_ntrip_demo_state)
                    {
                        ql_ntrip_demo_state = QL_NTRIP_RTK_CONNECTING;
                        ql_event.id = QL_NTRIP_RTK_APP_START_SOCKET;
                        ql_rtos_event_send(ntrip_rtk_demo_task, &ql_event);
                    }
                }
                break;
                default:
                break;
            } 
         }
    }
exit:
    ql_uart_close(QL_UART_PORT_2);
    if (recv_buff)
    {
        free(recv_buff);
    }

    if (gnss_nmea_buff)
    {
        free(gnss_nmea_buff);
    }

    
    ql_rtos_task_delete(NULL);	
}

QlOSStatus ql_ntrip_rtk_demo_init(void)
{	
	QlOSStatus err = QL_OSI_SUCCESS;
	err = ql_rtos_task_create(&ntrip_rtk_demo_task, NTRIP_RTK_DEMO_TASK_STACK_SIZE,NTRIP_RTK_DEMO_TASK_PRIO, "ql_ntrip_rtk_demo", ql_ntrip_rtk_demo_pthread, NULL, NTRIP_RTK_DEMO_TASK_EVENT_CNT);
	if(err != QL_OSI_SUCCESS)
	{
		QL_NTRIP_RTK_DEMO_LOG("demo_task created failed");
        return err;
	}
    
    return err;
}

