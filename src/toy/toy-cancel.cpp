// toy-cancel.cpp                                                     -*-C++-*-
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

#include "toy-sender.hpp"
#include "toy-stop_token.hpp"
#include "toy-networking.hpp"
#include "toy-task.hpp"
#include <iostream>

// ----------------------------------------------------------------------------

namespace hidden_never_sender {
struct never_sender {
    using result_t = int;

    template <typename R>
    struct state {
        struct fun {
            state& s;
            void operator()() {
                std::cout << "never_sender stopping\n";
                set_stopped(s.receiver);
            }
        };
        using stop_token = decltype(get_stop_token(std::declval<R>()));
        using callback = typename stop_token::template callback_type<fun>;

        R                       receiver;
        std::optional<callback> cb;
        friend void start(state& self) {
            self.cb.emplace(get_stop_token(self.receiver), fun{self});
        }
    };

    template <typename R>
    friend state<R> connect(never_sender, R receiver) {
        return state<R>{ receiver, {} };
    }
};
}
using never_sender = hidden_never_sender::never_sender;

int main() {
    toy::io_context context;
    toy::socket     server{ ::socket(PF_INET, SOCK_STREAM, 0) };
    ::sockaddr_in addr{ .sin_family = AF_INET,
        .sin_port = htons(12345),
        .sin_addr = { .s_addr = INADDR_ANY }
        };
    ::bind(server.fd, (::sockaddr*)&addr, sizeof(addr));
    ::listen(server.fd, 1);

    context.spawn([](auto& server)->toy::task<toy::io_context::scheduler> {
        using namespace std::chrono_literals;
        for (int i = 0; i != 3; ++i) {
            std::cout << "awaiting accept\n" << std::flush;
            std::optional<toy::socket> o = co_await toy::timeout(toy::async_accept(server), 5s);
            if (o) {
                std::cout << "accepted client\n" << std::flush;
            }
            else {
                std::cout << "accept timed out\n" << std::flush;
            }
        }
    }(server));

    context.run();
}
