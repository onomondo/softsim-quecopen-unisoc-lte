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
//									�汾V3.0.00
//
//--------------------------------------------------------------------------
/*�� ��������
    �� ϵͳ����͹���ģʽ���ɿ���WKUP��VCC
  �� �ӿڲ���
    �� ctx - �������͹���ǰ����Ҫ�����������
  �� ����ֵ
    �� ��*/
void csi_enter_lpm(void *ctx)
{
	
	Set_GPIO_State(0);
	Delay_Ms(2);

	return ;
}

/*�� ��������
    �� ϵͳ�˳��͹���ģʽ���ɿ���WKUP��VCC
  �� �ӿڲ���
    �� ctx - �������͹���ǰ����Ҫ�����������
  �� ����ֵ
    �� ��*/
void csi_exit_lpm(void *ctx)
{
	
	Set_GPIO_State(1);
	Delay_Ms(5);
				
	return ;
}

/*�� ��������
    �� ���������SE��ͬ�������û�����
  �� �ӿڲ���
    �� req - �����������ݰ�
    �� req_len - �����������ݰ�����
    �� rsp - ������Ӧ���ݰ�
    �� rsp_len - ������Ӧ���ݰ�����
  �� ����ֵ
    �� 0��ʾ�ɹ�����0��ʾʧ��*/
//quectel modified
//csi_error_t csi_user_cmd_process_sync(void *req, unsigned int req_len, void *rsp, unsigned int *rsp_len)
int32_t csi_user_cmd_process_sync(uint8_t *req, int32_t req_len, uint8_t *rsp, int32_t *rsp_len )
{
	if(req_len > 1228)
	{
		return CSI_ERROR;
	}

	//���λΪ1��ʾҪ��ͨ��Э��2
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

