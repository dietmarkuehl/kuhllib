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
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/get_scheduler.hpp"
#include "nstd/execution/get_stop_token.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/error_types_of_t.hpp"
#include "nstd/execution/value_types_of_t.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/hidden_names/merge_completion_signatures.hpp"
#include "nstd/hidden_names/variant_or_empty.hpp"
#include "nstd/stop_token/in_place_stop_token.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/type_traits/type_identity.hpp"
#include "nstd/utility/forward.hpp"
#include <atomic>
#include <exception>
#include <optional>
#include <tuple>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names::when_all {
    enum class completion_kind { set_value, set_error, set_stopped };
    template <::nstd::execution::receiver, typename>
    struct state_base_cloak;
    template <::nstd::execution::receiver Receiver, typename Error>
    using state_base = typename state_base_cloak<Receiver, Error>::state_base;

    template <::nstd::execution::receiver, typename>
    struct environment_cloak;
    template <::nstd::execution::receiver Receiver, typename Error>
    using environment = typename environment_cloak<Receiver, Error>::environment;

    template <::nstd::execution::receiver, typename, typename>
    struct receiver_cloak;
    template <::nstd::execution::receiver Receiver, typename Error, typename Result>
    using receiver = typename receiver_cloak<Receiver, Error, Result>::receiver;

    template <::nstd::execution::receiver, typename, ::nstd::execution::sender>
    struct inner_state_args_cloak;
    template <::nstd::execution::receiver Receiver, typename Error, ::nstd::execution::sender Sender>
    using inner_state_args = typename inner_state_args_cloak<Receiver, Error, Sender>::inner_state_args;

    template <::nstd::execution::receiver, typename, ::nstd::execution::sender>
    struct inner_state_cloak;
    template <::nstd::execution::receiver Receiver, typename Error, ::nstd::execution::sender Sender>
    using inner_state = typename inner_state_cloak<Receiver, Error, Sender>::inner_state;

    template <::nstd::execution::receiver, typename, ::nstd::execution::sender...>
    struct state_cloak;
    template <::nstd::execution::receiver Receiver, typename Error, ::nstd::execution::sender... Sender>
    using state = typename state_cloak<Receiver, Error, Sender...>::state;

    template <::nstd::execution::sender... Sender> struct sender_cloak;
    template <::nstd::execution::sender... Sender> using sender = typename sender_cloak<Sender...>::sender;

    template <typename> struct set_value_from_tuple_impl;
    template <typename... T>
    struct set_value_from_tuple_impl<::std::tuple<T...>> {
        using type = ::nstd::execution::set_value_t(T...);
    };
    template <typename T>
        using set_value_from_tuple = typename ::nstd::hidden_names::when_all::set_value_from_tuple_impl<T>::type;

    struct cpo {
        template <::nstd::execution::sender... Sender>
        friend auto tag_invoke(cpo, Sender&&... s) -> ::nstd::hidden_names::when_all::sender<Sender...> {
            return  { { ::nstd::utility::forward<Sender>(s)... } };
        }

        template <::nstd::execution::sender... Sender>
            requires (0ul != sizeof...(Sender))
                  && requires(cpo const& c, Sender&&... s) {
                    { ::nstd::tag_invoke(c, ::nstd::utility::forward<Sender>(s)...) } -> ::nstd::execution::sender;
                  }
        auto operator()(Sender&&... sender) const {
            return ::nstd::tag_invoke(*this, ::nstd::utility::forward<Sender>(sender)...);
        }
    };

    template <typename> struct transform;
    template <typename Arg>
    struct transform<::nstd::execution::set_error_t(Arg)> {
        using type = Arg;
    };
    template <typename, template <typename...> class>
    struct transform_list;
    template <typename... Signature, template <typename...> class Variant>
    struct transform_list<::nstd::execution::completion_signatures<Signature...>, Variant> {
        using type = Variant<typename transform<Signature>::type...>;
    };
    template <typename... T>
    using variant_t = ::nstd::execution::completion_signatures<::nstd::execution::set_error_t(T)...>;
}

// ----------------------------------------------------------------------------

namespace nstd::execution {
    using when_all_t = ::nstd::hidden_names::when_all::cpo;
    inline constexpr ::nstd::execution::when_all_t when_all{};
}

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver, typename Error>
struct nstd::hidden_names::when_all::state_base_cloak {
    struct state_base;
};

template <::nstd::execution::receiver Receiver, typename Error>
struct nstd::hidden_names::when_all::state_base_cloak<Receiver, Error>::state_base {
    using stop_token_t = decltype(::nstd::execution::get_stop_token(::nstd::type_traits::declval<Receiver const&>()));
    struct on_stop {
        ::nstd::stop_token_ns::in_place_stop_source& d_stop_source;
        auto operator()() {
            this->d_stop_source.stop();
        }
    };

    ::std::atomic<::nstd::hidden_names::when_all::completion_kind> d_kind{::nstd::hidden_names::when_all::completion_kind::set_value};
    ::std::atomic<::std::size_t>                                   d_count;
    ::nstd::stop_token_ns::in_place_stop_source                    d_stop_source;
    typename stop_token_t::template callback_type<on_stop>         d_stop_callback;
    ::nstd::type_traits::remove_cvref_t<Receiver>                  d_receiver;
    ::std::optional<Error>                                         d_error;

    template <::nstd::execution::receiver R>
    state_base(R&& receiver, ::std::size_t count)
        : d_count(count)
        , d_stop_callback(::nstd::execution::get_stop_token(receiver), on_stop{this->d_stop_source})
        , d_receiver(::nstd::utility::forward<R>(receiver)) {
    }
    state_base(state_base&&) = delete;
    auto complete() -> void {
        if (--this->d_count == 0u) {
            this->do_complete();
        }
    }
    virtual auto do_complete() -> void = 0;
};

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver, typename Error>
struct nstd::hidden_names::when_all::environment_cloak {
    struct environment;
};

template <::nstd::execution::receiver Receiver, typename Error>
struct nstd::hidden_names::when_all::environment_cloak<Receiver, Error>::environment {
    ::nstd::hidden_names::when_all::state_base<Receiver, Error>& d_state;

    friend auto tag_invoke(::nstd::execution::get_scheduler_t const&, environment const& self) noexcept {
        return ::nstd::execution::get_scheduler(::nstd::execution::get_env(self.d_state.d_receiver));
    }
    friend auto tag_invoke(::nstd::execution::get_stop_token_t const&, environment const& self) noexcept
        -> ::nstd::stop_token_ns::in_place_stop_token {
        return self.d_state.d_stop_source.token();
    }
    friend auto tag_invoke(::nstd::execution::get_env_t const&, environment const& self) noexcept {
        return ::nstd::execution::get_env(self.d_state.d_receiver);
    }
#if 0
    template <typename CPO, typename... Args>
        requires requires(CPO const& cpo, environment const& self, Args... args) {
            cpo(::nstd::execution::get_env(self.d_state.d_receiver),
                ::nstd::utility::forward<Args>(args)...);
        }
    friend auto tag_invoke(CPO const& cpo, environment const& self, Args&&... args) noexcept {
        return cpo(::nstd::execution::get_env(self.d_state.d_receiver),
                   ::nstd::utility::forward<Args>(args)...);
    }
#endif
};

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver, typename Error, typename Result>
struct nstd::hidden_names::when_all::receiver_cloak {
    struct receiver;
};

template <::nstd::execution::receiver Receiver, typename Error, typename Result>
struct nstd::hidden_names::when_all::receiver_cloak<Receiver, Error, Result>::receiver {
    state_base<Receiver, Error>&  d_state;
    ::std::optional<Result>&      d_result;

    friend auto tag_invoke(::nstd::execution::get_env_t, receiver const& self) noexcept
        -> ::nstd::hidden_names::when_all::environment<Receiver, Error> {
        return { self.d_state };
    }

    template <typename... T>
    friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& self, T&&... args) noexcept
         -> void {
        self.d_result.emplace(::nstd::utility::forward<T>(args)...);
        self.d_state.complete();
    }
    template <typename E>
    friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& self, E&& error) noexcept
         -> void {
        ::nstd::hidden_names::when_all::completion_kind expected{::nstd::hidden_names::when_all::completion_kind::set_value};
        if (self.d_state.d_kind.compare_exchange_strong(expected, ::nstd::hidden_names::when_all::completion_kind::set_error)) {
            self.d_state.d_stop_source.stop();
            (void)error; //-dk:TODO remove
            static_assert(::std::is_constructible_v<Error, E>); //-dk:TODO remove
            self.d_state.d_error.emplace(::nstd::utility::forward<E>(error));
        }
        self.d_state.complete();
    }
    friend auto tag_invoke(::nstd::execution::set_stopped_t, receiver&& self) noexcept
         -> void {
        ::nstd::hidden_names::when_all::completion_kind expected{::nstd::hidden_names::when_all::completion_kind::set_value};
        if (self.d_state.d_kind.compare_exchange_strong(expected, ::nstd::hidden_names::when_all::completion_kind::set_stopped)) {
            self.d_state.d_stop_source.stop();
        }
        self.d_state.complete();
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver, typename Error, ::nstd::execution::sender Sender>
struct nstd::hidden_names::when_all::inner_state_args_cloak {
    struct inner_state_args;
};

template <::nstd::execution::receiver Receiver, typename Error, ::nstd::execution::sender Sender>
struct nstd::hidden_names::when_all::inner_state_args_cloak<Receiver, Error, Sender>::inner_state_args {
    ::nstd::hidden_names::when_all::state_base<Receiver, Error>& d_outer_state;
    Sender&                                               d_sender;

    inner_state_args(::nstd::hidden_names::when_all::state_base<Receiver, Error>& outer_state,
                     Sender&                                               sender)
        : d_outer_state(outer_state)
        , d_sender(sender) {
    }
};

template <::nstd::execution::receiver Receiver, typename Error, ::nstd::execution::sender Sender>
struct nstd::hidden_names::when_all::inner_state_cloak {
    struct inner_state;
};

template <::nstd::execution::receiver Receiver, typename Error, ::nstd::execution::sender Sender>
struct nstd::hidden_names::when_all::inner_state_cloak<Receiver, Error, Sender>::inner_state {
    using result_t = ::nstd::execution::value_types_of_t<Sender, int, ::nstd::hidden_names::decayed_tuple, ::nstd::type_traits::type_identity_t>;
    using state_t = decltype(::nstd::execution::connect(::nstd::type_traits::declval<Sender>(),
                                                        ::nstd::type_traits::declval<::nstd::hidden_names::when_all::receiver<Receiver, Error, result_t>>()));

    ::nstd::hidden_names::when_all::state_base<Receiver, Error>& d_outer_state;
    ::std::optional<result_t>                             d_result;
    state_t                                               d_state;

    inner_state(inner_state_args<Receiver, Error, Sender>&& args)
        : d_outer_state(args.d_outer_state)
        , d_result()
        , d_state(::nstd::execution::connect(::nstd::utility::move(args.d_sender),
                                             ::nstd::hidden_names::when_all::receiver<Receiver, Error, result_t>{this->d_outer_state, this->d_result}))
    {
    }
    inner_state(inner_state&&) = delete;

    auto start() noexcept -> void {
        ::nstd::execution::start(this->d_state);
    }
    auto result() -> result_t { return ::nstd::utility::move(*this->d_result); }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver, typename Error, ::nstd::execution::sender... Sender>
struct nstd::hidden_names::when_all::state_cloak {
    struct state;
};

template <::nstd::execution::receiver Receiver, typename Error, ::nstd::execution::sender... Sender>
struct nstd::hidden_names::when_all::state_cloak<Receiver, Error, Sender...>::state
    : ::nstd::hidden_names::when_all::state_base<Receiver, Error>
{
    using state_t = ::std::tuple<::nstd::hidden_names::when_all::inner_state<Receiver, Error, Sender>...>;
    state_t                                       d_state;

    friend auto tag_invoke(::nstd::execution::start_t, state& self) noexcept
        -> void {
        ::std::apply([](auto&... inner){ (inner.start(), ...); }, self.d_state);
    }

    template<::nstd::execution::receiver R, typename Tuple>
    state(R&& receiver, Tuple&& sender)
        : ::nstd::hidden_names::when_all::state_base<Receiver, Error>(::nstd::utility::forward<R>(receiver), sizeof...(Sender))
        , d_state(::std::apply([this](auto&&...s){ return ::std::make_tuple(::nstd::hidden_names::when_all::inner_state_args<Receiver, Error, Sender>(*this, s)...); },
                               ::nstd::utility::forward<Tuple>(sender))) 
    {
    }

    auto do_complete() -> void override {
        switch (this->d_kind) {
        case ::nstd::hidden_names::when_all::completion_kind::set_value:
            ::std::apply([this](auto&&... a){ ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver), a...); },
                         ::std::apply([](auto&&... t){ return ::std::tuple_cat(t.result()...); }, this->d_state)
                         );
            break;
        case ::nstd::hidden_names::when_all::completion_kind::set_error:
            if constexpr (not ::nstd::type_traits::is_same_v<::nstd::type_traits::remove_cvref_t<decltype(*this->d_error)>, ::nstd::hidden_names::variant_or_empty_impl::empty_variant>) {
                ::std::visit([this](auto&& error){ ::nstd::execution::set_error(::nstd::utility::move(this->d_receiver), error); }, *this->d_error);
            }
            break;
        case ::nstd::hidden_names::when_all::completion_kind::set_stopped:
            ::nstd::execution::set_stopped(::nstd::utility::move(this->d_receiver));
            break;
        }
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::sender... Sender>
struct nstd::hidden_names::when_all::sender_cloak {
    struct sender;
};

template <::nstd::execution::sender... Sender>
struct nstd::hidden_names::when_all::sender_cloak<Sender...>::sender
{
    ::std::tuple<Sender...> d_sender;

    template <typename Env>
    friend auto tag_invoke(::nstd::execution::get_completion_signatures_t, sender const&, Env&&)
    {
        using env_t = ::nstd::type_traits::remove_cvref_t<Env>;
        using set_value_list = ::nstd::execution::completion_signatures<
            ::nstd::hidden_names::when_all::set_value_from_tuple<
                decltype(
                    ::std::tuple_cat(
                        ::nstd::type_traits::declval<
                            ::nstd::execution::value_types_of_t<Sender,
                                                               env_t,
                                                               ::std::tuple,
                                                               ::nstd::type_traits::type_identity_t>
                        >()
                        ...
                    )
                )
            >
        >;
        using set_error_list = ::nstd::hidden_names::merge_completion_signatures_t<
            env_t,
            ::nstd::execution::error_types_of_t<Sender,
                                                env_t,
                                                ::nstd::hidden_names::when_all::variant_t>...
            >;
        return ::nstd::hidden_names::merge_completion_signatures_t<
            env_t,
            set_value_list,
            set_error_list,
            ::nstd::execution::completion_signatures<::nstd::execution::set_stopped_t()>
        >();
    }
    template <::nstd::execution::receiver Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender&& self, Receiver&& receiver) {
            using env_t = decltype(::nstd::execution::get_env(receiver));
            using error_signatures_t = ::nstd::hidden_names::merge_completion_signatures_t<
                env_t,
                ::nstd::execution::error_types_of_t<Sender, env_t, ::nstd::hidden_names::when_all::variant_t>...
                >;
            using error_t = typename ::nstd::hidden_names::when_all::transform_list<error_signatures_t, ::nstd::hidden_names::variant_or_empty>::type;
            return ::nstd::hidden_names::when_all::state<Receiver, error_t, Sender...>(
                ::nstd::utility::forward<Receiver>(receiver),
                ::nstd::utility::move(self.d_sender)
            );
    }
};

// ----------------------------------------------------------------------------

#endif
