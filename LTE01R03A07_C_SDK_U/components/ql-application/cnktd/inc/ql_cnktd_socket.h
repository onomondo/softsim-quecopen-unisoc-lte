/**  @file
  ql_cnktd_socket.h

  @brief
  This file is used to define cnktd socket for different Quectel Project.

*/

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


#ifndef QL_CNKTD_SOCKET_H
#define QL_CNKTD_SOCKET_H

#include "ql_cnktd.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sockets.h"
#include "lwip/ip_addr.h"
#include "lwip/ip6_addr.h"

#include "lwip/netdb.h"
#include "lwip/netif.h"
#include "lwip/inet.h"
#include "lwip/tcp.h"

#include "ql_api_datacall.h"

#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
#if 0
/**
 * Socket IPv4/IPv6 Internet address union.
 */
struct ip46addr
{
    uint16_t type;
    /**< AF_INET or AF_INET6. */
    union 
    {
        unsigned long   addr4;
        /**< IPv4 address. */
        ip6_addr        addr6;
        /**< IPv6 address. */
    } a;
    /**< Address union. */
};
#endif

/** Commands to start/stop/disable a DNS client. */

typedef enum {
    QAPI_NET_DNS_DISABLE_E, /**< Functionality is deprecated. Do not use. */
    QAPI_NET_DNS_START_E,   /**< Allocate space for internal data structures when called for
                                 the first time. For subsequent calls, increases the ref count; DNS query
                                 is allowed after the start command. Processes DNS responses from
                                 the server. */
    QAPI_NET_DNS_STOP_E     /**< Stop sending DNS requests and processing DNS responses;
                                 keeps internal data structures. Frees the space for internal data structures
                                 only when the ref count reaches 0. */
} qapi_Net_DNS_Command_t;

/* Number of DNS servers in the system, which is a tunable configuration.
 *  Use ANY_SERVER_ID to populate a free entry, or use an index to update a specific entry.
 */
#define QAPI_NET_DNS_ANY_SERVER_ID         0xFFFF
/** DNS IPv4 primary server ID. */
#define QAPI_NET_DNS_V4_PRIMARY_SERVER_ID    0
/** DNS IPv4 secondary server ID. */
#define QAPI_NET_DNS_V4_SECONDARY_SERVER_ID  1
/** DNS IPv6 primary server ID. */
#define QAPI_NET_DNS_V6_PRIMARY_SERVER_ID    2
/** DNS IPv6 secondary server ID. */
#define QAPI_NET_DNS_V6_SECONDARY_SERVER_ID  3

/** Infinite time for the timeout_ms argument in qapi_select(). */
#define     QAPI_NET_WAIT_FOREVER   (0xFFFFFFFF)

/*========================================================================
 *  function Definition
 *========================================================================*/

/** @ingroup qapi_Net_DNSc_Is_Started
 *  Checks whether the DNS client has started.
 *
 * @return 0 if not started or 1 if started.
 */
 //unnecessary at EG915U,Just return true then.
int32_t qapi_Net_DNSc_Is_Started(void);

/** @ingroup qapi_DSS_Get_Device_Name

    Queries the data interface name for the data call associated with the
    specified data service handle.

    @note1hang
    len must be at least QAPI_DSS_CALL_INFO_DEVICE_NAME_MAX_LEN + 1 long.

    @param[in] hndl Handle received from qapi_DSS_Get_Data_Srvc_Hndl().
    @param[out] buf Buffer to hold the data interface name string.
    @param[in] len Length of the buffer allocated by the client.

    @return
    QAPI_OK -- Data interface name was returned successfully. \n
    QAPI_ERROR -- Data interface name was not returned successfully.

    @dependencies
    qapi_DSS_Init() must have been called first. \n
    A valid handle must be obtained by qapi_DSS_Get_Data_Srvc_Hndl().
*/
/*=========================================================================*/
//suggest to ignore this API, use dns resolution API directly.
qapi_Status_t qapi_DSS_Get_Device_Name(qapi_DSS_Hndl_t hndl, char *buf, int len);

#if 0
/** @ingroup qapi_Net_DNSc_Reshost_Iface
 *  Resolves an IP address text string into an actual IP address for an interface.
 *
 * @param[in] hostname  Pointer to an IP address string or host name string.
 * @param[in] addr      Pointer to struct ip46addr for the resolved IP address.
 *                      The caller must specify which IP address (v4 or v6) it intends
 *                      to resolve to: \n
 *                      If addr type is AF_INET, resolve to an IPv4 address. \n
 *                      If addr type is AF_INET6, resolve to an IPv6 address.
 * @param[in] iface_index Name of the PDN/APN for which the address text string is to be resolved.
 *
 * @return On success, 0 is returned. On error, < 0 is returned.
 */

int32_t qapi_Net_DNSc_Reshost_on_iface(char *hostname, struct ip46addr *addr, char *iface_index);
#endif

/** @ingroup qapi_Net_DNSc_Command
*
* Starts, stops, or disables the DNS client.
*
* @param[in] cmd   Command to start/stop/disable the DNS client.
*                  The supported commands are QAPI_NET_DNS_START_E, 
*                  QAPI_NET_DNS_STOP_E, and QAPI_NET_DNS_DISABLE_E.
* @return On success, 0 is returned. On error, -1 is returned.
*/
//unnecessary at EG915U,Just return true then.
int32_t qapi_Net_DNSc_Command(qapi_Net_DNS_Command_t cmd);

/** @} */

/** @ingroup qapi_Net_DNSc_Set_Client_timeout
 * Sets the DNSc client timeout value.
 *
 * @param[in] timeout_ms  Timeout value in millisecond.
 *
 * @return On success, QAPI_OK is returned. On error, QAPI_ERROR is returned.
 *
 */
qapi_Status_t qapi_Net_DNSc_Set_Client_timeout(uint32_t timeout_ms);

/** @ingroup qapi_Net_DNSc_Add_Server_Iface
 *  Adds a DNS server to a PDN interface.
 *
 * @param[in] svr_addr  Pointer to DNS server's IP address string.
 *
 * @param[in] id    Server ID can be QAPI_NET_DNS_V4_PRIMARY_SERVER_ID,
 *                  QAPI_NET_DNS_V4_SECONDARY_SERVER_ID, QAPI_NET_DNS_V6_PRIMARY_SERVER_ID,
 *                  QAPI_NET_DNS_V6_SECONDARY_SERVER_ID, or QAPI_NET_DNS_ANY_SERVER_ID.
 *
 * @param[in] profile_idx  PDP index, ranging from 1 to 7.
 *
 * @return On success, 0 is returned. On error, -1 is returned.
 */
int32_t qapi_Net_DNSc_Add_Server_on_iface(char *svr_addr, uint32_t id, int profile_idx);

/** @ingroup qapi_Net_DNSc_Reshost_Iface
 *  Resolves an IP address text string into an actual IP address for an interface.
 *
 * @param[in] hostname  Pointer to an IP address string or host name string.
 * @param[in] addr      Pointer to struct ip46addr for the resolved IP address.
 *                      The caller must specify which IP address (v4 or v6) it intends
 *                      to resolve to: \n
 *                      If addr type is AF_INET, resolve to an IPv4 address. \n
 *                      If addr type is AF_INET6, resolve to an IPv6 address.
 * @param[in] profile_idx  PDP index, ranging from 1 to 7.
 *
 * @return On success, 0 is returned. On error, < 0 is returned.
 */
int32_t qapi_Net_DNSc_Reshost_on_iface(char *hostname, struct addrinfo **addr, int profile_idx);

/** @ingroup qapi_fd_zero
 *
 * Initializes a socket that is set to zero.
 *
 * @param[in] set   Pointer to a list of sockets.
 *
 * @return On success, 0 is returned. On error, -1 is returned.
 */
qapi_Status_t qapi_fd_zero(fd_set *set);

/** @ingroup qapi_socket
 *
 * Creates an endpoint for communication.
 *
 * @param[in] family    Protocol family used for communication.
 *                      The supported families are: \n
 *                      - AF_INET     -- IPv4 Internet protocols
 *                      - AF_INET6    -- IPv6 Internet protocols @vertspace{-14}
 * @param[in] type      Transport mechanism used for communication. 
 *                      The supported types are: \n
 *                      - SOCK_STREAM     -- TCP
 *                      - SOCK_DGRAM      -- UDP @vertspace{-14}
 * @param[in] protocol  Must be set to 0.
 *
 * @return On success, a handle for the new socket is returned. \n
 * On error, -1 is returned.
 */
int qapi_socket(int32_t family, int32_t type, int32_t protocol);

/** @ingroup qapi_fd_set
 *
 * Adds a socket to the socket set.
 *
 * @param[in] handle    Socket handle returned from qapi_socket().
 * @param[in] set       Pointer to a list of sockets.
 *
 * @return On success, 0 is returned. On error, -1 is returned.
 */
qapi_Status_t qapi_fd_set(int32_t handle, fd_set *set);

/** @ingroup qapi_fd_clr
 *
 * Removes a socket from the socket set.
 *
 * @param[in] handle    Socket handle returned from qapi_socket().
 * @param[in] set       Pointer to a list of sockets.
 *
 * @return On success, 0 is returned. On error, -1 is returned.
 */
qapi_Status_t qapi_fd_clr(int32_t handle, fd_set *set);

/** @ingroup qapi_socketclose
 *
 * Closes a socket.
 *
 * @param[in] handle    Socket handle returned from qapi_socket().
 *
 * @return On success, 0 is returned. On error, -1 is returned.
 * Errors:
 *  ENOTCONN: Socket is not connected.
 *  EALREADY: Socket close already in progress.
 *  EINVAL :  Invalid socket parameter.
 */
qapi_Status_t qapi_socketclose(int32_t handle);

/** @ingroup qapi_sendto
 *
 * Sends a message on a socket to a target.
 *
 * @param[in] handle    Socket handle returned from qapi_socket().
 * @param[in] buf       Pointer to a buffer containing the message to be sent.
 * @param[in] len       Number of bytes to send.       
 * @param[in] flags     0, or it is formed by ORing one or more of: \n
 *                      - MSG_OOB -- Sends out-of-band data on sockets that support this
 *                      notion (e.g., of type SOCK_STREAM); the underlying protocol
 *                      must also support out-of-band data.
 *                      - MSG_DONTWAIT -- Enables a nonblocking operation; if the operation
 *                      blocks, the call fails with the error EAGAIN or EWOULDBLOCK.
 *                      - MSG_DONTROUTE -- Don not use a gateway to send the packet; only send it to
 *                      hosts on directly-connected networks. This is usually used only by
 *                      diagnostic or routing programs. @vertspace{-14} 
 * @param[in] to        Pointer to the address of the target.
 * @param[in] tolen     Size in bytes of the target address.
 *
 * @return The number of bytes sent, or -1 if an error occurred and errno is set appropriately.
 *  ENOTCONN : Socket is in disconnected state.
 *  EWOULDBLOCK : If the socket is in Non Blocking state and we have a send timeout set and
 *                we don't get any data for that duration.
 *  EPIPE: For a TCP Socket this shows that peer has disconnected the socket.
 *  ENOBUFS: No memory.
 */
int qapi_sendto(int32_t handle, char *buf, int32_t len, int32_t flags, struct sockaddr *to, int32_t tolen);

/** @ingroup qapi_select
 *
 * Monitors multiple socket handles, waiting until one or more of them become "ready"
 * for some class of I/O operation (e.g., read, write, etc.). 
 *
 * The call causes the calling process to block waiting for activity on any of a
 * list of sockets. Arrays of socket handles are passed for read, write, 
 * and exception events. A timeout in milliseconds is also passed.
 * 
 * @param[in] maxfdp1       the highest socket index in the sets.
 * @param[in] rd            Pointer to a list of read socket handles.
 * @param[in] wr            Pointer to a list of write socket handles.
 * @param[in] ex            Pointer to a list of exception socket handles.
 * @param[in] timeout_ms    Timeout values in milliseconds.
 *
 * @return The number of sockets that had an event occur and became ready.
 */
int qapi_select(int maxfdp1, fd_set *rd, fd_set *wr, fd_set *ex, int32_t timeout_ms);

/** @ingroup qapi_fd_isset
 *
 * Checks whether a socket is a member of a socket set.
 *
 * @param[in] handle    Socket handle returned from qapi_socket().
 * @param[in] set       Pointer to a list of sockets.
 *
 * @return On success, 0 is returned if the socket is not a member;
 * 1 is returned if the socket is a member. \n
 * On error, -1 is returned.
 */
qapi_Status_t qapi_fd_isset(int32_t handle, fd_set *set);

/** @ingroup qapi_recvfrom
 *
 * Receives a message from a socket.
 *
 * @param[in] handle    Socket handle returned from qapi_socket().
 * @param[in] buf       Pointer to a buffer for the received message.
 * @param[in] len       Number of bytes to receive.       
 * @param[in] flags     0, or it is formed by ORing one or more of: \n
 *                      - MSG_PEEK -- Causes the receive operation to return data from
 *                      the beginning of the receive queue without removing that data
 *                      from the queue. Thus, a subsequent receive call will return
 *                      the same data.
 *                      - MSG_OOB -- Requests receipt of out-of-band data that would not be
 *                      received in the normal data stream.
 *                      - MSG_DONTWAIT -- Enables a nonblocking operation; if the operation
 *                      blocks, the call fails with the error EAGAIN or EWOULDBLOCK. @vertspace{-14}
 * @param[in] from      If not NULL, and the underlying protocol provides the source address,
 *                      this source address is filled in. When NULL, nothing is filled in;
 *                      in this case, fromlen is not used, and should also be NULL.
 * @param[in] fromlen   This is a value-result argument, which the caller should initialize before
 *                      the call to the size of the buffer associated with from, and modified on
 *                      return to indicate the actual size of the source address.
 *
 * @return The number of bytes received, or -1 if an error occurred.
 * Errors:
 *  ENOTCONN : Socket is in disconnected state.
 *  EWOULDBLOCK : If the socket is in Non Blocking state and we have a receive timeout set and
 *                we don't get any data for that duration.
 *  ENOBUFS: No memory.
 */
int qapi_recvfrom(int32_t handle, char *buf, int32_t len, int32_t flags, struct sockaddr *from, int32_t *fromlen);

/** @ingroup qapi_connect
 *
 * Initiates a connection on a socket
 *
 * If the socket is of type SOCK_DGRAM, *svraddr is the address to which datagrams are
 * sent by default, and the only address from which datagrams are received. If the socket
 * is of type SOCK_STREAM, this call attempts to make a connection to the socket that is
 * bound to the address specified by *srvaddr.
 *
 * @param[in] handle    Socket handle returned from qapi_socket().
 * @param[in] srvaddr   Pointer to the peer's address to which the socket is connected. 
 * @param[in] addrlen   Specify the size (in bytes) of *srvaddr.
 *
 * @return On success, 0 is returned. On error, -1 is returned.
 * Errors:
 *  EINVAL : Invalid Arguments passed as argument to connect.
 *  EWOULDBLOCK : If the socket is marked as NON Blocking and connection is in establishing state and not
 *               established.
 *  ECONNABORTED : Connection is aborted.
 *  EISCONN : Connection is already established.
 *  ETIMEDOUT : Connection Establishment Timed out no response from Peer.
 *  ENOBUFFS : No Memory.
 */
qapi_Status_t qapi_connect(int32_t handle, struct sockaddr *srvaddr, int32_t addrlen);

/** @ingroup qapi_send
 *
 * Send a message on a socket.
 *
 * The call may be used only when the socket is in a connected state
 * (so that the intended recipient is known). It is equivalent to
 * qapi_sendto(handle, buf, len, flags, NULL, 0)
 *
 * @param[in] handle    Socket handle returned from qapi_socket().
 * @param[in] buf       Pointer to a buffer containing message to be sent.
 * @param[in] len       Number of bytes to send.       
 * @param[in] flags     0, or it is formed by ORing one or more of: \n
 *                      - MSG_OOB -- Sends out-of-band data on sockets that support this
 *                      notion (e.g., of type SOCK_STREAM); the underlying protocol
 *                      must also support out-of-band data.
 *                      - MSG_DONTWAIT -- Enables a nonblocking operation; if the operation
 *                      blocks, the call fails with the error EAGAIN or EWOULDBLOCK.
 *                      - MSG_DONTROUTE -- Do not use a gateway to send the packet; only send it to
 *                      hosts on directly-connected networks. This is usually used only by
 *                      diagnostic or routing programs. @vertspace{-14} 
 *
 * @return The number of bytes sent, or -1 if an error occurred and errno is set appropriately.
 *  Errors:
 *    ENOTCONN : Socket is in disconnected state.
 *    EWOULDBLOCK : If the socket is in Non Blocking state and we have a send timeout set and
 *                we don't get any data for that duration.
 *    EPIPE: For a TCP Socket this shows that peer has disconnected the socket.
 *    ENOBUFS: No memory.
 */
int qapi_send(int32_t handle, char *buf, int32_t len, int32_t flags);

/** @ingroup qapi_recv
 *
 * Receives a message from a socket.
 *
 * The qapi_recv() call is normally used only on a connected socket
 * and is identical to qapi_recvfrom(handle, buf, len, flags, NULL, NULL)
 *
 * @param[in] handle    Socket handle returned from qapi_socket().
 * @param[in] buf       Pointer to a buffer for the received message.
 * @param[in] len       Number of bytes to receive.       
 * @param[in] flags     0, or it is formed by ORing one or more of: \n
 *                      - MSG_PEEK -- Causes the receive operation to return data from
 *                      the beginning of the receive queue without removing that data
 *                      from the queue. Thus, a subsequent receive call will return
 *                      the same data.
 *                      - MSG_OOB -- Requests receipt of out-of-band data that would not be
 *                      received in the normal data stream.
 *                      - MSG_DONTWAIT -- Enables a nonblocking operation; if the operation
 *                      blocks, the call fails with the error EAGAIN or EWOULDBLOCK. @vertspace{-14} 
 *
 * @return The number of bytes received, or -1 if an error occurred.
 * Errors:
 *  ENOTCONN : Socket is in disconnected state.
 *  EWOULDBLOCK : If the socket is in Non Blocking state and we have a receive timeout set and
 *                we don't get any data for that duration.
 *  EPIPE: For a TCP Socket this shows that peer has disconnected the socket.
 *  ENOBUFS: No memory.
 */
int qapi_recv(int32_t handle, char *buf, int32_t len, int32_t flags);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_CNKTD_SOCKET_H */


