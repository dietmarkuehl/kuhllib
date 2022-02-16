// src/examples/simple_echo_server.cpp                                -*-C++-*-
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

#include "nstd/net.hpp"
#include <iostream>
#include <thread>

using tcp_acceptor = nstd::net::basic_socket_acceptor<nstd::net::ip::tcp>;
using tcp_socket   = nstd::net::basic_stream_socket<nstd::net::ip::tcp>;
using tcp_endpoint = nstd::net::ip::basic_endpoint<nstd::net::ip::tcp>;

// ----------------------------------------------------------------------------

void run_client(::nstd::net::ip::tcp::socket stream)
{
    char buffer[1024];
    while (true)
    {
        try {
            auto size = stream.read_some(::nstd::net::buffer(buffer));
            stream.write_some(::nstd::net::buffer(buffer, size));
        }
        catch (::std::exception const&) { ::std::cout << "Error processing\n"; }
    }
}

int main()
{
    using tcp = nstd::net::ip::tcp;

    tcp::acceptor server(tcp::endpoint(nstd::net::ip::address_v4::any(), 12345));
    while (true) {
        try { ::std::thread(run_client, server.accept()).detach(); }
        catch (::std::exception const&) { ::std::cout << "Error accepting\n"; }
    }
}