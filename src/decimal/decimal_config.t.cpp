// decimal_config.t.cpp                                               -*-C++-*-
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

#include "decimal_config.hpp"
#include "erltest_test.hpp"
#include <iostream>
#include <iomanip>

// ----------------------------------------------------------------------------

static erl::test::testcase const tests[] = {
    erl::test::expect_success("decimal_config<32> basics", []()->bool{
            return kuhllib::decimal_config<32>::k    ==  32
                && kuhllib::decimal_config<32>::w    ==   6
                && kuhllib::decimal_config<32>::t    ==  20
                && kuhllib::decimal_config<32>::p    ==   7
                && kuhllib::decimal_config<32>::emax ==  96
                && kuhllib::decimal_config<32>::emin == -95
                && kuhllib::decimal_config<32>::bias == 101
                ;
        }),
    erl::test::expect_success("decimal_config<64> basics", []()->bool{
                ;
            return kuhllib::decimal_config<64>::k    ==   64
                && kuhllib::decimal_config<64>::w    ==    8
                && kuhllib::decimal_config<64>::t    ==   50
                && kuhllib::decimal_config<64>::p    ==   16
                && kuhllib::decimal_config<64>::emax ==  384
                && kuhllib::decimal_config<64>::emin == -383
                && kuhllib::decimal_config<64>::bias ==  398
                ;
        }),
    erl::test::expect_success("decimal_config<128> basics", []()->bool{
            return kuhllib::decimal_config<128>::k    ==   128
                && kuhllib::decimal_config<128>::w    ==    12
                && kuhllib::decimal_config<128>::t    ==   110
                && kuhllib::decimal_config<128>::p    ==    34
                && kuhllib::decimal_config<128>::emax ==  6144
                && kuhllib::decimal_config<128>::emin == -6143
                && kuhllib::decimal_config<128>::bias ==  6176
                ;
        }),
};

int main(int ac, char* av[])
{
    return erl::test::run_tests("decimal_config<N>", ac, av, ::tests);
}
