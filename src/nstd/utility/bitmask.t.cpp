// nstd/utility/bitmask.t.cpp                                         -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/utility/bitmask.hpp"
#include "kuhl/test.hpp"

namespace NU = ::nstd;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace nstd {
    enum bm: unsigned short {
        a = 0x1,
        b = 0x2,
        c = 0x3
    };
    template <> struct bitmask<bm> { using type = unsigned short; };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("bitmask properties", [](KT::context& c)->bool{
            return KT::assert_bitmask<NU::bm>(c, "bitmask properties")
                ;
        }),
    KT::expect_success("bitmask operation", [](KT::context& c)->bool{
            ::nstd::bm land{::nstd::bm::a};
            ::nstd::bm lor{::nstd::bm::a};
            ::nstd::bm lxor{::nstd::bm::a};
            land &= ::nstd::bm::c;
            lor  |= ::nstd::bm::b;
            lxor ^= ::nstd::bm::c;
            return KT::assert_equal(c, "a & c == a", ::nstd::bm::a & ::nstd::bm::c, ::nstd::bm::a)
                && KT::assert_equal(c, "a | b == c", ::nstd::bm::a | ::nstd::bm::b, ::nstd::bm::c)
                && KT::assert_equal(c, "a ^ c == b", ::nstd::bm::a ^ ::nstd::bm::c, ::nstd::bm::b)
                && KT::assert_equal(c, "~a & a == bm()", ~::nstd::bm::a & ::nstd::bm::a, ::nstd::bm())
                && KT::assert_equal(c, "land == a", land, ::nstd::bm::a)
                && KT::assert_equal(c, "lor  == c", lor,  ::nstd::bm::c)
                && KT::assert_equal(c, "lxor == b", lxor, ::nstd::bm::b)
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("utility/bitmask", ac, av, ::tests);
}
