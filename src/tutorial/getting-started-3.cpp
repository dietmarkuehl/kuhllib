// getting-started-3.cpp                                              -*-C++-*-
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

#include <iostream>
#include <utility>
#include <cassert>

// ----------------------------------------------------------------------------
// just - a sender producing a value

namespace tut {
    template <typename Value>
    class just {
    private:
        template <typename Receiver>
        struct state {
            Receiver d_receiver;
            Value    d_value;

            void start() { d_receiver.set_value(d_value); }
        };

        Value d_value;
    public:
        using result_t = Value;

        just(Value const& value): d_value(value) {}
        template <typename Receiver>
        auto connect(Receiver receiver) {
            return state<Receiver>{ receiver, d_value };
        }
    };
}

// ----------------------------------------------------------------------------
// then - a sender taking the result of the other sender and passing it into a function

namespace tut
{
    template <typename InnerSender, typename Fn>
    class then
    {
    private:
        template <typename Receiver>
        struct inner_receiver
        {
            Fn d_function;
            Receiver d_nextReceiver;

            template <typename T>
            void set_value(T const &value)
            {
                d_nextReceiver.set_value(d_function(value));
            }
        };

        InnerSender d_innerSender;
        Fn d_function;

    public:
        using result_t = std::invoke_result_t<Fn, typename InnerSender::result_t>;

        then(InnerSender sender, Fn function) : d_innerSender(sender), d_function(function) {}

        // for review: can user call connect multiple times? (can we move objects into state?)
        // todo: describe it in documentation for "just"
        template <typename Receiver>
        auto connect(Receiver receiver)
        {
            return d_innerSender.connect(inner_receiver<Receiver>{d_function, receiver});
        }
    };
}

// ----------------------------------------------------------------------------
// A context with a scheduler to run work items manually controlled: each
// time run_next() is called, the next work item is extracted and run.

namespace tut {
    struct none { // type returned when there is no useful result
        friend std::ostream& operator<< (std::ostream& out, none) {
            return out << "<none>";
        }
    };

    class manual_context {
    public:
        class scheduler;
    
    private:
        struct state_base {
            state_base() = default;
            state_base(state_base&&) = delete;
            state_base* d_next{nullptr};
            virtual void complete() = 0;
        };
        template <typename Receiver>
        struct state
            : state_base
        {
            manual_context* d_context;
            Receiver        d_receiver;

            state(manual_context* context, Receiver receiver)
                : d_context(context)
                , d_receiver(receiver) {
            }
            void start() {
                d_context->submit(this);
            }
            void complete() override {
                d_receiver.set_value(tut::none{});
            }
        };

        class sender {
            friend class scheduler;
            manual_context* d_context;

        public:
            using result_t = tut::none;
            explicit sender(manual_context* context): d_context(context) {}
            template <typename Receiver>
            state<Receiver> connect(Receiver receiver){ return state<Receiver>(d_context, receiver); }
        };
    
        state_base* d_stack{nullptr};
        void submit(state_base* work) {
            work->d_next = std::exchange(d_stack, work);
        }
    public:
        class scheduler {
            friend class manual_context;
            manual_context* d_context;

        public:
            explicit scheduler(manual_context* context): d_context(context) {}
            sender schedule() { return sender(this->d_context); }
        };

        scheduler get_scheduler() { return scheduler(this); }
        bool      empty() const { return d_stack == nullptr; }
        void      run_next() {
            assert(!empty());
            std::exchange(d_stack, d_stack->d_next)->complete();
        }
    };
}

// ----------------------------------------------------------------------------

namespace {
    struct print_receiver {
        template <typename T>
        void set_value(T const& value) {
            std::cout << "print_receiver::set_value(" << value << ")\n";
        }
    };
}

// ----------------------------------------------------------------------------

int main()
{
    tut::manual_context context;

    auto state1(
        tut::then(
            tut::then(
                context.get_scheduler().schedule(),
                [](tut::none){ std::cout << "schedule 1 completed\n"; return tut::none{}; }
            ),
            [](tut::none){ std::cout << "chained then completed\n"; return tut::none{}; }
        ).connect(print_receiver()));
    auto state2(
        tut::then(
            context.get_scheduler().schedule(),
            [](tut::none){ std::cout << "schedule 2 completed\n"; return tut::none{}; }
        ).connect(print_receiver()));

    state1.start();
    state2.start();

    std::cout << "two state objects are started\n";

    while (!context.empty()) {
        std::cout << "running next\n";
        context.run_next();
    }
}
