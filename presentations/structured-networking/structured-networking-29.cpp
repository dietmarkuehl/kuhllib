// examples/structured_networking.cpp                                 -*-C++-*-
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
#include "nstd/net/io_context.hpp"
#include "nstd/net/async_read_some.hpp"
#include "nstd/net/basic_socket_acceptor.hpp"
#include "nstd/net/basic_stream_socket.hpp"
#include "nstd/net/ip/basic_endpoint.hpp"
#include "nstd/net/ip/tcp.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

using namespace nstd::execution;
using namespace nstd::net;

namespace EX = ::nstd::execution;
namespace NN = ::nstd::net;
namespace NI = ::nstd::net::ip;

using socket_acceptor = NN::basic_socket_acceptor<NI::tcp>;
using stream_socket = NN::basic_stream_socket<NI::tcp>;
using endpoint = NI::basic_endpoint<NI::tcp>;
using io_scheduler = NN::io_context::scheduler_type;

auto wait_for_read() { return just(0); }

// ----------------------------------------------------------------------------

template <std::size_t N>
struct ring_buffer {
    static constexpr int      producer{0};
    static constexpr int      consumer{1};
    static constexpr uint64_t mask{N - 1};
    uint64_t next[2];
    char buffer[N];

    struct state {
        friend void tag_invoke(start_t, state&) noexcept {}
    };
    struct sender {
        using completion_signatures = EX::completion_signatures<
            set_value_t(NN::const_buffer)
            >;
        template <EX::receiver Receiver>
        friend auto tag_invoke(connect_t, sender&&, Receiver&&) {
            return state{};
        }
    };
};

struct connection
{
    stream_socket stream;
    char          buffer[4];
    bool          done{false};
    connection(stream_socket&& stream): stream(std::move(stream)) {}
    connection(connection&& other): stream(std::move(other.stream)) {}
    ~connection() { std::cout << "destroying connection\n"; }
};

void run_client(io_scheduler scheduler, stream_socket&& stream)
{
    std::cout << "accepted a client\n";
    
    sender auto s
        = just()
        | let_value([=, client = connection(std::move(stream))]() mutable {
            return when_all(
                repeat_effect_until(
                       schedule(scheduler)
                    |  async_read_some(client.stream, buffer(client.buffer))
                    |  then([&](int n){
                        std::cout << "read(" << n << ")='"
                                  << std::string_view(client.buffer, n) << "'\n";
                        client.done = n <= 0;
                        return n;
                        }),
                    [&client]{ return client.done; }),
                repeat_effect_until(
                       wait_for_read()
                    |  let_value([&](int n){
                        return schedule(scheduler)
                            |  async_write(client.stream,
                                           buffer(client.buffer, std::max(0, n)))
                            ;
                       }),
                    [&client]{ return client.done; })
                );
        })
        ;

    start_detached(std::move(s));
}

// ----------------------------------------------------------------------------

int main()
{
    std::cout << std::unitbuf;
    io_context      context;
    socket_acceptor server(endpoint(NN::ip::address_v4::any(), 12345));

    run(context,
        repeat_effect(
              schedule(context.scheduler())
            | async_accept(server)
            | then([&](std::error_code ec, stream_socket stream){
                if (!ec) {
                    run_client(context.scheduler(), std::move(stream));
                }
              })
            )
        );
}
