# parse_it

parse_it is a generic parsing library using a functionnal c++ style. It aims to ease creation of parser by providing composable and extensible parsers.

## Requirements

* A C++20 compiler (tested on gcc 10.1.0)
* conan > 1.21
* cmake > 3.8

## Building

You can build the project by using the provided python 3 build script:

```
$ scripts/build.py
```

If you need to configure which compiler to use you can do so by specifying the CC and CXX env
variables.

```
$ CC=gcc-10.1 CXX=g++10.1 scripts/build.py
```

You can also build the project manually.

```
$ mkdir build
$ cd build
$ conan install --build=missing -s compiler.libcxx=libstdc++11 ..
$ CC=gcc-10.1 CXX=g++10.1 cmake -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake -DCMAKE_MODULE_PATH=${PWD} ..
$ cmake --build .
$ cmake --build . --target test
```
