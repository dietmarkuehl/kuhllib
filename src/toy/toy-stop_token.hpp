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

#endif
