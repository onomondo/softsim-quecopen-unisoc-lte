//include "type.h"              //quectel modified
#include "hs_private.h"

//quectel add
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vendor_se_v2.h"


extern unsigned char HS_IIC_Recv_Resp2(unsigned char *pData, unsigned int * wLen);
//--------------------------------------------------------------------------
//
//									版本V3.0.00
//
//--------------------------------------------------------------------------
/*○ 功能描述
    ■ 系统进入低功耗模式，可控制WKUP或VCC
  ○ 接口参数
    ■ ctx - 保存进入低功耗前的需要保存的上下文
  ○ 返回值
    ■ 无*/
void csi_enter_lpm(void *ctx)
{
	
	Set_GPIO_State(0);
	Delay_Ms(2);

	return ;
}

/*○ 功能描述
    ■ 系统退出低功耗模式，可控制WKUP或VCC
  ○ 接口参数
    ■ ctx - 保存进入低功耗前的需要保存的上下文
  ○ 返回值
    ■ 无*/
void csi_exit_lpm(void *ctx)
{
	
	Set_GPIO_State(1);
	Delay_Ms(5);
				
	return ;
}

/*○ 功能描述
    ■ 发送请求给SE且同步处理用户命令
  ○ 接口参数
    ■ req - 命令请求数据包
    ■ req_len - 命令请求数据包长度
    ■ rsp - 命令响应数据包
    ■ rsp_len - 命令响应数据包长度
  ○ 返回值
    ■ 0表示成功，非0表示失败*/
//quectel modified
//csi_error_t csi_user_cmd_process_sync(void *req, unsigned int req_len, void *rsp, unsigned int *rsp_len)
int32_t csi_user_cmd_process_sync(uint8_t *req, int32_t req_len, uint8_t *rsp, int32_t *rsp_len )
{
	if(req_len > 1228)
	{
		return CSI_ERROR;
	}

	//最高位为1表示要走通信协议2
	g_HSI2CBuf[0] = 0x80;
	g_HSI2CBuf[0] = (unsigned char)((req_len+5) / 256);
	g_HSI2CBuf[1] = (unsigned char)((req_len+5) % 256);
	g_HSI2CBuf[2] = 0x80;
	g_HSI2CBuf[3] = 0xC2;
	g_HSI2CBuf[4] = 0x00;
	g_HSI2CBuf[5] = (unsigned char)(req_len / 256);
	g_HSI2CBuf[6] = (unsigned char)(req_len % 256);
	lmemcpy(g_HSI2CBuf+7, (unsigned char *)req, req_len);

	HS_IIC_Send_Cmd(g_HSI2CBuf, req_len+7);

	//if(0 != HS_IIC_Recv_Resp2(rsp, rsp_len))
	if(0 != HS_IIC_Recv_Resp2(rsp, (unsigned int *)rsp_len))
	{
		return CSI_ERROR;
	}

	return CSI_OK;
}

