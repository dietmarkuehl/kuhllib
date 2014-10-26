// decimal_num_put.t.cpp -*-C++-*-
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

#include "decimal_num_put.hpp"
#include "decimal-literals.hpp"
#include "erltest_test.hpp"
#include <iterator>
#include <string>
#include <sstream>
#include <iostream>

// ----------------------------------------------------------------------------

template <int Bits>
static bool
test(std::string const& expect, kuhllib::decimal<Bits> const& value)
{
    std::ostringstream out;
    typedef kuhllib::decimal_num_put<char> facet;
    facet const& np(kuhllib::use_facet<facet>(out.getloc()));
    np.put(std::ostreambuf_iterator<char>(out), out, ' ', value);
    if (out.str() != expect) {
        std::cout << "'" << out.str() << "' vs. expected '" << expect << "'\n";
    }
    return out.str() == expect;
}

// ----------------------------------------------------------------------------

static erl::test::testcase const tests[] = {
    erl::test::expect_success("simple output", []()->bool{
            using namespace kuhllib;
            return test("123", 123_DD)
                && test("-123", -123_DD)
                && test("-12E1", -12E+1_DD);
        }),
};

int main(int ac, char* av[])
{
    return erl::test::run_tests("TODO", ac, av, ::tests);
}
