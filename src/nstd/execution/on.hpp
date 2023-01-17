// nstd/execution/on.hpp                                              -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_NSTD_EXECUTION_ON
#define INCLUDED_NSTD_EXECUTION_ON

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_completion_signatures.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/get_scheduler.hpp"
#include "nstd/execution/get_stop_token.hpp"
#include "nstd/execution/make_completion_signatures.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/copy_cvref.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"

#include <exception>
#include <optional>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    namespace hidden_names::on {
        template <::nstd::execution::sender Sender, ::nstd::execution::receiver Receiver>
        struct connector {
            using state_t = decltype(
                ::nstd::execution::connect(
                    ::nstd::type_traits::declval<Sender>(),
                    ::nstd::type_traits::declval<Receiver>()
                    )
                );

            state_t state;

            template <::nstd::execution::sender S, ::nstd::execution::receiver R>
            connector(S&& sender, R&& receiver)
                : state(::nstd::execution::connect(::nstd::utility::forward<S>(sender), ::nstd::utility::forward<R>(receiver)))
            {
            }
            auto start() noexcept -> void { ::nstd::execution::start(this->state); }
        };

        template <::nstd::execution::receiver Receiver>
        struct inner_receiver_cloak {
            struct inner_receiver {
                ::nstd::type_traits::remove_cvref_t<Receiver>& d_receiver;

                friend auto tag_invoke(::nstd::execution::get_env_t, inner_receiver const&) noexcept {
                    return 0;
                }
            };
        };
        template <::nstd::execution::receiver Receiver>
        using inner_receiver = typename inner_receiver_cloak<Receiver>::inner_receiver;

        template <::nstd::execution::scheduler Scheduler, typename Env>
        struct replace_env_cloak {
            struct replace_env {
                nstd::type_traits::remove_cvref_t<Scheduler>* scheduler;
                Env        env;

#if 0
                template <typename Tag, typename... Args>
                    requires (not ::std::same_as<::nstd::execution::get_scheduler_t, Tag>
                              && not ::std::same_as<::nstd::execution::set_value_t, Tag>
                              && not ::std::same_as<::nstd::execution::set_error_t, Tag>
                              && not ::std::same_as<::nstd::execution::set_stopped_t, Tag>
                              )
                friend auto tag_invoke(Tag tag, replace_env const& self, Args&&... args)
                    noexcept(noexcept(tag(self.env, ::nstd::utility::forward<Args>(args)...)))
                {
                    return tag(self.env, ::nstd::utility::forward<Args>(args)...);
                }
#endif
                friend auto tag_invoke(::nstd::execution::get_scheduler_t, replace_env const& self) noexcept {
                    return *self.scheduler;
                }
                friend auto tag_invoke(::nstd::execution::get_env_t, replace_env const& self) noexcept {
                    return self;
                }
            };
        };
        template <::nstd::execution::scheduler Scheduler, typename Env>
        using replace_env = typename replace_env_cloak<Scheduler, Env>::replace_env;

        template <::nstd::execution::scheduler Scheduler, ::nstd::execution::receiver Receiver>
        struct replace_receiver_cloak {
            struct replace_receiver {
                using env_t = decltype(::nstd::execution::get_env(::nstd::type_traits::declval<Receiver>()));
                Scheduler* scheduler;
                Receiver*  receiver;

                friend auto tag_invoke(::nstd::execution::get_env_t, replace_receiver const& self) noexcept {
                    return ::nstd::execution::hidden_names::on::replace_env<Scheduler, env_t>{
                        self.scheduler,
                        ::nstd::execution::get_env(*self.receiver)
                        };
                }
#if 0
                template <typename Tag, typename... Args>
                    requires (not ::std::same_as<::nstd::execution::get_env_t, Tag>)
                friend auto tag_invoke(Tag tag, replace_receiver&& self, Args&&... args)
                    noexcept(noexcept(tag(::nstd::utility::move(*self.receiver), ::nstd::utility::forward<Args>(args)...)))
                {
                    static_assert(::std::same_as<int, Tag>);
                    return tag(::nstd::utility::move(*self.receiver), ::nstd::utility::forward<Args>(args)...);
                }
#else
                friend auto tag_invoke(::nstd::execution::get_stop_token_t tag, replace_receiver const& self)
                    noexcept(noexcept(tag(::nstd::utility::move(*self.receiver))))
                {
                    return tag(::nstd::utility::move(*self.receiver));
                }
                template <typename... Args>
                friend auto tag_invoke(::nstd::execution::set_value_t tag, replace_receiver&& self, Args&&... args) noexcept -> void {
                    tag(::nstd::utility::move(*self.receiver), ::nstd::utility::forward<Args>(args)...);
                }
                template <typename Error>
                friend auto tag_invoke(::nstd::execution::set_error_t tag, replace_receiver&& self, Error&& error) noexcept -> void {
                    tag(::nstd::utility::move(*self.receiver), ::nstd::utility::forward<Error>(error));
                }
                friend auto tag_invoke(::nstd::execution::set_stopped_t tag, replace_receiver&& self) noexcept -> void {
                    tag(::nstd::utility::move(*self.receiver));
                }
#endif
            };
        };
        template <::nstd::execution::scheduler Scheduler, ::nstd::execution::receiver Receiver>
        using replace_receiver = typename replace_receiver_cloak<Scheduler, Receiver>::replace_receiver;

        template <::nstd::execution::scheduler Scheduler, ::nstd::execution::sender Sender, ::nstd::execution::receiver Receiver>
        struct state_base_cloak {
            struct state_base {
                using scheduler_t = ::nstd::type_traits::remove_cvref_t<Scheduler>;
                using sender_t    = ::nstd::type_traits::remove_cvref_t<Sender>;
                using receiver_t  = ::nstd::type_traits::remove_cvref_t<Receiver>;
                using connector_t = ::nstd::execution::hidden_names::on::connector<sender_t, replace_receiver<scheduler_t, receiver_t>>;

                scheduler_t                  scheduler;
                sender_t                     sender;
                receiver_t                   receiver;
                ::std::optional<connector_t> state;

                auto start() {
                    try {
                        this->state.emplace(::nstd::utility::move(this->sender),
                                            ::nstd::execution::hidden_names::on::replace_receiver<scheduler_t, receiver_t>{&this->scheduler, &this->receiver});
                        this->state->start();
                    }
                    catch (...) {
                        ::nstd::execution::set_error(::nstd::utility::move(this->receiver), ::std::current_exception());
                    }
                }
            };
        };
        template <::nstd::execution::scheduler Scheduler, ::nstd::execution::sender Sender, ::nstd::execution::receiver Receiver>
        using state_base = typename state_base_cloak<Scheduler, Sender, Receiver>::state_base;

        template <typename State>
        struct schedule_receiver_cloak {
            struct schedule_receiver {
                State* state;

                friend auto tag_invoke(::nstd::execution::set_value_t, schedule_receiver&& self, auto&&...) noexcept {
                    self.state->start();
                }
                friend auto tag_invoke(::nstd::execution::get_env_t, schedule_receiver const& self) noexcept {
                    return ::nstd::execution::get_env(self.state->receiver);
                }
            };
        };
        template <typename State>
        using schedule_receiver = typename schedule_receiver_cloak<State>::schedule_receiver;

        template <::nstd::execution::scheduler Scheduler,
                  ::nstd::execution::sender Sender,
                  ::nstd::execution::receiver Receiver>
        struct state_cloak {
            struct state
                : ::nstd::execution::hidden_names::on::state_base<Scheduler, Sender, Receiver> {
                using base =  ::nstd::execution::hidden_names::on::state_base<Scheduler, Sender, Receiver>;
                using scheduler_sender = decltype(::nstd::execution::schedule(::nstd::type_traits::declval<Scheduler>()));
                using schedule_receiver = ::nstd::execution::hidden_names::on::schedule_receiver<base>;

                ::nstd::execution::hidden_names::on::connector<scheduler_sender, schedule_receiver> scheduler_state;
                template <::nstd::execution::scheduler Sch,
                          ::nstd::execution::sender    S,
                          ::nstd::execution::receiver  R
                          >
                state(Sch&& scheduler, S&& sender, R&& receiver)
                    : base{ scheduler, ::nstd::utility::forward<S>(sender), ::nstd::utility::forward<R>(receiver), {} }
                    , scheduler_state(::nstd::execution::schedule(::nstd::utility::forward<Sch>(scheduler)),
                                      schedule_receiver{this})
                {
                }
                state(state&&) = delete;

                friend auto tag_invoke(::nstd::execution::start_t, state& self) noexcept {
                    self.scheduler_state.start();
                }
            };
        };
        template <::nstd::execution::scheduler Scheduler,
                  ::nstd::execution::sender Sender,
                  ::nstd::execution::receiver Receiver>
        using state = typename state_cloak<Scheduler, Sender, Receiver>::state;

        template <typename...>
        using no_value_completions = ::nstd::execution::completion_signatures<>;

        template <::nstd::execution::scheduler Scheduler, ::nstd::execution::sender Sender>
        struct sender_cloak {
            struct sender
                : ::nstd::execution::sender_tag
            {
                ::nstd::type_traits::remove_cvref_t<Scheduler> d_scheduler;
                ::nstd::type_traits::remove_cvref_t<Sender>    d_sender;

                template <::nstd::execution::receiver Receiver>
                friend auto tag_invoke(::nstd::execution::connect_t, sender&& self, Receiver&& receiver) noexcept {
                    return ::nstd::execution::hidden_names::on::state<Scheduler, Sender, Receiver>(
                            ::nstd::utility::move(self.d_scheduler),
                            ::nstd::utility::move(self.d_sender),
                            ::nstd::utility::forward<Receiver>(receiver)
                    );
                }
                template <::nstd::execution::receiver Receiver>
                friend auto tag_invoke(::nstd::execution::connect_t, sender const& self, Receiver&& receiver) noexcept {
                    return ::nstd::execution::hidden_names::on::state<Scheduler, Sender, Receiver>(
                            self.d_scheduler,
                            self.d_sender,
                            ::nstd::utility::forward<Receiver>(receiver)
                    );
                }

                template <typename Self, typename Env>
                    requires ::std::same_as<sender, ::nstd::type_traits::remove_cvref_t<Self>>
                friend auto tag_invoke(::nstd::execution::get_completion_signatures_t, Self&&, Env&&) noexcept {
                    //return ::nstd::execution::completion_signatures<::nstd::execution::set_error_t(::std::exception_ptr)>{};
                    return ::nstd::execution::make_completion_signatures<
                            ::nstd::type_traits::copy_cvref_t<Self, ::nstd::type_traits::remove_cvref_t<Sender>>,
                            ::nstd::execution::hidden_names::on::replace_env<Scheduler, Env>,
                            ::nstd::execution::make_completion_signatures<
                                decltype(::nstd::execution::schedule(::nstd::type_traits::declval<Scheduler>())),// ::nstd::execution::schedule_result_t<Scheduler>,
                                Env,
                                ::nstd::execution::completion_signatures<::nstd::execution::set_error_t(::std::exception_ptr)>,
                                ::nstd::execution::hidden_names::on::no_value_completions
                            >
                        >{};
                }
            };
        };
        template <::nstd::execution::scheduler Scheduler, ::nstd::execution::sender Sender>
        using sender = typename sender_cloak<Scheduler, Sender>::sender;

        struct cpo {
            template <::nstd::execution::scheduler Scheduler,
                       ::nstd::execution::sender Sender>
                requires ::nstd::tag_invocable<cpo, Scheduler, Sender>
                      && requires(cpo const& c, Scheduler&& scheduler, Sender&& sender) {
                        {
                            tag_invoke(c,
                                       ::nstd::utility::forward<Scheduler>(scheduler),
                                       ::nstd::utility::forward<Sender>(sender))
                        } -> nstd::execution::sender;
                      }
            auto operator()(Scheduler&& scheduler, Sender&& sender) const {
                return tag_invoke(*this,
                                  ::nstd::utility::forward<Scheduler>(scheduler),
                                  ::nstd::utility::forward<Sender>(sender));
            }
            template <::nstd::execution::scheduler Scheduler,
                       ::nstd::execution::sender Sender>
                requires (not ::nstd::tag_invocable<cpo, Scheduler, Sender>)
            auto operator()(Scheduler&& scheduler, Sender&& sender) const {
                return ::nstd::execution::hidden_names::on::sender<Scheduler, Sender>{
                    {},
                    ::nstd::utility::forward<Scheduler>(scheduler),
                    ::nstd::utility::forward<Sender>(sender)
                    };
            }
        };
    }

    using on_t = ::nstd::execution::hidden_names::on::cpo;
    inline constexpr on_t on{};
}

// ----------------------------------------------------------------------------

#endif
