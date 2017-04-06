// nstd/iterator/forward_iterator.hpp                                 -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_ITERATOR_FORWARD_ITERATOR
#define INCLUDED_NSTD_ITERATOR_FORWARD_ITERATOR

#include "nstd/iterator/forward_iterator_tag.hpp"
#include "nstd/iterator/iterator_traits.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace iterator {
        template <typename ForwardIterator> class forward_iterator;

        template <typename T, int Size>
        forward_iterator<T*> forward_begin(T (&)[Size]);
        template <typename T, int Size>
        forward_iterator<T*> forward_end(T (&)[Size]);
    }
}

// ----------------------------------------------------------------------------
// The purpose of `forward_iterator` is to constrain the interface of the
// iterator to match what is actually required for an interface. It is intended
// as tool for detecting accidental use of operations and for checking that
// algorithms work with iterators of different capabilities.

template <typename ForwardIterator>
class nstd::iterator::forward_iterator
{
private:
    ForwardIterator d_iterator;

public:
    using value_type        = typename ::nstd::iterator::iterator_traits<ForwardIterator>::value_type;
    using difference_type   = typename ::nstd::iterator::iterator_traits<ForwardIterator>::difference_type;
    using pointer           = typename ::nstd::iterator::iterator_traits<ForwardIterator>::pointer;
    using reference         = typename ::nstd::iterator::iterator_traits<ForwardIterator>::reference;
    using iterator_category = ::nstd::iterator::forward_iterator_tag;

    forward_iterator(): d_iterator() {}
    explicit forward_iterator(ForwardIterator iterator)
        : d_iterator(iterator) {
    }
    reference         operator*() const { return *this->d_iterator; }
    pointer           operator->() const { return &(*this->d_iterator); }
    forward_iterator& operator++() { ++this->d_iterator; return *this; }
    forward_iterator  operator++(int) { return forward_iterator(this->d_iterator++); }
    bool              operator== (forward_iterator const& other) const {
        return this->d_iterator == other.d_iterator;
    }
    bool              operator!= (forward_iterator const& other) const {
        return this->d_iterator != other.d_iterator;
    }
};

// ----------------------------------------------------------------------------

template <typename T, int Size>
nstd::iterator::forward_iterator<T*>
nstd::iterator::forward_begin(T (&array)[Size]) {
    return nstd::iterator::forward_iterator<T*>(array);
}

template <typename T, int Size>
nstd::iterator::forward_iterator<T*>
nstd::iterator::forward_end(T (&array)[Size]) {
    return nstd::iterator::forward_iterator<T*>(array + Size);
}

// ----------------------------------------------------------------------------

#endif
