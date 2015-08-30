// decimal-literals.hpp                                               -*-C++-*-
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

#ifndef INCLUDED_DECIMAL_LITERALS
#define INCLUDED_DECIMAL_LITERALS

#include "decimal.hpp"

// ----------------------------------------------------------------------------

namespace kuhllib
{
    inline namespace literals {
        template <char...> constexpr decimal<32>  operator"" _df();
        template <char...> constexpr decimal<32>  operator"" _DF();
        template <char...> constexpr decimal<64>  operator"" _dd();
        template <char...> constexpr decimal<64>  operator"" _DD();
#if defined(KUHLLIB_DEC128_LITERALS)
        template <char...> constexpr decimal<128> operator"" _dl();
        template <char...> constexpr decimal<128> operator"" _DL();
#endif
    }

    namespace detail
    {
        template <int Bits>
        constexpr decimal<Bits> parse_exponent(typename decimal_config<Bits>::rep_type significand,
                                               bool negative,
                                               int  exponent,
                                               int  quantum) {
            return decimal<Bits>(false, significand, quantum + (negative? -exponent: exponent));
        }
        template <int Bits, char Digit, char...C>
        constexpr decimal<Bits> parse_exponent(typename decimal_config<Bits>::rep_type significand,
                                               bool negative,
                                               int  exponent,
                                               int  quantum) {
            return parse_exponent<Bits, C...>(significand, negative, exponent * 10 + Digit - '0', quantum);
        }

        template <int Bits>
        constexpr decimal<Bits> parse_exponent(typename decimal_config<Bits>::rep_type significand,
                                               int exponent) {
            return decimal<Bits>(false, significand, exponent);
        }
        template <int Bits, char Digit, char...C>
        constexpr decimal<Bits> parse_exponent(typename decimal_config<Bits>::rep_type significand,
                                               int exponent) {
            return Digit == '-' || Digit == '+'
                ? parse_exponent<Bits, C...>(significand, Digit == '-', 0, exponent)
                : parse_exponent<Bits, C...>(significand, false,  Digit - '0', exponent);
        }

        template <int Bits>
        constexpr decimal<Bits> parse_fractional(typename decimal_config<Bits>::rep_type significand, int exponent) {
            return decimal<Bits>(false, significand, exponent);
        }
        template <int Bits, char Digit, char... C>
        constexpr decimal<Bits> parse_fractional(typename decimal_config<Bits>::rep_type significand, int exponent) {
            return (Digit == 'E' || Digit == 'e')
                ? parse_exponent<Bits, C...>(significand, exponent)
                : parse_fractional<Bits, C...>(significand * 10u + Digit - '0', exponent - 1);
        }

        template <int Bits>
        constexpr decimal<Bits> parse_rounding_decimal(typename decimal_config<Bits>::rep_type significand, int exponent) {
            return decimal<Bits>(false, significand, exponent);
        }
        template <int Bits, char Digit, char... C>
        constexpr decimal<Bits> parse_rounding_decimal(typename decimal_config<Bits>::rep_type significand, int exponent) {
            return (Digit == 'E' || Digit == 'e')
                ? parse_exponent<Bits, C...>(significand, exponent)
                : (Digit == '.'
                   ? parse_fractional<Bits, C...>(significand, exponent) //-dk:TODO use special rounding version
                   : parse_rounding_decimal<Bits, C...>(significand, exponent + 1)
                   )
                ;
        }

        template <int Bits>
        constexpr decimal<Bits> parse_decimal(typename decimal_config<Bits>::rep_type significand) {
            return decimal<Bits>(false, significand, 0);
        }
        template <int Bits, char Digit, char... C>
        constexpr decimal<Bits> parse_decimal(typename decimal_config<Bits>::rep_type significand) {
            return (Digit == 'E' || Digit == 'e')
                ? parse_exponent<Bits, C...>(significand, 0)
                : (Digit == '.'
                   ? parse_fractional<Bits, C...>(significand, 0)
                   : (significand * 10u < decimal_config<Bits>::max_significand
                      ? parse_decimal<Bits, C...>(significand * 10u + Digit - '0')
                      : (significand == decimal_config<Bits>::max_significand && (5 <= Digit - '0')
                         ? parse_rounding_decimal<Bits, Digit, C...>((decimal_config<Bits>::max_significand + 1u)/10u, 1)
                         : parse_rounding_decimal<Bits, Digit, C...>(significand + (5 <= Digit - '0'), 0)
                         )
                      )
                   );
        }

        template <int Bits, char... C>
        constexpr decimal<Bits> parse_decimal() {
            return parse_decimal<Bits, C...>(typename decimal_config<Bits>::rep_type());
        }
    }
}

// ----------------------------------------------------------------------------

template <char... C>
constexpr kuhllib::decimal<32>
kuhllib::literals::operator"" _df() {
    return kuhllib::detail::parse_decimal<32, C...>();
}
template <char... C>
constexpr kuhllib::decimal<32>
kuhllib::literals::operator"" _DF() {
    return kuhllib::detail::parse_decimal<32, C...>();
}

template <char... C>
constexpr kuhllib::decimal<64>
kuhllib::literals::operator"" _dd() {
    return kuhllib::detail::parse_decimal<64, C...>();
}
template <char... C>
constexpr kuhllib::decimal<64>
kuhllib::literals::operator"" _DD() {
    return kuhllib::detail::parse_decimal<64, C...>();
}

#if defined(KUHLLIB_DEC128_LITERALS)
//-dk:TODO decimal<128> literals
template <char... C>
constexpr kuhllib::decimal<128>
kuhllib::literals::operator"" _dl() {
    return kuhllib::detail::parse_decimal<128, C...>();
}
template <char... C>
constexpr kuhllib::decimal<128>
kuhllib::literals::operator"" _DL() {
    return kuhllib::detail::parse_decimal<128, C...>();
}
#endif

// ----------------------------------------------------------------------------

#endif
