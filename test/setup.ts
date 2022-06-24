process.env.MOCK_POSIX = '1';

import * as posix from '../dist';
const untyped_posix = posix as any;

beforeEach(() => {
  untyped_posix.mock_reset();
});
