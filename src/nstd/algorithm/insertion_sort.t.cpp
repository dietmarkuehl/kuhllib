// algorithm/insertion_sort.t.cpp                                     -*-C++-*-
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

#include "nstd/algorithm/insertion_sort.hpp"
#include "nstd/algorithm/equal.hpp"
#include "nstd/container/vector.hpp"
#include "nstd/functional/less.hpp"
#include "nstd/iterator/bidirectional_iterator.hpp"
#include "nstd/iterator/multi_pass_iterator.hpp"
#include "nstd/type_traits/decay.hpp"
#include "kuhl/test.hpp"
#include <iostream>

namespace NA  = ::nstd::algorithm;
namespace NC  = ::nstd::container;
namespace NF  = ::nstd::functional;
namespace NI  = ::nstd::iterator;
namespace NP  = ::nstd::projection;
namespace NT  = ::nstd::type_traits;
namespace KT  = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct value {
        unsigned long long val;
        bool operator== (value const& other) const { return this->val == other.val; }
        bool operator<  (value const& other) const { return this->val <  other.val; }
        bool operator!= (value const& other) const { return !(*this == other); }
    };

    value operator""_v (unsigned long long val) {
        return value{val};
    }

    template <typename Sort,
              typename InIt,  typename IEP,
              typename FwdIt, typename FEP>
    bool test_sort_call(KT::context& c, Sort sort, InIt ibeg, IEP iend, FwdIt ebeg, FEP eend) {
        using value_type = NT::decay_t<decltype(*ibeg)>;
        NC::vector<value_type> v(ibeg, iend);
        sort(v.begin(), v.end());
        return NA::equal(v.begin(), v.end(), ebeg, eend);
    }

    template <typename Sort,
              typename InIt,  typename IEP,
              typename FwdIt, typename FEP>
    bool test_sort(KT::context& c, Sort sort, InIt ibeg, IEP iend, FwdIt ebeg, FEP eend) {
        return test_sort_call(c, [sort](auto begin, auto end){
                sort(begin, end);
            }, ibeg, iend, ebeg, eend)
            && test_sort_call(c, [sort](auto begin, auto end){
                    sort(begin, end, NF::less<>());
            }, ibeg, iend, ebeg, eend)
            && test_sort_call(c, [sort](auto begin, auto end){
                    sort(NP::identity, begin, end);
            }, ibeg, iend, ebeg, eend)
            && test_sort_call(c, [sort](auto begin, auto end){
                    sort(NP::identity, begin, end, NF::less<>());
            }, ibeg, iend, ebeg, eend)
            && test_sort_call(c, [sort](auto begin, auto end){
                    sort(NP::identity, NP::identity, begin, end);
            }, ibeg, iend, ebeg, eend)
            && test_sort_call(c, [sort](auto begin, auto end){
                    sort(NP::identity, NP::identity, begin, end, NF::less<>());
            }, ibeg, iend, ebeg, eend)
            ;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("sort empty range", [](KT::context& c)->bool{
            value input[]  = { 0_v };
            value expect[] = { 0_v };
            return test_sort(c, NA::insertion_sort,
                             NI::bidirectional_begin(input),
                             NI::bidirectional_begin(input),
                             NI::multi_pass_begin(expect),
                             NI::multi_pass_begin(expect));
                ;
        }),
    KT::expect_success("sort one element range", [](KT::context& c)->bool{
            value input[]  = { 17_v };
            value expect[] = { 17_v };
            return test_sort(c, NA::insertion_sort,
                             NI::bidirectional_begin(input),
                             NI::bidirectional_end(input),
                             NI::multi_pass_begin(expect),
                             NI::multi_pass_end(expect));
                ;
        }),
    KT::expect_success("sort ordered two element range", [](KT::context& c)->bool{
            value input[]  = { 15_v, 17_v };
            value expect[] = { 15_v, 17_v };
            return test_sort(c, NA::insertion_sort,
                             NI::bidirectional_begin(input),
                             NI::bidirectional_end(input),
                             NI::multi_pass_begin(expect),
                             NI::multi_pass_end(expect));
                ;
        }),
    KT::expect_success("sort unordered two element range", [](KT::context& c)->bool{
            value input[]  = { 17_v, 15_v };
            value expect[] = { 15_v, 17_v };
            return test_sort(c, NA::insertion_sort,
                             NI::bidirectional_begin(input),
                             NI::bidirectional_end(input),
                             NI::multi_pass_begin(expect),
                             NI::multi_pass_end(expect));
                ;
        }),
    KT::expect_success("sort slightly bigger range element range", [](KT::context& c)->bool{
            value input[]  = { 9_v, 1_v, 8_v, 2_v, 7_v, 0_v, 3_v, 6_v, 4_v, 5_v };
            value expect[] = { 0_v, 1_v, 2_v, 3_v, 4_v, 5_v, 6_v, 7_v, 8_v, 9_v };
            return test_sort(c, NA::insertion_sort,
                             NI::bidirectional_begin(input),
                             NI::bidirectional_end(input),
                             NI::multi_pass_begin(expect),
                             NI::multi_pass_end(expect));
                ;
        }),
    KT::expect_success("sort decreasing range", [](KT::context& c)->bool{
            value input[]  = { 9_v, 8_v, 7_v, 6_v, 5_v, 4_v, 3_v, 2_v, 1_v, 0_v };
            value expect[] = { 0_v, 1_v, 2_v, 3_v, 4_v, 5_v, 6_v, 7_v, 8_v, 9_v };
            return test_sort(c, NA::insertion_sort,
                             NI::bidirectional_begin(input),
                             NI::bidirectional_end(input),
                             NI::multi_pass_begin(expect),
                             NI::multi_pass_end(expect));
                ;
        }),
    KT::expect_success("sort increasing range", [](KT::context& c)->bool{
            value input[]  = { 0_v, 1_v, 2_v, 3_v, 4_v, 5_v, 6_v, 7_v, 8_v, 9_v };
            value expect[] = { 0_v, 1_v, 2_v, 3_v, 4_v, 5_v, 6_v, 7_v, 8_v, 9_v };
            return test_sort(c, NA::insertion_sort,
                             NI::bidirectional_begin(input),
                             NI::bidirectional_end(input),
                             NI::multi_pass_begin(expect),
                             NI::multi_pass_end(expect));
                ;
        }),
    KT::expect_failure("placeholder", [](KT::context& c)->bool{
           return true;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("algorithm::insertion_sort()", ac, av, ::tests);
}
