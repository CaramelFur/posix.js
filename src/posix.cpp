#include <napi.h>

// Error if building on non-posix system
#if !defined(__unix__) && !defined(__linux__) && !defined(__APPLE__) && !defined(__ANDROID__)
#error "This module is only supported on POSIX systems"
#endif

#include <unistd.h>
#include <sys/resource.h> // setrlimit, getrlimit
#include <pwd.h>          // getpwnam, passwd
#include <grp.h>          // getgrnam, group
#include <syslog.h>       // openlog, closelog, syslog, setlogmask
#include <sys/swap.h>     // swapon, swapoff

Napi::Value node_getppid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    Napi::TypeError::New(env, "getppid: takes no arguments").ThrowAsJavaScriptException();

  return Napi::Number::New(env, getppid());
}

Napi::Value node_getpgid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::TypeError::New(env, "getpgid: takes 1 argument").ThrowAsJavaScriptException();

  if (!info[0].IsNumber())
    Napi::TypeError::New(env, "getpgid: first argument must be a number").ThrowAsJavaScriptException();

  return Napi::Number::New(env, getpgid(info[0].As<Napi::Number>().Int32Value()));
}

Napi::Value node_setpgid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
    Napi::TypeError::New(env, "setpgid: takes 2 arguments").ThrowAsJavaScriptException();

  if (!info[0].IsNumber())
    Napi::TypeError::New(env, "setpgid: first argument must be a number").ThrowAsJavaScriptException();

  if (!info[1].IsNumber())
    Napi::TypeError::New(env, "setpgid: second argument must be a number").ThrowAsJavaScriptException();

  if (setpgid(info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value()))
    Napi::Error::New(env, "setpgid: failed").ThrowAsJavaScriptException();

  return env.Undefined();
}

Napi::Value node_setsid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    Napi::TypeError::New(env, "setsid: takes no arguments").ThrowAsJavaScriptException();

  pid_t pid = setsid();
  if (pid == -1)
    Napi::Error::New(env, "setsid: failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, pid);
}

Napi::Value node_chroot(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::TypeError::New(env, "chroot: takes 1 argument").ThrowAsJavaScriptException();

  if (!info[0].IsString())
    Napi::TypeError::New(env, "chroot: first argument must be a string").ThrowAsJavaScriptException();

  const char *path = info[0].As<Napi::String>().Utf8Value().c_str();

  // Proper order is to first chdir() then chroot()
  if (chdir(path))
    Napi::Error::New(env, "chroot: failed to change directory").ThrowAsJavaScriptException();

  if (chroot(path))
    Napi::Error::New(env, "chroot: failed").ThrowAsJavaScriptException();

  return env.Undefined();
}

Napi::Value node_getrlimit(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::TypeError::New(env, "getrlimit: takes 1 argument").ThrowAsJavaScriptException();

  if (!info[0].IsNumber())
    Napi::TypeError::New(env, "getrlimit: first argument must be a number").ThrowAsJavaScriptException();

  struct rlimit limit;
  if (getrlimit(info[0].As<Napi::Number>().Int32Value(), &limit))
    Napi::Error::New(env, "getrlimit: failed").ThrowAsJavaScriptException();

  Napi::Object obj = Napi::Object::New(env);
  obj.Set("soft", Napi::Number::New(env, limit.rlim_cur));
  obj.Set("hard", Napi::Number::New(env, limit.rlim_max));

  return obj;
}

Napi::Value node_setrlimit(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
    Napi::TypeError::New(env, "setrlimit: takes 2 arguments").ThrowAsJavaScriptException();

  if (!info[0].IsNumber())
    Napi::TypeError::New(env, "setrlimit: first argument must be a number").ThrowAsJavaScriptException();

  if (!info[1].IsObject())
    Napi::TypeError::New(env, "setrlimit: second argument must be an object").ThrowAsJavaScriptException();

  struct rlimit limit;
  // Pre populate with current values
  if (getrlimit(info[0].As<Napi::Number>().Int32Value(), &limit))
    Napi::Error::New(env, "setrlimit: failed").ThrowAsJavaScriptException();

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
      Napi::TypeError::New(env, "setrlimit: second argument soft must be a number or null").ThrowAsJavaScriptException();
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
      Napi::TypeError::New(env, "setrlimit: second argument hard must be a number or null").ThrowAsJavaScriptException();
    }
  }

  if (setrlimit(info[0].As<Napi::Number>().Int32Value(), &limit))
    Napi::Error::New(env, "setrlimit: failed").ThrowAsJavaScriptException();

  return env.Undefined();
}

Napi::Value node_getpwnam(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::TypeError::New(env, "getpwnam: takes 1 argument").ThrowAsJavaScriptException();

  struct passwd *pwd;
  errno = 0; // reset errno before the call

  if (info[0].IsString())
    pwd = getpwnam(info[0].As<Napi::String>().Utf8Value().c_str());
  else if (info[0].IsNumber())
    pwd = getpwuid(info[0].As<Napi::Number>().Int32Value());
  else
    Napi::TypeError::New(env, "getpwnam: first argument must be a string or number").ThrowAsJavaScriptException();

  if (errno)
    Napi::Error::New(env, "getpwnam: failed").ThrowAsJavaScriptException();

  if (pwd == NULL)
    Napi::Error::New(env, "getpwnam: user not found").ThrowAsJavaScriptException();

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
    Napi::TypeError::New(env, "getgrnam: takes 1 argument").ThrowAsJavaScriptException();

  struct group *grp;
  errno = 0; // reset errno before the call

  if (info[0].IsString())
    grp = getgrnam(info[0].As<Napi::String>().Utf8Value().c_str());
  else if (info[0].IsNumber())
    grp = getgrgid(info[0].As<Napi::Number>().Int32Value());
  else
    Napi::TypeError::New(env, "getgrnam: first argument must be a string or number").ThrowAsJavaScriptException();

  if (errno)
    Napi::Error::New(env, "getgrnam: failed").ThrowAsJavaScriptException();

  if (grp == NULL)
    Napi::Error::New(env, "getgrnam: group not found").ThrowAsJavaScriptException();

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
    Napi::TypeError::New(env, "initgroups: takes 2 arguments").ThrowAsJavaScriptException();

  if (!info[0].IsString())
    Napi::TypeError::New(env, "initgroups: first argument must be a string").ThrowAsJavaScriptException();

  gid_t group;

  if (info[1].IsNumber())
    group = info[1].As<Napi::Number>().Int32Value();
  else if (info[1].IsString())
    group = getgrnam(info[1].As<Napi::String>().Utf8Value().c_str())->gr_gid;
  else
    Napi::TypeError::New(env, "initgroups: second argument must be a number or string").ThrowAsJavaScriptException();

  if (initgroups(info[0].As<Napi::String>().Utf8Value().c_str(), info[1].As<Napi::Number>().Int32Value()))
    Napi::Error::New(env, "initgroups: failed").ThrowAsJavaScriptException();

  return env.Undefined();
}

Napi::Value node_setregid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
    Napi::TypeError::New(env, "setregid: takes 2 arguments").ThrowAsJavaScriptException();

  gid_t rgid;
  gid_t egid;

  if (info[0].IsNumber())
    rgid = info[0].As<Napi::Number>().Int32Value();
  else if (info[0].IsString())
    rgid = getgrnam(info[0].As<Napi::String>().Utf8Value().c_str())->gr_gid;
  else
    Napi::TypeError::New(env, "setregid: first argument must be a string or number").ThrowAsJavaScriptException();

  if (info[1].IsNumber())
    egid = info[1].As<Napi::Number>().Int32Value();
  else if (info[1].IsString())
    egid = getgrnam(info[1].As<Napi::String>().Utf8Value().c_str())->gr_gid;
  else
    Napi::TypeError::New(env, "setregid: second argument must be a string or number").ThrowAsJavaScriptException();

  if (setregid(info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value()))
    Napi::Error::New(env, "setregid: failed").ThrowAsJavaScriptException();

  return env.Undefined();
}

Napi::Value node_setreuid(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
    Napi::TypeError::New(env, "setreuid: takes 2 arguments").ThrowAsJavaScriptException();

  uid_t ruid;
  uid_t euid;

  if (info[0].IsNumber())
    ruid = info[0].As<Napi::Number>().Int32Value();
  else if (info[0].IsString())
    ruid = getpwnam(info[0].As<Napi::String>().Utf8Value().c_str())->pw_uid;
  else
    Napi::TypeError::New(env, "setreuid: first argument must be a string or number").ThrowAsJavaScriptException();

  if (info[1].IsNumber())
    euid = info[1].As<Napi::Number>().Int32Value();
  else if (info[1].IsString())
    euid = getpwnam(info[1].As<Napi::String>().Utf8Value().c_str())->pw_uid;
  else
    Napi::TypeError::New(env, "setreuid: second argument must be a string or number").ThrowAsJavaScriptException();

  if (setreuid(info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value()))
    Napi::Error::New(env, "setreuid: failed").ThrowAsJavaScriptException();

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
    Napi::TypeError::New(env, "openlog: takes 3 arguments").ThrowAsJavaScriptException();

  if (!info[0].IsString())
    Napi::TypeError::New(env, "openlog: first argument must be a string").ThrowAsJavaScriptException();

  if (!info[1].IsNumber())
    Napi::TypeError::New(env, "openlog: second argument must be a number").ThrowAsJavaScriptException();

  if (!info[2].IsNumber())
    Napi::TypeError::New(env, "openlog: third argument must be a number").ThrowAsJavaScriptException();

  strncpy(syslog_ident, info[0].As<Napi::String>().Utf8Value().c_str(), MAX_SYSLOG_IDENT);
  syslog_ident[MAX_SYSLOG_IDENT] = 0;

  openlog(syslog_ident, info[1].As<Napi::Number>().Int32Value(), info[2].As<Napi::Number>().Int32Value());

  return env.Undefined();
}

Napi::Value node_closelog(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    Napi::TypeError::New(env, "closelog: takes 0 arguments").ThrowAsJavaScriptException();

  closelog();

  return env.Undefined();
}

Napi::Value node_syslog(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
    Napi::TypeError::New(env, "syslog: takes 2 arguments").ThrowAsJavaScriptException();

  if (!info[0].IsNumber())
    Napi::TypeError::New(env, "syslog: first argument must be a number").ThrowAsJavaScriptException();

  if (!info[1].IsString())
    Napi::TypeError::New(env, "syslog: second argument must be a string").ThrowAsJavaScriptException();

  syslog(info[0].As<Napi::Number>().Int32Value(), "%s", info[1].As<Napi::String>().Utf8Value().c_str());

  return env.Undefined();
}

Napi::Value node_setlogmask(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::TypeError::New(env, "setlogmask: takes 1 argument").ThrowAsJavaScriptException();

  if (!info[0].IsNumber())
    Napi::TypeError::New(env, "setlogmask: first argument must be a number").ThrowAsJavaScriptException();

  return Napi::Number::New(env, setlogmask(info[0].As<Napi::Number>().Int32Value()));
}

Napi::Value node_gethostname(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 0)
    Napi::TypeError::New(env, "gethostname: takes 0 arguments").ThrowAsJavaScriptException();

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif

  char hostname[HOST_NAME_MAX + 1] = {0};

  if (gethostname(hostname, HOST_NAME_MAX) != 0)
    Napi::Error::New(env, "gethostname: failed").ThrowAsJavaScriptException();

  return Napi::String::New(env, hostname);
}

Napi::Value node_sethostname(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::TypeError::New(env, "sethostname: takes 1 argument").ThrowAsJavaScriptException();

  if (!info[0].IsString())
    Napi::TypeError::New(env, "sethostname: first argument must be a string").ThrowAsJavaScriptException();

  if (sethostname(info[0].As<Napi::String>().Utf8Value().c_str(), info[0].As<Napi::String>().Utf8Value().length()) != 0)
    Napi::Error::New(env, "sethostname: failed").ThrowAsJavaScriptException();

  return env.Undefined();
}

Napi::Value node_swapon(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
    Napi::TypeError::New(env, "swapon: takes 2 arguments").ThrowAsJavaScriptException();

  if (!info[0].IsString())
    Napi::TypeError::New(env, "swapon: first argument must be a string").ThrowAsJavaScriptException();

  if (!info[1].IsNumber())
    Napi::TypeError::New(env, "swapon: second argument must be a number").ThrowAsJavaScriptException();

  if (swapon(info[0].As<Napi::String>().Utf8Value().c_str(), info[1].As<Napi::Number>().Int32Value()) != 0)
    Napi::Error::New(env, "swapon: failed").ThrowAsJavaScriptException();

  return env.Undefined();
}

Napi::Value node_swapoff(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::TypeError::New(env, "swapoff: takes 1 argument").ThrowAsJavaScriptException();

  if (!info[0].IsString())
    Napi::TypeError::New(env, "swapoff: first argument must be a string").ThrowAsJavaScriptException();

  if (swapoff(info[0].As<Napi::String>().Utf8Value().c_str()) != 0)
    Napi::Error::New(env, "swapoff: failed").ThrowAsJavaScriptException();

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

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set("getppid", Napi::Function::New(env, node_getppid));
  exports.Set("getpgid", Napi::Function::New(env, node_getpgid));
  exports.Set("setpgid", Napi::Function::New(env, node_setpgid));

  exports.Set("setsid", Napi::Function::New(env, node_setsid));

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
