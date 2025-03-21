/*
 * Tencent is pleased to support the open source community by making IoT Hub
 available.
 * Copyright (C) 2018-2020 THL A29 Limited, a Tencent company. All rights
 reserved.

 * Licensed under the MIT License (the "License"); you may not use this file
 except in
 * compliance with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/MIT

 * Unless required by applicable law or agreed to in writing, software
 distributed under the License is
 * distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 KIND,
 * either express or implied. See the License for the specific language
 governing permissions and
 * limitations under the License.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
//#include "utlTime.h"

#include "qcloud_iot_import.h"
#include "ql_api_osi.h"
#include "log_upload.h"


#ifdef timerclear
#undef timerclear
#endif

#ifdef timerisset
#undef timerisset
#endif

#ifdef timercmp
#undef timercmp
#endif

#ifdef timeradd
#undef timeradd
#endif

#ifdef timersub
#undef timersub
#endif

#define	timerclear(tvp)		((tvp)->tv_sec = (tvp)->tv_usec = 0)
#define	timerisset(tvp)		((tvp)->tv_sec || (tvp)->tv_usec)
#define	timercmp(tvp, uvp, cmp)					\
	(((tvp)->tv_sec == (uvp)->tv_sec) ?				\
	    ((tvp)->tv_usec cmp (uvp)->tv_usec) :			\
	    ((tvp)->tv_sec cmp (uvp)->tv_sec))
#define	timeradd(tvp, uvp, vvp)						\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;	\
		if ((vvp)->tv_usec >= 1000000) {			\
			(vvp)->tv_sec++;				\
			(vvp)->tv_usec -= 1000000;			\
		}							\
	} while (0)
#define	timersub(tvp, uvp, vvp)						\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;	\
		if ((vvp)->tv_usec < 0) {				\
			(vvp)->tv_sec--;				\
			(vvp)->tv_usec += 1000000;			\
		}							\
	} while (0)

bool HAL_Timer_expired(Timer *timer)
{
    ql_timeval_t temptime;
    struct timeval now, res;
    ql_gettimeofday(&temptime);
    now.tv_sec=temptime.sec;
    now.tv_usec=temptime.usec;
    timersub(&timer->end_time, &now, &res);
    return res.tv_sec < 0 || (res.tv_sec == 0 && res.tv_usec <= 0);
}

void HAL_Timer_countdown_ms(Timer *timer, unsigned int timeout_ms)
{
    ql_timeval_t temptime;
    struct timeval now;
    ql_gettimeofday(&temptime);
    now.tv_sec=temptime.sec;
    now.tv_usec=temptime.usec;
    struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
    timeradd(&now, &interval, &timer->end_time);
}

void HAL_Timer_countdown(Timer *timer, unsigned int timeout)
{
    ql_timeval_t temptime;
    ql_gettimeofday(&temptime);
    struct timeval now;
    now.tv_sec=temptime.sec;
    now.tv_usec=temptime.usec;
    struct timeval interval = {timeout, 0};
    timeradd(&now, &interval, &timer->end_time);
}

int HAL_Timer_remain(Timer *timer)
{
    ql_timeval_t temptime;
    struct timeval now,res;
    ql_gettimeofday(&temptime);    
    now.tv_sec=temptime.sec;
    now.tv_usec=temptime.usec;
    timersub(&timer->end_time, &now, &res);
    return (res.tv_sec < 0) ? 0 : res.tv_sec * 1000 + res.tv_usec / 1000;
}

void HAL_Timer_init(Timer *timer)
{
    timer->end_time = (struct timeval){0, 0};
}

char *HAL_Timer_current(char *time_str)
{
    if (time_str == NULL)
        return " ";
    ql_timeval_t temptime;
    struct timeval tv;
    ql_gettimeofday(&temptime);
    tv.tv_sec=temptime.sec;
    tv.tv_usec=temptime.usec;
    time_t now_time = tv.tv_sec;

    struct tm tm_tmp = *localtime(&now_time);
    strftime(time_str, TIME_FORMAT_STR_LEN, "%F %T", &tm_tmp);

    return time_str;
}

long HAL_Timer_current_sec(void)
{
    ql_timeval_t tv;
    ql_gettimeofday(&tv);

    return tv.sec;
}

#ifdef __cplusplus
}
#endif
