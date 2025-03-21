#ifndef __ALIPAY_MEM_H__
#define __ALIPAY_MEM_H__

#include "alipay_common.h"

EXTERNC uint16_t read16(const uint8_t* buffer);

EXTERNC uint32_t read32(const uint8_t* buffer);

EXTERNC void write16(uint8_t* buffer, uint16_t value);

EXTERNC void write32(uint8_t* buffer, uint32_t value);

EXTERNC bool is_little_endian(void) ;

#endif
