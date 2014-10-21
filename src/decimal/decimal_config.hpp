// decimal_config.hpp                                                 -*-C++-*-
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

#ifndef INCLUDED_DECIMAL_CONFIG
#define INCLUDED_DECIMAL_CONFIG

#include "uint128_t.hpp"
#include <cinttypes>

// ----------------------------------------------------------------------------

namespace kuhllib
{
    template <typename Rep, int Bits> struct decimal_config_base;
    template <int Bits> struct decimal_config;
    template <> struct decimal_config<32>;
    template <> struct decimal_config<64>;
    template <> struct decimal_config<128>;
}

// ----------------------------------------------------------------------------
// k=#bits w=k/16+4
// k= 32 => w+5 == 11
// k= 64 => w+5 == 13
// k=128 => w+5 == 17
// S G=w+5 T=J*10
// S sign
// G combination
// T sig main
// G table:
// G0 G1 G2 G3 G4 G5
//  0  y  x  x  x  x => exp = G0 .. Gw+1 sig = Gw+2 ... T
//  1  0  x  x  x  x => exp = G0 .. Gw+1 sig = Gw+2 ... T 
//  1  1  0  y  x  x => exp = G2 .. Gw+3 sig = 8 + Gw+4 ... T
//  1  1  1  0  x  x => exp = G2 .. Gw+3 sig = 8 + Gw+4 ... T
//  1  1  1  1  0  x => inf,  T is ignored
//  1  1  1  1  1  0 => qNaN, T is payload
//  1  1  1  1  1  1 => sNaN, T is payload

template <typename Rep, int Bits>
struct kuhllib::decimal_config_base
{
    typedef Rep rep_type;

    static constexpr int  k{Bits};
    static constexpr int  w{k/16 + 4};
    static constexpr int  t{k - (w+5) - 1};
    static constexpr int  p{3*t/10 + 1};
    static constexpr long emax{3 * (1l << (w-1))};
    static constexpr long emin{1l - emax};
    static constexpr long bias{emax + p - 2};

    static constexpr rep_type signbit{rep_type(1u) << (k - 1)};
    static constexpr rep_type special_mask{rep_type(3u) << (k - 3)};

    static constexpr rep_type normal_exp_mask{(~rep_type() << (k - (w + 2))) >> 1};
    static constexpr int      normal_sig_size{k - (w + 3)};
    static constexpr rep_type normal_sig_mask{~rep_type() >> (k - normal_sig_size)};
};

// ----------------------------------------------------------------------------

template <>
struct kuhllib::decimal_config<32>
    : kuhllib::decimal_config_base<std::uint32_t, 32>
{
};

template <>
struct kuhllib::decimal_config<64>
    : kuhllib::decimal_config_base<std::uint64_t, 64>
{
};

template <>
struct kuhllib::decimal_config<128>
    : kuhllib::decimal_config_base<kuhllib::uint128_t, 128>
{
};

// ----------------------------------------------------------------------------

template <typename Rep, int Bits>
constexpr Rep kuhllib::decimal_config_base<Rep, Bits>::signbit;
template <typename Rep, int Bits>
constexpr Rep kuhllib::decimal_config_base<Rep, Bits>::normal_exp_mask;
template <typename Rep, int Bits>
constexpr Rep kuhllib::decimal_config_base<Rep, Bits>::normal_sig_mask;

// ----------------------------------------------------------------------------

#endif
