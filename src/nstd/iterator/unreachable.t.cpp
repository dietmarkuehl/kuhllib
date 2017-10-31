// nstd/iterator/unreachable.t.cpp                                    -*-C++-*-
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

#include "nstd/iterator/unreachable.hpp"
#include "kuhl/test.hpp"

namespace NI = ::nstd::iterator;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct foo {
        constexpr foo() {};
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("equality comparing anything to unreachable is false",
                       [](KT::context& c)->bool {
                           return !(foo() == NI::unreachable)
                               && !(NI::unreachable == foo())
                               ;
                       }),
    KT::expect_success("unequality comparing anything to unreachable is true",
                       [](KT::context& c)->bool {
                           return (foo() != NI::unreachable)
                               && (NI::unreachable != foo())
                               ;
                       }),
    KT::expect_success("unreachable operations are constexpr",
                       [](KT::context& c)->bool {
                           static_assert(!(foo() == NI::unreachable));
                           static_assert(!(NI::unreachable == foo()));
                           static_assert(foo() != NI::unreachable);
                           static_assert(NI::unreachable != foo());
                           return true;
                       }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("nstd::iterator::unreachable", ac, av, ::tests);
}
