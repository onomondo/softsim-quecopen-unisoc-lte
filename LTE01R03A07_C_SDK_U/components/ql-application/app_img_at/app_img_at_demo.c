/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


/*===========================================================================
 * include files
 ===========================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "quec_at_engine.h"
#include "quec_at_param.h"
#include "quec_atresp.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_APP_IMG_AT_LOG_LEVEL                 QL_LOG_LEVEL_INFO
#define QL_APP_IMG_AT_LOG(msg, ...)             QL_LOG(QL_APP_IMG_AT_LOG_LEVEL, "ql_app_img_at", msg, ##__VA_ARGS__)
#define QL_APP_IMG_AT_LOG_PUSH(msg, ...)        QL_LOG_PUSH("ql_app_img_at", msg, ##__VA_ARGS__)
    


/*===========================================================================
 * Struct
 ===========================================================================*/

 
/*===========================================================================
 * Variate
 ===========================================================================*/

/*===========================================================================
 * Functions
 ===========================================================================*/
 void ql_exec_img_at_demo_cmd(atCommand_t *cmd)
{
	if (cmd->type == AT_CMD_SET)
    {
        //how to extract parameter, please refer to at_param.h
        
        //eg:
        //quec_atParamUintInList: to extract uint parameter, and check in list
        //quec_atParamUintInRange: to extract uint parameter, and check range
        //quec_atParamStr: to extract string parameter
        //quec_atParamInt: to extract int parameter
        //quec_atParamDefInt: to extract optional int parameter with default value
        //......
        // =========================  parameter extract demo  ==============================
        //  bool paramok = true;
        //  const char *data_str = quec_atParamStr(cmd->params[0], &paramok);
        //  unsigned short major = quec_atParamUintInRange(cmd->params[1], 0, 65535, &paramok);
        //  int temp_val =quec_atParamInt(cmd->params[2],&paramok);
        //  int scheme = quec_atParamDefInt(cmd->params[3], 0x04, &paramok);
        //  const uint32_t list[] = {4800, 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
        //  uint32_t baud = quec_atParamUintInList(cmd->params[4], list, sizeof(list) / sizeof(list[0]), &paramok);
        //  ......
        // =================================================================================
        bool paramok = true;
        const char *input_str = quec_atParamStr(cmd->params[0], &paramok);
        if ((cmd->param_count != 1) || !paramok)
        {
            quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_PARAM_INVALID);
        }
        quec_atCmdRespInfoNText(cmd->engine, input_str, strlen(input_str), 1);

        quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
    }
    else if (cmd->type == AT_CMD_READ)
    {
        quec_atResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK, "app img at demo read resp", 1);
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

ql_at_desc_t app_img_at_desc[] = {
    {"+IMGATDEMO", ql_exec_img_at_demo_cmd, 0},
    {NULL, NULL, 0}};

void ql_app_img_at_init(void)
{
    QL_APP_IMG_AT_LOG("add app img at");

    quec_app_at_add((const ql_at_desc_t *)app_img_at_desc, sizeof(app_img_at_desc)/sizeof(app_img_at_desc[0]));
}
