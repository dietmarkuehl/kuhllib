// examples/std-io-to-tty.cpp                                         -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2023 Dietmar Kuehl http://www.dietmar-kuehl.de
//
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without restriction,
//  including without limitation the rights to use, copy, modify,
//  merge, publish, distribute, sublicense, and/or sell copies of
//  the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "nstd/execution.hpp"
#include "nstd/file.hpp"
#include "nstd/net.hpp"
#include <string>
#include <cstdlib>
#include <fcntl.h>

namespace EX = nstd::execution;
namespace NF = nstd::file;
namespace NN = nstd::net;

namespace {
    template <NF::io_object In, std::size_t N, NF::io_object Out>
    auto copy_stream(In& in, char (&buffer)[N], Out& out) {
        return EX::repeat_effect(
            NN::async_read_some(in, NN::buffer(buffer))
            | EX::let_value([&](std::size_t n){
                return NN::async_write(out, NN::buffer(buffer, n));
            })
        );
    }
}

int main(int ac, char* av[]) {
    constexpr std::size_t buffer_size{16};

    NN::scope      scope;
    NF::stream     stdin(0);
    NF::stream     stdout(1);
    char           to_tty[buffer_size];
    char           from_tty[buffer_size];

    if (ac < 2) {
        std::string message("usage: " + std::string(av[0]) + " <tty>");
        scope.start(NN::async_write_some(stdout, NN::buffer(message)));
        scope.run();
        return EXIT_FAILURE;
    }

    NF::stream tty(::open(av[1], O_RDWR | O_NONBLOCK));

    scope.start(copy_stream(stdin, to_tty,   tty));
    scope.start(copy_stream(tty,   from_tty, stdout));

    scope.run();
}
