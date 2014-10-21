// uint128_t.hpp                                                      -*-C++-*-
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

// ----------------------------------------------------------------------------

namespace kuhllib
{
    class uint128_t;
    template <char...>
    constexpr uint128_t operator"" _uLL();

    template <typename cT, typename Traits>
    std::basic_ostream<cT, Traits>&
    operator<< (std::basic_ostream<cT, Traits>&, uint128_t);
}

// ----------------------------------------------------------------------------

class kuhllib::uint128_t
{
private:
    std::uint64_t d_low;
    std::uint64_t d_high;
public:
    static constexpr uint128_t multiply(std::uint64_t, std::uint64_t,
                                        std::uint64_t, std::uint64_t);
    static constexpr uint128_t multiply(std::uint64_t, std::uint64_t);

    constexpr uint128_t();
    explicit constexpr uint128_t(std::uint64_t value);
    explicit constexpr uint128_t(std::uint64_t low, std::uint64_t high);

    explicit constexpr operator bool() const;
    explicit constexpr operator std::int64_t() const;
    explicit constexpr operator std::uint64_t() const;

    constexpr bool operator== (uint128_t other) const;
    constexpr bool operator!= (uint128_t other) const;

    constexpr uint128_t operator~() const;
    constexpr uint128_t operator|(uint128_t other) const;
    constexpr uint128_t operator&(uint128_t other) const;
    constexpr uint128_t operator^(uint128_t other) const;

    constexpr uint128_t operator<< (std::size_t step) const;
    constexpr uint128_t operator>> (std::size_t step) const;

    constexpr uint128_t operator+ (std::uint64_t other) const;
    constexpr uint128_t operator+ (kuhllib::uint128_t other) const;
    template <unsigned int Factor>
    constexpr uint128_t multiply() const;
    constexpr uint128_t operator* (std::uint64_t other) const;
    constexpr uint128_t operator* (kuhllib::uint128_t other) const;
    template <unsigned int Factor>
    constexpr uint128_t divide() const;
};

// ----------------------------------------------------------------------------
// x * y
// == (xh << 32, xl) * (yh << 32, yl)
// == ((xh * yh) << 64) + ((xh * yl) + (xl * yh) << 32) + (xl * yl)

constexpr kuhllib::uint128_t
kuhllib::uint128_t::multiply(std::uint64_t xh, std::uint64_t xl,
                             std::uint64_t yh, std::uint64_t yl)
{
    return uint128_t(xl * yl, xh * yh)
        + (uint128_t(xh * yl, 0ull) + uint128_t(xl * yh, 0ull)) << 32u;
}

constexpr kuhllib::uint128_t
kuhllib::uint128_t::multiply(std::uint64_t x, std::uint64_t y)
{
    return multiply(x >> 32, x & 0xffffffffull,
                    y >> 32, y & 0xffffffffull);
}

// ----------------------------------------------------------------------------

constexpr
kuhllib::uint128_t::uint128_t()
    : d_low()
    , d_high() {
}
inline constexpr
kuhllib::uint128_t::uint128_t(std::uint64_t value)
    : d_low(value)
    , d_high(0u) {
}
inline constexpr
kuhllib::uint128_t::uint128_t(std::uint64_t low, std::uint64_t high)
    : d_low(low)
    , d_high(high) {
}

// ----------------------------------------------------------------------------

constexpr kuhllib::uint128_t::operator bool() const {
    return this->d_low | this->d_high;
}

constexpr kuhllib::uint128_t::operator std::int64_t() const {
    return std::int64_t(this->d_low);
}

constexpr kuhllib::uint128_t::operator std::uint64_t() const {
    return std::uint64_t(this->d_low);
}

// ----------------------------------------------------------------------------

inline constexpr bool
kuhllib::uint128_t::operator== (kuhllib::uint128_t other) const {
    return this->d_low == other.d_low
        && this->d_high == other.d_high;
}
inline constexpr bool
kuhllib::uint128_t::operator!= (kuhllib::uint128_t other) const {
    return !this->operator==(other);
}

// ----------------------------------------------------------------------------

inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::operator~() const
{
    return kuhllib::uint128_t(~this->d_low, ~this->d_high);
}

// ----------------------------------------------------------------------------

inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::operator|(kuhllib::uint128_t other) const
{
    return kuhllib::uint128_t(this->d_low | other.d_low,
                              this->d_high | other.d_high);
}

inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::operator&(kuhllib::uint128_t other) const
{
    return kuhllib::uint128_t(this->d_low & other.d_low,
                              this->d_high & other.d_high);
}

inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::operator^(kuhllib::uint128_t other) const
{
    return kuhllib::uint128_t(this->d_low ^ other.d_low,
                              this->d_high ^ other.d_high);
}

// ----------------------------------------------------------------------------

inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::operator<< (std::size_t step) const {
    return step < 64u
        ? (step == 0u
           ? *this
           : uint128_t(this->d_low << step, (this->d_high << step) | (this->d_low >> (64u - step)))
           )
        : uint128_t(0u, this->d_low << (step - 64u));
}
inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::operator>> (std::size_t step) const {
    return step < 64u
        ? (step == 0u
           ? *this
           : uint128_t((this->d_low >> step) | (this->d_high << (64u - step)), this->d_high >> step)
           )
        : uint128_t(this->d_high >> (step - 64u), 0u);
}

// ----------------------------------------------------------------------------

inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::operator+(std::uint64_t other) const
{
    return uint128_t(this->d_low + other,
                     this->d_high + 
                     + (std::numeric_limits<std::uint64_t>::max() - other < this->d_low));
}

inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::operator+(kuhllib::uint128_t other) const
{
    return uint128_t(this->d_low + other.d_low,
                     this->d_high + other.d_high
                     + (std::numeric_limits<std::uint64_t>::max() - other.d_low < this->d_low));
}

// ----------------------------------------------------------------------------

template <>
inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::multiply<2u>() const
{
    return *this << 1u;
}

template <>
inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::multiply<8u>() const
{
    return *this << 3u;
}

template <>
inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::multiply<10u>() const
{
    return (*this << 3u) + (*this << 1u);
}

template <>
inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::multiply<16u>() const
{
    return *this << 4u;
}

inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::operator*(std::uint64_t other) const
{
    return uint128_t(this->d_low * other,
                     this->d_high * other);
}

inline constexpr kuhllib::uint128_t
kuhllib::uint128_t::operator*(kuhllib::uint128_t ) const
{
    return *this;
}

// ----------------------------------------------------------------------------

template <>
constexpr kuhllib::uint128_t
kuhllib::uint128_t::divide<8u>() const
{
    return *this >> 3u;
}

template <>
constexpr kuhllib::uint128_t
kuhllib::uint128_t::divide<16u>() const
{
    return *this >> 4u;
}

// ----------------------------------------------------------------------------

namespace kuhllib
{
    namespace detail
    {
        template <unsigned int Base>
        constexpr kuhllib::uint128_t
        parse_u128_value(kuhllib::uint128_t value) {
            return value;
        }
        template <unsigned int Base, char Digit, char...C>
        constexpr kuhllib::uint128_t
        parse_u128_value(kuhllib::uint128_t value) {
            return parse_u128_value<Base, C...>(value.multiply<Base>()
                                                + kuhllib::uint128_t('0' <= Digit && Digit <= '9'
                                                                     ? Digit - '0'
                                                                     : 10 + ('A' <= Digit && Digit <= 'F'
                                                                             ? Digit - 'A'
                                                                             : Digit - 'a')));
        }

        template <unsigned int, char Base, char...C>
        constexpr kuhllib::uint128_t
        parse_u128_base2() {
            return Base == 'x' || Base == 'X'
                ? kuhllib::detail::parse_u128_value<16u, C...>(kuhllib::uint128_t())
                : (Base == 'b' || Base == 'B'
                   ? kuhllib::detail::parse_u128_value<2u, C...>(kuhllib::uint128_t())
                   : kuhllib::detail::parse_u128_value<8u, C...>(kuhllib::uint128_t(Base - '0')));
        }
        template <unsigned int Base>
        constexpr kuhllib::uint128_t
        parse_u128_base2() {
            return kuhllib::uint128_t();
        }

        template <char B, char...C>
        constexpr kuhllib::uint128_t
        parse_u128_base() {
            return B == '0'
                ? kuhllib::detail::parse_u128_base2<0u, C...>()
                : kuhllib::detail::parse_u128_value<10u, C...>(kuhllib::uint128_t(B - '0'));
        }
    }
}

template <char... C>
constexpr kuhllib::uint128_t kuhllib::operator"" _uLL()
{
    return kuhllib::detail::parse_u128_base<C...>();
}

// ----------------------------------------------------------------------------

#endif
