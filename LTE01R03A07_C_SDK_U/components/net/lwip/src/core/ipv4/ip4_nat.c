/**
 * NAT - NAT implementation for lwIP supporting TCP/UDP and ICMP.
 * Copyright (c) 2009 Christian Walter, ?Embedded Solutions, Vienna 2009.
 *
 * Copyright (c) 2010 lwIP project ;-)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is not a part of the lwIP TCP/IP stack.
 */

/*
 * File      : ipv4_nat.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2015, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-01-26     Hichard      porting to RT-Thread
 * 2015-01-27     Bernard      code cleanup for lwIP in RT-Thread
 * 2016-11-19     Ajay Bhargav Modified for lwIP 2.x/lwIP Head
 * 2017-04-01     Ajay Bhargav Updated license information
 */

/*
 * TODOS:
 *  - we should decide if we want to use static tables for NAT or a linked
 *    list.
 *  - we should allocate icmp ping id if multiple clients are sending
 *    ping requests.
 *  - maybe we could hash the identifiers for TCP, ICMP and UDP and use
 *    a single table structure. This would reduce the code amount although
 *    it will cost performance.
 *  - NAT code must check for broadcast addresses and NOT forward
 *    them.
 *
 *  - netif_remove must notify NAT code when a NAT'ed interface is removed
 *  - allocate NAT entries from a new memp pool instead of the heap
 *  - let ttl be ticks, not seconds
 *
 * HOWTO USE:
 *
 * Shows how to create NAT between a PPP interface and an internal NIC.
 * In this case the network 213.129.231.168/29 is nat'ed when packets
 * are sent to the destination network 10.0.0.0/24 (untypical example -
 * most users will have the other way around).
 *
 * Step 1) Execute when network interfaces are ready.
 *
 * new_nat_entry.out_if = (struct netif *)&PPP_IF;
 * new_nat_entry.in_if = (struct netif *)&EMAC_if;
 * IP4_ADDR(&new_nat_entry.source_net, 213, 129, 231, 168);
 * IP4_ADDR(&new_nat_entry.source_netmask, 255, 255, 255, 248);
 * IP4_ADDR(&new_nat_entry.dest_net, 10, 0, 0, 0);
 * IP4_ADDR(&new_nat_entry.dest_netmask, 255, 0, 0, 0);
 * ip4_nat_add(&new_nat_entry);
 */

#include "lwip/ip4_nat.h"
#include "lwip/opt.h"
#include "osi_log.h"

#if IP_NAT

#include "lwip/ip4.h"
#include "lwip/inet.h"
#include "lwip/netif.h"
#include "lwip/ip4_addr.h"
#include "lwip/icmp.h"
#include "lwip/prot/tcp.h"
#include "lwip/udp.h"
#include "lwip/mem.h"
#include "lwip/sys.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"

#include <limits.h>
#include <string.h>
#include "ql_api_datacall.h"
#include "quec_proj_config.h"
#include "netif.h"
#include "ql_api_osi.h"
/** Define this to enable debug output of this module */
#ifndef NAT_DEBUG
#define NAT_DEBUG      LWIP_DBG_OFF
#endif

#define LWIP_NAT_TTL_INFINITE                    (INT_MAX)
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
#define LWIP_NAT_DEFAULT_TCP_TTL_SECONDS         (60 * 60)
#define LWIP_NAT_TCP_TTL_ALLOW_COVERD            (5 * 60)
#if (LWIP_NAT_DEFAULT_TCP_TTL_SECONDS < LWIP_NAT_TCP_TTL_ALLOW_COVERD)
#error "LWIP_NAT_DEFAULT_TCP_TTL_SECONDS should larger than LWIP_NAT_TCP_TTL_ALLOW_COVERD"
#endif
#define LWIP_NAT_TCP_TTL_THRESHOLD_SECONDS       (LWIP_NAT_DEFAULT_TCP_TTL_SECONDS - LWIP_NAT_TCP_TTL_ALLOW_COVERD)

#define LWIP_NAT_TCP_TTL_OUT_OF_DATE             (-2)
#if (LWIP_NAT_TCP_TTL_OUT_OF_DATE >= 0)
#error "LWIP_NAT_TCP_TTL_OUT_OF_DATE should less than 0"
#endif
#endif 

#define LWIP_NAT_DEFAULT_TTL_SECONDS             (128)
#define LWIP_NAT_FORWARD_HEADER_SIZE_MIN         (sizeof(struct eth_hdr))

#define LWIP_NAT_DEFAULT_ICMP_TTL_SECONDS        (30)
#define LWIP_NAT_DEFAULT_STATE_TABLES_ICMP       (512)
#define LWIP_NAT_DEFAULT_STATE_TABLES_TCP        (1024)
#define LWIP_NAT_DEFAULT_STATE_TABLES_UDP        (1024)

#define LWIP_NAT_DEFAULT_TCP_SOURCE_PORT         (20000)
#define LWIP_NAT_DEFAULT_UDP_SOURCE_PORT         (40000)

#define IPNAT_ENTRY_RESET(x) do { \
  (x)->ttl = 0; \
} while(0)

typedef struct ip4_nat_conf
{
  struct ip4_nat_conf *next;
  ip4_nat_entry_t      entry;
} ip4_nat_conf_t;

typedef struct ip4_nat_entry_common
{
  s32_t           ttl; /* @todo: do we really need this to be signed?? */
  ip4_addr_t       source;
  ip4_addr_t       dest;
  ip4_nat_conf_t   *cfg;
} ip4_nat_entry_common_t;

typedef struct ip4_nat_entries_icmp
{
  ip4_nat_entry_common_t common;
  u16_t                 id;
  u16_t                 seqno;
} ip4_nat_entries_icmp_t;

typedef struct ip4_nat_entries_tcp
{
  ip4_nat_entry_common_t common;
  u16_t                 nport;
  u16_t                 sport;
  u16_t                 dport;
} ip4_nat_entries_tcp_t;

typedef struct ip4_nat_entries_udp
{
  ip4_nat_entry_common_t common;
  u16_t                 nport;
  u16_t                 sport;
  u16_t                 dport;
} ip4_nat_entries_udp_t;

typedef union u_nat_entry
{
  ip4_nat_entry_common_t *cmn;
  ip4_nat_entries_tcp_t  *tcp;
  ip4_nat_entries_icmp_t *icmp;
  ip4_nat_entries_udp_t  *udp;
} nat_entry_t;

static uint32_t sNATCfg = 0;
static uint16_t sNATTTL = LWIP_NAT_DEFAULT_TTL_SECONDS;

static ip4_nat_conf_t *ip4_nat_cfg = NULL;
static ip4_nat_entries_icmp_t ip4_nat_icmp_table[LWIP_NAT_DEFAULT_STATE_TABLES_ICMP];
static ip4_nat_entries_tcp_t ip4_nat_tcp_table[LWIP_NAT_DEFAULT_STATE_TABLES_TCP];
static ip4_nat_entries_udp_t ip4_nat_udp_table[LWIP_NAT_DEFAULT_STATE_TABLES_UDP];

#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
extern bool quec_is_assign_port(uint8_t nSim, int profile_idx, ql_nat_assign_type_e type, bool is_fast);
extern bool quec_is_assign_dns(uint8_t nSim, ql_nat_assign_type_e type);
#endif

/* ----------------------- Static functions (COMMON) --------------------*/
static void     ip4_nat_chksum_adjust(u8_t *chksum, const u8_t *optr, s16_t olen, const u8_t *nptr, s16_t nlen);
static void     ip4_nat_cmn_init(ip4_nat_conf_t *nat_config, const struct ip_hdr *iphdr,
                                 ip4_nat_entry_common_t *nat_entry);
static ip4_nat_conf_t *ip4_nat_shallnat(const struct ip_hdr *iphdr);
static void     ip4_nat_reset_state(ip4_nat_conf_t *cfg);

/* ----------------------- Static functions (DEBUG) ---------------------*/
#if defined(LWIP_DEBUG) && (NAT_DEBUG & LWIP_DBG_ON)
static void     ip4_nat_dbg_dump(const char *msg, const struct ip_hdr *iphdr);
static void     ip4_nat_dbg_dump_ip(const ip4_addr_t *addr);
static void     ip4_nat_dbg_dump_icmp_nat_entry(const char *msg, const ip4_nat_entries_icmp_t *nat_entry);
static void     ip4_nat_dbg_dump_tcp_nat_entry(const char *msg, const ip4_nat_entries_tcp_t *nat_entry);
static void     ip4_nat_dbg_dump_udp_nat_entry(const char *msg, const ip4_nat_entries_udp_t *nat_entry);
static void     ip4_nat_dbg_dump_init(ip4_nat_conf_t *ip4_nat_cfg_new);
static void     ip4_nat_dbg_dump_remove(ip4_nat_conf_t *cur);
#else /* defined(LWIP_DEBUG) && (NAT_DEBUG & LWIP_DBG_ON) */
#define ip4_nat_dbg_dump(msg, iphdr)
#define ip4_nat_dbg_dump_ip(addr)
#define ip4_nat_dbg_dump_icmp_nat_entry(msg, nat_entry)
#define ip4_nat_dbg_dump_tcp_nat_entry(msg, nat_entry)
#define ip4_nat_dbg_dump_udp_nat_entry(msg, nat_entry)
#define ip4_nat_dbg_dump_init(ip4_nat_cfg_new)
#define ip4_nat_dbg_dump_remove(cur)
#endif /* defined(LWIP_DEBUG) && (NAT_DEBUG & LWIP_DBG_ON) */

/* ----------------------- Static functions (TCP) -----------------------*/
static ip4_nat_entries_tcp_t *ip4_nat_tcp_lookup_incoming(const struct ip_hdr *iphdr, const struct tcp_hdr *tcphdr);
static ip4_nat_entries_tcp_t *ip4_nat_tcp_lookup_outgoing(ip4_nat_conf_t *nat_config,
                                                         const struct ip_hdr *iphdr, const struct tcp_hdr *tcphdr,
                                                         u8_t allocate);

/* ----------------------- Static functions (UDP) -----------------------*/
static ip4_nat_entries_udp_t *ip4_nat_udp_lookup_incoming(const struct ip_hdr *iphdr, const struct udp_hdr *udphdr);
static ip4_nat_entries_udp_t *ip4_nat_udp_lookup_outgoing(ip4_nat_conf_t *nat_config,
                                                         const struct ip_hdr *iphdr, const struct udp_hdr *udphdr,
                                                         u8_t allocate);

#ifdef CONFIG_QUEC_PROJECT_FEATURE
ql_mutex_t ql_nat_lock = NULL;
void quec_nat_try_lock(void)
{
	if(NULL == ql_nat_lock)
	{
        ql_rtos_mutex_create(&ql_nat_lock);
        if(NULL == ql_nat_lock)
        {
            return;
        }
	}
	ql_rtos_mutex_lock(ql_nat_lock, 0xffffffffUL);
}

void quec_nat_unlock(void)
{
	if(NULL == ql_nat_lock)
	{
		return;
	}
	ql_rtos_mutex_unlock(ql_nat_lock);
}
#endif
void set_nat_TTL(uint16_t natTTL)
{
    sNATTTL = natTTL;
}

uint16_t get_nat_TTL(void)
{
    return sNATTTL;
}
void set_nat_enable(uint32_t natCfg)
{
    sNATCfg = natCfg;
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
uint32_t get_nat_enable(void)
{
    return sNATCfg;
}
#endif

bool get_nat_enabled(uint8_t nSimId, uint8_t nCid)
{
#if IP_NAT_INTERNAL_FORWARD
    OSI_LOGI(0x0, "sNATCfg %d\n", sNATCfg);
    if(nCid == 0x0f && sNATCfg != 0) return true;
#endif
    if(nSimId > 2 || nCid > 8 || nCid < 1) return false;
    uint8_t off = 8*nSimId + nCid -1;
    if((sNATCfg>>off)&0x1)
    {
        return true;
    }
    return false;
}

ip4_nat_entry_t *add_static_route(ip4_addr_t dest_net, ip4_addr_t dest_netmask, struct netif *lan_netif, struct netif *wan_netif)
{
    if(lan_netif == NULL || wan_netif == NULL)
        return NULL;

    if((!netif_is_valid(lan_netif)) || (!netif_is_valid(wan_netif)))
        return NULL;

    if(wan_netif->link_mode != NETIF_LINK_MODE_NAT_WAN)
        return NULL;

    if((lan_netif->link_mode != NETIF_LINK_MODE_NAT_LWIP_LAN) && (lan_netif->link_mode != NETIF_LINK_MODE_NAT_PPP_LAN)
        && (lan_netif->link_mode != NETIF_LINK_MODE_NAT_NETDEV_LAN) )
        return NULL;

    ip4_nat_entry_t *nat_entry = calloc(1, sizeof(ip4_nat_entry_t));
    if(nat_entry != NULL)
    {
        nat_entry->in_if = lan_netif;
        nat_entry->out_if = wan_netif;
        ip4_addr_copy(nat_entry->dest_net, dest_net);
        ip4_addr_copy(nat_entry->dest_netmask, dest_netmask);
        IP4_ADDR(&(nat_entry->source_netmask), 255, 255, 255, 255);
        ip4_nat_add(nat_entry);
    }
    return nat_entry;
}

void remove_static_route(ip4_nat_entry_t *nat_entry)
{
    ip4_nat_remove(nat_entry);
}

/**
 * Timer callback function that calls ip4_nat_tmr() and reschedules itself.
 *
 * @param arg unused argument
 */
static void
nat_timer(void *arg)
{
  LWIP_UNUSED_ARG(arg);
  LWIP_DEBUGF(TIMERS_DEBUG, (0x0, "tcpip: nat_timer()\n"));

  ip4_nat_tmr();
  sys_timeout(LWIP_NAT_TMR_INTERVAL_SEC * 1000, nat_timer, NULL);
}

/** Initialize this module */
void
ip4_nat_init(void)
{
  int i;
  SYS_ARCH_DECL_PROTECT(lev);

  /* @todo: this can be omitted since we trust static variables
            to be initialized to zero */
  for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_ICMP; i++) {
    IPNAT_ENTRY_RESET(&ip4_nat_icmp_table[i].common);
  }
  for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_TCP; i++) {
    IPNAT_ENTRY_RESET(&ip4_nat_tcp_table[i].common);
  }
  for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_UDP; i++) {
    IPNAT_ENTRY_RESET(&ip4_nat_udp_table[i].common);
  }

  /* we must lock scheduler to protect following code */
  SYS_ARCH_PROTECT(lev);

  /* add a lwip timer for NAT */
  sys_timeout(LWIP_NAT_TMR_INTERVAL_SEC * 1000, nat_timer, NULL);

  /* un-protect */
  SYS_ARCH_UNPROTECT(lev);
}

/** Allocate a new ip4_nat_conf_t item */
static ip4_nat_conf_t*
ip4_nat_alloc(void)
{
  ip4_nat_conf_t *ret = (ip4_nat_conf_t*)mem_malloc(sizeof(ip4_nat_conf_t));
  return ret;
}

/** Free a removed ip4_nat_conf_t item */
static void
ip4_nat_free(ip4_nat_conf_t *item)
{
  LWIP_ASSERT("item != NULL", item != NULL);
  mem_free(item);
}

/** Add a new NAT entry
 *
 * @param new_entry pointer to a structure used to initialize the entry
 * @return ERR_OK if succeeded
 */
err_t
ip4_nat_add(const ip4_nat_entry_t *new_entry)
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    quec_nat_try_lock();
#endif
  err_t err = ERR_VAL;
  ip4_nat_conf_t *cur = ip4_nat_cfg;
  ip4_nat_conf_t *ip4_nat_cfg_new = ip4_nat_alloc();
  LWIP_ASSERT("new_entry != NULL", new_entry != NULL);

  if (ip4_nat_cfg_new != NULL) {
    SMEMCPY(&ip4_nat_cfg_new->entry, new_entry, sizeof(ip4_nat_entry_t));
    ip4_nat_cfg_new->entry.port_index = 0;
    ip4_nat_cfg_new->next = NULL;

    ip4_nat_dbg_dump_init(ip4_nat_cfg_new);

    if (ip4_nat_cfg == NULL) {
      ip4_nat_cfg = ip4_nat_cfg_new;
    } else {
      /* @todo: do we really need to enqueue the new entry at the end?? */
      uint16_t port_index = 0;
      if (new_entry->out_if == cur->entry.out_if)
      {
        port_index ++;
      }
      while (cur->next != NULL) {
        cur = cur->next;
        if (new_entry->out_if == cur->entry.out_if)
        {
          port_index ++;
        }
      }
      LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_add: find same out_if port_index: %d\n", port_index));

      ip4_nat_cfg_new->entry.port_index = port_index;
      cur->next = ip4_nat_cfg_new;
    }
    err = ERR_OK;
  } else {
    err = ERR_MEM;
  }
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    quec_nat_unlock();
#endif
  return err;
}

/** Remove a NAT entry previously added by 'ip4_nat_add()'.
 *
 * @param remove_entry describes the entry to remove
 */
void
ip4_nat_remove(const ip4_nat_entry_t *remove_entry)
{
  ip4_nat_conf_t *cur = ip4_nat_cfg;
  ip4_nat_conf_t *next;
  ip4_nat_conf_t *previous = NULL;

  while (cur != NULL) {
    /* Remove the NAT interfaces */
    if ((cur->entry.source_net.addr     == remove_entry->source_net.addr) &&
        (cur->entry.source_netmask.addr == remove_entry->source_netmask.addr) &&
        (cur->entry.dest_net.addr       == remove_entry->dest_net.addr) &&
        (cur->entry.dest_netmask.addr   == remove_entry->dest_netmask.addr) &&
        (cur->entry.out_if              == remove_entry->out_if) &&
        (cur->entry.in_if               == remove_entry->in_if))
    {
      ip4_nat_dbg_dump_remove(cur);

      ip4_nat_reset_state(cur);
      next = cur->next;
      if (cur == ip4_nat_cfg) {
        ip4_nat_cfg = next;
      } else {
        LWIP_ASSERT("NULL != previous", NULL != previous);
        previous->next = next;
      }
      /* free 'cur' or there will be a memory leak */
      ip4_nat_free(cur);
      return;
    } else {
      previous = cur;
      cur = cur->next;
    }
  }
}

/** Reset a NAT configured entry to be reused.
 * Effectively calls IPNAT_ENTRY_RESET() on 'cfg'.
 *
 * @param cfg NAT entry to reset
 */
static void
ip4_nat_reset_state(ip4_nat_conf_t *cfg)
{
  int i;

  /* @todo: optimize this!!!
     why do we search for it anyway, if we have the pointer??? */
  for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_ICMP; i++) {
    if(ip4_nat_icmp_table[i].common.cfg == cfg) {
      IPNAT_ENTRY_RESET(&ip4_nat_icmp_table[i].common);
    }
  }
  for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_TCP; i++) {
    if(ip4_nat_tcp_table[i].common.cfg == cfg) {
      IPNAT_ENTRY_RESET(&ip4_nat_tcp_table[i].common);
    }
  }
  for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_UDP; i++) {
    if(ip4_nat_udp_table[i].common.cfg == cfg) {
      IPNAT_ENTRY_RESET(&ip4_nat_udp_table[i].common);
    }
  }
}

/** Check if this packet should be routed or should be translated
 *
 * @param iphdr the IP header to check
 * @return - a NAT entry if the packet shall be translated,
 *         - NULL if the packet shall be routed normally
 */
static ip4_nat_conf_t *
ip4_nat_shallnat(const struct ip_hdr *iphdr)
{
  ip4_nat_conf_t *nat_config = ip4_nat_cfg;

  for (nat_config = ip4_nat_cfg; nat_config != NULL; nat_config = nat_config->next) {
      if (ip4_addr_netcmp(&(iphdr->dest), &(nat_config->entry.dest_net),
                       &(nat_config->entry.dest_netmask))) {
        if(netif_is_valid(nat_config->entry.in_if) && netif_is_valid(nat_config->entry.out_if) )
            break;
    }
  }
  if(nat_config != NULL) return nat_config;

  for (nat_config = ip4_nat_cfg; nat_config != NULL; nat_config = nat_config->next) {
      if (ip4_addr_netcmp(&(iphdr->src), &(nat_config->entry.source_net),
                     &(nat_config->entry.source_netmask))) {
        if(netif_is_valid(nat_config->entry.in_if) && netif_is_valid(nat_config->entry.out_if) )
            break;
    }
  }
  return nat_config;
}

#ifdef  CONFIG_QUEC_PROJECT_FEATURE_NETIF
/** Check if this packet should be routed or should be translated
 *
 * @param iphdr the IP header to check
 * @return - a NAT entry if the packet shall be translated,
 *         - NULL if the packet shall be routed normally
 */
static ip4_nat_conf_t *ip4_qdmz_shallnat(const struct ip_hdr *iphdr)
{
  ip4_nat_conf_t *nat_config = ip4_nat_cfg;

  for (nat_config = ip4_nat_cfg; nat_config != NULL; nat_config = nat_config->next) {
      if (ip4_addr_netcmp(&(iphdr->dest), &(nat_config->entry.dest_net), &(nat_config->entry.dest_netmask)) &&
          nat_config->entry.in_if->link_mode == NETIF_LINK_MODE_NAT_NETDEV_LAN) {
        break;
    }
  }

  return nat_config;
}

/**
 * This function checks for incoming packets if we already have a NAT entry.
 * If yes a pointer to the NAT entry is returned. Otherwise NULL.
 *
 * @param nat_config NAT configuration.
 * @param iphdr The IP header.
 * @param tcphdr The TCP header.
 * @return A pointer to an existing NAT entry or NULL if none is found.
 */
static ip4_nat_entries_tcp_t *
ip4_qdmz_tcp_lookup_incoming(const struct ip_hdr *iphdr, const struct tcp_hdr *tcphdr)
{
  int i;
  ip4_nat_entries_tcp_t *nat_entry = NULL;
  
  if(ql_qdmz_cfg.status){
	return NULL;
  }
  
  for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_TCP; i++) {
    if (ip4_nat_tcp_table[i].common.ttl) {
      if ((iphdr->dest.addr == ip4_nat_tcp_table[i].common.source.addr) &&
          (tcphdr->src == ip4_nat_tcp_table[i].dport) &&
          (tcphdr->dest == ip4_nat_tcp_table[i].nport)) {
        nat_entry = &ip4_nat_tcp_table[i];
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_tcp_lookup_incoming: i %d\n", i));
        ip4_nat_dbg_dump_tcp_nat_entry("ip4_nat_tcp_lookup_incoming: found existing nat entry: ",nat_entry);
        break;
      }
    }
  }
  return nat_entry;
}

/**
 * This function checks for incoming packets if we already have a NAT entry.
 * If yes a pointer to the NAT entry is returned. Otherwise NULL.
 *
 * @param nat_config NAT configuration.
 * @param iphdr The IP header.
 * @param udphdr The UDP header.
 * @return A pointer to an existing NAT entry or
 *         NULL if none is found.
 */
static ip4_nat_entries_udp_t *
ip4_qdmz_udp_lookup_incoming(const struct ip_hdr *iphdr, const struct udp_hdr *udphdr)
{
  int i;
  ip4_nat_entries_udp_t *nat_entry = NULL;
  if(ql_qdmz_cfg.status){
	return NULL;
  }
  for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_UDP; i++) {
    if (ip4_nat_udp_table[i].common.ttl) {
      if ((iphdr->dest.addr == ip4_nat_udp_table[i].common.source.addr) &&
          (udphdr->src == ip4_nat_udp_table[i].dport) &&
          (udphdr->dest == ip4_nat_udp_table[i].nport)) {
        nat_entry = &ip4_nat_udp_table[i];
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_udp_lookup_incoming: i %d\n", i));
        ip4_nat_dbg_dump_udp_nat_entry("ip4_nat_udp_lookup_incoming: found existing nat entry: ",
                                      nat_entry);
        break;
      }
    }
  }
  return nat_entry;
}
#endif
/** Check if the IP header can be hidden and if the remaining packet
 * is long enough. p->payload is reset to the IP header on return.
 *
 * @param p received packet, p->payload pointing to IP header
 * @param min_size minimum p->tot_len after hiding IP header
 * @return a pointer to the next header (after IP header),
 *         NULL if hiding IP header fails or the packet is too short
 */
static void*
ip4_nat_check_header(struct pbuf *p, u16_t min_size)
{
  void *ret = NULL;
  struct ip_hdr  *iphdr = (struct ip_hdr*)p->payload;
  s16_t iphdr_len = IPH_HL(iphdr) * 4;
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_check_header: iphdr_len %d\n", iphdr_len));
  if(p->len > iphdr_len)
  {
    if(!pbuf_header(p, -iphdr_len)) {
      if(p->tot_len >= min_size) {
        ret = p->payload;
      }
      /* Restore pbuf payload pointer from previous header check. */
      pbuf_header(p, iphdr_len);
    }
  }else
  {
    uint16_t offset = 0;
    struct pbuf *q1 = NULL;
    for (q1 = p; q1!= NULL; q1 = q1->next)
    {
        if(offset + q1->len > iphdr_len)
        {
            if(q1->len > min_size)
            {
                ret = q1->payload + offset - iphdr_len;
            }else
            {
                break;
            }
        }
        offset += q1->len;

    }
  }
  return ret;
}

/** Input processing: check if a received packet belongs to a NAT entry
 * and if so, translated it and send it on.
 *
 * @param p received packet
 * @return 1 if the packet has been consumed (it was a NAT packet),
 *         0 if the packet has not been consumed (no NAT packet)
 */
u8_t
ip4_nat_input(struct pbuf *p)
{
  struct ip_hdr        *iphdr = (struct ip_hdr*)p->payload;
  struct tcp_hdr       *tcphdr;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
  ip4_nat_conf_t       *nat_config = NULL;
  struct udp_hdr       *udphdr = NULL;
#else
  struct udp_hdr       *udphdr;
#endif
  struct icmp_echo_hdr *icmphdr;
  nat_entry_t           nat_entry;
  err_t                 err;
  u8_t                  consumed = 0;
  int                   i;
  struct pbuf          *q = NULL;

  nat_entry.cmn = NULL;
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_input: p %p\n", p));
  ip4_nat_dbg_dump("ip4_nat_in: checking nat for", iphdr);

  switch (IPH_PROTO(iphdr)) {
    case IP_PROTO_TCP:
      tcphdr = (struct tcp_hdr*)ip4_nat_check_header(p, sizeof(struct tcp_hdr));
      if (tcphdr == NULL) {
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_input: short tcp packet (%" U16_F " bytes) discarded\n", p->tot_len));
      } else {
        nat_entry.tcp = ip4_nat_tcp_lookup_incoming(iphdr, tcphdr);
        if (nat_entry.tcp != NULL) {
          /* Refresh TCP entry */
#ifdef  CONFIG_QUEC_PROJECT_FEATURE_NETIF
          nat_entry.tcp->common.ttl = LWIP_NAT_DEFAULT_TCP_TTL_SECONDS;
#else
          nat_entry.tcp->common.ttl = sNATTTL;
#endif
          tcphdr->dest = nat_entry.tcp->sport;
          /* Adjust TCP checksum for changed destination port */
          ip4_nat_chksum_adjust((u8_t *)&(tcphdr->chksum),
            (u8_t *)&(nat_entry.tcp->nport), 2, (u8_t *)&(tcphdr->dest), 2);

          /* Adjust TCP checksum for changing dest IP address */
          ip4_addr_t *outif_ip4 = ip_2_ip4(&(nat_entry.cmn->cfg->entry.out_if->ip_addr));
          ip4_nat_chksum_adjust((u8_t *)&(tcphdr->chksum),
            (u8_t *)&(outif_ip4->addr), 4,
            (u8_t *)&(nat_entry.cmn->source.addr), 4);

          consumed = 1;
        }
#ifdef  CONFIG_QUEC_PROJECT_FEATURE_NETIF
        else if(ql_qdmz_cfg.status)
        {
          /* Check if this packet should be routed or should be translated */
          nat_config = ip4_qdmz_shallnat(iphdr);
          if (nat_config != NULL && nat_config->entry.out_if != NULL) {
            nat_entry.tcp = ip4_nat_tcp_lookup_outgoing(nat_config, iphdr, tcphdr, 1);
            if (nat_entry.tcp != NULL) {
              LWIP_DEBUGF(NAT_DEBUG, (0x0, "qdmz: change TCP src port %d to %d, the dest port:%d", tcphdr->src, nat_entry.tcp->nport, tcphdr->dest));
              
              /* Reset ttl*/
              nat_entry.tcp->common.ttl = LWIP_NAT_DEFAULT_TCP_TTL_SECONDS;
              /* Adjust TCP checksum for changed source port */
              ip4_nat_chksum_adjust((u8_t *)&(tcphdr->chksum), (u8_t *)&tcphdr->src, 2, (u8_t *)&(nat_entry.tcp->nport), 2);
              tcphdr->src = nat_entry.tcp->nport;
              /* Adjust TCP checksum for changing dest IP address */
              ip4_nat_chksum_adjust((u8_t *)&(tcphdr->chksum), (u8_t *)&iphdr->dest.addr, 4, (u8_t *)&(ql_qdmz_cfg.ip_addr.addr), 4);
              consumed = 2;
            }
          }
        }
#endif
      }
      break;

    case IP_PROTO_UDP:
      udphdr = (struct udp_hdr *)ip4_nat_check_header(p, sizeof(struct udp_hdr));
      if (udphdr == NULL) {
        LWIP_DEBUGF(NAT_DEBUG,
          (0x0, "ip4_nat_input: short udp packet (%hu bytes) discarded\n",
          p->tot_len));
      } else {
        nat_entry.udp = ip4_nat_udp_lookup_incoming(iphdr, udphdr);
        if (nat_entry.udp != NULL) {
          /* Refresh UDP entry */
          nat_entry.udp->common.ttl = sNATTTL;
          udphdr->dest = nat_entry.udp->sport;
          if (udphdr->chksum != 0)
          {
              /* Adjust UDP checksum for changed destination port */
              ip4_nat_chksum_adjust((u8_t *)&(udphdr->chksum),
                (u8_t *)&(nat_entry.udp->nport), 2, (u8_t *)&(udphdr->dest), 2);
              /* Adjust UDP checksum for changing dest IP address */
              ip4_addr_t *outif_ip4 = ip_2_ip4(&(nat_entry.cmn->cfg->entry.out_if->ip_addr));
              ip4_nat_chksum_adjust((u8_t *)&(udphdr->chksum),
                (u8_t *)&(outif_ip4->addr), 4,
                (u8_t *)&(nat_entry.cmn->source.addr), 4);
          }
          consumed = 1;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
          //0x3500 => 0x0035 => 53. dns的端口号为53
          if (udphdr->src == 0x3500)
          {
            bool is_change = false;

            if (ql_qdmz_cfg.status) 
            {
                is_change = true;
            } 
            else if (nat_entry.udp->common.cfg->entry.in_if->link_mode == NETIF_LINK_MODE_NAT_NETDEV_LAN || \
                nat_entry.udp->common.cfg->entry.in_if->link_mode == NETIF_LINK_MODE_NAT_PPP_LAN)
            {
                uint8_t nSim = (nat_entry.udp->common.cfg->entry.in_if->sim_cid & 0xf0) >> 4;
                uint8_t type = ((nat_entry.udp->common.cfg->entry.in_if->link_mode == NETIF_LINK_MODE_NAT_NETDEV_LAN) ? QL_NAT_ASSIGN_TYPE_USBNET : QL_NAT_ASSIGN_TYPE_PPP);
              
                if (quec_is_assign_dns(nSim, type))  /* Multiplex qdmz dns */
                {
                    is_change = true;
                }
            }

            if (is_change)
            {
                if (udphdr->chksum != 0)
              	{
    	            /* Adjust UDP checksum for changed source addr */
    	            ip4_nat_chksum_adjust((u8_t *)&(udphdr->chksum), (u8_t *)&(iphdr->src.addr), 4, (u8_t *)&ql_qdmz_cfg.virt_dns_server.addr, 4);
              	}
                /* Adjust IP checksum for changed source addr */
                ip4_nat_chksum_adjust((u8_t *) & IPH_CHKSUM(iphdr),  (u8_t *)&(iphdr->src.addr), 4, (u8_t *)&ql_qdmz_cfg.virt_dns_server.addr, 4);
                iphdr->src.addr = ql_qdmz_cfg.virt_dns_server.addr;
            }
          }
#endif
        }
#ifdef  CONFIG_QUEC_PROJECT_FEATURE_NETIF
        else if(ql_qdmz_cfg.status)
        {
          /* Check if this packet should be routed or should be translated */
          nat_config = ip4_qdmz_shallnat(iphdr);
          if (nat_config != NULL && nat_config->entry.out_if != NULL) {
            nat_entry.udp = ip4_nat_udp_lookup_outgoing(nat_config, iphdr, udphdr, 1);
            if (nat_entry.udp != NULL) {
              LWIP_DEBUGF(NAT_DEBUG, (0x0, "qdmz: change UDP src port %d to %d, the dest port:%d", udphdr->src, nat_entry.udp->nport, udphdr->dest));
              /* Reset ttl*/
              nat_entry.udp->common.ttl = LWIP_NAT_DEFAULT_TTL_SECONDS;
			  if (udphdr->chksum != 0)
			  {
	              /* Adjust UDP checksum for changed source port */
	              ip4_nat_chksum_adjust((u8_t *)&(udphdr->chksum), (u8_t *)&udphdr->src, 2, (u8_t *)&(nat_entry.udp->nport), 2);
	              /* Adjust IP checksum for changing dest IP address */
	              ip4_nat_chksum_adjust((u8_t *)&(udphdr->chksum), (u8_t *)&iphdr->dest.addr, 4, (u8_t *)&(ql_qdmz_cfg.ip_addr.addr), 4);
			  }
			  udphdr->src = nat_entry.udp->nport;
              consumed = 2;
            }
          }
        }
#endif
      }
      break;

    case IP_PROTO_ICMP:
      icmphdr = (struct icmp_echo_hdr *)ip4_nat_check_header(p, sizeof(struct icmp_echo_hdr));
      if (icmphdr == NULL) {
        LWIP_DEBUGF(NAT_DEBUG,
          (0x0, "ip4_nat_out: short icmp echo reply packet (%" U16_F " bytes) discarded\n",
          p->tot_len));
      } else {
        if (ICMP_ER == ICMPH_TYPE(icmphdr)) {
          for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_ICMP; i++) {
            nat_entry.icmp = &ip4_nat_icmp_table[i];
            if ((nat_entry.icmp->common.ttl) &&
                (iphdr->src.addr == nat_entry.icmp->common.dest.addr) &&
                (nat_entry.icmp->id == icmphdr->id) &&
                (nat_entry.icmp->seqno == icmphdr->seqno)) {
              ip4_nat_dbg_dump_icmp_nat_entry("found existing nat entry: ", nat_entry.icmp);
              consumed = 1;
              IPNAT_ENTRY_RESET(nat_entry.cmn);
              break;
            }
          }
        }
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
        else if (ICMP_ECHO == ICMPH_TYPE(icmphdr) && ql_qdmz_cfg.status) {
            /* Check if this packet should be routed or should be translated */
            nat_config = ip4_qdmz_shallnat(iphdr);
            if (nat_config != NULL && nat_config->entry.out_if != NULL)
            {
                for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_ICMP; i++) {
                    if (!ip4_nat_icmp_table[i].common.ttl) {
                        nat_entry.icmp = &ip4_nat_icmp_table[i];
                        /* Reset ttl*/
                        nat_entry.icmp->common.ttl = LWIP_NAT_DEFAULT_TTL_SECONDS;
                        ip4_nat_cmn_init(nat_config, iphdr, nat_entry.cmn);
                        nat_entry.icmp->id = icmphdr->id;
                        nat_entry.icmp->seqno = icmphdr->seqno;
                        ip4_nat_dbg_dump_icmp_nat_entry(" ip4_nat_input: created new NAT entry ", nat_entry.icmp);
                        consumed = 2;
                        break;
                    }
                }
                if (NULL == nat_entry.icmp)
                {
                    LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_input: no more NAT entries for ICMP available\n"));
                }
            }
        }
#endif
      }
      break;

    default:
      break;
  }

  if(consumed) {
    /* packet consumed, send it out on in_if */
    struct netif *in_if;
#if 0
    /* check if the pbuf has room for link headers */
    if (pbuf_header(p, PBUF_LINK_HLEN)) {
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_input: pbuf has no room for link headersr\n"));
      /* pbuf has no room for link headers, allocate an extra pbuf */
      q = pbuf_alloc(PBUF_LINK, 0, PBUF_RAM);
      if (q == NULL) {
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_input: no pbuf for outgoing header\n"));
        /* @todo: stats? */
        pbuf_free(p);
        p = NULL;
        return 1;
      } else {
        pbuf_cat(q, p);
      }
    } else {
      /* restore p->payload to IP header */
      if (pbuf_header(p, -PBUF_LINK_HLEN)) {
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_input: restoring header failed\n"));
        /* @todo: stats? */
        pbuf_free(p);
        p = NULL;
        return 1;
      }
      else q = p;
    }
#endif
    q = p;
    /* if we come here, q is the pbuf to send (either points to p or to a chain) */
    in_if = nat_entry.cmn->cfg->entry.in_if;
    if(netif_is_valid(in_if) == false) return 0;
#ifdef  CONFIG_QUEC_PROJECT_FEATURE_NETIF
    if(consumed == 2)
    {
      LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_input: change dest ip %d to %d, the src ip:%d", iphdr->dest.addr, ql_qdmz_cfg.ip_addr.addr, iphdr->src.addr));
      /* Adjust IP checksum for changing dest IP address */
      ip4_nat_chksum_adjust((u8_t *)&IPH_CHKSUM(iphdr), (u8_t *)&iphdr->dest.addr, 4, (u8_t *)&(ql_qdmz_cfg.ip_addr.addr), 4);
      iphdr->dest.addr = ql_qdmz_cfg.ip_addr.addr;
      consumed = 1;
    }
    else
    {
#endif
    iphdr->dest.addr = nat_entry.cmn->source.addr;
    ip4_addr_t *outif_ip4 = ip_2_ip4(&(nat_entry.cmn->cfg->entry.out_if->ip_addr));
    ip4_nat_chksum_adjust((u8_t *) & IPH_CHKSUM(iphdr),
      (u8_t *)&(outif_ip4->addr), 4,
      (u8_t *) & (iphdr->dest.addr), 4);

#ifdef  CONFIG_QUEC_PROJECT_FEATURE_NETIF
    }
#endif
    ip4_nat_dbg_dump("ip4_nat_input: packet back to source after nat: ", iphdr);
    LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_input: sending packet on interface ("));
    ip4_nat_dbg_dump_ip(ip_2_ip4(&(in_if->ip_addr)));
    LWIP_DEBUGF(NAT_DEBUG, (0x0, ")\n"));

    //err = in_if->output(in_if, q, (ip4_addr_t *)&(iphdr->dest));
    LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_input: q %p\n", q));
#if 0
    uint8_t *pData = malloc(q->tot_len);
    uint16_t offset = 0;
    struct pbuf *q1 = NULL;
    for (q1 = q; q1!= NULL; q1 = q1->next)
    {
        memcpy(&pData[offset], q1->payload, q1->len);
        offset += q1->len;
    }

    sys_arch_dump(pData, q->tot_len);
    free(pData);
#endif
    //pbuf_ref(q);
    err = in_if->input(q, in_if);
    if(err != ERR_OK) {
      LWIP_DEBUGF(NAT_DEBUG,
        (0x0, "ip4_nat_input: failed to send rewritten packet. link layer returned %d\n",
        err));
    }
    /* now that q (and/or p) is sent (or not), give up the reference to it
       this frees the input pbuf (p) as we have consumed it. */
    //pbuf_free(q);
  }
  return consumed;
}

/** Check if one NAT entry timed out */
static void
ip4_nat_check_timeout(ip4_nat_entry_common_t *nat_entry)
{
  if(nat_entry->ttl > 0) {
    if(nat_entry->ttl != LWIP_NAT_TTL_INFINITE) {
      /* this is not a 'no-timeout' entry */
      if(nat_entry->ttl > LWIP_NAT_TMR_INTERVAL_SEC) {
        nat_entry->ttl -= LWIP_NAT_TMR_INTERVAL_SEC;
      } else {
        nat_entry->ttl = 0;
        nat_entry->dest.addr = 0;
        nat_entry->source.addr = 0;
      }
    }
  }
}

#ifdef  CONFIG_QUEC_PROJECT_FEATURE_NETIF
/** Check if one TCP NAT entry timed out */
static void
ip4_tcp_nat_check_timeout(ip4_nat_entry_common_t *nat_entry)
{
  if(nat_entry->ttl > 0) {
    if((nat_entry->ttl != LWIP_NAT_TTL_INFINITE) && (nat_entry->ttl != LWIP_NAT_TCP_TTL_OUT_OF_DATE)) {
      /* this is not a 'no-timeout' entry */
      if(nat_entry->ttl > LWIP_NAT_TMR_INTERVAL_SEC) {
        nat_entry->ttl -= LWIP_NAT_TMR_INTERVAL_SEC;
      } else {
        nat_entry->ttl = LWIP_NAT_TCP_TTL_OUT_OF_DATE;
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_tcp_nat_check_timeout: set old entries out of date\n"));
      }
    }
  }
}
#endif

/** The NAT timer function, to be called at an interval of
 * LWIP_NAT_TMR_INTERVAL_SEC seconds.
 */
void
ip4_nat_tmr(void)
{
  int i;

  LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_tmr: removing old entries\n"));

  for(i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_ICMP; i++) {
    ip4_nat_check_timeout((ip4_nat_entry_common_t *) & ip4_nat_icmp_table[i]);
  }
  for(i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_TCP; i++) {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
    ip4_tcp_nat_check_timeout((ip4_nat_entry_common_t *) & ip4_nat_tcp_table[i]);
#else
    ip4_nat_check_timeout((ip4_nat_entry_common_t *) & ip4_nat_tcp_table[i]);
#endif 
  }
  for(i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_UDP; i++) {
    ip4_nat_check_timeout((ip4_nat_entry_common_t *) & ip4_nat_udp_table[i]);
  }
}

/** Check if we want to perform NAT with this packet. If so, send it out on
 * the correct interface.
 *
 * @param p the packet to test/send
 * @return 1: the packet has been sent using NAT,
 *         0: the packet did not belong to a NAT entry
 */
u8_t
ip4_nat_out(struct pbuf *p)
{
  u8_t                  sent = 0;
  err_t                 err;
  struct ip_hdr        *iphdr = p->payload;
  struct icmp_echo_hdr *icmphdr;
  struct tcp_hdr       *tcphdr;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
  u8_t                  consumed = 0;
  struct udp_hdr       *udphdr = NULL;
#else
  struct udp_hdr       *udphdr;
#endif
  ip4_nat_conf_t        *nat_config;
  nat_entry_t           nat_entry;
  int             i;

  nat_entry.cmn = NULL;

  ip4_nat_dbg_dump("ip4_nat_out: checking nat for", iphdr);
#if 0
    uint8_t *pData = malloc(p->tot_len);
    uint16_t offset = 0;
    struct pbuf *q1 = NULL;
    for (q1 = p; q1!= NULL; q1 = q1->next)
    {
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_out: q1 %p q1->payload %p len %d\n", q1, q1->payload, q1->len));
        memcpy(&pData[offset], q1->payload, q1->len);
        offset += q1->len;
    }

    sys_arch_dump(pData, p->tot_len);
    free(pData);
#endif
  /* Check if this packet should be routed or should be translated */
  nat_config = ip4_nat_shallnat(iphdr);
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_out: ip4_nat_shallnat %p\n", nat_config));
  if (nat_config != NULL ) {
    if (nat_config->entry.out_if == NULL) {
      LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_out: no external interface for nat table entry\n"));
    } else {
      switch (IPH_PROTO(iphdr))
      {
      case IP_PROTO_TCP:
        tcphdr = (struct tcp_hdr *)ip4_nat_check_header(p, sizeof(struct tcp_hdr));
        if (tcphdr == NULL) {
          LWIP_DEBUGF(NAT_DEBUG,
            (0x0, "ip4_nat_out: short tcp packet (0x0, %hu bytes) discarded\n", p->tot_len));
        } else {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
          nat_entry.tcp = ip4_qdmz_tcp_lookup_incoming(iphdr, tcphdr);
          if (nat_entry.tcp != NULL) {
            LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_out: change TCP dest port %d to %d, src port=%d", tcphdr->dest, nat_entry.tcp->sport, tcphdr->src));
            /* Reset ttl*/
            nat_entry.tcp->common.ttl = LWIP_NAT_DEFAULT_TCP_TTL_SECONDS;
            /* Adjust TCP checksum for changing dest port */
            ip4_nat_chksum_adjust((u8_t *)&(tcphdr->chksum), (u8_t *)&(tcphdr->dest), 2, (u8_t *)&(nat_entry.tcp->sport), 2);
            tcphdr->dest = nat_entry.tcp->sport;
            /* Adjust TCP checksum for changing source IP address */
            ip4_nat_chksum_adjust((u8_t *)&(tcphdr->chksum), (u8_t *)&(iphdr->src.addr), 4, (u8_t *)&(nat_entry.cmn->dest.addr), 4);
            consumed = 2;
          }
          else
          {
#endif
          nat_entry.tcp = ip4_nat_tcp_lookup_outgoing(nat_config, iphdr, tcphdr, 1);
          if (nat_entry.tcp != NULL) {
            /* Reset ttl*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
            nat_entry.tcp->common.ttl = LWIP_NAT_DEFAULT_TCP_TTL_SECONDS;
#else
            nat_entry.tcp->common.ttl = sNATTTL;
#endif
            /* Adjust TCP checksum for changing source port */
            tcphdr->src = nat_entry.tcp->nport;
            ip4_nat_chksum_adjust((u8_t *)&(tcphdr->chksum),
              (u8_t *)&(nat_entry.tcp->sport), 2, (u8_t *)&(tcphdr->src), 2);
            /* Adjust TCP checksum for changing source IP address */
            ip4_addr_t *outif_ip4 = ip_2_ip4(&(nat_entry.cmn->cfg->entry.out_if->ip_addr));
            ip4_nat_chksum_adjust((u8_t *)&(tcphdr->chksum),
              (u8_t *)&(nat_entry.cmn->source.addr), 4,
              (u8_t *)&(outif_ip4->addr), 4);
          }
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
        }
#endif
        }
        break;

      case IP_PROTO_UDP:
        udphdr = (struct udp_hdr *)ip4_nat_check_header(p, sizeof(struct udp_hdr));
        if (udphdr == NULL) {
          LWIP_DEBUGF(NAT_DEBUG,
            (0x0, "ip4_nat_out: short udp packet (%hu bytes) discarded\n", p->tot_len));
        } else {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
        //0x3500 => 0x0035 => 53 => dns的端口号
        if(udphdr->dest == 0x3500 && IPH_PROTO(iphdr) == IP_PROTO_UDP)
        {
            bool is_change = false;
            
            if (ql_qdmz_cfg.status)
            {
                is_change = true;
            }
            else if (nat_config->entry.in_if->link_mode == NETIF_LINK_MODE_NAT_NETDEV_LAN || \
                nat_config->entry.in_if->link_mode == NETIF_LINK_MODE_NAT_PPP_LAN) 
            {
                uint8_t nSim = (nat_config->entry.in_if->sim_cid & 0xf0) >> 4;
                uint8_t type = ((nat_config->entry.in_if->link_mode == NETIF_LINK_MODE_NAT_NETDEV_LAN) ? QL_NAT_ASSIGN_TYPE_USBNET : QL_NAT_ASSIGN_TYPE_PPP);
                
                if (quec_is_assign_dns(nSim, type))  /* Multiplex qdmz dns */
                {
                    is_change = true;
                }
            }
            
            /* Exchange the IP dest address */
            if (is_change)
            {
                ql_qdmz_cfg.virt_dns_server.addr = iphdr->dest.addr;
                /* Adjust UDP checksum for changed dest addr */
                ip4_nat_chksum_adjust((u8_t *)&(udphdr->chksum), (u8_t *)&(iphdr->dest.addr), 4, (u8_t *)&ql_qdmz_cfg.dns_server.addr, 4);
                /* Adjust IP checksum for changed dest addr */
                ip4_nat_chksum_adjust((u8_t *)&(iphdr->_chksum), (u8_t *)&(iphdr->dest.addr), 4, (u8_t *)&ql_qdmz_cfg.dns_server.addr, 4);
                iphdr->dest.addr = ql_qdmz_cfg.dns_server.addr;
            }
        }

        nat_entry.udp = ip4_qdmz_udp_lookup_incoming(iphdr, udphdr);
        if (nat_entry.udp != NULL) {
          LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_out: change UDP dest port %d to %d, src port=%d", udphdr->dest, nat_entry.udp->sport, udphdr->src));
          
          /* Reset ttl*/
          nat_entry.udp->common.ttl = LWIP_NAT_DEFAULT_TTL_SECONDS;
		  if (udphdr->chksum != 0) {
	          /* Adjust TCP checksum for changing dest port */
	          ip4_nat_chksum_adjust((u8_t *)&(udphdr->chksum), (u8_t *)&(udphdr->dest), 2, (u8_t *)&(nat_entry.udp->sport), 2);
	          /* Adjust TCP checksum for changing source IP address */
	          ip4_nat_chksum_adjust((u8_t *)&(udphdr->chksum), (u8_t *)&(iphdr->src.addr), 4, (u8_t *)&(nat_entry.cmn->dest.addr), 4);
		  	}
		  udphdr->dest = nat_entry.udp->sport;
          consumed = 2;
        }
        else
        {
#endif
          nat_entry.udp = ip4_nat_udp_lookup_outgoing(nat_config, iphdr, udphdr, 1);
          if (nat_entry.udp != NULL) {
             /* Reset ttl*/
            nat_entry.udp->common.ttl = sNATTTL;
            udphdr->src = nat_entry.udp->nport;
            if (udphdr->chksum != 0)
            {
                /* Adjust UDP checksum for changing source port */
                ip4_nat_chksum_adjust((u8_t *)&(udphdr->chksum),
                  (u8_t *)&(nat_entry.udp->sport), 2, (u8_t *) & (udphdr->src), 2);
                /* Adjust UDP checksum for changing source IP address */
                ip4_addr_t *outif_ip4 = ip_2_ip4(&(nat_entry.cmn->cfg->entry.out_if->ip_addr));
                ip4_nat_chksum_adjust((u8_t *)&(udphdr->chksum),
                  (u8_t *)&(nat_entry.cmn->source.addr), 4,
                  (u8_t *)&(outif_ip4->addr), 4);
            }
          }
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
        }
#endif
        }
        break;

      case IP_PROTO_ICMP:
        {
        icmphdr = (struct icmp_echo_hdr *)ip4_nat_check_header(p, sizeof(struct icmp_echo_hdr));
        if(icmphdr == NULL) {
          LWIP_DEBUGF(NAT_DEBUG,
            (0x0, "ip4_nat_out: short icmp echo packet (%hu bytes) discarded\n", p->tot_len));
        } else {
          if (ICMPH_TYPE(icmphdr) == ICMP_ECHO) {
            for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_ICMP; i++) {
              if (!ip4_nat_icmp_table[i].common.ttl) {
                nat_entry.icmp = &ip4_nat_icmp_table[i];
                /* Reset ttl*/
                nat_entry.icmp->common.ttl = LWIP_NAT_DEFAULT_ICMP_TTL_SECONDS;
                ip4_nat_cmn_init(nat_config, iphdr, nat_entry.cmn);
                nat_entry.icmp->id = icmphdr->id;
                nat_entry.icmp->seqno = icmphdr->seqno;
                ip4_nat_dbg_dump_icmp_nat_entry(" ip4_nat_out: created new NAT entry ", nat_entry.icmp);
                break;
              }
            }
            if (NULL == nat_entry.icmp)
            {
              LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_out: no more NAT entries for ICMP available\n"));
            }
          }
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
          else if(ICMPH_TYPE(icmphdr) == ICMP_ER && ql_qdmz_cfg.status)
          {
            for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_ICMP; i++) {
              nat_entry.icmp = &ip4_nat_icmp_table[i];
              if ((nat_entry.icmp->common.ttl) && (iphdr->dest.addr == nat_entry.icmp->common.source.addr) &&
                  (nat_entry.icmp->id == icmphdr->id) && (nat_entry.icmp->seqno == icmphdr->seqno)) {
                ip4_nat_dbg_dump_icmp_nat_entry("found existing nat entry: ", nat_entry.icmp);
                IPNAT_ENTRY_RESET(nat_entry.cmn);
                consumed = 2;
                break;
              }
            }
          }
#endif
        }
        }
        break;
      default:
        break;
      }

      if (nat_entry.cmn != NULL) {
        struct netif *out_if = nat_entry.cmn->cfg->entry.out_if;
        if(netif_is_valid(out_if) == false) return 0;
        /* Exchange the IP source address with the address of the interface
        * where the packet will be sent.
        */
        
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
        if(consumed == 2)
        {
          /* Adjust IP checksum for changing source IP address */
          ip4_nat_chksum_adjust((u8_t *)&IPH_CHKSUM(iphdr), (u8_t *)&(iphdr->src.addr), 4, (u8_t *)&(nat_entry.cmn->dest.addr), 4);
          iphdr->src.addr = nat_entry.cmn->dest.addr;
        }
        else
        {
#endif
        /* @todo: check nat_config->entry.out_if agains nat_entry.cmn->cfg->entry.out_if */
        ip4_addr_t *outif_ip4 = ip_2_ip4(&(nat_entry.cmn->cfg->entry.out_if->ip_addr));
        iphdr->src.addr = outif_ip4->addr;
        ip4_nat_chksum_adjust((u8_t *) & IPH_CHKSUM(iphdr),
          (u8_t *) & (nat_entry.cmn->source.addr), 4, (u8_t *) & iphdr->src.addr, 4);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
        }
#endif
        ip4_nat_dbg_dump("ip4_nat_out: rewritten packet", iphdr);
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_out: sending packet on interface %c%c%hu(",out_if->name[0], out_if->name[1], (u16_t)out_if->num));
        ip4_nat_dbg_dump_ip(outif_ip4);
        LWIP_DEBUGF(NAT_DEBUG, (0x0, ")\n"));
        ip4_addr_t dest_ip4;
        ip4_addr_copy(dest_ip4, iphdr->dest);
        err = out_if->output(out_if, p, &dest_ip4);
        if (err != ERR_OK) {
          LWIP_DEBUGF(NAT_DEBUG,
            (0x0, "ip4_nat_out: failed to send rewritten packet. link layer returned %d\n", err));
        } else {
          sent = 1;
        }
      }
    }
  }

  return sent;
}

/** Initialize common parts of a NAT entry
 *
 * @param nat_config NAT config entry
 * @param iphdr IP header from which to initialize the entry
 * @param nat_entry entry to initialize
 */
static void
ip4_nat_cmn_init(ip4_nat_conf_t *nat_config, const struct ip_hdr *iphdr, ip4_nat_entry_common_t *nat_entry)
{
  LWIP_ASSERT("NULL != nat_entry", NULL != nat_entry);
  LWIP_ASSERT("NULL != nat_config", NULL != nat_config);
  LWIP_ASSERT("NULL != iphdr", NULL != iphdr);
  nat_entry->cfg = nat_config;
  ip4_addr_set(&nat_entry->dest, &iphdr->dest);
  ip4_addr_set(&nat_entry->source, &iphdr->src);
  nat_entry->ttl = sNATTTL;
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
/** Initialize common parts of a TCP NAT entry
 *
 * @param nat_config NAT config entry
 * @param iphdr IP header from which to initialize the entry
 * @param nat_entry entry to initialize
 */
static void
ip4_tcp_nat_cmn_init(ip4_nat_conf_t *nat_config, const struct ip_hdr *iphdr, ip4_nat_entry_common_t *nat_entry)
{
  LWIP_ASSERT("NULL != nat_entry", NULL != nat_entry);
  LWIP_ASSERT("NULL != nat_config", NULL != nat_config);
  LWIP_ASSERT("NULL != iphdr", NULL != iphdr);
  nat_entry->cfg = nat_config;
  ip4_addr_set(&nat_entry->dest, &iphdr->dest);
  ip4_addr_set(&nat_entry->source, &iphdr->src);
  nat_entry->ttl = LWIP_NAT_DEFAULT_TCP_TTL_SECONDS;
}
#endif

/**
 * This function checks for incoming packets if we already have a NAT entry.
 * If yes a pointer to the NAT entry is returned. Otherwise NULL.
 *
 * @param nat_config NAT configuration.
 * @param iphdr The IP header.
 * @param udphdr The UDP header.
 * @return A pointer to an existing NAT entry or
 *         NULL if none is found.
 */
static ip4_nat_entries_udp_t *
ip4_nat_udp_lookup_incoming(const struct ip_hdr *iphdr, const struct udp_hdr *udphdr)
{
  int i;
  ip4_nat_entries_udp_t *nat_entry = NULL;

  for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_UDP; i++) {
    if (ip4_nat_udp_table[i].common.ttl) {
      if ((iphdr->src.addr == ip4_nat_udp_table[i].common.dest.addr) &&
          (udphdr->src == ip4_nat_udp_table[i].dport) &&
          (udphdr->dest == ip4_nat_udp_table[i].nport)) {
        nat_entry = &ip4_nat_udp_table[i];
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_udp_lookup_incoming: i %d\n", i));
        ip4_nat_dbg_dump_udp_nat_entry("ip4_nat_udp_lookup_incoming: found existing nat entry: ",
                                      nat_entry);
        break;
      }
    }
  }
  return nat_entry;
}

/**
 * This function checks if we already have a NAT entry for this UDP connection.
 * If yes the a pointer to this NAT entry is returned.
 *
 * @param iphdr The IP header.
 * @param udphdr The UDP header.
 * @param allocate If no existing NAT entry is found and this flag is true
 *        a NAT entry is allocated.
 */
static ip4_nat_entries_udp_t *
ip4_nat_udp_lookup_outgoing(ip4_nat_conf_t *nat_config, const struct ip_hdr *iphdr,
                           const struct udp_hdr *udphdr, u8_t allocate)
{
  int i;
  nat_entry_t nat_entry;
  int last_free = -1;

  nat_entry.cmn = NULL;
  for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_UDP; i++) {
    if (ip4_nat_udp_table[i].common.ttl) {
      if ((iphdr->src.addr == ip4_nat_udp_table[i].common.source.addr) &&
          (iphdr->dest.addr == ip4_nat_udp_table[i].common.dest.addr) &&
          (udphdr->src == ip4_nat_udp_table[i].sport) &&
          (udphdr->dest == ip4_nat_udp_table[i].dport)) {
        nat_entry.udp = &ip4_nat_udp_table[i];
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_udp_lookup_outgoing: i %d\n", i));
        ip4_nat_dbg_dump_udp_nat_entry("ip4_nat_udp_lookup_outgoing: found existing nat entry: ",
                                      nat_entry.udp);
        break;
      }
    } else {
        if(last_free == -1)
            last_free = i;
    }
  }
  if (nat_entry.cmn == NULL) {
    if (allocate) {
      if (last_free != -1) {
        nat_entry.udp = &ip4_nat_udp_table[last_free];
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
        if (nat_config->entry.in_if->link_mode == NETIF_LINK_MODE_NAT_NETDEV_LAN || \
            nat_config->entry.in_if->link_mode == NETIF_LINK_MODE_NAT_PPP_LAN) {

            uint8_t nSim = (nat_config->entry.in_if->sim_cid & 0xf0) >> 4;
            uint8_t cid = (nat_config->entry.in_if->sim_cid & 0x0f);
            uint8_t type = ((nat_config->entry.in_if->link_mode == NETIF_LINK_MODE_NAT_NETDEV_LAN) ? QL_NAT_ASSIGN_TYPE_USBNET : QL_NAT_ASSIGN_TYPE_PPP);

            if (quec_is_assign_port(nSim, cid, type, true)) {
                nat_entry.udp->nport = udphdr->src;
            } else {
                nat_entry.udp->nport = htons((u16_t) (LWIP_NAT_DEFAULT_UDP_SOURCE_PORT + nat_config->entry.port_index * LWIP_NAT_DEFAULT_STATE_TABLES_UDP + last_free));
            }
        } else {
            nat_entry.udp->nport = htons((u16_t) (LWIP_NAT_DEFAULT_UDP_SOURCE_PORT + nat_config->entry.port_index * LWIP_NAT_DEFAULT_STATE_TABLES_UDP + last_free));
        }
#else 
        nat_entry.udp->nport = htons((u16_t) (LWIP_NAT_DEFAULT_UDP_SOURCE_PORT + nat_config->entry.port_index * LWIP_NAT_DEFAULT_STATE_TABLES_UDP + last_free));
#endif
        nat_entry.udp->sport = udphdr->src;
        nat_entry.udp->dport = udphdr->dest;
        ip4_nat_cmn_init(nat_config, iphdr, nat_entry.cmn);

        ip4_nat_dbg_dump_udp_nat_entry("ip4_nat_udp_lookup_outgoing: created new nat entry: ",
                                      nat_entry.udp);
      } else {
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_udp_lookup_outgoing: no more NAT entries available\n"));
      }
    }
  }
  return nat_entry.udp;
}

/**
 * This function checks for incoming packets if we already have a NAT entry.
 * If yes a pointer to the NAT entry is returned. Otherwise NULL.
 *
 * @param nat_config NAT configuration.
 * @param iphdr The IP header.
 * @param tcphdr The TCP header.
 * @return A pointer to an existing NAT entry or NULL if none is found.
 */
static ip4_nat_entries_tcp_t *
ip4_nat_tcp_lookup_incoming(const struct ip_hdr *iphdr, const struct tcp_hdr *tcphdr)
{
  int i;
  ip4_nat_entries_tcp_t *nat_entry = NULL;

  for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_TCP; i++) {
    if (ip4_nat_tcp_table[i].common.ttl) {
      if ((iphdr->src.addr == ip4_nat_tcp_table[i].common.dest.addr) &&
          (tcphdr->src == ip4_nat_tcp_table[i].dport) &&
          (tcphdr->dest == ip4_nat_tcp_table[i].nport)) {
        nat_entry = &ip4_nat_tcp_table[i];
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_tcp_lookup_incoming: i %d\n", i));
        ip4_nat_dbg_dump_tcp_nat_entry("ip4_nat_tcp_lookup_incoming: found existing nat entry: ",
                                      nat_entry);
        break;
      }
    }
  }
  return nat_entry;
}

/**
 * This function checks if we already have a NAT entry for this TCP connection.
 * If yes the a pointer to this NAT entry is returned.
 *
 * @param iphdr The IP header.
 * @param tcphdr The TCP header.
 * @param allocate If no existing NAT entry is found and this flag is true
 *   a NAT entry is allocated.
 */
static ip4_nat_entries_tcp_t *
ip4_nat_tcp_lookup_outgoing(ip4_nat_conf_t *nat_config, const struct ip_hdr *iphdr,
                           const struct tcp_hdr *tcphdr, u8_t allocate)
{
  int i;
  nat_entry_t nat_entry;
  int last_free = -1;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
  int oldest_entry = -1;
  s32_t min_ttl = LWIP_NAT_TCP_TTL_THRESHOLD_SECONDS;
  int offset = -1;
#endif

  nat_entry.cmn = NULL;
  for (i = 0; i < LWIP_NAT_DEFAULT_STATE_TABLES_TCP; i++) {
    if (ip4_nat_tcp_table[i].common.ttl) {
      if ((iphdr->src.addr == ip4_nat_tcp_table[i].common.source.addr) &&
          (iphdr->dest.addr == ip4_nat_tcp_table[i].common.dest.addr) &&
          (tcphdr->src == ip4_nat_tcp_table[i].sport) &&
          (tcphdr->dest == ip4_nat_tcp_table[i].dport)) {
        nat_entry.tcp = &ip4_nat_tcp_table[i];
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_tcp_lookup_outgoing: i %d\n", i));
        ip4_nat_dbg_dump_tcp_nat_entry("ip4_nat_tcp_lookup_outgoing: found existing nat entry: ",
                                      nat_entry.tcp);
        break;
      }
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
      else if (ip4_nat_tcp_table[i].common.ttl < min_ttl) {
        min_ttl = ip4_nat_tcp_table[i].common.ttl;
        oldest_entry = i;
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_tcp_lookup_outgoing: min_ttl=%d i=%d\n", min_ttl ,i));
      }
#endif
    } else {
        if(last_free == -1)
            last_free = i;
    }
  }
  if (nat_entry.cmn == NULL) {
    if (allocate) {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
      if (last_free != -1 || oldest_entry != -1) {
        offset = last_free != -1 ? last_free : oldest_entry;
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_tcp_lookup_outgoing: offset:%d last_free:%d oldest_entry:%d\n", offset,last_free,oldest_entry));
        nat_entry.tcp = &ip4_nat_tcp_table[offset];
#else
      if (last_free != -1) {
        nat_entry.tcp = &ip4_nat_tcp_table[last_free];
#endif
        u16_t src_port = ntohs(tcphdr->src);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
        if (nat_config->entry.in_if->link_mode == NETIF_LINK_MODE_NAT_NETDEV_LAN || \
            nat_config->entry.in_if->link_mode == NETIF_LINK_MODE_NAT_PPP_LAN) {

            uint8_t nSim = (nat_config->entry.in_if->sim_cid & 0xf0) >> 4;
            uint8_t cid = (nat_config->entry.in_if->sim_cid & 0x0f);
            uint8_t type = ((nat_config->entry.in_if->link_mode == NETIF_LINK_MODE_NAT_NETDEV_LAN) ? QL_NAT_ASSIGN_TYPE_USBNET : QL_NAT_ASSIGN_TYPE_PPP);
            
            if (quec_is_assign_port(nSim, cid, type, true)) {
                nat_entry.tcp->nport = tcphdr->src;
            } else {
                nat_entry.tcp->nport = htons((u16_t) (LWIP_NAT_DEFAULT_TCP_SOURCE_PORT + nat_config->entry.port_index * LWIP_NAT_DEFAULT_STATE_TABLES_TCP + src_port%LWIP_NAT_DEFAULT_STATE_TABLES_TCP));
            }
        } else {
            nat_entry.tcp->nport = htons((u16_t) (LWIP_NAT_DEFAULT_TCP_SOURCE_PORT + nat_config->entry.port_index * LWIP_NAT_DEFAULT_STATE_TABLES_TCP + src_port%LWIP_NAT_DEFAULT_STATE_TABLES_TCP));
        }
#else
        nat_entry.tcp->nport = htons((u16_t) (LWIP_NAT_DEFAULT_TCP_SOURCE_PORT + nat_config->entry.port_index * LWIP_NAT_DEFAULT_STATE_TABLES_TCP + src_port%LWIP_NAT_DEFAULT_STATE_TABLES_TCP));
#endif
        nat_entry.tcp->sport = tcphdr->src;
        nat_entry.tcp->dport = tcphdr->dest;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NETIF
        ip4_tcp_nat_cmn_init(nat_config, iphdr, nat_entry.cmn);
#else
        ip4_nat_cmn_init(nat_config, iphdr, nat_entry.cmn);
#endif

        ip4_nat_dbg_dump_tcp_nat_entry("ip4_nat_tcp_lookup_outgoing: created new nat entry: ",
                                      nat_entry.tcp);
      } else {
        LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_tcp_lookup_outgoing: no more NAT entries available\n"));
      }
    }
  }
  return nat_entry.tcp;
}

/** Adjusts the checksum of a NAT'ed packet without having to completely recalculate it
 * @todo: verify this works for little- and big-endian
 *
 * @param chksum points to the chksum in the packet
 * @param optr points to the old data in the packet
 * @param olen length of old data
 * @param nptr points to the new data in the packet
 * @param nlen length of new data
 */
static void
ip4_nat_chksum_adjust(u8_t *chksum, const u8_t *optr, s16_t olen, const u8_t *nptr, s16_t nlen)
{
  s32_t x, oldval, newval;

  LWIP_ASSERT("NULL != chksum", NULL != chksum);
  LWIP_ASSERT("NULL != optr", NULL != optr);
  LWIP_ASSERT("NULL != nptr", NULL != nptr);
#if NAT_DEBUG
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_chksum_adjust: chksum=%p, optr=%p, olen=%" U16_F ", nptr=%p, nlen=%" U16_F "\n",
    chksum, optr, olen, nptr, nlen));
#endif
  x = chksum[0] * 256 + chksum[1];
  x = ~x & 0xFFFF;
  while (olen) {
    oldval = optr[0] * 256 + optr[1];
    optr += 2;
    x -= oldval & 0xffff;
    if (x <= 0) {
      x--;
      x &= 0xffff;
    }
    olen -= 2;
  }
  while (nlen) {
    newval = nptr[0] * 256 + nptr[1];
    nptr += 2;
    x += newval & 0xffff;
    if (x & 0x10000) {
      x++;
      x &= 0xffff;
    }
    nlen -= 2;
  }
  x = ~x & 0xFFFF;
  chksum[0] = x / 256;
  chksum[1] = x & 0xff;
#if NAT_DEBUG
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_chksum_adjust: chksum = 0x%x\n", *((u16_t *) chksum)));
#endif
}

#if defined(LWIP_DEBUG) && (NAT_DEBUG & LWIP_DBG_ON)
/**
 * This function dumps an IP address
 *
 * @param addr IP address
 */
static void
ip4_nat_dbg_dump_ip(const ip4_addr_t *addr)
{
  LWIP_ASSERT("NULL != addr", NULL != addr);
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "%hu.%hu.%hu.%hu",
    ip4_addr1(addr), ip4_addr2(addr), ip4_addr3(addr), ip4_addr4(addr)));
}

/**
 * This function dumps an IP header
 *
 * @param msg a message to print
 * @param iphdr IP header
 */
static void
ip4_nat_dbg_dump(const char *msg, const struct ip_hdr *iphdr)
{
  LWIP_ASSERT("NULL != msg", NULL != msg);
  LWIP_ASSERT("NULL != iphdr", NULL != iphdr);
  OSI_LOGXI(OSI_LOGPAR_S, 0, "%s: IP: (", msg);
  ip4_addr_t src_ip4;
  ip4_addr_copy(src_ip4, iphdr->src);
  ip4_nat_dbg_dump_ip(&src_ip4);
  LWIP_DEBUGF(NAT_DEBUG, (0x0, " --> "));
  ip4_addr_t dest_ip4;
  ip4_addr_copy(dest_ip4, iphdr->dest);
  ip4_nat_dbg_dump_ip(&dest_ip4);
  LWIP_DEBUGF(NAT_DEBUG, (0x0, " id=%hu, chksum=%hu)\n",
    ntohs(IPH_ID(iphdr)), ntohs(IPH_CHKSUM(iphdr))));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "PROTO %d",IPH_PROTO(iphdr)));
}

/**
 * This function dumps an ICMP echo reply/recho request nat entry.
 *
 * @param msg a message to print
 * @param nat_entry the ICMP NAT entry to print
 */
static void
ip4_nat_dbg_dump_icmp_nat_entry(const char *msg, const ip4_nat_entries_icmp_t *nat_entry)
{
  LWIP_ASSERT("NULL != msg", NULL != msg);
  LWIP_ASSERT("NULL != nat_entry", NULL != nat_entry);
  LWIP_ASSERT("NULL != nat_entry->common.cfg", NULL != nat_entry->common.cfg);
  LWIP_ASSERT("NULL != nat_entry->common.cfg->entry.out_if",
    NULL != nat_entry->common.cfg->entry.out_if);
  OSI_LOGXI(OSI_LOGPAR_S, 0, "%s", msg);
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "ICMP : ("));
  ip4_nat_dbg_dump_ip(&(nat_entry->common.source));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, " --> "));
  ip4_nat_dbg_dump_ip(&(nat_entry->common.dest));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, " id=%hu", ntohs(nat_entry->id)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ", seq=%hu", ntohs(nat_entry->seqno)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ") mapped at ("));
  ip4_nat_dbg_dump_ip(ip_2_ip4(&(nat_entry->common.cfg->entry.out_if->ip_addr)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, " --> "));
  ip4_nat_dbg_dump_ip(&(nat_entry->common.dest));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, " id=%hu", ntohs(nat_entry->id)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ", seq=%hu", ntohs(nat_entry->seqno)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ")\n"));
}

/**
 * This function dumps an TCP nat entry.
 *
 * @param msg a message to print
 * @param nat_entry the TCP NAT entry to print
 */
static void
ip4_nat_dbg_dump_tcp_nat_entry(const char *msg, const ip4_nat_entries_tcp_t *nat_entry)
{
  LWIP_ASSERT("NULL != msg", NULL != msg);
  LWIP_ASSERT("NULL != nat_entry", NULL != nat_entry);
  LWIP_ASSERT("NULL != nat_entry->common.cfg", NULL != nat_entry->common.cfg);
  LWIP_ASSERT("NULL != nat_entry->common.cfg->entry.out_if",
    NULL != nat_entry->common.cfg->entry.out_if);
  OSI_LOGXI(OSI_LOGPAR_S, 0, "%s", msg);
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "TCP : ("));
  ip4_nat_dbg_dump_ip(&(nat_entry->common.source));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ":%hu", ntohs(nat_entry->sport)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, " --> "));
  ip4_nat_dbg_dump_ip(&(nat_entry->common.dest));
  LWIP_DEBUGF(NAT_DEBUG, (0x0,":%hu", ntohs(nat_entry->dport)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0,") mapped at ("));
  ip4_nat_dbg_dump_ip(ip_2_ip4(&(nat_entry->common.cfg->entry.out_if->ip_addr)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ":%hu", ntohs(nat_entry->nport)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, " --> "));
  ip4_nat_dbg_dump_ip(&(nat_entry->common.dest));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ":%hu", ntohs(nat_entry->dport)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ")\n"));
}

/**
 * This function dumps a UDP NAT entry.
 *
 * @param msg a message to print
 * @param nat_entry the UDP NAT entry to print
 */
static void
ip4_nat_dbg_dump_udp_nat_entry(const char *msg, const ip4_nat_entries_udp_t *nat_entry)
{
  LWIP_ASSERT("NULL != msg", NULL != msg);
  LWIP_ASSERT("NULL != nat_entry", NULL != nat_entry);
  LWIP_ASSERT("NULL != nat_entry->common.cfg", NULL != nat_entry->common.cfg);
  LWIP_ASSERT("NULL != nat_entry->common.cfg->entry.out_if",
    NULL != nat_entry->common.cfg->entry.out_if);
  OSI_LOGXI(OSI_LOGPAR_S, 0, "%s", msg);
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "UDP : ("));
  ip4_nat_dbg_dump_ip(&(nat_entry->common.source));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ":%hu", ntohs(nat_entry->sport)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, " --> "));
  ip4_nat_dbg_dump_ip(&(nat_entry->common.dest));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ":%hu", ntohs(nat_entry->dport)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ") mapped at ("));
  ip4_nat_dbg_dump_ip(ip_2_ip4(&(nat_entry->common.cfg->entry.out_if->ip_addr)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ":%hu", ntohs(nat_entry->nport)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, " --> "));
  ip4_nat_dbg_dump_ip(&(nat_entry->common.dest));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ":%hu", ntohs(nat_entry->dport)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, ")\n"));
}

/** Prints some info when creating a new NAT entry */
static void
ip4_nat_dbg_dump_init(ip4_nat_conf_t *ip4_nat_cfg_new)
{
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_init: added new NAT interface\n"));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_init:   "));
  ip4_nat_dbg_dump_ip(&(ip4_nat_cfg_new->entry.source_net));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "/"));
  ip4_nat_dbg_dump_ip(&(ip4_nat_cfg_new->entry.source_netmask));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "@"));
  ip4_nat_dbg_dump_ip(ip_2_ip4(&(ip4_nat_cfg_new->entry.in_if->ip_addr)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, " --> "));
  ip4_nat_dbg_dump_ip(&(ip4_nat_cfg_new->entry.dest_net));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "/"));
  ip4_nat_dbg_dump_ip(&(ip4_nat_cfg_new->entry.dest_netmask));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "@"));
  ip4_nat_dbg_dump_ip(ip_2_ip4(&(ip4_nat_cfg_new->entry.out_if->ip_addr)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "\n"));
}

/** Prints some info when removing a NAT entry */
static void
ip4_nat_dbg_dump_remove(ip4_nat_conf_t *cur)
{
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_remove: removing existing NAT interface\n"));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "ip4_nat_remove:   "));
  ip4_nat_dbg_dump_ip(&(cur->entry.source_net));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "/"));
  ip4_nat_dbg_dump_ip(&(cur->entry.source_netmask));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "@"));
  ip4_nat_dbg_dump_ip(ip_2_ip4(&(cur->entry.in_if->ip_addr)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, " --> "));
  ip4_nat_dbg_dump_ip(&(cur->entry.dest_net));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "/"));
  ip4_nat_dbg_dump_ip(&(cur->entry.dest_netmask));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "@"));
  ip4_nat_dbg_dump_ip(ip_2_ip4(&(cur->entry.out_if->ip_addr)));
  LWIP_DEBUGF(NAT_DEBUG, (0x0, "\n"));
}
#endif /* defined(LWIP_DEBUG) && (NAT_DEBUG & LWIP_DBG_ON) */

#endif /* IP_NAT */
