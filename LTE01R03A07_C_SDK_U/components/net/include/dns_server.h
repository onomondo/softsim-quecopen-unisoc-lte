/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _DNS_SERVER_H_
#define _DNS_SERVER_H_

#include "lwip/dns.h"

/* register callback funtion */
typedef void (*dns_server_found_callback)(struct pbuf *p, void *callback_arg);

enum
{
    ERR_DNS_PACKAGE = -17,
    ERR_CACHE_NUM = -18,
    ERR_DOMAIN = -19
};

/**
 * ///Nonblock function///
 * Receive dns requst by RNDIS, it will call lwip dns to get ipaddr, when revice dns answer package
 * it will call callback function to write back the dns answer package.
 * p:           input pbuf.
 * callback:    input callback function.
 * callbackarg: input param for callback.  
 * return:      ERR_LOCAL_OK   /Find the ipaddr in local cache, callback will be called immediately
 *              ERR_INPROGRESS /Requst ipaddr from remote server. Waiting for callback 
 *              other error message.
 */
err_t dnsServer_gethostbyname(struct pbuf *p, dns_server_found_callback callback, void *callbackarg);

#endif
