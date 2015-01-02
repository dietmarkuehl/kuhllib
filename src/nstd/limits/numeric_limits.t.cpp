// nstd/limits/numeric_limits.t.cpp                                   -*-C++-*-
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

#include "nstd/limits/numeric_limits.hpp"
#include "kuhl/test.hpp"

namespace NL = nstd::limits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct foo {
        int value;
        constexpr foo(): value(0) {}
        constexpr foo(int value): value(value) {}

        //-dk:TODO bool operator== (foo const& other) const { return this->value == other.value; }
        //-dk:TODO bool operator!= (foo const& other) const { return !(*this == other); }
    };
}

// ----------------------------------------------------------------------------

template <typename T>
static bool test_constexpr(KT::context& c) {
    constexpr auto is_specialized(nstd::numeric_limits<T>::is_specialized);

    constexpr auto min(nstd::numeric_limits<T>::min());
    constexpr auto max(nstd::numeric_limits<T>::max());
    constexpr auto lowest(nstd::numeric_limits<T>::lowest());

    constexpr auto digits(nstd::numeric_limits<T>::digits);
    constexpr auto digits10(nstd::numeric_limits<T>::digits10);
    constexpr auto max_digits10(nstd::numeric_limits<T>::max_digits10);

    constexpr auto is_signed(nstd::numeric_limits<T>::is_signed);
    constexpr auto is_integer(nstd::numeric_limits<T>::is_integer);
    constexpr auto is_exact(nstd::numeric_limits<T>::is_exact);

    constexpr auto radix(nstd::numeric_limits<T>::radix);
    constexpr auto epsilon(nstd::numeric_limits<T>::epsilon());
    constexpr auto round_error(nstd::numeric_limits<T>::round_error());

    constexpr auto min_exponent(nstd::numeric_limits<T>::min_exponent);
    constexpr auto min_exponent10(nstd::numeric_limits<T>::min_exponent10);
    constexpr auto max_exponent(nstd::numeric_limits<T>::max_exponent);
    constexpr auto max_exponent10(nstd::numeric_limits<T>::max_exponent10);

    constexpr auto has_infinity(nstd::numeric_limits<T>::has_infinity);
    constexpr auto has_quiet_NaN(nstd::numeric_limits<T>::has_quiet_NaN);
    constexpr auto has_signaling_NaN(nstd::numeric_limits<T>::has_signaling_NaN);
    constexpr auto has_denorm(nstd::numeric_limits<T>::has_denorm);
    constexpr auto has_denorm_loss(nstd::numeric_limits<T>::has_denorm_loss);
    constexpr auto infinity(nstd::numeric_limits<T>::infinity());
    constexpr auto quiet_NaN(nstd::numeric_limits<T>::quiet_NaN());
    constexpr auto signaling_NaN(nstd::numeric_limits<T>::signaling_NaN());
    constexpr auto denorm_min(nstd::numeric_limits<T>::denorm_min());
    constexpr auto is_iec559(nstd::numeric_limits<T>::is_iec559);
    constexpr auto is_bounded(nstd::numeric_limits<T>::is_bounded);
    constexpr auto is_modulo(nstd::numeric_limits<T>::is_modulo);
    constexpr auto traps(nstd::numeric_limits<T>::traps);
    constexpr auto tinyness_before(nstd::numeric_limits<T>::tinyness_before);
    constexpr auto round_style(nstd::numeric_limits<T>::round_style);

    return KT::assert_type<bool const, decltype(is_specialized)>(c, "is_specialized type")

        && KT::assert_type<T const, decltype(min)>(c, "min type")
        && KT::assert_type<T const, decltype(max)>(c, "max type")
        && KT::assert_type<T const, decltype(lowest)>(c, "lowest type")
        && KT::assert_type<int const, decltype(digits)>(c, "digits type")
        && KT::assert_type<int const, decltype(digits10)>(c, "digits10 type")
        && KT::assert_type<int const, decltype(max_digits10)>(c, "max_digits10 type")

        && KT::assert_type<bool const, decltype(is_signed)>(c, "is_signed type")
        && KT::assert_type<bool const, decltype(is_integer)>(c, "is_integer type")
        && KT::assert_type<bool const, decltype(is_exact)>(c, "is_exact type")

        && KT::assert_type<int const, decltype(radix)>(c, "radix type")
        && KT::assert_type<T const, decltype(epsilon)>(c, "epsilon type")
        && KT::assert_type<T const, decltype(round_error)>(c, "round_error type")

        && KT::assert_type<int const, decltype(min_exponent)>(c, "min_exponent type")
        && KT::assert_type<int const, decltype(min_exponent10)>(c, "min_exponent10 type")
        && KT::assert_type<int const, decltype(max_exponent)>(c, "max_exponent type")
        && KT::assert_type<int const, decltype(max_exponent10)>(c, "max_exponent10 type")
        && KT::assert_type<bool const, decltype(has_infinity)>(c, "has_infinity type")
        && KT::assert_type<bool const, decltype(has_quiet_NaN)>(c, "has_quiet_NaN type")
        && KT::assert_type<bool const, decltype(has_signaling_NaN)>(c, "has_signaling_NaN type")
        && KT::assert_type<nstd::float_denorm_style const, decltype(has_denorm)>(c, "has_denorm type")
        && KT::assert_type<bool const, decltype(has_denorm_loss)>(c, "has_denorm_loss type")

        && KT::assert_type<T const, decltype(infinity)>(c, "infinity type")
        && KT::assert_type<T const, decltype(quiet_NaN)>(c, "quiet_NaN type")
        && KT::assert_type<T const, decltype(signaling_NaN)>(c, "signaling_NaN type")
        && KT::assert_type<T const, decltype(denorm_min)>(c, "denorm_min type")

        && KT::assert_type<bool const, decltype(is_iec559)>(c, "is_iec559 type")
        && KT::assert_type<bool const, decltype(is_bounded)>(c, "is_bounded type")
        && KT::assert_type<bool const, decltype(is_modulo)>(c, "is_modulo type")

        && KT::assert_type<bool const, decltype(traps)>(c, "traps type")
        && KT::assert_type<bool const, decltype(tinyness_before)>(c, "tinyness_before type")
        && KT::assert_type<nstd::float_round_style const, decltype(round_style)>(c, "round_style type")
        ;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("non-specialized instantiation", [](KT::context& c)->bool{
            return test_constexpr<foo>(c)
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("limits/numeric_limits", ac, av, ::tests);
}
