// decimal-literals.t.cpp -*-C++-*-
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

#include "decimal-literals.hpp"
#include "kuhl/test.hpp"
#include <type_traits>
#include <utility>

namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

template <typename Expect, typename Actual>
typename std::enable_if<std::is_same<Expect, Actual>::value, bool>::type
verify(Actual value, bool sign, typename Actual::rep_type significand, int exponent)
{
    if (!(value.negative()    == sign
          && value.significand() == significand
          && value.exponent()    == exponent)) {
        std::cout << "expected=(" << sign << ", " << significand << ", " << exponent << ") "
                  << "received=(" << value.negative() << ", " << value.significand() << ", " << value.exponent() << ")\n";
    }
    return value.negative()    == sign
        && value.significand() == significand
        && value.exponent()    == exponent;
}

template <typename Expect, typename Actual>
typename std::enable_if<!std::is_same<Expect, Actual>::value, bool>::type
verify(Actual, bool, typename Actual::rep_type, int)
{
    return false;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("basic literals", []()->bool{
            using namespace kuhllib::literals;
            return verify<kuhllib::decimal<32>>(123_DF, false, 123, 0)
                && verify<kuhllib::decimal<64>>(123_DD, false, 123, 0)
                //-dk:TODO && verify<kuhllib::decimal<128>>(123_DL, false, 123, 0)
                && verify<kuhllib::decimal<32>>(-123_DF, true, 123, 0)
                && verify<kuhllib::decimal<64>>(-123_DD, true, 123, 0)
                //-dk:TODO && verify<kuhllib::decimal<128>>(-123_DL, true, 123, 0)
                ;
        }),
    KT::expect_success("fractional digit literals", []()->bool{
            using namespace kuhllib::literals;
            return verify<kuhllib::decimal<32>>(123.45_DF, false, 12345, -2)
                && verify<kuhllib::decimal<64>>(123.45_DD, false, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(123.45_DL, false, 12345, -2)
                && verify<kuhllib::decimal<32>>(-123.45_DF, true, 12345, -2)
                && verify<kuhllib::decimal<64>>(-123.45_DD, true, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(-123.45_DL, true, 12345, -2)
                ;
        }),
    KT::expect_success("pos digits, no sign exp literals", []()->bool{
            using namespace kuhllib::literals;
            return verify<kuhllib::decimal<32>>(12.345E1_DF,  false, 12345, -2)
                && verify<kuhllib::decimal<64>>(12.345E1_DD,  false, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(12.345E1_DL,  false, 12345, -2)
                && verify<kuhllib::decimal<32>>(12.345e1_DF,  false, 12345, -2)
                && verify<kuhllib::decimal<64>>(12.345e1_DD,  false, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(12.345e1_DL,  false, 12345, -2)
                ;
        }),

    KT::expect_success("pos digits, pos sign exp literals", []()->bool{
            using namespace kuhllib::literals;
            return verify<kuhllib::decimal<32>>(12.345E+1_DF, false, 12345, -2)
                && verify<kuhllib::decimal<64>>(12.345E+1_DD, false, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(12.345E+1_DL, false, 12345, -2)
                && verify<kuhllib::decimal<32>>(12.345e+1_DF, false, 12345, -2)
                && verify<kuhllib::decimal<64>>(12.345e+1_DD, false, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(12.345e+1_DL, false, 12345, -2)
                ;
        }),

    KT::expect_success("pos digits, neg sign exp literals", []()->bool{
            using namespace kuhllib::literals;
            return verify<kuhllib::decimal<32>>(1234.5E-1_DF, false, 12345, -2)
                && verify<kuhllib::decimal<64>>(1234.5E-1_DD, false, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(1234.5E-1_DL, false, 12345, -2)
                && verify<kuhllib::decimal<32>>(1234.5e-1_DF, false, 12345, -2)
                && verify<kuhllib::decimal<64>>(1234.5e-1_DD, false, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(1234.5e-1_DL, false, 12345, -2)
                ;
        }),
    KT::expect_success("neg digits, no sign exp literals", []()->bool{
            using namespace kuhllib::literals;
            return verify<kuhllib::decimal<32>>(-12.345E1_DF,  true, 12345, -2)
                && verify<kuhllib::decimal<64>>(-12.345E1_DD,  true, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(-12.345E1_DL,  true, 12345, -2)
                && verify<kuhllib::decimal<32>>(-12.345e1_DF,  true, 12345, -2)
                && verify<kuhllib::decimal<64>>(-12.345e1_DD,  true, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(-12.345e1_DL,  true, 12345, -2)
                ;
        }),

    KT::expect_success("neg digits, pos sign exp literals", []()->bool{
            using namespace kuhllib::literals;
            return verify<kuhllib::decimal<32>>(-12.345E+1_DF, true, 12345, -2)
                && verify<kuhllib::decimal<64>>(-12.345E+1_DD, true, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(-12.345E+1_DL, true, 12345, -2)
                && verify<kuhllib::decimal<32>>(-12.345e+1_DF, true, 12345, -2)
                && verify<kuhllib::decimal<64>>(-12.345e+1_DD, true, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(-12.345e+1_DL, true, 12345, -2)
                ;
        }),

    KT::expect_success("neg digits, neg sign exp literals", []()->bool{
            using namespace kuhllib::literals;
            return verify<kuhllib::decimal<32>>(-1234.5E-1_DF, true, 12345, -2)
                && verify<kuhllib::decimal<64>>(-1234.5E-1_DD, true, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(-1234.5E-1_DL, true, 12345, -2)
                && verify<kuhllib::decimal<32>>(-1234.5e-1_DF, true, 12345, -2)
                && verify<kuhllib::decimal<64>>(-1234.5e-1_DD, true, 12345, -2)
                //-dk:TODO && verify<kuhllib::decimal<128>>(-1234.5e-1_DL, true, 12345, -2)
                ;
        }),
    KT::expect_success("max number of digits", []()->bool{
            using namespace kuhllib::literals;
            return verify<kuhllib::decimal<32>>(1234567E0_DF, false, 1234567, 0)
                && verify<kuhllib::decimal<64>>(1234567890123456E0_DD, false, 1234567890123456, 0)
                //-dk:TODO && verify<kuhllib::decimal<128>>(1234567890123456789012345678901234E0_DL, false, 1234567890123456789012345678901234, 0)
                && verify<kuhllib::decimal<32>>(-1234567E0_DF, true, 1234567, 0)
                && verify<kuhllib::decimal<64>>(-1234567890123456E0_DD, true, 1234567890123456, 0)
                //-dk:TODO && verify<kuhllib::decimal<128>>(-1234567890123456789012345678901234E0_DL, true, 1234567890123456789012345678901234, 0)
                ;
        }),
    KT::expect_success("exceded number of digits", []()->bool{
            using namespace kuhllib::literals;
            return verify<kuhllib::decimal<32>>(1234567000E0_DF, false, 1234567, 3)
                && verify<kuhllib::decimal<64>>(1234567890123456000E0_DD, false, 1234567890123456, 3)
                //-dk:TODO && verify<kuhllib::decimal<128>>(1234567890123456789012345678901234000E0_DL, false, 1234567890123456789012345678901234, 3)
                && verify<kuhllib::decimal<32>>(-1234567000E0_DF, true, 1234567, 3)
                && verify<kuhllib::decimal<64>>(-1234567890123456000E0_DD, true, 1234567890123456, 3)
                //-dk:TODO && verify<kuhllib::decimal<128>>(-1234567890123456789012345678901234000E0_DL, true, 1234567890123456789012345678901234, 3)
                ;
        }),
    KT::expect_success("exceded number of digits (round down)", []()->bool{
            using namespace kuhllib::literals;
            return verify<kuhllib::decimal<32>>(1234567499E0_DF, false, 1234567, 3)
                && verify<kuhllib::decimal<64>>(1234567890123456499E0_DD, false, 1234567890123456, 3)
                //-dk:TODO && verify<kuhllib::decimal<128>>(1234567890123456789012345678901234499E0_DL, false, 1234567890123456789012345678901234, 3)
                && verify<kuhllib::decimal<32>>(-1234567499E0_DF, true, 1234567, 3)
                && verify<kuhllib::decimal<64>>(-1234567890123456499E0_DD, true, 1234567890123456, 3)
                //-dk:TODO && verify<kuhllib::decimal<128>>(-1234567890123456789012345678901234499E0_DL, true, 1234567890123456789012345678901234, 3)
                ;
        }),
    KT::expect_success("exceded number of digits (round up)", []()->bool{
            using namespace kuhllib::literals;
            return verify<kuhllib::decimal<32>>(1234567500E0_DF, false, 1234568, 3)
                && verify<kuhllib::decimal<64>>(1234567890123456500E0_DD, false, 1234567890123457, 3)
                //-dk:TODO && verify<kuhllib::decimal<128>>(1234567890123456789012345678901234500E0_DL, false, 1234567890123456789012345678901235, 3)
                && verify<kuhllib::decimal<32>>(-1234567500E0_DF, true, 1234568, 3)
                && verify<kuhllib::decimal<64>>(-1234567890123456500E0_DD, true, 1234567890123457, 3)
                //-dk:TODO && verify<kuhllib::decimal<128>>(-1234567890123456789012345678901234500E0_DL, true, 1234567890123456789012345678901235, 3)
                ;
        }),
    KT::expect_success("exceded number of at upper bound", []()->bool{
            using namespace kuhllib::literals;
            return verify<kuhllib::decimal<32>>(9999999500E0_DF, false, 1000000, 4)
                && verify<kuhllib::decimal<64>>(9999999999999999500E0_DD, false, 1000000000000000, 4)
                && verify<kuhllib::decimal<32>>(-9999999500E0_DF, true, 1000000, 4)
                && verify<kuhllib::decimal<64>>(-9999999999999999500E0_DD, true, 1000000000000000, 4)
                ;
        }),
    //-dk:TODO verify processing of fractional values using too many digits

    KT::expect_failure("placeholder", []()->bool{ return false; }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("decimal literals", ac, av, ::tests);
}
