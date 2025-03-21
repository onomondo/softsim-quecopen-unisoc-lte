
/**  @file
  ql_gnss.h

  @brief
  TODO

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
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_GNSS_H
#define QL_GNSS_H

#include "ql_api_common.h"
#include "ql_api_osi.h"
#include "ql_uart.h"
#include "time.h"
#include <time.h>
#include "ql_api_datacall.h"
#include "ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Typedef
 ===========================================================================*/
typedef void (*ql_gnss_callback)(uint32 ind_type, ql_uart_port_number_e port, uint32 size);
typedef char nmea_cardinal_t;
typedef int32 loc_client_handle_type;
/** Number of item types; unsigned integer. */
typedef unsigned int dsat_num_item_type;
typedef unsigned char boolean;
typedef uint64 loc_position_valid_mask_type;
extern unsigned char  quec_GPS_NMEATYPE_table[];
extern unsigned char  quec_GLONASS_NMEATYPE_table[];
extern unsigned char  quec_GALILEO_NMEATYPE_table[];
extern unsigned char  quec_BEIDOU_NMEATYPE_table[];
extern unsigned char  quec_GNSS_NMEATYPE_table[];

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define ql_isspace(c)           (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
#define ARRY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))
#define ARRAY_LENGTH(a) (sizeof a / sizeof (a[0]))
#define QL_GNSS_ERRCODE_BASE (QL_COMPONENT_BSP_GNSS<<16)
#define GNSS_UART QL_UART_PORT_3
#define QUEC_GPS_RECBUF_LEN_MAX  2048
#define QUEC_GPS_CMD_LEN_MAX  64
#define NMEA_END_CHAR_1 '\r'
#define NMEA_END_CHAR_2 '\n'
#define jmin(a, b) (((a) > (b)) ? (b) : (a))
#define jmax(a, b) (((a) > (b)) ? (a) : (b))
#define GPRS_APN_MAX_LEN 99
/* nmea gsv packet define */
#define NMEA_GPGSV_TOTAL_MSGS               0
#define NMEA_GPGSV_MSG_NR                   1
#define NMEA_GPGSV_SATS                     2
#define NMEA_GPGSV_SAT_INFO_NR1             3
#define NMEA_GPGSV_SAT_INFO_ELEV1           4
#define NMEA_GPGSV_SAT_INFO_AZIMUTH1        5
#define NMEA_GPGSV_SAT_INFO_SNR1            6
#define NMEA_GPGSV_SAT_INFO_NR2             7
#define NMEA_GPGSV_SAT_INFO_ELEV2           8
#define NMEA_GPGSV_SAT_INFO_AZIMUTH2        9
#define NMEA_GPGSV_SAT_INFO_SNR2            10
#define NMEA_GPGSV_SAT_INFO_NR3             11
#define NMEA_GPGSV_SAT_INFO_ELEV3           12
#define NMEA_GPGSV_SAT_INFO_AZIMUTH3        13
#define NMEA_GPGSV_SAT_INFO_SNR3            14
#define NMEA_GPGSV_SAT_INFO_NR4             15
#define NMEA_GPGSV_SAT_INFO_ELEV4           16
#define NMEA_GPGSV_SAT_INFO_AZIMUTH4        17
#define NMEA_GPGSV_SAT_INFO_SNR4            18
#define NMEA_GPGSV_SAT_INFO_SIGID           19

#define KNOTS_CONVERSION_FACTOR         (1.852)
            
#define QL_GPS_ERRCODE_BASE (QL_COMPONENT_BSP_GNSS<<16)
    
#define NMEA_MAX_LENGTH  (82)
    
#define NMEA_CARDINAL_DIR_NORTH		(nmea_cardinal_t) 'N'
#define NMEA_CARDINAL_DIR_EAST		(nmea_cardinal_t) 'E'
#define NMEA_CARDINAL_DIR_SOUTH		(nmea_cardinal_t) 'S'
#define NMEA_CARDINAL_DIR_WEST		(nmea_cardinal_t) 'W'
#define NMEA_CARDINAL_DIR_UNKNOWN	(nmea_cardinal_t) '\0'
    
#define NMEA_PREFIX_LENGTH          (5)
#define NMEA_TIME_FORMAT	"%H%M%S"
#define NMEA_TIME_FORMAT_LEN	6
    
#define NMEA_DATE_FORMAT	"%d%m%y"
#define NMEA_DATE_FORMAT_LEN	6

#define KNOTS_CONVERSION_FACTOR         (1.852)
            
#define QL_GPS_ERRCODE_BASE (QL_COMPONENT_BSP_GNSS<<16)
    
#define QL_GSV_MAX_SATS  (160)   //5*8*4   (gps gl ga gb gnss)*(max 8 line gsv)*(one gsv has 4 sat)  

#define NMEA_GPGGA_UTC  0

/* Value indexes */
#define NMEA_GPRMC_TIME			            0
#define NMEA_GPRMC_STATUS                   1
#define NMEA_GPRMC_LATITUDE		            2
#define NMEA_GPRMC_LATITUDE_CARDINAL	    3
#define NMEA_GPRMC_LONGITUDE		        4
#define NMEA_GPRMC_LONGITUDE_CARDINAL	    5
#define NMEA_GPRMC_SPEED		            6
#define NMEA_GPRMC_COURSE                   7
#define NMEA_GPRMC_DATE			            8
/*nmea gga packet define*/
//2,3,4,5,6,7
#define NMEA_GPGGA_LATITUDE                     1
#define NMEA_GPGGA_LATITUDE_CARDINAL            2
#define NMEA_GPGGA_LONGITUDE                    3
#define NMEA_GPGGA_LONGITUDE_CARDINAL           4
#define NMEA_GPGGA_QUALITY                      5
#define NMEA_GPGGA_SATELLITES_TRACKED           6
#define NMEA_GPGGA_HDOP                         7
#define NMEA_GPGGA_ALTITUDE                     8
/*nmea gsa packet define*/
#define NMEA_GPGSA_MODE                         0
#define NMEA_GPGSA_NAVMODE                      1
#define NMEA_GPGSA_PDOP                         14
#define NMEA_GPGSA_HDOP                         15
#define NMEA_GPGSA_VDOP                         16
#define NMEA_GPGSA_systemID                     17


#define PDAPI_NMEA_GGA 0x0001
#define PDAPI_NMEA_RMC 0x0002
#define PDAPI_NMEA_GSV 0x0004
#define PDAPI_NMEA_GSA 0x0008
#define PDAPI_NMEA_VTG 0x0010
    
/*nmea txt packet define*/
#define NMEA_GNTXT_FWVER                        3

/*GNSS AGPS PARAM DEFAULT CFG*/
#define QL_AGPS_URL_STR              "http://quectel-api1.rx-networks.cn/rxn-api/locationApi/rtcm\0"
#define QL_AGPS_VENDORID_STR         "wLgWwv6JQt\0"
#define QL_AGPS_MODELID_STR          "Quectel\0"
#define QL_AGPS_PASSWORD_STR         "aFltUERDZzZxeTY5cEp2eA==\0"

/*===========================================================================
* Enum
===========================================================================*/
 typedef enum
 {
     QUEC_GNSS_UNKNOW_TYPE,
     QUEC_GNSS_RESET_TYPE,
     QUEC_GNSS_BAUD_SET, 
     QUEC_GNSS_INFO,
     QUEC_GNSS_NMEA_TYPE,
     QUEC_GNSS_CFGSYS,
     QUEC_GNSS_SYS_SAVE,
     QUEC_CNSS_CUSTOM_CMD,
     QUEC_GNSS_CFGDYN,
}quec_gnss_cmd;
    
typedef enum
{
   GPS_UNKNOW,
   GSP_OK_CMD=0x01,
   GPS_FAIL_0_CMD=0x02,
   GPS_FAIL_1_CMD=0x04,
   GPS_INFO_CMD=0x08,
}quec_gnss_cmd_resp;
typedef enum {
    NMEA_UNKNOWN,
    NMEA_RMC,
    NMEA_GGA,
    NMEA_GSA,
    NMEA_GSV,
    NMEA_VTG,
	NMEA_GLL,
} nmea_type;

typedef enum {
    SAT_UNKNOWN,
    SAT_GPS,
    SAT_GLONASS,
    SAT_GALILEO,
    SAT_BDS,
    SAT_MULSYS
} satellite_type;

typedef enum {
    PORT_NONE,
    PORT_USBNMEA,
    PORT_UARTDEBUG,
    PORT_MAX,
} nmea_outport_row;
typedef enum {
    QGPS_PROTO_NMEA_SRC_GGA,        /* GPGGA */
    QGPS_PROTO_NMEA_SRC_GLL,        /* GPGGA */
    QGPS_PROTO_NMEA_SRC_GSA,        /* GPGSA GNGSA PQGSA */
    QGPS_PROTO_NMEA_SRC_GSV,        /* GPGSV GLGSV GAGSV PQGSV */
    QGPS_PROTO_NMEA_SRC_RMC,        /* GPRMC */
    QGPS_PROTO_NMEA_SRC_VTG,        /* GPVTG */
    QGPS_PROTO_NMEA_SRC_ZDA,        /* GPVTG */
    QGPS_PROTO_NMEA_SRC_GST,        /* GPVTG */
QGPS_PROTO_NMEA_SRC_MAX,
} qgps_proto_nmea_src_e_type;

typedef enum {
    QGPS_START_COLD,
    QGPS_START_HOT,
    QGPS_START_WARM,
    QGPS_START_XTRA
} qgps_start_e_type;

typedef enum
{
    QL_GNSS_SUCCESS  = QL_SUCCESS,

    QL_GNSS_EXECUTE_ERR                  = 1|QL_GNSS_ERRCODE_BASE,
    QL_GNSS_INVALID_PARAM_ERR,
    QL_GNSS_NOT_SUPPORT_ERR,
    QL_GNSS_UART_SET_ERR,
    QL_GNSS_CB_NULL_ERR                 = 5|QL_GNSS_ERRCODE_BASE,
    QL_GNSS_ALREADY_OPEN,
    QL_GNSS_NVM_WRITE_ERR,
    QL_GNSS_NEMA_PARSE_ERR,
    QL_GNSS_PARAM_NULL_ERR,                                           //传入的参数为NULL
    QL_GNSS_LOCK_EXE_ERR,
    QL_GNSS_ALREADY_CLOSE,

    QL_GNSS_SYS_INIT_ERR                 = 30|QL_GNSS_ERRCODE_BASE,   //初始化失败
    QL_GNSS_SYS_MODE_ERR,                                             //冷热启动模式通知错误
    QL_GNSS_SYS_AGPS_ONGOING,                                         //agps 正在进行中
    QL_GNSS_SYS_AGPS_ERR,                                             //agps 执行失败  
    QL_GNSS_SYS_START_RF_SERVICE_ERR,                                 //申请RF资源失败
    QL_GNSS_SYS_MODE_CONVER_ERR         = 35|QL_GNSS_ERRCODE_BASE,    //冷启动转化为热启动失败
    QL_GNSS_SYS_ABORT_INVALID,                                        //LTE通知GNSS关闭失败
    QL_GNSS_SYS_RECOVERY_INVALID,                                     //LTE通知GNSS恢复失败
    QL_GNSS_SYS_FIX_ERR,                                              //定位失败
    QL_GNSS_SYS_CLOSE_ERR,                                            //关闭gnss失败

    QL_GNSS_APFLASH_NO_ERR               = 50|QL_GNSS_ERRCODE_BASE,   //! No error occurred.
    QL_GNSS_APFLASH_MALLOC_ERR,                                       //buf malloc err
    QL_GNSS_APFLASH_OVERFLOW_ERR,                                     //buf overflow err
    QL_GNSS_APFLASH_HEAD_ERR,                                         //not find head err
    QL_GNSS_APFLASH_RX_GOING,                                         //buf rx ongoing 
    QL_GNSS_APFLASH_RX_ERR               = 55|QL_GNSS_ERRCODE_BASE,   //buf rx err
    QL_GNSS_APFLASH_RX_PASS,                                          //buf rx pass

    QL_GNSS_AGPS_NO_MEM_ERR              = 80|QL_GNSS_ERRCODE_BASE,
    QL_GNSS_AGPS_NW_NOT_REGISTERED_ERR,
    QL_GNSS_AGPS_SEMAPHORE_CREATED_ERR,
    QL_GNSS_AGPS_CFUN_DISABLED_ERR,
    QL_GNSS_AGPS_NO_SIM_ERR,
    QL_GNSS_AGPS_DATACALL_ERR            = 85|QL_GNSS_ERRCODE_BASE,
    QL_GNSS_AGPS_HTTP_CLIENT_CREATED_ERR,
    QL_GNSS_AGPS_HTTP_PERFORM_ERR,
    QL_GNSS_AGPS_HTTP_CODE_ERR,
    QL_GNSS_AGPS_FILE_OPERATOR_ERR,
}ql_errcode_gnss;


/**
 * GNSS switch select
 */
typedef enum
{
    GNSS_DISABLE,
    GNSS_ENABLE,
    GNSS_RESET
}ql_GnssSW;
typedef enum
{
     AUTO_GNSS_DISABLE,
     AUTO_GNSS_ENABLE
}ql_AutoGnssSW;
typedef enum
{
    AGPS_GNSS_DISABLE,
    AGPS_GNSS_ENABLE
}ql_AGPSGnssSW;

typedef enum
{
   GNSS_CLOSE,
   GNSS_FIRMWARE_UPDATE,
   GNSS_POSITIONING,
   GNSS_FIX
}ql_GnssState; 

typedef enum
{
    QL_GNSS_FIRMWARE_REPORT_CODE_SUCCESS = 0,
    QL_GNSS_FIRMWARE_REPORT_CODE_OTHER_ERR = 1,
    QL_GNSS_FIRMWARE_REPORT_CODE_FILE_TRANSFERED_ERR = 2,
    QL_GNSS_FIRMWARE_REPORT_CODE_FILE_NOT_EXIST_ERR = 3,
    QL_GNSS_FIRMWARE_REPORT_CODE_FILE_VERIFIED_ERR = 4,
}ql_gnss_firmware_report_code_e;

typedef enum
{
    QL_GNSS_APFLASH_REPORT_CODE_SUCCESS = 0,
    QL_GNSS_APFLASH_REPORT_CODE_OTHER_ERR = 1,
    QL_GNSS_APFLASH_REPORT_CODE_FILE_TRANSFERED_ERR = 2,
    QL_GNSS_APFLASH_REPORT_CODE_FILE_NOT_EXIST_ERR = 3,
    QL_GNSS_APFLASH_REPORT_CODE_FILE_VERIFIED_ERR = 4,
}ql_gnss_apflash_report_code_e;

typedef enum
{
    QL_GNSS_AGPS_REPORT_CODE_SUCCESS = 0,
    QL_GNSS_AGPS_REPORT_CODE_OTHER_ERR = 1,
    QL_GNSS_AGPS_REPORT_CODE_FILE_TRANSFERED_ERR = 2,
    QL_GNSS_AGPS_REPORT_CODE_SIM_STATUS_ERR = 3,
    QL_GNSS_AGPS_REPORT_CODE_NW_NOT_REGISTERED_ERR = 4,
    QL_GNSS_AGPS_REPORT_CODE_DATACALL_ERR = 5,
    QL_GNSS_AGPS_REPORT_CODE_SOCKET_ERR = 6,
    /* 400 - 500 represents http error code. */
}ql_gnss_agps_report_code_e;

typedef enum
{
    APFLASH_DISABLE = 0,
    APFLASH_ENABLE
}ql_gnss_apflash_enable_e;
 
typedef enum
{
   APFLASH_DATA_NOT_RECV = 0,
   APFLASH_DATA_SET_RECV
}ql_gnss_apflashdatarecv_e;

typedef enum
{
   APFLASH_RETRY_DISABLE = 0,
   APFLASH_RETRY_ENABLE
}ql_gnss_apflashretry_e;

typedef enum
{
   APFLASH_POSITIONING = 0,
   APFLASH_FIXOK
}ql_gnss_apflashupdate_e;


/*===========================================================================
 * data struct
===========================================================================*/
typedef struct ql_gnss_data_struct {
    /*是否有效定位*/
    unsigned char valid;
    /*经度*/
    double longitude;
    /*经度E(东经)或W(西经)*/
    unsigned char longitude_cardinal;
    /*纬度*/
    double latitude;
    /*纬度N(北纬)或S(南纬)*/
    unsigned char latitude_cardinal;
    /*hdop精度*/
    float  hdop;
    /*phop精度*/
    float pdop;
    /*航向 0-360*/
    double heading;
    /*速度 km/h*/
    float gps_speed;
    /*信号强度,max=5, data from avg_snr*/
    unsigned char gps_signal;
    /*最大信号值 db*/
    unsigned int max_cnr;
    unsigned int max_cnr2;
    /*最小信号值 db*/
    unsigned int min_cnr;
    /*平均信号值 db*/
    unsigned int avg_cnr;
    /*信号值数组*/
    unsigned int cnrs[QL_GSV_MAX_SATS];
    unsigned int cnrs_index;
    /*定位卫星数量*/
    unsigned int satellites_num;
    /*海拔高度*/
    float altitude;
    /*固件版本号*/
    char fwver[32];
    /*定位日期*/
    struct tm time;
    /*定位时间*/
    unsigned int UTC;
    /*当前定位质量*/
    unsigned char quality;
    /*定位模式*/
    unsigned char navmode;
}ql_gnss_data_t;

typedef struct
{
//   TelAtParserID       atp_index;
//    uint32              atHandle;
    quec_gnss_cmd       cmd_id;
    char*               cmd_data;
    char                cmd_len;
    char                cmd_checksum;
    quec_gnss_cmd_resp  resp_type;  
}QGPS_CMD_TYPE;

typedef struct {
    unsigned int ih_magic;
    unsigned int ih_checksum;
    unsigned int ih_time;
    unsigned int ih_size;
    unsigned int ih_reserved;
    unsigned int pkg_flash_addr;
    unsigned int pkg_run_addr;
    unsigned int ih_hcrc;
} quec_image_header_t;

struct nmea_s{
satellite_type sat_type;
nmea_type type;
void *data;
};

/* GPS position struct */
struct nmea_position{
double minutes;
int degrees;
nmea_cardinal_t cardinal;
};

/* nmea rmc packet define */
struct nmea_gprmc_s{
/*1: valid, 0: invalid*/
int valid;
struct nmea_position longitude;
struct nmea_position latitude;
double course;
double speed;
struct tm time;
} ;

struct nmea_sat_info {
/* PRN */
int nr;
/*elevation of satellite*/
int elevation;
/*azimuth of satellite*/
int azimuth;
/*Signal Noise Ratio*/
int snr;
};

struct nmea_gpgsv_s {
int cur_sats;
int sig_id;
/*total msg count*/
int total_msgs;
/*current msg index*/
int msg_nr;
/*total satellite count*/
int total_sats;
/*satellites info*/
struct nmea_sat_info sats[4];
};

struct nmea_gpgga_s{
struct nmea_position longitude;
struct nmea_position latitude;
unsigned char quality;
unsigned int satellites_tracked;
double hdop;
double altitude;
double time;
};

struct nmea_gpgsa_s {
unsigned char mode;
unsigned char navmode;
double pdop;
double hdop;
double vdop;
unsigned int systemid;
};

typedef struct {
  boolean                       nmeasrc_enable;
  qgps_proto_nmea_src_e_type    gnss_nmea_sentence_type;
/* GGA */
struct
{
    uint8 gp_gga_nmea_len;      /* GPGGA */
    char *gp_gga_sent;
}gga_sentences[5];  /* gpgga glgga gagga pqgga gngga */
/* GSA */
struct
{
    uint8 gp_gsa_nmea_len;
    char *gp_gsa_sent;
}gsa_sentences[5]; /* gpgsa glgsa gagsa pqgsa gngsa */
/* GSV */
struct 
{  
    uint8  gp_gsv_nmea_len[8];
    char   *gp_gsv_sent[8]; 
    uint8  cur_gsa_line;
}gsv_sentences[5];  /* gpgsv glgsv gagsv pqgsv gngsv*/
/* RMC */
struct
{
    uint8 gp_rmc_nmea_len;      /* GPRMC */
    char *gp_rmc_sent;
}rmc_sentences[5];  /* gprmc glrmc garmc pqrmc gnrmc*/
/* VTG */
struct
{
    uint8 gp_vtg_nmea_len;      /* GPVTG */
    char *gp_vtg_sent;
}vtg_sentences[5];  /* gpvtg glvtg gavtg pqvtg gnvtg*/
/*GLL*/
struct
{
    uint8 gp_gll_nmea_len;      /* GPGLL */
    char *gp_gll_sent;
}gll_sentences[5];  /* gpgll glgll gagll pqgll gngll*/
} nmeasrc_sentences;

typedef struct {
  boolean                       nmeasrc_enable;
  qgps_proto_nmea_src_e_type    gnss_nmea_sentence_type;
  uint32                        gnss_nmea_sys_type;
  uint32                        gps_gp_nmea_type;
  uint32                        gps_gl_nmea_type;
  uint32                        gps_ga_nmea_type;
  uint32                        gps_beidou_nmea_type; 
  uint32                        gnss_nmea_type;
} nmeasrc_sentences_cfg;

typedef struct
{
uint16        year;
unsigned char month;
unsigned char day_of_week;
unsigned char day;
unsigned char hour;
unsigned char minute;
unsigned char second;
uint16        millisecond;
} loc_calendar_time_s_type;

typedef struct
{
	dsat_num_item_type gnssmode;   // 
	dsat_num_item_type fixmaxtime; //最大超时重��?
	dsat_num_item_type fixmaxdist; // 定位精度
	dsat_num_item_type b_fixcount_flag;
	dsat_num_item_type fixcount;   // count of fs
	dsat_num_item_type fixrate;    //fs  per/s
}quec_gps_config;

typedef struct
{
boolean  outport_en;
nmea_outport_row type;
unsigned char out_channel;
}quec_gps_outport_config;

typedef struct
{
    bool agpsflag;
    bool agps_assist_data_valid;
    int64_t local_last_time_min;
    uint8 profile_inx;
    char agps_url[100];
    char agpsvendorID[30];
    char agpsmodelID[30];
    char agpspassWord[30];
}quec_agps;

typedef struct
{
	uint8 ip_version;
	char agps_apn[APN_LEN_MAX];
}quec_agps_apn_cfg;

typedef struct
{
	bool agpsflag;
    uint8 profile_inx;
    char agps_url[100];
    char agpsvendorID[30];
    char agpsmodelID[30];
    char agpspassWord[30];
	quec_agps_apn_cfg agps_apn_cfg;
}quec_agps_cfg_ex;

typedef struct
{
    ql_GnssState gnss_state;
    bool gnss_firmware_update_success;
    bool gnss_first_open_success;
    ql_mutex_t lock;
}quec_gnss_flag;

typedef struct
{
    quec_gps_config confg;
    loc_calendar_time_s_type time;
    unsigned char start_type;
    nmeasrc_sentences_cfg nmea_handle_cfg;
    quec_gps_outport_config outport;
    bool gpsautoflag;
    quec_agps agps;
    char device_information[100];
    uint32 gather;//No real meaning,Only used to solve byte alignment problems
	quec_agps_apn_cfg agps_apn_cfg;
    uint8_t simid;//init variable simid=0, so can directly add struct
    uint32 pcie_power;//Power supply for saving minipcie
}quec_gps_handle;

extern nmeasrc_sentences nmea_handle;
extern quec_gps_handle quec_gps_config_param;

struct nmea_gntxt_s {
char fwver[32];
};

//GNSS配置信息结构体，目前只有nmeasrc_sentences_cfg成员
typedef	struct
{
	nmeasrc_sentences_cfg nmea_cfg;
}ql_gnss_cfg_s;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
/**
 * @brief GNSS function switch
 * @param gnss_sw : GNSS function enable/disable
 * @return
 *       QL_GNSS_INVALID_PARAM_ERR
 *       QL_GNSS_NO_SUPPORT_ERR
 *       QL_GNSS_ALREADY_OPEN
 *       QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_switch(ql_GnssSW gnss_sw);

/**
 * @brief GNSS auto start gnss cfg
 * @param gnss_sw : GNSS auto start enable/disable
 * @return
 *       QL_GNSS_INVALID_PARAM_ERR
 *       QL_GNSS_SUCCESS
 */ 
ql_errcode_gnss ql_auto_gnss_cfg(ql_AutoGnssSW autoflag);

/**
 * @brief user's GNSS callback register
 * @param gnss_cb : set callback by user
 * @return
 *       QL_GNSS_CB_NULL_ERR
 *       QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_callback_register(ql_gnss_callback gnss_cb);

/**
 * @brief GNSS nmea infomation get
 * @param pbuff : point for nmea info
 * @param len   : length for nmea info
 * @return
 *       QL_GNSS_INVALID_PARAM_ERR
 *       QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_nmea_get(ql_uart_port_number_e uart_port,unsigned char *pbuff, uint32 len);

/**
 * @brief GNSS state info get
 * @param NULL
 * @return 
 *        ql_GnssState
 */
ql_GnssState ql_gnss_state_info_get(void);

/**
 * @brief GNSS agps cfg
 * @param gnssagpsflag:GNSS agps enable/disable
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
          QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_agps_cfg(ql_AGPSGnssSW gnssagpsflag);

/**
 * @brief GNSS agps param cfg
 * @param profile: agps httpdowmload profile
 *        URL:     agps httpdowmload URL
 *        vendorID:agps httpdowmload vendorID
 *        modelID: agps httpdowmload modelID
 *        password:agps httpdowmload password
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
          QL_GNSS_SUCCESS
 * @note  
 *        EN: After AGPS feature is enabled, the PDP context specified by <profile> is maybe de-activated. 
 *        So, please do not use the same PDP context with other applications at the same time, 
 *        preventing the application from abnormal network disconnection and need restarting a data call.
 *        中文: 启用 AGPS 功能后，<profile>指定的PDP上下文可能将去激活。
 *        因此，AGPS 功能不可与其他应用同时共用同一个<profile>，否则可能产生网络应用异常断开，需要重新拨号。
 */
ql_errcode_gnss ql_gnss_agps_param_cfg(uint8 profile,const char *URL,const char *vendorID,const char *modelID,const char *password);

/**
 * @brief GNSS agps param cfg ex
 * @param agpscfg_ex: agps httpdowmload Configuration parameter structure pointer
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
          QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_agps_param_cfg_ex(quec_agps_cfg_ex *agpscfg_ex);

/**
 * @brief GNSS agps param get
 * @param agpscfg_ex: agps httpdowmload parameter structure pointer
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
          QL_GNSS_EXECUTE_ERR
          QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_agps_get_cfg(quec_agps_cfg_ex *agpscfg_ex);

/**
 * @brief GNSS sys_type cfg
 * @param sys_type:
          GNSS定位的卫星系统。默认值：5。
             0   单GPS 
             3   GPS + GLONASS + Galileo混合定位
             4   GPS + GLONASS混合定位
             5   GPS + BeiDou混合定位（当模块为非EC200U-EU时）
                 GPS + BeiDou + Galileo混合定位（当模块为EC200U-EU时）
             6   GPS + Galileo混合定位
             7   单BeiDou
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
 *        QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_sys_type_cfg(uint32 sys_type);

/*****************************************************************
* Function: ql_gnss_get_cfg
*
* Description: 获取gnss的配置信息
* 
* Parameters:
*   gnss_cfg      [out]   获取的配置信息
*
* Return:
*	QL_GNSS_PARAM_NULL_ERR
*	QL_GNSS_SUCCESS
*
*****************************************************************/
ql_errcode_gnss ql_gnss_get_cfg(ql_gnss_cfg_s * gnss_cfg);

/**
 * @brief GNSS apflash cycle update
 * @param status:  
          APFLASH_POSITIONING ,
          APFLASH_FIXOK   
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
 *        QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_apflash_cycle_update(ql_gnss_apflashupdate_e status);


/**
 * @brief GNSS apflash getpvdata
 * @param data:  get pvdata of ql_gnss_data_t*:  
 *                           
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
 *        QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_apflash_getpvdata(ql_gnss_data_t *data);


/**
 * @brief GNSS apflash retry enable
 * @param status:  
 *        APFLASH_RETRY_DISABLE
 *        APFLASH_RETRY_ENABLE   
 *                 
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
 *        QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_apflash_retry_enable(ql_gnss_apflashretry_e status);


/**
 * @brief GNSS apflash Dataparse
 * @param status:  
 * @param pbuff : point for nmea info
 * @param len   : length for nmea info  
 *                 
 * @return 
 *        QL_GNSS_APFLASH_NO_ERR             
 *        QL_GNSS_APFLASH_MALLOC_ERR,                                       
 *        QL_GNSS_APFLASH_OVERFLOW_ERR,                                    
 *        QL_GNSS_APFLASH_HEAD_ERR,                                         
 *        QL_GNSS_APFLASH_RX_GOING,                                         
 *        QL_GNSS_APFLASH_RX_ERR,                                           
 *        QL_GNSS_APFLASH_RX_PASS
 *
 */
ql_errcode_gnss ql_gnss_apflash_data_parse(unsigned char *pbuff, uint32 len);


/**
 * @brief GNSS apflash set recv status
 * @param status:  
 *        APFLASH_DATA_NOT_RECV
 *        APFLASH_DATA_SET_RECV    
 *                 
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
 *        QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_apflash_set_recv_status(ql_gnss_apflashdatarecv_e status);


/**
 * @brief GNSS apflash get recv status
 * @param status:  
 *        APFLASH_DATA_NOT_RECV
 *        APFLASH_DATA_SET_RECV
 * 
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
 *        QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_apflash_get_recv_status(ql_gnss_apflashdatarecv_e *status);


/**
 * @brief GNSS apflash enable
 * @param status:  
 *        APFLASH_DISABLE   
 *        APFLASH_ENABLE
 *  
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
 *        QL_GNSS_SUCCESS
 * 
 */
ql_errcode_gnss ql_gnss_apflash_enable(ql_gnss_apflash_enable_e status);


/**
 * @brief GNSS apflash is enable
 * @param status:  
 *        APFLASH_DISABLE   
 *        APFLASH_ENABLE
 *         
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
 *        QL_GNSS_SUCCESS 
 *
 */
ql_errcode_gnss ql_gnss_apflash_is_enable(ql_gnss_apflash_enable_e *status);


#ifdef CONFIG_QUEC_PROJECT_FEATURE_QDSIM
/**
 * @brief GNSS dsds sim cfg: 双卡双待版本GNSS SIM 可配置并存储到NV(用于agps功能,只能在GNSS关闭状态下可配置)
 * @param status:  
 *        nSim   取值：0-1
 *        0 : sim0
 *        1 : sim1
 *         
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
 *        QL_GNSS_EXECUTE_ERR
 *        QL_GNSS_SUCCESS 
 *
 */
ql_errcode_gnss ql_gnss_dsds_sim_cfg(uint8_t nSim);
#endif

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_GNSS_H */


