/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395CMD.H
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395芯片命令接口文件,常用子程序外部调用
*******************************************************************************/

#ifndef __CH395CMD_H__
#define __CH395CMD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ch395_adapter.h"
#include "CH395INC.H"


void CH395CMDReset(void);                                            /* 复位 */

void CH395CMDSleep(void);                                            /* 睡眠 */

UINT8 CH395CMDGetVer(void);                                          /* 获取芯片及固件版本号 */

UINT8 CH395CMDCheckExist(UINT8 testdata);                            /* 测试命令 */

void CH395CMDSetPHY(UINT8 phystat);                                  /* 设置PHY状态  */

UINT8 CH395CMDGetPHYStatus(void);                                    /* 获取PHY状态 */

UINT8 CH395CMDGetGlobIntStatus(void);                                /* 获取CH395全局中断状态 */

UINT8 CH395CMDInitCH395(void);                                       /* 初始化CH395 */

void CH395CMDSetUartBaudRate(UINT32 baudrate);                       /* 设置波特率 */

UINT8 CH395GetCmdStatus(void);                                       /* 获取命令执行状态 */

void CH395CMDSetIPAddr(UINT8 *ipaddr);                               /* 设置CH395的IP地址 */

void CH395CMDSetGWIPAddr(UINT8 *gwipaddr);                           /* 设置CH395的网关IP地址 */

void CH395CMDSetMASKAddr(UINT8 *maskaddr);                           /* 设置子网掩码 */

void CH395CMDSetMACAddr(UINT8 *amcaddr);                             /* 设置CH395的MAC地址 */

void CH395CMDGetMACAddr(UINT8 *amcaddr);                             /* 获取MAC地址 */

void CH395CMDSetMACFilt(UINT8 filtype,UINT32 table0,UINT32 table1);  /* 设置CH395的MAC过滤 */

void CH395CMDGetUnreachIPPT(UINT8 *list);                            /* 获取不可达地址以及端口 */

void CH395CMDGetRemoteIPP(UINT8 sockindex,UINT8 *list);              /* 获取远端IP和端口，一般在TCP Server下使用 */

void CH395SetSocketDesIP(UINT8 sockindex,UINT8 *ipaddr);             /* 设置scoket n的目的IP地址 */

void CH395SetSocketProtType(UINT8 sockindex,UINT8 prottype);         /* 设置socket n的协议类型 */

void CH395SetSocketDesPort(UINT8 sockindex,UINT16 desprot);          /* 设置socket n的目的端口 */

void CH395SetSocketSourPort(UINT8 sockindex,UINT16 surprot);         /* 设置socket n的源端口 */

void CH395SetSocketIPRAWProto(UINT8 sockindex,UINT8 prototype);      /* 在IPRAW模式下，设置socket n的IP包协议字段 */

void CH395SetRecvThresLen(UINT8 sockindex,UINT16 len);               /* 设置socket n的接收中断阀值 */

void CH395SendData(UINT8 sockindex,UINT8 *databuf,UINT16 len);       /* 向socket n的发送缓冲区写数据 */

UINT16 CH395GetRecvLength(UINT8 sockindex);                          /* 获取socket n的接收长度 */

void CH395ClearRecvBuf(UINT8 sockindex);                             /* 清除socket n的接收缓冲区 */

void CH395GetRecvData(UINT8 sockindex,UINT16 len,UINT8 *pbuf);       /* 获取接收数据 */

void CH395CMDSetRetryCount(UINT8 count);                             /* 设置最大重试次数 */

void CH395CMDSetRetryPeriod(UINT16 period);                          /* 设置最大重试周期 单位 毫秒*/

void CH395CMDGetSocketStatus(UINT8 sockindex,UINT8 *status) ;        /* 获取socket n的状态 */

UINT8  CH395OpenSocket(UINT8 sockindex);                             /* 打开socket n*/

UINT8  CH395CloseSocket(UINT8 sockindex);                             /* 关闭socket n*/

UINT8 CH395TCPConnect(UINT8 sockindex);                               /* TCP连接 */

UINT8 CH395TCPListen(UINT8 sockindex);                                /* TCP监听 */

UINT8 CH395TCPDisconnect(UINT8 sockindex);                            /* TCP断开连接 */

UINT8 CH395GetSocketInt(UINT8 sockindex);                            /* 获取socket n的中断状态 */
                                                                     
UINT8 CH395CRCRet6Bit(UINT8 *mac_addr);                              /* 多播地址CRC32，用于HASH过滤 */
                                                                     /*Function Count = 36 */
void CH395GetIPInf(UINT8 *addr);

UINT8  CH395DHCPEnable(UINT8 flag);

void CH395WriteGPIOAddr(UINT8 regadd,UINT8 regval);

UINT8 CH395ReadGPIOAddr(UINT8 regadd);

UINT8 CH395EEPROMErase(void);

UINT8 CH395EEPROMWrite(UINT16 eepaddr,UINT8 *buf,UINT8 len);

void CH395EEPROMRead(UINT16 eepaddr,UINT8 *buf,UINT8 len);

void CH395SetTCPMss(UINT16 tcpmss);

void CH395SetSocketRecvBuf(UINT8 sockindex,UINT8 startblk,UINT8 blknum);

void CH395SetSocketSendBuf(UINT8 sockindex,UINT8 startblk,UINT8 blknum);

void CH395UDPSendTo(UINT8 *buf,UINT32 len,UINT8 *ip,UINT16 port,UINT8 sockindex);

void CH395SetStartPara(UINT32 mdata);

UINT16 CH395CMDGetGlobIntStatus_ALL(void);

void CH395SetKeepLive(UINT8 sockindex,UINT8 cmd);                                 /* keepalive 使能 */

void CH395KeepLiveCNT(UINT8 cnt);                                                /* keepalive 参数设置 */

void CH395KeepLiveIDLE(UINT32 idle);                                             /* keepalive 参数设置 */

void CH395KeepLiveINTVL(UINT32 intvl);                                           /* keepalive 参数设置 */

void CH395SetTTLNum(UINT8 sockindex,UINT8 TTLnum);                               /* TTL参数设置 */
 
void CH395EnablePing(UINT8 enable);                                              /* PING 使能 */

#ifdef __cplusplus
}
#endif
#endif
/**************************** endfile *************************************/
