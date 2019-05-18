// inclusive_scan.t.cpp                                               -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2019 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <vector>
#include <optional>

// ----------------------------------------------------------------------------

namespace nstd
{
    template <typename InIt, typename OutIt, typename Op>
    OutIt partial_sum(InIt it, InIt end, OutIt to, Op op)
    {
        if (it == end) {
            return to;
        }
        auto tmp(*it++);
        *to++ = tmp;

        for (; it != end; ++it) {
            tmp = op(tmp, *it);
            *to++ = tmp;
        }
        return to;
    }

    template <typename InIt, typename OutIt, typename Op, typename Value>
    OutIt inclusive_scan(InIt it, InIt end, OutIt to, Op op, Value value)
    {
        for (; it != end; ++it) {
            value = op(value, *it);
            *to++ = value;
        }
        return to;
    }
}

// ----------------------------------------------------------------------------

namespace nstd
{
    template <typename InIt, typename OutIt, typename Op>
    OutIt inclusive_scan(InIt begin, InIt end, OutIt to, Op op)
    {
        using ptrdiff_t = typename std::iterator_traits<InIt>::difference_type;
        constexpr ptrdiff_t chunkSize = 3;
        const     ptrdiff_t size = std::distance(begin, end);
        if (size <= chunkSize) {
            return std::partial_sum(begin, end, to, op);
        }
        const ptrdiff_t chunkCount = (size + chunkSize + 1) / chunkSize;

        std::vector<std::optional<std::decay_t<decltype(op(*begin, *begin))>>> intermediate(chunkCount - 1);
        for (ptrdiff_t i = 0; i < chunkCount - 1u; ++i) {
            intermediate[i] = std::accumulate(begin + chunkSize * i + 1, begin + chunkSize * (i + 1),
                                              begin[chunkSize * i], op);
        }
        std::partial_sum(intermediate.begin(), intermediate.end(), intermediate.begin(),
                         [op](auto& a, auto& b){ return op(*a, *b); });

        std::partial_sum(begin, begin + chunkSize, to, op);
        for (ptrdiff_t i = 1; i < chunkCount - 1; ++i) {
            nstd::inclusive_scan(begin + chunkSize * i, begin + chunkSize * (i + 1),
                                 to + chunkSize * i, op, *intermediate[i - 1]);
        }
        return nstd::inclusive_scan(begin + chunkSize * (chunkCount - 1), end,
                                    to + chunkSize * (chunkCount - 1), op, *intermediate.back());
    }
}

// ----------------------------------------------------------------------------

namespace {
    template <typename Range, typename Algo>
    void test_range(Range const& input, Algo algo) {
        std::vector<int> result1(input.size());
        auto it1 = std::partial_sum(input.begin(), input.end(), result1.begin(), std::plus<>());
        assert(it1 == result1.end());

        std::vector<int> result2(input.size());
        auto it2 = algo(input.begin(), input.end(), result2.begin(), std::plus<>());
        assert(it2 == result2.end());
        assert(result1 == result2);
    }
    template <typename Range>
    void test_range(Range const& input) {
        test_range(input, [](auto begin, auto end, auto to, auto op){
                return nstd::partial_sum(begin, end, to, op);
            });
    }
    template <typename Range>
    void test(Range const& input) {
        test_range(input);
    }
    void test(std::initializer_list<int> input) {
        test_range(input);
    }
}

int main()
{
    test({ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, });
    test({ 1, 2, 3, 4, 5 });
    test({ });
    test({ 1 });
    test({ 1, 2 });
}
