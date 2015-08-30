// erltest_test.t.cpp                                                 -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2013 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "erltest_test.hpp"
#include <sstream>
#include <iostream>

// ----------------------------------------------------------------------------
// Testing the tests - doing it the easy way and using the tests...

// ----------------------------------------------------------------------------

namespace ET = erl::test;

static erl::test::testcase const tests[] = {
    ET::expect_success("breathing test", []()->bool{ return true; }),
    ET::expect_success("test success", []()->bool{
            ET::testcase test(ET::expect_success("test name",
                                                 []{ return true; }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));
            return rc == true
                && out.str() == " 17: global name                   test name"
                "                               OK\n";
        }),
    ET::expect_success("test failure", []()->bool{
            ET::testcase test(ET::expect_success("test name",
                                                 []{ return false; }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail\n";
        }),
    ET::expect_success("test unexpected exception", []()->bool{
            ET::testcase test(ET::expect_success("test name",
                                                 []()->bool{ throw int(); }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               unexpected exception\n";
        }),
    ET::expect_success("test unexpected success", []()->bool{
            ET::testcase test(ET::expect_failure("test name",
                                                 []{ return true; }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == true
                && out.str() == " 17: global name                   test name"
                "                               OK (unexpected)\n";
        }),
    ET::expect_success("test expected failure", []()->bool{
            ET::testcase test(ET::expect_failure("test name",
                                                 []{ return false; }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (expected)\n";
        }),
    ET::expect_success("test exception instead of failure", []()->bool{
            ET::testcase test(ET::expect_failure("test name",
                                                 []()->bool{ throw int(); }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               exception (fail expected)\n";
        }),

    ET::expect_success("test expected exception", []()->bool{
            ET::testcase test(ET::expect_exception<int>("test name",
                                                        []()->bool{
                                                            throw int();
                                                        }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == true
                && out.str() == " 17: global name                   test name"
                "                               OK (received exception)\n";
        }),
    ET::expect_success("test expected different exception", []()->bool{
            ET::testcase test(ET::expect_exception<int>("test name",
                                                        []()->bool{
                                                            throw bool();
                                                        }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (different exception)\n";
        }),
    ET::expect_success("test success instead of exception", []()->bool{
            ET::testcase test(ET::expect_exception<int>("test name",
                                                        []{ return true; }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (OK instead of exception)"
                "\n";
        }),
    ET::expect_success("test failure instead of exception", []()->bool{
            ET::testcase test(ET::expect_exception<int>("test name",
                                                        []{ return false; }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (instead of exception)\n";
        }),
    ET::expect_success("test ctxt success", []()->bool{
            ET::testcase test(ET::expect_success("test name",
                                                 [](ET::context&){
                                                     return true;
                                                 }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == true
                && out.str() == " 17: global name                   test name"
                "                               OK\n";
        }),
    ET::expect_success("test ctxt failure", []()->bool{
            ET::testcase test(ET::expect_success("test name",
                                                 [](ET::context&){
                                                     return false;
                                                 }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail\n";
        }),
    ET::expect_success("test ctxt unexpected exception", []()->bool{
            ET::testcase test(ET::expect_success("test name",
                                                 [](ET::context&)->bool{
                                                     throw int();
                                                 }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               unexpected exception\n";
        }),
    ET::expect_success("test ctxt unexpected success", []()->bool{
            ET::testcase test(ET::expect_failure("test name",
                                                 [](ET::context&){
                                                     return true;
                                                 }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == true
                && out.str() == " 17: global name                   test name"
                "                               OK (unexpected)\n";
        }),
    ET::expect_success("test ctxt expected failure", []()->bool{
            ET::testcase test(ET::expect_failure("test name",
                                                 [](ET::context&){
                                                     return false;
                                                 }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (expected)\n";
        }),
    ET::expect_success("test ctxt exception instead of failure", []()->bool{
            ET::testcase test(ET::expect_failure("test name",
                                                 [](ET::context&)->bool{
                                                     throw int();
                                                 }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               exception (fail expected)\n";
        }),

    ET::expect_success("test ctxt expected exception", []()->bool{
            ET::testcase test(ET::expect_exception<int>("test name",
                                                        [](ET::context&)->bool{
                                                            throw int();
                                                        }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == true
                && out.str() == " 17: global name                   test name"
                "                               OK (received exception)\n";
        }),
    ET::expect_success("test ctxt expected different exception", []()->bool{
            ET::testcase test(ET::expect_exception<int>("test name",
                                                        [](ET::context&)->bool{
                                                            throw bool();
                                                        }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (different exception)\n";
        }),
    ET::expect_success("test ctxt success instead of exception", []()->bool{
            ET::testcase test(ET::expect_exception<int>("test name",
                                                        [](ET::context&){
                                                            return true;
                                                        }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (OK instead of exception)"
                "\n";
        }),
    ET::expect_success("test ctxt failure instead of exception", []()->bool{
            ET::testcase test(ET::expect_exception<int>("test name",
                                                        [](ET::context&){
                                                            return false;
                                                        }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (instead of exception)\n";
        }),
};

int main(int ac, char* av[])
{
    return erl::test::run_tests("erl::test::run_tests", ac, av, ::tests);
}
