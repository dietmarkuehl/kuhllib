// nstd/file/mapped_memory.hpp                                        -*-C++-*-
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

#ifndef INCLUDED_NSTD_FILE_MAPPED_MEMORY
#define INCLUDED_NSTD_FILE_MAPPED_MEMORY

#include <cstddef>

// ----------------------------------------------------------------------------

namespace nstd::file {
    class mapped_memory;
}

// ----------------------------------------------------------------------------

class nstd::file::mapped_memory
{
private:
    void*         d_base{nullptr};
    ::std::size_t d_length{};

public:
    mapped_memory() = default;
    mapped_memory(mapped_memory &&);
    mapped_memory(mapped_memory const&) = delete;
    auto operator=(mapped_memory &&) -> mapped_memory&;
    auto operator=(mapped_memory const&) -> mapped_memory& = delete;
    ~mapped_memory();

    auto map(::std::size_t length, int fd, ::std::ptrdiff_t offset) -> bool;
    explicit operator bool() const;
    template <typename T>
    auto at_offset(::std::size_t off) const -> T*;
};

// ----------------------------------------------------------------------------

inline nstd::file::mapped_memory::operator bool() const
{
    return this->d_base != nullptr;
}

template <typename T>
inline auto nstd::file::mapped_memory::at_offset(std::size_t off) const -> T*
{
    void* tmp(static_cast<char*>(this->d_base) + off);
    return static_cast<T*>(tmp);
}

// ----------------------------------------------------------------------------

#endif
