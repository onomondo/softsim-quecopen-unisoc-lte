/*
 * Copyright (c) 2024 Onomondo ApS. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include "ql_log.h"

#define SS_ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
uint32_t ss_log_mask = 0xffffffff;
//int	vsprintf (char *__restrict, const char *__restrict, __VALIST) _ATTRIBUTE ((__format__ (__printf__, 2, 0)));
/* TODO #64: add a mechanism to modify the log levels at runtime via getopt */
static uint32_t subsys_lvl[_NUM_LOG_SUBSYS] = {
    [SBTLV] = LERROR,   [SCTLV] = LERROR,      [SVPCD] = LERROR,
    [SIFACE] = LERROR,  [SUICC] = LERROR,      [SCMD] = LERROR,
    [SLCHAN] = LERROR,  [SFS] = LERROR,        [SSTORAGE] = LERROR,
    [SACCESS] = LERROR, [SADMIN] = LERROR,     [SSFI] = LERROR,
    [SDFNAME] = LERROR, [SFILE] = LERROR,      [SPIN] = LERROR,
    [SAUTH] = LERROR,   [SPROACT] = LERROR,    [STLV8] = LERROR,
    [SSMS] = LERROR,    [SREMOTECMD] = LERROR, [SREFRESH] = LERROR,
};

static const char *subsys_str[_NUM_LOG_SUBSYS] = {
    [SBTLV] = "BTLV",     [SCTLV] = "CTLV",     [SVPCD] = "VPCD",
    [SIFACE] = "IFACE",   [SUICC] = "UICC",     [SCMD] = "CMD",
    [SLCHAN] = "LCHAN",   [SFS] = "FS",         [SSTORAGE] = "STORAGE",
    [SACCESS] = "ACCESS", [SADMIN] = "ADMIN",   [SSFI] = "SFI",
    [SDFNAME] = "DFNAME", [SFILE] = "FILE",     [SPIN] = "PIN",
    [SAUTH] = "AUTH",     [SPROACT] = "PROACT", [SREMOTECMD] = "REMOTECMD",
    [STLV8] = "TLV8",     [SSMS] = "SMS",       [SREFRESH] = "REFRESH",
};

static const char *level_str[_NUM_LOG_LEVEL] = {
    [LERROR] = "ERROR",
    [LINFO] = "INFO",
    [LDEBUG] = "DEBUG",
};

void ss_logp(uint32_t subsys, uint32_t level, const char *file, int line, const char *format, ...){
  // return; // optionally return early here to disable all logging from softsim
  va_list ap;

  if (!(ss_log_mask & (1 << subsys)))
    return;

  // assert(subsys < SS_ARRAY_SIZE(subsys_lvl));

  if (level > subsys_lvl[subsys])
    return;
  char buf[1024];

  int len = sprintf(buf, "%8s %8s ", subsys_str[subsys], level_str[level]);
  va_start(ap, format);
  vsprintf(buf + len, format, ap);
  va_end(ap);

  // quectel specific log implementation
  QL_LOG(QL_LOG_LEVEL_INFO, "softsim", "%s", buf);
}
