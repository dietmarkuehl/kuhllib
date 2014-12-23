// nstd/projection/identity.t.cpp                                     -*-C++-*-
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

#include "nstd/projection/identity.hpp"
#include "kuhl/test.hpp"

namespace NP = nstd::projection;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct foo {};
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("identity of a reference", [](KT::context& c)->bool{
            foo object{};
            int value{17};
            NP::identity(value, 19);
            return KT::assert_type<foo&, decltype(NP::identity(object))>(c, "type")
                && KT::assert_equal(c, "same object", &object, &NP::identity(object))
                && KT::assert_equal(c, "value", 19, value)
                ;
        }),
    KT::expect_success("identity of a const reference", [](KT::context& c)->bool{
            foo const object{};
            return KT::assert_type<foo const&, decltype(NP::identity(object))>(c, "type")
                && KT::assert_equal(c, "same object", &object, &NP::identity(object))
                ;
        }),
    KT::expect_success("identity of temporary", [](KT::context& c)->bool{
            foo const object{};
            return KT::assert_type<foo, decltype(NP::identity(foo{}))>(c, "type")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("projection::identity", ac, av, ::tests);
}
