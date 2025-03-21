/*
 * Copyright (C) 2015-2019 Alibaba Group Holding Limited
 */

#ifndef __HAL_COMMON_H__
#define __HAL_COMMON_H__

#include <stdint.h>
#define IDENT_HASH_LEN 32
typedef enum {
    HAL_OK = 0,
    HAL_ERROR = -1,
    HAL_INVALID_PARAM = -2,
    HAL_BIND_VERIFY_FAILED = -3,
    HAL_OPEN_ASSET_ERROR = -4,
    HAL_ASSET_READ_ERROR = -5,
    HAL_KEY_INIT_ERROR = -6,
    HAL_KEY_READ_ERROR = -7,
    HAL_MALLOC_ERROR = -8,
    HAL_READ_CREDENT_ERROR = -9,
    HAL_DIGEST_GET_ERROR = -10,
    HAL_CREDENT_HASH_ERROR = -11,
    HAL_HUK_GET_ERROR = -12,
    HAL_HUK_CHECK_ERROR = -13,
    HAL_SAD_VERIFY_ERROR = -14,
} hal_error_t;

typedef struct {
    uint16_t type : 1;
    uint16_t rsvr : 15;
} __attribute__ ((packed)) parser_key_flag_t;

typedef struct {
    uint16_t          type;
    uint16_t          size;
    parser_key_flag_t flag;
    uint16_t          rsvr;
    uint32_t          offset;
    uint32_t          rsvr2;
} __attribute__ ((packed)) parser_key_info_t;

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t rsv;
    uint16_t keynum;
    uint16_t digest_type;
    uint32_t total_size;
} __attribute__ ((packed)) parser_head_t;

typedef struct {
    parser_head_t     head;
    parser_key_info_t key[1];
} __attribute__ ((packed)) parser_t;

typedef struct{
    uint32_t credent_len;
    uint8_t hash[IDENT_HASH_LEN];
} __attribute__ ((packed)) credent_info_t;

typedef struct{
    uint8_t *cid;
    uint32_t cid_len;
    uint8_t *huk;
    uint32_t huk_len;
    uint8_t *sad;
    uint32_t sad_len;
    credent_info_t credent_info;
} kp_key_t;

typedef struct
{
    uint32_t total[2];          /*!< number of bytes processed  */
    uint32_t state[8];          /*!< intermediate digest state  */
    unsigned char buffer[64];   /*!< data block being processed */
    int is224;                  /*!< 0 => SHA-256, else SHA-224 */
}sec_sha256_context;

typedef struct
{
    int nr;                     /*!<  number of rounds  */
    uint32_t *rk;               /*!<  AES round keys    */
    uint32_t buf[68];           /*!<  unaligned data    */
}sec_aes_context;


#endif /* __HAL_COMMON_H__ */