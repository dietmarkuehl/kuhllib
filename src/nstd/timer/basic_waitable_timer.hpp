// nstd/timer/basic_waitable_timer.hpp                                -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2023 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_NSTD_TIMER_BASIC_WAITABLE_TIMER
#define INCLUDED_NSTD_TIMER_BASIC_WAITABLE_TIMER

#include "nstd/net/netfwd.hpp"
#include "nstd/net/io_context.hpp"
#include <chrono>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename, typename> class basic_waitable_timer;

    using system_timer = ::nstd::net::basic_waitable_timer<::std::chrono::system_clock>;
    using steady_timer = ::nstd::net::basic_waitable_timer<::std::chrono::steady_clock>;
    using high_resolution_timer = ::nstd::net::basic_waitable_timer<::std::chrono::high_resolution_clock>;
}

// ----------------------------------------------------------------------------

template <typename Clock, typename Traits>
class nstd::net::basic_waitable_timer
{
public:
    using scheduler_type = ::nstd::net::io_context::scheduler_type;
    using clock_type = Clock;
    using duration = typename Clock::duration;
    using time_point = typename Clock::time_point;
    using traits_type = Traits;

private:
    scheduler_type  d_scheduler;
    time_point      d_time_point;

public:
    explicit basic_waitable_timer(::nstd::net::io_context&);
    basic_waitable_timer(::nstd::net::io_context&, time_point const&);
    basic_waitable_timer(::nstd::net::io_context&, duration const&);
    basic_waitable_timer(basic_waitable_timer const&) = delete;
    basic_waitable_timer(basic_waitable_timer&&);
    ~basic_waitable_timer();

    auto operator=(basic_waitable_timer const&) -> basic_waitable_timer& = delete;
    auto operator=(basic_waitable_timer&&) -> basic_waitable_timer&;

    auto get_scheduler() const noexcept -> scheduler_type;

    auto cancel() -> ::std::size_t;
    auto cancel_one() -> ::std::size_t;

    auto expiry() const -> time_point;
    auto expires_at(time_point const&) -> ::std::size_t;
    auto expires_after(duration const&) -> ::std::size_t;
};

// ----------------------------------------------------------------------------

template <typename Clock, typename Traits>
nstd::net::basic_waitable_timer<Clock, Traits>::basic_waitable_timer(::nstd::net::io_context& context)
    : basic_waitable_timer(context, time_point())
{
}
template <typename Clock, typename Traits>
nstd::net::basic_waitable_timer<Clock, Traits>::basic_waitable_timer(::nstd::net::io_context& context, time_point const& t)
    : d_scheduler(context.scheduler())
    , d_time_point(t)
{
}
template <typename Clock, typename Traits>
nstd::net::basic_waitable_timer<Clock, Traits>::basic_waitable_timer(::nstd::net::io_context& context, duration const& d)
    : basic_waitable_timer(context, Clock::now() + d)
{
}
template <typename Clock, typename Traits>
nstd::net::basic_waitable_timer<Clock, Traits>::basic_waitable_timer(basic_waitable_timer&&) = default;

template <typename Clock, typename Traits>
nstd::net::basic_waitable_timer<Clock, Traits>::~basic_waitable_timer()
{
}

// ----------------------------------------------------------------------------

template <typename Clock, typename Traits>
auto nstd::net::basic_waitable_timer<Clock, Traits>::get_scheduler() const noexcept
    -> scheduler_type
{
    return this->d_scheduler;
}

template <typename Clock, typename Traits>
auto nstd::net::basic_waitable_timer<Clock, Traits>::expiry() const
    -> time_point
{
    return this->d_time_point;
}

// ----------------------------------------------------------------------------

#endif
