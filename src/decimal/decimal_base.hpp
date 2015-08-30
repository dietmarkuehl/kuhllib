// decimal_base.hpp                                                   -*-C++-*-
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

#ifndef INCLUDED_DECIMAL_BASE
#define INCLUDED_DECIMAL_BASE

// ----------------------------------------------------------------------------

namespace kuhllib
{
    class decimal_base;
}

// ----------------------------------------------------------------------------

class kuhllib::decimal_base
{
public:
    enum classification {
        signaling_nan,
        quiet_nan,
        negative_infinity,
        negative_normal,
        // negative_subnormal: there are no subnormal decimal values
        negative_zero,
        positive_infinity,
        positive_normal,
        // positive_subnormal: there are no subnormal decimal values
        positive_zero,
    };
    enum bid_t { bid }; // construct from binary integer decimal representation
    enum dpd_t { dpd }; // construct from densely packed decimal representation
    enum inf_t { inf }; // construct one of the infinities
    enum nan_t { nan }; // construct a NaN
};

// ----------------------------------------------------------------------------

#endif
