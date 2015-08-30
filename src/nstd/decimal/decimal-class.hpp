// decimal-class.hpp                                                  -*-C++-*-
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

#ifndef INCLUDED_DECIMAL_CLASS
#define INCLUDED_DECIMAL_CLASS

#include "decimal_config.hpp"
#include "decimal_base.hpp"

// ----------------------------------------------------------------------------
// This file contains the definition of the actual decimal classes. Supporting
// operations are defined in various other files.

namespace kuhllib
{
    template <int Bits> class decimal;

    template <int Bits>
    constexpr decimal<Bits> operator+(decimal<Bits>);
    template <int Bits>
    constexpr decimal<Bits> operator-(decimal<Bits>);
}

// ----------------------------------------------------------------------------

template <int Bits>
class kuhllib::decimal
    : public kuhllib::decimal_base
{
public:
    typedef kuhllib::decimal_config<Bits>  config_type;
    typedef typename config_type::rep_type rep_type;

private:
    rep_type d_rep;

public:
    constexpr decimal() = default;
    constexpr decimal(bid_t, rep_type value);
    constexpr decimal(dpd_t, rep_type value);
    constexpr decimal(inf_t, bool negative);
    constexpr decimal(nan_t, bool signaling, rep_type payload);
    constexpr decimal(bool negative, rep_type significand, int exponent);

    constexpr classification classify() const;

    constexpr bool     negative() const;
    constexpr rep_type significand() const;
    constexpr int      exponent() const;
};

// ----------------------------------------------------------------------------

template <int Bits>
constexpr
kuhllib::decimal<Bits>::decimal(kuhllib::decimal<Bits>::bid_t, rep_type rep)
    : d_rep(rep)
{
}

template <int Bits>
constexpr
kuhllib::decimal<Bits>::decimal(bool negative, rep_type significand, int exponent)
    : d_rep((negative? config_type::signbit: 0u)
            | rep_type(exponent + config_type::bias) << config_type::normal_sig_size
            | significand)
{
}

// ----------------------------------------------------------------------------

template <int Bits>
constexpr kuhllib::decimal_base::classification
kuhllib::decimal<Bits>::classify() const {
    return kuhllib::decimal_base::positive_zero;
}

// ----------------------------------------------------------------------------

template <int Bits>
constexpr bool
kuhllib::decimal<Bits>::negative() const {
    return bool(this->d_rep & config_type::signbit);
}

template <int Bits>
constexpr typename kuhllib::decimal<Bits>::rep_type
kuhllib::decimal<Bits>::significand() const {
    return this->d_rep & config_type::normal_sig_mask;
}

template <int Bits>
constexpr int
kuhllib::decimal<Bits>::exponent() const {
    return std::int64_t((this->d_rep & config_type::normal_exp_mask) >> config_type::normal_sig_size) - config_type::bias;
}

// ----------------------------------------------------------------------------

template <int Bits>
constexpr kuhllib::decimal<Bits>
kuhllib::operator+(kuhllib::decimal<Bits> value) {
    return value;
}

template <int Bits>
constexpr kuhllib::decimal<Bits>
kuhllib::operator-(kuhllib::decimal<Bits> value) {
    return decimal<Bits>(!value.negative(), value.significand(), value.exponent());
}

// ----------------------------------------------------------------------------

#endif
