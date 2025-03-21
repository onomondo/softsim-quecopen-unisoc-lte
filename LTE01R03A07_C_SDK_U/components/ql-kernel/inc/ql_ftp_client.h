/**  
  @file
  ql_ftp_client.h

  @brief
  This file provides the definitions for ftp client, and declares the 
  API functions.

*/
/*============================================================================
  Copyright (c) 2017 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------

=============================================================================*/


#ifndef __QL_FTP_CLIENT_H__
#define __QL_FTP_CLIENT_H__

#include <time.h>
#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_FILE_NAME_SIZE		256
#define MAX_OWNER_SIZE		64
#define QL_FTP_CLIENT_FREE(p)   if(p){free(p);p=NULL;}
#define QL_FTP_ERRCODE_BASE     (QL_COMPONENT_LWIP_FTP << 16)

typedef enum
{
	QL_FTP_CLIENT_ERR_SUCCESS           = 0,                         //成功
	QL_FTP_CLIENT_ERR_INVALID_CLIENT    = 1 | QL_FTP_ERRCODE_BASE,   //不可用的ftp client句柄
	QL_FTP_CLIENT_ERR_INVALID_HOST,                                  //不可用的请求主机地址
	QL_FTP_CLIENT_ERR_DNS_FAIL,                                      //对服务器进行DNS解析失败
	QL_FTP_CLIENT_ERR_SOCK_CREATE_FAIL,                              //创建socket失败
	QL_FTP_CLIENT_ERR_SOCK_BIND_FAIL    = 5 | QL_FTP_ERRCODE_BASE,   //对socket进行bind操作失败
	QL_FTP_CLIENT_ERR_SOCK_CONN_FAIL,                                //执行socket连接失败
	QL_FTP_CLIENT_ERR_SOCK_SEND_FAIL,                                //发送数据失败
	QL_FTP_CLIENT_ERR_SOCK_RECV_FAIL,                                //接收数据失败
	QL_FTP_CLIENT_ERR_SOCK_CLOSE_FAIL,                               //断开连接失败
	QL_FTP_CLIENT_ERR_SOCK_DISCONNECTED = 10 | QL_FTP_ERRCODE_BASE,  //连接断开
	QL_FTP_CLIENT_ERR_SSL_CONN_FAIL,                                 // SSL 连接失败
	QL_FTP_CLIENT_ERR_RESP_TIMEOUT,                                  //请求超时
	QL_FTP_CLIENT_ERR_CREATE_FILE_FAIL,                              //创建文件失败
	QL_FTP_CLIENT_ERR_NO_FILE,                                       //文件不存在
	QL_FTP_CLIENT_ERR_NO_DIR            = 15 | QL_FTP_ERRCODE_BASE,  //目录不存在
	QL_FTP_CLIENT_ERR_INVALID_PARAMATER,                             //无效参数
	QL_FTP_CLIENT_ERR_REQUESTED_FILENAME_INVALID,                    //请求的文件名无效
	QL_FTP_CLIENT_ERR_UNKNOWN,                                       //其他错误
} QL_FTP_CLIENT_OLD_ERR_E;

typedef enum
{
	QL_FTP_ERR_OK 						= 0,							/*Operation successful*/
	QL_FTP_ERR_UNKNOWN 					= 601 | QL_FTP_ERRCODE_BASE,	/*Unknown error*/
	QL_FTP_ERR_WOULDBLOCK 				= 602 | QL_FTP_ERRCODE_BASE,	/*FTP(S) server blocked*/
	QL_FTP_ERR_BUSY 					= 603 | QL_FTP_ERRCODE_BASE,	/*FTP(S) server busy*/
	QL_FTP_ERR_DNS 						= 604 | QL_FTP_ERRCODE_BASE,	/*failed to parse domain name*/
	QL_FTP_ERR_NETWORK 					= 605 | QL_FTP_ERRCODE_BASE, 	/*failed to establish socket or the network is deactivated*/
	QL_FTP_ERR_CTRL_CLOSE 				= 606 | QL_FTP_ERRCODE_BASE,	/*the ftp session has been closed by the ftp server*/
	QL_FTP_ERR_DATA_CLOSE 				= 607 | QL_FTP_ERRCODE_BASE,	/*the data connection was closed by the ftp server*/
	QL_FTP_ERR_BEARER_FAIL 				= 608 | QL_FTP_ERRCODE_BASE,	/*Socket closed by peer*/
	QL_FTP_ERR_TIMEOUT 					= 609 | QL_FTP_ERRCODE_BASE,	/*Timeout error*/
	QL_FTP_ERR_INVALID_PARAM 			= 610 | QL_FTP_ERRCODE_BASE,	/*Invalid parameter*/
	QL_FTP_ERR_FILE_NOT_FOUND 			= 611 | QL_FTP_ERRCODE_BASE,	/*Failed to open file*/
	QL_FTP_ERR_FILE_POINT_ERR 			= 612 | QL_FTP_ERRCODE_BASE,	/*File position invalid*/
	QL_FTP_ERR_FS_FILE_ERROR 			= 613 | QL_FTP_ERRCODE_BASE,	/*File error*/
	QL_FTP_ERR_SERVICE_END 				= 614 | QL_FTP_ERRCODE_BASE,	/*Service not available, closing control connection*/
	QL_FTP_ERR_DATA_FAILED 				= 615 | QL_FTP_ERRCODE_BASE,	/*Open data connection failed*/
	QL_FTP_ERR_CLOSE_CONN 				= 616 | QL_FTP_ERRCODE_BASE,	/*Connection closed; transfer aborted*/
	QL_FTP_ERR_FILE_UNOPER 				= 617 | QL_FTP_ERRCODE_BASE,	/*Requested file action not taken*/
	QL_FTP_ERR_REQUEST_ABORT 			= 618 | QL_FTP_ERRCODE_BASE,	/*Requested action aborted: local error in processing*/
	QL_FTP_ERR_OVER_MEMORY 				= 619 | QL_FTP_ERRCODE_BASE,	/*Requested action not taken: insufficient system storage*/
	QL_FTP_ERR_COMMAND_ERROR 			= 620 | QL_FTP_ERRCODE_BASE,	/*Syntax error, command unrecognized*/
	QL_FTP_ERR_PARAM_ERROR 				= 621 | QL_FTP_ERRCODE_BASE,	/*Syntax error in parameters or arguments*/
	QL_FTP_ERR_COMMAND_FAILED 			= 622 | QL_FTP_ERRCODE_BASE,	/*Command not implemented*/
	QL_FTP_ERR_COMMAND_BAD_SEQUENCE 	= 623 | QL_FTP_ERRCODE_BASE,	/*Bad sequence of commands*/
	QL_FTP_ERR_COMMAND_NOT_IMPLENT 		= 624 | QL_FTP_ERRCODE_BASE,	/*Command parameter not implemented*/
	QL_FTP_ERR_UNLOGIN 					= 625 | QL_FTP_ERRCODE_BASE,	/*Not logged in*/
	QL_FTP_ERR_NO_ACCOUNT 				= 626 | QL_FTP_ERRCODE_BASE,	/*Need account for storing files*/
	QL_FTP_ERR_REQUEST_FAILED 			= 627 | QL_FTP_ERRCODE_BASE,	/*Requested action not taken*/
	QL_FTP_ERR_REQUEST_STOP 			= 628 | QL_FTP_ERRCODE_BASE,	/*Requested action aborted: page type unknown*/
	QL_FTP_ERR_FILEREQ_STOP 			= 629 | QL_FTP_ERRCODE_BASE,	/*Requested file action aborted*/
	QL_FTP_ERR_FILENAME_ERROR 			= 630 | QL_FTP_ERRCODE_BASE,	/*Requested file name invalid*/
	QL_FTP_ERR_SSL_AUTH_FAIL 			= 631 | QL_FTP_ERRCODE_BASE,	/*SSL authentication failed*/
	QL_FTP_ERR_INTERFACE_FAILED 		= 632 | QL_FTP_ERRCODE_BASE,	/*Source IP address for transmission cannot use*/
	QL_FTP_ERR_SEND_ERROR 				= 633 | QL_FTP_ERRCODE_BASE,	/*Send data failed*/
	QL_FTP_ERR_RECV_ERROR 				= 634 | QL_FTP_ERRCODE_BASE,	/*Receive data failed*/
} QL_FTP_CLIENT_ERR_E;

typedef enum {
    QL_FTP_CLIENT_SIM_CID,//设置SIM卡的CID
    QL_FTP_CLIENT_SSL_CTX,//设置FTP客户端使用的SSL Context
    QL_FTP_CLIENT_SSL_ENABLE,//设置FTP客户端是否使用SSL连接
	QL_FTP_CLIENT_OPT_PDP_CID, //设置ftp client使用的数据通道CID,执行数据拨号操作时使用的CID
	QL_FTP_CLIENT_OPT_SSL_CTX, //设置SSL
	QL_FTP_CLIENT_OPT_START_POS,//设置下载的起始偏移位置
	QL_FTP_CLIENT_SSL_VERIFY_LEVEL,//设置SSL的校验方式
    QL_FTP_CLIENT_SSL_CACERT_PATH,//设置SSL的证书
    QL_FTP_CLIENT_SSL_OWNCERT_PATH,//设置SSL的本地证书
    QL_FTP_CLIENT_SSL_CIPHERSUITE,//设置SSL的加密套件
    QL_FTP_CLIENT_SSL_VERSION,//设置SSL的版本信息
    QL_FTP_CLIENT_SSL_TRANSPORT,//设置SSL的协议类型
    QL_FTP_CLIENT_PORT_MODE,//设置FTP建立数据连接的主被动模式
    QL_FTP_CLIENT_IPV6_EXTEND,//设置IPV6扩展功能是否开启
    //QL_FTP_CLIENT_SSL_PSK,
    QL_FTP_CLIENT_ERR_FORMAT,//设置FTP使用的错误码
    QL_FTP_CLIENT_TIMEOUT,//设置FTP响应超时时间,最小20s,最大180s,默认90s
    QL_FTP_CLIENT_SSH_PRIVATE_PATH,//设置sftp证书登录的私钥文件路径,优先级高于密码登录的方式
}QL_FTP_CLIENT_OPT_E;

typedef enum {
    QL_FTP_CLIENT_SSL_TYPE_NONE,		// 连接FTP服务器
    QL_FTP_CLIENT_SSL_TYPE_IMP,			// 隐式加密连接FTPS服务器
    QL_FTP_CLIENT_SSL_TYPE_EXP,			// 显式加密连接FTPS服务器
    QL_FTP_CLIENT_SSL_TYPE_SSH,			// 连接SFTP服务器
}QL_FTP_CLIENT_SSL_TYPE_E;


typedef struct{
	char filename[MAX_FILE_NAME_SIZE];//文件或目录名
	int type;//类型，0：未知；1：目录；2：文件
	int permissions;//权限
	int links;//链接数
	char owner[MAX_OWNER_SIZE];//所属用户
	char group[MAX_OWNER_SIZE];//所属用户组
	struct tm timestamp;//创建时间
	unsigned long size;//文件大小，字节
}QL_FTP_CLIENT_FILE_INFO_T;


/*****************************************************************
* Function: QL_FTP_CLIENT_WRITE_CB_EX
*
* Description:
* 	使用回调函数接收从FTP服务器获取的文件数据。
* 
* Parameters:
* 	ptr	  		    [in] 	获取数据指针。可通过该指针将获得的数据写入用户所指定的存储位置。
* 	size    		[in] 	要被写入的每个元素的大小，以字节为单位。
* 	nmemb   		[in] 	元素的个数，每个元素的大小为size字节。
*   stream          [in]    用户传入的callback function参数指针，可通过该指针指定参数ptr中数据存放的位置。
* Return:
* 	如果成功，该函数需要返回size*nmemb大小的字节，否则会导致数据读错误，导致ftp异常结束。
*
*****************************************************************/
typedef size_t (*QL_FTP_CLIENT_WRITE_CB_EX)(void *ptr, size_t size, size_t nmemb, void *stream);


/*****************************************************************
* Function: QL_FTP_CLIENT_READ_CB_EX
*
* Description:
* 	使用回调函数发送数据到FTP服务器。
* 
* Parameters:
* 	ptr 	  		[out] 	读取数据的指针。用户传入的数据可被该指读取，完成用户数据的记录。
* 	size    		[in] 	要被读取的每个元素的大小，以字节为单位。
* 	nmemb   		[in] 	元素的个数，每个元素的大小为size字节。
*   stream          [in]    用户传入的callback function参数指针，可通过该指针向参数ptr传入数据。
* Return:
* 	如果成功，该函数需要返回size*nmemb大小的字节，否则会导致数据读错误，导致ftp异常结束。
*
*****************************************************************/
typedef size_t (*QL_FTP_CLIENT_READ_CB_EX)(void *ptr, size_t size, size_t nmemb, void *stream);



/*****************************************************************
* Function: ql_ftp_client_new
*
* Description:
* 	创建ftp client 句柄。
* 
* Parameters:
* 	无
*
* Return:
* 	不为 NULL, ftp client 句柄。
* 	NULL, 创建失败。
*
*****************************************************************/
void *ql_ftp_client_new(void);


/*****************************************************************
* Function: ql_ftp_client_release
*
* Description:
* 	释放ftp client资源。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
*
* Return:
* 	void。
*
*****************************************************************/
void ql_ftp_client_release(void *client);


/*****************************************************************
* Function: ql_ftp_client_setopt
*
* Description:
* 	配置ftp client相关属性。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
* 	tag	  			[in] 	属性标签，见QL_FTP_CLIENT_OPT_E。
* 	...	  			[in] 	属性参数。
*							tag为QL_FTP_CLIENT_OPT_PDP_CID是，此参数为int型，执行数据拨号操作时使用的CID值。
*							tag为QL_FTP_CLIENT_OPT_SSL_CTX是，此参数为指针，SSL 句柄。
*
* Return:
* 	0，成功。
*  	其他，错误码。
*
*****************************************************************/
int ql_ftp_client_setopt(void *client, QL_FTP_CLIENT_OPT_E tag, ...);


/*****************************************************************
* Function: ql_ftp_client_open
*
* Description:
* 	连接ftp服务器。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
* 	hostname	  	[in] 	ftp 服务器地址。
* 	username	  	[in] 	用户名。
* 	password	  	[in] 	密码。
*
* Return:
* 	0，成功。
*  	其他，错误码。
*
*****************************************************************/
int ql_ftp_client_open(void *client,char *hostname,char *username,char *password);


/*****************************************************************
* Function: ql_ftp_client_close
*
* Description:
* 	断开与ftp服务器的连接。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
*
* Return:
* 	0，成功。
*  	其他，错误码。
*
*****************************************************************/
int ql_ftp_client_close(void *client);


/*****************************************************************
* Function: ql_ftp_client_get_ex
*
* Description:
* 	从ftp服务器下载文件。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
* 	remotefile	  	[in] 	需要获取的ftp服务器上文件的文件名，可以包含相对或绝对路径。
* 	localfile	  	[in] 	存储到本地的文件名，可以包含相对或绝对路径，如果为NULL，则和ftp服务器上文件同名，本地默认使用的目录为U:/。
* 	write_cb	  	[in] 	使用回调函数接收从FTP服务器获取的文件数据。
* 	user_data	  	[in] 	用户数据。
*
* Return:
* 	0，成功。
*  	其他，错误码。
*
*****************************************************************/
int ql_ftp_client_get_ex(void *client,char *remotefile,char *localfile,QL_FTP_CLIENT_WRITE_CB_EX write_cb,void* user_data);
/*****************************************************************
* Function: ql_ftp_client_put_ex
*
* Description:
* 	上传本地文件到服务器。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
* 	localfile	  	[in] 	需要上传的本地文件名，可以包含相对或绝对路径，本地默认使用的目录为U:/。
* 	remotefile	  	[in] 	存储到ftp服务器上文件的文件名，可以包含相对或绝对路径，如果为NULL，则和本地文件同名，
* 	read_cb	  		[in] 	使用回调函数发送数据到FTP服务器，如果设置了此回调函数，localfile参数无效，remotefile参数必须设置。
* 	user_data	  	[in] 	用户数据。
*
* Return:
* 	0，成功。
*  	其他，错误码。
*
*****************************************************************/
int ql_ftp_client_put_ex(void *client,char *localfile,char *remotefile,QL_FTP_CLIENT_READ_CB_EX read_cb,void* user_data);


/*****************************************************************
* Function: ql_ftp_client_delete
*
* Description:
* 	删除ftp服务器上文件。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
* 	remotefile	  	[in] 	需要删除的ftp服务器上文件的文件名，可以包含相对或绝对路径，
*
* Return:
* 	0，成功。
*  	其他，错误码。
*
*****************************************************************/
int ql_ftp_client_delete(void *client,char *remotefile);


/*****************************************************************
* Function: ql_ftp_client_cwd
*
* Description:
* 	更改ftp服务器当前使用的目录。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
* 	path	  		[in] 	ftp服务器目录，可以是相对或绝对路径，
*
* Return:
* 	0，成功。
*  	其他，错误码。
*
*****************************************************************/
int ql_ftp_client_cwd(void *client, char *path);


/*****************************************************************
* Function: ql_ftp_client_pwd
*
* Description:
* 	获取ftp服务器当前使用的目录。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
* 	path	  		[out] 	ftp服务器目录，绝对路径。
*	path_len		[in]	path的最大存储长度。
*
* Return:
* 	0，成功。
*  	其他，错误码。
*
*****************************************************************/
int ql_ftp_client_pwd(void *client, char *path, int path_len);


/*****************************************************************
* Function: ql_ftp_client_mkdir
*
* Description:
* 	在ftp服务器上新建目录。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
* 	path	  		[in] 	需要新建的ftp服务器目录，可以是相对或绝对路径。
*
* Return:
* 	0，成功。
*  	其他，错误码。
*
*****************************************************************/
int ql_ftp_client_mkdir(void *client, char *path);


/*****************************************************************
* Function: ql_ftp_client_rmdir
*
* Description:
* 	删除ftp服务器上的目录。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
* 	path	  		[in] 	需要删除的ftp服务器目录，可以是相对或绝对路径。
*
* Return:
* 	0，成功。
*  	其他，错误码。
*
*****************************************************************/
int ql_ftp_client_rmdir(void *client, char *path);


/*****************************************************************
* Function: ql_ftp_client_list
*
* Description:
* 	获取指定ftp服务器上目录下文件和目录列表。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
* 	path_or_file	[in] 	ftp服务器上的目录或文件，可以是相对或绝对路径。
* 	match			[in] 	只列出包含match字符的结果。
* 	file_info_list	[out] 	获取到的文件和目录列表。
* 	max_file_num	[in] 	file_info_list可以存储结果的最大个数。
*
* Return:
* 	大于0，实际获取到的结果个数。
*  	-1，失败。
*
*****************************************************************/
int ql_ftp_client_list(void *client, char *path_or_file,char *match,QL_FTP_CLIENT_FILE_INFO_T *file_info_list, int max_file_num);

/*****************************************************************
* Function: ql_ftp_client_size
*
* Description:
* 	获取FTP服务器上指定路径下指定文件的大小。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
*   filename        [in]    FTP服务器上的目录或文件，可以是相对路径或绝对路径。
*   nSize           [out]   用于获取FTP服务器上所要查询指定文件的大小。
*
* Return:
* 	0，成功。
*   其他，错误码。
*
*****************************************************************/
int ql_ftp_client_size(void *client, const char *filename, double *nSize);

/*****************************************************************
* Function: ql_ftp_client_rename
*
* Description:
* 	对ftp服务器上的文件重命名。
* 
* Parameters:
* 	client	  		[in] 	ftp client 句柄。
*   oldname         [in]    FTP服务器上要重命名的文件名。
*   newname         [in]    重命名后的文件名。
*
* Return:
* 	0，成功。
*   其他，错误码。
*
*****************************************************************/
int ql_ftp_client_rename(void *client, const char *oldname, const char *newname);

#ifdef __cplusplus
}
#endif

#endif
