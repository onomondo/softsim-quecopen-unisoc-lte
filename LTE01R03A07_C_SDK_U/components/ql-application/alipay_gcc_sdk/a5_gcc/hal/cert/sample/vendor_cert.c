#include "alipay_common.h"
/*
 * 先擦除后写数据，即每次调用该接口都要先把原来存储的数据清除再写数据
 * 要求掉电保持，
 * 储存的数据不会在「解绑支付宝」和「恢复出厂设置」的时候清除
 *
 * parametr: in: data: 要写的数据
 *               data_len: 要写的数据长度
 *
 * return: 0: 表示成功
 *         -1: 表示失败
*/
static char tmp_ram_save_buffer[3*1024] = {0,};
static uint32_t tmp_save_length = 0;

ALIPAY_WEAK_SYMBOL
int alipay_secure_save_data(PARAM_IN void *data, PARAM_IN uint32_t data_len){
    #warning alipay_secure_save_data demo
    if((data == NULL) || (data_len == 0)){
        return -1;
    }
    if(data_len > sizeof(tmp_ram_save_buffer)){
        return -1;
    }
    memcpy(tmp_ram_save_buffer, data, data_len);
    tmp_save_length = data_len;
    return 0;
}