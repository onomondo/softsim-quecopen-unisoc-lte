/**  @file
  ql_cnktd_device.h

  @brief
  This file is used to define cnktd device for different Quectel Project.

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

=================================================================*/


#ifndef QL_CNKTD_DEVICE_H
#define QL_CNKTD_DEVICE_H

#include "ql_cnktd.h"

#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
typedef void(*qapi_QT_FOTA_DL_CB_t)(int error_id);
typedef void(*qapi_QT_FOTA_DL_Process_CB_t)(int percent);


/** @addtogroup qapi_timer
@{ */
/**
* Time type.
*
* Native timestamp type.
*/
typedef unsigned long qapi_qword[ 2 ];


/**
* Time type.
*
* Native timestamp type. Same as qapi_qword.
*/
typedef qapi_qword qapi_time_type;

/**
* Time unit type.
*
* Enumeration of the types of time that can be
* obtained from time get QAPI.
*/
typedef enum
{
  QAPI_TIME_STAMP = 0x10, /**< Return the time in timestamp format. */
  QAPI_TIME_MSECS, /**< Return the time in millisecond format. */
  QAPI_TIME_SECS, /**< Return the time in second format. */
  QAPI_TIME_JULIAN, /**< Return the time in Julian calendar format. */
}qapi_time_unit_type;

/**
* Time in Julian format.
*/
typedef struct
{
  uint16_t year;        /**< Year (1980 through 2100). */
  uint16_t month;       /**< Month of the year (1 through 12). */
  uint16_t day;         /**< Day of the month (1 through 31). */
  uint16_t hour;        /**< Hour of the day (0 through 23). */
  uint16_t minute;      /**< Minute of the hour (0 through 59). */
  uint16_t second;      /**< Second of the minute (0 through 59). */
  uint16_t day_of_week; /**< Day of the week (0 through 6 or Monday through 
                           Sunday). */
}
qapi_time_julian_type;

/**
* Used to specify parameters when getting the time. 
* @verbatim 
* Pointers depend on the value of qapi_time_unit_type.
* qapi_time_unit_type == QAPI_TIME_STAMP, 
* time_ts = Of type qapi_time_type
*  
* qapi_time_unit_type == QAPI_TIME_MSECS, 
* time_msecs = Of type unint64_t
*
* qapi_time_unit_type == QAPI_TIME_SECS, 
* time_secs = Of type unint64_t
*
* qapi_time_unit_type == QAPI_TIME_JULIAN, 
* time_julian = Of type qapi_time_julian_type
* @endverbatim
*/
typedef union
{
  qapi_time_type time_ts; /**< Specify the qapi_time_type variable pointer. */
  uint64_t time_msecs; /**< Variable for getting time in msec. */
  uint64_t time_secs; /**< Variable for getting time in sec. */
  qapi_time_julian_type time_julian; /**< Variable for getting time in Julian. */
}qapi_time_get_t;

typedef struct 
{
    uint32 fota_wd_switch;
    uint32 fota_wd_pin_num;
    uint32 fota_wd_feed_interval;
}qapi_QT_FOTA_WD_CFG_t; 

typedef enum
{
	QAPI_AT_PORT_0 = 0,
	QAPI_AT_PORT_1,
	QAPI_AT_PORT_2,	//Reserved
	QAPI_AT_PORT_3,	//Reserved

	QAPI_AT_PORT_MAX
}qapi_QT_AT_Port_e;

typedef signed short int2;
typedef int2	qapi_QT_AT_Stream_ID_t;

typedef struct
{
	char   data[2048];
	int    len;
} qapi_QT_AT_Pipe_Data_t; 


/*
@func
  qapi_QT_AT_Resp_Func_CB_t
@brief
  A callback funtion to register when open internal ATC pipe.
@param[out]
  data	- Pointer, to store the data retured from modem side, like AT command executed result or raw data.
*/
typedef void  (*qapi_QT_AT_Resp_Func_CB_t)(qapi_QT_AT_Pipe_Data_t *data);	//response callback


/**
* Timer unit type.
*
* Enumeration of the units in which timer duration can 
* be specified. 
*/
typedef enum {
  QAPI_TIMER_UNIT_TICK,     /**< Return time in ticks. */
  QAPI_TIMER_UNIT_USEC,     /**< Return time in microseconds. */
  QAPI_TIMER_UNIT_MSEC,     /**< Return time in milliseconds. */
  QAPI_TIMER_UNIT_SEC,      /**< Return time in seconds. */
  QAPI_TIMER_UNIT_MIN,      /**< Return time in minutes. */
  QAPI_TIMER_UNIT_HOUR,     /**< Return time in hours. */
  QAPI_TIMER_UNIT_MAX
}qapi_TIMER_unit_type;

/**
* Timer handle.
*
* Handle provided by the timer module to the client. Clients 
* must pass this handle as a token with subsequent timer calls. 
* Note that the clients should cache the handle. Once lost, it 
* cannot be queried back from the module. 
*/

typedef void* qapi_TIMER_handle_t;

/** @addtogroup qapi_timer
@{ */
/**
* Timer notification type.
*
* Enumeration of the notifications available on timer expiry. 
*/
typedef enum {
  QAPI_TIMER_NO_NOTIFY_TYPE,          /**< No notification. */  
  QAPI_TIMER_NATIVE_OS_SIGNAL_TYPE,   /**< Signal an object. */    
  QAPI_TIMER_FUNC1_CB_TYPE,           /**< Call back a function. */    
  QAPI_TIMER_INVALID_NOTIFY_TYPE    
} qapi_TIMER_notify_t;

/**
* Timer define attribute type.
*
* This type is used to specify parameters when defining a timer. 
* @verbatim 
* sigs_func_ptr will depend on the value of qapi_TIMER_notify_t.
* qapi_TIMER_notify_t == QAPI_TIMER_NO_NOTIFY_TYPE, 
* sigs_func_ptr = Don't care 
*  
* qapi_TIMER_notify_t == QAPI_TIMER_NATIVE_OS_SIGNAL_TYPE, 
* sigs_func_ptr = qurt signal object 
*  
* qapi_TIMER_notify_t == QAPI_TIMER_FUNC1_CB_TYPE, 
* sigs_func_ptr == specify a callback of type qapi_TIMER_cb_t
* @endverbatim
*/
typedef struct
{
  qbool_t                   deferrable; /**< FALSE = deferrable. */  
  qapi_TIMER_notify_t       cb_type;    /**< Type of notification to receive. */
  void*                     sigs_func_ptr; /**< Specify the signal object or callback function.  */  
  uint32_t                  sigs_mask_data; /**< Specify the signal mask or callback data. */
}qapi_TIMER_define_attr_t;

/**
* Type used to specify parameters when starting a timer.
*/
typedef struct
{
  uint64_t              time;   /**< Timer duration. */  
  uint64_t              reload; /**< Reload duration. */
  qapi_TIMER_unit_type  unit;   /**< Specify units for timer duration. */  
}qapi_TIMER_set_attr_t;

typedef enum
{
  default_val  ,
  hard_reset   ,         /* Hard reset event trigger */
  smpl         ,         /* SMPL trigger */             
  rtc          ,         /* RTC trigger */             
  dc_chg       ,         /* DC Charger trigger */             
  usb_chg      ,         /* USB Charger trigger */             
  pon1         ,         /* PON1 trigger */
  cblpwr       ,         /* CBL_PWR1_N trigger */
  kpdpwr       ,         /* KPDPWR_N trigger */
  soft_reset   ,         /* Software reset event trigger */
}Startup_type;

typedef void *qapi_ADC_Handle_t;


/** ADC input properties.
*/
typedef struct
{
   uint32_t  nDeviceIdx;   /**< Device index. */
   uint32_t  nChannelIdx;  /**< Channel index. */
} qapi_Adc_Input_Properties_Type_t;

/** Status of an ADC result. */
typedef enum
{
   EFI_ADC_RESULT_INVALID = 0,  /**< Result is invalid (result is out of range
                                     or an error occurred during conversion). */
   EFI_ADC_RESULT_VALID,        /**< Result is valid. */
   EFI_ADC_RESULT_TIMEOUT,      /**< Result is invalid because the conversion
                                     timed out. */
   EFI_ADC_RESULT_STALE,        /**< Result is stale. */
/** @cond */
   _EFI_ADC_RESULT_STATUS_MAX_INT32 = 0x7FFFFFFF
/** @endcond */
} EfiAdcResultStatusType;

/** ADC read results.
*/
typedef struct
{
   /*AdcResultStatusType*/unsigned int  eStatus;  /**< Status of the conversion. */
   uint32_t  nToken;       /**< Token that identifies the conversion. */
   uint32_t  nDeviceIdx;   /**< Device index for the conversion. */
   uint32_t  nChannelIdx;  /**< Channel index for the conversion. */
   int32_t   nPhysical;    /**< Result in physical units. Units depends on the BSP. */
   uint32_t  nPercent;     /**< Result as a percentage of the reference voltage used
                                for the conversion: 0 = 0%, 65535 = 100% */
   uint32_t  nMicrovolts;  /**< Result in microvolts. */
   uint32_t  nCode;        /**< Raw ADC code from the hardware. */
} qapi_ADC_Read_Result_t;


typedef uint32_t qapi_GPIO_ID_t;

/** @addtogroup qapi_tlmm
@{ */

/** 
 *  Pin direction enumeration. 
 *   
 * @details The enumeration is used to specify the direction when configuring
 *          a GPIO pin.
 */
typedef enum
{ 
  QAPI_GPIO_INPUT_E  = 0, /**< Specify the pin as an input to the SoC. */
  QAPI_GPIO_OUTPUT_E = 1, /**< Specify the pin as an output to the SoC. */

  QAPI_GPIO_INVALID_DIR_E = 0x7fffffff /* Placeholder - Do not use */

}qapi_GPIO_Direction_t;

/** 
   GPIO pin pull type.
    
  @details This enumeration specifies the type of pull (if any) to use when
           specifying the configuration for a GPIO pin.
*/
typedef enum
{
  QAPI_GPIO_NO_PULL_E    = 0x0, /**< Specify no pull. */
  QAPI_GPIO_PULL_DOWN_E  = 0x1, /**< Pull the GPIO down. */
  QAPI_GPIO_KEEPER_E  = 0x2,    /**< Keep the GPIO as it is. */
  QAPI_GPIO_PULL_UP_E    = 0x3, /**< Pull the GPIO up. */

  QAPI_GPIO_INVALID_PULL_E = 0x7fffffff /* Placeholder - Do not use */

}qapi_GPIO_Pull_t;

/** 
   GPIO pin drive strength. 
    
  @details This enumeration specifies the drive strength to use when specifying 
           the configuration of a GPIO pin. 
*/
typedef enum
{
  QAPI_GPIO_2MA_E     = 0,    /**< Specify a 2 mA drive. */
  QAPI_GPIO_4MA_E     = 0x1,  /**< Specify a 4 mA drive. */
  QAPI_GPIO_6MA_E     = 0x2,  /**< Specify a 6 mA drive. */
  QAPI_GPIO_8MA_E     = 0x3,  /**< Specify an 8 mA drive. */
  QAPI_GPIO_10MA_E    = 0x4,  /**< Specify a 10 mA drive. */
  QAPI_GPIO_12MA_E    = 0x5,  /**< Specify a 12 mA drive. */
  QAPI_GPIO_14MA_E    = 0x6,  /**< Specify a 14 mA drive. */
  QAPI_GPIO_16MA_E    = 0x7,  /**< Specify a 16 mA drive. */

  QAPI_GPIO_INVALID_STRENGTH_E = 0x7fffffff /* Placeholder - Do not use */

}qapi_GPIO_Drive_t;


/**
   GPIO configuration.
    
  @details This structure is used to specify the configuration for a GPIO on the SoC.
           The GPIO can be configured as an Input or Output, which can be
           driven High or Low by the software. The interface also allows the
    SoC pins to be configured for alternate functionality.
*/
 typedef struct
 {
  uint32_t              pin;    /**< Physical pin number. */
  uint32_t              func;   /**< Pin function select. */
  qapi_GPIO_Direction_t dir;    /**< Direction (input or output). */
  qapi_GPIO_Pull_t      pull;   /**< Pull value. */
  qapi_GPIO_Drive_t     drive;  /**< Drive strength. */

 }qapi_TLMM_Config_t;


typedef enum
{
	QAPI_FATAL_ERR_RESET = 0,
	QAPI_FATAL_ERR_SAHARA = 1,
	QAPI_FATAL_ERR_MAX
}qapi_QT_FATAL_ERR_Mode_e;

typedef enum
{
	QAPI_QT_DAM_CRASH_DUMP_MODE = 0,
	QAPI_QT_DAM_CRASH_UNLOAD_MODE = 1,
	QAPI_QT_DAM_CRASH_MODE_MAX = 2
}qapi_QT_DAM_Crash_Mode_e;

#ifndef QURT_THREAD_ATTR_NAME_MAXLEN
#define QURT_THREAD_ATTR_NAME_MAXLEN    20
#endif
typedef struct
{
	uint32_t sp_info;
	uint32_t lr_info;
	uint32_t pc_info;
	uint32_t fault_addr_info;
    char     thread_name[QURT_THREAD_ATTR_NAME_MAXLEN];
}qapi_QT_DAM_Crash_Info_t;

/*========================================================================
 *  function Definition
 *========================================================================*/
 
/**
*   
* Gets the time in the specified format. 
*
* @param[in] time_get_unit  Unit in which to get the time. 
* @param[in] time           Pointer to the #qapi_time_get_t variable. 

  @return
  QAPI_OK on success, an error code on failure.
*/
qapi_Status_t qapi_time_get(
   qapi_time_unit_type    time_get_unit,
   qapi_time_get_t* time
);

/*
@func
  qapi_QT_AP_FW_Ver_Get
@brief
  Get module AP firmware version number.
@param [in]
  version - pointer, to store the AP firmware number.
  len - pointer, the length of version string.
*/
qapi_Status_t qapi_QT_AP_FW_Ver_Get(char* version, uint16* len);

/*
@func
  qapi_QT_Remove_FOTA_Package
@brief
  Remove FOTA package.
*/
qapi_Status_t qapi_QT_Remove_FOTA_Package(void);

/*
@func
  qapi_QT_FOTA_Download_Start
@param[in]
url  FOTA package path. for example: HTTP://220.180.239.212:8005/BG95_112A_119.zip. 
response_cb  response callback, This callback will come when download failed or success.
@brief
  Start FOTA package download.
*/
qapi_Status_t qapi_QT_FOTA_Download_Start(char *url,qapi_QT_FOTA_DL_CB_t response_cb);

/*
@func
  qapi_QT_FOTA_DL_Process
@brief
  Show  fota download progress.
@param[in]
	cb:process callback This callback will come when downloading
*/
qapi_Status_t qapi_QT_FOTA_DL_Process(qapi_QT_FOTA_DL_Process_CB_t cb);

/*
@func
  qapi_QT_FOTA_Download_Cancel
@brief
  Cancel the current download process.
*/
qapi_Status_t qapi_QT_FOTA_Download_Cancel(void);


/*
@func
  qapi_QT_FOTA_WD_CFG_Set.
@brief
  Set the configuration fota watch dog.
@param[out]
  WD_info :Pointer,the configuration of fota watch dog.
@return One of the following error codes:
  		QAPI_QT_ERR_OK  -- OK.
  		Other error codes -- Failure.
*/
qapi_Status_t qapi_QT_FOTA_WD_CFG_Set(qapi_QT_FOTA_WD_CFG_t* WD_info);

/*
@func
  qapi_QT_FOTA_Update_Start
@brief
  Start FOTA Update.
  
  Note:
    Before use this interface, the delta firmware should be uploded to the flash.
*/
qapi_Status_t qapi_QT_FOTA_Update_Start(void);

/*
@func
  qapi_QT_Apps_AT_Port_Open
  [input] data:  allocate user space for the carried parameter of cb.
@brief
  Open Apps AT command port and register a callback. 
*/
qapi_Status_t qapi_QT_Apps_AT_Port_Open(qapi_QT_AT_Port_e port_id, qapi_QT_AT_Stream_ID_t *stream_id, qapi_QT_AT_Resp_Func_CB_t cb, qapi_QT_AT_Pipe_Data_t *data);

/*
@func
  qapi_QT_Apps_Send_AT
@brief
  Send AT command in specifial stream port id. 
*/
qapi_Status_t qapi_QT_Apps_Send_AT(qapi_QT_AT_Stream_ID_t stream_id, const char *command_name);

/*
@func
  qapi_QT_Apps_AT_Port_Close
@brief
  Close AT command port with stream port id. 
*/
void qapi_QT_Apps_AT_Port_Close(qapi_QT_AT_Stream_ID_t stream_id);

/**
* 
* Timed wait. Blocks a thread for a specified time. 
*
* @param[in] timeout  Specify the duration to block the thread.
* @param[in] unit     Specify the units of the duration.
* @param[in] non_deferrable  TRUE = processor (if in deep 
*       sleep or power collapse) will be awakened on timeout. \n
*       FALSE = processor will not be awakened from deep sleep
*       or power collapse on timeout. \n Whenever the processor
*       wakes up due to some other reason after timeout, the
*       thread will be unblocked.
*
* @return
* QAPI_OK on success, an error code on failure.
* 
*/
qapi_Status_t qapi_Timer_Sleep(
  uint64_t             timeout,
  qapi_TIMER_unit_type unit,
  qbool_t              non_deferrable
);

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
qapi_Status_t qapi_QT_Get_Timetick(unsigned long long *timetick);

/**
    
* Allocates internal memory in the timer module. The internal 
* memory is then formatted with parameters provided in the 
* timer_def_attr variable. The timer_handle is returned to the 
* client, and this handle must be used for any subsequent timer 
* operations. 
  
* @param[in] timer_handle  Handle to the timer. 
* @param[in] timer_attr    Attributes for defining the timer. 
  
  @return
  QAPI_OK on success, an error code on failure.
  
* @sideeffects
* Calling this API causes memory allocation. 
* Therefore, whenever the timer usage is done and
* not required, qapi_Timer_Undef() must be called
* to release the memory, otherwise it will cause a
* memory leak.
*/
qapi_Status_t qapi_Timer_Def(
   qapi_TIMER_handle_t*    timer_handle,
   qapi_TIMER_define_attr_t*  timer_attr  
);

/**
*   
* Starts the timer with the duration specified in timer_attr. 
* If the timer is specified as a reload timer in timer_attr, 
* the timer will restart after its expiry.
*
* @param[in] timer_handle  Handle to the timer. 
* @param[in] timer_attr    Attributes for setting the timer. 
*
* @return
* QAPI_OK on success, an error code on failure.
*
* @dependencies
* The qapi_Timer_Def() API should be called for the timer before 
* calling qapi_Timer_Set function.
*/
qapi_Status_t qapi_Timer_Set(
   qapi_TIMER_handle_t    timer_handle,
   qapi_TIMER_set_attr_t* timer_attr  
);


/**  
* Stops the timer.
*
* @note1hang This function does not deallocate the 
* memory that was allocated when the timer was defined.
  
* @param[in] timer_handle  Timer handle for which to stop the timer.
  
  @return
  QAPI_OK on success, an error code on failure.
* 
*/
qapi_Status_t qapi_Timer_Stop(
    qapi_TIMER_handle_t    timer_handle
);

/*
@func
  qapi_QT_Startup_Type_Get
@brief
  Gets the Module startup reason type
@param [out]
  Val  Return the Module startup reason type value
*/
qapi_Status_t qapi_QT_Startup_Type_Get(uint16 *val);

/*
@func
  qapi_QT_Chip_Serial_Num_Get
@brief
  Get the chip serial number by qapi_QT_Chip_Serial_Num_Get.
@param [out]
  Pointer, pointer to the chip serial number. 
*/
qapi_Status_t qapi_QT_Chip_Serial_Num_Get(uint32 *serial_num);

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
qapi_Status_t qapi_QT_Random_Data_Get(uint16 prng_size, uint8* prng_data);

/*============================================================================
                qapi_ADC_Open
============================================================================*/
/**
*   Opens the ADC for use by a software client.
*
*  @details ADC clients values can only be read after successfully opening ADC.
*
*  @param[out] Handle      Pointer to an ADC handle.
*  @param[in]  Attributes  Reserved parameter.
*
*  @return
*  - QAPI_OK                -- Call succeeded.
*  - QAPI_ERROR             -- Call failed.
*  - QAPI_ERR_INVALID_PARAM -- Invalid parameters were specified.
*  - QAPI_ERR_NO_MEMORY     -- No memory available to support this operation.
*  - QAPI_ERR_NO_RESOURCE   -- No more handles are available.
*/
qapi_Status_t qapi_ADC_Open
(
   qapi_ADC_Handle_t *Handle,
   uint32_t Attributes   
);

/*============================================================================
                qapi_ADC_Get_Input_Properties
============================================================================*/
/**
*  Gets the ADC channel configuration.
*
*  This function is used to get properties of ADC channels.
*
*  @param[in]  Handle           Handle provided by qapi_ADC_Open().
*  @param[in]  Channel_Name_Ptr Pointer to ADC channel name pointer.
*  @param[in]  Channel_Name_Size Size of channel name string.
*  @param[out] Properties_Ptr   ADC channel configuration.
*
*  @return
*  - QAPI_OK                -- Call succeeded.
*  - QAPI_ERROR             -- Call failed.
*  - QAPI_ERR_INVALID_PARAM -- Invalid parameters were specified.
*/
qapi_Status_t qapi_ADC_Get_Input_Properties
(
   qapi_ADC_Handle_t Handle,
   const char *Channel_Name_Ptr,
   uint32_t Channel_Name_Size,
   qapi_Adc_Input_Properties_Type_t *Properties_Ptr
);

/*============================================================================
                qapi_ADC_Close
============================================================================*/
/**
*  Closes a handle to the ADC when a software client is done with it.
*
*  @param[in] Handle         Handle provided by qapi_ADC_Open().
*  @param[in] keep_enabled   Reserved parameter.
*
*  @return
*  - QAPI_OK                -- Call succeeded.
*  - QAPI_ERROR             -- Call failed.
*  - QAPI_ERR_INVALID_PARAM -- Invalid parameters were specified.
*/
qapi_Status_t qapi_ADC_Close
(
   qapi_ADC_Handle_t Handle,
   qbool_t keep_enabled
);

/*============================================================================
                qapi_ADC_Read_Channel
============================================================================*/
/**
*  Reads an ADC channel.
*
*  This function performs a blocking ADC read for the device and channel
*  specified by the client in pAdcInputProps.
*
*  @param[in]  Handle           Handle provided by qapi_ADC_Open().
*  @param[in]  Input_Prop_Ptr   Properties pointer of channel provided by
*                                qapi_ADC_Get_Input_Properties().
*  @param[out] Result_Ptr       ADC reading result structure.
*
*  @return
*  - QAPI_OK                -- Call succeeded.
*  - QAPI_ERROR             -- Call failed.
*  - QAPI_ERR_INVALID_PARAM -- Invalid parameters were specified.
*/
qapi_Status_t qapi_ADC_Read_Channel
(
   qapi_ADC_Handle_t Handle,
   const qapi_Adc_Input_Properties_Type_t *Input_Prop_Ptr,
   qapi_ADC_Read_Result_t *Result_Ptr
);

/* ============================================================================
*  Function : qapi_TLMM_Get_Gpio_ID
* ============================================================================*/
/**
   Gets a unique access ID.
 
  @details This function provides a unique access ID for a specified GPIO. This is
           required in order to access GPIO configuration APIs.

  @param[in] qapi_TLMM_Config  Pointer to the pin configuration data.
  @param[in] qapi_GPIO_ID      Pointer to a location in which to store the access ID.
    
  @return
  QAPI_OK -- Pin GPIO ID was successfully created. \n
  QAPI_ERR -- Pin GPIO is currently in use or not programmable.
    
*/

qapi_Status_t qapi_TLMM_Get_Gpio_ID
(
  qapi_TLMM_Config_t* qapi_TLMM_Config,
  qapi_GPIO_ID_t*     qapi_GPIO_ID
);

/* ============================================================================
*  Function : qapi_TLMM_Config_Gpio
* ============================================================================*/
/**
   Changes the SoC pin configuration
 
  @details This function configures an SoC pin based on a set of fields specified
           in the configuration structure reference passed in as a parameter.
 
  @param[in] qapi_GPIO_ID      Pin access ID retrieved from the qapi_TLMM_Get_Gpio_ID() call.
  @param[in] qapi_TLMM_Config  Pin configuration to use.
    
  @return
  QAPI_OK  -- Pin was configured successfully. \n
  QAPI_ERR -- Pin could not be configured.
*/
qapi_Status_t qapi_TLMM_Config_Gpio
(
  qapi_GPIO_ID_t      qapi_GPIO_ID,
  qapi_TLMM_Config_t* qapi_TLMM_Config
);

/* ============================================================================
*  Function : qapi_TLMM_Release_Gpio_ID
* ============================================================================*/
/**
   Releases an SoC pin.
 
  @details This function allows a client to relinquish the lock on a GPIO pin. It
           facilitates sharing of a pin between two drivers in different system
       modes where each driver may need to reconfigure the pin. Using this
      function is not required unless such a condition dictates.

  @param[in]  qapi_TLMM_Config   Pointer to pin configuration data.
  @param[in]  qapi_GPIO_ID       Pin access ID retrieved from the qapi_TLMM_Get_Gpio_ID() call.
    
  @return
  QAPI_OK  -- Pin was released successfully. \n
  QAPI_ERR -- Pin could not be released.
    
*/
qapi_Status_t qapi_TLMM_Release_Gpio_ID
(
  qapi_TLMM_Config_t* qapi_TLMM_Config,
  qapi_GPIO_ID_t      qapi_GPIO_ID
);

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
qapi_Status_t qapi_QT_Sahara_Mode_Get(qapi_QT_FATAL_ERR_Mode_e* mode);

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
qapi_Status_t qapi_QT_Sahara_Mode_Set(qapi_QT_FATAL_ERR_Mode_e mode);

/*
@func
  qapi_QT_PMIC_WD_Kick
@brief
  Kick PMIC Watch Dog
*/
qapi_Status_t qapi_QT_PMIC_WD_Kick(void);

/*
@func
  qapi_QT_DAM_Crash_Mode_Set.
@brief
  Set the DAM crash mode.
@param[in]
  crash_mode : Set the DAM crash mode.
@return One of the following error codes:
  		QAPI_QT_ERR_OK  -- OK.
  		Other error codes -- Failure.
*/
qapi_Status_t qapi_QT_DAM_Crash_Mode_Set(qapi_QT_DAM_Crash_Mode_e crash_mode);

/*
@func
  qapi_QT_DAM_Crash_Mode_Get.
@brief
  Get the DAM crash mode.
@param[out]
  crash_mode : Pointer. Point to DAM crash mode.
@return One of the following error codes:
  		QAPI_QT_ERR_OK  -- OK.
  		Other error codes -- Failure.
*/
qapi_Status_t qapi_QT_DAM_Crash_Mode_Get(qapi_QT_DAM_Crash_Mode_e *crash_mode);

/*
@func
  qapi_QT_DAM_Crash_Info_Get.
@brief
  Get the DAM crash information.
@param[out]
  crash_info : Structure pointer. Point to DAM crash information.
@return One of the following error codes:
  		QAPI_QT_ERR_OK  -- OK.
  		Other error codes -- Failure.
*/
qapi_Status_t qapi_QT_DAM_Crash_Info_Get(qapi_QT_DAM_Crash_Info_t *crash_info);

/*
@func
  qapi_QT_USB_Mode_Set
@brief
  Set the sw usb on/off
@param [in]
  mode enable or disable the usb
       1  disable the usb
       0  able the usb   
*/
qapi_Status_t qapi_QT_USB_Mode_Set(boolean mode);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_CNKTD_DEVICE_H */


