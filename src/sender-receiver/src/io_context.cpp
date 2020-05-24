// src/io_context.cpp                                                 -*-C++-*-
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

#include <io_context.hpp>

#include <iostream> //-dk:TOOD remove
#include <limits>
#include <stdexcept>
#include <unistd.h>

// ----------------------------------------------------------------------------

int io_context = 0;

// ----------------------------------------------------------------------------

int cxxrt::net::io_context::timer_entry::timeout() const
{
    using ms = std::chrono::milliseconds;
    auto now = std::chrono::steady_clock::now();
    return (this->d_time < now)
        ? 0
        : (1 + std::chrono::duration_cast<ms>(this->d_time - now).count());
}

// ----------------------------------------------------------------------------

cxxrt::net::io_context::io_context()
{
    int fds[2];
    if (::pipe(fds)) {
        throw std::runtime_error("failed to construct io_context");
    }
    this->d_watch.emplace_back();
    this->d_watch.back().fd     = fds[0];
    this->d_watch.back().events = POLLIN;
    this->d_notify = fds[1];
}

cxxrt::net::io_context::~io_context()
{
    ::close(this->d_watch.front().fd);
    ::close(this->d_notify);
}

// ----------------------------------------------------------------------------

cxxrt::net::io_context::count_type
cxxrt::net::io_context::run()
{
    count_type n{};
    while (this->run_one())
    {
        if (n != std::numeric_limits<count_type>::max()) {
            ++n;
        }
    }
    return n;
}

cxxrt::net::io_context::count_type
cxxrt::net::io_context::run_one()
{
    while (!this->d_timers.empty() || this->d_watch.size() != 1u)
    {
        int timeout = this->d_timers.empty()? -1: this->d_timers.top().timeout();
        int rc = ::poll(this->d_watch.data(), this->d_timers.size(), timeout);

        if (!this->d_timers.empty()
            && this->d_timers.top().d_time <= std::chrono::steady_clock::now())
        {
            auto timer = this->d_timers.top().d_timer;
            this->d_timers.pop();
            timer->notify();
            return 1u;
        }

        if (0 < rc)
        {
            if (this->d_watch[0].revents & POLLIN)
            {
                char buffer[1000];
                ::read(this->d_watch[0].fd, buffer, sizeof(buffer));
            }
            auto it = std::find_if(this->d_watch.begin() + 1, this->d_watch.end(),
                                   [](auto const& p)->bool{ return p.revents; });
            if (it != this->d_watch.end())
            {
                it->revents = 0;
                //-dk:TODO trigger notification
                std::cout << "*** WARNING: io_context is still somewhat on strike and doesn't do any work\n";
                return 1u;
            }
        }
    }
    
    return 0u;
}

// ----------------------------------------------------------------------------

void cxxrt::net::io_context::add(time_point const& time, timer_base* timer)
{
    bool schedule = this->d_timers.empty() || time < this->d_timers.top().d_time;
    this->d_timers.emplace(time, timer);
    if (schedule)
    {
        std::cout << "schedule timer\n" << std::flush;
    }
}
