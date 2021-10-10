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
#include "nstd/file/context.hpp"
#include "nstd/file/ring_context.hpp"

// ----------------------------------------------------------------------------

namespace nstd::net {
    class io_context;
}

// ----------------------------------------------------------------------------

class nstd::net::io_context
{
private:
    ::std::unique_ptr<::nstd::file::context> d_context;

public:
    using count_type = ::nstd::file::context::count_type;

    io_context();
    explicit io_context(::nstd::file::ring_context::queue_size);
    explicit io_context(::std::size_t);
    io_context(io_context const&) = delete;
    io_context(io_context&&) = delete;
    auto operator=(io_context const&) -> io_context& = delete;
    auto operator=(io_context&&) -> io_context& = delete;

    class scheduler_type;
    auto hidden_context() -> ::nstd::file::context* { return this->d_context.get(); }

    auto scheduler() noexcept -> ::nstd::net::io_context::scheduler_type;

    auto run_one() -> count_type { return this->d_context->run_one(); }
    auto run() -> count_type {
        count_type count{};
        while (1u == this->d_context->run_one()) {
            ++count;
        }
        return count;
    }

    template <typename Rep, typename Period>
        auto run_for(::std::chrono::duration<Rep, Period> const&)
            -> ::nstd::file::ring_context::count_type;
    template <typename Clock, typename Duration>
        auto run_until(::std::chrono::time_point<Clock, Duration> const&)
            -> ::nstd::file::ring_context::count_type;
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

};

// ----------------------------------------------------------------------------

class nstd::net::io_context::scheduler_type
{
private:
    friend class ::nstd::net::io_context;
    ::nstd::net::io_context* d_context;
    explicit scheduler_type(::nstd::net::io_context* context) noexcept: d_context(context) {}

public:
    auto context() noexcept -> ::nstd::net::io_context* { return this->d_context; }

    auto operator== (scheduler_type const& other) const -> bool = default;
};

inline auto nstd::net::io_context::scheduler() noexcept
    -> ::nstd::net::io_context::scheduler_type
{
    return ::nstd::net::io_context::scheduler_type(this);
}

// ----------------------------------------------------------------------------

#endif
