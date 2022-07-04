import * as posix from '../dist';
import { test_function } from './util';

const untyped_posix = posix as any;

describe('rlimit - Failing', () => {
  test_function('rlimit', untyped_posix.getrlimit, [['string', 'number']]);

  test_function('setrlimit', untyped_posix.setrlimit, [
    ['string', 'number'],
    ['number'],
  ]);
});

describe('rlimit - Correct', () => {
  test('getrlimit("cpu")', () => {
    expect(posix.getrlimit('cpu')).toEqual({
      soft: null,
      hard: null,
    });
  });

  test('getrlimit(8)', () => {
    expect(posix.getrlimit(8)).toEqual({
      soft: null,
      hard: null,
    });
  });

  test('setrlimit("cpu")', () => {
    expect(posix.getrlimit('cpu')).toEqual({
      soft: null,
      hard: null,
    });

    posix.setrlimit('cpu', { soft: 1, hard: 2 });

    expect(posix.getrlimit('cpu')).toEqual({
      soft: 1,
      hard: 2,
    });

    posix.setrlimit('cpu', { hard: 4 });

    expect(posix.getrlimit('cpu')).toEqual({
      soft: 1,
      hard: 4,
    });
  });

  test('setrlimit(8)', () => {
    expect(posix.getrlimit(8)).toEqual({
      soft: null,
      hard: null,
    });

    posix.setrlimit(8, { soft: 5 });

    expect(posix.getrlimit(8)).toEqual({
      soft: 5,
      hard: null,
    });

    posix.setrlimit(8, { hard: 4 });

    expect(posix.getrlimit(8)).toEqual({
      soft: 5,
      hard: 4,
    });

    posix.setrlimit(8, { soft: null });

    expect(posix.getrlimit(8)).toEqual({
      soft: null,
      hard: 4,
    });
  });

  test('setrlimit("nothing") - Fail', () => {
    expect(() => {
      untyped_posix.setrlimit('nothing', { soft: 5, hard: 4 });
    }).toThrow();
  });

  test('setrlimit(200) - Fail', () => {
    expect(() => {
      untyped_posix.setrlimit(200, { soft: 5, hard: 4 });
    }).toThrow();
  });
});
