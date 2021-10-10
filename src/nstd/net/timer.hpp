// nstd/net/timer.hpp                                                 -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_TIMER
#define INCLUDED_NSTD_NET_TIMER

#include "nstd/net/netfwd.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/execution/receiver_of.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/operation_state.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include <chrono>
#include <system_error>
#include <exception>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename> struct wait_traits;
    template <typename, typename> class basic_waitable_timer;

    using system_timer = ::nstd::net::basic_waitable_timer<::std::chrono::system_clock>;
    using steady_timer = ::nstd::net::basic_waitable_timer<::std::chrono::steady_clock>;
    using high_resolution_timer = ::nstd::net::basic_waitable_timer<::std::chrono::high_resolution_clock>;

    inline constexpr struct async_wait_t {
        template <::nstd::execution::receiver_of<::std::error_code>, typename, typename> struct state;
        template <typename, typename> struct sender;
        template <typename Clock, typename Traits>
        friend auto tag_invoke(async_wait_t, ::nstd::net::basic_waitable_timer<Clock, Traits> const& timer)
            -> sender<Clock, Traits> {
                return { timer.get_scheduler().context()->hidden_context(), timer.expiry() };
        }
        template <typename Clock, typename Traits>
        auto operator()(::nstd::net::basic_waitable_timer<Clock, Traits> const& timer) const {
            return ::nstd::tag_invoke(*this, timer);
        }

    } async_wait;
}

// ----------------------------------------------------------------------------

template <typename Clock>
struct nstd::net::wait_traits
{
    static auto to_wait_duration(typename Clock::duration const& d)
        -> typename Clock::duration {
            return d;
    }
    static auto to_wait_duration(typename Clock::time_point const& t)
        -> typename Clock::duration {
            //-dk:TODO deal with boundary cases
            return t - Clock::now();
    }
};

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

template <::nstd::execution::receiver_of<::std::error_code> Receiver, typename Clock, typename Traits>
struct nstd::net::async_wait_t::state
    : public ::nstd::file::context::io_base
{
    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    ::nstd::file::context*                        d_context;
    typename Clock::time_point                    d_time_point;
    ::__kernel_timespec                           d_time; // tv_sec, tv_usec

    template <::nstd::execution::receiver_of<::std::error_code> R>
    state(R&& receiver, ::nstd::file::context* context, typename Clock::time_point time_point)
        : d_receiver(::nstd::utility::forward<R>(receiver))
        , d_context(context)
        , d_time_point(time_point)
    {
    }
    friend auto tag_invoke(::nstd::execution::start_t, state& s)
        noexcept -> void {
        auto duration = Traits::to_wait_duration(s.d_time_point);
        auto nsec = ::std::chrono::duration_cast<::std::chrono::nanoseconds>(duration);
        s.d_time.tv_sec  = nsec.count() / 1000000000;
        s.d_time.tv_nsec = nsec.count() % 1000000000;
        s.d_context->timer(&s.d_time, &s);
    }
    auto do_result(::std::int32_t, ::std::uint32_t)
        -> void override
    {
        ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver),
                                     ::std::error_code(0, ::std::system_category()));
    }
};

static_assert(::nstd::execution::operation_state<
                  ::nstd::net::async_wait_t::state<
                      ::nstd::execution::test_receiver_of<::std::error_code>,
                      ::std::chrono::system_clock,
                      ::nstd::net::wait_traits<::std::chrono::system_clock>>>);

template <typename Clock, typename Traits>
struct nstd::net::async_wait_t::sender
{
    template <template <typename...> class V, template <typename...> class T>
    using value_types = V<T<::std::error_code>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;
    static constexpr bool sends_done = true;

    ::nstd::file::context*                                                d_context;
    typename ::nstd::net::basic_waitable_timer<Clock, Traits>::time_point d_timepoint;

    template <::nstd::execution::receiver_of<::std::error_code> Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender const& sndr, Receiver&& receiver)
        noexcept -> ::nstd::net::async_wait_t::state<Receiver, Clock, Traits> {
            return  ::nstd::net::async_wait_t::state<Receiver, Clock, Traits>(
                ::nstd::utility::forward<Receiver>(receiver),
                sndr.d_context,
                sndr.d_timepoint
                );
    }
};

static_assert(::nstd::execution::sender<
                  ::nstd::net::async_wait_t::sender<::std::chrono::system_clock,
                                                    ::nstd::net::wait_traits<::std::chrono::system_clock>>>);

// ----------------------------------------------------------------------------

#endif
