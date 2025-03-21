/**
 * @file example_usage.c
 * @brief 应用示例代码
 *
 * @copyright Copyright (C) 2015-2023 Ant Group Holding Limited
 */

/***********************************************************************************************************************
 * Including File
***********************************************************************************************************************/
#include "bot_device_api.h"
#include "bot_adapter.h"
/***********************************************************************************************************************
 * Macro Definition
***********************************************************************************************************************/
#define BOT_PRINTF_BLANK_LINE        bot_printf("\r\n")

/***********************************************************************************************************************
 * Enumeration Definition
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Type & Structure Definition
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variable Definition
***********************************************************************************************************************/


/***********************************************************************************************************************
 * Fuction Declaration
***********************************************************************************************************************/
static void bot_dump_hex(char* msg, uint8_t* data, int len);
static void antc_iot_utils_hex2str(uint8_t *input, uint16_t input_len, char *output);
void bot_example_entry(void);

/***********************************************************************************************************************
 * Fuction Implementation
***********************************************************************************************************************/
static void bot_dump_hex(char* msg, uint8_t* data, int len) {
#if 0
    int i;
    bot_printf("%s(ex_hex): ", msg);
    for (i = 0; i < len; i++) {
        bot_printf("%02x", (uint8_t)data[i]);
    }
#else
    char tmp[130] = {'\0'};
    antc_iot_utils_hex2str(data, len, tmp);
    tmp[2 * len] = '\0';
    bot_printf("%s(ex_hex): %s", msg, tmp);
#endif
    BOT_PRINTF_BLANK_LINE;
    bot_printf("len: %d\r\n", len);
}

static void antc_iot_utils_hex2str(uint8_t *input, uint16_t input_len, char *output)
{
    char *zEncode = "0123456789ABCDEF";
    int i = 0, j = 0;

    for (i = 0; i < input_len; i++) {
        output[j++] = zEncode[(input[i] >> 4) & 0xf];
        output[j++] = zEncode[(input[i]) & 0xf];
    }
}

extern void HS_IIC_Init(void);

void bot_test_entry(void)
{
	int ret;
    uint8_t reg_data[BOT_USER_ASSET_STRING_MAX_SIZE] = {0};       //500
    int reg_outlen;
    uint8_t data_buf[] = "maasteam";                              //9
    uint8_t data_sig[BOT_SIGNATURE_EX_LEN] = {0};                 //64
    int sig_outlen;

    HS_IIC_Init();

    /* 示例1: 初始化 */
    /* 在下述示例2/3(设备注册、数据上报)调用前必须调用一次 */
    bot_printf("|---------------- test 1: bot_device_init() ---------------------------|\r\n");
    ret = bot_device_init();
    if(ret == 0){
        bot_printf("|---------------------bot_device_init() test pass!---------------------|\r\n");
    }
    else{
        bot_printf("bot_device_init() test failed! ret = %d\r\n", ret);
    }
    BOT_PRINTF_BLANK_LINE;

    /* 示例2: 设备注册 */
    bot_printf("|---------------- test 2: bot_asset_register() ------------------------|\r\n");
    user_assetInfo_get(reg_data);
    ret = bot_asset_register(reg_data, &reg_outlen, reg_data, strlen((const char *)reg_data));
    if(ret == 0){
        bot_printf("reg_data: %s\n, len: %d\n", reg_data, strlen((const char *)reg_data));
        bot_printf("|---------------------bot_asset_register() test pass!------------------|\r\n");
    }
    else {
        bot_printf("bot_asset_register() test failed! ret = %d\r\n", ret);
    }
    
    BOT_PRINTF_BLANK_LINE;

    /* 示例3: 数据上报 */
    bot_printf("|----------------- test 3: bot_data_upload() --------------------------|\r\n");
    ret = bot_data_upload(data_sig, &sig_outlen, data_buf, strlen((const char*)data_buf));
    if(ret == 0){
        bot_dump_hex("data_sig", data_sig, sig_outlen);
        bot_printf("|----------------------bot_data_upload() test pass!--------------------|\r\n");
    }
    else {
        bot_printf("bot_data_upload() test failed! ret = %d\r\n", ret);
    }
    BOT_PRINTF_BLANK_LINE;

}

int main()
{
    bot_test_entry();
    return 0;
}