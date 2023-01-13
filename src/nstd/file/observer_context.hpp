// nstd/file/observer_context.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_FILE_OBSERVER_CONTEXT
#define INCLUDED_NSTD_FILE_OBSERVER_CONTEXT

#include "nstd/file/context.hpp"
#include "nstd/container/intrusive_list.hpp"
#include <algorithm>
#include <mutex>

// ----------------------------------------------------------------------------

namespace nstd::file {
    class observer_context;
}

// ----------------------------------------------------------------------------

class nstd::file::observer_context
    : public ::nstd::file::context
{
public:
    struct continuation_base
        : ::nstd::file::context::io_base
    {
        ::nstd::container::intrusive_list_node<continuation_base> link;
        ::nstd::file::context::io_base*                           d_cont;
        continuation_base(::nstd::file::context::io_base* cont = nullptr)
            : d_cont(cont)
        {
        }
        auto do_result(::std::int32_t, ::std::uint32_t) -> void override {}
    };

private:
    template <typename> struct continuation;
    template <typename> friend struct continuation;
    ::std::mutex                                         d_mutex;
    ::nstd::container::intrusive_list<continuation_base> d_outstanding;
    ::nstd::file::context&                               d_context;

    template <typename Fun>
    auto add(nstd::file::context::io_base*, Fun&& fun) -> continuation_base*;

protected:
    auto do_run_one() -> count_type override;

    auto do_cancel(io_base*, io_base*) -> void override;
    auto do_nop(io_base*) -> void override;
    auto do_timer(time_spec*, io_base*) -> void override;
    auto do_accept(native_handle_type, ::sockaddr*, ::socklen_t*, int, io_base*) -> void override;
    auto do_connect(native_handle_type, ::sockaddr const*, ::socklen_t, io_base*) -> void override;
    auto do_sendmsg(native_handle_type, ::msghdr const*, int, io_base*) -> void override;
    auto do_recvmsg(native_handle_type, ::msghdr*, int, io_base*) -> void override;
    auto do_read(int, ::iovec*, ::std::size_t, io_base*) -> void override;
    auto do_write(int, ::iovec*, ::std::size_t, io_base*) -> void override;
    auto do_open_at(int, char const*, int, io_base*) -> void override;

public:
    observer_context(::nstd::file::context&);
};

// ----------------------------------------------------------------------------

#endif
