#pragma once

// Error if building on non-posix system
#if !defined(__unix__) && !defined(__linux__) && !defined(__APPLE__) && !defined(__ANDROID__)
#error "This module is only supported on POSIX systems"
#endif

#ifdef MOCK_POSIX
#include "posix-mock.h"
#endif

#include <unistd.h>
#include <sys/resource.h> // setrlimit, getrlimit
#include <pwd.h>          // getpwnam, passwd
#include <grp.h>          // getgrnam, group
#include <syslog.h>       // openlog, closelog, syslog, setlogmask
#include <sys/swap.h>     // swapon, swapoff
#include <errno.h>


#ifdef MOCK_POSIX
  const auto& posix_getuid = mock_getuid;
  const auto& posix_getgid = mock_getgid;
  const auto& posix_setuid = mock_setuid;
  const auto& posix_setgid = mock_setgid;

  const auto& posix_geteuid = mock_geteuid;
  const auto& posix_getegid = mock_getegid;
  const auto& posix_seteuid = mock_seteuid;
  const auto& posix_setegid = mock_setegid;

  const auto& posix_getpid = mock_getpid;
  const auto& posix_getpgrp = mock_getpgrp;
  const auto& posix_getppid = mock_getppid;
  const auto& posix_getpgid = mock_getpgid;
  const auto& posix_setpgid = mock_setpgid;

  const auto& posix_setsid = mock_setsid;
  const auto& posix_getsid = mock_getsid;

  const auto& posix_chdir = mock_chdir;
  const auto& posix_chroot = mock_chroot;

  const auto& posix_getrlimit = mock_getrlimit;
  const auto& posix_setrlimit = mock_setrlimit;

  const auto& posix_getpwnam = mock_getpwnam;
  const auto& posix_getpwuid = mock_getpwuid;
  const auto& posix_getgrnam = mock_getgrnam;
  const auto& posix_getgrgid = mock_getgrgid;

  const auto& posix_initgroups = mock_initgroups;
  const auto& posix_getgroups = mock_getgroups;
  const auto& posix_setgroups = mock_setgroups;

  const auto& posix_setregid = mock_setregid;
  const auto& posix_setreuid = mock_setreuid;

  const auto& posix_openlog = mock_openlog;
  const auto& posix_closelog = mock_closelog;
  const auto& posix_syslog = mock_syslog;
  const auto& posix_setlogmask = mock_setlogmask;

  const auto& posix_gethostname = mock_gethostname;
  const auto& posix_sethostname = mock_sethostname;

  const auto& posix_swapon = mock_swapon;
  const auto& posix_swapoff = mock_swapoff;
#else
  const auto& posix_getuid = getuid;
  const auto& posix_getgid = getgid;
  const auto& posix_setuid = setuid;
  const auto& posix_setgid = setgid;

  const auto& posix_geteuid = geteuid;
  const auto& posix_getegid = getegid;
  const auto& posix_seteuid = seteuid;
  const auto& posix_setegid = setegid;

  const auto& posix_getpid = getpid;
  const auto& posix_getpgrp = getpgrp;
  const auto& posix_getppid = getppid;
  const auto& posix_getpgid = getpgid;
  const auto& posix_setpgid = setpgid;

  const auto& posix_setsid = setsid;
  const auto& posix_getsid = getsid;

  const auto& posix_chdir = chdir;
  const auto& posix_chroot = chroot;

  const auto& posix_getrlimit = getrlimit;
  const auto& posix_setrlimit = setrlimit;

  const auto& posix_getpwnam = getpwnam;
  const auto& posix_getpwuid = getpwuid;
  const auto& posix_getgrnam = getgrnam;
  const auto& posix_getgrgid = getgrgid;

  const auto& posix_initgroups = initgroups;
  const auto& posix_getgroups = getgroups;
  const auto& posix_setgroups = setgroups;

  const auto& posix_setregid = setregid;
  const auto& posix_setreuid = setreuid;

  const auto& posix_openlog = openlog;
  const auto& posix_closelog = closelog;
  const auto& posix_syslog = syslog;
  const auto& posix_setlogmask = setlogmask;

  const auto& posix_gethostname = gethostname;
  const auto& posix_sethostname = sethostname;

  const auto& posix_swapon = swapon;
  const auto& posix_swapoff = swapoff;
#endif
