import * as posix from '../dist';
import { test_function } from './util';

const untyped_posix = posix as any;
describe('euid and egid - Failing', () => {
  test_function('getegid', untyped_posix.getegid, []);

  test_function('geteuid', untyped_posix.geteuid, []);

  test_function('setegid', untyped_posix.setegid, [['number', 'string']]);

  test_function('seteuid', untyped_posix.seteuid, [['number', 'string']]);
});

describe('euid and egid - Correct', () => {
  test('getegid()', () => {
    const result = posix.getegid();

    expect(result).toBe(0);
  });

  test('geteuid()', () => {
    const result = posix.geteuid();

    expect(result).toBe(0);
  });

  test('setegid("user")', () => {
    posix.setegid("user");

    const result = posix.getegid();

    expect(result).toBe(1000);
  });

  test('seteuid("user")', () => {
    posix.seteuid("user");

    let result = posix.geteuid();

    expect(result).toBe(1000);
  });

  test('setegid(0)', () => {
    posix.setegid(0);

    let result = posix.getegid();

    expect(result).toBe(0);
  });

  test('seteuid(0)', () => {
    posix.seteuid(0);

    let result = posix.geteuid();

    expect(result).toBe(0);
  });
});
