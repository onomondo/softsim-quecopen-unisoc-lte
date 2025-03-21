/*============================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------

=============================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_api_ss.h"
#include "ql_api_nw.h"
#include "ql_uart.h"
#include "ussd_demo.h"

#define QL_USSD_LOG_LEVEL	QL_LOG_LEVEL_INFO
#define QL_USSD_DEMO_LOG(msg, ...)			QL_LOG(QL_USSD_LOG_LEVEL, "ussd_demo", msg, ##__VA_ARGS__)

ql_task_t ussd_task = NULL;

ql_sem_t ql_ussd_nw_reg_sem = NULL;

uint8_t ql_ussd_buffer[QUEC_SS_USSD_UCS2_SIZE_MAX] = {0};

void ql_ussd_nw_notify_cb(uint8_t sim_id, unsigned int ind_type, void *ind_msg_buf)
{    
    switch(ind_type)
    {
        case QUEC_NW_DATA_REG_STATUS_IND:
        {
            ql_nw_common_reg_status_info_s  *data_reg_status=(ql_nw_common_reg_status_info_s  *)ind_msg_buf;
            QL_USSD_DEMO_LOG("Sim%d data reg status changed, current status is %d", sim_id, data_reg_status->state);
            if((QL_NW_REG_STATE_HOME_NETWORK == data_reg_status->state) || (QL_NW_REG_STATE_ROAMING == data_reg_status->state))
            {
                ql_rtos_semaphore_release(ql_ussd_nw_reg_sem);
            }
            break;
        }
    }
}

void ql_ss_ussd_ind_cb(uint8_t sim_id,unsigned int ind_type,void *ind_msg_buf)
{
    if(QUEC_SS_USSD_IND == ind_type)
    {
        ql_ss_ussd_str_s *ussd_str = (ql_ss_ussd_str_s *)ind_msg_buf;
        QL_USSD_DEMO_LOG("ussd resp:%d",ussd_str->resp_type);
        switch(ussd_str->resp_type)
        {
            case QL_SS_USSD_RESP_SUCCESS:
            {
                QL_USSD_DEMO_LOG("ussd dcs:%d,len:%d",ussd_str->dcs,ussd_str->len);
                if(0 < ussd_str->len)
                {
                    memcpy((void*)ql_ussd_buffer,(void*)ussd_str->str,ussd_str->len);
                    //Display
                    QL_USSD_DEMO_LOG("ussd code:%s",ql_ussd_buffer);

                    /*
                    int ret = ql_uart_open(QL_UART_PORT_1);
                    QL_USSD_DEMO_LOG("ussd uart open: 0x%x", ret);
                    int write_len = ql_uart_write(QL_UART_PORT_1, ql_ussd_buffer, strlen((char *)ql_ussd_buffer));
                    if(write_len < 0)
                    {
                        QL_USSD_DEMO_LOG("ussd uart send fail");
                    }
                    ret = ql_uart_close(QL_UART_PORT_1);
                    QL_USSD_DEMO_LOG("ussd uart close: 0x%x", ret);
                    */

                    bool state = false;
                    if(QL_SS_SUCCESS != ql_ss_ussd_get_session_state(sim_id,&state))
                    {
                        QL_USSD_DEMO_LOG("ussd get state fail");
                    }
                    if(state)
                    {
                        QL_USSD_DEMO_LOG("ussd session continue");
                        /*
                        //Select menu "2"
                        ql_ss_ussd_send_s ctx = {
                            .option = QL_SS_USSD_SESSION_INITIATE,
                            .str = "2",
                        };
                        ret = ql_ss_ussd_send(sim_id,&ctx);
                        */
                        /*
                        //User cancel current session.And str should be NULL when cancel session.
                        ql_ss_ussd_send_s ctx = {
                            .option = QL_SS_USSD_SESSION_CANCEL,
                            .str = NULL,
                        };
                        ret = ql_ss_ussd_send(sim_id,&ctx);
                        */
                    }
                    else
                    {
                        QL_USSD_DEMO_LOG("ussd session end");
                    }
                }
                break;
            }
            case QL_SS_USSD_CANCEL_SUCCESS:
            {
                QL_USSD_DEMO_LOG("user cancle success");
                break;
            }
            case QL_SS_USSD_OTHER_ERR:
            {
                QL_USSD_DEMO_LOG("ussd error code:%ld",ussd_str->err_code);
                /*
                if(QL_SS_UNKNOWN == ussd_str->err_code)
                {
                    //Try send again
                    ql_ss_ussd_send_s ctx = {
                        .option = QL_SS_USSD_SESSION_INITIATE,
                        .str = "*101#",
                    };
                    ret = ql_ss_ussd_send(sim_id,&ctx);
                }
                */
                break;
            }
            case QL_SS_USSD_SAT_SUCCESS:
            case QL_SS_USSD_NO_STR_ERR:
            default:break;
        }
    }
    return;
}

static void ussd_app_thread(void * arg)
{
    uint8_t sim_id = 0;
    uint8_t dcs = 0x0F;
    int ret = 0;
    //wait for nw register
    ql_nw_reg_status_info_s* nw_info =  (ql_nw_reg_status_info_s*)calloc(1,sizeof(ql_nw_reg_status_info_s));
    if(!nw_info)
    {
        QL_USSD_DEMO_LOG("nw_info no mem");
        goto exit;
    }
    ret = ql_rtos_semaphore_create(&ql_ussd_nw_reg_sem, 0);
    if(ret != QL_OSI_SUCCESS)
    {
        QL_USSD_DEMO_LOG("ql_ussd_nw_reg_sem created failed, ret = 0x%x", ret);
        goto exit;
    }
    if(QL_NW_SUCCESS != ql_nw_register_cb(ql_ussd_nw_notify_cb))
    {
        QL_USSD_DEMO_LOG("register network cb fail");
        goto exit;
    }

    ret = ql_nw_get_reg_status(sim_id, nw_info);
    if((QL_NW_REG_STATE_HOME_NETWORK != nw_info->data_reg.state) && (QL_NW_REG_STATE_ROAMING != nw_info->data_reg.state))
    {
        if(ql_rtos_semaphore_wait(ql_ussd_nw_reg_sem, QL_WAIT_FOREVER))
        {
            QL_USSD_DEMO_LOG("ql_ussd_nw_reg_sem time out");
            goto exit;
        }
    }

    if(QL_SS_SUCCESS != ql_ss_register_cb(ql_ss_ussd_ind_cb))
    {
        QL_USSD_DEMO_LOG("register ussd cb fail!");
        goto exit;
    }
    if(QL_SS_SUCCESS != ql_ss_ussd_set_dcs(sim_id,dcs))
    {
        QL_USSD_DEMO_LOG("user set dcs fail");
    }
    if(QL_SS_SUCCESS != ql_ss_ussd_get_dcs(sim_id,&dcs))
    {
        QL_USSD_DEMO_LOG("user get dcs%d",dcs);
    }
    QL_USSD_DEMO_LOG("ussd send");
    //send ussd code
    ql_ss_ussd_send_s ctx = {
        .option = QL_SS_USSD_SESSION_INITIATE,
        .str = "*101#",
    };
    ret = ql_ss_ussd_send(sim_id,&ctx);
    if(QL_SS_SUCCESS != ret)
    {
        QL_USSD_DEMO_LOG("ussd send err%x",ret);
    }
	while(1)
	{
        ql_rtos_task_sleep_ms(5000);
	}

exit:
    if(nw_info)
    {
        free(nw_info);
    }
    ql_rtos_semaphore_delete(ql_ussd_nw_reg_sem);
    ql_ussd_nw_reg_sem = NULL;
    ql_ss_register_cb(NULL);
    if(ussd_task != NULL)
    {
        ql_rtos_task_delete(ussd_task);
        ussd_task = NULL;
    }
}


void ql_ussd_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;

	err = ql_rtos_task_create(&ussd_task, 4*1024, APP_PRIORITY_NORMAL, "QUSSDDEMO", ussd_app_thread, NULL, 10);
    if(err != QL_OSI_SUCCESS)
	{
		QL_USSD_DEMO_LOG("task created failed");
        return;
	}
    
}

