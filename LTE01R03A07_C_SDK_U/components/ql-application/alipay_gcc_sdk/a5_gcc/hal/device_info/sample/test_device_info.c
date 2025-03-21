#include "alipay_common.h"
#include "vendor_device_info.h"
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

static int isValidMac(char *value)
{
    int r = 0; //r=0:valid, else not valid
    r += strlen(value)==17 ? 0 : 1;
    for(int i=0;i<strlen(value);i++){
        switch(i){
            case 2:
            case 5:
            case 8:
            case 11:
            case 14:
                r += *(value+i) == ':' ? 0 : 1;
                break;
            default:
                if(((*(value+i)>='0')&&(*(value+i)<='9')) || ((*(value+i)>='A')&&(*(value+i)<='F')) || ((*(value+i)>='a')&&(*(value+i)<='f')))
                    ;
                else
                    r += 1;
                break;
        }
    }
    return r;
}

TEST_CASE(alipay_get_mac){
    TEST_TAG;
    retval_e ret = ALIPAY_RV_OK;
    uint8_t bt_mac[18]={0,};
    uint32_t len_mac = sizeof(bt_mac);
    if((ret = alipay_get_mac(bt_mac, &len_mac)) != ALIPAY_RV_OK){
        ALIPAY_TEST_LOG("fail to get mac");
        return false;
    }
    if(len_mac != 17){
        ALIPAY_TEST_LOG("mac's len is not 17-[%d]", len_mac);
        return false;
    }
    ALIPAY_TEST_LOG("mac[%d]:%s", len_mac, bt_mac);
    if(isValidMac(bt_mac) != 0){
        ALIPAY_TEST_LOG("mac's format is invalid");
        return false;
    }
    return true;
}

#define SN_MAX_LEN      32
TEST_CASE(alipay_get_sn){
    TEST_TAG;
    retval_e ret = ALIPAY_RV_OK;
    uint8_t buf_sn[40]={0,};
    uint32_t len_buf_sn = sizeof(buf_sn);
    if((ret = alipay_get_sn(buf_sn, &len_buf_sn)) != ALIPAY_RV_OK){
        ALIPAY_TEST_LOG("fail to get sn");
        return false;
    }
    if(len_buf_sn > SN_MAX_LEN){
        ALIPAY_TEST_LOG("sn's len is longer than 32-[%d]", len_buf_sn);
        return false;
    }
    ALIPAY_TEST_LOG("sn[%d]:%s", len_buf_sn, buf_sn);
    return true;
}

#define IMEI_MAX_LEN      48
TEST_CASE(alipay_get_imei){
    TEST_TAG;
    retval_e ret = ALIPAY_RV_OK;
    uint8_t buf_temp[56]={0,};
    uint32_t len_buf_temp = sizeof(buf_temp);
    if((ret = alipay_get_imei(buf_temp, &len_buf_temp)) != ALIPAY_RV_OK){
        ALIPAY_TEST_LOG("fail to get imei");
        return false;
    }
    if(len_buf_temp > IMEI_MAX_LEN){
        ALIPAY_TEST_LOG("imei's len is longer than 48-[%d]", len_buf_temp);
        return false;
    }
    ALIPAY_TEST_LOG("imei[%d]:%s", len_buf_temp, buf_temp);
    return true;
}

TEST_CASE(alipay_get_productType){
    TEST_TAG;
    uint32_t temp_value = alipay_get_productType();

    if(temp_value > 5){
        ALIPAY_TEST_LOG("product type is invalid-[%d]", temp_value);
        return false;
    }
    ALIPAY_TEST_LOG("product type:%d", temp_value);
    return true;
}

#define COMPANY_NAME_MAX_LEN      15
TEST_CASE(alipay_get_companyName){
    TEST_TAG;
    retval_e ret = ALIPAY_RV_OK;
    uint8_t buf_temp[56]={0,};
    uint32_t len_buf_temp = sizeof(buf_temp);
    if((ret = alipay_get_companyName(buf_temp, &len_buf_temp)) != ALIPAY_RV_OK){
        ALIPAY_TEST_LOG("fail to get company name");
        return false;
    }
    if(len_buf_temp > COMPANY_NAME_MAX_LEN){
        ALIPAY_TEST_LOG("company name's len is longer than 15-[%d]", len_buf_temp);
        return false;
    }
    ALIPAY_TEST_LOG("company name[%d]:%s", len_buf_temp, buf_temp);
    return true;
}

#define PRODUCT_MODEL_MAX_LEN      23
TEST_CASE(alipay_get_productModel){
    TEST_TAG;
    retval_e ret = ALIPAY_RV_OK;
    uint8_t buf_temp[56]={0,};
    uint32_t len_buf_temp = sizeof(buf_temp);
    if((ret = alipay_get_productModel(buf_temp, &len_buf_temp)) != ALIPAY_RV_OK){
        ALIPAY_TEST_LOG("fail to get product model");
        return false;
    }
    if(len_buf_temp > PRODUCT_MODEL_MAX_LEN){
        ALIPAY_TEST_LOG("product model's len is longer than 23-[%d]", len_buf_temp);
        return false;
    }
    ALIPAY_TEST_LOG("product model[%d]:%s", len_buf_temp, buf_temp);
    return true;
}

int test_device_info(void){
    bool ret = 0;
    TEST_IT(alipay_get_mac);
    if(ret == false){
        return -1;
    }
    TEST_IT(alipay_get_sn);
    if(ret == false){
        return -2;
    }
    TEST_IT(alipay_get_imei);
    if(ret == false){
        return -3;
    }
    TEST_IT(alipay_get_productType);
    if(ret == false){
        return -4;
    }
    TEST_IT(alipay_get_companyName);
    if(ret == false){
        return -5;
    }
    TEST_IT(alipay_get_productModel);
    if(ret == false){
        return -6;
    }
    return 0;
}