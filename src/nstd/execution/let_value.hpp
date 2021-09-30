// nstd/execution/let_value.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_LET_VALUE
#define INCLUDED_NSTD_EXECUTION_LET_VALUE

#include "nstd/execution/sender.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/type_identity.hpp"

// ----------------------------------------------------------------------------

namespace nstd::execution {
    inline constexpr struct let_value_t {
        template <::nstd::execution::sender Sender, typename Function, typename Receiver>
        struct state
        {
            ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
            ::nstd::type_traits::remove_cvref_t<Function> d_function;
            ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
            template <typename S, typename F, typename R>
            state(S&& s, F&& f, R&& r)
                : d_sender(::nstd::utility::forward<S>(s))
                , d_function(::nstd::utility::forward<F>(f))
                , d_receiver(::nstd::utility::forward<R>(r))
            {
            }

            friend auto tag_invoke(::nstd::execution::start_t, state&)
                noexcept -> void {

            }
        };
        template <::nstd::execution::sender Sender, typename Function>
        struct sender
        {
            template <typename ... T>
            using call_type = decltype(::nstd::type_traits::declval<Function>()(::nstd::type_traits::declval<T>()...));
            using returned_sender = 
                typename Sender::template value_types<::nstd::type_traits::type_identity_t, call_type>;

            template <template <typename...> class V, template <typename...> class T>
            using value_types = typename returned_sender::template value_types<V, T>;
            template <template <typename...> class V>
            using error_types = V<::std::exception_ptr>;
            static constexpr bool sends_done = true;

            ::nstd::type_traits::remove_cvref_t<Sender>   d_sender;
            ::nstd::type_traits::remove_cvref_t<Function> d_function;
            template <typename Receiver>
            friend auto tag_invoke(::nstd::execution::connect_t, sender&& sndr, Receiver&& receiver)
                noexcept {
                return state<Sender, Function, Receiver>{
                    ::nstd::utility::move(sndr.d_sender),
                    ::nstd::utility::move(sndr.d_function),
                    ::nstd::utility::forward<Receiver>(receiver)
                    };
            }
            template <typename Receiver>
            friend auto tag_invoke(::nstd::execution::connect_t, sender& sndr, Receiver&& receiver)
                noexcept {
                return state<Sender, Function, Receiver>{
                    sndr.d_sender,
                    sndr.d_function,
                    ::nstd::utility::forward<Receiver>(receiver)
                    };
            }
        };

        template <::nstd::execution::sender Sender, typename Function>
        friend auto tag_invoke(let_value_t, Sender&& sndr, Function&& fun) {
            return sender<Sender, Function>{
                ::nstd::utility::forward<Sender>(sndr),
                ::nstd::utility::forward<Function>(fun)
                };
        }

        template <::nstd::execution::sender Sender, typename Function>
        auto operator()(Sender&& sender, Function&& fun) const {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::utility::forward<Sender>(sender),
                                      ::nstd::utility::forward<Function>(fun));
        }
    } let_value;
}

// ----------------------------------------------------------------------------

#endif
