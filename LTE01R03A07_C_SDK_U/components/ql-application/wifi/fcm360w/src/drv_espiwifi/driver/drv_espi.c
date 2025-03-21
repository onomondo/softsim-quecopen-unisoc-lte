#include "drv_espi.h"
#include "drvspi_platform.h"
#include "platform_os.h"

typedef struct {
    unsigned int gpioval;
    unsigned int init_level;
    platform_thread_t *rxtask;
    platform_thread_t *txtask;
    platform_sem_t *spibus;
    platform_sem_t *irqsem;
    platform_queue_t *txqueue;
    unsigned char *command;
} drv_espi_priv_t;

typedef struct {
    unsigned char *addr;
    unsigned int len;
    unsigned int mode;
} drv_espi_msg_t;

drv_espi_priv_t *g_espi_priv = NULL;

int drv_espi_read_status(void)
{
    unsigned char status[DRV_ESSPI_CMD_LEN + DRV_ESSPI_STATE_LEN] = {0};
    unsigned int cnt = 0;
    int ret = 0;

    do {
        if (cnt++ > DRV_ESSPI_STATE_TIMEOUT) {
            platform_log_e("%s[%d] state 0x%x\n", __FUNCTION__, __LINE__, ret);
            return -1;
        } else if (cnt > DRV_ESSPI_STATE_TIMEOUT/10) {
            platform_msleep(2);
        }

        status[0] = DRV_ESSPI_CMD_STATE;
        status[1] = DRV_ESSPI_CMD_DUMMY;
        ret = drv_spi_platfrom_read(status, sizeof(status));
        if (ret != 0) {
            return -1;
        }

        ret = status[2] | (status[3] << 8) | (status[4] << 16) | (status[5] << 24);
        platform_log_e("spi read state try %d/%08x",cnt,ret);
    } while ((ret & BIT16) != BIT16);

    return ret & 0xFFFF;
}

static int drv_espi_send_data(unsigned char *sdata, unsigned int len, unsigned int mode)
{
    unsigned char tbuf[DRV_ESSPI_CMD_LEN + DRV_ESSPI_STATE_LEN] = {DRV_ESSPI_CMD_WRITE, DRV_ESSPI_CMD_DUMMY};
    drv_espi_cfg_t *spicfg = (drv_espi_cfg_t *)&tbuf[DRV_ESSPI_CMD_LEN];
    int status = 0;

    spicfg->evt = DRV_ESPI_TYPE_HTOS;
    spicfg->len = len - DRV_ESSPI_CMD_LEN;
    spicfg->type = DRV_ESSPI_SERVICE_WRITE;

    if(mode == 0){
        spicfg->net = 1;//sta:0
    }
    else{
        spicfg->net = 0;//ap:0
    }

    status = drv_spi_platfrom_write(tbuf, DRV_ESSPI_CMD_LEN + DRV_ESSPI_STATE_LEN);
    if (status != 0) {
        return -1;
    }

    status = drv_espi_read_status();
    if (status < 0) {
        return -1;
    }

    if (status == 0) {
        return -2;
    }

    sdata[0] = DRV_ESSPI_CMD_WRITE;
    sdata[1] = DRV_ESSPI_CMD_DUMMY;

    return drv_spi_platfrom_write(sdata, len);
}

int drv_espi_send_type_data(drv_espi_type_e type, unsigned char *msg, unsigned int len)
{
    unsigned char tbuf[DRV_ESSPI_CMD_LEN + DRV_ESSPI_STATE_LEN] = {DRV_ESSPI_CMD_WRITE, DRV_ESSPI_CMD_DUMMY};
    drv_espi_cfg_t *spicfg = (drv_espi_cfg_t *)&tbuf[DRV_ESSPI_CMD_LEN];
    drv_espi_priv_t *priv = g_espi_priv;
    int status = 0;
    unsigned char *mbuff = platform_memory_alloc(len + DRV_ESSPI_CMD_LEN);
    if (mbuff == NULL) {
        platform_log_e("%s[%d] can not alloc msg buff\n", __FUNCTION__, __LINE__);
        return -1;
    }

    spicfg->evt = type;
    spicfg->len = len;
    spicfg->type = DRV_ESSPI_SERVICE_WRITE;

    status = platform_sem_wait(priv->spibus, PLATFORM_WAIT_FOREVER);
    if (status != 0) {
        platform_memory_free(mbuff);
        platform_log_e("%s[%d] spi bus sem take error\n", __FUNCTION__, __LINE__);
        return -1;
    }

    status = drv_spi_platfrom_write(tbuf, DRV_ESSPI_CMD_LEN + DRV_ESSPI_STATE_LEN);
    if (status != 0) {
        platform_memory_free(mbuff);
        platform_sem_post(priv->spibus);
        return -1;
    }
    uint8_t max_cnt = 0;
RETRY:
    status = drv_espi_read_status();
    platform_log_e("espi status %x",status);
    if (status < 0) {
        platform_memory_free(mbuff);
        platform_sem_post(priv->spibus);
        platform_msleep(2);
        return -1;
    }

    if (status == 0) {
        //platform_memory_free(mbuff);
        //platform_sem_post(priv->spibus);
        platform_msleep(2);
        if(max_cnt++ > 3)
        {
            platform_memory_free(mbuff);
            platform_sem_post(priv->spibus);
            platform_msleep(2);
            return -1;
        }
        goto RETRY;
    }

    mbuff[0] = DRV_ESSPI_CMD_WRITE;
    mbuff[1] = DRV_ESSPI_CMD_DUMMY;

    platform_memory_copy(mbuff + DRV_ESSPI_CMD_LEN, msg, len);
    status = drv_spi_platfrom_write(mbuff, len + DRV_ESSPI_CMD_LEN);
    platform_memory_free(mbuff);
    platform_sem_post(priv->spibus);
    return status;
}
#if 0
int drv_espi_read_info(int offset, unsigned char *rbuff, unsigned int len)
{
    esnet_wifi_msg_t msg = {0};
    unsigned char tbuf[DRV_ESSPI_CMD_LEN + DRV_ESSPI_STATE_LEN] = {DRV_ESSPI_CMD_WRITE, DRV_ESSPI_CMD_DUMMY};
    drv_espi_cfg_t *spicfg = (drv_espi_cfg_t *)&tbuf[DRV_ESSPI_CMD_LEN];
    drv_espi_priv_t *priv = g_espi_priv;
    int status = 0;

    spicfg->evt = offset;
    spicfg->len = len - DRV_ESSPI_CMD_LEN;
    spicfg->type = DRV_ESSPI_SERVICE_READ;

    status = platform_sem_wait(priv->spibus, PLATFORM_WAIT_FOREVER);
    if (status != 0) {
        platform_log_e("%s[%d] spi bus sem take error\n", __FUNCTION__, __LINE__);
        return -1;
    }

    status = drv_spi_platfrom_write(tbuf, DRV_ESSPI_CMD_LEN + DRV_ESSPI_STATE_LEN);
    if (status != 0) {
        platform_sem_post(priv->spibus);
        return -1;
    }

    status = drv_espi_read_status();
    if (status <= 0) {
        platform_sem_post(priv->spibus);
        return -1;
    }

    rbuff[0] = DRV_ESSPI_CMD_READ;
    rbuff[1] = DRV_ESSPI_CMD_DUMMY;
    status = drv_spi_platfrom_read(rbuff, len);
    if (status != 0) {
        platform_sem_post(priv->spibus);
        return -1;
    }
    platform_sem_post(priv->spibus);

    msg.msgType = offset;
    msg.msgAddr = rbuff;

    return esnet_wifi_send_msg(&msg);
}

int drv_espi_write_info(int offset, unsigned char *wbuff, unsigned int len)
{
    unsigned char tbuf[DRV_ESSPI_CMD_LEN + DRV_ESSPI_STATE_LEN] = {DRV_ESSPI_CMD_WRITE, DRV_ESSPI_CMD_DUMMY};
    drv_espi_cfg_t *spicfg = (drv_espi_cfg_t *)&tbuf[DRV_ESSPI_CMD_LEN];
    drv_espi_priv_t *priv = g_espi_priv;
    int status;

    spicfg->evt = offset;
    spicfg->len = len - DRV_ESSPI_CMD_LEN;
    spicfg->type = DRV_ESSPI_SERVICE_WRITE;

    status = platform_sem_wait(priv->spibus, PLATFORM_WAIT_FOREVER);
    if (status != 0) {
        platform_log_e("%s[%d] spi bus sem take error\n", __FUNCTION__, __LINE__);
        return -1;
    }

    status = drv_spi_platfrom_write(tbuf, DRV_ESSPI_CMD_LEN + DRV_ESSPI_STATE_LEN);
    if (status != 0) {
        platform_sem_post(priv->spibus);
        return -1;
    }

    status = drv_espi_read_status();
    if (status <= 0) {
        platform_sem_post(priv->spibus);
        return -1;
    }

    wbuff[0] = DRV_ESSPI_CMD_WRITE;
    wbuff[1] = DRV_ESSPI_CMD_DUMMY;
    status = drv_spi_platfrom_write(wbuff, len);
    if (status != 0) {
        platform_sem_post(priv->spibus);
        return -1;
    }

    platform_sem_post(priv->spibus);

    return 0;
}

#endif
static int drv_espi_read_data(drv_espi_priv_t *priv)
{
    esnet_wifi_msg_t msg = {0};
    unsigned char tbuf[DRV_ESSPI_RXDATA_MAX] = {DRV_ESSPI_CMD_WRITE, DRV_ESSPI_CMD_DUMMY};
    int status = 0;

    drv_espi_cfg_t *spicfg = (drv_espi_cfg_t *)&tbuf[DRV_ESSPI_CMD_LEN];

    spicfg->evt = DRV_ESPI_TYPE_STOH;
    spicfg->len = 0;
    spicfg->type = DRV_ESSPI_SERVICE_READ;

    status = drv_spi_platfrom_write(tbuf, DRV_ESSPI_CMD_LEN + DRV_ESSPI_STATE_LEN);
    if (status != 0) {
        return -1;
    }

    status = drv_espi_read_status();
    if (status <= 0) {
        return -1;
    }

    if (status == 0xFFFF) {
        platform_log_e("%s[%d] 0x%x\n", __FUNCTION__, __LINE__, status);
        return -2;
    }

    platform_log_e("espi rx 0x%x",status);

    if ((status & 0xF000) == DRV_ESSPI_CONTROL_AP) {
        platform_log_e("Data from AP");
        msg.msgType = DRV_ESSPI_CONTROL_AP;
        msg.msgValue = (status & 0xFFFF) + DRV_ESSPI_CMD_LEN;
    } else if ((status & 0xF000) == DRV_ESSPI_CONTROL_STA) {
        platform_log_e("Data from STA");
        msg.msgType = DRV_ESSPI_CONTROL_STA;
        msg.msgValue = (status & 0xFFFF) + DRV_ESSPI_CMD_LEN;
    } else if ((status & 0xF000) == DRV_ESSPI_CONTROL_MSG) {
        msg.msgType = DRV_ESPI_TYPE_MSG;
        msg.msgValue = (status & 0xFFF) + DRV_ESSPI_CMD_LEN;
    } else if ((status & 0xF000) == DRV_ESSPI_CONTROL_INT) {
        msg.msgType = DRV_ESPI_TYPE_INT;
        msg.msgValue = status & 0xFFF;
    } else {
        platform_log_e("Unknown Status");
        return -1;
    }

    if (msg.msgType != DRV_ESPI_TYPE_INT) {
        msg.msgAddr = platform_memory_alloc(msg.msgValue);
        if (msg.msgAddr == NULL) {
            platform_log_e("%s[%d] alloc failed\n", __FUNCTION__, __LINE__);
            return -1;
        }
        msg.msgAddr[0] = DRV_ESSPI_CMD_READ;
        msg.msgAddr[1] = DRV_ESSPI_CMD_DUMMY;
        drv_spi_platfrom_read(msg.msgAddr, msg.msgValue);
    }
    platform_log_e("espi rx 0x%04x/0x%04x/%d/%p",status,msg.msgType,msg.msgValue,msg.msgAddr);
    if(QL_WIFI_ADAPTER_SUCCESS != ql_wifi_adapter_msg_dispatching(&msg))
    {
        if(msg.msgAddr)
        {
            platform_memory_free(msg.msgAddr);
        }
        return -1;
    }
    return 0;
}

void drv_espi_rx_thread(void *param)
{
    drv_espi_priv_t *priv = (drv_espi_priv_t *)param;
    int ret;
    platform_log_d("spi rx thread start\n");

    do {
        priv->gpioval = drv_spi_platform_gpio_value();
        platform_log_e("spi platform rxrxrxrx %d",priv->gpioval);
        if (priv->gpioval == priv->init_level) {
            drv_spi_platform_irq_enable();
            ret = platform_sem_wait(priv->irqsem, PLATFORM_WAIT_FOREVER);
            if (ret != 0) {
                platform_log_e("%s[%d] spi irq sem take error\n", __FUNCTION__, __LINE__);
                break;
            }
        }
        platform_log_e("spi platform rxrxrxrx222");
        ret = platform_sem_wait(priv->spibus, PLATFORM_WAIT_FOREVER);
        if (ret != 0) {
            platform_log_e("%s[%d] spi bus recv sem take error\n", __FUNCTION__, __LINE__);
            break;
        }

        int ret = drv_espi_read_data(priv);
        if(ret == -2){
            ql_rtos_task_sleep_s(1);
        }
        platform_sem_post(priv->spibus);
    } while (1);

    platform_log_e("spi rx thread exit\n");

    while(1)
    {
        platform_log_e("rx thread exit wait");
        ql_rtos_task_sleep_s(10);
    }
}

int drv_espi_sendto_peer(unsigned char *data, unsigned int len, unsigned int mode)
{
    platform_log_e("drv_espi_sendto_peer");

    drv_espi_msg_t rxmsg = {0};
    drv_espi_priv_t *priv = g_espi_priv;

    rxmsg.addr = data;
    rxmsg.len = len;
    rxmsg.mode = mode;
    
    return platform_queue_send(priv->txqueue, (char *)&rxmsg, sizeof(drv_espi_msg_t), PLATFORM_WAIT_FOREVER);
}

void drv_espi_tx_thread(void *param)
{
    drv_espi_priv_t *priv = (drv_espi_priv_t *)param;
    drv_espi_msg_t rxmsg = {0};
    int ret = 0;
    platform_log_d("tx thread start\n");
    do
    {
        uint32_t cnt = DRV_ESSPI_STATE_TIMEOUT/10;
        ret = platform_queue_receive(priv->txqueue, (char *)&rxmsg, sizeof(rxmsg), PLATFORM_WAIT_FOREVER);
        if (ret != 0) {
            platform_log_e("%s[%d] spi txqueue recv error\n", __FUNCTION__, __LINE__);
            break;
        }

RETRY:
        ret = platform_sem_wait(priv->spibus, PLATFORM_WAIT_FOREVER);
        if (ret != 0) {
            platform_log_e("%s[%d] spi bus send sem take error\n", __FUNCTION__, __LINE__);
            break;
        }
        ret = drv_espi_send_data(rxmsg.addr, rxmsg.len, rxmsg.mode);
        if (ret == -2) {
            platform_sem_post(priv->spibus);
            platform_msleep(2);
            platform_log_e("tx retry %d",--cnt);
            if(cnt > 0)
            {
                goto RETRY;
            }
        }
        if(rxmsg.addr)
        {
            free(rxmsg.addr);
        }
        platform_sem_post(priv->spibus);
    } while(1);

    platform_log_e("tx thread exit\n");
    while(1)
    {
        platform_log_e("tx thread exit wait");
        ql_rtos_task_sleep_s(10);
    }
}

void drv_espi_service_exit(drv_espi_priv_t *priv)
{
    if (priv->spibus != NULL) {
        platform_sem_destroy(priv->spibus);
        priv->spibus = NULL;
    }

    if (priv->irqsem != NULL) {
        platform_sem_destroy(priv->irqsem);
        priv->irqsem = NULL;
    }

    if (priv->rxtask != NULL) {
        platform_thread_destory(priv->rxtask);
        priv->rxtask = NULL;
    }

    if (priv->txtask != NULL) {
        platform_thread_destory(priv->txtask);
        priv->txtask = NULL;
    }

    if (priv->txqueue != NULL) {
        platform_queue_destory(priv->txqueue);
        priv->txqueue = NULL;
    }

    if (priv != NULL) {
        platform_memory_free(priv);
        g_espi_priv = NULL;
    }
}

static void drv_espi_irq_call(void *args)
{
    drv_espi_priv_t *priv = (drv_espi_priv_t *)args;
    priv->gpioval = drv_spi_platform_gpio_value();
    if(priv->gpioval != priv->init_level)
    {
        drv_spi_platform_irq_disable();
        platform_sem_post(priv->irqsem);
    }
    platform_log_e("platform irq %d == %d?",priv->gpioval,priv->init_level);
}

int drv_espi_service_init(ql_wifi_adapter_driver_s* driver)
{
    if(g_espi_priv)
    {
        platform_log_e("espi created");
        return 0;
    }
    drv_espi_priv_t *priv = platform_memory_alloc(sizeof(drv_espi_priv_t));
    int ret = 0;

    platform_memory_set(priv, 0, sizeof(drv_espi_priv_t));
    g_espi_priv = priv;

    /* Configure SPI. */
    if (0 != drvspi_platform_init(&driver->spi))
    {
        return -1;
    }

    priv->txqueue = platform_queue_create("espitxqueue", DRV_ESSPI_QUEUE_DEPTH, sizeof(drv_espi_msg_t), 0);
    if (priv->txqueue == NULL) {
        return -1;
    }

    priv->spibus = platform_sem_create(1, 1);
    if (priv->spibus == NULL) {
        drv_espi_service_exit(priv);
    }

    priv->irqsem = platform_sem_create(1, 0);
    if (priv->irqsem == NULL) {
        drv_espi_service_exit(priv);
    }

    /* Configure GPIO. */
    driver->hrx_gpio.int_cb = (void*)drv_espi_irq_call;
    driver->hrx_gpio.cb_ctx = (void*)priv;
    ret = drv_spi_platform_irq_req("esirq",&driver->hrx_gpio);
    if (ret != 0) {
        drv_espi_service_exit(priv);
        return -1;
    }

    priv->init_level = (PULL_DOWN == driver->hrx_gpio.gpio_pull) ? LVL_LOW : LVL_HIGH;

    priv->rxtask = platform_thread_init("espirxtask", DRV_ESSPI_RXTASK_PRI, DRV_ESSPI_STACK_DEPTH, drv_espi_rx_thread, priv);
    if (priv->rxtask == NULL) {
        drv_espi_service_exit(priv);
    }

    priv->txtask = platform_thread_init("espitxtask", DRV_ESSPI_TXTASK_PRI, DRV_ESSPI_STACK_DEPTH, drv_espi_tx_thread, priv);
    if (priv->txtask == NULL) {
        drv_espi_service_exit(priv);
    }

    return 0;
}