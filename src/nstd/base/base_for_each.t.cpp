// nstd/base/for_each.t.cpp                                           -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/base/for_each.hpp"
#include <stdexcept> //-dk:TODO replace by own exceptions
#include "kuhl/test.hpp"

namespace NB = nstd::base;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    template <typename T, int Size>
    struct rep {
        T   args[Size];
        int count = 0;
    };
    
    template <typename T, int Size>
    struct fun {
        rep<T, Size>* r;
        void operator()(T&& arg) {
            if (r->count == Size) {
                throw ::std::runtime_error("too many calls");
            }
            r->args[r->count] = arg;
            ++r->count;
        }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("for_each() on an empty sequence",
                       [](KT::context& c)->bool{
            int         array[] = { 0 };
            rep<int, 1> r;
            NB::for_each([](auto&& value){ return value; }, array, array,
                         fun<int, 1>{&r});
            return KT::assert_equal(c, "there are no function calls", 0, r.count)
                ;
        }),
    KT::expect_success("for_each() function calls each element",
                       [](KT::context& c)->bool{
            int         array[] = { 1, 2, 3, 4, 5 };
            rep<int, 5> r;
            NB::for_each([](auto&& value){ return value; }, array, array + 5,
                         fun<int, 5>{&r});
            return KT::assert_equal(c, "registered all calls", 5, r.count)
                &&  KT::assert_equal(c, "1st value", 1, r.args[0])
                &&  KT::assert_equal(c, "2nd value", 2, r.args[1])
                &&  KT::assert_equal(c, "3rd value", 3, r.args[2])
                &&  KT::assert_equal(c, "4th value", 4, r.args[3])
                &&  KT::assert_equal(c, "5th value", 5, r.args[4])
                ;
        }),
};

// ----------------------------------------------------------------------------

int main(int ac, char* av[])
{
    return KT::run_tests("base::for_each", ac, av, ::tests);
}
