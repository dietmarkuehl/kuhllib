// uint_t.cpp                                                      -*-C++-*-
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

#include "uint_t.hpp"
#include <algorithm>
#include <ostream>
#include <iterator>
#include <locale>

// ----------------------------------------------------------------------------

namespace
{
    template <unsigned int Base, typename cT, typename Traits, typename Rep>
    std::basic_ostream<cT, Traits>&
    format(std::basic_ostream<cT, Traits>& out,
           kuhllib::uint_t<Rep>            value)
    {
        std::ctype<cT> const& ct(std::use_facet<std::ctype<cT> >(out.getloc()));
        char  buffer[128];
        char* end(buffer);

        do {
            auto p(kuhllib::uint_ops<Rep, Base>::div_mod(value));
            *end++ = "0123456789abcdef"[static_cast<uint64_t>(p.second)];
            value = p.first;
        }
        while (value);

        std::transform(std::reverse_iterator<char*>(end),
                       std::reverse_iterator<char*>(buffer),
                       std::ostreambuf_iterator<cT>(out),
                       [&](char c){ return ct.widen(c); });
        return out;;
    }
}

// ----------------------------------------------------------------------------

template <typename cT, typename Traits, typename Rep>
std::basic_ostream<cT, Traits>&
kuhllib::operator<< (std::basic_ostream<cT, Traits>& out,
                     kuhllib::uint_t<Rep>            value)
{
    switch (out.flags() & std::ios_base::basefield)
    {
    case std::ios_base::oct:
        return format<8u>(out, value);
        break;
    case std::ios_base::hex:
        return format<16u>(out, value);
    case std::ios_base::dec:
    default:
        return format<10u>(out, value);
        break;

    }
    return out;
}

// ----------------------------------------------------------------------------

template
std::basic_ostream<char, std::char_traits<char> >&
kuhllib::operator<< (std::basic_ostream<char, std::char_traits<char> >&,
                     kuhllib::uint128_t);
template
std::basic_ostream<wchar_t, std::char_traits<wchar_t> >&
kuhllib::operator<< (std::basic_ostream<wchar_t, std::char_traits<wchar_t> >&,
                     kuhllib::uint128_t);
