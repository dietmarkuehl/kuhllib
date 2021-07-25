// nstd/concepts/boolean_testable.t.cpp                               -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/concepts/boolean_testable.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD  = test_declarations;
namespace CON = ::nstd::concepts;
namespace KT  = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct no_conv {};

        struct non_const_boolean {
            explicit operator bool();
        };

        struct boolean {
            explicit operator bool() const;
        };

        struct integer {
            operator int() const;
        };

        struct pointer {
            operator pointer const*() const;
        };

        struct member {
            using type = int member::*;
            operator type () const;
        };

        struct member_fun {
            using type = int (member_fun::*)(int);
            operator type () const;
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("bool is boolean_testable", []{
            return CON::boolean_testable<bool>;
        }),
    KT::expect_success("type without a conversion isn't boolean_testable", []{
            return !CON::boolean_testable<TD::no_conv>;
        }),
    KT::expect_success("type with explict, const conversion to bool is boolean_testable", []{
            return CON::boolean_testable<TD::boolean>;
        }),
    KT::expect_success("type with implicit, const conversion to int is boolean_testable", []{
            return CON::boolean_testable<TD::integer>;
        }),
    KT::expect_success("type with explict, non-const conversion to bool isn't boolean_testable", []{
            return !CON::boolean_testable<TD::non_const_boolean>;
        }),
    KT::expect_success("type with implicit, const conversion to pointer type is boolean_testable", []{
            return CON::boolean_testable<TD::pointer>;
        }),
    KT::expect_success("type with implicit, const conversion to pointer-to-member type is boolean_testable", []{
            return CON::boolean_testable<TD::pointer>;
        }),
    KT::expect_success("type with implicit, const conversion to pointer-to-member-fun type is boolean_testable", []{
            return CON::boolean_testable<TD::pointer>;
        }),
};

static KT::add_tests suite("boolean_testable", ::tests);
