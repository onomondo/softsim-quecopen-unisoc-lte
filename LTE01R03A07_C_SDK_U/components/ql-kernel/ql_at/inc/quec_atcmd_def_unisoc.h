/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

#ifndef _QUEC_ATCMD_DEF_UNISOC_H_
#define _QUEC_ATCMD_DEF_UNISOC_H_

#ifdef CONFIG_QUEC_PROJECT_FEATURE
//以下AT请勿裁剪,需要用于调试
I,              atCmdHandleI, 0
+GSN,			atCmdHandleGSN, 0
+CGSN,			atCmdHandleCGSN, 0
+CFUN,			atCmdHandleCFUN, 0
+CSQ,			atCmdHandleCSQ, 0
#ifndef CONFIG_LTE_ONLY_SUPPORT
+CREG,			atCmdHandleCREG, AT_CON_NOT_CALIB_MODE			// 7.2 Network registration
#endif
+EGMR,			atCmdHandleEGMR, 0
+CGPADDR,		atCmdHandleCGPADDR, 0
+CGDCONT,		atCmdHandleCGDCONT, 0
+CGACT, 		atCmdHandleCGACT, 0
#ifdef CONFIG_QUEC_PROJECT_FEATURE_AUDIO
//音频校准AT,如果不使用音频功能可裁剪,如果使用音频功能可能会要调节音质,这些AT会被音频校准工具调用
+AUDCH, 		atCmdHandleAUDCH, 0 		// Set receiver, headset and loudspeaker channel
+CACCP, 		atCmdHandleCACCP, AT_CON_NOT_CALIB_MODE 		// Audio Codec Calibration Param
+CAVQE, 		atCmdHandleCAVQE, AT_CON_NOT_CALIB_MODE 		// Audio ZSP VQE Calibration Param
+CAPRE, 		atCmdHandleCAPRE, AT_CON_NOT_CALIB_MODE 		// Audio ZSP Pre -Processing Calibration Param
+CAPOST,		atCmdHandleCAPOST, AT_CON_NOT_CALIB_MODE		// Audio ZSP Post -Processing Calibration Param
+CAWTF, 		atCmdHandleCAWTF, AT_CON_NOT_CALIB_MODE 		// Write calibration param to flash
+CAIET, 		atCmdHandleCAIET, AT_CON_NOT_CALIB_MODE 		// Export calibration param from flash or import calibration param to flash
+CADTF, 		atCmdHandleCADTF, AT_CON_NOT_CALIB_MODE 		// dump PCM data to Tflash card
+CAVCT, 		atCmdHandleCAVCT, AT_CON_NOT_CALIB_MODE 		// version
+CANXP, 		atCmdHandleCANXP, AT_CON_NOT_CALIB_MODE 		//
#endif
#endif

//以下AT可以根据需要来裁剪,打开CONFIG_QUEC_PROJECT_AT_CUT后,
//下方所有AT都会被裁剪,用户也可以根据需要自行保留需要的AT
#ifndef CONFIG_QUEC_PROJECT_AT_CUT
// V.250
Z,              atCmdHandleZ, 0             // (6.1.1) Reset to default configuration
&F,             atCmdHandleAndF, 0          // (6.1.2) Set to factory defined configuration
+GMI,           atCmdHandleGMI, 0           // (6.1.4) Request manufacturer identification
+GMM,           atCmdHandleGMM, 0           // (6.1.5) Request model identification
+GMR,           atCmdHandleGMR, 0           // (6.1.6) Request revision identification
S3,             atCmdHandleS3, 0            // (6.2.1) Command line termination character
S4,             atCmdHandleS4, 0            // (6.2.2) Response formatting character
S5,             atCmdHandleS5, 0            // (6.2.3) Command line editing character
E,              atCmdHandleE, 0             // (6.2.4) Command echo
Q,              atCmdHandleQ, 0             // (6.2.5) Result code suppression
V,              atCmdHandleV, 0             // (6.2.6) DCE response format
X,              atCmdHandleX, 0             // (6.2.7) Result code selection and call progress monitoring control
&D,             atCmdHandleAndD, 0          // (6.2.9) Circuit 108/2 (DTR) behavior
+IPR,           atCmdHandleIPR, 0           // (6.2.10) Fixed DTE rate
+ICF,           atCmdHandleICF, 0           // (6.2.11) DTE-DCE character framing
+IFC,           atCmdHandleIFC, 0           // (6.2.12) DTE-DCE local flow control
D,              atCmdHandleD, AT_CON_NOT_CALIB_MODE             // (6.3.1) Dial
A,              atCmdHandleA, AT_CON_NOT_CALIB_MODE             // (6.3.5) Answer
H,              atCmdHandleH, AT_CON_NOT_CALIB_MODE             // (6.3.6) Hook control
O,              atCmdHandleO, 0             // (6.3.7) Return to online data state
S0,             atCmdHandleS0, 0            // (6.3.8) Automatic answer
&W,             atCmdHandleAndW, 0          // Store current profile
&Y,             atCmdHandleAndY, 0          // Set default reset profile
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+CLAC,          atCmdHandleCLAC, 0          // List all command
+URC,           atCmdHandleURC, 0	    // Set Urc Enable/disable
#ifdef CONFIG_AT_CAMERA_SUPPORT
+CAM,           atCmdHandleCAM, 0           //Camera Function
#endif
#ifdef CONFIG_SOC_8910
+TUEINFO,       atCmdHandleTUEINFO, 0       // Show Tue status info
#endif
#ifdef CONFIG_AT_SSIM_SUPPORT
+SCANRSSI,      atCmdHandleSCANRSSI, 0      // Scan RSSI according to band
#endif
#endif

#ifdef __QUEC_OEM_VER_OPC__
+DPSD,          atCmdHandleDiscardPSData, 0
#endif
//
// 3GPP TS 27.005 (R14)
//
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
//added by ryan.yi。若quectel SMS功能裁了，把unisoc sms指令也裁了。否则替换quectel at引擎后编译报错
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SMS
+CSMS,          atCmdHandleCSMS, AT_CON_NOT_CALIB_MODE          // 3.2.1 Select Message Service
+CPMS,          atCmdHandleCPMS, AT_CON_NOT_CALIB_MODE          // 3.2.2 Preferred Message Storage
+CMGF,          atCmdHandleCMGF, AT_CON_NOT_CALIB_MODE          // 3.2.3 Message Format
+CSCA,          atCmdHandleCSCA, AT_CON_NOT_CALIB_MODE          // 3.3.1 Service Centre Address
+CSMP,          atCmdHandleCSMP, AT_CON_NOT_CALIB_MODE          // 3.3.2 Set Text Mode Parameters
+CSDH,          atCmdHandleCSDH, 0          // 3.3.3 Show Text Mode Parameters
+CSCB,          atCmdHandleCSCB, AT_CON_NOT_CALIB_MODE          // 3.3.4 Select Cell Broadcast Message Types

+CSAS,          atCmdHandleCSAS, 0          // 3.3.5 Save Settings
+CRES,          atCmdHandleCRES, 0          // 3.3.6 Restore Settings
+CNMI,          atCmdHandleCNMI, AT_CON_NOT_CALIB_MODE          // 3.4.1 New Message Indications to TE
+CMGL,          atCmdHandleCMGL, AT_CON_NOT_CALIB_MODE          // 3.4.2/4.1 List Messages
+CMGR,          atCmdHandleCMGR, AT_CON_NOT_CALIB_MODE          // 3.4.3/4.2 Read Message
+CNMA,          atCmdHandleCNMA, AT_CON_NOT_CALIB_MODE          // 3.4.4 New Message Acknowledgement to ME/TA
+CMGS,          atCmdHandleCMGS, AT_CON_NOT_CALIB_MODE          // 3.5.1/4.3 Send Message
+CMSS,          atCmdHandleCMSS, AT_CON_NOT_CALIB_MODE          // 3.5.2/4.7 Send Message from Storage
+CMGW,          atCmdHandleCMGW, AT_CON_NOT_CALIB_MODE         // 3.5.3/4.4 Write Message to Memory
+CMGD,          atCmdHandleCMGD, AT_CON_NOT_CALIB_MODE          // 3.5.4 Delete Message
+CMGC,          atCmdHandleCMGC, AT_CON_NOT_CALIB_MODE          // 3.5.5/4.5 Send Command
+CMMS,          atCmdHandleCMMS, AT_CON_NOT_CALIB_MODE          // 3.5.6 More Messages to Send
+CGSEND,        atCmdHandleCGSEND, AT_CON_NOT_CALIB_MODE
#endif
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#ifdef CONFIG_CONCAT_SMS_SUPPORT
+CONCATMSG,    atCmdHandleCONCATMSG, AT_CON_NOT_CALIB_MODE
#endif
#endif

//
// 3GPP TS 27.007 (R14)
//
+CGMI,          atCmdHandleCGMI, 0          // 5.1 Request manufacturer identification
+CGMM,          atCmdHandleCGMM, 0          // 5.2 Request model identification
+CGMR,          atCmdHandleCGMR, 0          // 5.3 Request revision identification
+CSCS,          atCmdHandleCSCS, 0          // 5.5 Select TE character set
+CIMI,          atCmdHandleCIMI, 0          // 5.6 Request international mobile subscriber identity
+CEID,           atCmdHandleEID, 0           // get eid
+CIMIM,         atCmdHandleCIMIM, 0          // 5.6 Request international mobile subscriber identity

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef CONFIG_ATR_CMUX_SUPPORT
+CMUX,          atCmdHandleCMUX, 0          // 5.7 Multiplexing mode
#endif
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+CGBV,          atCmdHandleCGBV, 0
#endif

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+CMOD,          atCmdHandleCMOD, 0          // 6.4 Call mode
+CHUP,          atCmdHandleCHUP, 0          // 6.5 Hangup call
+CR,            atCmdHandleCR, 0            // 6.9 Service reporting control
+CRC,           atCmdHandleCRC, 0           // 6.11 Cellular result codes
+CHLD,          atCmdHandleCHLD, 0          // 7.13 Call related supplementary services
#if defined (LTE_NBIOT_SUPPORT) || defined (LTE_SUPPORT)
+CDU,           atCmdHandleCDU, 0           // 13.2.3 Hangup of current calls
+CHCCS,         atCmdHandleCHCCS, AT_CON_NOT_CALIB_MODE         // 13.2.3 Hangup of current calls
#if defined (CONFIG_SOC_8910)
+CPSMS,         atCmdHandleCPSMS, AT_CON_NOT_CALIB_MODE         // 7.38 Power saving mode setting
+CEDRXS,        atCmdHandleCEDRXS, AT_CON_NOT_CALIB_MODE        // 7.40 eDRX setting
+CEDRXRDP,      atCmdHandleCEDRXRDP, AT_CON_NOT_CALIB_MODE      // 7.41 eDRX read dynamic parameters
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+CFGEDRX,       atCmdHandleCFGEDRX, AT_CON_NOT_CALIB_MODE
#endif
#endif
#endif
#ifdef CFW_VOLTE_SUPPORT
+CCFCU,         atCmdHandleCCFCU, AT_CON_NOT_CALIB_MODE         // 7.35 Communication forwarding number and conditions with URI support
+CISRVCC,       atCmdHandleCISRVCC, AT_CON_NOT_CALIB_MODE       // 8.63 IMS single radio voice call continuity
+CEUS,          atCmdHandleCEUS, 0          // 10.1.34 UE's usage setting for EPS
+CEVDP,         atCmdHandleCEVDP, 0         // 10.1.35 UE's voice domain preference E-UTRAN
#endif

+VTS,           atCmdHandleVTS, 0           // C.2.11 DTMF and tone generation
+VTD,           atCmdHandleVTD, 0           // C.2.12 Tone duration
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE
#ifdef CONFIG_DTMF_KEY_DETECT_SUPPORT
+DVT,           atCmdHandleDVT, 0           // Detect Voice Tone
#endif
#endif

//
// SIM && PBK related commands
//
+CSIM,          atCmdHandleCSIM, 0
+CCHO,          atCmdHandleCCHO, 0
+CCHC,          atCmdHandleCCHC, 0
+CGLA,          atCmdHandleCGLA, 0
+CPIN2,         atCmdHandleCPIN2, AT_CON_NOT_CALIB_MODE         // Authentication(for Sim)
^CPINC,         atCmdHandleCPINC, AT_CON_NOT_CALIB_MODE         // Total times of Access the Sim Card
+CRSML,         atCmdHandleCRSML, AT_CON_NOT_CALIB_MODE         // Read records of EF file on (U)SIM
#ifndef CONFIG_QUEC_PROJECT_FEATURE
^SIMOPERTIMES,  atCmdHandleSIMOperationTimes,   AT_CON_NOT_CALIB_MODE       //Get the total number of SIM card operations.
+SIMCON,       atCmdHandleSIMCON,0
+SIMCNT,       atCmdHandleSIMCNT,0
#endif

// Network service related commands
//
+CNUM,          atCmdHandleCNUM, AT_CON_NOT_CALIB_MODE          // 7.1 Subscriber number
+COPS,          atCmdHandleCOPS, AT_CON_NOT_CALIB_MODE          // 7.3 PLMN selection
+COPN,          atCmdHandleCOPN, AT_CON_NOT_CALIB_MODE          // 7.21 Read operator names
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+CLIP,          atCmdHandleCLIP, AT_CON_NOT_CALIB_MODE          // 7.6 Calling line identification presentation
+CLIR,          atCmdHandleCLIR, AT_CON_NOT_CALIB_MODE          // 7.7 Calling line identification restriction
+CCWA,          atCmdHandleCCWA, AT_CON_NOT_CALIB_MODE          // 7.12 Call waiting
+CUSD,          atCmdHandleCUSD, 0          // 7.15 Unstructured supplementary service data
#endif
+CLCC,          atCmdHandleCLCC, AT_CON_NOT_CALIB_MODE          // 7.18 List current calls
+CPOL,          atCmdHandleCPOL, AT_CON_NOT_CALIB_MODE          // 7.19 Preferred PLMN list
+CPLS,          atCmdHandleCPLS, AT_CON_NOT_CALIB_MODE          // 7.20 Selection of preferred PLMN list
+CPWD,          atCmdHandleCPWD, AT_CON_NOT_CALIB_MODE          // 7.5 Change password
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+COLP,          atCmdHandleCOLP, 0          // 7.8 Connected line identification presentation
+CCFC,       	atCmdHandleCCFC, AT_CON_NOT_CALIB_MODE          // 7.11 Call forwarding number and conditions
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+CCED,          atCmdHandleCCED, AT_CON_NOT_CALIB_MODE
+MJDC,          atCmdHandleMJDC, 0          //Detect the jamming.
+MJRI,          atCmdHandleMJRI, 0          //jamming rssi info
#endif
+SDMBS,         atCmdHandleSDMBS, 0         //Set Pseudo base station identification.
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+SGGO,         atCmdHandleSGGO, 0         //if gsmGprsOnly is 1,we will not register for GSM-only network.
#ifndef CONFIG_LTE_ONLY_SUPPORT
+CTEC,          atCmdHandleCTEC, AT_CON_NOT_CALIB_MODE          // Change RAT
#endif
^BLACKLIST,     atCmdHandleBLACKLIST, AT_CON_NOT_CALIB_MODE
^BLACKLISTTMR,  atCmdHandleBLACKLISTTMR, AT_CON_NOT_CALIB_MODE
+UJDC,          atCmdHandleUJDC, AT_CON_NOT_CALIB_MODE //Detect the jamming.
+UJDCP,         atCmdHandleUJDCP,AT_CON_NOT_CALIB_MODE //Detect the jamming parameters.
+UJRI,          atCmdHandleUJRI, AT_CON_NOT_CALIB_MODE //jamming rssi info.
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#if defined (CONFIG_SOC_8910)
+CDAC,          atCmdHandleCDAC,0           //Set&Get CDAC value
#endif
#endif

#if defined (LTE_NBIOT_SUPPORT) || defined (LTE_SUPPORT)
+CEREG,         atCmdHandleCEREG, AT_CON_NOT_CALIB_MODE         // 10.1.22 EPS network registration status
+CSCON,         atCmdHandleCSCON, AT_CON_NOT_CALIB_MODE         // 10.1.30 Signalling connection status +CSCON
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+NPSMR,        atCmdHandleNPSMR, 0
#endif
#endif
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+CSDF,          atCmdHandleCSDF, 0          // 6.22 Settings date format +CSDF
+CPAS,          atCmdHandleCPAS, 0          // 8.1 Phone activity status
#endif
+CPIN,          atCmdHandleCPIN, AT_CON_NOT_CALIB_MODE          // 8.3 Enter PIN
+CLCK,          atCmdHandleCLCK, AT_CON_NOT_CALIB_MODE          // 7.4 Facility lock
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+CDIS,          atCmdHandleCDIS, 0          // 8.8 Display control
+CMER,          atCmdHandleCMER, 0          // 8.10 Mobile termination event reporting
+CPBS,          atCmdHandleCPBS, AT_CON_NOT_CALIB_MODE          // 8.11 Select phonebook memory storage +CPBS
+CPBR,          atCmdHandleCPBR, AT_CON_NOT_CALIB_MODE          // 8.12 Read phonebook entries +CPBR
+CPBF,          atCmdHandleCPBF, AT_CON_NOT_CALIB_MODE          // 8.13 Find phonebook entries +CPBF
+CPBW,          atCmdHandleCPBW, AT_CON_NOT_CALIB_MODE          // 8.14 Write phonebook entry +CPBW
#endif
+CCLK,          atCmdHandleCCLK, 0          // 8.15 Clock +CCLK
+CRSM,          atCmdHandleCRSM, AT_CON_NOT_CALIB_MODE          // 8.18 Restricted SIM access
+CACM,          atCmdHandleCACM, AT_CON_NOT_CALIB_MODE          // 8.25 Accumulated call meter
+CTZU,          atCmdHandleCTZU, 0          // 8.40 Automatic time zone update +CTZU
+CTZR,          atCmdHandleCTZR, 0          // 8.41 Time zone reporting +CTZR
+CPINR,         atCmdHandleCPINR, 0         // 8.65 Remaining PIN retries +CPINR
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+TPIN,          atCmdHandleCPINR, 0         // same as +CPINR
#endif
+CCID,          atCmdHandleCCID, AT_CON_NOT_CALIB_MODE          // Get Iccid
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+SIM,           atCmdHandleSIM, AT_CON_NOT_CALIB_MODE
^SIMIF,         atCmdHandleSIMIF, 0
+SIMCROSS,      atCmdHandleSIMCROSS, AT_CON_NOT_CALIB_MODE      //Switch sim id
#endif
+QSPN,          atCmdHandleQSPN, AT_CON_NOT_CALIB_MODE
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+QGID,          atCmdHandleQGID, AT_CON_NOT_CALIB_MODE

+POC,           atCmdHandlePOC, 0
#ifdef CONFIG_DUAL_SIM_SUPPORT
+PSIM,          atCmdHandlePSIM, 0
+PAS,           atCmdHandlePAS, 0
+PUSIM,         atCmdHandlePUSIM, 0
#endif

#if 0
^STA,           atCmdHandleSTA, 0           //
^STGI,          atCmdHandleSTGI, 0           //
^STR,           atCmdHandleSTR, 0           //
^STSF,          atCmdHandleSTSF,0
#endif

+STKTR,         atCmdHandleSTKTR, 0
+STKENV,        atCmdHandleSTKENV, 0
+STKEN,         atCmdHandleSTKEN, 0
+STKM,         atCmdHandleSTKM, 0
+STKPD,		   atCmdHandleSTKPD, 0

^CVSIMRESET,    atCmdHandleCVSIMRESET, 0  //reset vsim Command
#endif

#ifdef CONFIG_ATS_ALARM_SUPPORT
+CALA,          atCmdHandleCALA, 0          // 8.16 Alarm +CALA
+CALD,          atCmdHandleCALD, 0          // 8.38 Delete alarm +CALD
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+CIPMUX,        AT_TCPIP_CmdFunc_CIPMUX, 0
+CIPSGTXT,      AT_TCPIP_CmdFunc_CIPSGTXT, 0
+CIPSTART,      AT_TCPIP_CmdFunc_CIPSTART, 0
+CIFSR,         AT_TCPIP_CmdFunc_CIFSR, 0
+CIPSTATUS,     AT_TCPIP_CmdFunc_CIPSTATUS, 0
+CIPSCONT,      AT_TCPIP_CmdFunc_CIPSCONT, 0
+CIPCLOSE,      AT_TCPIP_CmdFunc_CIPCLOSE, AT_CON_NOT_CALIB_MODE
+CIPSHUT,       AT_TCPIP_CmdFunc_CIPSHUT, AT_CON_NOT_CALIB_MODE
+CIICR,         AT_TCPIP_CmdFunc_CIICR, 0
+CIPSEND,       AT_TCPIP_CmdFunc_CIPSEND, AT_CON_NOT_CALIB_MODE
+CIPQSEND,      AT_TCPIP_CmdFunc_CIPQSEND, AT_CON_NOT_CALIB_MODE
+CIPRXGET,      AT_TCPIP_CmdFunc_CIPRXGET, AT_CON_NOT_CALIB_MODE
+CSTT,          AT_TCPIP_CmdFunc_CSTT, AT_CON_NOT_CALIB_MODE
+CIPATS,        AT_TCPIP_CmdFunc_CIPATS, 0
+CDNSGIP,       AT_TCPIP_CmdFunc_CDNSGIP, AT_CON_NOT_CALIB_MODE
+CDNSCFG,       AT_TCPIP_CmdFunc_CDNSCFG, AT_CON_NOT_CALIB_MODE
+CIPSPRT,       AT_TCPIP_CmdFunc_CIPSPRT, 0
+CIPHEAD,       AT_TCPIP_CmdFunc_CIPHEAD, 0
+CIPCSGP,       AT_TCPIP_CmdFunc_CIPCSGP, 0
+CIPSRIP,       AT_TCPIP_CmdFunc_CIPSRIP, 0
+CIPDPDP,       AT_TCPIP_CmdFunc_CIPDPDP, 0
+CIPMODE,       AT_TCPIP_CmdFunc_CIPMODE, AT_CON_NOT_CALIB_MODE
+CIPCCFG,       AT_TCPIP_CmdFunc_CIPCCFG, 0
+CIPSHOWTP,     AT_TCPIP_CmdFunc_CIPSHOWTP, 0
+CIPUDPMODE,    AT_TCPIP_CmdFunc_CIPUDPMODE, AT_CON_NOT_CALIB_MODE
+CIPACK,        AT_TCPIP_CmdFunc_CIPACK, AT_CON_NOT_CALIB_MODE
+CLPORT,        AT_TCPIP_CmdFunc_CLPORT, 0
+CIPSERVER,     AT_TCPIP_CmdFunc_CIPSERVER, AT_CON_NOT_CALIB_MODE
+CIPTKA,        AT_TCPIP_CmdFunc_CIPTKA, 0
+CIPRDTIMER,    AT_TCPIP_CmdFunc_CIPRDTIMER, 0
+PING,          AT_TCPIP_CmdFunc_PING, AT_CON_NOT_CALIB_MODE
+PINGSTOP,      AT_TCPIP_CmdFunc_PINGSTOP, AT_CON_NOT_CALIB_MODE
+SETROUTE,    AT_TCPIP_CmdFunc_SETROUTE, 0
+UNSETROUTE,    AT_TCPIP_CmdFunc_UNSETROUTE, 0
#if 0
+TSOCR,         AT_TCPIP_CmdFunc_TSOCR, 0
+TSOST,         AT_TCPIP_CmdFunc_TSOST, 0
+TSOSTF,        AT_TCPIP_CmdFunc_TSOSTF, 0
+TSORF,         AT_TCPIP_CmdFunc_TSORF, AT_CON_NOT_CALIB_MODE
+TSOCL,         AT_TCPIP_CmdFunc_TSOCL, AT_CON_NOT_CALIB_MODE
+TPING,         AT_TCPIP_CmdFunc_TPING, AT_CON_NOT_CALIB_MODE
#endif
#endif
^NETIF,         AT_NET_CmdFunc_NetInfo, AT_CON_NOT_CALIB_MODE
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+SSLSTART,        AT_TCPIP_CmdFunc_SSLSTART, 0
+SSLSEND,         AT_TCPIP_CmdFunc_SSLSEND, 0
+SSLCLOSE,        AT_TCPIP_CmdFunc_SSLCLOSE, 0
#endif
//
// Mobile Termination control and status commands
//
+CBC,           atCmdHandleCBC, 0           // 8.4 Battery charge
+CIND,          atCmdHandleCIND, 0          // 8.9 Indicator control
+CESQ,          atCmdHandleCESQ, AT_CON_NOT_CALIB_MODE          // 8.69 Extended signal quality
+CALIBINFO,     atCmdHandleCALIBINFO, AT_CON_NOT_CALIB_MODE     // Check the calibration mark bit
+CMEE,          atCmdHandleCMEE, 0          // 9.1 Report mobile termination error
+CEER,          atCmdHandleCEER, 0          // 6.10 Extended error report
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+CPOF,          atCmdHandleCPOF, 0
+TRB,           atCmdHandleTRB, 0
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+CSCLK,         atCmdHandleCSCLK, 0
#endif
+CGREG,         atCmdHandleCGREG, AT_CON_NOT_CALIB_MODE         // 10.1.20 GPRS network registration status
+CGQMIN,        atCmdHandleCGQMIN, AT_CON_NOT_CALIB_MODE        // 10.1.5 Quality of service profile (minimum acceptable)
+CGATT,         atCmdHandleCGATT, AT_CON_NOT_CALIB_MODE         // 10.1.9 PS attach or detach
+CGDEL,         atCmdHandleCGDEL, AT_CON_NOT_CALIB_MODE         // 10.1.29 Delete non-active PDP contexts
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+CFGIMSPDN,     atCmdHandleCFGIMSPDN, 0
^PDNACTINFO,    atCmdHandlePDNACTINFO, 0    // RW PDN act or deact retry timer and retry maxcount
#endif

+CGAUTH,        atCmdHandleCGAUTH, 0        // 10.1.31 Define PDP context authentication parameters
#ifdef LTE_NBIOT_SUPPORT
+CGDSCONT,      atCmdHandleCGDSCONT, AT_CON_NOT_CALIB_MODE      // 10.1.2 Define secondary PDP context
#endif
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#if defined(LTE_NBIOT_SUPPORT) || defined(LTE_SUPPORT)
+CGTFT,         atCmdHandleCGTFT, AT_CON_NOT_CALIB_MODE         // 10.1.3 Traffic flow template
+CGTFTRDP,      atCmdHandleCGTFTRDP, 0      // 10.1.25 Traffic flow template read dynamic parameters
+CGEQREQ,       atCmdHandleCGEQREQ, 0       // 10.1.6 3G quality of service profile (requested)
+CGEQMIN,       atCmdHandleCGEQMIN, 0       // 10.1.7 3G quality of service profile (minimum acceptable)
#endif
#endif
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#if defined(LTE_NBIOT_SUPPORT) || defined(LTE_SUPPORT)
+CGCMOD,        atCmdHandleCGCMOD, AT_CON_NOT_CALIB_MODE        // 10.1.11 PDP context modify
#endif
+CGEREP,        atCmdHandleCGEREP, 0        // 10.1.19 Packet domain event reporting
+CGSMS,         atCmdHandleCGSMS, AT_CON_NOT_CALIB_MODE         // 10.1.21 Select service for MO SMS messages
+CGCONTRDP,      atCmdHandleCGCONTRDP, AT_CON_NOT_CALIB_MODE    // 10.1.23 PDP context read dynamic parameters +CGCONTRDP
+CGSCONTRDP,    atCmdHandleCGSCONTRDP, AT_CON_NOT_CALIB_MODE    // 10.1.24 Secondary PDP context read dynamic parameters
#if defined(LTE_NBIOT_SUPPORT) || defined(LTE_SUPPORT)
+CGEQOS,        atCmdHandleCGEQOS, AT_CON_NOT_CALIB_MODE        // 10.1.26 Define EPS quality of service
#endif
#endif
+CEMODE,        atCmdHandleCEMODE, AT_CON_NOT_CALIB_MODE        // 10.1.28 UE modes of operation for EPS
#if defined(LTE_NBIOT_SUPPORT) || defined(LTE_SUPPORT)
+CGEQOSRDP,     atCmdHandleCGEQOSRDP, AT_CON_NOT_CALIB_MODE     // 10.1.27 EPS quality of service read dynamic parameters
#endif
#if defined(LTE_NBIOT_SUPPORT) || defined(LTE_SUPPORT)
+CSODCP,        atCmdHandleCSODCP, AT_CON_NOT_CALIB_MODE        // 10.1.43 Sending of originating data via the control plane +CSODCP
#endif
+CGDATA,        atCmdHandleCGDATA, AT_CON_NOT_CALIB_MODE        // 10.1.12 Enter data state
+CGAUTO,        atCmdHandleCGAUTO, AT_CON_NOT_CALIB_MODE        // 10.1.15 Automatic response to a network request for PDP context activation
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+CGQREQ,        atCmdHandleCGQREQ, AT_CON_NOT_CALIB_MODE        // 10.1.4 Quality of service profile (requested)
+CGANS,         atCmdHandleCGANS, AT_CON_NOT_CALIB_MODE         // 10.1.16 Manual response to a network request for PDP context activation
+CGCLASS,       atCmdHandleCGCLASS, AT_CON_NOT_CALIB_MODE       // 10.1.17 GPRS mobile station class
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+CGPDNSADDR,    atCmdHandleCGPDNSADDR, AT_CON_NOT_CALIB_MODE
#endif
#if defined(LTE_NBIOT_SUPPORT)
+CRTDCP,        atCmdHandleCRTDCP, 0        // 10.1.44 Reporting of terminating data via the control plane +CRTDCP
#ifndef CONFIG_QUEC_PROJECT_FEATURE
#if defined(LTE_SUPPORT)
+QSCANF,        atCmdHandleQSCANF, 0        // use to freq scan
+QSCANFA,      atCmdHandleQSCANFA, 0        // use to freq scan all(LTE and GSM)
#endif
#ifdef CONFIG_SOC_6760
+SETCARRIERNUM, atCmdHandleCarrierNum, 0
+CFGDFTPDN,     atCmdHandleCFGDFTPDN, 0
+LOGRANK,       atCmdHandleLOGRANK, 0
+NVCFGARFCN,    atCmdHandleCfgArfcn, 0
+NVSETLOCKFREQ, atCmdHandleLockFreq, 0
#endif
#endif
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#ifdef LTE_SUPPORT
+SETLOCK,       atCmdHandleSETLOCK, AT_CON_NOT_CALIB_MODE
+SETSTSEN,      atCmdHandleSETSTSEN,   0
+SETRATEPRIOR,  atCmdHandleSETRATEPRIOR, 0
#endif

#ifdef CONFIG_CFW_DEBUG_IPFILTER
+IPFLT,         atCmdHandleIPFLT, 0         // Filter IP packets inside CFW. DEBUG ONLY!!
#endif

+QGPCLASS,      atCmdHandleQGPCLASS, 0
#endif

#ifdef CFW_VOLTE_SUPPORT
#ifndef CONFIG_QUEC_PROJECT_FEATURE
//+XCAPIP,        atCmdHandleXCAPIP, 0
+SETVOLTE,      atCmdHandleSETVOLTE, AT_CON_NOT_CALIB_MODE
#endif
+CIREG,         atCmdHandleCIREG, 0
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE
+DLST,          atCmdHandleDLST, AT_CON_NOT_CALIB_MODE
+ECSQ,          atCmdHandleECSQ, 0          // Enhanced signal quality report



// filesystem
#ifdef CONFIG_ATS_FS_SUPPORT
+FSDWNFILE,     atCmdHandleFSDWNFILE, 0     // Download file
+FSRDFILE,      atCmdHandleFSRDFILE, 0      // Read file
+FSRDBLOCK,     atCmdHandleFSRDBLOCK, 0     // Read file block
+FSDELFILE,     atCmdHandleFSDELFILE, 0     // Delete file
+FSMKDIR,       atCmdHandleFSMKDIR, 0       // Create directory
+FSRMDIR,       atCmdHandleFSRMDIR, 0       // Remove directory
+FSLSTFILE,     atCmdHandleFSLSTFILE, 0     // List file or directory
+FSLSTPART,     atCmdHandleFSLSTPART, 0     // List partition free space
+FSREMOUNT,     atCmdHandleFSREMOUNT, 0     // Remount as read-only or read-write
#endif

#ifdef CONFIG_AT_HTTP_SUPPORT
+HTTPINIT,      AT_CmdFunc_HTTPINIT, AT_CON_NOT_CALIB_MODE
+HTTPTERM,      AT_CmdFunc_HTTPTERM, AT_CON_NOT_CALIB_MODE
+HTTPPARA,      AT_CmdFunc_HTTPPARA, AT_CON_NOT_CALIB_MODE
+HTTPREAD,      AT_CmdFunc_HTTPREAD, AT_CON_NOT_CALIB_MODE
+HTTPDATA,      AT_CmdFunc_HTTPDATA, AT_CON_NOT_CALIB_MODE
+HTTPACTION,    AT_CmdFunc_HTTPACTION, AT_CON_NOT_CALIB_MODE
+HTTPSTATUS,    AT_CmdFunc_HTTPSTATUS, AT_CON_NOT_CALIB_MODE
+HTTPSSETCRT,   AT_CmdFunc_HTTPSSETCRT, AT_CON_NOT_CALIB_MODE
#endif

+SNTP,          AT_CmdFunc_SNTP, 0

#if defined(CONFIG_AT_FTP_SUPPORT) || defined(CONFIG_AT_NEWFTP_SUPPORT)
^FTPOPEN,       AT_FTP_CmdFunc_OPEN, AT_CON_NOT_CALIB_MODE
^FTPCLOSE,      AT_FTP_CmdFunc_CLOSE, AT_CON_NOT_CALIB_MODE
^FTPSIZE,       AT_FTP_CmdFunc_SIZE, AT_CON_NOT_CALIB_MODE
^FTPGET,        AT_FTP_CmdFunc_GET, AT_CON_NOT_CALIB_MODE
^FTPGETSET,     AT_FTP_CmdFunc_GETSET, AT_CON_NOT_CALIB_MODE
^FTPPUT,        AT_FTP_CmdFunc_PUT, AT_CON_NOT_CALIB_MODE
^FTPPUTSET,     AT_FTP_CmdFunc_PUTSET, AT_CON_NOT_CALIB_MODE
#endif

#ifdef CONFIG_AT_COAP_SUPPORT
^COAPGET,       AT_COAP_CmdFunc_GET, AT_CON_NOT_CALIB_MODE
^COAPPUT,       AT_COAP_CmdFunc_PUT, AT_CON_NOT_CALIB_MODE
^COAPPOST,      AT_COAP_CmdFunc_POST, AT_CON_NOT_CALIB_MODE
^COAPDELETE,    AT_COAP_CmdFunc_DELETE, AT_CON_NOT_CALIB_MODE
^COAPDATA,      AT_COAP_CmdFunc_DATA, AT_CON_NOT_CALIB_MODE
^COAPREG,       AT_COAP_CmdFunc_DATAREG, AT_CON_NOT_CALIB_MODE
#endif

#ifdef CONFIG_AT_PAHO_MQTT_SUPPORT
+MQTTCONN,      AT_GPRS_CmdFunc_MQTTCONN, 0
+MQTTDISCONN,   AT_GPRS_CmdFunc_MQTTDISCONN, 0
+MQTTSUBUNSUB,  AT_GPRS_CmdFunc_MQTT_SUB_UNSUB, 0
+MQTTPUB,       AT_GPRS_CmdFunc_MQTTPUB, 0
#endif

#ifdef CONFIG_AT_ALIC_SUPPORT
+ALICAUTH,            AT_ALIC_CmdFunc_AUTH, 0
+ALICCONN,            AT_ALIC_CmdFunc_CONN, 0
+ALICPUB,              AT_ALIC_CmdFunc_PUB, 0
+ALICSUB,              AT_ALIC_CmdFunc_SUB, 0
+ALICUNSUB,          AT_ALIC_CmdFunc_UNSUB, 0
+ALICDISCONN,       AT_ALIC_CmdFunc_DISCONN, 0
#endif

#ifdef CONFIG_SOC_6760
+SIGMOD,        atCmdHandleSIGMOD, 0        // signal mode
+DLPARAM,       atCmdHandleDLPARAM, 0       // downlink param
+DLRCV,         atCmdHandleDLRCV, 0         // downlink recieve enable
+ULPARAM,       atCmdHandleULPARAM, 0       // uplink param
+ULRCV,         atCmdHandleULRCV, 0         // uplink recieve enable
+APC,           atCmdHandleAPC, 0           // APC
+AGC,           atCmdHandleAGC, 0           // AGC
+SETLIMIT,      atCmdHandleSETLIMIT, 0      // set temperature upper limit
+NWSTATUS,      atCmdHandleNWSTATUS, 0      // set network status ind
#endif

// debug
^SWJTAG,        atCmdHandleSWJTAG, 0        // Switch JTAG pin mux
^PMSTART,       atCmdHandlePMSTART, 0       // Start PM core
^PMSTOP,        atCmdHandlePMSTOP, 0        // Stop PM core
^FORCEDNLD,     atCmdHandleFORCEDNLD, 0     // Reset to force download mode
+SPREF,         atCmdHandleSPREF, 0         // Reset to force download mode by Download tools
#endif
#ifndef CONFIG_SOC_6760
#ifndef CONFIG_QUEC_PROJECT_FEATURE
^TRACECTRL,     atCmdHandleTrace, AT_CON_NOT_CALIB_MODE         // Enable/disable trace
#endif
+SYSNV,         atCmdHandleSYSNV, 0         // read/write sysnv
#endif
^SSIT,          atCmdHandleSSIT, AT_CON_NOT_CALIB_MODE
#ifndef CONFIG_QUEC_PROJECT_FEATURE
#ifndef CONFIG_SOC_6760
^HEAPINFO,      atCmdHandleHEAPINFO, 0      // Show heap info
^BLKDEVINFO,    atCmdHandleBLKDEVINFO, 0    // Show block device info
#endif
^TIMEOUTABORT,  atCmdHandleTIMEOUTABORT, 0  // Trivial command to test timeout and abort
^UPTIME,        atCmdHandleUpTime, 0        // Get up time
+UPTIME,        atCmdHandleUpTimePlus, 0    // Get up time
+SPENGMD,       atCmdHandleSPENGMD, 0       // Only support assert system
#ifdef CONFIG_SOC_8910
+SET7SRESET,    atCmdHandleSet7sReset, 0    // set powerkey 7s reset(1:true 0:false)
+USBSWITCH,     atCmdHandleUSBSWITCH, 0     // enable/disable usb function
+USBRMTWK,      atCmdHandleUSBRMTWK, 0      // usb remote wakeup
#endif
#ifdef CONFIG_TWOLINE_WAKEUP_ENABLE
+SLEEPDELAY,    atCmdHandleMcuNotifySleep, 0,    //mcu notify module should into deepsleep
#endif

#ifdef CONFIG_SRV_SIMLOCK_ENABLE
^SPHUK,          atCmdHandleSimlockSPHUK, 0    //save the random number "HUK"
^SPFACAUTHPUBKEY, atCmdHandleSimlockSPFACAUTHPUBKEY, 0    //check and save the RSA public key
^SPIDENTIFYSTART, atCmdHandleSimlockSPIDENTIFYSTART, 0    //start authentication
^SPIDENTIFYEND, atCmdHandleSimlockSPIDENTIFYEND, 0    //end authentication
^SPDATAENCRYPT, atCmdHandleSimlockSPDATAENCRYPT, 0    //encrypt the original simlock keys
^SPSIMLOCKDATAWRITE, atCmdHandleSimlockSimlockData, 0    //update the simlock data
^SPSIMLOCKIMEI, atCmdHandleSimlockSPSIMLOCKIMEI, 0    // no handle this cmd just return ok.
#endif

#ifdef CONFIG_AT_EMMCDDRSIZE_SUPPORT
+EMMCDDRSIZE,     atCmdHandleEmmcDDRSize, 0    // read flash size and exram size
#endif
#ifdef CONFIG_ATR_CMUX_SUPPORT
+CMUXEND,       atCmdHandleCMUXEND, AT_CON_NOT_CLAC     // exit cmux mode forcedly
#endif
#endif

#ifdef CONFIG_AT_AUDIO_SUPPORT
#ifndef CONFIG_QUEC_PROJECT_FEATURE    //adjust by kevin 20201128, as quec at command have at+crsl, at+clvl, but param is not same to it 
+CRSL,          atCmdHandleCRSL, AT_CON_NOT_CALIB_MODE          // 8.21 Ringer sound level
+CLVL,          atCmdHandleCLVL, AT_CON_NOT_CALIB_MODE          // 8.23 Loudspeaker volume level
+CMUT,          atCmdHandleCMUT, AT_CON_NOT_CALIB_MODE          // 8.24 Mute control
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+CAUDPLAY,      atCmdHandleCAUDPLAY, AT_CON_NOT_CALIB_MODE      // Play/stop/pause/resume audio file
+CAUDREC,       atCmdHandleCAUDREC, AT_CON_NOT_CALIB_MODE       // Voice call recording
+CDTMF,         atCmdHandleCDTMF, AT_CON_NOT_CALIB_MODE         // Local DTMF tone
+LBTEST,        atCmdHandleLBTEST, AT_CON_NOT_CALIB_MODE        // Loopback test
#ifdef CONFIG_SOC_8910
+MAI2SY,        atCmdHandleMAI2SY, AT_CON_NOT_CALIB_MODE        // Config external codec mode
+PCMMODE,       atCmdHandlePCMMODE, 0       // Set PCM frame synchronization singal mode
+CALM,          atCmdHandleCALM, 0          // Control playtone of call and sms
#endif
#endif
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#ifdef CONFIG_TTS_SUPPORT
+CTTS,          atCmdHandleCTTS, 0          // TTS
#endif
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#ifdef CONFIG_SOC_8910
//+TSTSETCS,      atCmdHandleTSTSETCS, AT_CON_NOT_CALIB_MODE      // Just use to test close CS domain
+SIMLOCKTEST,      atCmdHandleSimlockTest, 0      // Just use to test close CS domain
#endif

//command for lwm2m
#ifdef CONFIG_AT_CISSDK_MIPL_SUPPORT
+MIPLCREATE,        AT_CmdFunc_MIPLCREATE, AT_CON_NOT_CALIB_MODE
+MIPLDELETE,        AT_CmdFunc_MIPLDELETE, AT_CON_NOT_CALIB_MODE
+MIPLOPEN,          AT_CmdFunc_MIPLOPEN, AT_CON_NOT_CALIB_MODE
+MIPLCLOSE,         AT_CmdFunc_MIPLCLOSE, AT_CON_NOT_CALIB_MODE
+MIPLADDOBJ,        AT_CmdFunc_MIPLADDOBJ, AT_CON_NOT_CALIB_MODE
+MIPLDELOBJ,        AT_CmdFunc_MIPLDELOBJ, AT_CON_NOT_CALIB_MODE
+MIPLNOTIFY,        AT_CmdFunc_MIPLNOTIFY, AT_CON_NOT_CALIB_MODE
+MIPLREADRSP,       AT_CmdFunc_MIPLREADRSP, AT_CON_NOT_CALIB_MODE
+MIPLWRITERSP,      AT_CmdFunc_MIPLWRITERSP, AT_CON_NOT_CALIB_MODE
+MIPLEXECUTERSP,    AT_CmdFunc_MIPLEXECUTERSP, AT_CON_NOT_CALIB_MODE
+MIPLOBSERVERSP,    AT_CmdFunc_MIPLOBSERVERSP, AT_CON_NOT_CALIB_MODE
+MIPLDISCOVERRSP,   AT_CmdFunc_MIPLDISCOVERRSP, AT_CON_NOT_CALIB_MODE
+MIPLPARAMETERRSP,  AT_CmdFunc_MIPLPARAMETERRSP, AT_CON_NOT_CALIB_MODE
+MIPLUPDATE,        AT_CmdFunc_MIPLUPDATE, AT_CON_NOT_CALIB_MODE
+MIPLVER,           AT_CmdFunc_MIPLVER, 0
+MIPLCLEARSTATE,    AT_CmdFunc_MIPLCLEARSTATE, 0
#endif

#ifdef CONFIG_AT_OCEANCONNECT_SUPPORT
+NCDPOPEN,    AT_CmdFunc_NCDPOPEN, 0
+NCDPCLOSE,   AT_CmdFunc_NCDPCLOSE, 0
+NMGS,        AT_CmdFunc_NMGS, 0
+NMGR,        AT_CmdFunc_NMGR, 0
+NNMI,        AT_CmdFunc_NNMI, 0
#endif
#ifdef CONFIG_ATS_UPDATE_SUPPORT
+UPDATE,            atCmdHandleUPDATE, 0            // Firmware update, data from AT
#endif

#if defined(CONFIG_SOC_8909) || defined(CONFIG_SOC_8910)
+CFGCIOT,           atCmdHandleCFGCIOT, AT_CON_NOT_CALIB_MODE           // NB static NVM
#endif
#endif

#ifdef CONFIG_SOC_8910
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+SPCLEANINFO,       atCmdHandleSPCLEANINFO, AT_CON_NOT_CALIB_MODE       // This command is used to clean history ba into for all rat.
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+L1PARAM,           atCmdHandleL1PARAM, AT_CON_NOT_CALIB_MODE           // This command is used to set L1 dual-mode parameters of GSM.
#endif
+GRRLTEFREQ,        atCmdHandleGRRLTEFREQ, AT_CON_NOT_CALIB_MODE        // Set dummy FDD/TDD LTE freq configuration for GSM.
+SPLTEDUMMYPARA,    atCmdHandleSPLTEDUMMYPARA, AT_CON_NOT_CALIB_MODE    // This command is used to dual-mode LTE parameters for LTE.
+RRTMPARAM,         atCmdHandleRRTMPARAM, AT_CON_NOT_CALIB_MODE         // This command is used to dual-mode LTE parameters for GRR.
+CFGDFTPDNMODE,         atCmdHandleCFGDFTPDNMODE, 0
+VERCTRL,           atCmdHandleVERCTRL, 0
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+CFGDFTPDN,           atCmdHandleCFGDFTPDN, 0
#endif
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#ifdef CONFIG_AT_SSIM_SUPPORT
+SSIMAUTH,          atCmdHandleSSIMAUTH, 0          // begin soft sim authentication
+SSIMAUTHEXIT,      atCmdHandleSSIMAUTHEXIT, 0      // exit soft sim authentication
+SSIMCHANGEPW,      atCmdHandleSSIMCHANGEPW, 0      // change authentication password
+SSIMINFO,          atCmdHandleSSIMINFO, 0          // set and query soft sim info
+SSIMPLMN,          atCmdHandleSSIMPLMN, 0          // set and query sim plmn
+SSIMFILEHASH,      atCmdHandleSSIMFILEHASH, 0      // get soft sim file hash value
+SSIMMSIN,          atCmdHandleSSIMMSIN, 0          // set and get the high 3 bit of the MSIN
+SSIMDELFILE,       atCmdHandleSSIMDELFILE, 0       // delete file
#endif

#ifdef CONFIG_AT_DM_LWM2M_SUPPORT
+SELFREGISTER,      atCmdHandleSELFREGISTER, 0      // self-register flag
+SETDMADPPLATFORM,  atCmdHandleSETDMADPPLATFORM, 0      // set platform address
#endif

#if defined(CONFIG_AT_MYFTP_SUPPORT)
$MYFTPOPEN,     AT_GPRS_CmdFunc_MYFTPOPEN, 0
$MYFTPCLOSE,    AT_GPRS_CmdFunc_MYFTPCLOSE, 0
$MYFTPSIZE,     AT_GPRS_CmdFunc_MYFTPSIZE, 0
$MYFTPGET,      AT_GPRS_CmdFunc_MYFTPGET, 0
$MYFTPPUT,      AT_GPRS_CmdFunc_MYFTPPUT, 0
#elif defined(CONFIG_AT_NEWMYFTP_SUPPORT)
$MYFTPOPEN,     AT_MYFTP_CmdFunc_OPEN, 0
$MYFTPCLOSE,    AT_MYFTP_CmdFunc_CLOSE, 0
$MYFTPSIZE,     AT_MYFTP_CmdFunc_SIZE, 0
$MYFTPGET,      AT_MYFTP_CmdFunc_GET, 0
$MYFTPPUT,      AT_MYFTP_CmdFunc_PUT, 0
#endif

#if defined(CONFIG_AT_MYNET_TCPIP_SUPPORT)
$MYNETSRV,      AT_TCPIP_CmdFunc_MYNETSRV, AT_CON_NOT_CALIB_MODE
$MYNETOPEN,     AT_TCPIP_CmdFunc_MYNETOPEN, AT_CON_NOT_CALIB_MODE
$MYNETREAD,     AT_TCPIP_CmdFunc_MYNETREAD, AT_CON_NOT_CALIB_MODE
$MYNETWRITE,    AT_TCPIP_CmdFunc_MYNETSEND, AT_CON_NOT_CALIB_MODE
$MYNETCLOSE,    AT_TCPIP_CmdFunc_MYNETCLOSE, AT_CON_NOT_CALIB_MODE
$MYNETACK,      AT_TCPIP_CmdFunc_MYNETACK, AT_CON_NOT_CALIB_MODE
$MYNETACCEPT,   AT_TCPIP_CmdFunc_MYNETACCEPT, AT_CON_NOT_CALIB_MODE
$MYNETCREATE,   AT_TCPIP_CmdFunc_MYNETCREATE, AT_CON_NOT_CALIB_MODE
$MYNETCON,      AT_TCPIP_CmdFunc_MYNETCON, AT_CON_NOT_CALIB_MODE
$MYNETACT,      AT_GPRS_CmdFunc_MYNETACT, AT_CON_NOT_CALIB_MODE
$MYIPFILTER,    AT_TCPIP_CmdFunc_MYIPFILTER, AT_CON_NOT_CALIB_MODE
$MYNETURC,      AT_TCPIP_CmdFunc_MYNETURC, 0
$MYSOCKETLED,   AT_TCPIP_CmdFunc_MYSOCKETLED, 0
#endif

#if defined(CONFIG_SOC_6760) || (defined(CONFIG_ATS_SGCC_CATM_SUPPORT) && defined(CONFIG_SOC_8910))
$MYGETKEY,      AT_TCPIP_CmdFunc_MYGETKEY, 0
$MYSETINFO,     AT_TCPIP_CmdFunc_MYSETINFO, 0
$MYTYPE,        AT_TCPIP_CmdFunc_MYTYPE, 0
$MYCALIB,       AT_TCPIP_CmdFunc_MYCALIB, 0
$MYSYSINFO,     atCmdHandleMYSYSINFO, AT_CON_NOT_CALIB_MODE
$MYSYSMODE,     atCmdHandleMYSYSMODE, 0
#endif
#endif
// NV
+NVPC,          atCmdHandleNVPC, AT_CON_NOT_CALIB_MODE
+NVGV,          atCmdHandleNVGV, AT_CON_NOT_CALIB_MODE
+RFCALIB,       atCmdHandleRFCALIB, AT_CON_NOT_CALIB_MODE

#ifdef CONFIG_SOC_8910
#ifndef CONFIG_QUEC_PROJECT_FEATURE
#ifndef CONFIG_LTE_ONLY_SUPPORT
$MYBCCH,        atCmdHandleMYBCCH, AT_CON_NOT_CALIB_MODE
$MYBAND,        atCmdHandleMYBAND, AT_CON_NOT_CALIB_MODE
#endif
$MYCCID,        atCmdHandleCCID, AT_CON_NOT_CALIB_MODE
#ifdef CONFIG_AT_MYINFO_SUPPORT
$MYPOWEROFF,    AT_TCPIP_CmdFunc_MYPOWEROFF, 0
#endif
$MYGMR,         atCmdHandleGMR, 0     // (6.1.6) Request revision identification
#endif
+SETTRIBAND,   atCmdHandleSETTRIBAND, AT_CON_NOT_CALIB_MODE
#ifndef CONFIG_LTE_ONLY_SUPPORT
+LOCKBCCH,      atCmdHandleMYBCCH, AT_CON_NOT_CALIB_MODE
+SETBAND,       atCmdHandleMYBAND, AT_CON_NOT_CALIB_MODE
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE
+SETDTPORT,     atCmdHandleSETDTPORT, AT_CON_NOT_CALIB_MODE
+RFTEMPERATURE, atCmdHandleRFTEMPERATURE, AT_CON_NOT_CALIB_MODE
+SETLTEFRP,     atCmdHandleSETLTEFRP, 0
+SETRFFRP,      atCmdHandleSETRFFRP, 0
#ifdef CONFIG_AT_AUDIO_SUPPORT
+SETVOS,        atCmdHandleSETVOS, 0
#endif
+SPLMN,    atCmdHandleSPLMN, AT_CON_NOT_CALIB_MODE
#endif
+BANDINFO,       atCmdHandleBANDINFO, 0
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#ifdef CONFIG_AT_BT_APP_SUPPORT
+BTAPP,         atCmdHandle_BTAPP, 0 //BT APP handle, used to running BT Demo
#endif
#endif
#ifdef CONFIG_SOC_8910
+SPBTCTRL,      atCmdHandleSPBTCTRL, 0      // BT control commands
#endif


#ifdef CONFIG_AT_BLUEU_VERIFY_SUPPORT
+SPBTTEST,      atCmdHandle_SPBTTEST, 0
+SPBLETEST,     atCmdHandle_SPBLETEST, 0
#ifndef CONFIG_QUEC_PROJECT_FEATURE_ATC_PARSE
+SPBQBTEST,     atCmdHandle_SPBQBTEST, 0
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE
+BTCOMM,        atCmdHandle_BTCOMM, 0
+BTHF,          atCmdHandle_BTHF,  0
+BTAG,          atCmdHandle_BTAG,   0
+BTSINK,        atCmdHandle_BTSINK, 0
+BTSRC,         atCmdHandle_BTSRC,  0
+BTSPP,         atCmdHandle_BTSPP, 0

+BLECOMM,       atCmdHandle_BLECOMM, 0
+BLEADV,        atCmdHandle_BLEADV, 0
+BLESCAN,       atCmdHandle_BLESCAN, 0
+BLESMP,        atCmdHandle_BLESMP, 0
+BLEGATTSRV,    atCmdHandle_BLEGATTSRV,0
+BLEGATTCLI,    atCmdHandle_BLEGATTCLI,0
#endif
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#ifdef CONFIG_AT_BT_CLASSIC_SUPPORT
+BTONOFF,       AT_BTApp_CmdFunc_ONOFF, 0
+BTVISIBLE,     AT_BTApp_CmdFunc_VISIBLE, 0
+BTNAME,        AT_BTApp_CmdFunc_NAME, 0
+BTADDR,        AT_BTApp_CmdFunc_ADDR, 0
+BTINQ,         AT_BTApp_CmdFunc_INQ, 0
+BTPAIR,        AT_BTApp_CmdFunc_PAIR, 0
+BTPIN,         AT_BTApp_CmdFunc_PIN, 0
+BTLISTPD,      AT_BTApp_CmdFunc_LISTPD, 0
+BTREMOVEPD,    AT_BTApp_CmdFunc_REMOVEPD, 0
+BTCONNECT,     AT_BTApp_CmdFunc_CONNECT, 0
+BTSEND,        AT_BTApp_CmdFunc_SEND, 0
+BTREAD,        AT_BTApp_CmdFunc_READ, 0
#endif
#endif

#ifdef CONFIG_AT_WIFISCAN_SUPPORT
+WIFION,      atCmdHandleWifiOpen,   0
+WIFIOFF,     atCmdHandleWifiClose,  0
+WIFISCAN,      atCmdHandleWifiScan,   0
#ifdef CONFIG_AT_WIFI_SENSITIVITY_TEST_SUPPORT
+WIFISENTEST,   atCmdHandleWifiSensitivityTest, 0
#endif
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE
+SIMHOTSWAP,    atCmdHandleSIMHOTSWAP, 0      //set sim hot-plug function
+GTSET,         atCmdHandleGTSET,      0      //set sim hot-plut trigger mode
+NETMSG,        atCmdHandleNETMSG,     0      //Get netinfo
+GTCCINFO,      atCmdHandleGTCCINFO,   0      //inqury GSM/LTE service cell and neighbor cell
#endif

#ifdef CONFIG_SOC_8910
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
+NCESIM,        atCmdHandleNCESIM, 0          // close or open network card authentication
#ifndef CONFIG_QUEC_PROJECT_FEATURE_NW
+NSTCFG,        atCmdHandleNSTCFG, 0
+NST,           atCmdHandleNST, 0
+SETCSPAGFLAG,  atCmdHandleSETCSPAGFLAG, 0    // Set flag of ingnoring CS paging
#endif
+CSVM,          atCmdHandleCSVM, 0
+CSTA,          atCmdHandleCSTA, 0
+T3302,         atCmdHandleT3302, 0
+CSSN,          atCmdHandleCSSN, 0            // Enables/disables the presentation of notification result codes from TA to TE.
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE_NW
#ifdef CONFIG_AT_AUDIO_SUPPORT
+MMICG,         atCmdHandleMMICG, 0
#endif
+LOCREL,        atCmdHandleLOCREL, 0
#endif
+DRXTM,         atCmdHandleDRXTM, 0
+LCT,           atCmdHandleLCT, 0
#ifndef CONFIG_QUEC_PROJECT_FEATURE_NW
+EFCI,           atCmdHandleEFCI, 0            //Set electric fence cell info.
#endif
+CFGNVUPFLAG,   atCmdHandleCFGNVUPFLAG, AT_CON_NOT_CALIB_MODE
#ifndef CONFIG_QUEC_PROJECT_FEATURE_NW
+QENG,          atCmdHandleQENG, AT_CON_NOT_CALIB_MODE // Get cell info
+COPST,  atCmdHandleCOPST, 0
#endif
#endif

+COCSIM,      atCmdHandleCOCSIM, 0

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#ifdef CONFIG_LPA_SUPPORT
+LPA,       atCmdHandleLPA, 0
#endif

#ifdef CONFIG_AT_U_CREATE_AT_CHANNEL
+UCREATEATCH, atCmdUCreateAtChannel, 0
#endif

+SETSIGQUA,      atCmdHandleSETSIGQUA,   AT_CON_NOT_CALIB_MODE

#ifdef CONFIG_WDT_ENABLE
+UWDTCTL,   atCmdUWdtCtlHandle, 0
#endif

#ifdef CONFIG_DUAL_SIM_SUPPORT
+SETSIM,        atCmdHandleSETSIM,0
#endif

+PPM,    atCmdHandleSyncPPM, 0
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#if defined(CONFIG_AUDIO_SET_LOOPBACK_GAIN_SUPPORT) && defined(CONFIG_SOC_8910)
+AUDLBGAIN,        atCmdHandleAUDLBGAIN,0
#endif

+FSEMMCERASE,    atCmdHandleFSEMMCERASE,0

+MOSRC,   atCmdHandleMOSRC, 0 // MO SMS RETRY COUNT
#endif

#endif
#endif
