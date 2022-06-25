import * as posix from '../dist';
import { test_function } from './util';

const untyped_posix = posix as any;

describe('groups - Failing', () => {
  test_function('initgroups', untyped_posix.initgroups, [
    ['string'],
    ['string', 'number'],
  ]);
  test_function('getgroups', untyped_posix.getgroups, []);
  test_function('setgroups', untyped_posix.setgroups, [['array']]);
});

describe('groups - Correct', () => {
  test('getgroups()', () => {
    expect(posix.getgroups()).toEqual([1000]);
  });

  test('setgroups([1000, 1001])', () => {
    posix.setgroups([1000, 1001]);
    expect(posix.getgroups()).toEqual([1000, 1001]);
  });

  test('setgroups(["user", "group2"])', () => {
    posix.setgroups(['user', 'group2']);
    expect(posix.getgroups()).toEqual([1000, 1001]);
  });

  test('setgroups(["user", 1001])', () => {
    posix.setgroups(['user', 1001]);
    expect(posix.getgroups()).toEqual([1000, 1001]);
  });

  test('initgroups("user", 1001)', () => {
    posix.initgroups('user', 1001);
    expect(posix.getgroups()).toEqual([1000, 1001]);
  });

  test('initgroups("user", "group2")', () => {
    posix.initgroups('user', 'group2');
    expect(posix.getgroups()).toEqual([1000, 1001]);
  });
});
