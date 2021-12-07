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

#include "nstd/concepts/same_as.hpp"
#include "nstd/execution.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/net/basic_socket_acceptor.hpp"
#include "nstd/net/basic_stream_socket.hpp"
#include "nstd/net/ip/basic_endpoint.hpp"
#include "nstd/net/ip/tcp.hpp"
#include <list>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace EX = ::nstd::execution;
namespace NC = ::nstd::concepts;
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
        // starter is only usable for apps that never exit
        ~starter() {
            if (!d_container.empty()) {
              ::std::terminate();
            }
        }

        struct receiver
        {
            container*          d_container;
            container::iterator d_iterator;

            template <typename Tag>
                requires NC::same_as<Tag, EX::set_value_t> || NC::same_as<Tag, EX::set_error_t> || NC::same_as<Tag, EX::set_done_t>
            friend auto tag_invoke(Tag, receiver&& r, auto&&...)
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
        bool          d_done = false;
        char          d_buffer[1024];
        explicit client(stream_socket&& socket): d_socket(::std::move(socket)) {}
        client(client const&) = delete;
        // required to keep in the capture of a lambda passed to an algo
        client(client&& o) : 
            d_socket(::std::move(o.d_socket)),
            d_done(::std::move(o.d_done))
            // do not copy d_buffer contents
        {
        }
        client& operator=(client&&) = delete;
    };
    
    template <typename Fn>
    auto compose(Fn&& fn) {
        return EX::let_value(::std::forward<Fn>(fn));
    }
    template <typename Fn>
    auto defer(Fn&& fn) {
        return EX::let_value(EX::just(), ::std::forward<Fn>(fn));
    }
    auto read_some_write(
        client& owner,
        NN::io_context&  context) 
    {
        return 
            EX::repeat_effect_until(
                NN::async_read_some(owner.d_socket,
                                    context.scheduler(),
                                    NN::buffer(owner.d_buffer))
                | compose(
                    [&context, &owner](::std::size_t n){
                        ::std::cout << "read='" << ::std::string_view(owner.d_buffer, n) << "'\n";
                        owner.d_done = n == 0;
                        return EX::schedule(context.scheduler())
                            |  NN::async_write(owner.d_socket, NN::buffer(owner.d_buffer, n))
                            ;
                    }),
                [&owner]{ return owner.d_done; }
            );
    }
    auto run_client(starter&         outstanding,
                    NN::io_context&  context,
                    stream_socket&&  socket)
    {
        outstanding.start(
            defer(
                [&context, owner = client{::std::move(socket)}]() mutable {
                    ::std::cout << "client connected\n";
                    return read_some_write(owner, context);
                })
            | EX::then([](auto&&...){
                ::std::cout << "client disconnected\n";
            })
        );
    }
    auto run_server(starter&         outstanding,
                    NN::io_context&  context,
                    socket_acceptor& server)
    {
        return
            EX::repeat_effect(
                NN::async_accept(server, context.scheduler())
                | EX::then([&outstanding, &context](::std::error_code, stream_socket client){
                        ::std::cout << "client.is_open()=" << ::std::boolalpha << client.is_open() << "\n";
                        run_client(outstanding, context, ::std::move(client));
                    })
            );
    }
}

// ----------------------------------------------------------------------------

int main()
{
    ::std::cout << ::std::unitbuf;
#if 0
    // if the setup of the iouring fails due to a lack of kernel resources
    // use this constructor to reduce the size of the ring
    NN::io_context  context(nstd::file::ring_context::queue_size{512});
#else
    // default ring is 1024 at the time this was written
    NN::io_context  context;
#endif
    socket_acceptor server(endpoint(NI::address_v4::any(), 12345));
    starter         outstanding;

    EX::run(context, run_server(outstanding, context, server));
}