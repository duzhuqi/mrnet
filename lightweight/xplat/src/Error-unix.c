#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "xplat/Error.h"

char* Error_GetErrorString(int code)
{
  return strerror(code);
}

int Error_ETimedOut(int code)
{
  return (code == ETIMEDOUT);
}

int Error_EAddrInUse(int code)
{
  return (code == EADDRINUSE);
}

int Error_EConnRefused(int code)
{
  return (code == ECONNREFUSED);
}