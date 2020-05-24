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
#include <deque>

namespace EX = cxxrt::execution;
namespace NET = cxxrt::net;

// ----------------------------------------------------------------------------

namespace
{
    struct receiver
    {
        int index;
        explicit receiver(int i): index(i) {}
        void set_value() { std::cout << "receiver(" << this->index << "):set_value\n"; }
        void set_error(std::exception_ptr) { std::cout << "receiver(" << this->index << "):set_error\n"; }
        void set_done() { std::cout << "receiver(" << this->index << "):set_done\n"; }
    };
}

// ----------------------------------------------------------------------------

int main()
{
    std::cout << std::unitbuf;
    using namespace std::literals::chrono_literals;

    NET::io_context c;
    auto& timers = *new std::deque<NET::steady_timer>;
    using state_t = decltype(EX::connect(timers.back().sender_wait(), receiver(0)));
    std::deque<state_t> states;

    for (int i = 0; i != 3; ++i)
    {
        timers.emplace_back(c, std::chrono::seconds(i));
        states.emplace_back(EX::connect(timers.back().sender_wait(), receiver(i)));
        states.back().start();
    }

    c.run();
}
