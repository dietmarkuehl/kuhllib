// nstd/file/test_context.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_FILE_TEST_CONTEXT
#define INCLUDED_NSTD_FILE_TEST_CONTEXT

#include "nstd/file/context.hpp"
#include <functional>
#include <deque>
#include <tuple>

// ----------------------------------------------------------------------------

namespace nstd::file {
    class test_context;
}

// ----------------------------------------------------------------------------

class nstd::file::test_context
    : public ::nstd::file::context
{
private:
    ::std::deque<::std::tuple<int, int, ::nstd::file::context::io_base*>> d_ready;

protected:
    auto do_run_one() -> ::nstd::file::context::count_type override;

    auto do_nop(::nstd::file::context::io_base*) -> void override;
    auto do_timer(::nstd::file::context::time_spec*, ::nstd::file::context::io_base*) -> void override;
    auto do_accept(int, ::sockaddr*, ::socklen_t*, int, ::nstd::file::context::io_base*) -> void override;
    auto do_connect(int, ::sockaddr const*, ::socklen_t, ::nstd::file::context::io_base*) -> void override;
    auto do_sendmsg(int, ::msghdr const*, int, ::nstd::file::context::io_base*) -> void override;
    auto do_recvmsg(int, ::msghdr*, int, ::nstd::file::context::io_base*) -> void override;
    auto do_read(int, ::iovec*, ::std::size_t, ::nstd::file::context::io_base*) -> void override;
    auto do_open_at(int, char const*, int, ::nstd::file::context::io_base*) -> void override;

public:
    test_context();

    auto make_ready(int, int, ::nstd::file::context::io_base*) -> void;

    ::std::function<auto (::nstd::file::context::io_base*) -> void>                                      on_nop;
    ::std::function<auto (::nstd::file::context::time_spec*, ::nstd::file::context::io_base*) -> void>   on_timer;
    ::std::function<auto (int, ::sockaddr*, ::socklen_t*, int, ::nstd::file::context::io_base*) -> void> on_accept;
    ::std::function<auto (int, ::sockaddr const*, ::socklen_t, ::nstd::file::context::io_base*) -> void> on_connect;
    ::std::function<auto (int, ::msghdr const*, int, ::nstd::file::context::io_base*) -> void>           on_sendmsg;
    ::std::function<auto (int, ::msghdr*, int, ::nstd::file::context::io_base*) -> void>                 on_recvmsg;
    ::std::function<auto (int, ::iovec*, ::std::size_t, ::nstd::file::context::io_base*) -> void>        on_read;
    ::std::function<auto (int, char const*, int, ::nstd::file::context::io_base*) -> void>               on_open_at;
};

// ----------------------------------------------------------------------------

#endif
