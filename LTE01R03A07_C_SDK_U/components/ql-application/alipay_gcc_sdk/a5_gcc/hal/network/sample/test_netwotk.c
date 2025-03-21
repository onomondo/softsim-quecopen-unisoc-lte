#include "alipay_common.h"
#include "alipay_net_kal.h"
#include <stdio.h>

#define ALIPAY_TEST_LOG(format, ...) \
    printf("[ALIPAY_TEST]"#format"\n", ##__VA_ARGS__)

#define TEST_CASE(func_name) \
    bool TEST_##func_name()

#define TEST_TAG ALIPAY_TEST_LOG("[ALIPAY_TEST]===================TEST:[%s]\n", __func__)

#define TEST_IT(func)                                         \
do {                                                            \
    ALIPAY_TEST_LOG("==============BEGIN %s", #func);           \
    ret = TEST_##func(); \
    if (ret == true) ALIPAY_TEST_LOG("SUCCESS: %s", #func); \
    else ALIPAY_TEST_LOG("FAILURE: %s",#func); \
    ALIPAY_TEST_LOG("==============END %s", #func); \    
} while(0)

TEST_CASE(iot_time){
    TEST_TAG;
    alipay_iot_timeval now;
    alipay_iot_gettimeofday(&now);
    ALIPAY_TEST_LOG("alipay_iot_gettimeofday-[%d]-[%d]",now.tv_sec,now.tv_usec);
    if(now.tv_sec < 1694598755){
        ALIPAY_TEST_LOG("alipay_iot_gettimeofday time is invalid");
        return false;
    }

    alipay_iot_timeval now_t1, now_t2;
    uint32_t time_diff = 0;
    alipay_iot_gettimeofday(&now_t1);
    alipay_task_sleep(1100);
    alipay_iot_gettimeofday(&now_t2);
    time_diff = now_t2.tv_sec - now_t1.tv_sec;
    if((time_diff<1) || (time_diff>2)){
        ALIPAY_TEST_LOG("alipay_iot_gettimeofday is error[%d]", time_diff);
        return false;
    }

    return true;
}

#define HTTP_GET "GET /index.html HTTP/1.1\nHost: www.baidu.com:8000\nContent-Type: text/html\nContent-Length: 0\n\r\n"
TEST_CASE(socket) {
    char* host_name = "www.baidu.com";
    unsigned char ip[4];
    uint32_t port = 8000;
    int fd = 0;
    bool val = true;
    struct alipay_iot_sockaddr sa;
    char send_data[256]={0,};

    //通过dns服务获取www.baidu.com的ip地址
    if(0 != alipay_iot_dns(host_name, ip)){
        ALIPAY_TEST_LOG("alipay_iot_dns request www.baidu.com ip fail");
        return false;
    }
    //创建套接字
    fd = alipay_iot_socket_create(ALIPAY_IOT_SOC_PF_INET, ALIPAY_IOT_SOC_SOCK_STREAM, 0);
    if(fd < 0){
        ALIPAY_TEST_LOG("alipay_iot_socket_create fail");
        return false;
    }
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_family = ALIPAY_IOT_SOC_PF_INET;
    sa.data.sin_data.port = port;

    memcpy(sa.data.sin_data.ip, ip, sizeof(ip));
    //与目标建立连接
    if(alipay_iot_socket_connect(fd, &sa, sizeof(sa) < 0))
    {
        ALIPAY_TEST_LOG("alipay_iot_socket_connect(net->fd, &sa, sizeof(sa) WRONG: alipay_iot_socket_connect fail");
        alipay_iot_socket_close(fd);
        return false;
    }
    strcpy(send_data,HTTP_GET);
    // CHECK_TRUE_MSG(alipay_iot_socket_write(fd,send_data,alipay_iot_strlen(send_data))>=0,"alipay_iot_socket_write fail", return false);
    //发http的get包
    if(alipay_iot_socket_write(fd,send_data,strlen(send_data)) < 0)
    {
        ALIPAY_TEST_LOG("alipay_iot_socket_write(fd,send_data,alipay_iot_strlen(send_data)) WRONG: alipay_iot_socket_write fail");
        alipay_iot_socket_close(fd);
        return false;
    }

    //接收回的包
    alipay_iot_timeval tv={0,};
    int selectRet = 0;
    alipay_iot_fd_set t_set1;
    alipay_iot_fd_zero(&t_set1);
    alipay_iot_fd_setbit(fd, &t_set1);
    selectRet = alipay_iot_select(fd + 1, &t_set1, NULL, NULL, &tv);
    if (selectRet < 0) {
        alipay_iot_socket_close(fd);
        ALIPAY_TEST_LOG("alipay_iot_select(sockfd + 1, &t_set1, NULL, NULL, &tv) WRONG: alipay_iot_select fail");
        return false;
    }
    if (selectRet > 0){
        char buf[4096] = {0};
        int readLen = 0;
        memset(buf, 0, 4096);
        readLen = alipay_iot_socket_read(fd, buf, 4095);
        if(readLen <= 0)
        {
            alipay_iot_socket_close(fd);
            ALIPAY_TEST_LOG("alipay_iot_socket_read(fd, buf, 4095); WRONG: alipay_iot_socket_read fail\n");
            return false;
        }
    }

    alipay_iot_socket_close(fd);
    return true;
}

int test_network(void){
    bool ret;
    TEST_IT(iot_time);
    if(ret == false){
        return -1;
    }
    TEST_IT(socket);
    if(ret == false){
        return -2;
    }
    return 0;
}