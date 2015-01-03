// nstd/limits/numeric_limits.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_LIMITS_NUMERIC_LIMITS
#define INCLUDED_NSTD_LIMITS_NUMERIC_LIMITS

// ----------------------------------------------------------------------------

namespace nstd {
    namespace limits {
        namespace detail {
            template <typename T, bool, T, T> class integer_limits;
            template <typename> class float_limits;
        }
    }

    template <typename> class numeric_limits;
    template <> class numeric_limits<bool>;

    template <> class numeric_limits<char>;
    template <> class numeric_limits<char16_t>;
    template <> class numeric_limits<char32_t>;
    template <> class numeric_limits<wchar_t>;
    template <> class numeric_limits<signed char>;
    template <> class numeric_limits<unsigned char>;

    template <> class numeric_limits<signed short>;
    template <> class numeric_limits<signed int>;
    template <> class numeric_limits<signed long>;
    template <> class numeric_limits<signed long long>;
    template <> class numeric_limits<unsigned short>;
    template <> class numeric_limits<unsigned int>;
    template <> class numeric_limits<unsigned long>;
    template <> class numeric_limits<unsigned long long>;
    
    template <> class numeric_limits<float>;
    template <> class numeric_limits<double>;
    template <> class numeric_limits<long double>;
    
    enum float_round_style {
        round_indeterminate        = -1,
        round_towards_zero         =  0,
        round_towards_nearest      =  1,
        round_towards_infinity     =  2,
        round_towards_neg_infinity =  3
    };
    enum float_denorm_style {
        denorm_indeterminate = -1,
        denorm_absent        =  0,
        denorm_present       =  1
    };
}

// ----------------------------------------------------------------------------

template <typename T, bool Signed, T Min, T Max>
class nstd::limits::detail::integer_limits {
public:
    static constexpr bool is_specialized{true};

    static auto constexpr min() -> T { return Min; }
    static auto constexpr max() -> T { return Max; }
    static auto constexpr lowest() -> T { return Min; }

    static constexpr int digits{8};
    static constexpr int digits10{0};
    static constexpr int max_digits10{0};

    static constexpr bool is_signed{Signed};
    static constexpr bool is_integer{true};
    static constexpr bool is_exact{false};

    static constexpr int radix{0};
    static auto constexpr epsilon() -> T { return T(); }
    static auto constexpr round_error() -> T { return T(); }
    static constexpr int min_exponent{0};
    static constexpr int min_exponent10{0};
    static constexpr int max_exponent{0};
    static constexpr int max_exponent10{0};
    static constexpr bool has_infinity{false};
    static constexpr bool has_quiet_NaN{false};
    static constexpr bool has_signaling_NaN{false};
    static constexpr nstd::float_denorm_style has_denorm{nstd::denorm_absent};
    static constexpr bool has_denorm_loss{0};
    static auto constexpr infinity() noexcept(true) -> T { return T(); }
    static auto constexpr quiet_NaN() noexcept(true) -> T { return T(); }
    static auto constexpr signaling_NaN() noexcept(true) -> T { return T(); }
    static auto constexpr denorm_min() noexcept(true) -> T { return T(); }
    static constexpr bool is_iec559{false};
    static constexpr bool is_bounded{false};
    static constexpr bool is_modulo{false};
    static constexpr bool traps{false};
    static constexpr bool tinyness_before{false};
    static constexpr nstd::float_round_style round_style{nstd::round_towards_zero};
};

// ----------------------------------------------------------------------------

template <typename T>
class nstd::numeric_limits {
public:
    static constexpr bool is_specialized{false};

    static auto constexpr min() -> T { return T(); }
    static auto constexpr max() -> T { return T(); }
    static auto constexpr lowest() -> T { return T(); }

    static constexpr int digits{0};
    static constexpr int digits10{0};
    static constexpr int max_digits10{0};

    static constexpr bool is_signed{false};
    static constexpr bool is_integer{false};
    static constexpr bool is_exact{false};

    static constexpr int radix{0};
    static auto constexpr epsilon() -> T { return T(); }
    static auto constexpr round_error() -> T { return T(); }
    static constexpr int min_exponent{0};
    static constexpr int min_exponent10{0};
    static constexpr int max_exponent{0};
    static constexpr int max_exponent10{0};
    static constexpr bool has_infinity{false};
    static constexpr bool has_quiet_NaN{false};
    static constexpr bool has_signaling_NaN{false};
    static constexpr nstd::float_denorm_style has_denorm{nstd::denorm_absent};
    static constexpr bool has_denorm_loss{0};
    static auto constexpr infinity() noexcept(true) -> T { return T(); }
    static auto constexpr quiet_NaN() noexcept(true) -> T { return T(); }
    static auto constexpr signaling_NaN() noexcept(true) -> T { return T(); }
    static auto constexpr denorm_min() noexcept(true) -> T { return T(); }
    static constexpr bool is_iec559{false};
    static constexpr bool is_bounded{false};
    static constexpr bool is_modulo{false};
    static constexpr bool traps{false};
    static constexpr bool tinyness_before{false};
    static constexpr nstd::float_round_style round_style{nstd::round_towards_zero};
};

// ----------------------------------------------------------------------------

template <>
class nstd::numeric_limits<bool>
    : public ::nstd::limits::detail::integer_limits<bool, false, false, true> {
};

template <>
class nstd::numeric_limits<char>
    : public ::nstd::limits::detail::integer_limits<char, true, -128, 127> {
};
template <>
class nstd::numeric_limits<char16_t>
    : public ::nstd::limits::detail::integer_limits<char16_t, false, 0, 65535> {
};
template <>
class nstd::numeric_limits<char32_t>
    : public ::nstd::limits::detail::integer_limits<char32_t, false, 0, 4294967295> {
};
template <>
class nstd::numeric_limits<wchar_t>
    : public ::nstd::limits::detail::integer_limits<wchar_t, false, 0, 65535> {
};
template <>
class nstd::numeric_limits<signed char>
    : public ::nstd::limits::detail::integer_limits<signed char, true, -128, 127> {
};
template <>
class nstd::numeric_limits<unsigned char>
    : public ::nstd::limits::detail::integer_limits<unsigned char, false, 0, 255> {
};

template <>
class nstd::numeric_limits<signed short>
    : public ::nstd::limits::detail::integer_limits<signed short, true, -32768, 32767> {
};
template <>
class nstd::numeric_limits<unsigned short>
    : public ::nstd::limits::detail::integer_limits<unsigned short, false, 0, 65535> {
};

template <>
class nstd::numeric_limits<signed int>
    : public ::nstd::limits::detail::integer_limits<signed int, true, -2147483648, 2147483647> {
};
template <>
class nstd::numeric_limits<unsigned int>
    : public ::nstd::limits::detail::integer_limits<unsigned int, false, 0, 4294967295> {
};

template <>
class nstd::numeric_limits<signed long>
    : public ::nstd::limits::detail::integer_limits<signed long, true, -2147483648, 2147483647> {
};
template <>
class nstd::numeric_limits<unsigned long>
    : public ::nstd::limits::detail::integer_limits<unsigned long, false, 0, 4294967295> {
};

template <>
class nstd::numeric_limits<signed long long> //-dk:TODO
    : public ::nstd::limits::detail::integer_limits<signed long long, true, 0, 0> {
};
template <>
class nstd::numeric_limits<unsigned long long> //-dk:TODO 
    : public ::nstd::limits::detail::integer_limits<unsigned long long, false, 0, 0> {
};

// ----------------------------------------------------------------------------

#endif
