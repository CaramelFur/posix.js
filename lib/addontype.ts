export interface SyslogConstantsPriorities {
  emerg: number;
  alert: number;
  crit: number;
  err: number;
  warning: number;
  notice: number;
  info: number;
  debug: number;
}

export interface SyslogConstantsFacilities {
  auth: number;
  authpriv?: number;
  cron: number;
  daemon: number;
  ftp?: number;
  kern: number;
  lpr: number;
  mail: number;
  news: number;
  syslog: number;
  user: number;
  uucp: number;
  local0: number;
  local1: number;
  local2: number;
  local3: number;
  local4: number;
  local5: number;
  local6: number;
  local7: number;
}

export interface SyslogConstantsOptions {
  pid: number;
  cons: number;
  ndelay: number;
  odelay: number;
  nowait: number;
}

export interface SyslogConstants {
  priorities: SyslogConstantsPriorities;
  mask_priorities: SyslogConstantsPriorities;
  facilities: SyslogConstantsFacilities;
  option_flags: SyslogConstantsOptions;
}

export interface SwapConstants {
  prefer: number;
  discard?: number;
}

export interface AddonType {
  getppid(): number;
  getpgid(pid: number): number;
  setpgid(pid: number, pgid: number): void;

  setsid(): number;

  chroot(path: string): void;

  getrlimit(type: number): { cur: number; max: number };
  setrlimit(type: number, lim: { cur: number; max: number }): void;

  getpwnam(name: string): {
    name: string;
    passwd: string;
    uid: number;
    gid: number;
    gecos: string;
    dir: string;
    shell: string;
  };
  getgrnam(name: string): {
    name: string;
    passwd: string;
    gid: number;
    members: string[];
  };

  initgroups(user: string, group: number | string): void;

  setregid(rgid: number | string, egid: number | string): void;
  setreuid(ruid: number, euid: number): void;

  openlog(ident: string, option: number, facility: number): void;
  closelog(): void;
  syslog(priority: number, message: string): void;
  setlogmask(maskpriority: number): number;

  gethostname(): string;
  sethostname(name: string): void;

  swapon(path: string, flags: number): void;
  swapoff(path: string): void;

  getsyslogConstants(): SyslogConstants;
  getswapConstants(): SwapConstants;
}
