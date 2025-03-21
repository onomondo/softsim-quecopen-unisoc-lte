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
#include <stdint.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_api_esim.h"

	
#define QL_ESIM_LOG_LEVEL	                QL_LOG_LEVEL_INFO
#define QL_ESIM_DEMO_LOG(msg, ...)			QL_LOG(QL_ESIM_LOG_LEVEL, "ql_esim_demo", msg, ##__VA_ARGS__)

ql_task_t esim_task = NULL;

void esim_event_callback(uint8_t sim, ql_esim_command_rsp_e event_id, ql_esim_result_errno_e result,void *contextPtr)
{
    ql_event_t esim_event = {0};
    void *ctx = NULL;

    QL_ESIM_DEMO_LOG("esim:%d event:0x%X,result:0x%X,contextPtr:0x%X",sim,event_id,result,contextPtr);    
    switch(event_id)
    {
        case QL_ESIM_LPA_GET_EID_RSP:
            if(result == QL_ESIM_OK)
            {
                ctx = (char *)calloc(1,ESIM_LPA_DATA_MAX);
                memcpy(ctx,(char *)contextPtr,strlen((char *)contextPtr));
                QL_ESIM_DEMO_LOG("eid:%s",ctx);
                
            }
        break;
        case QL_ESIM_LPA_PROFILE_LIST_RSP:
            if(result == QL_ESIM_OK)
            {
                ql_esim_profile_list_t *list_info = (ql_esim_profile_list_t *)calloc(1,sizeof(ql_esim_profile_list_t));
                memcpy(list_info,(ql_esim_profile_list_t *)contextPtr,sizeof(ql_esim_profile_list_t));
                
                for(int i=0;i<list_info->profile_num;i++)
                {
                    QL_ESIM_DEMO_LOG("sim:%d,profile[%d]: iccid:%s,status:%d,nickname:%s,provider:%s", 
                              sim,i,list_info->profile_list[i].iccid,list_info->profile_list[i].status,
                              list_info->profile_list[i].nickname,list_info->profile_list[i].provider);                
                }
                ctx = (void *)list_info;
            }
            break;
        case QL_ESIM_LPA_NICKNAME_RSP:
            if(result == QL_ESIM_OK)
            {
                QL_ESIM_DEMO_LOG("nickname success");
            }
        default:
            break;
        }
    esim_event.id = event_id;
    esim_event.param1 = sim;
    esim_event.param2 = result;
    esim_event.param3 = (uint32_t)ctx;
    ql_rtos_event_send(esim_task, &esim_event);
}


void esim_demo_task(void * param)
{
    QL_ESIM_DEMO_LOG("enter");
    int err = 0;
    ql_event_t esim_event = {0};
    uint8_t nSim = 0;

    //wait init ok;
    ql_esim_service_init(QL_ESIM_MODE_IPA);
    ql_rtos_task_sleep_s(5);
    ql_esim_register_cb(esim_event_callback);

    err = ql_esim_lpa_get_eid();
    QL_ESIM_DEMO_LOG("get eid,ret:%d",err);
    err = ql_esim_lpa_get_profiles_info(QL_ESIM_LIST_PROFILE_INFO);
    QL_ESIM_DEMO_LOG("get profiles info,ret:%d",err);

    while(1){
        ql_event_wait(&esim_event, QL_WAIT_FOREVER);
        QL_ESIM_DEMO_LOG("event id = 0x%x,result:0x%x",esim_event.id,esim_event.param2);
        nSim = esim_event.param1;
        switch(esim_event.id)
        {
            case QL_ESIM_LPA_GET_EID_RSP:
            {
                if(esim_event.param2 == QL_ESIM_OK)
                {
                    if(esim_event.param3)
                    {
                        QL_ESIM_DEMO_LOG("sim:%d,eid:%s",nSim,esim_event.param3);
                    }
                }
                break;
            }
            case QL_ESIM_LPA_PROFILE_LIST_RSP:
            {
                if(esim_event.param2 == QL_ESIM_OK)
                {
                    if(esim_event.param3)
                    {
                        ql_esim_profile_list_t *list_info = (ql_esim_profile_list_t *)esim_event.param3;
                        for(int i=0;i<list_info->profile_num;i++)
                        {
                            QL_ESIM_DEMO_LOG("sim:%d,profile[%d]: iccid:%s,status:%d,nickname:%s,provider:%s", 
                            nSim,i,list_info->profile_list[i].iccid,list_info->profile_list[i].status,
                            list_info->profile_list[i].nickname,list_info->profile_list[i].provider);
                        }
                        //set nickname
                        if(list_info->profile_num != 0)
                        {
                            char name[]="ESIM1";
                            err = ql_esim_lpa_set_nickname((uint8_t *)list_info->profile_list[0].iccid,(uint8_t *)name);
                            QL_ESIM_DEMO_LOG("set iccid nickname,ret:%d",err);
                        }
                    }
                }
                break;
            }
            case QL_ESIM_LPA_NICKNAME_RSP:
            {
                QL_ESIM_DEMO_LOG("nickname result:0x%x",esim_event.param2);
            }
            default: break;
        }
        if(esim_event.param3)
        {   
            free((void *)esim_event.param3);
        }
    }
    ql_rtos_task_delete(NULL);
}



QlOSStatus ql_esim_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;
	
	err = ql_rtos_task_create(&esim_task, 4096, APP_PRIORITY_NORMAL, "esim_task", esim_demo_task, NULL, 10);
	if(err != QL_OSI_SUCCESS)
	{
		QL_ESIM_DEMO_LOG("esim_demo_task created failed, ret = 0x%x", err);
	}
	
	return err;
}

