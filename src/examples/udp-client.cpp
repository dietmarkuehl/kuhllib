// examples/udp-client.cpp                                            -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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
#include "nstd/hidden_names/log_completion.hpp"
#include "nstd/hidden_names/print_completion_signatures.hpp"
#include "nstd/net.hpp"
#include "nstd/thread/sync_wait.hpp"
#include <iostream>

namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace NF = ::nstd::file;
namespace NN = ::nstd::net;

// ----------------------------------------------------------------------------

int main() {
    NN::scope scope;
    auto stop = [&scope]{ scope.stop(); };

    char buffer[128];
    NF::stream stdin(0);
    char datagram[128];
    NN::basic_datagram_socket<NN::ip::udp> socket(NN::ip::udp::v4());
    NN::ip::basic_endpoint<NN::ip::udp>    endpoint(NN::ip::address_v4::any(), 12345);

    scope.start(
        EX::repeat_effect(
              NN::async_read_some(stdin, NN::buffer(buffer))
            | EX::let_value([&buffer, &endpoint, &socket, stop](int n){
                if (n == 0 || ::std::string_view(buffer, n).starts_with("lstop")) {
                    std::cout << "stopping client\n";
                    stop();
                }
                return NN::async_send_to(socket, NN::buffer(buffer, n), endpoint)
                    |  HN::log_completion("--> async_send_to(stdin)")
                    ;
            })
            | HN::log_completion("--> let_value(stdin)")
        )
        | HN::log_completion("--> repeat_effect(stdin)")
    );
    scope.start(
        EX::repeat_effect(
              NN::async_receive_from(socket, NN::buffer(datagram))
            | HN::log_completion("----> async_receive_from")
            | EX::then([&datagram](int n, auto endpoint){
                ::std::string_view sv(datagram, 0 < n && datagram[n-1] == '\n'? n-1: n);
                ::std::cout << "received='" << sv << "' from " << endpoint << "\n";
            })
            | HN::log_completion("async_receive_from/then")
        )
        | HN::log_completion("--> repeat_effect(socket)")
    );

    scope.run();
    while (scope.run_one()) {
        std::cout << "ran one\n";
    }
}
