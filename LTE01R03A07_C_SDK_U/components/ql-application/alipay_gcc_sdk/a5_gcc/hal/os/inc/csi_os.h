#ifndef __CSI_OS_H__
#define __CSI_OS_H__

#include "alipay_common.h"
#include "csi_common.h"


/*○ description:
    ■ get system timestamp(unix timestamp format)
  ○ param
    ■ tm - the var which store the timestamp
  ○ return
    ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_get_timestamp(uint32_t *tm);

/*○ description:
  ■ 获取SDK编译时间
  ○ param:
  ■ timestamp 编译时间戳
  ○ return:
  ■ CSI_OK:success，otherwise:fail*/
csi_error_t csi_get_compile_timestamp(uint32_t *timestamp);


/*○ description:
    ■ allocates space for an object whose size is specified by 'size' and whose value is indeterminate
  ○ param:
    ■ size - allocated space length
  ○ return:
    ■ either a null pointer or a pointer to the allocated space*/
void *csi_malloc(uint32_t size);

/*○ description:
    ■ allocates space for an array of nmemb objects, each of whose size is 'size'. The space is initialised to all bits zero
  ○ param:
    ■ nblock - number of parameters
    ■ size - parameter length
  ○ return:
    ■ either a null pointer or a pointer to the allocated space*/
void *csi_calloc(uint32_t nblock, uint32_t size);

/*○ description:
    ■ causes the space pointed to by ptr to be deallocated
  ○ param:
    ■ pt - the pointer to be deallocated
  ○ return:
    ■ */
void csi_free(void *pt);

/*○ description:
    ■ changes the size of the object pointed to by ptr to the size specified by size
  ○ param:
    ■ pt - the pointer to be changed
    ■ size - new length of pt 
  ○ return:
    ■ either a null pointer or a pointer to the possibly moved allocated space*/
void *csi_realloc(void *pt, uint32_t size);

/*○ description:
    ■ get random data
  ○ param
    ■ random - buffer to store random data
    ■ size - length of random data
  ○ return
    ■ CSI_OK:success，otherwise:fail*/
EXTERNC csi_error_t csi_rng_get_data(void *random, uint32_t size);

/*○ description:
    ■ print log
  ○ param
    ■ format - log string in format
    ■ val_list - ouput values
  ○ return
    ■ none*/
EXTERNC void csi_log_ext(const char *format, va_list* val_list);

#endif