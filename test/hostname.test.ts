import * as posix from '../dist';
import { test_function } from './util';

const untyped_posix = posix as any;

describe('hostname - Failing', () => {
  test_function('gethostname', untyped_posix.gethostname, []);
  test_function('sethostname', untyped_posix.sethostname, [['string']]);
});

describe('hostname - Correct', () => {
  test('gethostname()', () => {
    expect(posix.gethostname()).toEqual('localhost');
  });

  test('sethostname("machine")', () => {
    posix.sethostname('machine');
    expect(posix.gethostname()).toEqual('machine');
  });

  test('sethostname(verylongstring) - Fail', () => {
    expect(() => {
      posix.sethostname(''.padStart(280, 'a'));
    }).toThrow();

    expect(posix.gethostname()).toEqual('localhost');
  });
});
