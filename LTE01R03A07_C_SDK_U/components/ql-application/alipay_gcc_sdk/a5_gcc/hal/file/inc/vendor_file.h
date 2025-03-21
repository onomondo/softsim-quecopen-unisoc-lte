#ifndef __VENDOR_FILE_H__
#define __VENDOR_FILE_H__

#include "alipay_common.h"

/*
 * 打开文件名为filename的文件，要求权限为可读写；
 * 参数：filename [in] 文件名(字符串)
 * 返回值: 文件句柄: 成功
 *        -1 或者 NULL: 失败
*/
EXTERNC void* alipay_open_rsvd_part(PARAM_IN char filename[128]);

/*
 * 往文件中写数据
 * 参数：fd [in] 文件句柄
 *      data [in] 要写入的数据
 *      data_len [in] 要写入的数据的长度
 * 返回值: 0: 写入成功
 *        -1: 写入失败
*/
EXTERNC int alipay_write_rsvd_part(PARAM_IN void* fd, PARAM_IN void *data,PARAM_IN uint32_t data_len);

/*
 * 从提供给alipay kv存储的文件中读取数据
 * 参数：fd [in] 文件句柄
 *      buffer [out] 存放读取的数据的缓存
 *      read_len [in] 允许读取的最大长度
 *      read_len [out] 实际读取的数据长度
 * 返回值: 0: 读取成功
 *        -1: 读取失败,或者长度为0
*/
EXTERNC int alipay_read_rsvd_part(PARAM_IN void* fd,PARAM_OUT void *buffer,PARAM_INOUT uint32_t* read_len);

/*
 * 关闭打开的文件
 * 参数：fd [in] 文件句柄
 * 返回值: 0: 关闭成功
 *        -1: 关闭失败
*/
EXTERNC int alipay_close_rsvd_part(PARAM_IN void* fd);

/*
 * 查看文件是否存在
 * 参数: filename [in] 文件名(字符串)
 * 返回值: 1: 文件存在
          0: 文件不存在
 */
EXTERNC int alipay_access_rsvd_part(PARAM_IN char filename[128]);

/*
 * 删除文件
 * 参数: filename [in] 文件名(字符串)
 * 返回值: 0: 删除成功
 *       -1: 删除失败
 */
EXTERNC int alipay_remove_rsvd_part(PARAM_IN char filename[128]);

/*
 * 清除所有通过alipay_write_rsvd_part存储的alipay文件
 * 返回值: 0: 清除成功
 *       -1: 清除失败
 */
EXTERNC int alipay_clear_rsvd_part(void);

#endif