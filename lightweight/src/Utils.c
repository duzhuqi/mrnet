/****************************************************************************
 *  Copyright 2003-2009 Dorian C. Arnold, Philip C. Roth, Barton P. Miller  *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mrnet/Network.h"
#include "Utils.h"

#include "xplat/NetUtils.h"
#include "xplat/Error.h"
#include "xplat/PathUtils.h"

#if !defined(os_windows)
#include <sys/time.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#endif // defined(os_windows)

#if defined(os_solaris)
#include <sys/sockio.h> // only for solaris
#endif //defined(os_solaris)

Rank myrank = (Rank)-1;

struct hostent * copy_hostent (struct hostent *in)
{
  struct hostent * out;
  unsigned int i = 0;
  unsigned int count = 0;
  char** tmp;
  char tmp2[4];

  // copy h_name, h_addrtype, and h_length
  out->h_name = strdup(in->h_name);
  out->h_addrtype = in->h_addrtype;
  out->h_length = in->h_length;

  // deep copy h_aliases
  while (in->h_aliases[count] != NULL)
    count++;

  //char* tmp[count+1];
  tmp = (char**)malloc(sizeof(char)*(count+1));
  out->h_aliases = tmp;
  for (i=0; i < count; i++) {
    out->h_aliases[i] = strdup(in->h_aliases[i]);
  }
  out->h_aliases[count]=NULL;

  // deep copy h_addr_list
  count = 0;
  while (in->h_addr_list[count] != 0)
    count++;

  out->h_addr_list = tmp;
  for (i = 0; i < count; i++) {
    out->h_addr_list[i] = tmp2;
    out->h_addr_list[i][0] = in->h_addr_list[i][0];
    out->h_addr_list[i][1] = in->h_addr_list[i][1];
    out->h_addr_list[i][2] = in->h_addr_list[i][2];
    out->h_addr_list[i][3] = in->h_addr_list[i][3];
  }
  out->h_addr_list[count] = NULL;
  return out;
}

void delete_hostent(struct hostent *in)
{
    //TODO
#if 0 
    free(in->h_name);


    unsigned int count=0;
    while(in->h_aliases[count] != NULL)
        free(in->h_aliases[count++]);
    
    free(in->h_aliases);

    
    count = 0;
    while (in->h_addr_list[count] != NULL)
        free(in->h_addr_list[count++]);
    

    free(in->h_addr_list);
    
    
    free(in);
#endif
}

struct hostent * mrnet_gethostbyname(const char* name)
{

  struct hostent * temp_hostent = gethostbyname(name);
  struct hostent * ret_hostent;

  if (temp_hostent == NULL) {
    return NULL;
  }
  ret_hostent = copy_hostent(temp_hostent);

  return ret_hostent;
}

int connectHost ( int *sock_in, /*const*/ char* hostname, 
                  Port port, int num_retry)
{
  int err, sock = *sock_in;
  struct sockaddr_in server_addr;
  struct hostent *server_hostent = NULL;
  const char* host = hostname;
  unsigned int nConnectTries = 0;
  int cret = -1;
  int optVal;
  int ssoret;

  mrn_dbg(3, mrn_printf(FLF, stderr, "In connectHost(%s:%d) sock:%d..\n",
                        host, port, sock));

  if (sock <= 0) {
    sock = socket ( AF_INET, SOCK_STREAM, 0 );
    if ( sock == -1 ) {
      err = NetUtils_GetLastError();
      perror( "socket()" );
      mrn_dbg(1, mrn_printf(FLF, stderr, "socket() failed, errno=%d\n", err));
      return -1;
    }
    mrn_dbg(5, mrn_printf(FLF, stderr, "socket() => %d\n", sock));
  }

  server_hostent = gethostbyname( host ); 
  if ( server_hostent == NULL ) {
    perror ( "gethostbyname()" );
    return -1;
  } 

  memset( &server_addr, 0, sizeof( server_addr ));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons (port);
  memcpy (&server_addr.sin_addr, server_hostent->h_addr_list[0],
          sizeof (struct in_addr ));
  mrn_dbg(5, mrn_printf(FLF, stderr, "about to call delete_hostent\n"));
  delete_hostent ( server_hostent );
  mrn_dbg(5, mrn_printf(FLF, stderr, "returned from delete_hostent\n"));

  do {
    cret = connect (sock, (struct sockaddr *) & server_addr, sizeof (server_addr));
    if (cret == -1 ) {
      err = NetUtils_GetLastError(); 
      mrn_dbg(5, mrn_printf(FLF, stderr, "connectHost: connect() failed, err=%d\n", err));
      if (!(Error_ETimedOut(err) || Error_EConnRefused(err))) {
        mrn_dbg(1, mrn_printf(FLF, stderr, "connect() to %s:%d failed: %s\n", host, port, Error_GetErrorString(err)));
        return -1;
      }

      nConnectTries++;
      mrn_dbg(3, mrn_printf(FLF, stderr, "connect() to %s:%d timed out %d times\n", host, port, nConnectTries));
      if ((num_retry > 0 ) && (nConnectTries >= num_retry ))
        break;

      // delay before trying again
      sleep (nConnectTries);
    }
  } while ( cret == -1 );

  if (cret == -1) {
    mrn_dbg(1, mrn_printf(FLF, stderr, "connect() to %s:%d failed: %s\n", host, port, Error_GetErrorString(err)));
    return -1;
  }

  mrn_dbg(5, mrn_printf(FLF, stderr, "connectHost: connected!\n"));

#if defined(TCP_NODELAY)
  // turn of Nagle algorithm for coalescing packets
  optVal = 1;
  ssoret = setsockopt (sock,
                             IPPROTO_TCP,
                             TCP_NODELAY,
                             (const char*)&optVal,
                             sizeof( optVal));
  if (ssoret == -1 ) {
    mrn_dbg(1, mrn_printf(FLF, stderr, "failed to set TCP_NODELAY\n"));
  }

#endif // defined (TCP_NODELAY)

  mrn_dbg(3, mrn_printf(FLF, stderr, "Leaving connectHost(). Returning sock: %d\n", sock));

  *sock_in = sock;
  return 0;
}


int mrn_printf( const char *file, int line, const char * func,
                FILE * ifp, const char *format, ... )
{
  static FILE * fp = NULL;
  char *node_type = "be";
  int retval;
  va_list arglist;

  struct timeval tv;
  int rank = getrank();
  FILE * tmp_fp = NULL;
  char* this_host = (char*)malloc(sizeof(char)*256);
  struct stat s;
  char host[256];
  char logdir[256];
  char logfile[512];
  const char* home = getenv("HOME");
  const char* varval = getenv( "MRNET_DEBUG_LOG_DIRECTORY" );
  FILE *f;
  
  while (gettimeofday( &tv, NULL ) == -1 ) {}

  if ( (fp == NULL) && (rank != UnknownRank) ) {
    logfile[0] = '\0';

    NetUtils_GetLocalHostName(this_host);
    strncpy(host, this_host, 256);
    host[255] = '\0';

    // find log directory
    if (varval != NULL) {
      if ( (stat(varval, &s) == 0) && (S_IFDIR & s.st_mode) )
            snprintf( logdir, sizeof(logdir), "%s", varval);
      }
      else if (home != NULL) {
        snprintf( logdir, sizeof(logdir), "%s/mrnet-log", home );
        if(! ((stat(logdir, &s) == 0) && (S_IFDIR & s.st_mode)))
              snprintf( logdir, sizeof(logdir), "/tmp" );
      }
      // set file name format
      snprintf(logfile, sizeof(logfile), "%s/%s_%s_%d",
                logdir, node_type, host, rank );
      tmp_fp = fopen(logfile, "w");
      if (tmp_fp != NULL)
          fp = tmp_fp;
    }
  
  f = fp;
  if (f == NULL)
    f = ifp;

  if (file) {
    
      fprintf( f, "%ld.%ld: %s: %s(): %d: ",
              tv.tv_sec-MRN_RELEASE_DATE_SECS, 
              tv.tv_usec, 
              GetFilename(file),
              func, 
              line);
  }

  va_start(arglist, format);
  retval = vfprintf(f, format, arglist);
  va_end(arglist);
  fflush(f);

  return retval;
}

Timer_t new_Timer_t()
{
  Timer_t time;
  time.offset = 0;
  time.first_time=true;
}

double tv2dbl(struct timeval tv)
{
  return ((double)tv.tv_sec + (double)tv.tv_usec / 1000000.0);
}

struct timeval dbl2tv(double d)
{
  struct timeval tv;

  tv.tv_sec = (long) d;
  tv.tv_usec = (long) ((d - (long) d) * 1000000.0);


  return tv;
}

void Timer_start(Timer_t time) 
{
  while (gettimeofday(&(time.start_tv), NULL) == -1) {}
  time.start_d = tv2dbl(time.start_tv) + (time.offset/1000.0);
  time.start_tv = dbl2tv(time.start_d);
}

void Timer_stop(Timer_t time)
{
  while (gettimeofday(&(time.stop_tv), NULL) == -1) {}
  time.stop_d = tv2dbl(time.stop_tv) + (time.offset/1000.0);
  time.stop_tv = dbl2tv(time.stop_d);
}

double Timer_get_latency_secs(Timer_t time) 
{
  return time.stop_d - time.start_d;
}   

double Timer_get_latency_msecs(Timer_t time)
{
  return 1000 * Timer_get_latency_secs(time);
}

Rank getrank() {return myrank;}

void setrank(Rank ir) {
    myrank=ir;
}
