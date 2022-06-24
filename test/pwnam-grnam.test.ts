import * as posix from '../dist';
import { test_function } from './util';

const untyped_posix = posix as any;

describe('pwnam and grnam - Failing', () => {
  test_function('pwnam', untyped_posix.pwnam, [['string', 'number']]);
  test_function('grnam', untyped_posix.grnam, [['string', 'number']]);
});

describe('pwnam and grnam - Correct', () => {
  test('getpwnam("user")', () => {
    expect(posix.getpwnam('user')).toEqual({
      name: 'user',
      passwd: 'x',
      uid: 1000,
      gid: 1000,
      gecos: 'user',
      dir: '/home/user',
      shell: '/bin/bash',
    });
  });

  test('getpwnam(1001)', () => {
    expect(posix.getpwnam(1001)).toEqual({
      name: 'user2',
      passwd: 'x',
      uid: 1001,
      gid: 1000,
      gecos: 'user2',
      dir: '/home/user2',
      shell: '/bin/bash',
    });
  });

  test('getgrnam("user")', () => {
    expect(posix.getgrnam('user')).toEqual({
      name: 'user',
      passwd: 'x',
      gid: 1000,
      members: ['user', 'user2'],
    });
  });

  test('getgrnam(1001)', () => {
    expect(posix.getgrnam(1000)).toEqual({
      name: 'user',
      passwd: 'x',
      gid: 1000,
      members: ['user', 'user2'],
    });
  });

  test('getpwnam(800) - Fail', () => {
    expect(() => posix.getpwnam(800)).toThrow();
  });

  test('getgrnam(800) - Fail', () => {
    expect(() => posix.getgrnam(800)).toThrow();
  });
});
