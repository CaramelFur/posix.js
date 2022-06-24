import * as posix from '../dist';
import { test_function } from './util';

const untyped_posix = posix as any;

describe('uid and gid - Failing', () => {
  test_function('getgid', untyped_posix.getgid, []);

  test_function('getuid', untyped_posix.getuid, []);

  test_function('setgid', untyped_posix.setgid, [['number', 'string']]);

  test_function('setuid', untyped_posix.setuid, [['number', 'string']]);
});

describe('uid and gid - Correct', () => {
  test('getgid()', () => {
    const result = posix.getgid();

    expect(result).toBe(0);
  });

  test('getuid()', () => {
    const result = posix.getuid();

    expect(result).toBe(0);
  });

  test('setgid(1000)', () => {
    posix.setgid(1000);

    const result = posix.getgid();

    expect(result).toBe(1000);
  });

  test('setuid(1000)', () => {
    posix.setuid(1000);

    let result = posix.getuid();

    expect(result).toBe(1000);
  });

  test('setgid("user")', () => {
    posix.setgid('user');

    const result = posix.getgid();

    expect(result).toBe(1000);
  });

  test('setuid("user")', () => {
    posix.setuid('user');

    let result = posix.getuid();

    expect(result).toBe(1000);
  });

  test('setgid(0) - Fail', () => {
    posix.setuid(1000);
    posix.setgid(1000);

    expect(() => {
      posix.setgid(0);
    }).toThrow();
  });

  test('setuid(0) - Fail', () => {
    posix.setuid(1000);
    posix.setgid(1000);

    expect(() => {
      posix.setuid(0);
    }).toThrow();
  });
});
