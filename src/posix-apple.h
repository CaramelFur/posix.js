#pragma once
#include <errno.h>

int swapon(const char *path, int flags)
{
  errno = ENOSYS;
  return -1;
}
int swapoff(const char *path)
{
  errno = ENOSYS;
  return -1;
}
