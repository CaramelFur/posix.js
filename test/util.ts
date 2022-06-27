const possible_parameters = {
  string: 'test',
  number: 3453448,
  boolean: true,
  object: {
    a: 'b',
    toString: 1,
  },
  array: [1, 2, 3],
};

export function test_function(
  name: string,
  f: Function,
  params: Array<Array<keyof typeof possible_parameters>>,
) {
  let possible_args: any[][] = [];
  for (let i = 0; i < 5; i++) {
    if (i === 0 && params.length === 0) continue;

    for (let j = 0; j < (i + 1) ** 3; j++) {
      let possible_arg: any[] = [];
      for (let k = 0; k < i; k++) {
        let possible_values: any;

        if (i == params.length)
          possible_values = Object.entries(possible_parameters)
            .filter(([key, value]) => !params[k].includes(key as any))
            .map(([key, value]) => value);
        else possible_values = Object.values(possible_parameters);

        if (possible_values.length === 0) continue;

        possible_arg.push(
          possible_values[Math.floor(Math.random() * possible_values.length)],
        );
      }
      possible_args.push(possible_arg);
    }
  }

  test(`${name} - Incorrect arguments`, () => {
    for (let i = 0; i < possible_args.length; i++) {
      let possible_arg = possible_args[i];

      expect(() => {
        try {
          f(...possible_arg);
        } catch (e: any) {
          if (e.name === 'TypeError') {
            throw new TypeError(e.message);
          } else {
            throw e;
          }
        }
      }).toThrowError(TypeError);
    }
  });
}
