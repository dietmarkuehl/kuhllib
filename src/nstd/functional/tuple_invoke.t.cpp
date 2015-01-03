// nstd/functional/tuple_invoke.t.cpp                                 -*-C++-*-
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

#include "nstd/functional/tuple_invoke.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/type_traits/decay.hpp"
#include "nstd/utility/tuple.hpp"
#include "kuhl/test.hpp"

namespace NF = nstd::functional;
namespace NU = nstd::utility;
namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct foo {
        template <typename... T>
        NU::tuple<NT::decay_t<T>...> operator()(T&&... args) {
            return NU::make_tuple(NU::forward<T>(args)...);
        }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("invoke with no argument", [](KT::context& c)->bool{
            return KT::assert_type<NU::tuple<>, decltype(NF::tuple_invoke(foo(), NU::make_tuple()))>(c, "type")
                && KT::assert_true(c, "value", NU::tuple<>() == NF::tuple_invoke(foo(), NU::make_tuple()))
                ;
        }),
    KT::expect_success("invoke with one argument", [](KT::context& c)->bool{
            return KT::assert_type<NU::tuple<int>, decltype(NF::tuple_invoke(foo(), NU::make_tuple(17)))>(c, "type")
                && KT::assert_true(c, "value", NU::tuple<int>(17) == NF::tuple_invoke(foo(), NU::make_tuple(17)))
                ;
        }),
    KT::expect_success("invoke with two arguments", [](KT::context& c)->bool{
            return KT::assert_type<NU::tuple<int, char>, decltype(NF::tuple_invoke(foo(), NU::make_tuple(17, 'c')))>(c, "type")
                && KT::assert_true(c, "value", NU::tuple<int, char>(17, 'c') == NF::tuple_invoke(foo(), NU::make_tuple(17, 'c')))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("functional::tuple_invoke", ac, av, ::tests);
}
