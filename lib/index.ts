import LoadBinding from 'bindings';
import {
  AddonType,
  SwapConstants,
  SyslogConstantsFacilities,
  SyslogConstantsOptions,
  SyslogConstantsPriorities
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

const Posix = LoadBinding(
  process.env['MOCK_POSIX'] ? 'posix_addon_mocked' : 'posix_addon',
) as AddonType;

export const {
  getgid,
  getuid,
  setgid,
  setuid,
  getegid,
  geteuid,
  setegid,
  seteuid,
  getpid,
  getpgrp,
  getppid,
  getpgid,
  setpgid,
  setsid,
  getsid,
  chroot,
  getrlimit,
  setrlimit,
  getpwnam,
  getgrnam,
  initgroups,
  getgroups,
  setgroups,
  setregid,
  setreuid,
  closelog,
  gethostname,
  sethostname,
  swapoff,
} = Posix;

/** @internal */
export const mock_reset = Posix.mock_reset;
/** @internal */
export const mock_getsyslog = Posix.mock_getsyslog;

const swapConstants = Posix.getswapConstants();
const syslogConstants = Posix.getsyslogConstants();

export function swapon(path: string, swapflags: Array<keyof SwapConstants>) {
  if (arguments.length !== 2) throw new TypeError('swapon: takes 2 arguments');
  if (!Array.isArray(swapflags))
    throw new TypeError('swapon: second argument must be an array');

  let flags = 0;
  for (const swapflag of swapflags) {
    if (typeof swapflag !== 'string')
      throw new TypeError(
        'swapon: second argument must be an array of strings',
      );

    const flag = swapConstants[swapflag];
    if (flag === undefined)
      throw new Error(`swapon: unknown swap flag: ${flag}`);
    flags |= flag;
  }

  Posix.swapon(path, flags);
}

export function openlog(
  ident: string,
  options: Array<keyof SyslogConstantsOptions>,
  facility: keyof SyslogConstantsFacilities,
) {
  if (arguments.length !== 3) {
    throw new TypeError(`openlog: takes 3 arguments`);
  }

  if (!Array.isArray(options))
    throw new TypeError(`openlog: second argument must be an array`);
  if (typeof facility !== 'string')
    throw new TypeError(`openlog: third argument must be a string`);

  let flags = 0;
  for (const opt of options) {
    if (typeof opt !== 'string')
      throw new TypeError(
        `openlog: second argument must be an array of strings`,
      );

    const flag = syslogConstants.option_flags[opt];
    if (flag === undefined) throw new Error(`openlog: unknown option ${opt}`);
    flags |= flag;
  }

  const fac = syslogConstants.facilities[facility];
  if (fac === undefined)
    throw new Error(`openlog: unknown facility ${facility}`);

  Posix.openlog(ident, flags, fac);
}

export function syslog(
  priority: keyof SyslogConstantsPriorities,
  message: string,
) {
  if (arguments.length !== 2) throw new TypeError(`syslog: takes 2 arguments`);

  if (typeof priority !== 'string')
    throw new TypeError(`syslog: first argument must be a string`);

  const pri = syslogConstants.priorities[priority];
  if (pri === undefined)
    throw new Error(`syslog: unknown priority ${priority}`);

  Posix.syslog(pri, message);
}

export function setlogmask(
  mask_priorities: Array<keyof SyslogConstantsPriorities>,
) {
  if (arguments.length !== 1)
    throw new TypeError(`setlogmask: takes 1 argument`);

  if (!Array.isArray(mask_priorities))
    throw new TypeError(`setlogmask: first argument must be an array`);

  let mask = 0;
  for (const mask_priority of mask_priorities) {
    if (typeof mask_priority !== 'string')
      throw new TypeError(
        `setlogmask: first argument must be an array of strings`,
      );

    const prio = syslogConstants.priorities[mask_priority];
    if (prio === undefined)
      throw new Error(`setlogmask: nknown priority ${mask_priority}`);

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
