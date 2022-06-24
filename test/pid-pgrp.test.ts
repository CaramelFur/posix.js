import * as posix from '../dist';
import { test_function } from './util';

const untyped_posix = posix as any;

describe('pid and pgrp - Failing', () => {
  test_function('getpid', untyped_posix.getpid, []);

  test_function('getpgrp', untyped_posix.getpgrp, []);

  test_function('getppid', untyped_posix.getppid, []);

  test_function('getpgid', untyped_posix.getpgid, [['number']]);

  test_function('setpgid', untyped_posix.setpgid, [['number'], ['number']]);
});

describe('pid and pgrp - Correct', () => {
  test('getpid()', () => {
    expect(posix.getpid()).toBe(3044);
  });

  test('getpgrp()', () => {
    expect(posix.getpgrp()).toBe(3020);
  });

  test('getppid()', () => {
    expect(posix.getppid()).toBe(3011);
  });

  test('getpgid(3011)', () => {
    expect(posix.getpgid(3011)).toBe(3001);
  });

  test('setpgid(3044, 3001)', () => {
    expect(posix.getpgrp()).toBe(3020);

    posix.setpgid(3044, 3001);

    expect(posix.getpgrp()).toBe(3001);
  });

  test('setpgid(100, 3020) - Fail', () => {
    expect(posix.getpgrp()).toBe(3020);

    expect(() => {
      posix.setpgid(100, 3020);
    }).toThrow();

    expect(posix.getpgrp()).toBe(3020);
  });
});
