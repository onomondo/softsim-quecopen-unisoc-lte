/**  @file
  ql_atcmd_handler.c

  @brief
  This file is used to define at command callback functions for different Quectel OPEN SDK Project.

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


/*===========================================================================
 * include files
============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "osi_mem.h"
#include "at_engine.h"
#include "at_command.h"
#include "at_response.h"
#include "atr_config.h"
#include "ql_app_feature_config.h"
#include "ql_log.h"
#include "quec_at_engine.h"
#include "quec_proj_config_at.h"



/*************************************************************************
*  							Macro
**************************************************************************/
#define ql_at_handler_log(msg, ...)  QL_LOG(QL_LOG_LEVEL_INFO, "QATHDLR", msg, ##__VA_ARGS__)


/*************************************************************************
*  							Enum
**************************************************************************/


/*************************************************************************
*  							Struct
**************************************************************************/


/*************************************************************************
*	                        Variable
**************************************************************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CSDK_VERINFO_AT
const char ql_ver_date[] = __DATE__;
const char ql_ver_time[] = __TIME__;
const char ql_ver_authors[] = "Authors: Onomondo";
#endif

/*************************************************************************
*	                        Function
**************************************************************************/
#ifndef QL_CORE_VERSION
#define QL_CORE_VERSION "QuecOPENSDK"
#endif

void ql_exec_ver_cmd(atCommand_t *cmd)
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CSDK_VERINFO_AT
    char *verInfo = NULL;
	int verinfo_len;

	verinfo_len = strlen(QL_CORE_VERSION)+strlen(ql_ver_date)+strlen(ql_ver_time)+strlen(ql_ver_authors)+20;
	verInfo = (char *)malloc(verinfo_len);
	if (NULL == verInfo)
	{
		quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_ERROR, CMD_RC_ERROR);
		return;
	}

	memset(verInfo, 0, verinfo_len);
    /* get ver information */
    sprintf(verInfo, "VERSION: %s\r\n%s %s\r\n%s", QL_CORE_VERSION, ql_ver_date, ql_ver_time, ql_ver_authors);

    /*return the result code and response to the terminal */
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK, verInfo, 1);

	free(verInfo);
#else
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_ERROR, CMD_RC_ERROR, NULL, 0);
#endif
	return;
}

void ql_exec_atdemo_cmd(atCommand_t *cmd)
{
	if (cmd->type == AT_CMD_SET)
    {
        //how to extract parameter, please refer to at_param.h

        //eg:
        //atParamUintInList: to extract uint parameter, and check in list
        //atParamUintInRange: to extract uint parameter, and check range
        //atParamStr: to extract string parameter
        //atParamInt: to extract int parameter
        //atParamDefInt: to extract optional int parameter with default value
        //......
        // =========================  parameter extract demo  ==============================
        //  bool paramok = true;
        //  char *data_str = atParamStr(cmd->params[0], &paramok);
        //  unsigned short major = atParamUintInRange(cmd->params[1], 0, 65535, &paramok);
        //  int temp_val =atParamInt(cmd->params[2],&paramok);
        //  int scheme = atParamDefInt(cmd->params[3], 0x04, &paramok);
        //  const uint32_t list[] = {4800, 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
        //  uint32_t baud = atParamUintInList(cmd->params[4], list, sizeof(list) / sizeof(list[0]), &paramok);
        //  ......
        // =================================================================================
        quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
    }
    else if (cmd->type == AT_CMD_READ)
    {
        quec_atResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK, "atdemo read resp", 1);
    }
    else if (cmd->type == AT_CMD_TEST)
    {
        quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
    }
    else
    {
        quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_OPTION_NOT_SURPORT);
    }
}

extern void ql_download_handler(atCommand_t *cmd);
void atCmdHandleFORCEDNLD(atCommand_t *cmd)
{
    //If you do not want to use the kernel logic, you can comment it out,
    //and then refer to the following content to implement your own logic
    ql_download_handler(cmd);

    // if (cmd->type == AT_CMD_EXE)
    // {
    //     quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
    //     osiShutdown(OSI_SHUTDOWN_FORCE_DOWNLOAD);
    // }
    // else if (cmd->type == AT_CMD_SET)
    // {
    //     quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
    //     osiShutdown(OSI_SHUTDOWN_FORCE_DOWNLOAD);
    // }
    // else
    // {
    //     quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_OPERATION_NOT_SUPPORTED);
    // }
}
