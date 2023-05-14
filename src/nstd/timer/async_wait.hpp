// nstd/timer/async_wait.hpp                                          -*-C++-*-
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

#ifndef INCLUDED_NSTD_TIMER_ASYNC_WAIT
#define INCLUDED_NSTD_TIMER_ASYNC_WAIT

#include "nstd/timer/basic_waitable_timer.hpp"
#include "nstd/timer/wait_traits.hpp"
#include "nstd/net/netfwd.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/receiver.hpp"
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
    inline constexpr struct async_wait_t {
        template <::nstd::execution::receiver, typename, typename> struct state;
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

template <::nstd::execution::receiver Receiver, typename Clock, typename Traits>
struct nstd::net::async_wait_t::state
    : public ::nstd::file::context::io_base
{
    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    ::nstd::file::context*                        d_context;
    typename Clock::time_point                    d_time_point;
    ::nstd::file::context::time_spec              d_time;

    template <::nstd::execution::receiver R>
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
        s.d_time.sec  = nsec.count() / 1000000000;
        s.d_time.nsec = nsec.count() % 1000000000;
        s.d_context->timer(&s.d_time, &s);
    }
    auto do_result(::std::int32_t, ::std::uint32_t)
        -> void override
    {
        ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver),
                                     ::std::error_code(0, ::std::system_category()));
    }
};

#if 0
//-dk:TODO
static_assert(::nstd::execution::operation_state<
                  ::nstd::net::async_wait_t::state<
                      ::nstd::execution::hidden_names::test_receiver_of<::std::error_code>,
                      ::std::chrono::system_clock,
                      ::nstd::net::wait_traits<::std::chrono::system_clock>>>);
#endif

template <typename Clock, typename Traits>
struct nstd::net::async_wait_t::sender
{
    using completion_signatures
        = ::nstd::execution::completion_signatures<
            ::nstd::execution::set_value_t(::std::error_code),
            ::nstd::execution::set_stopped_t()
            >;
    template <template <typename...> class T, template <typename...> class V>
    using value_types = V<T<::std::error_code>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;
    static constexpr bool sends_done = true;

    ::nstd::file::context*                                                d_context;
    typename ::nstd::net::basic_waitable_timer<Clock, Traits>::time_point d_timepoint;

    template <::nstd::execution::receiver Receiver>
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
