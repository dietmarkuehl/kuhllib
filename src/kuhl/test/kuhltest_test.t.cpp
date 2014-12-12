// kuhltest_test.t.cpp                                                -*-C++-*-
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

#include "kuhltest_test.hpp"
#include <sstream>
#include <iostream>

// ----------------------------------------------------------------------------
// Testing the tests - doing it the easy way and using the tests...

// ----------------------------------------------------------------------------

namespace KT = kuhl::test;

static kuhl::test::testcase const tests[] = {
    KT::expect_success("breathing test", []()->bool{ return true; }),
    KT::expect_success("test success", []()->bool{
            KT::testcase test(KT::expect_success("test name",
                                                 []{ return true; }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));
            return rc == true
                && out.str() == " 17: global name                   test name"
                "                               OK\n";
        }),
    KT::expect_success("test failure", []()->bool{
            KT::testcase test(KT::expect_success("test name",
                                                 []{ return false; }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail\n";
        }),
    KT::expect_success("test unexpected exception", []()->bool{
            KT::testcase test(KT::expect_success("test name",
                                                 []()->bool{ throw int(); }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               unexpected exception\n";
        }),
    KT::expect_success("test unexpected success", []()->bool{
            KT::testcase test(KT::expect_failure("test name",
                                                 []{ return true; }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == true
                && out.str() == " 17: global name                   test name"
                "                               OK (unexpected)\n";
        }),
    KT::expect_success("test expected failure", []()->bool{
            KT::testcase test(KT::expect_failure("test name",
                                                 []{ return false; }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (expected)\n";
        }),
    KT::expect_success("test exception instead of failure", []()->bool{
            KT::testcase test(KT::expect_failure("test name",
                                                 []()->bool{ throw int(); }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               exception (fail expected)\n";
        }),

    KT::expect_success("test expected exception", []()->bool{
            KT::testcase test(KT::expect_exception<int>("test name",
                                                        []()->bool{
                                                            throw int();
                                                        }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == true
                && out.str() == " 17: global name                   test name"
                "                               OK (received exception)\n";
        }),
    KT::expect_success("test expected different exception", []()->bool{
            KT::testcase test(KT::expect_exception<int>("test name",
                                                        []()->bool{
                                                            throw bool();
                                                        }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (different exception)\n";
        }),
    KT::expect_success("test success instead of exception", []()->bool{
            KT::testcase test(KT::expect_exception<int>("test name",
                                                        []{ return true; }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (OK instead of exception)"
                "\n";
        }),
    KT::expect_success("test failure instead of exception", []()->bool{
            KT::testcase test(KT::expect_exception<int>("test name",
                                                        []{ return false; }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (instead of exception)\n";
        }),
    KT::expect_success("test ctxt success", []()->bool{
            KT::testcase test(KT::expect_success("test name",
                                                 [](KT::context&){
                                                     return true;
                                                 }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == true
                && out.str() == " 17: global name                   test name"
                "                               OK\n";
        }),
    KT::expect_success("test ctxt failure", []()->bool{
            KT::testcase test(KT::expect_success("test name",
                                                 [](KT::context&){
                                                     return false;
                                                 }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail\n";
        }),
    KT::expect_success("test ctxt unexpected exception", []()->bool{
            KT::testcase test(KT::expect_success("test name",
                                                 [](KT::context&)->bool{
                                                     throw int();
                                                 }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               unexpected exception\n";
        }),
    KT::expect_success("test ctxt unexpected success", []()->bool{
            KT::testcase test(KT::expect_failure("test name",
                                                 [](KT::context&){
                                                     return true;
                                                 }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == true
                && out.str() == " 17: global name                   test name"
                "                               OK (unexpected)\n";
        }),
    KT::expect_success("test ctxt expected failure", []()->bool{
            KT::testcase test(KT::expect_failure("test name",
                                                 [](KT::context&){
                                                     return false;
                                                 }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (expected)\n";
        }),
    KT::expect_success("test ctxt exception instead of failure", []()->bool{
            KT::testcase test(KT::expect_failure("test name",
                                                 [](KT::context&)->bool{
                                                     throw int();
                                                 }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               exception (fail expected)\n";
        }),

    KT::expect_success("test ctxt expected exception", []()->bool{
            KT::testcase test(KT::expect_exception<int>("test name",
                                                        [](KT::context&)->bool{
                                                            throw int();
                                                        }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == true
                && out.str() == " 17: global name                   test name"
                "                               OK (received exception)\n";
        }),
    KT::expect_success("test ctxt expected different exception", []()->bool{
            KT::testcase test(KT::expect_exception<int>("test name",
                                                        [](KT::context&)->bool{
                                                            throw bool();
                                                        }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (different exception)\n";
        }),
    KT::expect_success("test ctxt success instead of exception", []()->bool{
            KT::testcase test(KT::expect_exception<int>("test name",
                                                        [](KT::context&){
                                                            return true;
                                                        }));
            std::ostringstream out;
            bool rc(test.run(out, 17, "global name"));

            return rc == false
                && out.str() == " 17: global name                   test name"
                "                               fail (OK instead of exception)"
                "\n";
        }),
    KT::expect_success("test ctxt failure instead of exception", []()->bool{
            KT::testcase test(KT::expect_exception<int>("test name",
                                                        [](KT::context&){
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
    return kuhl::test::run_tests("kuhl::test::run_tests", ac, av, ::tests);
}
