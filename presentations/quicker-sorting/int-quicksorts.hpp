// int-quicksorts.hpp                                                 -*-C++-*-
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

#ifndef INCLUDED_INT_QUICKSORTS
#define INCLUDED_INT_QUICKSORTS

#include "median.h"
#include <algorithm>
#include <utility>
#include <cassert>
#include <iostream>
#include <string>

// ----------------------------------------------------------------------------

namespace pivots
{
    struct begin {
        static std::string name() { return "begin"; }
        template <typename Iterator>
        Iterator operator()(Iterator begin, Iterator) const {
            return begin;
        }
        template <typename Iterator>
        static inline Iterator get(Iterator begin, Iterator) {
            return begin;
        }
    };
    struct end {
        static std::string name() { return "end"; }
        template <typename Iterator>
        Iterator operator()(Iterator, Iterator end) const {
            return --end;
        }
        template <typename Iterator>
        static inline Iterator get(Iterator, Iterator end) {
            return --end;
        }
    };
    struct mid {
        static std::string name() { return "mid"; }
        template <typename Iterator>
        Iterator operator()(Iterator begin, Iterator end) const {
            return begin + std::distance(begin, end) / 2;
        }
        template <typename Iterator>
        static inline Iterator get(Iterator begin, Iterator end) {
            return begin + std::distance(begin, end) / 2;
        }
    };
    struct median {
        static std::string name() { return "median"; }
        template <typename Iterator>
        Iterator operator()(Iterator begin, Iterator end) const {
            return quicksort::find_median(begin, begin + std::distance(begin, end) / 2, end - 1);
        }
        template <typename Iterator>
        static inline Iterator get(Iterator begin, Iterator end) {
            return quicksort::find_median(begin, begin + std::distance(begin, end) / 2, end - 1);
        }
    };
}

// ----------------------------------------------------------------------------

namespace fallback
{
    struct none {
        static std::string name() { return "none"; }

        template <typename I>
        static inline constexpr int init(I, I) { return 0; }
        template <typename I>
        static inline constexpr bool run(I, I, int, int) { return false; }

        template <typename I>
        constexpr int operator()(I, I) const { return 0; }
        template <typename I>
        constexpr bool operator()(I, I, int, int) const { return false; }
    };

    template <typename Sort>
    struct intro {
        static std::string name() { return "intro<" + Sort::name() + ">"; }
        template <typename I>
        static inline int init(I begin, I end) {
            std::size_t size(std::distance(begin, end));
            int max(0);
            while (size >>=1) {
                ++max;
            }
            return 2 * max;
        }
        template <typename I>
        static inline bool run(I begin, I end, int depth, int max) {
            if (depth < max) {
                return false;
            }
            Sort::run(begin, end);
            return true;
        }
        
        template <typename I>
        int operator()(I begin, I end) const {
            return init(begin, end);
        }
        template <typename I>
        bool operator()(I begin, I end, int depth, int max) const {
            return run(begin, end, depth, max);
        }
    };
}

// ----------------------------------------------------------------------------

namespace post
{
    struct none
    {
        static std::string name() { return "none"; }
        template <typename I>
        static inline bool done(I begin, I end) { return std::distance(begin, end) < 2; }
        template <typename I>
        constexpr void operator()(I, I) {}
        template <typename I>
        static inline void run(I, I) {}
    };

    template <int Size>
    struct insertion
    {
        static std::string name() { return "insertion<" + std::to_string(Size) + ">"; }
        template <typename I>
        static inline bool done(I begin, I end) { return std::distance(begin, end) < Size; }
        template <typename I>
        static inline void run(I begin, I end) {
            if (begin != end) {
                for (I it = begin; ++it != end; ) {
                    for (I cur = it; begin != cur && *cur < *(cur - 1); --cur) {
                        std::iter_swap(cur, cur - 1);
                    }
                }
            }
        }
        template <typename I>
        void operator()(I begin, I end) const {
            run(begin, end);
        }
    };

    template <int Size>
    struct move_insertion
    {
        static std::string name() { return "move_insertion<" + std::to_string(Size) + ">"; }
        template <typename I>
        static inline bool done(I begin, I end) { return std::distance(begin, end) < Size; }
        template <typename I>
        static inline void run(I begin, I end) {
            if (begin != end) {
                for (I it = begin; ++it != end; ) {
                    if (*it < *(it - 1)) {
                        I cur = it;
                        auto tmp = *cur;
                        *cur = std::move(*(cur - 1));
                        while (begin != --cur && tmp < *(cur - 1)) {
                            *cur = std::move(*(cur - 1));
                        }
                        *cur = tmp;
                    }
                }
            }
        }
        template <typename I>
        void operator()(I begin, I end) const {
            run(begin, end);
        }
    };

    template <int Size>
    struct direct_move_insertion
    {
        static std::string name() { return "direct_move_insertion<" + std::to_string(Size) + ">"; }
        template <typename I>
        static inline bool done(I begin, I end) {
            if (std::distance(begin, end) < Size) {
                if (begin != end) {
                    for (I it = begin; ++it != end; ) {
                        if (*it < *(it - 1)) {
                            I cur = it;
                            auto tmp = *cur;
                            *cur = std::move(*(cur - 1));
                            while (begin != --cur && tmp < *(cur - 1)) {
                                *cur = std::move(*(cur - 1));
                            }
                            *cur = tmp;
                        }
                    }
                }
                return true;
            }
            return false;
        }
        template <typename I>
        static inline void run(I, I) {}
        template <typename I>
        void operator()(I begin, I end) const {
            run(begin, end);
        }
    };

    template <int Size>
    struct fancy_move_insertion
    {
        template <typename I>
        static inline void sort3(I p0, I p1, I p2) {
            if (*p1 < *p0) {
                if (*p2 < *p1) {
                    // p2 < p1 && p1 < p0
                    std::iter_swap(p0, p2);
                }
                else if (*p2 < *p0) {
                    // p1 <= p2 && p2 < p0
                    auto tmp = std::move(*p0);
                    *p0 = std::move(*p1);
                    *p1 = std::move(*p2);
                    *p2 = std::move(tmp);
                }
                else {
                    // p1 < p0 && p0 <= p2
                    std::iter_swap(p0, p1);
                }
                
            }
            else {
                if (*p2 < *p0) {
                    // p2 < p0 &&  < p0 <= p1
                    auto tmp = std::move(*p2);
                    *p2 = std::move(*p1);
                    *p1 = std::move(*p0);
                    *p0 = std::move(tmp);
                }
                else if (*p2 < *p1) {
                    // p0 <= p2 && p2 < p1
                    std::iter_swap(p1, p2);
                }
                else {
                    // p0 <= p1 <= p2
                }
            }
        }
        template <typename I>
        static inline void sort4(I p0, I p1, I p2, I p3) {
            sort3(p0, p1, p2);
            if (*p3 < *p2) {
                if (*p3 < *p1) {
                    auto tmp = std::move(*p3);
                    *p3 = std::move(*p2);
                    *p2 = std::move(*p1);
                    if (tmp < *p0) {
                        *p1 = std::move(*p0);
                        *p0 = std::move(tmp);
                    }
                    else {
                        *p1 = std::move(tmp);
                    }
                }
                else {
                    std::iter_swap(p2, p3);
                }
            }
        }
        template <typename I>
            static inline void sort5(I p0, I p1, I p2, I p3, I p4) {
            sort4(p0, p1, p2, p3);
            if (*p4 < *p3) {
                if (*p4 < *p2) {
                    auto tmp = std::move(*p4);
                    *p4 = std::move(*p3);
                    *p3 = std::move(*p2);
                    if (tmp < *p1) {
                        *p2 = std::move(*p1);
                        if (tmp < *p0) {
                            *p1 = std::move(*p0);
                            *p0 = std::move(tmp);
                        }
                        else {
                            *p1 = std::move(tmp);
                        }
                    }
                    else {
                        *p2 = std::move(tmp);
                    }
                }
                else {
                    std::iter_swap(p3, p4);
                }
            }
        }
        static std::string name() { return "fancy_move_insertion<" + std::to_string(Size) + ">"; }
        template <typename I>
        static inline bool done(I begin, I end) {
            using std::move;
            switch (std::distance(begin, end)) {
            case 0:
            case 1:
                return true;
            case 2:
                if (*--end < *begin)  {
                    std::iter_swap(begin, end);
                }
                return true;
            case 3:
                sort3(begin, begin + 1, begin + 2);
                return true;
            case 4:
                sort4(begin, begin + 1, begin + 2, begin + 3);
                return true;
            case 5:
                sort5(begin, begin + 1, begin + 2, begin + 3, begin + 4);
                return true;
            default:
                if (std::distance(begin, end) < Size) {
                    if (begin != end) {
                        for (I it = begin; ++it != end; ) {
                            if (*it < *(it - 1)) {
                                I cur = it;
                                auto tmp = *cur;
                                *cur = std::move(*(cur - 1));
                                while (begin != --cur && tmp < *(cur - 1)) {
                                    *cur = std::move(*(cur - 1));
                                }
                                *cur = tmp;
                            }
                        }
                    }
                    return true;
                }
            }
            return false;
        }
        template <typename I>
        static inline void run(I, I) {}
        template <typename I>
        void operator()(I begin, I end) const {
            run(begin, end);
        }
    };

    template <std::ptrdiff_t Size>
    struct sentinel
    {
        static std::string name() { return "sentinel<" + std::to_string(Size) + ">"; }
        template <typename I>
        static inline bool done(I begin, I end) { return std::distance(begin, end) < Size; }
        template <typename I>
        static inline void run(I begin, I end) {
            if (begin == end) {
                return;
            }
            std::ptrdiff_t presize = std::min<std::ptrdiff_t>(std::distance(begin, end), 2 * Size);
            //std::ptrdiff_t presize = std::distance(begin, end);
            I preend = begin + presize;
            for (I it = begin; ++it != preend; ) {
                for (I cur = it; begin != cur && *cur < *(cur - 1); --cur) {
                    std::iter_swap(cur, cur - 1);
                }
            }
            for (I it = preend; it != end; ++it) {
                for (I cur = it; *cur < *(cur - 1); --cur) {
                    std::iter_swap(cur, cur - 1);
                }
            }
        }
        template <typename I>
        void operator()(I begin, I end) const {
            run(begin, end);
        }
    };

    template <std::ptrdiff_t Size>
    struct move_sentinel
    {
        static std::string name() { return "move-sentinel<" + std::to_string(Size) + ">"; }
        template <typename I>
        static inline bool done(I begin, I end) { return std::distance(begin, end) < Size; }
        template <typename I>
        static inline void run(I begin, I end) {
            using std::move;
            if (begin == end) {
                return;
            }
            std::ptrdiff_t presize = std::min<std::ptrdiff_t>(std::distance(begin, end), 2 * Size);
            I preend = begin + presize;
            for (I it = begin; ++it != preend; ) {
                if (*it < *(it - 1)) {
                    I cur = it;
                    auto tmp = *cur;
                    *cur = move(*(cur - 1));
                    while (begin != --cur && tmp < *(cur - 1)) {
                        *cur = move(*(cur - 1));
                    }
                    *cur = tmp;
                }
            }
            for (I it = preend; it != end; ++it) {
                if (*it < *(it - 1)) {
                    I cur = it;
                    auto tmp = *cur;
                    *cur = move(*(cur - 1));
                    while (tmp < *(--cur - 1)) {
                        *cur = move(*(cur - 1));
                    }
                    *cur = tmp;
                }
            }
        }
        template <typename I>
        void operator()(I begin, I end) const {
            run(begin, end);
        }
    };
}

// ----------------------------------------------------------------------------

namespace fat_partition
{
    struct none
    {
        static std::string name() { return "none"; }
        template <typename I>
        static inline constexpr bool run(I, I, I) { return false; }
        template <typename I>
        constexpr bool operator()(I, I, I) { return false; }
    };

    struct fat
    {
        static std::string name() { return "fat"; }
        template <typename I>
        static inline bool run(I& mid, I begin, I pivot) {
            if (mid == begin) {
                int* mid = std::partition(begin, pivot,
                                          [=](int value){ return *pivot <= value; });
                std::iter_swap(mid, pivot);
                return true;
            }
            else {
                return false;
            }
        }
        template <typename I>
        bool operator()(I& mid, I begin, I pivot)  const {
            return run(mid, begin, pivot);
        }
    };
}

// ----------------------------------------------------------------------------

namespace partition
{
    struct std_partition
    {
        static std::string name() { return "std::partition"; }
        template <typename I, typename P>
        static inline I run(I begin, I end, P pred) {
            return std::partition(begin, end, pred);
        }
        template <typename I, typename P>
        I operator()(I begin, I end, P pred) const {
            return std::partition(begin, end, pred);
        }
    };

    struct simple{
        static std::string name() { return "simple"; }
        template <typename BiDirIt, typename Pred>
        static inline BiDirIt run(BiDirIt begin, BiDirIt end, Pred pred) {
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

    struct manual {
        static std::string name() { return "manual"; }
        template <typename BiDirIt, typename Pred>
        static inline BiDirIt run(BiDirIt begin, BiDirIt end, Pred pred) {
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
        BiDirIt operator()(BiDirIt begin, BiDirIt end, Pred pred) const {
            return run(begin, end, pred);
        }
    };

    struct sentinel {
        static std::string name() { return "sentinel"; }
        template <typename BiDirIt, typename Pred>
        static inline BiDirIt run(BiDirIt begin, BiDirIt end, Pred pred) {
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
        BiDirIt operator()(BiDirIt begin, BiDirIt end, Pred pred) const {
            return run(begin, end, pred);
        }
    };
}

// ----------------------------------------------------------------------------

namespace quicksort {
    template <typename Pivot,
              typename Fallback,
              typename Post = post::none,
              typename Partition = partition::std_partition,
              typename FatPartition = fat_partition::none>
    struct quick_sort {
        static std::string name() {
            return "quick_sort<p=" + Pivot::name() + ", "
                + "fb=" + Fallback::name() + ", "
                + "post=" + Post::name() + ", "
                + "p=" + Partition::name() + ", "
                + "fat=" + FatPartition::name() + ", "
                + ">";
        }
        template <typename I>
        static inline void run(I begin, I end) {
            return run(&*begin, &*begin + std::distance(begin, end));
        }
        static inline void run(int* begin, int* end) {
            run(begin, end, 0, Fallback::init(begin, end));
            Post::run(begin, end);
        }
        static inline void run(int* begin, int* end, int depth, int max) {
            if (!Post::done(begin, end)
                && !Fallback::run(begin, end, depth, max)) {
                int* pivot = end;
                std::iter_swap(Pivot::get(begin, end), --pivot);

                int* mid = Partition::run(begin, pivot,
                                          [=](int value){ return value < *pivot; });
                if (!FatPartition::run(mid, begin, pivot)) {
                    std::iter_swap(mid, pivot);
                    run(begin, mid, depth + 1, max);
                    run(mid + 1, end, depth + 1, max);
                }
                else {
                    run(mid, end, depth + 1, max);
                }
            }
        }
    };

    template <typename Pivot,
              typename Fallback,
              typename Post = post::none,
              typename Partition = partition::std_partition,
              typename FatPartition = fat_partition::none>
    struct quick_sort_tail {
        static std::string name() {
            return "quick_sort<p=" + Pivot::name() + ", "
                + "fb=" + Fallback::name() + ", "
                + "post=" + Post::name() + ", "
                + "p=" + Partition::name() + ", "
                + "fat=" + FatPartition::name() + ", "
                + "> (tail)";
        }
        template <typename I>
        static inline void run(I begin, I end) {
            return run(&*begin, &*begin + std::distance(begin, end));
        }
        static inline void run(int* begin, int* end) {
            run(begin, end, 0, Fallback::init(begin, end));
            Post::run(begin, end);
        }
        static inline void run(int* begin, int* end, int depth, int max) {
            while (!Post::done(begin, end)
                   && !Fallback::run(begin, end, depth, max)) {
                int* pivot = end;
                std::iter_swap(Pivot::get(begin, end), --pivot);

                int* mid = Partition::run(begin, pivot,
                                          [=](int value){ return value < *pivot; });
                if (!FatPartition::run(mid, begin, pivot)) {
                    std::iter_swap(mid, pivot);
                    if (std::distance(begin, mid) < std::distance(mid + 1, end)) {
                        run(begin, mid, depth + 1, max);
                        // run(mid + 1, end, depth + 1, max);
                        begin = mid + 1;
                        ++depth;
                    }
                    else {
                        run(mid + 1, end, depth + 1, max);
                        // run(begin, mid, depth + 1, max);
                        end = mid;
                        ++depth;
                    }
                }
                else {
                    // run(mid, end, depth + 1, max);
                    begin = mid;
                    ++depth;
                }
            }
        }
    };

    template <typename Pivot,
              typename Fallback,
              typename Post = post::none,
              typename Partition = partition::std_partition,
              typename FatPartition = fat_partition::none>
    struct quick_sort_tail_value {
        static std::string name() {
            return "quick_sort<p=" + Pivot::name() + ", "
                + "fb=" + Fallback::name() + ", "
                + "post=" + Post::name() + ", "
                + "p=" + Partition::name() + ", "
                + "fat=" + FatPartition::name() + ", "
                + "> (tail, value)";
        }
        template <typename I>
        static inline void run(I begin, I end) {
            return run(&*begin, &*begin + std::distance(begin, end));
        }
        static inline void run(int* begin, int* end) {
            run(begin, end, 0, Fallback::init(begin, end));
            Post::run(begin, end);
        }
        static inline void run(int* begin, int* end, int depth, int max) {
            while (!Post::done(begin, end)
                   && !Fallback::run(begin, end, depth, max)) {
                int* pivot = end;
                std::iter_swap(Pivot::get(begin, end), --pivot);

                int  p = *pivot;
                int* mid = Partition::run(begin, pivot,
                                          [=](int value){ return value < p; });
                if (!FatPartition::run(mid, begin, pivot)) {
                    std::iter_swap(mid, pivot);
                    if (std::distance(begin, mid) < std::distance(mid + 1, end)) {
                        run(begin, mid, depth + 1, max);
                        // run(mid + 1, end, depth + 1, max);
                        begin = mid + 1;
                        ++depth;
                    }
                    else {
                        run(mid + 1, end, depth + 1, max);
                        // run(begin, mid, depth + 1, max);
                        end = mid;
                        ++depth;
                    }
                }
                else {
                    // run(mid, end, depth + 1, max);
                    begin = mid;
                    ++depth;
                }
            }
        }
    };
}

// ----------------------------------------------------------------------------

#endif
