// nstd/execution/when_all.hpp                                        -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_WHEN_ALL
#define INCLUDED_NSTD_EXECUTION_WHEN_ALL

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include <atomic>
#include <exception>
#include <tuple>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    inline constexpr struct when_all_t
    {
        template <::nstd::execution::receiver Receiver>
        struct common {
            ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
            ::std::atomic<::std::size_t>                  d_count;
            auto complete() -> void {
                if (--this->d_count == 0u) {
                    ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver));
                }
            }
        };
        template <::nstd::execution::receiver Receiver>
        struct receiver {
            common<Receiver>* d_common;
            friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& r, auto&&...)
                noexcept -> void {
                r.d_common->complete();
            }
            friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& r, auto&&)
                noexcept -> void {
                r.d_common->complete();
            }
            friend auto tag_invoke(::nstd::execution::set_done_t, receiver&& r)
                noexcept -> void {
                r.d_common->complete();
            }
        };
        template <::nstd::execution::sender Sender, ::nstd::execution::receiver Receiver>
        struct nested_state_builder {
            Sender            d_sender;
            common<Receiver>* d_common;
        };
        template <::nstd::execution::sender Sender, ::nstd::execution::receiver Receiver>
        struct nested_state {
            decltype(::nstd::execution::connect(::nstd::type_traits::declval<Sender>(),
                                                ::nstd::type_traits::declval<receiver<Receiver>>())) d_state;
            nested_state(nested_state_builder<Sender, Receiver>&& b)
                : d_state(::nstd::execution::connect(::nstd::utility::move(b.d_sender),
                                                     receiver<Receiver>{b.d_common}))
            {
            }
            auto start() noexcept -> void { ::nstd::execution::start(this->d_state); }
        };
        template <::nstd::execution::receiver Receiver, ::nstd::execution::sender... Sender>
        struct state {
            common<Receiver>                                d_common;
            ::std::tuple<nested_state<Sender, Receiver>...> d_state;

            template <::nstd::execution::receiver R>
            state(R&& r, Sender&&... s)
                : d_common{::nstd::utility::forward<R>(r), sizeof...(Sender)}
                , d_state(nested_state_builder<Sender, Receiver>{::nstd::utility::move(s), &this->d_common}...)
            {
            }
            state(state&&) = delete;
            state(state const&) = delete;
            friend auto tag_invoke(::nstd::execution::start_t, state& s)
                noexcept -> void {
                ::std::apply([](auto&... st){ (st.start(), ...); }, s.d_state);
            }
        };
        template <::nstd::execution::sender... Sender>
        struct sender {
            using completion_signatures = ::nstd::execution::completion_signatures<
                    //-dk:TODO fix when_all completion_signals
                    ::nstd::execution::set_value_t(),
                    ::nstd::execution::set_done_t()
                >;

            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<>>;
            template <template <typename...> class V>
            using error_types = V<::std::exception_ptr>;
            static constexpr bool sends_done = true;

            ::std::tuple<::nstd::type_traits::remove_cvref_t<Sender>...> d_sender;
            template <::nstd::execution::receiver Receiver>
            friend auto tag_invoke(::nstd::execution::connect_t,
                                   sender&& s,
                                   Receiver&& r)
                noexcept -> state<Receiver, Sender...> {
                return ::std::apply([&r](auto&&... s){
                    return state<Receiver, Sender...>(
                        ::nstd::utility::forward<Receiver>(r),
                        ::nstd::utility::move(s)...);
                    }, ::nstd::utility::move(s.d_sender));
            }
        };
        template <::nstd::execution::sender... Sender>
        auto operator()(Sender&&... s) const {
            return sender<Sender...>{ { ::nstd::utility::forward<Sender>(s)... } };
        }
        auto operator()() const { // deal with the odd case of no senders
            return ::nstd::execution::just();
        }
    } when_all;
}

// ----------------------------------------------------------------------------

#endif
