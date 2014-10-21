// uint_t.hpp                                                         -*-C++-*-
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

#ifndef INCLUDED_UINT128
#define INCLUDED_UINT128

#include <cinttypes>
#include <cstddef>
#include <iosfwd>
#include <limits>
#include <utility>
#include <iostream>

// ----------------------------------------------------------------------------

namespace kuhllib
{
    template <typename Rep> class uint_t;
    template <typename Rep, std::size_t> struct uint_ops;
    template <typename Rep> struct uint_ops<Rep,  2u>;
    template <typename Rep> struct uint_ops<Rep,  8u>;
    template <typename Rep> struct uint_ops<Rep, 10u>;
    template <typename Rep> struct uint_ops<Rep, 16u>;
    template <typename Rep, std::size_t> struct uint_ops10;
    template <typename Rep> struct uint_ops10<Rep, 0u>;

    using uint128_t = uint_t<std::uint64_t>;
    using uint256_t = uint_t<uint128_t>;

    template <char...> constexpr uint128_t operator"" _u128();
    template <char...> constexpr uint256_t operator"" _u256();

    template <typename cT, typename Traits, typename Rep>
    std::basic_ostream<cT, Traits>&
    operator<< (std::basic_ostream<cT, Traits>&, uint_t<Rep>);
}

// ----------------------------------------------------------------------------

namespace std
{
    template <typename Rep>
    class numeric_limits<kuhllib::uint_t<Rep> >
    {
    public:
        static constexpr std::size_t digits = 2u * std::numeric_limits<Rep>::digits;
    };
}

// ----------------------------------------------------------------------------

template <typename Rep>
class kuhllib::uint_t
{
public:
    using rep_type = Rep;
    static constexpr std::size_t rep_digits = std::numeric_limits<Rep>::digits;
    static constexpr std::size_t digits = 2u * rep_digits;

private:
    rep_type d_low;
    rep_type d_high;
public:
    static constexpr uint_t multiply(rep_type, rep_type, rep_type, rep_type);
    static constexpr uint_t multiply(rep_type, rep_type);

    constexpr uint_t();
    explicit constexpr uint_t(rep_type value);
    explicit constexpr uint_t(rep_type low, rep_type high);

    explicit constexpr operator bool() const;
    explicit constexpr operator std::int64_t() const;
    explicit constexpr operator std::uint64_t() const;

    constexpr bool operator== (uint_t other) const;
    constexpr bool operator!= (uint_t other) const;
    constexpr bool operator<  (uint_t other) const;
    constexpr bool operator<= (uint_t other) const;
    constexpr bool operator>  (uint_t other) const;
    constexpr bool operator>= (uint_t other) const;

    constexpr uint_t operator~() const;
    constexpr uint_t operator|(uint_t other) const;
    constexpr uint_t operator&(uint_t other) const;
    constexpr uint_t operator^(uint_t other) const;

    constexpr uint_t operator<< (std::size_t step) const;
    constexpr uint_t operator>> (std::size_t step) const;

    constexpr uint_t operator+ (rep_type other) const;
    constexpr uint_t operator+ (kuhllib::uint_t<Rep> other) const;
    constexpr uint_t operator- (rep_type other) const;
    constexpr uint_t operator- (kuhllib::uint_t<Rep> other) const;
    constexpr uint_t operator* (rep_type other) const;
    constexpr uint_t operator* (kuhllib::uint_t<Rep> other) const;
};

// ----------------------------------------------------------------------------
// x * y
// == (xh << 32, xl) * (yh << 32, yl)
// == ((xh * yh) << 64) + ((xh * yl) + (xl * yh) << 32) + (xl * yl)

template <typename Rep>
constexpr kuhllib::uint_t<Rep>
kuhllib::uint_t<Rep>::multiply(rep_type xh, rep_type xl,
                               rep_type yh, rep_type yl)
{
    return uint_t(xl * yl, xh * yh)
        + (uint_t(xh * yl, 0ull) + uint_t(xl * yh, 0ull)) << 32u;
}

template <typename Rep>
constexpr kuhllib::uint_t<Rep>
kuhllib::uint_t<Rep>::multiply(rep_type x, rep_type y)
{
    return multiply(x >> (digits / 2), x & (~rep_type() >> (digits / 2)),
                    y >> (digits / 2), y & (~rep_type() >> (digits / 2)));
}

// ----------------------------------------------------------------------------

template <typename Rep>
constexpr
kuhllib::uint_t<Rep>::uint_t()
    : d_low()
    , d_high() {
}
template <typename Rep>
constexpr
kuhllib::uint_t<Rep>::uint_t(rep_type low)
    : d_low(low)
    , d_high() {
}
template <typename Rep>
constexpr
kuhllib::uint_t<Rep>::uint_t(rep_type low, rep_type high)
    : d_low(low)
    , d_high(high) {
}

// ----------------------------------------------------------------------------

template <typename Rep>
constexpr kuhllib::uint_t<Rep>::operator bool() const {
    return this->d_low | this->d_high;
}

template <typename Rep>
constexpr kuhllib::uint_t<Rep>::operator std::int64_t() const {
    return std::int64_t(this->d_low);
}

template <typename Rep>
constexpr kuhllib::uint_t<Rep>::operator std::uint64_t() const {
    return std::uint64_t(this->d_low);
}

// ----------------------------------------------------------------------------

template <typename Rep>
constexpr bool
kuhllib::uint_t<Rep>::operator== (kuhllib::uint_t<Rep> other) const {
    return this->d_low  == other.d_low
        && this->d_high == other.d_high;
}
template <typename Rep>
constexpr bool
kuhllib::uint_t<Rep>::operator!= (kuhllib::uint_t<Rep> other) const {
    return !this->operator==(other);
}

template <typename Rep>
constexpr bool
kuhllib::uint_t<Rep>::operator<  (kuhllib::uint_t<Rep> other) const {
    return this->d_high == other.d_high
         ? this->d_low  <  other.d_low
         : this->d_high <  other.d_high;
}

// ----------------------------------------------------------------------------

template <typename Rep>
constexpr kuhllib::uint_t<Rep>
kuhllib::uint_t<Rep>::operator~() const
{
    return kuhllib::uint_t<Rep>(~this->d_low, ~this->d_high);
}

// ----------------------------------------------------------------------------

template <typename Rep>
constexpr kuhllib::uint_t<Rep>
kuhllib::uint_t<Rep>::operator|(kuhllib::uint_t<Rep> other) const
{
    return kuhllib::uint_t<Rep>(this->d_low  | other.d_low,
                                this->d_high | other.d_high);
}

template <typename Rep>
constexpr kuhllib::uint_t<Rep>
kuhllib::uint_t<Rep>::operator&(kuhllib::uint_t<Rep> other) const
{
    return kuhllib::uint_t<Rep>(this->d_low  & other.d_low,
                                this->d_high & other.d_high);
}

template <typename Rep>
constexpr kuhllib::uint_t<Rep>
kuhllib::uint_t<Rep>::operator^(kuhllib::uint_t<Rep> other) const
{
    return kuhllib::uint_t<Rep>(this->d_low  ^ other.d_low,
                                this->d_high ^ other.d_high);
}

// ----------------------------------------------------------------------------

template <typename Rep>
constexpr kuhllib::uint_t<Rep>
kuhllib::uint_t<Rep>::operator<< (std::size_t step) const {
    return step < rep_digits
        ? (step == 0u
           ? *this
           : uint_t<Rep>(this->d_low << step, (this->d_high << step) | (this->d_low >> (rep_digits - step)))
           )
        : uint_t<Rep>(0u, this->d_low << (step - rep_digits));
}
template <typename Rep>
constexpr kuhllib::uint_t<Rep>
kuhllib::uint_t<Rep>::operator>> (std::size_t step) const {
    return step < rep_digits
        ? (step == 0u
           ? *this
           : uint_t<Rep>((this->d_low >> step) | (this->d_high << (rep_digits - step)), this->d_high >> step)
           )
        : uint_t<Rep>(this->d_high >> (step - rep_digits), 0u);
}

// ----------------------------------------------------------------------------

template <typename Rep>
constexpr kuhllib::uint_t<Rep>
kuhllib::uint_t<Rep>::operator+(rep_type other) const
{
    return uint_t<Rep>(this->d_low + other,
                       this->d_high + (std::numeric_limits<rep_type>::max() - other < this->d_low));
}

template <typename Rep>
constexpr kuhllib::uint_t<Rep>
kuhllib::uint_t<Rep>::operator+(kuhllib::uint_t<Rep> other) const
{
    return uint_t<Rep>(this->d_low + other.d_low,
                       this->d_high + other.d_high
                       + (std::numeric_limits<rep_type>::max() - other.d_low < this->d_low));
}

template <typename Rep>
constexpr kuhllib::uint_t<Rep>
kuhllib::uint_t<Rep>::operator-(rep_type other) const
{
    return uint_t<Rep>(this->d_low - other,
                       this->d_high - (this->d_low < other));
}

template <typename Rep>
constexpr kuhllib::uint_t<Rep>
kuhllib::uint_t<Rep>::operator-(kuhllib::uint_t<Rep> other) const
{
    return uint_t<Rep>(this->d_low - other.d_low,
                       this->d_high - other.d_high - (this->d_low < other.d_low));
}

// ----------------------------------------------------------------------------

template <typename Rep>
struct kuhllib::uint_ops<Rep, 2u>
{
    static constexpr kuhllib::uint_t<Rep>
    multiply(kuhllib::uint_t<Rep> value) {
        return value << 1u;
    }
    static constexpr kuhllib::uint_t<Rep>
    divide(kuhllib::uint_t<Rep> value) {
        return value >> 1u;
    }
    static std::pair<kuhllib::uint_t<Rep>, kuhllib::uint_t<Rep> >
    div_mod(kuhllib::uint_t<Rep> value) {
        return std::make_pair(value >> 1u, value & kuhllib::uint_t<Rep>(1u));
    }
};

template <typename Rep>
struct kuhllib::uint_ops<Rep, 8u>
{
    static constexpr kuhllib::uint_t<Rep>
    multiply(kuhllib::uint_t<Rep> value) {
        return value << 3u;
    }
    static constexpr kuhllib::uint_t<Rep>
    divide(kuhllib::uint_t<Rep> value) {
        return value >> 3u;
    }
    static std::pair<kuhllib::uint_t<Rep>, kuhllib::uint_t<Rep> >
    div_mod(kuhllib::uint_t<Rep> value) {
        return std::make_pair(value >> 3u, value & kuhllib::uint_t<Rep>(7u));
    }
};


template <typename Rep>
struct kuhllib::uint_ops10<Rep, 0u>
{
    static std::pair<kuhllib::uint_t<Rep>, kuhllib::uint_t<Rep> >
    div_mod(kuhllib::uint_t<Rep> value, kuhllib::uint_t<Rep> result) {
        return value < kuhllib::uint_t<Rep>(10u)
            ? std::make_pair(result << 1u, value)
            : std::make_pair((result << 1u) + 1u, value - kuhllib::uint_t<Rep>(10u));
    }
};

template <typename Rep, std::size_t Shift>
struct kuhllib::uint_ops10
{
    static std::pair<kuhllib::uint_t<Rep>, kuhllib::uint_t<Rep> >
    div_mod(kuhllib::uint_t<Rep> value, kuhllib::uint_t<Rep> result) {
        return value < (kuhllib::uint_t<Rep>(10u) << Shift)
            ? kuhllib::uint_ops10<Rep, Shift - 1u>::div_mod(value, result << 1u)
            : kuhllib::uint_ops10<Rep, Shift - 1u>::div_mod(value - (kuhllib::uint_t<Rep>(10u) << Shift),
                                                            (result << 1u) + 1u);
    }
};

template <typename Rep>
struct kuhllib::uint_ops<Rep, 10u>
{
    static constexpr kuhllib::uint_t<Rep>
    multiply(kuhllib::uint_t<Rep> value) {
        return (value << 3u) + (value << 1u);
    }

    static std::pair<kuhllib::uint_t<Rep>, kuhllib::uint_t<Rep> >
    div_mod(kuhllib::uint_t<Rep> value) {
        return kuhllib::uint_ops10<Rep, kuhllib::uint_t<Rep>::digits - 5u>::div_mod(value, kuhllib::uint_t<Rep>());
    }
};

template <typename Rep>
struct kuhllib::uint_ops<Rep, 16u>
{
    static constexpr kuhllib::uint_t<Rep>
    multiply(kuhllib::uint_t<Rep> value) {
        return value << 4u;
    }
    static constexpr kuhllib::uint_t<Rep>
    divide(kuhllib::uint_t<Rep> value) {
        return value >> 4u;
    }
    static std::pair<kuhllib::uint_t<Rep>, kuhllib::uint_t<Rep> >
    div_mod(kuhllib::uint_t<Rep> value) {
        return std::make_pair(value >> 4u, value & kuhllib::uint_t<Rep>(0xfu));
    }
};

// ----------------------------------------------------------------------------

namespace kuhllib
{
    namespace detail
    {
        template <unsigned int Base, typename Rep>
        constexpr kuhllib::uint_t<Rep>
        parse_u128_value(kuhllib::uint_t<Rep> value) {
            return value;
        }
        template <unsigned int Base, typename Rep, char Digit, char...C>
        constexpr kuhllib::uint_t<Rep>
        parse_u128_value(kuhllib::uint_t<Rep> value) {
            return parse_u128_value<Base, Rep, C...>(kuhllib::uint_ops<Rep, Base>::multiply(value)
                                                     + kuhllib::uint_t<Rep>('0' <= Digit && Digit <= '9'
                                                                            ? Digit - '0'
                                                                            : 10 + ('A' <= Digit && Digit <= 'F'
                                                                                    ? Digit - 'A'
                                                                                    : Digit - 'a')));
        }

        template <typename Rep, char Base, char...C>
        constexpr kuhllib::uint_t<Rep>
        parse_u128_base2() {
            return Base == 'x' || Base == 'X'
                ? kuhllib::detail::parse_u128_value<16u, Rep, C...>(kuhllib::uint_t<Rep>())
                : (Base == 'b' || Base == 'B'
                   ? kuhllib::detail::parse_u128_value<2u, Rep, C...>(kuhllib::uint_t<Rep>())
                   : kuhllib::detail::parse_u128_value<8u, Rep, C...>(kuhllib::uint_t<Rep>(Base - '0')));
        }
        template <typename Rep>
        constexpr kuhllib::uint_t<Rep>
        parse_u128_base2() {
            return kuhllib::uint_t<Rep>();
        }

        template <typename Rep, char B, char...C>
        constexpr kuhllib::uint_t<Rep>
        parse_u128_base() {
            return B == '0'
                ? kuhllib::detail::parse_u128_base2<Rep, C...>()
                : kuhllib::detail::parse_u128_value<10u, Rep, C...>(kuhllib::uint_t<Rep>(B - '0'));
        }
    }
}

template <char... C>
constexpr kuhllib::uint128_t kuhllib::operator"" _u128()
{
    return kuhllib::detail::parse_u128_base<std::uint64_t, C...>();
}
template <char... C>
constexpr kuhllib::uint256_t kuhllib::operator"" _u256()
{
    return kuhllib::detail::parse_u128_base<kuhllib::uint128_t, C...>();
}

// ----------------------------------------------------------------------------

#endif
