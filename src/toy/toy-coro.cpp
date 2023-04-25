// toy-coro.cpp                                                       -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2023 Dietmar Kuehl http://www.dietmar-kuehl.de
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
#include <iostream>
#include <coroutine>
#include <new>
#include <cassert>

// ----------------------------------------------------------------------------

namespace toy {
    template <std::size_t S>
    struct memory_handle {
        void* memory;
    };
    template <std::size_t S>
    auto memory(char (&array)[S]) { return memory_handle<S>{array}; }

    template <typename Scheduler>
    struct coro {
        template <typename S>
        struct awaiter {
            using type = sender_result_t<S>;
            struct receiver {
                awaiter* a;
                friend toy::never_stop_token get_stop_token(receiver) { return {}; }
                friend void set_value(receiver self, auto v) {
                    self.a->value.emplace(std::move(v));
                    self.a->handle.resume();
                }
                friend void set_error(receiver self, auto e) {
                    self.a->error = e;
                    self.a->handle.resume();
                } 
                friend void set_stopped(receiver&&) {
                    //-dk:TODO get rid of the handle?
                }
                friend Scheduler get_scheduler(receiver const& self) {
                    return self.a->sched;
                }
            };

            using state_t = decltype(connect(std::declval<S>(), std::declval<receiver>()));

            Scheduler                   sched{ nullptr };
            std::coroutine_handle<void> handle;
            state_t                     state;
            std::optional<type>         value;
            std::exception_ptr          error;

            awaiter(Scheduler sched, S s): sched(sched), state(connect(std::move(s), receiver{this})) {}
            bool await_ready() { return false; }
            void await_suspend(std::coroutine_handle<void> handle) {
                this->handle = handle;
                start(state);
            }
            type await_resume() {
                if (error) std::rethrow_exception(error);
                return std::move(*value);
            }
        };

        struct none {};
        using result_t = none;

        struct state_base: immovable {
            virtual void complete() = 0;
            Scheduler sched;
            state_base(Scheduler sched): sched(sched) {}
        };
        struct promise_type
            : immovable {
            state_base* state = nullptr;

            coro get_return_object() { return { std::coroutine_handle<promise_type>::from_promise(*this) }; }
            std::suspend_always initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept {
                if (state) state->complete();
                return {};
            }
            void return_void() {}
            void unhandled_exception() { std::terminate() ; }
            template <typename S>
            awaiter<S> await_transform(S s) { return awaiter<S>(state->sched, s); }
            template <std::size_t Size>
            void* operator new(std::size_t size, memory_handle<Size> mem, auto&&...) {
                assert(size < Size);
                return mem.memory;
            }
        };

        template <typename R>
        struct state: state_base {
            std::coroutine_handle<void> handle;
            R                           receiver;

            state(auto&& handle, R receiver)
                : state_base(get_scheduler(receiver))
                , handle(handle)
                , receiver(receiver){
                handle.promise().state = this;
            }
            friend void start(state& self) {
                self.handle.resume();
            }
            void complete() override final {
                //static_assert(std::same_as<int, decltype(receiver)>);
                set_value(std::move(receiver), none{});
            }
        };

        std::coroutine_handle<promise_type> handle;

        template <typename R>
        friend state<R> connect(coro&& self, R receiver) {
            return state<R>(std::move(self.handle), receiver);
        }
    };
}

// ----------------------------------------------------------------------------

namespace toy::hidden_then_async {
    template <typename S, typename Coro>
    struct then_async_sender
        : toy::sender_base {
        using result_t = int; //-dk:TODO
        
        using coro_type = decltype(std::declval<Coro>()(std::declval<toy::memory_handle<1>>(),
                                                        std::declval<toy::sender_result_t<S>>()));
        template <typename FinalReceiver>
        struct state {
            struct receiver {
                state* s;
                friend void set_value(receiver&& self, auto&& value) {
                    std::cout << "state->set_value()\n";
                    self.s->coroutine.emplace(Coro()(toy::memory(self.s->memory), value), self.s->final_receiver);
                    start(*self.s->coroutine);
                }
                friend void set_error(receiver&& self, auto&& error) {
                    std::cout << "state->set_error()\n";
                    set_error(self.s->final_receiver, error);
                }
                friend auto get_stop_token(receiver const& self) {
                    return get_stop_token(self.s->final_receiver);
                }
            };
            using upstream_state = decltype(connect(std::declval<S>(), std::declval<receiver>()));
            using coroutine_state = decltype(connect(std::declval<coro_type>(), std::declval<FinalReceiver>()));
            FinalReceiver                                  final_receiver;
            upstream_state                                 upstream;
            std::optional<toy::connector<coroutine_state>> coroutine{};
            char                                           memory[128];

            state(auto sender, FinalReceiver fr)
                : final_receiver(fr)
                , upstream(connect(sender, receiver{this}))
            {
            }
            friend void start(state& self) {
                std::cout << "start\n";
                start(self.upstream);
            }
        };

        std::remove_cvref_t<S> sender;
        template <typename R>
        friend auto connect(then_async_sender const& self, R&& final_receiver) {
            return state<R>(self.sender, std::forward<R>(final_receiver));
        }
    };
}

namespace toy {
    template <typename S, typename Coro>
    auto then_async(S sender, Coro) {
        return hidden_then_async::then_async_sender<S, Coro>{{}, std::forward<S>(sender)};
    }
    template <typename Coro>
    auto then_async(Coro fun) {
        return [fun](auto&& sender){
            return then_async(std::forward<decltype(sender)>(sender), fun);
        };
    }
}

// ----------------------------------------------------------------------------

int main() {
    toy::io_context context;

    auto sender = toy::just(17)
        | toy::then_async([](auto, int v)->toy::coro<toy::io_context::scheduler>{
            std::cout << "coro(" << co_await toy::just(std::move(v)) << ")\n";
        })
        | toy::then([](auto&&...){ std::cout << "after coro\n"; })
        ;
    context.spawn(sender);
    context.run();
}