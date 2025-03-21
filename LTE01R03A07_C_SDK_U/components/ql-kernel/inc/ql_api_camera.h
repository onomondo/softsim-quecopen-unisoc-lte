
/**  @file
  ql_api_camera.h

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
    
/**  
  @file
  ql_api_camera.h

  @brief
  This file provides the definitions for camera API functions.

*/
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_API_CAMERA_H
#define QL_API_CAMERA_H


#ifdef __cplusplus
extern "C" {
#endif

#include "ql_api_common.h"
#include "ql_api_osi.h"


/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_CAMERA_ERRCODE_BASE (QL_COMPONENT_BSP_CAMERA<<16)

/*===========================================================================
 * Struct
 ===========================================================================*/

typedef struct
{
    bool Cam_Init_status;       //the status of camera init
    bool Cam_Power_status;      //the status of camera pwoer
    bool Cam_Preview_status;    //the status of camera preview
}ql_cam_status_s;

typedef struct
{
    char *pNamestr;         //the name of camera
    uint32_t img_width;     //the width of image
    uint32_t img_height;    //the height of image
    uint32_t nPixcels;      //the pixcels of image
    ql_cam_status_s Status;  //the status of camera
    ql_mutex_t camera_mutex;
} ql_cam_drv_s;

/*===========================================================================
 * Enum
===========================================================================*/

typedef enum
{
    QL_CAMERA_SUCCESS = QL_SUCCESS,

    QL_CAMERA_INIT_ERR                 = 1|QL_CAMERA_ERRCODE_BASE,
    QL_CAMERA_ALREADY_INIT_ERR,
    QL_CAMERA_NO_INIT_ERR,

    QL_CAMERA_POWER_ON_ERR             = 5|QL_CAMERA_ERRCODE_BASE,
    QL_CAMERA_ALREADY_POWER_ON_ERR,
    QL_CAMERA_CLOSE_ERR,           // power off, had closed

    QL_CAMERA_PREVIEW_ERR              = 10|QL_CAMERA_ERRCODE_BASE,
    QL_CAMERA_PREVIEWING_ERR,      // Preview has started
    QL_CAMERA_NO_PREVIEW_ERR,      // Preview has ended
    QL_CAMERA_STOP_PREVIEW_ERR,    // stop preview API execute error

    QL_CAMERA_CAPTURE_ERR              = 15|QL_CAMERA_ERRCODE_BASE,
    QL_CAMERA_GET_INFO_ERR,        // camera info
    QL_CAMERA_LCDINFO_GET_ERR,     // LCD info
    QL_CAMERA_PRINT_ERR,
    QL_CAMERA_DQBUF_ERR,
    QL_CAMERA_QBUF_ERR,            // no use
    QL_CAMERA_SET_BUF_ERR,
    QL_CAMERA_CREATE_MUTEX_ERR,
    QL_CAMERA_MUTEX_ALREADY_EXIST_ERR,
    QL_CAMERA_MUTEX_TIMEOUT_ERR,
    QL_CAMERA_DELETE_MUTEX_ERR,
    QL_CAMERA_I2C_READ_ERR,
    QL_CAMERA_I2C_WRITE_ERR,
}ql_errcode_camera_e;

typedef enum
{
    single_buf,
    double_buf,
}ql_buf_config_e;

/*===========================================================================
 * Variate
 ===========================================================================*/



/*===========================================================================
 * Functions
 ===========================================================================*/

/*****************************************************************
* Function: ql_CamInit
*
* Description: Initialize the camera
* 
* Parameters:
*   width           [in]    the width of the camera
*   height          [in]    the height of the camera
*
* Return:
* 	ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamInit(uint16_t width, uint16_t height);

/*****************************************************************
* Function: ql_CamPowerOn
*
* Description: power on the camera
* 
* Parameters:
*
* Return:
* 	ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamPowerOn(void);

/*****************************************************************
* Function: ql_CamPowerOff
*
* Description: power off the camera
* 
* Parameters:
*
* Return:
* 	ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamPowerOff(void);

/*****************************************************************
* Function: ql_CamClose
*
* Description: close the camera, don't need to use ql_CamPowerOff API first
* 
* Parameters:
*
* Return:
* 	ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamClose(void);

/*****************************************************************
* Function: ql_CamGetSensorInfo
*
* Description: get the information of camera
* 
* Parameters:
*   *pCamDevice         [out]    the camera driver
* Return:
* 	ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamGetSensorInfo(ql_cam_drv_s *pCamDevice);

/*****************************************************************
* Function: ql_CamPreview
*
* Description: start preview
* 
* Parameters:
*
* Return:
* 	ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamPreview(void);

/*****************************************************************
* Function: ql_CamStopPreview
*
* Description: stop preview
* 
* Parameters:
*
* Return:
* 	ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamStopPreview(void);

/*****************************************************************
* Function: ql_CamCaptureImage
*
* Description: capture function
* 
* Parameters:
*   **pFrameBuf     [out]   the buf used to save capture image
* Return:
* 	ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamCaptureImage(uint16_t **pFrameBuf);

/*****************************************************************
* Function: ql_camPrint
*
* Description: display the image on lcd
* 
* Parameters:
*   *imageBuf       [in]   the image need to be displayed
* Return:
* 	ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_camPrint(uint16_t *imageBuf);

/*****************************************************************
* Function: ql_CamSetBufopt
*
* Description: set the number of buf
* 
* Parameters:
*   buf_cfg           [in]    the number of buffer
*
* Return:
* 	ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamSetBufopt(ql_buf_config_e buf_cfg);

/*****************************************************************
* Function: ql_CamInit0310SPI
*
* Description: modify the infomation of 0310
* 
* Parameters:void
*
* Return:
*   ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamInit0310SPI(uint16_t width, uint16_t height);

/*****************************************************************
* Function: ql_CamPreviewDQBUF
*
* Description: preview image function, DQBUF means dequeue preview buffer.
*              get preview buffer from queue. After used, must be add preview buffer to queue.
*              !!! use with ql_CamPreviewQBUF
*                  this API is only used for bwscreen LCD, color LCD still use ql_camPrint.
*
* Parameters:
*   **pFrameBuf     [out]   the buf used to get preview image
*
* Return:
*   ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamPreviewDQBUF(uint16_t **pFrameBuf);

/*****************************************************************
* Function: ql_CamPreviewQBUF
*
* Description: preview image function, QBUF means enqueue preview buffer.
*              Add free memory to the queue that can preview
*              !!! use with ql_CamPreviewDQBUF
*
* Parameters:
*   *pFrameBuf     [in]   the buf used to restart preview image
*
* Return:
*   ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamPreviewQBUF(uint16_t *pFrameBuf);

/*****************************************************************
* Function: ql_CamI2CRead
*
* Description: Cami2c master read,after the camera is successfully powered on
*
* Parameters:
*   addr     [in]   the i2c slave regiser address
*   data     [out]  the data that was read
*   len      [in]   the length of the data
*
* Return:
*   ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamI2CRead(uint8_t addr, uint8_t *data, uint32_t len);

/*****************************************************************
* Function: ql_CamI2CWrite
*
* Description: Cami2c master write,after the camera is successfully powered on
*
* Parameters:
*   addr     [in]   the i2c slave regiser address
*   data     [in]   the data need to be sent
*
* Return:
*   ql_errcode_camera_e 
*
*****************************************************************/
ql_errcode_camera_e ql_CamI2CWrite(uint8_t addr, uint8_t data);



#ifdef __cplusplus
    } /*"C" */
#endif

#endif /* QL_API_CAMERA_H*/

