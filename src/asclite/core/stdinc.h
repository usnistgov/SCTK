/*
 * asclite
 * Author: Jerome Ajot, Nicolas Radde, Chris Laprun
 *
 * This software was developed at the National Institute of Standards and Technology by
 * employees of the Federal Government in the course of their official duties.  Pursuant to
 * Title 17 Section 105 of the United States Code this software is not subject to copyright
 * protection within the United States and is in the public domain. asclite is
 * an experimental system.  NIST assumes no responsibility whatsoever for its use by any party.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS."  With regard to this software, NIST MAKES NO EXPRESS
 * OR IMPLIED WARRANTY AS TO ANY MATTER WHATSOEVER, INCLUDING MERCHANTABILITY,
 * OR FITNESS FOR A PARTICULAR PURPOSE.
 */
	
#ifndef STDINC_H
#define STDINC_H

#include <string.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <list>
#include <vector>
#include <set>
#include <assert.h>
#include <sstream>
#include <float.h>
#include <limits.h>

#include "ucl/ucl.h"

using namespace std;

typedef unsigned int uint;
typedef unsigned long int ulint;
typedef unsigned long ulong;

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 255
#endif

#ifndef INT_MAX2
#define INT_MAX2 INT_MAX/2
#endif

#ifndef M_LOG2E
#define M_LOG2E (double) 1.4426950408889634074
#endif

#ifndef F_ROUND
#define F_ROUND(_n,_p)    (((double)((int)((_n) * pow(10.0,(double)(_p)) + ((_n>0.0)?0.5:-0.5)))) / pow(10.0,(double)(_p)))
#endif

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>

#ifndef __GNUC__
#define EPOCHFILETIME (116444736000000000i64)
#else
#define EPOCHFILETIME (116444736000000000LL)
#endif

struct timeval
{
    long tv_sec;   /* seconds */
    long tv_usec;  /* microseconds */
};

struct timezone
{
    int tz_minuteswest; /* minutes W of Greenwich */
    int tz_dsttime;     /* type of dst correction */
};

__inline int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    FILETIME        ft;
    LARGE_INTEGER   li;
    __int64         t;
    static int      tzflag;

    if (tv)
    {
        GetSystemTimeAsFileTime(&ft);
        li.LowPart  = ft.dwLowDateTime;
        li.HighPart = ft.dwHighDateTime;
        t  = li.QuadPart;       /* In 100-nanosecond intervals */
        t -= EPOCHFILETIME;     /* Offset to the Epoch time */
        t /= 10;                /* In microseconds */
        tv->tv_sec  = (long)(t / 1000000);
        tv->tv_usec = (long)(t % 1000000);
    }

    if (tz)
    {
        if (!tzflag)
        {
            _tzset();
            ++tzflag;
        }
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
    }

    return 0;
}

#else  /* _WIN32 */

#include <sys/time.h>

#endif /* _WIN32 */

#endif
