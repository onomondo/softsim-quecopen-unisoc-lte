//#include "IIC_Master_Driver.h"    //quectel modified

//quectel add
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hs_private.h"

//quectel modified
extern unsigned char * IIC_Master_Init(void);

extern void IIC_Master_Send(unsigned char byAddr, unsigned char *pData, unsigned short wLen);

extern void IIC_Master_Receive(unsigned char byAddr, unsigned char *pData, unsigned short wLen);


unsigned char *g_HSI2CBuf;
unsigned char g_DefTransKey[16] = {0x54,0x8E,0x8D,0x7A,0x26,0x35,0x85,0x11,0x25,0x2E,0xF8,0xD6,0xB7,0x17,0xA4,0xE4};
unsigned char g_TransTmpKey[16];

//CRC单字节计算——循环左移，先移最高bit
unsigned short LeftShift_CRC(unsigned char byValue, unsigned short *lpwCrc)
{
	byValue ^= (unsigned char)(*lpwCrc>>8);
	byValue ^= (byValue>>4);

	*lpwCrc = (*lpwCrc<<8) ^ (unsigned short)(byValue);
	*lpwCrc ^= (unsigned short)byValue<<12;
	*lpwCrc ^= (unsigned short)byValue<<5;

	return (*lpwCrc);
}

void ComputeCRC(unsigned char *Data, unsigned short Length, unsigned char *pbyOut)
{
	unsigned char chBlock;
	unsigned short wCrc;
	
	wCrc = 0x0000; // ITU-V.41
	
	while(Length--)
	{
		chBlock = *Data++;
		LeftShift_CRC(chBlock, &wCrc);
	}
	
	pbyOut[0] = (unsigned char) ((wCrc >> 8) & 0xFF);
	pbyOut[1] = (unsigned char) (wCrc & 0xFF);
	
	return;
}

void lmemset(unsigned char *pBuf, unsigned char byVal, unsigned short wLen)
{
    unsigned short i;
    
    for(i=0; i<wLen; ++i)
    {
        pBuf[i] = byVal;
    }
    return ;
}

void lmemcpy(unsigned char *pDst, unsigned char *pSrc, unsigned short wLen)
{
	unsigned short i;

	if((pSrc == pDst) || (wLen == 0))
	{
		return;
	}

	if(pSrc < pDst)
	{
		for(i = wLen-1; i != 0; --i)
		{
			pDst[i] = pSrc[i];
		}
		pDst[0] = pSrc[0];
	}
	else
	{
		for(i = 0; i < wLen; ++i)
		{
			pDst[i] = pSrc[i];
		}
	}

	return ;
}

//相同返回0，不同返回1
unsigned char lmemcmp(unsigned char *pBuf1, unsigned char *pBuf2, unsigned short wLen)
{
	unsigned short i;

	for(i=0; i<wLen; ++i)
	{
		if(pBuf1[i] != pBuf2[i])
		{
			return 1;
		}
	}

	return 0;
}

void HS_IIC_Init(void)
{
	unsigned char ver[29];
	unsigned char i;

    extern void ql_alipay_cert_init();
    ql_alipay_cert_init();
	
	g_HSI2CBuf = IIC_Master_Init();
	lmemset(g_TransTmpKey, 0x00, 16);
	//IIC通信之前要保证SE上电3MS以上
	g_HSI2CBuf[0] = 0x00;
	g_HSI2CBuf[1] = 0x05;
	g_HSI2CBuf[2] = 0x00;
	g_HSI2CBuf[3] = 0xCA;
	g_HSI2CBuf[4] = 0x00;
	g_HSI2CBuf[5] = 0x00;
	g_HSI2CBuf[6] = 0x00;	
	HS_IIC_Send_Cmd(g_HSI2CBuf, 7);
	
	if(0 != HS_IIC_Recv_Resp1(ver, 29))//如果获取初值密钥指令执行异常，就用16字节FF进行加密通信
	{
		lmemset(g_TransTmpKey, 0xFF, 16);
		return;
	}

	if((ver[0] == 0x80) && (ver[1] == 0x10) && (ver[18] == 0x81) && (ver[19] == 0x02))//简单的检查一下数据格式内容
	{
		for(i=0; i<16; ++i)
		{
			g_TransTmpKey[i] = ver[2+i] ^ g_DefTransKey[i];
		}
	}
	else
	{
		lmemset(g_TransTmpKey, 0xFF, 16);
	}
}

//发送: 	C8 + LEN(2) + {命令及数据} ^ g_TransTmpKey + CRC
//返回FALSE表示接收指令或数据CRC校验错误
void HS_IIC_Send_Cmd(unsigned char *pData, unsigned short wLen)
{
	unsigned short i;

	for(i=0; i<(wLen-2); ++i)
	{
	    pData[2+i] ^= g_TransTmpKey[i % 16];
	}

	ComputeCRC(pData, wLen, pData+wLen);

	IIC_Master_Send(0xC8, pData, wLen+2);
}

unsigned char HS_IIC_Recv_Resp1(unsigned char *pData, unsigned int wLen)
{
	unsigned short wRspLen;
	unsigned char crc[2];
	unsigned int dwCnt;
	unsigned short i;

	
	g_HSI2CBuf[0] = 0x00;
	dwCnt = 0;
	while(g_HSI2CBuf[0] != 0x55)
	{
		++dwCnt;
		if(dwCnt > 0x1000)
		{
			return 4;
		}
		IIC_Master_Receive(0xC9, g_HSI2CBuf, 1);
	}

	IIC_Master_Receive(0xC9, g_HSI2CBuf, wLen+6);
    
	wRspLen = g_HSI2CBuf[0];
	wRspLen <<= 8;
	wRspLen |= g_HSI2CBuf[1];
	if(wRspLen != wLen+2)
	{
		return 2;
	}
	
	ComputeCRC(g_HSI2CBuf+2, wLen+2, crc);
	if((g_HSI2CBuf[wLen+4] != crc[0]) || (g_HSI2CBuf[wLen+5] != crc[1]))
	{
		return 1;
	}

	for(i=0; i<wLen+2; ++i)
	{
		g_HSI2CBuf[2+i] ^= g_TransTmpKey[i % 16];
	}
	
	if((g_HSI2CBuf[2+wLen] != 0x90) || (g_HSI2CBuf[3+wLen] != 0x00))//状态字
	{
		pData[0] = g_HSI2CBuf[2+wLen];
		pData[1] = g_HSI2CBuf[3+wLen];

		return 3;
	}

	lmemcpy(pData, g_HSI2CBuf+2, wLen);

	return 0;
}
//功能:	接收wLen长度的数据，到pData缓冲区中
//返回:
//			0 表示正确接收
//			1 表示接收数据CRC错误
//			2 表示实际接收数据长度域不是wLen
//			3 表示指令返回状态字不是9000
//			4 表示指令超时未响应
unsigned char HS_IIC_Recv_Resp2(unsigned char *pData, unsigned int * wLen)
{
	unsigned short wRspLen;
	unsigned char crc[2];
	unsigned int dwCnt;
	unsigned short i;
	
	g_HSI2CBuf[0] = 0x00;
	dwCnt = 0;
	*wLen = 0;

	while(g_HSI2CBuf[0] != 0x55)
	{
		++dwCnt;
		if(dwCnt > 0x1000)
		{
			return 4;
		}
		IIC_Master_Receive(0xC9, g_HSI2CBuf,1);
	}
	//读取长度
	IIC_Master_Receive(0xC9, g_HSI2CBuf, 2);	
	
	wRspLen = (unsigned short)(g_HSI2CBuf[0]<<0x08)|(unsigned short)g_HSI2CBuf[1];
	
	//读取数据和CRC	
	IIC_Master_Receive(0xC9, g_HSI2CBuf, wRspLen+2);
    		
	ComputeCRC(g_HSI2CBuf, wRspLen, crc);
	if((g_HSI2CBuf[wRspLen] != crc[0]) || (g_HSI2CBuf[wRspLen+1] != crc[1]))
	{
		return 1;
	}

	for(i=0; i<wRspLen; ++i)
	{
		g_HSI2CBuf[i] ^= g_TransTmpKey[i % 16];
	}
	
	if((g_HSI2CBuf[wRspLen-2] != 0x90) || (g_HSI2CBuf[wRspLen-1] != 0x00))//状态字
	{
		return 3;
	}

	lmemcpy(pData, g_HSI2CBuf, wRspLen-2);
	*wLen = wRspLen-2;

	return 0;
}


