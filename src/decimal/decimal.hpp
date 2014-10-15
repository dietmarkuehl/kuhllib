// decimal.hpp                                                        -*-C++-*-
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

#ifndef INCLUDED_DECIMAL
#define INCLUDED_DECIMAL

#include "decimal_config.hpp"
#include <iosfwd>

// ----------------------------------------------------------------------------

namespace kuhllib
{
    template <int Bits> class decimal;

    template <int Bits>
    bool operator== (decimal<Bits> const&, decimal<Bits> const&);

    template <typename cT, typename Tr, int Bits>
    std::basic_ostream<cT, Tr>& operator<< (std::basic_ostream<cT, Tr>& out,
                                            decimal<Bits> const& value);
}

// ----------------------------------------------------------------------------

template <int Bits>
class kuhllib::decimal
{
public:
    typedef typename kuhllib::decimal_config<Bits>::rep_type rep_type;

private:
    rep_type d_rep;

    constexpr decimal(rep_type rep);
public:
    constexpr decimal() = default;
    constexpr decimal(bool negative, rep_type significand, int exponent);

    bool     negative() const;
    rep_type significand() const;
    int      exponent() const;

    constexpr decimal<Bits> operator+() const;
    constexpr decimal<Bits> operator-() const;
};

// ----------------------------------------------------------------------------

template <int Bits>
constexpr
kuhllib::decimal<Bits>::decimal(rep_type rep)
    : d_rep(rep)
{
}

template <int Bits>
constexpr
kuhllib::decimal<Bits>::decimal(bool negative, rep_type significand, int)
    : d_rep((negative? decimal_config<Bits>::signbit: 0u) | significand)
{
}

// ----------------------------------------------------------------------------

template <int Bits>
bool
kuhllib::decimal<Bits>::negative() const {
    return this->d_rep & decimal_config<Bits>::signbit;
}

template <int Bits>
typename kuhllib::decimal<Bits>::rep_type
kuhllib::decimal<Bits>::significand() const {
    return this->d_rep & ~decimal_config<Bits>::signbit;
}

template <int Bits>
int
kuhllib::decimal<Bits>::exponent() const {
    return 0;
}

// ----------------------------------------------------------------------------

template <int Bits>
constexpr kuhllib::decimal<Bits>
kuhllib::decimal<Bits>::operator+() const {
    return *this;
}

template <int Bits>
constexpr kuhllib::decimal<Bits>
kuhllib::decimal<Bits>::operator-() const {
    return decimal<Bits>(decimal_config<Bits>::signbit ^ this->d_rep);
}

// ----------------------------------------------------------------------------

template <int Bits>
bool
kuhllib::operator== (kuhllib::decimal<Bits> const& d0,
                     kuhllib::decimal<Bits> const& d1) {
    //-dk:TODO deal with NaNs
    //-dk:TODO deal with zero
    //-dk:TODO deal with different scales
    return d0.negative()    == d1.negative()
        && d0.significand() == d1.significand()
        && d0.exponent()    == d1.exponent();
}

// ----------------------------------------------------------------------------

#endif
