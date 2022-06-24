import * as posix from '../dist';
import { test_function } from './util';

const untyped_posix = posix as any;

describe('sid - Failing', () => {
  test_function('setsid', untyped_posix.setsid, []);

  test_function('getsid', untyped_posix.getsid, [['number']]);
});

describe('sid - Correct', () => {
  test('setsid()', () => {
    const pid = posix.getpid();

    expect(posix.setsid()).toBe(pid);
  });

  test('setsid() - Fail', () => {
    const pid = posix.getpid();

    expect(posix.setsid()).toBe(pid);

    expect(() => {
      posix.setsid();
    }).toThrow();
  });

  test('getsid(0)', () => {
    const pid = posix.getpid();

    posix.setsid();

    expect(posix.getsid(0)).toBe(pid);
  });

  test('getsid(pid)', () => {
    const pid = posix.getpid();

    posix.setsid();

    expect(posix.getsid(pid)).toBe(pid);
  });

  test('getsid(100) - Fail', () => {
    posix.setsid();

    expect(() => {
      posix.getsid(100);
    }).toThrow();
  });

  test('getsid(0) - Fail', () => {
    expect(() => {
      posix.getsid(0);
    }).toThrow();
  });
});
