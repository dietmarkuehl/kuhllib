// partition.hpp                                                      -*-C++-*-
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

#ifndef INCLUDED_PARTITION
#define INCLUDED_PARTITION

#include <algorithm>
#include <functional>
#include <iterator>
#include <string>

// ----------------------------------------------------------------------------

namespace quicksort {
    struct std_partition;
    struct simple_partition;
    struct manual_partition;
    struct internal_sentinel_partition;
}

// ----------------------------------------------------------------------------

struct quicksort::std_partition {
    static std::string name() { return "std::partition"; }
    template <typename BiDirIt, typename Pred>
    static BiDirIt run(BiDirIt begin, BiDirIt end, Pred pred) {
        return std::partition(begin, end, pred);
    }
    template <typename BiDirIt, typename Pred>
    BiDirIt operator()(BiDirIt begin, BiDirIt end, Pred pred) {
        return run(begin, end, pred);
    }
};

// ----------------------------------------------------------------------------

struct quicksort::simple_partition {
    static std::string name() { return "simple-partition"; }
    template <typename BiDirIt, typename Pred>
    static BiDirIt run(BiDirIt begin, BiDirIt end, Pred pred) {
        typedef typename std::iterator_traits<BiDirIt>::value_type value_type;
        while (true) {
            begin = std::find_if(begin, end,
                                 [=](value_type const& object){ return !pred(object); });
            end   = std::find_if(std::reverse_iterator<BiDirIt>(end),
                                 std::reverse_iterator<BiDirIt>(begin),
                                 pred).base();
            if (begin == end || begin == --end) {
                break;
            }
            std::iter_swap(begin, end);
            ++begin;
        }
        return begin;
    }
    template <typename BiDirIt, typename Pred>
    BiDirIt operator()(BiDirIt begin, BiDirIt end, Pred pred) {
        return run(begin, end, pred);
    }
};

// ----------------------------------------------------------------------------

struct quicksort::manual_partition {
    static std::string name() { return "manual-partition"; }
    template <typename BiDirIt, typename Pred>
    static BiDirIt run(BiDirIt begin, BiDirIt end, Pred pred) {
        while (true) {
            while (begin != end && pred(*begin)) {
                ++begin;
            }
            while (begin != end && !pred(*--end)) {
            }
            if (begin == end) {
                break;
            }
            std::iter_swap(begin, end);
            ++begin;
        }
        return begin;
    }
    template <typename BiDirIt, typename Pred>
    BiDirIt operator()(BiDirIt begin, BiDirIt end, Pred pred) {
        return run(begin, end, pred);
    }
};

// ----------------------------------------------------------------------------

struct quicksort::internal_sentinel_partition {
    static std::string name() { return "internal-sentinel-partition"; }
    template <typename BiDirIt, typename Pred>
    static BiDirIt run(BiDirIt begin, BiDirIt end, Pred pred) {
        if (begin != end && !pred(*begin)) {
            while (begin != end && !pred(*--end)) {
            }
            if (begin == end) {
                return begin;
            }
            std::iter_swap(begin, end);
            ++begin;
        }
        while (true) {
            while (pred(*begin)) {
                ++begin;
            }
            while (!pred(*--end)) {
            }
            if (end <= begin) {
                break;
            }
            std::iter_swap(begin, end);
            ++begin;
        }
        return begin;
    }
    template <typename BiDirIt, typename Pred>
    BiDirIt operator()(BiDirIt begin, BiDirIt end, Pred pred) {
        return run(begin, end, pred);
    }
};

// ----------------------------------------------------------------------------

#endif
