import * as posix from '../dist';
import { test_function } from './util';

const untyped_posix = posix as any;

describe('syslog - Failing', () => {
  test_function('openlog', untyped_posix.openlog, [
    ['string'],
    ['array'],
    ['string'],
  ]);

  test_function('closelog', untyped_posix.closelog, []);

  test_function('syslog', untyped_posix.syslog, [['string'], ['string']]);

  test_function('setlogmask', untyped_posix.setlogmask, [['array']]);
});

describe('syslog - Correct', () => {
  test('openlog()', () => {
    posix.openlog('test', ['pid'], 'daemon');

    const syslog = untyped_posix.mock_getsyslog() as string;

    console.log('pog', syslog.toString());

    expect(syslog).toEqual('OPENED_LOG test 1 24\n');
  });

  test('closelog()', () => {
    posix.closelog();

    const syslog = untyped_posix.mock_getsyslog() as string;

    expect(syslog).toEqual('CLOSED_LOG\n');
  });

  test('syslog()', () => {
    posix.openlog('test', ['pid'], 'daemon');

    posix.syslog('info', 'message');

    const syslog = untyped_posix.mock_getsyslog() as string;

    expect(syslog).toEqual('OPENED_LOG test 1 24\ntest 6: message\n');
  });

  test('setlogmask()', () => {
    posix.openlog('test', ['pid'], 'daemon');

    const prevmask = posix.setlogmask(['err']);

    expect(prevmask).toBe(0);

    posix.syslog('info', 'message one');
    posix.syslog('err', 'message two');

    const syslog = untyped_posix.mock_getsyslog() as string;

    expect(syslog).toEqual('OPENED_LOG test 1 24\ntest 3: message two\n');
  });
});
