// toy-stop_token.hpp                                                 -*-C++-*-
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

#ifndef INCLUDED_TOY_STOP_TOKEN
#define INCLUDED_TOY_STOP_TOKEN

#include "toy-utility.hpp"
#include <iostream>
#include <utility>
#include <optional>
#include <tuple>
#include <variant>

// ----------------------------------------------------------------------------

namespace toy {
    struct never_stop_token {
        constexpr bool stop_requested() const { return false; }
        template <typename F>
        struct callback_type {
            callback_type(never_stop_token, F) {
            };
        };
    };

    struct in_place_stop_source
        : toy::immovable {
        struct base
            : toy::immovable {
            base* next{this};
            base* prev{this};
            ~base() {
                next->prev = prev;
                prev->next = next;
            }
            virtual void call() {}
        };

        base cbs;
        bool stopped{false};

        void add_callback(base* b) {
            if (!stopped) {
                cbs.next->prev = b;
                b->next = cbs.next;
                cbs.next = b;
                b->prev = &cbs;
            }
            else {
                b->call();
            }
        }

        struct stop_token;
        template <typename F>
        struct callback_type final
            : base {
            F fun;
            callback_type(stop_token token, F fun)
                : fun(fun) {
                token.source->add_callback(this);
            }
            void call() override { fun(); }
        };

        struct stop_token {
            template <typename F>
            using callback_type = toy::in_place_stop_source::callback_type<F>;
            in_place_stop_source* source{};
            bool stop_requested() const { return source->stopped; } 
        };


        stop_token token() { return { this }; }
        void       stop() {
            if (not std::exchange(stopped, true)) {
                for (auto it = cbs.next; it != &cbs; ) {
                    std::exchange(it, it->next)->call();
                }
            }
        }
    };
}

// ----------------------------------------------------------------------------

namespace toy {
    template <typename... S>
    struct when_any {
        struct none {};
        using result_t = std::variant<typename S::result_t...>;

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
                friend void set_value(inner_receiver self, auto v) {
                    if (!self.s->result && !self.s->error) {
                        self.s->result.emplace(v);
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
                        set_value(receiver, std::move(*result));
                    else if (error)
                        set_error(receiver, error);
                    //else
                    //    set_stopped(receiver);
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

// ----------------------------------------------------------------------------

#endif
