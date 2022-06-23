import * as posix from '../dist';


const untyped_posix = posix as any;

describe('euid and egid - Failing', () => {
  test('getegid() - Incorrect arguments', () => {
    expect(() => {
      untyped_posix.getegid(1000);
    }).toThrow();
  });

  test('geteuid() - Incorrect arguments', () => {
    expect(() => {
      untyped_posix.geteuid(1000);
    }).toThrow();
  });

  test('setegid() - Incorrect arguments', () => {
    expect(() => {
      untyped_posix.setegid();
    }).toThrow();

    expect(() => {
      untyped_posix.setegid(1000, 1000);
    }).toThrow();
  });

  test('seteuid() - Incorrect arguments', () => {
    expect(() => {
      untyped_posix.seteuid();
    }).toThrow();

    expect(() => {
      untyped_posix.seteuid(1000, 1000);
    }).toThrow();
  });
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

  test('setgid()', () => {
    posix.setegid(1000);

    const result = posix.getegid();

    expect(result).toBe(1000);
  });

  test('setuid()', () => {
    posix.seteuid(1000);

    let result = posix.geteuid();

    expect(result).toBe(1000);
  });
});