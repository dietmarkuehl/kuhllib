// toy-task.hpp                                                       -*-C++-*-
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

#ifndef INCLUDED_TOY_TASK
#define INCLUDED_TOY_TASK

#include "toy-stop_token.hpp"
#include "toy-utility.hpp"
#include "toy-coroutine.hpp"
#include <exception>
#include <optional>
#include <type_traits>
#include <utility>

namespace toy
{

// ----------------------------------------------------------------------------

namespace hidden_task {
    template <typename Scheduler>
    struct task
    {
        task() {}
        task(auto&& handle): handle(std::move(handle)) {}
        task(task&& other): handle(std::exchange(other.handle, std::coroutine_handle<promise_type>())) {}
        ~task() {
            if (handle) {
                handle.destroy();
            }
        }

        template <typename S>
        struct awaiter {
            using type = sender_result_t<S>;
            struct receiver {
                awaiter* a;
                friend toy::in_place_stop_source::stop_token get_stop_token(receiver self) {
                    return self.a->token();
                }
                friend void set_value(receiver self, auto v) {
                    self.a->value.emplace(std::move(v));
                    self.a->handle.resume();
                }
                friend void set_error(receiver self, auto e) {
                    self.a->error = e;
                    self.a->handle.resume();
                } 
                friend void set_stopped(receiver&& self) {
                    self.a->stop();
                    //-dk:TODO get rid of the handle?
                }
                friend Scheduler get_scheduler(receiver const& self) {
                    return self.a->sched;
                }
            };

            using state_t = decltype(connect(std::declval<S>(), std::declval<receiver>()));

            Scheduler                   sched{ nullptr };
            toy::coroutine_handle<void> handle;
            state_t                     state;
            std::optional<type>         value;
            std::exception_ptr          error;

            awaiter(Scheduler sched, S s): sched(sched), state(connect(std::move(s), receiver{this})) {}
            bool await_ready() { return false; }
            void await_suspend(toy::coroutine_handle<void> handle) {
                this->handle = handle;
                start(state);
            }
            type await_resume() {
                if (error) std::rethrow_exception(error);
                return std::move(*value);
            }
            void stop();
            toy::in_place_stop_source::stop_token token();
        };

        struct none {};
        using result_t = none;

        struct state_base: immovable {
            virtual void complete() = 0;
            virtual void stop() = 0;
            Scheduler sched;
            toy::in_place_stop_source stop_source;
            state_base(Scheduler sched): sched(sched) {}
        };
        struct promise_type
            : immovable {
            state_base* state = nullptr;

            task get_return_object() { return task(toy::coroutine_handle<promise_type>::from_promise(*this)); }
            toy::suspend_always initial_suspend() { return {}; }
            toy::suspend_always final_suspend() noexcept {
                if (state) state->complete();
                return {};
            }
            void return_void() {}
            void unhandled_exception() { std::terminate() ; }
            template <typename S>
            awaiter<S> await_transform(S s) { return awaiter<S>(state->sched, std::move(s)); }
        };

        template <typename R>
        struct state: state_base {
            struct callback {
                state_base* s;
                void operator()() const { this->s->stop_source.stop(); }
            };
            using stop_token = decltype(get_stop_token(std::declval<R>()));
            using token_callback = typename stop_token::template callback_type<callback>; 

            toy::coroutine_handle<void>   handle;
            R                             receiver;
            std::optional<token_callback> cb;

            state(auto&& handle, R receiver)
                : state_base(get_scheduler(receiver))
                , handle(handle)
                , receiver(receiver){
                handle.promise().state = this;
            }
            friend void start(state& self) {
                self.handle.resume();
                self.cb.emplace(get_stop_token(self.receiver), callback{&self});
            }
            void complete() override final {
                set_value(std::move(receiver), none{});
            }
            void stop() override final {
                set_stopped(std::move(receiver));
            }
        };

        toy::coroutine_handle<promise_type> handle;

        template <typename R>
        friend state<R> connect(task&& self, R receiver) {
            return state<R>(std::exchange(self.handle, std::coroutine_handle<promise_type>()), receiver);
        }
    };
    template <typename Scheduler>
        template <typename S>
    void task<Scheduler>::awaiter<S>::stop() {
        auto h = toy::coroutine_handle<promise_type>::from_address(this->handle.address());
        h.promise().state->stop();
    }
    template <typename Scheduler>
        template <typename S>
    toy::in_place_stop_source::stop_token task<Scheduler>::awaiter<S>::token() {
        auto h = toy::coroutine_handle<promise_type>::from_address(this->handle.address());
        return h.promise().state->stop_source.token();
    }
}

template <typename Scheduler>
using task = hidden_task::task<Scheduler>;

// ----------------------------------------------------------------------------

}

#endif
