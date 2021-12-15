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

#include "nstd/execution/connect.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/receiver_of.hpp"
#include "nstd/execution/sender_base.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/hidden_names/movable_value.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/type_traits/is_nothrow_constructible.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/move.hpp"
#include <exception>
#include <tuple>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    template <typename... A>
    struct just_sender
        : ::nstd::execution::piped_sender_base
    {
        template <template <typename...> class T, template <typename...> class V>
        using value_types = V<T<A...>>;
        template <template <typename...> class V>
        using error_types = V<::std::exception_ptr>;
        static constexpr bool sends_done = false;

        template <::nstd::execution::receiver Receiver>
        struct operation_state {
            ::std::tuple<A...>  d_value;
            Receiver            d_receiver;
            friend auto tag_invoke(::nstd::execution::start_t, operation_state& s) noexcept -> void
                try {
                    (void)s;
                    ::std::apply([&s](A&&... a){
                                       nstd::execution::set_value(::nstd::utility::move(s.d_receiver),
                                                                  ::nstd::utility::forward<decltype(a)>(a)...);
                                 },
                                 ::nstd::utility::move(s.d_value));
                }
                catch (...) {
                    ::nstd::execution::set_error(::nstd::utility::move(s.d_receiver),
                                                 ::std::current_exception());
                }
        };

        ::std::tuple<A...> d_value;

        template <::nstd::execution::receiver Receiver>
            requires ::nstd::execution::receiver_of<Receiver, A...>
        friend auto tag_invoke(::nstd::execution::connect_t,
                               just_sender const& sender,
                               Receiver&&  receiver)
        {
            return operation_state<Receiver>{sender.d_value, ::nstd::utility::forward<Receiver>(receiver)};
        }
        template <::nstd::execution::receiver Receiver>
            requires ::nstd::execution::receiver_of<Receiver, A...>
        friend auto tag_invoke(::nstd::execution::connect_t,
                               just_sender&& sender,
                               Receiver&&  receiver)
        {
            return operation_state<Receiver>{::nstd::utility::move(sender.d_value),
                                             ::nstd::utility::forward<Receiver>(receiver)};
        }
    };

    static_assert(::nstd::execution::operation_state<::nstd::hidden_names::just_sender<int>::operation_state<::nstd::execution::test_receiver>>);
}

namespace nstd::execution {
    template <::nstd::hidden_names::movable_value... A>
    auto just(A&&... a) noexcept((::nstd::type_traits::is_nothrow_constructible_v<
                                      ::nstd::type_traits::remove_cvref_t<A>,
                                      A> && ...))
    {
        return ::nstd::hidden_names::just_sender<::nstd::type_traits::remove_cvref_t<A>...>{
            {}, ::std::make_tuple(::nstd::utility::forward<A>(a)...)
            };
    }
}

// ----------------------------------------------------------------------------

#endif
