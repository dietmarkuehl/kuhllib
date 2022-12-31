// nstd/execution/repeat_effect_until.hpp                             -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_REPEAT_EFFECT_UNTIL
#define INCLUDED_NSTD_EXECUTION_REPEAT_EFFECT_UNTIL

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/type_traits/declval.hpp"
#include <exception>
#include <new>
#include <iostream>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names::repeat_effect_until
{
    template <::nstd::execution::sender Sender, typename Predicate, typename Receiver>
    struct state_base
    {
        using sender_type = ::nstd::type_traits::remove_cvref_t<Sender>;

        sender_type                                    d_sender;
        ::nstd::type_traits::remove_cvref_t<Predicate> d_predicate;
        ::nstd::type_traits::remove_cvref_t<Receiver>  d_receiver;

        template <::nstd::execution::sender S,
                  typename P,
                  ::nstd::execution::receiver R>
        state_base(S&& sender, P&& predicate, R&& receiver)
            : d_sender(::nstd::utility::forward<S>(sender))
            , d_predicate(::nstd::utility::forward<P>(predicate))
            , d_receiver(::nstd::utility::forward<R>(receiver))
        {
        }

        static_assert(::nstd::execution::sender<sender_type>);

        virtual auto next() -> void = 0;
    };

    template <::nstd::execution::sender Sender, typename Predicate, typename Receiver>
    struct receiver {
        state_base<Sender, Predicate, Receiver>* d_state;
        friend auto tag_invoke(::nstd::execution::get_env_t, receiver const& self) noexcept
            -> decltype(::nstd::execution::get_env(::nstd::type_traits::declval<Receiver const&>()))
        {
            return ::nstd::execution::get_env(self.d_state->d_receiver);
        }
        friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& r, auto&&...)
            noexcept -> void
        {
            r.d_state->next();
        }
        template <typename Error>
        friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& r, Error&& e)
            noexcept -> void {
            ::nstd::execution::set_error(::nstd::utility::move(r.d_state->d_receiver),
                                         ::nstd::utility::forward<Error>(e));
        }
        friend auto tag_invoke(::nstd::execution::set_stopped_t, receiver&& r)
            noexcept -> void
        {
            ::nstd::execution::set_stopped(::nstd::utility::move(r.d_state->d_receiver));
        }
    };

    template <::nstd::execution::sender Sender, typename Predicate, typename Receiver>
    struct state
        : state_base<Sender, Predicate, Receiver>
    {
        using state_base   = ::nstd::hidden_names::repeat_effect_until::state_base<Sender, Predicate, Receiver>;
        using sender_type   = typename state_base::sender_type;
        using receiver_type = ::nstd::hidden_names::repeat_effect_until::receiver<Sender, Predicate, Receiver>;
        static_assert(::nstd::execution::receiver<receiver_type>);
        struct internal_state {
            using state_type = decltype(::nstd::execution::connect(::nstd::type_traits::declval<Sender>(),
                                                                   ::nstd::type_traits::declval<receiver_type>()));
            state_type d_state;
            template <typename S>
            internal_state(S&& sender, receiver_type&& receiver)
                : d_state(::nstd::execution::connect(Sender(::nstd::utility::forward<S>(sender)), ::nstd::utility::move(receiver)))
            {
                (void)sender;
                (void)receiver;
            }

            friend auto tag_invoke(::nstd::execution::start_t, internal_state& self) noexcept -> void {
                (void)self;
                ::nstd::execution::start(self.d_state);
            }
        };

        bool d_engaged = false;
        union holder {
            holder(){}
            ~holder(){}
            internal_state d_state;
        } d_internal_state;

        template <typename S, typename P, typename R>
        state(S&& sndr, P&& predicate, R&& receiver)
            : state_base(::nstd::utility::forward<S>(sndr),
                         ::nstd::utility::forward<P>(predicate),
                         ::nstd::utility::forward<R>(receiver))
        {
        }
        ~state() {
            if (this->d_engaged) {
                this->d_internal_state.d_state.~internal_state();
            }
        }

        friend auto tag_invoke(::nstd::execution::start_t, state& s) noexcept -> void {
            new(&s.d_internal_state.d_state) internal_state(s.d_sender, receiver_type{&s});
            s.d_engaged = true;
            ::nstd::execution::start(s.d_internal_state.d_state);
        }
        auto next() noexcept -> void override try {
            if (this->d_predicate()) {
                ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver));
            }
            else {
                this->d_internal_state.d_state.~internal_state();
                new(&this->d_internal_state.d_state) internal_state(this->d_sender, receiver_type{this});
                ::nstd::execution::start(this->d_internal_state.d_state);
            }
        }
        catch (...) {
            ::nstd::execution::set_error(::nstd::utility::move(this->d_receiver),
                                         ::std::current_exception());
        }
    };


    template <::nstd::execution::sender Sender, typename Predicate>
    struct sender {
        using completion_signatures = ::nstd::execution::completion_signatures<
                ::nstd::execution::set_value_t(),
                ::nstd::execution::set_error_t(::std::exception_ptr),
                ::nstd::execution::set_stopped_t()
            >;

        ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
        ::nstd::type_traits::remove_cvref_t<Predicate> d_predicate;

        template <typename Receiver>
        friend auto tag_invoke(::nstd::execution::connect_t, sender&& sndr, Receiver&& receiver) noexcept {
            return ::nstd::hidden_names::repeat_effect_until::state<Sender, Predicate, Receiver>(
                        ::nstd::utility::move(sndr.d_sender),
                        ::nstd::utility::move(sndr.d_predicate),
                        ::nstd::utility::forward<Receiver>(receiver));
        }
        template <typename Receiver>
        friend auto tag_invoke(::nstd::execution::connect_t, sender const& sndr, Receiver&& receiver) noexcept {
            return ::nstd::hidden_names::repeat_effect_until::state<Sender const&, Predicate, Receiver>(
                        sndr.d_sender,
                        sndr.d_predicate,
                        ::nstd::utility::forward<Receiver>(receiver));
        }
    };

    struct cpo {
        template <::nstd::execution::sender Sender, typename Predicate>
        friend auto tag_invoke(cpo, Sender&& sndr, Predicate&& predicate)
        {
            return ::nstd::hidden_names::repeat_effect_until::sender<Sender, Predicate>{::nstd::utility::forward<Sender>(sndr),
                                             ::nstd::utility::forward<Predicate>(predicate)};
        }

        template <::nstd::execution::sender Sender, typename Predicate>
        auto operator()(Sender&& sndr, Predicate&& predicate) const {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::utility::forward<Sender>(sndr),
                                      ::nstd::utility::forward<Predicate>(predicate));
        }
    };
}

// ----------------------------------------------------------------------------

namespace nstd::execution {
    using repeat_effect_until_t = ::nstd::hidden_names::repeat_effect_until::cpo;
    inline constexpr repeat_effect_until_t repeat_effect_until{};
}

// ----------------------------------------------------------------------------

#endif
