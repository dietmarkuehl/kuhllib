// nstd/execution/just.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_JUST
#define INCLUDED_NSTD_EXECUTION_JUST

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_attrs.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/receiver_of.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/hidden_names/movable_value.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/type_traits/is_nothrow_constructible.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/move.hpp"
#include <tuple>

// ----------------------------------------------------------------------------
// [exec.just]

namespace nstd::hidden_names::just {
    struct env {};

    template <typename Signal, typename... A>
    struct sender_cloak {
        struct sender
            : ::nstd::execution::sender_tag
        {
            using completion_signatures
                = ::nstd::execution::completion_signatures<
                    Signal(A...),
                    ::nstd::execution::set_error_t(::std::exception_ptr)
                    >;

            template <::nstd::execution::receiver Receiver>
            struct operation_state_cloak {
                struct operation_state {
                    ::std::tuple<A...>  d_value;
                    Receiver            d_receiver;
                    friend auto tag_invoke(::nstd::execution::start_t, operation_state& s) noexcept -> void
                        try {
                            ::std::apply([&s](A&&... a){
                                                Signal{}(::nstd::utility::move(s.d_receiver),
                                                         ::nstd::utility::forward<decltype(a)>(a)...);
                                         },
                                         ::nstd::utility::move(s.d_value));
                        }
                        catch (...) {
                            ::nstd::execution::set_error(::nstd::utility::move(s.d_receiver),
                                                         ::std::current_exception());
                        }
                };
            };

            [[no_unique_address]] ::std::tuple<A...> d_value;

            template <typename... Args>
                requires ::std::is_constructible_v<::std::tuple<A...>, Args...>
            sender(Args&&... args): d_value{::nstd::utility::forward<Args>(args)...} {}
            sender(sender&) = default;
            sender(sender&&) = default;
            sender(sender const&) = default;

            friend auto tag_invoke(::nstd::execution::get_attrs_t, sender const&) noexcept
                -> env
            {
                return {};
            }
            template <::nstd::execution::receiver Receiver>
                requires ::nstd::execution::receiver_of<Receiver, completion_signatures>
            friend auto tag_invoke(::nstd::execution::connect_t,
                                   sender const& sndr,
                                   Receiver&&  receiver)
            {
                return typename operation_state_cloak<Receiver>::operation_state{sndr.d_value, ::nstd::utility::forward<Receiver>(receiver)};
            }
            template <::nstd::execution::receiver Receiver>
                requires ::nstd::execution::receiver_of<Receiver, completion_signatures>
            friend auto tag_invoke(::nstd::execution::connect_t,
                                   sender&& sndr,
                                   Receiver&&  receiver)
            {
                return typename operation_state_cloak<Receiver>::operation_state{::nstd::utility::move(sndr.d_value),
                                                 ::nstd::utility::forward<Receiver>(receiver)};
            }
        };
    };
}

namespace nstd::execution {
    template <::nstd::hidden_names::movable_value... A>
    inline auto just(A&&... a) noexcept((::std::is_nothrow_constructible_v<::nstd::type_traits::remove_cvref_t<A>, A> && ...))
        -> typename ::nstd::hidden_names::just::sender_cloak<::nstd::execution::set_value_t, ::nstd::type_traits::remove_cvref_t<A>...>::sender {
        return typename ::nstd::hidden_names::just::sender_cloak<::nstd::execution::set_value_t, ::nstd::type_traits::remove_cvref_t<A>...>::sender(
            ::nstd::utility::forward<A>(a)...
            );
    }

    template <::nstd::hidden_names::movable_value E>
    inline auto just_error(E&& e) noexcept(::std::is_nothrow_constructible_v<::nstd::type_traits::remove_cvref_t<E>, E>)
        -> typename ::nstd::hidden_names::just::sender_cloak<::nstd::execution::set_error_t, ::nstd::type_traits::remove_cvref_t<E>>::sender {
        return typename ::nstd::hidden_names::just::sender_cloak<::nstd::execution::set_error_t, ::nstd::type_traits::remove_cvref_t<E>>::sender(
            ::nstd::utility::forward<E>(e)
            );
    }

    inline auto just_stopped() noexcept
        -> typename ::nstd::hidden_names::just::sender_cloak<::nstd::execution::set_stopped_t>::sender {
        return typename ::nstd::hidden_names::just::sender_cloak<::nstd::execution::set_stopped_t>::sender();
    }
}

// ----------------------------------------------------------------------------

#endif
