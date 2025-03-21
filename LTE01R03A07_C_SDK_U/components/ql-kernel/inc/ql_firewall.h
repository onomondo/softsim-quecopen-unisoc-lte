#ifndef __QL_FIREWALL_H__
#define __QL_FIREWALL_H__

/*
RTOS 基于LWIP的IDS检测原理简介:

数据处理位置:
    在modem网络与lwip input数据入口处添加IDS检测逻辑，符合规则的数据包将会送给lwip继续处理

检测逻辑简介:
    一、攻击检测会自动过滤非法数据包如：
    1.回环IP地址攻击
    2.源端口为0的攻击行为
    3.TCP非法Flags攻击

    二、黑白名单处理:
    1.白名单:上层应用层开启用到的端口号会加入访问白名单数据送入后可以提交给lwip
    2.黑名单:未使用的端口号被外网访问则在满足一定门限之后加入到黑名单并记录访问日志

    三、门限简介:
    QL_TCP_BLACKLIST_CLEAN_TIME      TCP黑名单自动清空定时时间
    QL_TCP_RECORD_LIST_CLEAN_TIME    TCP访问IPlist表清除时间
    QL_ICMP_BLACKLIST_CLEAN_TIME     ICMP黑名单自动清空定时时间
    QL_ICMP_RECORD_LIST_CLEAN_TIME   ICMP访问IPlist表清除时间
    QL_TCP_ACCESS_PERCENTAGE         防止端口扫描正确访问端口号百分比（现有原理上慢速攻击无法检测 eg:1s一次端口扫描）
    QL_ICMP_INPUT_INTERVAL           ICMP风暴攻击间隔百分比（现有原理上慢速攻击无法检测 eg:500ms一次ICMP）
*/

typedef enum
{
    QL_TCP_FIREWALL_ENABLE = 0,
    QL_TCP_BLACKLIST_CLEAN_TIME= 1,
    QL_TCP_RECORD_LIST_CLEAN_TIME,
    QL_ICMP_BLACKLIST_CLEAN_TIME,
    QL_ICMP_RECORD_LIST_CLEAN_TIME,
    QL_TCP_ACCESS_PERCENTAGE,
    QL_TCP_INPUT_SCALE = 6,
    QL_ICMP_INPUT_SCALE,
    QL_ICMP_INPUT_INTERVAL,

    QL_FIREWALL_CONFIG_MAX
}ql_firewall_config_type;

/*****************************************************************
* Function: ql_firewall_get_config
*
* Description: 获取firewall检测门限值
*
* Parameters:
*   config		模式
* Return:
*  int			firewall检测门限值
*
*****************************************************************/
int ql_firewall_get_config(ql_firewall_config_type config);


/*****************************************************************
* Function: ql_firewall_set_config
*
* Description: 配置firewall检测门限值
*
* Parameters:
*   config		模式
*   value       要调整的数值
* Return:
*  void			无	
*
*****************************************************************/
void ql_firewall_set_config(ql_firewall_config_type config, int value);

/*****************************************************************
* Function: ql_firewall_enable
*
* Description: 启动firewall功能，默认只需要调用enable即可，后台均有以调试好的默认配置
*              无需调用config进行相关配置。
*
* Parameters:
*  enbale      0   关闭IDS检测防攻击功能
*              1   开启IDS检测攻击功能
* Return:
*  void
*
*****************************************************************/
void ql_firewall_enable(int enable);

#endif
