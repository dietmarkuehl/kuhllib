// nstd/execution/timeout.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_TIMEOUT
#define INCLUDED_NSTD_EXECUTION_TIMEOUT

#include "nstd/execution/receiver.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/get_stop_token.hpp"
#include "nstd/execution/operation_state.hpp"
#include "nstd/stop_token/in_place_stop_token.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include <atomic>
#include <optional>
#include <tuple>
#include <variant>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    inline constexpr struct timeout_t
    {
        template <::nstd::execution::receiver, ::nstd::execution::sender>
        struct sender_receiver;
        template <::nstd::execution::receiver, ::nstd::execution::sender>
        struct timer_receiver;
        template <::nstd::execution::receiver, ::nstd::execution::sender>
        struct state_base;
        template <::nstd::execution::receiver, ::nstd::execution::sender, ::nstd::execution::sender>
        struct state;
        template <::nstd::execution::sender, ::nstd::execution::sender>
        struct sender;

        template <::nstd::execution::sender Sender,
                  ::nstd::execution::scheduler Scheduler,
                  typename Time>
        auto operator()(Sender&&, Scheduler&&, Time&&) const
            -> sender<Sender, decltype(::nstd::execution::just())>;
    } timeout;
}

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver,
          ::nstd::execution::sender   Sender>
struct nstd::execution::timeout_t::state_base
{
    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    ::nstd::stop_token::in_place_stop_source      d_source;
    ::std::optional<typename Sender::template value_types<::std::tuple, ::std::variant>> d_values;
    ::std::optional<typename Sender::template error_types<::std::variant>>               d_errors;
    ::std::atomic<unsigned> d_done{0u};
    auto timer_done() -> void {
        if (1u == ++this->d_done) {
            this->d_source.stop();
        }
        else {
            if (this->d_errors) {
                ::std::visit([this](auto&& error){
                    ::nstd::execution::set_error(::nstd::utility::move(this->d_receiver), ::nstd::utility::move(error));
                }, *this->d_errors);
            }
            else if (this->d_values) {
                ::std::visit([&receiver = this->d_receiver](auto&& values)->void{
                    ::std::apply([&receiver](auto&&... args)->void{
                        ::nstd::execution::set_value(::nstd::utility::move(receiver),
                                                     ::nstd::utility::forward<decltype(args)>(args)...);
                    }, values);
                }, *this->d_values);
            }
            else {
                ::nstd::execution::set_stopped(::nstd::utility::move(this->d_receiver));
            }
        }
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver, ::nstd::execution::sender Sender>
struct nstd::execution::timeout_t::sender_receiver
{
    struct nstd::execution::timeout_t::state_base<Receiver, Sender>* d_state;

    friend auto tag_invoke(::nstd::execution::get_stop_token_t, sender_receiver const& r) noexcept {
        return r.d_state->d_source.token();
    }
    friend auto tag_invoke(::nstd::execution::set_value_t, sender_receiver&&, auto&&...) noexcept -> void {
    }
    friend auto tag_invoke(::nstd::execution::set_error_t, sender_receiver&&, auto&&) noexcept -> void {
    }
    friend auto tag_invoke(::nstd::execution::set_stopped_t, sender_receiver&& r) noexcept -> void {
        r.d_state->d_source.stop();
        unsigned done = ++r.d_state->d_done;
        if (2u == done) { // ++r.d_state->d_done) {
            ::nstd::execution::set_stopped(::nstd::utility::move(r.d_state->d_receiver));
        }
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver, ::nstd::execution::sender Sender>
struct nstd::execution::timeout_t::timer_receiver
{
    struct nstd::execution::timeout_t::state_base<Receiver, Sender>* d_state;

    friend auto tag_invoke(::nstd::execution::get_stop_token_t, timer_receiver const& r) noexcept {
        return r.d_state->d_source.token();
    }
    friend auto tag_invoke(::nstd::execution::set_value_t, timer_receiver&& r, auto&&...) noexcept -> void {
        r.d_state->timer_done();
    }
    friend auto tag_invoke(::nstd::execution::set_error_t, timer_receiver&& r, auto&&) noexcept -> void {
        r.d_state->timer_done();
    }
    friend auto tag_invoke(::nstd::execution::set_stopped_t, timer_receiver&& r) noexcept -> void {
        r.d_state->timer_done();
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver,
          ::nstd::execution::sender   Sender,
          ::nstd::execution::sender   Timer>
struct nstd::execution::timeout_t::state
    : nstd::execution::timeout_t::state_base<Receiver, Sender>
{
    using SenderReceiver = ::nstd::execution::timeout_t::sender_receiver<Receiver, Sender>;
    using TimerReceiver = ::nstd::execution::timeout_t::timer_receiver<Receiver, Sender>;
    using SenderState = decltype(::nstd::execution::connect(::nstd::type_traits::declval<Sender>(),
                                                            ::nstd::type_traits::declval<SenderReceiver>()));
    using TimerState = decltype(::nstd::execution::connect(::nstd::type_traits::declval<Timer>(),
                                                            ::nstd::type_traits::declval<TimerReceiver>()));

    SenderState d_sender;
    TimerState  d_timer;

    template <::nstd::execution::receiver Recv,
              ::nstd::execution::sender   Send,
              ::nstd::execution::sender   Time>
    state(Recv&& receiver, Send&& sender, Time&& timer)
        : ::nstd::execution::timeout_t::state_base<Receiver, Sender>{ ::nstd::utility::forward<Recv>(receiver), {}, {}, {}, {} }
        , d_sender(::nstd::execution::connect(::nstd::utility::forward<Send>(sender), SenderReceiver{this}))
        , d_timer(::nstd::execution::connect(::nstd::utility::forward<Time>(timer), TimerReceiver{this}))
    {
    }

    friend auto tag_invoke(::nstd::execution::start_t, state& s)
        noexcept -> void
    {
        ::nstd::execution::start(s.d_sender);
        ::nstd::execution::start(s.d_timer);
    }
};

//-dk:TODO static_assert(::nstd::execution::operation_state<::nstd::execution::timeout_t::state<::nstd::execution::test_receiver>>);

template <::nstd::execution::sender Sender, ::nstd::execution::sender Timer>
struct nstd::execution::timeout_t::sender
{
    template <template <typename...> class T, template <typename...> class V>
    using value_types = typename Sender::template value_types<T, V>;
    template <template <typename...> class V>
    using error_types = typename Sender::template error_types<V>; //-dk:TODO merge with own errors
    static constexpr bool sends_done = true;

    ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
    ::nstd::type_traits::remove_cvref_t<Timer>  d_timer;

    template <::nstd::execution::receiver Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender&& sndr, Receiver&& receiver)
        noexcept -> ::nstd::execution::timeout_t::state<Receiver, Sender, Timer>
    {
        return {
            ::nstd::utility::forward<Receiver>(receiver),
            ::nstd::utility::move(sndr.d_sender),
            ::nstd::utility::move(sndr.d_timer)
        };
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::sender Sender,
          ::nstd::execution::scheduler Scheduler,
          typename Time>
auto ::nstd::execution::timeout_t::operator()(Sender&&    sndr,
                                              Scheduler&& ,
                                              Time&&      ) const
    -> sender<Sender, decltype(::nstd::execution::just())>
{
    return { ::nstd::utility::forward<Sender>(sndr), ::nstd::execution::just() };
}

// ----------------------------------------------------------------------------

#endif
