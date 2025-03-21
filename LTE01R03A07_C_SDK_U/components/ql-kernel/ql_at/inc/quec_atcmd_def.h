/**  @file
  quec_atcmd_def.h

  @brief
  This file is used to define at command for different Quectel Project.

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
#ifndef QUEC_ATCMD_DEF_H
#define QUEC_ATCMD_DEF_H

#include "quec_cust_feature.h"

#ifdef CONFIG_QUEC_PROJECT_FEATURE_QUECOS
#include "quecos_atcmd_cfg.h"
#endif

#ifdef CONFIG_QUEC_BUILD_IN_CSDK
//用户自定义AT列表
#include "ql_atcmd_def.h"
#endif

/************************ Basic AT ***************************/
//以下AT请勿裁剪,需要用于调试
+QPOWD, 	quec_exec_qpowd_cmd , 				0
+QRESET,	quec_exec_qreset_cmd,				0
+QDBGCFG,	quec_exec_qdbgcfg_cmd,				0
+QDOWNLOAD, atCmdHandleFORCEDNLD,				0
+CVERSION,	quec_exec_cversion_cmd, 			0
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NW_AT
+QNWINFO,   quec_exec_qnwinfo_cmd,              0
+QENG,      quec_exec_qeng_cmd,                 0
+QCSQ,		quec_exec_qcsq_cmd,  				0
#ifdef __QUEC_OEM_VER_LT__
+QLTCSQ,    quec_exec_qltcsq_cmd,  				0
#endif
#endif

//以下AT可以根据需要来裁剪,打开CONFIG_QUEC_PROJECT_AT_CUT后,
//下方所有AT都会被裁剪,用户也可以根据需要自行保留需要的AT
#ifndef CONFIG_QUEC_PROJECT_AT_CUT

#ifdef CONFIG_QUEC_PROJECT_FEATURE_TCPIP_AT
/************************ping at cmd ***************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_PING_AT
+QPING,      at_ping_qping_cmd_func,           0
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_XLAT_AT
+QPRFMOD,      at_xlat_qprfmod_cmd_func,       0
#endif
//********************** TCPIP/udp at cmd ***********************/
+QICSGP,     at_tcpip_qicsgp_cmd_func,         0
+QIACT,      at_tcpip_qiact_cmd_func,          0
+QIACTEX,    at_tcpip_qiactex_cmd_func,        0
+QIDEACT,    at_tcpip_qideact_cmd_func,        0
+QIDEACTEX,  at_tcpip_qideactex_cmd_func,      0
+QIOPEN,     at_tcpip_qiopen_cmd_func,         0
+QISEND,     at_tcpip_qisend_cmd_func,         0
+QISENDEX,   at_tcpip_qisendex_cmd_func,       0
+QISENDHEX,	 at_tcpip_qisendhex_cmd_func,	   0
+QISDE,      at_tcpip_qisde_cmd_func,          0
+QIRD,       at_tcpip_qird_cmd_func,           0
+QIACCEPT,   at_tcpip_qiaccept_cmd_func,       0
+QICLOSE,    at_tcpip_qiclose_cmd_func,        0
+QISTATE,    at_tcpip_qistate_cmd_func,        0
+QISWTMD,    at_tcpip_qiswtmd_cmd_func,        0
+QICFG,      at_tcpip_qicfg_cmd_func,          0
+QIGETERROR, at_tcpip_qigeterror_cmd_func,     0
+QIDNSCFG,   at_tcpip_qidnscfg_cmd_func,       0
+QIDNSGIP,   at_tcpip_qidnsgip_cmd_func,       0

#if defined(__QUEC_OEM_VER_CAP__)
+QILISTEN,   at_tcpip_qilisten_cmd_func,       0
#endif

/********************** ctwing ***********************/
#if defined(CONFIG_QUEC_PROJECT_FEATURE_CTWING_AT) && defined(__QUEC_OEM_VER_OPC__)
+QICTWCFG,   at_tcpip_qictwcfg_cmd_func,       0
#endif


/************************ntp at cmd ***************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NTP_AT
+QNTP, 	     at_ntp_qntp_cmd_func,		       0
#endif


//********************** ssl at cmd ***********************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SSL
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SSL_AT
+QSSLCFG,	 at_ssl_qsslcfg_cmd_func,		   0
+QSSLOPEN,	 at_ssl_qsslopen_cmd_func,		   0
+QSSLSTATE,  at_ssl_qsslstate_cmd_func, 	   0
+QSSLSEND,	 at_ssl_qsslsend_cmd_func,		   0
+QSSLRECV,	 at_ssl_qsslrecv_cmd_func,		   0
+QSSLCLOSE,  at_ssl_qsslclose_cmd_func, 	   0
+QSSLOPENEX, at_ssl_qsslopenex_cmd_func,	   0
#endif /*CONFIG_QUEC_PROJECT_FEATURE_SSL_AT*/
#endif
#if defined(__QUEC_OEM_VER_WB__) /*Project macro compilation, XXCD exit*/
+TCPINIT,	 at_xxcd_tcpip_tcpinit_cmd_func,        0
+GETIP,    	 at_xxcd_tcpip_getip_cmd_func,          0
+SETIP,      at_xxcd_tcpip_setip_cmd_func,          0
+TCPSEND,  	 at_xxcd_tcpip_tcpsend_cmd_func,        0
+TCPCLOSE, 	 at_xxcd_tcpip_tcpclose_cmd_func,		0
#endif /*endof __QUEC_OEM_VER_WB__*/

#endif


//********************** http at cmd ***********************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_HTTP
#ifdef CONFIG_QUEC_PROJECT_FEATURE_HTTP_AT
+QHTTPURL,       at_http_qhttpurl_cmd_func,        0
+QHTTPCFG,       at_http_qhttpcfg_cmd_func,        0
+QHTTPGET,       at_http_qhttpget_cmd_func,        0
+QHTTPGETEX,	 at_http_qhttpgetex_cmd_func, 	   0
+QHTTPPOST,      at_http_qhttppost_cmd_func,       0
+QHTTPREAD,      at_http_qhttpread_cmd_func,       0
+QHTTPPUT,       at_http_qhttpput_cmd_func,        0
+QHTTPPATCH,     at_http_qhttppatch_cmd_func, 	   0
+QHTTPPOSTFILE,  at_http_qhttppostf_cmd_func,	   0
+QHTTPREADFILE,  at_http_qhttpreadf_cmd_func,	   0
+QHTTPSTOP,      at_http_qhttpstop_cmd_func,       0
+QHTTPPUTFILE,	 at_http_qhttppostf_cmd_func, 	   0
+QHTTPPATCHFILE, at_http_qhttppostf_cmd_func,	   0
+QHTTPCFGEX,	 at_http_qhttpcfgex_cmd_func, 	   0
+QHTTPSEND,	     at_http_qhttpsend_cmd_func, 	   0

#endif /*CONFIG_QUEC_PROJECT_FEATURE_HTTP_AT*/
#if defined(CONFIG_QUEC_PROJECT_FEATURE_CTWING_AT) && defined(__QUEC_OEM_VER_OPC__)
+QHTTPCTWING,	 at_http_qhttpctwing_cmd_func, 	   0
+QHTTPCTWURL,	 at_http_qhttpctwurl_cmd_func, 	   0
+QHTTPCTWPOST,	 at_http_qhttpctwpost_cmd_func,    0
#endif
#endif

#if defined(__QUEC_OEM_VER_OPC__) || defined(__QUEC_OEM_VER_BUX__)
#ifdef CONFIG_QUEC_PROJECT_FEATURE_DMHTTP
#ifdef CONFIG_QUEC_PROJECT_FEATURE_DMHTTP_AT
+QDMHTTPCFG,     at_http_qdmhttpcfg_cmd_func,           0
+QDMHTTPINFO,    at_http_qdmhttpinfo_cmd_func,          0
+QDMHTTPREG,     at_http_qdmhttpreg_cmd_func,           0
#endif
#endif
#endif

//********************** mqtt at cmd ***********************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MQTT
#ifdef __QUEC_OEM_VER_CUC__
//联通AT模组指令
+UNIKEYINFOM,			at_mqtt_uniyfkeyinfom_cmd_func,		0
+UNIDELKEYINFOM,		at_mqtt_unidelfkeyinfom_cmd_func,	0
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_UNIOT_AT
//联通AT终端指令
+UNIKEYINFO, 			at_mqtt_uniyfkeyinfo_cmd_func,	   	0
//+UNIKEYINFOM, 			at_mqtt_uniyfkeyinfo_cmd_func,	   	0
//+UNIDELKEYINFOM			at_mqtt_unidelfkeyinfo_cmd_func,	0
+UNIDELKEYINFO,			at_mqtt_unidelfkeyinfo_cmd_func,	0
//+UNICERTINFO,			at_mqtt_unicertinfo_cmd_func,		0
+UNIMQTTCON,	  		at_mqtt_unimqttconn_cmd_func,		0
+UNIMQTTDISCON,			at_mqtt_unimqttdiscon_cmd_func,		0
+UNIMQTTSTATE,			at_mqtt_unimqttstate_cmd_func,		0
+UNIMQTTSUB,			at_mqtt_unimqttsub_cmd_func,		0
+UNIMQTTPUB,			at_mqtt_unimqttpub_cmd_func,		0

+UNIMQTTBIND,			at_mqtt_unimqttbind_cmd_func,		0	
+UNIMQTTBINDSUB,		at_mqtt_unimqttbindsub_cmd_func,	0	
+UNIMQTTSHADOWSUB,		at_mqtt_unimqttshadowsub_cmd_func,	0	
+UNIMQTTSHADOWGET, 		at_mqtt_unimqttshadowget_cmd_func,	0
+UNIMQTTSHADOWPUB,		at_mqtt_unimqttshadowpub_cmd_func,	0
+UNIPSMSET,				at_mqtt_unipsmset_cmd_func,			0
#endif /* CONFIG_QUEC_PROJECT_FEATURE_UNIOT_AT */

#ifdef CONFIG_QUEC_PROJECT_FEATURE_MQTT_AT
+QMTCFG,   at_mqtt_qmtcfg_cmd_func,            0
+QMTOPEN,  at_mqtt_qmtopen_cmd_func,           0
+QMTCONN,  at_mqtt_qmtconn_cmd_func,           0
+QMTSUB,   at_mqtt_qmtsub_cmd_func,            0
+QMTUNS,   at_mqtt_qmtunsub_cmd_func,          0
+QMTPUB,   at_mqtt_qmtpub_cmd_func,            0
+QMTPUBEX, at_mqtt_qmtpub_cmd_func, 		   0
+QMTDISC,  at_mqtt_qmtdisc_cmd_func,           0
+QMTRECV,  at_mqtt_qmtrecv_cmd_func,           0
+QMTCLOSE, at_mqtt_qmtclose_cmd_func,          0
+QMTUNIOTREG, at_mqtt_qmtuniotreg_cmd_func,    0
#endif /*CONFIG_QUEC_PROJECT_FEATURE_MQTT_AT*/
#endif


//********************** mms at cmd ***********************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MMS
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MMS_AT
+QMMSCFG,  at_mms_qmmscfg_cmd_func,            0
+QMMSEDIT, at_mms_qmmsedit_cmd_func,           0
+QMMSEND,  at_mms_qmmsend_cmd_func,            0
#endif /*CONFIG_QUEC_PROJECT_FEATURE_MMS_AT*/
#endif


//********************** smtp at cmd ***********************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SMTP_AT
+QSMTPCFG,	at_smtp_qsmtpcfg_cmd_func,			0
+QSMTPDST,	at_smtp_qsmtpdst_cmd_func,			0
+QSMTPSUB,  at_smtp_qsmtpsub_cmd_func,			0
+QSMTPBODY,	at_smtp_qsmtpbody_cmd_func,			0
+QSMTPATT,	at_smtp_qsmtpatt_cmd_func,			0
+QSMTPCLR,	at_smtp_qsmtpclr_cmd_func,			0
+QSMTPPUT,	at_smtp_qsmtpput_cmd_func,			0
#endif /*CONFIG_QUEC_PROJECT_FEATURE_SMTP_AT*/


//********************** queclocator at cmd ***********************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LBS
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LBS_AT
+QLBSCFG,  at_lbs_qlbscfg_cmd_func,            0
+QLBS,     at_lbs_qlbs_cmd_func,               0
+QLBSEX,   at_lbs_qlbs_ext_input_cmd_func,     0
#endif /*CONFIG_QUEC_PROJECT_FEATURE_LBS_AT*/
#endif


//********************** ctsreg at cmd ***********************/
#ifdef __QUEC_OEM_VER_OPC__
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CTSREG_AT
+QIOTRPTCFG,  at_ctsreg_qiotrptcfg_cmd_func,            0
+QIOTRPT,     at_ctsreg_qiotrpt_cmd_func,               0
#endif
#endif

//********************** ftp at cmd ***********************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FTP
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FTP_AT
+QFTPCFG,		at_ftp_qftpcfg_cmd_func,			0
+QFTPOPEN,		at_ftp_qftpopen_cmd_func,			0
+QFTPCLOSE,		at_ftp_qftpclose_cmd_func,			0
+QFTPPUT,		at_ftp_qftpput_cmd_func,			0
+QFTPPUTEX,     at_ftp_qftpputex_cmd_func,          0
+QFTPGET,		at_ftp_qftpget_cmd_func,			0
+QFTPSIZE,		at_ftp_qftpsize_cmd_func,			0
+QFTPSTAT,		at_ftp_qftpstat_cmd_func,			0
+QFTPLEN,		at_ftp_qftplen_cmd_func,			0
+QFTPRENAME,	at_ftp_qftprename_cmd_func,			0
+QFTPDEL,   	at_ftp_qftpdel_cmd_func,			0
+QFTPMKDIR,		at_ftp_qftpmkdir_cmd_func,			0
+QFTPRMDIR,		at_ftp_qftprmdir_cmd_func,			0
+QFTPLIST,		at_ftp_qftplist_cmd_func,			0
+QFTPNLST,		at_ftp_qftpnlst_cmd_func,			0
+QFTPCWD,		at_ftp_qftpcwd_cmd_func,			0
+QFTPPWD,		at_ftp_qftppwd_cmd_func,			0
+QFTPMDTM,		at_ftp_qftpmdtm_cmd_func,			0
+QFTPMLSD,		at_ftp_qftpmlsd_cmd_func,			0
+QFTPDBG,       at_ftp_qftpdbg_cmd_func,            0
#endif /*CONFIG_QUEC_PROJECT_FEATURE_FTP_AT*/
#if defined(__QUEC_OEM_VER_WB__) /*Project macro compilation, XXCD exit*/
+FTPINIT,    at_xxcd_tcpip_tcpinit_cmd_func,        0
+FTPOPEN,    at_xxcd_ftp_ftpopen_cmd_func,          0
+FTPSIZE,     at_xxcd_ftp_ftpsize_cmd_func,         0
+FTPPUT,     at_xxcd_ftp_ftpput_cmd_func,           0
+FTPGET,	  at_xxcd_ftp_ftpget_cmd_func,		    0
+FTPCLOSE,	  at_xxcd_ftp_ftpclose_cmd_func,		0
#endif /*endof __QUEC_OEM_VER_WB__*/
#endif

//********************** snmp at cmd ***********************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SNMP
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SNMP_AT
+QSNMPCFG,		at_snmp_qsnmpcfg_cmd_func,			0
+QSNMPOPEN,		at_snmp_qsnmpopen_cmd_func,			0
+QSNMPCLOSE,    at_snmp_qsnmpclose_cmd_func,		0
#endif
#endif

//********************** heart beat at cmd ***********************/
#if defined(CONFIG_QUEC_PROJECT_FEATURE_HEART_BEAT_AT) && defined(__QUEC_OEM_VER_HW__)
+QHBCFG,         at_exec_qhbcfg_cmd_func,         0
+QHBSTART,       at_exec_qhbstart_cmd_func,       0
+QHBSTOP,        at_exec_qhbstop_cmd_func,        0
#endif

#ifdef __QUEC_OEM_VER_ALIPAY__
#ifdef CONFIG_QUEC_PROJECT_FEATURE_ALIPAY_IOT_SDK_AT
+ALIPAYCFG,		quec_alipay_cfg_cmd_func, 	   		0
+ALIPAYOPEN,	quec_alipay_open_cmd_func,	   		0
+ALIPAYSHUT,	quec_alipay_shut_cmd_func, 			0
+ALIPAYDID,	 	quec_alipay_did_cmd_func, 	   		0
+ALIPAYSIGN,	quec_alipay_sign_cmd_func, 	   		0
+ALIPAYREP,	 	quec_alipay_rep_cmd_func, 	   		0
+ALIPAYPINFO,	quec_alipay_info_cmd_func, 	   		0
+ALIPAYACT,	 	quec_alipay_act_cmd_func, 	   		0
+ALIPAYUTC,	 	quec_alipay_utc_cmd_func, 	   		0
+ALIPAYVER,	 	quec_alipay_version_cmd_func, 	   	0
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_ALIIOTSMARTCARD_AT
+MNENABLE,		quec_aliot_smartcard_mnenable_cmd_func, 	   		0
+MNCARDLIST,	quec_aliot_smartcard_mncardlist_cmd_func,			0
+MNCARDSEL,		quec_aliot_smartcard_mncardsel_cmd_func,			0
+MNPOLICY,		quec_aliot_smartcard_mnpolicy_cmd_func,				0
+MNSCAN,		quec_aliot_smartcard_mnscan_cmd_func,				0
+MNSAVE,		quec_aliot_smartcard_mnsave_cmd_func,				0
+QMNCFG,		quec_aliot_smartcard_qmncfg_cmd_func,				0
+QMNSTARTUP,	quec_aliot_smartcard_qmnstartup_cmd_func,			0
#endif
#endif

/************************websocket at cmd ***************************/
#ifdef __QUEC_OEM_VER_WWS__
#ifdef CONFIG_QUEC_PROJECT_FEATURE_WEBSOCKET_AT
+QWSCFG,		at_ws_exec_qwscfg_cmd,			   					0
+QWSOPEN,		at_ws_exec_qwsopen_cmd, 		   					0
+QWSWRITE,		at_ws_exec_qwswrite_cmd, 		   					0
+QWSREAD,		at_ws_exec_qwsread_cmd, 		  	 				0
+QWSCLOSE,		at_ws_exec_qwsclose_cmd, 		   					0
+QWSTATE,		at_ws_exec_qwstate_cmd,								0
#endif
#endif /*__QUEC_OEM_VER_WWS*/
//************************ mayilian maas at cmd *************************/
#ifdef __QUEC_OEM_VER_ALILINK__
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MAYILIAN
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MAYILIAN_AT
+MAASCONNSTATE,      quec_exec_maas_connstate_cmd,        0
+MAASREGSTATUS,      quec_exec_maas_regstatus_cmd,        0
+MAASDEVREG,         quec_exec_maas_devreg_cmd,           0
+MAASPUB,            quec_exec_maas_pub_cmd,              0
+MAASSDKVERSION,     quec_exec_maas_dkversion_cmd,        0
+MAASCONFIG,         quec_exec_maas_config_cmd,           0
+MAASNETCTRL,        quec_exec_maas_netctrl_cmd,          0
#endif
#endif
#endif

//************************ hw_iot_device_sdk_tiny at cmd *************************/
#ifdef __QUEC_OEM_VER_HWIOT__
#ifdef CONFIG_QUEC_PROJECT_FEATURE_HW_IOT_DEVICE_SDK
#ifdef CONFIG_QUEC_PROJECT_FEATURE_HW_IOT_DEVICE_SDK_AT
+HQPDP,      at_hwiot_hqpdp_cmd_func,        0
+HMVER,      at_hwiot_hmver_cmd_func,        0
+HMCON,      at_hwiot_hmcon_cmd_func,        0
+HMDIS,      at_hwiot_hmdis_cmd_func,        0
+HMPUB,      at_hwiot_hmpub_cmd_func,        0
+HMSUB,      at_hwiot_hmsub_cmd_func,        0
+HMUNS,      at_hwiot_hmuns_cmd_func,        0
+HMPKS,      at_hwiot_hmpks_cmd_func,        0
#endif
#endif
#endif

//********************** common/platform at cmd ***********************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_URC
+QURCCFG,   quec_exec_qurccfg_cmd,				0
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_CMUX
+CMUX,   quec_exec_cmux_cmd,				0
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_GENERAL_AT
#ifdef CONFIG_QL_OPEN_EXPORT_PKG
+QTESTDEMO,     quec_exec_at_start_app,         0
#endif
+RESET,	    quec_exec_qreset_cmd,				0
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+QCERT,     quec_exec_qcert_cmd,                0
#endif
+QRDY,      quec_exec_qrdy_cmd,                 0
+CSUB,		quec_exec_csub_cmd,					0
+QGMR,		quec_exec_qgmr_cmd,					0
+QGSN,		quec_exec_qgsn_cmd,					0
+QINF,		quec_exec_qinf_cmd,					0
+QSVN,      at_qsvn_cmd_func,                   0
+QUESTAT,   quec_exec_questat_cmd,              0
#ifdef QUECTEL_AT_AP_MODEM_NEW_VERSION
+QAPVER,	quec_exec_qapver_cmd,				0
+QSUB,		quec_exec_qsub_cmd,					0
+QAPSUB,	quec_exec_qapsub_cmd,				0
#endif
+QCALIBINFO,atCmdHandleCALIBINFO, AT_CON_NOT_CALIB_MODE     // Check the calibration mark bit
#ifdef CONFIG_QUEC_PROJECT_FEATURE_REDDA_SECURE
+QSECCFG,   quec_exec_qseccfg_cmd,              0
+QREDDAVN,   quec_exec_qreddavn_cmd,            0
#endif

//************************ flash erase times at cmd *************************/
+QFTEST,    quec_exec_qftest_cmd,               0
+RSTSET,    quec_exec_rstset_cmd,               0
+QWSETMAC,	quec_exec_qwsetmac_cmd,				0
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+QINDCFG,   quec_exec_qindcfg_cmd,				0
#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB
+QUSBCFG,   quec_exec_qusbcfg_cmd,              0
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_UART
+CBAUD,     quec_exec_cbaud_cmd,                0
+QUARTCFG,  quec_exec_quartcfg_cmd,				0
+QIPR,      quec_exec_qipr_cmd,                 0
+QICF,      quec_exec_qicf_cmd,                 0
+QIFC,      quec_exec_qifc_cmd,                 0
#endif
#endif

#if defined(__QUEC_OEM_VER_CUC__) || defined(__QUEC_OEM_VER_OPC__)
+CLAC,          quec_exec_clac_cmd, 0          // List all command
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SLEEP
S24,        quec_exec_ats24_cmd,                0     //add by sum 2021/2/25
#ifndef __QUEC_OEM_VER_GENUS__
+QSCLK,     quec_exec_qsclk_cmd,				0
+QSCLKEX,   quec_exec_qsclkex_cmd,              0
#endif
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef __QUEC_OEM_VER_HW__
+QDORIND,   quec_exec_qdorind_cmd,              0       //Dormancy indicator
+WAKEUPTIMER,   quec_exec_qwakeuptime_cmd,      0
#endif
#endif

#endif
&C,         atCmdHandleAndC,                    0      
&V, 		atCmdHandleAndV,					0 
#ifdef CONFIG_QUEC_PROJECT_FEATURE_QCFG_AT
+QCFG,      quec_exec_qcfg_cmd,                 0
#endif
+QTEMP,		quec_exec_qtemp_cmd,				0
#endif /*CONFIG_QUEC_PROJECT_FEATURE_GENERAL_AT*/

#ifdef CONFIG_QUEC_PROJECT_FEATURE_FOTA
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FOTA_AT
#ifdef __QUEC_OEM_VER_LGR__
+QFOTADL,	quec_exec_qfotadl_lgr_cmd,			0
#else
+QFOTADL,	quec_exec_qfotadl_cmd,				0
#endif
#endif /*CONFIG_QUEC_PROJECT_FEATURE_FOTA_AT*/
#endif/*CONFIG_QUEC_PROJECT_FEATURE_FOTA*/

#ifdef CONFIG_QUEC_PROJECT_FEATURE_GPIO
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GPIO_AT
+QGPIOR,	quec_exec_qgpior_cmd,				0
+QGPIOW,	quec_exec_qgpiow_cmd,				0
+QGPIOS,	quec_exec_qgpios_cmd,               0
+QPINS,     quec_exec_qpins_cmd,                0
+QGPIOCFG,  quec_exec_qgpiocfg_cmd,             0
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+QPINEXT,   quec_exec_qpinext_cmd,              0
#ifdef __QUEC_OEM_VER_HW__
+GPIOCTRL,  quec_exec_qgpioctrl_cmd,            0
#endif
#endif
#endif
#endif 

#ifdef CONFIG_QUEC_PROJECT_FEATURE_ADC
#if defined(CONFIG_QUEC_PROJECT_FEATURE_ADC_AT) || defined(CONFIG_QUEC_PROJECT_FEATURE_GENERAL_AT)
+QADC,		quectel_exec_adc_cmd,				0
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_ADC_AT
+QADCCFG,	quec_exec_qadccfg_cmd,				0
#endif
#endif

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef __QUEC_OEM_VER_QDMHYT__
+QHYTGPIO,	    quec_exec_hrgpio_cmd,			0
#endif
#endif

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LEDCFG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LEDCFG_AT
+QLEDCFG, 	quec_exec_qledcfg_cmd, 				0
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+QPWMCFG,   quec_exec_qpwmcfg_cmd,              0
+QPWMMODE,  quec_exec_qpwmmode_cmd,             0
+QLEDEXC,   quec_exec_qledexc_cmd,              0
#endif
#ifdef QUEC_LED_TEST
+QLEDTEST,  quec_exec_qledtest_cmd,				0
#endif
+QLEDSTAT, 	quec_exec_qledstat_cmd, 			0
#ifdef QUEC_VIRTUAL_PWM
+QVPWM,     quec_exec_virtualpwm_cmd,           0
#endif
#endif
#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SDMMC
+QMKFS, 	quec_exec_sdmmc_mkfs_cmd,			0
+QSDMOUNT,	quec_exec_sdmmc_mount_cmd,			0
+QFTCMD,    quec_exec_sdmmc_qft_cmd,            0
#endif

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FS_NAND_FLASH
+QNANDMKFS, 	quec_exec_nand_mkfs_cmd,			0
+QNANDMOUNT,	quec_exec_nand_mount_cmd,			0
#endif
#endif

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FS_NOR_FLASH
+QNORMKFS, 	quec_exec_nor_mkfs_cmd,			   0
+QNORMOUNT,	quec_exec_nor_mount_cmd,		   0
#endif
#endif

#if (defined CONFIG_QUEC_PROJECT_FEATURE_AUDIO) || (defined CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO)

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_TTS_AT
+QTTS,		  quec_exec_qtts_cmd,				0
+QTTSETUP,    quec_exec_qttsetup_cmd,           0
+QWTTS,       quec_exec_qwtts_cmd,              0

#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_AUDIO_AT
+QAUDSTOP,	  quec_exec_qaudstop_cmd,			0
+QAUDPLAY,	  quec_exec_qaudplay_cmd,           0
+CLVL,		  quec_exec_clvl_cmd,				0
+CLVLEX,      quec_exec_clvlex_cmd,				0
+QAUDPLAYEX,  quec_exec_qaudplayex_cmd,         0
+QAUDCFG, 	  quec_exec_qaudcfg_cmd,			0

#ifdef CONFIG_QUEC_PROJECT_FEATURE_AUDIO
#ifdef CONFIG_QUEC_PROJECT_FEATURE_AUDIO_RECORD
+QAUDRD,	  quec_exec_qaudrd_cmd,				0
+QAUDLOOP,    quec_exec_qaudloop_cmd,           0
#endif

+QPSND,		  quec_exec_qpsnd_cmd,				0
+CMUT,		  quec_exec_cmut_cmd,				0
+QAUDMOD,	  quec_exec_qaudmod_cmd,			0
+QAUDPASW,    quec_exec_qaudpasw_cmd,			0
+QAUDCALIB,   quec_exec_qaudcalib_cmd,			0
+QICSIDET,	  quec_exec_qicsidet_cmd,			0
+QICMIC,	  quec_exec_qicmic_cmd,				0
+QRXGAIN,	  quec_exec_qrxgain_cmd,			0

#ifdef CONFIG_QUEC_PROJECT_FEATURE_DTMF
#ifdef CONFIG_QUEC_PROJECT_FEATURE_DTMF_AT
+QTONEDET,	  quec_exec_qtonedetect_cmd,		0
+QLDTMF,      quec_exec_qldtmf_cmd,             0
+QWDTMF,      quec_exec_qwdtmf_cmd,             0
+QLTONE,      quec_exec_qltone_cmd,             0
+QLTONEX,      quec_exec_qltonex_cmd,             0
+QDTMFDETSETEX,    quec_exec_qdtmfdetset_cmd,       0

#endif
#endif


#ifndef __QUEC_OEM_VER_LGR__
#ifdef CONFIG_QUEC_PROJECT_FEATURE_EXT_CODEC
#ifdef CONFIG_QUEC_PROJECT_FEATURE_EXT_CODEC_AT
+QIIC,		  quec_exec_qiic_cmd,				0
+QAUDSW,	  quec_exec_audsw_cmd,				0
+QDAI,	  	  quec_exec_qdai_cmd,				0
#endif
#endif
#endif

#ifdef QUEC_AT_CRSL_SUPPORT
+CRSL,        quec_exec_crsl_cmd,				0
#endif

#endif /*CONFIG_QUEC_PROJECT_FEATURE_AUDIO*/
#endif /*CONFIG_QUEC_PROJECT_FEATURE_AUDIO AT*/
#endif /*CONFIG_QUEC_PROJECT_FEATURE_AUDIO || CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO*/

//********************** network/modem/rf at cmd ***********************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NW
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NW_AT                 
+QCEER,     quec_exec_qceer_cmd,                0
+QNWLOCK,   quec_exec_qnwlock_cmd,              0
+QDFTPDN,	quec_exec_qdftpdn_cmd,				0
+QOPS,      quec_exec_qops_cmd,                 0
+QLTS,   	quec_exec_qlts_cmd,                 0
+QCELL,	    quec_exec_qcellinfo_cmd, 			0
+QCELLEX,	quec_exec_qcellinfo_cmd, 			0
+QCELLINFO,	quec_exec_qcellinfo_cmd, 			0
+QGDCNT,    quec_exec_qgdcnt_cmd,               0
+QAUGDCNT,  quec_exec_qaugdcnt_cmd,             0
+QPDPGDCNT, quec_exec_qpdpgdcnt_cmd,            0
+QPDPTIMER, quec_exec_qpdptimer_cmd,			0
+CGATTEX,	atCmdHandleCGATT,					0
+QLOCREL,	quec_exec_qlocrel_cmd,				0
+QBLACKCELL,quec_exec_qblackcell_cmd,           0
+QBLACKCELLCFG,quec_exec_qblackcellcfg_cmd,     0
+QPWRBACKOFF,  quec_exec_qpwrbackoff_cmd,       0
+QRFPWRCFG, quec_exec_qrfpwrcfg_cmd,            0
+QPWRGSM,      quec_exec_qpwrgsm_cmd,           0
+QCEERCATCFG,  quec_exec_qceercatcfg_cmd,       0
+QPTWEDRXS,    quec_exec_qptwedrxs_cmd,         0
+QNWCFG,	quec_exec_qnwcfg_cmd,				0
#ifdef __QUEC_OEM_VER_CHP__
+QRFGPIO, atCmdHandleRFGPIO,  0
#endif
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+QDMZ,      quec_exec_qdmz_cmd,                 0
#if defined(__QUEC_OEM_VER_WB__)
+RSRP,      at_xxcd_rsrp_cmd_func,              0
#endif
#ifdef __QUEC_OEM_VER_HJ__
+QOEM,      quec_exec_qoem_cmd,                 0
^HCSQ,      quec_exec_hcsq_cmd,                 0
^SYSINFOEX, quec_exec_sysinfoex_cmd,            0
^SYSCFGEX,  quec_exec_syscfgex_cmd,             0
#endif
#ifdef __QUEC_OEM_VER_LT__
+QAVGRSSI,  quec_exec_qavgrssi_cmd,             0
+QNWSCAN,   quec_exec_qnwscan_cmd,              0
#endif
#endif
#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USBNET
+QNETDEVCTL,quec_exec_qnetdevctl_cmd,           0
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_RFTEST
#ifdef CONFIG_QUEC_PROJECT_FEATURE_RFTEST_AT
+QRXFTM,    quec_exec_qrxftm_cmd,               0
+QRFTEST,   quec_exec_qrftest_cmd,              0
+QRFTESTMODE, quec_exec_qrftestmode_cmd,        0
#endif  /* CONFIG_QUEC_PROJECT_FEATURE_RFTEST_AT */
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_JAMDET
#ifdef CONFIG_QUEC_PROJECT_FEATURE_JAMDET_AT
+QJDCFG,    quec_exec_qjdcfg_cmd,               0
+QJDR,      quec_exec_qjdr_cmd,                 0
#endif
#endif

//********************** sms/call/volte at cmd ***********************/
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SMS
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SMS_AT
//+CSCB,          atCmdHandleCSCB,                0

+QCMGS,         quectel_exec_qcmgs_cmd,         0
+QCMGR,         quectel_exec_qcmgr_cmd,         0
+QCSMP, 		quectel_exec_qcsmp_cmd, 		0
+QCMRC,         quectel_exec_qcmrc_cmd,         0
+QCMGL,         quectel_exec_qcmgl_cmd,         0
+QSMSCFG,       quectel_exec_qsmscfg_cmd,       0
+QTRUSTNUM,     quectel_exec_qtrustnum_cmd,     0  /* Use SMS macros to prevent SMS whitelists from not working after PBK is closed */
+QTNUMCTL,      quectel_exec_qtnumctl_cmd,      0
+QREFUSECS,     quectel_exec_qrefusecs_cmd,     0
#endif 
#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_VOICE_CALL
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CALL_AT
+CVHU,          quectel_exec_cvhu_cmd,          0
+QCHLDIPMPTY,   quectel_exec_QCHLDIPMPTY_cmd,   0
S7,             quectel_exec_S7_cmd,            0
+QECCNUM,       quectel_exec_QECCNUM_cmd,       0
H0,		        atCmdHandleH,		            0
O0,			    atCmdHandleO,					0
+QHUP,          quectel_exec_QHUP_cmd,          0
^DSCI,          quec_exec_dsci_cmd,             0
+QIMS,			quec_exec_qims_cmd, 			0
+CVMOD,         quec_exec_cvmod_cmd,            0
+QIMSCFG,       quec_exec_qimscfg_cmd,          0
+QIMSNV,        quec_exec_qimsnv_cmd,           0
#ifdef __QUEC_OEM_VER_LT__
*IMSRCFG,       quec_exec_imsrcfg_cmd,          0
*IMSCFG,        quec_exec_imscfg_cmd,           0
#endif
#endif /*CONFIG_QUEC_PROJECT_FEATURE_CALL_AT*/
#endif

//************************* sim at cmd **************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SIM
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SIM_AT
+QSIMWRITECNT,  quectel_exec_qsimwritecnt_cmd,  0
+QCCID,         atCmdHandleCCID,                0
+ICCID,         atCmdHandleCCID,                0
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+QINISTAT,      quectel_exec_qinistat_cmd,      0
+QSIMDET,       quectel_exec_qsimdet_cmd,       0
+QSLOTSTAT,     quec_exec_qslotstat_cmd,        0
#endif
+QSIMSTAT,      quectel_exec_qsimstat_cmd,      0
+QPINC,			quec_exec_qpinc_cmd,			0
+QWRITESIM,		quec_exec_qwritesim_cmd,		0
+QDSIM,			quec_exec_qdsim_cmd,			0
#ifndef CONFIG_QUEC_PROJECT_FEATURE_QESIM
+QESIM,         quec_exec_qesim_cmd,            0
#endif
+QSIMPLMNINFO,  quec_exec_qsimplmninfo_cmd,     0
+QSIMCHK,       quec_exec_qsimchk_cmd,          0
#endif /*CONFIG_QUEC_PROJECT_FEATURE_SIM_AT*/
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE
#ifdef __QUEC_OEM_VER_YW__
+WSLQ,          quec_exec_wslq_cmd,            0     //将CSQ转成百分比
#endif
#endif


#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+QDSREG,    quec_exec_qdsreg_cmd,               0
+QDSGREG,	quec_exec_qdsgreg_cmd,				0
+QDSEREG,	quec_exec_qdsereg_cmd,				0
+QDSQ,		quec_exec_qdsq_cmd,					0
+QDSOPS,	quec_exec_qdsops_cmd,				0
+QDSIMI,        quec_exec_qdsimi_cmd,           0
+QDSCCID,		quec_exec_qdsccid_cmd,			0
+QDSPIN,		quec_exec_qdspin_cmd,			0
#ifndef CONFIG_QUEC_PROJECT_FEATURE_QDSIM
+QDSIMCFG,      quec_exec_qdsimcfg_cmd,         0
#endif
+QDSTYPE,       quec_exec_qdstype_cmd,          0
#endif

//************************ vsim at cmd *************************/
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_QVSIM
#ifdef CONFIG_QUEC_PROJECT_FEATURE_QVSIM_AT
+QVSIM,         quec_exec_vsim_cmd,             0
+QVSIMW,        quec_exec_vsim_cmd,             0
#endif
#endif
#endif

//************************ esim at cmd *************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_QESIM
#ifdef CONFIG_QUEC_PROJECT_FEATURE_QESIM_AT
+QESIM,         quec_exec_esim_cmd,             0
#endif
#endif
//************************ audio at cmd *************************/


//************************* file at cmd *************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FILE
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FILE_AT
+QFUPL, 	    quec_exec_qfupl_cmd,		    0
+QFOPEN,        quec_exec_qfopen_cmd,			0
+QFLST,	        quec_exec_file_qflst_cmd,		0
+QFDEL, 		quec_exec_file_qfdel_cmd,		0
+QFCLOSE, 		quec_file_qfclose_cmd,    		0
+QFREAD,		quec_exec_file_qfread_cmd,      0
+QFLDS,         quec_exec_file_qflds_cmd,	    0
+QFWRITE,     	quec_exec_file_qfwrite_cmd,	    0
+QFDWL,         quec_exec_file_qfdwl_cmd, 		0
+QFPOSITION,	quec_exec_file_qfposition_cmd,  0
+QFSEEK,	    quec_exec_file_qfseek_cmd,		0
+QFMKDIR,	    quec_exec_file_qfmkdir_cmd,		0
+QFRMDIR,	    quec_exec_file_qfrmdir_cmd,		0
+QFTESTWR,      quec_exec_file_qftestwr_cmd,    0
+QFTUCAT,       quec_exec_file_qftucat_cmd,     0
+QFMD5,         quec_exec_file_qfmd5_cmd,       0
#endif
#ifdef CONFIG_QL_OPEN_EXPORT_PKG
+QCUSTNVM,      quec_exec_qcustnvm_cmd,         0
+QFACCUSTNVM,   quec_exec_qfaccustnvm_cmd,      0
#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
+QEFSMKFS,      quec_exec_efs_mkfs_cmd,         0  //format spi6 ext nor flash mount partition
#endif

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI4_EXT_NOR_SFFS
+QEXNMKFS,  quec_exec_exnsffs_mkfs_cmd,     0  //format spi4 ext nor flash mount partition
#endif
#endif

//************************ bt/ble at cmd ************************/
+EBTADDR,       quec_exec_ebtaddr_cmd,          0
+QBTLEIBEACFG,  quec_exec_bt_ibeacon_cfg_cmd,   0
+QBTNAME,       quec_exec_bt_ble_name_cmd,      0
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT_AT
+QBTPWR,        quec_exec_bt_poweronoff_cmd,    0
+QBTLEADDR,     quec_exec_bt_leaddr_cmd,        0
+QBTLERANADDR,  quec_exec_bt_lerandomaddr_cmd,  0
+QBTGATADV,     quec_exec_bt_adv_param_cmd,     0
+QBTADVDATA,    quec_exec_bt_adv_data_cmd,      0
+QBTADVSTR,     quec_exec_bt_adv_str_cmd,       0
+QBTADVRSPDATA, quec_exec_bt_adv_rspdata_cmd,   0
+QBTADV,        quec_exec_bt_adv_cmd,           0

+QBTSCANPARA,   quec_exec_bt_scan_param_cmd,    0
+QBTGATSCAN,    quec_exec_bt_scan_cmd,          0
+QBTGATCONN,    quec_exec_bt_gatt_connect_cmd,  0
+QBTGATDISCONN, quec_exec_bt_gatt_disconnect_cmd,  0

+QBTGATCONNP,   quec_exec_bt_gatt_update_conn_param_cmd,   0
+QBTLEADDWHL,   quec_exec_bt_add_whitelist_cmd,   0
+QBTLEDELWHL,   quec_exec_bt_del_whitelist_cmd,   0
+QBTLEWHLINFO,  quec_exec_bt_get_whitelist_cmd,   0
+QBTLEEXMTU,    quec_exec_bt_exchange_mtu_cmd,   0
+QBTLEIBEA,     quec_exec_bt_ibeacon_cmd,       0

+QBTSCANMODE,   quec_exec_bt_scanmode_cmd,      0
+QBTINQUIRY,    quec_exec_bt_inquiry_cmd,       0
+QBTFILTER,     quec_exec_bt_filter_config_cmd,   0
+QBTBOND,       quec_exec_bt_bond_cmd,          0   
+QBTSEND,       quec_exec_bt_ble_send_cmd,      0 
+QBTCFG,      quec_exec_qbtcfg_cmd,   0 

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BLE_GATT
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BLE_GATT_AT
+QBTGATSNOD,    quec_exec_bt_sendnod_cmd,       0
+QBTGATSIND,    quec_exec_bt_sendind_cmd,       0
+QBTLESTATE,    quec_exec_bt_gatt_state_cmd,    0
+QBTLESEND,     quec_exec_bt_gatt_send_cmd,     0
+QBTLEGSND,     quec_exec_bt_gatt_channel_cmd,  0
+QBTLERCVM,     quec_exec_bt_gatt_recv_mode_cmd,  0
+QBTLEREAD,     quec_exec_bt_gatt_read_cmd,     0

+QBTGATSERV,    quec_exec_bt_gatt_service_cmd,  0
+QBTGATINC,     quec_exec_bt_gatt_includes_cmd, 0
+QBTGATCHAR,    quec_exec_bt_gatt_chara_cmd,    0
+QBTGATDESC,    quec_exec_bt_gatt_desc_cmd,     0
+QBTWRCHAR,     quec_exec_bt_gatt_wrchar_cmd,   0
+QBTWRCHARNORSP,    quec_exec_bt_gatt_wrchar_norsp_cmd,   0
+QBTRDCHARHAND,     quec_exec_bt_gatt_rdchar_handle_cmd,  0
+QBTRDCHARUUID,     quec_exec_bt_gatt_rdchar_uuid_cmd,  0
+QBTGATWRDESC,  quec_exec_bt_gatt_wrdesc_cmd,   0
+QBTGATRDDESC,  quec_exec_bt_gatt_rddesc_cmd,   0

+QBTGATSS,      quec_exec_bt_gatt_add_service_cmd,   0
+QBTGATSC,      quec_exec_bt_gatt_add_chara_cmd,   0
+QBTGATSCV,     quec_exec_bt_gatt_add_charaval_cmd,   0
+QBTGATCHSCV,   quec_exec_bt_gatt_change_charaval_cmd,   0
+QBTGATSCD,     quec_exec_bt_gatt_add_charades_cmd,   0
+QBTGATSSC,     quec_exec_bt_gatt_add_service_complete_cmd,   0

+QGATSERCFG,   quec_exec_bt_gatt_srv_fast_cfg_cmd,   0
+QGATCHARAHDL, quec_exec_bt_gatt_get_chara_handle_cmd,   0

#endif /*CONFIG_QUEC_PROJECT_FEATURE_BLE_GATT_AT*/
#endif /*CONFIG_QUEC_PROJECT_FEATURE_BLE_GATT*/

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT_HFP
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT_HFP_AT
+QBTHFPCONN,    quec_exec_hfp_connect_cmd,      0
+QBTHFPDISCONN, quec_exec_hfp_disconnect_cmd,   0
+QBTHFPVOL,     quec_exec_hfp_set_vol_cmd,      0
+QBTHFPCALL,    quec_exec_hfp_call_cmd,         0
+QBTHFPDIAL,    quec_exec_hfp_dial_cmd,         0
+QBTHFPVOLR,    quec_exec_hfp_voice_rec_cmd,    0
+QBTHFPTHRC,    quec_exec_hfp_three_way_cmd,    0

#endif /*CONFIG_QUEC_PROJECT_FEATURE_BT_HFP_AT*/
#endif /*CONFIG_QUEC_PROJECT_FEATURE_BT_HFP*/

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT_HFP_AG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT_HFP_AG_AT
+QBTHFPAGDIAL,  quec_exec_hfp_ag_dial_cmd,      0
#endif /*CONFIG_QUEC_PROJECT_FEATURE_BT_HFP_AG_AT*/
#endif /*CONFIG_QUEC_PROJECT_FEATURE_BT_HFP_AG*/

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT_A2DP_AVRCP
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT_A2DP_AVRCP_AT
+QBTA2DPDISCONN,    quec_exec_a2dp_disconnect_cmd,     0
+QBTAVRCPVOL,       quec_exec_avrcp_vol_cmd,            0
+QBTAVRCPCTRL,      quec_exec_avrcp_ctrl_cmd,         0
+QBTAVRCPSTATE,     quec_exec_avrcp_get_state_cmd,      0
+QBTA2DPCONN,       quec_exec_a2dp_connect_cmd,     0
#endif /*CONFIG_QUEC_PROJECT_FEATURE_BT_A2DP_AVRCP_AT*/
#endif /*CONFIG_QUEC_PROJECT_FEATURE_BT_A2DP_AVRCP*/


#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT_SPP
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT_SPP_AT
+QBTSPPCONN,        quec_exec_spp_connect_cmd,     0
+QBTSPPDISCONN,     quec_exec_spp_disconnect_cmd,  0
+QBTSPPSENDDATA,    quec_exec_spp_senddata_cmd,    0
#endif /*CONFIG_QUEC_PROJECT_FEATURE_BT_SPP_AT*/
#endif /*CONFIG_QUEC_PROJECT_FEATURE_BT_SPP*/


#endif /*CONFIG_QUEC_PROJECT_FEATURE_BT_AT*/
#endif /*CONFIG_QUEC_PROJECT_FEATURE_BT*/

//************************ wifi at cmd ************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_WIFISCAN
#ifdef CONFIG_QUEC_PROJECT_FEATURE_WIFISCAN_AT
+QWIFISCAN,         quec_exec_wifiscan_cmd,          0
+QWIFISCANEX,	    quec_exec_async_wifiscan_cmd,	 0
+QWIFISCANOFFSET,   quec_exec_wifiscan_offset_cmd,    0
#endif
#endif

//************************ stk at cmd ************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_STK
#ifdef CONFIG_QUEC_PROJECT_FEATURE_STK_AT
+QSTK,          quec_exec_qstk_cmd,             0
+QSTKPD,        quec_exec_qstkpd_cmd,           0
+QSTKSTATE,     quec_exec_qstkstate_cmd,        0
+QSTKGI,        quec_exec_qstkgi_cmd,           0
+QSTKRSP,       quec_exec_qstkrsp_cmd,          0
#endif
#endif

//************************* GNSS at cmd *************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS_AT
+QGPSCFG, 	    at_exec_qgpscfg_cmd,		    0
+QGPSDEL,       at_exec_qgpsdel_cmd,			0
+QGPS,	        at_exec_qgps_cmd,		0
+QGPSEND, 		at_exec_qgpsend_cmd,		0
+QGPSLOC, 		at_exec_qgpsloc_cmd,    		0
+QGPSGNMEA,		at_exec_qgpsgnmea_cmd,      0
+QAGPS,         at_exec_qagps_cmd,      0
+QGPSINFO,      at_exec_qgpsinfo_cmd,      0
//+QGPSXTRA,      at_exec_qgpsxtra_cmd,	    0
//+QGPSXTRATIME,  at_exec_qgpsxtratime_cmd,	    0
//+QGPSXTRADATA,  at_exec_qgpsxtradata_cmd, 		0
+QGPSCMD,	    at_exec_qgpscmd_cmd,		0
+QAGPSCFG,      at_exec_qagpscfg_cmd,            0
+QGPSPOWER,     at_exec_qgpspower_cmd,           0
#endif /*CONFIG_QUEC_PROJECT_FEATURE_GNSS_AT*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_RTK
#ifdef CONFIG_QUEC_PROJECT_FEATURE_RTK_AT 
+QRTK, 	        at_exec_qrtk_cmd,		        0
+QRTKNMEA,      at_exec_qrtknmea_cmd,			0
#endif /*CONFIG_QUEC_PROJECT_FEATURE_RTK_AT*/
#endif

#endif


//************************ camera  at cmd *************************/
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA_AT
+QCAMOPEN,		  quec_exec_qcamopen_cmd,				0
+QCAMIDFY,        quec_exec_qcamidfy_cmd,               0   
+QCAMCLOSE,       quec_exec_qcamclose_cmd,              0
+QCAMUPLOAD,      quec_exec_qcamupload_cmd,             0
+QCAMAPPVER,      quec_exec_qcamappver_cmd,             0
+QCAMCFG,         quec_exec_qcamcfg_cmd,                0 
#endif
#endif


//************************ decoder at cmd *************************/
+QAUTHCODE,     quec_exec_authcode_cmd,         0

//************************ other at cmd *************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE
L, 			 atCmdHandleAT,					   0
M,			 atCmdHandleAT,					   0
#endif

//************************ firewall at cmd *************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FIREWALL_AT
+QFIREWALLCFG,    at_firewall_qfirewallcfg_cmd_func, 0
#endif

//************************ slas at cmd *************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SLAS
+QLICENSE,     quec_exec_qlicense_cmd,         0
+QLICENSEINFO,     quec_exec_qlicenseinfo_cmd,         0
#endif

#if  defined(__QUEC_OEM_VER_LWM2M__) || defined(__QUEC_OEM_VER_RFR__)
#ifdef CONFIG_OSA_QUECTEL_LWM2M_AT_FUNC
+QLWCFG,        quec_exec_lwm2m_qlwcfg_cmd,     0
+QLWCONFIG,     quec_exec_lwm2m_qlwconfig_cmd,  0
+QLWREG,        quec_exec_lwm2m_qlwreg_cmd,     0
+QLWUPDATE,     quec_exec_lwm2m_qlwupdate_cmd,  0
+QLWDEREG,      quec_exec_lwm2m_qlwdereg_cmd,   0
+QLWSTATUS,     quec_exec_lwm2m_qlwstatus_cmd,  0
+QLWADDOBJ,     quec_exec_lwm2m_qlwaddobj_cmd,  0
+QLWDELOBJ,     quec_exec_lwm2m_qlwdelobj_cmd,  0
+QLWRDRSP,      quec_exec_lwm2m_qlwrdrsp_cmd,   0
+QLWWRRSP,      quec_exec_lwm2m_qlwwrrsp_cmd,   0
+QLWEXERSP,     quec_exec_lwm2m_qlwexersp_cmd,  0
+QLWOBSRSP,     quec_exec_lwm2m_qlwobsrsp_cmd,  0
+QLWNOTIFY,     quec_exec_lwm2m_qlwnotify_cmd,  0
+QLWRD,         quec_exec_lwm2m_qlwrd_cmd,      0
+QLWRESET,      quec_exec_lwm2m_qlwreset_cmd,   0
#endif /*CONFIG_OSA_QUECTEL_LWM2M_AT_FUNC*/
#endif /*OEM_VER_LWM2M || OEM_VER_RFR */

#endif /* CONFIG_QUEC_PROJECT_AT_CUT */

#endif /* QUEC_ATCMD_DEF_H */

