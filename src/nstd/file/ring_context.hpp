// nstd/file/ring_context.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_FILE_RING_CONTEXT
#define INCLUDED_NSTD_FILE_RING_CONTEXT

#include "nstd/nstd-config.hpp"

#ifdef NSTD_HAS_LINUX_IO_URING

#include "nstd/file/context.hpp"
#include "nstd/file/descriptor.hpp"
#include "nstd/file/mapped_memory.hpp"
#include "nstd/file/ring.hpp"
#include <linux/io_uring.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <chrono>
#include <mutex>
#include <cstddef>
#include <cstdint>
#include <iostream> //-dk:TODO remove

// ----------------------------------------------------------------------------

namespace nstd::file {
    class ring_context; // I/O context base on io_uring
}

// ----------------------------------------------------------------------------

class nstd::file::ring_context
    : public ::nstd::file::context
{
private:
    ::nstd::file::descriptor            d_fd;
    ::nstd::file::mapped_memory         d_smem; // submission ring memory
    ::nstd::file::mapped_memory         d_cmem; // potentially needed completion ring memory
    ::nstd::file::mapped_memory         d_emem; // submission element memory
    ::nstd::file::ring<unsigned int>    d_submission;
    ::nstd::file::ring<::io_uring_cqe>  d_completion;
    ::io_uring_sqe*                     d_submission_elements;
    ::std::atomic<unsigned int>         d_outstanding;
    ::std::atomic<bool>                 d_stopped{false};
    ::std::mutex                        d_mutex;

    auto intern_submit(::std::size_t) -> void;
    auto process_result() -> ::std::size_t;

public:
    class run_guard;

    template <typename Op>
    auto submit(Op op) -> void;

    class scheduler_type;
    using count_type = ::std::size_t;
    enum queue_size: int { max = ::std::numeric_limits<int>::max() }; // extension

    ring_context();
    explicit ring_context(queue_size size); // extension
    explicit ring_context(int);
    ring_context(ring_context const&) = delete;
    auto operator=(ring_context const&) -> ring_context& = delete;
    ~ring_context();

    auto setup(queue_size size) -> int; // extension; return better error?
    auto is_setup() const -> bool;      // extension

    auto scheduler() noexcept -> ::nstd::file::ring_context::scheduler_type;

    auto run() -> ::nstd::file::ring_context::count_type;
    template <typename Rep, typename Period>
        auto run_for(::std::chrono::duration<Rep, Period> const&)
            -> ::nstd::file::ring_context::count_type;
    template <typename Clock, typename Duration>
        auto run_until(::std::chrono::time_point<Clock, Duration> const&)
            -> ::nstd::file::ring_context::count_type;
    auto run_one() -> ::nstd::file::ring_context::count_type;
    template <typename Rep, typename Period>
        auto run_one_for(::std::chrono::duration<Rep, Period> const&)
            -> ::nstd::file::ring_context::count_type;
    template <typename Clock, typename Duration>
        auto run_one_until(::std::chrono::time_point<Clock, Duration> const&)
            -> ::nstd::file::ring_context::count_type;

    auto poll() -> ::nstd::file::ring_context::count_type;
    auto poll_one() -> ::nstd::file::ring_context::count_type;
    auto stop() -> void;
    auto stopped() const noexcept -> bool;
    auto restart() -> void;

    auto do_run_one() -> ::nstd::file::ring_context::count_type override;

    auto do_nop(io_base*) -> void override;
    auto do_timer(::nstd::file::context::time_spec*, io_base*) -> void override;
    auto do_accept(int, ::sockaddr*, ::socklen_t*, int, io_base*) -> void override;
    auto do_connect(int, ::sockaddr const*, ::socklen_t, io_base*) -> void override;
    auto do_sendmsg(int, ::msghdr const*, int, io_base*) -> void override;
    auto do_recvmsg(int, ::msghdr*, int, io_base*) -> void override;
    auto do_read(int, ::iovec*, ::std::size_t, io_base*) -> void override;
    auto do_open_at(int, char const*, int, io_base*) -> void override;
};

// ----------------------------------------------------------------------------

class nstd::file::ring_context::scheduler_type
{
private:
    friend class ::nstd::file::ring_context;
    ::nstd::file::ring_context* d_context;
    explicit scheduler_type(::nstd::file::ring_context* context) noexcept: d_context(context) {}

public:
    auto context() noexcept -> ::nstd::file::ring_context* { return this->d_context; }

    auto operator== (scheduler_type const& other) const -> bool = default;
};

inline auto nstd::file::ring_context::scheduler() noexcept
    -> ::nstd::file::ring_context::scheduler_type
{
    return ::nstd::file::ring_context::scheduler_type(this);
}

// ----------------------------------------------------------------------------

template <typename Op>
auto nstd::file::ring_context::submit(Op op) ->void
{
    ::std::lock_guard kerberos(this->d_mutex);
    auto tail(this->d_submission.tail());
    auto index(this->d_submission.mask(tail));
    op(this->d_submission_elements[index]);
    this->d_submission.d_array[index] = index;
    ++this->d_outstanding;
    this->d_submission.advance_tail();
    //-dk:TODO remove ::std::cout << "ring_context::submit()\n";
    this->intern_submit(1u);
}

#endif

// ----------------------------------------------------------------------------

#endif
