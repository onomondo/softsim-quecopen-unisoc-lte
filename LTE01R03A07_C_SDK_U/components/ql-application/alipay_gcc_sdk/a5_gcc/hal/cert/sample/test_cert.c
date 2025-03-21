#include "alipay_common.h"
#include "vendor_cert.h"
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

//采用空发的方式下载证书
#define CERT_DOWNLOAD_ONLINE
#define MOCK_CERT "{\"certIssuer\":\"OPENAPI\",\"publicKey\":\"-----BEGIN PUBLIC KEY-----\\nMFkwEwYHKzEGp2yMNtQQ2aGn6eZOptUmy5ViY5uF1hfCaSIDjaXwrGGY1DnO8LJNnohEug==\\n-----END PUBLIC KEY-----\\n\",\"sn\":\"213D2404131F236\",\"companyCode\":\"202309121653\",\"productModel\":\"test_model\",\"productType\":\"WATCH\",\"securityNegotiation\":\"SE\",\"deviceInformation\":{\"deviceSn\":\"testsb123456789\",\"deviceMac\":\"00:11:22:33:44:55\",\"deviceImei\":\"1234567890\"},\"signAlgorithm\":\"BASE64_OVER_RSA2048\",\"sign\":\"tzkAW23NCdEMQHwCnsdhfisdubabsdkasbdkjhqjaA6i3GC8ALoAbrSymJF0VxMqN6be39hhBcBJkTzxHp6DrGPsdAlQIOJAXcxlxTTOhVpzXJsgux9ivMAoAPLbBvIxP2yXyO3VBDgy411KEDFTHdGoaUbq0prSMcOLu4zKaIUUQxziZvGel8sJiiPnVW6HMPU0Y4U78GxyDyAksdjhushih87y876as9dhasiudh78y239eywhdskdwthAqERQ==\"}"
TEST_CASE(alipay_secure_save_data){
    TEST_TAG;
#ifdef CERT_DOWNLOAD_ONLINE
    if(alipay_secure_save_data(MOCK_CERT, strlen(MOCK_CERT)+1) < 0){
        ALIPAY_TEST_LOG("fail to alipay_secure_save_data");
        return false;
    }
#endif
    return true;
}


int test_cert(void){
    bool ret = 0;
    TEST_IT(alipay_secure_save_data);
    if(ret == false){
        return -1;
    }

    return 0;
}