import * as posix from '../dist';

const untyped_posix = posix as any;

describe('pid and pgrp - Failing', () => {
  test('getpid() - Incorrect arguments', () => {
    expect(() => {
      untyped_posix.getpid(1000);
    }).toThrow();
  });

  test('getpgrp() - Incorrect arguments', () => {
    expect(() => {
      untyped_posix.getpgrp(1000);
    }).toThrow();
  });
});

describe('pid and pgrp - Correct', () => {
  test('getpid()', () => {
    const result = posix.getpid();

    expect(result).toBeGreaterThan(0);
  });

  test('getpgrp()', () => {
    const result = posix.getpgrp();

    expect(result).toBeGreaterThan(0);
  });
});
