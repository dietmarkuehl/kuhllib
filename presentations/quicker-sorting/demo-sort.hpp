// demo-sort.hpp                                                      -*-C++-*-
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

#ifndef INCLUDED_DEMO_SORT
#define INCLUDED_DEMO_SORT

#include "small.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace demo {
    struct simple;
    struct simple_mid;
    struct simple_median;
    struct intro;
    template <int Size> struct intro_local_insertion;
    template <int Size> struct intro_global_insertion;
    struct tail_recursion;
    struct tail_recursion1;
    struct tail_recursion2;
    struct partition1;
    struct partition2;
    struct partition_sentinel1;
    struct partition_sentinel2;
    struct partition_sentinel3;
    struct small_ranges;
    struct monitor_swaps;
    struct libcxx_mod;
    struct libcxx;
}

// ----------------------------------------------------------------------------

struct demo::simple {
    static std::string name() { return "simple"; }
    static
    void sort(int* begin, int* end) {
        std::size_t len(std::distance(begin, end));
        if (len <= 1)
            return;
        int* pivot(end - 1);
        int* mid = std::partition(begin, pivot,
                                  [=](int arg){ return arg < *pivot; });
        std::swap(*mid, *pivot);
        sort(begin, mid);
        sort(mid + 1, end);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

struct demo::simple_mid {
    static std::string name() { return "simple/mid"; }
    static
    void sort(int* begin, int* end) {
        std::size_t len(std::distance(begin, end));
        if (len <= 1)
            return;
        int* pivot = end - 1;
        int* mid = begin + std::distance(begin, pivot) / 2;
        std::swap(*mid, *pivot);
        mid = std::partition(begin, pivot,
                             [=](int arg){ return arg < *pivot; });
        std::swap(*mid, *pivot);
        sort(begin, mid);
        sort(mid + 1, end);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

struct demo::simple_median {
    static std::string name() { return "simple/median"; }
    static
    void sort(int* begin, int* end) {
        std::size_t len(std::distance(begin, end));
        if (len <= 1)
            return;

        int* pivot = end - 1, * mid = begin + len / 2;
        small::sort(begin, pivot, mid);
        mid = std::partition(begin, pivot,
                             [=](int arg){ return arg < *pivot; });
        std::swap(*mid, *pivot);
        sort(begin, mid);
        sort(mid + 1, end);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

struct demo::intro {
    static std::string name() { return "intro"; }
    static
    void sort(int* begin, int* end, int depth, int max) {
        std::size_t len(std::distance(begin, end));
        if (len <= 1)
            return;

        if (++depth < max) {
            int* pivot = end - 1, * mid = begin + len / 2;
            std::swap(*mid, *pivot);
            mid = std::partition(begin, pivot,
                                 [=](int arg){ return arg < *pivot; });
            std::swap(*mid, *pivot);
            sort(begin, mid, depth, max);
            sort(mid + 1, end, depth, max);
        }
        else {
            std::stable_sort(begin, end);
        }
    }
    static
    void sort(int* begin, int* end) {
        std::size_t size(std::distance(begin, end)), max(0);
        while (size >>=1) { ++max; }
        sort(begin, end, 0, 2 * max);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

template <int Size>
struct demo::intro_local_insertion {
    static std::string name() { return "intro/local insertion<" + std::to_string(Size) + ">"; }
    static
    void sort(int* begin, int* end, int depth, int max) {
        std::size_t len(std::distance(begin, end));
        if (len <= Size) {
            if (begin == end) return;
            for (int* it = begin; ++it != end; )
                if (*it < *(it - 1)) {
                    int* c = it, t = *c;
                    *c = *(c - 1);
                    while (begin != --c && t < *(c - 1))
                        *c = *(c - 1);
                    *c = t;
                }
            return;
        }

        if (++depth < max) {
            int* pivot = end - 1, * mid = begin + len / 2;
            std::swap(*mid, *pivot);
            mid = std::partition(begin, pivot,
                                 [=](int arg){ return arg < *pivot; });
            std::swap(*mid, *pivot);
            sort(begin, mid, depth, max);
            sort(mid + 1, end, depth, max);
        }
        else {
            std::stable_sort(begin, end);
        }
    }
    static
    void sort(int* begin, int* end) {
        std::size_t size(std::distance(begin, end)), max(0);
        while (size >>=1) { ++max; }
        sort(begin, end, 0, 2 * max);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

template <int Size>
struct demo::intro_global_insertion {
    static std::string name() { return "intro/global insertion<" + std::to_string(Size) + ">"; }
    static
    void sort(int* begin, int* end, int depth, int max) {
        std::size_t len(std::distance(begin, end));
        if (len <= Size) return;
        

        if (++depth < max) {
            int* pivot = end - 1, * mid = begin + len / 2;
            std::swap(*mid, *pivot);
            mid = std::partition(begin, pivot,
                                 [=](int arg){ return arg < *pivot; });
            std::swap(*mid, *pivot);
            sort(begin, mid, depth, max);
            sort(mid + 1, end, depth, max);
        }
        else {
            std::stable_sort(begin, end);
        }
    }
    static
    void sort(int* begin, int* end) {
        std::size_t size(std::distance(begin, end)), max(0);
        while (size >>=1) { ++max; }
        sort(begin, end, 0, 2 * max);
    }
    static void run(int* begin, int* end) {
        if (begin == end) return;
        sort(begin, end);
        for (int* it = begin; ++it != end; )
            if (*it < *(it - 1)) {
                int* c = it, t = *c;
                *c = *(c - 1);
                while (begin != --c && t < *(c - 1))
                    *c = *(c - 1);
                *c = t;
            }
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

struct demo::tail_recursion {
    static std::string name() { return "intro/tail-recursion"; }
    static
    void sort(int* begin, int* end, int depth, int max) {
        std::size_t len;
        while (20 < (len = std::distance(begin, end))) {
            if (++depth < max) {
                int* pivot = end - 1, * mid = begin + len / 2;
                std::swap(*mid, *pivot);
                mid = std::partition(begin, pivot,
                                     [=](int arg){ return arg < *pivot; });
                std::swap(*mid, *pivot);
                sort(begin, mid, depth, max);
                begin = mid + 1;
            }
            else {
                std::stable_sort(begin, end);
                return;
            }
        }
        if (begin == end) return;
        for (int* it = begin; ++it != end; )
            if (*it < *(it - 1)) {
                int* c = it, t = *c;
                *c = *(c - 1);
                while (begin != --c && t < *(c - 1))
                    *c = *(c - 1);
                *c = t;
            }
        return;
    }
    static
    void sort(int* begin, int* end) {
        std::size_t size(std::distance(begin, end)), max(0);
        while (size >>=1) { ++max; }
        sort(begin, end, 0, 2 * max);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

struct demo::tail_recursion1 {
    static std::string name() { return "intro/tail-recursion1"; }
    static
    void sort(int* begin, int* end, int depth, int max) {
        std::size_t len;
        while (20 < (len = std::distance(begin, end))) {
            if (++depth < max) {
                int* pivot = end - 1, * mid = begin + len / 2;
                std::swap(*mid, *pivot);
                mid = std::partition(begin, pivot,
                                     [=](int arg){ return arg < *pivot; });
                std::swap(*mid, *pivot);
                if (mid - begin < end - mid) {
                    sort(begin, mid, depth, max);
                    begin = mid + 1;
                }
                else {
                    sort(mid + 1, end, depth, max);
                    end = mid;
                }
            }
            else {
                std::stable_sort(begin, end);
                return;
            }
        }
        if (begin == end) return;
        for (int* it = begin; ++it != end; )
            if (*it < *(it - 1)) {
                int* c = it, t = *c;
                *c = *(c - 1);
                while (begin != --c && t < *(c - 1))
                    *c = *(c - 1);
                *c = t;
            }
        return;
    }
    static
    void sort(int* begin, int* end) {
        std::size_t size(std::distance(begin, end)), max(0);
        while (size >>=1) { ++max; }
        sort(begin, end, 0, 2 * max);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

struct demo::tail_recursion2 {
    static std::string name() { return "intro/tail-recursion2"; }
    static
    void sort(int* begin, int* end, int depth, int max) {
        std::size_t len;
        while (20 < (len = std::distance(begin, end))) {
            if (++depth < max) {
                int* pivot = end - 1, * mid = begin + len / 2;
                std::swap(*mid, *pivot);
                mid = std::partition(begin, pivot,
                                     [=](int arg){ return arg < *pivot; });
                std::swap(*mid, *pivot);
                if (mid - begin < end - mid) {
                    sort(begin, mid, depth, max);
                    begin = mid + 1;
                }
                else {
                    sort(mid + 1, end, depth, max);
                    end = mid;
                }
            }
            else {
                std::stable_sort(begin, end);
                return;
            }
        }
        if (begin == end) return;
        for (int* it = begin; ++it != end; )
            if (*it < *(it - 1)) {
                int* c = it, t = *c;
                *c = *(c - 1);
                while (begin != --c && t < *(c - 1))
                    *c = *(c - 1);
                *c = t;
            }
        return;
    }
    static
    void sort(int* begin, int* end) {
        std::size_t size(std::distance(begin, end)), max(0);
        while (size >>=1) { ++max; }
        sort(begin, end, 0, 2 * max);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

struct demo::partition1 {
    static std::string name() { return "intro/partition1"; }
    static
    void sort(int* begin, int* end, int depth, int max) {
        std::size_t len;
        while (20 < (len = std::distance(begin, end))) {
            if (++depth < max) {
                int* pivot = end - 1, * mid = begin + len / 2;
                std::swap(*mid, *pivot);

                auto pred = [=](int arg){ return arg < *pivot; };
                int* pbegin = begin, * pend = end;
                while (true) {
                    while (pbegin != pend && pred(*pbegin)) ++pbegin;
                    while (pbegin != pend && !pred(*--pend)) ;
                    if (pbegin == pend) break;
                    std::iter_swap(pbegin, pend);
                    ++pbegin;
                }
                mid = pbegin;

                std::swap(*mid, *pivot);
                if (mid - begin < end - mid) {
                    sort(begin, mid, depth, max);
                    begin = mid + 1;
                }
                else {
                    sort(mid + 1, end, depth, max);
                    end = mid;
                }
            }
            else {
                std::stable_sort(begin, end);
                return;
            }
        }
        if (begin == end) return;
        for (int* it = begin; ++it != end; )
            if (*it < *(it - 1)) {
                int* c = it, t = *c;
                *c = *(c - 1);
                while (begin != --c && t < *(c - 1))
                    *c = *(c - 1);
                *c = t;
            }
        return;
    }
    static
    void sort(int* begin, int* end) {
        std::size_t size(std::distance(begin, end)), max(0);
        while (size >>=1) { ++max; }
        sort(begin, end, 0, 2 * max);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

struct demo::partition2 {
    static std::string name() { return "intro/partition2"; }
    static
    void sort(int* begin, int* end, int depth, int max) {
        std::size_t len;
        while (20 < (len = std::distance(begin, end))) {
            if (++depth < max) {
                int* pivot = end - 1, * mid = begin + len / 2;
                std::swap(*mid, *pivot);

                int* pbegin = begin, * pend = end;
                while (true) {
                    while (pbegin != pend && *pbegin < *pivot) ++pbegin;
                    while (pbegin != pend && !(*--pend < *pivot));
                    if (pbegin == pend) break;
                    std::iter_swap(pbegin, pend);
                    ++pbegin;
                }
                mid = pbegin;

                std::swap(*mid, *pivot);
                if (mid - begin < end - mid) {
                    sort(begin, mid, depth, max);
                    begin = mid + 1;
                }
                else {
                    sort(mid + 1, end, depth, max);
                    end = mid;
                }
            }
            else {
                std::stable_sort(begin, end);
                return;
            }
        }
        if (begin == end) return;
        for (int* it = begin; ++it != end; )
            if (*it < *(it - 1)) {
                int* c = it, t = *c;
                *c = *(c - 1);
                while (begin != --c && t < *(c - 1))
                    *c = *(c - 1);
                *c = t;
            }
        return;
    }
    static
    void sort(int* begin, int* end) {
        std::size_t size(std::distance(begin, end)), max(0);
        while (size >>=1) { ++max; }
        sort(begin, end, 0, 2 * max);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

struct demo::partition_sentinel1 {
    static std::string name() { return "intro/partition-sentinel1"; }
    static
    void sort(int* begin, int* end, int depth, int max) {
        std::size_t len;
        while (20 < (len = std::distance(begin, end))) {
            if (++depth < max) {
                int* pivot = end - 1, * mid = begin + len / 2;
                std::swap(*mid, *pivot);

                auto pred = [=](int arg){ return arg < *pivot; };
                int* pbegin = begin, * pend = end;
                if (pbegin != pend && !pred(*pbegin)) {
                    while (pbegin != pend && !pred(*--pend)) {
                    }
                    if (pbegin != pend) {
                        std::iter_swap(pbegin, pend);
                        ++pbegin;
                    }
                }
                if (pbegin != pend) {
                    while (true) {
                        while (pred(*pbegin)) {
                            ++pbegin;
                        }
                        while (!pred(*--pend)) {
                        }
                        if (pend <= pbegin) {
                            break;
                        }
                        std::iter_swap(pbegin, pend);
                        ++pbegin;
                    }
                }
                mid = pbegin;

                std::swap(*mid, *pivot);
                if (mid - begin < end - mid) {
                    sort(begin, mid, depth, max);
                    begin = mid + 1;
                }
                else {
                    sort(mid + 1, end, depth, max);
                    end = mid;
                }
            }
            else {
                std::stable_sort(begin, end);
                return;
            }
        }
        if (begin == end) return;
        for (int* it = begin; ++it != end; )
            if (*it < *(it - 1)) {
                int* c = it, t = *c;
                *c = *(c - 1);
                while (begin != --c && t < *(c - 1))
                    *c = *(c - 1);
                *c = t;
            }
        return;
    }
    static
    void sort(int* begin, int* end) {
        std::size_t size(std::distance(begin, end)), max(0);
        while (size >>=1) { ++max; }
        sort(begin, end, 0, 2 * max);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

struct demo::partition_sentinel2 {
    static std::string name() { return "intro/partition-sentinel2"; }
    static
    void sort(int* begin, int* end, int depth, int max) {
        std::size_t len;
        while (20 < (len = std::distance(begin, end))) {
            if (++depth < max) {
                int* pivot = end - 1, * mid = begin + len / 2;
                std::swap(*mid, *pivot);

                int p = *pivot;
                auto pred = [=](int arg){ return arg < p; };
                int* pbegin = begin, * pend = end;
                if (pbegin != pend && !pred(*pbegin)) {
                    while (pbegin != pend && !pred(*--pend)) {
                    }
                    if (pbegin != pend) {
                        std::iter_swap(pbegin, pend);
                        ++pbegin;
                    }
                }
                if (pbegin != pend) {
                    while (true) {
                        while (pred(*pbegin)) {
                            ++pbegin;
                        }
                        while (!pred(*--pend)) {
                        }
                        if (pend <= pbegin) {
                            break;
                        }
                        std::iter_swap(pbegin, pend);
                        ++pbegin;
                    }
                }
                mid = pbegin;

                std::swap(*mid, *pivot);
                if (mid - begin < end - mid) {
                    sort(begin, mid, depth, max);
                    begin = mid + 1;
                }
                else {
                    sort(mid + 1, end, depth, max);
                    end = mid;
                }
            }
            else {
                std::stable_sort(begin, end);
                return;
            }
        }
        if (begin == end) return;
        for (int* it = begin; ++it != end; )
            if (*it < *(it - 1)) {
                int* c = it, t = *c;
                *c = *(c - 1);
                while (begin != --c && t < *(c - 1))
                    *c = *(c - 1);
                *c = t;
            }
        return;
    }
    static
    void sort(int* begin, int* end) {
        std::size_t size(std::distance(begin, end)), max(0);
        while (size >>=1) { ++max; }
        sort(begin, end, 0, 2 * max);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

struct demo::partition_sentinel3 {
    static std::string name() { return "intro/partition-sentinel3"; }
    static
    void sort(int* begin, int* end, int depth, int max) {
        std::size_t len;
        while (20 < (len = std::distance(begin, end))) {
            if (++depth < max) {
                int* pivot = end - 1, * mid = begin + len / 2;
                std::swap(*mid, *pivot);

                int p = *pivot;
                int* pbegin = begin, * pend = end;
                if (pbegin != pend && !(*pbegin < p)) {
                    while (pbegin != pend && !(*--pend < p)) {
                    }
                    if (pbegin != pend) {
                        std::iter_swap(pbegin, pend);
                        ++pbegin;
                    }
                }
                if (pbegin != pend) {
                    while (true) {
                        while (*pbegin < p) {
                            ++pbegin;
                        }
                        while (!(*--pend < p)) {
                        }
                        if (pend <= pbegin) {
                            break;
                        }
                        std::iter_swap(pbegin, pend);
                        ++pbegin;
                    }
                }
                mid = pbegin;

                std::swap(*mid, *pivot);
                if (mid - begin < end - mid) {
                    sort(begin, mid, depth, max);
                    begin = mid + 1;
                }
                else {
                    sort(mid + 1, end, depth, max);
                    end = mid;
                }
            }
            else {
                std::stable_sort(begin, end);
                return;
            }
        }
        if (begin == end) return;
        for (int* it = begin; ++it != end; )
            if (*it < *(it - 1)) {
                int* c = it, t = *c;
                *c = *(c - 1);
                while (begin != --c && t < *(c - 1))
                    *c = *(c - 1);
                *c = t;
            }
        return;
    }
    static
    void sort(int* begin, int* end) {
        std::size_t size(std::distance(begin, end)), max(0);
        while (size >>=1) { ++max; }
        sort(begin, end, 0, 2 * max);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

struct demo::small_ranges {
    static std::string name() { return "intro/small_ranges"; }
    static
    void sort(int* begin, int* end, int depth, int max) {
        std::size_t len;
        while (20 < (len = std::distance(begin, end))) {
            if (++depth < max) {
                int* pivot = end - 1, * mid = begin + len / 2;
                std::swap(*mid, *pivot);

                int p = *pivot;
                auto pred = [=](int arg){ return arg < p; };
                int* pbegin = begin, * pend = end;
                if (pbegin != pend && !pred(*pbegin)) {
                    while (pbegin != pend && !pred(*--pend)) {
                    }
                    if (pbegin != pend) {
                        std::iter_swap(pbegin, pend);
                        ++pbegin;
                    }
                }
                if (pbegin != pend) {
                    while (true) {
                        while (pred(*pbegin)) {
                            ++pbegin;
                        }
                        while (!pred(*--pend)) {
                        }
                        if (pend <= pbegin) {
                            break;
                        }
                        std::iter_swap(pbegin, pend);
                        ++pbegin;
                    }
                }
                mid = pbegin;

                std::swap(*mid, *pivot);
                if (mid - begin < end - mid) {
                    sort(begin, mid, depth, max);
                    begin = mid + 1;
                }
                else {
                    sort(mid + 1, end, depth, max);
                    end = mid;
                }
            }
            else {
                std::stable_sort(begin, end);
                return;
            }
        }
        switch (std::distance(begin, end)) {
        case 0:
        case 1:
            return;
        case 2:
            if (*--end < *begin) std::swap(*begin, *end);
            return;
        case 3:
            small::sort(begin, begin + 1, begin + 2);
            return;
        case 4:
            small::sort(begin, begin + 1, begin + 2, begin + 3);
            return;
        }
        for (int* it = begin; ++it != end; )
            if (*it < *(it - 1)) {
                int* c = it, t = *c;
                *c = *(c - 1);
                while (begin != --c && t < *(c - 1))
                    *c = *(c - 1);
                *c = t;
            }
        return;
    }
    static
    void sort(int* begin, int* end) {
        std::size_t size(std::distance(begin, end)), max(0);
        while (size >>=1) { ++max; }
        sort(begin, end, 0, 2 * max);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

struct demo::monitor_swaps {
    static std::string name() { return "intro/monitor-swaps"; }
    static bool incomplete_insertion(int* begin, int* end) {
        switch (std::distance(begin, end)) {
        case 0:
        case 1:
            return true;
        case 2:
            if (*--end < *begin) std::swap(*begin, *end);
            return true;
        case 3:
            small::sort(begin, begin + 1, begin + 2);
            return true;
        case 4:
            small::sort(begin, begin + 1, begin + 2, begin + 3);
            return true;
        case 5:
            small::sort(begin, begin + 1, begin + 2, begin + 3, begin + 4);
            return true;
        }
        constexpr int limit = 8;
        int  count(0);
        for (int* it = begin; ++it != end; )
            if (*it < *(it - 1)) {
                if (++count == limit) return false;
                int* c = it, t = *c;
                *c = *(c - 1);
                while (begin != --c && t < *(c - 1)) {
                    *c = *(c - 1);
                    if (++count == limit) {
                        *(c - 1) = t;
                        return false;
                    }
                }
                *c = t;
            }
        return true;
    }
    static
    void sort(int* begin, int* end, int depth, int max) {
        std::size_t len;
        while (20 < (len = std::distance(begin, end))) {
            if (++depth < max) {
                int* pivot = end - 1, * mid = begin + len / 2;
                std::swap(*mid, *pivot);

                unsigned swaps = 0;
                int p = *pivot;
                auto pred = [=](int arg){ return arg < p; };
                int* pbegin = begin, * pend = end;
                if (pbegin != pend && !pred(*pbegin)) {
                    while (pbegin != pend && !pred(*--pend)) {
                    }
                    if (pbegin != pend) {
                        std::iter_swap(pbegin, pend);
                        ++swaps;
                        ++pbegin;
                    }
                }
                if (pbegin != pend) {
                    while (true) {
                        while (pred(*pbegin)) {
                            ++pbegin;
                        }
                        while (!pred(*--pend)) {
                        }
                        if (pend <= pbegin) {
                            break;
                        }
                        std::iter_swap(pbegin, pend);
                        ++pbegin;
                        ++swaps;
                    }
                }
                mid = pbegin;
                std::swap(*mid, *pivot);

                if (swaps == 0
                    && incomplete_insertion(begin, mid)
                    && incomplete_insertion(mid + 1, end)) {
                    return;
                }
                if (mid - begin < end - mid) {
                    sort(begin, mid, depth, max);
                    begin = mid + 1;
                }
                else {
                    sort(mid + 1, end, depth, max);
                    end = mid;
                }
            }
            else {
                std::stable_sort(begin, end);
                return;
            }
        }
        switch (std::distance(begin, end)) {
        case 0:
        case 1:
            return;
        case 2:
            if (*--end < *begin) std::swap(*begin, *end);
            return;
        case 3:
            small::sort(begin, begin + 1, begin + 2);
            return;
        case 4:
            small::sort(begin, begin + 1, begin + 2, begin + 3);
            return;
#if 0
        case 5:
            small::sort(begin, begin + 1, begin + 2, begin + 3, begin + 4);
            return;
#endif
        }
        for (int* it = begin; ++it != end; )
            if (*it < *(it - 1)) {
                int* c = it, t = *c;
                *c = *(c - 1);
                while (begin != --c && t < *(c - 1))
                    *c = *(c - 1);
                *c = t;
            }
        return;
    }
    static
    void sort(int* begin, int* end) {
        std::size_t size(std::distance(begin, end)), max(0);
        while (size >>=1) { ++max; }
        sort(begin, end, 0, 2 * max);
    }
    static void run(int* begin, int* end) {
        sort(begin, end);
    }
    template <typename I>
    static void run(I begin, I end) {
        run(&*begin, &*begin + std::distance(begin, end));
    }
};

// ----------------------------------------------------------------------------

#endif
