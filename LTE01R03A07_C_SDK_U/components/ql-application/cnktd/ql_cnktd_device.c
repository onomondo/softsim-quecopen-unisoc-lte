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

#include "ql_cnktd_device.h"
#include "ql_api_osi.h"

#include "ql_log.h"
#include "ql_api_dev.h"
#include "ql_usb.h"
#include "ql_adc.h"
#include "ql_power.h"
#include "ql_api_fota.h"
#include "ql_api_virt_at.h"
#include "ql_pin_cfg.h"
#include "ql_gpio.h"

#define QL_CNKTD_DEVIE_LOG_LEVEL                QL_LOG_LEVEL_INFO
#define QL_CNKTD_DEVIE_LOG(msg, ...)            QL_LOG(QL_CNKTD_DEVIE_LOG_LEVEL, "ql_cnktd", msg, ##__VA_ARGS__)
#define QL_CNKTD_DEVIE_LOG_PUSH(msg, ...)       QL_LOG_PUSH("ql_cnktd", msg, ##__VA_ARGS__)


/**
*�@return
* - #QAPI_OK � Success
* - #QAPI_ERR_INVALID_PARAM - Failure for invalid parameter�
* - #QAPI_ERROR - Failure for anything other than the above two
* @brief  
* Gets the time in the specified format. 
*
* @param[in] time_get_unit  Unit in which to get the time. 
* @param[in] time           Pointer to qapi_time_get_t variable. 

*
* @dependencies
* none
*/
qapi_Status_t qapi_time_get(
   qapi_time_unit_type    time_get_unit,
   qapi_time_get_t* time
)
{
    int64_t up_time = 0;
    
    if (time == NULL || time_get_unit == QAPI_TIME_STAMP || time_get_unit == QAPI_TIME_JULIAN)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    up_time = ql_rtos_up_time_ms();
    time->time_msecs = up_time;
    time->time_secs = up_time * 1000;
	return QAPI_OK;
}

/*
@func
  qapi_QT_AP_FW_Ver_Get
@brief
  Get module AP firmware version number.
@param [in]
  version - pointer, to store the AP firmware number.
*/
qapi_Status_t qapi_QT_AP_FW_Ver_Get(char* version, uint16 *len)
{
    ql_errcode_dev_e ret = QL_DEV_SUCCESS;
    
#define QL_CNKTD_VERSION_LEN    128
    char version_buf[QL_CNKTD_VERSION_LEN] = {0};

    if(version == NULL || len == NULL)
    {
        return QAPI_QT_ERR_INVALID_PARAM;
    }
    
	ret = ql_dev_get_firmware_version(version_buf, sizeof(version_buf));
    if (ret != QL_DEV_SUCCESS)
    {
        return QAPI_QT_ERR_NORMAL_FAIL;
    }

    memcpy(version, version_buf, strlen(version_buf));
    *len = strlen(version_buf);
    
    return QAPI_QT_ERR_OK;
}

/** qapi_QT_Remove_FOTA_Package
  @brief remove FOTA package
  @return One of the following error codes:
  		QAPI_QT_ERR_OK  -- OK.
  		Other error codes -- Failure.
*/

qapi_Status_t qapi_QT_Remove_FOTA_Package(void)
{
    if (QL_FOTA_SUCCESS != ql_fota_file_reset(true))
    {
        return QAPI_QT_ERR_NORMAL_FAIL;
    }
        
    return QAPI_QT_ERR_OK;
}

/** qapi_QT_FOTA_Download_Start
  @brief start FOTA package download
  @param [in] 
  url  FOTA package path. for example: HTTP://220.180.239.212:8005/BG95_112A_119.zip. 
  response_cb  response callback, This callback will come when download failed or success.
  @return One of the following error codes:
  		QAPI_QT_ERR_OK  -- OK.
  		Other error codes -- Failure.
*/
qapi_Status_t qapi_QT_FOTA_Download_Start(char *url,qapi_QT_FOTA_DL_CB_t response_cb)
{
	if(response_cb == NULL)
	{
		return QAPI_QT_ERR_INVALID_PARAM;
	}
		
	return QAPI_QT_ERR_OK;	
}

/*
@func
  qapi_QT_FOTA_DL_Process
@brief
  Show fota download progress.
@param[in]
  cb  This callback will come when download process update.
@return One of the following error codes:
  		QAPI_QT_ERR_OK  -- OK.
  		Other error codes -- Failure.
*/
qapi_Status_t qapi_QT_FOTA_DL_Process(qapi_QT_FOTA_DL_Process_CB_t cb)
{
	return QAPI_QT_ERR_OK;
}

/*
@func
  qapi_QT_FOTA_Download_Cancel
@brief
  Cancel the current download process.
*/
qapi_Status_t qapi_QT_FOTA_Download_Cancel(void)
{
	return QAPI_QT_ERR_OK;
}

/*
@func
  qapi_QT_FOTA_WD_CFG_Set.
@brief
  Set the configuration fota watch dog.
@param[out]
  WD_info :the configuration of fota watch dog.
@return One of the following error codes:
  		QAPI_QT_ERR_OK  -- OK.
  		Other error codes -- Failure.

*/
qapi_Status_t qapi_QT_FOTA_WD_CFG_Set(qapi_QT_FOTA_WD_CFG_t* wd_cfg)
{
    qapi_Status_t status = QAPI_QT_ERR_OK;
    
    if(wd_cfg == NULL)
    {
        return QAPI_QT_ERR_INVALID_PARAM;
    }

    /*check switch*/
    if((wd_cfg->fota_wd_switch != 0) && (wd_cfg->fota_wd_switch != 1))
    {
        return QAPI_QT_ERR_INVALID_PARAM;
    }

    return status;
}

/** qapi_QT_FOTA_Update_Start
  @brief start FOTA upgrade
  @return One of the following error codes:
  		QAPI_QT_ERR_OK  -- OK.
  		Other error codes -- Failure.
*/
qapi_Status_t qapi_QT_FOTA_Update_Start(void)
{
    return QAPI_QT_ERR_OK;
}

#define QL_CNKTD_VIRT_AT_RX_BUFF_SIZE                1024


static qapi_QT_AT_Pipe_Data_t *cnktd_virt_data[QAPI_AT_PORT_MAX] = {0};
static qapi_QT_AT_Resp_Func_CB_t cnktd_virt_cb = NULL;

static void ql_cnktd_virt_cb(ql_virt_at_port_number_e port, uint32 ind_type, uint32 size)
{
    unsigned char *recv_buff = calloc(1, QL_CNKTD_VIRT_AT_RX_BUFF_SIZE+1);
    unsigned int real_size = 0;
    
    if(QUEC_VIRT_AT_RX_RECV_DATA_IND == ind_type)
    {
        memset(recv_buff, 0, QL_CNKTD_VIRT_AT_RX_BUFF_SIZE+1);
        real_size= CNKTD_MIN(size, QL_CNKTD_VIRT_AT_RX_BUFF_SIZE);
        
        ql_virt_at_read(port, recv_buff, real_size);

        if (cnktd_virt_data[port])
        {
            memcpy(cnktd_virt_data[port]->data, recv_buff, real_size);
            cnktd_virt_data[port]->len = real_size;
            cnktd_virt_cb(cnktd_virt_data[port]);
        }
    }
    free(recv_buff);
    recv_buff = NULL;
}

static void ql_cnktd_virt_port0(uint32 ind_type, uint32 size)
{
    ql_cnktd_virt_cb(QL_VIRT_AT_PORT_0, ind_type, size);
}

static void ql_cnktd_virt_port1(uint32 ind_type, uint32 size)
{
    ql_cnktd_virt_cb(QL_VIRT_AT_PORT_1, ind_type, size);
}

/*
@func
  qapi_QT_Apps_AT_Port_Open
@brief
  Open Apps AT command port and register a callback. 
*/
qapi_Status_t qapi_QT_Apps_AT_Port_Open(qapi_QT_AT_Port_e port_id, qapi_QT_AT_Stream_ID_t *stream_id, qapi_QT_AT_Resp_Func_CB_t cb, qapi_QT_AT_Pipe_Data_t *data)
{
    int port = port_id;
    ql_virt_at_callback virt_at_cb = NULL;
    
    if ((port_id < QAPI_AT_PORT_0) || (port_id > QAPI_AT_PORT_1))
	{
		return QAPI_QT_ERR_PORT_ID;
	}
#if 0
    if (cnktd_virt_data[port] == NULL)
    {
        cnktd_virt_data[port] = (qapi_QT_AT_Pipe_Data_t *)malloc(sizeof(qapi_QT_AT_Pipe_Data_t));
        if (cnktd_virt_data[port] == NULL)
        {
            return QAPI_QT_ERR_NO_MEM;
        }
    }
#else
    cnktd_virt_data[port] = data;
#endif
    
    *stream_id = port_id;
    cnktd_virt_cb = cb;

    if (port_id == QAPI_AT_PORT_0)
    {
        virt_at_cb = ql_cnktd_virt_port0;
    }
    else 
    {
        virt_at_cb = ql_cnktd_virt_port1;
    }
    if (QL_VIRT_AT_SUCCESS != ql_virt_at_open((ql_virt_at_port_number_e)port_id, virt_at_cb))
    {
        return QAPI_QT_ERR_NORMAL_FAIL;
    }
	return QAPI_QT_ERR_OK;
}

/*
@func
  qapi_QT_Apps_Send_AT
@brief
  Send AT command in specifial stream port id. 
*/
qapi_Status_t qapi_QT_Apps_Send_AT(qapi_QT_AT_Stream_ID_t stream_id, const char *command_name)
{
    if ((stream_id < QAPI_AT_PORT_0) || (stream_id > QAPI_AT_PORT_1))
	{
		return QAPI_QT_ERR_INVALID_PARAM;
	}
    
    if (QL_VIRT_AT_SUCCESS != ql_virt_at_write((ql_virt_at_port_number_e)stream_id, (unsigned char*)command_name, strlen((char *)command_name)))
    {
        return QAPI_QT_ERR_NORMAL_FAIL;
    }   
	return QAPI_QT_ERR_OK;
}

/*
@func
  qapi_QT_Apps_AT_Port_Close
@brief
  Close AT command port with stream port id. 
*/
void qapi_QT_Apps_AT_Port_Close(qapi_QT_AT_Stream_ID_t stream_id)
{
	if ((stream_id < QAPI_AT_PORT_0) || (stream_id > QAPI_AT_PORT_1))
	{
		return ;
	}
#if 0
    if (cnktd_virt_data[stream_id])
    {
        free(cnktd_virt_data[stream_id]);
        cnktd_virt_data[stream_id] = NULL;
    }
#endif
    if (QL_VIRT_AT_SUCCESS != ql_virt_at_close((ql_virt_at_port_number_e)stream_id))
    {
        return;
    }   
    
	return ;
}

#define QL_CNKTD_TICK_MS    (20)
/**
*�@return
* - #QAPI_OK � Success�
* - #QAPI_ERR_INVALID_PARAM - Failure for invalid parameter
* - #QAPI_ERROR - Failure for any other error
  @brief  
* Timed wait. Blocks a thread for specified time. 
  
* @param[in] timeout - Specify the duration to block the thread 
* @param[in] unit    - Specify the unit of duration 
* @param[in] non-deferrable  - true = processor (if in deep 
*       sleep or power collapse) will be woken up on timeout.
*       false = processor will not be woken up from deep sleep
*       or power collapse on timeout. Whenever the processor
*       wakes up due to some other reason after timeout, the
*       thread will be unblocked.
  
  @dependencies
* 
*/
qapi_Status_t qapi_Timer_Sleep(
  uint64_t             timeout,
  qapi_TIMER_unit_type unit,
  qbool_t              non_deferrable
)
{
    uint64_t sleep_time = 0;

    if (unit >= QAPI_TIMER_UNIT_MAX) 
    {
        return QAPI_ERR_INVALID_PARAM;  
    }

    if (non_deferrable == false)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if (unit == QAPI_TIMER_UNIT_USEC)
    {
        if (QL_OSI_SUCCESS != ql_delay_us(timeout))
        {
            return QAPI_ERROR; 
        }
    }
    else 
    {
        switch (unit)
        {
            case QAPI_TIMER_UNIT_TICK:
                sleep_time = timeout * QL_CNKTD_TICK_MS;
                ql_rtos_task_sleep_ms(sleep_time);
            break;
            case QAPI_TIMER_UNIT_MSEC:
                sleep_time = timeout;
                ql_rtos_task_sleep_ms(sleep_time);
            break;
            case QAPI_TIMER_UNIT_SEC:
                sleep_time = timeout;
                ql_rtos_task_sleep_s(sleep_time);
            break;
            case QAPI_TIMER_UNIT_MIN:
                sleep_time = 60*timeout;
                ql_rtos_task_sleep_s(sleep_time);
            break;
            case QAPI_TIMER_UNIT_HOUR:
                sleep_time = 60*60*timeout;
                ql_rtos_task_sleep_s(sleep_time);
            break;
            default:
            break;
        }
    }
    
    return QAPI_OK;
}

/*
@func
  qapi_QT_Get_Timetick
@brief
  Get the value of timetick.
@param[out]
  timetick:Pointer tio get time tick  .
@return One of the following error codes:
  		QAPI_QT_ERR_OK  -- OK.
  		Other error codes -- Failure.
*/
qapi_Status_t qapi_QT_Get_Timetick(unsigned long long *timetick)
{
	if(timetick == NULL)
	{
		return QAPI_QT_ERR_INVALID_PARAM;
	}	

    *timetick = ql_rtos_get_system_tick();
	return QAPI_QT_ERR_OK;		
}

typedef struct
{
    ql_timer_t timer;
    qbool_t                   deferrable; 
}ql_cnked_timer_s;

/**
*�@return
* - #QAPI_OK � Success�
* - #QAPI_ERR_INVALID_PARAM - Failure for invalid parameter
* - #QAPI_ERR_NO_MEMORY - Failure for not able to allocate memory for timer handle
* - #QAPI_ERROR - Failure for anything other than the above two
  @brief  
* Allocates internal memory in the timer module. The internal 
* memory is then formatted with parameters provided in 
* timer_def_attr variable. The timer_handle is returned to the 
* client and this handle should be used for any subsequent timer 
* operations. 
  
* @param[in] timer_handle  Handle to the timer. 
* @param[in] timer_attr    Attributes for defining timer. 
  
  @dependencies
* None. 
*  
* @sideeffects Calling this API will cause memory allocation. 
*              Therefore, whenever the timer usage is done and
*              not required, qapi_Timer_Undef shoule be called
*              to release the memory otherwise it will cause a
*              memory leak.
*/
qapi_Status_t qapi_Timer_Def(
   qapi_TIMER_handle_t*    timer_handle,
   qapi_TIMER_define_attr_t*  timer_attr  
)
{
    if (timer_attr == NULL || timer_attr->cb_type != QAPI_TIMER_FUNC1_CB_TYPE)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    ql_cnked_timer_s *cnktd_timer = (ql_cnked_timer_s *)malloc(sizeof(ql_cnked_timer_s));
    if (cnktd_timer == NULL)
    {
        return QAPI_ERR_NO_MEMORY;
    }
    
    if (QL_OSI_SUCCESS != ql_rtos_timer_create(&cnktd_timer->timer, QL_TIMER_IN_SERVICE, timer_attr->sigs_func_ptr, (void *)timer_attr->sigs_mask_data))
    {
        return QAPI_ERROR;
    }
    cnktd_timer->deferrable = timer_attr->deferrable;
    *timer_handle = cnktd_timer;

	return QAPI_OK;
}

/**
*�@return
* - #QAPI_OK � Success�
* - #QAPI_ERR_INVALID_PARAM - Failure for invalid parameter
* - #QAPI_ERROR - Failure for any other error
  @brief  
* Starts the timer with the duration specified in timer_attr. 
* If the timer is specified as a reload timer in timer_attr, 
* then the timer will restart after its expiry 
  
* @param[in] timer_handle  Handle to the timer. 
* @param[in] timer_attr    Attributes for setting timer. 
  
  @dependencies
* qapi_Timer_Def API should be called for the timer before 
* calling qapi_Timer_Set function.
*/
qapi_Status_t qapi_Timer_Set(
   qapi_TIMER_handle_t    timer_handle,
   qapi_TIMER_set_attr_t* timer_attr     
)
{
    QlOSStatus ret = QL_OSI_SUCCESS;
    uint32 set_time = 0;
    unsigned char reloard = 0;

    if(NULL == timer_handle || NULL == timer_attr || timer_attr->unit >= QAPI_TIMER_UNIT_MAX) 
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    ql_cnked_timer_s *cnktd_timer = timer_handle;

    if (timer_attr->unit== QAPI_TIMER_UNIT_USEC)
    {
        if (cnktd_timer->deferrable == false || timer_attr->reload > 0)
        {
            return QAPI_ERR_INVALID_PARAM;
        }

        set_time = timer_attr->time;
        ret = ql_rtos_timer_start_us(cnktd_timer->timer, set_time);
    }
    else 
    {

        switch (timer_attr->unit)
        {
            case QAPI_TIMER_UNIT_TICK:
                set_time = timer_attr->time * QL_CNKTD_TICK_MS;
            break;
            case QAPI_TIMER_UNIT_MSEC:
                set_time = timer_attr->time;
            break;
            case QAPI_TIMER_UNIT_SEC:
                set_time = 1000*timer_attr->time;
            break;
            case QAPI_TIMER_UNIT_MIN:
                set_time = 60*1000*timer_attr->time;
            break;
            case QAPI_TIMER_UNIT_HOUR:
                set_time = 60*60*1000*timer_attr->time;
            break;
            default:
            break;
        }
    }

    reloard = timer_attr->reload > 0 ? 1 : 0;
    if (cnktd_timer->deferrable == false)
    {
        ret = ql_rtos_timer_start_relaxed(cnktd_timer->timer, set_time, reloard, 0xFFFFFFFF);
    }
    else 
    {
        ret = ql_rtos_timer_start(cnktd_timer->timer, set_time, reloard);
    }
    if (QL_OSI_SUCCESS != ret)
    {
        return QAPI_ERROR; 
    }
    
    return QAPI_OK;
}

/**
*�@return
* - #QAPI_OK � Success
* - #QAPI_ERR_INVALID_PARAM - Failure for invalid parameter
* - #QAPI_ERROR - Failure for any other error
  @brief  
* Stops the timer. Note: This function does NOT deallocate the 
* memory that was allocated when timer was defined.  
  
* @param[in] handle - timer handle to stop the timer
  
  @dependencies
* 
*/
qapi_Status_t qapi_Timer_Stop(
    qapi_TIMER_handle_t    timer_handle
)
{
    if(NULL == timer_handle) 
    {
        return QAPI_ERR_INVALID_PARAM;  
    }

    ql_cnked_timer_s *cnktd_timer = timer_handle;
    if (QL_OSI_SUCCESS != ql_rtos_timer_stop(cnktd_timer->timer))
    {
        return QAPI_ERROR;
    }
    return QAPI_OK;
}

/*
@func
  qapi_QT_Startup_Type_Get
@brief
  Gets the Module startup reason type
@param [out]
  Val  Return the Module startup reason type value
*/
qapi_Status_t qapi_QT_Startup_Type_Get(uint16 *val)
{
    ql_errcode_power ret = QL_POWER_POWD_SUCCESS;
    uint32_t boot_cause = 0;
    uint8_t pwrup_reason = 0;

    ret = ql_get_boot_cause(&boot_cause);
    if (ret != QL_POWER_POWD_SUCCESS)
    {
        goto exit;
    }

    if (boot_cause & QL_BOOTCAUSE_SW_RST)
    {
        *val = soft_reset;
        return QAPI_QT_ERR_OK;
        
    }

    ret = ql_get_powerup_reason(&pwrup_reason);
    if (ret != QL_POWER_POWD_SUCCESS)
    {
        goto exit;
    }

    switch(pwrup_reason)
    {
        case QL_PWRUP_UNKNOWN:
            *val = default_val;
        break;
        case QL_PWRUP_PWRKEY:
            *val = kpdpwr;
        break;
        case QL_PWRUP_PIN_RESET:
            *val = hard_reset;
        break;
        case QL_PWRUP_ALARM:
            *val = rtc;
        break;
        case QL_PWRUP_CHARGE:
            *val = usb_chg;
        break;
        default:
            *val = default_val;
        break;
    }
	return QAPI_QT_ERR_OK;
exit:
    QL_CNKTD_DEVIE_LOG("cnktd startup get err=%x", ret);
    return QAPI_QT_ERR_NORMAL_FAIL;
}

/*
@func
  qapi_QT_Chip_Serial_Num_Get
@brief
  Get the chip serial number by qapi_QT_Chip_Serial_Num_Get.
@param [out]
  Pointer, pointer to the chip serial number. 
*/
qapi_Status_t qapi_QT_Chip_Serial_Num_Get(uint32 *serial_num)
{
	if(serial_num == NULL)
	{
		return QAPI_QT_ERR_INVALID_PARAM;
	}

    unsigned long long p_chip_id = 0;
    if (QL_DEV_SUCCESS != ql_dev_get_cpu_uid(&p_chip_id))
    {
        return QAPI_QT_ERR_NORMAL_FAIL;
    }

    *serial_num = p_chip_id;
	return QAPI_QT_ERR_OK;
}

/*
@func
  qapi_QT_Random_Data_Get
@brief
  Gets the hardware random number
@param [in]
  prng_size  get the length of random Number.Parameters range of the support:1,2,4...512 
@param [out]
  prng_data  Stores the generated random Number,the size of the data returned depends on prng_size
*/
qapi_Status_t qapi_QT_Random_Data_Get(uint16 prng_size, uint8* prng_data)
{
    uint32_t num = 0; 
    int count = prng_size / 4;
    int flag = prng_size % 4;

    //0,8,9,10,11,12
    if(prng_data == NULL)
    {
        return QAPI_QT_ERR_INVALID_PARAM;
    }
    
    for (int i = 0; i < count; i++) 
    {
        num = ql_dev_get_sys_srand();
        memcpy(prng_data+i*4, &num, 4);
    }

    if (flag)
    {
        memcpy(prng_data+count*4, &num, flag);
    }
	
	return QAPI_QT_ERR_OK;
}

#define QL_CNKTD_ADC_NUM    2                       //can not be changed
char * quec_adc_name[QL_CNKTD_ADC_NUM] =
{
    "ql_adc_channel_1",
    "ql_adc_channel_2",
};


/* API for Opening ADC */
qapi_Status_t qapi_ADC_Open(qapi_ADC_Handle_t *Handle, uint32_t Attributes)
{
   return QAPI_QT_ERR_OK;
}

/* API to get properties of ADC channels */
qapi_Status_t qapi_ADC_Get_Input_Properties(qapi_ADC_Handle_t Handle,
                                                 const char *Channel_Name_Ptr,
                                                 uint32_t Channel_Name_Size,
                                                 qapi_Adc_Input_Properties_Type_t *Properties_Ptr)
{
    int i = 0;
    bool exit = false;
    
    if ((Channel_Name_Ptr == NULL) || (Channel_Name_Size == 0) || (Properties_Ptr == NULL))
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if (Channel_Name_Size != strlen(quec_adc_name[0]))
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    for (i=0; i<QL_CNKTD_ADC_NUM; i++)
    {
        if (0 == strcmp(quec_adc_name[i], Channel_Name_Ptr))
        {
            exit = true;
            break;
        }        
    }

    if (exit == false)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    Properties_Ptr->nDeviceIdx = i;
    Properties_Ptr->nChannelIdx = i;
    return QAPI_OK;
}

/* API for closing ADC */
qapi_Status_t qapi_ADC_Close(qapi_ADC_Handle_t Handle,qbool_t keep_enabled)
{
    return QAPI_OK;
}

/* API for reading an ADC channel */
qapi_Status_t qapi_ADC_Read_Channel(qapi_ADC_Handle_t Handle,
                                         const qapi_Adc_Input_Properties_Type_t *Input_Prop_Ptr,
                                         qapi_ADC_Read_Result_t *Result_Ptr)
{
    
    ql_errcode_adc_e ret = QL_CNKTD_ADC_NUM;
    int adc_value = 0;
    unsigned int scale = 0;
    
    if ((Handle == NULL) || (Input_Prop_Ptr == NULL) || (Result_Ptr == NULL))
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    ret = ql_adc_get_volt(Input_Prop_Ptr->nChannelIdx, &adc_value);
    if (QL_ADC_SUCCESS != ret)
    {
        QL_CNKTD_DEVIE_LOG("cnktd adc err=%x", ret);
        return QAPI_ERROR;
    }

    if (adc_value < 1250)
    {
        scale = 1250;
    }
    else if (adc_value < 2444)
    {
        scale = 2444;
    }
    else if (adc_value < 3233)
    {
        scale = 3233;
    } 
    else
    {
        scale = 5000;
    }

    Result_Ptr->eStatus = EFI_ADC_RESULT_VALID;
    Result_Ptr->nDeviceIdx = Input_Prop_Ptr->nDeviceIdx;
    Result_Ptr->nChannelIdx = Input_Prop_Ptr->nChannelIdx;
    Result_Ptr->nPercent = ((unsigned int)adc_value * 65535) / scale;
    Result_Ptr->nMicrovolts = adc_value;
    return QAPI_OK;
}

/*=============================================================================
FUNCTION      qapi_TLMM_Get_Gpio_ID
=============================================================================*/
qapi_Status_t qapi_TLMM_Get_Gpio_ID
(
qapi_TLMM_Config_t* qapi_TLMM_Config,
qapi_GPIO_ID_t*     qapi_GPIO_ID
)
{
    return QAPI_OK;
}

/*=============================================================================
  FUNCTION      qapi_TLMM_Config_Gpio
=============================================================================*/
qapi_Status_t qapi_TLMM_Config_Gpio
(
  qapi_GPIO_ID_t      qapi_GPIO_ID,
  qapi_TLMM_Config_t* qapi_TLMM_Config
)
{
    int index = 0;
    if (qapi_TLMM_Config == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    for( index = 0; index < QL_GPIO_PIN_MAX; index++ )
    {
        if (QUEC_PIN_NONE == ql_pin_cfg_map[index].pin_num)
        {
            QL_CNKTD_DEVIE_LOG("pin err");
            return QAPI_ERR_INVALID_PARAM;
        }
        
        if (ql_pin_cfg_map[index].pin_num == qapi_TLMM_Config->pin)
        {
            break;
        }
    }

    if (QL_GPIO_SUCCESS != ql_pin_set_func(qapi_TLMM_Config->pin, qapi_TLMM_Config->func))
    {
        goto err_exit;
    }

    if (QL_GPIO_SUCCESS != ql_pin_set_drving(qapi_TLMM_Config->pin, qapi_TLMM_Config->drive))
    {
        goto err_exit;
    }

    if (qapi_TLMM_Config->func == ql_pin_cfg_map[index].gpio_func)
    {
        if(qapi_TLMM_Config->dir == QAPI_GPIO_INPUT_E)
        {
            if (QL_GPIO_SUCCESS != ql_gpio_set_direction(ql_pin_cfg_map[index].gpio_num, GPIO_INPUT))
            {
                goto err_exit;
            }

            if(qapi_TLMM_Config->pull == QAPI_GPIO_PULL_DOWN_E)
            {
                if (QL_GPIO_SUCCESS != ql_gpio_set_pull(ql_pin_cfg_map[index].gpio_num, PULL_DOWN))
                {
                    goto err_exit;
                }
            }
            else if(qapi_TLMM_Config->pull == QAPI_GPIO_PULL_UP_E)
            {
                if (QL_GPIO_SUCCESS != ql_gpio_set_pull(ql_pin_cfg_map[index].gpio_num, PULL_UP))
                {
                    goto err_exit;
                }
            }
            
        }
        else if(qapi_TLMM_Config->dir == QAPI_GPIO_OUTPUT_E)
        {
            if (QL_GPIO_SUCCESS != ql_gpio_set_direction(ql_pin_cfg_map[index].gpio_num, GPIO_OUTPUT))
            {
                goto err_exit;
            }

            if(qapi_TLMM_Config->pull == QAPI_GPIO_PULL_DOWN_E)
            {
                if (QL_GPIO_SUCCESS != ql_gpio_set_level(ql_pin_cfg_map[index].gpio_num, LVL_LOW))
                {
                    goto err_exit;
                }
            }
            else if(qapi_TLMM_Config->pull == QAPI_GPIO_PULL_UP_E)
            {
                if (QL_GPIO_SUCCESS != ql_gpio_set_level(ql_pin_cfg_map[index].gpio_num, LVL_HIGH))
                {
                    goto err_exit;
                }
            }
        }
    }

    return QAPI_OK;
err_exit:
    return QAPI_ERROR;
}

/*=============================================================================
  FUNCTION      qapi_TLMM_Release_Gpio_ID
=============================================================================*/
qapi_Status_t qapi_TLMM_Release_Gpio_ID
(
  qapi_TLMM_Config_t* qapi_TLMM_Config,
  qapi_GPIO_ID_t      qapi_GPIO_ID
)
{
    return QAPI_OK;
}

/*
@func
  qapi_QT_Sahara_Mode_Get
@brief
  Get the NV Item value of Sahara mode setting.
@param[out]
  mode - Configure options.
    QAPI_FATAL_ERR_RESET -- Reset mode.
    QAPI_FATAL_ERR_SAHARA -- Sahara DUMP mode.
*/
qapi_Status_t qapi_QT_Sahara_Mode_Get(qapi_QT_FATAL_ERR_Mode_e *mode)
{
    uint8_t opt = 0;

    if (mode == NULL)
    {
        return QAPI_QT_ERR_INVALID_PARAM;
    }
    if (QL_DEV_SUCCESS != ql_dev_get_wdt_cfg(&opt))
    {
        return QAPI_QT_ERR_NORMAL_FAIL;
    }
    if (1 == opt)
    {
        *mode = QAPI_FATAL_ERR_RESET;
    }
    else 
    {
        *mode = QAPI_FATAL_ERR_SAHARA;
    }
	
	return QAPI_QT_ERR_OK;
}

/*
@func
  qapi_QT_Sahara_Mode_Set
@brief
  Enable or disable module enter sahara dump mode when module crash.
  It will take effect after the module restarted.
@param[in]
  mode - Configure options.
    QAPI_FATAL_ERR_RESET -- Reset mode.
    QAPI_FATAL_ERR_SAHARA -- Sahara DUMP mode.
*/
qapi_Status_t qapi_QT_Sahara_Mode_Set(qapi_QT_FATAL_ERR_Mode_e mode)
{
    uint8_t opt = 0;

    if((mode != QAPI_FATAL_ERR_RESET) && (mode != QAPI_FATAL_ERR_SAHARA))
    {
        return QAPI_QT_ERR_INVALID_PARAM;
    }

    if (mode == QAPI_FATAL_ERR_RESET)
    {
        opt = 1;
    }

    if (QL_DEV_SUCCESS != ql_dev_cfg_wdt(opt))
    {
        return QAPI_QT_ERR_NORMAL_FAIL;
    }
	return QAPI_QT_ERR_OK;
}

qapi_Status_t qapi_QT_PMIC_WD_Kick(void)
{
    uint8_t opt = 0;
    if (QL_DEV_SUCCESS != ql_dev_get_wdt_cfg(&opt))
    {
        return QAPI_QT_ERR_NORMAL_FAIL;
    }

    if (1 == opt)
    {
        ql_dev_feed_wdt();
        return QAPI_QT_ERR_OK;
    }
    else 
    {
        return QAPI_QT_ERR_NORMAL_FAIL;
    }
}

int ql_cnktd_crash_mode = 1;
qapi_Status_t qapi_QT_DAM_Crash_Mode_Set(qapi_QT_DAM_Crash_Mode_e crash_mode)
{
    ql_dump_config_t dump_config = {0};
	if(QAPI_QT_DAM_CRASH_UNLOAD_MODE == crash_mode)
	{
        ql_dump_config_t dump_config = {0};
    	dump_config.save_dump_info = true;
    	dump_config.del_dump_file = false;
    	dump_config.max_dump_cnt = 1;
    	dump_config.clean_dump_cnt = true;
    	if (QL_DEV_SUCCESS != ql_dev_config_dump(&dump_config))
    	{
            return QAPI_QT_ERR_NORMAL_FAIL;
    	}
		ql_cnktd_crash_mode = 1;
	}
	else if(QAPI_QT_DAM_CRASH_DUMP_MODE == crash_mode)
	{
        //enable save dump info to file system
    	dump_config.save_dump_info = true;
    	dump_config.del_dump_file = false;
    	dump_config.max_dump_cnt = 1;
    	dump_config.clean_dump_cnt = false;
    	if (QL_DEV_SUCCESS != ql_dev_config_dump(&dump_config))
    	{
            return QAPI_QT_ERR_NORMAL_FAIL;
    	}

        ql_cnktd_crash_mode = 0;
	}
	else
	{
		return QAPI_QT_ERR_INVALID_PARAM;
	}

    return QAPI_QT_ERR_OK;
}

qapi_Status_t qapi_QT_DAM_Crash_Mode_Get(qapi_QT_DAM_Crash_Mode_e *crash_mode)
{
	if(crash_mode == NULL)
	{
		return QAPI_QT_ERR_INVALID_PARAM;
	}
	
	if(1 == ql_cnktd_crash_mode)
	{
		*crash_mode = QAPI_QT_DAM_CRASH_UNLOAD_MODE;
	}
	else if(0 == ql_cnktd_crash_mode)
	{
		*crash_mode = QAPI_QT_DAM_CRASH_DUMP_MODE;
	}
	else
	{
		return QAPI_QT_ERR_NORMAL_FAIL;
	}

	return QAPI_QT_ERR_OK;
}

typedef enum
{
    QL_CNKTD_CRASH_SP = 0,
    QL_CNKTD_CRASH_LR,
    QL_CNKTD_CRASH_PC,
    QL_CNKTD_CRASH_CPSR,
    QL_CNKTD_CRASH_NAME,
}ql_cnktd_crash_type_e;

int ql_cnktd_str_to_int(char *str)
{
    char str_data[4] = {0};
    char c = 0;
    char *str_f = str;
    char data_temp = 0;
    int data = 0;
    
    for (int i=0; i<4; i++)
    {
        c = str_f[i*2];
        if (c >= '0' && c <= '9')
        {
            data_temp = c - '0';
        }
        else if (c >= 'a' && c <= 'f')
        {
            data_temp = c - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F')
        {
            data_temp = c - 'A' + 10;
        }

        str_data[i] = data_temp;
        c = str_f[i*2+1];
        if (c >= '0' && c <= '9')
        {
            data_temp = c - '0';
        }
        else if (c >= 'a' && c <= 'f')
        {
            data_temp = c - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F')
        {
            data_temp = c - 'A' + 10;
        }
        str_data[i] = (str_data[i] << 4) | data_temp;
    }

    data = (str_data[0] << 24) | (str_data[1] << 16) | (str_data[2] << 8) | str_data[3];
    return data;
}

bool ql_cnktd_get_crash_info(const char *buffer, ql_cnktd_crash_type_e type, void *value)
{
    char *str = NULL;
    char *end = NULL;
    switch(type)
    {
        case QL_CNKTD_CRASH_SP:
        {
            str = strstr(buffer, "sp/0x");
            int *value_temp = (int *)value;
            *value_temp = ql_cnktd_str_to_int(str+5);
        }
        break;
        case QL_CNKTD_CRASH_LR:
        {
            str = strstr(buffer, "lr/0x");
            int *value_temp = (int *)value;
            *value_temp = ql_cnktd_str_to_int(str+5);
        }
        break;
        case QL_CNKTD_CRASH_PC:
        {
            str = strstr(buffer, "pc/0x");
            int *value_temp = (int *)value;
            *value_temp = ql_cnktd_str_to_int(str+5);
        }
        break;
        case QL_CNKTD_CRASH_NAME:
        {
            str = strstr(buffer, "state/");
            while(str)
            {
                end = str + 6;
                if (*end == '0')
                {
                    break;
                }
                str = strstr(end, "state/");
            }
            
            for (int i=0; ;i--)
            {
                end = str + i;
                if (*end == 0x28)
                {
                    break;
                }
            }

            memcpy(value, end+1, (str-end)-3);  //(str-2) - (end +1)=str-end-3
        }
        break;
        default:
        break;
    }
    
    return true;
}

qapi_Status_t qapi_QT_DAM_Crash_Info_Get(qapi_QT_DAM_Crash_Info_t *crash_info)
{
	ql_dump_info_t dump_info = {0};
	dump_info.dump_buffer = NULL;
	dump_info.length = 0;
	if(ql_dev_get_dump_info(&dump_info))
	{
        return QAPI_QT_ERR_NORMAL_FAIL;
	}
    //根据返回的length去合理的申请buffer大小，再去读取数据到dump buffer中
    dump_info.dump_buffer = malloc(dump_info.length+1);
    if(NULL == dump_info.dump_buffer)
    {
        return QAPI_QT_ERR_NORMAL_FAIL;
    }
    memset(dump_info.dump_buffer, 0, dump_info.length+1);
    ql_dev_get_dump_info(&dump_info);
    //QL_CNKTD_DEVIE_LOG("buffer_len=%d, dump_cnt=%d", dump_info.length, dump_info.current_dump_cnt);
    //QL_CNKTD_DEVIE_LOG("dump_buffer=%s", dump_info.dump_buffer);
    ql_cnktd_get_crash_info(dump_info.dump_buffer, QL_CNKTD_CRASH_SP, &crash_info->sp_info);
    ql_cnktd_get_crash_info(dump_info.dump_buffer, QL_CNKTD_CRASH_LR, &crash_info->lr_info);
    ql_cnktd_get_crash_info(dump_info.dump_buffer, QL_CNKTD_CRASH_PC, &crash_info->pc_info);
    //QL_CNKTD_DEVIE_LOG("sp=%x,lr=%x,pc=%x", crash_info->sp_info, crash_info->lr_info, crash_info->pc_info);
    ql_cnktd_get_crash_info(dump_info.dump_buffer, QL_CNKTD_CRASH_NAME, crash_info->thread_name);
    //QL_CNKTD_DEVIE_LOG("name=%s", crash_info->thread_name);
    free(dump_info.dump_buffer);

	return QAPI_QT_ERR_OK;
}


/*
@func
  qapi_QT_USB_Mode_Set
@brief
  Set the sw usb on/off.this API need restart and take effect
@param [in]
  mode enable or disable the usb
       1  disable the usb
       0  able the usb   
*/
qapi_Status_t qapi_QT_USB_Mode_Set(boolean mode)
{
    ql_errcode_usb_e ret = QL_USB_SUCCESS;

    if (mode < 2)
	{
        if (mode == 0)
        {
            ret = ql_usb_enable();
        }
        else 
        {
            ret = ql_usb_disable();
        }
	}
	else
	{
		return QAPI_QT_ERR_NORMAL_FAIL; 

	}

    if (ret != QL_USB_SUCCESS)
    {
        return QAPI_QT_ERR_NORMAL_FAIL;
    }
    else 
    {
        return QAPI_QT_ERR_OK;
    }
    
}



