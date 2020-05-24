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
#include <socket.hpp>

#include <iostream>
#include <string>
#include <cstdlib>

namespace NET = cxxrt::net;

// ----------------------------------------------------------------------------

int main(int ac, char* av[])
{
    using endpoint = NET::ip::basic_endpoint<NET::ip::tcp>;
    using socket   = NET::basic_stream_socket<NET::ip::tcp>;

    if (ac != 3)
    {
        std::cerr << "usage: " << av[0] << " <address> <port>\n";
        return EXIT_FAILURE;
    }

    NET::io_context context;
    socket          sock(context);
    endpoint        ep(NET::ip::make_address_v4(av[1]), std::stoi(av[2]));
    // should really resolve the address
    std::cout << "connecting to " << ep << '\n'; 

    //auto sender = sock.sender_connect(ep);
    //     | then([](auto const& ep){
    //               std::cout << "connected ep=" << ep << "\n";
    //            })
    //     ;

    std::cout << "running context\n";
    context.run();
}

