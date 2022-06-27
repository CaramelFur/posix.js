import * as posix from '../dist';
import { test_function } from './util';

const untyped_posix = posix as any;

describe('swap - Failing', () => {
  test_function('swapon', untyped_posix.swapon, [['string'], ['array']]);
  test_function('swapoff', untyped_posix.swapoff, [['string']]);
});

describe('swap - Correct', () => {
  test('swapon()', () => {
    posix.swapon('/dev/swap', []);

    posix.swapon('/dev/swap2', ['prefer']);
  });

  test('swapoff()', () => {
    posix.swapon('/dev/swap', []);

    posix.swapon('/dev/swap2', ['prefer']);

    posix.swapoff('/dev/swap');

    posix.swapon('/dev/swap3', []);

    posix.swapoff('/dev/swap2');

    posix.swapoff('/dev/swap3');
  });

  test('swapoff() - Fail', () => {
    expect(() => {
      posix.swapoff('/dev/swap');
    }).toThrow();
  });

  test('swapon() - Fail', () => {
    expect(() => {
      for (let i = 0; i < 64; i++) {
        posix.swapon('/dev/swap' + i, []);
      }
    }).toThrow();
  });
});
