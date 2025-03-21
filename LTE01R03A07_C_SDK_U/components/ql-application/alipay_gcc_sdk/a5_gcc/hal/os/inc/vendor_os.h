/****************************************************************************
 * IoTPay modules 1.0 2017/10/19
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
 * ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. ALIPAY, INC.
 * AND ITS LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE
 * AS A RESULT OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS
 * DERIVATIVES. IN NO EVENT WILL ALIPAY OR ITS LICENSORS BE LIABLE FOR ANY LOST
 * REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL,
 * INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY
 * OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE,
 * EVEN IF ALIPAY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 ****************************************************************************
 * You acknowledge that this software is not designed, licensed or intended
 * for use in the design, construction, operation or maintenance of any
 * nuclear facility.
*****************************************************************************/

#ifndef __VENDOR_OS_H__
#define __VENDOR_OS_H__

#include "alipay_common.h"

/*获取当前系统时间戳，单位为秒
 * 注:UTC时间戳，从1970开始的
 * @param 
 * @return 时间戳
 */
uint32_t alipay_get_timestamp(void);

/*设置手表时间，设置之后要求及时生效，并且通过 alipay_get_timestamp 接口获取的是修改后的时间
 * 注:UTC时间戳，从1970开始的
 * @param [in] timestamp_s  时间戳，单位为s
 * @return 
 */
void alipay_set_system_time(PARAM_IN int32_t timestamp_s);

/*获取编译时候的时间戳，单位为秒
 * @return 编译时候的时间戳，单位为秒
 */
uint32_t alipay_get_compile_timestamp(void);

/*
 *功能同malloc函数，分配成功之后做清零操作
 */
void* alipay_malloc(uint32_t size);

/*
 *功能同calloc函数，分配成功之后做清零操作
 */
void* alipay_calloc(uint32_t nblock,uint32_t size);

/*
 *功能同free函数
 */
void alipay_free(void* pt);

/**
* 获取一个非0的随机数
*/
int alipay_rand(void);

/*
 *功能同realloc函数
 */
void* alipay_realloc(void* pt, uint32_t size);

/**
* 设定随机数种子
*/
void alipay_srand(unsigned int seed);

/////////////////////////////////////////////
////日志信息输出接口
/////////////////////////////////////////////
EXTERNC void alipay_log_ext(const char *format, ...);

int32_t get_Rng_number(uint8_t *data, unsigned size);


typedef struct {
    int32_t year;
    int32_t month;
    int32_t wday;
    int32_t day;
    int32_t hour;
    int32_t minute;
    int32_t second;
    int32_t isdst;
} alipay_iot_local_time;


/*
* 获取系统当前时间
* month 为1表示1月，day 为1表示1号，year为2018表示2018年。
* hour 取值0-23.  min取值0-59. second取值0-59.
*/
void alipay_iot_get_local_time(alipay_iot_local_time *time);

#endif






