// nstd/execution/lazy_then.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_LAZY_THEN
#define INCLUDED_NSTD_EXECUTION_LAZY_THEN

#include "nstd/functional/tag_invoke.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/typed_sender.hpp"
#include "nstd/execution/sender_base.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/is_void.hpp"
#include "nstd/type_traits/type_identity.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include <exception>
#include <functional>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {

    template <typename Fun, template <typename...> class To>
    struct invoked_type {
        template <typename... A>
        using type = To<decltype(::std::invoke(::nstd::type_traits::declval<Fun>(),
                                               ::nstd::type_traits::declval<A>()...))>;
    };
}

namespace nstd::execution {
    inline constexpr struct lazy_then_t {
        template <typename Fun, typename Receiver>
        struct lazy_then_receiver {
            Fun      d_fun;
            Receiver d_receiver;

            template <typename... T>
                requires requires(Fun&& f, T&&... args) {
                    std::invoke(::nstd::utility::move(f),
                                ::nstd::utility::forward<T>(args)...);
                    }
            friend auto
            tag_invoke(::nstd::execution::set_value_t, lazy_then_receiver&& r, T&&... args) noexcept
                try {
                    if constexpr (::nstd::type_traits::is_void_v<decltype(::std::invoke(::nstd::utility::move(r.d_fun), ::nstd::utility::forward<T>(args)...))>) {
                        ::std::invoke(::nstd::utility::move(r.d_fun), ::nstd::utility::forward<T>(args)...);
                        ::nstd::execution::set_value(::nstd::utility::move(r.d_receiver));
                    }
                    else {
                        ::nstd::execution::set_value(::nstd::utility::move(r.d_receiver),
                                                     ::std::invoke(::nstd::utility::move(r.d_fun), ::nstd::utility::forward<T>(args)...));
                    }
                }
                catch (...) {
                    ::nstd::execution::set_error(::nstd::utility::move(r.d_receiver),
                                                 ::std::current_exception());
                }
            template <typename... T>
                requires requires(Fun const& f, T&&... args) {
                    std::invoke(f, ::nstd::utility::forward<T>(args)...);
                    }
            friend auto tag_invoke(::nstd::execution::set_value_t, lazy_then_receiver const& r, T&&... args) noexcept
                try {
                    ::nstd::execution::set_value(r.d_receiver,
                                                 ::std::invoke(r.d_fun,
                                                               ::nstd::utility::forward<T>(args)...));
                }
                catch (...) {
                    ::nstd::execution::set_error(::nstd::utility::move(r.d_receiver),
                                                 ::std::current_exception());
                }
            template <typename Error>
            friend auto tag_invoke(::nstd::execution::set_error_t, lazy_then_receiver&& r, Error&& error) noexcept {
                ::nstd::execution::set_error(::nstd::utility::move(r.d_receiver),
                                             ::nstd::utility::forward<Error>(error));
            }
            friend auto tag_invoke(::nstd::execution::set_done_t, lazy_then_receiver&& r) noexcept {
                ::nstd::execution::set_done(::nstd::utility::move(r.d_receiver));
            }
        };

        template <::nstd::execution::typed_sender Sender, typename Fun>
        struct lazy_then_sender
            : ::nstd::execution::piped_sender_base 
        {
            template <template <typename...> class V, template <typename...> class T>
            using xvalue_types
                      = typename Sender::template value_types<V, ::nstd::hidden_names::invoked_type<Fun, T>::template type>;

            using fun_type
                    = typename Sender::template value_types<::nstd::type_traits::type_identity_t,
                                                            ::nstd::hidden_names::invoked_type<Fun, ::nstd::type_traits::type_identity_t>::template type>;


            template <bool, template <typename...> class V, template <typename...> class T>
            struct make_value_types { using type = V<T<fun_type>>; };
            template <template <typename...> class V, template <typename...> class T>
            struct make_value_types<true, V, T> { using type = V<T<>>; };
            template <bool C, template <typename...> class V, template <typename...> class T>
            using make_value_types_t = typename make_value_types<C, V, T>::type;

            template <template <typename...> class V, template <typename...> class T>
            using value_types = make_value_types_t<::nstd::type_traits::is_void_v<fun_type>, V, T>;

            template <template <typename...> class V>
            using error_types = V<::std::exception_ptr>; //-dk:TODO merge with Sender errors
            static constexpr bool sends_done = Sender::sends_done;

            Sender  d_sender;
            Fun     d_fun;

            template <::nstd::execution::receiver Receiver>
            friend auto tag_invoke(::nstd::execution::connect_t, lazy_then_sender<Sender, Fun> const& s, Receiver&& r) noexcept {
                return ::nstd::execution::connect(s.d_sender,
                                                  lazy_then_receiver<Fun, ::nstd::type_traits::remove_cvref_t<Receiver>>{
                                                      s.d_fun,
                                                      ::nstd::utility::forward<Receiver>(r)
                                                      });
            }

            template <::nstd::execution::receiver Receiver>
            friend auto tag_invoke(::nstd::execution::connect_t, lazy_then_sender<Sender, Fun>&& s, Receiver&& r) noexcept {
                return ::nstd::execution::connect(::nstd::utility::move(s.d_sender),
                                                  lazy_then_receiver<Fun, ::nstd::type_traits::remove_cvref_t<Receiver>>{
                                                      ::nstd::utility::move(s.d_fun),
                                                      ::nstd::utility::forward<Receiver>(r)
                                                      });
            }
            template <typename Tag>
                requires requires {
                    { ::nstd::execution::get_completion_scheduler<Tag>(::nstd::type_traits::declval<Sender>()) } noexcept -> ::nstd::execution::scheduler;
                }
            friend auto tag_invoke(::nstd::execution::get_completion_scheduler_t<Tag> const&, lazy_then_sender const& self) noexcept {
                return ::nstd::execution::get_completion_scheduler<Tag>(self.d_sender);
            }
        };

        template <::nstd::execution::sender Sender, typename Fun>
            requires requires (Sender&& sender, Fun&& fun) {
                {
                    ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::execution::lazy_then_t const&>(),
                                       ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                       ::nstd::utility::forward<Sender>(sender),
                                       ::nstd::utility::forward<Fun>(fun))
                } -> ::nstd::execution::sender;
            }
        auto operator()(Sender&& sender, Fun&& fun) const {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                      ::nstd::utility::forward<Sender>(sender),
                                      ::nstd::utility::forward<Fun>(fun));
        }
        template <::nstd::execution::sender Sender, typename Fun>
            requires
                (not requires (Sender&& sender, Fun&& fun) {
                    {
                        ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::execution::lazy_then_t const&>(),
                                           ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                           ::nstd::utility::forward<Sender>(sender),
                                           ::nstd::utility::forward<Fun>(fun))
                    } -> ::nstd::execution::sender;
                })
                && requires (Sender&& sender, Fun&& fun) {
                    {
                        ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::execution::lazy_then_t const&>(),
                                           ::nstd::utility::forward<Sender>(sender),
                                           ::nstd::utility::forward<Fun>(fun))
                    } -> ::nstd::execution::sender;
                }
        auto operator()(Sender&& sender, Fun&& fun) const {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::utility::forward<Sender>(sender),
                                      ::nstd::utility::forward<Fun>(fun));
        }
        template <::nstd::execution::sender Sender, typename Fun>
            requires
                (not requires (Sender&& sender, Fun&& fun) {
                    {
                        ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::execution::lazy_then_t const&>(),
                                           ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                           ::nstd::utility::forward<Sender>(sender),
                                           ::nstd::utility::forward<Fun>(fun))
                    } -> ::nstd::execution::sender;
                })
                && (not requires (Sender&& sender, Fun&& fun) {
                    {
                        ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::execution::lazy_then_t const&>(),
                                           ::nstd::utility::forward<Sender>(sender),
                                           ::nstd::utility::forward<Fun>(fun))
                    } -> ::nstd::execution::sender;
                })
        auto operator()(Sender&& s, Fun&& fun) const {
            return lazy_then_sender<::nstd::type_traits::remove_cvref_t<Sender>, ::nstd::type_traits::remove_cvref_t<Fun>>{
                {},
                ::nstd::utility::forward<Sender>(s),
                ::nstd::utility::forward<Fun>(fun)
            };
        }
    } lazy_then;
}

// ----------------------------------------------------------------------------

#endif
