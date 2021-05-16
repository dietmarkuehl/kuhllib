// nstd/functional/is_bind_expression.t.cpp                           -*-C++-*-
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

#include "nstd/functional/is_bind_expression.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "kuhl/test.hpp"

namespace NF = nstd::functional;
namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct foo;
    struct bar {};
}

namespace nstd {
    namespace functional {
        template <>
        struct is_bind_expression<bar>
            : NT::true_type {
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("non-bind expression", [](KT::context& c)->bool{
            constexpr bool value{NF::is_bind_expression<foo>::value};
            return KT::assert_false(c, "foo", value)
                ;
        }),
    KT::expect_success("specialized bind expression", [](KT::context& c)->bool{
            constexpr bool value{NF::is_bind_expression<bar>::value};
            return KT::assert_true(c, "bar", value)
                ;
        }),
};

static KT::add_tests suite("functional::is_bind_expression", ::tests);
