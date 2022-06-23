import * as posix  from '../dist';

test('getuid()', () => {
  const result = posix.getuid();

  expect(result).toBe(1000);
});