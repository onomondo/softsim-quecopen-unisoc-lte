//include "type.h"              //quectel modified
#include "hs_private.h"

//quectel add
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vendor_se_v1.h"

extern void IIC_Master_Receive(unsigned char byAddr, unsigned char *pData, unsigned short wLen);

//--------------------------------------------------------------------------
//
//									版本V2.0.00
//
//--------------------------------------------------------------------------





/*○ 功能描述
    ■ 获取读取资产的文件句柄
  ○ 接口参数
    ■ fd  - 存放资产句柄
    ■ asset_name - 资产名字
  ○ 返回值
    ■ 0表示成功，非0表示失败*/
csi_error_t csi_open_asset(void **fd, char asset_name[36])
{
	unsigned char tmpBuf[10];
	unsigned char i;
	unsigned short j;

	for(i=0; i<36; ++i)
	{
		if(asset_name[i] == 0)
		{
			break;
		}
	}

	if((fd == 0) || (i == 0))
	{
		return CSI_ERROR;
	}
	
	g_HSI2CBuf[0] = 0x00;
	g_HSI2CBuf[1] = i+5;
	g_HSI2CBuf[2] = 0x80;
	g_HSI2CBuf[3] = 0xE0;
	g_HSI2CBuf[4] = 0x00;
	g_HSI2CBuf[5] = 0x00;
	g_HSI2CBuf[6] = i;
	lmemcpy(g_HSI2CBuf+7, (unsigned char *)asset_name, i);
	if(gHSEncTransSign == 0x01)
	{
	    for(j=0; j<i+5; ++j)
	    {
	        g_HSI2CBuf[2+j] ^= g_TransTmpKey[j % 16];
	    }
	}
	ComputeCRC(g_HSI2CBuf, i+7, g_HSI2CBuf+i+7);
	HS_IIC_Send_Cmd(g_HSI2CBuf, i+9);
	if(0 != HS_IIC_Recv_Resp(tmpBuf, 2))
	{
		return CSI_ERROR;
	}
	*fd = (void*)(tmpBuf[0] * 256 + tmpBuf[1]);	//文件地址
	return CSI_OK;
}

/*○ 功能描述
    ■ 写入指定的资产
  ○ 接口参数
    ■ fd  - 存放资产句柄
    ■ offset - 资产数据偏移
    ■ buffer - 存放资产数据缓冲区
    ■ data_len - 写入资产数据个数
  ○ 返回值
    ■ 0表示成功，非0表示失败*/
csi_error_t csi_write_asset(void* fd,uint32_t offset,void *data,uint32_t data_len)
{
    unsigned int dwBufOff;
	unsigned char abyBuf[10];
	unsigned short i;

	if(offset+data_len > 4096)
	{
		return CSI_ERROR;
	}
	if(data == 0)
	{
		return CSI_ERROR;
	}
	
	//先根据fd选择文件
	g_HSI2CBuf[0] = 0x00;
	g_HSI2CBuf[1] = 0x07;
	g_HSI2CBuf[2] = 0x00;
	g_HSI2CBuf[3] = 0xA4;
	g_HSI2CBuf[4] = 0x00;
	g_HSI2CBuf[5] = 0x00;
	g_HSI2CBuf[6] = 0x02;
	g_HSI2CBuf[7] = (unsigned char)((unsigned int)fd / 256);
	g_HSI2CBuf[8] = (unsigned int)fd % 256;

	if(gHSEncTransSign == 0x01)
	{
	    for(i=0; i<7; ++i)
	    {
	        g_HSI2CBuf[2+i] ^= g_TransTmpKey[i];
	    }
	}

	ComputeCRC(g_HSI2CBuf, 9, g_HSI2CBuf+9);
	HS_IIC_Send_Cmd(g_HSI2CBuf, 11);
	if(0 != HS_IIC_Recv_Resp(abyBuf, 2))
	{
		return CSI_ERROR;
	}

	if(data_len == 0)
	{
		return CSI_OK;
	}

    dwBufOff = 0;
	while(data_len > 1024)
	{
		g_HSI2CBuf[0] = 0x04;
		g_HSI2CBuf[1] = 0x07;
		g_HSI2CBuf[2] = 0x00;
		g_HSI2CBuf[3] = 0xd6;
		g_HSI2CBuf[4] = (unsigned char)(offset / 256);
		g_HSI2CBuf[5] = (unsigned char)(offset % 256);
		g_HSI2CBuf[6] = 0x00;
		g_HSI2CBuf[7] = 0x04;
		g_HSI2CBuf[8] = 0x00;
		lmemcpy(g_HSI2CBuf+9, (unsigned char *)data + dwBufOff, 1024);
		
		if(gHSEncTransSign == 0x01)
		{
			for(i=0; i<1031; ++i)
	        {
	            g_HSI2CBuf[2+i] ^= g_TransTmpKey[i % 16];
	        }
		}

		ComputeCRC(g_HSI2CBuf, 1033, g_HSI2CBuf+1033);
		HS_IIC_Send_Cmd(g_HSI2CBuf, 1035);
		if(0 != HS_IIC_Recv_Resp(abyBuf, 0))
		{
			return CSI_ERROR;
		}
		offset += 1024;
		data_len -= 1024;
		dwBufOff += 1024;
	}

	g_HSI2CBuf[0] = (unsigned char)((data_len+7) / 256);
	g_HSI2CBuf[1] = (unsigned char)((data_len+7) % 256);
	g_HSI2CBuf[2] = 0x80;
	g_HSI2CBuf[3] = 0xd6;
	g_HSI2CBuf[4] = (unsigned char)(offset / 256);
	g_HSI2CBuf[5] = (unsigned char)(offset % 256);
	g_HSI2CBuf[6] = 0x00;
	g_HSI2CBuf[7] = (unsigned char)(data_len / 256);
	g_HSI2CBuf[8] = (unsigned char)(data_len % 256);
	lmemcpy(g_HSI2CBuf+9, (unsigned char *)data + dwBufOff, data_len);

	if(gHSEncTransSign == 0x01)
	{
	    for(i=0; i<data_len+7; ++i)
	    {
	        g_HSI2CBuf[2+i] ^= g_TransTmpKey[i % 16];
	    }
	}

	ComputeCRC(g_HSI2CBuf, data_len+9, g_HSI2CBuf+data_len+9);
	HS_IIC_Send_Cmd(g_HSI2CBuf, data_len+11);
	if(0 != HS_IIC_Recv_Resp(abyBuf, 0))
	{
		return CSI_ERROR;
	}
	
	return CSI_OK;
}

/*○ 功能描述
    ■ 读取指定的资产
  ○ 接口参数
    ■ fd  - 存放资产句柄
    ■ offset - 资产数据偏移
    ■ buffer - 存放资产数据缓冲区
    ■ data_len - 返回读取资产数据个数
  ○ 返回值
    ■ 0表示成功，非0表示失败*/
csi_error_t csi_read_asset(void* fd,uint32_t offset,void *buffer,uint32_t* data_len)
{
	unsigned int dwOff;
	unsigned char abyBuf[10];
	unsigned short wFileLen;
	uint32_t dwTmpLen;
	unsigned short i;

	
	if(*data_len == 0)
	{
		return CSI_ERROR;
	}

	if(buffer == 0)
	{
		return CSI_ERROR;
	}

	//先根据fd选择文件
	g_HSI2CBuf[0] = 0x00;
	g_HSI2CBuf[1] = 0x07;
	g_HSI2CBuf[2] = 0x00;
	g_HSI2CBuf[3] = 0xA4;
	g_HSI2CBuf[4] = 0x00;
	g_HSI2CBuf[5] = 0x00;
	g_HSI2CBuf[6] = 0x02;
	g_HSI2CBuf[7] = (unsigned char)((unsigned int)fd / 256);
	g_HSI2CBuf[8] = (unsigned int)fd % 256;
	if(gHSEncTransSign == 0x01)
	{
	    for(i=0; i<7; ++i)
	    {
	        g_HSI2CBuf[2+i] ^= g_TransTmpKey[i];
	    }
	}
    
	ComputeCRC(g_HSI2CBuf, 9, g_HSI2CBuf+9);
	HS_IIC_Send_Cmd(g_HSI2CBuf, 11);
	if(0 != HS_IIC_Recv_Resp(abyBuf, 2))
	{
		return CSI_ERROR;
	}
	wFileLen = abyBuf[0] * 256 + abyBuf[1];

	if(wFileLen == 0)
	{
		*data_len = 0;
		return CSI_ERROR;
	}
		
	if(offset >= wFileLen)
	{
		return CSI_ERROR;
	}

	if(offset + *data_len > wFileLen)
	{
		*data_len = wFileLen - offset;
	}

	dwOff = 0;
	dwTmpLen = *data_len;
	while(dwTmpLen > 1024)
	{
        g_HSI2CBuf[0] = 0x00;
        g_HSI2CBuf[1] = 0x07;
        g_HSI2CBuf[2] = 0x00;
        g_HSI2CBuf[3] = 0xB0;
        g_HSI2CBuf[4] = (unsigned char)(offset / 256);
        g_HSI2CBuf[5] = (unsigned char)(offset % 256);
        g_HSI2CBuf[6] = 0x00;
        g_HSI2CBuf[7] = 0x04;
        g_HSI2CBuf[8] = 0x00;
		if(gHSEncTransSign == 0x01)
		{
	   		for(i=0; i<7; ++i)
	        {
	            g_HSI2CBuf[2+i] ^= g_TransTmpKey[i];
	        }
		}

		ComputeCRC(g_HSI2CBuf, 9, g_HSI2CBuf+9);
		HS_IIC_Send_Cmd(g_HSI2CBuf, 11);
		if(0 == HS_IIC_Recv_Resp((unsigned char *)buffer + dwOff, 1024))
		{
            offset += 1024;
			dwOff += 1024;
			dwTmpLen -= 1024;
		}
		else
		{
			return CSI_ERROR;
		}
	}

	g_HSI2CBuf[0] = 0x00;
	g_HSI2CBuf[1] = 0x07;
	g_HSI2CBuf[2] = 0x00;
	g_HSI2CBuf[3] = 0xB0;
	g_HSI2CBuf[4] = (unsigned char)(offset / 256);
	g_HSI2CBuf[5] = (unsigned char)(offset % 256);
	g_HSI2CBuf[6] = 0x00;
	g_HSI2CBuf[7] = (unsigned char)(dwTmpLen / 256);
	g_HSI2CBuf[8] = (unsigned char)(dwTmpLen % 256);
	if(gHSEncTransSign == 0x01)
	{
	    for(i=0; i<7; ++i)
	    {
	        g_HSI2CBuf[2+i] ^= g_TransTmpKey[i];
	    }
	}
	ComputeCRC(g_HSI2CBuf, 9, g_HSI2CBuf+9);
	HS_IIC_Send_Cmd(g_HSI2CBuf, 11);
	if(0 != HS_IIC_Recv_Resp((unsigned char *)buffer + dwOff, dwTmpLen))
	{
		return CSI_ERROR;
	}

	return CSI_OK;
}

/*○ 功能描述
    ■ 关闭指定的资产句柄
  ○ 接口参数
    ■ fd  - 存放资产句柄
  ○ 返回值
    ■ 0表示成功，非0表示失败*/
csi_error_t csi_close_asset(void* fd)
{
	unsigned char abySW[2];
	unsigned char i;

	
	g_HSI2CBuf[0] = 0x00;
	g_HSI2CBuf[1] = 0x07;
	g_HSI2CBuf[2] = 0x80;
	g_HSI2CBuf[3] = 0xA6;
	g_HSI2CBuf[4] = 0x00;
	g_HSI2CBuf[5] = 0x00;
	g_HSI2CBuf[6] = 0x02;
	g_HSI2CBuf[7] = (unsigned char)((unsigned int)fd / 256);
	g_HSI2CBuf[8] = (unsigned int)fd % 256;
	if(gHSEncTransSign == 0x01)
	{
	    for(i=0; i<7; ++i)
	    {
	        g_HSI2CBuf[2+i] ^= g_TransTmpKey[i];
	    }
	}

	ComputeCRC(g_HSI2CBuf, 9, g_HSI2CBuf+9);
	HS_IIC_Send_Cmd(g_HSI2CBuf, 11);
	if(0 != HS_IIC_Recv_Resp(abySW, 0))
	{
		return CSI_ERROR;
	}

	return CSI_OK;
}

/*○ 功能描述
    ■ 确认指定的资产是否可访问
  ○ 接口参数
    ■ access-flag - 资产访问结果标识: 资产存在设置为1，资产不存在设置为0
    ■ asset_name - 资产名字
  ○ 返回值
    ■ 0表示成功，非0表示失败*/
csi_error_t csi_access_asset(uint32_t *access_flag, char asset_name[36])
{
	unsigned char i;
	unsigned char abyBuf[10];
	unsigned short j;

	for(i=0; i<36; ++i)
	{
		if(asset_name[i] == 0)
		{
			break;
		}
	}
	if(i == 0)
	{
		return CSI_ERROR;
	}
	
	g_HSI2CBuf[0] = 0x00;
	g_HSI2CBuf[1] = i+5;
	g_HSI2CBuf[2] = 0x80;
	g_HSI2CBuf[3] = 0xA4;
	g_HSI2CBuf[4] = 0x04;
	g_HSI2CBuf[5] = 0x00;
	g_HSI2CBuf[6] = i;
	lmemcpy(g_HSI2CBuf+7, (unsigned char *)asset_name, i);
	if(gHSEncTransSign == 0x01)
	{
	    for(j=0; j<i+5; ++j)
	    {
	        g_HSI2CBuf[2+j] ^= g_TransTmpKey[j % 16];
	    }
	}

	ComputeCRC(g_HSI2CBuf, i+7, g_HSI2CBuf+i+7);
	HS_IIC_Send_Cmd(g_HSI2CBuf, i+9);
	abyBuf[0] = 0x00;
	abyBuf[1] = 0x00;
	if(0 != HS_IIC_Recv_Resp(abyBuf, 0))
	{
		if((abyBuf[0] == 0x6A) && (abyBuf[1] == 0x82))
		{
			*access_flag = 0;
			return CSI_OK;
		}
		return CSI_ERROR;
	}

	*access_flag = 1;

	return CSI_OK;
}

/*○ 功能描述
    ■ 清除指定的资产
  ○ 接口参数
    ■ asset_name - 资产名字
  ○ 返回值
    ■ 0表示成功，非0表示失败*/
csi_error_t csi_remove_asset(char asset_name[36])
{
	unsigned char i;
	unsigned char abySW[2];
	unsigned short j;

	for(i=0; i<36; ++i)
	{
		if(asset_name[i] == 0)
		{
			break;
		}
	}
	if(i == 0)
	{
		return CSI_ERROR;
	}
	
	g_HSI2CBuf[0] = 0x00;
	g_HSI2CBuf[1] = i+5;
	g_HSI2CBuf[2] = 0x00;
	g_HSI2CBuf[3] = 0x0E;
	g_HSI2CBuf[4] = 0x00;
	g_HSI2CBuf[5] = 0x00;
	g_HSI2CBuf[6] = i;
	lmemcpy(g_HSI2CBuf+7, (unsigned char *)asset_name, i);
	if(gHSEncTransSign == 0x01)
	{
	    for(j=0; j<i+5; ++j)
	    {
	        g_HSI2CBuf[2+j] ^= g_TransTmpKey[j % 16];
	    }
	}

	ComputeCRC(g_HSI2CBuf, i+7, g_HSI2CBuf+i+7);
	HS_IIC_Send_Cmd(g_HSI2CBuf, i+9);
	if(0 != HS_IIC_Recv_Resp(abySW, 0))
	{
		return CSI_ERROR;
	}

	return CSI_OK;
}

/*○ 功能描述
    ■ 清除所有资产
  ○ 接口参数
    ■ 无
  ○ 返回值
    ■ 0表示成功，非0表示失败*/
csi_error_t csi_clear_assets(void)
{
	unsigned char abySW[2];
	unsigned char i;

	g_HSI2CBuf[0] = 0x00;
	g_HSI2CBuf[1] = 0x05;
	g_HSI2CBuf[2] = 0x80;
	g_HSI2CBuf[3] = 0x0E;
	g_HSI2CBuf[4] = 0x00;
	g_HSI2CBuf[5] = 0x00;
	g_HSI2CBuf[6] = 0x00;
	if(gHSEncTransSign == 0x01)
	{
	    for(i=0; i<5; ++i)
	    {
	        g_HSI2CBuf[2+i] ^= g_TransTmpKey[i];
	    }
	}
	ComputeCRC(g_HSI2CBuf, 7, g_HSI2CBuf+7);
	HS_IIC_Send_Cmd(g_HSI2CBuf, 9);
	if(0 != HS_IIC_Recv_Resp(abySW, 0))
	{
		return CSI_ERROR;
	}

	return CSI_OK;
}



/*○ 功能描述
    ■ 获取安全芯片固件版本号
  ○ 接口参数
    ■ ver - 存放版本号buffer
  ○ 返回值
    ■ 0表示成功，非0表示失败*/
csi_error_t csi_get_fw_version(char ver[8])
{
    unsigned char i;

	g_HSI2CBuf[0] = 0x00;
	g_HSI2CBuf[1] = 0x05;
	g_HSI2CBuf[2] = 0x00;
	g_HSI2CBuf[3] = 0xBC;
	g_HSI2CBuf[4] = 0x00;
	g_HSI2CBuf[5] = 0x00;
	g_HSI2CBuf[6] = 0x08;
	if(gHSEncTransSign == 0x01)
	{
	    for(i=0; i<5; ++i)
	    {
	        g_HSI2CBuf[2+i] ^= g_TransTmpKey[i];
	    }
	}
	ComputeCRC(g_HSI2CBuf, 7, g_HSI2CBuf+7);
	HS_IIC_Send_Cmd(g_HSI2CBuf, 9);
	if(0 != HS_IIC_Recv_Resp((unsigned char *)ver, 8))
	{
		return CSI_ERROR;
	}

	return CSI_OK;

}


/*○ 功能描述
    ■ 系统进入低功耗模式
  ○ 接口参数
    ■ ctx - 保存进入低功耗前的需要保存的上下文
  ○ 返回值
    ■ 无*/
void csi_enter_lpm(void *ctx)
{
	unsigned char abySW[2];
	unsigned char i;

	g_HSI2CBuf[0] = 0x00;
	g_HSI2CBuf[1] = 0x0C;
	g_HSI2CBuf[2] = 0x80;
	g_HSI2CBuf[3] = 0xFB;
	g_HSI2CBuf[4] = 0x00;
	g_HSI2CBuf[5] = 0x00;
	g_HSI2CBuf[6] = 0x07;

	g_HSI2CBuf[7] = 0x53;
	g_HSI2CBuf[8] = 0x54;
	g_HSI2CBuf[9] = 0x41;
	g_HSI2CBuf[10] = 0x4E;
	g_HSI2CBuf[11] = 0x44;
	g_HSI2CBuf[12] = 0x42;
	g_HSI2CBuf[13] = 0x59;

	if(gHSEncTransSign == 0x01)
	{
	    for(i=0; i<12; ++i)
	    {
	        g_HSI2CBuf[2+i] ^= g_TransTmpKey[i];
	    }
	}

	ComputeCRC(g_HSI2CBuf, 14, g_HSI2CBuf+14);
	HS_IIC_Send_Cmd(g_HSI2CBuf, 16);
	if(0 != HS_IIC_Recv_Resp(abySW, 0))
	{
		return ;
	}

	Delay_Ms(30);

	return ;
}


/*○ 功能描述
    ■ 系统退出低功耗模式
  ○ 接口参数
    ■ ctx - 保存进入低功耗前的需要保存的上下文
  ○ 返回值
    ■ 无*/
void csi_exit_lpm(void *ctx)
{
	unsigned char abyVer[8];
	unsigned char i;

	g_HSI2CBuf[0] = 0x00;
	g_HSI2CBuf[1] = 0x05;
	g_HSI2CBuf[2] = 0x00;
	g_HSI2CBuf[3] = 0xBC;
	g_HSI2CBuf[4] = 0x00;
	g_HSI2CBuf[5] = 0x00;
	g_HSI2CBuf[6] = 0x08;
	if(gHSEncTransSign == 0x01)
	{
	    for(i=0; i<5; ++i)
	    {
	        g_HSI2CBuf[2+i] ^= g_TransTmpKey[i];
	    }
	}
	ComputeCRC(g_HSI2CBuf, 7, g_HSI2CBuf+7);

	for(i=0; i<3; ++i)
	{
		Set_GPIO_State(0);

		Delay_Ms(1);

		Set_GPIO_State(1);

		Delay_Ms(15);

		HS_IIC_Send_Cmd(g_HSI2CBuf, 9);
		if(0 == HS_IIC_Recv_Resp(abyVer, 8))
		{
			if((abyVer[0] == 'V') && (abyVer[2] == '.'))
			{
				return ;
			}
		}
	}
			
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
int32_t csi_user_cmd_process_sync(uint8_t *req, int32_t req_len, uint8_t *rsp, int32_t *rsp_len)
{
    unsigned short i;
	unsigned short wRspLen;
	unsigned char crc[2];
	unsigned int dwCnt;

	if(req_len > 1024)
	{
		return CSI_ERROR;
	}

	g_HSI2CBuf[0] = (unsigned char)((req_len+5) / 256);
	g_HSI2CBuf[1] = (unsigned char)((req_len+5) % 256);
	g_HSI2CBuf[2] = 0x80;
	g_HSI2CBuf[3] = 0xC2;
	g_HSI2CBuf[4] = 0x00;
	g_HSI2CBuf[5] = (unsigned char)(req_len / 256);
	g_HSI2CBuf[6] = (unsigned char)(req_len % 256);
	lmemcpy(g_HSI2CBuf+7, (unsigned char *)req, req_len);
	if(gHSEncTransSign == 0x01)
	{
	    for(i=0; i<req_len+5; ++i)
	    {
	        g_HSI2CBuf[2+i] ^= g_TransTmpKey[i % 16];
	    }
	}

	ComputeCRC(g_HSI2CBuf, req_len+7, g_HSI2CBuf+req_len+7);
	HS_IIC_Send_Cmd(g_HSI2CBuf, req_len+9);


	g_HSI2CBuf[0] = 0x00;
	dwCnt = 0;
	while(g_HSI2CBuf[0] != 0x55)
	{
		++dwCnt;
		if(dwCnt > 0x1000)
		{
			return CSI_ERROR;
		}
		IIC_Master_Receive(0xC9, g_HSI2CBuf, 1);
	}

	IIC_Master_Receive(0xC9, g_HSI2CBuf, 256);
	wRspLen = g_HSI2CBuf[0];
	wRspLen <<= 8;
	wRspLen |= g_HSI2CBuf[1];
	
	ComputeCRC(g_HSI2CBuf+2, wRspLen, crc);
	if((g_HSI2CBuf[wRspLen+2] != crc[0]) || (g_HSI2CBuf[wRspLen+3] != crc[1]))
	{
		return CSI_ERROR;
	}

	if(gHSEncTransSign == 0x01)
	{
		for(i=0; i<wRspLen; ++i)
		{
			g_HSI2CBuf[2+i] ^= g_TransTmpKey[i % 16];
		}
	}
	if((g_HSI2CBuf[wRspLen] != 0x90) || (g_HSI2CBuf[1+wRspLen] != 0x00))//状态字
	{
		return CSI_ERROR;
	}

	*rsp_len = wRspLen-2;
	lmemcpy(rsp, g_HSI2CBuf+2, wRspLen-2);
	
	return CSI_OK;
}

