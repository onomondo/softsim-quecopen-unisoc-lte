#include "alipay_common.h"
#include "vendor_file.h"
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

#define TEST_FILE_NAME      "test_file_name_long_long_long_long"
#define TEST_FILE_DATA      "qwertyuiopasdfghjklzxcvbnm1234567890mnbvcxzlkjhgfdsaqwertyuiop"
TEST_CASE(file_read){
    TEST_TAG;

    void* fd = NULL;
    uint8_t buf_temp[128] = {0,};
    uint32_t len_buf_temp = sizeof(buf_temp);
    if((int)(fd = alipay_open_rsvd_part(TEST_FILE_NAME)) <= 0){
        ALIPAY_TEST_LOG("fail to open file");
        return false;
    }
    if((alipay_read_rsvd_part(fd, buf_temp, &len_buf_temp) == 0)){
        ALIPAY_TEST_LOG("should fail to read[%d]", len_buf_temp);
        alipay_close_rsvd_part(fd);
        return false;
    }

    if(alipay_close_rsvd_part(fd) < 0){
        ALIPAY_TEST_LOG("fail to close file");
        return false;
    }

    return true;
}

TEST_CASE(file_write_and_read){
    TEST_TAG;
    void* fd = NULL;
    if((int)(fd = alipay_open_rsvd_part(TEST_FILE_NAME)) <= 0){
        ALIPAY_TEST_LOG("fail to open file");
        return false;
    }
    if(alipay_write_rsvd_part(fd, TEST_FILE_DATA, strlen(TEST_FILE_DATA)+1) != 0){
        ALIPAY_TEST_LOG("fail to write");
        return false;
    }
    if(alipay_close_rsvd_part(fd) != 0){
        ALIPAY_TEST_LOG("fail to close");
        return false;
    }
    fd = NULL;

    uint8_t buf_temp[128] = {0,};
    uint32_t len_buf_temp = sizeof(buf_temp);
    if((int)(fd = alipay_open_rsvd_part(TEST_FILE_NAME)) <= 0){
        ALIPAY_TEST_LOG("fail to open file");
        return false;
    }
    if((alipay_read_rsvd_part(fd, buf_temp, &len_buf_temp) != 0) || (len_buf_temp == 0) || (len_buf_temp != (strlen(TEST_FILE_DATA)+1))){
        ALIPAY_TEST_LOG("fail to read[%d]", len_buf_temp);
        return false;
    }

    if(alipay_clear_rsvd_part() < 0){
        ALIPAY_TEST_LOG("alipay_clear_rsvd_part error");
        return false;
    }

    if(alipay_access_rsvd_part(TEST_FILE_NAME) != 0){
        ALIPAY_TEST_LOG("fail to clear file");
        return false;
    }

    return true;
}

TEST_CASE(file_delete_and_access){
    TEST_TAG;

    void* fd = NULL;
    if((int)(fd = alipay_open_rsvd_part(TEST_FILE_NAME)) <= 0){
        ALIPAY_TEST_LOG("fail to open file");
        return false;
    }
    if(alipay_write_rsvd_part(fd, TEST_FILE_DATA, strlen(TEST_FILE_DATA)+1) != 0){
        ALIPAY_TEST_LOG("fail to write");
        return false;
    }
    if(alipay_close_rsvd_part(fd) != 0){
        ALIPAY_TEST_LOG("fail to close");
        return false;
    }

    if(alipay_access_rsvd_part(TEST_FILE_NAME) != 1){
        ALIPAY_TEST_LOG("alipay_access_rsvd_part error");
        return false;
    }

    if(alipay_remove_rsvd_part(TEST_FILE_NAME) < 0){
        ALIPAY_TEST_LOG("fail to remove file");
        return false;
    }

    if(alipay_access_rsvd_part(TEST_FILE_NAME) != 0){
        ALIPAY_TEST_LOG("fail to remove file");
        return false;
    }

    return true;
}

int test_file(void){
    bool ret;
    TEST_IT(file_read);
    if(ret == false){
        return -1;
    }
    TEST_IT(file_write_and_read);
    if(ret == false){
        return -2;
    }
    TEST_IT(file_delete_and_access);
    if(ret == false){
        return -3;
    }
    return 0;
}