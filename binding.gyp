{
  "targets": [
    {
      "target_name": "posix_addon",
      "cflags!": [ "-O3 -march=native -mtune=native -Wall -Wextra -pedantic-errors -fno-exceptions" ],
      "cflags_cc!": [ "-O3 -march=native -mtune=native -Wall -Wextra -pedantic-errors -fno-exceptions" ],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "CLANG_CXX_LANGUAGE_STANDARD": "c++2a"
      },
      "msvs_settings": {
        "VCCLCompilerTool": { 
          "ExceptionHandling": 1, 
          "AdditionalOptions": [ "-std:c++2a" ] 
        }
      },
      "sources": [ "src/posix.cpp", "src/*.h" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    },
    {
      "target_name": "posix_addon_mocked",
      "cflags!": [ "-g -Wall -Wextra -pedantic-errors -fno-exceptions" ],
      "cflags_cc!": [ "-g -Wall -Wextra -pedantic-errors -fno-exceptions" ],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "CLANG_CXX_LANGUAGE_STANDARD": "c++2a"
      },
      "msvs_settings": {
        "VCCLCompilerTool": { 
          "ExceptionHandling": 1, 
          "AdditionalOptions": [ "-std:c++2a" ] 
        }
      },
      "sources": [ "src/posix.cpp", "src/posix-mock.cpp", "src/*.h" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS', 'MOCK_POSIX' ],
    }
  ],
}
