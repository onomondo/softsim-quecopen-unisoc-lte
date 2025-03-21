
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/

#ifndef QL_LBS_CLIENT_H
#define QL_LBS_CLIENT_H

#define lbs_print(msg, ...)     custom_log_tag("LBS_CLI", QUEC_LOG_TAG_LBS, msg, ##__VA_ARGS__)

#define LBS_MAX_CELL_NUM     6
#define LBS_MAX_POS_NUM      6
#define LBS_MAX_WIFI_NUM     6

#define LBS_SINGLE_CELL_LOC_METHOD  1
#define LBS_CELL_LOC_METHOD  4
#define LBS_CELL_WIFI_LOC_METHOD  5
#define LBS_WIFI_LOC_METHOD  6

typedef struct{
   uint8_t     type;// 类型 01表示定位，02表示上报
   uint8_t     encrypt;//加密方案，01表示异或
   uint8_t     key_index;// 01的加密方案中该参数表示"加扰KEY编号",本地随机生成，范围在0-7
   uint8_t     pos_format;//回应数据包类型01 不需要地址信息, 02 需要地址信息
   uint8_t     loc_method;//01-普通基站轮询,02-多基站分别轮询（废弃）, 03-服务端三角定位，04-第三方接口/模块端三角定位，05-cell+wifi定位，06-仅wifi定位。
}lbs_basic_info_t;

typedef struct{
    char      user_name[64];//用户名
    char      user_pwd[64];//密码
    char      token[128];//定位访问凭证
    char      imei[64];//国际移动设备识别码
    uint16_t  rand;//随机数
}lbs_auth_info_t;

typedef struct{
    uint8_t         radio;//发射类型
    uint16_t        mcc;//移动国家号码
    uint16_t        mnc;// 移动网络号码
    int             lac_id;//区域码
    int             cell_id;//基站码
    int16_t         signal;//信号强度
    uint16_t        tac;//跟踪区编码
    uint16_t        bcch;//BCCH信道的绝对无线频道编号
    uint8_t         bsic;//基站识别码/基站色码,GSM用
    uint16_t        uarfcndl;//下行频率,WCDMA 用
    uint16_t        psc;//主扰码,WCDMA用
    int16_t         rsrq;//参考信号接收质量,LTE用
    uint16_t        pci;//物理小区标识,LTE用
    uint16_t        earfcn;//绝对频点号,LTE用
    uint16_t        reserve;//保留
}lbs_cell_info_t;

typedef struct{
	char            wifi_mac[18];
	int             wifi_rssi;
	char            wifi_ssid[32];
}lbs_wifi_mac_info_t;

typedef struct{
	int                  pdp_cid;
	int                  sim_id;
	int                  req_timeout;
	lbs_basic_info_t    *basic_info;
	lbs_auth_info_t     *auth_info;
	int                  cell_num;
	lbs_cell_info_t     *cell_info;
	int                  wifi_num;
	lbs_wifi_mac_info_t *wifi_info;
}lbs_option_t;


typedef struct{
    float             longitude;//经度
    float             latitude;//纬度
    uint16_t          accuracy;//精度
    uint8_t           flag;//0 - 正常基站     1 – 无效基站
}lbs_postion_info_t;

typedef enum{
	QL_LBS_OK = 0,
	QL_LBS_LOC_FAIL							= (QL_COMPONENT_LWIP_LBS << 16) | 10000,	/* 定位失败 */
	QL_LBS_IMEI_ILLEGAL						= (QL_COMPONENT_LWIP_LBS << 16) | 10001,	/* 非法imei号 */
	QL_LBS_TOKEN_NOT_EXIST					= (QL_COMPONENT_LWIP_LBS << 16) | 10002,	/* token不存在 */
	QL_LBS_TOKEN_LOC_EXCEED_MAX				= (QL_COMPONENT_LWIP_LBS << 16) | 10003,	/* token的定位次数超过最大值 */
	QL_LBS_IMEI_LOC_EXCEED_DAY_MAX			= (QL_COMPONENT_LWIP_LBS << 16) | 10004,	/* 设备的每天定位次数超过最大值 */
	QL_LBS_IMEI_LOC_VISIT_EXCEED_MAX		= (QL_COMPONENT_LWIP_LBS << 16) | 10005,	/* token连接设备数超过最大值 */
	QL_LBS_TOKEN_EXPIRED					= (QL_COMPONENT_LWIP_LBS << 16) | 10006,	/* token过期 */
	QL_LBS_IMEI_NO_AUTHORITY				= (QL_COMPONENT_LWIP_LBS << 16) | 10007,	/* 该imei号设备不可访问服务 */
	QL_LBS_TOKEN_LOC_VISIT_EXCEED_MAX		= (QL_COMPONENT_LWIP_LBS << 16) | 10008,	/* token每天定位次数超过最大值 */
	QL_LBS_TOKEN_LOC_EXCEED_PERIOD_MAX		= (QL_COMPONENT_LWIP_LBS << 16) | 10009,	/* token周期内定位次数超过最大值 */
	QL_LBS_DNS_FAIL							= (QL_COMPONENT_LWIP_LBS << 16) | 10101,
	QL_LBS_MD5_FAIL							= (QL_COMPONENT_LWIP_LBS << 16) | 10102,
	QL_LBS_MEMORY_FAIL						= (QL_COMPONENT_LWIP_LBS << 16) | 10103,
	QL_LBS_NET_FAIL							= (QL_COMPONENT_LWIP_LBS << 16) | 10104,
	QL_LBS_PARAM_FORMAT_FAIL				= (QL_COMPONENT_LWIP_LBS << 16) | 10105,
}lbs_result_code_e;

typedef int lbs_client_hndl;

typedef struct{
	lbs_client_hndl		hndl;		/* LBS客户端句柄。由ql_lbs_get_position获取          */
	lbs_result_code_e	result;		/* 定位结果，对应lbs_result_code_e        */
	int 				pos_num;	/* LBS定位成功后，得到位置信息的个数          */
	lbs_postion_info_t	*pos_info;	/* BS得到的位置信息        */
	char 				*date;		/* 服务器返回的时间         */
	void				*arg;		/* 用户自定义参数,由ql_lbs_get_position传入           */
}lbs_response_data_t;

/*****************************************************************
* Function: ql_lbs_response_callback
*
* Description:
* 	LBS的定位结果，通过该函数告知应用程序。
* 
* Parameters:
* 	response_data	[in]	定位响应数据，对应lbs_response_data_t
*
* Return: void
*
*****************************************************************/
typedef void(*ql_lbs_response_callback)(lbs_response_data_t *response_data);

/*****************************************************************
* Function: ql_lbs_get_position
*
* Description:
* 	基于基站信息/wifi信息，查询设备所在的位置信息。
* 
* Parameters:
*   lbs_hndl     [out]  创建的LBS客户端句柄
* 	host	     [in] 	服务器地址 
*	user_opts    [in] 	LBS请求的参数 
*   cb           [in]   LBS请求结果的回调函数
*	arg			 [in]	用户自定义参数
*
* Return:结果码,参考lbs_result_code_e
*
*****************************************************************/
lbs_result_code_e ql_lbs_get_position(lbs_client_hndl *lbs_hndl, char *host, lbs_option_t *user_opts, ql_lbs_response_callback cb, void *arg);


#endif



