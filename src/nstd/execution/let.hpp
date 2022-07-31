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
#include "nstd/execution/sender.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/type_traits/remove_reference.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/type_identity.hpp"
#include <optional>
#include <tuple>
#include <variant>
#include <iostream>

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
    template <typename Tag, typename Fun>
    concept invocable
        =  (not ::std::same_as<Tag, ::nstd::execution::set_stopped_t>)
        || ::std::invocable<Fun>
        ;

    template <::nstd::execution::receiver, typename, typename> struct inner_state;
    template <::nstd::execution::receiver Receiver, typename Fun, typename Tag, typename... Args>
    struct inner_state<Receiver, Fun, Tag(Args...)> {
        using sender_t = decltype(::nstd::type_traits::declval<Fun>()(::nstd::type_traits::declval<::nstd::type_traits::remove_reference_t<Args>>()...));
        using state_t  = decltype(::nstd::execution::connect(::nstd::type_traits::declval<sender_t>(), ::nstd::type_traits::declval<Receiver>()));

        ::std::tuple<::nstd::type_traits::remove_reference_t<Args>...> d_args;
        state_t                                                        d_state;

        template <::nstd::execution::receiver R, typename F, typename... A>
        inner_state(R&& receiver, F&& fun, A&&... args)
            : d_args(::nstd::utility::forward<A>(args)...)
            , d_state(::nstd::execution::connect(::std::apply(::nstd::utility::forward<F>(fun), this->d_args),
                                                 ::nstd::utility::move(receiver)))
        {
            ::nstd::execution::start(this->d_state);
        }
    };

    template <::nstd::execution::receiver Receiver, typename Fun>
    struct state_base {
        ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
        ::nstd::type_traits::remove_cvref_t<Fun>      d_fun;
    };

    template <typename Tag, ::nstd::execution::receiver Receiver, typename Fun>
    struct receiver {
        ::nstd::hidden_names::let::state_base<Receiver, Fun>& d_state;

        template <typename... A>
        friend auto tag_invoke(Tag, receiver&& self, A&&... a) noexcept -> void {
            ::std::cout << "special tag_invoke\n";
            inner_state<Receiver, Fun, Tag(A...)> xstate(::nstd::utility::move(self.d_state.d_receiver),
                                                         ::nstd::utility::move(self.d_state.d_fun),
                                                         ::nstd::utility::forward<A>(a)...);
            (void)xstate;

            ::std::tuple<::nstd::type_traits::remove_reference_t<A>...> args(::nstd::utility::forward<A>(a)...);
            auto state = ::nstd::execution::connect(::std::apply(self.d_state.d_fun, args),
                                                    ::nstd::utility::move(self.d_state.d_receiver));
            ::nstd::execution::start(state);
        }
        template <typename T, typename... A>
        friend auto tag_invoke(T, receiver&& self, A&&... args) noexcept {
            ::std::cout << "forwarded r-value tag_invoke\n";
            return T()(::nstd::utility::move(self.d_state.d_receiver), ::nstd::utility::forward<A>(args)...);
        }
        template <typename T, typename... A>
        friend auto tag_invoke(T, receiver const& self, A&&... args) noexcept {
            ::std::cout << "forwarded const l-value tag_invoke\n";
            return T()(::nstd::utility::move(self.d_state.d_receiver), ::nstd::utility::forward<A>(args)...);
        }
    };

    template <typename                    Tag,
              ::nstd::execution::sender   Sender,
              ::nstd::execution::receiver Receiver,
              typename                    Fun>
    struct state
        : ::nstd::hidden_names::let::state_base<Receiver, Fun> {
        using receiver_t = ::nstd::hidden_names::let::receiver<Tag, Receiver, Fun>;
        static_assert(::nstd::execution::receiver<receiver_t>);
        static_assert(::nstd::execution::receiver_of<receiver_t, typename Sender::completion_signatures>);
        using state_t = decltype(::nstd::execution::connect(::nstd::type_traits::declval<Sender>(),
                                                            ::nstd::type_traits::declval<receiver_t>()));

        state_t d_state;

        template <typename F, ::nstd::execution::receiver R>
        state(Sender&& sender, F&& fun, R&& receiver)
            : ::nstd::hidden_names::let::state_base<Receiver, Fun> { ::nstd::utility::forward<R>(receiver), ::nstd::utility::forward<F>(fun) }
            , d_state(::nstd::execution::connect(::nstd::utility::forward<Sender>(sender),
                                                 receiver_t{ *this }))
        {
        }
        friend auto tag_invoke(::nstd::execution::start_t, state& self) noexcept -> void {
            ::std::cout << "state.start()\n";
            (void)self;
            ::nstd::execution::start(self.d_state);
        }
    };

    template <typename Tag, ::nstd::execution::sender Sender, typename Fun>
    struct sender {
        // using completion_signatures = ::nstd::execution::completion_signatures<>;
        using completion_signatures = typename Sender::completion_signatures;

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
            -> ::nstd::hidden_names::let::state<Tag, Sender const&, Fun, Receiver> {
            return ::nstd::hidden_names::let::state<Tag, Sender const&, Fun, Receiver>(
                self.d_sender,
                ::nstd::utility::move(self.d_fun),
                ::nstd::utility::forward<Receiver>(receiver)
            );
        }
    };

    template <typename Tag>
    struct cpo {
#if 0
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
#endif
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

#if 0
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
        friend auto tag_invoke(::nstd::execution::set_stopped_t, upstream_receiver r)
            noexcept -> void {
            ::nstd::execution::set_stopped(::nstd::utility::move(r.d_state->d_receiver));
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
        friend auto tag_invoke(::nstd::execution::set_stopped_t, downstream_receiver r)
            noexcept -> void {
            ::nstd::execution::set_stopped(::nstd::utility::move(r.d_state->d_receiver));
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

    using completion_signatures = ::nstd::execution::completion_signatures<
            //-dk:TODO define completion_signatures
        >;

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
#endif

// ----------------------------------------------------------------------------

#endif