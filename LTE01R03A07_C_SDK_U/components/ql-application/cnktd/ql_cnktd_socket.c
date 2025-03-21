/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"

#include "ql_cnktd_socket.h"

#include "ql_log.h"

int32_t qapi_Net_DNSc_Is_Started(void)
{
    return 1;
}

qapi_Status_t qapi_DSS_Get_Device_Name(qapi_DSS_Hndl_t hndl, char *buf, int len)
{
    qapi_Status_t status = QAPI_OK;

    return status;
}

#if 0
int32_t qapi_Net_DNSc_Reshost_on_iface(char *hostname, struct ip46addr *addr, char *iface_index)
{
    return 0
}
#endif


int32_t qapi_Net_DNSc_Command(qapi_Net_DNS_Command_t cmd)
{
    return 0;
}

qapi_Status_t qapi_Net_DNSc_Set_Client_timeout(uint32_t timeout_ms)
{
    qapi_Status_t status = QAPI_QT_ERR_OK;

    return status;
}

int32_t qapi_Net_DNSc_Add_Server_on_iface(char *svr_addr, uint32_t id, int profile_idx)
{
	qapi_Status_t status = QAPI_OK;
	ql_datacall_errcode_e ret = QL_DATACALL_SUCCESS;
	ql_datacall_dns_info_s dns_pri = {0};
	ql_datacall_dns_info_s dns_sec = {0};

    switch(id)
    {
       case QAPI_NET_DNS_V4_PRIMARY_SERVER_ID:
	        dns_pri.type = QL_PDP_TYPE_IP;
			ip4addr_aton(svr_addr, &(dns_pri.ip4));
	        ret = ql_datacall_set_dns_addr(0, profile_idx, &dns_pri, NULL);
	   break;
       case QAPI_NET_DNS_V4_SECONDARY_SERVER_ID:
	   	    dns_sec.type = QL_PDP_TYPE_IP;
			ip4addr_aton(svr_addr, &(dns_sec.ip4));
	        ret = ql_datacall_set_dns_addr(0, profile_idx, NULL, &dns_sec);
	   break;
	   case QAPI_NET_DNS_V6_PRIMARY_SERVER_ID:
	   	    dns_pri.type = QL_PDP_TYPE_IPV6;
			ip6addr_aton(svr_addr, &(dns_pri.ip6));
	   	    ret = ql_datacall_set_dns_addr(0, profile_idx, &dns_pri, NULL);
	   break;
	   case QAPI_NET_DNS_V6_SECONDARY_SERVER_ID:
	   	    dns_pri.type = QL_PDP_TYPE_IPV6;
			ip6addr_aton(svr_addr, &(dns_sec.ip6));
	        ret = ql_datacall_set_dns_addr(0, profile_idx, NULL, &dns_sec);
	   break;
	   case QAPI_NET_DNS_ANY_SERVER_ID:
	   default:
	   	    status = -1;
	   break;
	}
	if(ret != QL_DATACALL_SUCCESS)
	{
    	status = -1;
	}
	return status;
}

int32_t qapi_Net_DNSc_Reshost_on_iface(char *hostname, struct addrinfo **addr, int profile_idx)
{
	return getaddrinfo_with_pcid(hostname, NULL, NULL, addr, profile_idx);
}

qapi_Status_t qapi_fd_zero(fd_set *set)
{
    if(set == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
	else
	{
		FD_ZERO(set);
		return QAPI_OK;
	}
}

int qapi_socket(int32_t family, int32_t type, int32_t protocol)
{
	return socket(family, type, protocol);
}

qapi_Status_t qapi_fd_set(int32_t handle, fd_set *set)
{
    if(set == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
	else
    {
		FD_SET(handle, set);
	    return QAPI_OK;
	}
}

qapi_Status_t qapi_fd_clr(int32_t handle, fd_set *set)
{
    if(set == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
	else
	{
		FD_CLR(handle, set);
		return QAPI_OK;
	}
}

qapi_Status_t qapi_socketclose(int32_t handle)
{
    return close(handle);
}

int qapi_sendto(int32_t handle, char *buf, int32_t len, int32_t flags, struct sockaddr *to, int32_t tolen)
{
    if(buf == NULL || to == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    return sendto(handle, buf, len, flags, to, tolen);
}

int qapi_select(int maxfdp1, fd_set *rd, fd_set *wr, fd_set *ex, int32_t timeout_ms)
{
	struct timeval tm;
	tm.tv_sec = timeout_ms/1000;
    tm.tv_usec = timeout_ms%1000*1000;
    return select(maxfdp1, rd, wr, ex, &tm);
}

qapi_Status_t qapi_fd_isset(int32_t handle, fd_set *set)
{
    if(set == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    return FD_ISSET(handle, set);
}

int qapi_recvfrom(int32_t handle, char *buf, int32_t len, int32_t flags, struct sockaddr *from, int32_t *fromlen)
{
    if(buf == NULL || from == NULL || fromlen == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    return recvfrom(handle, buf, len, flags, from, (socklen_t *)fromlen);
}

qapi_Status_t qapi_connect(int32_t handle, struct sockaddr *srvaddr, int32_t addrlen)
{
    if(srvaddr == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    return connect(handle, srvaddr, addrlen);
}

int qapi_send(int32_t handle, char *buf, int32_t len, int32_t flags)
{
    if(buf == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    return send(handle, buf, len, flags);
}

int qapi_recv(int32_t handle, char *buf, int32_t len, int32_t flags)
{
    if(buf == NULL)
    {
        return QAPI_ERR_INVALID_PARAM;
    }
    return recv(handle, buf, len, flags);
}





