// buffer/mutable_buffer.cpp                                          -*-C++-*-
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

#include <buffer/mutable_buffer.hpp>
#include <algorithm>

namespace NET = cxxrt::net;

int buffer_mutable_buffer_dummy = 0;

// ----------------------------------------------------------------------------

NET::mutable_buffer::mutable_buffer(void* data, std::size_t size) noexcept
    : d_data(data)
    , d_size(size)
{
}

NET::mutable_buffer&
NET::mutable_buffer::operator+= (std::size_t n) noexcept
{
    auto adjust(std::min(this->d_size, n));
    this->d_data = adjust + static_cast<char*>(this->d_data);
    this->d_size -= adjust;
    return *this;
}

// ----------------------------------------------------------------------------

NET::mutable_buffer const*
NET::buffer_sequence_begin(NET::mutable_buffer const& b) noexcept
{
    return &b;
}

NET::mutable_buffer const*
NET::buffer_sequence_end(NET::mutable_buffer const& b) noexcept
{
    return &b + 1;
}

// ----------------------------------------------------------------------------

NET::mutable_buffer
NET::operator+ (NET::mutable_buffer const& b, std::size_t n) noexcept
{
    return NET::mutable_buffer(b) += n;
}

NET::mutable_buffer
NET::operator+ (std::size_t n, NET::mutable_buffer const& b) noexcept
{
    return NET::mutable_buffer(b) += n;
}

// ----------------------------------------------------------------------------

NET::mutable_buffer
NET::buffer(void* data, std::size_t size) noexcept
{
    return NET::mutable_buffer(data, size);
}

NET::mutable_buffer
NET::buffer(NET::mutable_buffer const& b) noexcept
{
    return b;
}

NET::mutable_buffer
NET::buffer(NET::mutable_buffer const& b, std::size_t size) noexcept
{
    return NET::mutable_buffer(b.data(), std::min(b.size(), size));
};

