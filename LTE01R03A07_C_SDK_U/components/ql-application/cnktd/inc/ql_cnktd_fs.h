/**  @file
  ql_cnktd_fs.h

  @brief
  This file is used to define cnktd fs for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_CNKTD_FS_H
#define QL_CNKTD_FS_H

#include "ql_cnktd.h"
#include "ql_fs.h"


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define	_FOPEN		(-1)	/* from sys/file.h, kernel use only */
#define	_FREAD		0x0001	/* read enabled */
#define	_FWRITE		0x0002	/* write enabled */
#define	_FAPPEND	0x0008	/* append (writes guaranteed at the end) */
#define	_FMARK		0x0010	/* internal; mark during gc() */
#define	_FDEFER		0x0020	/* internal; defer for next gc pass */
#define	_FASYNC		0x0040	/* signal pgrp when data ready */
#define	_FSHLOCK	0x0080	/* BSD flock() shared lock present */
#define	_FEXLOCK	0x0100	/* BSD flock() exclusive lock present */
#define	_FCREAT		0x0200	/* open with file create */
#define	_FTRUNC		0x0400	/* open with truncation */
#define	_FEXCL		0x0800	/* error on open if file exists */
#define	_FNBIO		0x1000	/* non blocking I/O (sys5 style) */
#define	_FSYNC		0x2000	/* do all writes synchronously */
#define	_FNONBLOCK	0x4000	/* non blocking I/O (POSIX style) */
#define	_FNDELAY	_FNONBLOCK	/* non blocking I/O (4.2 style) */
#define	_FNOCTTY	0x8000	/* don't assign a ctty on this open */
#if defined (__CYGWIN__)
#define	_FBINARY	0x10000
#define	_FTEXT		0x20000
#endif
#define	_FNOINHERIT	0x40000
#define	_FDIRECT	0x80000
#define	_FNOFOLLOW	0x100000
#define	_FDIRECTORY	0x200000
#define	_FEXECSRCH	0x400000
#if defined (__CYGWIN__)
#define	_FTMPFILE	0x800000
#define	_FNOATIME	0x1000000
#endif

/*
 * Flag values for open(2) and fcntl(2)
 * The kernel adds 1 to the open modes to turn it into some
 * combination of FREAD and FWRITE.
 */
#define	O_RDONLY	0		/* +1 == FREAD */
#define	O_WRONLY	1		/* +1 == FWRITE */
#define	O_RDWR		2		/* +1 == FREAD|FWRITE */
#define	O_APPEND	_FAPPEND
#define	O_CREAT		_FCREAT
#define	O_TRUNC		_FTRUNC
#define	O_EXCL		_FEXCL
#define O_SYNC		_FSYNC
/*	O_NDELAY	_FNDELAY 	set in include/fcntl.h */
/*	O_NDELAY	_FNBIO 		set in include/fcntl.h */
#define	O_NONBLOCK	_FNONBLOCK
#define	O_NOCTTY	_FNOCTTY

typedef int32              qapi_FS_Status_t;
typedef long long          qapi_FS_Offset_t;
typedef uint16             qapi_FS_Mode_t;


/** Statistics type, used in the qapi_FS_Stat() API. */
 struct qapi_FS_Stat_Type_s {
  uint16          st_dev;       /**< Unique device ID among the mounted file
                                     systems. */
  uint32          st_ino;       /**< INode number associated with the file. */
  uint16          st_Mode;      /**< Mode associated with the file. */
  uint8           st_nlink;     /**< Number of active links that are
                                     referencing this file. The	space occupied
                                     by the file is released after its
                                     references are reduced to 0. */
  uint32          st_size;      /**< File size in bytes. */
  unsigned long   st_blksize;   /**< Block size; smallest allocation unit of
                                     the file system. The unit in which
                                     the block Count is represented. */
  unsigned long   st_blocks;    /**< Number of blocks allocated for this file
                                     in st_blksize units. */
  uint32          st_atime;     /**< Last access time. This is not updated, so 
                                     it might have an incorrect value. */
  uint32          st_mtime;     /**< Last modification time. Currently, this 
                                     indicates the time when the file was
                                     created. */
  uint32          st_ctime;     /**< Last status change time. Currently, this
                                     indicates the time when the file was
                                     created. */
  uint32          st_rdev;      /**< Major and minor device number for special
                                     device files. */
  uint16          st_uid;       /**< Owner or user ID of the file. */
  uint16          st_gid;       /**< Group ID of the file. The stored file data
                                     blocks are charged to the quota of this
                                     group ID. */
};

/*========================================================================
 *  function Definition
 *========================================================================*/

/**
 *  @brief  Returns the statistics of a file.
 *
 *  @param[in] Path  File descriptor of the file.
 *
 *  @param[out] SBuf  For information on what is returned in the structure, 
 *                    see struct #qapi_FS_Stat_Type_s.
 *
 *  @return  Returns QAPI_OK on success and -ve error code is returned on failure.
 *
 *
 *  @note1hang If this API is called from user space with MMU enabled,
 *             the parameters must be user space addresses, otherwise
 *             an invalid parameter error will be returned.
 */
qapi_FS_Status_t qapi_FS_Stat(const char* Path, 
                              struct qapi_FS_Stat_Type_s* SBuf);

/*============================================================================
              qapi_FS_Seek
============================================================================*/
/**
*  @brief Changes the file offset for the opened file descriptor.
*
*  @details Changing the file offset does not modify the file. If you lseek 
*           past the end of the file and then write, the gap will be filled  
*           with zero bytes. This gap may not actually allocate space. Using  
*           this API file can be seeked up to (4 GB -1) offset.
*
*  @param[in] Fd  File descriptor obtained via the qapi_FS_Open() API.
*  @param[in] Offset  New offset of the file.
*  @param[in] Whence  Indicates how the new offset is computed: \n
*                      QAPI_FS_SEEK_SET_E -- Set to Offset. \n
*                      QAPI_FS_SEEK_CUR_E -- Set to Offset + current position. \n
*                      QAPI_FS_SEEK_END_E -- Set to Offset + file size.
*  @param[out] Actual_Offset_Ptr  Upon success, the resulting offset as bytes from 
*               the beginning of the file is filled in this parameter.
*               On failure, the variable is set to -1. 
*
*  @return  Returns QAPI_OK on success and -ve error code is returned on failure.
*  - QAPI_ERR_EINVAL -- Invalid operation.
*  - QAPI_ERR_EBADF -- File descriptor was not found.
*  - QAPI_ERR_ESPIPE -- Some file descriptors (like pipes and FIFOs) are not seekable.
*
*  @note1hang If this API is called from user space with MMU enabled,
*             the parameters must be user space addresses, otherwise
*             an invalid parameter error will be returned.
*/
qapi_FS_Status_t qapi_FS_Seek(int Fd,
                            qapi_FS_Offset_t Offset,
                            int Whence,
                            qapi_FS_Offset_t* Actual_Offset_Ptr);


/*============================================================================
            qapi_FS_Read
============================================================================*/
/**
*  @brief Attempts to read Count bytes of data from the file associated with
*         the specified file descriptor.
*
*  @details Zero is a valid result and generally indicates that the end of 
*           the file has been reached. It is permitted for qapi_FS_Read to 
*           return fewer bytes than were requested, even if the data is
*           available in the file.
*
*  @param[in] Fd  File descriptor obtained via the qapi_FS_Open() function.
*  @param[out] Buf  Buffer where the read bytes from the file will be 
*                   stored.
*  @param[in] Count  Number of bytes to read from the file.
*  @param[out] Bytes_Read_Ptr  This is a return from the function with the 
*                              number of bytes actually read.
*
*  @return Returns QAPI_OK on success, and -ve error code is returned on failure.
*
*  @note1hang If this API is called from user space with MMU enabled,
*             the parameters must be user space addresses, otherwise
*             an invalid parameter error will be returned.
*/
qapi_FS_Status_t qapi_FS_Read(int Fd, 
                          uint8* Buf, 
                          uint32 Count,
                          uint32* Bytes_Read_Ptr);


/*============================================================================
              qapi_FS_Mk_Dir
============================================================================*/
/**
*  @brief Creates a new directory.
*
* @param[in] Path  Path for the directory.
* @param[in] Mode  Permission bits of the new directory. See the 
*                  qapi_FS_Open() API for information on Mode bits.
*
*  @return  Returns QAPI_OK on success and -ve error code is returned on failure.
*  - QAPI_ERR_ENOENT -- No such Path was found.
*  - QAPI_ERR_EINVAL -- Invalid operation or parameter.
*  - QAPI_ERR_ENOSPC -- The operation could not be completed because the device is full.
*  - QAPI_ERR_ENAMETOOLONG -- File name or directory name exceeded the NAME_MAX
*                           limit, or the path name exceeded the Path_MAX limit.
*                           The maximum length of a full path name, not including a 
*                           trailing '\0' character: Path_MAX = 1024.
*/
qapi_FS_Status_t qapi_FS_Mk_Dir(const char* Path, 
                              qapi_FS_Mode_t Mode);

/*
@func
qapi_QT_DAM_CFG_Partition_Read.
@brief
Read data from DAM CFG partition.
@param[in]
data :Need to be read from the flash buffer. 
@param[in]
len :Length of data read. 
@param[out]
len :Actual length of data read.  
@return One of the following error codes:
      QAPI_QT_ERR_OK  -- OK.
      Other error codes -- Failure.
*/
qapi_Status_t qapi_QT_DAM_CFG_Partition_Read(void *data, int len);

/*
@func
  qapi_QT_DAM_CFG_Partition_Write.
@brief
  Write data to DAM CFG partition.
@param[in]
  data :Need to be written to the flash buffer. 
@param[in]
  len :Length of data written. 
@return One of the following error codes:
  		QAPI_QT_ERR_OK  -- OK.
  		Other error codes -- Failure.
*/
qapi_Status_t qapi_QT_DAM_CFG_Partition_Write(void *data, int len);

/*============================================================================
                qapi_FS_Open
============================================================================*/
/**
 *  @brief   Opens a file as per the specified Oflag.
 *
 *  @details The parameters, error codes, and return types are the same as in
 *           the API qapi_FS_Open_With_Mode(). This function does not require
 *           the mode as an input argument. It opens the file in Default mode,
 *           which gives read and write permissions to the user, but not
 *           execute permissions.
 *
 *  @param[in] Path   Path of the file that is to be opened.
 *  @param[in] Oflag  Argument that describes how this file should
 *                    be opened. See qapi_FS_Open_With_Mode().
 *  @param[out] Fd_ptr  Address of the file descriptor variable.
 *                      On success, the file descriptor of an opened file is
 *                      written to it. On failure, the variable is set to -1.
 *
 * @return
 * See qapi_FS_Open_With_Mode().
 *
 * @note1hang If this API is called from user space with MMU enabled,
 *            the parameters must be user space addresses, otherwise
 *            an invalid parameter error will be returned.
 */
qapi_FS_Status_t qapi_FS_Open(const char* Path, 
                              int Oflag,
                              int* Fd_ptr);

/*============================================================================
              qapi_FS_Close
============================================================================*/
/**
*  @brief  Closes the file descriptor.
*
*  @details The descriptor will no longer refer to any file and
*            will be allocated to subsequent calls to qapi_FS_Open().
*
*  @param[in] Fd  File descriptor obtained via the qapi_FS_Open() function.
*
*  @return Returns QAPI_OK on success and -ve erro code is returned on failure.
*/
qapi_FS_Status_t qapi_FS_Close(int Fd);

/*============================================================================
                qapi_FS_Write
============================================================================*/
/**
 *  @brief Attempts to write 'Count' bytes of data to the file associated with
 *         the specified file descriptor.
 *
 *  @details The write ioperation may happen at the current file pointer or 
 *           at the end of the file if the file is opened with QAPI_FS_O_APPEND.
 *           It is permitted for qapi_FS_Write to write fewer bytes than were 
 *           requested, even if space is available. If the number of bytes written 
 *           is zero, it indicates that the file system is full (writing), which 
 *           will result in an QAPI_ERR_ENOSPC error.
 *
 *  @param[in] Fd  File descriptor obtained via the qapi_FS_Open() function.
 *
 *  @param[in] Buf  Buffer to which the file is to be written.
 *
 *  @param[in] Count  Number of bytes to write to the file.
 *
 *  @param[out] Bytes_Written_Ptr  This is a return from the function 
 *                                 with the number of bytes actually written.
 *
 *  @return Returns QAPI_OK on success and -ve error code is returned on failure.
 *
 *  @note1hang If this API is called from user space with MMU enabled,
 *             the parameters must be user space addresses, otherwise
 *             an invalid parameter error will be returned.
 */
qapi_FS_Status_t qapi_FS_Write(int Fd, 
                               const uint8* Buf,
                               uint32 Count,
                               uint32* Bytes_Written_Ptr);

/*============================================================================
               qapi_FS_Truncate
============================================================================*/
/**
* @brief Truncates a file to a specified length.
*
* @note1hang
*    If the supplied length is greater than the current file size, it
*    depends on the underlying file system to determine whether the file
*    can grow in size.
*
* @param[in] Path    Path of the file whose length is to be truncated.
* @param[in] Length  New size of the file. The length is in the range
*           (-4 * 1024 * 1024 * 1024, + 4 * 1024 * 1024 * 1024 -1) bytes.
*
*  @return Returns QAPI_OK on success and -ve error code is returned on failure.
*  - QAPI_ERR_EINVAL -- Truncation is not possible. Invalid operation or parameter.
*  - QAPI_ERR_ENOENT -- A file with the specified path was not found.
*  - QAPI_ERR_ENODEV -- The device does not exist.
*  - QAPI_ERR_ENAMETOOLONG -- File-name or directory name exceeded the QAPI_FS_NAME_MAX  
*                           limit, or the path name exceeded the Path_MAX limit.
*                           The maximum length of a full path name, not including a 
*                           trailing '\0' character: Path_MAX = 1024.
*  - QAPI_ERR_EEOF -- Truncation is not allowed beyond End of File (EOF) on this  
*                     file system.
*/
qapi_FS_Status_t qapi_FS_Truncate(const char* Path, qapi_FS_Offset_t Length);

/*============================================================================
                qapi_FS_Unlink
============================================================================*/
/**
 *   @brief   Removes a link to a closed file.
 *
 *  @details  If the link Count goes to zero, this will also remove the
 *            file. The qapi_FS_Unlink() API can be used
 *            on all file system objects except for directories.
 *            Use qapi_FS_Rm_Dir() for directories.
 *
 *  @note1hang The file must be closed for unlinking or removing. If it is open,
 *             the error QAPI_ERR_ETXTBSY is returned, indicating that the file
 *             is not closed.
 *
 * @param[in] Path  File to which the link is to be removed.
 *
 *  @return  Returns QAPI_OK on success and -ve error code is returned on failure.
 *  - QAPI_ERR_ENOENT -- No such path was found.
 *  - QAPI_ERR_EPERM -- Permission is denied.
 *  - QAPI_ERR_ETXTBSY -- The file is in use or open.
 *  - QAPI_ERR_EINVAL -- Invalid parameter.
 *
 */
qapi_FS_Status_t qapi_FS_Unlink(const char* Path);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_CNKTD_FS_H */


