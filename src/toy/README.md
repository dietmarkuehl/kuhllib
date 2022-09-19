# Toy Networking

This gist is a toy sender/receiver networking implementation.  It
is intended as a learning tool only.  __This code is not good for
_any_ use!__ It was inspired by Eric Niebler's ["Working with
Asynchrony Generally"](https://www.youtube.com/watch?v=xiaqNvqRB2E)
at CppEurope 2022 building a basic executor framework. At CppCon
2022 I gave a similar presentation building senders for networking.
The corresponding code is in [`toy-cppcon-2022.cpp`](toy-cppcon-2022.cpp)
(when implementing the code live at CppCon 2022 I incorrectly used
`0` as the third argument to `poll()`: while this works it causes
busy waiting; this error is corrected here).

## Building

The code should build on POSIX systems using `make`. By default
this will produce `toy-netowrking` implementing a basic echo
server.
