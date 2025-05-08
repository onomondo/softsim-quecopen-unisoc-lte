/*
 * Copyright (c) 2024 Onomondo ApS. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once

#include <stdint.h>


void ss_logp(uint32_t subsys, uint32_t level, const char *file, int line, const char *format, ...)
	__attribute__((format(printf, 5, 6)));

enum log_subsys {
  SBTLV,
  SCTLV,
  SVPCD,
  SIFACE,
  SUICC,
  SCMD,
  SLCHAN,
  SFS,
  SSTORAGE,
  SACCESS,
  SADMIN,
  SSFI,
  SDFNAME,
  SFILE,
  SPIN,
  SAUTH,
  SPROACT,
  STLV8,
  SSMS,
  SREMOTECMD,
  SREFRESH,
  SAPDU,
  _NUM_LOG_SUBSYS
};

enum log_level {
	LERROR,
	LINFO,
	LDEBUG,
	_NUM_LOG_LEVEL
};
