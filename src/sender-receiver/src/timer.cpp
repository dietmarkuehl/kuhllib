// src/timer.cpp                                                      -*-C++-*-
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
#include <chrono>
#include <exception>
#include <functional>
#include <iostream>
#include <vector>

namespace EX = cxxrt::execution;

// ----------------------------------------------------------------------------

namespace
{
    class io_context
    {
    private:
        std::vector<std::function<void()>> d_timers;
        
    public:
        void run() {
            for (auto&& timer: this->d_timers) {
                timer();
            }
        }
        template <typename T>
        void add_timer(T&& t) { this->d_timers.emplace_back(std::forward<T>(t)); }
    };
}

// ----------------------------------------------------------------------------

namespace
{
    class steady_timer
    {
    public:
        using clock_type = std::chrono::steady_clock;
        using duration   = clock_type::duration;

    private:
        io_context* d_context;
        duration    d_duration;
        
    public:
        steady_timer(io_context& context): d_context(&context) {}
        void expires_after(const duration& d) { this->d_duration = d; }

        template <typename R>
        struct state
        {
            R d_r;
            state(R&& r, io_context* context)
                : d_r(std::forward<R>(r))
            {
                context->add_timer([this]{ this->start(); });
            }
            ~state() { std::cout << "state::~state()\n"; }
            void operator= (state&&) = delete;
            void start() noexcept { EX::set_value(std::move(this->d_r)); }
        };
        struct sender
            : EX::sender_base
        {
            io_context* d_context;
            duration    d_duration;

            template <typename R>
            state<R> connect(R&& r) { return { std::move(r), this->d_context }; }
        };
        sender sender_wait() { return sender{{}, this->d_context, this->d_duration}; }
    };
}

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

    io_context c;
    steady_timer t(c);
    t.expires_after(5s);
    auto state = EX::connect(t.sender_wait(), receiver());
    c.run();
}
