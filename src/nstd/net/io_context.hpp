// nstd/net/io_context.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_IO_CONTEXT
#define INCLUDED_NSTD_NET_IO_CONTEXT

#include "nstd/net/netfwd.hpp"
#include "nstd/file/descriptor.hpp"
#include "nstd/file/mapped_memory.hpp"
#include "nstd/file/ring.hpp"
#include <linux/io_uring.h>
#include <chrono>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace nstd::net {
    class io_context;
}

// ----------------------------------------------------------------------------

class nstd::net::io_context
    : public nstd::net::execution_context
{
private:
    ::nstd::file::descriptor            d_fd;
    ::nstd::file::mapped_memory         d_smem; // submission ring memory
    ::nstd::file::mapped_memory         d_cmem; // potentially needed completion ring memory
    ::nstd::file::mapped_memory         d_emem; // submission element memory
    ::nstd::file::ring<unsigned int>    d_submission;
    ::nstd::file::ring<::io_uring_cqe>  d_completion;
    ::io_uring_sqe*                     d_submission_elements;

public:
    class executor_type;
    using count_type = ::std::size_t;
    enum queue_size: int { max = ::std::numeric_limits<int>::max() };

    io_context();
    explicit io_context(queue_size size); // extension
    explicit io_context(int);
    io_context(io_context const&) = delete;
    auto operator=(io_context const&) -> io_context& = delete;
    ~io_context();

    auto setup(queue_size size) -> int; // extension; return better error?
    auto is_setup() const -> bool;      // extension

    auto get_executor() noexcept -> ::nstd::net::io_context::executor_type;

    auto run() -> ::nstd::net::io_context::count_type;
    template <typename Rep, typename Period>
        auto run_for(::std::chrono::duration<Rep, Period> const&)
            -> ::nstd::net::io_context::count_type;
    template <typename Clock, typename Duration>
        auto run_until(::std::chrono::time_point<Clock, Duration> const&)
            -> ::nstd::net::io_context::count_type;
    auto run_one() -> ::nstd::net::io_context::count_type;
    template <typename Rep, typename Period>
        auto run_one_for(::std::chrono::duration<Rep, Period> const&)
            -> ::nstd::net::io_context::count_type;
    template <typename Clock, typename Duration>
        auto run_one_until(::std::chrono::time_point<Clock, Duration> const&)
            -> ::nstd::net::io_context::count_type;

    auto poll() -> ::nstd::net::io_context::count_type;
    auto poll_one() -> ::nstd::net::io_context::count_type;
    auto stop() -> void;
    auto stopped() const noexcept -> bool;
    auto restart() -> void;
};

// ----------------------------------------------------------------------------

class nstd::net::io_context::executor_type
{

};

// ----------------------------------------------------------------------------

#endif
