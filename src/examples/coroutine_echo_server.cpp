// src/examples/coroutine_echo_server.cpp                             -*-C++-*-
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
//-dk:TODO this implementation is currently work in progress - just
//    experimenting with stuff for now.

#ifdef __clang__
int main() {}
#else

#include "nstd/execution.hpp"
#include "nstd/net.hpp"
#include <coroutine>
#include <iostream>
#include <optional>
#include <thread>
#include <new>
#include <cstdlib>

namespace EX = ::nstd::execution;

// ----------------------------------------------------------------------------

void* operator new(::std::size_t size) {
    ::std::cout << "operator new(" << size << ")\n";
    return ::std::malloc(size);
}
void* operator new[](::std::size_t size) {
    ::std::cout << "operator new[](" << size << ")\n";
    return ::std::malloc(size);
}
void  operator delete(void* ptr, ::std::size_t) { return ::std::free(ptr); }
void  operator delete[](void* ptr, ::std::size_t) { return ::std::free(ptr); }
void  operator delete(void* ptr) { return ::std::free(ptr); }
void  operator delete[](void* ptr) { return ::std::free(ptr); }

// ----------------------------------------------------------------------------

namespace
{
    struct task
    {
        struct promise_type;
        using handle_type = ::std::coroutine_handle<promise_type>;

        struct awaiter_base {
            ::std::coroutine_handle<void> d_handle;
            ::std::optional<int>          d_result;
            awaiter_base(handle_type h): d_handle(h) {}
        };
        struct receiver {
            awaiter_base* d_awaiter;
            friend auto tag_invoke(EX::set_value_t, receiver&& self, int v) noexcept -> void {
                ::std::cout << "set_value\n";
                self.d_awaiter->d_result = v;
                self.d_awaiter->d_handle.resume();
            }
            friend auto tag_invoke(EX::set_error_t, receiver&&, auto&&) noexcept -> void {
                ::std::cout << "set_error\n";
            }
            friend auto tag_invoke(EX::set_stopped_t, receiver&&) noexcept -> void {
                ::std::cout << "set_stopped\n";
            }
        };
        template <::nstd::execution::sender Sender>
        struct awaiter
            : awaiter_base
        {
            decltype(EX::connect(::std::declval<Sender>(), ::std::declval<receiver>())) d_state;
            ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
            template <typename S> awaiter(promise_type& p, S&& s)
                : awaiter_base(::std::coroutine_handle<promise_type>::from_promise(p))
                , d_state(EX::connect(::std::forward<S>(s), receiver{this})) {
                std::cout << "awaiter::awaiter(S)\n";
            }
            awaiter(awaiter&&) = delete;
            ~awaiter() { std::cout << "awaiter::~awaiter(S)\n"; }

            constexpr auto await_ready() -> bool {
                std::cout << "await_ready\n";
                return false;
            }
            auto await_resume() -> int { ::std::cout << "await_resume()\n"; return *this->d_result; }
            auto await_suspend(handle_type) -> bool {
                ::std::cout << "await_suspend()\n";
                EX::start(this->d_state);
                std::cout << "await_suspend(S): sender started (ready=" << ::std::boolalpha << static_cast<bool>(this->d_result) << ")\n";
                return true;
            }
        };
        struct promise_type
        {
            ~promise_type() { ::std::cout << "promise_type::~promise_type()\n"; }
            auto get_return_object() -> task { return { ::std::coroutine_handle<promise_type>::from_promise(*this) }; }
            auto initial_suspend() -> auto { return std::suspend_always(); }
            auto final_suspend() noexcept -> auto { return std::suspend_never(); }
            auto return_void() -> void {}
            auto unhandled_exception() -> void {}

            template <::nstd::execution::sender Sender>
            auto await_transform(Sender&& s) -> awaiter<Sender> {
                return awaiter<Sender>(*this, ::nstd::utility::forward<Sender>(s));
            }
        };

        handle_type d_handle;
        ~task() { ::std::cout << "task::~task()\n"; }
        auto run() -> void {
            std::cout << "task::run()\n";
            this->d_handle.resume();
            std::cout << "task::run() done\n";
        }
    };
}

// ----------------------------------------------------------------------------

task run_client()
{
    std::cout << "run_client start\n";
    int value = co_await ::nstd::execution::just(17);
    std::cout << "run_client end: " << value << "\n";
    co_return;
}

int main()
{
    std::cout << "before run_client()\n";
    auto task = run_client();
    std::cout << "after run_client()\n";

    task.run();
    std::cout << "1st task.run() done\n";
    //task.run();
    //std::cout << "2nd task.run() done\n";
}

#endif