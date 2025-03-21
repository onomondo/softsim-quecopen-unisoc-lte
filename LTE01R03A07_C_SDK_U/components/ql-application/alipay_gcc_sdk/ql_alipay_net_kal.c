#include "alipay_common.h"
#include <sys/select.h>
#include "lwip/netdb.h"
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "ql_api_osi.h"
#include "alipay_net_kal.h"
#include "ql_api_datacall.h"

extern void alipay_log_ext(const char *format, ...) ;

//判断pdp是否激活
void quec_active_pdp()
{
    uint8_t nSim = 0;
    int profile_idx = 1;
    int ret = 0;
    int i = 0;
    bool is_active = false;

    is_active = ql_datacall_get_sim_profile_is_active(nSim, profile_idx);
    if (is_active == true)
        return;

    //wait 11s
    while ((ret = ql_network_register_wait(nSim, 1)) != 0 && i < 10)
    {
        i++;
        ql_rtos_task_sleep_ms(100);
    }
    if (ret == 0)
    {
        alipay_log_ext("====network registered!!!!====");
    }
    else
    {
        alipay_log_ext("====network register failure!!!!!====");
        return;
    }

    ql_set_data_call_asyn_mode(nSim, profile_idx, 0);

    alipay_log_ext("===start data call====");
    ret = ql_start_data_call(nSim, profile_idx, QL_PDP_TYPE_IP, "", NULL, NULL, 0);
    if (ret != 0)
    {
        alipay_log_ext("====data call failure!!!!=====");
        ql_stop_data_call(nSim, profile_idx);
        return;
    }
    alipay_log_ext("===start data call sucess====");
    return;
}



/*************************socket adapt*************************/
static bool alipay_socket_init_flag = false;
static int32_t alipay_sockets[ALIPAY_IOT_MAX_IP_SOCKET_NUM] = {-1};
// static aos_mutex_t alipay_socket_lock;
//static alipay_iot_mutex alipay_socket_lock = NULL;

static alipay_iot_mutex get_mtx()
{
    static alipay_iot_mutex s_mtx_socket;
    static bool s_isinitialized = false;
    if (!s_isinitialized) {
        s_mtx_socket = alipay_iot_mutex_create("socket_mutex");
        s_isinitialized = true;
    }
    return s_mtx_socket;
}

static int alipay_iot_fd_init()
{
    int ret = 0;

    if (alipay_socket_init_flag == true) {
        alipay_log_ext("alipay fd have already inited, should be called twice\n");
        return -1;
    }

    memset(alipay_sockets, -1, sizeof(alipay_sockets));
    alipay_socket_init_flag = true;
    return ret;
}

static int alloc_alipay_socket(int origin_fd)
{
    int i;

    if (alipay_socket_init_flag != true) {
        alipay_iot_fd_init();
        alipay_log_ext("alipay fd haven't init yet\n");
        // return -1;
    }
    
    alipay_iot_mutex_lock(get_mtx());
    
    for (i = 0; i < ALIPAY_IOT_MAX_IP_SOCKET_NUM; i++) {
        if (alipay_sockets[i] == -1) {
            alipay_sockets[i] = origin_fd;
            alipay_iot_mutex_unlock(get_mtx());
            return i;
        }
    }

    alipay_iot_mutex_unlock(get_mtx());
    return -1;
}

static int free_alipay_socket(int s)
{
    if (alipay_socket_init_flag != true) {
        // //printf("alipay fd haven't init yet\n");
        // return -1;
        alipay_iot_fd_init();
    }

    if (s >= ALIPAY_IOT_MAX_IP_SOCKET_NUM || s < 0) {
        return -1;
    }

    alipay_iot_mutex_lock(get_mtx());
    alipay_sockets[s] = -1;
    alipay_iot_mutex_unlock(get_mtx());
    
    return 0;
}

static int get_origin_fd_by_alipay_socket(int socket)
{
    int fd = -1;
    if (alipay_socket_init_flag != true) {
        alipay_log_ext("alipay fd haven't init yet\n");
        // return -1;
        alipay_iot_fd_init();
    }
    
    if (socket >= ALIPAY_IOT_MAX_IP_SOCKET_NUM || socket < 0){
        alipay_log_ext("invalid input socket %d\n", socket);
        return -1;
    }

    alipay_iot_mutex_lock(get_mtx());
    fd = alipay_sockets[socket];
    alipay_iot_mutex_unlock(get_mtx());
    return fd;
}

static void alipay_iot_fd_setResultbit(int fd, alipay_iot_fd_set* fdset)
{
    if(fdset != NULL && fd >=0 && fd < ALIPAY_IOT_MAX_IP_SOCKET_NUM )
    {
        fdset->fds_bits[fd] |= 0x02;
    }
}

static int alipay_iot_fd_check(int fd, alipay_iot_fd_set* fdset)
{
    if(fdset != NULL && fd >=0 && fd < ALIPAY_IOT_MAX_IP_SOCKET_NUM )
    {
        return fdset->fds_bits[fd] & 0x01;
    }
    return 0;
}

int alipay_iot_fd_isset(int fd, alipay_iot_fd_set* fdset)
{
    alipay_log_ext("isset fd: %d\n", fd);

    if(fdset != NULL && fd >=0 && fd < ALIPAY_IOT_MAX_IP_SOCKET_NUM )
    {
        return fdset->fds_bits[fd] & 0x02;
    }
    return 0;
}

void alipay_iot_fd_setbit(int fd, alipay_iot_fd_set* fdset)
{
    alipay_log_ext("setbit fd: %d\n", fd);

    if(fdset != NULL && fd >=0 && fd < ALIPAY_IOT_MAX_IP_SOCKET_NUM )
    {
        fdset->fds_bits[fd] |= 0x01;
    }
}

void alipay_iot_fd_zero(alipay_iot_fd_set* fdset)
{
    if(fdset != NULL)
    {
        memset(fdset, 0, sizeof(alipay_iot_fd_set));
    }
}

int alipay_iot_select(int maxfdp1,
                      alipay_iot_fd_set* readset,
                      alipay_iot_fd_set* writeset,
                      alipay_iot_fd_set* exceptset,
                      alipay_iot_timeval* timeout)
{
    fd_set  set_r, set_w, set_e;
    int i = 0;
    int origin_fd = -1;
    int origin_max = -1;
    int count = 0;
    
    FD_ZERO(&set_r);
    FD_ZERO(&set_w);
    FD_ZERO(&set_e);

    if(maxfdp1 < 0)
    {
        alipay_log_ext("maxfdp1 error %d\n", maxfdp1);
        return -1;
    }

    alipay_log_ext("maxfdp1 is %d, timeout sec %d, usec %d\r\n ", maxfdp1, timeout->tv_sec, timeout->tv_usec);
    for(i = 0; i < maxfdp1 + 1; i++)
    {
        origin_fd = get_origin_fd_by_alipay_socket(i);
        if (origin_fd == -1) {
           alipay_log_ext("fail to get alipay sock %d origin sock\r\n", i);
            continue;
        }
        
        if (origin_fd > origin_max - 1) {
            origin_max = origin_fd + 1;
        }
        
        if(alipay_iot_fd_check(i, readset))
        {
            alipay_log_ext("set fd %d read\n", origin_fd);
            FD_SET(origin_fd, &set_r);
        }

        if(alipay_iot_fd_check(i, writeset))
        {
            alipay_log_ext("set fd %d write\n", origin_fd);
            FD_SET(origin_fd, &set_w);
        }

        if(alipay_iot_fd_check(i, exceptset))
        {
            alipay_log_ext("set fd %d except\n", origin_fd);
            FD_SET(origin_fd, &set_e);
        }
        
        alipay_log_ext("maxfp1 is %d , i is %d, fd is %d maxfd is %d\n",maxfdp1, i, origin_fd, origin_max);
    }

    struct timeval iTimeout = {0};
    if (timeout != NULL) {
        iTimeout.tv_sec = timeout->tv_sec;
        iTimeout.tv_usec = timeout->tv_usec;
    }
    count = select(origin_max, &set_r, &set_w, &set_e, &iTimeout);
    alipay_log_ext("select return count is 0x%x\n", count);

    for(i = 0; i < maxfdp1 + 1; i++)
    {
        origin_fd = get_origin_fd_by_alipay_socket(i);
        if (origin_fd == -1) {
            continue;
        }
        
        if(FD_ISSET(origin_fd, &set_r))
        {
            alipay_iot_fd_setResultbit(i, readset);
        }
        if(FD_ISSET(origin_fd, &set_w))
        {
            alipay_iot_fd_setResultbit(i, writeset);
        }
        if(FD_ISSET(origin_fd, &set_e))
        {
            alipay_iot_fd_setResultbit(i, exceptset);
        }
    }

    return count;
}

int alipay_iot_dns(const char *name, unsigned char ip[4]) {

	  struct hostent *host_entry;
	  struct in_addr **addr_list;
      quec_active_pdp();  
	  alipay_log_ext("%s name = %s\r\n", __func__, name);
      if ((host_entry = gethostbyname_with_pcid(name,1)) == NULL)
      {
		  alipay_log_ext("%s gethostbyname failed\r\n", __func__);
		  return -1;
	  }
	  
	  addr_list = (struct in_addr **)host_entry->h_addr_list;
	  if (addr_list[0] == NULL) {
		  alipay_log_ext("%s addr_list[0] is NULL\r\n", __func__);
		  return -1;
	  }
	  
	  char *p_tmp = inet_ntoa(*addr_list[0]);
	  sscanf(p_tmp, "%hhd.%hhd.%hhd.%hhd", &ip[0], &ip[1], &ip[2], &ip[3]);
	  alipay_log_ext("%s p_tmp = %s\r\n", __func__, p_tmp);
	  
	  return 0;

}


int alipay_iot_socket_create(int domain, alipay_socket_type_enum type, int protocol) {
    int socket_type = -1;
    int fd = -1;
    switch (type) {
        case ALIPAY_IOT_SOC_SOCK_STREAM: {
            socket_type = SOCK_STREAM;
            break;
        }
        case ALIPAY_IOT_SOC_SOCK_DGRAM: {
            socket_type = SOCK_DGRAM;
            break;
        }
        default: {
            return -1;
        }
    }
    quec_active_pdp();
    int socket_fd = -1;
    if ((socket_fd = socket(PF_INET, socket_type, 0)) < 0)
      return -1;

    alipay_log_ext("socket_fd: %d\n", socket_fd);
    fd = alloc_alipay_socket(socket_fd); 
    if (fd == -1) {
        alipay_log_ext("fail to create alipay socket 0x%x\n", fd);
        close(socket_fd);
        return -1;
    }

    alipay_log_ext("fd: %d\n", fd);
    return fd;
}

int alipay_iot_socket_close(int s) {
    int fd = -1;
    int ret  = -1;
    
    fd = get_origin_fd_by_alipay_socket(s); 
    if (fd == -1) {
        return -1;
    }

    ret = close(fd);

    ret |= free_alipay_socket(s);
    if (ret != 0) {
		alipay_log_ext("close socket %d origin is %d ret %d", s, fd, ret);
        return -1;
    }

	alipay_log_ext("close socket %d origin is %d ret %d", s, fd, ret);
    return ret;
}


int alipay_iot_socket_bind(int                        s,
                    const struct alipay_iot_sockaddr *name,
                    unsigned int               namelen) {
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(name->data.sin_data.port);
    memcpy(&serv_addr.sin_addr, name->data.sin_data.ip, 4);

    int fd = -1;
    fd = get_origin_fd_by_alipay_socket(s); 
    if (fd == -1) {
       alipay_log_ext("fail to connect alipay sock %d\n", s);
        return -1;
    }
    int val = bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (val < 0) {
        alipay_log_ext("alipay_iot_socket_bind failed, %d", val);
        return -1;
    }

    return 0;
}

int alipay_iot_socket_connect(int                            s,
                        const struct alipay_iot_sockaddr *name,
                        unsigned int                    namelen) {
    struct sockaddr_in serv_addr; 
    serv_addr.sin_family = AF_INET;
    // serv_addr.sin_port = htons(name->data.sin_data.port);
    serv_addr.sin_port = htons(name->data.sin_data.port);
    //printf("[test]ip sin_port: %d\n", serv_addr.sin_port);
    memcpy(&serv_addr.sin_addr, name->data.sin_data.ip, 4);
    
    char str[100] = {0};
    inet_ntop(AF_INET, &(serv_addr.sin_addr), str, INET_ADDRSTRLEN);
    alipay_log_ext("ip address: %s\n", str);
    alipay_log_ext("port: %d - %d\n", serv_addr.sin_port, name->data.sin_data.port);

    int fd = -1;
    fd = get_origin_fd_by_alipay_socket(s);
    if (fd == -1) {
        alipay_log_ext("fail to connect alipay sock %d\n", s);
        return -1;
    }
    if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0)
      return -1;

    return 0;
}

int alipay_iot_socket_sendto(int                           s,
                        const void                  *dataptr,
                        int                          size,
                        int                            flags,
                        const struct alipay_iot_sockaddr *to,
                        unsigned int                    tolen) {
    alipay_log_ext("send socket fd: %d\n", s);
    struct sockaddr_in server_addr;
    server_addr.sin_family = to->sa_family;
    server_addr.sin_port = htons(443);
    // server_addr.sin_port = htons(to->data.sin_data.port);
    alipay_log_ext("[test]sin_port %d\n",server_addr.sin_port);
    memcpy(&server_addr.sin_addr, to->data.sin_data.ip, 4);

    int fd = -1;
    fd = get_origin_fd_by_alipay_socket(s);
    if (fd == -1) {
        alipay_log_ext("fail to connect alipay sock %d\n", s);
        return -1;
    }
    int bytes_send = sendto(fd, dataptr, size, 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    alipay_log_ext("bytes send: %d\n", bytes_send);    
    return bytes_send;
}

int alipay_iot_socket_write(int s, const void *dataptr, int len) {
    alipay_log_ext("write fd: %d - len: %d\n", s, len);
    int fd = -1;
    fd = get_origin_fd_by_alipay_socket(s);
    if (fd == -1) {
            alipay_log_ext("fail to connect alipay sock %d\n", s);
        return -1;
    }
    return write(fd, dataptr, len);
}

int alipay_iot_socket_recvfrom(int                     s,
                        void                        *mem,
                        int                       len,
                        int                        flags,
                        struct alipay_iot_sockaddr *from,
                        unsigned int            *fromlen) {
    alipay_log_ext("socket recvfrom: %d\n", s);
    int bytes_received = -1;
    
    int fd = -1;
    fd = get_origin_fd_by_alipay_socket(s);
    if (fd == -1) {
        alipay_log_ext("fail to connect alipay sock %d\n", s);
        return -1;
    }

    if (from) {
        struct sockaddr_in server_addr;
        server_addr.sin_family = from->sa_family;
            /* server_addr.sin_port = htons(443); */
        server_addr.sin_port = htons(from->data.sin_data.port);
        memcpy(&server_addr.sin_addr, from->data.sin_data.ip, 4);
    
        int addrlen = sizeof(server_addr);
        bytes_received = recvfrom(fd, mem, len, flags, (struct sockaddr *)&server_addr, &addrlen);
        alipay_log_ext("bytes_received: %d\n", bytes_received);
    } else {
        fcntl(fd, F_SETFL, fcntl(fd, F_GETFL,0) | O_NONBLOCK);
        bytes_received = recv(fd, mem, len, flags);
        alipay_log_ext("bytes_received: %d\n", bytes_received);
    }
            
    return bytes_received;
}

int alipay_iot_socket_read(int s, void *mem, int len) {
    alipay_log_ext("read fd: %d - len: %d\n", s, len);
    int n = alipay_iot_socket_recvfrom(s, mem, len, 0, NULL, NULL);
        /* int n =  read(s, mem, len); */
    alipay_log_ext("socket read %d\n", n);
    if (n <= 0 || n > len)
      return -2;

    return n;
}

int alipay_iot_socket_setsockopt(int          s,
                          int          level,
                          int          optname,
                          const void  *opval,
                          unsigned int optlen) {
    alipay_log_ext("setsockopt :%d\n", s);
    return setsockopt(s, level, optname, opval, (int)optlen);

    return 0;
}

int alipay_iot_socket_getsockopt(int         s,
                          int            level,
                          int          optname,
                          void          *opval,
                          unsigned int *optlen) {
    return getsockopt(s, level, optname, opval, (int*)optlen);
}
