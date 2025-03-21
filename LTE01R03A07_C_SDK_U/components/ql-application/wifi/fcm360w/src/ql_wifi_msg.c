/**
  @file

  @brief

*/
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

/*===========================================================================
 * include files
 ===========================================================================*/
#include "ql_api_wifi.h"
#include "ql_api_osi.h"

#include "lwip/prot/ethernet.h"
#include "ql_wifi_adapter.h"
#include "ql_wifi_msg.h"
#include "ql_wifi_net.h"
#include "ql_wifi_app.h"
/*========================================================================
 *	Macro Definition
 *========================================================================*/
#define QL_WIFI_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_WIFI_MSG_LOG(msg, ...) QL_LOG(QL_WIFI_LOG_LEVEL, "wifi_msg", msg, ##__VA_ARGS__)

#define QL_WIFI_MSG_TASK_EVENT_CNT 5
#define QL_WIFI_MSG_MSG_QUEUE_CNT 128

#define QL_WIFI_MSG_TASK_QUEUE_WAIT 2000 // QL_WAIT_FOREVER
#define QL_WIFI_MSG_TASK_DEINIT_WAIT 5000

#define QL_WIFI_MSG_MAGIC 0xAA55

/*========================================================================
 *	Enumeration Definition
 *========================================================================*/
typedef enum
{
    QL_WIFI_MSG_TASK_TX,
    QL_WIFI_MSG_TASK_RX,
    QL_WIFI_MSG_TASK_MAX,
} ql_wifi_msg_task_type_e;

typedef enum
{
    QL_WIFI_MSG_TO_CREATE,
    QL_WIFI_MSG_TO_DESTROY,
    QL_WIFI_MSG_TODO_MAX,
} ql_wifi_msg_todo_e;
/*========================================================================
 *  Type Definition
 *=========================================================================*/
typedef struct
{
    ql_task_t task;
    ql_queue_t queue;
    ql_sem_t task_delete_sem;
} ql_wifi_msg_task_s;

typedef struct
{
    ql_wifi_msg_task_s task[QL_WIFI_MSG_TASK_MAX];
} ql_wifi_msg_manager_s;

/*========================================================================
 *   Global Variable
 *========================================================================*/
ql_wifi_msg_manager_s ql_wifi_msg_manager = {0};
ql_mutex_t ql_wifi_msg_mutex = NULL;
/*========================================================================
 *	function Definition
 *========================================================================*/
ql_wifi_msg_errcode_e ql_wifi_msg_recv(void *argv)
{
    ql_wifi_msg_task_s *task = &ql_wifi_msg_manager.task[QL_WIFI_MSG_TASK_RX];
    ql_wifi_msg_s *msg = (ql_wifi_msg_s *)argv;
    ql_wifi_msg_errcode_e ret = QL_WIFI_MSG_SUCCESS;
    ql_rtos_mutex_try_lock(ql_wifi_msg_mutex);

    if (QL_OSI_SUCCESS != ql_rtos_queue_release(task->queue, sizeof(ql_wifi_msg_s), (uint8 *)msg, QL_NO_WAIT))
    {
        QL_WIFI_MSG_LOG("release msg_queue failed");
        ret = QL_WIFI_MSG_EXECUTE_ERR;
    }
    ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
    return ret;
}

int ql_wifi_msg_tag_check(ql_wifi_msg_tag_e *tag, ql_wifi_msg_tlv_s *tlv)
{
    if (QL_WIFI_MSG_TAG_SEND_AP_CONFIG == (*tag & 0x00FF)
     || QL_WIFI_MSG_TAG_SEND_STA_CONFIG == (*tag & 0x00FF))
    {
        tlv->tag = (*tag >> 8);
        *tag &= 0x00FF;
        return 0;
    }
    return -1;
}

ql_wifi_msg_errcode_e ql_wifi_msg_write(ql_wifi_msg_tag_e tag, uint8_t *data, uint32_t len)
{
    if ((!data && 0 != len) || (data && 0 == len))
    {
        return QL_WIFI_MSG_INVALID_PARAM_ERR;
    }
    ql_wifi_msg_errcode_e ret = QL_WIFI_MSG_SUCCESS;
    int sub_tlv_header_len = 0;
    ql_wifi_msg_tlv_s tlv = {0};
    /* Update tag and sub tlv.*/
    if (0 == ql_wifi_msg_tag_check(&tag, &tlv))
    {
        sub_tlv_header_len = sizeof(ql_wifi_msg_tlv_s);
        tlv.length = len;
    }

    uint8_t *packet = (uint8_t *)calloc(1, sizeof(ql_wifi_msg_header_s) + (sub_tlv_header_len + len) + 1);
    if (!packet)
    {
        ret = QL_WIFI_MSG_NO_MEM_ERR;
        goto exit;
    }
    ql_wifi_msg_header_s *pHeader = (ql_wifi_msg_header_s *)packet;
    pHeader->magic = QL_WIFI_MSG_MAGIC;
    pHeader->len = sizeof(ql_wifi_msg_header_s) + (sub_tlv_header_len + len) + 1;
    pHeader->field.tag = tag;
    pHeader->field.length = (sub_tlv_header_len) + len;
    if (data)
    {
        if (sub_tlv_header_len)
        {
            memcpy(pHeader->field.value, &tlv, sub_tlv_header_len);
        }
        memcpy(pHeader->field.value + sub_tlv_header_len, data, len);
    }
    uint8_t checksum = packet[0];
    for (uint32_t i = 1; i < pHeader->len; i++)
    {
        checksum += packet[i];
    }
    packet[pHeader->len - 1] = checksum;
    QL_WIFI_MSG_LOG("spi header: %p", packet);
    QL_WIFI_MSG_LOG("spi header magic: %04x", pHeader->magic);
    QL_WIFI_MSG_LOG("spi header len: %04x(hex)=%d(dec)", pHeader->len, pHeader->len);
    QL_WIFI_MSG_LOG("spi header field tag: %04x", pHeader->field.tag);
    QL_WIFI_MSG_LOG("spi header field len: %04x(hex)=%d(dec)", pHeader->field.length, pHeader->field.length);
    QL_WIFI_MSG_LOG("spi header checksum: %02x/%02x", checksum, ((char *)pHeader)[pHeader->len - 1]);

    ql_wifi_msg_s msg = {.type = QL_WIFI_MSG_TYPE_WRITE,
                         .msg.msgType = QL_WIFI_ADAPTER_MSG_TYPE_MSG,
                         .msg.msgAddr = packet,
                         .msg.msgValue = pHeader->len};
    ql_rtos_mutex_try_lock(ql_wifi_msg_mutex);
    ql_wifi_msg_task_s *task = &ql_wifi_msg_manager.task[QL_WIFI_MSG_TASK_TX];
    if (QL_OSI_SUCCESS != ql_rtos_queue_release(task->queue, sizeof(ql_wifi_msg_s), (uint8 *)&msg, QL_NO_WAIT))
    {
        QL_WIFI_MSG_LOG("release msg_queue failed");
        ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
        if (packet)
        {
            free(packet);
        }
        ret = QL_WIFI_MSG_EXECUTE_ERR;
    }
    uint32 cnt = 0;
    ql_rtos_queue_get_cnt(task->queue, &cnt);
    QL_WIFI_MSG_LOG("msg queue %p/%d", task->queue, cnt);
    ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
exit:
    return ret;
}

void ql_wifi_msg_delete_task(ql_wifi_msg_task_s *task)
{
    if (!task)
    {
        return;
    }
    ql_wifi_msg_s msg = {0};
    int ret = 0;
    ql_rtos_mutex_try_lock(ql_wifi_msg_mutex);
    ql_rtos_semaphore_release(task->task_delete_sem);
    ql_rtos_semaphore_delete(task->task_delete_sem);
    uint32 cnt = 0;
    ql_rtos_queue_get_cnt(task->queue, &cnt);
    QL_WIFI_MSG_LOG("del msg queue cnt %d/%p", cnt, task);
    while (0 < cnt)
    {
        /* Wait for message queue. */
        ret = ql_rtos_queue_wait(task->queue, (uint8 *)&msg, sizeof(ql_wifi_msg_s), QL_WAIT_FOREVER);
        if (QL_OSI_SUCCESS != ret)
        {
            QL_WIFI_MSG_LOG("wait error err= %d", ret);
            break;
        }
        if (msg.msg.msgAddr)
        {
            /* This parameter is malloced with platform_memory_alloc. So free with same API. */
            ql_wifi_adapter_memory_free(msg.msg.msgAddr);
        }
        cnt--;
        QL_WIFI_MSG_LOG("remain msg %d", cnt);
    }
    ql_rtos_queue_delete(task->queue);
    memset(task, 0, sizeof(ql_wifi_msg_task_s));
    ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
    ql_rtos_task_delete(NULL);
}

static void ql_wifi_msg_tx_thread(void *argv)
{
    ql_wifi_msg_task_s *task = (ql_wifi_msg_task_s *)argv;
    QlOSStatus ret = QL_OSI_SUCCESS;
    ql_wifi_msg_s msg = {0};
    QL_WIFI_MSG_LOG("wifi msg tx task start");
    while (1)
    {
        /* Sem release. */
        ret = ql_rtos_semaphore_wait(task->task_delete_sem, QL_NO_WAIT);
        if (QL_OSI_SUCCESS != ret)
        {
            QL_WIFI_MSG_LOG("tx deleting");
            /*User called ql_wifi_msg_deinit and took the sem. */
            break;
        }
        /* Wait for message queue. */
        ret = ql_rtos_queue_wait(task->queue, (uint8 *)&msg, sizeof(ql_wifi_msg_s), QL_WIFI_MSG_TASK_QUEUE_WAIT);
        if (QL_OSI_SUCCESS != ret)
        {
            if (QL_OSI_TASK_PARAM_INVALID == ret)
            {
                break;
            }
            ql_rtos_semaphore_release(task->task_delete_sem);
            continue;
        }

        switch (msg.type)
        {
        case QL_WIFI_MSG_TYPE_WRITE: {
            ql_wifi_msg_header_s *p = (ql_wifi_msg_header_s *)(msg.msg.msgAddr);
            QL_WIFI_MSG_LOG("spi msg tag=0x%04x ,len=%d", p->field.tag, p->field.length);
            if (QL_WIFI_ADAPTER_SUCCESS != ql_wifi_adapter_msg_send(msg.msg.msgAddr, msg.msg.msgValue))
            {
                QL_WIFI_MSG_LOG("spi msg to write err: %p/%04x(hex)=%d(dec)", msg.msg.msgAddr, msg.msg.msgValue, msg.msg.msgValue);
                break;
            }
            else
            {
                QL_WIFI_MSG_LOG("spi msg to write suc: %p/%04x(hex)=%d(dec)", msg.msg.msgAddr, msg.msg.msgValue, msg.msg.msgValue);
                break;
            }
            break;
        }
        default: {
            break;
        }
        }
        if (msg.msg.msgAddr)
        {
            /* This parameter is malloced with platform_memory_alloc. So free with same API. */
            ql_wifi_adapter_memory_free(msg.msg.msgAddr);
        }
        ql_rtos_semaphore_release(task->task_delete_sem);
    }
    QL_WIFI_MSG_LOG("wifi msg tx task end");
    ql_wifi_msg_delete_task(task);
}

ql_wifi_msg_errcode_e ql_wifi_msg_parse(uint8_t *data, uint32_t len)
{
    int ret = QL_WIFI_MSG_SUCCESS;
    uint32_t padding = 2;
    uint8_t *pData = data + padding;
    len -= padding;
    /* It makes sure that the raw data is a independent frame.So it don't need cache. */
    ql_wifi_msg_header_s *pHeader = (ql_wifi_msg_header_s *)pData;
    if (QL_WIFI_MSG_MAGIC != pHeader->magic || len != pHeader->len)
    {
        QL_WIFI_MSG_LOG("wifi msg parse err 0x%x/0x%x,%d/%d", pHeader->magic, QL_WIFI_MSG_MAGIC, pHeader->len, len);
        ret = QL_WIFI_EXECUTE_ERR;
        goto exit;
    }
    uint8_t src_checksum = pData[len - 1];
    uint8_t exp_checksum = 0;
    for (int i = 0; i < len - 1; i++)
    {
        exp_checksum += pData[i];
    }
    if (src_checksum != exp_checksum)
    {
        QL_WIFI_MSG_LOG("wifi msg checksum err 0x%x/0x%x", src_checksum, exp_checksum);
        ret = QL_WIFI_EXECUTE_ERR;
        goto exit;
    }
    /* Parse TLV. */
    QL_WIFI_MSG_LOG("wifi msg tag %04x", pHeader->field.tag);

    ql_wifi_mode_e mode = QL_WIFI_MODE_MAX;
    ql_wifi_msg_todo_e todo = QL_WIFI_MSG_TODO_MAX;
    switch (pHeader->field.tag)
    {
    case QL_WIFI_MSG_TAG_REVC_STA_CONNECTED_PASSIVE:
    case QL_WIFI_MSG_TAG_REVC_STA_CONNECTED_ACTIVE:
    {
        mode = QL_WIFI_MODE_STA;
        todo = QL_WIFI_MSG_TO_CREATE;
        break;
    }
    case QL_WIFI_MSG_TAG_REVC_STA_DISCONNECTED_PASSIVE:
    case QL_WIFI_MSG_TAG_REVC_STA_DISCONNECTED_ACTIVE:
    {
        mode = QL_WIFI_MODE_STA;
        todo = QL_WIFI_MSG_TO_DESTROY;
        break;
    }
    case QL_WIFI_MSG_TAG_RECV_AP_CREATED:
    case QL_WIFI_MSG_TAG_REVC_AP_CLIENT_CONNECTED: {
        mode = QL_WIFI_MODE_AP;
        todo = QL_WIFI_MSG_TO_CREATE;
        break;
    }
    case QL_WIFI_MSG_TAG_REVC_AP_CLIENT_DISCONNECTED: {
        mode = QL_WIFI_MODE_AP;
        todo = QL_WIFI_MSG_TO_DESTROY;
        break;
    }
    case QL_WIFI_MSG_TAG_RECONNECT: 
    case QL_WIFI_MSG_TAG_REASON_CALL:
    case QL_WIFI_MSG_TAG_CONNECTED_CALL:{
        break;
    }
    default: {
        ret = QL_WIFI_EXECUTE_ERR;
        QL_WIFI_MSG_LOG("wifi msg err event");
        goto exit;
    }
    }

    if(QL_WIFI_MSG_TO_CREATE == todo)
    {
        if (pHeader->field.length != sizeof(ql_wifi_msg_recv_info_s))
        {
            ret = QL_WIFI_EXECUTE_ERR;
            QL_WIFI_MSG_LOG("wifi msg err event");
            goto exit;
        }
        ql_wifi_msg_recv_info_s *info = (ql_wifi_msg_recv_info_s *)pHeader->field.value;
        if (QL_WIFI_NET_SUCCESS != ql_wifi_net_create(mode, info, pHeader->field.tag))
        {
            ret = QL_WIFI_EXECUTE_ERR;
            QL_WIFI_MSG_LOG("wifi msg err event");
            goto exit;
        }
    }
    else if(QL_WIFI_MSG_TO_DESTROY == todo)
    {
        if (pHeader->field.length != sizeof(ql_wifi_msg_recv_info_s))
        {
            ret = QL_WIFI_EXECUTE_ERR;
            QL_WIFI_MSG_LOG("wifi msg err event");
            goto exit;
        }
        ql_wifi_msg_recv_info_s *info = (ql_wifi_msg_recv_info_s *)pHeader->field.value;
        if (QL_WIFI_NET_SUCCESS != ql_wifi_net_destroy(mode, info, pHeader->field.tag))
        {
            ret = QL_WIFI_EXECUTE_ERR;
            QL_WIFI_MSG_LOG("wifi msg err event");
            goto exit;
        }
    }
exit:
    return ret;
}

ql_wifi_msg_errcode_e ql_wifi_msg_send_data_streams(uint8_t *data, uint32_t len)
{
    int ret = QL_WIFI_MSG_SUCCESS;
    uint32_t padding = 2;
    uint8_t *pData = data + padding;
    len -= padding;

    ql_wifi_mode_e mode = QL_WIFI_MODE_MAX;
    uint8_t id = 0;

    if(QL_WIFI_APP_SUCCESS != ql_wifi_app_get_interface(&mode,&id))
    {
        QL_WIFI_MSG_LOG("Get interface err");
        ret = QL_WIFI_MSG_EXECUTE_ERR;
        goto exit;
    }

    /* Notify wifi net. */
    ql_wifi_net_s msg = {.type = QL_WIFI_NPTO_802_3_STREAM};
    msg.msg.msgValue = len;
    msg.msg.msgAddr = (uint8_t *)calloc(1, msg.msg.msgValue);
    if (!msg.msg.msgAddr)
    {
        ret = QL_WIFI_EXECUTE_ERR;
        goto exit;
    }
    msg.msg.msgType = (((uint32_t)mode & 0x0000FFFF) << 16) | ((uint32_t)id & 0x0000FFFF);
    memcpy(msg.msg.msgAddr, (void *)pData, msg.msg.msgValue);
    QL_WIFI_MSG_LOG("wifi msg to net %p/%d", msg.msg.msgAddr, msg.msg.msgValue);
    ql_wifi_net_recv(&msg);
exit:
    return ret;
}

static void ql_wifi_msg_rx_thread(void *argv)
{
    ql_wifi_msg_task_s *task = (ql_wifi_msg_task_s *)argv;
    QlOSStatus ret = QL_OSI_SUCCESS;
    ql_wifi_msg_s msg = {0};
    QL_WIFI_MSG_LOG("wifi msg rx task start");
    while (1)
    {
        /* Sem take. */
        ret = ql_rtos_semaphore_wait(task->task_delete_sem, QL_NO_WAIT);
        if (QL_OSI_SUCCESS != ret)
        {
            QL_WIFI_MSG_LOG("rx deleting");
            /*User called ql_wifi_msg_deinit and took the sem. */
            break;
        }
        /* Wait for message queue. */
        ret = ql_rtos_queue_wait(task->queue, (uint8 *)&msg, sizeof(ql_wifi_msg_s), QL_WIFI_MSG_TASK_QUEUE_WAIT);
        if (QL_OSI_SUCCESS != ret)
        {
            if (QL_OSI_TASK_PARAM_INVALID == ret)
            {
                break;
            }
            ql_rtos_semaphore_release(task->task_delete_sem);
            continue;
        }
        QL_WIFI_MSG_LOG("wifi msg rx 0x%x,0x%x", msg.type, msg.msg.msgType);
        switch (msg.type)
        {
        case QL_WIFI_MSG_TYPE_READ: {
            if (QL_WIFI_ADAPTER_MSG_TYPE_MSG == msg.msg.msgType)
            {
                /* Phase. */
                if (QL_WIFI_MSG_SUCCESS != ql_wifi_msg_parse(msg.msg.msgAddr, msg.msg.msgValue))
                {
                    QL_WIFI_MSG_LOG("spi msg slave parse err");
                }
            }
            else if (QL_WIFI_ADAPTER_MSG_TYPE_AP == msg.msg.msgType || QL_WIFI_ADAPTER_MSG_TYPE_STA == msg.msg.msgType)
            {
                /* Phase. */
                if (QL_WIFI_MSG_SUCCESS != ql_wifi_msg_send_data_streams(msg.msg.msgAddr, msg.msg.msgValue))
                {
                    QL_WIFI_MSG_LOG("spi msg send streams err");
                }
            }
            break;
        }
        default: {
            break;
        }
        }
        if (msg.msg.msgAddr)
        {
            /* This parameter is malloced with platform_memory_alloc. So free with same API. */
            ql_wifi_adapter_memory_free(msg.msg.msgAddr);
        }
        ret = ql_rtos_semaphore_release(task->task_delete_sem);
    }
    QL_WIFI_MSG_LOG("wifi msg rx task end");
    ql_wifi_msg_delete_task(task);
}

ql_wifi_msg_errcode_e ql_wifi_msg_task_create(ql_wifi_msg_task_type_e type)
{
    ql_wifi_msg_errcode_e ret = QL_WIFI_MSG_SUCCESS;
    if (type >= QL_WIFI_MSG_TASK_MAX)
    {
        return QL_WIFI_MSG_INVALID_PARAM_ERR;
    }
    ql_wifi_msg_task_s *task = &ql_wifi_msg_manager.task[type];
    /* Create task delete sem. */
    if (QL_OSI_SUCCESS != ql_rtos_semaphore_create(&(task->task_delete_sem), 1))
    {
        ret = QL_WIFI_MSG_SEM_CREATE_ERR;
        goto exit;
    }
    /* Create msg queue. */
    if (QL_OSI_SUCCESS != ql_rtos_queue_create(&task->queue, sizeof(ql_wifi_msg_s), QL_WIFI_MSG_MSG_QUEUE_CNT))
    {
        ret = QL_WIFI_MSG_MSG_QUEUE_CREATE_ERR;
        goto exit;
    }
    /* Create msg task. */

    char name[][20] = {"ql_wifi_msg_tx", "ql_wifi_msg_rx"};

    void (*thread[])(void *) = {ql_wifi_msg_tx_thread, ql_wifi_msg_rx_thread};

    if (QL_OSI_SUCCESS != ql_rtos_task_create(&task->task, 4096, APP_PRIORITY_REALTIME, (char *)name[type], thread[type], task, QL_WIFI_MSG_TASK_EVENT_CNT))
    {
        QL_WIFI_MSG_LOG("%s create err", name[type]);
        ret = QL_WIFI_MSG_TASK_CREATE_ERR;
        goto exit;
    }
exit:
    if (QL_WIFI_MSG_SUCCESS != ret)
    {
        if (task->task_delete_sem)
        {
            ql_rtos_semaphore_delete(task->task_delete_sem);
        }
        if (task->queue)
        {
            ql_rtos_queue_delete(task->queue);
        }
        memset(task, 0, sizeof(ql_wifi_msg_task_s));
    }
    return ret;
}

ql_wifi_msg_errcode_e ql_wifi_msg_init(void *argv)
{
    ql_wifi_msg_task_s *tx_task = &ql_wifi_msg_manager.task[QL_WIFI_MSG_TASK_TX];
    ql_wifi_msg_task_s *rx_task = &ql_wifi_msg_manager.task[QL_WIFI_MSG_TASK_RX];
    ql_wifi_msg_errcode_e ret = QL_WIFI_MSG_SUCCESS;
    /* Create mutex.Once mutex has been created,it will never delete for safety(e.g. Deleting task while sending msg queue). */
    if (NULL == ql_wifi_msg_mutex)
    {
        if (QL_OSI_SUCCESS != ql_rtos_mutex_create(&(ql_wifi_msg_mutex)))
        {
            return QL_WIFI_MSG_MUTEX_CREATE_ERR;
        }
    }
    /* Check if tasks have been created. */
    if (tx_task->task && rx_task->task)
    {
        goto exit;
    }
    else if (tx_task->task || rx_task->task)
    {
        ret = QL_WIFI_MSG_EXECUTE_ERR;
        goto exit;
    }
    ql_rtos_mutex_try_lock(ql_wifi_msg_mutex);
    /* Create TX task. */
    if (QL_WIFI_MSG_SUCCESS != (ret = ql_wifi_msg_task_create(QL_WIFI_MSG_TASK_TX)))
    {
        ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
        goto exit;
    }
    /* Create RX task. */
    if (QL_WIFI_MSG_SUCCESS != (ret = ql_wifi_msg_task_create(QL_WIFI_MSG_TASK_RX)))
    {
        ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
        goto exit;
    }
    ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
exit:
    if (QL_WIFI_MSG_SUCCESS != ret)
    {
        ql_wifi_msg_deinit(NULL);
    }
    return ret;
}

ql_wifi_msg_errcode_e ql_wifi_msg_deinit(void *argv)
{
    ql_wifi_msg_task_s *tx_task = &ql_wifi_msg_manager.task[QL_WIFI_MSG_TASK_TX];
    ql_wifi_msg_task_s *rx_task = &ql_wifi_msg_manager.task[QL_WIFI_MSG_TASK_RX];
    int ret = 0;
    /* All tasks have been deleted if both tasks are NULL. */
    if (!tx_task->task && !rx_task->task)
    {
        return QL_WIFI_MSG_SUCCESS;
    }
    ql_rtos_mutex_try_lock(ql_wifi_msg_mutex);
    /* Take sem. */
    if (QL_WIFI_SUCCESS != (ret = ql_rtos_semaphore_wait(tx_task->task_delete_sem, QL_WIFI_MSG_TASK_DEINIT_WAIT)))
    {
        QL_WIFI_MSG_LOG("wait tx sem %x", ret);
    }
    if (QL_WIFI_SUCCESS != (ret = ql_rtos_semaphore_wait(rx_task->task_delete_sem, QL_WIFI_MSG_TASK_DEINIT_WAIT)))
    {
        QL_WIFI_MSG_LOG("wait rx sem %x", ret);
    }
    ql_rtos_mutex_unlock(ql_wifi_msg_mutex);
    return QL_WIFI_MSG_SUCCESS;
}