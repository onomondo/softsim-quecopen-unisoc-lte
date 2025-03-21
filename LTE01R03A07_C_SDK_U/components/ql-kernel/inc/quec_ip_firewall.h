#ifndef __QUEC_IP_FIREWALL_H_
#define __QUEC_IP_FIREWALL_H_

typedef enum
{
    QUEC_FIREWALL_ENABLE = 0,
    QUEC_TCP_BLACKLIST_CLEAN_TIME= 1,
    QUEC_TCP_RECORD_LIST_CLEAN_TIME,
    QUEC_ICMP_BLACKLIST_CLEAN_TIME,
    QUEC_ICMP_RECORD_LIST_CLEAN_TIME,
    QUEC_TCP_ACCESS_PERCENTAGE,
    QUEC_TCP_INPUT_SCALE = 6,
    QUEC_ICMP_INPUT_SCALE,
    QUEC_ICMP_INPUT_INTERVAL,
    QUEC_SAVE_FILE_LOG_TIME,

    QUEC_FIREWALL_CONFIG_MAX
}quec_firewall_config_type;

typedef enum{
	QUEC_FIREWALL_MIN 		= 1001,
	QUEC_FIREWALL_WRITE 	= 1002,	//保存log到文件
	QUEC_FIREWALL_CLOSE 	= 1003,	//关闭定时器
	QUEC_FIREWALL_TIME_SET 	= 1004,	//关闭定时器
	QUEC_FIREWALL_PORT_SET	= 1005,	//清空port记录
	QUEC_FIREWALL_DNS_DELAY	= 1006,	//清空DNS允许记录
}quec_firewall_msg_id;

typedef struct
{
	int socket_fd;	//socketfd
	int src_port;
	int dest_port;
	char remote_ip[16];
	char local_ip[16];
}quec_tcp_pcb;

void quec_firewall_set_config(quec_firewall_config_type type, int value);
int quec_firewall_get_config(quec_firewall_config_type type);
int quec_firewall_set_local_port(int s, int port);
void quec_firewall_dns_flag_set(int flag);
int quec_firewall_dns_flag_get(void);
void quec_firewall_tcp_pcb_set_local(int socket_fd, int protocol, int port, char *ip_addr);
void quec_firewall_tcp_pcb_set_remote(int socket_fd, int protocol, int port, char *ip_addr);
void quec_firewall_tcp_pcb_clear(int socket_fd);
quec_tcp_pcb *quec_firewall_tcp_pcb_get(int socket_fd);
int quec_firewall_get_enable(void);

void quec_firewall_delete(int s, int err);
void quec_firewall_shut_delete(int s, int err, int how);
void quec_firewall_remove_pcb(int socket_fd);

#endif /*__QUEC_IP_FIREWALL_H_*/
