// nstd/type_traits/detected.t.cpp                                    -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/type_traits/detected.hpp"
#include "nstd/type_traits/is_same.hpp"
#include "kuhl/test.hpp"

namespace KT = ::kuhl::test;
namespace NT = ::nstd::type_traits;

// ----------------------------------------------------------------------------

namespace {
    template <typename T>
    using foo_op = decltype(std::declval<T>().foo());

    struct bar {};
    struct foobar {
        bar foo() { return bar(); }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("dummy call of foobar::foo", [](KT::context& c)->bool{
            foobar().foo();
            return true
                ;
        }),
    KT::expect_success("nonesuch", [](KT::context& c)->bool{
            return KT::assert_false(c, "nonesuch isn't void",
                                    NT::is_same_v<void, NT::nonesuch>)
                ;
        }),
    KT::expect_success("detection failure", [](KT::context& c)->bool {
            return KT::assert_false(c, "NT::detect<...>::value",
                                    NT::detect<NT::nonesuch, foo_op, int>::value)
                && KT::assert_false(c, "NT::is_detected<...>::value",
                                    NT::is_detected<foo_op, int>::value)
                && KT::assert_false(c, "NT::is_detected_v<...>",
                                    NT::is_detected_v<foo_op, int>)
                && KT::assert_true(c, "NT::detect<...>::type",
                                   NT::is_same_v<NT::nonesuch,
                                                 typename NT::detect<NT::nonesuch, foo_op, int>::type>)
                && KT::assert_true(c, "NT::detect<...>::type",
                                   NT::is_same_v<NT::nonesuch,
                                                 NT::detect_t<NT::nonesuch, foo_op, int>>)
                ;
        }),
    KT::expect_success("detection success", [](KT::context& c)->bool {
            return KT::assert_true(c, "NT::detect<...>::value",
                                   NT::detect<NT::nonesuch, foo_op, foobar>::value)
                && KT::assert_true(c, "NT::is_detected<...>::value",
                                   NT::is_detected<foo_op, foobar>::value)
                && KT::assert_true(c, "NT::is_detected_v<...>",
                                   NT::is_detected_v<foo_op, foobar>)
                && KT::assert_true(c, "NT::detect<...>::type",
                                    NT::is_same_v<bar,
                                                  typename NT::detect<NT::nonesuch, foo_op, foobar>::type>)
                && KT::assert_true(c, "NT::detect<...>::type",
                                   NT::is_same_v<bar,
                                                 NT::detect_t<NT::nonesuch, foo_op, foobar>>)
                ;
        }),
    KT::expect_failure("placeholder", [](KT::context& c)->bool{
        return false;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("TODO", ac, av, ::tests);
}
