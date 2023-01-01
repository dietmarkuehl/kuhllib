// nstd/execution/let.hpp                                             -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_LET
#define INCLUDED_NSTD_EXECUTION_LET

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/get_completion_signatures.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/sender_adaptor_closure.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/hidden_names/filter_completions.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/type_traits/remove_reference.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/type_identity.hpp"
#include <optional>
#include <tuple>
#include <variant>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    template <typename CPO, typename Sender, typename... Args>
    concept cpo_scheduler_tag_invocable
        = requires(CPO const& cpo, Sender&& sender, Args&&... args){
                {
                    tag_invoke(cpo,
                               ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                               ::nstd::utility::forward<Sender>(sender), ::nstd::utility::forward<Args>(args)...)
                } -> ::nstd::execution::sender;
            }
        ;
    template <typename CPO, typename Sender, typename... Args>
    concept cpo_tag_invocable
        = requires(CPO const& cpo, Sender&& sender, Args&&... args){
                {
                    tag_invoke(cpo,
                               ::nstd::utility::forward<Sender>(sender), ::nstd::utility::forward<Args>(args)...)
                } -> ::nstd::execution::sender;
            }
        ;
}

namespace nstd::hidden_names::let {
    struct none {};

    template <typename Tag>
    concept is_completion_signal
        =  (::std::same_as<::nstd::execution::set_value_t, ::nstd::type_traits::remove_cvref_t<Tag>>)
        || (::std::same_as<::nstd::execution::set_error_t, ::nstd::type_traits::remove_cvref_t<Tag>>)
        || (::std::same_as<::nstd::execution::set_stopped_t, ::nstd::type_traits::remove_cvref_t<Tag>>)
        ;

    template <typename Tag, typename Fun>
    concept invocable
        =  (not ::std::same_as<Tag, ::nstd::execution::set_stopped_t>)
        || ::std::invocable<Fun>
        ;

    template <::nstd::execution::receiver, typename, typename> struct inner_state;
    template <::nstd::execution::receiver FinalReceiver, typename Fun, typename Tag, typename... Args>
    struct inner_state<FinalReceiver, Fun, Tag(Args...)> {
        using sender_t = ::std::invoke_result_t<Fun, ::nstd::type_traits::remove_reference_t<Args>...>;
        using state_t  = ::std::invoke_result_t<::nstd::execution::connect_t, sender_t, FinalReceiver>;

        ::std::tuple<::nstd::type_traits::remove_reference_t<Args>...> d_args;
        state_t                                                        d_state;

        template <::nstd::execution::receiver R, typename F>
        inner_state(R&& receiver, F&& fun, Args... args)
            : d_args(::nstd::utility::forward<Args const&>(args)...)
            , d_state(::nstd::execution::connect(
                          ::std::apply([&](auto&&... a){ return fun(::nstd::utility::forward<Args>(a)...); }, this->d_args),
                          ::nstd::utility::forward<R>(receiver)))
        {
            static_assert(::std::same_as<decltype(::std::apply([&](auto&&... a){ return fun(::nstd::utility::forward<Args>(a)...); }, this->d_args)), sender_t>);
            ::nstd::execution::start(this->d_state);
        }
    };

    template <typename> struct state_base_emplace;
    template <typename C, typename... Args>
    struct state_base_emplace<C(Args...)> {
        template <typename V, typename R, typename F, typename... A>
        void emplace(V& var, R&& r, F&& f, A... a) {
            var.template emplace<inner_state<R, F, C(Args...)>>(::nstd::utility::move(r),
                                                                ::nstd::utility::move(f),
                                                                ::nstd::utility::forward<A>(a)...);
        }
    };
    template <::nstd::execution::receiver, typename, typename>
    struct state_base;
    template <::nstd::execution::receiver FinalReceiver, typename Fun, typename... C>
    struct state_base<FinalReceiver, Fun, ::nstd::execution::completion_signatures<C...>>
        : state_base_emplace<C>...
    {
        ::nstd::type_traits::remove_cvref_t<FinalReceiver> d_receiver;
        ::nstd::type_traits::remove_cvref_t<Fun>           d_fun;
        ::std::variant<::nstd::hidden_names::let::none, ::nstd::hidden_names::let::inner_state<FinalReceiver, Fun, C>...> d_inner_state;
        template <typename R, typename F>
        state_base(R&& r, F&& f)
            : d_receiver(::nstd::utility::forward<R>(r))
            , d_fun(::nstd::utility::forward<F>(f))
            , d_inner_state(::nstd::hidden_names::let::none()) {
        }
    };

    template <typename Tag, ::nstd::execution::receiver FinalReceiver, typename Fun, typename C>
    struct receiver_impl {
        struct hidden {
        ::nstd::hidden_names::let::state_base<FinalReceiver, Fun, C>& d_state;

        template <typename CPO, typename... A>
            requires (::nstd::hidden_names::let::is_completion_signal<CPO>)
        friend auto tag_invoke(CPO cpo, hidden&& self, A&&... a) noexcept -> void {
            if constexpr (::std::same_as<Tag, ::nstd::type_traits::remove_cvref_t<CPO>>) {
                self.d_state.emplace(self.d_state.d_inner_state,
                                     ::nstd::utility::move(self.d_state.d_receiver),
                                     ::nstd::utility::move(self.d_state.d_fun),
                                     ::nstd::utility::forward<A>(a)...);
            }
            else {
                //static_assert(::std::same_as<::nstd::execution::set_error_t(::std::exception_ptr), Tag(...)>
                //              || ::std::same_as<::nstd::execution::set_value_t(::std::string), Tag(...)>
                //              || ::std::same_as<int, Tag(A...)>);
                cpo(::nstd::utility::move(self.d_state.d_receiver), ::nstd::utility::forward<A>(a)...);
            }
        }

#if 0
        friend auto tag_invoke(::nstd::execution::get_env_t cpo, hidden const& self) noexcept {
            return cpo(self.d_state.d_receiver);
        }
#else
        template <typename CPO, typename... A>
            requires (not ::nstd::hidden_names::let::is_completion_signal<CPO>)
        friend auto tag_invoke(CPO cpo, hidden&& self, A&&... args) noexcept {
            return cpo(::nstd::utility::move(self.d_state.d_receiver), ::nstd::utility::forward<A>(args)...);
        }
        template <typename CPO, typename... A>
            requires (not ::nstd::hidden_names::let::is_completion_signal<CPO>)
        friend auto tag_invoke(CPO cpo, hidden const& self, A&&... args) noexcept {
            return cpo(self.d_state.d_receiver, ::nstd::utility::forward<A>(args)...);
        }
#endif
        };
    };

    template <typename Tag, ::nstd::execution::receiver FinalReceiver, typename Fun, typename C>
    using receiver = typename receiver_impl<Tag, FinalReceiver, Fun, C>::hidden;

    template <typename                    Tag,
              ::nstd::execution::sender   Sender,
              ::nstd::execution::receiver Receiver,
              typename                    Fun>
    struct state
        : ::nstd::hidden_names::let::state_base<Receiver, Fun, ::nstd::hidden_names::filter_completions_t<Tag, ::nstd::hidden_names::get_completion_signatures::from_sender_receiver<Sender, Receiver>>>
    {
        using completions_t = ::nstd::hidden_names::filter_completions_t<Tag, ::nstd::hidden_names::get_completion_signatures::from_sender_receiver<Sender, Receiver>>;
        using base_t = ::nstd::hidden_names::let::state_base<Receiver, Fun, completions_t>;
        using receiver_t = ::nstd::hidden_names::let::receiver<Tag, Receiver, Fun, completions_t>;
        static_assert(::nstd::execution::receiver<receiver_t>);
        static_assert(::nstd::execution::receiver_of<receiver_t, completions_t>);
        using state_t = decltype(::nstd::execution::connect(::nstd::type_traits::declval<Sender>(),
                                                            ::nstd::type_traits::declval<receiver_t>()));

        state_t d_state;

        template <typename F, ::nstd::execution::receiver R>
        state(Sender&& sender, F&& fun, R&& receiver)
            : base_t(::nstd::utility::forward<R>(receiver), ::nstd::utility::forward<F>(fun))
            , d_state(::nstd::execution::connect(::nstd::utility::forward<Sender>(sender),
                                                 receiver_t{ *this }))
        {
        }
        friend auto tag_invoke(::nstd::execution::start_t, state& self) noexcept -> void {
            ::nstd::execution::start(self.d_state);
        }
    };

    template <typename...> struct join_completions;
    template <>
    struct join_completions<> {
        using type = ::nstd::execution::completion_signatures<>;
    };
    template <typename... C>
    struct join_completions<::nstd::execution::completion_signatures<C...>> {
        using type = ::nstd::execution::completion_signatures<C...>;
    };
    template <typename... A, typename... B, typename... T>
    struct join_completions<::nstd::execution::completion_signatures<A...>, ::nstd::execution::completion_signatures<B...>, T...> {
        using type = typename join_completions<::nstd::execution::completion_signatures<A..., B...>, T...>::type;
    };

    template <typename, ::nstd::execution::sender, typename> struct rest;
    template <::nstd::execution::sender Sender, typename Env>
    struct rest<::nstd::execution::set_value_t, Sender, Env>
    {
        using completion_signatures = decltype(::nstd::execution::get_completion_signatures(::nstd::type_traits::declval<Sender>(),
                                                                                            ::nstd::type_traits::declval<Env>()));
        using type = typename nstd::hidden_names::let::join_completions<
            ::nstd::hidden_names::filter_completions_t<::nstd::execution::set_error_t, completion_signatures>,
            ::nstd::hidden_names::filter_completions_t<::nstd::execution::set_stopped_t, completion_signatures>
            >::type;
    };
    template <::nstd::execution::sender Sender, typename Env>
    struct rest<::nstd::execution::set_error_t, Sender, Env>
    {
        using completion_signatures = decltype(::nstd::execution::get_completion_signatures(::nstd::type_traits::declval<Sender>(),
                                                                                            ::nstd::type_traits::declval<Env>()));
        using type = typename nstd::hidden_names::let::join_completions<
            ::nstd::hidden_names::filter_completions_t<::nstd::execution::set_value_t, completion_signatures>,
            ::nstd::hidden_names::filter_completions_t<::nstd::execution::set_stopped_t, completion_signatures>
            >::type;
    };
    template <::nstd::execution::sender Sender, typename Env>
    struct rest<::nstd::execution::set_stopped_t, Sender, Env>
    {
        using completion_signatures = decltype(::nstd::execution::get_completion_signatures(::nstd::type_traits::declval<Sender>(),
                                                                                            ::nstd::type_traits::declval<Env>()));
        using type = typename nstd::hidden_names::let::join_completions<
            ::nstd::hidden_names::filter_completions_t<::nstd::execution::set_value_t, completion_signatures>,
            ::nstd::hidden_names::filter_completions_t<::nstd::execution::set_error_t, completion_signatures>
            >::type;
    };

    template <typename, typename, typename> struct transform_completion;
    template <typename Env, typename Fun, typename Tag, typename... A>
    struct transform_completion<Env, Fun, Tag(A...)> {
        using type = decltype(::nstd::execution::get_completion_signatures(::nstd::type_traits::declval<::std::invoke_result_t<Fun, A...>>(),
                                                                           ::nstd::type_traits::declval<Env>()));
    };

    template <typename, typename, typename> struct transform_completions;
    template <typename E, typename Fun, typename... C>
    struct transform_completions<E, Fun, ::nstd::execution::completion_signatures<C...>> {
        using type = typename join_completions<typename transform_completion<E, Fun, C>::type...>::type;
    };

    template <typename Tag, ::nstd::execution::sender Sender, typename Fun>
    struct sender {
        template <typename E>
        friend auto tag_invoke(::nstd::execution::get_completion_signatures_t, sender const&, E const& e) noexcept
            -> typename ::nstd::hidden_names::let::join_completions<
                   typename transform_completions<E, Fun, ::nstd::hidden_names::filter_completions_t<Tag,
                       decltype(::nstd::execution::get_completion_signatures(::nstd::type_traits::declval<Sender>(), e))
                    >>::type,
                   typename rest<Tag, Sender, E>::type
                   >::type {
            return {};
        }

        ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
        ::nstd::type_traits::remove_cvref_t<Fun>    d_fun;

        template <::nstd::execution::receiver Receiver>
        friend auto tag_invoke(::nstd::execution::connect_t, sender&& self, Receiver&& receiver)
            -> ::nstd::hidden_names::let::state<Tag, Sender, Receiver, Fun> {
            return ::nstd::hidden_names::let::state<Tag, Sender, Receiver, Fun>(
                ::nstd::utility::move(self.d_sender),
                ::nstd::utility::move(self.d_fun),
                ::nstd::utility::forward<Receiver>(receiver)
            );
        }
        template <::nstd::execution::receiver Receiver>
        friend auto tag_invoke(::nstd::execution::connect_t, sender const& self, Receiver&& receiver)
            -> ::nstd::hidden_names::let::state<Tag, Sender, Fun, Receiver> {
            return ::nstd::hidden_names::let::state<Tag, Sender, Fun, Receiver>(
                self.d_sender,
                self.d_fun,
                ::nstd::utility::forward<Receiver>(receiver)
            );
        }
    };

    template <typename Tag>
    struct cpo
        : ::nstd::execution::sender_adaptor_closure<cpo<Tag>>
    {
        using ::nstd::execution::sender_adaptor_closure<cpo>::operator();

        template <::nstd::execution::sender Sender, typename Fun>
            requires ::nstd::hidden_names::let::invocable<Tag, Fun>
                  && cpo_scheduler_tag_invocable<cpo<Tag>, Sender, Fun>
        auto operator()(Sender&& sender, Fun&& fun) const
        {
            auto scheduler = ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender);
            return tag_invoke(*this, scheduler, ::nstd::utility::forward<Sender>(sender), ::nstd::utility::forward<Fun>(fun));
        }

        template <::nstd::execution::sender Sender, typename Fun>
            requires ::nstd::hidden_names::let::invocable<Tag, Fun>
                  && (not cpo_scheduler_tag_invocable<cpo<Tag>, Sender, Fun>)
                  && cpo_tag_invocable<cpo<Tag>, Sender, Fun>
        auto operator()(Sender&& sender, Fun&& fun) const
        {
            return tag_invoke(*this, ::nstd::utility::forward<Sender>(sender), ::nstd::utility::forward<Fun>(fun));
        }

        template <::nstd::execution::sender Sender, typename Fun>
            requires ::nstd::hidden_names::let::invocable<Tag, Fun>
                  && (not cpo_scheduler_tag_invocable<cpo<Tag>, Sender, Fun>)
                  && (not cpo_tag_invocable<cpo<Tag>, Sender, Fun>)
        auto operator()(Sender&& sender, Fun&& fun) const -> ::nstd::hidden_names::let::sender<Tag, Sender, Fun>
        {
            return ::nstd::hidden_names::let::sender<Tag, Sender, Fun>{
                ::nstd::utility::forward<Sender>(sender),
                ::nstd::utility::forward<Fun>(fun)
                };
        }
        template <typename Function>
        auto operator()(Function&& fun) const {
            return [this, fun = ::nstd::utility::forward<Function>(fun)](::nstd::execution::sender auto&& sender) mutable{
                return (*this)(::nstd::utility::forward<decltype(sender)>(sender),
                               ::nstd::utility::move(fun));
            };
        }
    };
}

// ----------------------------------------------------------------------------

namespace nstd::execution::inline customization_points {
    using let_error_t   = ::nstd::hidden_names::let::cpo<::nstd::execution::set_error_t>;
    using let_stopped_t = ::nstd::hidden_names::let::cpo<::nstd::execution::set_stopped_t>;
    using let_value_t   = ::nstd::hidden_names::let::cpo<::nstd::execution::set_value_t>;

    inline constexpr let_error_t   let_error{};
    inline constexpr let_stopped_t let_stopped{};
    inline constexpr let_value_t   let_value{};
}

// ----------------------------------------------------------------------------

#endif
