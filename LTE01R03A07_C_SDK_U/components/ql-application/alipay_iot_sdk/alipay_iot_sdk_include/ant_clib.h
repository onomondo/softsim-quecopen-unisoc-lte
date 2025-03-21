/**
 * ant_clib.h
 *
 * 系统C库兼容接口
 */
#ifndef __ANT_CLIB_H__
#define __ANT_CLIB_H__
#include <stdarg.h>
#include "ant_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ANT_MIN(a, b) ((a) > (b) ? (b) : (a))
#define ANT_MAX(a, b) ((a) > (b) ? (a) : (b))
#define ANT_NOT_NULL(a) ((a) ? (a) : "NULL")
#define ANT_NULL_DEF(a, def) (((a) && ant_strlen(a)) ? (a) : ((def) ? (def) : "NULL"))
#define ANT_ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

// === STDIO STRING STDLIB ===
void *ant_memset(void *s, int c, size_t n);

void *ant_memcpy(void *dst, const void *src, size_t n);

void *ant_memmove(void *dst, const void *src, size_t n);

int   ant_memcmp(const void *s1, const void *s2, size_t n);

void *ant_memmem(const void *s, size_t len, const void *sub, size_t sub_len);

size_t ant_strlen(const char *s);

char *ant_strcpy(char *dst, const char *src);

char *ant_strncpy(char *dst, const char *src, int n);

char *ant_strcat(char *dst, const char *src);

char *ant_strncat(char *dst, const char *src, int n);

char *ant_strchr(const char *s, int c);

char *ant_strrchr(const char *s, int c);

char *ant_strstr(const char *str, const char *sub);

char *ant_strtok(char *str, const char *delim);

int   ant_split(char *src, const char *sep, char **dest, int num);

int   ant_tolower(int c);

char *ant_strdup(const char *s);

int   ant_stricmp(const char *s, const char *t);

int   ant_strnicmp(const char *s, const char *t, int n);

int   ant_strcmp(const char *s, const char *t);

int   ant_strncmp(const char *s, const char *t, int n);

int   ant_snprintf(char *str, size_t size, const char *format, ...);
int   ant_vsnprintf(char *str, size_t size, const char *format, va_list args);

int   ant_sscanf(const char *str, const char *format, ...);
int   ant_vsscanf(const char * buffer, const char * format, va_list ap);

double ant_fabs(double value);


unsigned long int ant_strtol(const char *nptr, char **endptr, int base);
unsigned long long ant_strtoll(const char *nptr, char **endptr, int base);
unsigned long int ant_strtoul(const char *nptr, char **endptr, int base);
unsigned long long ant_strtoull(const char *nptr, char **endptr, int base);
int ant_atoi(const char *nptr);
long ant_atol(const char *nptr);
long long ant_atoll(const char *nptr);
double ant_strtod(const char *nptr, char **endptr);
float ant_strtof(const char *nptr, char **endptr);
void ant_hextostr(char *str, const ant_u8 *hex, int n, int caps);
// === STDIO STRING STDLIB end ===

ant_u16 ant_ntohs(ant_u16 netshort);
ant_u32 ant_ntohl(ant_u32 netlong);
ant_u16 ant_htons(ant_u32 hostshort);
ant_u32 ant_htonl(ant_u32 hostlong);
char   *ant_inet_ntoa(ant_u32 addr);


void ant_assert(ant_bool expression);

/**
 * 产生系统随机数
 *
 * @param p_rng 随机数发生句柄，可填NULL
 * @param output 随机数缓存
 * @param output_len 随机数长度
 * @return
 */
int ant_random(void *p_rng, unsigned char *output, size_t output_len);

/**
 * 比较版本号
 *
 * @param v1 第一个版本号字符串，形如"a.b.c.d..."
 * @param v2 第二个版本号字符串，形如"a.b.c.d..."
 * @param n  比较前n个由'.'区隔的版本数字
 *
 * @return -1: 版本@v1 < 版本@v2
 *          0: 版本@v1 = 版本@v2
 *          1: 版本@v1 > 版本@v2
 *
 * 说明: 1. 版本数字通过'.'区隔，从高到底逐个比较
 */
int ant_vercmp(const char *v1, const char *v2, int n);

int ant_random_digit(void *p_rng, unsigned char *output, size_t output_len);

/**
 * 获取系统编译日期时间
 *
 * @return 返回系统编译日期时间，格式：“yyyy/MM/dd MM:hh:ss"
 */
char * ant_build_date_time(void);

void ant_halt(void);

/**
 * 关于文件打开模式的说明(Linux C Manpage)
 *
 * "r"
 * Open text file for reading. The stream is positioned at the beginning of the file.
 *
 * "r+"
 * Open for reading and writing. The stream is positioned at the beginning of the file.
 *
 * "w"
 * Truncate file to zero length or create text file for writing. The stream is positioned
 * at the beginning of the file.'
 *
 * "w+"
 * Open for reading and writing. The file is created if it does not exist, otherwise it is
 * truncated. The stream is positioned at the beginning of the file.
 *
 * "a"
 * Open for appending (writing at end of file). The file is created if it does not exist.
 * The stream is positioned at the end of the file.
 *
 * "a+"
 * Open for reading and appending (writing at end of file). The file is created if it does not
 * exist. The initial file position for reading is at the beginning of the file, but output is
 * always appended to the end of the file.
*/

/**
 * 将支付宝定义的文件打开标识转换成fopen识别的打开模式
 *
 * @param flag 蚂蚁定义的文件打开模式
 *
 * @return fopen识别的打开模式, 对应关系如下
 *
 * "r",  "rb"  <--> ANT_FS_READ
 * "r+", "rb+" <--> ANT_FS_READ_WRITE
 * "w",  "wb"  <--> ANT_FS_WRITE
 * "w+", "wb+" <--> ANT_FS_WRITE|ANT_FS_READ
 * "a",  "ab"  <--> ANT_FS_APPEND
 * "a+", "ab+" <--> ANT_FS_APPEND|ANT_FS_READ
 *
 * 注意: 其他值或组合值接口返回NULL
 */
const char *ant_fs_open_mode(ant_s32 flag);

/**
 * 将fopen识别的文件打开模式转换成支付宝定义的文件打开标识
 *
 * @param mode flag fopen识别的文件打开模式
 *
 * @return 蚂蚁定义的文件打开模式, 对应关系如下
 *
 * "r",  "rb"  <--> ANT_FS_READ
 * "r+", "rb+" <--> ANT_FS_READ_WRITE
 * "w",  "wb"  <--> ANT_FS_WRITE
 * "w+", "wb+" <--> ANT_FS_WRITE|ANT_FS_READ
 * "a",  "ab"  <--> ANT_FS_APPEND
 * "a+", "ab+" <--> ANT_FS_APPEND|ANT_FS_READ
 */
ant_s32 ant_fs_open_flag(const char *mode);

/**
 * 将支付宝定义的文件打开标识转换成open识别的打开模式
 *
 * @param flag 蚂蚁定义的文件打开模式
 *
 * @return open识别的打开模式, 对应关系如下
 *
 * O_RDONLY                   <--> ANT_FS_READ
 * O_RDWR                     <--> ANT_FS_READ_WRITE
 * O_CREATE|O_TRUNC|O_WRONLY  <--> ANT_FS_WRITE
 * O_CREATE|O_TRUNC|O_RDWR    <--> ANT_FS_WRITE|ANT_FS_READ
 * O_CREATE|O_APPEND|O_WRONLY <--> ANT_FS_APPEND
 * O_CREATE|O_APPEND|O_RDWR   <--> ANT_FS_APPEND|ANT_FS_READ
 *
 * 注意: 其他值或组合值接口返回0
 */
ant_s32 ant_fs_open_opt(ant_s32 flag);


ant_u32 ant_hashword(const ant_u32 *k, ant_size length, ant_u32 initval);

#ifdef __cplusplus
} // extern "C"
#endif

#endif

