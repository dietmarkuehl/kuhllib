// kuhl/test/assertions.t.cpp                                         -*-C++-*-
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

#include "kuhl/test/assertions.hpp"
#include "kuhl/test.hpp"
#include "kuhl/mini/string.hpp"

namespace KT = kuhl::test;
namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

namespace {
    struct foo {};
    struct bar { using type = int; };
    struct test_true_type { static constexpr bool value = true; };
    struct test_false_type { static constexpr bool value = false; };

    struct base {};
    struct derived: base {};
    struct other;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("successful static assertion for true: true and no message", [](KT::context& c)->bool{
            KT::context d;
            return assert_true(c, "assertion passed", KT::assert_static_true<test_true_type>(d, "successful assertion"))
                && assert_true(c, "no message", d.empty())
                ;
        }),
    KT::expect_success("failed static assertion for true: missing value", [](KT::context& c)->bool{
            KT::context d;
            return assert_false(c, "assertion failed", KT::assert_static_true<foo>(d, "failed assertion"))
                && assert_true(c, "message", !d.empty())
                && assert_equal(c, "failed assertion message", d.c_str(), KM::string("no value: failed assertion"));
                ;
        }),
    KT::expect_success("failed static assertion for true: wrong value", [](KT::context& c)->bool{
            KT::context d;
            return assert_false(c, "assertion failed", KT::assert_static_true<test_false_type>(d, "failed assertion"))
                && assert_true(c, "message", !d.empty())
                && assert_equal(c, "failed assertion message", d.c_str(), KM::string("wrong value: failed assertion"));
                ;
        }),
    KT::expect_success("successful static assertion for false: true and no message", [](KT::context& c)->bool{
            KT::context d;
            return assert_true(c, "assertion passed", KT::assert_static_false<test_false_type>(d, "successful assertion"))
                && assert_true(c, "no message", d.empty())
                ;
        }),
    KT::expect_success("failed static assertion for false: missing value", [](KT::context& c)->bool{
            KT::context d;
            return assert_false(c, "assertion failed", KT::assert_static_false<foo>(d, "failed assertion"))
                && assert_true(c, "message", !d.empty())
                && assert_equal(c, "failed assertion message", d.c_str(), KM::string("no value: failed assertion"));
                ;
        }),
    KT::expect_success("failed static assertion for false: wrong value", [](KT::context& c)->bool{
            KT::context d;
            return assert_false(c, "assertion failed", KT::assert_static_false<test_true_type>(d, "failed assertion"))
                && assert_true(c, "message", !d.empty())
                && assert_equal(c, "failed assertion message", d.c_str(), KM::string("wrong value: failed assertion"));
                ;
        }),
    KT::expect_success("successful assertion for false: true and no message", [](KT::context& c)->bool{
            KT::context d;
            return assert_true(c, "assertion passed", assert_true(d, "successful assertion", true))
                && assert_true(c, "no message", d.empty())
                ;
        }),
    KT::expect_success("failed assertion for true: false and message", [](KT::context& c)->bool{
            KT::context d;
            return assert_false(c, "assertion failed", assert_true(d, "failed assertion", false))
                && assert_false(c, "the message is non-empty", d.empty())
                && assert_equal(c, "assertion message", d.c_str(), KM::string("failed assertion"));
                ;
        }),
    KT::expect_success("successful assertion for true: true", [](KT::context& c)->bool{
            KT::context d;
            return assert_true(c, "assertion passed", assert_true(d, true))
                && assert_true(c, "no message", d.empty())
                ;
        }),
    KT::expect_success("failed assertion for true: false", [](KT::context& c)->bool{
            KT::context d;
            return assert_false(c, "assertion failed", assert_true(d, false))
                && assert_true(c, "the message is empty", d.empty())
                ;
        }),
    KT::expect_success("successful assertion for false: true and no message", [](KT::context& c)->bool{
            KT::context d;
            return assert_true(c, "assertion passed", assert_false(d, "successful assertion", false))
                && assert_true(c, "no message", d.empty())
                ;
        }),
    KT::expect_success("failed assertion for false: false and message", [](KT::context& c)->bool{
            KT::context d;
            return assert_false(c, "assertion failed", assert_false(d, "failed assertion", true))
                && assert_false(c, "the message is non-empty", d.empty())
                && assert_equal(c, "assertion message", d.c_str(), KM::string("failed assertion"));
                ;
        }),
    KT::expect_success("successful assertion for false: true", [](KT::context& c)->bool{
            KT::context d;
            return assert_true(c, "assertion passed", assert_false(d, false))
                && assert_true(c, "no message", d.empty())
                ;
        }),
    KT::expect_success("failed assertion for false: false", [](KT::context& c)->bool{
            KT::context d;
            return assert_false(c, "assertion failed", assert_false(d, true))
                && assert_true(c, "the message is empty", d.empty())
                ;
        }),
    KT::expect_success("successful equal: true and no message", [](KT::context& c)->bool{
            KT::context d;
            return assert_equal(d, 3, 3)
                && assert_equal(c, "no message", d.c_str(), KM::string())
                ;
        }),
    KT::expect_success("failed equal: false and a message", [](KT::context& c)->bool{
            KT::context d;
            return !assert_equal(d, 2, 3)
                && assert_equal(c, "failed assertion", d.c_str(), KM::string("2 == 3 is false"))
                ;
        }),
    KT::expect_success("successful type assertion: true and no message", [](KT::context& c)->bool{
            KT::context d;
            return assert_true(c, "assertion success", KT::assert_type<int, int>(d, "no message"))
                && assert_true(c, "no message", d.empty())
                ;
        }),
    KT::expect_success("failed type assertion: false and message", [](KT::context& c)->bool{
            KT::context d;
            return assert_true(c, "assertion failed", !KT::assert_type<int&, int>(d, "type check"))
                && assert_false(c, "there is a message", d.empty())
                && assert_equal(c, "message", d.c_str(), KM::string("type check"))
                ;
        }),
    KT::expect_success("nested type exists", [](KT::context& c) -> bool {
            KT::context d;
            return assert_true(c, KT::assert_no_nested_type<int>(d, "int"))
                && assert_true(c, KT::assert_no_nested_type<foo>(d, "foo"))
                && assert_false(c, KT::assert_no_nested_type<bar>(d, "bar"))
                && assert_false(c, "there is a message", d.empty())
                && assert_equal(c, "message", d.c_str(), KM::string("bar"))
                ;

        }),
    KT::expect_success("check for self-derived", [](KT::context& c)-> bool {
            KT::context d;
            return assert_true(c, "base of itself", KT::assert_is_base<base, base>(d, "base is base"))
                && assert_true(c, "no message", d.empty())
                ;
        }),
    KT::expect_success("check for actual derived", [](KT::context& c)-> bool {
            KT::context d;
            return assert_true(c, "base of derived", KT::assert_is_base<base, derived>(d, "base of derived"))
                && assert_true(c, "no message", d.empty())
                ;
        }),
    KT::expect_success("check for reverse", [](KT::context& c)-> bool {
            KT::context d;
            return assert_false(c, "derived of base", KT::assert_is_base<derived, base>(d, "wrong way"))
                && assert_false(c, "there is a message", d.empty())
                ;
        }),
    KT::expect_success("base and unrelated", [](KT::context& c)-> bool {
            KT::context d;
            return assert_false(c, "base of other", KT::assert_is_base<base, other>(d, "unrelated"))
                && assert_false(c, "there is a message", d.empty())
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("kuhl::test::assertions", ac, av, ::tests);
}
