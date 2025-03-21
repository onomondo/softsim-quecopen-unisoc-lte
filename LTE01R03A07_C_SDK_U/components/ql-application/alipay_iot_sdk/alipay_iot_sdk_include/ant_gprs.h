#ifndef __ANT_GPRS_H__
#define __ANT_GPRS_H__
#include "ant_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ANT_IMEI_MAXLEN 32
#define ANT_IMSI_MAXLEN 32
#define ANT_IMSI_MAXLEN 32
#define ANT_APN_MAXLEN  16

#define ANT_IMEI_LEN    15
#define ANT_IMSI_LEN    15
#define ANT_ICCID_LEN   20
#define ANT_APN_LEN     5

#define ANT_LTE_LABOR_MAX  3

/**
 * GPRS&LTE网络消息定义
 *
 * 1001: ANT_GPRS_EVENT_SIMCARD
 * ------------------------------
 *    SIM卡事件，消息格式: 
 *      ant_msg.message = ANT_GPRS_EVENT_SIMCARD;
 *      ant_msg.param1  = (ant_ptr)<status>
 *      status: 0 表示未插卡, 1 表示已插卡, -1 表示状态未知
 *
 * 1002: ANT_GPRS_EVENT_NETWORK_STATUS
 * ------------------------------
 *    驻网状态变化事件，消息格式: 
 *      ant_msg.message = ANT_GPRS_EVENT_NETWORK_STATUS;
 *      ant_msg.param1  = (ant_ptr)<status>
 *      status: 0 表示未驻网, 1 表示已驻网
 */
typedef enum {
    ANT_GPRS_EVENT_BASE             = 1000,
    ANT_GPRS_EVENT_SIMCARD,
    ANT_GPRS_EVENT_NETWORK_STATUS,
} ant_gprs_event_e;

typedef enum {
    SIM_STATUS_ABSENT   = 0,
    SIM_STATUS_PRESENT  = 1,
} ant_sim_status_e;

typedef enum {
    SERVICE_INVALID  = 0,
    SERVICE_VALID   = 1,
} ant_gprs_serv_status_e;

typedef enum {
    NETWORK_UNREGISTER  = 0,
    NETWORK_REGISTRED   = 1,
} ant_network_status_e;

typedef enum {
    NETWORK_TYPE_2G      = 0,
    NETWORK_TYPE_3G      = 1,
    NETWORK_TYPE_4G      = 2,
    NETWORK_TYPE_UNKNOWN = -1,    
} ant_network_type_e;

typedef enum {
    ANT_SIM_ID_NONE = -1,
    ANT_SIM_ID_SIM1 = 0,  /* sim card one */
    ANT_SIM_ID_SIM2,	  /* sim card two */
    ANT_SIM_ID_SIM3,	  /* sim card there */
    ANT_SIM_ID_SIM4,	  /* sim card four */
    ANT_SIM_ID_NUM		  /* total sim card number */
} ant_sim_id_e;

typedef enum {
    ANT_GPRS_BAND_NONE        = 0x00,     /* No network */
    ANT_GPRS_BAND_GSM900      = 0x02,     /* GSM900 */
    ANT_GPRS_BAND_DCS1800     = 0x08,     /* DCS1800 */
    ANT_GPRS_BAND_PCS1900     = 0x10,     /* PCS1900 */
    ANT_GPRS_BAND_GSM450      = 0x20,     /* GSM450 */
    ANT_GPRS_BAND_GSM480      = 0x40,     /* GSM480 */
    ANT_GPRS_BAND_GSM850      = 0x80,     /* GSM850 */

    ANT_GPRS_BAND_SCDMA       = 0x0101,   /* 中国移动TD-SCDMA, up: 2010-2025MHz, down: 2010-2025MHz */
    ANT_GPRS_BAND_LTE1880     = 0x0102,   /* 中国移动TD-LTE,   up: 1880-1890MHz, down: 1880-1890MHz */
    ANT_GPRS_BAND_LTE2320     = 0x0103,   /* 中国移动TD-LTE,   up: 2320-2370MHz, down: 2320-2370MHz */
    ANT_GPRS_BAND_LTE2575     = 0x0104,   /* 中国移动TD-LTE,   up: 2575-2635MHz, down: 2575-2635MHz */
    ANT_GPRS_BAND_WCDMA       = 0x0201,   /* 中国联通WCDMA,    up: 1940-1955MHz, down: 1940-1955MHz */
    ANT_GPRS_BAND_LTE2300     = 0x0202,   /* 中国联通TD-LTE,   up: 2300-2320MHz, down: 2300-2320MHz */
    ANT_GPRS_BAND_LTE2555     = 0x0203,   /* 中国联通TD-LTE,   up: 2555-2575MHz, down: 2555-2575MHz */
    ANT_GPRS_BAND_LTE1850     = 0x0204,   /* 中国联通TD-LTE,   up: 1755-1765MHz, down: 1850-1860MHz */
    ANT_GPRS_BAND_CDMA2000    = 0x0301,   /* 中国电信CDMA2000, up: 1920-1935MHz, down: 2110-2125MHz */
    ANT_GPRS_BAND_LTE2370     = 0x0302,   /* 中国电信TD-LTE,   up: 2370-2390MHz, down: 2370-2390MHz */
    ANT_GPRS_BAND_LTE2635     = 0x0303,   /* 中国电信TD-LTE,   up: 2635-2655MHz, down: 2635-2655MHz */
    ANT_GPRS_BAND_LTE1860     = 0x0304,   /* 中国电信FDD-LTE,  up: 1765-1780MHz, down: 1860-1875MHz */
    ANT_GPRS_BAND_OTHERS      = 0xf000,   /* Other band not defined above. */
} ant_gprs_band_e;


typedef struct {
    ant_u16 mcc;
    ant_u16 mnc;
    ant_u16 tac;
    ant_u32 cellid;
    ant_s16 rsrp;
    ant_s16 rsrq;
} ant_lte_scell_info_t;

typedef struct {
    ant_u16 tac;
    ant_u16 cellid;
    ant_s16 rsrp;
    ant_s16 rsrq;    
} ant_lte_ncell_Info_t;

typedef struct {
    ant_u16 mcc;
    ant_u16 mnc;
    ant_u16 lac;
    ant_u32 cellid;
    ant_u8  rxlev;
} ant_gsm_cell_info_t;

typedef struct {
    int labor_count;
    ant_lte_scell_info_t connect;
    ant_lte_ncell_Info_t labor[ANT_LTE_LABOR_MAX];
} ant_lte_cells_info_t;

typedef struct {
    int labor_count;
    ant_gsm_cell_info_t connect;
    ant_gsm_cell_info_t labor[ANT_LTE_LABOR_MAX];
} ant_gsm_cells_info_t;

typedef struct {
    int network_type; // refer ant_network_type_e
    union {
        ant_lte_cells_info_t lte;
        ant_gsm_cells_info_t gsm;
    } cell_info;
} ant_lbs_info_t;
/**
 * GPRS(2G)或PDP(4G/CAT1)模块初始化
 *
 * @return = 0: 初始化成功
 *         < 0: 初始化失败
 */
ant_s32 ant_gprs_init(void);

/**
 * GPRS(2G)或PDP(4G/CAT1)数据链路激活
 *
 * @param sim_id SIM卡ID
 *
 * @return = 0: 初始化成功，数据链路成功激活
 *         < 0: 数据链路初始化失败
 */
ant_s32 ant_gprs_connect(ant_sim_id_e sim_id);

/**
 * 获取本机IPv4地址
 *
 * @param ipaddr IPv4地址出口参数
 *
 * @return  = 0: 获取成功
 *          < 0: 获取失败，无有效IPv4地址
 */
ant_s32 ant_gprs_getlocalipaddr(ant_u32 *ipaddr);

/**
 * 获取DNS服务器IP地址
 *
 * @param addr1 返回DNS1 IP地址
 * @param addr2 返回DNS2 IP地址
 *
 * @return  = 0: 获取成功
 *          < 0: 获取失败
 */
ant_s32 ant_gprs_getdnsaddr(ant_u32 *addr1, ant_u32 *addr2);

/**
 * 设置DNS服务器IP地址
 *
 * @param addr1 DNS1 IP地址
 * @param addr2 DNS2 IP地址
 *
 * @return  = 0: 设置成功
 *          < 0: 设置失败
 */
ant_s32 ant_gprs_setdnsaddr(ant_u32 addr1, ant_u32 addr2);

/**
 * 获取当前信号强度档位
 *
 * @return 信号强度档位: 0~5
 */
ant_s32 ant_gprs_get_signal_level(void);

/**
 * 获取当前信号参数
 *
 * @param rsrp      返回值 -140 ~ -43
 * @param rsrq      返回值 -20 ~ -3
 * @param rssnr     返回值 -10 ~ 40
 *
 * @return  0 获取成功
 *         -1 获取失败
 */
ant_s32 ant_gprs_get_reference_signal_quality(int *rsrp, int *rsrq, int *rssnr);

/**
 * 获取当前信号csq信息
 *
 * @param strength  返回CSQ信号强度, 范围0~31,99(未知)
 * @param ber       返回误码率, 范围0~7
 *
 * @return  0 获取成功
 *         -1 获取失败
 */
ant_s32 ant_gprs_get_csq_info(ant_s32 * rssi, ant_s32 * ber);


/**
 * 获取当前驻网状态(PDP是否已经激活)
 *
 * @return 返回当前服务状态，取值范围参见ant_network_status_e
 */
ant_s32 ant_gprs_get_network_status(void);


/**
 * 获取当前网络服务可用状态
 *
 * @return 当前网络服务可用状态 参见@ant_gprs_serv_status_e
 */

ant_s32 ant_gprs_get_service_status(void);

/**
 * 获取当前频段
 *
 * @return 返回当前数据频段，取值范围参见@ant_gprs_band_e
 */
ant_s32 ant_gprs_get_band(void);

/**
 * 获取当前连接基站的地理位置
 *
 * @param lbs 返回当前连接基站的地理位置信息
 *
 * @return
 */
ant_s32 ant_gprs_get_lbs(ant_lbs_info_t *lbs);

/**
 * 返回当前是否已插卡
 *
 * @return 1： 已插卡
 *         0： 未插卡
 */
ant_s32 ant_gprs_has_sim_card(void);

/**
 * 获取设备的IMEI号
 *
 * @param imei 存储IMEI号的缓存
 * @param size 存储IMEI号缓存的大小
 *
 * @return  0： 获取成功
 *         -1： 获取失败
 */
ant_s32 ant_gprs_get_imei(ant_char *imei, ant_u32 size);

/**
 * 获取本机SIM卡的ICCID
 *
 * @param iccid 返回ICCID
 * @param size ICCID缓存长度
 * @return = 0: 获取成功
 *         < 0: 获取失败
 */
ant_s32 ant_gprs_get_iccid(char *iccid, ant_u32 size);


/**
 * 获取当前SIM卡的IMSI
 *
 * @param imsi 存储IMSI的缓存
 * @param size 存储IMSI地址缓存的大小
 *
 * @return  0： 获取成功
 *         -1： 获取失败
 */
ant_s32 ant_gprs_get_imsi(char *imsi, ant_u32 size);

/**
 * 获取当前SIM卡的APN
 *
 * @param apn 返回APN
 * @param size APN缓存长度
 *
 * @return = 0: 获取成功
 *         < 0: 获取失败
 */
ant_s32 ant_gprs_get_apn(char *apn, ant_u32 size);

/**
 * 获取当前连接的网络类型
 *
 * @return 网络类型, 取值参见ant_network_type_e
 */
ant_s32 ant_gprs_get_network_type(void);

/**
 * 获取当前连接的基站信息(lac/tac+cellid)
 *
 * @return = 0 成功
           < 0 失败
 */
ant_s32 ant_gprs_get_location_info(ant_u32 *lac, ant_u32 *cid);

/**
 * 获取rrc周期
 *
 * @return 返回获取的rrc周期
 */
ant_s32 ant_gprs_get_rrc_cycle(void);


/**
 * 获取drx周期
 *
 * @return 返回获取的drx周期
 */
ant_s32 ant_gprs_get_drx_cycle(void);

ant_s32 ant_gprs_get_sn(ant_char *sn, ant_u32 buffer_size);


#ifdef __cplusplus
}
#endif

#endif

