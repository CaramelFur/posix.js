import LoadBinding from 'bindings';
import {
  AddonType,
  SwapConstants,
  SyslogConstantsFacilities,
  SyslogConstantsOptions,
  SyslogConstantsPriorities,
} from './addontype';

const SupportedPlatforms = [
  'aix',
  'android',
  'darwin',
  'freebsd',
  'haiku',
  'linux',
  'openbsd',
  'sunos',
  'netbsd',
];

if (!SupportedPlatforms.includes(process.platform)) {
  throw new Error(`Platform ${process.platform} is not supported`);
}

const Posix = LoadBinding('posix_addon') as AddonType;

export const {
  getegid,
  geteuid,
  getgid,
  getuid,
  setegid,
  seteuid,
  setgid,
  setuid,
} = process as Required<typeof process>;
export const {
  getppid,
  getpgid,
  setpgid,
  setsid,
  chroot,
  getrlimit,
  setrlimit,
  getpwnam,
  getgrnam,
  initgroups,
  setregid,
  setreuid,
  closelog,
  gethostname,
  sethostname,
  swapoff,
} = Posix;

export function getpgrp() {
  return getpgid(0);
}

const swapConstants = Posix.getswapConstants();
const syslogConstants = Posix.getsyslogConstants();

export function swapon(path: string, swapflags: Array<keyof SwapConstants>) {
  let flags = 0;
  for (const swapflag of swapflags) {
    const flag = swapConstants[swapflag];
    if (flag === undefined) throw new Error(`Unknown swap flag: ${flag}`);
    flags |= flag;
  }

  Posix.swapon(path, flags);
}

export function openlog(
  ident: string,
  options: Array<keyof SyslogConstantsOptions>,
  facility: keyof SyslogConstantsFacilities,
) {
  let flags = 0;
  for (const opt of options) {
    const flag = syslogConstants.option_flags[opt];
    if (flag === undefined) throw new Error(`Unknown option ${opt}`);
    flags |= flag;
  }

  const fac = syslogConstants.facilities[facility];
  if (fac === undefined) throw new Error(`Unknown facility ${facility}`);

  Posix.openlog(ident, flags, fac);
}

export function syslog(
  priority: keyof SyslogConstantsPriorities,
  message: string,
) {
  const pri = syslogConstants.priorities[priority];
  if (pri === undefined) throw new Error(`Unknown priority ${priority}`);

  Posix.syslog(pri, message);
}

export function setlogmask(
  mask_priorities: Array<keyof SyslogConstantsPriorities>,
) {
  let mask = 0;
  for (const mask_priority of mask_priorities) {
    const prio = syslogConstants.priorities[mask_priority];
    if (prio === undefined)
      throw new Error(`Unknown priority ${mask_priority}`);

    mask |= prio;
  }

  const results = Posix.setlogmask(mask);

  let flags: {
    [key in keyof SyslogConstantsPriorities]: boolean;
  } = {} as any;

  for (const opt of Object.keys(syslogConstants.mask_priorities) as Array<
    keyof SyslogConstantsPriorities
  >) {
    flags[opt] = (results & syslogConstants.mask_priorities[opt]) !== 0;
  }

  return flags;
}
