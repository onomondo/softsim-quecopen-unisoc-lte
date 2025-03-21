/**  @file
  quec_virt_func_modem.c

  @brief
  This file is used to run virt function.

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
#include "at_response.h"
#include "at_engine.h"
#include "quec_cust_feature.h"
#include "quec_common.h"
#include "at_command.h"
#include "at_param.h"
#include "ql_api_osi.h"
#include "quec_at_engine.h"
#include "quec_log.h"
//#include "quec_rtos.h"
#include "ql_api_osi.h"
#include "ql_api_usbnet.h"
#include "ql_api_stk.h"
#include "ql_api_sms.h"
#include "ql_api_vsim_adapt.h"

#define QUEC_VIRT_MODEM_FUNC_LOG(msg, ...)  custom_log("VIRT_MFUNC", msg, ##__VA_ARGS__)

//l2tp
#ifndef CONFIG_QUEC_PROJECT_FEATURE_L2TP
__attribute__((weak)) bool quec_l2tp_need_repacket(uint8_t nSim, uint8_t cid, void *data)
{
    return false;
}
__attribute__((weak)) int quec_l2tp_output_repacket(void *data)
{
    return 0;
}
#if !PPP_AUTH_SUPPORT
__attribute__((weak)) void ppp_set_auth(void *pcb, uint8_t authtype, const char *user, const char *passwd)
{
    return;
}

__attribute__((weak)) void ppp_set_auth_required(void *pcb, uint8_t value)
{
    return;
}
#endif
#endif
//usbnet
#ifndef CONFIG_QUEC_PROJECT_FEATURE_USBNET
__attribute__((weak)) bool quec_usbnet_sendevent(uint32_t nMsg, uint32_t nParam1, uint8_t nParam2, uint8_t nParam3, uint16_t nParam4)
{
	return false;
}

__attribute__((weak)) ql_task_t *quec_get_usbnet_threadID(void)
{
	return NULL;
}

__attribute__((weak)) ql_usbnet_errcode_e ql_usbnet_get_type(ql_usbnet_type_e *usbnet_type)
{
	return QL_USBNET_EXECUTE_ERR;
}

__attribute__((weak)) ql_usbnet_errcode_e ql_usbnet_set_type(ql_usbnet_type_e usbnet_type)
{
	return QL_USBNET_EXECUTE_ERR;
}

__attribute__((weak)) void quec_exec_qnetdevctl_cmd(atCommand_t *cmd)
{
	return;
}

__attribute__((weak)) void ql_usbnet_set_status(ql_usbnet_state_e status)
{
	return;
}

__attribute__((weak)) bool quec_usbnet_send_osi_event_set_if(quec_netif *net_if, quec_netif *wan_if)
{
    return false;
}

__attribute__((weak)) ql_usbnet_datalink_callback quec_usbnet_uplink_data = NULL;

__attribute__((weak)) quec_netif *quec_usbnet_wan_netif_get(void)
{
    return NULL;
}
#endif/*CONFIG_QUEC_PROJECT_FEATURE_USBNET*/


//pbk
#ifndef CONFIG_QUEC_PROJECT_FEATURE_PBK
__attribute__((weak)) int quec_pbk_process_output_name(uint8_t *dest, uint8_t destlen, uint8_t cscs_charset, uint8_t *source, uint8_t sourcelen)
{
    return 0;
}
__attribute__((weak)) void quec_pbk_init(uint8_t nSim)
{
    return;
};
__attribute__((weak)) int quec_pbk_add_lastdialling_phone(uint8_t nSim, const char *phone_num, uint16_t len)
{
    return 0;
};
#endif/*CONFIG_QUEC_PROJECT_FEATURE_PBK*/

//stk
#ifndef CONFIG_QUEC_PROJECT_FEATURE_STK
__attribute__((weak)) void quec_stk_sim_lost(uint8_t nSim)
{
    return;
}

__attribute__((weak)) void quec_stk_replace_profile(void)
{
    return;
}

__attribute__((weak)) bool quec_stk_is_cmd_in_register_table(uint8_t cmd)
{
    return false;
}

__attribute__((weak)) void quec_stk_no_proactive(uint8_t nSim)
{
    return;
}

__attribute__((weak)) bool quec_stk_sat_plugin_take_over(const osiEvent_t *event)
{
    return false;
}
#endif/*CONFIG_QUEC_PROJECT_FEATURE_STK*/

#ifndef CONFIG_QUEC_PROJECT_FEATURE_STK_AT
__attribute__((weak)) void quec_exec_qstk_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_INVALID_COMMAND_LINE, NULL, 1);
    return;
}

__attribute__((weak)) void quec_exec_qstkpd_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_INVALID_COMMAND_LINE, NULL, 1);
    return;
}

__attribute__((weak)) void quec_exec_qstkstate_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_INVALID_COMMAND_LINE, NULL, 1);
    return;
}

__attribute__((weak)) void quec_exec_qstkgi_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_INVALID_COMMAND_LINE, NULL, 1);
    return;
}

__attribute__((weak)) void quec_exec_qstkrsp_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_INVALID_COMMAND_LINE, NULL, 1);
    return;
}
#endif /* CONFIG_QUEC_PROJECT_FEATURE_STK_AT */

#ifndef CONFIG_QUEC_PROJECT_FEATURE_BIP
__attribute__((weak)) ql_stk_errcode_e ql_stk_get_bip_config(ql_stk_bip_config_s *config)
{
    if (NULL == config)
    {
        return QL_STK_INVALID_PARAM_ERR;
    }

    config->mode = QL_STK_BIP_CONFIG_MODE_DISABLE;
    config->cid = 1;
    config->status_report = false;
    return QL_STK_SUCCESS;
}
#endif /*CONFIG_QUEC_PROJECT_FEATURE_BIP*/

//vsim_adapt
#ifndef CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT
bool set_vsim_in_poweron;
ql_vsim_adapt_callback ql_vsim_adapt_cb = NULL;
#endif

//volte
#ifndef CONFIG_QUEC_PROJECT_FEATURE_VOLTE
__attribute__((weak)) void quec_volte_init(void)
{
    return;
};

__attribute__((weak)) int ql_volte_user_agent_init(void)
{
    return 0;
};

__attribute__((weak)) bool quec_ImsIsSet(uint8_t nSim)
{
    return false;
};

__attribute__((weak)) int ql_volte_set(uint8_t nSim, uint8_t mode)
{
    return false;
}
__attribute__((weak)) int ql_ims_diable_plmn_handle(uint8_t nSim, void *plmn)
{
    return -1;
}

__attribute__((weak)) int ql_ims_isim_set(void *isim_cfg)
{
    return -1;
}

__attribute__((weak)) int ql_ims_isim_get(void *isim_cfg)
{
    return -1;
}
#endif/*CONFIG_QUEC_PROJECT_FEATURE_VOLTE*/

//sms
#ifndef CONFIG_QUEC_PROJECT_FEATURE_SMS
__attribute__((weak)) int quec_sms_init_me_size(void)
{
  return 0;
}

__attribute__((weak)) int ql_sms_get_me_erase_flag(bool *meEraseFlag)
{
  return 0;
}

__attribute__((weak)) bool quec_trust_num_check(uint8_t nSim, const char *phone_num, bool is_call)
{
    return true;
}

__attribute__((weak)) ql_sms_errcode_e quec_get_concat_sms_cfg(void *cfg)
{
    return QL_SMS_ERROR;
}

__attribute__((weak)) ql_sms_errcode_e quec_set_concat_sms_cfg(void *cfg)
{
    return QL_SMS_ERROR;
}

__attribute__((weak)) bool quec_is_concat_sms(void *pConcatInfo, void *sms_data)
{
    return false;
}

__attribute__((weak)) void quec_concat_sms_update(void *sms_data)
{
    return ;
}

__attribute__((weak)) ql_sms_errcode_e ql_sms_get_storage_info(uint8_t nSim, ql_sms_stor_info_s *stor_info)
{
    return QL_SMS_ERROR;
}
#endif


#ifndef CONFIG_QUEC_PROJECT_FEATURE_PPP
__attribute__((weak)) bool g_pppTermframe = false;

__attribute__((weak)) void *pppSessionCreate(uint8_t cid, uint8_t sim, void *dl_thread,
                               void * output_cb, void *output_cb_ctx,
                               void * end_cb, void *end_cb_ctx,
                               void * flowctrl_cb, void *flowctrl_cb_ctx,
                               int _iActiavedbyPPP)
{
    return NULL;
}
__attribute__((weak)) bool pppSessionDelete(void *ppp)
{
    return NULL;
}
__attribute__((weak)) bool pppSessionClose(void *ppp, uint8_t termflag)
{
    return NULL;
}
__attribute__((weak)) int pppSessionSuspend(void *ppp)
{
    return 0;
}
__attribute__((weak)) int pppSessionResume(void *ppp)
{
    return 0;
}
__attribute__((weak)) int pppSessionInput(void *ppp, const void *data, size_t size)
{
    return -1;
}
__attribute__((weak)) void pppSessionDeleteByNetifDestoryed(uint8_t nSimId, uint8_t nCid)
{
    return;
}
__attribute__((weak)) void pppSessionDeleteBySimCid(uint8_t nSimId, uint8_t nCid, uint8_t forceClose)
{
    return;
}
__attribute__((weak)) void ppp_StartGprs(uint8_t *usrname, uint8_t *usrpwd, void *ctx)
{
    
}
__attribute__((weak)) void ppp_ip_output(void *p, void *ctx)
{
    return;
}
__attribute__((weak)) void quec_ppp_event_proc(const osiEvent_t *event)
{
	QUEC_VIRT_MODEM_FUNC_LOG("ppp not support");
}
__attribute__((weak)) void quec_ppp_mode_switch_handler(void *th, uint8 cause)
{

}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_XLAT
__attribute__((weak)) int ql_xlat_set_src_prefix_len(struct netif *inp, ip6_addr_t *prefix_addr, int len, int lwip_modem)
{
	return -1;
}
__attribute__((weak)) int ql_xlat_set_ipv6_mtu(struct netif *inp)
{
	return -1;
}
#endif


