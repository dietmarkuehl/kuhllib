// nstd/functional/placeholders.t.cpp                                 -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/functional/placeholders.hpp"
#include "kuhl/test.hpp"

namespace NF = nstd::functional;
namespace NFP = NF::placeholders;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct foo;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("is_placeholder", [](KT::context& c)->bool{
            return KT::assert_false(c, "is_placeholder<foo>", NF::is_placeholder<foo>::value)
                && KT::assert_true(c, "is_placeholder<_1>", NF::is_placeholder<decltype(NFP::_1)>::value)
                && KT::assert_equal(c, "is_placeholder<_1> value", 1, NF::is_placeholder<decltype(NFP::_1)>())
                && KT::assert_true(c, "is_placeholder<_2>", NF::is_placeholder<decltype(NFP::_2)>::value)
                && KT::assert_equal(c, "is_placeholder<_2> value", 2, NF::is_placeholder<decltype(NFP::_2)>())
                && KT::assert_true(c, "is_placeholder<_3>", NF::is_placeholder<decltype(NFP::_3)>::value)
                && KT::assert_equal(c, "is_placeholder<_3> value", 3, NF::is_placeholder<decltype(NFP::_3)>())
                && KT::assert_true(c, "is_placeholder<_4>", NF::is_placeholder<decltype(NFP::_4)>::value)
                && KT::assert_equal(c, "is_placeholder<_4> value", 4, NF::is_placeholder<decltype(NFP::_4)>())
                && KT::assert_true(c, "is_placeholder<_5>", NF::is_placeholder<decltype(NFP::_5)>::value)
                && KT::assert_equal(c, "is_placeholder<_5> value", 5, NF::is_placeholder<decltype(NFP::_5)>())
                && KT::assert_true(c, "is_placeholder<_6>", NF::is_placeholder<decltype(NFP::_6)>::value)
                && KT::assert_equal(c, "is_placeholder<_6> value", 6, NF::is_placeholder<decltype(NFP::_6)>())
                && KT::assert_true(c, "is_placeholder<_7>", NF::is_placeholder<decltype(NFP::_7)>::value)
                && KT::assert_equal(c, "is_placeholder<_7> value", 7, NF::is_placeholder<decltype(NFP::_7)>())
                && KT::assert_true(c, "is_placeholder<_8>", NF::is_placeholder<decltype(NFP::_8)>::value)
                && KT::assert_equal(c, "is_placeholder<_8> value", 8, NF::is_placeholder<decltype(NFP::_8)>())
                && KT::assert_true(c, "is_placeholder<_9>", NF::is_placeholder<decltype(NFP::_9)>::value)
                && KT::assert_equal(c, "is_placeholder<_9> value", 9, NF::is_placeholder<decltype(NFP::_9)>())
                ;
        }),
    KT::expect_success("equality", [](KT::context& c)->bool{
            return KT::assert_true(c, "_1 == _1", NFP::_1 == NFP::_1)
                && KT::assert_false(c, "_1 == _2", NFP::_1 == NFP::_2)
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("functional::placeholders", ac, av, ::tests);
}
