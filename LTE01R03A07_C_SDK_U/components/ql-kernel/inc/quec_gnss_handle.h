
/**  @file
  quec_gnss_handle.h

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
 ******************************************************************************
 * @file    quec_gnss_handle.h
 * @author  lambert.zhao
 * @version V1.0.0
 * @date    2020/11/25
 * @brief   This file contains the quec_gnss_handle functions's declaration
 ******************************************************************************
 */

#ifndef QUEC_GNSS_HANDLE_H_
#define QUEC_GNSS_HANDLE_H_
#ifdef __cplusplus 
extern "C" {
#endif

#include "ql_uart.h"
#include "ql_api_osi.h"
#include "ql_gnss.h"

#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS

typedef unsigned char boolean;

#define QUEC_GNSS_LOCK_TIMEOUT     10000   //10sec

/*===========================================================================
 * Macro Definition
 ===========================================================================*/

/*===========================================================================
 *  Enumeration Definition
 ===========================================================================*/
typedef enum
{
    QL_GNSS_FIRMWARE_REPORT_TYPE = 0,
    QL_GNSS_APFLASH_REPORT_TYPE = 1,
    QL_GNSS_AGPS_REPORT_TYPE = 2,
    QL_GNSS_STARTMODE_REPORT_TYPE = 3,
    QL_GNSS_REPORT_TYPE_MAX,
}ql_gnss_report_type_e;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
extern quec_gnss_flag gnss_operation_param;

int quec_gnss_param_lock(ql_mutex_t mutexRef);
int quec_gnss_param_unlock(ql_mutex_t mutexRef);
QlOSStatus quec_gnss_init(void);
QlOSStatus  quec_gnss_config_init(void);
void quec_gnss_notify_cb(uint32 ind_type, ql_uart_port_number_e port, uint32 size);
void uart_gnss_data_recv(ql_uart_port_number_e port,unsigned int datelen);
int quec_gps_send_cmd(quec_gnss_cmd cmd, unsigned int *data,int len);
void quec_gnss_power_onff(bool status);
int quec_gps_handle_set(void);
int quec_gps_handle_get(quec_gps_handle *ptr_cfg);
QlOSStatus quec_gnss_uart_task_init(void);
QlOSStatus quec_gnss_cmd_task_init(void);
unsigned short crc16_ccitt(const unsigned char *buf, int len);
unsigned short quec_get_crc16 ( char *ptr, unsigned short count );
int xmodem_flash_send(ql_uart_port_number_e uart_port, int block_size, unsigned char *Name, unsigned int data_size);
int gpsautocheck(void);
nmea_type nmea_get_type(const char *sentence);
struct nmea_s* nmea_parse(char *sentence, int length, int check_checksum);
int nmea_value_update(struct nmea_s *nmea, ql_gnss_data_t *gps_data);
int ql_get_gnss_info(ql_gnss_data_t *data);
char *strptime(const char *buf, const char *fmt, struct tm *tm);
void quec_gnss_baud_set(char id,ql_uart_baud_e baudrate);
ql_errcode_gnss ql_gnss_hal_init(ql_uart_baud_e baudrate);
ql_errcode_gnss ql_gnss_open(void);
ql_errcode_gnss ql_gnss_close(void);
int ql_gnss_location_info_get(ql_uart_port_number_e uart_port,unsigned int len);
void nmea_write_pqgsv_addsystemid(unsigned char * nmea_sentence,int len);

ql_errcode_gnss ql_gnss_report_urc_flag_set(uint8_t flag);
ql_errcode_gnss ql_gnss_report_urc_flag_get(uint8_t *flag);
void ql_gnss_report_urc_flag_read_from_nv(void);

int quec_gnss_report_code_get(ql_gnss_report_type_e type);
void quec_gnss_report_code_set(ql_gnss_report_type_e type,int report);
void quec_gnss_report_hanlder(ql_gnss_report_type_e type,int report);
#endif

#ifdef __cplusplus 
}
#endif
#endif /* !QUEC_GNSS_HANDLE_H_ */

