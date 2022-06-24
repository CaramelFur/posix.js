#include "posix-mock.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PASSWDS 3
static passwd passwd_list[PASSWDS] = {
    {
        .pw_name = (char *)"root",
        .pw_passwd = (char *)"x",
        .pw_uid = 0,
        .pw_gid = 0,
        .pw_gecos = (char *)"root",
        .pw_dir = (char *)"/root",
        .pw_shell = (char *)"/bin/bash",
    },
    {
        .pw_name = (char *)"user",
        .pw_passwd = (char *)"x",
        .pw_uid = 1000,
        .pw_gid = 1000,
        .pw_gecos = (char *)"user",
        .pw_dir = (char *)"/home/user",
        .pw_shell = (char *)"/bin/bash",
    },
    {
        .pw_name = (char *)"user2",
        .pw_passwd = (char *)"x",
        .pw_uid = 1001,
        .pw_gid = 1000,
        .pw_gecos = (char *)"user2",
        .pw_dir = (char *)"/home/user2",
        .pw_shell = (char *)"/bin/bash",
    },
};

#define GROUPS 2
static group group_list[GROUPS] = {
    {
        .gr_name = (char *)"root",
        .gr_passwd = (char *)"x",
        .gr_gid = 0,
        .gr_mem = (char *[]){
            (char *)"root",
            NULL,
        },
    },
    {
        .gr_name = (char *)"user",
        .gr_passwd = (char *)"x",
        .gr_gid = 1000,
        .gr_mem = (char *[]){
            (char *)"user",
            (char *)"user2",
            NULL,
        },
    },
};

static uid_t current_user;
static gid_t current_group;
static uid_t current_effective_user;
static gid_t current_effective_group;

static pid_t current_pid;
static pid_t current_pid_group;
static pid_t current_parent_pid;
static pid_t current_parent_pid_group;

static pid_t current_session;

static rlimit current_rlimits[RLIMIT_NLIMITS];

void mock_reset()
{
  current_user = 0;
  current_group = 0;
  current_effective_user = 0;
  current_effective_group = 0;

  current_pid = 3044;
  current_pid_group = 3020;
  current_parent_pid = 3011;
  current_parent_pid_group = 3001;

  current_session = 0;

  for (int i = 0; i < RLIMIT_NLIMITS; i++)
  {
    current_rlimits[i].rlim_cur = RLIM_INFINITY;
    current_rlimits[i].rlim_max = RLIM_INFINITY;
  }
}

uid_t mock_getuid()
{
  return current_user;
}
uid_t mock_getgid()
{
  return current_group;
}

int mock_setuid(uid_t uid)
{
  if (current_user != 0 && current_group != 0)
  {
    errno = EPERM;
    return -1;
  }

  // Check if the user exists
  for (int i = 0; i < PASSWDS; i++)
  {
    if (passwd_list[i].pw_uid == uid)
    {
      current_user = uid;
      current_effective_user = uid;
      return 0;
    }
  }

  errno = EINVAL;
  return -1;
}

int mock_setgid(gid_t gid)
{
  if (current_user != 0 && current_group != 0)
  {
    errno = EPERM;
    return -1;
  }

  // Check if the group exists
  for (int i = 0; i < GROUPS; i++)
  {
    if (group_list[i].gr_gid == gid)
    {
      current_group = gid;
      current_effective_group = gid;
      return 0;
    }
  }

  errno = EINVAL;
  return -1;
}

uid_t mock_geteuid()
{
  return current_effective_user;
}
uid_t mock_getegid()
{
  return current_effective_group;
}
int mock_seteuid(uid_t uid)
{
  if (current_user != 0 && current_group != 0)
  {
    errno = EPERM;
    return -1;
  }

  // Check if the user exists
  for (int i = 0; i < PASSWDS; i++)
  {
    if (passwd_list[i].pw_uid == uid)
    {
      current_effective_user = uid;
      return 0;
    }
  }

  errno = EINVAL;
  return -1;
}
int mock_setegid(gid_t gid)
{
  if (current_user != 0 && current_group != 0)
  {
    errno = EPERM;
    return -1;
  }

  // Check if the group exists
  for (int i = 0; i < GROUPS; i++)
  {
    if (group_list[i].gr_gid == gid)
    {
      current_effective_group = gid;
      return 0;
    }
  }

  errno = EINVAL;
  return -1;
}

pid_t mock_getpid()
{
  return current_pid;
}
pid_t mock_getpgrp()
{
  return current_pid_group;
}
pid_t mock_getppid()
{
  return current_parent_pid;
}
pid_t mock_getpgid(pid_t pid)
{
  if (pid == current_pid)
  {
    return current_pid_group;
  }
  else if (pid == current_parent_pid)
  {
    return current_parent_pid_group;
  }
  else
  {
    errno = EINVAL;
    return -1;
  }
}
int mock_setpgid(pid_t pid, pid_t pgid)
{
  if (pid == current_pid)
  {
    current_pid_group = pgid;
    return 0;
  }
  else if (pid == current_parent_pid)
  {
    current_parent_pid_group = pgid;
    return 0;
  }
  else
  {
    errno = EINVAL;
    return -1;
  }
}

pid_t mock_setsid()
{
  if (current_session != 0)
  {
    errno = EPERM;
    return -1;
  }

  current_session = current_pid;
  return current_session;
}

pid_t mock_getsid(pid_t pid)
{
  if (pid == 0)
    pid = current_pid;

  if (pid != current_pid)
  {
    errno = EINVAL;
    return -1;
  }

  if (current_session == 0)
  {
    errno = ESRCH;
    return -1;
  }

  return current_session;
}

int mock_chdir(const char *path)
{
  if (strcmp(path, "/test") == 0)
  {
    return 0;
  }
  else
  {
    errno = ENOTDIR;
    return -1;
  }
}
int mock_chroot(const char *path)
{
  if (strcmp(path, "/test") == 0)
  {
    return 0;
  }
  else
  {
    errno = ENOTDIR;
    return -1;
  }
}

int mock_getrlimit(int resource, rlimit *rlimits) {
  if (resource < 0 || resource >= RLIMIT_NLIMITS) {
    errno = EINVAL;
    return -1;
  }
  
  *rlimits = current_rlimits[resource];
  return 0;
}
int mock_setrlimit(int resource, const rlimit *rlimits) {
  if (resource < 0 || resource >= RLIMIT_NLIMITS) {
    errno = EINVAL;
    return -1;
  }
  
  current_rlimits[resource] = *rlimits;
  return 0;
}

passwd *mock_getpwnam(const char *name)
{
  for (int i = 0; i < PASSWDS; i++)
  {
    if (strcmp(passwd_list[i].pw_name, name) == 0)
    {
      return &passwd_list[i];
    }
  }
  errno = EINVAL;
  return NULL;
}
passwd *mock_getpwuid(uid_t uid)
{
  for (int i = 0; i < PASSWDS; i++)
  {
    if (passwd_list[i].pw_uid == uid)
    {
      return &passwd_list[i];
    }
  }
  errno = EINVAL;
  return NULL;
}
group *mock_getgrnam(const char *name)
{
  for (int i = 0; i < GROUPS; i++)
  {
    if (strcmp(group_list[i].gr_name, name) == 0)
    {
      return &group_list[i];
    }
  }
  errno = EINVAL;
  return NULL;
}
group *mock_getgrgid(gid_t gid)
{
  for (int i = 0; i < GROUPS; i++)
  {
    if (group_list[i].gr_gid == gid)
    {
      return &group_list[i];
    }
  }
  errno = EINVAL;
  return NULL;
}

int mock_initgroups(const char *user, gid_t group) { return 0; }

int mock_setregid(gid_t rgid, gid_t egid) { return 0; }
int mock_setreuid(uid_t ruid, uid_t euid) { return 0; }

void mock_openlog(const char *ident, int option, int facility) { return; }
void mock_closelog() { return; }
void mock_syslog(int priority, const char *format, ...) { return; }
int mock_setlogmask(int mask) { return 0; }

int mock_gethostname(char *name, size_t len) { return 0; }
int mock_sethostname(const char *name, size_t len) { return 0; }

int mock_swapon(const char *path, int flags) { return 0; }
int mock_swapoff(const char *path) { return 0; }
