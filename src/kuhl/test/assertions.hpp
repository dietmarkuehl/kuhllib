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
//#include <source_location>
#include <initializer_list>
#include <utility>
#include <cstring>

// ----------------------------------------------------------------------------

namespace kuhl
{
    namespace test {
        namespace detail {
            template <typename T0, typename T1> struct is_same;
            template <typename T> struct is_same<T, T>;

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

        template <typename>
        auto assert_static_true(context&, char const* message) -> bool;
        template <typename>
        auto assert_static_false(context&, char const* message) -> bool;
        auto assert_true(kuhl::test::context&, char const* message, bool value) -> bool;
        auto assert_true(kuhl::test::context&, bool value) -> bool;
        auto assert_false(kuhl::test::context&, char const* message, bool value) -> bool;
        auto assert_false(kuhl::test::context&, bool value) -> bool;
        template <bool>
        auto assert_constexpr_true(context&, char const* message) -> bool;
        template <bool>
        auto assert_constexpr_false(context&, char const* message) -> bool;

        template <typename A0, typename A1>
        auto assert_equal(kuhl::test::context& context, char const* message, A0&& a0, A1&& a1) -> bool;
        template <typename A0, typename A1>
        auto assert_equal(kuhl::test::context& context, A0&& a0, A1&& a1) -> bool;

        template <typename T0, typename T1>
        auto assert_type(kuhl::test::context& context, char const* message) -> bool;
        template <typename T0, typename T1>
        auto assert_type(kuhl::test::context& context, T1, char const* message) -> bool;
        template <typename T>
        auto assert_no_nested_type(kuhl::test::context& context, char const* message) -> bool;

        template <typename>
        inline constexpr bool assert_type_exists = true;
        template <template <typename...> class T, typename... A>
        inline constexpr bool assert_template_exists = assert_type_exists<T<A...>>;

        template <typename> struct type_t {};
        template <typename T>
        inline constexpr type_t<T> type;
        template <typename T0, typename T1>
        bool operator== (type_t<T0>, type_t<T1>);
        template <typename T>               bool operator== (type_t<T>, type_t<T>) { return true; }
        template <typename T> type_t<T> get_type(T&&) { return {}; }

        template <typename B>
        auto assert_bitmask(context&, ::std::initializer_list<B>) -> bool;
    }

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

template <bool Value>
auto kuhl::test::assert_constexpr_true(context& c, char const* message) -> bool {
    if (!Value) {
        c << "wrong value: " << message;
    }
    return Value;
}

template <bool Value>
auto kuhl::test::assert_constexpr_false(context& c, char const* message) -> bool {
    return kuhl::test::assert_constexpr_true<!Value>(c, message);
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

template <typename T0, typename T1>
auto kuhl::test::assert_type(kuhl::test::context& context, T1, char const* message) -> bool
{
    kuhl::test::detail::is_same<T0, T1>::print(context, message);
    return kuhl::test::detail::is_same<T0, T1>::value;
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

template <typename B>
auto kuhl::test::assert_bitmask(context& context, ::std::initializer_list<B> es) -> bool
{
    bool rc{true};
    if (1u < es.size()) {
        for (auto it(es.begin()), end(es.end()); it != end; ++it) {
            rc = rc && kuhl::test::assert_true(context, "element zero value", *it != B());
            for (auto sit(it); ++sit != end; ) {
                rc = rc && kuhl::test::assert_equal(context, "elements with overlapping value", *it & *sit, B());
            }
        }
        auto it(es.begin());
        auto v0(*it);
        auto v1(*++it);

        auto all(v0 | v1);
        rc = rc && kuhl::test::assert_true(context, "both bits set", (all & v0) == v0 && (all & v1) == v1);
        auto none(v0 & v1);
        rc = rc && kuhl::test::assert_true(context, "no bits set", none == B());
        rc = rc && kuhl::test::assert_true(context, "xor bits", (all ^ v0) == v1 && (all ^ v1) == v0);
        rc = rc && kuhl::test::assert_true(context, "invert bits", (~v0 & v0) == B() && (~v0 & v1) == v1 && (~v1 & v0) == v0 && (~v1 & v1) == B());

        auto v_or(v0);
        v_or |= v1;
        rc = rc && kuhl::test::assert_equal(context, "or-assign", v_or, v0 | v1);
        auto v_and(all);
        v_and &= v1;
        rc = rc && kuhl::test::assert_equal(context, "and-assign", v_and, v1);
        auto v_xor(v0);
        v_xor ^= (v0 | v1);
        rc = rc && kuhl::test::assert_equal(context, "xor-assign", v_xor, v1);
    }
    (void)context;
    constexpr auto and_result = B() & B();
    constexpr auto or_result = B() | B();
    constexpr auto xor_result = B() ^ B();
    constexpr auto invert_result = ~B();
    return kuhl::test::type<decltype(and_result)> == kuhl::test::type<B const>
        && kuhl::test::type<decltype(or_result)> == kuhl::test::type<B const>
        && kuhl::test::type<decltype(xor_result)> == kuhl::test::type<B const>
        && kuhl::test::type<decltype(invert_result)> == kuhl::test::type<B const>
        && rc
        ;
}

// ----------------------------------------------------------------------------

#endif
