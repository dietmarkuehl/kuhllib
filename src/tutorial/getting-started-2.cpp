// getting-started-2.cpp                                              -*-C++-*-
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
#include <type_traits>

// ----------------------------------------------------------------------------
// just - a sender producing a value

namespace tut
{
    template <typename Value>
    class just
    {
    private:
        template <typename Receiver>
        struct operation_state
        {
            Receiver d_receiver;
            Value d_value;

            void start() { d_receiver.set_value(d_value); }
        };

        Value d_value;

    public:
        using result_t = Value;

        just(Value const& value): d_value(value) {}

        template <typename Receiver>
        operation_state<Receiver> connect(Receiver receiver)
        {
            return { receiver, d_value };
        }
    };
}

// ----------------------------------------------------------------------------

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

        template <typename Receiver>
        auto connect(Receiver receiver)
        {
            return d_innerSender.connect(inner_receiver<Receiver>{d_function, receiver});
        }
    };
}

// ----------------------------------------------------------------------------

namespace
{
    struct print_receiver
    {
        template <typename T>
        void set_value(T const &value)
        {
            std::cout << "print_receiver::set_value(" << value << ")\n";
        }
    };
}

// ----------------------------------------------------------------------------

int main()
{
    auto sender1 = tut::just(42);
    auto sender2 = tut::then(sender1, [](auto value)
                             { return value * 100; });
    auto sender3 = tut::then(sender2, [](auto value)
                             { return value + 55; });
    auto state = sender3.connect(print_receiver{});
    state.start();
}
