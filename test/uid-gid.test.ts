import * as posix from '../dist';

const untyped_posix = posix as any;

describe('uid and gid - Failing', () => {
  test('getgid() - Incorrect arguments', () => {
    expect(() => {
      untyped_posix.getgid(1000);
    }).toThrow();
  });

  test('getuid() - Incorrect arguments', () => {
    expect(() => {
      untyped_posix.getuid(1000);
    }).toThrow();
  });

  test('setgid() - Incorrect arguments', () => {
    expect(() => {
      untyped_posix.setgid();
    }).toThrow();

    expect(() => {
      untyped_posix.setgid(1000, 1000);
    }).toThrow();
  });

  test('setuid() - Incorrect arguments', () => {
    expect(() => {
      untyped_posix.setuid();
    }).toThrow();

    expect(() => {
      untyped_posix.setuid(1000, 1000);
    }).toThrow();
  });
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

  test('setgid(0)', () => {
    posix.setgid(0);

    const result = posix.getgid();

    expect(result).toBe(0);
  });

  test('setuid(0)', () => {
    posix.setuid(0);

    let result = posix.getuid();

    expect(result).toBe(0);
  });
});
