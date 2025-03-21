/**  
  @file
  rs485_demo.c

  @brief
  quectel rs485_demo.

*/
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
07/06/2021        Neo         Init version
=================================================================*/




/*===========================================================================
 * include files
 ===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_uart.h"
#include "ql_gpio.h"
#include "ql_pin_cfg.h"


/*===========================================================================
 *Definition
 ===========================================================================*/
#define QL_RS485_DEMO_LOG_LEVEL			QL_LOG_LEVEL_INFO
#define QL_RS485_DEMO_LOG(msg, ...)		QL_LOG(QL_RS485_DEMO_LOG_LEVEL, "ql_rs485_demo", msg, ##__VA_ARGS__)


#define QL_RS485_TASK_STACK_SIZE     		4096
#define QL_RS485_TASK_PRIO          	 	APP_PRIORITY_HIGH           //ensure the priority of the task
#define QL_RS485_TASK_EVENT_CNT      		10
#define QL_RS485_WRITE_WAIT_TIMEOUT         500                         //send timeout


#define QL_UART_RX_BUFF_SIZE                2048
#define QL_UART_TX_BUFF_SIZE                2048

#define MIN(a,b) ((a) < (b) ? (a) : (b))



/*===========================================================================
 * Variate
 ===========================================================================*/
ql_task_t rs485_task = NULL;



/*===========================================================================
 * Functions
 ===========================================================================*/
void ql_rs485_notify_cb(unsigned int ind_type, ql_uart_port_number_e port, unsigned int size)
{    
    //QL_RS485_DEMO_LOG("UART port %d receive ind type:0x%x, receive data size:%d", port, ind_type, size);
    ql_event_t event;
    
    switch(ind_type)
    {
        case QUEC_UART_RX_OVERFLOW_IND:  //rx buffer overflow
        {
            QL_RS485_DEMO_LOG("rx overflow");
        }

        case QUEC_UART_RX_RECV_DATA_IND:
        {
            QL_RS485_DEMO_LOG("rx data coming");
            if(size > 0)
            {
                event.id = QUEC_UART_RX_RECV_DATA_APP_IND;
                event.param1 = size;
                ql_rtos_event_send(rs485_task, &event);
            }
            break;
        }

        case QUEC_UART_TX_FIFO_COMPLETE_IND:
        {
            QL_RS485_DEMO_LOG("tx fifo complete");
            event.id = QUEC_UART_TX_COMPLETE_APP_IND;
            ql_rtos_event_send(rs485_task, &event);
            break;
        }
    }
}

int ql_rs485_init(ql_uart_port_number_e port, ql_uart_config_s *dcb, ql_uart_callback uart_cb)
{
    int ret = 0;
    ql_uart_config_s dcb_uart;

    ret = ql_uart_set_dcbconfig(port, dcb);
	if(QL_UART_SUCCESS != ret)
	{
	    QL_RS485_DEMO_LOG("ret: 0x%x", ret);
		return 1;
	}

    ret = ql_uart_open(port);
	if(QL_UART_SUCCESS != ret)
	{
	    QL_RS485_DEMO_LOG("ret: 0x%x", ret);
	    return 1;
    }
    ret = ql_uart_register_cb(port, uart_cb);
    if(QL_UART_SUCCESS != ret)
	{
	    QL_RS485_DEMO_LOG("ret: 0x%x", ret);
	    return 1;
    }

    memset(&dcb_uart, 0, sizeof(ql_uart_config_s));
    ret = ql_uart_get_dcbconfig(port, &dcb_uart);
    if(QL_UART_SUCCESS != ret)
	{
	    QL_RS485_DEMO_LOG("ret: 0x%x", ret);
	    return 1;
    }
    
    QL_RS485_DEMO_LOG("ret: 0x%x, baudrate=%d, flow_ctrl=%d, data_bit=%d, stop_bit=%d, parity_bit=%d", 
                       ret, dcb_uart.baudrate, dcb_uart.flow_ctrl, dcb_uart.data_bit, dcb_uart.stop_bit, dcb_uart.parity_bit);

    return 0;
}

int ql_rs485_deinit(ql_uart_port_number_e port)
{
    int ret = 0;
    ret = ql_uart_close(port);
    if(ret)
    {
        QL_RS485_DEMO_LOG("ret: 0x%x", ret);
        return 1;
    }
    
    return 0;
}

//when the module is in the write state, the data sent by host will be lost
int ql_rs485_write(ql_uart_port_number_e port, unsigned char *data, unsigned int data_len)
{
    int write_len = 0;
    ql_uart_tx_status_e tx_status;
    ql_uart_errcode_e ret;
    
    ql_gpio_set_level(QL_RS485_GPIO_NUM, LVL_HIGH);
    write_len = ql_uart_write(port, data, data_len);
    while(1)
    {
        //wait for FIFO data transmission to complete
        ret = ql_uart_get_tx_fifo_status(port, &tx_status);
        if(ret)
        {
            break;
        }
        if(tx_status == QL_UART_TX_COMPLETE)
        {
            break;
        }
    }

    //UART FIFO is counted in bytes. When FIFO is empty, there may still be several bits that are not sent out in the hardware circuit, 
    //including stop bits, so it takes some time to delay at low baud rate
    //The baud rate is 115200 without delay. The baud rate is 9600. It is recommended to delay 180us. 
    //The specific delay time needs to be combined with the actual test
    //ql_delay_us(180);
    
    //after writing, pull down directly to make the module in the state of receiving data
    ql_gpio_set_level(QL_RS485_GPIO_NUM, LVL_LOW);
    
    return write_len;
}

//the serial port cache is only 4K bytes. after receiving the data, it should be taken away as soon as possible. If the cache is full, it will lose the data
int ql_rs485_read(ql_uart_port_number_e port, unsigned char *data, unsigned int data_len)
{
    ql_gpio_set_level(QL_RS485_GPIO_NUM, LVL_LOW);
    return ql_uart_read(port, data, data_len);
}

static void ql_rs485_demo_thread(void *param)
{
    int ret = 0;
	QlOSStatus err = 0;
    ql_event_t event;
    ql_uart_config_s uart_cfg = {0};
    int write_len = 0, read_len = 0;
    unsigned int real_size = 0, size = 0;
    uint32_t event_mask = 0;
    
    unsigned char data[] = "hello rs485 demo\r\n";
    unsigned char *recv_buff = calloc(1, QL_UART_RX_BUFF_SIZE+1);
    
	//step1:uart pin init
	//1. If QL_UART_PORT_1 is selected for use, there is no need to set TX and RX pin and function
	//2. According to the QuecOpen GPIO table, user should select the correct PIN to set function
    ret = ql_pin_set_func(QL_UART2_TX_PIN, QL_UART2_TX_FUNC);
    if(QL_GPIO_SUCCESS != ret)
    {
        goto exit;
    }
    ret = ql_pin_set_func(QL_UART2_RX_PIN, QL_UART2_RX_FUNC);
    if(QL_GPIO_SUCCESS != ret)
    {
        goto exit;
    }

    //step2:gpio pin init, default pull down, 485 needs a gpio pin to control the converter send or receive
    ret = ql_pin_set_func(QL_RS485_PIN_GPIO, QL_RS485_PIN_GPIO_FUNC_GPIO);
    if(QL_GPIO_SUCCESS != ret)
    {
        goto exit;
    }
    ql_gpio_deinit(QL_RS485_GPIO_NUM);
    ql_gpio_init(QL_RS485_GPIO_NUM, GPIO_OUTPUT, QUEC_PIN_NONE, LVL_LOW);

    //step3:set event mask
    event_mask = QL_UART_EVENT_RX_ARRIVED | QL_UART_EVENT_RX_OVERFLOW;
    ret = ql_uart_set_event_mask(QL_UART_PORT_2, event_mask);
	if(QL_UART_SUCCESS != ret)
	{
		goto exit;
	}

    //step4:rs485 init, baud rate and other parameters need to be configured
    uart_cfg.baudrate = QL_UART_BAUD_115200;
    uart_cfg.flow_ctrl = QL_FC_NONE;
    uart_cfg.data_bit = QL_UART_DATABIT_8;
    uart_cfg.stop_bit = QL_UART_STOP_1;
    uart_cfg.parity_bit = QL_UART_PARITY_NONE;

    ret = ql_rs485_init(QL_UART_PORT_2, &uart_cfg, ql_rs485_notify_cb);
	if(ret)
	{
		goto exit;
	}

    //step5:test rs485 read and write function     
    while(1)
    {
        ql_event_try_wait(&event);
        if (event.id == QUEC_UART_RX_RECV_DATA_APP_IND)
        {
            size = event.param1;
            while(size > 0)
            {
                memset(recv_buff, 0, QL_UART_RX_BUFF_SIZE+1);
                real_size= MIN(size, QL_UART_RX_BUFF_SIZE);
                
                read_len = ql_rs485_read(QL_UART_PORT_2, recv_buff, real_size);
                QL_RS485_DEMO_LOG("read_len=%d, recv_data=%s", read_len, recv_buff);
                if((read_len > 0) && (size >= read_len))
                {
                    size -= read_len;
                }
                else
                {
                    break;
                }
            }
        }

        write_len = ql_rs485_write(QL_UART_PORT_2, data, strlen((char *)data));
        QL_RS485_DEMO_LOG("write_len:%d", write_len);
    }

exit:
    QL_RS485_DEMO_LOG("ret: 0x%x", ret);
    free(recv_buff);
    err = ql_rtos_task_delete(NULL);
	if(err != QL_OSI_SUCCESS)
	{
		QL_RS485_DEMO_LOG("task deleted failed");
	}
}

void ql_rs485_app_init(void)
{
	QlOSStatus err = 0;
	
	err = ql_rtos_task_create(&rs485_task, QL_RS485_TASK_STACK_SIZE, QL_RS485_TASK_PRIO, "QRS485DEMO", ql_rs485_demo_thread, NULL, QL_RS485_TASK_EVENT_CNT);
	if (err != QL_OSI_SUCCESS)
	{
		QL_RS485_DEMO_LOG("demo task created failed");
        return;
	}
}


