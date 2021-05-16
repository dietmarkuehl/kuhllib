// nstd/type_traits/choose_type.t.cpp                                 -*-C++-*-
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

#include "nstd/type_traits/choose_type.hpp"
#include "nstd/type_traits/is_same.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct A;
    struct B;
    struct C;
    struct D;
}

// ----------------------------------------------------------------------------

namespace
{
    template <typename T>
    bool test(T*, typename NT::choose_type<T, NT::choice<false, A>>::type* = 0) {
        return true;
    }
    char test(void*) {
        return 'n';
    }
}

// ----------------------------------------------------------------------------

static kuhl::test::testcase const tests[] = {
    kuhl::test::expect_success("no match, no type", []()->bool{
            return NT::is_same<char, decltype(test(static_cast<NT::choice<false, int>*>(0)))>::value
                && 'n' == test(static_cast<NT::choice<false, int>*>(0))
                && NT::is_same<bool, decltype(test(static_cast<NT::choice<true, int>*>(0)))>::value
                && test(static_cast<NT::choice<true, int>*>(0))
                ;
        }),
    kuhl::test::expect_success("first type matches", []()->bool{
            return NT::is_same<A, typename NT::choose_type<NT::choice<true, A>,
                                                           NT::choice<true, B>,
                                                           NT::choice<true, C>,
                                                           NT::choice<true, D>>::type>::value
                && NT::is_same<A, NT::choose_type_t<NT::choice<true, A>,
                                                    NT::choice<true, B>,
                                                    NT::choice<true, C>,
                                                    NT::choice<true, D>>>::value;
        }),
    kuhl::test::expect_success("second type matches", []()->bool{
            return NT::is_same<B, typename NT::choose_type<NT::choice<false, A>,
                                                           NT::choice<true, B>,
                                                           NT::choice<true, C>,
                                                           NT::choice<true, D>>::type>::value
                && NT::is_same<B, NT::choose_type_t<NT::choice<false, A>,
                                                    NT::choice<true, B>,
                                                    NT::choice<true, C>,
                                                    NT::choice<true, D>>>::value;
        }),
    kuhl::test::expect_success("third type matches", []()->bool{
            return NT::is_same<C, typename NT::choose_type<NT::choice<false, A>,
                                                           NT::choice<false, B>,
                                                           NT::choice<true, C>,
                                                           NT::choice<true, D>>::type>::value
                && NT::is_same<C, NT::choose_type_t<NT::choice<false, A>,
                                                    NT::choice<false, B>,
                                                    NT::choice<true, C>,
                                                    NT::choice<true, D>>>::value;
        }),
    kuhl::test::expect_success("last type matches", []()->bool{
            return NT::is_same<D, typename NT::choose_type<NT::choice<false, A>,
                                                           NT::choice<false, B>,
                                                           NT::choice<false, C>,
                                                           NT::choice<true, D>>::type>::value
                && NT::is_same<D, NT::choose_type_t<NT::choice<false, A>,
                                                    NT::choice<false, B>,
                                                    NT::choice<false, C>,
                                                    NT::choice<true, D>>>::value;
        }),
};

static KT::add_tests suite("type_traits::choose_type", ::tests);
