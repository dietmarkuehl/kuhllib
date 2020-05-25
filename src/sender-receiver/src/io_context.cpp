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

#include <ostream>
#include <limits>
#include <stdexcept>
#include <unistd.h>

// ----------------------------------------------------------------------------

std::ostream& cxxrt::net::io_context::format_poll::print(std::ostream& out, short e)
{
    out << "POLL(";
    (e & POLLIN) && out << "IN(" << POLLIN << ") ";
    (e & POLLPRI) && out << "PRI(" << POLLPRI << ") ";
    (e & POLLOUT) && out << "OUT(" << POLLOUT << ") ";
    (e & POLLERR) && out << "ERR(" << POLLERR << ") ";
    (e & POLLHUP) && out << "HUP(" << POLLHUP << ") ";
    (e & POLLNVAL) && out << "NVAL(" << POLLNVAL << ") ";
    (e & POLLRDNORM) && out << "RDNORM(" << POLLRDNORM << ") ";
    (e & POLLRDBAND) && out << "RDBAND(" << POLLRDBAND << ") ";
    (e & POLLWRNORM) && out << "WRNORM(" << POLLWRNORM << ") ";
    (e & POLLWRBAND) && out << "WRBAND(" << POLLWRBAND << ") ";
    return out << ')';
}

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

void cxxrt::net::io_context::wake_up()
{
    ::write(this->d_notify_fd, "", 1);
}

bool cxxrt::net::io_context::do_notify(int fd, short events)
{
    // The pipe was signaled to wake this thread up: extract some bytes.
    if (events & POLLIN)
    {
        char buffer[1000];
        ::read(fd, buffer, sizeof(buffer));
    }
    return false;
}

// ----------------------------------------------------------------------------

cxxrt::net::io_context::io_context()
{
    int fds[2];
    if (::pipe(fds)) {
        throw std::runtime_error("failed to construct io_context");
    }
    this->d_notify_fd = fds[1];
    this->add(fds[0], POLLIN, this);
}

cxxrt::net::io_context::~io_context()
{
    ::close(this->d_watch.front().fd);
    ::close(this->d_notify_fd);
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
        int rc = ::poll(this->d_watch.data(), this->d_watch.size(), timeout);

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
            for (auto it = this->d_watch.begin();
                 this->d_watch.end() != (it = std::find_if(it, this->d_watch.end(),
                                                         [](auto const& p)->bool{ return p.revents; })); )
            {
                std::size_t index(it - this->d_watch.begin());
                if (this->d_waiters[index]->notify(it->fd, it->revents))
                {
                    if (index + 1 != this->d_waiters.size())
                    {
                        *it = this->d_watch.back();
                        this->d_waiters[index] = this->d_waiters.back();
                    }
                    this->d_waiters.pop_back();
                    this->d_watch.pop_back();
                }
                else
                {
                    it->revents = 0;
                }
                if (index != 0u)
                {
                    return 1u;
                }
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
        this->wake_up();
    }
}

void cxxrt::net::io_context::add(int fd, short events, cxxrt::net::detail::waiter* w)
{
    this->d_watch.emplace_back();
    this->d_watch.back().fd     = fd;
    this->d_watch.back().events = events;
    try
    {
        this->d_waiters.emplace_back(w);
    }
    catch (...)
    {
        this->d_watch.pop_back();
        throw;
    }
    this->wake_up();
}
