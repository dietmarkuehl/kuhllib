// decimal.t.cpp -*-C++-*-
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

#include "decimal-all.hpp"
#include "erltest_test.hpp"
#include <sstream>

// ----------------------------------------------------------------------------

static erl::test::testcase const tests[] = {
    erl::test::expect_success("basic construction", []()->bool{
            constexpr kuhllib::decimal<64> d1(false, 1, 0);
            constexpr kuhllib::decimal<64> d2(true, 1, 0);
            return d1.negative() == false
                && d1.significand() == 1
                && d1.exponent() == 0
                && d2.negative() == true
                && d2.significand() == 1
                && d2.exponent() == 0;
        }),
    erl::test::expect_success("basic output", []()->bool{
            std::ostringstream out;
            out << kuhllib::decimal<64>(true, 12, 1);
            return out.str() == "-12E1";
        }),
    erl::test::expect_success("decimal<64> literal", []()->bool{
            using namespace kuhllib;
            constexpr decimal<64> value(-12E+0_DD);
            return value== kuhllib::decimal<64>(true, 12, 0);
        }),
};

int main(int ac, char* av[])
{
    return erl::test::run_tests("decimal", ac, av, ::tests);
}
