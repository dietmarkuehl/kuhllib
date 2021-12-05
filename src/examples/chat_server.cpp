// src/examples/chat_server.cpp                                       -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/net/io_context.hpp"
#include "nstd/net/basic_socket_acceptor.hpp"
#include "nstd/net/basic_stream_socket.hpp"
#include "nstd/net/ip/basic_endpoint.hpp"
#include "nstd/net/ip/address_v4.hpp"
#include "nstd/net/ip/tcp.hpp"
#include "nstd/execution/run.hpp"
#include "nstd/execution/repeat_effect.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/execution/start_detached.hpp"
#include "nstd/execution/just.hpp"

#include <iostream>
#include <list>
#include <vector>
#include <utility>

namespace EX = ::nstd::execution;
namespace NN = ::nstd::net;
namespace NI = ::nstd::net::ip;

using socket_acceptor = NN::basic_socket_acceptor<NI::tcp>;
using stream_socket = NN::basic_stream_socket<NI::tcp>;
using endpoint = NI::basic_endpoint<NI::tcp>;

// ----------------------------------------------------------------------------

namespace {
    class clients
    {
    private:
        using container = ::std::list<::std::pair<stream_socket, bool>>;
        container d_clients;

        auto remove_client(container::iterator it) -> void {
            this->d_clients.erase(it);
            ::std::cout << "removed client\n" << ::std::flush;
        }
        auto distribute(auto scheduler, ::std::vector<char> buffer) {
            for (auto& client: this->d_clients) {
                EX::start_detached(EX::let_value(EX::just(), [&client, scheduler, buffer]{
                    return NN::async_write(client.first, scheduler, NN::buffer(buffer))
                        |  EX::then([](auto&&...){})
                        ;
                }));
            }
        }
    public:
        clients() = default;
        clients(clients&&) = delete;
        auto add_client(stream_socket client, auto scheduler) -> void {
            ::std::cout << "adding client\n" << ::std::flush;
            this->d_clients.emplace_back(::std::move(client), false);
            EX::start_detached(
                EX::repeat_effect_until(
                    EX::let_value(EX::just(),
                        [this,
                        &client = this->d_clients.back().first,
                        &done = this->d_clients.back().second,
                        buffer = ::std::vector<char>(1024),
                        scheduler]() mutable {
                            return NN::async_read_some(client, scheduler, NN::buffer(buffer))
                                |  EX::then([this, &buffer, &done, scheduler](::std::size_t n){
                                    ::std::cout << "read='" << ::std::string_view(buffer.data(), n) << "'\n";
                                    if (n != 0u) {
                                        buffer.resize(n);
                                        this->distribute(scheduler, ::std::move(buffer));
                                    }
                                    else {
                                        done = true;
                                    }
                                })
                                ;
                        }
                    )
                    , [&done = this->d_clients.back().second]{ return done; }
                )
                | EX::then([this, it = --this->d_clients.end()]{
                    this->remove_client(it);
                })
            );
            ::std::cout << "added client\n" << ::std::flush;
        }
    };
}

// ----------------------------------------------------------------------------

namespace {
    auto run_server(clients& cs, NN::io_context::scheduler_type scheduler, socket_acceptor& server) {
        return EX::repeat_effect(
                NN::async_accept(server, scheduler)
                | EX::then([&cs, scheduler](::std::error_code, stream_socket client) {
                    ::std::cout << "accepted a new client\n";
                    cs.add_client(::std::move(client), scheduler);
                  })
               );
    }
}

// ----------------------------------------------------------------------------

int main()
{
    ::std::cout << ::std::unitbuf;

    int const port(12345);
    socket_acceptor server(endpoint(NI::address_v4::any(), port));
    clients         cs;
    ::std::cout << "using port=" << port << "\n";

    NN::io_context context(nstd::file::ring_context::queue_size{64});
    EX::run(context, run_server(cs, context.scheduler(), server));

    ::std::cout << "done\n";
}
