#pragma once
#include <errno.h>

#define RLIMIT_NLIMITS RLIM_NLIMITS

#define RLIMIT_SIGPENDING -1
#define RLIMIT_MSGQUEUE -1
#define RLIMIT_NICE -1
#define RLIMIT_RTPRIO -1
#define RLIMIT_RTTIME -1

#define SWAP_FLAG_PREFER -1

inline int apple_swapon(const char *path, int flags)
{
  errno = ENOSYS;
  return -1;
}
inline int apple_swapoff(const char *path)
{
  errno = ENOSYS;
  return -1;
}
