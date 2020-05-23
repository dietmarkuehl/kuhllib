// src/run-timer.cpp                                                  -*-C++-*-
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

#include <execution.hpp>
#include <io_context.hpp>
#include <timer.hpp>

#include <chrono>
#include <exception>
#include <functional>
#include <iostream>
#include <vector>

namespace EX = cxxrt::execution;
namespace NET = cxxrt::net;

// ----------------------------------------------------------------------------

namespace
{
    struct receiver
    {
        void set_value() { std::cout << "set_value\n"; }
        void set_error(std::exception_ptr) { std::cout << "set_error\n"; }
        void set_done() { std::cout << "set_done\n"; }
    };
}

// ----------------------------------------------------------------------------

int main()
{
    using namespace std::literals::chrono_literals;

    NET::io_context c;
    NET::steady_timer t(c, 5s);
    auto state = EX::connect(t.sender_wait(), receiver());
    state.start();

    c.run();
}
