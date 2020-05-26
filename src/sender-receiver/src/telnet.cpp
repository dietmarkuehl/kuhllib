// src/telnet.cpp                                                     -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2020 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include <internet.hpp>
#include <io_context.hpp>
#include <io_operation.hpp>
#include <socket.hpp>
#include <execution.hpp>
#include <sender.hpp>

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <system_error>

namespace NET = cxxrt::net;
namespace EX  = cxxrt::execution;

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------


template <typename Protocol, typename Receiver>
void async_connect(NET::basic_stream_socket<Protocol>*      socket,
                   NET::ip::basic_endpoint<Protocol> const& ep,
                   Receiver&&                               receiver)
{
    cxxrt::net::io_operation<Receiver, cxxrt::net::connect_operation> op(std::forward<Receiver>(receiver));
    op.connect(socket, ep);
}

// ----------------------------------------------------------------------------

namespace
{
    struct resolve_receiver
    {
        template <typename P>
        void set_value(NET::ip::basic_endpoint<P> const& e)
        {
            std::cout << "resolved(" << e << ")\n";
        }
        void set_error(std::exception_ptr const&){ std::cout << "resolve threw\n"; }
        void set_done() { std::cout << "resolve canceled\n"; }
    };

    struct connect_receiver
    {
        template <typename... A>
        void set_value(A&&...) { std::cout << "connect_receiver::connected XXX\n"; }
        void set_value() { std::cout << "connect_receiver::connected\n"; }
        void set_error(std::error_code const& ec) noexcept
        {
            std::cout << "connect failure: " << std::strerror(ec.value()) << "\n";
        }
        void set_error(std::exception_ptr const&) noexcept { std::cout << "connect threw\n"; }
        void set_done() noexcept { std::cout << "connect canceled\n"; }
    };
    static_assert(EX::receiver<connect_receiver>);
    static_assert(EX::receiver<connect_receiver, std::error_code>);
}

// ----------------------------------------------------------------------------

int main(int ac, char* av[])
{
    std::cout << std::unitbuf;
    using endpoint = NET::ip::basic_endpoint<NET::ip::tcp>;
    using socket   = NET::basic_stream_socket<NET::ip::tcp>;

    if (ac != 3)
    {
        std::cerr << "usage: " << av[0] << " <address> <port>\n";
        return EXIT_FAILURE;
    }

    NET::io_context context;
    socket          sock(context);
    endpoint        addr(NET::ip::make_address_v4(av[1]), std::stoi(av[2]));

    //char const hello[] = { 'h', 'e', 'l', 'l', 'o', '\n' };
    auto s0
        = EX::just(addr) // this should really resolve
        | EX::then([](auto&& ep){
                std::cout << "resolved(" << ep << ")\n";
                return std::move(ep);
            })
        | async_connect(sock)
        //| sock.async_send(hello)
        ;
    auto st = EX::connect(std::move(s0), connect_receiver());
    st.start();

    std::cout << "running context\n";
    context.run();
    std::cout << "done running context\n";
}

