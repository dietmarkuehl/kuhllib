// getting-started-1.cpp                                              -*-C++-*-
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

// ----------------------------------------------------------------------------
// just - a sender producing a value

namespace tut {
    template <typename Value>
    class just {
    private:
        template <typename Receiver>
        struct operation_state {
            Receiver d_receiver;
            Value    d_value;

            void start() { d_receiver.set_value(d_value); }
        };

        Value d_value;
    public:
        using result_t = Value;

        just(Value const& value): d_value(value) {}

        template <typename Receiver>
        operation_state<Receiver> connect(Receiver receiver) {
            return { receiver, d_value };
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
    auto state(tut::just(42).connect(print_receiver()));
    state.start();
}
