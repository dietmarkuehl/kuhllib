// nstd/type_traits/integral_constant.t.cpp                           -*-C++-*-
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

#include "nstd/type_traits/integral_constant.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;

// ----------------------------------------------------------------------------

namespace
{
    // This does roughly what nstd::type_traits::is_same does but this class
    // template needs integral_constant for a proper definition. To avoid a
    // cyclic dependency the types are tested using a custom implementation.

    template <typename S, typename T>
    struct is_same {
        static constexpr bool value{false};
    };
    template <typename T>
    struct is_same<T, T> {
        static constexpr bool value{true};
    };
}

// ----------------------------------------------------------------------------

static kuhl::test::testcase const tests[] = {
    kuhl::test::expect_success("integral_constant<short, 1134>", []()->bool{
            constexpr NT::integral_constant<short, 1134> value;
            constexpr short value0{NT::integral_constant<short, 1134>::value};
            constexpr short value1{value};
            constexpr short value2{value()};
            return is_same<short, NT::integral_constant<short, 1134>::value_type>::value
                && is_same<short const, decltype(NT::integral_constant<short, 1134>::value)>::value
                && value0 == 1134
                && value1 == 1134
                && value2 == 1134
                && noexcept(NT::integral_constant<short, 1134>())
                && noexcept(short(value))
                && noexcept(value())
                ;
        }),
    kuhl::test::expect_success("true_type", []()->bool{
            constexpr NT::true_type value;
            constexpr bool value0{NT::true_type::value};
            constexpr bool value1{value};
            constexpr bool value2{value()};
            return is_same<NT::integral_constant<bool, true>, NT::true_type>::value
                && is_same<bool, NT::true_type::value_type>::value
                && is_same<bool const, decltype(NT::true_type::value)>::value
                && value0 == true
                && value1 == true
                && value2 == true
                && noexcept(NT::true_type())
                && noexcept(bool(value))
                && noexcept(value())
                ;
        }),
    kuhl::test::expect_success("false_type", []()->bool{
            constexpr NT::false_type value;
            constexpr bool value0{NT::false_type::value};
            constexpr bool value1{value};
            constexpr bool value2{value()};
            return is_same<NT::integral_constant<bool, false>, NT::false_type>::value
                && is_same<bool, NT::false_type::value_type>::value
                && is_same<bool const, decltype(NT::false_type::value)>::value
                && value0 == false
                && value1 == false
                && value2 == false
                && noexcept(NT::false_type())
                && noexcept(bool(value))
                && noexcept(value())
                ;
        })
};

int main(int ac, char* av[])
{
    return kuhl::test::run_tests("nstd::integral_constant", ac, av, ::tests);
}
