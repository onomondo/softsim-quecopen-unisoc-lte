#ifndef __CSI_COMMON_H__
#define __CSI_COMMON_H__

#include <stdint.h>
#include <ctype.h>
#include <stdarg.h>
#include <alipay_common.h>

typedef enum {
    CSI_OK          =  0,
    CSI_ERROR       = -1,
    CSI_BUSY        = -2,
    CSI_TIMEOUT     = -3,
    CSI_UNSUPPORTED = -4
 } csi_error_t;
 
 typedef enum {
    AES_KEY_LEN_BITS_128  = 128,
    AES_KEY_LEN_BITS_256  = 256
} csi_aes_key_bits_t;
 
#define CHECK_UNUSED_ARG(a)	
 
 #endif