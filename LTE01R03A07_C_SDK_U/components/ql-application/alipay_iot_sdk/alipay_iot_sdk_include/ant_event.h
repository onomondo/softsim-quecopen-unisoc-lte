/**
 * ant_event.h
 *
 * 绯荤粺浜嬩欢鎺ュ彛
 */
#ifndef __ANT_EVENT_H__
#define __ANT_EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "ant_typedef.h"
#include "ant_os.h"

#define ANT_MSG_ID_IDX_BASE 0
/**
 * 事件定义
 */
typedef enum {
    ANT_EVENT_POWERON = ANT_MSG_ID_IDX_BASE,
    ANT_EVENT_SIMCARD_READY,                  // 1
    ANT_EVENT_KEY,                            // 2
    ANT_EVENT_KEY_FUNC_LONG_PRESSED,          // 3
    ANT_EVENT_KEY_COMB_FUNC_LONG_PRESSED,     // 4
    ANT_EVENT_AUDIO_PLAY_REQUEST,             // 5
    ANT_EVENT_AUDIO_PLAY_FINISHED,            // 6
    ANT_EVENT_AUDIO_STOP_REQUEST,              // 7 
    ANT_EVENT_NETWORK,                        // 8
    ANT_EVENT_NETWORK_SERVICE,                // 9  
    ANT_EVENT_NVRAM_REQ,                      // 10
    ANT_EVENT_NVRAM_RSP,                      // 11
    ANT_EVENT_DOWNLOAD_AUDIO,                 // 12
    ANT_EVENT_DOWNLOAD_ORDER,                 // 13
    ANT_EVENT_TIMESYNC,                       // 14
    ANT_EVENT_TIMESYNC_FINISH,                // 15
    ANT_EVENT_OTA,                            // 16
    ANT_EVENT_STAT_CHECK,                     // 17
    ANT_EVENT_IOT_SDK_INIT_REQ,               // 18
    ANT_EVENT_IOT_SDK_EVENT,                  // 19
    ANT_EVENT_ALIPAY_IOT_EVENT,               // 20
    // battery&charger management
    ANT_EVENT_BATTERY_CHARGE_IN,              // 21
    ANT_EVENT_BATTERY_CHARGE_OUT,             // 22
    ANT_EVENT_BATTERY_CHARGE_COMPLETE,        // 23
    ANT_EVENT_BATTERY_CHARGE_LOW_CURRENT,     // 24
    ANT_EVENT_BATTERY_CHARGE_OVER_CURRENT,    // 25
    ANT_EVENT_BATTERY_CHARGE_LOW_TEMPERATURE, // 26
    ANT_EVENT_BATTERY_CHARGE_OVER_TEMPERAURE, // 27
    ANT_EVENT_BATTERY_CHARGE_OVER_VOLTAGE,    // 28
    ANT_EVENT_BATTERY_CHARGE_STATUS,          // 29
    ANT_EVENT_USB_INSERT_STATUS,			  // 30
    // exception event
    ANT_EVENT_FS_CRASH,                       // 31
    ANT_EVENT_MEM_FAIL,                       // 32
    ANT_EVENT_MOS,                            // 33
    ANT_EVENT_LED_BLINK,                      // 34
    ANT_EVENT_FACTORY_RESET,                 // 35
    ANT_EVENT_BREAK_OFF_SHUTDOWN,             // 36
    ANT_EVENT_IDLE_EVENT,
    ANT_EVENT_VOICE_PAC_CLEAN,
    ANT_EVENT_END
} ant_event_e;

/**
 * 按键键值枚举
 */
typedef enum {
    ANT_KEY_POWER  = 0,
    ANT_KEY_FUNC,
    ANT_KEY_VOLDN,
    ANT_KEY_VOLUP,
    ANT_KEY_RESET,
    ANT_KEY_UNDEF
} ant_keycode_e;

/**
 * 按键类型枚举
 */
typedef enum {
    ANT_KEYTYPE_PRESS       = 0,
    ANT_KEYTYPE_RELEASE,
    ANT_KEYTYPE_LONG_PRESS,
    ANT_KEYTYPE_REPEAT,
    ANT_KEYTYPE_UNDEF
} ant_keytype_e;

/**
 * 按键结构体定义
 */
typedef struct {
    ant_keycode_e code;
    ant_keytype_e type;
} ant_key_event;

typedef struct{
    ant_u32 start;
    ant_u32 cnt;
    ant_u32 mode;
}ant_box_idle_t;
/**
 * 消息事件结构体定义
 */
typedef struct {
    ant_u32 message;           // ant message id
    ant_ptr param1;            // user defined value
    ant_ptr param2;            // user defined value
    ant_ptr param3;            // user defined value
    ant_ptr param4;            // user defined value
    ant_task_t src_task_id;    // task which send message
    const char *src_task_name; // name of task which send message
} ant_msg;

ant_bool ant_event_init(void);

/**
 * 订阅事件
 *
 * @param msg_id 订阅事件，取值参见ant_event_e
 * @return  0： 订阅成功
 *         -1： 订阅失败
 */
ant_s32 ant_event_subscribe(ant_u32 msg_id);

/**
 * 取消订阅事件
 *
 * @param msg_id 订阅事件，取值参见ant_event_e
 * @return  0： 取消订阅成功
 *         -1： 取消订阅失败
 */
ant_s32 ant_event_unsubscribe(ant_u32 msg_id);

/**
 * 广播事件。订阅了事件id的模块（任务）都会接收到该事件
 *
 * @param msg 广播事件对象
 */
void    ant_event_publish(ant_msg *msg);

/**
 * 当前线程接收事件。接收到事件之前一直阻塞。
 *
 * @param msg 返回接收到的事件
 *
 * @return  0 接收消息超时
 *          1 接收消息成功
 *         -1 接收消息失败
 *         参考 ant_que_sem_status_e
 *          

 */
ant_s32 ant_event_recv_msg(ant_msg *msg);

/**
 * 当前线程接收事件。
 * 
 * @param msg      返回接收到的事件
 * @param timeout  接收超时时间, 单位为毫秒, ANT_WAIT_FOREVER(-1)表示永久等待
 *
 * @return  0 接收消息超时
 *          1 接收消息成功
 *         -1 接收消息失败
 *         参考 ant_que_sem_status_e
 *          
 */ 
ant_s32 ant_event_recv_msg_timeout(ant_msg *msg, ant_s32 timeout);

/**
 * 向指定任务发送事件
 * 
 * @param dst_task_id 任务ID（即ant_task_t)
 * @param msg 发送事件的指针
 * @return  0 发送消息超时
 *          1 发送消息成功
 *         -1 发送消息失败
 *         参考 ant_que_sem_status_e
 *          

 */
ant_s32 ant_event_send_msg(ant_u32 dst_task_id, ant_msg *msg);


/**
 * 向指定任务名发送事件
 * 
 * @param task_name 任务名
 * @param msg 发送事件的指针
 * @return  0： 发送事件成功
 *         -1： 发送事件失败
 */
ant_s32 ant_event_send_msg_by_name(ant_char * task_name,ant_msg * msg);


ant_s32 ant_event_from_system(ant_u32 msg_id);
void    ant_event_wait_bootup_completed(void);
const char *ant_event_description(ant_event_e event);


typedef struct message_send_s {
    struct message_send_s *next;
    void *msg;
} message_send_t;

typedef struct {
    void *memory;
    ant_bool *used;
    message_send_t *pending;
    ant_size elem_size;
    ant_u32 elem_num;
} message_elem_t;

typedef struct {
    ant_sem msg_sem;
    ant_sem claim_sem;
    ant_mutex lock;
    message_elem_t elem;
} message_queue_t;

message_queue_t * message_queue_create(ant_size elem_size, ant_u32 elem_num);
void message_queue_destroy(message_queue_t *queue);
void message_queue_send(message_queue_t *queue, void *message, int is_front);
ant_s32 message_queue_recv(message_queue_t *queue, void *message, ant_s32 time_out);


#ifdef __cplusplus
}
#endif

#endif

