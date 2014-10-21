// decimal-class.t.cpp                                                -*-C++-*-
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

#include "decimal-class.hpp"
#include "erltest_test.hpp"
#include <iostream>

// ----------------------------------------------------------------------------

namespace {
    template <int Bits>
    bool test_default() {
        // Test the [defaulted] default constructor:
        // - verify that a default constructed object can be created
        // - verify that a default constructed object can be a constexpr
        // - verify that a default constructed object is a specific positive zero
        kuhllib::decimal<Bits> du;
        erl::test::use(du);

        constexpr kuhllib::decimal<Bits> d0{};
        return d0.negative() == false
            && d0.significand() == typename kuhllib::decimal<Bits>::rep_type()
            && d0.exponent() == -kuhllib::decimal_config<Bits>::bias
            && d0.classify() == kuhllib::decimal<Bits>::positive_zero
            ;
    }
}

// ----------------------------------------------------------------------------

namespace {
    template <int Bits>
    kuhllib::decimal<Bits>
    make_bid(typename kuhllib::decimal<Bits>::rep_type rep) {
        return kuhllib::decimal<Bits>(kuhllib::decimal<Bits>::bid, rep);
    }

    template <int Bits>
    bool verify_parts(kuhllib::decimal<Bits>                    value,
                      bool                                      sign,
                      typename kuhllib::decimal<Bits>::rep_type significand,
                      int                                       exponent) {
        if (!(value.negative()    == sign
              && value.significand() == significand
              && value.exponent()    == exponent)) {
            std::cout << "sign=" << value.negative() << " sig=" << value.significand() << " exp=" << value.exponent() << '\n';
        }
        return value.negative()    == sign
            && value.significand() == significand
            && value.exponent()    == exponent;
    }
}

// ----------------------------------------------------------------------------

static erl::test::testcase const tests[] = {
    erl::test::expect_success("default ctor", []()->bool{
            return test_default<32>()
                && test_default<64>()
                && test_default<128>()
                ;
        }),
    erl::test::expect_success("BID ctor", []()->bool{
            using namespace kuhllib;
            constexpr kuhllib::decimal<32>  d32 (kuhllib::decimal<32>::bid,  0x318004D2u);
            constexpr kuhllib::decimal<64>  d64 (kuhllib::decimal<64>::bid,  0x31800000000004D2ull);
            constexpr kuhllib::decimal<128> d128(kuhllib::decimal<128>::bid, 0x31800000000004D2_uLL);

            return verify_parts(make_bid<32>(0x00000000u), false, 0, -101)
                && verify_parts(make_bid<32>(0x318004D2u), false, 1234, -2)
                && verify_parts(make_bid<32>(0xB18004D2u), true, 1234, -2)
                && verify_parts(make_bid<64>(0x0000000000000000ull), false, 0, -398)
                && verify_parts(make_bid<64>(0x31800000000004D2ull), false, 1234, -2)
                && verify_parts(make_bid<64>(0xB1800000000004D2ull), true, 1234, -2)
                ;
        }),
    erl::test::expect_failure("placeholder", []()->bool{ return false; }),
};

int main(int ac, char* av[])
{
    return erl::test::run_tests("decimal class template", ac, av, ::tests);
}
