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
#include "ql_api_sim.h"
#include "ql_pbk_demo.h"

#define QL_PBK_LOG_LEVEL           QL_LOG_LEVEL_INFO
#define QL_PBK_LOG(msg, ...)       QL_LOG(QL_PBK_LOG_LEVEL, "ql_pbk_demo", msg, ##__VA_ARGS__)

#define QL_PBK_TASK_STACK_SIZE     4096
#define QL_PBK_TASK_PRIO           APP_PRIORITY_NORMAL
#define QL_PBK_TASK_EVENT_CNT      5

ql_task_t pbk_task = NULL;
ql_sem_t pbk_init_sem = NULL;

void user_pbk_event_callback(uint8_t nSim, int event_id, void *ctx)
{
    switch (event_id)
    {
    case QL_PBK_INIT_OK_IND: {
        QL_PBK_LOG("QL_PBK_INIT_OK_IND sim:%d", nSim);
        if (0 == nSim)
        {
            ql_rtos_semaphore_release(pbk_init_sem);
        }
        break;
    }
    default:
        break;
    }
}

static void ql_pbk_demo_thread(void *param)
{
    QlOSStatus err = QL_OSI_SUCCESS;
    ql_sim_errcode_e ret = QL_SIM_SUCCESS;
    uint8_t sim_id = 0;
    int start_index = 1;
    int end_index = 20;
    uint8_t *username = NULL;
    uint8_t username_len = 0;
    ql_sim_pbk_item_info_s item = {0};
    ql_sim_pbk_itemset_info_s *itemset = NULL;

    itemset = (ql_sim_pbk_itemset_info_s *)malloc(sizeof(ql_sim_pbk_itemset_info_s));
    if (itemset == NULL)
    {
        QL_PBK_LOG("malloc itemset fail");
        goto exit;
    }

    ql_pbk_callback_register(user_pbk_event_callback);
    ql_sim_set_pbk_encoding(QL_SIM_PBK_GSM);
    
    //wait pbk ok
    if (ql_rtos_semaphore_wait(pbk_init_sem, QL_WAIT_FOREVER))
    {
        QL_PBK_LOG("Waiting for PBK init timeout");
    }

    //add
    item.index = 1;
    strcpy((char *)item.username, "quectel");
    strcpy((char *)item.phonenum, "+0123456789");
    item.username_len = strlen((const char *)item.username);

    ret = ql_sim_write_pbk_item(sim_id, QL_SIM_PBK_STORAGE_SM, &item);
    QL_PBK_LOG("add item ret:0x%x", ret);

    //read
    start_index = 1;
    end_index = QL_PBK_ITEM_COUNT_MAX * 2; //You can actually read QL_PBK_ITEM_COUNT_MAX items
    memset((void *)itemset, 0, sizeof(ql_sim_pbk_itemset_info_s));

    ret = ql_sim_read_pbk_item(sim_id, QL_SIM_PBK_STORAGE_SM, start_index, end_index, username, username_len, itemset);
    QL_PBK_LOG("read item ret:0x%x", ret);
    if (QL_SIM_SUCCESS == ret)
    {
        QL_PBK_LOG("read item count:%d", itemset->item_count);
        for (size_t i = 0; i < itemset->item_count; i++)
        {
            QL_PBK_LOG("read item index:%-3d phonenum:%-24.24s username:%-32.32s namelen:%d",
                       itemset->item[i].index,
                       itemset->item[i].phonenum,
                       itemset->item[i].username,
                       itemset->item[i].username_len);
        }
    }

    //search
    start_index = 0;
    username = (uint8_t *)"quec";
    username_len = strlen((char const *)username);
    memset((void *)itemset, 0, sizeof(ql_sim_pbk_itemset_info_s));

    ret = ql_sim_read_pbk_item(sim_id, QL_SIM_PBK_STORAGE_SM, start_index, end_index, username, username_len, itemset);
    QL_PBK_LOG("search item ret:0x%x", ret);
    if (QL_SIM_SUCCESS == ret)
    {
        QL_PBK_LOG("search item count:%d", itemset->item_count);
        for (size_t i = 0; i < itemset->item_count; i++)
        {
            QL_PBK_LOG("search item index:%-3d phonenum:%-24.24s username:%-32.32s namelen:%d",
                       itemset->item[i].index,
                       itemset->item[i].phonenum,
                       itemset->item[i].username,
                       itemset->item[i].username_len);
        }
    }

    //delete
    item.index = 1;
    memset(item.phonenum, 0, sizeof(item.phonenum) / sizeof(item.phonenum[0]));

    ret = ql_sim_write_pbk_item(sim_id, QL_SIM_PBK_STORAGE_SM, &item);
    QL_PBK_LOG("delete item ret:0x%x", ret);

    free(itemset);

exit:
    err = ql_rtos_task_delete(NULL);
    if (err != QL_OSI_SUCCESS)
    {
        QL_PBK_LOG("task deleted failed");
    }
}

void ql_pbk_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;

    err = ql_rtos_semaphore_create(&pbk_init_sem, 0);
    if (err != QL_OSI_SUCCESS)
    {
        QL_PBK_LOG("pbk_init_sem created failed, ret = 0x%x", err);
    }

    err = ql_rtos_task_create(&pbk_task, QL_PBK_TASK_STACK_SIZE, QL_PBK_TASK_PRIO, "QPBKDEMO", ql_pbk_demo_thread, NULL, QL_PBK_TASK_EVENT_CNT);
    if (err != QL_OSI_SUCCESS)
    {
        QL_PBK_LOG("task created failed");
    }
}
