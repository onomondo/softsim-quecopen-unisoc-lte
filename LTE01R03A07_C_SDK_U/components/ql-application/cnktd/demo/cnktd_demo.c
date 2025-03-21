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
#include "ql_log.h"

#include "ql_cnktd_device.h"
#include "ql_cnktd_uart.h"
#include "cnktd_demo.h"
#include "ql_cnktd_fs.h"
#include "ql_cnktd_spi.h"


#define CNKTD_APP_LOG_LEVEL              QL_LOG_LEVEL_INFO
#define CNKTD_APP_LOG(msg, ...)          QL_LOG(CNKTD_APP_LOG_LEVEL, "ql_cnktd", msg, ##__VA_ARGS__)
#define CNKTD_APP_LOG_PUSH(msg, ...)     QL_LOG_PUSH("ql_cnktd", msg, ##__VA_ARGS__)


ql_task_t cnktd_demo_task = NULL;

void cnktd_demo_virt_cb(qapi_QT_AT_Pipe_Data_t * data)
{
    if (data)
    {
        CNKTD_APP_LOG("virt len=%d", data->len);
        //If it is a string, print it as a string
        CNKTD_APP_LOG("virt data=%s", data->data);
    }    
}

void cnktd_demo_timer_cb(void *ctx)
{
    int data = (int)ctx;
    CNKTD_APP_LOG("cnktd timer data=%d", data);
}

void cnktd_demo_uart_rx_cb(uint32_t num_bytes, void *cb_data)
{
    CNKTD_APP_LOG("uart rx =%d", num_bytes);
    if (cb_data)
    {
        //If it is a string, print it as a string
        CNKTD_APP_LOG("uart rx data=%s", (char *)cb_data);
    }
    
}

void cnktd_demo_uart_tx_cb(uint32_t num_bytes, void *cb_data)
{
    CNKTD_APP_LOG("uart tx =%d", num_bytes);
}

void cnktd_demo_spi_cb(uint32_t status, void *callback_Ctxt)
{
    CNKTD_APP_LOG("spi cb tx =%d", status);
}

static void cnktd_demo_task_pthread(void *ctx)
{
    QlOSStatus err = 0;
    qapi_Status_t status = QAPI_OK;
#if 0 
    status = qapi_QT_USB_Mode_Set(1);
    CNKTD_APP_LOG("usb mode status=%x", status);
    
    //adc test
    qapi_ADC_Handle_t adc_handle;
    qapi_Adc_Input_Properties_Type_t adc_proerty = {0};
    qapi_ADC_Read_Result_t adc_resulte = {0};

    qapi_ADC_Open(&adc_handle, 0);
    status = qapi_ADC_Get_Input_Properties(adc_handle, "ql_adc_channel_1", strlen("ql_adc_channel_1"), &adc_proerty);
    CNKTD_APP_LOG("adc pro status=%x", status);
    if (status == QAPI_OK)
    {
        CNKTD_APP_LOG("adc value=%d %d", adc_proerty.nDeviceIdx, adc_proerty.nChannelIdx);
    }

    status = qapi_ADC_Read_Channel(adc_handle, &adc_proerty, &adc_resulte);
    CNKTD_APP_LOG("adc read status=%x", status);
    if (status == QAPI_OK)
    {
        CNKTD_APP_LOG("adc result =%d channel=%d  percent=%d value=%d", adc_resulte.eStatus, adc_resulte.nChannelIdx, adc_resulte.nPercent, adc_resulte.nMicrovolts);
    }
    qapi_ADC_Close(adc_handle, 0);


    uint16 val = 0;
    status = qapi_QT_Startup_Type_Get(&val);
    CNKTD_APP_LOG("startup status=%x", status);
    if (status == QAPI_QT_ERR_OK)
    {
        CNKTD_APP_LOG("startup value=%d", val);
    }


    #define CNKTD_RANDAM_LEN    (19)
    uint8 prng_data[CNKTD_RANDAM_LEN] = {0};
    status = qapi_QT_Random_Data_Get(CNKTD_RANDAM_LEN, prng_data);
    CNKTD_APP_LOG("random status=%x", status);
    if (status == QAPI_QT_ERR_OK)
    {
        CNKTD_APP_LOG("random data %02x %02x %02x %02x %02x %02x %02x %02x", prng_data[0], prng_data[1], prng_data[2], prng_data[3], \
            prng_data[CNKTD_RANDAM_LEN-4], prng_data[CNKTD_RANDAM_LEN-3], prng_data[CNKTD_RANDAM_LEN-2], prng_data[CNKTD_RANDAM_LEN-1]);
    }

    qapi_time_get_t time = {0};
    status = qapi_time_get(QAPI_TIME_MSECS, &time);
    CNKTD_APP_LOG("get time status=%x", status);
    if (status == QAPI_OK)
    {
        CNKTD_APP_LOG("get time %lld", time.time_msecs);
    }

    char version[128] = {0};
    uint16 ver_len = 0;
    status = qapi_QT_AP_FW_Ver_Get(version, &ver_len);
    CNKTD_APP_LOG("get ap version=%x", status);
    if (status == QAPI_QT_ERR_OK)
    {
        CNKTD_APP_LOG("ap version %s", version);
    }

    CNKTD_APP_LOG("virt demo");
    qapi_QT_AT_Stream_ID_t strem_id = 0;
    char *cmd0 = "ATI\r\n";
    qapi_QT_AT_Pipe_Data_t *virt_data = (qapi_QT_AT_Pipe_Data_t *)malloc(sizeof(qapi_QT_AT_Pipe_Data_t));
    if (virt_data == NULL)
    {
        goto exit;
    }

    CNKTD_APP_LOG("virt open11=%x", status);
    status = qapi_QT_Apps_AT_Port_Open(QAPI_AT_PORT_0, &strem_id, cnktd_demo_virt_cb, virt_data);
    CNKTD_APP_LOG("virt open=%x", status);
    if (status == QAPI_QT_ERR_OK)
    {
        CNKTD_APP_LOG("virt strem_id=%d", strem_id);
        status = qapi_QT_Apps_Send_AT(strem_id, cmd0);
        CNKTD_APP_LOG("virt send=%x", status);
        ql_rtos_task_sleep_s(5);
        qapi_QT_Apps_AT_Port_Close(strem_id);
    }

    free(virt_data);
    virt_data = NULL;

    status = qapi_Timer_Sleep(1000, QAPI_TIMER_UNIT_MSEC, true);
    CNKTD_APP_LOG("sleep status=%x", status);

    unsigned long long timetick = 0;
    status = qapi_QT_Get_Timetick(&timetick);
    CNKTD_APP_LOG("time tick status=%x", status);
    if (status == QAPI_QT_ERR_OK)
    {
        CNKTD_APP_LOG("time tick=%lld", timetick);
    }


    qapi_TIMER_handle_t timer_handle = NULL;
    qapi_TIMER_define_attr_t timer_attr = {0};
    timer_attr.cb_type = QAPI_TIMER_FUNC1_CB_TYPE;
    timer_attr.deferrable = false;
    timer_attr.sigs_func_ptr = cnktd_demo_timer_cb;
    timer_attr.sigs_mask_data = 500;
    status = qapi_Timer_Def(&timer_handle, &timer_attr);
    CNKTD_APP_LOG("cnktd timer status=%x", status);
    if (status == QAPI_OK)
    {
        qapi_TIMER_set_attr_t timer_set_attr = {0};
        timer_set_attr.time = 1000;
        timer_set_attr.unit = QAPI_TIMER_UNIT_MSEC;
        timer_set_attr.reload = 1;
        status = qapi_Timer_Set(timer_handle, &timer_set_attr);
        CNKTD_APP_LOG("start cnktd timer =%x", status);
        status = qapi_Timer_Sleep(5, QAPI_TIMER_UNIT_SEC, true);
        CNKTD_APP_LOG("sleep status=%x", status);
        status = qapi_Timer_Stop(timer_handle);
        CNKTD_APP_LOG("stop cnktd timer =%x", status);
    }

    uint32 serial_num = 0;

    status = qapi_QT_Chip_Serial_Num_Get(&serial_num);
    CNKTD_APP_LOG("cnktd serial num=%x", status);
    if (status == QAPI_QT_ERR_OK)
    {
        CNKTD_APP_LOG("serial num=%x", serial_num);
    }


    qapi_TLMM_Config_t gpio_config = {0};
    gpio_config.pin = 4;
    gpio_config.func = 0;
    gpio_config.drive = 2;
    gpio_config.dir = QAPI_GPIO_OUTPUT_E;
    gpio_config.pull = QAPI_GPIO_PULL_DOWN_E;

    qapi_GPIO_ID_t gpio_id = 0;
    status = qapi_TLMM_Get_Gpio_ID(&gpio_config, &gpio_id);
    CNKTD_APP_LOG("cnktd get gpio=%x", status);    
    status = qapi_TLMM_Config_Gpio(gpio_id, &gpio_config);
    CNKTD_APP_LOG("cnktd config gpio=%x", status);  
    status = qapi_TLMM_Release_Gpio_ID(&gpio_config, gpio_id);
    CNKTD_APP_LOG("cnktd release gpio=%x", status);

    qapi_UART_Handle_t uart_handle = NULL;
    qapi_UART_Open_Config_t uart_config = {0};
    uart_config.baud_Rate = 115200;
    uart_config.bits_Per_Char = QAPI_UART_8_BITS_PER_CHAR_E;
    uart_config.enable_Flow_Ctrl = false;
    uart_config.enable_Loopback = true;
    uart_config.num_Stop_Bits = QAPI_UART_1_0_STOP_BITS_E;
    uart_config.parity_Mode = QAPI_UART_NO_PARITY_E;
    uart_config.rx_CB_ISR = cnktd_demo_uart_rx_cb;
    uart_config.tx_CB_ISR = cnktd_demo_uart_tx_cb;
    
    status = qapi_UART_Open(&uart_handle, QAPI_UART_PORT_001_E, &uart_config);
    CNKTD_APP_LOG("cnktd uart open=%x", status);
    if (status == QAPI_OK)
    {
        qapi_Timer_Sleep(5, QAPI_TIMER_UNIT_SEC, true);
        char read_data[64] = {0};
        qapi_UART_Receive(uart_handle, read_data, 64, read_data);
    }

    int fd = 0;
    int len = 0;

    #define QL_CNKTD_TEST_FILE_NAME     "test.txt"
    #define QL_CNKTD_TEST_DIR           "UFS:/test_dir"
    #define QL_CNKTD_TEST_FILE          ""QL_CNKTD_TEST_DIR"/"QL_CNKTD_TEST_FILE_NAME""

    CNKTD_APP_LOG("cnktd tesd dir=%s,fs=%s", QL_CNKTD_TEST_DIR, QL_CNKTD_TEST_FILE);
    status = qapi_FS_Mk_Dir(QL_CNKTD_TEST_DIR, 0);
    CNKTD_APP_LOG("cnktd mk dir=%x", status);
    if (status == QAPI_OK)
    {
        status = qapi_FS_Open(QL_CNKTD_TEST_FILE, O_RDWR|O_CREAT|O_TRUNC, &fd);
        CNKTD_APP_LOG("cnktd open fs=%x", status);
        if (status == QAPI_OK)
        {
            status = qapi_FS_Write(fd, (const uint8 *)"1234567890", 10, (uint32*)&len);
            CNKTD_APP_LOG("cnktd wr fs=%x, len=%d", status, len);

            status = qapi_FS_Seek(fd, 0, QL_SEEK_SET, (qapi_FS_Offset_t *)&len);
            CNKTD_APP_LOG("cnktd seek fs=%x, len=%d", status, len);

            uint8 buffer[64] = {0};
            status = qapi_FS_Read(fd, buffer, 10, (uint32*)&len);
            CNKTD_APP_LOG("cnktd read fs=%x, buffer=%s,len=%d", status, buffer, len);

            status = qapi_FS_Close(fd);
            CNKTD_APP_LOG("cnktd close fs=%x", status);

            struct qapi_FS_Stat_Type_s sbuf = {0};
            status = qapi_FS_Stat(QL_CNKTD_TEST_FILE, &sbuf);
            CNKTD_APP_LOG("cnktd stat fs=%x,size=%d", status,sbuf.st_size);

            status = qapi_FS_Truncate(QL_CNKTD_TEST_FILE, 3);
            CNKTD_APP_LOG("cnktd truncate fs=%x", status);

            status = qapi_FS_Stat(QL_CNKTD_TEST_FILE, &sbuf);
            CNKTD_APP_LOG("cnktd stat fs=%x,size=%d", status,sbuf.st_size);

            status = qapi_FS_Unlink(QL_CNKTD_TEST_FILE);
            CNKTD_APP_LOG("cnktd remove fs=%x", status);
        }
    }


    void * spi_hadnle = NULL;
    status = spi_qapi_handler(TXM_QAPI_SPI_OPEN, QAPI_SPIM_INSTANCE_1_E, (int)&spi_hadnle, 0, 0, 0, 0, 0, 0);
    CNKTD_APP_LOG("cnktd spi open=%x", status);
    if (status == QAPI_OK)
    {
        status = spi_qapi_handler(TXM_QAPI_SPI_POWER_ON, (int)spi_hadnle, 0, 0, 0, 0, 0, 0, 0);
        CNKTD_APP_LOG("cnktd spi on=%x", status);
        if (status == QAPI_OK)
        {
            qapi_SPIM_Config_t config = {0};
            config.SPIM_Mode = QAPI_SPIM_MODE_0_E;
            config.SPIM_CS_Polarity = QAPI_SPIM_CS_ACTIVE_LOW_E;
            config.SPIM_endianness = SPI_BIG_ENDIAN;
            config.SPIM_Bits_Per_Word = 8;
            config.Clk_Freq_Hz = QL_SPI_CLK_20MHZ;
            config.CS_Clk_Delay_Cycles = QL_SPI_CLK_DELAY_0;

            #define QL_CNKTD_SPI_TEST_LEN       800
            qapi_SPIM_Descriptor_t desc = {0};
            desc.tx_buf = calloc(1, QL_CNKTD_SPI_TEST_LEN);
            desc.rx_buf = calloc(1, QL_CNKTD_SPI_TEST_LEN);
            if (desc.tx_buf == NULL || desc.rx_buf == NULL)
            {
                status = spi_qapi_handler(TXM_QAPI_SPI_POWER_OFF, (int)spi_hadnle, 0, 0, 0, 0, 0, 0, 0);
                CNKTD_APP_LOG("cnktd spi off=%x", status);
                goto exit;
            }

            desc.len = QL_CNKTD_SPI_TEST_LEN;
            for(int i=0; i< QL_CNKTD_SPI_TEST_LEN-1; i++)
            {
                desc.tx_buf[i] = '1';
            }
            status = spi_qapi_handler(TXM_QAPI_SPI_FULL_DUPLEX, (int)spi_hadnle, (int)&config, (int)&desc, QL_CNKTD_SPI_TEST_LEN, (int)cnktd_demo_spi_cb, 0, 0, 0);
            CNKTD_APP_LOG("cnktd spi wr=%x", status);

            if (desc.tx_buf)
            {
                free(desc.tx_buf);
            }
            if (desc.rx_buf)
            {
                free(desc.rx_buf);
            }
            status = spi_qapi_handler(TXM_QAPI_SPI_POWER_OFF, (int)spi_hadnle, 0, 0, 0, 0, 0, 0, 0);
            CNKTD_APP_LOG("cnktd spi off=%x", status);
            
        }
    }  

    qapi_QT_FATAL_ERR_Mode_e mode = QAPI_FATAL_ERR_RESET;
    status = qapi_QT_Sahara_Mode_Get(&mode);
    CNKTD_APP_LOG("cnktd sahara get=%x,mode=%d", status, mode);

    mode = QAPI_FATAL_ERR_SAHARA;
    status = qapi_QT_Sahara_Mode_Set(mode);
    CNKTD_APP_LOG("cnktd sahara get=%x,mode=%d", status, mode);
    
    mode = QAPI_FATAL_ERR_RESET;
    status = qapi_QT_Sahara_Mode_Get(&mode);
    CNKTD_APP_LOG("cnktd sahara get=%x,mode=%d", status, mode);  
#endif
    qapi_QT_DAM_Crash_Mode_e mode = QAPI_QT_DAM_CRASH_MODE_MAX;
    status = qapi_QT_DAM_Crash_Mode_Get(&mode);
    CNKTD_APP_LOG("cnktd crash get=%x,mode=%d", status, mode);

    mode = QAPI_QT_DAM_CRASH_DUMP_MODE;
    status = qapi_QT_DAM_Crash_Mode_Set(mode);
    CNKTD_APP_LOG("cnktd crash get=%x,mode=%d", status, mode);

    mode = QAPI_QT_DAM_CRASH_MODE_MAX;
    status = qapi_QT_DAM_Crash_Mode_Get(&mode);
    CNKTD_APP_LOG("cnktd crash get=%x,mode=%d", status, mode);

    qapi_QT_DAM_Crash_Info_t crash_info = {0};
    status = qapi_QT_DAM_Crash_Info_Get(&crash_info);
    CNKTD_APP_LOG("cnktd crash get=%x,sp=%x,lr=%x", status, crash_info.sp_info, crash_info.lr_info);
    CNKTD_APP_LOG("cnktd crash pc=%x,name=%s", crash_info.pc_info, crash_info.thread_name);
    
    goto exit;
exit:
    err = ql_rtos_task_delete(NULL);
    if(err != QL_OSI_SUCCESS)
	{
		CNKTD_APP_LOG("task deleted failed");
	}
}


QlOSStatus cnktd_demo_init(void)
{	
	QlOSStatus err = QL_OSI_SUCCESS;

	err = ql_rtos_task_create(&cnktd_demo_task, CNKTD_DEMO_TASK_STACK_SIZE, CNKTD_DEMO_TASK_PRIO, "cnktd_demo", cnktd_demo_task_pthread, NULL, CNKTD_DEMO_TASK_EVENT_CNT);
	if(err != QL_OSI_SUCCESS)
	{
		CNKTD_APP_LOG("demo_task created failed");
        return err;
	}
    
    return err;
}


