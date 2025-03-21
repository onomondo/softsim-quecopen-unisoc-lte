#ifndef __ALIPAY_NET_KAL_H__
#define __ALIPAY_NET_KAL_H__

#include "alipay_common.h"


typedef struct 
{
    long tv_sec;
    long tv_usec;
} alipay_iot_timeval;


/*
* @brief: 获取UTC时间
* note:  1.精确级别,微妙级别
*        2.受系统时间修改影响
*        3.返回的秒数是从1970年1月1日0时0分0秒开始
*/
EXTERNC void alipay_iot_gettimeofday(alipay_iot_timeval *now);

/*
* @brief: 使任务休眠milliseconds毫秒
* @input: milliseconds 休眠的时间，单位毫秒
*/
EXTERNC void alipay_task_sleep(uint32_t ms);

typedef void * alipay_iot_mutex;
/*
* @brief: 创建互斥量
* @input: mutex_name 互斥量的名称
* @return: 其他 创建成功
*          NULL 创建失败
*/
EXTERNC alipay_iot_mutex alipay_iot_mutex_create(const char* mutex_name);

/*
* @brief: 对互斥量加锁
* @input: mutex 要加锁的互斥量
* @return: 0 加锁成功
*          -1 加锁失败
*/
EXTERNC int alipay_iot_mutex_lock(alipay_iot_mutex mutex);

/*
* @brief: 对互斥量解锁
* @input: mutex 要解锁的互斥量
* @return: 0 解锁成功
*          -1 解锁失败
*/
EXTERNC int alipay_iot_mutex_unlock(alipay_iot_mutex mutex);

/*
* @brief: 删除互斥量
* @input: mutex 要删除的互斥量
* @return: 0 删除成功
*          -1 删除失败
*/
int alipay_iot_mutex_delete(alipay_iot_mutex mutex);


/*
 * Protocol Familty.  Currently, we only support Internet 
 */
#define ALIPAY_IOT_SOC_PF_INET (0)

/* Socket Type */
typedef enum
{
    ALIPAY_IOT_SOC_SOCK_STREAM = 0,  /* stream socket, TCP */
    ALIPAY_IOT_SOC_SOCK_DGRAM,       /* datagram socket, UDP */
}alipay_socket_type_enum;

/* Socket Options */
typedef enum
{
    ALIPAY_IOT_SOC_OOBINLINE     = 0x01 << 0,  /* not support yet */
    ALIPAY_IOT_SOC_LINGER        = 0x01 << 1,  /* linger on close */
    ALIPAY_IOT_SOC_NBIO          = 0x01 << 2,  /* Nonblocking */
    ALIPAY_IOT_SOC_ASYNC         = 0x01 << 3,  /* Asynchronous notification */   

    ALIPAY_IOT_SOC_NODELAY       = 0x01 << 4,  /* disable Nagle algorithm or not */
    ALIPAY_IOT_SOC_KEEPALIVE     = 0x01 << 5,  /* enable/disable the keepalive */
    ALIPAY_IOT_SOC_RCVBUF        = 0x01 << 6,  /* set the socket receive buffer size */
    ALIPAY_IOT_SOC_SENDBUF       = 0x01 << 7,  /* set the socket send buffer size */
}alipay_soc_option_enum;

#define ALIPAY_IOT_MAX_IP_SOCKET_NUM   1  //根据实际情况修改，最多16

typedef struct
{
    uint8_t fds_bits[ALIPAY_IOT_MAX_IP_SOCKET_NUM];
} alipay_iot_fd_set;


EXTERNC void alipay_iot_fd_zero(alipay_iot_fd_set* fdset);
EXTERNC void alipay_iot_fd_setbit(int fd, alipay_iot_fd_set* fdset);
/*
* 检查fdset中对应的fd是否有被selected
*/
EXTERNC int  alipay_iot_fd_isset(int fd, alipay_iot_fd_set* fdset);

/**
 * 返回-1表示失败，返回-2表示应再次尝试，
 * 返回0表示没有事件，返回正数表示有事件的句柄数。
 * 不会返回别的值。
 */
EXTERNC int alipay_iot_select(int          maxfdp1,
                      alipay_iot_fd_set  *readset,
                      alipay_iot_fd_set  *writeset,
                      alipay_iot_fd_set  *exceptset,
                      alipay_iot_timeval *timeout);

/**
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
EXTERNC int alipay_iot_dns(const char *name, unsigned char ip[4]);

/**
 * 创建套接口。
 * domain目前只支持ALIPAY_IOT_SOC_PF_INET
 * type为SOC_SOCK_STREAM表示TCP，为SOC_SOCK_DGRAM表示UDP。
 * protocol填0，除非type填了SOC_SOCK_RAW，则要指定如下之一：
 *     SOC_IPPROTO_IP
 *     SOC_IPPROTO_HOPOPTS
 *     SOC_IPPROTO_ICMP
 *     SOC_IPPROTO_IGMP
 *     SOC_IPPROTO_IPV4
 *     SOC_IPPROTO_IPIP
 *     SOC_IPPROTO_TCP
 *     SOC_IPPROTO_UDP
 *     SOC_IPPROTO_ESP
 *     SOC_IPPROTO_AH
 *     SOC_IPPROTO_IPCOMP
 *     SOC_IPPROTO_RAW
 *     SOC_IPPROTO_DONE
 * 返回-1表示失败，返回其他为套接口的描述符。
 */
EXTERNC int alipay_iot_socket_create(int domain, alipay_socket_type_enum type, int protocol);

/**
 * 关闭套接口。
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
EXTERNC int alipay_iot_socket_close(int s);


struct alipay_iot_sockaddr {
  unsigned char sa_len;      //此值忽略。
  unsigned char sa_family;   //目前只支持ALIPAY_IOT_SOC_PF_INET。
  union {
    struct {
        unsigned short port; //网络字节序
        unsigned char  ip[4];
    }    sin_data;
    char sa_data[14];
  }             data;
};

/**
 * 设置套接口的本地地址。
 * name->sa_len无效，
 * name->sa_family必须是SOC_PF_INET，
 * name->data.sin_data.port和name->data.sin_data.ip分别填入端口号和IPv4地址
 * namelen无效。
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
EXTERNC int alipay_iot_socket_bind(int                        s,
                    const struct alipay_iot_sockaddr *name,
                    unsigned int               namelen);

/**
 * 若是TCP套接口，则向指定地址发起连接；
 * 若是UDP套接口，则设置缺省的对端地址。
 * name->sa_len无效，
 * name->sa_family必须是SOC_PF_INET，
 * name->data.sin_data.port和name->data.sin_data.ip分别填入端口号和IPv4地址
 * namelen无效。
 * 返回-1表示失败，返回-2表示异步操作正在进行中，返回0表示成功。不会返回别的值。
 */
EXTERNC int alipay_iot_socket_connect(int                            s,
                        const struct alipay_iot_sockaddr *name,
                        unsigned int                    namelen);

/**
 * 向指定地址发送数据。一般用于UDP套接口。
 * flags无效。
 * to->sa_len无效，
 * to->sa_family必须是SOC_PF_INET，
 * to->data.sin_data.port和to->data.sin_data.ip分别填入端口号和IPv4地址
 * tolen无效。
 * 返回-1表示失败，返回-2表示缓冲区已满，返回0或正数表示成功发送的字节数。
 * 不会返回别的值。
 */
EXTERNC int alipay_iot_socket_sendto(int                           s,
                        const void                  *dataptr,
                        int                          size,
                        int                            flags,
                        const struct alipay_iot_sockaddr *to,
                        unsigned int                    tolen);

/**
 * 发送数据。
 * 返回-1表示失败，返回-2表示缓冲区已满，返回0或正数表示成功发送的字节数。
 * 不会返回别的值。
 */
EXTERNC int alipay_iot_socket_write(int s, const void *dataptr, int len);

/**
 * 接收数据并告知对端地址。一般用于UDP套接口。
 * flags可以用或操作组合以下任意值：
 *     SOC_MSG_OOB
 *     SOC_MSG_PEEK
 *     SOC_MSG_DONTROUTE
 *     SOC_MSG_EOR
 *     SOC_MSG_TRUNC
 *     SOC_MSG_CTRUNC
 *     SOC_MSG_WAITALL
 *     SOC_MSG_DONTWAIT
 * from->sa_len无效，
 * from->sa_family必须是SOC_PF_INET，
 * from->data.sin_data.port和from->data.sin_data.ip分别填入端口号和IPv4地址
 * fromlen指向的值无效。
 * 返回-1表示失败，返回-2表示无数据可读，返回0表示对端已关闭写，
 * 返回正数表示成功接收的字节数。不会返回别的值。
 */
EXTERNC int alipay_iot_socket_recvfrom(int                     s,
                        void                        *mem,
                        int                       len,
                        int                        flags,
                        struct alipay_iot_sockaddr *from,
                        unsigned int            *fromlen);

/**
 * 接收数据。
 * 返回-1表示失败，返回-2表示无数据可读，返回0表示对端已关闭写，
 * 返回正数表示成功接收的字节数。不会返回别的值。
 */
EXTERNC int alipay_iot_socket_read(int s, void *mem, int len);

/**
 * 设置套接口的参数。
 * level无效。
 * optname取值如下之一：
 *     SOC_OOBINLINE，目前不支持
 *     SOC_LINGER，opval指向soc_linger_struct
 *     SOC_NBIO，opval指向kal_bool
 *     SOC_ASYNC，opval指向kal_uint8
 *     SOC_NODELAY，opval指向kal_bool
 *     SOC_KEEPALIVE，opval指向kal_bool
 *     SOC_RCVBUF，opval指向kal_uint32
 *     SOC_SENDBUF，opval指向kal_uint32
 *     SOC_PKT_SIZE，opval指向kal_uint32
 *     SOC_SILENT_LISTEN，opval指向kal_bool
 *     SOC_QOS，opval指向kal_uint8
 *     SOC_TCP_MAXSEG，opval指向kal_uint8
 *     SOC_IP_TTL，opval指向kal_uint8
 *     SOC_LISTEN_BEARER，opval指向kal_uint8
 *     SOC_UDP_ANY_FPORT，opval指向kal_bool
 *     SOC_WIFI_NOWAKEUP，opval指向kal_bool
 *     SOC_UDP_NEED_ICMP，opval指向kal_bool
 *     SOC_IP_HDRINCL，opval指向kal_bool
 *     SOC_IPSEC_POLICY，opval指向sadb_x_policy
 *     SOC_TCP_ACKED_DATA，opval指向kal_uint32
 *     SOC_TCP_DELAYED_ACK，opval指向kal_bool
 *     SOC_TCP_SACK，opval指向kal_bool
 *     SOC_TCP_TIME_STAMP，opval指向kal_bool
 *     SOC_TCP_ACK_MSEG，opval指向kal_bool
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
EXTERNC int alipay_iot_socket_setsockopt(int          s,
                          int          level,
                          int          optname,
                          const void  *opval,
                          unsigned int optlen);

/**
 * 获取套接口的参数。
 * level无效。
 * optname取值如下之一：
 *     SOC_OOBINLINE，目前不支持
 *     SOC_LINGER，opval指向soc_linger_struct
 *     SOC_NBIO，opval指向kal_bool
 *     SOC_ASYNC，opval指向kal_uint8
 *     SOC_NODELAY，opval指向kal_bool
 *     SOC_KEEPALIVE，opval指向kal_bool
 *     SOC_RCVBUF，opval指向kal_uint32
 *     SOC_SENDBUF，opval指向kal_uint32
 *     SOC_NREAD，opval指向kal_uint32
 *     SOC_PKT_SIZE，opval指向kal_uint32
 *     SOC_SILENT_LISTEN，opval指向kal_bool
 *     SOC_QOS，opval指向kal_uint8
 *     SOC_TCP_MAXSEG，opval指向kal_uint8
 *     SOC_IP_TTL，opval指向kal_uint8
 *     SOC_LISTEN_BEARER，opval指向kal_uint8
 *     SOC_UDP_ANY_FPORT，opval指向kal_bool
 *     SOC_WIFI_NOWAKEUP，opval指向kal_bool
 *     SOC_UDP_NEED_ICMP，opval指向kal_bool
 *     SOC_IP_HDRINCL，opval指向kal_bool
 *     SOC_IPSEC_POLICY，opval指向sadb_x_policy
 *     SOC_TCP_ACKED_DATA，opval指向kal_uint32
 *     SOC_TCP_DELAYED_ACK，opval指向kal_bool
 *     SOC_TCP_SACK，opval指向kal_bool
 *     SOC_TCP_TIME_STAMP，opval指向kal_bool
 *     SOC_TCP_ACK_MSEG，opval指向kal_bool
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
EXTERNC int alipay_iot_socket_getsockopt(int         s,
                          int            level,
                          int          optname,
                          void          *opval,
                          unsigned int *optlen);


#endif