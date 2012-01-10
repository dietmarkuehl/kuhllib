// -*-c++-*- property_map_find.cpp 
// -----------------------------------------------------------------------------
// Copyright 2012 Dietmar Kuehl http://www.dietmar-kuehl.de              
// Distributed under the Boost Software License, Version 1.0. (See file  
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).     
// -----------------------------------------------------------------------------

#include <iostream>
#include <map>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// NOTE: this file is just a demonstraction of a simple algorithm implemented
//   with property map support. It is NOT meant to be used for anything else
//   than demonstrating how this can be used to search different structures.
//   In particular, it omits a lot of the type tagging needed in the actual
//   implementation to create a complete and self-contained example.

namespace demo
{
    // declarations of a few simple property maps:
    struct identity_pm;
    struct first_pm;
    struct second_pm;
    struct deref_pm;
    namespace accessors
    {
        extern first_pm const _1;
        extern second_pm const _2;
    }

    // declarations of a simple algorithms
    template <typename Readable, typename SinglePass, typename Value>
    SinglePass find(Readable pm, SinglePass it, SinglePass end, Value value);
    template <typename SinglePass, typename Value>
    SinglePass find(SinglePass it, SinglePass end, Value value);
}

// -----------------------------------------------------------------------------
// property map definition and implementation; note that a real implementation
// would determine the correct return type.

struct demo::identity_pm
{
    template <typename T> 
    T operator()(T&& key) const { return std::forward<T>(key); }
};

struct demo::first_pm
{
    template <typename F, typename S>
    F const& operator()(std::pair<F, S> const& key) const { return key.first; }
};

struct demo::second_pm
{
    template <typename F, typename S>
    S const& operator()(std::pair<F, S> const& key) const { return key.second; }
};

struct demo::deref_pm
{
    template <typename T>
    T const& operator()(std::shared_ptr<T> const& key) const { return *key; }
};

// -----------------------------------------------------------------------------
// algorithm implementation

template <typename Readable, typename SinglePass, typename Value>
SinglePass demo::find(Readable pm, SinglePass it, SinglePass end, Value value)
{
    while (it != end && pm(*it) != value)
    {
        ++it;
    }
    return it;
}

template <typename SinglePass, typename Value>
SinglePass demo::find(SinglePass it, SinglePass end, Value value)
{
    return demo::find(demo::identity_pm(), it, end, value);
}

// -----------------------------------------------------------------------------

int main(int ac, char* av[])
{
    using namespace demo::accessors;

    int value(ac == 1? 7: atoi(av[1]));
    std::cout << "value=" << value << "\n";

    std::vector<int> const vi({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    auto vit(demo::find(vi.begin(), vi.end(), value)); // well-known
    std::cout << "vit  offset=" << (vit - vi.begin()) << "\n";

    std::vector<std::pair<int, int>> pi({
                    std::make_pair(1, 2), std::make_pair(3, 4),
                    std::make_pair(5, 6), std::make_pair(7, 8),
                    std::make_pair(9, 10), std::make_pair(11, 12)
                });
    auto pit1(demo::find(_1, pi.begin(), pi.end(), value));
    auto pit2(demo::find(_2, pi.begin(), pi.end(), value));
    std::cout << "pit1 offset=" << (pit1 - pi.begin()) << "\n";
    std::cout << "pit2 offset=" << (pit2 - pi.begin()) << "\n";

    std::vector<std::shared_ptr<int>> si({
                    std::make_shared<int>(1), std::make_shared<int>(2),
                    std::make_shared<int>(3), std::make_shared<int>(4),
                    std::make_shared<int>(5), std::make_shared<int>(6),
                    std::make_shared<int>(7), std::make_shared<int>(8),
                    std::make_shared<int>(9), std::make_shared<int>(10),
                });
    auto sit(demo::find(demo::deref_pm(), si.begin(), si.end(), value));
    std::cout << "sit  offset=" << (sit - si.begin()) << "\n";
}
