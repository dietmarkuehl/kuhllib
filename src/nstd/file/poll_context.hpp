// nstd/file/poll_context.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_FILE_POLL_CONTEXT
#define INCLUDED_NSTD_FILE_POLL_CONTEXT

#include "nstd/file/context.hpp"
#include <chrono>
#include <queue>
#include <poll.h>
#include <functional>
#include <list>
#include <vector>

// ----------------------------------------------------------------------------

namespace nstd::file {
    class poll_context; // I/O context based on [e]poll()
}

// ----------------------------------------------------------------------------

class nstd::file::poll_context
    : public ::nstd::file::context
{
private:
    struct operation {
        int                           d_fd;
        short                         d_events;
        ::std::function<auto()->bool> d_operation;
        operation(int, short, ::std::function<auto()->bool>);
    };
    struct timer_event {
        ::std::chrono::steady_clock::time_point d_expiry;
	io_base*                                d_continuation;
	timer_event(::std::chrono::steady_clock::time_point const&, io_base*);
	auto operator< (timer_event const&) const -> bool;
    };
    ::std::vector<::pollfd>           d_poll;
    ::std::vector<::pollfd>::iterator d_next_poll;
    ::std::list<operation>            d_outstanding;
    ::std::priority_queue<timer_event> d_timers;

    auto handle_scheduled() -> count_type;
    auto handle_timer() -> count_type;
    auto handle_io() -> count_type;

    auto submit_io(int, short, ::std::function<auto()->bool>) -> void;
    template <typename Fun>
    auto submit(int, short, Fun&& fun) -> void;

protected:
    auto do_run_one() -> count_type override;

    auto do_nop(io_base*) -> void override;
    auto do_timer(time_spec*, io_base*) -> void override;
    auto do_accept(native_handle_type, ::sockaddr*, ::socklen_t*, int, io_base*) -> void override;
    auto do_connect(native_handle_type, ::sockaddr const*, ::socklen_t, io_base*) -> void override;
    auto do_sendmsg(native_handle_type, ::msghdr const*, int, io_base*) -> void override;
    auto do_recvmsg(native_handle_type, ::msghdr*, int, io_base*) -> void override;
    auto do_read(int, ::iovec*, ::std::size_t, io_base*) -> void override;
    auto do_open_at(int, char const*, int, io_base*) -> void override;

public:
    poll_context();
};

// ----------------------------------------------------------------------------

#endif
