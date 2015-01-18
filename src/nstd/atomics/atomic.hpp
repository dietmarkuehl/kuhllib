// nstd/atomics/atomic.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_NSTD_ATOMICS_ATOMIC
#define INCLUDED_NSTD_ATOMICS_ATOMIC

#include "nstd/atomics/memory_order.hpp"
#include "nstd/limits/numeric_limits.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    template <typename T, bool = ::nstd::numeric_limits<T>::is_integer>
    struct atomic_base;
    template <typename T> struct atomic_base<T, false>;
    template <typename T> struct atomic_base<T, true>;
    template <typename> struct atomic;
}

// ----------------------------------------------------------------------------
//-dk:TODO well, so far this is merely the interface, not a proper implementation!

template <typename T>
struct nstd::atomic_base<T, false> {
    T value;
    constexpr atomic_base(T value) noexcept(true): value(value) {}
    T load(::nstd::memory_order = ::nstd::memory_order_seq_cst) {
        return this->value;
    }
    T exchange(T value, ::nstd::memory_order = ::nstd::memory_order_seq_cst) {
        T rc(this->value);
        this->value = value;
        return rc;
    }
};

// ----------------------------------------------------------------------------

template <typename T>
struct nstd::atomic_base<T, true> {
    T value;
    constexpr atomic_base(T value) noexcept(true): value(value) {}
    T load(::nstd::memory_order = ::nstd::memory_order_seq_cst) {
        return this->value;
    }
    T exchange(T value, ::nstd::memory_order = ::nstd::memory_order_seq_cst) {
        T rc(this->value);
        this->value = value;
        return rc;
    }
    T operator++() noexcept(true) { return ++this->value; }
    T operator--() noexcept(true) { return --this->value; }
};

// ----------------------------------------------------------------------------

template <typename T>
struct nstd::atomic
    : ::nstd::atomic_base<T> {
    using atomic_base<T>::atomic_base;
};

// ----------------------------------------------------------------------------

#endif
