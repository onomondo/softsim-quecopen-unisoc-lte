#ifndef __DRVSPI_PLATFORM_H__
#define __DRVSPI_PLATFORM_H__

#include "ql_wifi_adapter.h"

#define CSI_OK 0

typedef void (*drvspi_platform_irq_call)(void *);
int drv_spi_platfrom_write(unsigned char *wbuff, unsigned int len);
int drv_spi_platfrom_read(unsigned char *rbuff, unsigned int len);


int drvspi_platform_init(ql_wifi_spi_s* spi);
int drv_spi_platform_irq_req(char *irqname,ql_wifi_gpio_s *gpio);
void drv_spi_platform_irq_enable(void);
void drv_spi_platform_irq_disable(void);
int drv_spi_platform_gpio_value(void);

#endif