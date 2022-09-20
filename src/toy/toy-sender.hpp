// toy-sender.hpp                                                     -*-C++-*-
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

#ifndef INCLUDED_TOY_SENDER
#define INCLUDED_TOY_SENDER

#if !defined(TOY_NETWORKING_HPP)
#   define TOY_NETWORKING_HPP "toy-networking-poll.hpp"
#endif

#include TOY_NETWORKING_HPP
#include "toy-stop_token.hpp"
#include "toy-utility.hpp"
#include <concepts>
#include <exception>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace toy {

// ----------------------------------------------------------------------------

namespace chain {
    struct sender_base {};

    auto operator| (auto&& sender, auto&& op) {
        return op(sender);
    }
}

// ----------------------------------------------------------------------------

using toy::chain::sender_base;

// ----------------------------------------------------------------------------

template <typename R>
struct sync_wait_receiver {
    struct inline_scheduler {}; //-dk:TODO

    R& result;
    friend inline_scheduler get_scheduler(sync_wait_receiver const&) { return {}; }
    friend void set_value(sync_wait_receiver const& self, auto&& value){ self.result.emplace(value); }
    friend void set_error(sync_wait_receiver const&, std::exception_ptr ex){ std::rethrow_exception(ex); }
    friend void set_stopped(sync_wait_receiver const&){}
};

template <typename S>
std::optional<sender_result_t<S>> sync_wait(S&& sender) {
    using type = std::optional<sender_result_t<S>>;
    type result;
    auto state = connect(std::forward<S>(sender), sync_wait_receiver<type>{result});
    start(state);
    return result;
}

// ----------------------------------------------------------------------------

namespace hidden_just {
    template <typename T>
    struct just_sender
        : toy::sender_base {
        using result_t = T;

        template <typename R>
        struct state {
            std::remove_cvref_t<R> receiver;
            T value;
            friend void start(state& self) { set_value(std::move(self.receiver), self.value); }
        };

        T value;
        just_sender(T value): value(value) {}
        template <typename R>
        friend auto connect(just_sender const& self, R&& receiver) {
            return state<R>{receiver, self.value};
        }
    };
}
template <typename T>
hidden_just::just_sender<T> just(T&& value) {
    return hidden_just::just_sender<T>(std::forward<T>(value));
}

// ----------------------------------------------------------------------------

namespace hidden_then {
    template <typename S, typename F>
    struct then_sender
        : toy::sender_base {
        using aux_t = decltype(std::declval<F>()(std::declval<toy::sender_result_t<S>>()));
        using result_t = std::conditional_t<std::same_as<aux_t, void>, toy::none, aux_t>;
        
        template <typename R>
        struct receiver {
            std::remove_cvref_t<R> final_receiver;
            F fun;
            friend decltype(get_scheduler(std::declval<std::remove_cvref_t<R> const&>())) get_scheduler(receiver const& self) {
                return get_scheduler(self.final_receiver);
            }
            template <typename V>
            friend void set_value(receiver&& self, V&& value) {
                if constexpr (std::same_as<void, decltype(self.fun(std::forward<V>(value)))>) {
                    self.fun(std::forward<V>(value));
                    set_value(std::move(self.final_receiver), toy::none{});
                }
                else {
                    set_value(std::move(self.final_receiver), self.fun(std::forward<V>(value)));
                }
            }
            friend void set_error(receiver&& self, auto&& ex) {
                set_error(std::move(self.final_receiver), ex);
            }
        };

        std::remove_cvref_t<S> sender;
        F                      fun;
        template <typename R>
        friend auto connect(then_sender const& self, R&& final_receiver) {
            return connect(self.sender, receiver<R>{ std::forward<R>(final_receiver), self.fun });
        }
    };
}

template <typename S, typename F>
auto then(S sender, F fun) {
    return hidden_then::then_sender<S, F>{{}, std::forward<S>(sender), fun};
}
template <typename F>
auto then(F fun) {
    return [fun](auto&& sender){
        return then(std::forward<decltype(sender)>(sender), fun);
    };
}

// ----------------------------------------------------------------------------

namespace hidden_let_value {
    template <typename S, typename F>
    struct let_value_sender
        : toy::sender_base {
        using upstream_result_t = toy::sender_result_t<S>;
        using inner_sender_t = decltype(std::declval<std::remove_cvref_t<F>>()(std::declval<upstream_result_t>()));
        using result_t = toy::sender_result_t<inner_sender_t>;

        template <typename R, typename IS>
        struct receiver {
            R&                                 rec;
            std::optional<toy::connector<IS>>& inner_state;
            std::remove_cvref_t<F>             fun;
            template <typename V>
            friend void set_value(receiver&& self, V&& value) {
                self.inner_state.emplace(self.fun(std::forward<V>(value)), std::move(self.rec));
                start(*self.inner_state);
            }
            friend void set_error(receiver&& self, std::exception_ptr ex) {
                set_error(std::move(self.rec), ex);
            }
        };

        template <typename R>
        struct state {
            using inner_state_t = decltype(connect(std::declval<inner_sender_t>(), std::declval<R>()));
            using outer_state_t = decltype(connect(std::declval<S>(), std::declval<receiver<R, inner_state_t>>()));

            std::remove_cvref_t<R>                       rec;
            toy::connector<outer_state_t>                outer_state;
            std::optional<toy::connector<inner_state_t>> inner_state;

            template <typename CR, typename CS, typename CF>
            state(CR&& rec, CS&& sender, CF&& fun)
                : rec(std::forward<CR>(rec))
                , outer_state(std::forward<CS>(sender), receiver<R, inner_state_t>{rec, inner_state, std::forward<CF>(fun)}) {
            }
            friend void start(state& self) {
                start(self.outer_state);
            }
        };

        std::remove_cvref_t<S> sender;
        std::remove_cvref_t<F> fun;

        template <typename R>
        friend auto connect(let_value_sender&& self, R&& receiver) {
            return state<R>{std::forward<R>(receiver), std::move(self.sender), std::move(self.fun)};
        }
        template <typename R>
        friend auto connect(let_value_sender const& self, R&& receiver) {
            return state<R>{std::forward<R>(receiver), self.sender, self.fun};
        }
    };
}
template <typename S, typename F>
auto let_value(S&& sender, F&& fun) {
    return hidden_let_value::let_value_sender<S, F>{{}, std::forward<S>(sender), std::forward<F>(fun)};
}
template <typename F>
auto let_value(F&& fun) {
    return [fun = std::forward<F>(fun)](auto&& sender){
        return let_value(std::forward<decltype(sender)>(sender), std::move(fun));
    };
}

// ----------------------------------------------------------------------------

namespace hidden_when_all {
    template <typename... S>
    struct when_all {
        using result_t = std::tuple<toy::sender_result_t<S>...>;

        struct state_base {
            std::tuple<std::optional<toy::sender_result_t<S>>...> result;
            std::exception_ptr     error;
            std::size_t            outstanding{sizeof...(S)};

            state_base()
            {
            }
            virtual void complete() = 0;
            void done() {
                if (0u == --outstanding) {
                    complete();
                }
            }
        };

        template <typename R>
        struct receiver {
            state_base&       result;
            std::optional<R>& value;
            template <typename T>
            friend void set_value(receiver&& self, T&& value) {
                self.value.emplace(std::forward<T>(value));
                self.result.done();
            }
            friend void set_error(receiver&& self, std::exception_ptr error) {
                if (!self.result.error)
                    self.result.error = error;
                self.result.done();
            }
        };

        template <typename R>
        struct state
            : state_base {
            template <typename X>
            using when_all_state_t = decltype(connect(std::declval<X>(),
                                            std::declval<receiver<sender_result_t<X>>>()));
            std::remove_cvref_t<R> final_receiver;
            std::tuple<toy::connector<when_all_state_t<S>>...> inner_state;

            template <std::size_t... I>
            state(R&& r, std::index_sequence<I...>, auto&&... s)
                : final_receiver(std::forward<R>(r))
                , inner_state(std::make_pair(std::move(s),
                                            receiver<sender_result_t<S>>{*this, std::get<I>(this->result)}) ...)
            {
            }
            void complete() override {
                if (this->error)
                    set_error(std::move(final_receiver), std::move(this->error));
                else
                    std::apply(
                        [this](auto&&... r){ set_value(std::move(this->final_receiver), result_t(std::move(*r)...)); },
                        this->result);
            }
            friend void start(state& self) {
                std::apply([](auto&... inner){ (start(inner), ...); }, self.inner_state);
                (void)self;
            }
        };

        std::tuple<std::remove_cvref_t<S>...> senders;
        when_all(S&&... s): senders(std::forward<S>(s)...) {}

        template <typename R>
        friend state<R> connect(when_all&& self, R&& receiver) {
            return std::apply(
                [&](auto&&... s){
                    return state<R>(std::forward<R>(receiver),
                                    std::make_index_sequence<sizeof...(s)>(),
                                    std::move(s)...);
                    }
                , self.senders);
        }
    };
}

template <typename... S>
hidden_when_all::when_all<S...> when_all(S&&... sender) {
    return hidden_when_all::when_all<S...>(std::forward<S>(sender)...);
}

// ----------------------------------------------------------------------------

namespace hidden_when_any {
    template <typename... S>
    struct when_any {
        using result_t = std::variant<typename std::decay_t<S>::result_t...>;

        std::tuple<S...> sender;
        when_any(S... s)
            : sender(s...)
        {}

        template <typename R>
        struct state
            : toy::immovable {
            struct inner_receiver {
                state* s;
                friend toy::in_place_stop_source::stop_token get_stop_token(inner_receiver const& self) {
                    return self.s->source.token();
                }
                friend decltype(get_scheduler(std::declval<R>())) get_scheduler(inner_receiver const& self) {
                    return get_scheduler(self.s->receiver);
                } 
                friend void set_value(inner_receiver self, auto v) {
                    if (!self.s->result && !self.s->error) {
                        self.s->result.emplace(std::move(v));
                        self.s->source.stop();
                    }
                    self.s->complete();
                }
                friend void set_error(inner_receiver self, std::exception_ptr e) {
                    if (!self.s->result && !self.s->error) {
                        self.s->error = e;
                        self.s->source.stop();
                    }
                    self.s->complete();
                }
                friend void set_stopped(inner_receiver self) {
                    self.s->source.stop();
                    self.s->complete();
                }
            };
            template <typename T>
            struct inner_state {
                decltype(connect(std::declval<T>(), std::declval<inner_receiver>())) s;
                inner_state(auto p)
                    : s(connect(p.first, p.second)) {
                }
                friend void start(inner_state& self) {
                    start(self.s);
                }
            };
            
            R                             receiver;
            std::optional<result_t>       result;
            std::exception_ptr            error;
            toy::in_place_stop_source     source;
            std::tuple<inner_state<S>...> is;
            std::size_t                   outstanding{sizeof...(S)};

            state(R receiver, S... s)
                : receiver(receiver)
                , is(std::make_pair(s, inner_receiver{this})...) {
            }

            friend void start(state& self) {
                std::apply([](auto&... s) { (start(s), ...); },
                        self.is);
            }

            void complete() {
                if (--outstanding == 0u) {
                    if (result)
                        set_value(std::move(receiver), std::move(*result));
                    else if (error)
                        set_error(std::move(receiver), error);
                    //else
                    //    set_stopped(std::move(receiver));
                }
            }
        };
        template <typename R>
        friend state<R> connect(when_any self, R receiver) {
            return std::apply([&receiver](auto... s) {
                return state<R>(receiver, s...);
            }, self.sender);
        }
    };
}

template <typename... S>
hidden_when_any::when_any<S...> when_any(S&&... sender) {
    return hidden_when_any::when_any<S...>(std::forward<S>(sender)...);
}

// ----------------------------------------------------------------------------

template <typename S, typename D>
auto timeout(S sender, D duration) {
    using result_t = std::optional<typename S::result_t>;
    struct visitor {
        result_t operator()(typename S::result_t r) const { return result_t(std::move(r)); }
        result_t operator()(toy::none) const { return result_t(); }
    };
    return toy::then(toy::when_any(sender, toy::async_sleep_for{duration}),
                     [](auto v) { return std::visit(visitor(), std::move(v)); }
                    );
}

// ----------------------------------------------------------------------------

namespace hidden_repeat_effect_until {
    template <typename S, typename P>
    struct repeat_effect_until {
        using result_t = toy::none;

        template <typename> struct state;

        template <typename R>
        struct receiver {
            state<R>& state_;

            void on_value();
            void on_error(std::exception_ptr);

            friend decltype(get_scheduler(std::declval<std::remove_cvref_t<R>>()))
                get_scheduler(receiver const& self) {
                    return self.state_.final_receiver;
            }
            friend void set_value(receiver&& self, toy::none) { self.on_value(); }
            friend void set_error(receiver&& self, std::exception_ptr error) { self.on_error(error); }
        };

        struct track {
            bool& b;
            track(bool& b): b(b) { b = true; }
            ~track() { b = false; }
        };
        template <typename R>
        struct state {
            using inner_state_t = decltype(connect(std::declval<S>(), std::declval<receiver<R>>()));

            std::remove_cvref_t<R>                       final_receiver;
            std::remove_cvref_t<S>                       sender;
            std::remove_cvref_t<P>                       predicate;
            std::optional<toy::connector<inner_state_t>> inner_state;
            bool                                         recursive{false};
            bool                                         ready{false};

            state(R&& r, auto&& s, auto&& p)
                : final_receiver(r)
                , sender(std::move(s))
                , predicate(std::move(p)) {
            }
            friend void start(state& self) { self.run(); }
            void run() {
                if (!predicate()) {
                    inner_state.emplace(sender, receiver<R>{*this});
                    if (recursive) {
                        ready = true;
                    }
                    else {
                        track is(recursive);
                        for (ready = true; ready; ) {
                            ready = false;
                            start(*inner_state);
                        }
                    }
                }
                else {
                    set_value(std::move(final_receiver), toy::none());
                }
            }
        };

        std::remove_cvref_t<S> sender;
        std::remove_cvref_t<P> predicate;

        repeat_effect_until(S&& s, P&& p)
            : sender(std::forward<S>(s))
            , predicate(std::forward<P>(p)) {
        }

        template <typename R>
        friend state<R> connect(repeat_effect_until&& self, R&& r) {
            return state<R>(std::forward<R>(r), std::move(self.sender), std::move(self.predicate));
        }
    };

    template <typename S, typename P>
        template <typename R>
    inline void repeat_effect_until<S, P>::template receiver<R>::on_value() {
        state_.run();
    }

    template <typename S, typename P>
        template <typename R>
    inline void repeat_effect_until<S, P>::template receiver<R>::on_error(std::exception_ptr error) {
        set_error(std::move(state_.final_receiver), error);
    }
}

template <typename S, typename P>
hidden_repeat_effect_until::repeat_effect_until<S, P> repeat_effect_until(S&& s, P&& p) {
    return hidden_repeat_effect_until::repeat_effect_until<S, P>{std::forward<S>(s), std::forward<P>(p)};
}

template <typename S>
auto repeat_effect(S&& sender) {
    return toy::repeat_effect_until(std::forward<S>(sender), []{ return false; });
}

// ----------------------------------------------------------------------------

}

#endif
