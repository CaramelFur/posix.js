{
  "targets": [
    {
      "target_name": "posix_addon",
      "cflags!": [ "-O3 -march=native -mtune=native -Wall -Wextra -pedantic-errors -fno-exceptions" ],
      "cflags_cc!": [ "-O3 -march=native -mtune=native -Wall -Wextra -pedantic-errors -fno-exceptions" ],
      "sources": [ "src/posix.cpp" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
