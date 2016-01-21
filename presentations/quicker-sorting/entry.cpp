// entry.cpp                                                          -*-C++-*-
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

#include "demo-sort.hpp"
#include "quicksort.hpp"
#include "std-sort.hpp"
#include "int-quicksorts.hpp"
#include "partition.hpp"
#include "timer.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

#include <cstddef>
#include <cstdlib>

// ----------------------------------------------------------------------------

static std::vector<int> random(std::size_t size) {
    std::vector<int> rc;
    rc.reserve(size);
    std::generate_n(std::back_inserter(rc), size, std::rand);
    return rc;
}

static std::vector<int> sorted(std::size_t size) {
    std::vector<int> rc = random(size);
    std::sort(rc.begin(), rc.end());
    return rc;
}

static std::vector<int> reverse(std::size_t size) {
    std::vector<int> rc = sorted(size);
    std::reverse(rc.begin(), rc.end());
    return rc;
}

static std::vector<int> identical(std::size_t size) {
    return std::vector<int>(size, 42);
}

// ----------------------------------------------------------------------------

template <typename C, typename T>
void append(C& container, T const& test) {
    container.push_back(std::make_pair(test, test));
    std::sort(container.back().second.begin(), container.back().second.end());
}

// ----------------------------------------------------------------------------

template <typename Algo, typename Range>
static void measure(Algo algo, Range const& inputs)
{
    Range tests(inputs);
    utility::timer timer;
    for (auto&& test: tests) {
        algo.run(test.first.begin(), test.first.end());
    }
    auto duration = timer.stop();
    bool is_sorted(true);
    for (auto&& test: tests) {
        is_sorted = is_sorted && test.first == test.second;
    }
    std::cout << "name=" << std::setw(120) << algo.name() << ' '
              << "size=" << std::setw(9) << tests[0].first.size() << ' '
              << "duration=" << std::setw(6) << duration << ' '
              << "is-sorted=" << is_sorted << ' '
              << '\n' << std::flush;
}

// ----------------------------------------------------------------------------

int main()
{
    try {
        constexpr int patch = 20;
        (void)patch;
        std::cout << std::boolalpha;
        for (std::size_t scale(10u); scale < 1000000u; scale *= 10u) {
            for (std::size_t factor(1u); factor < 10u; factor *= 2u) {
                std::size_t size(scale * factor);
                std::vector<std::pair<std::vector<int>, std::vector<int> > > test;
                append(test, random(size));
                append(test, random(size));
                append(test, random(size));
                append(test, random(size));
                append(test, sorted(size));
                append(test, reverse(size));
                append(test, identical(size));

                // ---- BASE --------------------------------------------------

                measure(std_sort::sort(), test);
                measure(std_sort::heap_sort(), test);
                measure(std_sort::stable_sort(), test);

                // ---- DEMO --------------------------------------------------
#if 1
                if (size < 40000) measure(demo::simple(), test);
                if (size < 40000) measure(demo::simple_median(), test);
#endif
#if 1
                if (size < 40000) measure(demo::simple_mid(), test);
#endif
                measure(demo::intro(), test);
#if 0
                measure(demo::intro_global_insertion<12>(), test);
                measure(demo::intro_global_insertion<16>(), test);
                measure(demo::intro_global_insertion<20>(), test);
                measure(demo::intro_global_insertion<24>(), test);
                measure(demo::intro_global_insertion<28>(), test);
                measure(demo::intro_global_insertion<32>(), test);
#endif

#if 0
                measure(demo::intro_local_insertion<12>(), test);
                measure(demo::intro_local_insertion<16>(), test);
#endif
                measure(demo::intro_local_insertion<20>(), test);
#if 0
                measure(demo::intro_local_insertion<24>(), test);
                measure(demo::intro_local_insertion<28>(), test);
                measure(demo::intro_local_insertion<32>(), test);
#endif
#if 0
                measure(demo::tail_recursion(), test);
#endif
                measure(demo::tail_recursion1(), test);
#if 0
                measure(demo::tail_recursion2(), test);
#endif
#if 0
                measure(demo::partition1(), test);
#endif
                measure(demo::partition2(), test);
#if 0
                measure(demo::partition_sentinel1(), test);
#endif
                measure(demo::partition_sentinel2(), test);
#if 0
                measure(demo::partition_sentinel3(), test);
#endif
                measure(demo::small_ranges(), test);
                measure(demo::monitor_swaps(), test);
#if 0
                measure(demo::libcxx_mod(), test);
                measure(demo::libcxx(), test);
#endif
                // ---- EXPERIMENTAL ------------------------------------------
#if 0
                measure(quicksort::quick_sort<
                        pivots::begin, fallback::none>(),  test);
                measure(quicksort::quick_sort<
                        pivots::end, fallback::none>(),    test);
                measure(quicksort::quick_sort<
                        pivots::mid, fallback::none>(),    test);
                measure(quicksort::quick_sort<
                        pivots::median, fallback::none>(), test);
#endif

#if 0
                measure(quicksort::quick_sort<
                        pivots::begin,
                        fallback::intro<std_sort::heap_sort>,
                        post::none>(), test);
                measure(quicksort::quick_sort<
                        pivots::end,
                        fallback::intro<std_sort::heap_sort>,
                        post::none>(), test);
                measure(quicksort::quick_sort<
                        pivots::mid,
                        fallback::intro<std_sort::heap_sort>,
                        post::none>(), test);
                measure(quicksort::quick_sort<
                        pivots::median,
                        fallback::intro<std_sort::heap_sort>,
                        post::none>(), test);

                measure(quicksort::quick_sort<
                        pivots::mid,
                        fallback::intro<std_sort::heap_sort>,
                        post::insertion<patch>>(), test);
                
                measure(quicksort::quick_sort<
                        pivots::mid,
                        fallback::intro<std_sort::heap_sort>,
                        post::move_insertion<patch>>(), test);

                measure(quicksort::quick_sort<
                        pivots::mid,
                        fallback::intro<std_sort::stable_sort>,
                        post::move_insertion<patch>>(), test);

                measure(quicksort::quick_sort<
                        pivots::mid,
                        fallback::intro<std_sort::heap_sort>,
                        post::sentinel<patch>>(), test);

                measure(quicksort::quick_sort<
                        pivots::mid,
                        fallback::intro<std_sort::heap_sort>,
                        post::move_insertion<patch>,
                        partition::std_partition,
                        fat_partition::none>(), test);

                measure(quicksort::quick_sort<
                        pivots::mid,
                        fallback::intro<std_sort::heap_sort>,
                        post::move_insertion<patch>,
                        partition::simple,
                        fat_partition::none>(), test);

                measure(quicksort::quick_sort<
                        pivots::mid,
                        fallback::intro<std_sort::heap_sort>,
                        post::move_insertion<patch>,
                        partition::manual,
                        fat_partition::none>(), test);

                measure(quicksort::quick_sort<
                        pivots::mid,
                        fallback::intro<std_sort::heap_sort>,
                        post::move_insertion<patch>,
                        partition::sentinel,
                        fat_partition::fat>(), test);

                measure(quicksort::quick_sort<
                        pivots::mid,
                        fallback::intro<std_sort::stable_sort>,
                        post::move_insertion<patch>,
                        partition::sentinel,
                        fat_partition::fat>(), test);

                measure(quicksort::quick_sort<
                        pivots::mid,
                        fallback::intro<std_sort::stable_sort>,
                        post::move_sentinel<patch>,
                        partition::sentinel,
                        fat_partition::fat>(), test);

                measure(quicksort::quick_sort<
                        pivots::mid,
                        fallback::intro<std_sort::stable_sort>,
                        post::move_insertion<patch>,
                        partition::sentinel,
                        fat_partition::fat>(), test);

                measure(quicksort::quick_sort_tail<
                        pivots::mid,
                        fallback::intro<std_sort::heap_sort>,
                        post::move_insertion<patch>,
                        partition::simple,
                        fat_partition::none>(), test);

                measure(quicksort::quick_sort_tail<
                        pivots::mid,
                        fallback::intro<std_sort::heap_sort>,
                        post::move_insertion<patch>,
                        partition::manual,
                        fat_partition::none>(), test);

                measure(quicksort::quick_sort_tail<
                        pivots::mid,
                        fallback::intro<std_sort::heap_sort>,
                        post::move_insertion<patch>,
                        partition::sentinel,
                        fat_partition::fat>(), test);

                measure(quicksort::quick_sort_tail<
                        pivots::mid,
                        fallback::intro<std_sort::stable_sort>,
                        post::move_insertion<patch>,
                        partition::sentinel,
                        fat_partition::fat>(), test);

                measure(quicksort::quick_sort_tail<
                        pivots::mid,
                        fallback::intro<std_sort::stable_sort>,
                        post::move_sentinel<patch>,
                        partition::sentinel,
                        fat_partition::fat>(), test);

                measure(quicksort::quick_sort_tail<
                        pivots::mid,
                        fallback::intro<std_sort::stable_sort>,
                        post::move_insertion<patch>,
                        partition::sentinel,
                        fat_partition::fat>(), test);

                measure(quicksort::quick_sort_tail<
                        pivots::mid,
                        fallback::intro<std_sort::heap_sort>,
                        post::direct_move_insertion<patch>,
                        partition::simple,
                        fat_partition::none>(), test);

                measure(quicksort::quick_sort_tail<
                        pivots::mid,
                        fallback::intro<std_sort::stable_sort>,
                        post::direct_move_insertion<patch>,
                        partition::simple,
                        fat_partition::none>(), test);

                measure(quicksort::quick_sort_tail<
                        pivots::mid,
                        fallback::intro<std_sort::stable_sort>,
                        post::direct_move_insertion<patch>,
                        partition::sentinel,
                        fat_partition::none>(), test);

                measure(quicksort::quick_sort_tail_value<
                        pivots::mid,
                        fallback::intro<std_sort::stable_sort>,
                        post::direct_move_insertion<patch>,
                        partition::simple,
                        fat_partition::none>(), test);

                measure(quicksort::quick_sort_tail_value<
                        pivots::mid,
                        fallback::intro<std_sort::stable_sort>,
                        post::direct_move_insertion<patch>,
                        partition::sentinel,
                        fat_partition::none>(), test);

                measure(quicksort::quick_sort_tail_value<
                        pivots::mid,
                        fallback::intro<std_sort::stable_sort>,
                        post::fancy_move_insertion<patch>,
                        partition::sentinel,
                        fat_partition::none>(), test);
                measure(quicksort::quick_sort_tail_value<
                        pivots::mid,
                        fallback::intro<std_sort::stable_sort>,
                        post::fancy_move_insertion<patch>,
                        partition::sentinel,
                        fat_partition::none>(), test);
#endif

#if 0
                if (scale < 10000u)
                    measure(quicksort::simple(), test);
                if (scale < 10000u)
                    measure(quicksort::median(), test);
                if (scale < 10000u)
                    measure(quicksort::mid(), test);
                measure(quicksort::fat(), test);
                measure(quicksort::intro(), test);
#endif
#if 0
                measure(quicksort::simple_quicksort<1, quicksort::std_partition>(), test);
                measure(quicksort::simple_quicksort<1, quicksort::simple_partition>(), test);
                measure(quicksort::simple_quicksort<1, quicksort::manual_partition>(), test);
                measure(quicksort::simple_quicksort<1, quicksort::internal_sentinel_partition>(), test);
#endif
#if 0
                measure(quicksort::fat_quicksort<1, quicksort::internal_sentinel_partition>(), test);
                measure(quicksort::insertion_quicksort<patch, quicksort::fat_quicksort<patch, quicksort::internal_sentinel_partition> >(), test);
                measure(quicksort::insertion_sentinel_quicksort<patch, quicksort::fat_quicksort<patch, quicksort::internal_sentinel_partition> >(), test);
                measure(quicksort::insertion_rotate_quicksort<patch, quicksort::fat_quicksort<patch, quicksort::internal_sentinel_partition> >(), test);
#endif
                std::cout << "\n";
            }
        }
    }
    catch (std::exception const &ex) {
        std::cout << "ERROR: " << ex.what() << "\n";
    }
}
