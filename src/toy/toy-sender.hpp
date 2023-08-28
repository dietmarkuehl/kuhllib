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

#include "toy-networking.hpp"
#include "toy-stop_token.hpp"
#include "toy-utility.hpp"
#include <concepts>
#include <exception>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <iostream> //-dk:TODO remove

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

struct inline_scheduler {
    struct sender {
        using result_t = toy::none;
        template <typename Receiver>
        struct state {
            Receiver receiver;
            friend void start(state& self) {
                set_state(self.receiver, toy::none{});
            }
        };
        template <typename Receiver>
        friend state<Receiver> connect(sender, Receiver receiver) {
            return state<Receiver>(receiver);
        }
    };
    friend std::ostream& operator<< (std::ostream& out, inline_scheduler) {
        return out << "inline scheduler\n";
    }

    friend sender schedule(inline_scheduler) { return sender{}; }
};

template <typename R>
struct sync_wait_receiver {
    R& result;
    friend inline_scheduler get_scheduler(sync_wait_receiver const&) { return {}; }
    friend never_stop_token get_stop_token(sync_wait_receiver const&) { return {}; }
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
            friend decltype(get_stop_token(std::declval<std::remove_cvref_t<R> const&>())) get_stop_token(receiver const& self) {
                return get_stop_token(self.final_receiver);
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
            friend void set_stopped(receiver&& self) {
                set_stopped(std::move(self.final_receiver));
            }
        };

        std::remove_cvref_t<S> sender;
        F                      fun;
        template <typename R>
        friend auto connect(then_sender const& self, R&& final_receiver) {
            return connect(self.sender, receiver<R>{ std::forward<R>(final_receiver), self.fun });
        }
        template <typename R>
        friend auto connect(then_sender&& self, R&& final_receiver) {
            return connect(std::move(self.sender), receiver<R>{ std::forward<R>(final_receiver), self.fun });
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

        template <typename R> struct state;
        template <typename R, typename IS>
        struct receiver {
            state<R>&               d_state;
            std::remove_cvref_t<F>  d_fun;

            friend decltype(get_scheduler(std::declval<R&>())) get_scheduler(receiver const& self) {
                return get_scheduler(self.d_state.rec);
            }
            friend decltype(get_stop_token(std::declval<R const&>())) get_stop_token(receiver const& self) {
                return get_stop_token(self.d_state.rec);
            }
            template <typename V>
            friend void set_value(receiver&& self, V&& value) {
                self.d_state.inner_state.emplace(self.d_fun(std::forward<V>(value)), std::move(self.d_state.rec));
                start(*self.d_state.inner_state);
            }
            friend void set_error(receiver&& self, std::exception_ptr ex) {
                set_error(std::move(self.d_state.rec), ex);
            }
            friend void set_stopped(receiver&& self) {
                set_stopped(std::move(self.d_state.rec));
            }
        };

        template <typename R>
        struct state
            : toy::immovable {
            using inner_state_t = decltype(connect(std::declval<inner_sender_t>(), std::declval<R>()));
            using outer_state_t = decltype(connect(std::declval<S>(), std::declval<receiver<R, inner_state_t>>()));

            decltype(get_scheduler(std::declval<std::remove_cvref_t<R>>())) scheduler;
            std::remove_cvref_t<R>                       rec;
            toy::connector<outer_state_t>                outer_state;
            std::optional<toy::connector<inner_state_t>> inner_state;

            template <typename CR, typename CS, typename CF>
            state(CR&& rec, CS&& sender, CF&& fun)
                : scheduler(get_scheduler(rec))
                , rec(std::forward<CR>(rec))
                , outer_state(std::forward<CS>(sender), receiver<R, inner_state_t>{*this, std::forward<CF>(fun)}) {
            }
            friend void start(state& self) {
                start(self.outer_state);
            }
        };

        std::remove_cvref_t<S> sender;
        std::remove_cvref_t<F> fun;

        template <typename R>
        friend auto connect(let_value_sender&& self, R&& receiver) {
            return state<std::remove_cvref_t<R>>{std::forward<R>(receiver), std::move(self.sender), std::move(self.fun)};
        }
        template <typename R>
        friend auto connect(let_value_sender const& self, R&& receiver) {
            return state<std::remove_cvref_t<R>>{std::forward<R>(receiver), self.sender, self.fun};
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

        template <typename FR>
        struct state_base {
            std::remove_cvref_t<FR> final_receiver;
            std::tuple<std::optional<toy::sender_result_t<S>>...> result;
            std::exception_ptr     error;
            std::size_t            outstanding{sizeof...(S)};
            toy::in_place_stop_source stop_source;

            auto get_scheduler_() { return get_scheduler(this->final_receiver); }
            auto get_stop_token_() { return this->stop_source.token(); }

            template <typename R>
            state_base(R&& final_receiver)
                : final_receiver(std::forward<R>(final_receiver))
            {
            //auto get_stop_token_() { return get_stop_token(this->final_receiver); }
            }
            virtual void complete() = 0;
            void done() {
                if (0u == --outstanding) {
                    complete();
                }
            }
        };

        template <typename FR, typename R>
        struct receiver {
            state_base<FR>&   result;
            std::optional<R>& value;

            friend decltype(std::declval<state_base<FR>&>().get_scheduler_()) get_scheduler(receiver const& self) {
                return self.result.get_scheduler_();
            }
            friend decltype(std::declval<state_base<FR>&>().get_stop_token_()) get_stop_token(receiver const& self) {
                return self.result.get_stop_token_();
            }
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
            friend void set_stopped(receiver&& self) {
                self.result.stop_source.stop();
                self.result.done();
            }
        };

        template <typename R>
        struct state
            : state_base<R> {
            template <typename X>
            using when_all_state_t = decltype(connect(std::declval<X>(),
                                            std::declval<receiver<R, sender_result_t<X>>>()));
            std::tuple<toy::connector<when_all_state_t<S>>...> inner_state;

            template <std::size_t... I>
            state(R&& r, std::index_sequence<I...>, auto&&... s)
                : state_base<R>(std::forward<R>(r))
                , inner_state(std::make_pair(std::move(s),
                                            receiver<R, sender_result_t<S>>{*this, std::get<I>(this->result)}) ...)
            {
            }
            void complete() override {
                if (this->error)
                    set_error(std::move(this->final_receiver), std::move(this->error));
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

        std::tuple<std::decay_t<S>...> sender;
        template <typename... A>
        when_any(A&&... s)
            : sender(std::forward<A>(s)...)
        {}

        template <typename R>
        struct state
            : toy::immovable {
            template <std::size_t I>
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
                        self.s->result.emplace(std::in_place_index<I>, std::move(v));
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
            template <std::size_t I, typename T>
            struct inner_state {
                decltype(connect(std::declval<T>(), std::declval<inner_receiver<I>>())) s;
                inner_state(auto p)
                    : s(connect(std::move(p.first), inner_receiver<I>(p.second))) {
                }
                friend void start(inner_state& self) {
                    start(self.s);
                }
            };

            template <typename, typename...> struct state_helper;
            template <std::size_t... I, typename... X>
            struct state_helper<std::index_sequence<I...>, X...>
            {
                using type = std::tuple<inner_state<I, S>...>;
            };

            template <typename... X>
            using tuple_of_states = typename state_helper<std::make_index_sequence<sizeof...(X)>, X...>::type;
            
            R                             receiver;
            std::optional<result_t>       result;
            std::exception_ptr            error;
            toy::in_place_stop_source     source;
            tuple_of_states<S...>         is;
            std::size_t                   outstanding{sizeof...(S)};

            template <typename... A>
            state(R receiver, A&&... s)
                : receiver(receiver)
                , is(std::make_pair(std::forward<A>(s), this)...) {
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
                    else
                        set_stopped(std::move(receiver));
                }
            }
        };
        template <typename R>
        friend state<R> connect(when_any const& self, R receiver) {
            return std::apply([&receiver](auto... s) {
                return state<R>(receiver, s...);
            }, self.sender);
        }
        template <typename R>
        friend state<R> connect(when_any&& self, R receiver) {
            return std::apply([&receiver](auto... s) {
                return state<R>(receiver, std::move(s)...);
            }, std::move(self.sender));
        }
    };
}

template <typename... S>
hidden_when_any::when_any<S...> when_any(S&&... sender) {
    return hidden_when_any::when_any<S...>(std::forward<S>(sender)...);
}

// ----------------------------------------------------------------------------

template <typename S, typename D>
auto timeout(S&& sender, D duration) {
    using result_t = std::optional<typename S::result_t>;
    struct visitor {
        result_t operator()(typename S::result_t r) const { return result_t(std::move(r)); }
        result_t operator()(toy::none) const { return result_t(); }
    };
    return toy::then(toy::when_any(
        std::forward<S>(sender),
        toy::then(toy::async_sleep_for(duration), [](auto v){ return v; })
        ),
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
                    return get_scheduler(self.state_.final_receiver);
            }
            friend decltype(get_stop_token(std::declval<std::remove_cvref_t<R> const&>()))
                get_stop_token(receiver const& self) {
                    return get_stop_token(self.state_.final_receiver);
            }
            friend void set_value(receiver&& self, toy::none) { self.on_value(); }
            friend void set_error(receiver&& self, std::exception_ptr error) { self.on_error(error); }
            friend void set_stopped(receiver&& self) {
                set_stopped(std::move(self.state_.final_receiver));
            }
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

template <typename, typename...> struct sequence_sender;

template <std::size_t... I, typename... S>
struct sequence_sender<std::index_sequence<I...>, S...> {
    using result_t = sender_result_t<std::tuple_element_t<sizeof...(S)-1, std::tuple<S...>>>;

    std::tuple<S...> senders;

    template <typename R> struct state;

    template <std::size_t P, typename R>
    struct receiver {
        state<R>& s;

        friend decltype(get_scheduler(std::declval<R const&>())) get_scheduler(receiver const& self) {
            return get_scheduler(self.s.final_receiver);
        }
        friend decltype(get_stop_token(std::declval<R const&>())) get_stop_token(receiver const& self) {
            return get_stop_token(self.s.final_receiver);
        }
        template <typename V>
        friend void set_value(receiver&& self, V&& value) {
            if constexpr (P == sizeof...(S) - 1) {
                set_value(std::move(self.s.final_receiver), std::forward<V>(value));
            }
            else {
                state<R>& ref{self.s};
                ref.is.template emplace<P + 2>(std::move(std::get<P + 1>(ref.senders)), ref);
            }
        }
        friend void set_error(receiver&& self, auto error) {
            set_error(std::move(self.s.final_receiver), std::move(error));
        }
        friend void set_stopped(receiver&& self) {
            set_stopped(std::move(self.s.final_receiver));
        }
    };

    template <std::size_t P, typename IS, typename R>
    struct inner_state {
        decltype(connect(std::declval<IS>(), std::declval<receiver<P, R>>())) d_state;
        inner_state(IS&& sender, state<R>& outer)
            : d_state(connect(std::forward<IS>(sender), receiver<P, R>{outer})) {
                start(d_state);
            }
    };

    template <typename R>
    struct state {
        R final_receiver;
        std::tuple<S...>                      senders;
        std::variant<std::monostate, inner_state<I, S, R>...> is;
        friend void start(state& self) {
            self.is.template emplace<1>(std::move(std::get<0>(self.senders)), self);
        }
    };

    template <typename R>
    friend state<R> connect(sequence_sender&& self, R r) {
        return { r, std::move(self.senders), {} };
    }
};

template <typename... S>
sequence_sender<std::make_index_sequence<sizeof...(S)>, S...> sequence(S... s) {
    return { { s... } };
};

// ----------------------------------------------------------------------------

auto async_write(toy::socket& socket, char const* buffer, std::size_t n) {
    return let_value(toy::just(toy::none{}),
                     [&socket, it = buffer, end = buffer + n, n, error = false](auto) mutable {
                        (void)error;
                        (void)n;
        return sequence(
            repeat_effect_until(
                then(async_write_some(socket, it, end - it),
                    [&it, &error](std::size_t d){
                        if (d <= 0)
                            error = true;
                        else {
                            it += d;
                        }
                    }),
                [&it, end, &error]{ return it == end || error; }
            ),
            just(n - (end - it))
        );
    });
}

// ----------------------------------------------------------------------------

struct cancel {
    using result_t = toy::none;
    template <typename Receiver>
    struct state {
        Receiver receiver;
        friend void start(state& self) {
            set_stopped(std::move(self.receiver));
        }
    };
    template <typename Receiver>
    friend state<Receiver> connect(cancel const&, Receiver receiver) {
        return state<Receiver>{ receiver };
    }
};

// ----------------------------------------------------------------------------

}

#endif
