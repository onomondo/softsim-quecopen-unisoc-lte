
/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

/**  @file
  ql_i2c.h

  @brief
  This file provides the definitions for i2c API functions.

*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/
                
/**  
  @file
  ql_i2c.h

  @brief
  This file provides the definitions for i2c API functions.

*/
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_I2C_H
#define QL_I2C_H

#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
===========================================================================*/
#define i2c_max_count 3

#define QL_I2C_ERRCODE_BASE (QL_COMPONENT_BSP_I2C<<16)

/*===========================================================================
 * Enum
===========================================================================*/
typedef enum
{
    STANDARD_MODE = 0,  //Standard mode (100K)
    FAST_MODE = 1,      //Fast mode (400K)
    SLOW_MODE = 5,      //Slow mode (50K)
} ql_i2c_mode_e;

typedef enum
{
    i2c_1 = 0,  //i2c channel 1
    i2c_2,      //i2c channel 2
    i2c_3,      //i2c channel 3
}ql_i2c_channel_e;

typedef enum
{
    QL_I2C_SUCCESS = QL_SUCCESS,

    QL_I2C_INIT_ERR                 = 1|QL_I2C_ERRCODE_BASE,
    QL_I2C_NOT_INIT_ERR,
    QL_I2C_INVALID_PARAM_ERR,

    QL_I2C_WRITE_ERR                = 5|QL_I2C_ERRCODE_BASE,
    QL_I2C_READ_ERR,
    QL_I2C_RELEASE_ERR,

    QL_I2C_CREATE_MUTEX_ERR,
    QL_I2C_MUTEX_TIMEOUT_ERR
}ql_errcode_i2c_e;

/*===========================================================================
 * Struct
===========================================================================*/

/*===========================================================================
 * Function
===========================================================================*/

/*****************************************************************
* Function: ql_I2cInit
*
* Description: Initialize the i2c master
* 
* Parameters:
*   i2c_no        [in]    the i2c channel 
*   fastMode      [in]    the i2c speed mode
*
* Return:
* 	QL_I2C_SUCCESS
*	QL_I2C_INIT_ERR 
*
*****************************************************************/
ql_errcode_i2c_e ql_I2cInit(ql_i2c_channel_e i2c_no, ql_i2c_mode_e Mode);

/*****************************************************************
* Function: ql_I2cWrite
*
* Description: i2c master write
* 
* Parameters:
*   i2c_no        [in]    the i2c channel 
*   slave         [in]    the i2c slave address
*   addr          [in]    the i2c slave regiser address
*   data          [in]    the data need to be sent
*   length        [in]    the length of the data
*
* Return:
* 	QL_I2C_SUCCESS
*	QL_I2C_WRITE_ERR
*
*****************************************************************/
ql_errcode_i2c_e ql_I2cWrite(ql_i2c_channel_e i2c_no, uint8_t slave, uint8_t addr, uint8_t *data, uint32_t length);

/*****************************************************************
* Function: ql_I2cRead
*
* Description: i2c master read
* 
* Parameters:
*   i2c_no        [in]    the i2c channel 
*   slave         [in]    the i2c slave address
*   addr          [in]    the i2c slave regiser address
*   buf           [out]   the data that was read
*   length        [in]    the length of the data
*
* Return:
* 	QL_I2C_SUCCESS
*	QL_I2C_READ_ERR
*
*****************************************************************/
ql_errcode_i2c_e ql_I2cRead(ql_i2c_channel_e i2c_no, uint8_t slave, uint8_t addr, uint8_t *buf, uint32_t length);

/*****************************************************************
* Function: ql_I2cRelease
*
* Description: i2c master release
* 
* Parameters:
*   i2c_no        [in]    the i2c channel 
*
* Return:
* 	QL_I2C_SUCCESS
*	QL_I2C_RELEASE_ERR
*
*****************************************************************/
ql_errcode_i2c_e ql_I2cRelease(ql_i2c_channel_e i2c_no);

/*****************************************************************
* Function: ql_I2cWrite_16bit_addr
*
* Description: i2c master write
* 
* Parameters:
*   i2c_no        [in]    the i2c channel 
*   slave         [in]    the i2c slave address
*   addr          [in]    the i2c slave regiser address
*   data          [in]    the data need to be sent
*   length        [in]    the length of the data
*
* Return:
* 	QL_I2C_SUCCESS
*	QL_I2C_WRITE_ERR
*
*****************************************************************/
ql_errcode_i2c_e ql_I2cWrite_16bit_addr(ql_i2c_channel_e i2c_no, uint8_t slave, uint16_t addr, uint8_t *data, uint32_t length);

/*****************************************************************
* Function: ql_I2cRead_16bit_addr
*
* Description: i2c master read
* 
* Parameters:
*   i2c_no        [in]    the i2c channel 
*   slave         [in]    the i2c slave address
*   addr          [in]    the i2c slave regiser address
*   buf           [out]   the data that was read
*   length        [in]    the length of the data
*
* Return:
* 	QL_I2C_SUCCESS
*	QL_I2C_READ_ERR
*
*****************************************************************/
ql_errcode_i2c_e ql_I2cRead_16bit_addr(ql_i2c_channel_e i2c_no, uint8_t slave, uint16_t addr, uint8_t *buf, uint32_t length);


/****** I2C's slave API 是为特定场景使用，其他场景请勿使用 ******/
/****** 请在明确需求后使用以下API,以下API没有寄存器的概念 ******/
/****** 根据从机地址找到设备后，直接传输data，没有寄存器的设置 ******/

/****** 7bits API 是为了让slave API与其他I2C API在从机地址上保持一致 ******/
// for ql_i2c_write_slave_data/ql_i2c_read_slave_data, [slave] param shift 1 bit left from the slave address.
// for ql_i2c_write_slave_data_7bits/ql_i2c_read_slave_data_7bits, same as ql_I2cWrite, the [slave] param don't need left shift.
#define ql_i2c_write_slave_data_7bits(i2c_no, slave, data, length) \
        ql_i2c_write_slave_data(i2c_no, (slave << 1), data, length)

#define ql_i2c_read_slave_data_7bits(i2c_no, slave, data, length) \
        ql_i2c_read_slave_data(i2c_no, (slave << 1) + 1, data, length)

/*****************************************************************
* Function: ql_i2c_write_slave_data
*
* Description: i2c master write pure data
* 
* Parameters:
*   i2c_no        [in]    the i2c channel 
*   slave         [in]    the i2c slave address
*                      notice: the value shift 1 bit left from the slave address
*                              write operation: value no change
*   data          [in]    the data need to be sent
*   length        [in]    the length of the data
*
* Return:
* 	QL_I2C_SUCCESS
*	QL_I2C_WRITE_ERR
*
*****************************************************************/
ql_errcode_i2c_e ql_i2c_write_slave_data(ql_i2c_channel_e i2c_no, uint8_t slave, uint8_t *data, uint32_t length);

/*****************************************************************
* Function: ql_i2c_read_slave_data
*
* Description: i2c master read pure value
* 
* Parameters:
*   i2c_no        [in]    the i2c channel 
*   slave         [in]    the i2c slave address
*                      notice: the value shift 1 bit left from the slave address
*                              read operation: value need add 1
*   buf           [out]   the data that was read
*   length        [in]    the length of the data
*
* Return:
* 	QL_I2C_SUCCESS
*	QL_I2C_READ_ERR
*
*****************************************************************/
ql_errcode_i2c_e ql_i2c_read_slave_data(ql_i2c_channel_e i2c_no, uint8_t slave, uint8_t *buf, uint32_t length);


#ifdef __cplusplus
   } /*"C" */
#endif
   
#endif /* QL_I2C_H */
   




