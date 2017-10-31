// algorithm/equal.t.cpp                                              -*-C++-*-
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

#include "nstd/algorithm/equal.hpp"
#include "kuhl/test.hpp"
#include <initializer_list>

namespace NA = ::nstd::algorithm;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct value {
        unsigned long long val;
    };
    bool operator== (value const& v0, value const& v1) {
        return v0.val == v1.val;
    }
    bool operator!= (value const& v0, value const& v1) {
        return !(v0 == v1);
    }
    value operator""_v (unsigned long long v) {
        return value{v};
    }
}

// ----------------------------------------------------------------------------

template <typename Range1, typename Range2>
bool test_equal(Range1&& r1, Range2&& r2, bool open, bool closed) {
    return (r2.size() < r1.size() || open   == NA::equal(r1.begin(), r1.end(), r2.begin()))
        && closed == NA::equal(r1.begin(), r1.end(), r2.begin(), r2.end())
        //-dk:TODO add other overloads
        ;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("value basics", [](KT::context& c)->bool {
            return  (1_v == 1_v)
                && !(1_v == 2_v)
                && !(1_v != 1_v)
                &&  (1_v != 2_v)
                ;
        }),
    KT::expect_success("empty ranges", [](KT::context& c)->bool {
            return test_equal(std::initializer_list<value>{},
                              std::initializer_list<value>{},
                              true, true)
                ;
        }),
    KT::expect_success("empty vs. non-mepty range", [](KT::context& c)->bool {
            return test_equal(std::initializer_list<value>{},
                              std::initializer_list<value>{ 1_v },
                              true, false)
                ;
        }),
    KT::expect_success("non-empty equal ranges", [](KT::context& c)->bool {
            return test_equal(std::initializer_list<value>{ 1_v, 2_v, 3_v, 4_v },
                              std::initializer_list<value>{ 1_v, 2_v, 3_v, 4_v },
                              true, true);
        }),
    KT::expect_success("first element differs", [](KT::context& c)->bool {
            return test_equal(std::initializer_list<value>{ 1_v, 2_v, 3_v, 4_v },
                              std::initializer_list<value>{ 5_v, 2_v, 3_v, 4_v },
                              false, false);
        }),
    KT::expect_success("middle element differs", [](KT::context& c)->bool {
            return test_equal(std::initializer_list<value>{ 1_v, 2_v, 5_v, 4_v, 5_v },
                              std::initializer_list<value>{ 1_v, 2_v, 3_v, 4_v, 5_v },
                              false, false);
        }),
    KT::expect_success("last element differs", [](KT::context& c)->bool {
            return test_equal(std::initializer_list<value>{ 1_v, 2_v, 3_v, 4_v },
                              std::initializer_list<value>{ 1_v, 2_v, 3_v, 5_v },
                              false, false);
        }),
    KT::expect_success("longer second sequence", [](KT::context& c)->bool {
            return test_equal(std::initializer_list<value>{ 1_v, 2_v, 3_v, 4_v },
                              std::initializer_list<value>{ 1_v, 2_v, 3_v, 4_v, 5_v },
                              true, false);
        }),
    KT::expect_success("longer first sequence", [](KT::context& c)->bool {
            return test_equal(std::initializer_list<value>{ 1_v, 2_v, 3_v, 4_v, 5_v },
                              std::initializer_list<value>{ 1_v, 2_v, 3_v, 4_v },
                              true, false);
        }),
    KT::expect_failure("placeholder", [](KT::context& c)->bool{
           return true;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("equal", ac, av, ::tests);
}
