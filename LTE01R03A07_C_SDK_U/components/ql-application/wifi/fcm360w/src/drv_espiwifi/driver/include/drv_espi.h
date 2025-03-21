#ifndef __DRV_ESPI_H__
#define __DRV_ESPI_H__

#include "ql_wifi_adapter.h"

typedef struct {
    unsigned int evt:12;
    unsigned int net:2;
    unsigned int len:16;
    unsigned int type:2;
} drv_espi_cfg_t;

typedef enum {
    DRV_ESPI_TYPE_INFO,
    DRV_ESPI_TYPE_HTOS = 0x100,
    DRV_ESPI_TYPE_STOH = 0x200,
    DRV_ESPI_TYPE_OTA = 0x300,
    DRV_ESPI_TYPE_MSG = 0x500,
    DRV_ESPI_TYPE_INT,
} drv_espi_type_e;

typedef enum {
    DRV_ESSPI_CONTROL_AP = 0x0000,
    DRV_ESSPI_CONTROL_STA = 0x1000,
    DRV_ESSPI_CONTROL_INFO = 0x3000,
    DRV_ESSPI_CONTROL_MSG = 0xA000,
    DRV_ESSPI_CONTROL_CNT = 0x5000,
    DRV_ESSPI_CONTROL_INT = 0x9000,
} drv_espi_control_e;

#define DRV_ESSPI_CMD_LEN 2
#define DRV_ESSPI_DATA_OFFSET DRV_ESSPI_CMD_LEN

#define DRV_ESSPI_CMD_READ  0x0B
#define DRV_ESSPI_CMD_WRITE 0x51
#define DRV_ESSPI_CMD_STATE 0x05
#define DRV_ESSPI_CMD_DUMMY 0x00

#define DRV_ESSPI_CMD_LEN 2
#define DRV_ESSPI_STATE_LEN 4

#define DRV_ESSPI_SERVICE_READ  0x00
#define DRV_ESSPI_SERVICE_WRITE 0x01
#define DRV_ESSPI_STATE_TIMEOUT 1000

#define DRV_ESSPI_TXTASK_PRI    APP_PRIORITY_REALTIME
#define DRV_ESSPI_RXTASK_PRI    (DRV_ESSPI_TXTASK_PRI - 1)
#define DRV_ESSPI_STACK_DEPTH   4096 * 2

#define DRV_ESSPI_QUEUE_DEPTH   128
#define DRV_ESSPI_RXDATA_MAX    2048

int drv_espi_read_info(int offset, unsigned char *rbuff, unsigned int len);
int drv_espi_write_info(int offset, unsigned char *wbuff, unsigned int len);
int drv_espi_sendto_peer(unsigned char *data, unsigned int len, unsigned int mode);
int drv_espi_send_type_data(drv_espi_type_e type, unsigned char *msg, unsigned int len);

int drv_espi_read_status(void);

int drv_espi_service_init(ql_wifi_adapter_driver_s* driver);
#endif