// nstd/execution/task.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_TASK
#define INCLUDED_NSTD_EXECUTION_TASK

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/start.hpp"

#include <coroutine>
#include <iostream>
#include <memory>
#include <optional>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    template <typename> struct task_state_base;
    template <>         struct task_state_base<void>;
    template <typename, typename, ::nstd::execution::receiver>
    struct task_state;
    template <typename Promise, ::nstd::execution::receiver Receiver>
    struct task_state<void, Promise, Receiver>;

    template <typename> struct task_promise_base;
    template <>         struct task_promise_base<void>;
    template <typename = void> struct task;
}

// ----------------------------------------------------------------------------

template <typename Type>
struct nstd::execution::task_state_base {
    virtual void complete(Type const&) = 0;
    virtual void complete(Type&) = 0;
    virtual void complete(Type&&) = 0;
};

template <typename Type, typename Promise, ::nstd::execution::receiver Receiver>
struct nstd::execution::task_state
    : nstd::execution::task_state_base<Type>
{
    std::coroutine_handle<Promise> handle;
    std::remove_cvref_t<Receiver>  receiver;

    template <::nstd::execution::receiver R>
    task_state(std::coroutine_handle<Promise>&& handle, R&& r)
        : handle(std::move(handle))
        , receiver(std::forward<R>(r)) {
    }

    friend auto tag_invoke(::nstd::execution::start_t, task_state& self) noexcept {
        std::cout << "task<Type>::start\n";
        self.handle.promise().completion = &self;
        self.handle.resume();
    }
    void complete(Type const& value) override {
        std::cout << "task::complete(Type const&)\n";
        ::nstd::execution::set_value(std::move(this->receiver), value);
    }
    void complete(Type& value) override {
        std::cout << "task::complete(Type&)\n";
        ::nstd::execution::set_value(std::move(this->receiver), value);
    }
    void complete(Type&& value) override {
        std::cout << "task::complete(Type&&)\n";
        ::nstd::execution::set_value(std::move(this->receiver), std::move(value));
    }
};

// ----------------------------------------------------------------------------

template <>
struct nstd::execution::task_state_base<void> {
    virtual void complete() = 0;
};

template <typename Promise, ::nstd::execution::receiver Receiver>
struct nstd::execution::task_state<void, Promise, Receiver>
    : nstd::execution::task_state_base<void>
{
    std::coroutine_handle<Promise> handle;
    std::remove_cvref_t<Receiver>  receiver;

    template <::nstd::execution::receiver R>
    task_state(std::coroutine_handle<Promise>&& handle, R&& r)
        : handle(std::move(handle))
        , receiver(std::forward<R>(r)) {
    }

    friend auto tag_invoke(::nstd::execution::start_t, task_state& self) noexcept {
        std::cout << "task<void>::start\n";
        self.handle.promise().completion = &self;
        self.handle.resume();
    }
    void complete() override {
        std::cout << "task<void>::complete\n";
        ::nstd::execution::set_value(std::move(this->receiver));
    }
};

// ----------------------------------------------------------------------------

template <typename Type>
struct nstd::execution::task_promise_base {
    template <template <typename...> class T, template <typename...> class V>
    using value_types = V<T<Type>>;
    using completion_signatures = ::nstd::execution::completion_signatures<
        ::nstd::execution::set_value_t(Type),
        ::nstd::execution::set_error_t(),
        ::nstd::execution::set_stopped_t()
    >;

    task_state_base<Type>* completion{nullptr};
    template <typename T>
    void return_value(T&& value) {
        std::cout << "return_value: " << value<< "\n";
        this->completion->complete(std::forward<T>(value));
    }
};

template <>
struct nstd::execution::task_promise_base<void> {
    template <template <typename...> class T, template <typename...> class V>
    using value_types = V<T<>>;
    using completion_signatures = ::nstd::execution::completion_signatures<
        ::nstd::execution::set_value_t(),
        ::nstd::execution::set_error_t(),
        ::nstd::execution::set_stopped_t()
    >;

    task_state_base<void>* completion{nullptr};
    void return_void() {
        std::cout << "return_void\n";
        this->completion->complete();
    }
};

template <typename Type>
struct nstd::execution::task {
    using state_base = task_state_base<Type>;
    struct promise_type
        : task_promise_base<Type>
    {
        auto initial_suspend() { return std::suspend_always(); }
        auto final_suspend() noexcept { return std::suspend_always(); }
        auto get_return_object() {
            std::cout << "get_return_object()\n";
            return std::coroutine_handle<promise_type>::from_promise(*this);
        }
        void unhandled_exception() {
            std::cout << "unhandled_exception()\n";
            std::terminate();
        }

        template <::nstd::execution::sender Sender>
        struct awaitable {
            using result_type = typename Sender::template value_types<std::tuple, std::type_identity_t>;

            //awaitable(awaitable&&) = delete;
            //awaitable(awaitable const&) = delete;
            struct receiver {
                std::optional<result_type>*  result;
                std::coroutine_handle<void>* handle;

                template <typename... Args>
                friend void tag_invoke(::nstd::execution::set_value_t, receiver&& self, Args&&... args) noexcept {
                    *self.result = std::make_tuple(std::forward<Args>(args)...);
                    self.handle->resume();
                }
                template <typename Error>
                friend void tag_invoke(::nstd::execution::set_error_t, receiver&&, Error&& ) noexcept {
                    std::cout << "awaitable::receiver::set_error_t\n";
                }
                friend void tag_invoke(::nstd::execution::set_stopped_t, receiver&&) noexcept {
                    std::cout << "awaitable::receiver::set_stopped_t\n";
                }
            };

            struct state {
                using op_state = decltype(::nstd::execution::connect(std::declval<Sender>(), std::declval<receiver>()));

                std::coroutine_handle<void> handle;
                std::optional<result_type>  result;
                op_state                    d_op_state;
                state(std::coroutine_handle<void> handle,
                      Sender&&                    sender)
                    : handle(std::move(handle))
                    , d_op_state(::nstd::execution::connect(std::move(sender), receiver(&this->result, &this->handle)))
                {
                    std::cout << "state::state()\n";
                }
                ~state() {
                    std::cout << "state::~state()\n";
                }
                state(state&&) = delete;
                state(state const&) = delete;
            };

            std::remove_cvref_t<Sender> d_sender;
            std::shared_ptr<state>      d_state;

            template <::nstd::execution::sender S>
            awaitable(S&& s): d_sender(std::forward<S>(s)) {}

            bool await_ready() { std::cout << "awaitable::await_ready()\n"; return false; }
            void await_suspend(std::coroutine_handle<void> handle){
                std::cout << "awaitable::await_suspend\n";
                this->d_state = std::make_shared<state>(std::move(handle), std::move(this->d_sender));
                ::nstd::execution::start(this->d_state->d_op_state);
            }
            auto await_resume() {
                std::cout << "await_resume()\n";
                return *this->d_state->result;
            }
        };

        template <::nstd::execution::sender Sender>
        auto await_transform(Sender&& s) {
            std::cout << "await_transform\n";
            return awaitable<Sender>(std::forward<Sender>(s));
        }
    };


    template <template <typename...> class T, template <typename...> class V>
    using value_types = typename task_promise_base<Type>::template value_types<T, V>;
    template <template <typename...> class V>
    using error_types = V<int>;
    static constexpr bool sends_done = true;

    using completion_signatures = typename task_promise_base<Type>::completion_signatures;

    std::coroutine_handle<promise_type> handle;

    template <::nstd::execution::receiver Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, task&& self, Receiver&& receiver) {
        std::cout << "task::connect\n";
        return ::nstd::execution::task_state<Type, promise_type, Receiver>(
	    std::move(self.handle),
	    std::forward<Receiver>(receiver)
	    );
    }
};

// ----------------------------------------------------------------------------

#endif
