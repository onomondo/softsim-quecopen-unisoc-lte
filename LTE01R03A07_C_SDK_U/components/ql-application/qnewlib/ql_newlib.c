/* system calls for the Visium processor.

   Copyright (c) 2015 Rolls-Royce Controls and Data Services Limited.
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of Rolls-Royce Controls and Data Services Limited nor
       the names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
   THE POSSIBILITY OF SUCH DAMAGE.  */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include "ql_fs.h"
#include "ql_log.h"
#include "ql_api_osi.h"

#define QL_NEWLIB_LOG_LEVEL				QL_LOG_LEVEL_INFO
#define QL_NEWLIB_LOG(msg, ...)			QL_LOG(QL_NEWLIB_LOG_LEVEL, "ql_NEWLIB", msg, ##__VA_ARGS__)
#define QL_NEWLIB_LOG_PUSH(msg, ...)	QL_LOG_PUSH("ql_NEWLIB", msg, ##__VA_ARGS__)

extern QFILE ql_open(const char *file_name, int mode);

int close (int fildes)
{
	return ql_fclose(fildes);
}

int fstat (int fildes, struct stat *st)
{
	return ql_fstat(fildes,st);
}

int gettimeofday (struct timeval *__p, void *__tz)
{
	ql_timeval_t *timeval = (ql_timeval_t *)__p;	
    ql_gettimeofday(timeval);
	return 0;
}

/*如果参数fildes所代表的文件描述词为一终端机则返回1，否则返回0*/
int isatty (int fildes)
{
	return 0;
}

off_t lseek (int fildes, off_t offset, int whence)
{
	return ql_fseek(fildes, offset, whence);
}

int open (const char *path, int oflag, ...)
{ 
	int fd = ql_open(path, oflag);
	QL_NEWLIB_LOG("open %s %d %d",path,oflag,fd);
	return fd;
}

int read (int fildes, void *buf, size_t nbyte)
{
	size_t size = ql_fread(buf, nbyte, 1, fildes);
	QL_NEWLIB_LOG("read %d %d %s %d",fildes,nbyte,buf,size);
	return size;
}

int rename (const char *old, const char *new)
{
	return ql_rename(old,new);
}

int stat (const char *path, struct stat *st)
{
	return ql_stat(path, st);
}

int unlink (const char *path)
{
	return ql_remove(path);
}

int write (int fildes, const void *buf, size_t nbyte)
{
	size_t size = ql_fwrite((void *)buf, nbyte, 1, fildes);
	QL_NEWLIB_LOG("write %d %d %s %d",fildes,nbyte,buf,size);
	return size;
}




