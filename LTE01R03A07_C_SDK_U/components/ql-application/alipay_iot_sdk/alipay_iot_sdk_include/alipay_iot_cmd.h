#ifndef __H_ALIPAY_IOT_CMD
#define __H_ALIPAY_IOT_CMD
#include "alipay_iot_type.h"

/*
* 指令通知消息： MSG_ID_ALIPAY_SDK_CMD_IND
* 数据为 alipay_sdk_cmd_ind_struct，释放函数为 alipay_iot_cmd_ind_struct_destory
*/
typedef struct
{
    char    *app_id;        //应用名称，即指令业务类型
    char    *execution_id;  //命令编号，类似token
    char    *ext_info;      //扩展信息
    char    *cmd_content;   //指令内容信息，包含指令名称等
    char    *sign;          //签名数据
    uint64_t cmd_time;      //指令下发服务端时间-毫秒时间戳
}alipay_sdk_cmd_ind_struct;


//释放 alipay_sdk_cmd_ind_struct
void alipay_iot_cmd_ind_struct_destory(alipay_sdk_cmd_ind_struct *ind);

typedef enum
{
    EXEC_STATUS_STEP_SUCCESS = 1,   //当前步骤成功
    EXEC_STATUS_STEP_FAIL    = 2,   //当前步骤失败，不影响其他步骤时用
    EXEC_STATUS_ALL_SUCCESS  = 3,   //当前是最后一步骤，所有步骤都成功
    EXEC_STATUS_ALL_FAIL     = 4,   //当前步骤失败，并且总体执行被认为失败
    EXEC_STATUS_UNKNOWN      = 99
}ALIPAY_IOT_CMD_EXEC_STATUS;

/*
* 指令执行结果报告消息： MSG_ID_ALIPAY_SDK_CMD_CNF
* 数据为 alipay_sdk_cmd_cnf_struct，构造函数为 alipay_iot_cmd_cnf_struct_create
*/
typedef struct
{
    char                          *execution_id;    //命令编号，类似token
    uint32_t                       exec_step;       //指令执行阶段序号
    ALIPAY_IOT_CMD_EXEC_STATUS     exec_status;     //指令执行状态
    uint64_t                       start_time;      //指令执行开始时间，毫秒时间戳
    uint64_t                       end_time;        //指令执行结束时间
    char                          *exec_desc;       //执行结果字符描述
    char                          *sign;            //签名数据
}alipay_sdk_cmd_cnf_struct;

//构造 alipay_sdk_cmd_cnf_struct
alipay_sdk_cmd_cnf_struct* alipay_iot_cmd_cnf_struct_create(char        * execution_id, uint32_t exec_step, ALIPAY_IOT_CMD_EXEC_STATUS exec_status,
    uint64_t start_time, uint64_t end_time,char *exec_desc, char *sign);

void alipay_iot_cmd_cnf_struct_destory(alipay_sdk_cmd_cnf_struct* cnf);

#endif
