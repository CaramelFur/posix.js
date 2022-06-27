import * as posix from '../dist';
import { test_function } from './util';

const untyped_posix = posix as any;
describe('reuid and regid - Failing', () => {
  test_function('setregid', untyped_posix.setregid, [
    ['number', 'string'],
    ['number', 'string'],
  ]);

  test_function('setreuid', untyped_posix.setreuid, [
    ['number', 'string'],
    ['number', 'string'],
  ]);
});

describe('reuid and regid - Correct', () => {
  test('setregid(1000, 1001)', () => {
    posix.setregid(1000, 1001);

    expect(posix.getgid()).toBe(1000);
    expect(posix.getegid()).toBe(1001);
  });

  test('setregid("user", "group2")', () => {
    posix.setregid('user', 'group2');

    expect(posix.getgid()).toBe(1000);
    expect(posix.getegid()).toBe(1001);
  });

  test('setregid(1000, "group2")', () => {
    posix.setregid(1000, 'group2');

    expect(posix.getgid()).toBe(1000);
    expect(posix.getegid()).toBe(1001);
  });

  test('setreuid(1000, 1001)', () => {
    posix.setreuid(1000, 1001);

    expect(posix.getuid()).toBe(1000);
    expect(posix.geteuid()).toBe(1001);
  });

  test('setreuid("user", "user2")', () => {
    posix.setreuid('user', 'user2');

    expect(posix.getuid()).toBe(1000);
    expect(posix.geteuid()).toBe(1001);
  });

  test('setreuid(1000, "user2")', () => {
    posix.setreuid(1000, 'user2');

    expect(posix.getuid()).toBe(1000);
    expect(posix.geteuid()).toBe(1001);
  });

  test('setregid(1500, 1600) - Fail', () => {
    expect(() => {
      posix.setregid(1500, 1600);
    }).toThrow();
  });

  test('setregid("nope", "nothing") - Fail', () => {
    expect(() => {
      posix.setregid('nope', 'nothing');
    }).toThrow();
  });

  test('setreuid(1500, 1600) - Fail', () => {
    expect(() => {
      posix.setreuid(1500, 1600);
    }).toThrow();
  });

  test('setreuid("nope", "nothing") - Fail', () => {
    expect(() => {
      posix.setreuid('nope', 'nothing');
    }).toThrow();
  });
});
