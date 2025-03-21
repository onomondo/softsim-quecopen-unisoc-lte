#include "drvspi_platform.h"
#include "platform_os.h"

typedef struct {
    ql_wifi_adapter_driver_s driver;
} drv_spi_platform_priv_t;

drv_spi_platform_priv_t g_drvspi_priv;

static drv_spi_platform_priv_t *drvspi_platform_get_priv(void)
{
    return &g_drvspi_priv;
}

void ql_wifi_hex_to_str_print(uint8_t *wbuff,uint32_t len,int flag)
{
#ifdef QUEC_DRVSPI_PLATFORM_DEBUG
    uint32_t buf_size = 5 * len + 1;
    char *buf = platform_memory_alloc(5 * len + 1);
    char* pBuf = buf;
    uint32_t remain = 0;
    if(!buf)
    {
        return;
    }
    for(int i = 0; i < len;i++)
    {
         remain = snprintf((char*)pBuf,buf_size - remain,"%02x ",wbuff[i]);
         pBuf += remain;
    }
    *pBuf = '\0';
    platform_log_e("spi %s data:%s",flag == 0?"w":"r",buf);
    free(buf);
#endif
}

int drv_spi_platform_gpio_value()
{
    /* Get GPIO level. */
    drv_spi_platform_priv_t *priv = drvspi_platform_get_priv();

    ql_LvlMode level = LVL_LOW; 
    if(QL_GPIO_SUCCESS != ql_gpio_get_level(priv->driver.hrx_gpio.gpio_num,&level))
    {
        return 0;
    }
    return (LVL_LOW == level)? 0 : 1;
}

int drv_spi_platfrom_read(unsigned char *rbuff, unsigned int len)
{
    /* Mallock data receive buff. */
    uint8_t *syncbuff = platform_memory_alloc(len);
    if(!syncbuff)
    {
        return -1;
    }
    ql_wifi_adapter_spi_set_cs(LVL_LOW);
    if(QL_WIFI_ADAPTER_SUCCESS != ql_wifi_adapter_spi_write_read(syncbuff,rbuff,len))
    {
        platform_memory_free(syncbuff);
        return -1;
    }
    ql_wifi_adapter_spi_set_cs(LVL_HIGH);
    platform_memory_set(rbuff, 0, len);
    memcpy(rbuff, syncbuff, len);
    platform_memory_free(syncbuff);
    ql_wifi_hex_to_str_print(rbuff,len,1);
    return 0;
}

int drv_spi_platfrom_write(unsigned char *wbuff, unsigned int len)
{
    ql_wifi_hex_to_str_print(wbuff,len,0);
    return drv_spi_platfrom_read(wbuff, len);
}

void drv_spi_platform_irq_enable(void)
{
    /* Enable irq. */
    drv_spi_platform_priv_t *priv = drvspi_platform_get_priv();
    platform_log_e("drv_spi_platform_irq_enable %d",priv->driver.hrx_gpio.gpio_num);
    ql_wifi_gpio_s *gpio = &priv->driver.hrx_gpio;
    if(QL_WIFI_ADAPTER_SUCCESS != ql_wifi_adapter_int_enable(gpio->gpio_num,gpio->gpio_trigger,gpio->gpio_debounce,gpio->gpio_edge,gpio->gpio_pull,gpio->int_cb,gpio->cb_ctx))
    {
        platform_log_e("%s[%d] csi_gpio_irq_enable failed\n", __FUNCTION__, __LINE__);
    }
    platform_log_e("drv_spi_platform_irq_enable suc %d",gpio->gpio_num);
}

void drv_spi_platform_irq_disable(void)
{
    /* Disable irq. */
    drv_spi_platform_priv_t *priv = drvspi_platform_get_priv();
    platform_log_e("drv_spi_platform_irq_disable %d",priv->driver.hrx_gpio.gpio_num);
    if (QL_WIFI_ADAPTER_SUCCESS != ql_wifi_adapter_int_disable(priv->driver.hrx_gpio.gpio_num)) 
    {
        platform_log_e("%s[%d] csi_gpio_irq_enable failed\n", __FUNCTION__, __LINE__);
    }
    platform_log_e("drv_spi_platform_irq_disable_suc %d",priv->driver.hrx_gpio.gpio_num);
}

int drv_spi_platform_irq_req(char *irqname,ql_wifi_gpio_s *gpio)
{
    /* Configure GPIO. */
    drv_spi_platform_priv_t *priv = drvspi_platform_get_priv();
    platform_memory_copy(&priv->driver.hrx_gpio,gpio,sizeof(ql_wifi_gpio_s));
    if(QL_WIFI_ADAPTER_SUCCESS != ql_wifi_adapter_pin_set_func(gpio->pin_num, gpio->func_sel))
    {
        goto exit;
    }
    platform_log_e("gpio cb %p/%p",gpio->int_cb,gpio->cb_ctx);

    if(QL_WIFI_ADAPTER_SUCCESS != ql_wifi_adapter_int_enable(gpio->gpio_num,gpio->gpio_trigger,gpio->gpio_debounce,gpio->gpio_edge,gpio->gpio_pull,gpio->int_cb,gpio->cb_ctx))
    {
        goto exit;
    }

    platform_log_e("gpio init suc");
exit:
    return 0;
}

static int drvspi_platform_driver_init(void)
{
    /* Init SPI. */
    drv_spi_platform_priv_t *priv = drvspi_platform_get_priv();
    if(QL_WIFI_ADAPTER_SUCCESS != ql_wifi_adapter_spi_init(&priv->driver.spi))
    {
        platform_log_e("spi init err");
        return -1;
    }
    return 0;
}

int drvspi_platform_init(ql_wifi_spi_s* spi)
{
    /* Init SPI. */
    drv_spi_platform_priv_t *priv = drvspi_platform_get_priv();
    int ret;
    platform_memory_set(priv, 0, sizeof(drv_spi_platform_priv_t));
    platform_memory_copy(&priv->driver.spi,spi,sizeof(ql_wifi_spi_s));
    ret = drvspi_platform_driver_init();
    if (ret != CSI_OK) {
        return -1;
    }
    platform_log_e("spi init suc");
    return 0;
}

void drvspi_platform_exit(void)
{
    drv_spi_platform_priv_t *priv = drvspi_platform_get_priv();
    if(QL_WIFI_ADAPTER_SUCCESS != ql_wifi_adapter_spi_deinit(NULL))
    {
        platform_log_e("spi deinit err");
    }
    platform_memory_set(priv, 0, sizeof(drv_spi_platform_priv_t));
}

