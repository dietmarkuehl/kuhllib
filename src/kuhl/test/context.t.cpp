// kuhl/test/context.t.cpp                                            -*-C++-*-
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

#include "kuhl/test.hpp"
#include "kuhl/mini/string.hpp"

namespace KT = kuhl::test;
namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("default constructed context is empty", [](KT::context& c)->bool{
            KT::context d;
            return d.empty()
                && assert_equal(c, d.c_str(), KM::string());
        }),
    KT::expect_success("context with a message", [](KT::context& c)->bool{
            KT::context d;
            KM::string message1("message1");
            KM::string message2("message2");
            d << message1 << message2;
            return !d.empty()
                && assert_equal(c, d.c_str(), message1 + message2);
        }),
    KT::expect_success("reset context removes prior message", [](KT::context& c)->bool{
            KT::context d;
            KM::string message1("hello, world");
            KM::string message2("goodbye");
            d << message1;
            d.reset();
            d << message2;
            return assert_equal(c, d.c_str(), message2);
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("kuhl::test::context", ac, av, ::tests);
}
