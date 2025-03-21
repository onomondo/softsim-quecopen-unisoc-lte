#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ql_api_osi.h"
#include "ql_api_common.h"
#include "ql_fs.h"
#include "ql_api_datacall.h"
#include "ql_log.h"
#include "ql_api_osi.h"

#include "alipay_iot_sdk_include/ant_kal.h"
#include "alipay_iot_sdk_include/alipay_iot_sdk.h"
#include "alipay_iot_sdk_include/alipay_iot_coll_biz.h"
#include "alipay_iot_sdk_include/alipay_iot_kal.h"
#include "alipay_iot_sdk_include/alipay_iot_sdk.h"
#include "alipay_iot_sdk_include/alipay_iot_coll_biz.h"
#include "alipay_iot_sdk_include/alipay_iot_signer.h"
#include "alipay_iot_sdk_include/alipay_iot_cmd.h"
#include "alipay_iot_sdk_include/alipay_iot_ota.h"
#include "alipay_iot_sdk_include/alipay_iot_news.h"
#include "alipay_iot_sdk_include/alipay_iot_network.h"
#include "alipay_iot_sdk_include/alipay_iot_distribution.h"

//本 alipay SDK ,只需要打开此三个宏即可
#define __ALIPAY_IOT_IOTDID__
#define __ALIPAY_IOT_BIZSIGN__
#define __ALIPAY_IOT_MDAP__


#define QL_ALIPAY_IOT_LOG_LEVEL					QL_LOG_LEVEL_INFO
#define QL_ALI_IOT_APP_LOG(msg, ...)			QL_LOG(QL_ALIPAY_IOT_LOG_LEVEL, "QL_ALI_IOT_APP_LOG", msg, ##__VA_ARGS__)


#define QL_ALI_IOT_TIMER_NUM_MAX 				20
#define QL_ALI_IOT_LOCAL_MSG_SDK_START 			999
#define QL_ALI_IOT_LOCAL_MSG_TIMER_EXPIRED 		998

#define QL_ALI_IOT_ANT_PDP_CID					1
#define QL_ALI_IOT_SDK_OPEN_EVENT_ID 			30001


/*hardware supplier NO. quectel硬件供应商编号*/
#define QL_ALI_IOT_APP_MAN_SUPPLIER_ID      "201901220900654009"
#define QL_ALI_IOT_SDK_QUEUE_L1_SIZE 10
#define QL_ALI_IOT_SDK_QUEUE_L2_SIZE 10  /*FLASH空间大小 (6+6)*4 =48K*/
#define QL_ALI_IOT_SDK__EVN_VALUE_MAX 5

#define QL_ALI_IOT_APP_TASK_PRIORITY		20
#define QL_ALI_IOT_SDK_TASK_PRIORITY		25


typedef enum{
	QL_ALI_IOT_SDK_IDLE = 0,
    QL_ALI_IOT_SDK_RUN,
    QL_ALI_IOT_SDK_SHUT,
}ql_ali_iot_sdk_state_e;

typedef enum{
	QL_ALI_IOT_SIGN_MODE_RECV_QRCODE = 1,/*用户被扫*/
	QL_ALI_IOT_SIGN_MODE_FACE,/*用户人脸*/
	QL_ALI_IOT_SIGN_MODE_QRCODE,/*用户主扫*/
	QL_ALI_IOT_SIGN_MODE_DELEGATE,/*第三方代扣*/
}ql_ali_iot_sign_mode_e;

typedef enum{
	QL_ALI_IOT_RESULT_REBOOT = -2,
	QL_ALI_IOT_RESULT_FAIL= -1,
	QL_ALI_IOT_RESULT_OK= 0,
	QL_ALI_IOT_RESULT_PARAM_ERR,//1
	QL_ALI_IOT_RESULT_UNINIT,
	QL_ALI_IOT_RESULT_CACHE_ERR,
	QL_ALI_IOT_RESULT_MEM_ERR,
	QL_ALI_IOT_RESULT_NETBUSY_ERR,//5
	QL_ALI_IOT_RESULT_CACHE_FULL,
	QL_ALI_IOT_RESULT_DATABK_ERR,
	QL_ALI_IOT_RESULT_ERR,
	QL_ALI_IOT_RESULT_RSV1_ERR,
    QL_ALI_IOT_RESULT_NETWORK_ERR = 10,
    QL_ALI_IOT_RESULT_CODE_MAX,
}ql_ali_iot_app_result_code_e;

typedef struct
{
    int     mode;
    char    need_sign_data[128];
    char    amount[32];
}ql_ali_iot_app_paysign_str;
typedef struct
{
    int     broadcast_count;
	int     scan_count;
    char    *data;
}ql_ali_iot_app_payact_str;
typedef struct
{
    int     human_verify;
    char    *data;
}ql_ali_iot_app_payinfo_str;
typedef struct
{
    char    business_no[BUSINESS_NO_MAX_LEN+1];
    char    qr_code[QRCODE_MAX_LEN+1];
    int     amount;
    int     time_consuming;
    int     trade_result;
    unsigned int     timestamp;
    int     result;
    int     max_record_num;
}ql_ali_iot_app_payrep_str;
/*save parameter, 需要掉电保存的参数*/
typedef struct
{
    int     							rsv1;
    alipay_sdk_profile_info_struct     	profile_info_st;
	char 								terminal_id[SDK_DEVICE_SN_MAX_LEN+1]; //SN 设备的序列号sn
}ql_ali_iot_app_save_info_str;
typedef struct
{
    //char    					business_no[64+1];
	char 						account_iccid[ICCID_MAX_LEN+1]; //流量账户信息/SIM卡的ICCID
	char 						imei[SDK_DEVICE_IMEI_MAX_LEN+1]; //IMEI
	char 						sn[SDK_DEVICE_SN_MAX_LEN+1]; //SN
	char 						clear_sdk_cache;//clear cache flag
    int     					running;/*sdk run flag*/
    ql_ali_iot_app_save_info_str	app_save_info;
}ql_ali_iot_app_ctx_str;




static int g_ql_ali_iot_sdk_task_id 							= 0;
static int g_ql_ali_iot_app_server_task_id 						= 0;
static ant_timer g_ql_ali_iot_app_time_arry[QL_ALI_IOT_TIMER_NUM_MAX] = {0};
static ql_ali_iot_app_ctx_str ql_ali_iot_app_ctx 				= {0};
static ql_task_t ql_ali_iot_app_task_id 	= NULL;

static int ql_ali_iot_task_send_message(int target_id, int src_id, int msg_id, void *data)
{
    ant_msg msg = {0};
    msg.message = ANT_EVENT_IOT_SDK_EVENT;
    msg.param1 = (ant_ptr)msg_id;
    msg.param2 = (ant_ptr)data;
	msg.src_task_id = target_id;
	QL_ALI_IOT_APP_LOG("target_id: %d,src_id: %d,msg_id: %d,data: %p,msg: %p",target_id,src_id,msg_id,data,&msg);
    if (1 != ant_event_send_msg(target_id, &msg))
    {
		QL_ALI_IOT_APP_LOG("app task send message fail!");
        return -1;
	}
    return 1;
}

static int ql_ali_iot_task_recevie_message(int *src_id, int *msg_id, void **data)
{
    ant_msg msg = {0};
    if (1 != ant_event_recv_msg(&msg)) 
	{
		QL_ALI_IOT_APP_LOG("Demo task recv message fail!");
        return -1;
    }
	QL_ALI_IOT_APP_LOG("src_id: %d,message: %d,msg_id: %d,data: %p,msg: %p", msg.src_task_id,msg.message,(int)msg.param1,(void *)msg.param2);	
    if(msg.message == ANT_EVENT_IOT_SDK_EVENT)
	{
        *src_id = msg.src_task_id;
        *msg_id = (int)msg.param1;
        *data   = (void *)msg.param2;
		QL_ALI_IOT_APP_LOG("app task send message success!");	
        return 1;
    }
	else
	{
		QL_ALI_IOT_APP_LOG("fail: message %d  :  %d",msg.message,ANT_EVENT_IOT_SDK_EVENT);			
        return -1;
    }
}

void ql_ali_iot_sdk_manage_init(int iot_sdk_tid)
{
    static int my_count = 0;
    QL_ALI_IOT_APP_LOG("tid %d, count %d",iot_sdk_tid,my_count++);
    ql_ali_iot_task_send_message(iot_sdk_tid,(int)-1,MSG_ID_ALIPAY_SDK_INIT_REQ,NULL);
}

void ql_ali_iot_sdk_manage_nw_init(int iot_sdk_tid)
{
	QL_ALI_IOT_APP_LOG("iot_sdk_tid: %d",iot_sdk_tid);
    ql_ali_iot_task_send_message(iot_sdk_tid,(int)-1,MSG_ID_ALIPAY_SDK_NW_INIT_REQ,NULL);
}


static int ql_ali_iot_sdk_open_notify(int result)
{
	ql_event_t event = {0};
	event.id = QL_ALI_IOT_SDK_OPEN_EVENT_ID;
	event.param1 = (uint32)result;
    ql_rtos_event_send(ql_ali_iot_app_task_id, &event);
	QL_ALI_IOT_APP_LOG("ql ali iot sdk open %d",result);
	return 0;
}

void ql_ali_iot_sdk_manage_sdk_close(int iot_sdk_tid)
{
    QL_ALI_IOT_APP_LOG("iot_sdk_tid: %d",iot_sdk_tid);
    ql_ali_iot_task_send_message(iot_sdk_tid,(int)-1,MSG_ID_ALIPAY_SDK_CLOSE_IND,NULL);
}

void ql_ali_iot_stop_timer(int16_t id)
{
    if(g_ql_ali_iot_app_time_arry[id] != NULL)
    {
        ant_timer_stop(g_ql_ali_iot_app_time_arry[id]);
    }
}
void ql_ali_iot_start_timer(int16_t id, int32_t time, callback_on_timer callback_function)
{
    char timer_name[20]  = {0};
	int temp = id;
    QL_ALI_IOT_APP_LOG("%d",id);
    if(g_ql_ali_iot_app_time_arry[id] != NULL)
    {
        ant_timer_destroy(g_ql_ali_iot_app_time_arry[id]);
        g_ql_ali_iot_app_time_arry[id] = NULL;
    }
    QL_ALI_IOT_APP_LOG("%d",g_ql_ali_iot_app_time_arry[id]);
    if(g_ql_ali_iot_app_time_arry[id] == NULL)
    {
        alipay_iot_snprintf(timer_name, sizeof(timer_name), "%d", id);
        g_ql_ali_iot_app_time_arry[id] = ant_timer_create(timer_name);
    }
    QL_ALI_IOT_APP_LOG("%d  %p ",g_ql_ali_iot_app_time_arry[id],id);
    if(g_ql_ali_iot_app_time_arry[id] != NULL)
    {
        ant_timer_start(g_ql_ali_iot_app_time_arry[id], callback_function,(void*)temp, time, 0);
    }
}

void ql_ali_iot_sdk_close_cnf(void* msg)
{
	/*目前SDK 是不会执行的*/
    if(msg != NULL)
    {
        alipay_sdk_close_cnf_struct *rsp = (alipay_sdk_close_cnf_struct*)msg;
        if(rsp != NULL)
        {
            QL_ALI_IOT_APP_LOG("got result %d",rsp->result);
            alipay_sdk_close_cnf_struct_destory(rsp);
			ql_ali_iot_app_ctx.running = QL_ALI_IOT_SDK_SHUT;
			g_ql_ali_iot_sdk_task_id = 0;
        }
    }
}
void ql_ali_iot_reset_sdk_sta(void)
{
	ql_ali_iot_app_ctx.running = QL_ALI_IOT_SDK_IDLE;
}

void ql_ali_iot_waiting_sdk_udpate_time_expirey(int result)
{
    QL_ALI_IOT_APP_LOG("result :%d",result);
	/*report err URC*/
	ql_ali_iot_sdk_open_notify(result);
	
	/*clear alipay state*/
	ql_ali_iot_reset_sdk_sta();
}

static void ql_ali_iot_iot_timer_expired_in_task(void *id)
{
	int temp = (int)id;
    QL_ALI_IOT_APP_LOG("timer %p, expired id :%d",id,temp);
    switch(temp)
    {
        case 5:
			ql_ali_iot_sdk_manage_sdk_close(g_ql_ali_iot_sdk_task_id);
        break;

        case 9:
            ql_ali_iot_waiting_sdk_udpate_time_expirey(-1);
        break;

        default:
        break;
    }
}
static void ql_ali_iot_timer_expired(void *id)
{
    QL_ALI_IOT_APP_LOG("id:%p",id);
    ql_ali_iot_task_send_message(g_ql_ali_iot_app_server_task_id, -1, QL_ALI_IOT_LOCAL_MSG_TIMER_EXPIRED,id);
}

void ql_ali_iot_sdk_event_notify(void *msg)
{
    alipay_sdk_event_notify_struct *notify=(alipay_sdk_event_notify_struct*)msg;
    if(notify ==NULL)
    {
		QL_ALI_IOT_APP_LOG("param null");
        return;
    }
	QL_ALI_IOT_APP_LOG("event_id=%d",notify->event_id);
    alipay_sdk_event_notify_struct_destory(notify);
}

void ql_ali_iot_sdk_nw_init_rsp(void* msg)
{

    if(msg != NULL)
    {
        alipay_sdk_nw_init_rsp_struct *rsp = (alipay_sdk_nw_init_rsp_struct*)msg;
        if(rsp != NULL)
        {
            QL_ALI_IOT_APP_LOG("got result %d",rsp->result);
			ql_ali_iot_stop_timer(9);
			/*report URC*/
            if(rsp->result != 0)
            {
				/*clear alipay state*/
				ql_ali_iot_reset_sdk_sta();
			}
			ql_ali_iot_sdk_open_notify(rsp->result);

            alipay_sdk_nw_init_rsp_struct_destory(rsp);
        }
    }
}

//SDK通知更新了系统时间
void ql_ali_iot_sdk_sys_time_update_ind(void* msg)
{
	char tmp_buf[64]={0}; 
    if(msg != NULL)
    {
        alipay_sdk_sys_time_update_ind_struct *update_ind = (alipay_sdk_sys_time_update_ind_struct*)msg;
        if(update_ind != NULL)
        {
            QL_ALI_IOT_APP_LOG("cache=%d, got result %d",ql_ali_iot_app_ctx.clear_sdk_cache, update_ind->result);

            //如果需要清理SDK的所有缓存，只能在这里进行。
			//if(TRUE == ql_ali_iot_app_ctx.clear_sdk_cache){
            //	alipay_iot_clear_sdk_cache();//del cache file
			//}

			/*update SIM iccid*/
			if(0 == ant_gprs_get_iccid(tmp_buf, 64)){
				strcpy(ql_ali_iot_app_ctx.account_iccid, tmp_buf);
			}
            QL_ALI_IOT_APP_LOG("account_iccid= %s",ql_ali_iot_app_ctx.account_iccid);
            //获取时间成功，初始化SDK
            if(update_ind->result == 0)
            {
                ql_ali_iot_sdk_manage_init(g_ql_ali_iot_sdk_task_id);
            }
            alipay_sdk_sys_time_update_ind_struct_destory(update_ind);
        }

		//此时不等初始化sdk完成，已经可以进行数据存储不发送, 后续可以优化定时器启动和关闭位置逻辑
        //ql_ali_iot_stop_timer(9);
    }
}


void ql_ali_iot_sdk_init_rsp(void* msg)
{
    QL_ALI_IOT_APP_LOG("msg=%p",msg);
    if(msg != NULL)
    {
        alipay_sdk_init_rsp_struct *rsp = (alipay_sdk_init_rsp_struct*)msg;
		//IOT_LOG_INFO(LOG_TAG,"%s, rsp=%p", __func__, rsp);
        if(rsp != NULL)
        {
			QL_ALI_IOT_APP_LOG("rsp=%p result = %d",rsp,rsp->result);
            if(rsp->result != 0)/*激活成功*/
            {
                //初始化成功后，才可以初始化应用网络
                ql_ali_iot_sdk_manage_nw_init(g_ql_ali_iot_sdk_task_id);
            }
			else{
				ql_ali_iot_stop_timer(9);
				ql_ali_iot_sdk_open_notify(rsp->result);

				/*clear alipay state*/
				ql_ali_iot_reset_sdk_sta();
			}
            alipay_sdk_init_rsp_struct_destory(rsp);
        }
    }
}


int ql_ali_iot_sdk_create(int priority)//创建sdk+查状态和动作
{
    int32_t ret = 0;
	static int exec_mark=0;
	ql_ali_iot_app_save_info_str iot_save;	
	ant_memset(&iot_save, 0, sizeof(ql_ali_iot_app_save_info_str));

	ant_strncpy(iot_save.profile_info_st.module_supplier_no,"200",SDK_SUPPLIER_NO_MAX_LEN + 1);
	ant_strncpy(iot_save.profile_info_st.device_supplier_no,"200",SDK_DEVICE_SUPPLIER_NO_MAX_LEN + 1);
	ant_strncpy(iot_save.profile_info_st.merchant_user_id,"200",SDK_MERCHANT_USER_ID_MAX_LEN + 1);
	ant_strncpy(iot_save.profile_info_st.item_id,"2018121906963192",SDK_ITEM_ID_MAX_LEN + 1);
	ant_strncpy(iot_save.profile_info_st.product_key,"PAY_BOX",SDK_PRODUCT_KEY_MAX_LEN + 1);
	ant_strncpy(iot_save.terminal_id,"D1Q21A834011533",SDK_DEVICE_SN_MAX_LEN + 1);
	iot_save.profile_info_st.env = 5;
	//memcpy(ql_ali_iot_app_ctx.app_save_info.terminal_id,iot_save.terminal_id,SDK_DEVICE_SN_MAX_LEN);
	ant_memcpy(iot_save.profile_info_st.module_supplier_no, QL_ALI_IOT_APP_MAN_SUPPLIER_ID, strlen(QL_ALI_IOT_APP_MAN_SUPPLIER_ID));
	ant_memcpy(&ql_ali_iot_app_ctx.app_save_info,&iot_save,sizeof(ql_ali_iot_app_save_info_str));

	
    QL_ALI_IOT_APP_LOG("ret:%d, env=%d, sn=%s, msn=%s",ret,iot_save.profile_info_st.env,
						iot_save.terminal_id, iot_save.profile_info_st.module_supplier_no);
	
	if (!(exec_mark & 0x01))
	{
		ret = alipay_iot_sdk_set_profile(&iot_save.profile_info_st);
	    QL_ALI_IOT_APP_LOG("ret:%d", ret);
	    if(0 != ret)
	    {
			return ret;
	    }
		exec_mark |=0x01;
	}
	
	if (!(exec_mark & 0x02)){
		ret = alipay_iot_coll_cache_open(DATA_QUEUE_TYPE_MDAP,QL_ALI_IOT_SDK_QUEUE_L1_SIZE,QL_ALI_IOT_SDK_QUEUE_L2_SIZE);
	    if(0 != ret) return ret;
		exec_mark |=0x02;
	}

    QL_ALI_IOT_APP_LOG("mark=%x, ret:%d",exec_mark,ret);
    if(0 == ret)
    {
		QL_ALI_IOT_APP_LOG("alipay_iot_start...");
        g_ql_ali_iot_sdk_task_id = alipay_iot_start(priority,ALIPAY_SDK_MODE_NORMAL);
		QL_ALI_IOT_APP_LOG("alipay_iot_start end");
		ret = (g_ql_ali_iot_sdk_task_id > 0) ? 0:-1;
    }
	
    QL_ALI_IOT_APP_LOG("ret:%d, g_ql_ali_iot_sdk_task_id=%d",ret,g_ql_ali_iot_sdk_task_id);
	return ret;
}


void ql_ali_iot_sdk_create_and_run(void)
{
    int ret = 0;
	QL_ALI_IOT_APP_LOG("");
    ret = ql_ali_iot_sdk_create(QL_ALI_IOT_SDK_TASK_PRIORITY);//sdk创建
    //设置定时器，防止sdk的时间接口无响应
    QL_ALI_IOT_APP_LOG("ret=%d",ret);
	if(0 == ret)
    {
        ql_ali_iot_start_timer(9, 15000, ql_ali_iot_timer_expired);
    }
	else
	{
		ql_ali_iot_sdk_open_notify(QL_ALI_IOT_RESULT_FAIL);
		/*clear alipay state*/
		ql_ali_iot_reset_sdk_sta();
	}
}

void ql_ali_iot_sdk_thread(void* para)
{
    int srcID = 0;
    int msgID = 0;
    void * data = NULL;
    do
    {
        QL_ALI_IOT_APP_LOG("msg rt<<");
        int rt = ql_ali_iot_task_recevie_message(&srcID, &msgID, &data);
        QL_ALI_IOT_APP_LOG("msg rt= %d",rt);
        if (rt < 0)
        {
            continue;
        }
        QL_ALI_IOT_APP_LOG("got msgID= %d",msgID);
        switch(msgID)
        {
            case QL_ALI_IOT_LOCAL_MSG_SDK_START:/*999*/
                ql_ali_iot_sdk_create_and_run();
            break;
            case QL_ALI_IOT_LOCAL_MSG_TIMER_EXPIRED:/*998 Timeout process*/
                ql_ali_iot_iot_timer_expired_in_task(data);
            break;
            case MSG_ID_ALIPAY_SYS_TIME_UPDATE_IND:/*310* Update time*/
                ql_ali_iot_sdk_sys_time_update_ind(data);
            break;
            case MSG_ID_ALIPAY_SDK_INIT_RSP:/*302 Activated success*/
                ql_ali_iot_sdk_init_rsp(data);
            break;
            case MSG_ID_ALIPAY_SDK_NW_INIT_RSP:/*304 Network communication initialization completed*/
                ql_ali_iot_sdk_nw_init_rsp(data);
            break;
            case MSG_ID_ALIPAY_SDK_EVENT_NOTIFY_IND:/*311*/
                ql_ali_iot_sdk_event_notify(data);
            break;
            case MSG_ID_ALIPAY_SDK_CLOSE_CNF:
                ql_ali_iot_sdk_close_cnf(data);
            break;
            break;
        }
        QL_ALI_IOT_APP_LOG("got msgID %d exit.",msgID);
    }while(1);
}


int ql_ali_iot_start_sdk(void)
{
	int ret =0;
	QL_ALI_IOT_APP_LOG( "");
	//系统适配层初始化动作，不同系统不同
	ret = ant_task_create(ql_ali_iot_sdk_thread,"ali_app", 32*1024, sizeof(ant_msg), 20, NULL,QL_ALI_IOT_APP_TASK_PRIORITY);
	//注意stack不足可能导致崩溃
	QL_ALI_IOT_APP_LOG("demo_task_id %d", ret);
	if(ret > 0)
	{
		g_ql_ali_iot_app_server_task_id = ret;
	    ant_task_sleep_ms(500);
    	QL_ALI_IOT_APP_LOG("alipay app start send msg ....");
	    ql_ali_iot_task_send_message(g_ql_ali_iot_app_server_task_id, -1, QL_ALI_IOT_LOCAL_MSG_SDK_START, NULL);
		QL_ALI_IOT_APP_LOG("alipay app start send msg end");
		ret = 0;
	}
	else
	{
		ret = -1;
		QL_ALI_IOT_APP_LOG("task fail.");
	}
	return ret;
}


int ql_ali_iot_get_ver(char *version, char* build_time, int size)
{
    int32_t ret = 0;
	snprintf(version, size, "\"%s\"", ant_system_version_get());
	snprintf(build_time, size, "\"%s\"", ant_build_date_time());
    QL_ALI_IOT_APP_LOG("version=%s, build_time=%s\n", version, build_time);
	return ret;
}


int ql_ali_iot_start_open()
{
    int32_t ret = 0;
	if(ant_gprs_get_network_status() != NETWORK_REGISTRED)
	{
		return QL_ALI_IOT_RESULT_NETWORK_ERR;
	}
	

	if(QL_ALI_IOT_SDK_RUN == ql_ali_iot_app_ctx.running)
	{
		return 0;
	}
	
	if(QL_ALI_IOT_SDK_SHUT == ql_ali_iot_app_ctx.running)
	{
		return QL_ALI_IOT_RESULT_REBOOT;/*Need to power off and restart operation*/
	}
	
	ql_ali_iot_app_ctx.running = QL_ALI_IOT_SDK_RUN;

	ret = ql_ali_iot_start_sdk();
	if(ret != 0)
	{
		ql_ali_iot_app_ctx.running = QL_ALI_IOT_SDK_IDLE;
		ret = -1;
	}
	return ret;
}

/*AT+ALIPAYSHUT*/
int ql_ali_iot_close_shut(void *arg)
{
    int32_t ret = 0;
    QL_ALI_IOT_APP_LOG("");

	if((QL_ALI_IOT_SDK_RUN == ql_ali_iot_app_ctx.running)||(g_ql_ali_iot_sdk_task_id > 0))
	{
		ql_ali_iot_app_ctx.running = QL_ALI_IOT_SDK_SHUT;		
	
		ql_ali_iot_sdk_manage_sdk_close(g_ql_ali_iot_sdk_task_id);
		//g_ql_ali_iot_sdk_task_id = 0;/*alipay SDK  maybe destroyed task*/
		/*
		*目前alipay SDK 该接口是空的, 执行后上层需屏蔽不可再操作AT
		*At present, the SHUT interface of Alipay SDK is empty. 
		*After execution, the upper layer needs to be shielded and cannot be operated AT.
		*/
	}
	return ret;
}


ql_ali_iot_app_result_code_e ql_ali_iot_app_get_did(char *did,int size)
{
    int32_t ret = QL_ALI_IOT_RESULT_OK;
	char *biztid=NULL;
	if(did == NULL || size < 64)
	{
		QL_ALI_IOT_APP_LOG("did param null!");
		return QL_ALI_IOT_RESULT_PARAM_ERR;
	}

	biztid = alipay_iot_get_biztid();//alipay_iot_get_biztid()必须在sdk create出来之后调用
	QL_ALI_IOT_APP_LOG("biztid after sdk init. %s",biztid == NULL ? "null" : biztid);
	if(biztid != NULL)
	{
		ant_memset(did,0,size);
		ant_memcpy(did,biztid,strlen(biztid));
	}
	else
	{
		ret = QL_ALI_IOT_RESULT_FAIL;
	}
    alipay_iot_free_biztid(biztid);
    biztid = NULL;
	return ret;
}


int ql_ali_iot_app_set_sign(ql_ali_iot_app_paysign_str *paysign,char *result,int* len_result)
{
    int32_t ret = ALIPAY_IOT_SIGN_RV_UNKNOWN;
	
    //注意可能出现内存超出的情况; 假定输入长度是X（X包含金额amount长度、qr_code长度），最终长度应该是512+X
    
	if(NULL == paysign || len_result == NULL || result == NULL) 
	{
		return ALIPAY_IOT_SIGN_RV_PARAM_ERROR;
	}
	int buf_len = *len_result;
	
	memset(result, 0, *len_result);
    QL_ALI_IOT_APP_LOG("mode=%d,amount=%s,need_sign_data=%s",paysign->mode,paysign->amount,paysign->need_sign_data);
	switch(paysign->mode)
	{
        case QL_ALI_IOT_SIGN_MODE_RECV_QRCODE:/*1 用户被扫(反扫)*/
			ret = alipay_iot_sign_qrcode(paysign->need_sign_data, paysign->amount, result, len_result);
        break;
        case QL_ALI_IOT_SIGN_MODE_FACE:/*2 用户人脸*/
			ret = alipay_iot_sign_face_token(paysign->need_sign_data, paysign->amount, result, len_result);
        break;
        case QL_ALI_IOT_SIGN_MODE_QRCODE:/*3 用户主扫*/
			ret = alipay_iot_sign_recv_qrcode(paysign->need_sign_data, paysign->amount, result, len_result);
        break;
        case QL_ALI_IOT_SIGN_MODE_DELEGATE:/*4 第三方代扣*/
			ret = alipay_iot_sign_delegated(paysign->need_sign_data, paysign->amount, result, len_result);
        break;
		default:
			ret = ALIPAY_IOT_SIGN_RV_PARAM_ERROR;
			break;
	}
    ant_assert(*len_result < buf_len);/*Data overflow*/	
    QL_ALI_IOT_APP_LOG("ret %d, buf_len %d; len_result %d",ret,buf_len,*len_result);
    QL_ALI_IOT_APP_LOG("result %s",result);
	return ret;
}


alipay_iot_biz_result_code_t ql_ali_iot_app_set_info(ql_ali_iot_app_payinfo_str *payinfo)
{
	char iccid_buf[64]={0};
	int32_t ret = alipay_iot_biz_result_succ;

	if(ant_gprs_get_network_status() != NETWORK_REGISTRED)
	{		
		QL_ALI_IOT_APP_LOG("ali iot app payinfo network not activate!");
		return alipay_iot_biz_result_para_err;
	}
	if(NULL == payinfo)
	{
		QL_ALI_IOT_APP_LOG("payinfo param null!");
		return alipay_iot_biz_result_para_err;
	}

	alipay_iot_production_info_struct pd_info = {0};
	pd_info.human_verify = payinfo->human_verify;
	
	ant_system_get_device_info(ANT_DEVICE_ICCID, iccid_buf, 64);
	snprintf(pd_info.account_flow, sizeof(pd_info.account_flow), "%s",iccid_buf);
	ret = snprintf(pd_info.iccid, sizeof(pd_info.iccid), "%s", iccid_buf);
	if(ret < 0)
	{
		QL_ALI_IOT_APP_LOG("ret:%d", ret);
	}

	ret = alipay_iot_coll_production_info_send(&pd_info);
	
	QL_ALI_IOT_APP_LOG("ret %d, account_flow=%s, iccid=%s",ret,pd_info.account_flow, pd_info.iccid);
	return ret;
}

alipay_iot_biz_result_code_t ql_ali_iot_app_set_rep(ql_ali_iot_app_payrep_str *payrep)
{
	alipay_iot_biz_result_code_t ret = alipay_iot_biz_result_para_err;
	alipay_iot_transaction_data_struct pd_rep = {0};

	if(ant_gprs_get_network_status() != NETWORK_REGISTRED)
	{
		QL_ALI_IOT_APP_LOG("ali iot app payrep network not activate!");
		return alipay_iot_biz_result_para_err;
	}

	if(NULL == payrep)
	{
		QL_ALI_IOT_APP_LOG("payrep param null!");
		return alipay_iot_biz_result_para_err;
	}
	
	pd_rep.amount = payrep->amount;          
	pd_rep.timestamp = payrep->timestamp;
	pd_rep.time_consuming = payrep->time_consuming;
	pd_rep.fail_reason = payrep->trade_result;
	snprintf(pd_rep.business_no, sizeof(pd_rep.business_no), "%s", payrep->business_no);
	snprintf(pd_rep.qr_code, sizeof(pd_rep.qr_code), "%s", payrep->qr_code);
	ret = alipay_iot_coll_transaction_data_send_rt(&pd_rep);
	QL_ALI_IOT_APP_LOG("ret %d, amount=%d, business_no=%s, qr_code=%s",ret,pd_rep.amount,pd_rep.business_no,pd_rep.qr_code);
	return ret;
}

ql_ali_iot_app_result_code_e ql_ali_iot_app_get_utc(UINT32 *seconds)
{
	if(NULL == seconds)
	{
		QL_ALI_IOT_APP_LOG("seconds param null!");
		return QL_ALI_IOT_RESULT_PARAM_ERR;
	}
	
	*seconds = ant_get_utc_seconds();
	QL_ALI_IOT_APP_LOG("utc_seconds=%lu",*seconds);
	return QL_ALI_IOT_RESULT_OK;
}

/*
 alipay iot sdk 获取 sdk 的版本以及 编译的时间
*/

void ql_ali_iot_app_get_sdk_ver_buildtime(char *version, char* build_time, int size)
{
	snprintf(version, size, "\"%s\"", ant_system_version_get());
	snprintf(build_time, size, "\"%s\"", ant_build_date_time());
    QL_ALI_IOT_APP_LOG("ali iot sdk ver=%s,build_time=%s!", version, build_time);
}


alipay_iot_biz_result_code_t ql_ali_iot_app_set_act(  ql_ali_iot_app_payact_str *payact)
{
    int32_t ret = alipay_iot_biz_result_succ;	
	alipay_iot_action_data_struct action = {0};
	if(ant_gprs_get_network_status() != NETWORK_REGISTRED)
	{
		QL_ALI_IOT_APP_LOG("ali iot app payact network not activate!");
		return alipay_iot_biz_result_para_err;
	}
	
	if(NULL == payact )
	{
		QL_ALI_IOT_APP_LOG("payact param null!");
		return alipay_iot_biz_result_para_err;
	}
	
	action.brocast_count = payact->broadcast_count;
	action.scan_count = payact->scan_count;
	ret = alipay_iot_coll_action_data_send(&action);
	QL_ALI_IOT_APP_LOG("ret=%d,brocast_count=%d, scan_count=%d",ret,action.brocast_count, action.scan_count);
	return ret;
}



int ql_ali_iot_app_net_register(uint8_t nSim,int profile_idx)
{
    int ret = 0;
    ql_data_call_info_s info ={0};

    for (int i = 0; i < 10; i++)
    {
        ret = ql_network_register_wait(nSim, 120);
        if (0 == ret)
        {
            break;
        }
        ql_rtos_task_sleep_s(1);
    }

    if (0 != ret)
    {
        QL_ALI_IOT_APP_LOG("network register failure!");
        return -1;
    }

    ret = ql_set_data_call_asyn_mode(nSim, profile_idx, 0);
    QL_ALI_IOT_APP_LOG("network datacall asyn mode ret = %d",ret);

    ql_start_data_call(nSim, profile_idx, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0);
    QL_ALI_IOT_APP_LOG("network datacall result ret = %d",ret);
    if (0 != ret)
    {
        QL_ALI_IOT_APP_LOG("data call failure!");
    }

    ret = ql_get_data_call_info(nSim, profile_idx, &info);
    if (0 != ret)
    {
        ql_stop_data_call(nSim, profile_idx);
        return -1;
    }
    QL_ALI_IOT_APP_LOG("ql_get_data_call_info ret: 0x%x", ret);
    QL_ALI_IOT_APP_LOG("info.profile_idx: %d, info.ip_version: %d", info.profile_idx, info.ip_version);
	QL_ALI_IOT_APP_LOG("info->v4.state: %d, info.v6.state: %d", info.v4.state, info.v6.state);
    if(info.v4.state)
    {
		QL_ALI_IOT_APP_LOG("info.v4.addr.ip: %s", ip4addr_ntoa(&(info.v4.addr.ip)));
		QL_ALI_IOT_APP_LOG("info.v4.addr.pri_dns: %s", ip4addr_ntoa(&(info.v4.addr.pri_dns)));
		QL_ALI_IOT_APP_LOG("info.v4.addr.sec_dns: %s", ip4addr_ntoa(&(info.v4.addr.sec_dns)));
    }
    if(info.v6.state)
    {
		QL_ALI_IOT_APP_LOG("info.v6.addr.ip: %s", ip6addr_ntoa(&(info.v6.addr.ip)));
		QL_ALI_IOT_APP_LOG("info.v6.addr.pri_dns: %s", ip6addr_ntoa(&(info.v6.addr.pri_dns)));
		QL_ALI_IOT_APP_LOG("info.v6.addr.sec_dns: %s", ip6addr_ntoa(&(info.v6.addr.sec_dns)));
    }
	
    return ret;
}

static void ql_ali_iot_app_task_id_fxn(void * arg)
{
	int ret = 0;
	ql_event_t event = {0};
	QL_ALI_IOT_APP_LOG("=========>>>> ql ali iot sdk demo begin!");
	ql_rtos_task_sleep_s(10);
	ql_ali_iot_app_net_register(0,1);

	ret = ql_ali_iot_start_open();
	QL_ALI_IOT_APP_LOG("=========>>>> ali iot app  open: %d!",ret);
	if(ret != 0)
	{
		goto EXIT;	
	}
	QL_ALI_IOT_APP_LOG("=========>>>> wait ali iot sdk open...!");
	ql_event_try_wait(&event);
	if(event.id == QL_ALI_IOT_SDK_OPEN_EVENT_ID)
	{
		if(event.param1 != 0)
		{
			QL_ALI_IOT_APP_LOG("=========>>>> ali iot sdk open fail %d!",event.param1);
			goto EXIT;
		}
		else 
		{
			UINT32 seconds = 0;
			int temp_buf_size = 64;
			ql_ali_iot_app_result_code_e  app_err = QL_ALI_IOT_RESULT_OK;
			ALIPAY_IOT_SIGN_RET	iot_sign_ret = ALIPAY_IOT_SIGN_RV_OK;
			alipay_iot_biz_result_code_t result_code = alipay_iot_biz_result_succ;

			QL_ALI_IOT_APP_LOG("=========>>>> ali iot sdk open success...!");

			char* temp_buf_1 = ant_malloc(temp_buf_size+1);
			char* temp_buf_2 = ant_malloc(temp_buf_size+1);
			if(temp_buf_1 == NULL || temp_buf_2 == NULL)
			{
				QL_ALI_IOT_APP_LOG("=========>>>> ali iot sdk demo malloc fail!");
				if(temp_buf_1 != NULL)
				{
					ant_free(temp_buf_1);
				}
				if(temp_buf_2 != NULL)
				{
					ant_free(temp_buf_2);
				}
				goto EXIT;
			}
			
			ant_memset(temp_buf_1,0,temp_buf_size);
			ant_memset(temp_buf_2,0,temp_buf_size);
			
			ql_ali_iot_app_payinfo_str payinfo = {0};
			ql_ali_iot_app_paysign_str paysign = {0};
			ql_ali_iot_app_payrep_str payrep = {0};
			ql_ali_iot_app_payact_str payact = {0};

			ql_ali_iot_app_get_sdk_ver_buildtime(temp_buf_1,temp_buf_2,temp_buf_size);
			QL_ALI_IOT_APP_LOG("=========>>>> ali iot sdk demo build version: %s;build time: %s",temp_buf_1,temp_buf_2);
			ql_rtos_task_sleep_s(3);
			ant_memset(temp_buf_1,0,temp_buf_size);
			app_err = ql_ali_iot_app_get_did(temp_buf_1,temp_buf_size);
			QL_ALI_IOT_APP_LOG("=========>>>> ali iot sdk demo get biztid ret :%d;biztid: %s",app_err,temp_buf_1);
			ql_rtos_task_sleep_s(5);
			
			paysign.mode = 4;
			ant_memcpy(paysign.need_sign_data, "1234567890", strlen("1234567890"));
			ant_memcpy(paysign.amount, "10.25",strlen("10.25"));
			ant_memset(temp_buf_1,0,temp_buf_size);
			
			QL_ALI_IOT_APP_LOG("paysign.mode: %d",paysign.mode);
			QL_ALI_IOT_APP_LOG("paysign.need_sign_data :%s",paysign.need_sign_data);
			QL_ALI_IOT_APP_LOG("paysign.amount :%s",paysign.amount);
			int result_length = sizeof(ql_ali_iot_app_paysign_str)+1000;
			char* result = ant_malloc(result_length);
			iot_sign_ret = ql_ali_iot_app_set_sign(&paysign,result,&result_length);
			QL_ALI_IOT_APP_LOG("=========>>>> ali iot sdk demo set sign ret :%d;result length :%d;result: %s",iot_sign_ret,result_length,result);
			ql_rtos_task_sleep_s(5);
			ant_free(result);

			payinfo.human_verify = 1;
			QL_ALI_IOT_APP_LOG("payinfo.human_verify :%d",payinfo.human_verify);
			result_code = ql_ali_iot_app_set_info(&payinfo);
			QL_ALI_IOT_APP_LOG("=========>>>> ali iot sdk demo set payinfo result_code :%d",result_code);
			ql_rtos_task_sleep_s(3);

			
			memcpy(payrep.business_no, "698888", strlen("698888"));
			memcpy(payrep.qr_code, "95765",strlen("95765"));
			payrep.amount = 1678;
			payrep.time_consuming = 8;
			payrep.trade_result = 5;
			payrep.timestamp = 55555;

			QL_ALI_IOT_APP_LOG("payrep.business_no :%s",payrep.business_no);
			QL_ALI_IOT_APP_LOG("payrep.qr_code :%s",payrep.qr_code);
			QL_ALI_IOT_APP_LOG("payrep.amount :%d",payrep.amount);
			QL_ALI_IOT_APP_LOG("payrep.time_consuming :%d",payrep.time_consuming);
			QL_ALI_IOT_APP_LOG("payrep.trade_result :%d",payrep.trade_result);
			QL_ALI_IOT_APP_LOG("payrep.timestamp :%lu",payrep.timestamp);
			ant_memset(temp_buf_1,0,temp_buf_size);
			result_code = ql_ali_iot_app_set_rep(&payrep);
			QL_ALI_IOT_APP_LOG("=========>>>> ali iot sdk demo set rep result_code :%d",result_code);
			ql_rtos_task_sleep_s(3);

			ql_ali_iot_app_get_utc(&seconds);
			QL_ALI_IOT_APP_LOG("=========>>>> ali iot sdk demo get utc seconds :%d",seconds);
			ql_rtos_task_sleep_s(3);


			payact.broadcast_count = 1;
			payact.scan_count = 1;
			QL_ALI_IOT_APP_LOG("payact.broadcast_count :%d",payact.broadcast_count);
			QL_ALI_IOT_APP_LOG("payact.scan_count :%d",payact.scan_count);
			result_code = ql_ali_iot_app_set_act(&payact);
			QL_ALI_IOT_APP_LOG("=========>>>> ali iot sdk demo set act result_code :%d",result_code);
			ql_rtos_task_sleep_s(10);
			
			ant_free(temp_buf_1);
			ant_free(temp_buf_2);
		}

	}
EXIT:
	ql_rtos_task_delete(ql_ali_iot_app_task_id);
	ql_rtos_task_sleep_s(10);
	QL_ALI_IOT_APP_LOG("=========>>>>ali iot sdk demo END!");
}

void ql_alipay_iot_sdk_app_init(void)
{
	int ret = 0;
	char tmp_buf[64]={0}; 
	ant_network_init();
	ant_iot_kv_init();
	ant_gprs_init();
	QlOSStatus err = QL_OSI_SUCCESS;
	QL_ALI_IOT_APP_LOG("========>>>>ql ali iot sdk demo begin!");
	memset(&ql_ali_iot_app_ctx, 0, sizeof(ql_ali_iot_app_ctx_str));
	ql_ali_iot_app_ctx.running = QL_ALI_IOT_SDK_IDLE;
	ql_ali_iot_app_ctx.clear_sdk_cache = FALSE;
	ret = ant_timer_init();
	if(0 != ret)
	{
		QL_ALI_IOT_APP_LOG("ant timer init fail!");
		ql_ali_iot_app_ctx.running = QL_ALI_IOT_SDK_SHUT;
		return ;
	}
	memset(tmp_buf, 0, 64);
	ret = ant_gprs_get_imsi(tmp_buf, 64);
	if(0 == ret)
	{
		strcpy(ql_ali_iot_app_ctx.imei, tmp_buf);
	}
	
	memset(tmp_buf, 0, 64);
	ret = ant_gprs_get_sn(tmp_buf, 64);
	if(0 == ret)
	{
		strcpy(ql_ali_iot_app_ctx.sn, tmp_buf);
	}
	
    err = ql_rtos_task_create(&ql_ali_iot_app_task_id, 64*1024, QL_ALI_IOT_APP_TASK_PRIORITY-2, "QALIAPP", ql_ali_iot_app_task_id_fxn, NULL, 5);
	if(err != QL_OSI_SUCCESS)
    {
		QL_ALI_IOT_APP_LOG("ali iot app task: %d!",err);
		return ;
	}
	QL_ALI_IOT_APP_LOG("ali iot app task ret: %d!", err);
}
