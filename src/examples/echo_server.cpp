// examples/echo_server.cpp                                           -*-C++-*-
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

#include <iostream>

#include "../nstd/execution/run.hpp"
#include "../nstd/execution/connect.hpp"
#include "../nstd/execution/start.hpp"
#include "../nstd/execution/sender.hpp"
#include "../nstd/execution/then.hpp"
#include "../nstd/net/io_context.hpp"
#include "../nstd/net/basic_socket_acceptor.hpp"
#include "../nstd/net/basic_stream_socket.hpp"
#include "../nstd/net/ip/basic_endpoint.hpp"
#include "../nstd/net/ip/tcp.hpp"
#include <list>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace EX = ::nstd::execution;
namespace NN = ::nstd::net;
namespace NI = ::nstd::net::ip;

using socket_acceptor = NN::basic_socket_acceptor<NI::tcp>;
using stream_socket = NN::basic_stream_socket<NI::tcp>;
using endpoint = NI::basic_endpoint<NI::tcp>;

// ----------------------------------------------------------------------------

namespace
{
    struct starter
    {
        struct state_base { virtual ~state_base() = default; };
        using container = ::std::list<::std::unique_ptr<state_base>>;
        container d_container;

        struct receiver
        {
            container*          d_container;
            container::iterator d_iterator;

            friend auto tag_invoke(auto, receiver&& r, auto&&...)
                noexcept -> void
            {
                r.d_container->erase(r.d_iterator);
            }
        };
        template <typename Sender>
        struct state: state_base {
            using operation = decltype(EX::connect(::std::declval<Sender>(),
                                                   ::std::declval<receiver>()));
            operation d_state;
            template <typename S>
            state(container* c, container::iterator it, S&& sender)
                : d_state(EX::connect(::std::forward<S>(sender), receiver{c, it}))
            {
                EX::start(this->d_state);
            }
        };
        template <EX::sender Sender>
        auto start(Sender&& sender) -> void {
            container::iterator it = this->d_container.emplace(this->d_container.begin());
            *it = ::std::make_unique<state<Sender>>(&this->d_container, it, ::std::forward<Sender>(sender));
        }
    };
}

// ----------------------------------------------------------------------------

namespace
{
    struct client
    {
        stream_socket d_socket;
        char          d_buffer[1024];
        client(stream_socket&& socket): d_socket(::std::move(socket)) {
            ::std::cout << "client connected\n";
        }
        ~client() {
            ::std::cout << "client disconnected\n";
        }
        static auto read(starter& outstanding, NN::io_context& context, ::std::unique_ptr<client>&& ptr)
            -> void
        {
            client& cl(*ptr);
            outstanding.start(
                NN::async_read_some(cl.d_socket,
                                    context.scheduler(),
                                 NN::buffer(cl.d_buffer))
                | EX::then([&outstanding, &context, ptr = ::std::move(ptr)](::std::size_t n) mutable {
                    if (n != 0u)
                    {
                        ::std::string_view view(ptr->d_buffer, n);
                        client::write(outstanding, context, ::std::move(ptr), n);
                    }
                    })
                );

        }
        static auto write(starter& outstanding, NN::io_context& context, ::std::unique_ptr<client>&& ptr, ::std::size_t n)
            -> void
        {
            client& cl(*ptr);
            outstanding.start(
                NN::async_write_some(cl.d_socket,
                                     context.scheduler(),
                                     NN::buffer(cl.d_buffer, n))
                | EX::then([&outstanding, &context, ptr = ::std::move(ptr)](::std::size_t) mutable {
                    client::read(outstanding, context, ::std::move(ptr));
                    })
                );
        }
    };

    auto run_client(starter&         outstanding,
                    NN::io_context&  context,
                    stream_socket&&  socket)
    {
        client::read(outstanding, context, ::std::make_unique<client>(::std::move(socket)));
    }

    auto run_accept(starter&         outstanding,
                    NN::io_context&  context,
                    socket_acceptor& server) -> void
    {
        static ::std::string welcome("welcome\n");
        outstanding.start(
              NN::async_accept(server, context.scheduler())
            | EX::then([&outstanding, &context](stream_socket client){
                    outstanding.start(NN::async_write_some(client, context.scheduler(), NN::buffer(welcome)));
                    return client;
                })
            | EX::then([&outstanding, &context, &server](stream_socket client){
                    run_accept(outstanding, context, server);
                    return client;
                })
            | EX::then([&outstanding, &context](stream_socket client){
                    run_client(outstanding, context, ::std::move(client));
                })
            );
    }
}

// ----------------------------------------------------------------------------

int main()
{
    NN::io_context  context;
    socket_acceptor server(endpoint(NI::address_v4::any(), 12345));
    starter         outstanding;

    run_accept(outstanding, context, server);
    context.run();
}