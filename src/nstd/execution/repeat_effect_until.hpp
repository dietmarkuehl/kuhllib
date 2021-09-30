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

#include "nstd/execution/sender.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/type_traits/declval.hpp"
#include <exception>
#include <new>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    inline constexpr struct repeat_effect_until_t {
        template <::nstd::execution::sender Sender, typename Predicate, typename Receiver>
        struct state {
            struct receiver {
                state* d_state;
                friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& r)
                    noexcept -> void {
                    r.d_state->next();
                }
                template <typename Error>
                friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& r, Error&& e)
                    noexcept -> void {
                    ::nstd::execution::set_error(::nstd::utility::move(r.d_state->d_receiver),
                                                 ::nstd::utility::forward<Error>(e));
                }
                friend auto tag_invoke(::nstd::execution::set_done_t, receiver&& r)
                    noexcept -> void {
                    ::nstd::execution::set_done(::nstd::utility::move(r.d_state->d_receiver));
                }
            };
            using sender_type = ::nstd::type_traits::remove_cvref_t<Sender>;

            sender_type d_sender;
            ::nstd::type_traits::remove_cvref_t<Predicate> d_predicate;
            ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
            using internal_state
                = decltype(::nstd::execution::connect(::nstd::type_traits::declval<sender_type>(),
                                                      ::nstd::type_traits::declval<receiver>()));

            bool d_engaged = false;
            union holder {
                holder(){}
                ~holder(){}
                internal_state d_state;
            } d_internal_state;

            template <typename P, typename R>
            state(Sender&& sndr, P&& predicate, R&& receiver)
                : d_sender(::nstd::utility::move(sndr))
                , d_predicate(::nstd::utility::forward<P>(predicate))
                , d_receiver(::nstd::utility::forward<R>(receiver))
            {
            }
            ~state() {
                if (this->d_engaged) {
                    this->d_internal_state.d_state.~internal_state();
                }
            }

            friend auto tag_invoke(::nstd::execution::start_t, state& s) noexcept {
                new(&s.d_internal_state.d_state)
                    internal_state(::nstd::execution::connect(s.d_sender,
                                                              receiver{&s}));
                s.d_engaged = true;
                ::nstd::execution::start(s.d_internal_state.d_state);
            }
            auto next() noexcept -> void try {
                if (this->d_predicate()) {
                    ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver));
                }
                else {
                    this->d_internal_state.d_state.~internal_state();
                    new(&this->d_internal_state.d_state)
                        internal_state(::nstd::execution::connect(this->d_sender,
                                                                  receiver{this}));
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
            template <template <typename...> class V, template <typename...> class T>
            using value_types = V<T<>>;
            template <template <typename...> class V>
            using error_types = V<::std::exception_ptr>;
            static constexpr bool sends_done = true;

            ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
            ::nstd::type_traits::remove_cvref_t<Predicate> d_predicate;

            template <typename Receiver>
            friend auto tag_invoke(::nstd::execution::connect_t, sender&& sndr, Receiver&& receiver) noexcept {
                return state<Sender, Predicate, Receiver>(::nstd::utility::move(sndr.d_sender),
                             ::nstd::utility::move(sndr.d_predicate),
                             ::nstd::utility::forward<Receiver>(receiver));
            }
        };

        template <::nstd::execution::sender Sender, typename Predicate>
        friend auto tag_invoke(repeat_effect_until_t, Sender&& sndr, Predicate&& predicate)
        {
            return sender<Sender, Predicate>{::nstd::utility::forward<Sender>(sndr),
                                             ::nstd::utility::forward<Predicate>(predicate)};
        }

        template <::nstd::execution::sender Sender, typename Predicate>
        auto operator()(Sender&& sndr, Predicate&& predicate) const {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::utility::forward<Sender>(sndr),
                                      ::nstd::utility::forward<Predicate>(predicate));
        }
    } repeat_effect_until;
}

// ----------------------------------------------------------------------------

#endif
