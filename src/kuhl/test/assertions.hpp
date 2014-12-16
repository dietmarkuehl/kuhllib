// kuhl/test/assertions.hpp                                           -*-C++-*-
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

#ifndef INCLUDED_KUHL_TEST_ASSERTIONS
#define INCLUDED_KUHL_TEST_ASSERTIONS

#include "kuhl/test/context.hpp"
#include <utility>
#include <cstring>

// ----------------------------------------------------------------------------

namespace kuhl
{
    namespace test {
        namespace detail {
            template <typename T0, typename T1> struct is_same;
            template <typename T> struct is_same<T, T>;
        }

        auto assert_true(kuhl::test::context&, char const* message, bool value) -> bool;
        auto assert_true(kuhl::test::context&, bool value) -> bool;
        auto assert_false(kuhl::test::context&, char const* message, bool value) -> bool;
        auto assert_false(kuhl::test::context&, bool value) -> bool;

        template <typename A0, typename A1>
        auto assert_equal(kuhl::test::context& context, char const* message, A0&& a0, A1&& a1) -> bool;
        template <typename A0, typename A1>
        auto assert_equal(kuhl::test::context& context, A0&& a0, A1&& a1) -> bool;

        template <typename T0, typename T1>
        auto assert_type(kuhl::test::context& context, char const* message) -> bool;
    }

}

// ----------------------------------------------------------------------------

template <typename A0, typename A1>
auto kuhl::test::assert_equal(kuhl::test::context& context, char const* message, A0&& a0, A1&& a1) -> bool {
    if (!(a0 == a1)) {
        context<< message << (std::strlen(message)? ": ": "") << a0 << " != " << a1;
        return false;
    }
    return true;
}

template <typename A0, typename A1>
auto kuhl::test::assert_equal(kuhl::test::context& context, A0&& a0, A1&& a1) -> bool {
    return assert_equal(context, "", std::forward<A0>(a0), std::forward<A1>(a1));
}

// ----------------------------------------------------------------------------

template <typename T0, typename T1>
struct kuhl::test::detail::is_same {
    static constexpr bool value{false};
    static auto print(kuhl::test::context& context, char const* message) -> void {
        context << message;
    }
};
template <typename T>
struct kuhl::test::detail::is_same<T, T> {
    static constexpr bool value{true};
    static auto print(kuhl::test::context&, char const*) -> void {
    }
};

template <typename T0, typename T1>
auto kuhl::test::assert_type(kuhl::test::context& context, char const* message) -> bool
{
    kuhl::test::detail::is_same<T0, T1>::print(context, message);
    return kuhl::test::detail::is_same<T0, T1>::value;
}

// ----------------------------------------------------------------------------


#endif
