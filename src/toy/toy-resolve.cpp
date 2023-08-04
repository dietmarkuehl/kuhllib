// toy-resolve.cpp                                                    -*-C++-*-
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

#include "toy-networking.hpp"
#include "toy-resolve.hpp"
#include "toy-sender.hpp"
#include "toy-task.hpp"
#include <iostream>

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

int main() {
    std::cout << std::unitbuf;
    toy::io_context context;
    
    context.spawn([]()->toy::task<toy::io_context::scheduler> {
        try {
            toy::address addr = co_await toy::resolve("www.google.com");
            std::cout << "addr=" << addr << "\n";
            addr = co_await toy::resolve("www.google.com", AF_INET);
            std::cout << "addr=" << addr << "\n";
        }
        catch (std::exception const& ex) {
            std::cout << "ERROR: " << ex.what() << "\n";
        }
    }());

    std::cout << "work created\n";
    context.run();
    std::cout << "done\n";
}
