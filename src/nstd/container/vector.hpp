// nstd/container/vector.hpp                                          -*-C++-*-
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

#ifndef INCLUDED_NSTD_CONTAINER_VECTOR
#define INCLUDED_NSTD_CONTAINER_VECTOR

// ----------------------------------------------------------------------------

#include "nstd/memory/allocator.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/cheaders/cstddef.hpp"
#include <new>

// ----------------------------------------------------------------------------

namespace nstd {
    template <typename T, typename = ::nstd::allocator<T> >
    class vector;
}

// ----------------------------------------------------------------------------

template <typename T, typename>
class nstd::vector {
    T* begin_;
    T* end_;
    T* capacity_;

public:
    using iterator = T*;

    vector();
    vector(vector const&);
    vector(vector&);
    ~vector();
    void operator= (vector const&) = delete;

    auto push_back(T const&) -> void;
    auto empty() const -> bool;
    auto size() const -> nstd::size_t;
    
    auto begin() -> T*;
    auto end() -> T*;
};

// ----------------------------------------------------------------------------

template <typename T, typename A>
nstd::vector<T, A>::vector()
    : begin_()
    , end_()
    , capacity_() {
}

template <typename T, typename A>
nstd::vector<T, A>::vector(vector const& other)
    : begin_(static_cast<T*>(operator new(sizeof(T) * other.size())))
    , end_(this->begin_)
    , capacity_(this->begin_ + other.size()) {
    for (nstd::size_t index(0u), size(other.size()); index != size; ++index) {
        new(this->begin_ + index) T(other.begin_[index]);
        ++this->end_;
    }
}

template <typename T, typename A>
nstd::vector<T, A>::~vector() {
    for (iterator it(this->end_), begin(this->begin_); it != begin; ) {
        --it;
        it->~T();
    }
}

// ----------------------------------------------------------------------------

template <typename T, typename A>
auto nstd::vector<T, A>::empty() const
    -> bool {
    return this->end_ == this->begin_;
}

template <typename T, typename A>
auto nstd::vector<T, A>::size() const
    -> nstd::size_t {
    return this->end_ - this->begin_;
}

// ----------------------------------------------------------------------------

template <typename T, typename A>
auto nstd::vector<T, A>::begin()
    -> T* {
    return this->begin_;
}
template <typename T, typename A>
auto nstd::vector<T, A>::end()
    -> T* {
    return this->end_;
}

// ----------------------------------------------------------------------------

template <typename T, typename A>
auto nstd::vector<T, A>::push_back(T const& value) -> void {
    if (this->end_ == this->capacity_) {
        nstd::size_t size(this->size());
        nstd::size_t newsize(size < 16u? 16u: size * 1.5);
        T* memory = static_cast<T*>(operator new(sizeof(T) * newsize)); //-dk:TODO use allocator
        if (!this->empty()) {
            for (nstd::size_t index(0); index != size; ++index) { //-dk:TODO use algorithm for copy/move
                new(memory + index) T(nstd::utility::move(this->begin_[index]));
            }
            for (nstd::size_t index(size); 0 < index--; ) {
                this->begin_[index].~T();
            }
            operator delete(this->begin_);
        }
        this->begin_ = memory;
        this->end_ = this->begin_ + size;
        this->capacity_ = this->begin_ + newsize;
    }
    new(this->end_++) T(value);
}

// ----------------------------------------------------------------------------

#endif
