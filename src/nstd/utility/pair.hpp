// nstd/utility/pair.hpp                                              -*-C++-*-
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

#ifndef INCLUDED_NSTD_UTILITY_PAIR
#define INCLUDED_NSTD_UTILITY_PAIR

#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/utility/swap.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace utility {
        template <typename T0, typename T1>
        struct pair;
    }

}

// ----------------------------------------------------------------------------

template <typename T0, typename T1>
struct nstd::utility::pair
{
    typedef T0 first_type;
    typedef T1 second_type;

    T0 first;
    T1 second;

    constexpr pair(): first() , second() {}
    pair(pair<T0, T1> const&) = default;
    pair(pair<T0, T1> &&) = default;

    constexpr pair(T0 const& a0, T1 const& a1) : first(a0) , second(a1) {}
    template <typename A0, typename A1>
    constexpr pair(A0&& a0, A1&& a1)
        : first(nstd::utility::forward<A0>(a0))
        , second(nstd::utility::forward<A1>(a1)) {
    }
    template <typename A0, typename A1>
    constexpr pair(nstd::utility::pair<A0, A1> const& other)
        : first(other.first)
        , second(other.second) {
    }
    template <typename A0, typename A1>
    constexpr pair(nstd::utility::pair<A0, A1>&& other)
        : first(nstd::utility::move(other.first))
        , second(nstd::utility::move(other.second)) {
    }
    //-dk:TODO piecewise construct

    auto operator= (pair<T0, T1> const& other) -> pair<T0, T1>& {
        this->first  = other.first;
        this->second = other.second;
        return *this;
    }
    template <typename A0, typename A1>
    auto operator= (pair<A0, A1> const& other) -> pair<T0, T1>& {
        this->first  = other.first;
        this->second = other.second;
        return *this;
    }
    auto operator= (pair<T0, T1>&& other) -> pair<T0, T1>& {
        this->first  = nstd::utility::move(other.first);
        this->second = nstd::utility::move(other.second);
        return *this;
    }
    template <typename A0, typename A1>
    auto operator= (pair<A0, A1>&& other) -> pair<T0, T1>& {
        this->first  = nstd::utility::move(other.first);
        this->second = nstd::utility::move(other.second);
        return *this;
    }

    auto swap(pair<T0, T1>& other) -> void {
        using nstd::utility::swap;
        swap(this->first,  other.first);
        swap(this->second, other.second);
    }
};

// ----------------------------------------------------------------------------

#endif
