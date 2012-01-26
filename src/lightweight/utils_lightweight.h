/****************************************************************************
 *  Copyright 2003-2011 Dorian C. Arnold, Philip C. Roth, Barton P. Miller  *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

#if !defined(__utils_lightweight_h)
#define __utils_lightweight_h 1

#if !defined (__STDC_LIMIT_MACROS)
#  define __STDC_LIMIT_MACROS
#endif
#if !defined (__STDC_CONSTANT_MACROS)
#  define __STDC_CONSTANT_MACROS
#endif
#if !defined (__STDC_FORMAT_MACROS)
#  define __STDC_FORMAT_MACROS
#endif

#ifndef UNUSED
#if defined(__GNUC__)
#   define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#else
#   define UNUSED(x) x
#endif
#endif
 
#include <stdio.h>
#include <stdlib.h>

#include "mrnet_lightweight/Types.h"
#include "xplat_lightweight/Types.h"

#ifndef os_windows // unix

# include "config.h"

# ifdef HAVE_SIGNAL_H
#  include <signal.h>
# endif
# ifdef HAVE_UNISTD_H
#  include <unistd.h>
# endif
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# endif

#else // windows

# include <io.h>
# include <sys/timeb.h>
# include <winsock2.h>

# define srand48(x) srand((unsigned int)x)
# define drand48 (double)rand
# define snprintf _snprintf
# define sleep(x) Sleep(1000*(DWORD)x)
# define inline __inline
# define strtok_r strtok_s
# define strdup _strdup

int gettimeofday(struct timeval *tv, struct timezone *tz);

#endif // ifndef os_windows

#define MRN_RELEASE_DATE_SECS 1308200400

struct hostent *copy_hostent (struct hostent *in);
void delete_hostent(struct hostent *in);
struct hostent * mrnet_gethostbyname(const char* name);

int connectHost ( int *sock_in, char* hostname, 
                  Port port, int num_retry);

#if defined(DEBUG_MRNET)
# define _fprintf(X) fprintf X;
# define _perror(X) perror(X);
#else
# define _fprintf(X) ;
# define _perror(X) ;
#endif
                        // defined(DEBUG_MRNET)

extern int CUR_OUTPUT_LEVEL;
#define mrn_dbg( x, y) \
do{ \
    if (CUR_OUTPUT_LEVEL >= x) {      \
        y;                            \
    }                                 \
}while(0);

#define NULL_STRING ""

// FLF is used to call mrn_printf(FLF, ...)
#if !defined( __GNUC__)
#define FLF __FILE__,__LINE__,"unknown"
#else
#define FLF __FILE__,__LINE__,__FUNCTION__
#endif

#define mrn_dbg_func_end()                                    \
do { \
    mrn_dbg(3, mrn_printf(FLF, stderr, "Function exit\n"));   \
}while(0);

#define mrn_dbg_func_begin()                                  \
do { \
    mrn_dbg(3, mrn_printf(FLF, stderr, "Function start ...\n")); \
}while(0);


typedef struct {  
    struct timeval start_tv;
    struct timeval stop_tv;
    double start_d;
    double stop_d;
} Timer_t;

Timer_t new_Timer_t();

double tv2dbl(struct timeval tv);

struct timeval dbl2tv(double d);

void Timer_start(Timer_t time); 

void Timer_stop(Timer_t time);

double Timer_get_latency_secs(Timer_t time); 

double Timer_get_latency_msecs(Timer_t time);

Rank getrank();

void setrank(Rank ir);

int isBigEndian();

/* void endianTest(); */

#endif /* __utils_h */

