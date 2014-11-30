// decimal_num_put.hpp                                                -*-C++-*-
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

#ifndef INCLUDED_DECIMAL_NUM_PUT
#define INCLUDED_DECIMAL_NUM_PUT

#include "decimal.hpp"
#include <algorithm>
#include <locale>
#include <iterator>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace kuhllib
{
    template <typename cT, typename OutIt = std::ostreambuf_iterator<cT>>
    class decimal_num_put;

    template <typename T>
    T const& use_facet(std::locale const& loc);
    template <>
    decimal_num_put<char> const& use_facet<decimal_num_put<char> >(std::locale const&);
    template <>
    decimal_num_put<wchar_t> const& use_facet<decimal_num_put<wchar_t> >(std::locale const&);
}

// ----------------------------------------------------------------------------

template <typename cT, typename OutIt>
class kuhllib::decimal_num_put
    : public std::locale::facet
{
public:
    typedef cT    char_type;
    typedef OutIt iter_type;

    explicit decimal_num_put(std::size_t refs = 0);

    iter_type put(iter_type it, std::ios_base& fmt, char_type fill, kuhllib::decimal<32>  val) const;
    iter_type put(iter_type it, std::ios_base& fmt, char_type fill, kuhllib::decimal<64>  val) const;
    iter_type put(iter_type it, std::ios_base& fmt, char_type fill, kuhllib::decimal<128> val) const;

    static std::locale::id id;

protected:
    iter_type do_put(iter_type it, std::ios_base& fmt, char_type fill, kuhllib::decimal<32>  val) const;
    iter_type do_put(iter_type it, std::ios_base& fmt, char_type fill, kuhllib::decimal<64>  val) const;
    iter_type do_put(iter_type it, std::ios_base& fmt, char_type fill, kuhllib::decimal<128> val) const;

private:
    template <int Bits>
    iter_type format(iter_type it, std::ios_base& fmt, char_type fill, kuhllib::decimal<Bits> val) const;
    template <typename Integer>
    char* format(char* end, Integer val) const;
};

// ----------------------------------------------------------------------------

template <typename cT, typename OutIt>
std::locale::id kuhllib::decimal_num_put<cT, OutIt>::id;

template <typename T>
T const& kuhllib::use_facet(std::locale const& loc)
{
    return std::use_facet<T>(loc);
}

// ----------------------------------------------------------------------------

template <typename cT, typename OutIt>
kuhllib::decimal_num_put<cT, OutIt>::decimal_num_put(std::size_t refs)
    : std::locale::facet(refs)
{
}

// ----------------------------------------------------------------------------

template <typename cT, typename OutIt>
typename kuhllib::decimal_num_put<cT, OutIt>::iter_type
kuhllib::decimal_num_put<cT, OutIt>::put(typename kuhllib::decimal_num_put<cT, OutIt>::iter_type it,
                                         std::ios_base&                                          fmt,
                                         typename kuhllib::decimal_num_put<cT, OutIt>::char_type fill,
                                         kuhllib::decimal<32>                                    val) const
{
    return this->do_put(it, fmt, fill, val);
}

template <typename cT, typename OutIt>
typename kuhllib::decimal_num_put<cT, OutIt>::iter_type
kuhllib::decimal_num_put<cT, OutIt>::put(typename kuhllib::decimal_num_put<cT, OutIt>::iter_type it,
                                         std::ios_base&                                          fmt,
                                         typename kuhllib::decimal_num_put<cT, OutIt>::char_type fill,
                                         kuhllib::decimal<64>                                    val) const
{
    return this->do_put(it, fmt, fill, val);
}

template <typename cT, typename OutIt>
typename kuhllib::decimal_num_put<cT, OutIt>::iter_type
kuhllib::decimal_num_put<cT, OutIt>::put(typename kuhllib::decimal_num_put<cT, OutIt>::iter_type it,
                                         std::ios_base&                                          fmt,
                                         typename kuhllib::decimal_num_put<cT, OutIt>::char_type fill,
                                         kuhllib::decimal<128>                                   val) const
{
    return this->do_put(it, fmt, fill, val);
}

// ----------------------------------------------------------------------------

template <typename cT, typename OutIt>
typename kuhllib::decimal_num_put<cT, OutIt>::iter_type
kuhllib::decimal_num_put<cT, OutIt>::do_put(typename kuhllib::decimal_num_put<cT, OutIt>::iter_type it,
                                            std::ios_base&                                          fmt,
                                            typename kuhllib::decimal_num_put<cT, OutIt>::char_type fill,
                                            kuhllib::decimal<32>                                    val) const
{
    return this->format(it, fmt, fill, val);
}

template <typename cT, typename OutIt>
typename kuhllib::decimal_num_put<cT, OutIt>::iter_type
kuhllib::decimal_num_put<cT, OutIt>::do_put(typename kuhllib::decimal_num_put<cT, OutIt>::iter_type it,
                                            std::ios_base&                                          fmt,
                                            typename kuhllib::decimal_num_put<cT, OutIt>::char_type fill,
                                            kuhllib::decimal<64>                                    val) const
{
    return this->format(it, fmt, fill, val);
}

template <typename cT, typename OutIt>
typename kuhllib::decimal_num_put<cT, OutIt>::iter_type
kuhllib::decimal_num_put<cT, OutIt>::do_put(typename kuhllib::decimal_num_put<cT, OutIt>::iter_type it,
                                            std::ios_base&                                          ,
                                            typename kuhllib::decimal_num_put<cT, OutIt>::char_type ,
                                            kuhllib::decimal<128>                                   ) const
{
    //-dk:TODO format decimal<128>: return this->format(it, fmt, fill, val);
    return it;
}

// ----------------------------------------------------------------------------

template <typename cT, typename OutIt>
    template <typename Integer>
char*
kuhllib::decimal_num_put<cT, OutIt>::format(char*   end,
                                            Integer val) const
{
    char* it(end);
    do {
        //-dk:TODO actually, the formatting needs to be done in terms of Int!
        //-dk:TODO probably, the dividend and the remainder should be computed in one go
        unsigned char c(static_cast<unsigned long long>(val % 10));
        val /= 10;
        *--it = "0123456789"[c];
    } while (val);
    return it;
}

// ----------------------------------------------------------------------------

template <typename cT, typename OutIt>
    template <int Bits>
typename kuhllib::decimal_num_put<cT, OutIt>::iter_type
kuhllib::decimal_num_put<cT, OutIt>::format(typename kuhllib::decimal_num_put<cT, OutIt>::iter_type it,
                                            std::ios_base&                                          fmt,
                                            typename kuhllib::decimal_num_put<cT, OutIt>::char_type ,
                                            kuhllib::decimal<Bits>                                  val) const
{
    //-dk:TODO format: special values (NaN, +/-inf)
    //-dk:TODO format: proper use of precision
    //-dk:TODO format: scientific/fixed/...
    //-dk:TODO format: deal with proper padding

    std::ctype<cT> const& ctype(std::use_facet<std::ctype<cT>>(fmt.getloc()));
    char                  buffer[std::numeric_limits<typename kuhllib::decimal<Bits>::rep_type>::digits10];
    char*                 start(this->format(std::end(buffer), val.significand()));
    unsigned int          digits(std::end(buffer) - start);

    if (val.negative()) {
        *it++ = ctype.widen('-');
    }
    else if (fmt.flags() & std::ios_base::showpos) {
        *it++ = ctype.widen('+');
    }
    if (0 <= val.exponent()) {
        it = std::transform(start, std::end(buffer), it, [&](char c){ return ctype.widen(c); });
        it = std::generate_n(it, val.exponent(), [&](){ return ctype.widen('0'); });
    }
    else {
        unsigned int offset(-val.exponent());
        if (offset < digits) {
            it = std::transform(start, start + (digits - offset), it, [&](char c){ return ctype.widen(c); }); 
            *it++ = ctype.widen('.');
            it = std::transform(start + (digits - offset), std::end(buffer), it, [&](char c){ return ctype.widen(c); }); 
        }
    }
    // if (val.exponent()) {
    //     *it++ = ctype.widen('E');
    //     char ebuffer[10]; //-dk:TODO use proper value
    //     char* estart(format(std::end(ebuffer), val.exponent()));
    // }
    return it;
}

// ----------------------------------------------------------------------------
// The versions for the standard streams are externally instantiated

extern template class kuhllib::decimal_num_put<char, std::ostreambuf_iterator<char>>;
extern template class kuhllib::decimal_num_put<wchar_t, std::ostreambuf_iterator<wchar_t>>;

// ----------------------------------------------------------------------------

#endif
