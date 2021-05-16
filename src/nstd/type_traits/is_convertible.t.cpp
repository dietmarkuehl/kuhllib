// nstd/type_traits/is_convertible.t.cpp                              -*-C++-*-
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

#include "nstd/type_traits/is_convertible.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct base {};
    struct derived: base {};
}

// ----------------------------------------------------------------------------
//-dk:TODO add more conversions

static kuhl::test::testcase const tests[] = {
    kuhl::test::expect_success("pointer to base can be converted to itself", []()->bool{
            constexpr NT::is_convertible<base*, base*> obj;
            NT::true_type const* ptr{&obj};
            return obj
                && ptr
                && noexcept(NT::is_convertible<base*, base*>{})
                ;
        }),
    kuhl::test::expect_success("pointer to base cannot be converted to derived", []()->bool{
            constexpr NT::is_convertible<base*, derived*> obj;
            NT::false_type const* ptr{&obj};
            return !obj
                && ptr
                && noexcept(NT::is_convertible<base*, derived*>{})
                ;
        }),
    kuhl::test::expect_success("pointer to derived can be converted to base", []()->bool{
            constexpr NT::is_convertible<derived*, base*> obj;
            NT::true_type const* ptr{&obj};
            return obj
                && ptr
                && noexcept(NT::is_convertible<derived*, base*>{})
                ;
        }),
};

static KT::add_tests suite("type_traits::is_convertible", ::tests);
