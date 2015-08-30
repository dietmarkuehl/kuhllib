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

#include "decimal-class.hpp"
#include <iosfwd>
#include <iostream>
#include <iomanip>

// ----------------------------------------------------------------------------

namespace kuhllib
{
    template <int Bits>
    bool operator== (decimal<Bits> const&, decimal<Bits> const&);

    template <typename cT, typename Tr, int Bits>
    std::basic_ostream<cT, Tr>& operator<< (std::basic_ostream<cT, Tr>& out,
                                            decimal<Bits> const& value);
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
