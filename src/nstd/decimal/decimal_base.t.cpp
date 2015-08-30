// decimal_base.t.cpp                                                 -*-C++-*-
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

#include "decimal_base.hpp"
#include "kuhl/test.hpp"

namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("is empty", []()->bool{
            struct one: kuhllib::decimal_base { char c; };
            return 1u == sizeof(one);
        }),
    KT::expect_success("contains classification", []()->bool{
            constexpr kuhllib::decimal_base::classification sn = kuhllib::decimal_base::signaling_nan;
            constexpr kuhllib::decimal_base::classification qn = kuhllib::decimal_base::quiet_nan;
            constexpr kuhllib::decimal_base::classification ni = kuhllib::decimal_base::negative_infinity;
            constexpr kuhllib::decimal_base::classification nn = kuhllib::decimal_base::negative_normal;
            constexpr kuhllib::decimal_base::classification nz = kuhllib::decimal_base::negative_zero;
            constexpr kuhllib::decimal_base::classification pi = kuhllib::decimal_base::positive_infinity;
            constexpr kuhllib::decimal_base::classification pn = kuhllib::decimal_base::positive_normal;
            constexpr kuhllib::decimal_base::classification pz = kuhllib::decimal_base::positive_zero;
            return sn != qn
                && ni != nn
                && nz != pi
                && pn != pz;
        }),
    KT::expect_success("contains bid_t", []()->bool{
            constexpr kuhllib::decimal_base::bid_t b = kuhllib::decimal_base::bid;
            return kuhllib::decimal_base::bid_t() == b;
        }),
    KT::expect_success("contains dpd_t", []()->bool{
            constexpr kuhllib::decimal_base::dpd_t d = kuhllib::decimal_base::dpd;
            return kuhllib::decimal_base::dpd_t() == d;
        }),
    KT::expect_success("contains inf_t", []()->bool{
            constexpr kuhllib::decimal_base::inf_t i = kuhllib::decimal_base::inf;
            return kuhllib::decimal_base::inf_t() == i;
        }),
    KT::expect_success("contains nan_t", []()->bool{
            constexpr kuhllib::decimal_base::nan_t n = kuhllib::decimal_base::nan;
            return kuhllib::decimal_base::nan_t() == n;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("decimal_base", ac, av, ::tests);
}
