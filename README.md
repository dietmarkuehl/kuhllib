# kuhllib

This repository is intended to contain yet another implementation
of the standard C++ library. This is work in progress, intended
primarily to provide an improved interface for the standard algorithms
and a fast implementation of the I/O streams and locales library.
While prototypes for most of the components planned to be implemented
exist, many of them aren't in the repository.

## Build

You can just use `make` in the current directory. It will invoke `cmake`,
build the code, and run tests. To have more control over the build-process
you can use `cmake` with the `CMakeLists.txt` in the `src` directory.
