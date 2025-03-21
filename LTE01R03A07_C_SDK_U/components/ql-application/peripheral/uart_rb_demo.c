
/*================================================================
  Copyright (c) 2022, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/

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
#define QL_UART_DEMO_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_UART_DEMO_LOG(msg, ...) QL_LOG(QL_UART_DEMO_LOG_LEVEL, "main_uart", msg, ##__VA_ARGS__)

#define QL_UART_TASK_STACK_SIZE 4096
#define QL_UART_TASK_PRIO APP_PRIORITY_NORMAL
#define QL_UART_TASK_EVENT_CNT 5

#define QL_UART_RX_BUFF_SIZE 2048
#define QL_UART_TX_BUFF_SIZE 2048
#define QL_RING_BUFF_SIZE 4096

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/*===========================================================================
 * Variate
 ===========================================================================*/
typedef struct
{
    unsigned char *buff;
    volatile unsigned int wp;
    volatile unsigned int rp;
    volatile unsigned int length;
    unsigned int size;
} ring_buff_t;

static ring_buff_t ql_ringbuf = {0};
static ql_task_t uart_task_main = NULL;
static ql_task_t uart_process_task = NULL;
static ql_timer_t uart_process_timer = NULL;
static ql_sem_t process_data_sem = NULL;

/*===========================================================================
 * Functions
 ===========================================================================*/
void rb_init(ring_buff_t *rb, unsigned char *buff, unsigned int size);
void rb_clear(ring_buff_t *rb);
unsigned int rb_write(ring_buff_t *rb, const unsigned char *data, unsigned int write_len);
unsigned int rb_read(ring_buff_t *rb, unsigned char *data, unsigned int read_len);
unsigned int rb_used_len(ring_buff_t *rb);
unsigned int rb_free_len(ring_buff_t *rb);

/**
 * @Brief : Initialize the ringbuffer object
 * @param  rb
 * @param  buff
 * @param  size
 */
void rb_init(ring_buff_t *rb, unsigned char *buff, unsigned int size)
{
    rb->buff = buff;
    rb->size = size;
    rb->length = 0;
    rb->rp = 0;
    rb->wp = 0;
    memset(rb->buff, 0, rb->size);
}

/**
 * @Brief : Clearing a specified ringbuffer
 * @param  rb
 */
void rb_clear(ring_buff_t *rb)
{
    memset(rb->buff, 0, rb->size);
    rb->length = 0;
    rb->rp = 0;
    rb->wp = 0;
}

/**
 * @Brief : Writes data to a ringbuffer
 * @param  rb
 * @param  data
 * @param  write_len
 * @return unsigned int 
 */
unsigned int rb_write(ring_buff_t *rb, const unsigned char *data, unsigned int write_len)
{
    unsigned int write_a, write_b;

    if (rb == NULL || data == NULL)
    {
        return 0;
    }

    if (rb->length + write_len > rb->size)
    {
        return 0;
    }

    if (rb->size - rb->wp < write_len)
    {
        write_a = rb->size - rb->wp;
        write_b = write_len - write_a;
    }
    else
    {
        write_a = write_len;
        write_b = 0;
    }

    if (write_b != 0)
    {
        memcpy(rb->buff + rb->wp, data, write_a);
        memcpy(rb->buff, data + write_a, write_b);
        rb->length +=  write_len;
        rb->wp = write_b;
    }
    else
    {
        memcpy(rb->buff + rb->wp, data, write_a);
        rb->length +=  write_len;
        rb->wp += write_a;
        if (rb->wp == rb->size)
            rb->wp = 0;
    }

    return write_len;

}

/**
 * @Brief : Read data from a circular ringbuffer
 * @param  rb
 * @param  data
 * @param  read_len
 * @return unsigned int
 */
unsigned int rb_read(ring_buff_t *rb, unsigned char *data, unsigned int read_len)
{
    unsigned int real_len, read_a, read_b;
    if (rb == NULL || data == NULL)
    {
        return 0;
    }

    real_len = MIN(rb->length, read_len);

    if (rb->rp + real_len > rb->size)
    {
        read_a = rb->size - rb->rp;
        read_b = real_len - read_a;
    }
    else
    {
        read_a = real_len;
        read_b = 0;
    }

    if (read_b != 0)
    {
        memcpy(data, rb->buff + rb->rp, read_a);
        memcpy(data + read_a, rb->buff, read_b);
        rb->rp = read_b;
        rb->length -= real_len;
    }
    else
    {
        memcpy(data, rb->buff + rb->rp, read_a);
        rb->rp += read_a;
        rb->length -= real_len;
        if (rb->rp == rb->size)
            rb->rp = 0;
    }

    return real_len;
}

/**
 * @Brief : Returns the size of the stored space
 * @param  rb
 * @return unsigned int 
 */
unsigned int rb_used_len(ring_buff_t *rb)
{
    return rb->length;
}

/**
 * @Brief : Returns the size of free space
 * @param  rb
 * @return unsigned int 
 */
unsigned int rb_free_len(ring_buff_t *rb)
{
    return (rb->size - rb->length);
}

static void uart_process_callback(void *ctx)
{
    ql_rtos_semaphore_release(process_data_sem);
}

void ql_uart_notify_cb(unsigned int ind_type, ql_uart_port_number_e port, unsigned int size)
{
    unsigned char *recv_buff = calloc(1, QL_UART_RX_BUFF_SIZE + 1);
    unsigned int real_size = 0;
    int read_len = 0;

    QL_UART_DEMO_LOG("UART port %d receive ind type:0x%x, receive data size:%d", port, ind_type, size);
    switch (ind_type)
    {
    case QUEC_UART_RX_OVERFLOW_IND: // rx buffer overflow
    case QUEC_UART_RX_RECV_DATA_IND: {
        while (size > 0)
        {
            if (ql_rtos_timer_is_running(uart_process_timer))
            {
                ql_rtos_timer_stop(uart_process_timer);
            }

            memset(recv_buff, 0, QL_UART_RX_BUFF_SIZE + 1);
            real_size = MIN(size, QL_UART_RX_BUFF_SIZE);
            read_len = ql_uart_read(port, recv_buff, real_size);
            QL_UART_DEMO_LOG("read_len=%d, recv_data=%s", read_len, recv_buff);

            // put the read UART data into the RingBuffer
            rb_write(&ql_ringbuf, recv_buff, read_len);

            // You can modify the timeout to use as a frame of data
            ql_rtos_timer_start(uart_process_timer, 50, 0);
            if ((read_len > 0) && (size >= read_len))
            {
                size -= read_len;
            }
            else
            {
                break;
            }
        }
        break;
    }
    case QUEC_UART_TX_FIFO_COMPLETE_IND: {
        QL_UART_DEMO_LOG("tx fifo complete");
        break;
    }
    }
    free(recv_buff);
    recv_buff = NULL;
}

static void ql_uart_main(void *param)
{
    int ret = 0;
    QlOSStatus err = 0;
    ql_uart_config_s uart_cfg = {0};
    ql_event_t event = {0};

    QL_UART_DEMO_LOG("ql_uart_demo_main comming\n");

    uart_cfg.baudrate = QL_UART_BAUD_115200;
    uart_cfg.flow_ctrl = QL_FC_NONE;
    uart_cfg.data_bit = QL_UART_DATABIT_8;
    uart_cfg.stop_bit = QL_UART_STOP_1;
    uart_cfg.parity_bit = QL_UART_PARITY_NONE;

    ret = ql_uart_set_dcbconfig(QL_UART_PORT_1, &uart_cfg);
    if (QL_UART_SUCCESS != ret)
    {
        QL_UART_DEMO_LOG("ql_uart_set_dcbconfig fail\n");
        goto exit;
    }

    ret = ql_uart_open(QL_UART_PORT_1);
    QL_UART_DEMO_LOG("ret: 0x%x", ret);

    if (QL_UART_SUCCESS == ret)
    {
        ret = ql_uart_register_cb(QL_UART_PORT_1, ql_uart_notify_cb);
        if (QL_UART_SUCCESS != ret)
        {
            goto exit;
        }
    }

    while (1)
    {
        ql_event_try_wait(&event);
        QL_UART_DEMO_LOG("enter task, id is %d", event.id);
    }

exit:
    QL_UART_DEMO_LOG("exit uart task\n");

    err = ql_rtos_task_delete(NULL);
    if (err != QL_OSI_SUCCESS)
    {
        QL_UART_DEMO_LOG("task deleted failed");
    }
}

static void ql_uart_process(void *param)
{
    QlOSStatus err = 0;
    unsigned char *rb_buf = NULL;
    unsigned char *read_buf = NULL;
    unsigned int valid_size = 0;

    rb_buf = (unsigned char *)calloc(1, QL_RING_BUFF_SIZE);
    if (rb_buf == NULL)
    {
        QL_UART_DEMO_LOG("No memory");
        goto exit;
    }

    rb_init(&ql_ringbuf, rb_buf, QL_RING_BUFF_SIZE);
    while (1)
    {
        ql_rtos_semaphore_wait(process_data_sem, QL_WAIT_FOREVER);

        valid_size = rb_used_len(&ql_ringbuf);
        if (valid_size != 0)
        {
            read_buf = (unsigned char *)calloc(1, valid_size + 1);
            err = rb_read(&ql_ringbuf, read_buf, valid_size);
            //Process your data
            QL_UART_DEMO_LOG("rb read len:%d,buf=%s", err, read_buf);
        }

        if (read_buf != NULL)
        {
            free(read_buf);
            read_buf = NULL;
        }
    }

exit:
    QL_UART_DEMO_LOG("exit uart process task\n");

    err = ql_rtos_task_delete(NULL);
    if (err != QL_OSI_SUCCESS)
    {
        QL_UART_DEMO_LOG("task deleted failed");
    }
}

void ql_uart_rb_app_init(void)
{
    QlOSStatus err = 0;

    err = ql_rtos_task_create(&uart_task_main, QL_UART_TASK_STACK_SIZE, QL_UART_TASK_PRIO, "uart_task", ql_uart_main, NULL, QL_UART_TASK_EVENT_CNT);
    if (err != QL_OSI_SUCCESS)
    {
        QL_UART_DEMO_LOG("Uart Task created failed");
        return;
    }

    err = ql_rtos_task_create(&uart_process_task, QL_UART_TASK_STACK_SIZE * 4, QL_UART_TASK_PRIO, "uart_process_task", ql_uart_process, NULL, QL_UART_TASK_EVENT_CNT);
    if (err != QL_OSI_SUCCESS)
    {
        QL_UART_DEMO_LOG("Uart process Task created failed");
        return;
    }

    err = ql_rtos_timer_create(&uart_process_timer, uart_task_main, uart_process_callback, NULL);
    if (err != QL_OSI_SUCCESS)
    {
        QL_UART_DEMO_LOG("timer created failed");
        return;
    }

    err = ql_rtos_semaphore_create_ex(&process_data_sem, 0, 1);
    if (err != QL_OSI_SUCCESS)
    {
        QL_UART_DEMO_LOG("sem created failed");
        return;
    }
}
