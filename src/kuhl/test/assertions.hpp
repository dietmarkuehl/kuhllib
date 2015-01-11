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
#include "kuhl/mini/forward.hpp"
#include <string.h>

namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

namespace kuhl
{
    namespace test {
        namespace detail {
            template <typename T0, typename T1> struct is_same;
            template <typename T> struct is_same<T, T>;
            template <typename T0, typename T1> struct is_base;

            struct static_bool {
                template <typename T, typename = decltype(T::value)>
                static auto test(void*) -> bool { return true; }
                template <typename T>
                static auto test(...) -> bool { return false; }

                template <typename T, bool = T::value>
                static auto value(void*) -> bool { return T::value; }
                template <typename T>
                static auto value(...) -> bool { return false; }
            };
            struct no_nested_type {
                template <typename T, typename = typename T::type>
                static auto test(void*) -> bool { return false; }
                template <typename T>
                static auto test(...) -> bool { return true; }
            };
        }

        template <typename... T>
        void use(T const&...);

        template <typename T>
        auto declval() -> T&&;

        template <typename>
        auto assert_static_true(context&, char const* message) -> bool;
        template <typename>
        auto assert_static_false(context&, char const* message) -> bool;
        auto assert_true(kuhl::test::context&, char const* message, bool value) -> bool;
        auto assert_true(kuhl::test::context&, bool value) -> bool;
        auto assert_false(kuhl::test::context&, char const* message, bool value) -> bool;
        auto assert_false(kuhl::test::context&, bool value) -> bool;

        template <typename A0, typename A1>
        auto assert_equal(kuhl::test::context& context, char const* message, A0&& a0, A1&& a1) -> bool;
        template <typename A0, typename A1>
        auto assert_equal(kuhl::test::context& context, A0&& a0, A1&& a1) -> bool;
        template <typename A0, typename A1>
        auto assert_not_equal(kuhl::test::context& context, char const* message, A0&& a0, A1&& a1) -> bool;

        template <typename>
        auto assert_declared(context&, char const* message) -> bool;
        template <typename T0, typename T1>
        auto assert_type(kuhl::test::context& context, char const* message) -> bool;
        template <typename T0, typename T1>
        auto assert_is_base(kuhl::test::context& context, char const* message) -> bool;
        template <typename T>
        auto assert_no_nested_type(kuhl::test::context& context, char const* message) -> bool;
        template <typename T>
        auto assert_bitmask(kuhl::test::context& context, char const* message) -> bool;
    }
}

// ----------------------------------------------------------------------------

template <typename... T>
void kuhl::test::use(T const&...)
{
}

// ----------------------------------------------------------------------------

template <typename T>
auto kuhl::test::assert_static_true(context& c, char const* message) -> bool {
    if (!kuhl::test::detail::static_bool::test<T>(0)) {
        c << "no value: " << message;
        return false;
    }
    else if (!kuhl::test::detail::static_bool::value<T>(0)) {
        c << "wrong value: " << message;
        return false;
    }
    else {
        return true;
    }
}

template <typename T>
auto kuhl::test::assert_static_false(context& c, char const* message) -> bool {
    if (!kuhl::test::detail::static_bool::test<T>(0)) {
        c << "no value: " << message;
        return false;
    }
    else if (kuhl::test::detail::static_bool::value<T>(0)) {
        c << "wrong value: " << message;
        return false;
    }
    else {
        return true;
    }
}

// ----------------------------------------------------------------------------

template <typename A0, typename A1>
auto kuhl::test::assert_equal(kuhl::test::context& context, char const* message, A0&& a0, A1&& a1) -> bool {
    if (!(a0 == a1)) {
        context<< message << (strlen(message)? ": ": "") << a0 << " == " << a1 << " is false";
        return false;
    }
    else if (a0 != a1) {
        context<< message << (strlen(message)? ": ": "") << a0 << " != " << a1 << " is true";
        return false;
    }
    return true;
}

template <typename A0, typename A1>
auto kuhl::test::assert_equal(kuhl::test::context& context, A0&& a0, A1&& a1) -> bool {
    return assert_equal(context, "", KM::forward<A0>(a0), KM::forward<A1>(a1));
}

template <typename A0, typename A1>
auto kuhl::test::assert_not_equal(kuhl::test::context& context, char const* message, A0&& a0, A1&& a1) -> bool {
    if (a0 == a1) {
        context<< message << (strlen(message)? ": ": "") << a0 << " == " << a1 << " is true";
        return false;
    }
    else if (!(a0 != a1)) {
        context<< message << (strlen(message)? ": ": "") << a0 << " != " << a1 << " is false";
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------

template <typename>
auto kuhl::test::assert_declared(context&, char const*) -> bool {
    return true;
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

template <typename T0, typename T1>
struct kuhl::test::detail::is_base {
    static bool test_intern(T0*) { return true; }
    static bool test_intern(...) { return false; }
    static bool test() {
        return kuhl::test::detail::is_base<T0, T1>::test_intern(static_cast<T1*>(0));
    }
};

template <typename T0, typename T1>
auto kuhl::test::assert_is_base(kuhl::test::context& context, char const* message) -> bool {
    if (!kuhl::test::detail::is_base<T0, T1>::test()) {
        context << "not a base " << message;
    }
    return kuhl::test::detail::is_base<T0, T1>::test();
}

// ----------------------------------------------------------------------------

template <typename T>
auto kuhl::test::assert_no_nested_type(kuhl::test::context& context, char const* message) -> bool
{
    if (!kuhl::test::detail::no_nested_type::test<T>(0)) {
        context << message;
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------

template <typename T>
auto kuhl::test::assert_bitmask(kuhl::test::context& context, char const* message) -> bool {
    namespace KT = kuhl::test;
    constexpr T a0{};
    constexpr T a1{};
    T a2{};
    constexpr T bitmask_and{a0 & a1};
    constexpr T bitmask_or{a0 & a1};
    constexpr T bitmask_xor{a0 & a1};
    constexpr T bitmask_invert{~a0};
    T bitmask_land{a2 &= a1};
    T bitmask_lor{a2 |= a1};
    T bitmask_lxor{a2 ^= a1};
    KT::use(a0, a1, a2);
    KT::use(bitmask_and, bitmask_or, bitmask_xor);
    KT::use(bitmask_invert);
    KT::use(bitmask_land, bitmask_lor, bitmask_lxor);
    bool result(KT::assert_type<T, decltype(~KT::declval<T>())>(context, "invert ")
                );
    if (!result) {
        context << message;
    }
    return result;
}

// ----------------------------------------------------------------------------

#endif
