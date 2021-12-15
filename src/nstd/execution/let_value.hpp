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
#include "nstd/execution/receiver.hpp"
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
#include <optional>
#include <tuple>
#include <variant>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    inline constexpr struct let_value_t {
        template <::nstd::execution::sender, typename> struct connector;
        template <::nstd::execution::sender Sender, typename Function, typename Receiver>
        struct state;
        template <::nstd::execution::sender Sender, typename Function>
        struct sender;

        template <::nstd::execution::sender Sender, typename Function>
        friend auto tag_invoke(let_value_t, Sender&& sndr, Function&& fun) {
            return sender<::nstd::type_traits::remove_cvref_t<Sender>, ::nstd::type_traits::remove_cvref_t<Function>>{
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
        template <typename Function>
        auto operator()(Function&& fun) const {
            return [this, fun = ::nstd::utility::forward<Function>(fun)](::nstd::execution::sender auto&& sender) mutable{
                return (*this)(::nstd::utility::forward<decltype(sender)>(sender),
                               ::nstd::utility::move(fun));
            };
        }
    } let_value;
}

// ----------------------------------------------------------------------------

template <::nstd::execution::sender Sender, typename Receiver>
struct nstd::execution::let_value_t::connector
{
    using state_type
        = ::nstd::type_traits::remove_cvref_t<decltype(
            ::nstd::execution::connect(::nstd::type_traits::declval<Sender>(),
                                       ::nstd::type_traits::declval<Receiver>())
            )>;
    state_type d_state;
    template <::nstd::execution::sender S, typename R>
    connector(S&& sender, R&& receiver)
        : d_state(::nstd::execution::connect(::nstd::utility::forward<S>(sender),
                                             ::nstd::utility::forward<R>(receiver)))
    {
    }
    auto start()
    {
        ::nstd::execution::start(this->d_state);
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::sender Sender, typename Function, typename Receiver>
struct nstd::execution::let_value_t::state
{
    template <typename... A>
    using value_tuple = ::std::tuple<::nstd::type_traits::remove_cvref_t<A>...>;
    using upstream_result_type = typename Sender::template value_types<value_tuple, ::std::variant>;

    template <typename ... T>
    using call_type = decltype(::nstd::type_traits::declval<Function>()(::nstd::type_traits::declval<T>()...));
    using downstream_sender = 
        typename Sender::template value_types<call_type, ::nstd::type_traits::type_identity_t>;

    struct upstream_receiver {
        state* d_state;
        template <typename... A>
        friend auto tag_invoke(::nstd::execution::set_value_t, upstream_receiver r, A&&... args)
            noexcept -> void {
            using type = value_tuple<A...>;
            r.d_state->d_upstream_result.template emplace<type>(::nstd::utility::forward<A>(args)...);
            r.d_state->start_downstream();
        }
        template <typename E>
        friend auto tag_invoke(::nstd::execution::set_error_t, upstream_receiver r, E&& e)
            noexcept -> void {
            ::nstd::execution::set_error(::nstd::utility::move(r.d_state->d_receiver), ::nstd::utility::forward<E>(e));
        }
        friend auto tag_invoke(::nstd::execution::set_done_t, upstream_receiver r)
            noexcept -> void {
            ::nstd::execution::set_done(::nstd::utility::move(r.d_state->d_receiver));
        }
    };
    struct downstream_receiver {
        state* d_state;
        template <typename... A>
        friend auto tag_invoke(::nstd::execution::set_value_t, downstream_receiver r, A&&... args)
            noexcept -> void {
            ::nstd::execution::set_value(::nstd::utility::move(r.d_state->d_receiver),
                                         ::nstd::utility::forward<A>(args)...);
        }
        template <typename E>
        friend auto tag_invoke(::nstd::execution::set_error_t, downstream_receiver r, E&& e)
            noexcept -> void {
            ::nstd::execution::set_error(::nstd::utility::move(r.d_state->d_receiver), ::nstd::utility::forward<E>(e));
        }
        friend auto tag_invoke(::nstd::execution::set_done_t, downstream_receiver r)
            noexcept -> void {
            ::nstd::execution::set_done(::nstd::utility::move(r.d_state->d_receiver));
        }
    };

    ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
    ::nstd::type_traits::remove_cvref_t<Function> d_function;
    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    ::std::optional<connector<Sender, upstream_receiver>> d_upstream_state;
    upstream_result_type d_upstream_result;
    ::std::optional<connector<downstream_sender, downstream_receiver>> d_downstream_state;
    
    template <typename S, typename F, typename R>
    state(S&& s, F&& f, R&& r)
        : d_sender(::nstd::utility::forward<S>(s))
        , d_function(::nstd::utility::forward<F>(f))
        , d_receiver(::nstd::utility::forward<R>(r))
    {
    }

    auto start_downstream() {
        ::std::visit([this](auto&& result){
            this->d_downstream_state.emplace(::std::apply([this](auto&&...a){
                 return this->d_function(::nstd::utility::forward<decltype(a)>(a)...);
                }, result),
                downstream_receiver{this});
        }, this->d_upstream_result),
        this->d_downstream_state->start();
    }

    friend auto tag_invoke(::nstd::execution::start_t, state& s)
        noexcept -> void {
        s.d_upstream_state.emplace(s.d_sender, upstream_receiver{&s});
        s.d_upstream_state->start();
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::sender Sender, typename Function>
struct nstd::execution::let_value_t::sender
{
    template <typename ... T>
    using call_type = decltype(::nstd::type_traits::declval<Function>()(::nstd::type_traits::declval<T>()...));
    using returned_sender = 
        typename Sender::template value_types<call_type, ::nstd::type_traits::type_identity_t>;

    template <template <typename...> class T, template <typename...> class V>
    using value_types = typename returned_sender::template value_types<T, V>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;
    static constexpr bool sends_done = true;

    Sender   d_sender;
    Function d_function;
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
    friend auto tag_invoke(::nstd::execution::connect_t, sender const& sndr, Receiver&& receiver)
        noexcept {
        return state<Sender, Function, Receiver>{
            sndr.d_sender,
            sndr.d_function,
            ::nstd::utility::forward<Receiver>(receiver)
            };
    }
};

// ----------------------------------------------------------------------------

#endif
