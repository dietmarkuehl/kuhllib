// nstd/algorithm/sort.t.cpp                                          -*-C++-*-
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

#include "nstd/algorithm/sort.hpp"
#include "nstd/algorithm/mismatch.hpp"
#include "nstd/cursor/array.hpp"
#include "nstd/execution/execution.hpp"
#include "nstd/projection/identity.hpp"
#include "kuhl/test.hpp"

namespace NA = ::nstd::algorithm;
namespace NC = ::nstd::cursor;
namespace NE = ::nstd::execution;
namespace NP = ::nstd::projection;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing/no policy/no compare", [](KT::context& c)->bool{
            int source[] = { 1, 9, 2, 8, 3, 7, 4, 6, 5, 5 };
            int target[] = { 1, 2, 3, 4, 5, 5, 6, 7, 8, 9 };
            NA::sort(NC::begin(source), NC::end(source));
            return NA::mismatch(NP::identity, NC::begin(source), NC::end(source),
                                NP::identity, NC::begin(target), NC::end(target),
                                [](auto a0, auto a1){ return a0 == a1; }).first
                    == NC::end(source);
                ;
        }),
    KT::expect_success("breathing/no policy/compare", [](KT::context& c)->bool{
            int source[] = { 1, 9, 2, 8, 3, 7, 4, 6, 5, 5 };
            int target[] = { 9, 8, 7, 6, 5, 5, 4, 3, 2, 1 };
            NA::sort(NC::begin(source), NC::end(source),
                     [](auto a0, auto a1){ return a0 > a1; });
            return NA::mismatch(NP::identity, NC::begin(source), NC::end(source),
                                NP::identity, NC::begin(target), NC::end(target),
                                [](auto a0, auto a1){ return a0 == a1; }).first
                    == NC::end(source);
                ;
        }),
    KT::expect_success("breathing/seq policy/no compare", [](KT::context& c)->bool{
            int source[] = { 1, 9, 2, 8, 3, 7, 4, 6, 5, 5 };
            int target[] = { 1, 2, 3, 4, 5, 5, 6, 7, 8, 9 };
            NA::sort(NE::seq, NC::begin(source), NC::end(source));
            return NA::mismatch(NP::identity, NC::begin(source), NC::end(source),
                                NP::identity, NC::begin(target), NC::end(target),
                                [](auto a0, auto a1){ return a0 == a1; }).first
                    == NC::end(source);
                ;
        }),
    KT::expect_success("breathing/seq policy/compare", [](KT::context& c)->bool{
            int source[] = { 1, 9, 2, 8, 3, 7, 4, 6, 5, 5 };
            int target[] = { 9, 8, 7, 6, 5, 5, 4, 3, 2, 1 };
            NA::sort(NE::seq, NC::begin(source), NC::end(source),
                     [](auto a0, auto a1){ return a0 > a1; });
            return NA::mismatch(NP::identity, NC::begin(source), NC::end(source),
                                NP::identity, NC::begin(target), NC::end(target),
                                [](auto a0, auto a1){ return a0 == a1; }).first
                    == NC::end(source);
                ;
        }),
    KT::expect_success("breathing/par policy/no compare", [](KT::context& c)->bool{
            int source[] = { 1, 9, 2, 8, 3, 7, 4, 6, 5, 5 };
            int target[] = { 1, 2, 3, 4, 5, 5, 6, 7, 8, 9 };
            NA::sort(NE::par, NC::begin(source), NC::end(source));
            return NA::mismatch(NP::identity, NC::begin(source), NC::end(source),
                                NP::identity, NC::begin(target), NC::end(target),
                                [](auto a0, auto a1){ return a0 == a1; }).first
                    == NC::end(source);
                ;
        }),
    KT::expect_success("breathing/par policy/compare", [](KT::context& c)->bool{
            int source[] = { 1, 9, 2, 8, 3, 7, 4, 6, 5, 5 };
            int target[] = { 9, 8, 7, 6, 5, 5, 4, 3, 2, 1 };
            NA::sort(NE::par, NC::begin(source), NC::end(source),
                     [](auto a0, auto a1){ return a0 > a1; });
            return NA::mismatch(NP::identity, NC::begin(source), NC::end(source),
                                NP::identity, NC::begin(target), NC::end(target),
                                [](auto a0, auto a1){ return a0 == a1; }).first
                    == NC::end(source);
                ;
        }),
    KT::expect_success("breathing/par_unseq policy/no compare", [](KT::context& c)->bool{
            int source[] = { 1, 9, 2, 8, 3, 7, 4, 6, 5, 5 };
            int target[] = { 1, 2, 3, 4, 5, 5, 6, 7, 8, 9 };
            NA::sort(NE::par_unseq, NC::begin(source), NC::end(source));
            return NA::mismatch(NP::identity, NC::begin(source), NC::end(source),
                                NP::identity, NC::begin(target), NC::end(target),
                                [](auto a0, auto a1){ return a0 == a1; }).first
                    == NC::end(source);
                ;
        }),
    KT::expect_success("breathing/par_unseq policy/compare", [](KT::context& c)->bool{
            int source[] = { 1, 9, 2, 8, 3, 7, 4, 6, 5, 5 };
            int target[] = { 9, 8, 7, 6, 5, 5, 4, 3, 2, 1 };
            NA::sort(NE::par_unseq, NC::begin(source), NC::end(source),
                     [](auto a0, auto a1){ return a0 > a1; });
            return NA::mismatch(NP::identity, NC::begin(source), NC::end(source),
                                NP::identity, NC::begin(target), NC::end(target),
                                [](auto a0, auto a1){ return a0 == a1; }).first
                    == NC::end(source);
                ;
        }),
    KT::expect_success("breathing/tbb policy/no compare", [](KT::context& c)->bool{
            int source[] = { 1, 9, 2, 8, 3, 7, 4, 6, 5, 5 };
            int target[] = { 1, 2, 3, 4, 5, 5, 6, 7, 8, 9 };
            NA::sort(NE::tbb, NC::begin(source), NC::end(source));
            return NA::mismatch(NP::identity, NC::begin(source), NC::end(source),
                                NP::identity, NC::begin(target), NC::end(target),
                                [](auto a0, auto a1){ return a0 == a1; }).first
                    == NC::end(source);
                ;
        }),
    KT::expect_success("breathing/tbb policy/compare", [](KT::context& c)->bool{
            int source[] = { 1, 9, 2, 8, 3, 7, 4, 6, 5, 5 };
            int target[] = { 9, 8, 7, 6, 5, 5, 4, 3, 2, 1 };
            NA::sort(NE::tbb, NC::begin(source), NC::end(source),
                     [](auto a0, auto a1){ return a0 > a1; });
            return NA::mismatch(NP::identity, NC::begin(source), NC::end(source),
                                NP::identity, NC::begin(target), NC::end(target),
                                [](auto a0, auto a1){ return a0 == a1; }).first
                    == NC::end(source);
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("algorithm::sort", ac, av, ::tests);
}
