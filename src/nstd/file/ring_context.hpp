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

#include "nstd/file/descriptor.hpp"
#include "nstd/file/mapped_memory.hpp"
#include "nstd/file/ring.hpp"
#include <linux/io_uring.h>
#include <sys/socket.h>
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
    class io_base;
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

    auto accept(int, void*, void*, int, io_base*) -> void;
    auto connect(int, void const*, socklen_t, io_base*) -> void;
    auto sendmsg(int, msghdr const*, int, io_base*) -> void;
    auto recvmsg(int, msghdr*, int, io_base*) -> void;
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
};

inline auto nstd::file::ring_context::scheduler() noexcept
    -> ::nstd::file::ring_context::scheduler_type
{
    return ::nstd::file::ring_context::scheduler_type(this);
}

// ----------------------------------------------------------------------------

class nstd::file::ring_context::io_base
{
protected:
    virtual ~io_base() = default;
    virtual auto do_result(::std::int32_t, ::std::uint32_t) -> void = 0;

public:
    auto result(::std::int32_t res, ::std::uint32_t flags) -> void { this->do_result(res, flags); }
};

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

// ----------------------------------------------------------------------------

#endif
