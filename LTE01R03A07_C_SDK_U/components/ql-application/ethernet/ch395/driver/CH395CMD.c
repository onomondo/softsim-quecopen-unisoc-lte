/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395CMD.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395芯片命令接口文件
*                      
*******************************************************************************/

/* 头文件包含*/
#include "CH395INC.H"
#include "CH395CMD.H"

#include "ch395_adapter.h"

/********************************************************************************
* Function Name  : CH395CMDReset
* Description    : 复位CH395芯片
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDReset(void)
{
    xWriteCH395Cmd(CMD00_RESET_ALL);
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395CMDSleep
* Description    : 使CH395进入睡眠状态
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSleep(void)
{
    xWriteCH395Cmd(CMD00_ENTER_SLEEP);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSleep
* Description    : 获取芯片以及固件版本号，1字节，高四位表示芯片版本，
                   低四位表示固件版本
* Input          : None
* Output         : None
* Return         : 1字节芯片及固件版本号
*******************************************************************************/
UINT8 CH395CMDGetVer(void)
{
    UINT8 i;
    xWriteCH395Cmd(CMD01_GET_IC_VER);
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

/********************************************************************************
* Function Name  : CH395CMDCheckExist
* Description    : 测试命令，用于测试硬件以及接口通讯
* Input          : testdata 1字节测试数据
* Output         : None
* Return         : 硬件OK，返回 testdata按位取反
*******************************************************************************/
UINT8 CH395CMDCheckExist(UINT8 testdata)
{
    UINT8 i;

    xWriteCH395Cmd(CMD11_CHECK_EXIST);
    xWriteCH395Data(testdata);
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

/********************************************************************************
* Function Name  : CH395CMDSetPHY
* Description    : 设置PHY，主要设置CH395 PHY为100/10M 或者全双工半双工，CH395默
                    为自动协商。
* Input          : phystat 参考PHY 命令参数/状态
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetPHY(UINT8 phystat)
{
    xWriteCH395Cmd(CMD10_SET_PHY);
    xWriteCH395Data(phystat);
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395CMDGetPHYStatus
* Description    : 获取PHY的状态
* Input          : None
* Output         : None
* Return         : 当前CH395PHY状态，参考PHY参数/状态定义
*******************************************************************************/
UINT8 CH395CMDGetPHYStatus(void)
{
    UINT8 i;

    xWriteCH395Cmd(CMD01_GET_PHY_STATUS);
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

/*******************************************************************************
* Function Name  : CH395CMDGetGlobIntStatus
* Description    : 获取全局中断状态，收到此命令CH395自动取消中断，0x43及以下版本使用
* Input          : None
* Output         : None
* Return         : 返回当前的全局中断状态
*******************************************************************************/
UINT8 CH395CMDGetGlobIntStatus(void)
{
    UINT8 init_status;

    xWriteCH395Cmd(CMD01_GET_GLOB_INT_STATUS);
    init_status = xReadCH395Data();
    xEndCH395Cmd();
    return  init_status;
}

/********************************************************************************
* Function Name  : CH395CMDInitCH395
* Description    : 初始化CH395芯片。
* Input          : None
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
UINT8 CH395CMDInitCH395(void)
{
    UINT8 i = 0;
    UINT8 s = 0;

    xWriteCH395Cmd(CMD0W_INIT_CH395);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(10);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出,本函数需要500MS以上执行完毕 */
    }
    return s;
}

/********************************************************************************
* Function Name  : CH395CMDSetUartBaudRate
* Description    : 设置CH395串口波特率，仅在串口模式下有效
* Input          : baudrate 串口波特率
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetUartBaudRate(UINT32 baudrate)
{
	//UINT8 i;
    xWriteCH395Cmd(CMD31_SET_BAUDRATE);
    xWriteCH395Data((UINT8)baudrate);
    xWriteCH395Data((UINT8)((UINT16)baudrate >> 8));
    xWriteCH395Data((UINT8)(baudrate >> 16));
	//i = xReadCH395Data();
    xReadCH395Data();
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395GetCmdStatus
* Description    : 获取命令执行状态，某些命令需要等待命令执行结果
* Input          : None
* Output         : None
* Return         : 返回上一条命令执行状态
*******************************************************************************/
UINT8 CH395GetCmdStatus(void)
{
    UINT8 i;

    xWriteCH395Cmd(CMD01_GET_CMD_STATUS);
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

/********************************************************************************
* Function Name  : CH395CMDSetIPAddr
* Description    : 设置CH395的IP地址
* Input          : ipaddr 指IP地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetIPAddr(UINT8 *ipaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD40_SET_IP_ADDR);
    for(i = 0; i < 4;i++)xWriteCH395Data(*ipaddr++);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetGWIPAddr
* Description    : 设置CH395的网关IP地址
* Input          : ipaddr 指向网关IP地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetGWIPAddr(UINT8 *gwipaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD40_SET_GWIP_ADDR);
    for(i = 0; i < 4;i++)xWriteCH395Data(*gwipaddr++);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetMASKAddr
* Description    : 设置CH395的子网掩码，默认为255.255.255.0
* Input          : maskaddr 指子网掩码地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetMASKAddr(UINT8 *maskaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD40_SET_MASK_ADDR);
    for(i = 0; i < 4;i++)xWriteCH395Data(*maskaddr++);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetMACAddr
* Description    : 设置CH395的MAC地址。
* Input          : amcaddr MAC地址指针
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetMACAddr(UINT8 *amcaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD60_SET_MAC_ADDR);
    for(i = 0; i < 6;i++)xWriteCH395Data(*amcaddr++);
    xEndCH395Cmd();
    mDelaymS(100); 
}

/********************************************************************************
* Function Name  : CH395CMDGetMACAddr
* Description    : 获取CH395的MAC地址。
* Input          : amcaddr MAC地址指针
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDGetMACAddr(UINT8 *amcaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD06_GET_MAC_ADDR);
    for(i = 0; i < 6;i++)*amcaddr++ = xReadCH395Data();
    xEndCH395Cmd();
 }

/*******************************************************************************
* Function Name  : CH395CMDSetMACFilt
* Description    : 设置MAC过滤。
* Input          : filtype 参考 MAC过滤
                   table0 Hash0
                   table1 Hash1
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetMACFilt(UINT8 filtype,UINT32 table0,UINT32 table1)
{
    xWriteCH395Cmd(CMD90_SET_MAC_FILT);
    xWriteCH395Data(filtype);
    xWriteCH395Data((UINT8)table0);
    xWriteCH395Data((UINT8)((UINT16)table0 >> 8));
    xWriteCH395Data((UINT8)(table0 >> 16));
    xWriteCH395Data((UINT8)(table0 >> 24));

    xWriteCH395Data((UINT8)table1);
    xWriteCH395Data((UINT8)((UINT16)table1 >> 8));
    xWriteCH395Data((UINT8)(table1 >> 16));
    xWriteCH395Data((UINT8)(table1 >> 24));
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDGetUnreachIPPT
* Description    : 获取不可达信息 (IP,Port,Protocol Type)
* Input          : list 保存获取到的不可达
                        第1个字节为不可达代码，请参考 不可达代码(CH395INC.H)
                        第2个字节为IP包协议类型
                        第3-4字节为端口号
                        第4-8字节为IP地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDGetUnreachIPPT(UINT8 *list)
{
    UINT8 i;

    xWriteCH395Cmd(CMD08_GET_UNREACH_IPPORT);
    for(i = 0; i < 8; i++)
    {
        *list++ = xReadCH395Data();
    }   
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDGetRemoteIPP
* Description    : 获取远端的IP和端口地址，一般在TCP Server模式下使用
* Input          : sockindex Socket索引
                   list 保存IP和端口
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDGetRemoteIPP(UINT8 sockindex,UINT8 *list)
{
    UINT8 i;

    xWriteCH395Cmd(CMD06_GET_REMOT_IPP_SN);
    xWriteCH395Data(sockindex);
    for(i = 0; i < 6; i++)
    {
        *list++ = xReadCH395Data();
    }   
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketDesIP
* Description    : 设置socket n的目的IP地址
* Input          : sockindex Socket索引
                   ipaddr 指向IP地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketDesIP(UINT8 sockindex,UINT8 *ipaddr)
{
    xWriteCH395Cmd(CMD50_SET_IP_ADDR_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(*ipaddr++);
    xWriteCH395Data(*ipaddr++);
    xWriteCH395Data(*ipaddr++);
    xWriteCH395Data(*ipaddr++);
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketProtType
* Description    : 设置socket 的协议类型
* Input          : sockindex Socket索引
                   prottype 协议类型，请参考 socket协议类型定义(CH395INC.H)
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketProtType(UINT8 sockindex,UINT8 prottype)
{
    xWriteCH395Cmd(CMD20_SET_PROTO_TYPE_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(prottype);
    xEndCH395Cmd();
}

/*******************************************************************************

* Function Name  : CH395SetSocketDesPort
* Description    : 设置socket n的协议类型
* Input          : sockindex Socket索引
                   desprot 2字节目的端口
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketDesPort(UINT8 sockindex,UINT16 desprot)
{
    xWriteCH395Cmd(CMD30_SET_DES_PORT_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data((UINT8)desprot);
    xWriteCH395Data((UINT8)(desprot >> 8));
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketSourPort
* Description    : 设置socket n的协议类型
* Input          : sockindex Socket索引
                   desprot 2字节源端口
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketSourPort(UINT8 sockindex,UINT16 surprot)
{
    xWriteCH395Cmd(CMD30_SET_SOUR_PORT_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data((UINT8)surprot);
    xWriteCH395Data((UINT8)(surprot>>8));
    xEndCH395Cmd();
}

/******************************************************************************
* Function Name  : CH395SetSocketIPRAWProto
* Description    : IP模式下，socket IP包协议字段
* Input          : sockindex Socket索引
                   prototype IPRAW模式1字节协议字段
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketIPRAWProto(UINT8 sockindex,UINT8 prototype)
{
    xWriteCH395Cmd(CMD20_SET_IPRAW_PRO_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(prototype);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395EnablePing
* Description    : 开启/关闭 PING
* Input          : enable : 1  开启PING
                          ：0  关闭PING
* Output         : None
* Return         : None
*******************************************************************************/
void CH395EnablePing(UINT8 enable)
{
    xWriteCH395Cmd(CMD01_PING_ENABLE);
    xWriteCH395Data(enable);
    xEndCH395Cmd();
}


/********************************************************************************
* Function Name  : CH395SendData
* Description    : 向发送缓冲区写数据
* Input          : sockindex Socket索引
                   databuf  数据缓冲区
                   len   长度
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SendData(UINT8 sockindex,UINT8 *databuf,UINT16 len)
{
    //UINT16 i;

    xWriteCH395Cmd(CMD30_WRITE_SEND_BUF_SN);
    xWriteCH395Data((UINT8)sockindex);
    xWriteCH395Data((UINT8)len);
    xWriteCH395Data((UINT8)(len>>8));
    mDelayuS(1);   
/*
    for(i = 0; i < len; i++)
    {
        xWriteCH395Data(*databuf++);
    }
*/
    xWriteCH395DataBlock(databuf,len);
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395GetRecvLength
* Description    : 获取接收缓冲区长度
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回接收缓冲区有效长度
*******************************************************************************/
UINT16 CH395GetRecvLength(UINT8 sockindex)
{
    UINT16 i;

    xWriteCH395Cmd(CMD12_GET_RECV_LEN_SN);
    xWriteCH395Data((UINT8)sockindex);
    i = xReadCH395Data();
    i = (UINT16)(xReadCH395Data()<<8) + i;
    xEndCH395Cmd();
    return i;
}

/*******************************************************************************
* Function Name  : CH395ClearRecvBuf
* Description    : 清除接收缓冲区
* Input          : sockindex Socket索引
* Output         : None
* Return         : None
*******************************************************************************/
void CH395ClearRecvBuf(UINT8 sockindex)
{
    xWriteCH395Cmd(CMD10_CLEAR_RECV_BUF_SN);
    xWriteCH395Data((UINT8)sockindex);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395GetRecvLength
* Description    : 读取接收缓冲区数据
* Input          : sockindex Socket索引
                   len   长度
                   pbuf  缓冲区
* Output         : None
* Return         : None
*******************************************************************************/
void CH395GetRecvData(UINT8 sockindex,UINT16 len,UINT8 *pbuf)
{
    //UINT16 i;
    if(!len)return;
    xWriteCH395Cmd(CMD30_READ_RECV_BUF_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data((UINT8)len);
    xWriteCH395Data((UINT8)(len>>8));
    mDelayuS(1);
/*
    for(i = 0; i < len; i++)
    {
       *pbuf = xReadCH395Data();
       pbuf++;
    }
*/
    xReadCH395DataBlock(pbuf,len);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetRetryCount
* Description    : 设置重试次数
* Input          : count 重试值，最大为20次
* Output         : None
* Return         : None
********************************************************************************/
void CH395CMDSetRetryCount(UINT8 count)
{
    xWriteCH395Cmd(CMD10_SET_RETRAN_COUNT);
    xWriteCH395Data(count);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetRetryPeriod
* Description    : 设置重试周期
* Input          : period 重试周期单位为毫秒，最大1000ms
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetRetryPeriod(UINT16 period)
{
    xWriteCH395Cmd(CMD10_SET_RETRAN_COUNT);
    xWriteCH395Data((UINT8)period);
    xWriteCH395Data((UINT8)(period>>8));
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDGetSocketStatus
* Description    : 获取socket
* Input          : None
* Output         : socket n的状态信息，第1字节为socket 打开或者关闭
                   第2字节为TCP状态
* Return         : None
*******************************************************************************/
void CH395CMDGetSocketStatus(UINT8 sockindex,UINT8 *status)
{
    xWriteCH395Cmd(CMD12_GET_SOCKET_STATUS_SN);
    xWriteCH395Data(sockindex);
    *status++ = xReadCH395Data();
    *status++ = xReadCH395Data();
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395OpenSocket
* Description    : 打开socket，此命令需要等待执行成功
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
UINT8  CH395OpenSocket(UINT8 sockindex)
{
    UINT8 i = 0;
    UINT8 s = 0;
    xWriteCH395Cmd(CMD1W_OPEN_SOCKET_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(5);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/*******************************************************************************
* Function Name  : CH395CloseSocket
* Description    : 关闭socket，
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
UINT8  CH395CloseSocket(UINT8 sockindex)
{
    UINT8 i = 0;
    UINT8 s = 0;
    xWriteCH395Cmd(CMD1W_CLOSE_SOCKET_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(5);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/********************************************************************************
* Function Name  : CH395TCPConnect
* Description    : TCP连接，仅在TCP模式下有效，此命令需要等待执行成功
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
UINT8 CH395TCPConnect(UINT8 sockindex)
{
    UINT8 i = 0;
    UINT8 s = 0;
    xWriteCH395Cmd(CMD1W_TCP_CONNECT_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(5);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/******************************************************************************
* Function Name  : CH395TCPListen
* Description    : TCP监听，仅在TCP模式下有效，此命令需要等待执行成功
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
UINT8 CH395TCPListen(UINT8 sockindex)
{
    UINT8 i = 0;
    UINT8 s = 0;
    xWriteCH395Cmd(CMD1W_TCP_LISTEN_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(5);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/********************************************************************************
* Function Name  : CH395TCPDisconnect
* Description    : TCP断开，仅在TCP模式下有效，此命令需要等待执行成功
* Input          : sockindex Socket索引
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 CH395TCPDisconnect(UINT8 sockindex)
{
    UINT8 i = 0;
    UINT8 s = 0;
    xWriteCH395Cmd(CMD1W_TCP_DISNCONNECT_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(5);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/*******************************************************************************
* Function Name  : CH395GetSocketInt
* Description    : 获取socket n的中断状态
* Input          : sockindex   socket索引
* Output         : None
* Return         : 中断状态
*******************************************************************************/
UINT8 CH395GetSocketInt(UINT8 sockindex)
{
    UINT8 intstatus;
    xWriteCH395Cmd(CMD11_GET_INT_STATUS_SN);
    xWriteCH395Data(sockindex);
    mDelayuS(2);
    intstatus = xReadCH395Data();
    xEndCH395Cmd();
    return intstatus;
}

/*******************************************************************************
* Function Name  : CH395CRCRet6Bit
* Description    : 对多播地址进行CRC运算，并取高6位。
* Input          : mac_addr   MAC地址
* Output         : None
* Return         : 返回CRC32的高6位
*******************************************************************************/
UINT8 CH395CRCRet6Bit(UINT8 *mac_addr)
{
    INT32 perByte;
    INT32 perBit;
    const UINT32 poly = 0x04C11DB7;
    UINT32 crc_value = 0xFFFFFFFF;
    UINT8 c;
    for ( perByte = 0; perByte < 6; perByte ++ ) 
    {
        c = *(mac_addr++);
        for ( perBit = 0; perBit < 8; perBit++ ) 
        {
            crc_value = (crc_value<<1)^((((crc_value>>31)^c)&0x01)?poly:0);
            c >>= 1;
        }
    }
    crc_value=crc_value>>26;                                      
    return ((UINT8)crc_value);
}

/******************************************************************************
* Function Name  : CH395DHCPEnable
* Description    : 启动/停止DHCP
* Input          : flag   1:启动DHCP;0：停止DHCP
* Output         : None
* Return         : 执行状态
*******************************************************************************/
UINT8  CH395DHCPEnable(UINT8 flag)
{
    UINT8 i = 0;
    UINT8 s;
    xWriteCH395Cmd(CMD10_DHCP_ENABLE);
    xWriteCH395Data(flag);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(20);
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/******************************************************************************
* Function Name  : CH395GetDHCPStatus
* Description    : 获取DHCP状态
* Input          : None
* Output         : None
* Return         : DHCP状态，0为成功，其他值表示错误
*******************************************************************************/
UINT8 CH395GetDHCPStatus(void)
{
    UINT8 status;
    xWriteCH395Cmd(CMD01_GET_DHCP_STATUS);
    status = xReadCH395Data();
    xEndCH395Cmd();
    return status;
}

/*******************************************************************************
* Function Name  : CH395GetIPInf
* Description    : 获取IP，子网掩码和网关地址
* Input          : None
* Output         : 12个字节的IP,子网掩码和网关地址
* Return         : None
*******************************************************************************/
void CH395GetIPInf(UINT8 *addr)
{
    UINT8 i;
    xWriteCH395Cmd(CMD014_GET_IP_INF);
    for(i = 0; i < 20; i++)
    {
     *addr++ = xReadCH395Data();
    }
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395WriteGPIOAddr
* Description    : 写GPIO寄存器
* Input          : regadd   寄存器地址
*                ：regval   寄存器值
* Output         : None
* Return         : None
*******************************************************************************/
void CH395WriteGPIOAddr(UINT8 regadd,UINT8 regval)
{
    xWriteCH395Cmd(CMD20_WRITE_GPIO_REG);
    xWriteCH395Data(regadd);
    xWriteCH395Data(regval);
}

/*******************************************************************************
* Function Name  : CH395ReadGPIOAddr
* Description    : 读GPIO寄存器
* Input          : regadd   寄存器地址
* Output         : None
* Return         : 寄存器的值
*******************************************************************************/
UINT8 CH395ReadGPIOAddr(UINT8 regadd)
{
    UINT8 i;
    xWriteCH395Cmd(CMD10_READ_GPIO_REG);
    xWriteCH395Data(regadd);
    mDelaymS(1);
    i = xReadCH395Data();
    return i;
}

/*******************************************************************************
* Function Name  : CH395EEPROMErase
* Description    : 擦除EEPROM
* Input          : None
* Output         : None
* Return         : 执行状态
*******************************************************************************/
UINT8 CH395EEPROMErase(void)
{
    UINT8 i;    
    xWriteCH395Cmd(CMD00_EEPROM_ERASE);
    while(1)
    {
        mDelaymS(20);
       i = CH395GetCmdStatus();
       if(i == CH395_ERR_BUSY)continue;
       break;
    }
    return i;
}

/*******************************************************************************
* Function Name  : CH395EEPROMWrite
* Description    : 写EEPROM
* Input          : eepaddr  EEPROM地址
*                ：buf      缓冲区地址
*                ：len      长度
* Output         : None
* Return         : 执行状态
*******************************************************************************/
UINT8 CH395EEPROMWrite(UINT16 eepaddr,UINT8 *buf,UINT8 len)
{
    UINT8 i;
    xWriteCH395Cmd(CMD30_EEPROM_WRITE);
    xWriteCH395Data((UINT8)(eepaddr));
    xWriteCH395Data((UINT8)(eepaddr >> 8));
    xWriteCH395Data(len);  
    while(len--)xWriteCH395Data(*buf++);
    while(1)
    {
        mDelaymS(20);
       i = CH395GetCmdStatus();
       if(i == CH395_ERR_BUSY)continue;
       break;
    }
    return i;
}
  
/*******************************************************************************
* Function Name  : CH395EEPROMRead
* Description    : 写EEPROM
* Input          : eepaddr  EEPROM地址
*                ：buf      缓冲区地址
*                ：len      长度
* Output         : None
* Return         : None
*******************************************************************************/
void CH395EEPROMRead(UINT16 eepaddr,UINT8 *buf,UINT8 len)
{
    xWriteCH395Cmd(CMD30_EEPROM_READ);
    xWriteCH395Data((UINT8)(eepaddr));
    xWriteCH395Data((UINT8)(eepaddr >> 8));
    xWriteCH395Data(len);  
    mDelaymS(1);
    while(len--)*buf++ = xReadCH395Data();
}

/*******************************************************************************
* Function Name  : CH395SetTCPMss
* Description    : 设置TCP MSS值
* Input          : tcpmss 
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetTCPMss(UINT16 tcpmss)
{
    xWriteCH395Cmd(CMD20_SET_TCP_MSS);
    xWriteCH395Data((UINT8)(tcpmss));
    xWriteCH395Data((UINT8)(tcpmss >> 8));
}

/*******************************************************************************
* Function Name  : CH395SetSocketRecvBuf
* Description    : 设置Socket接收缓冲区
* Input          : sockindex  socket索引
                 ：startblk   起始地址
                 ：blknum     单位缓冲区个数 ，单位为512字节
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketRecvBuf(UINT8 sockindex,UINT8 startblk,UINT8 blknum)
{
    xWriteCH395Cmd(CMD30_SET_RECV_BUF);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(startblk);
    xWriteCH395Data(blknum);
}

/*******************************************************************************
* Function Name  : CH395SetSocketSendBuf
* Description    : 设置Socket发送缓冲区
* Input          : sockindex  socket索引
                 ：startblk   起始地址
                 ：blknum     单位缓冲区个数
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketSendBuf(UINT8 sockindex,UINT8 startblk,UINT8 blknum)
{
    xWriteCH395Cmd(CMD30_SET_SEND_BUF);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(startblk);
    xWriteCH395Data(blknum);
}

/*******************************************************************************
* Function Name  : CH395UDPSendTo
* Description    : UDP向指定的IP和端口发送数据
* Input          : buf     : 发送数据缓冲区
                   len     : 发送数据长度
				   ip      : 目标IP
				   port    : 目标端口
				   sockeid : socket索引值
* Output         : None
* Return         : None
*******************************************************************************/
void CH395UDPSendTo(UINT8 *buf,UINT32 len,UINT8 *ip,UINT16 port,UINT8 sockindex)
{
    CH395SetSocketDesIP(sockindex,ip);                            /* 设置socket 0目标IP地址 */         
    CH395SetSocketDesPort(sockindex,port);
    CH395SendData(sockindex,buf,len);    
}

/*******************************************************************************
* Function Name  : CH395SetStartPara
* Description    : 设置CH395启动参数
* Input          : mdata
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetStartPara(UINT32 mdata)
{
    xWriteCH395Cmd(CMD40_SET_FUN_PARA);
    xWriteCH395Data((UINT8)mdata);
    xWriteCH395Data((UINT8)((UINT16)mdata>>8));
    xWriteCH395Data((UINT8)(mdata >> 16));
    xWriteCH395Data((UINT8)(mdata >> 24));
}

/*******************************************************************************
* Function Name  : CH395CMDGetGlobIntStatus
* Description    : 获取全局中断状态，收到此命令CH395自动取消中断,0x44及以上版本使用
* Input          : None
* Output         : None
* Return         : 返回当前的全局中断状态
*******************************************************************************/
UINT16 CH395CMDGetGlobIntStatus_ALL(void)
{
		UINT16 init_status;
		xWriteCH395Cmd(CMD02_GET_GLOB_INT_STATUS_ALL);
		mDelayuS(2);
		init_status = xReadCH395Data();
		init_status = (UINT16)(xReadCH395Data()<<8) + init_status;
		xEndCH395Cmd();
		return 	init_status;
}

/*******************************************************************************
* Function Name  : CH395SetKeepLive
* Description    : 设置keepalive功能
* Input          : sockindex Socket号
*                  cmd 0：关闭 1：开启
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetKeepLive(UINT8 sockindex,UINT8 cmd)
{
    xWriteCH395Cmd(CMD20_SET_KEEP_LIVE_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(cmd);
}

/*******************************************************************************
* Function Name  : CH395KeepLiveCNT
* Description    : 设置keepalive重试次数
* Input          : cnt 重试次数（）
* Output         : None
* Return         : None
*******************************************************************************/
void CH395KeepLiveCNT(UINT8 cnt)
{
    xWriteCH395Cmd(CMD10_SET_KEEP_LIVE_CNT);
    xWriteCH395Data(cnt);
}

/*******************************************************************************
* Function Name  : CH395KeepLiveIDLE
* Description    : 设置KEEPLIVE空闲
* Input          : idle 空闲时间（单位：ms）
* Output         : None
* Return         : None
*******************************************************************************/
void CH395KeepLiveIDLE(UINT32 idle)
{
    xWriteCH395Cmd(CMD40_SET_KEEP_LIVE_IDLE);
    xWriteCH395Data((UINT8)idle);
    xWriteCH395Data((UINT8)((UINT16)idle>>8));
    xWriteCH395Data((UINT8)(idle >> 16));
    xWriteCH395Data((UINT8)(idle >> 24));
}

/*******************************************************************************
* Function Name  : CH395KeepLiveINTVL
* Description    : 设置KeepLive间隔时间 
* Input          : intvl 间隔时间（单位：ms）
* Output         : None
* Return         : None
*******************************************************************************/
void CH395KeepLiveINTVL(UINT32 intvl)
{
    xWriteCH395Cmd(CMD40_SET_KEEP_LIVE_INTVL);
    xWriteCH395Data((UINT8)intvl);
    xWriteCH395Data((UINT8)((UINT16)intvl>>8));
    xWriteCH395Data((UINT8)(intvl >> 16));
    xWriteCH395Data((UINT8)(intvl >> 24));
}

/*******************************************************************************
* Function Name  : CH395SetTTLNum
* Description    : 设置TTL
* Input          : sockindex Socket号
*                  TTLnum:TTL数
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetTTLNum(UINT8 sockindex,UINT8 TTLnum)
{
    xWriteCH395Cmd(CMD20_SET_TTL);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(TTLnum);
}

/**************************** endfile *************************************/



