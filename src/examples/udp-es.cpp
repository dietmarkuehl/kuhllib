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
#include "nstd/net.hpp"
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

using protocol = NI::udp;
using datagram_socket = NN::basic_datagram_socket<protocol>;
using endpoint = NI::basic_endpoint<protocol>;

// ----------------------------------------------------------------------------

int main()
{
    ::std::cout << ::std::unitbuf;
    NN::scope scope;
    auto stop = [&scope]{ scope.stop(); };

    endpoint ep(NI::address_v4::any(), 12345);
    datagram_socket socket(ep);
    char buffer[16];

    scope.start(
        EX::repeat_effect(
              NN::async_receive_from(socket, NN::buffer(buffer))
            | EX::let_value([&socket, &buffer, stop](::std::size_t n, auto&& endpoint){
                return NN::async_send_to(socket, NN::buffer(buffer, n), endpoint)
                     | EX::then([&buffer, stop](std::size_t n) {
                        if (::std::string_view(buffer, n).starts_with("stop")) {
                            stop();
                        }
                     })
                     ;
            })
        )
    );
    ::std::cout << "ready to echo\n";
    scope.run();
    ::std::cout << "done\n";
}