// nstd/algorithm/for_each.t.cpp                                      -*-C++-*-
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

#include "nstd/algorithm/for_each.hpp"
#include "nstd/algorithm/mismatch.hpp"
#include "nstd/cursor/single_pass.hpp"
#include "nstd/cursor/model_single_pass.hpp"
#include "nstd/iterator/forward_iterator.hpp"
#include "nstd/iterator/bidirectional_iterator.hpp"
#include "nstd/projection/identity.hpp"
#include "nstd/projection/model_readable.hpp"
#include "nstd/utility/equality_comparable.hpp"
#include "kuhl/test.hpp"
#include <vector>
#include <cstddef>
#include <iostream>

namespace NA = nstd::algorithm;
namespace NC = nstd::cursor;
namespace NE = nstd::execution;
namespace NI = nstd::iterator;
namespace NP = nstd::projection;
namespace NU = nstd::utility;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct movable {
        std::vector<int> values;

        movable() = default;
        movable(movable&&) = default;
        movable(movable const&) = delete;
        void operator()(NP::model_value<int> value) {
            this->values.push_back(value.get_value());
        }

        template <typename T, std::size_t Size>
        auto check_values(T (&array)[Size]) -> bool {
            auto result = NA::mismatch(NP::identity, values.begin(), values.end(),
                                       NP::model_readable<NU::equality_comparable>(),
                                       NC::single_pass_begin(array), NC::single_pass_end(array));
            return NC::at_same_pos(result.first, values.end())
                && NC::at_same_pos(result.second, NC::single_pass_end(array))
                ;
            return false;
        }
        bool empty() const { return this->values.empty(); }
    };
    struct copyable {
        std::vector<int> values;

        copyable() = default;
        copyable(copyable const&) = default;
        void operator()(NP::model_value<int> value) {
            this->values.push_back(value.get_value());
        }

        template <typename T, std::size_t Size>
        auto check_values(T (&array)[Size]) -> bool {
            auto result = NA::mismatch(NP::identity, values.begin(), values.end(),
                                       NP::model_readable<NU::equality_comparable>(),
                                       NC::single_pass_begin(array), NC::single_pass_end(array));
            return NC::at_same_pos(result.first, values.end())
                && NC::at_same_pos(result.second, NC::single_pass_end(array))
                ;
            return false;
        }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("for_each() on an empty sequence", [](KT::context& c)->bool{
            int array[] = { 0 };
            auto it(NC::single_pass_begin(array));
            NC::step(it);
            auto end(NC::single_pass_end(array));
            return KT::assert_true(c, "initially at end", NC::at_same_pos(it, end))
                && KT::assert_true(c, "iterator is at end",
                                   NC::at_same_pos(NA::for_each(NP::model_readable<>(), it, end, movable()).first,
                                                   end))
                && KT::assert_true(c, "no call registered",
                                   NA::for_each(NP::model_readable<>(), it, end, movable()).second.empty())
                ;
        }),
    KT::expect_success("for_each() function isn't necessarily copyable", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4, 5 };
            return KT::assert_type<NU::pair<NC::model_single_pass<int>, movable>,
                                   decltype(NA::for_each(NP::model_readable<>(),
                                                         NC::single_pass_begin(array), NC::single_pass_end(array),
                                                         movable()))>(c, "return type")
                && KT::assert_true(c, "registered all calls",
                                   NA::for_each(NP::model_readable<>(),
                                                NC::single_pass_begin(array), NC::single_pass_end(array),
                                                movable()).second.check_values(array))
                ;
        }),
    KT::expect_success("for_each() function can be copyable", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4, 5 };
            return KT::assert_type<NU::pair<NC::model_single_pass<int>, copyable>,
                                   decltype(NA::for_each(NP::model_readable<>(),
                                                         NC::single_pass_begin(array), NC::single_pass_end(array),
                                                         copyable()))>(c, "return type")
                && KT::assert_true(c, "registered all calls",
                                   NA::for_each(NP::model_readable<>(),
                                                NC::single_pass_begin(array), NC::single_pass_end(array),
                                                copyable()).second.check_values(array))
                ;
        }),
    KT::expect_success("for_each() with forward iterator", [](KT::context& c)->bool{
            std::vector<int> source({ 1, 2, 3, 4, 5 });
            std::vector<int> target;
            NA::for_each(NI::forward_begin(source), NI::forward_end(source),
                         [&](int value){ target.push_back(value); });
            return KT::assert_true(c, "registered all calls",
                                   source.end() == 
                                       NA::mismatch(NP::identity, source.begin(), source.end(),
                                                    NP::identity, target.begin(), target.end()).first)
                ;
        }),
    KT::expect_success("for_each(seq, ) with forward iterator", [](KT::context& c)->bool{
            std::vector<int> source({ 1, 2, 3, 4, 5 });
            std::vector<int> target;
            NA::for_each(NE::seq, NI::forward_begin(source), NI::forward_end(source),
                         [&](int value){ target.push_back(value); });
            return KT::assert_true(c, "registered all calls",
                                   source.end() == 
                                       NA::mismatch(NP::identity, source.begin(), source.end(),
                                                    NP::identity, target.begin(), target.end()).first)
                ;
        }),
    KT::expect_success("for_each(seq, ) with bidirectional iterator", [](KT::context& c)->bool{
            std::vector<int> source({ 1, 2, 3, 4, 5 });
            std::vector<int> target;
            NA::for_each(NE::seq, NI::bidirectional_begin(source), NI::bidirectional_end(source),
                         [&](int value){ target.push_back(value); });
            return KT::assert_true(c, "registered all calls",
                                   source.end() == 
                                       NA::mismatch(NP::identity, source.begin(), source.end(),
                                                    NP::identity, target.begin(), target.end()).first)
                ;
        }),
    KT::expect_success("for_each(seq, ) with random access iterator", [](KT::context& c)->bool{
            std::vector<int> source({ 1, 2, 3, 4, 5 });
            std::vector<int> target;
            NA::for_each(NE::seq, source.begin(), source.end(),
                         [&](int value){ target.push_back(value); });
            return KT::assert_true(c, "registered all calls",
                                   source.end() == 
                                       NA::mismatch(NP::identity, source.begin(), source.end(),
                                                    NP::identity, target.begin(), target.end()).first)
                ;
        }),
    KT::expect_success("for_each(par, ) with forward iterator", [](KT::context& c)->bool{
            std::vector<int> source({ 1, 2, 3, 4, 5 });
            std::vector<int> target;
            NA::for_each(NE::par, NI::forward_begin(source), NI::forward_end(source),
                         [&](int value){ target.push_back(value); });
            return KT::assert_true(c, "registered all calls",
                                   source.end() == 
                                       NA::mismatch(NP::identity, source.begin(), source.end(),
                                                    NP::identity, target.begin(), target.end()).first)
                ;
        }),
    KT::expect_success("for_each(par, ) with bidirectional iterator", [](KT::context& c)->bool{
            std::vector<int> source({ 1, 2, 3, 4, 5 });
            std::vector<int> target;
            NA::for_each(NE::par, NI::bidirectional_begin(source), NI::bidirectional_end(source),
                         [&](int value){ target.push_back(value); });
            return KT::assert_true(c, "registered all calls",
                                   source.end() == 
                                       NA::mismatch(NP::identity, source.begin(), source.end(),
                                                    NP::identity, target.begin(), target.end()).first)
                ;
        }),
    KT::expect_success("for_each(par, ) with random access iterator", [](KT::context& c)->bool{
            std::vector<int> source({ 1, 2, 3, 4, 5 });
            std::vector<int> target;
            NA::for_each(NE::par, source.begin(), source.end(),
                         [&](int value){ target.push_back(value); });
            return KT::assert_true(c, "registered all calls",
                                   source.end() == 
                                       NA::mismatch(NP::identity, source.begin(), source.end(),
                                                    NP::identity, target.begin(), target.end()).first)
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("algorithm::for_each", ac, av, ::tests);
}
