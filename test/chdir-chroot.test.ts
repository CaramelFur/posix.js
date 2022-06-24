import * as posix from '../dist';
import { test_function } from './util';

const untyped_posix = posix as any;

describe('chdir and chroot - Failing', () => {
  test_function('chroot', untyped_posix.chroot, [['string']]);
});

describe('chdir and chroot - Correct', () => {
  test('chroot("/test")', () => {
    expect(posix.chroot('/test')).toBeUndefined();
  });

  test('chroot("/") - Fail', () => {
    expect(() => {
      posix.chroot('/');
    }).toThrow();
  });
});
