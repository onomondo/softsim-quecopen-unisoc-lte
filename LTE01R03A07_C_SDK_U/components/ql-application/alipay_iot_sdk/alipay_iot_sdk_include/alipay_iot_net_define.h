#ifndef alipay_iot_net_define_h
#define alipay_iot_net_define_h

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define ALIPAY_IOT_true  1
#define ALIPAY_IOT_false 0
#define ALIPAY_IOT_null  0
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef char ALIPAY_IOT_bool_t;
/*========================================================================*/
//若编译环境有size_t的定义，可以将IOT_size_t定义为size_t的别名。
typedef unsigned long int ALIPAY_IOT_size_t;
/*========================================================================*/
//若编译环境有uintptr_t的定义，可以将IOT_uintptr_t定义为uintptr_t的别名。
typedef unsigned long int ALIPAY_IOT_uintptr_t;
/*========================================================================*/
//若编译环境有intptr_t的定义，可以将IOT_intptr_t定义为intptr_t的别名。
typedef signed long int ALIPAY_IOT_intptr_t;
/*========================================================================*/
//日志的级别。
typedef enum ALIPAY_IOT_lvl {
    ALIPAY_IOT_LVL_vbs, //啰嗦。
    ALIPAY_IOT_LVL_dbg, //调试。
    ALIPAY_IOT_LVL_inf, //提示。
    ALIPAY_IOT_LVL_wrn, //警告。
    ALIPAY_IOT_LVL_err, //错误。
    ALIPAY_IOT_LVL_ftl  //致命。
} ALIPAY_IOT_LVL_t;
/*========================================================================*/
//连接状态。
typedef enum IOT_state {
    IOT_STATE_broken,   //连接断开。
    IOT_STATE_dns,      //正在查DNS。
    IOT_STATE_linking,  //正在连接。
    IOT_STATE_connected, //已经连上。
} IOT_STATE_t;
//http状态。
typedef enum IOT_HTTP_state {
    IOT_HTTP_state_wait,     //http 等待发送
    IOT_HTTP_state_sended,   //http 已发送
    IOT_HTTP_state_error,    //http 返回错误
    IOT_HTTP_state_succ,     //http 返回成功
} IOT_HTTP_state_t;
/*========================================================================*/
//用于存储字符串或二进制数据。
typedef struct ALIPAY_IOT_bin {
    void      *bin;
    ALIPAY_IOT_size_t len; //bin指向的数据的字节数。
} ALIPAY_IOT_BIN_t;
/*========================================================================*/
//用于记录地址。
typedef struct ALIPAY_IOT_addr {
    unsigned char ip[4];
    unsigned int  port; //范围在[0, 65535]。
} ALIPAY_IOT_ADDR_t;
/*========================================================================*/
//用于记录时间。
typedef struct ALIPAY_IOT_time {
    signed long int sec; //秒数。
    signed int      ms;  //毫秒数。
} ALIPAY_IOT_TIME_t;
/*========================================================================*/
//用于反馈事件。
typedef struct ALIPAY_IOT_event {
    void        *ext;
    ALIPAY_IOT_intptr_t sk;
    ALIPAY_IOT_bool_t   in;  //可读（入参、出参）。
    ALIPAY_IOT_bool_t   out; //可写（入参、出参）。
    ALIPAY_IOT_bool_t   err; //出错（出参）。
} ALIPAY_IOT_EVENT_t;

typedef struct ALIPAY_IOT_OBJ_event {
    ALIPAY_IOT_EVENT_t *event;
    ALIPAY_IOT_size_t   count;
    ALIPAY_IOT_size_t   caret;
} ALIPAY_IOT_OBJ_EVENT_t;
#endif /* alipay_iot_net_define_h */
