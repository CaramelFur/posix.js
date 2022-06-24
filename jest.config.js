/** @type {import('ts-jest/dist/types').InitialOptionsTsJest} */
module.exports = {
  preset: 'ts-jest',
  testEnvironment: 'node',
  reporters: [
    'default',
    ['jest-junit', { outputDirectory: './reports/junit' }],
  ],
  setupFiles: ['./test/setup.ts'],
  automock: false,
  clearMocks: true,
  restoreMocks: true,
};
