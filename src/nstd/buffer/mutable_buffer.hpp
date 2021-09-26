// nstd/buffer/mutable_buffer.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_BUFFER_MUTABLE_BUFFER
#define INCLUDED_NSTD_BUFFER_MUTABLE_BUFFER

#include "nstd/algorithm/min.hpp"
#include <cstddef>
#include <sys/uio.h>

// ----------------------------------------------------------------------------

namespace nstd::net {
    class mutable_buffer;
}

// ----------------------------------------------------------------------------

class nstd::net::mutable_buffer
    : private ::iovec
{
public:
    constexpr mutable_buffer();
    constexpr mutable_buffer(void*, ::std::size_t);

    constexpr auto data() const noexcept -> void*;
    constexpr auto size() const noexcept -> ::std::size_t;
    constexpr auto operator+= (::std::size_t) noexcept -> mutable_buffer& ;
};

// ----------------------------------------------------------------------------

constexpr nstd::net::mutable_buffer::mutable_buffer()
    : ::iovec{ .iov_base = nullptr, .iov_len = ::std::size_t() }
{
}

constexpr nstd::net::mutable_buffer::mutable_buffer(void* d, ::std::size_t l)
    : ::iovec{ .iov_base = const_cast<void*>(d), .iov_len = l }
{
}

// ----------------------------------------------------------------------------

constexpr auto nstd::net::mutable_buffer::data() const noexcept -> void *
{
    return this->iov_base;
}

constexpr auto nstd::net::mutable_buffer::size() const noexcept -> ::std::size_t
{
    return this->iov_len;
}


constexpr auto nstd::net::mutable_buffer::operator+= (::std::size_t n) noexcept
    -> ::nstd::net::mutable_buffer&
{
    n = ::nstd::algorithm::min(n, this->size());
    this->iov_base = n + static_cast<char*>(this->iov_base);
    this->iov_len  -= n;
    return *this;
}

// ----------------------------------------------------------------------------

#endif
