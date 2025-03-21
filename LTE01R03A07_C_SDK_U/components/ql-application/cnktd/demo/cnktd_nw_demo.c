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
#include "ql_cnktd_nw.h"
#include "ql_api_nw.h"

#define QL_SIM_ID_0 0
#define CNKTD_NW_LOG(msg, ...)          QL_LOG(QL_LOG_LEVEL_INFO, "ql_cnktd_nw", msg, ##__VA_ARGS__)

ql_task_t cnktd_nw_task = NULL;

qapi_DSS_Hndl_s data_call_param = {
    . Profile_PDP_number = 1,
    . Profile_PDP_Context = {
        .apn = "",
        .user_name = "",
        .pass_word = "",
        .auth_type = QAPI_DSS_IP_VERSION_4_6,
    }
};
void ql_cnktd_nw_notify_cb(uint8_t sim_id, unsigned int ind_type, void *ind_msg_buf)
{    
    switch(ind_type)
    {
        case QUEC_NW_VOICE_REG_STATUS_IND:
        {
             ql_nw_common_reg_status_info_s  *voice_reg_status=(ql_nw_common_reg_status_info_s  *)ind_msg_buf;
             CNKTD_NW_LOG("Sim%d voice: state:%d; lac:0x%X; cid:0x%X; act:%d; ue_stat:%d",
			 			sim_id,
                        voice_reg_status->state,
                        voice_reg_status->lac,
                        voice_reg_status->cid,
                        voice_reg_status->act,
                        voice_reg_status->ue_state);
              break;
        }
        case QUEC_NW_DATA_REG_STATUS_IND:
        {
             ql_nw_common_reg_status_info_s  *data_reg_status=(ql_nw_common_reg_status_info_s  *)ind_msg_buf;
             CNKTD_NW_LOG("Sim%d data: state:%d; lac:0x%X; cid:0x%X; act:%d; ue_stat:%d",
			 			sim_id,
                        data_reg_status->state,
                        data_reg_status->lac,
                        data_reg_status->cid,
                        data_reg_status->act,
                        data_reg_status->ue_state);
              break;
        }
        case QUEC_NW_NITZ_TIME_UPDATE_IND:
        {
             ql_nw_nitz_time_info_s  *nitz_info=(ql_nw_nitz_time_info_s  *)ind_msg_buf;
             CNKTD_NW_LOG("nitz update: nitz_time:%s, abs_time:%ld", nitz_info->nitz_time, nitz_info->abs_time);
             break;
        }
        case QUEC_NW_CAUSE_IND:
        {
             ql_nw_cause_s  *cause_info=(ql_nw_cause_s  *)ind_msg_buf;
             CNKTD_NW_LOG("cause info: causeId:%d, causeDetail:%d", cause_info->causeId, cause_info->causeDetail);
             break;
        }
    }
}

void cnktd_set_data_call_param(qapi_DSS_Hndl_t dss_handle)
{
    qapi_DSS_Call_Param_Value_t param_info;
    // set data call param for APN
    char *apn = "";
    param_info.buf_val = apn;
    param_info.num_val = strlen(apn);
    CNKTD_NW_LOG("Setting APN:%s, APN length:%d", param_info.buf_val, param_info.num_val);
    qapi_DSS_Set_Data_Call_Param(dss_handle, QAPI_DSS_CALL_INFO_APN_NAME_E, &param_info);

    param_info.buf_val = NULL;
    param_info.num_val = QAPI_DSS_IP_VERSION_4_6;
    CNKTD_NW_LOG("Setting family to IP");
    qapi_DSS_Set_Data_Call_Param(dss_handle, QAPI_DSS_CALL_INFO_IP_VERSION_E, &param_info);

    /* set data call param for CAT1*/
    param_info.buf_val = NULL;
    param_info.num_val = 1;
    CNKTD_NW_LOG("setting profile number to: %d", 1);
    qapi_DSS_Set_Data_Call_Param(dss_handle, QAPI_DSS_CALL_INFO_LTE_PROFILE_IDX_E, &param_info);
}

static void cnktd_nw_demo_task(void *ctx)
{
    int err = 0;
    char fw_ver[64] = {0};
    uint16 len = sizeof(fw_ver);
    err = qapi_QT_MP_FW_Ver_Get(fw_ver, &len);
    CNKTD_NW_LOG("qapi_QT_MP_FW_Ver_Get err = %d, fw_ver = %s", err,fw_ver);

    char imei[16] = {0};
    uint16_t imei_len = sizeof(imei);
    err = qapi_QT_IMEI_Get(imei, &imei_len);
    CNKTD_NW_LOG("qapi_QT_IMEI_Get err=%d, IMEI = %s", err, imei);

    err = ql_nw_register_cb(ql_cnktd_nw_notify_cb);
    CNKTD_NW_LOG("ql_nw_register_cb err = %d", err);

    qapi_QT_NW_Band_Params_t band_pref = {0};
    err = qapi_QT_NW_Band_Pref_Get(&band_pref);
    CNKTD_NW_LOG("qapi_QT_NW_Band_Pref_Get err = %d, gsm_band=0x%X, cat1_band_low=0x%llX, cat1_band_high=0x%llX", err, 
                  band_pref.gsm_band, 
                  band_pref.cat1_band_low,
                  band_pref.cat1_band_high);

    err = ql_network_register_wait(QL_SIM_ID_0, 60);
    if(err != QL_SUCCESS){
        CNKTD_NW_LOG("ql_network_register_wait fail ");
		goto exit;
    }

    qapi_QT_NW_CFUN_MODE_e cfun = 0;
    err = qapi_QT_Phone_Func_Get(&cfun);
    CNKTD_NW_LOG("qapi_QT_Phone_Func_Get err = %d, cfun = %d", err, cfun);

    qapi_QT_NW_RAT_PREF_e rat_pref = 0;
    err = qapi_QT_NW_Rat_Pref_Get(&rat_pref);
    CNKTD_NW_LOG("qapi_QT_NW_Rat_Pref_Get err = %d, rat_pref = %d", err, rat_pref);

    qapi_QT_NW_Req_PSM_Cfg_t psm_cfg = {
        .req_psm_enable = 1,
        .req_active_timer_value = 0b0010001, //1 minute
        .req_periodic_tau_timer_value = 0b00100001, // 1 hour
    };
    err = qapi_QT_NW_PSM_Cfg_Set(&psm_cfg);
    CNKTD_NW_LOG("qapi_QT_NW_PSM_Cfg_Set err = %d", err);

    qapi_QT_NW_Alloc_PSM_Cfg_t alloc_psm_cfg = {0};
    err = qapi_QT_NW_PSM_Cfg_Get(&alloc_psm_cfg);
    CNKTD_NW_LOG("qapi_QT_NW_PSM_Cfg_Get err = %d,alloc_psm_enabled=%d, active_timer_value=%d, tau_timer_value=%d", 
                err,
                alloc_psm_cfg.alloc_psm_enabled,
                alloc_psm_cfg.alloc_active_timer_value,
                alloc_psm_cfg.alloc_periodic_tau_timer_value);


    qapi_QT_NW_Req_eDRX_Cfg_t edrx_cfg = {
       .req_edrx_enable = 1,
       .req_edrx_cycle = 0b0000,
       .req_ptw_cycle = 0b0000,
    };
    err = qapi_QT_NW_eDRX_Cfg_Set(&edrx_cfg);
    CNKTD_NW_LOG("qapi_QT_NW_eDRX_Cfg_Set err = %d", err);

    qapi_QT_NW_Alloc_eDRX_Cfg_t alloc_edrx_cfg = {0};
    qapi_QT_NW_RAT_e rat_mode = 0;
    err = qapi_QT_NW_eDRX_Cfg_Get(&rat_mode,&alloc_edrx_cfg);
    CNKTD_NW_LOG("qapi_QT_NW_eDRX_Cfg_Get err = %d,edrx_enabled=%d, edrx_cycle=%d, ptw_cycle=%d", 
                err,
                alloc_edrx_cfg.alloc_edrx_enable,
                alloc_edrx_cfg.alloc_edrx_cycle,
                alloc_edrx_cfg.alloc_ptw_cycle);

    qapi_QT_NW_LTE_Meas_Info_t lte_meas_info = {0};
    err = qapi_QT_NW_LTE_Meas_Info_Get(&lte_meas_info);
    CNKTD_NW_LOG("qapi_QT_NW_LTE_Meas_Info_Get err = %d", err);
    CNKTD_NW_LOG("lte_meas_info: earfcn:%d,mcc:%d,mnc:%d,tac:0x%X,cell_id:0x%X,freq_band:%d,pci:%d,rsrp:%d,rsrq:%d,rssi:%d,sinr:%d",
                  lte_meas_info.earfcn,
                  lte_meas_info.mcc,
                  lte_meas_info.mnc,
                  lte_meas_info.tac,
                  lte_meas_info.cell_id,
                  lte_meas_info.freq_band,
                  lte_meas_info.pci,
                  lte_meas_info.rsrp,
                  lte_meas_info.rsrq,
                  lte_meas_info.rssi,
                  lte_meas_info.sinr);

    qapi_DSS_Hndl_t dss_handle = NULL;
    qapi_DSS_Get_Data_Srvc_Hndl(NULL, NULL, &dss_handle);
    if (dss_handle == NULL)
    {
        CNKTD_NW_LOG("dss_handle is NULL");
        goto exit;
    }

    cnktd_set_data_call_param(dss_handle);

    err = qapi_DSS_Start_Data_Call(dss_handle);
    CNKTD_NW_LOG("qapi_DSS_Start_Data_Call, err = %d", err);

    qapi_QT_NW_DS_Profile_PDP_Context_t profile = {0};
    uint8_t pdp_context_number = 1;
    qapi_QT_NW_PDP_Cfg_Get(&pdp_context_number, &profile);
    CNKTD_NW_LOG("qapi_QT_NW_PDP_Cfg_Get err = %d, apn=%s, pdp_type=%d", err, profile.apn, profile.pdp_type);

    uint8_t ip_addr_cnt = 1;
    if(profile.pdp_type == QT_NW_DS_PROFILE_PDP_IPV4V6){
        ip_addr_cnt = 2;
    }

    if(ip_addr_cnt)
    {
        char ip_v4[16] = {0};
        char ip_v6[130] = {0};
        char ip_v4_sub[16] = {0};
        char gt_v4[16] = {0};
        char gt_v6[130] = {0};
        char dp_v4[16] = {0};
        char ds_v4[16] = {0};
        char dp_v6[130] = {0};
        char ds_v6[130] = {0};
        qapi_DSS_Addr_Info_t *qapi_DSS_Addr_Info_ptr = calloc(1, sizeof(qapi_DSS_Addr_Info_t) * ip_addr_cnt);
        qapi_DSS_Addr_Info_t *info_ptr = qapi_DSS_Addr_Info_ptr;
        if(info_ptr == NULL)
        {
            CNKTD_NW_LOG("calloc info_ptr failed");
            goto exit;
        }

        err = qapi_DSS_Get_IP_Addr(dss_handle, info_ptr, ip_addr_cnt);
        CNKTD_NW_LOG("qapi_DSS_Get_IP_Addr err = %d", err);
        if(err == QAPI_OK)
        {
            if(profile.pdp_type == QAPI_DSS_IP_VERSION_4 || profile.pdp_type == QAPI_DSS_IP_VERSION_4_6)
            {
                CNKTD_NW_LOG("V4, ip=%s, subnet mask=%s, gateway=%s, Primary DNS=%s, Secondary DNS=%s",
                                inet_ntop(AF_INET, &info_ptr->iface_addr_s.addr.v4, ip_v4, sizeof(ip_v4)),
                                inet_ntop(AF_INET, &info_ptr->iface_mask, ip_v4_sub, sizeof(ip_v4_sub)),
                                inet_ntop(AF_INET, &info_ptr->gtwy_addr_s.addr.v4, gt_v4, sizeof(gt_v4)),
                                inet_ntop(AF_INET, &info_ptr->dnsp_addr_s.addr.v4, dp_v4, sizeof(dp_v4)),
                                inet_ntop(AF_INET, &info_ptr->dnss_addr_s.addr.v4, ds_v4, sizeof(ds_v4)));
                if(profile.pdp_type == QAPI_DSS_IP_VERSION_4_6)
                {
                    info_ptr++;
                }
            }

            if(profile.pdp_type == QAPI_DSS_IP_VERSION_6 || profile.pdp_type == QAPI_DSS_IP_VERSION_4_6)
            {
                CNKTD_NW_LOG("V6, ip=%s, prefix length=%u, gateway=%s, gateway length=%u, Primary DNS=%s, Secondary DNS=%s",
                            inet_ntop(AF_INET6, &info_ptr->iface_addr_s.addr.v6_addr64, ip_v6, sizeof(ip_v6)),
                            info_ptr->iface_mask,
                            inet_ntop(AF_INET6, &info_ptr->gtwy_addr_s.addr.v6_addr64, gt_v6, sizeof(gt_v6)),
                            info_ptr->gtwy_mask,
                            inet_ntop(AF_INET6, &info_ptr->dnsp_addr_s.addr.v6_addr64, dp_v6, sizeof(dp_v6)),
                            inet_ntop(AF_INET6, &info_ptr->dnss_addr_s.addr.v6_addr64, ds_v6, sizeof(ds_v6)));
            }
        }
        free(qapi_DSS_Addr_Info_ptr);
    }

    err = qapi_DSS_Stop_Data_Call(dss_handle);
    CNKTD_NW_LOG("qapi_DSS_Stop_Data_Call err = %d", err);

    qapi_DSS_Rel_Data_Srvc_Hndl(dss_handle);

    while(1)
    {
        ql_rtos_task_sleep_ms(1000);
    }

exit:
    err = ql_rtos_task_delete(NULL);
    if(err != QL_OSI_SUCCESS)
	{
		//CNKTD_NW_LOG("task deleted failed");
	}
}

QlOSStatus cnktd_nw_demo_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;

	err = ql_rtos_task_create(&cnktd_nw_task, 4*1024, APP_PRIORITY_NORMAL, "cnktd_nw_demo", cnktd_nw_demo_task, NULL, 4);
	CNKTD_NW_LOG("cnktd_nw_demo_task, err = %d", err);
    
    return err;
}