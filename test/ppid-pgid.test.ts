import * as posix from '../dist';

const untyped_posix = posix as any;

describe('ppid and pgid - Failing', () => {
  test('getppid() - Incorrect arguments', () => {
    expect(() => {
      untyped_posix.getppid(1000);
    }).toThrow();
  });

  test('getpgid() - Incorrect arguments', () => {
    expect(() => {
      untyped_posix.getpgid();
    }).toThrow();

    expect(() => {
      untyped_posix.getpgid(1000, 1000);
    }).toThrow();
  });

  test('setpgid() - Incorrect arguments', () => {
    expect(() => {
      untyped_posix.setpgid();
    }).toThrow();

    expect(() => {
      untyped_posix.setpgid(1000);
    }).toThrow();

    expect(() => {
      untyped_posix.setpgid(1000, 1000, 1000);
    }).toThrow();
  });
});

describe('euid and egid - Correct', () => {
  test('getppid()', () => {
    const result = posix.getppid();

    expect(result).toBeGreaterThan(0);
  });

  test('getpgid()', () => {
    const ppid = posix.getppid();

    const result = posix.getpgid(ppid);

    expect(result).toBeGreaterThan(0);
  });

  test('setpgid()', () => {
    const pid = posix.getpid();

    const grp = posix.getpgrp();

    posix.setpgid(pid, grp);

    const result = posix.getpgid(pid);

    expect(result).toBe(grp);
  });
});
