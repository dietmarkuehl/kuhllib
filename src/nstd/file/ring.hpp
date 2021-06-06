// nstd/file/ring.hpp                                                 -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_FILE_RING
#define INCLUDED_NSTD_FILE_RING

#include "nstd/file/mapped_memory.hpp"
#include <atomic>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace nstd::file {
    template <typename T> class ring;
}

// ----------------------------------------------------------------------------

template <typename T>
class nstd::file::ring
{
private:
    // The code assumes that unsigned ints can be processed like atomics...
    static_assert(sizeof(::std::atomic<unsigned int>) == sizeof(unsigned int));

public:
    ring() = default;
    ring(::nstd::file::mapped_memory const& mem,
         ::std::size_t                      head_offset,
         ::std::size_t                      tail_offset,
         ::std::size_t                      mask_offset,
         ::std::size_t                      array_offset);
    ::std::atomic<unsigned int>* d_head{nullptr};
    ::std::atomic<unsigned int>* d_tail{nullptr};
    unsigned int                 d_mask{};
    T*                           d_array{nullptr};

    auto head() const -> unsigned int { return this->d_head->load(::std::memory_order_seq_cst); }
    auto tail() const -> unsigned int { return this->d_tail->load(::std::memory_order_seq_cst); }
    auto mask(unsigned int index) const -> unsigned int { return index & this->d_mask; }
    auto get(unsigned int index) const -> T { return this->d_array[this->mask(index)]; }
    auto advance_head() -> void { ++*this->d_head; }
    auto advance_tail() -> void { ++*this->d_tail; }
};

// ----------------------------------------------------------------------------

template <typename T>
nstd::file::ring<T>::ring(::nstd::file::mapped_memory const& mem,
                          ::std::size_t                       head_offset,
                          ::std::size_t                       tail_offset,
                          ::std::size_t                       mask_offset,
                          ::std::size_t                       array_offset)
    : d_head(mem.at_offset<::std::atomic<unsigned int>>(head_offset))
    , d_tail(mem.at_offset<::std::atomic<unsigned int>>(tail_offset))
    , d_mask(*mem.at_offset<unsigned int>(mask_offset))
    , d_array(mem.at_offset<T>(array_offset))
{
}

// ----------------------------------------------------------------------------

#endif
