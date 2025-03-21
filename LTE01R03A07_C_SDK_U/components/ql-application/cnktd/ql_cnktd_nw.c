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
#include "ql_cnktd_nw.h"
#include "ql_log.h"
#include "ql_api_nw.h"
#include "ql_power.h"
#include "ql_api_dev.h"


#define QL_CNKTD_NW_LOG(msg, ...)	QL_LOG_TAG(QL_LOG_LEVEL_INFO, QL_LOG_TAG_OPEN, "ql_cnktd_nw", msg, ##__VA_ARGS__)       

#define QL_SIM_ID_0 0

/**
 * Initializes the device information context.
 *
 * @param[in] device_info_hndl Pointer to device info hndl
 *
 *
 * @return QAPI_OK on success, QAPI_ERROR on failure.
 */
qapi_Status_t qapi_Device_Info_Init_v2(qapi_Device_Info_Hndl_t *device_info_hndl)
{
    return QAPI_OK;
}

qapi_Status_t qapi_Device_Info_Get_v2(qapi_Device_Info_Hndl_t device_info_hndl, qapi_Device_Info_ID_t id, qapi_Device_Info_t *info)
{
    return QAPI_OK;
}

bool qapi_ByteToBitStr(char *bitStrDest, uint8_t byteSource, uint8_t bitLen)
{
    uint8_t i = 0;
    uint8_t bit;

    if (NULL == bitStrDest)
    {
        return false;
    }

    if (bitLen > 8 || bitLen == 0)
    {
        return false;
    }

    for (i = 0; i < bitLen; i++)
    {
        bit = (byteSource >> (bitLen - i - 1)) & 0x01;
        if (bit == 1)
        {
            bitStrDest[i] = '1';
        }
        else if (bit == 0)
        {
            bitStrDest[i] = '0';
        }
    }
    bitStrDest[i] = '\0';
    return true;
}

/*
@func
  qapi_QT_NW_PSM_Cfg_Get
@brief
  Get PSM of configuration.
*/
qapi_Status_t qapi_QT_NW_PSM_Cfg_Get(qapi_QT_NW_Alloc_PSM_Cfg_t* psm_cfg)
{
    qapi_Status_t status = QAPI_OK;
    if(NULL == psm_cfg){
        return QAPI_ERR_INVALID_PARAM;
    }

    ql_psm_timer_s psm_timer = {0};
    if(ql_psm_get_timer(QL_SIM_ID_0, &psm_timer) != 0){
        return QAPI_ERROR;
    }

    psm_cfg->alloc_active_timer_value = psm_timer.active_time;
    psm_cfg->alloc_periodic_tau_timer_value = psm_timer.periodic_TAU_time;
    psm_cfg->alloc_psm_enabled = psm_timer.is_valid;

    return status;
}

/*
@func
  qapi_QT_NW_PSM_Cfg_Set
@brief
  Set configaration of PSM.
*/
qapi_Status_t qapi_QT_NW_PSM_Cfg_Set(qapi_QT_NW_Req_PSM_Cfg_t *psm_cfg)
{
    qapi_Status_t status = QAPI_OK;

    if(psm_cfg == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    if(psm_cfg->req_active_timer_value > 0xFF || psm_cfg->req_periodic_tau_timer_value > 0xFF)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    
    char periodic_TAU_time[10] = {0};
    char active_time[10] = {0};

    qapi_ByteToBitStr(periodic_TAU_time, psm_cfg->req_periodic_tau_timer_value, 8);
    qapi_ByteToBitStr(active_time, psm_cfg->req_active_timer_value, 8);

    if(ql_psm_sleep_enable(QL_SIM_ID_0, psm_cfg->req_psm_enable, periodic_TAU_time, active_time) != 0){
        return QAPI_ERROR;
    }

    return status;
}

/*
@func
  qapi_QT_NW_eDRX_Cfg_Get
@brief
  get configaration of eDRX.
*/
qapi_Status_t qapi_QT_NW_eDRX_Cfg_Get(qapi_QT_NW_RAT_e *rat_mode, qapi_QT_NW_Alloc_eDRX_Cfg_t* edrx_cfg)
{
    qapi_Status_t status = QAPI_OK;
    
    ql_edrx_cfg_s ql_edrx_cfg = {0};
    if(ql_nw_get_edrx(QL_SIM_ID_0, &ql_edrx_cfg) != 0){
        return QAPI_ERR_INVALID_PARAM;
    }
    edrx_cfg->alloc_edrx_cycle = ql_edrx_cfg.nwEDRX;
    edrx_cfg->alloc_edrx_enable = ql_edrx_cfg.sib1_edrx_enable;
    edrx_cfg->alloc_ptw_cycle = ql_edrx_cfg.nwPTW;
    edrx_cfg->req_edrx_cycle = ql_edrx_cfg.reqEDRX;
    edrx_cfg->req_ptw_cycle = ql_edrx_cfg.reqPTW;
    return status;
}

/*
@func
  qapi_QT_NW_eDRX_Cfg_Set
@brief
  Set configaration of eDRX.
*/
qapi_Status_t qapi_QT_NW_eDRX_Cfg_Set(qapi_QT_NW_Req_eDRX_Cfg_t *edrx_cfg)
{
    qapi_Status_t status = QAPI_OK;

    if(edrx_cfg == NULL)
    {
        return QAPI_ERROR;
    }

	ql_edrx_cfg_s ql_edrx_cfg = {0};
    ql_edrx_cfg.actType = QL_NW_EDRX_ACTTYPE;
    ql_edrx_cfg.mode = edrx_cfg->req_edrx_enable;
    ql_edrx_cfg.reqEDRX = edrx_cfg->req_edrx_cycle;
    ql_edrx_cfg.reqPTW = edrx_cfg->req_ptw_cycle;

    if(ql_nw_set_edrx(0, &ql_edrx_cfg) != 0)
    {
        status = QAPI_ERR_INVALID_PARAM;
    }
    return status;
}


/*
@func
  qapi_QT_Phone_Func_Get
@brief
  Get phone function. 
@param
  fun -- Current configure parameter.
*/
qapi_Status_t qapi_QT_Phone_Func_Get(qapi_QT_NW_CFUN_MODE_e *fun)
{
    qapi_Status_t status = QAPI_OK;

    if(fun == NULL)
    {
        return QAPI_ERROR;
    }

    int ret = ql_dev_get_modem_fun(fun, QL_SIM_ID_0);
    if(ret != 0){
        status = QAPI_ERROR;
    }

    return status;
}


/*
@func
  qapi_QT_Phone_Func_Set
@brief
  Set phone function. 
@param
  fun -- Configure option.
*/
qapi_Status_t qapi_QT_Phone_Func_Set(qapi_QT_NW_CFUN_MODE_e *fun)
{
    qapi_Status_t status = QAPI_OK;
    int ret = 0;
    if(fun == NULL)
    {
        return QAPI_ERROR;
    }
    if(*fun >= QT_NW_CFUN_MAX)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    switch(*fun)
    {
        case QT_NW_CFUN_MIN_FUNC:
            ret = ql_dev_set_modem_fun(QL_DEV_CFUN_MIN, false, QL_SIM_ID_0);
        break;
        case QT_NW_CFUN_FUNN_FUNC:
            ret = ql_dev_set_modem_fun(QL_DEV_CFUN_FULL, false, QL_SIM_ID_0);
        break;
        case QT_NW_CFUN_SHUT_DOWN:
            ret = ql_power_down(POWD_ORDINARY);
        break;
        case QT_NW_CFUN_RESET:
            ret = ql_power_reset(RESET_NORMAL);
        break;
        default:
        break;
    }

    if(ret == 0){
        status = QAPI_OK;
    }
    else if(ret == QL_DEV_INVALID_PARAM_ERR || ret == QL_POWER_POWD_INVALID_PARAM_ERR){
        status = QAPI_ERR_INVALID_PARAM;
    }else{
        status = QAPI_ERROR;
    }
	
    return status;
}


/*
@func
  qapi_QT_NW_Rat_Pref_Get
@brief
  Get RAT of configuration.
*/
qapi_Status_t qapi_QT_NW_Rat_Pref_Get(qapi_QT_NW_RAT_PREF_e *mode)
{
    qapi_Status_t status = QAPI_OK;

    if(mode == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    ql_nw_mode_type_e nw_mode = 0;
    if(ql_nw_get_mode(QL_SIM_ID_0, &nw_mode) != 0){
        return QAPI_ERROR;
    }

    /*Don't support CATNB*/
    if(nw_mode == QL_NW_MODE_AUTO){
        *mode = QT_NW_PREF_GSM_CAT1;
    }else if(nw_mode == QL_NW_MODE_GSM){
        *mode = QT_NW_PREF_GSM;
    }else if(nw_mode == QL_NW_MODE_LTE){
        *mode = QT_NW_PREF_CAT1;
    }

    return status;
}

/*
@func
  qapi_QT_NW_Rat_Scan_Pre_Get
@brief
  Get RAT scan of configuration.
*/
qapi_Status_t qapi_QT_NW_Rat_Scan_Pre_Get(qapi_QT_NW_RAT_SCAN_ORDER_e *order)
{
    return QAPI_ERR_NOT_SUPPORTED;
}


/*
@func
  qapi_QT_NW_Rat_Scan_Pre_Set
@brief
  Set preferred RAT scan.
*/
qapi_Status_t qapi_QT_NW_Rat_Scan_Pre_Set(qapi_QT_NW_RAT_SCAN_ORDER_e *order)
{
    return QAPI_ERR_NOT_SUPPORTED;
}


/*
@func
  qapi_QT_NW_Srv_Domain_Pref_Get
@brief
  Get service domain of configuration.
*/
qapi_Status_t qapi_QT_NW_Srv_Domain_Pref_Get(qapi_QT_NW_SRV_DOMAIN_PREF_e *domain)
{
    return QAPI_ERR_NOT_SUPPORTED;
}

/*
@func
  qapi_QT_NW_Srv_Domain_Pref_Set
@brief
  Set perferred service domain.
*/
qapi_Status_t qapi_QT_NW_Srv_Domain_Pref_Set(qapi_QT_NW_SRV_DOMAIN_PREF_e *domain)
{
    return QAPI_ERR_NOT_SUPPORTED;
}

/*
@func
  qapi_QT_NW_LTE_Meas_Info_Get
@brief
  Get measure information of LTE.
*/
qapi_Status_t qapi_QT_NW_LTE_Meas_Info_Get(qapi_QT_NW_LTE_Meas_Info_t* meas_info)
{
    qapi_Status_t status = QAPI_OK;

    if(meas_info == NULL)
    {
        return QAPI_ERROR;
    }

    ql_nw_reg_status_info_s signal_info = {0};
    if(ql_nw_get_reg_status(QL_SIM_ID_0, &signal_info) != 0){
        return QAPI_ERROR;
    }

    ql_nw_seclection_info_s select_info = {0};
    if(ql_nw_get_selection(QL_SIM_ID_0, &select_info) != 0){
        return QAPI_ERROR;
    }
    meas_info->mcc     = atoi(select_info.mcc);
    meas_info->mnc     = atoi(select_info.mnc);
    meas_info->cell_id = signal_info.data_reg.cid;
    meas_info->tac     = signal_info.data_reg.lac;

    meas_info->freq_band = signal_info.scell_info.band;
    meas_info->earfcn    = signal_info.scell_info.freq;

    meas_info->pci       = signal_info.scell_info.info.lte.pci;
    meas_info->rssi      = signal_info.scell_info.info.lte.rssi;
    meas_info->rsrp      = signal_info.scell_info.info.lte.rsrp;
    meas_info->rsrq      = signal_info.scell_info.info.lte.rsrq;
    meas_info->sinr      = signal_info.scell_info.info.lte.sinr;
	
    return status;
}

/*
@func
  qapi_QT_NW_Band_Pref_Get
@brief
  Config preferred band(B1 ~ B64).
*/
qapi_Status_t qapi_QT_NW_Band_Pref_Get(qapi_QT_NW_Band_Params_t *band_pref)
{
    ql_nw_band_info_t band_info = {0};
    if(band_pref == NULL)
    {
        return QAPI_ERROR;
    }

    if(ql_nw_get_cfg_band(QL_SIM_ID_0, &band_info) != 0){
        return QAPI_ERROR;
    }

    band_pref->gsm_band = band_info.gsmbandval;
    band_pref->cat1_band_low = band_info.ltebandval;
    band_pref->cat1_band_high = band_info.lteextbandval;
    band_pref->nb_band_low = 0;

    return QAPI_OK;
}


/*
@func
  qapi_QT_NW_Band_Pref_Set
@brief
  Config preferred band(B1 ~ B64).
*/
qapi_Status_t qapi_QT_NW_Band_Pref_Set(qapi_QT_NW_Band_Params_t *band_pref)
{
    if(band_pref == NULL)
    {
        return QAPI_ERROR;
    }

    ql_nw_band_info_t band_info = {0};
    band_info.gsmbandval = band_pref->gsm_band;
    band_info.ltebandval = band_pref->cat1_band_low;
    band_info.lteextbandval = band_pref->cat1_band_high;
    
    if(ql_nw_set_cfg_band(QL_SIM_ID_0, &band_info) != 0){
        return QAPI_ERROR;
    }

    return QAPI_OK;
}

/*
@func
  qapi_QT_NW_Extend_Band_Pref_Get
@brief
  Config preferred band(B65 ~ B128).
*/
qapi_Status_t qapi_QT_NW_Extend_Band_Pref_Get(uint64 *cat1_band, uint64 *nb_band)
{
    //Please use qapi_QT_NW_Band_Pref_Get to get band(B65 ~ B128).
    return QAPI_ERR_NOT_SUPPORTED;
}

/*
@func
  qapi_QT_NW_Extend_Band_Pref_Set
@brief
  Config preferred band(B65 ~ B128).
*/
qapi_Status_t qapi_QT_NW_Extend_Band_Pref_Set(uint64_t *cat1_band, uint64_t *nb_band)
{
    //Please use qapi_QT_NW_Band_Pref_Set to set band(B65 ~ B128).
    return QAPI_ERR_NOT_SUPPORTED;
}


/*
@func
  qapi_QT_MP_FW_Ver_Get
@brief
  Get module firmware version number.
@param [in]
  version - pointer, to the buffer which store the user application log.
            Maximum support 64 bytes.
  len - pointer,to the buffer which get the version size.
*/
qapi_Status_t qapi_QT_MP_FW_Ver_Get(char* version, uint16 *len)
{
    if(version == NULL || len == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    if(ql_dev_get_cp_version(version, (uint8_t *)len)!= 0){
        return QAPI_ERROR;
    }

    return QAPI_OK;
}

/*
@func
  qapi_QT_NW_PDP_Cfg_Get
@brief
  Get PDP parameter of configuration.
*/
qapi_Status_t qapi_QT_NW_PDP_Cfg_Get(uint8_t *pdp_context_number, qapi_QT_NW_DS_Profile_PDP_Context_t* profile)
{
    qapi_Status_t status = QAPI_OK;

    if((pdp_context_number == NULL)||(profile == NULL))
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if(*pdp_context_number > 7){
        return QAPI_ERR_INVALID_PARAM;
    }

    ql_data_call_info_s data_call_info = {0};
    if(ql_get_data_call_info(QL_SIM_ID_0, *pdp_context_number, &data_call_info) != 0){
        return QAPI_ERROR;
    }

    profile->pdp_type  = data_call_info.ip_version-1; //change QL_PDP_TYPE_XXX to qapi_QT_NW_DS_PROFILE_PDP_TYPE_e
    profile->auth_type = data_call_info.auth_type;
    strcpy((char *)profile->apn, (char *)data_call_info.apn);
    strcpy((char *)profile->user_name, (char *)data_call_info.apn_user);
    strcpy((char *)profile->pass_word, (char *)data_call_info.apn_pwd);

    return status;
}

/*
@func
  qapi_QT_NW_PDP_Cfg_Set
@brief
  Set PDP parameter.
*/
qapi_Status_t qapi_QT_NW_PDP_Cfg_Set(uint8_t *pdp_context_number, qapi_QT_NW_DS_Profile_PDP_Context_t *profile)
{
    //Please use qapi_DSS_Set_Data_Call_Param to set pdp cfg.
    return QAPI_ERR_NOT_SUPPORTED;
}

qapi_Status_t qapi_DSS_Get_IP_Addr_Count(qapi_DSS_Hndl_t hndl, unsigned int *ip_addr_cnt)
{
    qapi_DSS_Hndl_s *qapi_DSS_Hndl = hndl;
    if(ip_addr_cnt == NULL || qapi_DSS_Hndl == NULL)
    {
        return QAPI_ERROR;
    }

    if(ql_datacall_get_sim_profile_is_active(QL_SIM_ID_0, qapi_DSS_Hndl->Profile_PDP_number) == false){
        *ip_addr_cnt = 0;
        return QAPI_OK;
    }

    ql_data_call_info_s data_call_info = {0};
    if(ql_get_data_call_info(QL_SIM_ID_0, qapi_DSS_Hndl->Profile_PDP_number, &data_call_info) != 0){
        return QAPI_ERROR;
    }

    if(data_call_info.ip_version == QL_PDP_TYPE_IPV4V6){
        *ip_addr_cnt = 2;
    }else{
        *ip_addr_cnt = 1;
    }
    
    return QAPI_OK;
}

qapi_Status_t qapi_DSS_Get_IP_Addr(qapi_DSS_Hndl_t hndl, qapi_DSS_Addr_Info_t  *info_ptr, int len)
{
    qapi_DSS_Hndl_s *qapi_DSS_Hndl = hndl;
    qapi_DSS_Addr_Info_t *addr_info_ptr = NULL; 
    if(info_ptr == NULL || qapi_DSS_Hndl == NULL || len == 0 || len > 2)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    ql_data_call_info_s data_call_info = {0};
    if(ql_get_data_call_info(QL_SIM_ID_0, qapi_DSS_Hndl->Profile_PDP_number, &data_call_info) != 0){
        return QAPI_ERROR;
    }

    addr_info_ptr = info_ptr;
    if( data_call_info.v4.state == true && addr_info_ptr != NULL && 
       (data_call_info.ip_version == QL_PDP_TYPE_IP || data_call_info.ip_version == QL_PDP_TYPE_IPV4V6))
    {
        addr_info_ptr->iface_addr_s.valid_addr = data_call_info.v4.state;
        memset(addr_info_ptr->iface_addr_s.addr.v6_addr32, 0, 16);

        addr_info_ptr->iface_addr_s.addr.v4 = ip4_addr_get_u32(&data_call_info.v4.addr.ip);
        addr_info_ptr->iface_mask = 0x00FFFFFF;

        addr_info_ptr->dnsp_addr_s.addr.v4 = ip4_addr_get_u32(&data_call_info.v4.addr.pri_dns);
        addr_info_ptr->dnss_addr_s.addr.v4 = ip4_addr_get_u32(&data_call_info.v4.addr.sec_dns);

        addr_info_ptr->gtwy_addr_s.addr.v4 = 0;
        addr_info_ptr->gtwy_mask = 0;

        if(data_call_info.ip_version == QL_PDP_TYPE_IPV4V6)
        {
            addr_info_ptr++;
        }
    }

    if( data_call_info.v6.state == true && addr_info_ptr != NULL && 
       (data_call_info.ip_version == QL_PDP_TYPE_IPV6 || data_call_info.ip_version == QL_PDP_TYPE_IPV4V6))
    {
        addr_info_ptr->iface_addr_s.addr.v4 = 0;
        addr_info_ptr->iface_addr_s.valid_addr = data_call_info.v6.state;

        memcpy(addr_info_ptr->iface_addr_s.addr.v6_addr32, data_call_info.v6.addr.ip.addr, 16);

        memcpy(addr_info_ptr->dnsp_addr_s.addr.v6_addr32, data_call_info.v6.addr.pri_dns.addr, 16);
        memcpy(addr_info_ptr->dnss_addr_s.addr.v6_addr32, data_call_info.v6.addr.sec_dns.addr, 16);
    }

    return QAPI_OK;
}

qapi_Status_t qapi_DSS_Init(int mode)
{
    return QAPI_OK;
}

qapi_Status_t qapi_DSS_Get_Data_Srvc_Hndl(qapi_DSS_Net_Ev_CB_t user_cb_fn, void *user_data, qapi_DSS_Hndl_t *hndl)
{
    if(hndl == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    qapi_DSS_Hndl_s *qapi_DSS_Hndl = calloc(1, sizeof(qapi_DSS_Hndl_s));
    QL_CNKTD_NW_LOG("calloc qapi_DSS_Hndl_t %p", qapi_DSS_Hndl);
    *hndl = qapi_DSS_Hndl;
    if(hndl == NULL){
        return QAPI_ERR_NO_MEMORY;
    }
    return QAPI_OK;
}

qapi_Status_t qapi_DSS_Set_Data_Call_Param(qapi_DSS_Hndl_t hndl, qapi_DSS_Call_Param_Identifier_t identifier, qapi_DSS_Call_Param_Value_t *info)
{
    qapi_Status_t status = QAPI_OK;
    qapi_DSS_Hndl_s *qapi_DSS_Hndl = hndl;
    if(qapi_DSS_Hndl == NULL || info == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    if(info->num_val == 0)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    switch(identifier)
    {
        //QAPI_DSS_CALL_INFO_UMTS_PROFILE_IDX_E,   /**< UMTS profile ID. */
        case QAPI_DSS_CALL_INFO_APN_NAME_E:  /**< APN name. */
        {
            if(info->num_val > QT_DS_PROFILE_MAX_APN_STRING_LEN){
                return QAPI_ERR_INVALID_PARAM;
            }
            strcpy((char *)qapi_DSS_Hndl->Profile_PDP_Context.apn, info->buf_val);
        }
        break;
        case QAPI_DSS_CALL_INFO_USERNAME_E:  /**< APN user name. */
        {
            if(info->num_val > QT_DS_PROFILE_MAX_USERNAME_LEN){
                return QAPI_ERR_INVALID_PARAM;
            }
            strcpy((char *)qapi_DSS_Hndl->Profile_PDP_Context.user_name, info->buf_val);
        }
        break;
        case QAPI_DSS_CALL_INFO_PASSWORD_E: /**< APN password. */
        {
            if(info->num_val > QT_DS_PROFILE_MAX_PASSWORD_LEN){
                return QAPI_ERR_INVALID_PARAM;
            }
            strcpy((char *)qapi_DSS_Hndl->Profile_PDP_Context.pass_word, info->buf_val);
        }
        break;
        case QAPI_DSS_CALL_INFO_AUTH_PREF_E:/**< Authentication preference. */
        {
            if(info->num_val >= QT_NW_DS_PROFILE_AUTH_TYPE_MAX){
                return QAPI_ERR_INVALID_PARAM;
            }
            qapi_DSS_Hndl->Profile_PDP_Context.auth_type = info->num_val;
        }
        break;
        case QAPI_DSS_CALL_INFO_IP_VERSION_E:/**< Preferred IP family for the call. */
        {
            if(info->num_val >= QT_NW_DS_PROFILE_PDP_MAX){
                return QAPI_ERR_INVALID_PARAM;
            }
            qapi_DSS_Hndl->Profile_PDP_Context.pdp_type = info->num_val;
        }
        break;
        case QAPI_DSS_CALL_INFO_LTE_PROFILE_IDX_E:
        {
            if(info->num_val > PROFILE_IDX_MAX || info->num_val < PROFILE_IDX_MIN){
                return QAPI_ERR_INVALID_PARAM;
            }
            qapi_DSS_Hndl->Profile_PDP_number = info->num_val;
        }
        break;
        default:
        return QAPI_ERR_INVALID_PARAM;
    }

    return status;
}

qapi_Status_t qapi_DSS_Start_Data_Call(qapi_DSS_Hndl_t hndl)
{
    qapi_DSS_Hndl_s *qapi_DSS_Hndl = hndl;
    if(qapi_DSS_Hndl == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    QL_CNKTD_NW_LOG("qapi_DSS_Hndl=%p, Profile_PDP_number %d",qapi_DSS_Hndl, qapi_DSS_Hndl->Profile_PDP_number);
    
    int ret = ql_start_data_call(QL_SIM_ID_0, 
                        qapi_DSS_Hndl->Profile_PDP_number, 
                        qapi_DSS_Hndl->Profile_PDP_Context.pdp_type+1, //Convert qapi_QT_NW_DS_PROFILE_PDP_TYPE_e to QL_PDP_TYPE_XX
                        (char *)qapi_DSS_Hndl->Profile_PDP_Context.apn,
                        (char *)qapi_DSS_Hndl->Profile_PDP_Context.user_name,
                        (char *)qapi_DSS_Hndl->Profile_PDP_Context.pass_word,
                        qapi_DSS_Hndl->Profile_PDP_Context.auth_type);

    if(ret != QL_DATACALL_SUCCESS && ret != QL_DATACALL_REPEAT_ACTIVE_ERR)
    {
        return QAPI_ERROR;
    }
    return QAPI_OK;
}

qapi_Status_t qapi_DSS_Stop_Data_Call(qapi_DSS_Hndl_t hndl)
{
    qapi_DSS_Hndl_s *qapi_DSS_Hndl = hndl;
    if(qapi_DSS_Hndl == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }

    if(ql_stop_data_call(QL_SIM_ID_0, qapi_DSS_Hndl->Profile_PDP_number) != 0)
    {
        return QAPI_ERROR;
    }

    return QAPI_OK;
}

qapi_Status_t qapi_DSS_Rel_Data_Srvc_Hndl(qapi_DSS_Hndl_t hndl)
{
    if(hndl != NULL)
    {
        QL_CNKTD_NW_LOG("free qapi_DSS_Hndl_t %p", hndl);
        free(hndl);
    }
    return QAPI_OK;
}

qapi_Status_t qapi_DSS_Release(int mode)
{
    return QAPI_OK;
}

/*
@func
  qapi_QT_IMEI_Get
@brief
  Get module IMEI number.
@param [in]
  imei - pointer, to store the IMEI number.
*/
qapi_Status_t qapi_QT_IMEI_Get(char* imei, uint16 *len)
{
    if (ql_dev_get_imei(imei, *len, QL_SIM_ID_0) != 0){
        return QAPI_ERROR;
    }

    return QAPI_OK;
}


