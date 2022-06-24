#include <napi.h>

#include "posix.h"

Napi::Value throw_error(const Napi::CallbackInfo &info, bool typeError, const char *message, int errorno = 0)
{
  size_t length = strlen(message);
  char *error_str = NULL;

  if (errorno != 0)
  {
    error_str = strerror(errorno);
    if (error_str != NULL)
      length += strlen(error_str);
  }

  length += 32;

  char *error_message = new char[length];
  memset(error_message, 0, length);

  strcat(error_message, "[ERROR] ");
  if (error_str != NULL)
  {
    strcat(error_message, "(");
    strcat(error_message, error_str);
    strcat(error_message, ") ");
  }
  strcat(error_message, message);

  Napi::Env env = info.Env();
  if (typeError)
  {
    Napi::TypeError::New(env, error_message).ThrowAsJavaScriptException();
  }
  else
  {
    Napi::Error::New(env, error_message).ThrowAsJavaScriptException();
  }
  return env.Undefined();
}

Napi::Value node_getuid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    return throw_error(info, true, "getuid: takes no arguments");

  return Napi::Number::New(env, posix_getuid());
}

Napi::Value node_getgid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    return throw_error(info, true, "getgid: takes no arguments");

  return Napi::Number::New(env, posix_getgid());
}

Napi::Value node_setuid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    return throw_error(info, true, "setuid: takes 1 argument");

  uid_t uid;

  if (info[0].IsNumber())
  {
    uid = info[0].As<Napi::Number>().Int32Value();
  }
  else if (info[0].IsString())
  {
    errno = 0;
    passwd *pwd = posix_getpwnam(info[0].As<Napi::String>().Utf8Value().c_str());
    if (errno)
      return throw_error(info, false, "setuid: getpwnam failed", errno);
    if (!pwd)
      return throw_error(info, false, "setuid: user not found");
    uid = pwd->pw_uid;
  }
  else
  {
    return throw_error(info, true, "setuid: argument must be a number or a string");
  }

  if (posix_setuid(uid) == -1)
    return throw_error(info, false, "setuid: failed to set uid", errno);

  return env.Undefined();
}

Napi::Value node_setgid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    return throw_error(info, true, "setgid: takes 1 argument");

  gid_t gid;

  if (info[0].IsNumber())
  {
    gid = info[0].As<Napi::Number>().Int32Value();
  }
  else if (info[0].IsString())
  {
    errno = 0;
    group *grp = posix_getgrnam(info[0].As<Napi::String>().Utf8Value().c_str());
    if (errno)
      return throw_error(info, false, "setgid: getgrnam failed", errno);
    if (!grp)
      return throw_error(info, false, "setgid: group not found");
    gid = grp->gr_gid;
  }
  else
  {
    return throw_error(info, true, "setgid: argument must be a number or a string");
  }

  if (posix_setgid(gid) == -1)
    return throw_error(info, false, "setgid: failed to set gid", errno);

  return env.Undefined();
}

Napi::Value node_geteuid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    return throw_error(info, true, "geteuid: takes no arguments");

  return Napi::Number::New(env, posix_geteuid());
}

Napi::Value node_getegid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    return throw_error(info, true, "getegid: takes no arguments");

  return Napi::Number::New(env, posix_getegid());
}

Napi::Value node_seteuid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    return throw_error(info, true, "seteuid: takes 1 argument");

  uid_t uid;

  if (info[0].IsNumber())
  {
    uid = info[0].As<Napi::Number>().Int32Value();
  }
  else if (info[0].IsString())
  {
    errno = 0;
    passwd *pwd = posix_getpwnam(info[0].As<Napi::String>().Utf8Value().c_str());
    if (errno)
      return throw_error(info, false, "seteuid: getpwnam failed", errno);
    if (!pwd)
      return throw_error(info, false, "seteuid: user not found");
    uid = pwd->pw_uid;
  }
  else
  {
    return throw_error(info, true, "seteuid: argument must be a number or a string");
  }

  if (posix_seteuid(uid) == -1)
    return throw_error(info, false, "seteuid: failed to set uid", errno);

  return env.Undefined();
}

Napi::Value node_setegid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    return throw_error(info, true, "setegid: takes 1 argument");

  gid_t gid;

  if (info[0].IsNumber())
  {
    gid = info[0].As<Napi::Number>().Int32Value();
  }
  else if (info[0].IsString())
  {
    errno = 0;
    group *grp = posix_getgrnam(info[0].As<Napi::String>().Utf8Value().c_str());
    if (errno)
      return throw_error(info, false, "setegid: getgrnam failed", errno);
    if (!grp)
      return throw_error(info, false, "setegid: group not found");
    gid = grp->gr_gid;
  }
  else
  {
    return throw_error(info, true, "setegid: argument must be a number or a string");
  }

  if (posix_setegid(gid) == -1)
    return throw_error(info, false, "setegid: failed to set gid", errno);

  return env.Undefined();
}

Napi::Value node_getpid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    return throw_error(info, true, "getpid: takes no arguments");

  return Napi::Number::New(env, posix_getpid());
}

Napi::Value node_getpgrp(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    return throw_error(info, true, "getgrp: takes no arguments");

  return Napi::Number::New(env, posix_getpgrp());
}

Napi::Value node_getppid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    return throw_error(info, true, "getppid: takes no arguments");

  return Napi::Number::New(env, posix_getppid());
}

Napi::Value node_getpgid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    return throw_error(info, true, "getpgid: takes 1 argument");

  if (!info[0].IsNumber())
    return throw_error(info, true, "getpgid: first argument must be a number");

  pid_t pid = posix_getpgid(info[0].As<Napi::Number>().Int32Value());
  if (pid == -1)
    return throw_error(info, false, "getpgid: failed to get pgid", errno);

  return Napi::Number::New(env, pid);
}

Napi::Value node_setpgid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
    return throw_error(info, true, "setpgid: takes 2 arguments");

  if (!info[0].IsNumber())
    return throw_error(info, true, "setpgid: first argument must be a number");

  if (!info[1].IsNumber())
    return throw_error(info, true, "setpgid: second argument must be a number");

  if (posix_setpgid(info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value()))
    return throw_error(info, false, "setpgid: failed", errno);

  return env.Undefined();
}

Napi::Value node_setsid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    return throw_error(info, true, "setsid: takes no arguments");

  pid_t pid = posix_setsid();
  if (pid == -1)
    return throw_error(info, false, "setsid: failed", errno);

  return Napi::Number::New(env, pid);
}

Napi::Value node_getsid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    return throw_error(info, true, "getsid: takes 1 argument");

  if (!info[0].IsNumber())
    return throw_error(info, true, "getsid: first argument must be a number");

  pid_t pid = posix_getsid(info[0].As<Napi::Number>().Int32Value());
  if (pid == -1)
    return throw_error(info, false, "getsid: failed", errno);

  return Napi::Number::New(env, pid);
}

Napi::Value node_chroot(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    return throw_error(info, true, "chroot: takes 1 argument");

  if (!info[0].IsString())
    return throw_error(info, true, "chroot: first argument must be a string");

  const char *path = info[0].As<Napi::String>().Utf8Value().c_str();

  // Proper order is to first chdir() then chroot()
  if (posix_chdir(path))
    return throw_error(info, false, "chroot: failed to change directory", errno);

  if (posix_chroot(path))
    return throw_error(info, false, "chroot: failed", errno);

  return env.Undefined();
}

int rlimit_from_string(const char *rlimit)
{
  if (strcmp(rlimit, "cpu") == 0)
    return RLIMIT_CPU;
  else if (strcmp(rlimit, "fsize") == 0)
    return RLIMIT_FSIZE;
  else if (strcmp(rlimit, "data") == 0)
    return RLIMIT_DATA;
  else if (strcmp(rlimit, "stack") == 0)
    return RLIMIT_STACK;
  else if (strcmp(rlimit, "core") == 0)
    return RLIMIT_CORE;
  else if (strcmp(rlimit, "rss") == 0)
    return RLIMIT_RSS;
  else if (strcmp(rlimit, "nproc") == 0)
    return RLIMIT_NPROC;
  else if (strcmp(rlimit, "nofile") == 0)
    return RLIMIT_NOFILE;
  else if (strcmp(rlimit, "memlock") == 0)
    return RLIMIT_MEMLOCK;
  else if (strcmp(rlimit, "as") == 0)
    return RLIMIT_AS;
  else if (strcmp(rlimit, "locks") == 0)
    return RLIMIT_NLIMITS;
  else if (strcmp(rlimit, "sigpending") == 0)
    return RLIMIT_SIGPENDING;
  else if (strcmp(rlimit, "msgqueue") == 0)
    return RLIMIT_MSGQUEUE;
  else if (strcmp(rlimit, "nice") == 0)
    return RLIMIT_NICE;
  else if (strcmp(rlimit, "rtprio") == 0)
    return RLIMIT_RTPRIO;
  else if (strcmp(rlimit, "rttime") == 0)
    return RLIMIT_RTTIME;
  else
    return -1;
}

Napi::Value node_getrlimit(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    return throw_error(info, true, "getrlimit: takes 1 argument");

  int resource;

  if (info[0].IsNumber())
  {
    resource = info[0].As<Napi::Number>().Int32Value();
  }
  else if (info[0].IsString())
  {
    const char *str = info[0].As<Napi::String>().Utf8Value().c_str();
    resource = rlimit_from_string(str);
  }
  else
  {
    return throw_error(info, true, "setrlimit: first argument must be a number or a string");
  }

  struct rlimit limit;
  if (posix_getrlimit(resource, &limit))
    return throw_error(info, false, "getrlimit: failed", errno);

  Napi::Object obj = Napi::Object::New(env);
  if (limit.rlim_cur == RLIM_INFINITY)
    obj.Set("soft", env.Null());
  else
    obj.Set("soft", Napi::Number::New(env, limit.rlim_cur));

  if (limit.rlim_max == RLIM_INFINITY)
    obj.Set("hard", env.Null());
  else
    obj.Set("hard", Napi::Number::New(env, limit.rlim_max));

  return obj;
}

Napi::Value node_setrlimit(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
    return throw_error(info, true, "setrlimit: takes 2 arguments");

  if (!info[1].IsObject())
    return throw_error(info, true, "setrlimit: second argument must be an object");

  int resource;

  if (info[0].IsNumber())
  {
    resource = info[0].As<Napi::Number>().Int32Value();
  }
  else if (info[0].IsString())
  {
    const char *str = info[0].As<Napi::String>().Utf8Value().c_str();
    resource = rlimit_from_string(str);
  }
  else
  {
    return throw_error(info, true, "setrlimit: first argument must be a number or a string");
  }

  struct rlimit limit;
  // Pre populate with current values
  if (posix_getrlimit(resource, &limit))
    return throw_error(info, false, "setrlimit: failed", errno);

  Napi::Object obj = info[1].As<Napi::Object>();

  if (obj.Has("soft"))
  {
    if (obj.Get("soft").IsNull())
    {
      limit.rlim_cur = RLIM_INFINITY;
    }
    else if (obj.Get("soft").IsNumber())
    {
      limit.rlim_cur = obj.Get("soft").As<Napi::Number>().Int32Value();
    }
    else
    {
      return throw_error(info, true, "setrlimit: second argument soft must be a number or null");
    }
  }

  if (obj.Has("hard"))
  {
    if (obj.Get("hard").IsNull())
    {
      limit.rlim_max = RLIM_INFINITY;
    }
    else if (obj.Get("hard").IsNumber())
    {
      limit.rlim_max = obj.Get("hard").As<Napi::Number>().Int32Value();
    }
    else
    {
      return throw_error(info, true, "setrlimit: second argument hard must be a number or null");
    }
  }

  if (posix_setrlimit(resource, &limit))
    return throw_error(info, false, "setrlimit: failed", errno);

  return env.Undefined();
}

Napi::Value node_getpwnam(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    return throw_error(info, true, "getpwnam: takes 1 argument");

  struct passwd *pwd;

  errno = 0;
  if (info[0].IsString())
    pwd = posix_getpwnam(info[0].As<Napi::String>().Utf8Value().c_str());
  else if (info[0].IsNumber())
    pwd = posix_getpwuid(info[0].As<Napi::Number>().Int32Value());
  else
    return throw_error(info, true, "getpwnam: first argument must be a string or number");

  if (errno)
    return throw_error(info, false, "getpwnam: failed", errno);

  if (pwd == NULL)
    return throw_error(info, false, "getpwnam: user not found");

  Napi::Object obj = Napi::Object::New(env);
  obj.Set("name", Napi::String::New(env, pwd->pw_name));
  obj.Set("passwd", Napi::String::New(env, pwd->pw_passwd));
  obj.Set("uid", Napi::Number::New(env, pwd->pw_uid));
  obj.Set("gid", Napi::Number::New(env, pwd->pw_gid));
#ifdef __ANDROID__
  obj.Set("gecos", Napi::String::New(env, ""));
#else
  obj.Set("gecos", Napi::String::New(env, pwd->pw_gecos ? pwd->pw_gecos : ""));
#endif
  obj.Set("dir", Napi::String::New(env, pwd->pw_dir ? pwd->pw_dir : ""));
  obj.Set("shell", Napi::String::New(env, pwd->pw_shell ? pwd->pw_shell : ""));

  return obj;
}

Napi::Value node_getgrnam(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    return throw_error(info, true, "getgrnam: takes 1 argument");

  struct group *grp;

  errno = 0;
  if (info[0].IsString())
    grp = posix_getgrnam(info[0].As<Napi::String>().Utf8Value().c_str());
  else if (info[0].IsNumber())
    grp = posix_getgrgid(info[0].As<Napi::Number>().Int32Value());
  else
    return throw_error(info, true, "getgrnam: first argument must be a string or number");

  if (errno)
    return throw_error(info, false, "getgrnam: failed", errno);

  if (grp == NULL)
    return throw_error(info, false, "getgrnam: group not found");

  Napi::Object obj = Napi::Object::New(env);
  obj.Set("name", Napi::String::New(env, grp->gr_name));
  obj.Set("passwd", Napi::String::New(env, grp->gr_passwd));
  obj.Set("gid", Napi::Number::New(env, grp->gr_gid));

  Napi::Array members = Napi::Array::New(env);
  for (int i = 0; grp->gr_mem[i]; i++)
    members.Set(i, Napi::String::New(env, grp->gr_mem[i]));
  obj.Set("members", members);

  return obj;
}

Napi::Value node_initgroups(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
    return throw_error(info, true, "initgroups: takes 2 arguments");

  if (!info[0].IsString())
    return throw_error(info, true, "initgroups: first argument must be a string");

  gid_t gid;

  if (info[1].IsNumber())
  {
    gid = info[1].As<Napi::Number>().Int32Value();
  }
  else if (info[1].IsString())
  {
    errno = 0;
    group *grp = posix_getgrnam(info[0].As<Napi::String>().Utf8Value().c_str());
    if (errno)
      return throw_error(info, false, "initgroups: getgrnam failed", errno);
    if (!grp)
      return throw_error(info, false, "initgroups: group not found");
    gid = grp->gr_gid;
  }
  else
  {
    return throw_error(info, true, "initgroups: second argument must be a number or string");
  }

  if (posix_initgroups(info[0].As<Napi::String>().Utf8Value().c_str(), gid))
    return throw_error(info, false, "initgroups: failed", errno);

  return env.Undefined();
}

Napi::Value node_setregid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
    return throw_error(info, true, "setregid: takes 2 arguments");

  gid_t rgid;
  gid_t egid;

  if (info[0].IsNumber())
  {
    rgid = info[0].As<Napi::Number>().Int32Value();
  }
  else if (info[0].IsString())
  {
    errno = 0;
    group *grp = posix_getgrnam(info[0].As<Napi::String>().Utf8Value().c_str());
    if (errno)
      return throw_error(info, false, "setregid: getgrnam failed", errno);
    if (!grp)
      return throw_error(info, false, "setregid: group not found");
    rgid = grp->gr_gid;
  }
  else
  {
    return throw_error(info, true, "setregid: first argument must be a string or number");
  }

  if (info[1].IsNumber())
  {
    egid = info[1].As<Napi::Number>().Int32Value();
  }
  else if (info[1].IsString())
  {
    errno = 0;
    group *grp = posix_getgrnam(info[1].As<Napi::String>().Utf8Value().c_str());
    if (errno)
      return throw_error(info, false, "setregid: getgrnam failed", errno);
    if (!grp)
      return throw_error(info, false, "setregid: group not found");
    egid = grp->gr_gid;
  }
  else
  {
    return throw_error(info, true, "setregid: second argument must be a string or number");
  }

  if (posix_setregid(rgid, egid))
    return throw_error(info, false, "setregid: failed", errno);

  return env.Undefined();
}

Napi::Value node_setreuid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
    return throw_error(info, true, "setreuid: takes 2 arguments");

  uid_t ruid;
  uid_t euid;

  if (info[0].IsNumber())
  {
    ruid = info[0].As<Napi::Number>().Int32Value();
  }
  else if (info[0].IsString())
  {
    errno = 0;
    passwd *pwd = posix_getpwnam(info[0].As<Napi::String>().Utf8Value().c_str());
    if (errno)
      return throw_error(info, false, "setreuid: getpwnam failed", errno);
    if (!pwd)
      return throw_error(info, false, "setreuid: user not found");
    ruid = pwd->pw_uid;
  }
  else
  {
    return throw_error(info, true, "setreuid: first argument must be a string or number");
  }

  if (info[1].IsNumber())
  {
    euid = info[1].As<Napi::Number>().Int32Value();
  }
  else if (info[1].IsString())
  {
    errno = 0;
    passwd *pwd = posix_getpwnam(info[1].As<Napi::String>().Utf8Value().c_str());
    if (errno)
      return throw_error(info, false, "setreuid: getpwnam failed", errno);
    if (!pwd)
      return throw_error(info, false, "setreuid: user not found");
    euid = pwd->pw_uid;
  }
  else
  {
    return throw_error(info, true, "setreuid: second argument must be a string or number");
  }

  if (posix_setreuid(ruid, euid))
    return throw_error(info, false, "setreuid: failed", errno);

  return env.Undefined();
}

// openlog() first argument (const char* ident) is not guaranteed to be
// copied within the openlog() call so we need to keep it in a safe location
static const size_t MAX_SYSLOG_IDENT = 100;
static char syslog_ident[MAX_SYSLOG_IDENT + 1] = {0};

Napi::Value node_openlog(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 3)
    return throw_error(info, true, "openlog: takes 3 arguments");

  if (!info[0].IsString())
    return throw_error(info, true, "openlog: first argument must be a string");

  if (!info[1].IsNumber())
    return throw_error(info, true, "openlog: second argument must be a number");

  if (!info[2].IsNumber())
    return throw_error(info, true, "openlog: third argument must be a number");

  strncpy(syslog_ident, info[0].As<Napi::String>().Utf8Value().c_str(), MAX_SYSLOG_IDENT);
  syslog_ident[MAX_SYSLOG_IDENT] = 0;

  posix_openlog(syslog_ident, info[1].As<Napi::Number>().Int32Value(), info[2].As<Napi::Number>().Int32Value());

  return env.Undefined();
}

Napi::Value node_closelog(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    return throw_error(info, true, "closelog: takes 0 arguments");

  posix_closelog();

  return env.Undefined();
}

Napi::Value node_syslog(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
    return throw_error(info, true, "syslog: takes 2 arguments");

  if (!info[0].IsNumber())
    return throw_error(info, true, "syslog: first argument must be a number");

  if (!info[1].IsString())
    return throw_error(info, true, "syslog: second argument must be a string");

  posix_syslog(info[0].As<Napi::Number>().Int32Value(), "%s", info[1].As<Napi::String>().Utf8Value().c_str());

  return env.Undefined();
}

Napi::Value node_setlogmask(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    return throw_error(info, true, "setlogmask: takes 1 argument");

  if (!info[0].IsNumber())
    return throw_error(info, true, "setlogmask: first argument must be a number");

  return Napi::Number::New(env, posix_setlogmask(info[0].As<Napi::Number>().Int32Value()));
}

Napi::Value node_gethostname(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    return throw_error(info, true, "gethostname: takes 0 arguments");

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif

  char hostname[HOST_NAME_MAX + 1] = {0};

  if (posix_gethostname(hostname, HOST_NAME_MAX) != 0)
    return throw_error(info, false, "gethostname: failed", errno);

  return Napi::String::New(env, hostname);
}

Napi::Value node_sethostname(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    return throw_error(info, true, "sethostname: takes 1 argument");

  if (!info[0].IsString())
    return throw_error(info, true, "sethostname: first argument must be a string");

  if (posix_sethostname(info[0].As<Napi::String>().Utf8Value().c_str(), info[0].As<Napi::String>().Utf8Value().length()) != 0)
    return throw_error(info, false, "sethostname: failed", errno);

  return env.Undefined();
}

Napi::Value node_swapon(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
    return throw_error(info, true, "swapon: takes 2 arguments");

  if (!info[0].IsString())
    return throw_error(info, true, "swapon: first argument must be a string");

  if (!info[1].IsNumber())
    return throw_error(info, true, "swapon: second argument must be a number");

  if (posix_swapon(info[0].As<Napi::String>().Utf8Value().c_str(), info[1].As<Napi::Number>().Int32Value()) != 0)
    return throw_error(info, false, "swapon: failed", errno);

  return env.Undefined();
}

Napi::Value node_swapoff(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    return throw_error(info, true, "swapoff: takes 1 argument");

  if (!info[0].IsString())
    return throw_error(info, true, "swapoff: first argument must be a string");

  if (posix_swapoff(info[0].As<Napi::String>().Utf8Value().c_str()) != 0)
    return throw_error(info, false, "swapoff: failed", errno);

  return env.Undefined();
}

// Contants

Napi::Value node_get_syslog_constants(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  Napi::Object priorities = Napi::Object::New(env);

  priorities.Set("emerg", Napi::Number::New(env, LOG_EMERG));
  priorities.Set("alert", Napi::Number::New(env, LOG_ALERT));
  priorities.Set("crit", Napi::Number::New(env, LOG_CRIT));
  priorities.Set("err", Napi::Number::New(env, LOG_ERR));
  priorities.Set("warning", Napi::Number::New(env, LOG_WARNING));
  priorities.Set("notice", Napi::Number::New(env, LOG_NOTICE));
  priorities.Set("info", Napi::Number::New(env, LOG_INFO));
  priorities.Set("debug", Napi::Number::New(env, LOG_DEBUG));

  Napi::Object mask_priorities = Napi::Object::New(env);

  mask_priorities.Set("emerg", Napi::Number::New(env, LOG_MASK(LOG_EMERG)));
  mask_priorities.Set("alert", Napi::Number::New(env, LOG_MASK(LOG_ALERT)));
  mask_priorities.Set("crit", Napi::Number::New(env, LOG_MASK(LOG_CRIT)));
  mask_priorities.Set("err", Napi::Number::New(env, LOG_MASK(LOG_ERR)));
  mask_priorities.Set("warning", Napi::Number::New(env, LOG_MASK(LOG_WARNING)));
  mask_priorities.Set("notice", Napi::Number::New(env, LOG_MASK(LOG_NOTICE)));
  mask_priorities.Set("info", Napi::Number::New(env, LOG_MASK(LOG_INFO)));
  mask_priorities.Set("debug", Napi::Number::New(env, LOG_MASK(LOG_DEBUG)));

  Napi::Object facilities = Napi::Object::New(env);

  facilities.Set("auth", Napi::Number::New(env, LOG_AUTH));
#ifdef LOG_AUTHPRIV
  facilities.Set("authpriv", Napi::Number::New(env, LOG_AUTHPRIV));
#endif
  facilities.Set("cron", Napi::Number::New(env, LOG_CRON));
  facilities.Set("daemon", Napi::Number::New(env, LOG_DAEMON));
#ifdef LOG_FTP
  facilities.Set("ftp", Napi::Number::New(env, LOG_FTP));
#endif
  facilities.Set("kern", Napi::Number::New(env, LOG_KERN));
  facilities.Set("lpr", Napi::Number::New(env, LOG_LPR));
  facilities.Set("mail", Napi::Number::New(env, LOG_MAIL));
  facilities.Set("news", Napi::Number::New(env, LOG_NEWS));
  facilities.Set("syslog", Napi::Number::New(env, LOG_SYSLOG));
  facilities.Set("user", Napi::Number::New(env, LOG_USER));
  facilities.Set("uucp", Napi::Number::New(env, LOG_UUCP));
  facilities.Set("local0", Napi::Number::New(env, LOG_LOCAL0));
  facilities.Set("local1", Napi::Number::New(env, LOG_LOCAL1));
  facilities.Set("local2", Napi::Number::New(env, LOG_LOCAL2));
  facilities.Set("local3", Napi::Number::New(env, LOG_LOCAL3));
  facilities.Set("local4", Napi::Number::New(env, LOG_LOCAL4));
  facilities.Set("local5", Napi::Number::New(env, LOG_LOCAL5));
  facilities.Set("local6", Napi::Number::New(env, LOG_LOCAL6));
  facilities.Set("local7", Napi::Number::New(env, LOG_LOCAL7));

  Napi::Object option_flags = Napi::Object::New(env);

  option_flags.Set("pid", Napi::Number::New(env, LOG_PID));
  option_flags.Set("cons", Napi::Number::New(env, LOG_CONS));
  option_flags.Set("ndelay", Napi::Number::New(env, LOG_NDELAY));
  option_flags.Set("odelay", Napi::Number::New(env, LOG_ODELAY));
  option_flags.Set("nowait", Napi::Number::New(env, LOG_NOWAIT));

  Napi::Object result = Napi::Object::New(env);
  result.Set("priorities", priorities);
  result.Set("mask_priorities", mask_priorities);
  result.Set("facilities", facilities);
  result.Set("option_flags", option_flags);

  return result;
}

Napi::Value node_get_swap_constants(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  Napi::Object obj = Napi::Object::New(env);

  obj.Set("prefer", Napi::Number::New(env, SWAP_FLAG_PREFER));
#ifdef SWAP_FLAG_DISCARD
  obj.Set("discard", Napi::Number::New(env, SWAP_FLAG_DISCARD));
#endif

  return obj;
}

#ifdef MOCK_POSIX
Napi::Value node_mock_reset(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    return throw_error(info, true, "mock_reset: takes no arguments");

  mock_reset();

  return env.Undefined();
}
#endif

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  // Get isolate

#ifdef MOCK_POSIX
  exports.Set("mock_reset", Napi::Function::New(env, node_mock_reset));
#endif

  exports.Set("getuid", Napi::Function::New(env, node_getuid));
  exports.Set("getgid", Napi::Function::New(env, node_getgid));
  exports.Set("setuid", Napi::Function::New(env, node_setuid));
  exports.Set("setgid", Napi::Function::New(env, node_setgid));

  exports.Set("geteuid", Napi::Function::New(env, node_geteuid));
  exports.Set("getegid", Napi::Function::New(env, node_getegid));
  exports.Set("seteuid", Napi::Function::New(env, node_seteuid));
  exports.Set("setegid", Napi::Function::New(env, node_setegid));

  exports.Set("getpid", Napi::Function::New(env, node_getpid));
  exports.Set("getpgrp", Napi::Function::New(env, node_getpgrp));
  exports.Set("getppid", Napi::Function::New(env, node_getppid));
  exports.Set("getpgid", Napi::Function::New(env, node_getpgid));
  exports.Set("setpgid", Napi::Function::New(env, node_setpgid));

  exports.Set("setsid", Napi::Function::New(env, node_setsid));
  exports.Set("getsid", Napi::Function::New(env, node_getsid));

  exports.Set("chroot", Napi::Function::New(env, node_chroot));

  exports.Set("getrlimit", Napi::Function::New(env, node_getrlimit));
  exports.Set("setrlimit", Napi::Function::New(env, node_setrlimit));

  exports.Set("getpwnam", Napi::Function::New(env, node_getpwnam));
  exports.Set("getgrnam", Napi::Function::New(env, node_getgrnam));

  exports.Set("initgroups", Napi::Function::New(env, node_initgroups));

  exports.Set("setregid", Napi::Function::New(env, node_setregid));
  exports.Set("setreuid", Napi::Function::New(env, node_setreuid));

  exports.Set("openlog", Napi::Function::New(env, node_openlog));
  exports.Set("closelog", Napi::Function::New(env, node_closelog));
  exports.Set("syslog", Napi::Function::New(env, node_syslog));
  exports.Set("setlogmask", Napi::Function::New(env, node_setlogmask));

  exports.Set("gethostname", Napi::Function::New(env, node_gethostname));
  exports.Set("sethostname", Napi::Function::New(env, node_sethostname));

  exports.Set("swapon", Napi::Function::New(env, node_swapon));
  exports.Set("swapoff", Napi::Function::New(env, node_swapoff));

  exports.Set("getsyslogConstants", Napi::Function::New(env, node_get_syslog_constants));
  exports.Set("getswapConstants", Napi::Function::New(env, node_get_swap_constants));

  return exports;
}

NODE_API_MODULE(addon, Init)
